/*******************************************************************************************/
/**<!-------Copyright (C) 2021 Alpine Electronics, Inc. All rights reserved. --------------->
 * 
 * \file					Led_core.c
 * \date					2021/09/16
 * \author					Liyongnan
 * \model					EDA
 * \description				LED Control Routine
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

#include "rpc_if.h"
#include "rpc_event.h"
#include "Eepromcontrol.h"

#include "HAL_Led.h"
#include "led_if.h"
#include "led_core.h"

/********************************************************************************************/
/*							Global Variable Definition Section								*/
/********************************************************************************************/
Type_uByte nubLED_State		= LED_STATUS_ACC_OFF;
Type_uByte nubLED_KeyLight	= LED_KEY_LIGHT_VALUE_POSITIVE_0;
Type_uByte nubLED_ILL		= LED_STATUS_OFF;
HAL_HANDLE nvbLED_HalHandle = STD_NULL;

/********************************************************************************************/
/*							Static Variable Definition Section								*/
/********************************************************************************************/

/********************************************************************************************/
/*							ROM Table Section												*/
/********************************************************************************************/
/* KEY LED Duty -5~5 */
const Type_uHWord nstLED_KeyDutySave_T[] = 
{	
#if (TEMP_COMPILE_SWITCH_05WORK_10WORK == TEMP_COMPILE_SWITCH_05WORK)
	0x001E,	0x003C,	0x0069,	0x008C,	0x00BE,
	0x00FA,	0x0154,	0x01C2,	0x0258,	0x032A,
	0x03FC
#else /* 1wk */
#if (COMPILE_SWITCH_DISP_SIZ == COMPILE_SWITCH_8INCH)
	0x0041, 0x006C, 0x00A0, 0x00E6, 0x0140,
	0x01B8, 0x026C, 0x0339, 0x046A, 0x05F0,
	0x076C
#else /* 10inch */
	0x0003, 0x0006, 0x000B, 0x000F, 0x0014,
	0x001B, 0x0025, 0x0031, 0x0041, 0x0057,
	0x006E
#endif
#endif
};

/********************************************************************************************/
/*							Static Prototype Declaration Section							*/
/********************************************************************************************/
static Type_uByte nubLed_SetLedValue(Type_uByte aubLedValue);

