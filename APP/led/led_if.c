/*******************************************************************************************/
/**<!-------Copyright (C) 2021 Alpine Electronics, Inc. All rights reserved. --------------->
 * 
 * \file					Led_if.c
 * \date					2021/09/16
 * \author					Liyongnan
 * \model					EDA
 * \description				LED API block
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

#include "HAL_devices.h"
#include "HAL_Led.h"
#include "led_if.h"
#include "led_core.h"

/********************************************************************************************/
/*							Global Variable Definition Section								*/
/********************************************************************************************/

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
/**	\function		wubLed_StandbyOut
 *	\date			2021/09/16
 *	\author			liyongnan
 *	\description	ACC on
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		none
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			RES_OK
 *					RES_NG
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/09/16		liyongnan		New
 ********************************************************************************************/
Type_uByte wubLed_StandbyOut(void)
{
	LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d> ", __FUNCTION__, __LINE__);
	nubLED_State = LED_STATUS_ACC_ON_POWER_ON;
	nubLED_KeyLight = wubLed_GetLedValue();
	nvbLED_HalHandle = HAL_Open(HAL_LED, HAL_LED_OPTION_NORMAL);
	return RES_OK;
}
/********************************************************************************************/
/**	\function		wubLed_StandbyIn
 *	\date			2021/09/16
 *	\author			liyongnan
 *	\description	ACC off
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		none
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			RES_OK
 *					RES_NG
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/09/16		liyongnan		New
 ********************************************************************************************/
Type_uByte wubLed_StandbyIn(void)
{
	LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d> ", __FUNCTION__, __LINE__);
	HAL_Close(nvbLED_HalHandle);
	wubLed_VariableInit();
	return RES_OK;
}
/********************************************************************************************/
/**	\function		wubLed_EnforceStandby
 *	\date			2021/09/16
 *	\author			liyongnan
 *	\description	Batt off
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		none
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			RES_OK
 *					RES_NG
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/09/16		liyongnan		New
 ********************************************************************************************/
Type_uByte wubLed_EnforceStandby(void)
{
	HAL_Close(nvbLED_HalHandle);
	wubLed_VariableInit();
	return RES_OK;
}
/********************************************************************************************/
/**	\function		wubLed_PowerOn
 *	\date			2021/09/16
 *	\author			liyongnan
 *	\description	Power on
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		none
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			RES_OK
 *					RES_NG
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/09/16		liyongnan		New
 ********************************************************************************************/
Type_uByte wubLed_PowerOn(void)
{
	LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d> ", __FUNCTION__, __LINE__);
	Type_uHWord auhDuty = LED_VALUE_0;

	nubLED_State = LED_STATUS_ACC_ON_POWER_ON;
	if (nubLED_ILL == LED_STATUS_ON) {
		auhDuty = nstLED_KeyDutySave_T[nubLED_KeyLight];
		HAL_Ioctl(nvbLED_HalHandle, HAL_LED_IOCTL_SET_DUTY, &auhDuty, STD_NULL, STD_NULL, STD_NULL);
	} else {
		auhDuty = LED_ILL_OFF;
		HAL_Ioctl(nvbLED_HalHandle, HAL_LED_IOCTL_SET_DUTY, &auhDuty, STD_NULL, STD_NULL, STD_NULL);
        LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d>nubLED_ILL is OFF:%d", __FUNCTION__, __LINE__, auhDuty);
	}
	return RES_OK;
}
/********************************************************************************************/
/**	\function		wubLed_PowerOff
 *	\date			2021/09/16
 *	\author			liyongnan
 *	\description	Power off
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		none
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			RES_OK
 *					RES_NG
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/09/16		liyongnan		New
 ********************************************************************************************/
Type_uByte wubLed_PowerOff(void)
{
	LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d> ", __FUNCTION__, __LINE__);
	Type_uHWord auhDuty = LED_VALUE_0;

	nubLED_State = LED_STATUS_ACC_ON_POWER_OFF;
	auhDuty = 0;
	HAL_Ioctl(nvbLED_HalHandle, HAL_LED_IOCTL_SET_DUTY, &auhDuty, STD_NULL, STD_NULL, STD_NULL);

	/*
	  if (nubLED_ILL == LED_STATUS_ON) {
		auhDuty = 0;
		HAL_Ioctl(nvbLED_HalHandle, HAL_LED_IOCTL_SET_DUTY, &auhDuty, STD_NULL, STD_NULL, STD_NULL);
	} else {
		LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d>nubLED_ILL is OFF", __FUNCTION__, __LINE__);
	}
	*/

	return RES_OK;
}
/********************************************************************************************/
/**	\function		wubLed_RpcCallback
 *	\date			2021/09/16
 *	\author			liyongnan
 *	\description	Receive rpc message(callback function of wsbRPCInit)
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		auhEventCode
 *					aubQueueBoxID
 *					aubQueuePriority
 *					avdQueueBuffer_p
 *					aubQueueSize
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			RES_OK
 *					RES_NG
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/09/16		liyongnan		New
 ********************************************************************************************/
void wvdLed_RpcCallback(const Type_uByte aubsrc, const Type_uHWord auhEventCode, const void* avdQueueBuffer_p, const Type_uHWord aubQueueSize)

{
	LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d> ", __FUNCTION__, __LINE__);
	OS_StdQueue_ts astStdQueue;
	OS_VoidPtr_t avdMpf_p;
	OS_ErrCode_t aswErrCode;
	Type_uByte aubReturnCode;
	
	/* Variable Initialize */
	aubReturnCode = RES_OK;
	aswErrCode = OS_ERR_OK;
	avdMpf_p = STD_NULL;
	OSAL_MemSet(&astStdQueue, LED_VALUE_0, sizeof(astStdQueue));

	if (nubLED_State != LED_STATUS_ACC_OFF) {
		astStdQueue.swSrcTaskId = aubsrc;
		astStdQueue.uhEventCode = auhEventCode;
		if((avdQueueBuffer_p != STD_NULL) && (aubQueueSize != LED_VALUE_0)) {
			aswErrCode = OS_GetFMemoryBlock(&avdMpf_p, aubQueueSize);
			/* Get Memory Block Result Check */
			if (OS_ERR_OK == aswErrCode) {
				wvdGen_MemCpy(avdMpf_p, avdQueueBuffer_p, aubQueueSize);
				astStdQueue.vdQueueBuffer_p = avdMpf_p;
				astStdQueue.uwQueueSize = aubQueueSize;
			} else {
				/* Can`t get memory, Ignore the message */
				aubReturnCode = RES_NG;
				LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>aswErrCode:%d", __FUNCTION__, __LINE__,aswErrCode);
			}
		} else {
			astStdQueue.vdQueueBuffer_p = STD_NULL;
			astStdQueue.uwQueueSize = LED_VALUE_0;
		}

		/* Send Msg to LED Task */
		if (aubReturnCode == RES_OK) {
			aswErrCode = OS_SendQueue(MBX_ID_LED, OS_MAIL_PRI_NORMAL, &astStdQueue);
			/* Send Msg Result Check */
			if (OS_ERR_OK == aswErrCode) {
				/* No Action */
				;
			} else {
			    LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>aswErrCode:%d", __FUNCTION__, __LINE__,aswErrCode);
				/* Get Memory Block Check */
				if (avdMpf_p != STD_NULL) {
					OS_ReleaseFMemoryBlock(avdMpf_p);
				} else {
					/* No Action */
					;
				}
			}
		}
	} else {
		LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>nubLED_State is OFF", __FUNCTION__, __LINE__);
	} 
	return;
}

/* end of file */
