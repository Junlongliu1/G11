/*******************************************************************************************/
/**<!-------Copyright (C) 2021 Alpine Electronics, Inc. All rights reserved. --------------->
 *
 * \file					sensor_if.c
 * \date					2021/10/25
 * \author					Guanjiafu
 * \model					EDA
 * \description				SNR API block
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
#include "os_framework.h"
#include "generic.h"
#include "rpc_event.h"
#include "rpc_data.h"

#include "HAL_devices.h"
#include "HAL_Snr.h"
#include "sensor_if.h"
#include "sensor_core.h"

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
/**	\function		wubSnr_StandbyOut
 *	\date			2021/10/25
 *	\author			Guanjiafu
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
 *					0001	2021/10/25		Guanjiafu		New
 ********************************************************************************************/
Type_uByte wubSnr_StandbyOut(void)
{
	LOG_OUTPUT(LOG_APP_ID_SNR, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d> ", __FUNCTION__, __LINE__);
	nubSnr_State = SNR_STATUS_LOAD;
	nvbSnr_HalHandle = HAL_Open(HAL_SNR, HAL_SNR_OPTION_NORMAL);
	return RES_OK;
}

/********************************************************************************************/
/**	\function		wubSnr_StandbyIn
 *	\date			2021/10/25
 *	\author			Guanjiafu
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
 *					0001	2021/10/25		Guanjiafu		New
 ********************************************************************************************/
Type_uByte wubSnr_StandbyIn(void)
{
	LOG_OUTPUT(LOG_APP_ID_SNR, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d> ", __FUNCTION__, __LINE__);
	HAL_Close(nvbSnr_HalHandle);
	wvdSnr_VariableInit();
	return RES_OK;
}

/********************************************************************************************/
/**	\function		wubSnr_EnforceStandby
 *	\date			2021/10/25
 *	\author			Guanjiafu
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
 *					0001	2021/10/25		Guanjiafu		New
 ********************************************************************************************/
Type_uByte wubSnr_EnforceStandby(void)
{
	//LOG_OUTPUT(LOG_APP_ID_SNR, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d> ", __FUNCTION__, __LINE__);
	HAL_Close(nvbSnr_HalHandle);
	wvdSnr_VariableInit();
	return RES_OK;
}

/********************************************************************************************/
/**	\function		wvdSnr_RpcCallback
 *	\date			2021/10/25
 *	\author			Guanjiafu
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
 *					0001	2021/10/25		Guanjiafu		New
 ********************************************************************************************/
void wvdSnr_RpcCallback(const Type_uByte aubsrc,const Type_uHWord auhEventCode, const void* avdQueueBuffer_p, const Type_uHWord aubQueueSize)
{
	Type_uByte		aubReturnCode;
	OS_ErrCode_t	aswErrCode;
	OS_StdQueue_ts	astStdQueue;
	OS_VoidPtr_t	avdMpf_p;

	/* Variable Initialize */
	aubReturnCode	= RES_OK;
	aswErrCode		= OS_ERR_OK;
	avdMpf_p		= STD_NULL;
	OSAL_MemSet(&astStdQueue, SNR_VALUE_0, sizeof(astStdQueue));

	astStdQueue.swSrcTaskId = aubsrc;
	astStdQueue.uhEventCode = auhEventCode;
	if ((avdQueueBuffer_p != STD_NULL) && (aubQueueSize != SNR_VALUE_0)) {
		aswErrCode = OS_GetFMemoryBlock(&avdMpf_p, aubQueueSize);
		/* Get Memory Block Result Check */
		if (OS_ERR_OK == aswErrCode) {
			wvdGen_MemCpy(avdMpf_p, avdQueueBuffer_p, aubQueueSize);
			astStdQueue.vdQueueBuffer_p = avdMpf_p;
			astStdQueue.uwQueueSize = aubQueueSize;
		} else {
			LOG_OUTPUT(LOG_APP_ID_SNR, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>aswErrCode:%d ", __FUNCTION__, __LINE__, aswErrCode);
			/* Can`t get memory, Ignore the message */
			aubReturnCode = RES_NG;
		}
	} else {
		astStdQueue.vdQueueBuffer_p = STD_NULL;
		astStdQueue.uwQueueSize = SNR_VALUE_0;
	}

	/* send msg to SENSOR task */
	if (aubReturnCode == RES_OK) {
		aswErrCode = OS_SendQueue(MBX_ID_SNR, OS_MAIL_PRI_NORMAL, &astStdQueue);
		/* Send msg Result Check */
		if (OS_ERR_OK == aswErrCode) {
			/* No Action */
			aubReturnCode = RES_OK;
		} else {
			LOG_OUTPUT(LOG_APP_ID_SNR, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>aswErrCode:%d ", __FUNCTION__, __LINE__, aswErrCode);
			/* Get Memory Block Check */
			if (avdMpf_p != STD_NULL) {
				(void)OS_ReleaseFMemoryBlock(avdMpf_p);
			} else {
				/* No Action */
				;
			}
			aubReturnCode = RES_NG;
		}
	}
	return;
}
/********************************************************************************************/
/**	\function		wuhSnr_GetTemp
 *	\date			2021/10/25
 *	\author			Guanjiafu
 *	\description	Get temp value
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		none
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			temp value
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/10/25		Guanjiafu		New
 ********************************************************************************************/
Type_uHWord wuhSnr_GetTemp(void)
{
	return nuhSnr_FanTempValue;
}
/********************************************************************************************/
/**	\function		wuhSnr_GetVsp
 *	\date			2021/10/25
 *	\author			Guanjiafu
 *	\description	Get Vsp value
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		none
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			vsp value
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/10/25		Guanjiafu		New
 ********************************************************************************************/
Type_uHWord wuhSnr_GetVsp(void)
{
	return nuhSnr_VspValue;
}
/********************************************************************************************/
/**	\function		wubSnr_GetILL
 *	\date			2021/10/25
 *	\author			Guanjiafu
 *	\description	Get ill value
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		none
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			ill value
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/10/25		Guanjiafu		New
 ********************************************************************************************/
Type_uByte wubSnr_GetILL(void)
{
	return nubSnr_IllmStatus;
}
/********************************************************************************************/
/**	\function		wubSnr_GetTelMute
 *	\date			2021/10/25
 *	\author			Guanjiafu
 *	\description	Get tel mute value
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		none
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			ill value
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/10/25		Guanjiafu		New
 ********************************************************************************************/
Type_uByte wubSnr_GetTelMute(void)
{
	return nubSnr_TelMuteStatus;
}

