/*******************************************************************************************/
/**<!-------Copyright (C) 2021 Alpine Electronics, Inc. All rights reserved. --------------->
 *
 * \file					Sensor_core.c
 * \date					2021/10/25
 * \author					Guanjiafu
 * \model					EDA
 * \description				LED Control Routine
 * \version					0.55
 *
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/10/25		Guanjiafu		New
 ********************************************************************************************/

/********************************************************************************************/
/*                      	Include File Section                           				    */
/********************************************************************************************/
#include "apn_basic_type.h"
#include "generic.h"
#include "os_framework.h"
#include "rpc_if.h"
#include "rpc_event.h"
#include "rpc_data.h"
#include "sensor_if.h"
#include "sensor_core.h"
#include "HAL_Snr.h"

/********************************************************************************************/
/*							Global Variable Definition Section								*/
/********************************************************************************************/
HAL_HANDLE nvbSnr_HalHandle;
Type_uByte nubSnr_State;

static Type_uHWord nuhSnr_ClockTempCheckCount;
static Type_uHWord nuhSnr_FanTempCheckCount;
static Type_uHWord nuhSnr_ParkCheckCount;
static Type_uHWord nuhSnr_VspCheckCount;
static Type_uHWord nuhSnr_IllmCheckCount;
static Type_uHWord nuhSnr_TelMuteCheckCount;
  
Type_uByte nubSnr_IllmStatus;
Type_uByte nubSnr_TempStatus;
Type_uByte nubSnr_ParkStatus;
Type_uByte nubSnr_TelMuteStatus;
Type_uHWord nuhSnr_VspValue;
Type_uHWord nuhSnr_FanTempValue;

Type_uByte nubSnr_FanTempMode;

/********************************************************************************************/
/*							ROM Table Section												*/
/********************************************************************************************/
/********************************************************************************************/
/*							Static Prototype Declaration Section							*/
/********************************************************************************************/
static void nvdSnr_FanTempChk(void);
static void nvdSnr_ClockTempChk(void);
static void nvdSnr_ILLSignalChk(void);
static void nvdSnr_ParkSignalChk(void);
static void nvdSnr_VspChk(void);
static void nvdSnr_TelMuteSignalChk(void);
/********************************************************************************************/
/*							Function Definition Section										*/
/********************************************************************************************/
/********************************************************************************************/
/**	\function		CYC_SNR	
 *	\date			2021/10/25
 *	\author			Guanjiafu
 *	\description	CYC Task (2ms)
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		none
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			none
 *
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/10/25		Guanjiafu		New
 *                  0002    2022/10/11      Zhangruishan    Change cyc_snr to tsk_cyc_snr
 ********************************************************************************************/
void TSK_CYC_SNR(void)
{
	for( ; ; ) 
	{
		
		OS_SleepTask(2);
		
		if (SNR_CTRL_LOAD == nubSnr_State) {
			//CLOCK
			nuhSnr_ClockTempCheckCount++;
			if (SNR_CHKCNT_CLOCK_TEMP <= nuhSnr_ClockTempCheckCount) {
				nvdSnr_ClockTempChk();
				nuhSnr_ClockTempCheckCount = SNR_VALUE_0;
			} else {
				 ;
			}
			//FAN
			if (nubSnr_FanTempMode == SNR_INF_FAN_CONTROL) {
				nuhSnr_FanTempCheckCount++;
				if (SNR_CHKCNT_FAN_TEMP <= nuhSnr_FanTempCheckCount) {
				   nvdSnr_FanTempChk();
				   nuhSnr_FanTempCheckCount = SNR_VALUE_0;
				} else {
					;
				}
			} else {
				/* linetool mode not check */
				nuhSnr_FanTempCheckCount = SNR_VALUE_0;
			}
			 //PARK
			nuhSnr_ParkCheckCount++;
			if (SNR_CHKCNT_PARK <= nuhSnr_ParkCheckCount) {
				nvdSnr_ParkSignalChk();
			 	nuhSnr_ParkCheckCount = SNR_VALUE_0;
			} else {
				 ;
			}

			//ILLM
			nuhSnr_IllmCheckCount++;
			if (SNR_CHKCNT_ILLM <= nuhSnr_IllmCheckCount) {
				nvdSnr_ILLSignalChk();
				nuhSnr_IllmCheckCount = SNR_VALUE_0;
			} else {
				;
			}
			//VSP
			nuhSnr_VspCheckCount++;
			if (SNR_CHKCNT_VSP <= nuhSnr_VspCheckCount) {
				nvdSnr_VspChk();
				nuhSnr_VspCheckCount = SNR_VALUE_0;
			} else {
				;
			}
			
			//TEL MUTE
			nuhSnr_TelMuteCheckCount++;
			if (SNR_CHKCNT_TELMUTE <= nuhSnr_TelMuteCheckCount) {
				nvdSnr_TelMuteSignalChk();
				nuhSnr_TelMuteCheckCount = SNR_VALUE_0;
			} else {
				 ;
			}
		 }
		 else {
			 ;
		 }
	}
 }
