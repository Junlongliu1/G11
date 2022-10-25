/*******************************************************************************************/
/**<!-------Copyright (C) 2021 Alpine Electronics, Inc. All rights reserved. --------------->
 * 
 * \file					Key_if.c
 * \date					2021/09/16
 * \author					Liyongnan
 * \model					EDA
 * \description				KEY API block
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
#include "HAL_ADCKey.h"
#include "HAL_I2CKey.h"
#include "HAL_GPIOKey.h"
#include "key_if.h"
#include "key_core.h"

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
/**	\function		wubKey_StandbyOut
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
Type_uByte wubKey_StandbyOut(void)
{
	LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d>", __FUNCTION__, __LINE__);
	nubKEY_State = KEY_STATUS_ON_NORMAL;//KEY_STATUS_ON_LINETOOL;
	nvbKEY_AdcHalHandle = HAL_Open(HAL_ADC_KEY, HAL_ADCKEY_OPTION_NORMAL);
	nvbKEY_I2cHalHandle = HAL_Open(HAL_I2C_KEY, HAL_I2CKEY_OPTION_NORMAL);
	nvbKEY_GpioHalHandle = HAL_Open(HAL_GPIO_KEY, HAL_GPIOKEY_OPTION_NORMAL);
	return RES_OK;
}

/********************************************************************************************/
/**	\function		wubKey_StandbyIn
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
Type_uByte wubKey_StandbyIn(void)
{
	LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d>", __FUNCTION__, __LINE__);
	HAL_Close(nvbKEY_AdcHalHandle);
	HAL_Close(nvbKEY_I2cHalHandle);
	HAL_Close(nvbKEY_GpioHalHandle);
	wvdKey_VariableInit();
	return RES_OK;
}

/********************************************************************************************/
/**	\function		wubKey_EnforceStandby
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
Type_uByte wubKey_EnforceStandby(void)
{
	// LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d>", __FUNCTION__, __LINE__);
	HAL_Close(nvbKEY_AdcHalHandle);
	HAL_Close(nvbKEY_I2cHalHandle);
	HAL_Close(nvbKEY_GpioHalHandle);
	wvdKey_VariableInit();
	return RES_OK;
}

/********************************************************************************************/
/**	\function		wubKey_RpcCallback
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
void wvdKey_RpcCallback(const Type_uByte aubsrc,const Type_uHWord auhEventCode, const void* avdQueueBuffer_p, const Type_uHWord aubQueueSize)

{
	LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d>", __FUNCTION__, __LINE__);
	OS_StdQueue_ts astStdQueue;
	OS_VoidPtr_t avdMpf_p;
	OS_ErrCode_t aswErrCode;
 	Type_uByte aubReturnCode;
	
	/* Variable Initialize */
	aubReturnCode	= RES_OK;
	aswErrCode = OS_ERR_OK;
	avdMpf_p = STD_NULL;
	OSAL_MemSet(&astStdQueue, KEY_VALUE_0, sizeof(astStdQueue));
	
	astStdQueue.swSrcTaskId = aubsrc;
	astStdQueue.uhEventCode = auhEventCode;
	if((avdQueueBuffer_p != STD_NULL) && (aubQueueSize != KEY_VALUE_0)) {
		aswErrCode = OS_GetFMemoryBlock(&avdMpf_p, aubQueueSize);
		/* Get Memory Block Result Check */
		if (OS_ERR_OK == aswErrCode) {
			wvdGen_MemCpy(avdMpf_p, avdQueueBuffer_p, aubQueueSize);
			astStdQueue.vdQueueBuffer_p = avdMpf_p;
			astStdQueue.uwQueueSize = aubQueueSize;
		} else {
			/* Can`t get memory, Ignore the message */
			LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d> aswErrCode:%d", __FUNCTION__, __LINE__, aswErrCode);
			aubReturnCode = RES_NG;
		}
	} else {
		LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d> avdQueueBuffer_p NULL", __FUNCTION__, __LINE__);
		astStdQueue.vdQueueBuffer_p = STD_NULL;
		astStdQueue.uwQueueSize = KEY_VALUE_0;
	}


	/* send msg to KEY task */
	if (aubReturnCode == RES_OK) {
		aswErrCode = OS_SendQueue(MBX_ID_KEY, OS_MAIL_PRI_NORMAL, &astStdQueue);
		/* Send Msg Result Check */
		if (OS_ERR_OK == aswErrCode) {
			/* No Action */
			aubReturnCode = RES_OK;
		} else {
			/* Get Memory Block Check */
			LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d> OS_SendQueue:%d", __FUNCTION__, __LINE__, aswErrCode);
			if (avdMpf_p != STD_NULL) {
				aswErrCode = OS_ReleaseFMemoryBlock(avdMpf_p);
				if (OS_ERR_OK != aswErrCode) {
					LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d> OS_ReleaseFMemory:%d", __FUNCTION__, __LINE__, aswErrCode);
				} else {
					;
				}
			} else {
				/* No Action */
				;
			}
			aubReturnCode = RES_NG;
		}
	}
	return;
}