/********************************************************************************************/
/*							Function Definition Section										*/
/********************************************************************************************/
/********************************************************************************************/
/**	\function		TSK_LED
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
void TSK_LED(void)
{
	OS_ErrCode_t aswErrCode;
	OS_StdQueue_ts *astStdQueue_p;
	OS_StdQueue_ts astStdQueue_st;
	OS_EventCode_t auhEventCode;

	Type_uByte aubDuty_a[LED_DUTY_SIZE];
	Type_uHWord auhDuty;
	Type_sWord aswRetHal;
	Type_uByte aubLedValue;
	Type_uByte aubEepRet;
	Type_sByte asbRpcRet;
	
	aswErrCode = OS_ERR_OK;
	astStdQueue_p = STD_NULL;
	auhEventCode  = LED_VALUE_0;
	aswRetHal = HAL_ERR_NOT_INITIALIZED;

	auhDuty = LED_VALUE_0;
	aubLedValue = LED_VALUE_0;
	aubEepRet = RES_NG;
	asbRpcRet = RPC_NG;

	OSAL_MemSet(&astStdQueue_st, LED_VALUE_0, sizeof(OS_StdQueue_ts));
	OSAL_MemSet(aubDuty_a, LED_VALUE_0, sizeof(aubDuty_a));
	astStdQueue_p = &astStdQueue_st;

	wubLed_VariableInit();

	/* regiest rpc  */
	(void)wsbRPCInit(ARPC_ID_LED,(wvdRPCCallback)wvdLed_RpcCallback);

	while (1) {
		aswErrCode = OS_ReceiveQueue(MBX_ID_LED, OS_TOUT_FOREVER, &astStdQueue_p);
		if (aswErrCode !=OS_ERR_OK) {
			LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>aswErrCode:%d", __FUNCTION__, __LINE__, aswErrCode);
		} else {
			auhEventCode = astStdQueue_p->uhEventCode;
			LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d>auhEventCode:%x", __FUNCTION__, __LINE__, auhEventCode);
			switch (auhEventCode) {
				case HMI_LED_VALUE_GET_REQ:
					/* get LED value from eeprom */
					aubLedValue = wubLed_GetLedValue();
					asbRpcRet = wsbRPCSendMsg(ARPC_ID_LED, ARPC_ID_HMI, HMI_LED_VALUE_NOTICE, &aubLedValue, sizeof(aubLedValue));
					if (asbRpcRet == RPC_OK) {
						LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d>send to hmi", __FUNCTION__, __LINE__);
					} else {
						LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>rpc send err:%d", __FUNCTION__, __LINE__, asbRpcRet);
					}

					break;
		
				case HMI_LED_VALUE_SET:
					if (astStdQueue_p->vdQueueBuffer_p != STD_NULL) {
						aubLedValue = *(Type_uByte *)astStdQueue_p->vdQueueBuffer_p;
						if (aubLedValue <= sizeof(nstLED_KeyDutySave_T) / sizeof(Type_uHWord)) {
							LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d>aubLedValue:%d", __FUNCTION__, __LINE__, aubLedValue);
							/* write EEPROM */
							aubEepRet = nubLed_SetLedValue(aubLedValue);
							if (aubEepRet == RES_OK) {
								/* eep write OK, set pwm */
								auhDuty = nstLED_KeyDutySave_T[aubLedValue];
								aswRetHal = (Type_sWord)HAL_Ioctl(nvbLED_HalHandle, HAL_LED_IOCTL_SET_DUTY, &auhDuty, STD_NULL, STD_NULL, STD_NULL);
								if (aswRetHal != HAL_OK) {
									LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>HAL_Ioctl err:%d", __FUNCTION__, __LINE__, aswRetHal);
								}
								nubLED_KeyLight = aubLedValue;
							} else {
								/* eep write error, not set LED, and return current LED value to HMI */
								aubLedValue = nubLED_KeyLight;
								LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>aubEepRet:%d", __FUNCTION__, __LINE__, aubEepRet);
							}
							asbRpcRet = wsbRPCSendMsg(ARPC_ID_LED, ARPC_ID_HMI, HMI_LED_VALUE_NOTICE, &aubLedValue, sizeof(aubLedValue));
							if (asbRpcRet == RPC_OK) {
								LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d>send to hmi", __FUNCTION__, __LINE__);
							} else {
								LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>rpc send err:%d", __FUNCTION__, __LINE__, asbRpcRet);
							}
						} else {
							/* parameter error */
							LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>aubLedValue:%d", __FUNCTION__, __LINE__, aubLedValue);
						}
					}else {
						/* parameter error */
						LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>vdQueueBuffer_p is NULL", __FUNCTION__, __LINE__);
					}
					break;

				case DIAGAPP_EV_ALL_CLEAR_NOTIFY:
					aubLedValue = LED_KEY_LIGHT_VALUE_POSITIVE_0;
					/* write EEPROM */
					aubEepRet = nubLed_SetLedValue(aubLedValue);
					if (aubEepRet == RES_OK) {
						nubLED_KeyLight = aubLedValue;
						auhDuty = LED_ILL_OFF;
					    //auhDuty = 0;
						aswRetHal = (Type_sWord)HAL_Ioctl(nvbLED_HalHandle, HAL_LED_IOCTL_SET_DUTY, &auhDuty, STD_NULL, STD_NULL, STD_NULL);
						if (aswRetHal != HAL_OK) {
							aubLedValue = nubLED_KeyLight;
							LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>HAL_Ioctl err:%d", __FUNCTION__, __LINE__, aswRetHal);
						}
					} else {
						aubLedValue = nubLED_KeyLight;
						LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>aubEepRet:%d", __FUNCTION__, __LINE__, aubEepRet);
					}
					asbRpcRet = wsbRPCSendMsg(ARPC_ID_LED, ARPC_ID_DIAGAPP, DIAGAPP_EV_ALL_CLEAR_CB, &aubLedValue, sizeof(aubLedValue));
					if (asbRpcRet == RPC_OK) {
						LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d>send to diagApp", __FUNCTION__, __LINE__);
					} else {
						LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>rpc send err:%d", __FUNCTION__, __LINE__, asbRpcRet);
					}
					break;

				case DIAGAPP_EV_FACTORY_RESET_NOTIFY:
					aubLedValue = LED_KEY_LIGHT_VALUE_POSITIVE_0;
					/* write EEPROM */
					aubEepRet = nubLed_SetLedValue(aubLedValue);
					if (aubEepRet == RES_OK) {
						nubLED_KeyLight = aubLedValue;
						auhDuty = LED_ILL_OFF;
					    //auhDuty = 0;
						aswRetHal = (Type_sWord)HAL_Ioctl(nvbLED_HalHandle, HAL_LED_IOCTL_SET_DUTY, &auhDuty, STD_NULL, STD_NULL, STD_NULL);
						if (aswRetHal != HAL_OK) {
							aubLedValue = nubLED_KeyLight;
							LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>HAL_Ioctl err:%d", __FUNCTION__, __LINE__, aswRetHal);
						}
					} else {
						aubLedValue = nubLED_KeyLight;
						LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>aubEepRet:%d", __FUNCTION__, __LINE__, aubEepRet);
					}
					asbRpcRet = wsbRPCSendMsg(ARPC_ID_LED, ARPC_ID_DIAGAPP, DIAGAPP_EV_FACTORY_RESET_CB, &aubLedValue, sizeof(aubLedValue));
					if (asbRpcRet == RPC_OK) {
						LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d>send to diagApp", __FUNCTION__, __LINE__);
					} else {
						LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>rpc send err:%d", __FUNCTION__, __LINE__, asbRpcRet);
					}
					break;
		
				case HMI_LED_PWM_SET:
					if (astStdQueue_p->vdQueueBuffer_p != STD_NULL) {
						wvdGen_MemCpy(aubDuty_a, (Type_uByte*)astStdQueue_p->vdQueueBuffer_p, LED_DUTY_SIZE);
						auhDuty = ((Type_uHWord)aubDuty_a[LED_DUTY_ARRAY_1]<<8) | aubDuty_a[LED_DUTY_ARRAY_0];
						aswRetHal = (Type_sWord)HAL_Ioctl(nvbLED_HalHandle, HAL_LED_IOCTL_SET_DUTY, &auhDuty, STD_NULL, STD_NULL, STD_NULL);
						if (aswRetHal != HAL_OK) {
							LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>HAL_Ioctl err:%d", __FUNCTION__, __LINE__, aswRetHal);
						}
					} else {
						/* parameter error */
					    LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>vdQueueBuffer_p is NULL", __FUNCTION__, __LINE__);
					}
					break;
		
				case LINETOOL_LED_SET_REQ:
					if (astStdQueue_p->vdQueueBuffer_p != STD_NULL) {
						aubLedValue = *(Type_uByte *)astStdQueue_p->vdQueueBuffer_p;
						
						if (aubLedValue == LED_OFF) {
							auhDuty = LED_ILL_OFF;
							//auhDuty = 0;
							aswRetHal = (Type_sWord)HAL_Ioctl(nvbLED_HalHandle, HAL_LED_IOCTL_SET_DUTY, &auhDuty, STD_NULL, STD_NULL, STD_NULL);
							if (aswRetHal != HAL_OK) {
								LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>HAL_Ioctl err:%d", __FUNCTION__, __LINE__, aswRetHal);
							} else {
								asbRpcRet = wsbRPCSendMsg(ARPC_ID_LED, ARPC_ID_LINETOOLAPP, LINETOOL_LED_SET_RES, &aubLedValue, sizeof(aubLedValue));
								if (asbRpcRet == RPC_OK) {
									LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d>send to linetool", __FUNCTION__, __LINE__);
								} else {
									LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>rpc send err:%d", __FUNCTION__, __LINE__, asbRpcRet);
								}
							}
						} else if (aubLedValue == LED_ON) {
							auhDuty = nstLED_KeyDutySave_T[nubLED_KeyLight];
							aswRetHal = (Type_sWord)HAL_Ioctl(nvbLED_HalHandle, HAL_LED_IOCTL_SET_DUTY, &auhDuty, STD_NULL, STD_NULL, STD_NULL);
							if (aswRetHal != HAL_OK) {
								LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>HAL_Ioctl err:%d", __FUNCTION__, __LINE__, aswRetHal);
							} else {
								asbRpcRet = wsbRPCSendMsg(ARPC_ID_LED, ARPC_ID_LINETOOLAPP, LINETOOL_LED_SET_RES, &aubLedValue, sizeof(aubLedValue));
								if (asbRpcRet == RPC_OK) {
									LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d>send to linetool", __FUNCTION__, __LINE__);
								} else {
									LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>rpc send err:%d", __FUNCTION__, __LINE__, asbRpcRet);
								}
							}
						} else {
							/* parameter error */
							LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>aubLedValue:%x", __FUNCTION__, __LINE__, aubLedValue);
						}
					} else {
						/* parameter error */
						LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>vdQueueBuffer_p is NULL", __FUNCTION__, __LINE__);	
					}
					break;

				case SNR_EV_INF_ILLUMI_CHG:
					if (astStdQueue_p->vdQueueBuffer_p != STD_NULL) {
						aubLedValue = *(Type_uByte *)astStdQueue_p->vdQueueBuffer_p;
						
						if (aubLedValue == LED_OFF) {
							nubLED_ILL = LED_STATUS_OFF;
							if (nubLED_State == LED_STATUS_ACC_ON_POWER_OFF) {
								/* do action when power off/on */
							    auhDuty = 0;
								aswRetHal = (Type_sWord)HAL_Ioctl(nvbLED_HalHandle, HAL_LED_IOCTL_SET_DUTY, &auhDuty, STD_NULL, STD_NULL, STD_NULL);
								if (aswRetHal != HAL_OK) {
									LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>HAL_Ioctl err:%d", __FUNCTION__, __LINE__, aswRetHal);
								}
								LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d>nubLED_State is OFF", __FUNCTION__, __LINE__);
							} else {
								auhDuty = LED_ILL_OFF;
						        //auhDuty = 0;
							    aswRetHal = (Type_sWord)HAL_Ioctl(nvbLED_HalHandle, HAL_LED_IOCTL_SET_DUTY, &auhDuty, STD_NULL, STD_NULL, STD_NULL);
							    if (aswRetHal != HAL_OK) {
								   LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>HAL_Ioctl err:%d", __FUNCTION__, __LINE__, aswRetHal);
							    }
							}	
						} else if (aubLedValue == LED_ON) {
							/* backup state for power off/on */
							nubLED_ILL = LED_STATUS_ON;
							if (nubLED_State == LED_STATUS_ACC_ON_POWER_OFF) {
								/* do action when power on */
							    auhDuty = 0;
								aswRetHal = (Type_sWord)HAL_Ioctl(nvbLED_HalHandle, HAL_LED_IOCTL_SET_DUTY, &auhDuty, STD_NULL, STD_NULL, STD_NULL);
								if (aswRetHal != HAL_OK) {
									LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>HAL_Ioctl err:%d", __FUNCTION__, __LINE__, aswRetHal);
								}
								LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d>nubLED_State is OFF", __FUNCTION__, __LINE__);
							} else {
								auhDuty = nstLED_KeyDutySave_T[nubLED_KeyLight];
								aswRetHal = (Type_sWord)HAL_Ioctl(nvbLED_HalHandle, HAL_LED_IOCTL_SET_DUTY, &auhDuty, STD_NULL, STD_NULL, STD_NULL);
								if (aswRetHal != HAL_OK) {
									LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>HAL_Ioctl err:%d", __FUNCTION__, __LINE__, aswRetHal);
								}
							}
						} else {
							/* parameter error */
							LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>aubLedValue:%x", __FUNCTION__, __LINE__, aubLedValue);
						}
					} else {
						/* parameter error */
						LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>vdQueueBuffer_p is NULL", __FUNCTION__, __LINE__);	
					}
					break;
		
				default:
					break;
			}
			
			if (astStdQueue_p->vdQueueBuffer_p != STD_NULL) {
				aswErrCode = OS_ReleaseFMemoryBlock(astStdQueue_p->vdQueueBuffer_p);
				if (aswErrCode != OS_ERR_OK) {
					LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>OS_ReleaseFMemoryBlock:%d", __FUNCTION__, __LINE__, aswErrCode);
				} else {
					;
				}
			} else {
				/* no action */
				LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>vdQueueBuffer_p is NULL", __FUNCTION__, __LINE__);
			}
		}
	}
}
/********************************************************************************************/
/**	\function		wubLed_VariableInit
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
void wubLed_VariableInit(void) 
{
	nubLED_State = LED_STATUS_ACC_OFF;
	nubLED_KeyLight = LED_KEY_LIGHT_VALUE_POSITIVE_0;
	nubLED_ILL = LED_STATUS_OFF;
	nvbLED_HalHandle = STD_NULL;
}
/********************************************************************************************/
/**	\function		nubLed_GetLedValue
 *	\date			2021/09/16
 *	\author			liyongnan
 *	\description	Get Led Value From EEPROM 
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		none
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			Led Value (LED_KEY_LIGHT_VALUE_NEGATIVE_5 ~ LED_KEY_LIGHT_VALUE_POSITIVE_5)
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/09/16		liyongnan		New
 ********************************************************************************************/