/********************************************************************************************/
/**	\function		TSK_SNR	
 *	\date			2021/10/25
 *	\author			Guanjiafu
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
 *					0001	2021/10/25		Guanjiafu		New
 ********************************************************************************************/
void TSK_SNR(void)
{
	LOG_OUTPUT(LOG_APP_ID_SNR, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d>", __FUNCTION__, __LINE__);
	OS_ErrCode_t aswErrCode;
	OS_StdQueue_ts *astStdQueue_p;
	OS_StdQueue_ts astStdQueue_st;
	OS_EventCode_t auhEventCode;
	Type_sWord aswRetHal;
	Type_uHWord auhVsp;

	Type_uByte aubFan;
	Type_uByte aubIllStatus;
	Type_uByte aubParkStatus;
	Type_sByte asbRpcRet;

	aswErrCode = OS_ERR_OK;
	astStdQueue_p = STD_NULL;
	auhEventCode = SNR_VALUE_0;	
	aswRetHal = HAL_ERR_NOT_INITIALIZED;
	aubFan = SNR_VALUE_0;
	aubIllStatus = SNR_VALUE_0;
	aubParkStatus = SNR_VALUE_0;
	auhVsp = SNR_VALUE_0;
	asbRpcRet = RPC_NG;
	OSAL_MemSet(&astStdQueue_st, SNR_VALUE_0, sizeof(OS_StdQueue_ts));
	astStdQueue_p = &astStdQueue_st;

	wvdSnr_VariableInit();


	/* regiest rpc  */
	(void)wsbRPCInit(ARPC_ID_SENSOR,(wvdRPCCallback)wvdSnr_RpcCallback);

	while (1) {
		aswErrCode = OS_ReceiveQueue(MBX_ID_SNR, OS_TOUT_FOREVER, &astStdQueue_p);

		if (aswErrCode !=OS_ERR_OK) {
			LOG_OUTPUT(LOG_APP_ID_SNR, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d> aswErrCode:%d", __FUNCTION__, __LINE__, aswErrCode);
		} else {
			auhEventCode = astStdQueue_p->uhEventCode;
			switch (auhEventCode) {
				case SNR_EV_INF_FANSET_REQ:
					/*FAN*/
					if (astStdQueue_p->vdQueueBuffer_p != STD_NULL) {
						aubFan = *(Type_uByte *)astStdQueue_p->vdQueueBuffer_p;
						nubSnr_FanTempMode = aubFan;
						if (aubFan != SNR_INF_FAN_CONTROL) {
							aswRetHal = (Type_sWord)HAL_Ioctl(nvbSnr_HalHandle, HAL_SNR_IOCTL_SET_FAN, &aubFan, STD_NULL, STD_NULL, STD_NULL);
							if (aswRetHal != HAL_OK) {
								LOG_OUTPUT(LOG_APP_ID_SNR, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>HAL_Ioctl err:%d", __FUNCTION__, __LINE__, aswRetHal);
							}
						} else {
							/* control by temp */
						}
						/* send response to linetool */
						if ((astStdQueue_p->swSrcTaskId == ARPC_ID_LINETOOLAPP) && (aswRetHal == HAL_OK)) {
							asbRpcRet = wsbRPCSendMsg(ARPC_ID_SENSOR, ARPC_ID_LINETOOLAPP, SNR_EV_INF_FANSET_RES, STD_NULL, SNR_VALUE_0);
							if (asbRpcRet == RPC_OK) {
								LOG_OUTPUT(LOG_APP_ID_SNR, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d>send to linetool", __FUNCTION__, __LINE__);
							} else {
								LOG_OUTPUT(LOG_APP_ID_SNR, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>rpc send err:%d", __FUNCTION__, __LINE__, asbRpcRet);
							}
						}
					} else {
						/* parameter error */
						LOG_OUTPUT(LOG_APP_ID_SNR, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, 
							"\n <%s:%d> vdQueueBuffer_p is NULL", __FUNCTION__, __LINE__);
					}
					break;

				case SNR_EV_INF_HMI_READY_REQ:
					//send ill/park/reverse/vsp to HMI
					aubIllStatus = nubSnr_IllmStatus;
					aubParkStatus = nubSnr_ParkStatus;
					auhVsp = nuhSnr_VspValue;
					asbRpcRet = wsbRPCSendMsg(ARPC_ID_SENSOR, ARPC_ID_HMI, SNR_EV_INF_ILLUMI_CHG, (void*)&aubIllStatus, sizeof(aubIllStatus));
					if (asbRpcRet != RPC_OK) {
						LOG_OUTPUT(LOG_APP_ID_SNR, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>rpc send err:%d", __FUNCTION__, __LINE__, asbRpcRet);
					}
					asbRpcRet = wsbRPCSendMsg(ARPC_ID_SENSOR, ARPC_ID_HMI, SNR_EV_INF_PARK_CHG, (void*)&aubParkStatus, sizeof(aubParkStatus));
					if (asbRpcRet != RPC_OK) {
						LOG_OUTPUT(LOG_APP_ID_SNR, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>rpc send err:%d", __FUNCTION__, __LINE__, asbRpcRet);
					}
					asbRpcRet = wsbRPCSendMsg(ARPC_ID_SENSOR, ARPC_ID_HMI, SNR_EV_INF_VSP_CHG, (void*)&auhVsp, sizeof(auhVsp));
					if (asbRpcRet != RPC_OK) {
						LOG_OUTPUT(LOG_APP_ID_SNR, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>rpc send err:%d", __FUNCTION__, __LINE__, asbRpcRet);
					}
					LOG_OUTPUT(LOG_APP_ID_SNR, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF,
						"\n <%s:%d>ill:%d park:%d vsp:%d to HMI", __FUNCTION__, __LINE__, aubIllStatus, aubParkStatus, auhVsp);
 					break;

				case SNR_EV_INF_ILLUMI_GET_REQ:
					aubIllStatus = nubSnr_IllmStatus;
					asbRpcRet = wsbRPCSendMsg(ARPC_ID_SENSOR, ARPC_ID_VIDEOSOURCE, SNR_EV_INF_ILLUMI_GET_RES, (void*)&aubIllStatus, sizeof(aubIllStatus));
					if (asbRpcRet != RPC_OK) {
						LOG_OUTPUT(LOG_APP_ID_SNR, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>rpc send err:%d", __FUNCTION__, __LINE__, asbRpcRet);
					} else {
						LOG_OUTPUT(LOG_APP_ID_SNR, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, 
							"\n <%s:%d> send ill:%d to videosource", __FUNCTION__, __LINE__, aubIllStatus);
					}
					break;

				/* below is send from CYC check */
				case SNR_EV_INF_PARK_CHG:
					if (astStdQueue_p->vdQueueBuffer_p != STD_NULL) {
						asbRpcRet = wsbRPCSendMsg(ARPC_ID_SENSOR, ARPC_ID_HMI, SNR_EV_INF_PARK_CHG, astStdQueue_p->vdQueueBuffer_p, astStdQueue_p->uwQueueSize);
						if (asbRpcRet != RPC_OK) {
							LOG_OUTPUT(LOG_APP_ID_SNR, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>rpc send err:%d", __FUNCTION__, __LINE__, asbRpcRet);
						} else {
							LOG_OUTPUT(LOG_APP_ID_SNR, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, 
								"\n <%s:%d> send park:%d to hmi", __FUNCTION__, __LINE__, *(Type_uByte *)astStdQueue_p->vdQueueBuffer_p);
						}
					}
					break;

				case SNR_EV_INF_ILLUMI_CHG:
					if (astStdQueue_p->vdQueueBuffer_p != STD_NULL) {
						asbRpcRet = wsbRPCSendMsg(ARPC_ID_SENSOR, ARPC_ID_LED, SNR_EV_INF_ILLUMI_CHG, astStdQueue_p->vdQueueBuffer_p, astStdQueue_p->uwQueueSize);
						if (asbRpcRet != RPC_OK) {
							LOG_OUTPUT(LOG_APP_ID_SNR, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>rpc send err:%d", __FUNCTION__, __LINE__, asbRpcRet);
						}
						asbRpcRet = wsbRPCSendMsg(ARPC_ID_SENSOR, ARPC_ID_HMI, SNR_EV_INF_ILLUMI_CHG, astStdQueue_p->vdQueueBuffer_p, astStdQueue_p->uwQueueSize);
						if (asbRpcRet != RPC_OK) {
							LOG_OUTPUT(LOG_APP_ID_SNR, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>rpc send err:%d", __FUNCTION__, __LINE__, asbRpcRet);
						}
						LOG_OUTPUT(LOG_APP_ID_SNR, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, 
							"\n <%s:%d> send ill:%d to hmi/led", __FUNCTION__, __LINE__, *(Type_uByte *)astStdQueue_p->vdQueueBuffer_p);
					}
					break;

				case SNR_EV_INF_VSP_CHG:
					if (astStdQueue_p->vdQueueBuffer_p != STD_NULL) {
						asbRpcRet = wsbRPCSendMsg(ARPC_ID_SENSOR, ARPC_ID_HMI, SNR_EV_INF_VSP_CHG, astStdQueue_p->vdQueueBuffer_p, astStdQueue_p->uwQueueSize);
						if (asbRpcRet != RPC_OK) {
							LOG_OUTPUT(LOG_APP_ID_SNR, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>rpc send err:%d", __FUNCTION__, __LINE__, asbRpcRet);
						}
						asbRpcRet = wsbRPCSendMsg(ARPC_ID_SENSOR, ARPC_ID_SOUNDAPP, SNR_EV_INF_VSP_CHG, astStdQueue_p->vdQueueBuffer_p, astStdQueue_p->uwQueueSize);
						if (asbRpcRet != RPC_OK) {
							LOG_OUTPUT(LOG_APP_ID_SNR, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>rpc send err:%d", __FUNCTION__, __LINE__, asbRpcRet);
						}
						asbRpcRet = wsbRPCSendMsg(ARPC_ID_SENSOR, ARPC_ID_REARCAMERA, SNR_EV_INF_VSP_CHG, astStdQueue_p->vdQueueBuffer_p, astStdQueue_p->uwQueueSize);
						if (asbRpcRet != RPC_OK) {
							LOG_OUTPUT(LOG_APP_ID_SNR, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>rpc send err:%d", __FUNCTION__, __LINE__, asbRpcRet);
						}
						LOG_OUTPUT(LOG_APP_ID_SNR, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, 
							"\n <%s:%d> send vsp:%d to hmi/sound/rear cam", __FUNCTION__, __LINE__, *(Type_uByte *)astStdQueue_p->vdQueueBuffer_p);
					}
					break;

				case SNR_EV_INF_TEMP_1H:
					if (astStdQueue_p->vdQueueBuffer_p != STD_NULL) {
						asbRpcRet = wsbRPCSendMsg(ARPC_ID_SENSOR, ARPC_ID_CLOCK, SNR_EV_INF_TEMP_1H, astStdQueue_p->vdQueueBuffer_p, astStdQueue_p->uwQueueSize);
						if (asbRpcRet != RPC_OK) {
							LOG_OUTPUT(LOG_APP_ID_SNR, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>rpc send err:%d", __FUNCTION__, __LINE__, asbRpcRet);
						} else {
							LOG_OUTPUT(LOG_APP_ID_SNR, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, 
								"\n <%s:%d> send temp:%x to clock", __FUNCTION__, __LINE__, *(Type_uByte *)astStdQueue_p->vdQueueBuffer_p);
						}
					}
					break;

				case SNR_EV_INF_TELMUTE_CHG:
					if (astStdQueue_p->vdQueueBuffer_p != STD_NULL) {
						asbRpcRet = wsbRPCSendMsg(ARPC_ID_SENSOR, ARPC_ID_HMI, SNR_EV_INF_TELMUTE_CHG, astStdQueue_p->vdQueueBuffer_p, astStdQueue_p->uwQueueSize);
						if (asbRpcRet != RPC_OK) {
							LOG_OUTPUT(LOG_APP_ID_SNR, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>rpc send err:%d", __FUNCTION__, __LINE__, asbRpcRet);
						} else {
							LOG_OUTPUT(LOG_APP_ID_SNR, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, 
								"\n <%s:%d> send telM:%d to hmi", __FUNCTION__, __LINE__, *(Type_uByte *)astStdQueue_p->vdQueueBuffer_p);
						}
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
				/* NO ACTION */
			}
		}
	}

}
/********************************************************************************************/
/**	\function		wvdSensor_VariableInit
 *	\date			2021/10/25
 *	\author			Guanjiafu
 *	\description	Init Global Variable
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		none
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			none
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/10/25		Guanjiafu		New
 ********************************************************************************************/
void wvdSnr_VariableInit(void)
{
	nvbSnr_HalHandle = STD_NULL;
	nubSnr_State = SNR_STATUS_UNLOAD;

	nubSnr_IllmStatus = SNR_BYTE_INVALID;
	nubSnr_ParkStatus = SNR_BYTE_INVALID;
	nubSnr_TempStatus = SNR_BYTE_INVALID;
	nuhSnr_VspValue = SNR_HWORD_INVALID;
	nuhSnr_FanTempValue = SNR_HWORD_INVALID;
	nubSnr_TelMuteStatus = SNR_VALUE_0;

	nuhSnr_ClockTempCheckCount = SNR_VALUE_0;
	nuhSnr_FanTempCheckCount = SNR_VALUE_0;
	nuhSnr_ParkCheckCount = SNR_VALUE_0;
	nuhSnr_VspCheckCount = SNR_VALUE_0;
	nuhSnr_IllmCheckCount = SNR_VALUE_0;
	nuhSnr_TelMuteCheckCount = SNR_VALUE_0;

	nubSnr_FanTempMode = SNR_INF_FAN_CONTROL;
}
/********************************************************************************************/
/**	\function		nvdSnr_FanTempChk	
 *	\date			2021/10/25
 *	\author			Guanjiafu
 *	\description	Check temp and set FAN
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		none
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			none
 *
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/10/25		Guanjiafu		New
 ********************************************************************************************/
static void nvdSnr_FanTempChk(void)
{
	Type_uHWord auhTempValue;
	Type_sWord aswRetHal;
	Type_uByte aubTempStauts;
	Type_uByte aubFan;

	auhTempValue = SNR_VALUE_0;
	aswRetHal = HAL_ERR_NOT_INITIALIZED;
	aubTempStauts = SNR_VALUE_0;
	aubFan = SNR_VALUE_0;

	aswRetHal = (Type_sWord)HAL_Ioctl(nvbSnr_HalHandle, HAL_SNR_IOCTL_CHECK_HiTEMP, &auhTempValue, STD_NULL, STD_NULL, STD_NULL);
	if (aswRetHal == HAL_OK) {
	 	nuhSnr_FanTempValue = (Type_uHWord)auhTempValue;
		if (nuhSnr_FanTempValue <= SNR_FAN_TEMP_HIGH) {
			//high temp
			aubTempStauts = SNR_STATUS_TEMP_HI;
		} else if (nuhSnr_FanTempValue >= SNR_FAN_TEMP_LOW) {
			 //normal temp
			aubTempStauts = SNR_STATUS_TEMP_NORMAL;
		} else {
			//keep
			aubTempStauts = SNR_STATUS_TEMP_KEEP;
		}

		if ((aubTempStauts != SNR_STATUS_TEMP_KEEP) && (nubSnr_TempStatus != aubTempStauts)) {
			LOG_OUTPUT(LOG_APP_ID_SNR, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF,  
				"\n <%s:%d> auhTempValue:%x", __FUNCTION__, __LINE__, auhTempValue);
			/* status change, send */
			nubSnr_TempStatus = aubTempStauts;
			if (aubTempStauts == SNR_STATUS_TEMP_HI) {
				aubFan = SNR_INF_FAN_ON;
			} else {
				/* normal  */
				aubFan = SNR_INF_FAN_OFF;
			}
			aswRetHal = (Type_sWord)HAL_Ioctl(nvbSnr_HalHandle, HAL_SNR_IOCTL_SET_FAN, &aubFan, STD_NULL, STD_NULL, STD_NULL);
			if (aswRetHal != HAL_OK) {
				LOG_OUTPUT(LOG_APP_ID_SNR, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>HAL_Ioctl err:%d", __FUNCTION__, __LINE__, aswRetHal);
			}
		} else {
			;
		}
	 } else {
		 /* HAL error */
		 ;
	 }

}
/********************************************************************************************/
/**	\function		nvdSnr_ClockTempChk	
 *	\date			2021/10/25
 *	\author			Guanjiafu
 *	\description	get 1H average temp
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		none
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			none
 *
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/10/25		Guanjiafu		New
 ********************************************************************************************/
static void nvdSnr_ClockTempChk(void)
{
	Type_sWord	aswRetHal;
	Type_uHWord auhTempValue;
	Type_uByte aubTempValue[SNR_VALUE_2];

	aswRetHal = HAL_ERR_NOT_INITIALIZED;
	auhTempValue = SNR_VALUE_0;
	OSAL_MemSet(aubTempValue, SNR_VALUE_0, sizeof(aubTempValue));

	aswRetHal = HAL_Ioctl(nvbSnr_HalHandle, HAL_SNR_IOCTL_CHECK_CLOCKTEMP, &auhTempValue, STD_NULL, STD_NULL, STD_NULL);
	if (aswRetHal == HAL_OK) {
		aubTempValue[0] = (Type_uByte)(auhTempValue & 0x00ff);
		aubTempValue[1] = (Type_uByte)((auhTempValue & 0xff00) >> 8);
		wsbRPCSendMsg(ARPC_ID_SENSOR, ARPC_ID_SENSOR, SNR_EV_INF_TEMP_1H, aubTempValue, sizeof(aubTempValue));
	} else {
		/* HAL error */
		;
	}

}
/********************************************************************************************/
/**	\function		nvdSnr_ILLSignalChk	
 *	\date			2021/10/25
 *	\author			Guanjiafu
 *	\description	Check ill signal
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		none
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			none
 *
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/10/25		Guanjiafu		New
 ********************************************************************************************/
static void nvdSnr_ILLSignalChk(void)
{
	Type_sWord	aswRetHal;
	Type_uByte	aubIllValue;

	 aswRetHal = HAL_ERR_NOT_INITIALIZED;
	 aubIllValue = SNR_VALUE_0;

	 aswRetHal = HAL_Ioctl(nvbSnr_HalHandle, HAL_SNR_IOCTL_CHECK_ILLM, &aubIllValue, STD_NULL, STD_NULL, STD_NULL);
	 if (aswRetHal == HAL_OK) {
		if (nubSnr_IllmStatus != aubIllValue) {
			nubSnr_IllmStatus = aubIllValue;
			/* status change, send */
			wsbRPCSendMsg(ARPC_ID_SENSOR, ARPC_ID_SENSOR, SNR_EV_INF_ILLUMI_CHG, &aubIllValue, sizeof(aubIllValue));
		} else {
			;
		}
	 } else {
		 /* HAL error */
		 ;
	 }

}
/********************************************************************************************/
/**	\function		nvdSnr_ParkSignalChk	
 *	\date			2021/10/25
 *	\author			Guanjiafu
 *	\description	Check park signal
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		none
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			none
 *
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/10/25		Guanjiafu		New
 ********************************************************************************************/
static void nvdSnr_ParkSignalChk(void)
{
	Type_sWord	aswRetHal;
	Type_uByte	aubParkValue;

	aswRetHal = HAL_ERR_NOT_INITIALIZED;
	aubParkValue = SNR_VALUE_0;

	aswRetHal = HAL_Ioctl(nvbSnr_HalHandle, HAL_SNR_IOCTL_CHECK_PARK, &aubParkValue, STD_NULL, STD_NULL, STD_NULL);
	if (aswRetHal == HAL_OK) {
		if (nubSnr_ParkStatus != aubParkValue) {
			nubSnr_ParkStatus = aubParkValue;
			/* status change, send */
			wsbRPCSendMsg(ARPC_ID_SENSOR, ARPC_ID_SENSOR, SNR_EV_INF_PARK_CHG, &aubParkValue, sizeof(aubParkValue));
		} else {
			;
		}
	} else {
		 /* HAL error */
		 ;
	 }

}

/********************************************************************************************/
/**	\function		nvdSnr_VspChk	
 *	\date			2021/10/25
 *	\author			Guanjiafu
 *	\description	Check Vsp
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		none
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			none
 *
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/10/25		Guanjiafu		New
 ********************************************************************************************/
static void nvdSnr_VspChk(void)
{
	Type_sWord aswRetHal;
	Type_uByte aubVspValue[SNR_VALUE_2];
	Type_uHWord  auhVspValue;

	OSAL_MemSet(aubVspValue, SNR_VALUE_0, sizeof(aubVspValue));
	aswRetHal = HAL_ERR_NOT_INITIALIZED;
	auhVspValue = SNR_VALUE_0;

	aswRetHal = HAL_Ioctl(nvbSnr_HalHandle, HAL_SNR_IOCTL_CHECK_VSP, &auhVspValue, STD_NULL, STD_NULL, STD_NULL);
	if (aswRetHal == HAL_OK) {
		if (nuhSnr_VspValue != auhVspValue) {
			nuhSnr_VspValue = auhVspValue;
			aubVspValue[0] = (Type_uByte)(auhVspValue & 0x00ff);
			aubVspValue[1] = (Type_uByte)((auhVspValue & 0xff00) >> 8);
			wsbRPCSendMsg(ARPC_ID_SENSOR, ARPC_ID_SENSOR, SNR_EV_INF_VSP_CHG, aubVspValue, sizeof(aubVspValue));
		} else {
			;
		}
	} else {
		/* HAL error */
		;
	}

}
/********************************************************************************************/
/**	\function		nvdSnr_TelMuteSignalChk	
 *	\date			2021/10/25
 *	\author			Guanjiafu
 *	\description	Check telMute signal
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		none
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			none
 *
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/10/25		Guanjiafu		New
 ********************************************************************************************/
static void nvdSnr_TelMuteSignalChk(void)
{
	Type_sWord	aswRetHal;
	Type_uByte	aubTelMuteValue;

	aswRetHal = HAL_ERR_NOT_INITIALIZED;
	aubTelMuteValue = SNR_VALUE_0;

	aswRetHal = HAL_Ioctl(nvbSnr_HalHandle, HAL_SNR_IOCTL_CHECK_TELMUTE, &aubTelMuteValue, STD_NULL, STD_NULL, STD_NULL);
	if (aswRetHal == HAL_OK) {
		if (nubSnr_TelMuteStatus != aubTelMuteValue) {
			nubSnr_TelMuteStatus = aubTelMuteValue;
			/* status change, send */
			wsbRPCSendMsg(ARPC_ID_SENSOR, ARPC_ID_SENSOR, SNR_EV_INF_TELMUTE_CHG, &aubTelMuteValue, sizeof(aubTelMuteValue));
		} else {
			;
		}
	} else {
		 /* HAL error */
		 ;
	}

}

