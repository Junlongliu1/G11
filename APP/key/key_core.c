/*******************************************************************************************/
/**<!-------Copyright (C) 2021 Alpine Electronics, Inc. All rights reserved. --------------->
 * 
 * \file					Key_core.c
 * \date					2021/09/16
 * \author					Liyongnan
 * \model					EDA
 * \description				KEY Control Routine
 * \version					0.55
 *
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/09/16		liyongnan		New
 ********************************************************************************************/

/********************************************************************************************/
/*                      	Include File Section                           				    */
/********************************************************************************************/
#include "apn_basic_type.h"
#include "os_framework.h"
#include "temp_compile_switch.h"

#include "rpc_if.h"
#include "rpc_event.h"

#include "HAL_ADCKey.h"
#include "HAL_I2CKey.h"
#include "HAL_GPIOKey.h"
#include "key_core.h"
#include "key_if.h"
#include "rpc_data.h"


/********************************************************************************************/
/*							Global Variable Definition Section								*/
/********************************************************************************************/
Type_uByte nubKEY_State = KEY_STATUS_OFF;
HAL_HANDLE nvbKEY_AdcHalHandle = STD_NULL;
HAL_HANDLE nvbKEY_I2cHalHandle = STD_NULL;
HAL_HANDLE nvbKEY_GpioHalHandle = STD_NULL;
/********************************************************************************************/
/*							Static Variable Definition Section								*/
/********************************************************************************************/

/********************************************************************************************/
/*							ROM Table Section												*/
/********************************************************************************************/

/********************************************************************************************/
/*							Static Prototype Declaration Section							*/
/********************************************************************************************/

/********************************************************************************************/
/*							Function Definition Section										*/
/********************************************************************************************/
/********************************************************************************************/
/**	\function		adKey_table
 *	\date			2022/04/29
 *	\author			guanjiafu
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		none
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			none
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2022/04/29		guanjiafu		New
 ********************************************************************************************/
	static Key_I2CState_ts nstI2cStateToKey_Tbl[] = {
	/* Audio Key */
	/* Key data 				Key state		 */
	{ STK_KEY_TYPE_VOL_DOWN,    KEY_ACTION_OFF,      }, /*Vol_down*/
	{ STK_KEY_TYPE_VOL_UP,      KEY_ACTION_OFF,      }, /*Vol_up*/
	{ STK_KEY_TYPE_CH_DOWN,   	KEY_ACTION_OFF,      },	/*Ch_down*/
	{ STK_KEY_TYPE_CH_UP,   	KEY_ACTION_OFF,      },	/*Ch_up*/
	{ STK_KEY_TYPE_MODE,      	KEY_ACTION_OFF,      },	/*Mode_down*/
	};

/********************************************************************************************/
/**	\function		adKey_table
 *	\date			2022/04/29
 *	\author			guanjiafu
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		none
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			none
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2022/04/29		guanjiafu		New
 ********************************************************************************************/
	static Key_ADState_ts nstAdStateToKey_Tbl[] = {
	/* HFT Key */
	/* Key data 				Key state		 */
	{ STK_KEY_TYPE_ON_HOOK, 	KEY_ACTION_OFF,		 },	/* On Hook */
	{ STK_KEY_TYPE_OFF_HOOK,	KEY_ACTION_OFF, 	 },	/* Off Hook */
	{ STK_KEY_TYPE_TALK,		KEY_ACTION_OFF, 	 },	/* Talk */
	};
/********************************************************************************************/
/*							Function Definition Section										*/
/********************************************************************************************/
/********************************************************************************************/
/**	\function		TSK_KEY
 *	\date			2021/09/16
 *	\author			liyongnan
 *	\description	Main Task
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		none
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			none
 *					
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/09/16		liyongnan		New
 ********************************************************************************************/