Type_uByte wubLed_GetLedValue(void) 
{
	Type_uByte aubKeyLightValue = LED_VALUE_0;
	Type_uByte aubReturn = RES_BUSY;
	Type_uByte aubRetryCnt = LED_VALUE_0;

	/* check eep result, if NG retry 3  */
	for(aubRetryCnt = LED_VALUE_0; (aubRetryCnt < LED_VALUE_3) && (aubReturn == RES_BUSY); aubRetryCnt++) {
		aubReturn = wubEEP_BlockRead(LED_KEY_LIGHT_VALUE_ADDRESS, LED_KEY_LIGHT_VALUE_SIZE, &aubKeyLightValue);
		OS_SleepTask(LED_VALUE_1);
	}
	
	if (aubReturn != RES_OK) {
		LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>EEP_Read:%d", __FUNCTION__, __LINE__, aubReturn);
		/* get from eep failure, return default value */
		aubKeyLightValue =  LED_KEY_LIGHT_VALUE_POSITIVE_0;
	} else {
		LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d>Value:%d", __FUNCTION__, __LINE__, aubKeyLightValue);
	}
	
	return aubKeyLightValue;

}
/********************************************************************************************/
/**	\function		nubLed_SetLedValue
 *	\date			2021/09/16
 *	\author			liyongnan
 *	\description	Set Led Value To EEPROM 
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		Led Value (LED_KEY_LIGHT_VALUE_NEGATIVE_5 ~ LED_KEY_LIGHT_VALUE_POSITIVE_5)
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			Eep write result
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/09/16		liyongnan		New
 ********************************************************************************************/
static Type_uByte nubLed_SetLedValue(Type_uByte aubLedValue) 
{
	Type_uByte aubReturn = RES_BUSY;
	Type_uByte aubRetryCnt = LED_VALUE_0;
	
	/* check eep result, if NG retry 3  */
	for(aubRetryCnt = LED_VALUE_0; (aubRetryCnt < LED_VALUE_3) && (aubReturn == RES_BUSY); aubRetryCnt++) {
		aubReturn = wubEEP_BlockWrite( LED_KEY_LIGHT_VALUE_ADDRESS, LED_VALUE_1, &aubLedValue);
		OS_SleepTask(LED_VALUE_1);
	}
	return aubReturn;
}

/* end of file */