void TSK_KEY(void)
{
	KEY_DBG("\n\nTSK_KEY%s:%d", __FUNCTION__, __LINE__);
	LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d>TSK_KEY", __FUNCTION__, __LINE__);

	OS_ErrCode_t aswErrCode;		/* OS Error Code			*/
	OS_StdQueue_ts *astStdQueue_p;	/* Queue Pointer				*/
	OS_StdQueue_ts astStdQueue_st;	/* Queue Pointer Init		*/
	OS_EventCode_t auhEventCode;	/* Event Code				*/
	Type_uByte aubKeyValue[KEY_VALUE_2];
	Type_sWord aswRetHal;
	Type_sByte asbRpcRet;

	
	aswErrCode = OS_ERR_OK;
	astStdQueue_p = STD_NULL;
	auhEventCode = KEY_VALUE_0;
	aswRetHal = HAL_ERR_NOT_INITIALIZED;
	asbRpcRet = RPC_NG;
	OSAL_MemSet(aubKeyValue, KEY_VALUE_0, sizeof(aubKeyValue));
	OSAL_MemSet(&astStdQueue_st, KEY_VALUE_0, sizeof(OS_StdQueue_ts));
	astStdQueue_p = &astStdQueue_st;

	wvdKey_VariableInit();

	/* regiest rpc  */
	(void)wsbRPCInit(ARPC_ID_R5_KEY, (wvdRPCCallback)wvdKey_RpcCallback);


	while (1U) {
		aswErrCode = OS_ReceiveQueue(MBX_ID_KEY, OS_TOUT_FOREVER, &astStdQueue_p);
		if (aswErrCode != OS_ERR_OK) {
			LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>OS_ReceiveQueue:%d", __FUNCTION__, __LINE__, aswErrCode);
		} else {
			auhEventCode = astStdQueue_p->uhEventCode;
			LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d>auhEventCode:%x", __FUNCTION__, __LINE__, auhEventCode);
			switch (auhEventCode) {
				case EVENT_CODE_LINETOOL_KEY_MODE_ON:
					nubKEY_State = KEY_STATUS_OFF;
					asbRpcRet = wsbRPCSendMsg(ARPC_ID_R5_KEY, ARPC_ID_LINETOOLAPP, EVENT_CODE_LINETOOL_KEY_MODE_ON_RES, STD_NULL, KEY_VALUE_0);
					if (asbRpcRet == RPC_OK) {
						LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d>send to linetool", __FUNCTION__, __LINE__);
					} else {
						LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>rpc send err:%d", __FUNCTION__, __LINE__, asbRpcRet);
					}
					break;

				case EVENT_CODE_LINETOOL_KEY_MODE_OFF:
					nubKEY_State = KEY_STATUS_ON_NORMAL;
					aswRetHal = (Type_sWord)HAL_Ioctl(nvbKEY_AdcHalHandle, HAL_ADCKEY_IOCTL_ADCKEY_RESET, STD_NULL, STD_NULL, STD_NULL, STD_NULL);
					if (aswRetHal != HAL_OK) {
						LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>HAL_Ioctl err:%d", __FUNCTION__, __LINE__, aswRetHal);
					}
					
					aswRetHal = (Type_sWord)HAL_Ioctl(nvbKEY_I2cHalHandle, HAL_I2CKEY_IOCTL_I2CKEY_RESET, STD_NULL, STD_NULL, STD_NULL, STD_NULL);
					if (aswRetHal != HAL_OK) {
						LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>HAL_Ioctl err:%d", __FUNCTION__, __LINE__, aswRetHal);
					}

					aswRetHal = (Type_sWord)HAL_Ioctl(nvbKEY_GpioHalHandle, HAL_GPIOKEY_IOCTL_GPIOKEY_RESET, STD_NULL, STD_NULL, STD_NULL, STD_NULL);
					if (aswRetHal != HAL_OK) {
						LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>HAL_Ioctl err:%d", __FUNCTION__, __LINE__, aswRetHal);
					}

					asbRpcRet = wsbRPCSendMsg(ARPC_ID_R5_KEY, ARPC_ID_LINETOOLAPP, EVENT_CODE_LINETOOL_KEY_MODE_OFF_RES, STD_NULL, KEY_VALUE_0);
					if (asbRpcRet == RPC_OK) {
						LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d>send to linetool", __FUNCTION__, __LINE__);
					} else {
						LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>rpc send err:%d", __FUNCTION__, __LINE__, asbRpcRet);
					}
					break;
				
				case EVENT_CODE_LINETOOL_KEY_MODE_CHECK:
					nubKEY_State = KEY_STATUS_ON_LINETOOL;
					aswRetHal = (Type_sWord)HAL_Ioctl(nvbKEY_AdcHalHandle, HAL_ADCKEY_IOCTL_ADCKEY_RESET, STD_NULL, STD_NULL, STD_NULL, STD_NULL);
					if (aswRetHal != HAL_OK) {
						LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>HAL_Ioctl err:%d", __FUNCTION__, __LINE__, aswRetHal);
					}
					
					aswRetHal = (Type_sWord)HAL_Ioctl(nvbKEY_I2cHalHandle, HAL_I2CKEY_IOCTL_I2CKEY_RESET, STD_NULL, STD_NULL, STD_NULL, STD_NULL);
					if (aswRetHal != HAL_OK) {
						LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>HAL_Ioctl err:%d", __FUNCTION__, __LINE__, aswRetHal);
					}

					aswRetHal = (Type_sWord)HAL_Ioctl(nvbKEY_GpioHalHandle, HAL_GPIOKEY_IOCTL_GPIOKEY_RESET, STD_NULL, STD_NULL, STD_NULL, STD_NULL);
					if (aswRetHal != HAL_OK) {
						LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>HAL_Ioctl err:%d", __FUNCTION__, __LINE__, aswRetHal);
					}
					asbRpcRet = wsbRPCSendMsg(ARPC_ID_R5_KEY, ARPC_ID_LINETOOLAPP, EVENT_CODE_LINETOOL_KEY_MODE_CHECK_RES, STD_NULL, KEY_VALUE_0);
					if (asbRpcRet == RPC_OK) {
						LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d>send to linetool", __FUNCTION__, __LINE__);
					} else {
						LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>rpc send err:%d", __FUNCTION__, __LINE__, asbRpcRet);
					}
					break;

				case EVENT_CODE_HARDKEY:
				case EVENT_CODE_STEERINGKEY:
					if (astStdQueue_p->vdQueueBuffer_p != STD_NULL) {					
						wvdGen_MemCpy(aubKeyValue, (Type_uByte*)astStdQueue_p->vdQueueBuffer_p, sizeof(aubKeyValue) / sizeof(Type_uByte));
						/* send to HMI or LINETOOL */
						if (nubKEY_State == KEY_STATUS_ON_NORMAL) {
							asbRpcRet = wsbRPCSendMsg(ARPC_ID_R5_KEY, ARPC_ID_HMI, auhEventCode, aubKeyValue, sizeof(aubKeyValue) / sizeof(Type_uByte));
  //                           KEY_DBG("\n\n\nTSK_KEY	 auhEventCode :%x		aubKeyValue:%x     aubKeyValue:%d", auhEventCode, aubKeyValue[0],aubKeyValue[1]); 
							if (asbRpcRet == RPC_OK) {
								LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d>send %x:%x[%d] to hmi", __FUNCTION__, __LINE__, auhEventCode, aubKeyValue[0], aubKeyValue[1]);
							} else {
								LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>rpc send err:%d", __FUNCTION__, __LINE__, asbRpcRet);
							}
						} else if (nubKEY_State == KEY_STATUS_ON_LINETOOL) {
							asbRpcRet = wsbRPCSendMsg(ARPC_ID_R5_KEY, ARPC_ID_LINETOOLAPP, auhEventCode, aubKeyValue, sizeof(aubKeyValue) / sizeof(Type_uByte));
							if (asbRpcRet == RPC_OK) {
								LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d>send %x:%x[%d] to linetool", __FUNCTION__, __LINE__, auhEventCode, aubKeyValue[0], aubKeyValue[1]);
							} else {
								LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>rpc send err:%d", __FUNCTION__, __LINE__, asbRpcRet);
							}

						} else {
							/* error */
							LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>nubKEY_State:%d", __FUNCTION__, __LINE__, nubKEY_State);
						}
					} else {
						/* error */
						LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>vdQueueBuffer_p NULL", __FUNCTION__, __LINE__);
					}
					break;
				default:
					break;
			}
			
			if (astStdQueue_p->vdQueueBuffer_p != STD_NULL) {
				aswErrCode = OS_ReleaseFMemoryBlock(astStdQueue_p->vdQueueBuffer_p);
				if (aswErrCode != OS_ERR_OK) {
					LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>OS_ReleaseFMemoryBlock:%d", __FUNCTION__, __LINE__, aswErrCode);
				} else {
					;
				}
			} else {
				;
			}
		}
	}
}

/********************************************************************************************/
/**	\function		TSK_KEY_ADC
 *	\date			2021/09/16
 *	\author			liyongnan
 *	\description	ADC KEY Detect Task
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		none
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			none
 *					
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/09/16		liyongnan		New
 ********************************************************************************************/
void TSK_KEY_ADC(void)
{
	//KEY_DBG("\n\nTSK_KEY_ADC%s:%d", __FUNCTION__, __LINE__);
	LOG_OUTPUT(LOG_APP_ID_ADCK, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d>TSK_KEY_ADC", __FUNCTION__, __LINE__);
	Type_uByte aubkeyData[KEY_VALUE_2];
	Type_sWord aswRetHal;
	Type_uByte aubKeyVal;
	Type_uByte aubKeyAction;
	Type_uByte aubKeyMode;
	Type_uByte aubKeyLoop;
	Type_sByte asbRpcRet;
//	Key_ADState_ts nstAdStateToKey_st;
	Type_uByte aubKeyid;
	Type_uByte aubKeystate;


	aswRetHal = HAL_ERR_NOT_INITIALIZED;
	aubKeyVal = KEY_VALUE_0;
	aubKeyAction = KEY_VALUE_0;
	aubKeyMode = KEY_STATUS_OFF;
	aubKeyLoop = KEY_ADCKEY_LOOP_CNT;
	asbRpcRet = RPC_NG;
	OSAL_MemSet(aubkeyData, KEY_VALUE_0, sizeof(aubkeyData));

	while (1U) {
		if (nubKEY_State == KEY_STATUS_OFF) {
			;
		} else {
			if(nvbKEY_AdcHalHandle != STD_NULL) {
				aubKeyMode = nubKEY_State;
				aswRetHal = (Type_sWord)HAL_Ioctl(nvbKEY_AdcHalHandle, HAL_ADCKEY_IOCTL_ADCKEY_READ, &aubKeyMode, &aubKeyLoop, &aubKeyVal, &aubKeyAction);
				if((aswRetHal == HAL_OK) && (aubKeyVal != KEY_VALUE_0) && (aubKeyAction != KEY_VALUE_0)) {
					if(aubKeyAction==KEY_ACTION_ON)
                    	{
                          for(aubKeyid=0; aubKeyid<(sizeof(nstAdStateToKey_Tbl) /sizeof(Key_ADState_ts));aubKeyid++)
                          {
							  if(nstAdStateToKey_Tbl[aubKeyid].ubKey==aubKeyVal)
							  {
								nstAdStateToKey_Tbl[aubKeyid].ubKeyState=KEY_ACTION_ON;
								aubkeyData[0] =nstAdStateToKey_Tbl[aubKeyid].ubKey;
								aubkeyData[1] =nstAdStateToKey_Tbl[aubKeyid].ubKeyState;
								KEY_DBG("\n\nTSK_KEY_ADC_action on	 aubKeyVal :%x			aubKeyAction:%x", aubkeyData[0], aubkeyData[1]);							
								asbRpcRet =wsbRPCSendMsg(ARPC_ID_R5_KEY, ARPC_ID_R5_KEY, EVENT_CODE_STEERINGKEY, aubkeyData, sizeof(aubkeyData) / sizeof(Type_uByte));
                                if (asbRpcRet == RPC_OK) {
									LOG_OUTPUT(LOG_APP_ID_ADCK, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "<%s>send to KEY:key=%x sts=%x", __FUNCTION__, aubkeyData[0], aubkeyData[1]);
								} 
								else 
								{
									LOG_OUTPUT(LOG_APP_ID_ADCK, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "<%s>rpc send err:key=%x sts=%x", __FUNCTION__, aubkeyData[0], aubkeyData[1], asbRpcRet);
								}
								break;
							  }
							  else
								{;}
                          	}
                    	}
					else if(aubKeyAction ==KEY_ACTION_OFF)
					{									
						for (aubKeystate = 0; aubKeystate < (sizeof(nstAdStateToKey_Tbl) / sizeof(Key_ADState_ts)); aubKeystate++)							
							{
								if(nstAdStateToKey_Tbl[aubKeystate].ubKeyState == KEY_ACTION_ON)
								{	
									nstAdStateToKey_Tbl[aubKeystate].ubKeyState=KEY_ACTION_OFF;
									aubkeyData[0] = nstAdStateToKey_Tbl[aubKeystate].ubKey;
									aubkeyData[1] = nstAdStateToKey_Tbl[aubKeystate].ubKeyState;
									KEY_DBG("\n\nTSK_KEY_ADC_action off	 aubKeyVal :%x			aubKeyAction:%x", aubkeyData[0], aubkeyData[1]); 							
									asbRpcRet =wsbRPCSendMsg(ARPC_ID_R5_KEY, ARPC_ID_R5_KEY, EVENT_CODE_STEERINGKEY, aubkeyData, sizeof(aubkeyData) / sizeof(Type_uByte));
									if (asbRpcRet == RPC_OK) {
										LOG_OUTPUT(LOG_APP_ID_ADCK, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "<%s>send to KEY:key=%x sts=%x", __FUNCTION__, aubkeyData[0], aubkeyData[1]);
									} 
									else 
									{
										LOG_OUTPUT(LOG_APP_ID_ADCK, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "<%s>rpc send err:key=%x sts=%x", __FUNCTION__, aubkeyData[0], aubkeyData[1], asbRpcRet);
									}

								}
								
								else
								{
									;
								}
							}

					}
					else
				    {
						;
					}
				} else {                
				     ;
                                }
			} else {
				LOG_OUTPUT(LOG_APP_ID_ADCK, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>AdcHalHandle NULL", __FUNCTION__, __LINE__);
			}
		}
 		OS_SleepTask(KEY_ADCKEY_WAIT_TIME);
	}
}
/********************************************************************************************/
/**	\function		TSK_KEY_I2C
 *	\date			2021/09/16
 *	\author			liyongnan
 *	\description	I2C KEY Detect Task
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		none
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			none
 *					
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/09/16		liyongnan		New
 ********************************************************************************************/
void TSK_KEY_I2C(void)
{
	//KEY_DBG("\n\nTSK_KEY_I2C%s:%d", __FUNCTION__, __LINE__);
	LOG_OUTPUT(LOG_APP_ID_I2CK, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d>TSK_KEY_I2C", __FUNCTION__, __LINE__);
	Type_uByte aubkeyData[KEY_VALUE_2];
	Type_sWord aswRetHal;
	Type_uByte aubKeyVal;
	Type_uByte aubKeyAction;
	Type_uByte aubKeyMode;
	Type_uByte aubKeyLoop;
	Type_sByte asbRpcRet;
	Type_uByte aubi2cKeyid;
	Type_uByte aubi2cKeystate;

	aswRetHal = HAL_ERR_NOT_INITIALIZED;
	aubKeyVal = KEY_VALUE_0;
	aubKeyAction = KEY_VALUE_0;
	aubKeyMode = KEY_STATUS_OFF;
	aubKeyLoop = KEY_I2CKEY_LOOP_CNT;
	asbRpcRet = RPC_NG;
	OSAL_MemSet(aubkeyData, KEY_VALUE_0, sizeof(aubkeyData));

	while (1U) {
		if (nubKEY_State == KEY_STATUS_OFF) {
			;
		} else {
			if(nvbKEY_I2cHalHandle != STD_NULL) {
				aubKeyMode = nubKEY_State;
				aswRetHal = (Type_sWord)HAL_Ioctl(nvbKEY_I2cHalHandle, HAL_I2CKEY_IOCTL_I2CKEY_READ, &aubKeyMode, &aubKeyLoop, &aubKeyVal, &aubKeyAction);
				if((aswRetHal == HAL_OK) && (aubKeyVal != KEY_VALUE_0) && (aubKeyAction != KEY_VALUE_0)) {
							if(aubKeyAction==KEY_ACTION_ON)
                    	{
                          for(aubi2cKeyid=0; aubi2cKeyid<(sizeof(nstI2cStateToKey_Tbl) /sizeof(Key_I2CState_ts));aubi2cKeyid++)
                          {
							  if(nstI2cStateToKey_Tbl[aubi2cKeyid].ubKey==aubKeyVal)
							  {
								nstI2cStateToKey_Tbl[aubi2cKeyid].ubKeyState=KEY_ACTION_ON;
								aubkeyData[0] =nstI2cStateToKey_Tbl[aubi2cKeyid].ubKey;
								aubkeyData[1] =nstI2cStateToKey_Tbl[aubi2cKeyid].ubKeyState;
								KEY_DBG("\n\nTSK_KEY_I2C_action on	 aubKeyVal :%x			aubKeyAction:%x", aubkeyData[0], aubkeyData[1]);							
								asbRpcRet =wsbRPCSendMsg(ARPC_ID_R5_KEY, ARPC_ID_R5_KEY, EVENT_CODE_STEERINGKEY, aubkeyData, sizeof(aubkeyData) / sizeof(Type_uByte));
                                if (asbRpcRet == RPC_OK) {
									LOG_OUTPUT(LOG_APP_ID_I2CK, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "<%s>send to KEY:key=%x sts=%x", __FUNCTION__, aubkeyData[0], aubkeyData[1]);
								} 
								else 
								{
									LOG_OUTPUT(LOG_APP_ID_I2CK, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "<%s>rpc send err:key=%x sts=%x", __FUNCTION__, aubkeyData[0], aubkeyData[1], asbRpcRet);
								}
								break;
							  }
							  else
								{;}
                          	}
                    	}
					else if(aubKeyAction ==KEY_ACTION_OFF)
					{									
						for (aubi2cKeystate = 0; aubi2cKeystate < (sizeof(nstI2cStateToKey_Tbl) / sizeof(Key_I2CState_ts)); aubi2cKeystate++)							
							{
								if(nstI2cStateToKey_Tbl[aubi2cKeystate].ubKeyState == KEY_ACTION_ON)
								{	
									nstI2cStateToKey_Tbl[aubi2cKeystate].ubKeyState=KEY_ACTION_OFF;
									aubkeyData[0] = nstI2cStateToKey_Tbl[aubi2cKeystate].ubKey;
									aubkeyData[1] = nstI2cStateToKey_Tbl[aubi2cKeystate].ubKeyState;
									KEY_DBG("\n\nTSK_KEY_I2C_action off	 aubKeyVal :%x			aubKeyAction:%x", aubkeyData[0], aubkeyData[1]); 							
									asbRpcRet =wsbRPCSendMsg(ARPC_ID_R5_KEY, ARPC_ID_R5_KEY, EVENT_CODE_STEERINGKEY, aubkeyData, sizeof(aubkeyData) / sizeof(Type_uByte));
									if (asbRpcRet == RPC_OK) {
										LOG_OUTPUT(LOG_APP_ID_I2CK, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "<%s>send to KEY:key=%x sts=%x", __FUNCTION__, aubkeyData[0], aubkeyData[1]);
									} 
									else 
									{
										LOG_OUTPUT(LOG_APP_ID_I2CK, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "<%s>rpc send err:key=%x sts=%x", __FUNCTION__, aubkeyData[0], aubkeyData[1], asbRpcRet);
									}

								}
								
								else
								{
									;
								}
							}

					}
					else
				    {
						;
					}

			} 
					}else {
				LOG_OUTPUT(LOG_APP_ID_I2CK, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>I2cHalHandle NULL", __FUNCTION__, __LINE__);
			}
		}
		OS_SleepTask(KEY_I2CKEY_WAIT_TIME);
	}
}

/********************************************************************************************/
/**	\function		TSK_KEY_GPIO
 *	\date			2021/09/16
 *	\author			liyongnan
 *	\description	GPIO KEY Detect Task
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		none
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			none
 *					
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/09/16		liyongnan		New
 ********************************************************************************************/
void TSK_KEY_GPIO(void)
{
// 0.5wk,GPIO Key work at A53
#if (TEMP_COMPILE_SWITCH_05WORK_10WORK == TEMP_COMPILE_SWITCH_10WORK)
	//KEY_DBG("\n\nTSK_KEY_GPIO%s:%d", __FUNCTION__, __LINE__);
	LOG_OUTPUT(LOG_APP_ID_GPK, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d>TSK_KEY_GPIO", __FUNCTION__, __LINE__);
	Type_uByte aubkeyData[KEY_VALUE_2];
	Type_sWord aswRetHal;
	Type_uByte aubKeyVal[KEY_GPIOKEY_CNT];
	Type_uByte aubKeyAction[KEY_GPIOKEY_CNT];
	Type_uByte aubKeyMode;
	Type_uByte aubKeyLoop;
	Type_uByte i;
	Type_sByte asbRpcRet;

	aswRetHal = HAL_ERR_NOT_INITIALIZED;
	aubKeyMode = KEY_STATUS_OFF;
	aubKeyLoop = KEY_GPIOKEY_LOOP_CNT;
	asbRpcRet = RPC_NG;

	OSAL_MemSet(aubkeyData, KEY_VALUE_0, sizeof(aubkeyData));
	OSAL_MemSet(aubKeyVal, KEY_VALUE_0, sizeof(aubKeyVal));
	OSAL_MemSet(aubKeyAction, KEY_VALUE_0, sizeof(aubKeyAction));

	while (1U) {
		if (nubKEY_State == KEY_STATUS_OFF) {
			;
		} else {
			if(nvbKEY_GpioHalHandle != STD_NULL) {
				aubKeyMode = nubKEY_State;
				aswRetHal = (Type_sWord)HAL_Ioctl(nvbKEY_GpioHalHandle, HAL_GPIOKEY_IOCTL_GPIOKEY_READ, &aubKeyMode, &aubKeyLoop, &aubKeyVal, &aubKeyAction);
				if(aswRetHal == HAL_OK) {
					for (i = 0; i < HAL_GPIOKEY_VALUE_6; i++) {
						if ((aubKeyVal[i] != KEY_VALUE_0) && (aubKeyAction[i] != KEY_VALUE_0)) {
							KEY_DBG("\n\nTSK_KEY_GPIO	aubKeyVal :%x		   aubKeyAction:%x", aubKeyVal[i], aubKeyAction[i]);
							LOG_OUTPUT(LOG_APP_ID_GPK, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d>Val:%x  Action:%x", __FUNCTION__, __LINE__, aubKeyVal[i], aubKeyAction[i]);
							aubkeyData[0] = aubKeyVal[i];
							aubkeyData[1] = aubKeyAction[i];
							asbRpcRet = wsbRPCSendMsg(ARPC_ID_R5_KEY, ARPC_ID_R5_KEY, EVENT_CODE_HARDKEY, aubkeyData, sizeof(aubkeyData) / sizeof(Type_uByte));
							if (asbRpcRet == RPC_OK) {
								LOG_OUTPUT(LOG_APP_ID_GPK, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d>send to KEY ", __FUNCTION__, __LINE__);
							} else {
								LOG_OUTPUT(LOG_APP_ID_GPK, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>rpc send err:%d", __FUNCTION__, __LINE__, asbRpcRet);
							}					
						}
					}
				} else {
					;
				}
			} else {
				LOG_OUTPUT(LOG_APP_ID_GPK, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>GpioHalHandle NULL", __FUNCTION__, __LINE__);
			}
		}
		OSAL_TaskDelay(KEY_GPIOKEY_WAIT_TIME);
	}
#else
	while (1U) {
		OSAL_TaskDelay(KEY_GPIOKEY_WAIT_TIME);
	}
#endif
}
/********************************************************************************************/
/**	\function		wvdKey_VariableInit
 *	\date			2021/09/16
 *	\author			liyongnan
 *	\description	Init Global Variable 
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		none
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			none
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/09/16		liyongnan		New
 ********************************************************************************************/
void wvdKey_VariableInit(void) 
{
	nubKEY_State = KEY_STATUS_OFF;
	nvbKEY_AdcHalHandle = STD_NULL;
	nvbKEY_I2cHalHandle = STD_NULL;
	nvbKEY_GpioHalHandle = STD_NULL;
}

/* end of file */


/* end of file */
