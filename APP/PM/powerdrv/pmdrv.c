/********************************************************************************************/
/* File Name	: pm.c																		*/
/* Date			: 2021/11/12																*/
/* Author		: wangying																	*/
/* Model		: EDA																		*/
/* Description	: power driver																*/
/* Copyright (c)  2020 Alpine Electronics (China) Co. LTD.  All Rights Reserved				*/
/*------------------------------------------------------------------------------------------*/
/* Revision History																			*/
/* No		Date		Revised by		Function Name										*/
/* 																							*/
/*------------------------------------------------------------------------------------------*/
/********************************************************************************************/

/********************************************************************************************/
/*							Include File Section										*/
/********************************************************************************************/

#include "apn_basic_type.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "os_framework_type.h"
#include "os_framework.h"
#include "tcc_timer.h"
#include <bsp.h>
#include <bsp_int.h>

#include "pm.h"
#include "pmdrv.h"
#include "gpio.h"
#include "pm_pin_define.h"
#include <pwm.h>
#include "rpc_if.h"
#include "rpc_event.h"
#include "rpc_data.h"
#include "HAL_PowerManager.h"
#include <HAL_devices.h>
#include "pm_gpio.h"
#include "Eep_core.h"
#include "logapp.h"


/********************************************************************************************/
/*							Macro Definition Section										*/
/********************************************************************************************/
#define PM_DRV_ARPC_ID_ANY			(0xFFU)

#define PM_DRV_PORT_MAX_COUNT		(9U)
#define PM_DRV_PORT_STATUS_NONE		(0xFFU)	
#define PM_DRV_PORT_STATUS_HIGH		GPIO_HIGH
#define PM_DRV_PORT_STATUS_LOW		GPIO_LOW
#define PM_DRV_PORT_STATUS_ANY		(0xFEU)

#define PM_DRV_PIN_SET_DATA_SIZE	(2U)

#define PM_DRV_PWM_PORT_NUM			(94U)
#define PM_DRV_PWM_PERIOD_NS		(13800000)
#define PM_DRV_PWM_DUTY_NS			(PM_DRV_PWM_PERIOD_NS/2)

#define PM_DRV_BATT_CHECK_TIME		(1000U)

#define PM_DRV_DATA_ZERO			(0x00U)
#define PM_DRV_DATA_ONE				(0x01U)

#define PM_DRV_PIN_SET_LINE_MAX		(2U)
#define PM_DRV_PIN_SET_ROW_MAX		(6U)
#define PM_DRV_PIN_SET_LINE_ON		(0U)
#define PM_DRV_PIN_SET_LINE_OFF		(1U)

#define PM_DRV_PIN_TIME_5			(5U)
#define PM_DRV_PIN_TIME_10			(10U)
#define PM_DRV_PIN_TIME_15			(15U)
#define PM_DRV_PIN_TIME_16			(16U)
#define PM_DRV_PIN_TIME_35			(35U)
#define PM_DRV_PIN_TIME_90			(90U)


/********************************************************************************************/
/*							Function Definition Section									*/
/********************************************************************************************/
extern void wubKlineDrv_Close(void);

static void nvdPmDrvInit(void);
static void nvdPmDrvPortSetup(void);
static Type_uByte nvdPmDrvPortUpdate(void);
static void nvdPmDrvRpcRcv(const Type_uByte aubsrc, const Type_uHWord auhCmd, void *avdData_p, const Type_uHWord auhLength);
static void nvdPmDrvSendMsg(Type_uByte aubReceiver, Type_uHWord auhEvent, void * auvData_p, Type_uHWord auhLength);
static void nvdPmDrvProcCtrl(void);

static void nvdPmDrvDetSet(OS_StdQueue_ts *astMessage_p);
static void nvdPmDrvStandbyOut(OS_StdQueue_ts *astMessage_p);
static void nvdPmDrvStandbyIn(OS_StdQueue_ts *astMessage_p);


/********************************************************************************************/
/*							ROM Table Section												*/
/********************************************************************************************/
#if (TEMP_COMPILE_SWITCH_05WORK_10WORK == TEMP_COMPILE_SWITCH_05WORK)
static const PmDrvPowerEvtCtrl_ts nstPmDrvPowerEvt_Tbl[] = 
{
	/*	A-DET 					H-DET						Event	*/
	{PM_DRV_PORT_STATUS_HIGH,	PM_DRV_PORT_STATUS_LOW,		EV_INF_PM_STANDBYOUT_START},
	{PM_DRV_PORT_STATUS_LOW,	PM_DRV_PORT_STATUS_ANY,		EV_INF_PM_STANDBYIN_START},
	{PM_DRV_PORT_STATUS_ANY,	PM_DRV_PORT_STATUS_HIGH,	EV_INF_PM_STANDBYIN_START},
};
#else
static const PmDrvPowerEvtCtrl_ts nstPmDrvPowerEvt_Tbl[] = 
{
	/*	A-DET 					H-DET						Event	*/
	{PM_DRV_PORT_STATUS_HIGH,	PM_DRV_PORT_STATUS_HIGH,	EV_INF_PM_STANDBYOUT_START},
	{PM_DRV_PORT_STATUS_LOW,	PM_DRV_PORT_STATUS_ANY,		EV_INF_PM_STANDBYIN_START},
	{PM_DRV_PORT_STATUS_ANY,	PM_DRV_PORT_STATUS_LOW,		EV_INF_PM_STANDBYIN_START},
};

#endif

static const PmDrvProcCtrl_ts nstPmDrvProcCtrl_Tbl[] = 
{
	{ARPC_ID_R5_POWERDRV,	EV_INF_STANDBYOUT_NOTIFY,		nvdPmDrvStandbyOut},
	{ARPC_ID_R5_POWERDRV,	EV_INF_STANDBYIN_NOTIFY,		nvdPmDrvStandbyIn},
	{PM_DRV_ARPC_ID_ANY,	EV_INF_GPIO_LEVEL_SET_REQ,		nvdPmDrvDetSet},
};

/********************************************************************************************/
/*							Static Variable Definition Section								*/
/********************************************************************************************/
static PmDrvPortInfo_ts nstPmDrvPortMng[PM_DRV_PORT_NUM];
static Type_uByte nubBattWait = FALSE;

/********************************************************************************************/
/********************************************************************************************/
/**	\function		TSK_POWERDRV
 *	\date			2021/11/12
 *	\author			wangying
 *	\description	main task
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
void TSK_POWERDRV(void)
{
	OS_ErrCode_t 	auwRst;
	OS_StdQueue_ts 	astMsg; 
	OS_StdQueue_ts * astMsg_p = &astMsg;
	Type_sByte 		asbRet;
	Type_uByte 		aubIndex = PM_DRV_DATA_ZERO;
	Type_uByte 		aubFlag = FALSE;
	
	nvdPmDrvInit();
	
	asbRet = wsbRPCInit(ARPC_ID_R5_POWERDRV, (wvdRPCCallback)nvdPmDrvRpcRcv);
	if(asbRet != RPC_OK){
		PMDRV_DBG("\n######## PMDRV RPC Init Failed:%d#######\n", asbRet);
		LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL,ATAT_LEVEL_OFF,"[%s]RPC Init Failed:%d",__func__, asbRet);
	}
	
	auwRst = OS_StartTimer(OS_TIMER_ID_PMDRVCYC);
	if(auwRst != OS_ERR_OK){
		PMDRV_DBG("########## PMDRV Timer Start Failed:%d ##########\n", auwRst);
		LOG_OUTPUT(LOG_APP_ID_PMDR, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]Timer Start Failed:%d",__func__, auwRst);
	}

	for( ; ; )
	{		
		auwRst = OS_ReceiveQueue(MBX_ID_POWERDRV, OS_TOUT_FOREVER, &astMsg_p);
		
		if (auwRst == OS_ERR_OK)
		{
			for(aubIndex = PM_DRV_DATA_ZERO; aubIndex < (sizeof(nstPmDrvProcCtrl_Tbl)/sizeof(PmDrvProcCtrl_ts)); aubIndex++){
				if(((nstPmDrvProcCtrl_Tbl[aubIndex].ubModuleID == PM_DRV_ARPC_ID_ANY) || (nstPmDrvProcCtrl_Tbl[aubIndex].ubModuleID == astMsg.swSrcTaskId))
					&& (nstPmDrvProcCtrl_Tbl[aubIndex].uhEvent == astMsg.uhEventCode)){
						if(nstPmDrvProcCtrl_Tbl[aubIndex].proc_fp != NULL){
							nstPmDrvProcCtrl_Tbl[aubIndex].proc_fp(astMsg_p);
						}
						aubFlag = TRUE;
						break;
					}
			}

			if(aubFlag == FALSE){
				PMDRV_DBG("\n######## PMDRV Invalid message receive:%x-%x ########\n", astMsg.swSrcTaskId, astMsg.uhEventCode);
				LOG_OUTPUT(LOG_APP_ID_PMDR, DEBUGLOG_LEVEL_ERROR,ATAT_LEVEL_OFF,"[%s]Invalid event:%x src:%x",__func__, astMsg.uhEventCode, astMsg.swSrcTaskId);
			}
	
			if((astMsg.uwQueueSize > PM_DRV_DATA_ZERO) && (NULL != astMsg.vdQueueBuffer_p))
			{
				auwRst = OS_ReleaseFMemoryBlock(astMsg.vdQueueBuffer_p);
				if (auwRst != OS_ERR_OK){
					PMDRV_DBG("\n######## PMDRV Failed to release memory:%d#######\n", auwRst);
					LOG_OUTPUT(LOG_APP_ID_PMDR, DEBUGLOG_LEVEL_FALAL,ATAT_LEVEL_OFF,"[%s]Failed to release memory:%d",__func__, auwRst);
				}
			}
		}

	}
 
}

/********************************************************************************************/
/**	\function		wubPmDrvBattStatusChk
 *	\date			2021/11/12
 *	\author			wangying
 *	\description	check B-DET status
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
 #ifdef PM_HAL_BEFORE_OS
Type_uByte wubPmDrvBattStatusChk(void)
{
	Type_uByte aubStatus = GPIO_LOW;
	Type_uByte aubRet = RES_OK;
	HAL_HANDLE avdHalHandle = NULL;
	Type_uByte aubBattPin = HAL_POWERPIN_BATT;	
	Type_sWord aswHalRet = RES_OK;

	avdHalHandle = wvdPmHalHandleGet();
	if(avdHalHandle == NULL){
		PMDRV_DBG("\n######### PMDRV Failed to get hal handle ######### \n");
		wvdEEPLOGWrite( TYPE_PMDRV, PM_EEP_ERROR_DRV_HALHANDLE );
		return aubRet;
	}
	
	aswHalRet = HAL_Ioctl(avdHalHandle, HAL_IO_PM_GET_POWERPIN_STATUS, &aubBattPin, &aubStatus, 0, 0);
	if(HAL_OK != aswHalRet){
		PMDRV_DBG("\n######## PMDRV Fail to HAL_Ioctl（GET_POWERPIN_STATUS）:%d#######\n", aswHalRet);
		wvdEEPLOGWrite( TYPE_PMDRV, PM_EEP_ERROR_DRV_HALGETBAT );
	}

	if(aubStatus == GPIO_LOW){
		BSP_IntSrcEn(INT_TIMER_0 + PMDRV_TIMER_CH);
		aswHalRet = tcc_timer_enable(PMDRV_TIMER_CH, PM_DRV_BATT_CHECK_TIME, (timer_handler_t)wswPmDrvBattCheckWait, 0);
		if(0 != aswHalRet){
			PMDRV_DBG("\n######## PMDRV Fail to Start hand timer:%d#######\n", aswHalRet);
			wvdEEPLOGWrite( TYPE_PMDRV, PM_EEP_ERROR_DRV_BATTIMER );
		}

		while(nubBattWait == RES_OK){
			NOP_DELAY();
		}
		
		aswHalRet = HAL_Ioctl(avdHalHandle, HAL_IO_PM_GET_POWERPIN_STATUS, &aubBattPin, &aubStatus, 0, 0);
		if(HAL_OK != aswHalRet){
			PMDRV_DBG("\n######## PMDRV Fail to HAL_Ioctl（GET_POWERPIN_STATUS）:%d#######\n", aswHalRet);
			wvdEEPLOGWrite( TYPE_PMDRV, PM_EEP_ERROR_DRV_HALGETBAT );
		}

		if(aubStatus == GPIO_LOW){
			aubRet = RES_NG;
		}
	}

	return aubRet;
}

#else
Type_uByte wubPmDrvBattStatusChk(void)
{
	Type_uByte aubStatus = PM_GPIO_STS_NG;
	Type_uByte aubRet = RES_OK;

	aubStatus = wubSysPowerGetPinStstus(SYS_POWERPIN_BATT);

	if(aubStatus == GPIO_LOW){
		OS_WaitTime(PM_DRV_BATT_CHECK_TIME);
		aubStatus = wubSysPowerGetPinStstus(SYS_POWERPIN_BATT);
		if(aubStatus == GPIO_LOW){
			aubRet = RES_NG;
		}
	}

	return aubRet;
}
#endif

/********************************************************************************************/
/**	\function		wswPmDrvBattCheckWait
 *	\date			2021/11/12
 *	\author			wangying
 *	\description	B-DET check wait
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
Type_sWord wswPmDrvBattCheckWait(Type_sWord dummy, void* pDummy)
{
	tcc_timer_disable(PMDRV_TIMER_CH);
	nubBattWait = TRUE;

	return TRUE;
}

/********************************************************************************************/
/**	\function		wubPmDrvStandbyOut
 *	\date			2021/11/12
 *	\author			StanbyOut
 *	\description	StandbyOut
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
Type_uByte wubPmDrvStandbyOut(void)
{
	LOG_OUTPUT(LOG_APP_ID_PMDR, DEBUGLOG_LEVEL_INFO,ATAT_LEVEL_OFF,"[%s]StandbyOut receive",__func__);
	
	nvdPmDrvSendMsg(ARPC_ID_R5_POWERDRV, EV_INF_STANDBYOUT_NOTIFY, NULL, PM_DRV_DATA_ZERO);

	return RES_OK;
}

/********************************************************************************************/
/**	\function		wubPmDrvStandbyIn
 *	\date			2021/11/12
 *	\author			StanbyOut
 *	\description	StandbyIn
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
Type_uByte wubPmDrvStandbyIn(void)
{
	LOG_OUTPUT(LOG_APP_ID_PMDR, DEBUGLOG_LEVEL_INFO,ATAT_LEVEL_OFF,"[%s]StandbyIn receive",__func__);
	
	nvdPmDrvSendMsg(ARPC_ID_R5_POWERDRV, EV_INF_STANDBYIN_NOTIFY, NULL, PM_DRV_DATA_ZERO);

	return RES_OK;
}

/********************************************************************************************/
/**	\function		wubPmDrvForceStandby
 *	\date			2021/11/12
 *	\author			StanbyOut
 *	\description	TASK Init
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
Type_uByte wubPmDrvForceStandby(void)
{
#if (TEMP_COMPILE_SWITCH_05WORK_10WORK != TEMP_COMPILE_SWITCH_05WORK)
	gpio_set(PM_A_MUTE,GPIO_LOW);
	gpio_set(PM_AMP_STBY,GPIO_LOW);
	gpio_set(PM_PANT_CONT,GPIO_LOW);
	gpio_set(PM_LCD_33V_CONT,GPIO_LOW);
	gpio_set(PM_TP_33V_CONT,GPIO_LOW);
	gpio_set(PM_M5V_CONT,GPIO_LOW);
	gpio_set(PM_VIDEO_DEC_12V_CONT,GPIO_LOW);
	gpio_set(PM_VIDEO_DEC_33V_CONT,GPIO_LOW);
	gpio_set(PM_GNSS_CONT,GPIO_LOW);	
	gpio_set(PM_LWC_CONT,GPIO_LOW);
	gpio_set(PM_RCV_CONT,GPIO_LOW);
	gpio_set(PM_K_LINE_TX,GPIO_LOW);
	gpio_set(PM_RCV_RESET,GPIO_LOW);
#endif

	wvdPmGpioClear();

	return RES_OK;
}

/********************************************************************************************/
/**	\function		nvdPmDrvInit
 *	\date			2021/11/12
 *	\author			wangying
 *	\description	TASK Init
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
void wvdPmDrvRequestCurrentEvent(void)
{
	nvdPmDrvProcCtrl();
}

/********************************************************************************************/
/**	\function		wvdPmDrvCycCtrl
 *	\date			2021/11/12
 *	\author			wangying
 *	\description	CYC Ctrl
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
void wvdPmDrvCycCtrl(void)
{
	Type_uByte aubChange = FALSE;
	
#ifdef PM_ST_EN
	if(wvdPmStNoGet()  == PM_ST_NO_6420){
		gpio_set(PM_A2_NC, GPIO_HIGH);
	}
#endif

	aubChange = nvdPmDrvPortUpdate();
	if(aubChange == TRUE){
		nvdPmDrvProcCtrl();
	}

#ifdef PM_ST_EN
	if(wvdPmStNoGet()  == PM_ST_NO_6420){
		gpio_set(PM_A2_NC, GPIO_LOW);
	}
#endif

}

/********************************************************************************************/
/**	\function		wubPmDrvGetStandbyDetSts
 *	\date			2021/11/12
 *	\author			wangying
 *	\description	Standby Det Status Get
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
OS_EventCode_t  wubPmDrvGetStandbyDetSts(void)
{
	Type_uByte aubIndex = PM_DRV_DATA_ZERO;
	OS_EventCode_t  aubSts = EV_INF_PM_STANDBYOUT_START;

	for(aubIndex = PM_DRV_DATA_ZERO; aubIndex < (sizeof(nstPmDrvPowerEvt_Tbl)/sizeof(PmDrvPowerEvtCtrl_ts));aubIndex++){
		if(((nstPmDrvPowerEvt_Tbl[aubIndex].ubAdetStatus == PM_DRV_PORT_STATUS_ANY) || (nstPmDrvPortMng[PM_DRV_PORT_ADET_INDEX].ubFilteredCurrent == nstPmDrvPowerEvt_Tbl[aubIndex].ubAdetStatus))
			&&((nstPmDrvPowerEvt_Tbl[aubIndex].ubHdetStatus == PM_DRV_PORT_STATUS_ANY) || (nstPmDrvPortMng[PM_DRV_PORT_HDET_INDEX].ubFilteredCurrent == nstPmDrvPowerEvt_Tbl[aubIndex].ubHdetStatus))){
			PMDRV_DBG("\n******** PMDRV A-DET is %d, H-DET is %d\n", nstPmDrvPortMng[PM_DRV_PORT_ADET_INDEX].ubFilteredCurrent, nstPmDrvPortMng[PM_DRV_PORT_HDET_INDEX].ubFilteredCurrent);
			LOG_OUTPUT(LOG_APP_ID_PMDR, DEBUGLOG_LEVEL_FALAL,ATAT_LEVEL_OFF,"[%s]A-DET:%d, H-DET:%d",__func__, nstPmDrvPortMng[PM_DRV_PORT_ADET_INDEX].ubFilteredCurrent, nstPmDrvPortMng[PM_DRV_PORT_HDET_INDEX].ubFilteredCurrent);
			aubSts = nstPmDrvPowerEvt_Tbl[aubIndex].uhEvent;
			break;
		}
	}

	return aubSts;
}

/********************************************************************************************/
/**	\function		nvdPmDrvInit
 *	\date			2021/11/12
 *	\author			wangying
 *	\description	TASK Init
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
static void nvdPmDrvInit(void)
{
	nvdPmDrvPortSetup();
}

/********************************************************************************************/
/**	\function		nvdPmDrvPortSetup
 *	\date			2021/11/12
 *	\author			wangying
 *	\description	Port Setup
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
static void nvdPmDrvPortSetup(void)
{
	HAL_HANDLE avdHalHandle = NULL;
	Type_sWord aswRet = HAL_OK;

	avdHalHandle = wvdPmHalHandleGet();
	if(avdHalHandle == NULL){
		PMDRV_DBG("\n######### PMDRV Failed to get hal handle ######### \n");
		LOG_OUTPUT(LOG_APP_ID_PMDR, DEBUGLOG_LEVEL_ERROR,ATAT_LEVEL_OFF,"[%s]Hal handle is null",__func__);
		return;
	}
	
	nstPmDrvPortMng[PM_DRV_PORT_ADET_INDEX].ubPort = HAL_POWERPIN_ACC;
	aswRet = HAL_Ioctl(avdHalHandle, HAL_IO_PM_GET_POWERPIN_STATUS, &nstPmDrvPortMng[PM_DRV_PORT_ADET_INDEX].ubPort, &nstPmDrvPortMng[PM_DRV_PORT_ADET_INDEX].ubRawLast, 0, 0);
	if(HAL_OK != aswRet){
		PM_DBG("\n######## PMDRV Fail to HAL_Ioctl（ADET）:%d#######\n", aswRet);
		LOG_OUTPUT(LOG_APP_ID_PMDR, DEBUGLOG_LEVEL_FALAL,ATAT_LEVEL_OFF,"[%s]Failed to get A-DET",__func__);
	}
	nstPmDrvPortMng[PM_DRV_PORT_ADET_INDEX].ubRawCurrent = PM_DRV_PORT_STATUS_NONE;
	nstPmDrvPortMng[PM_DRV_PORT_ADET_INDEX].ubFilteredLast = PM_DRV_PORT_STATUS_NONE;
	nstPmDrvPortMng[PM_DRV_PORT_ADET_INDEX].ubFilteredCurrent = PM_DRV_PORT_STATUS_NONE;
	nstPmDrvPortMng[PM_DRV_PORT_ADET_INDEX].ubCount = PM_DRV_DATA_ONE;

	nstPmDrvPortMng[PM_DRV_PORT_HDET_INDEX].ubPort = HAL_POWERPIN_HI;
	aswRet = HAL_Ioctl(avdHalHandle, HAL_IO_PM_GET_POWERPIN_STATUS, &nstPmDrvPortMng[PM_DRV_PORT_HDET_INDEX].ubPort, &nstPmDrvPortMng[PM_DRV_PORT_HDET_INDEX].ubRawLast, 0, 0);
	if(HAL_OK != aswRet){
		PM_DBG("\n######## PMDRV Fail to HAL_Ioctl（HDET）:%d#######\n", aswRet);
		LOG_OUTPUT(LOG_APP_ID_PMDR, DEBUGLOG_LEVEL_FALAL,ATAT_LEVEL_OFF,"[%s]Failed to get H-DET",__func__);
	}
	nstPmDrvPortMng[PM_DRV_PORT_HDET_INDEX].ubRawCurrent = PM_DRV_PORT_STATUS_NONE;
	nstPmDrvPortMng[PM_DRV_PORT_HDET_INDEX].ubFilteredLast = PM_DRV_PORT_STATUS_NONE;
	nstPmDrvPortMng[PM_DRV_PORT_HDET_INDEX].ubFilteredCurrent = PM_DRV_PORT_STATUS_NONE;
	nstPmDrvPortMng[PM_DRV_PORT_HDET_INDEX].ubCount = PM_DRV_DATA_ONE;
}

/********************************************************************************************/
/**	\function		nvdPmDrvPortUpdate
 *	\date			2021/11/12
 *	\author			wangying
 *	\description	Port Update
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
static Type_uByte nvdPmDrvPortUpdate(void)
{
	Type_uByte aubIndex = PM_DRV_DATA_ZERO;
	Type_uByte aubChange = FALSE;
	HAL_HANDLE avdHalHandle = NULL;
	Type_sWord aswRet = HAL_OK;

	avdHalHandle = wvdPmHalHandleGet();
	if(avdHalHandle == NULL){
		PMDRV_DBG("\n######### PMDRV Failed to get hal handle ######### \n");
		LOG_OUTPUT(LOG_APP_ID_PMDR, DEBUGLOG_LEVEL_FALAL,ATAT_LEVEL_OFF,"[%s]Hal handle is null",__func__);
		return aubChange;
	}

	for(aubIndex = PM_DRV_DATA_ZERO; aubIndex < PM_DRV_PORT_NUM; aubIndex++){
		nstPmDrvPortMng[aubIndex].ubRawLast = nstPmDrvPortMng[aubIndex].ubRawCurrent;
		aswRet = HAL_Ioctl(avdHalHandle, HAL_IO_PM_GET_POWERPIN_STATUS, &nstPmDrvPortMng[aubIndex].ubPort, &nstPmDrvPortMng[aubIndex].ubRawCurrent, 0, 0);
		if(HAL_OK != aswRet){
			PM_DBG("\n######## PMDRV Fail to HAL_Ioctl（POWERPIN_STATUS）:%d#######\n", aswRet);
			LOG_OUTPUT(LOG_APP_ID_PMDR, DEBUGLOG_LEVEL_FALAL,ATAT_LEVEL_OFF,"[%s]Failed to get pin status:%d",__func__, aubIndex);
		}

		if(nstPmDrvPortMng[aubIndex].ubRawCurrent == nstPmDrvPortMng[aubIndex].ubRawLast){
			if(nstPmDrvPortMng[aubIndex].ubCount < (PM_DRV_PORT_MAX_COUNT -1)){
				nstPmDrvPortMng[aubIndex].ubCount++;
#ifdef PM_ST_EN
				if((wvdPmStNoGet() == PM_ST_NO_5320) && (aubIndex == PM_DRV_PORT_ADET_INDEX) && (nstPmDrvPortMng[aubIndex].ubRawCurrent == GPIO_HIGH)){
					gpio_set(PM_A2_NC, GPIO_HIGH);
					gpio_set(PM_A2_NC, GPIO_LOW);
					//printf("\n@@@@@[%s] ST NO is %x \n", __func__, wvdPmStNoGet());
				}
				else if((wvdPmStNoGet() == PM_ST_NO_5321) && (aubIndex == PM_DRV_PORT_HDET_INDEX) && (nstPmDrvPortMng[aubIndex].ubRawCurrent == GPIO_HIGH)){
					gpio_set(PM_A2_NC, GPIO_HIGH);
					gpio_set(PM_A2_NC, GPIO_LOW);
					//printf("\n@@@@@[%s] ST NO is %x \n", __func__, wvdPmStNoGet());
				}
				else if((wvdPmStNoGet() == PM_ST_NO_5322) && (aubIndex == PM_DRV_PORT_ADET_INDEX) && (nstPmDrvPortMng[aubIndex].ubRawCurrent == GPIO_LOW)){
					gpio_set(PM_A2_NC, GPIO_HIGH);
					gpio_set(PM_A2_NC, GPIO_LOW);
					//printf("\n@@@@@[%s] ST NO is %x \n", __func__, wvdPmStNoGet());
				}
				else if((wvdPmStNoGet() == PM_ST_NO_5323) && (aubIndex == PM_DRV_PORT_HDET_INDEX) && (nstPmDrvPortMng[aubIndex].ubRawCurrent == GPIO_LOW)){
					gpio_set(PM_A2_NC, GPIO_HIGH);
					gpio_set(PM_A2_NC, GPIO_LOW);
					//printf("\n@@@@@[%s] ST NO is %x \n", __func__, wvdPmStNoGet());
				}
				else{
				}
#endif
			}
			else{
				nstPmDrvPortMng[aubIndex].ubFilteredCurrent = nstPmDrvPortMng[aubIndex].ubRawCurrent;
				if(nstPmDrvPortMng[aubIndex].ubFilteredCurrent != nstPmDrvPortMng[aubIndex].ubFilteredLast){
					nstPmDrvPortMng[aubIndex].ubFilteredLast = nstPmDrvPortMng[aubIndex].ubFilteredCurrent;
					aubChange = TRUE;
#ifdef PM_ST_EN
					if((wvdPmStNoGet() == PM_ST_NO_5320) && (aubIndex == PM_DRV_PORT_ADET_INDEX) && (nstPmDrvPortMng[aubIndex].ubRawCurrent == GPIO_HIGH)){
						gpio_set(PM_A2_NC, GPIO_HIGH);
						gpio_set(PM_A2_NC, GPIO_LOW);
						//printf("\n@@@@@[%s] ST NO is %x \n", __func__, wvdPmStNoGet());
					}
					else if((wvdPmStNoGet() == PM_ST_NO_5321) && (aubIndex == PM_DRV_PORT_HDET_INDEX) && (nstPmDrvPortMng[aubIndex].ubRawCurrent == GPIO_HIGH)){
						gpio_set(PM_A2_NC, GPIO_HIGH);
						gpio_set(PM_A2_NC, GPIO_LOW);
						//printf("\n@@@@@[%s] ST NO is %x \n", __func__, wvdPmStNoGet());
					}
					else if((wvdPmStNoGet() == PM_ST_NO_5322) && (aubIndex == PM_DRV_PORT_ADET_INDEX) && (nstPmDrvPortMng[aubIndex].ubRawCurrent == GPIO_LOW)){
						gpio_set(PM_A2_NC, GPIO_HIGH);
						gpio_set(PM_A2_NC, GPIO_LOW);
						//printf("\n@@@@@[%s] ST NO is %x \n", __func__, wvdPmStNoGet());
					}
					else if((wvdPmStNoGet() == PM_ST_NO_5323) && (aubIndex == PM_DRV_PORT_HDET_INDEX) && (nstPmDrvPortMng[aubIndex].ubRawCurrent == GPIO_LOW)){
						gpio_set(PM_A2_NC, GPIO_HIGH);
						gpio_set(PM_A2_NC, GPIO_LOW);
						//printf("\n@@@@@[%s] ST NO is %x \n", __func__, wvdPmStNoGet());
					}
					else{
					}
#endif

				}
			}
		}
		else{
			nstPmDrvPortMng[aubIndex].ubCount = PM_DRV_DATA_ONE;
#ifdef PM_ST_EN
			if((wvdPmStNoGet() == PM_ST_NO_5320) && (aubIndex == PM_DRV_PORT_ADET_INDEX) && (nstPmDrvPortMng[aubIndex].ubRawCurrent == GPIO_HIGH)){
				gpio_set(PM_A2_NC, GPIO_HIGH);
				gpio_set(PM_A2_NC, GPIO_LOW);
				//printf("\n@@@@@[%s] ST NO is %x \n", __func__, wvdPmStNoGet());
			}
			else if((wvdPmStNoGet() == PM_ST_NO_5321) && (aubIndex == PM_DRV_PORT_HDET_INDEX) && (nstPmDrvPortMng[aubIndex].ubRawCurrent == GPIO_HIGH)){
				gpio_set(PM_A2_NC, GPIO_HIGH);
				gpio_set(PM_A2_NC, GPIO_LOW);
				//printf("\n@@@@@[%s] ST NO is %x \n", __func__, wvdPmStNoGet());
			}
			else if((wvdPmStNoGet() == PM_ST_NO_5322) && (aubIndex == PM_DRV_PORT_ADET_INDEX) && (nstPmDrvPortMng[aubIndex].ubRawCurrent == GPIO_LOW)){
				gpio_set(PM_A2_NC, GPIO_HIGH);
				gpio_set(PM_A2_NC, GPIO_LOW);
				//printf("\n@@@@@[%s] ST NO is %x \n", __func__, wvdPmStNoGet());
			}
			else if((wvdPmStNoGet() == PM_ST_NO_5323) && (aubIndex == PM_DRV_PORT_HDET_INDEX) && (nstPmDrvPortMng[aubIndex].ubRawCurrent == GPIO_LOW)){
				gpio_set(PM_A2_NC, GPIO_HIGH);
				gpio_set(PM_A2_NC, GPIO_LOW);
				//printf("\n@@@@@[%s] ST NO is %x \n", __func__, wvdPmStNoGet());
			}
			else{
			}
#endif

		}
	}

	return aubChange;

}

/********************************************************************************************/
/**	\function		nvdPmDrvProcCtrl
 *	\date			2021/11/12
 *	\author			wangying
 *	\description	Process Ctrl
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
static void nvdPmDrvProcCtrl(void)
{
	Type_uByte aubIndex = PM_DRV_DATA_ZERO;;

	for(aubIndex = PM_DRV_DATA_ZERO; aubIndex < (sizeof(nstPmDrvPowerEvt_Tbl)/sizeof(PmDrvPowerEvtCtrl_ts));aubIndex++){
		if(((nstPmDrvPowerEvt_Tbl[aubIndex].ubAdetStatus == PM_DRV_PORT_STATUS_ANY) || (nstPmDrvPortMng[PM_DRV_PORT_ADET_INDEX].ubFilteredCurrent == nstPmDrvPowerEvt_Tbl[aubIndex].ubAdetStatus))
			&&((nstPmDrvPowerEvt_Tbl[aubIndex].ubHdetStatus == PM_DRV_PORT_STATUS_ANY) || (nstPmDrvPortMng[PM_DRV_PORT_HDET_INDEX].ubFilteredCurrent == nstPmDrvPowerEvt_Tbl[aubIndex].ubHdetStatus))){
			PMDRV_DBG("\n******** PMDRV A-DET is %d, H-DET is %d Evert is %x\n", nstPmDrvPortMng[PM_DRV_PORT_ADET_INDEX].ubFilteredCurrent, nstPmDrvPortMng[PM_DRV_PORT_HDET_INDEX].ubFilteredCurrent,nstPmDrvPowerEvt_Tbl[aubIndex].uhEvent);
			LOG_OUTPUT(LOG_APP_ID_PMDR, DEBUGLOG_LEVEL_FALAL,ATAT_LEVEL_OFF,"[%s]A-DET:%d, H-DET:%d",__func__, nstPmDrvPortMng[PM_DRV_PORT_ADET_INDEX].ubFilteredCurrent, nstPmDrvPortMng[PM_DRV_PORT_HDET_INDEX].ubFilteredCurrent);
			nvdPmDrvSendMsg(ARPC_ID_R5_POWERMNG, nstPmDrvPowerEvt_Tbl[aubIndex].uhEvent, NULL, PM_DRV_DATA_ZERO);
			break;
		}
	}
}

/********************************************************************************************/
/**	\function		wvdPmDrvRpcRcv
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	RPC Reive CallBack
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
static void nvdPmDrvRpcRcv(const Type_uByte aubsrc, const Type_uHWord auhCmd, void *avdData_p, const Type_uHWord auhLength)
{
	OS_ErrCode_t auwExeRslt = OS_ERR_TMOUT;
	OS_ErrCode_t auwGetRslt = OS_ERR_NOMEM;		
	OS_VoidPtr_t avdMsgData_p = NULL;
	OS_StdQueue_ts astMsg;

	wvdGen_MemSet(&astMsg, PM_DRV_DATA_ZERO, sizeof(OS_StdQueue_ts));
	astMsg.swSrcTaskId = aubsrc;
	astMsg.uhEventCode = auhCmd;
	
	if((avdData_p != NULL) && (auhLength > PM_DRV_DATA_ZERO)){
		auwGetRslt = OS_GetFMemoryBlock(&avdMsgData_p, auhLength);
		if(auwGetRslt == OS_ERR_OK)
		{
			wvdGen_MemSet(avdMsgData_p, PM_DRV_DATA_ZERO, auhLength);
			wvdGen_MemCpy(avdMsgData_p , avdData_p, auhLength);
			astMsg.vdQueueBuffer_p = avdMsgData_p ;
			astMsg.uwQueueSize = auhLength;	
		}
		else{
			PMDRV_DBG("\n######## PMDRV Failed to Get Memory #########");
			LOG_OUTPUT(LOG_APP_ID_PMDR, DEBUGLOG_LEVEL_FALAL,ATAT_LEVEL_OFF,"[%s]Failed to get memory",__func__);
		}
	}
		
	auwExeRslt = OS_SendQueue(MBX_ID_POWERDRV, OS_MAIL_PRI_NORMAL, &astMsg); 
	if(auwExeRslt != OS_ERR_OK){
		PMDRV_DBG("\n########## PMDRV Faild to Send Queue ##########\n");
		LOG_OUTPUT(LOG_APP_ID_PMDR, DEBUGLOG_LEVEL_FALAL,ATAT_LEVEL_OFF,"[%s]Failed to send queue",__func__);
		if(NULL != astMsg.vdQueueBuffer_p){
			auwExeRslt = OS_ReleaseFMemoryBlock(astMsg.vdQueueBuffer_p);
			if(auwExeRslt != OS_ERR_OK){
				PM_DBG("\n########## PM Faild to realease memory:%d ##########\n", auwExeRslt);
				LOG_OUTPUT(LOG_APP_ID_PMDR, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]Faild to realease memory:%d",__func__, auwExeRslt);
			}
		}

	}
	
}

/********************************************************************************************/
/**	\function		wvdPmDrvDetOnStage0
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	StandbyOut Timming 1
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
void wvdPmDrvDetOnStage0(void)
{
#if 0 //for TCC8031 poc start
		gpio_set(PM_BL_SYNC,GPIO_HIGH);
		gpio_set(PM_DG33V_CONT,GPIO_HIGH);
		gpio_set(PM_D12_18_SYNC,GPIO_HIGH);
	
		gpio_config(PM_BL_DIM,GPIO_FUNC(10)|GPIO_OUTPUT);
		gpio_set(PM_BL_DIM,GPIO_HIGH);
		set_pwm_port(0,94);
		pwm_config(0,13800000/2,13800000);
		pwm_enable(0);
	
		gpio_set(PM_DAB_18_12_PCONT,GPIO_HIGH);
	
	
		gpio_set(PM_A8R0V_CONT,GPIO_HIGH);
		gpio_set(PM_GNSSA_CONT,GPIO_HIGH);
		gpio_set(PM_PANT_CONT,GPIO_HIGH);
		gpio_set(PM_PAMP_EN,GPIO_HIGH);
		gpio_set(PM_FAN5V_CONT,GPIO_HIGH);
		gpio_set(PM_BL_EN,GPIO_HIGH);
#endif  //for TCC8031  Poc end
	
#if (TEMP_COMPILE_SWITCH_05WORK_10WORK == TEMP_COMPILE_SWITCH_05WORK)
			//gpio_set(PM_DAB_18_12_PCONT,GPIO_HIGH);
			//power timming6
			gpio_set(PM_BL_SYNC,GPIO_HIGH);
			gpio_set(PM_D3P_CONT,GPIO_HIGH);
			gpio_set(PM_BT_18V_CONT,GPIO_HIGH);
		
			gpio_set(PM_BL_PWM,GPIO_HIGH);
			set_pwm_port(PM_DRV_DATA_ZERO,PM_DRV_PWM_PORT_NUM);
			pwm_config(PM_DRV_DATA_ZERO,PM_DRV_PWM_DUTY_NS,PM_DRV_PWM_PERIOD_NS);
			pwm_enable(PM_DRV_DATA_ZERO);
			
			//power timming0
			gpio_set(PM_A_MUTE,GPIO_LOW);
	
			//power timming1
			gpio_set(PM_PANT_CONT,GPIO_HIGH);
			gpio_set(PM_LWC_CONT,GPIO_HIGH);		
			gpio_set(PM_RCV_CONT,GPIO_HIGH);
			gpio_set(PM_K_LINE_TX,GPIO_HIGH);
			gpio_set(PM_M6V_CONT,GPIO_HIGH);	
			gpio_set(PM_FAN_CONT,GPIO_HIGH);
			gpio_set(PM_EEP_CONT,GPIO_HIGH);
	
			//power timming2
			gpio_set(PM_M5V_CONT,GPIO_HIGH);
			gpio_set(PM_D3P_CONT,GPIO_HIGH);
			gpio_set(PM_GNSSA_CONT,GPIO_HIGH);
			gpio_set(PM_BT_33V_CONT,GPIO_HIGH);
			gpio_set(PM_BT_18V_CONT,GPIO_HIGH); 	
			gpio_set(PM_TPLCD_33V_CONT,GPIO_HIGH);
			gpio_set(PM_SAT_33V_CONT,GPIO_HIGH);		
			gpio_set(PM_DAB_18_12_PCONT,GPIO_HIGH);
			gpio_set(PM_DSP_RST,GPIO_HIGH);
			gpio_set(PM_AMP_STBY,GPIO_HIGH);
			//power timming3
			gpio_set(PM_LVDS_33V_CONT,GPIO_HIGH);		
			gpio_set(PM_LVDS_18V_CONT,GPIO_HIGH);
			gpio_set(PM_LCD_RST,GPIO_HIGH);//A53 LCD_RST
			//power timming4
			gpio_set(PM_TP_RST,GPIO_HIGH);//A53 TP_RST
	
			gpio_set(PM_LCD_PON,GPIO_HIGH);//A53 LCD_PON
			gpio_set(PM_LCD_DISP,GPIO_HIGH);//A53 LCD_DISP
			
			//A53 USBDH20_PCNT	
			//power timming5
			gpio_set(PM_BL_EN,GPIO_HIGH);

			PMDRV_DBG("R5 power gpio init over ...\n");

			nvdPmDrvSendMsg(ARPC_ID_R5_POWERMNG, EV_INF_STANDBYOUT_FINISH, NULL, PM_DRV_DATA_ZERO);
#else
			OS_ErrCode_t aswRet = OS_ERR_OK;

			gpio_set(PM_K_LINE_TX, GPIO_HIGH);

			aswRet = OS_StartTimer(OS_TIMER_ID_PMDRVPINSETUP1);
			if(aswRet != OS_ERR_OK){
				PMDRV_DBG("########## Timer Start PINUP1 Failed:%d ##########\n", aswRet);
				LOG_OUTPUT(LOG_APP_ID_PMDR, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]Timer Start PINUP1 Failed:%d",__func__, aswRet);
			}

			aswRet = OS_StartTimer(OS_TIMER_ID_PMDRVPINSETUP2);
			if(aswRet != OS_ERR_OK){
				PMDRV_DBG("########## Timer Start PINUP2 Failed:%d ##########\n", aswRet);
				LOG_OUTPUT(LOG_APP_ID_PMDR, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]Timer Start PINUP2 Failed:%d",__func__, aswRet);
			}

			aswRet = OS_StartTimer(OS_TIMER_ID_PMDRVPINSETUP3);
			if(aswRet != OS_ERR_OK){
				PMDRV_DBG("########## Timer Start PINUP3 Failed:%d ##########\n", aswRet);
				LOG_OUTPUT(LOG_APP_ID_PMDR, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]Timer Start PINUP3 Failed:%d",__func__, aswRet);
			}

			aswRet = OS_StartTimer(OS_TIMER_ID_PMDRVPINSETUP4);
			if(aswRet != OS_ERR_OK){
				PMDRV_DBG("########## Timer Start PINUP4 Failed:%d ##########\n", aswRet);
				LOG_OUTPUT(LOG_APP_ID_PMDR, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]Timer Start PINUP4 Failed:%d",__func__, aswRet);
			}

			aswRet = OS_StartTimer(OS_TIMER_ID_PMDRVPINSETUP5);
			if(aswRet != OS_ERR_OK){
				PMDRV_DBG("########## Timer Start PINUP5 Failed:%d ##########\n", aswRet);
				LOG_OUTPUT(LOG_APP_ID_PMDR, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]Timer Start PINUP5 Failed:%d",__func__, aswRet);
			}

#endif
		

}

#if (TEMP_COMPILE_SWITCH_05WORK_10WORK == TEMP_COMPILE_SWITCH_05WORK)
/********************************************************************************************/
/**	\function		nvdPmDrvDetOffStage0
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	StandbyOut Timming 1
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
void wvdPmDrvDetOffStage0(void)
{
	//TBD:det set
	//TBD:start timer nvdPmDrvDetOffStage2


	//gpio_set(PM_DAB_18_12_PCONT,GPIO_HIGH);
	//power timming6
	gpio_set(PM_BL_SYNC,GPIO_LOW);
	gpio_set(PM_D3P_CONT,GPIO_LOW);
	gpio_set(PM_BT_18V_CONT,GPIO_LOW);

	gpio_set(PM_BL_PWM,GPIO_LOW);
	
	//power timming0
	gpio_set(PM_A_MUTE,GPIO_LOW);

	//power timming1
	gpio_set(PM_PANT_CONT,GPIO_LOW);
	gpio_set(PM_LWC_CONT,GPIO_LOW);		
	gpio_set(PM_RCV_CONT,GPIO_LOW);
	gpio_set(PM_M6V_CONT,GPIO_LOW);	
	gpio_set(PM_FAN_CONT,GPIO_LOW);
	gpio_set(PM_EEP_CONT,GPIO_LOW);
	gpio_set(PM_K_LINE_TX,GPIO_LOW);

	//power timming2
	gpio_set(PM_M5V_CONT,GPIO_LOW);
	gpio_set(PM_D3P_CONT,GPIO_LOW);
	gpio_set(PM_GNSSA_CONT,GPIO_LOW);
	gpio_set(PM_BT_33V_CONT,GPIO_LOW);
	gpio_set(PM_BT_18V_CONT,GPIO_LOW); 	
	gpio_set(PM_TPLCD_33V_CONT,GPIO_LOW);
	gpio_set(PM_SAT_33V_CONT,GPIO_LOW);		
	gpio_set(PM_DAB_18_12_PCONT,GPIO_LOW);
	gpio_set(PM_DSP_RST,GPIO_LOW);
	gpio_set(PM_AMP_STBY,GPIO_LOW);
	//power timming3
	gpio_set(PM_LVDS_33V_CONT,GPIO_LOW);		
	gpio_set(PM_LVDS_18V_CONT,GPIO_LOW);
	gpio_set(PM_LCD_RST,GPIO_LOW);//A53 LCD_RST
	//power timming4
	gpio_set(PM_TP_RST,GPIO_LOW);//A53 TP_RST

	gpio_set(PM_LCD_PON,GPIO_LOW);//A53 LCD_PON
	gpio_set(PM_LCD_DISP,GPIO_LOW);//A53 LCD_DISP
	
	//A53 USBDH20_PCNT	
	//power timming5
	gpio_set(PM_BL_EN,GPIO_LOW);
		
	PMDRV_DBG("R5 power gpio off over ...\n");

	OS_StopTimer(OS_TIMER_ID_PMDRVCYC);
	nvdPmDrvSendMsg(ARPC_ID_R5_POWERMNG, EV_INF_STANDBYIN_FINISH, NULL, PM_DRV_DATA_ZERO);

}

#else
/********************************************************************************************/
/**	\function		wvdPmDrvDetOffStage0
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	StandbyOut Timming 1
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
void wvdPmDrvDetOffStage0(void)
{
	OS_ErrCode_t aswRet = OS_ERR_OK;

	gpio_set(PM_A_MUTE,GPIO_LOW);

#ifdef PM_ST_EN
		if(wvdPmStNoGet() == PM_ST_NO_1420){
			gpio_set(PM_A24_NC, GPIO_LOW);
		}
#endif

	aswRet = OS_StartTimer(OS_TIMER_ID_PMDRVPINSETDN1);
	if(aswRet != OS_ERR_OK){
		PMDRV_DBG("########## Timer Start PINDN1 Failed:%d ##########\n", aswRet);
		LOG_OUTPUT(LOG_APP_ID_PMDR, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]Timer Start PINDN1 Failed:%d",__func__, aswRet);
	}
}

/********************************************************************************************/
/**	\function		wvdPmDrvDetOnStage1
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	StandbyOut Timming 1
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
void wvdPmDrvDetOnStage1(void)
{	
	gpio_set(PM_LCD_33V_CONT, GPIO_HIGH);
	gpio_set(PM_TP_33V_CONT, GPIO_HIGH);
}

/********************************************************************************************/
/**	\function		wvdPmDrvDetOnStage2
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	StandbyOut Timming 2
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
void wvdPmDrvDetOnStage2(void)
{
	gpio_set(PM_VIDEO_DEC_12V_CONT, GPIO_HIGH);
	gpio_set(PM_VIDEO_DEC_33V_CONT, GPIO_HIGH);

#ifdef PM_ST_EN
	if(wvdPmStNoGet() == PM_ST_NO_1420){
		gpio_set(PM_A1_NC, GPIO_HIGH);
	}
#endif

}

/********************************************************************************************/
/**	\function		wvdPmDrvDetOnStage3
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	StandbyOut Timming 3
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
void wvdPmDrvDetOnStage3(void)
{
	gpio_set(PM_GNSS_CONT, GPIO_HIGH);
	
#ifdef PM_ST_EN
	if(wvdPmStNoGet() == PM_ST_NO_1420){
		gpio_set(PM_A11_NC, GPIO_HIGH);
	}
#endif	

}

/********************************************************************************************/
/**	\function		wvdPmDrvDetOnStage4
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	StandbyOut Timming 3
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
void wvdPmDrvDetOnStage4(void)
{
	gpio_set(PM_LWC_CONT, GPIO_HIGH);
	gpio_set(PM_RCV_CONT, GPIO_HIGH);

#ifdef PM_ST_EN
	if(wvdPmStNoGet() == PM_ST_NO_1420){
		gpio_set(PM_A24_NC, GPIO_HIGH);
	}
#endif

}


/********************************************************************************************/
/**	\function		wvdPmDrvDetOnStage5
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	StandbyOut Timming 4
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
void wvdPmDrvDetOnStage5(void)
{	
	gpio_set(PM_AMP_STBY, GPIO_HIGH);
	gpio_set(PM_PANT_CONT, GPIO_HIGH);
	gpio_set(PM_M5V_CONT, GPIO_HIGH);
	
#ifdef PM_ST_EN
		if(wvdPmStNoGet() == PM_ST_NO_1420){
			gpio_set(PM_A25_NC, GPIO_HIGH);
		}
#endif

	nvdPmDrvSendMsg(ARPC_ID_R5_POWERMNG, EV_INF_STANDBYOUT_FINISH, NULL, PM_DRV_DATA_ZERO);
}

/********************************************************************************************/
/**	\function		nvdPmDrvDetOffStage1
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	StandbyOut Timming 1
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
void wvdPmDrvDetOffStage1(void)
{
	gpio_set(PM_AMP_STBY,GPIO_LOW);

#ifdef PM_ST_EN
	if(wvdPmStNoGet() == PM_ST_NO_1420){
		gpio_set(PM_A11_NC, GPIO_LOW);
	}
#endif

}

/********************************************************************************************/
/**	\function		nvdPmDrvDetOffStage2
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	StandbyOut Timming 2
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
void wvdPmDrvDetOffStage2(void)
{
	OS_ErrCode_t aswRet = OS_ERR_OK;
	
	gpio_set(PM_PANT_CONT,GPIO_LOW);
	gpio_set(PM_LCD_33V_CONT,GPIO_LOW);
	gpio_set(PM_TP_33V_CONT,GPIO_LOW);

	aswRet = OS_StartTimer(OS_TIMER_ID_PMDRVPINSETDN3);
	if(aswRet != OS_ERR_OK){
		PMDRV_DBG("########## Timer Start PINDN3 Failed:%d ##########\n", aswRet);
		LOG_OUTPUT(LOG_APP_ID_PMDR, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]Timer Start PINDN3 Failed:%d",__func__, aswRet);
	}
}

/********************************************************************************************/
/**	\function		nvdPmDrvDetOffStage3
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	StandbyOut Timming 3
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
void wvdPmDrvDetOffStage3(void)
{
	OS_ErrCode_t aswRet = OS_ERR_OK;
	
	gpio_set(PM_M5V_CONT,GPIO_LOW);
	//gpio_set(PM_VIDEO_DEC_12V_CONT,GPIO_LOW);
	//gpio_set(PM_VIDEO_DEC_33V_CONT,GPIO_LOW);
	gpio_set(PM_GNSS_CONT,GPIO_LOW);
	gpio_set(PM_LWC_CONT,GPIO_LOW);
	gpio_set(PM_RCV_CONT,GPIO_LOW);

#ifdef PM_ST_EN
		if(wvdPmStNoGet() == PM_ST_NO_1420){
			gpio_set(PM_A1_NC, GPIO_LOW);
		}
#endif

	aswRet = OS_StartTimer(OS_TIMER_ID_PMDRVPINSETDN4);
	if(aswRet != OS_ERR_OK){
		PMDRV_DBG("########## Timer Start PINDN4 Failed:%d ##########\n", aswRet);
		LOG_OUTPUT(LOG_APP_ID_PMDR, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]Timer Start PINDN4 Failed:%d",__func__, aswRet);
	}

}

/********************************************************************************************/
/**	\function		nvdPmDrvDetOffStage4
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	StandbyOut Timming N
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
void wvdPmDrvDetOffStage4(void)
{
	OS_ErrCode_t aswRet = OS_ERR_OK;

	wubKlineDrv_Close();
	gpio_config(PM_K_LINE_TX, GPIO_OUTPUT | GPIO_FUNC(0));
	gpio_set(PM_K_LINE_TX, GPIO_LOW);

	aswRet = OS_StopTimer(OS_TIMER_ID_PMDRVCYC);
	if(aswRet != OS_ERR_OK){
		PMDRV_DBG("########## Timer Start Failed:%d ##########\n", aswRet);
		//LOG_OUTPUT(LOG_APP_ID_PMDR, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]Timer Stop Failed：%d",__func__, aswRet);
	}

	wvdPmGpioClear();
	nvdPmDrvSendMsg(ARPC_ID_R5_POWERMNG, EV_INF_STANDBYIN_FINISH, NULL, PM_DRV_DATA_ZERO);
}

#endif


/********************************************************************************************/
/**	\function		nvdPmDrvDetSet
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	PIN Level Set
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
static void nvdPmDrvDetSet(OS_StdQueue_ts *astMessage_p)
{
	Type_uByte aubRet = PM_GPIO_LEVEL_SET_FAIL;
	Type_uByte *aubData_p = astMessage_p->vdQueueBuffer_p;
	
	if((aubData_p == NULL) || (astMessage_p->uwQueueSize != PM_DRV_PIN_SET_DATA_SIZE)){
		PMDRV_DBG("\n########### PMDRV Rcv RPC PIN Set Event Data Error:Size = %d #########\n", astMessage_p->uwQueueSize);
		LOG_OUTPUT(LOG_APP_ID_PMDR, DEBUGLOG_LEVEL_ERROR,ATAT_LEVEL_OFF,"[%s]Invalid data size:%d",__func__, astMessage_p->uwQueueSize);
		return;
	}

	if((aubData_p[PM_DRV_DATA_ZERO] != PM_GPIO_LEVEL_LOW) && (aubData_p[PM_DRV_DATA_ZERO] != PM_GPIO_LEVEL_HIGH)){
		PMDRV_DBG("\n########### PMDRV Rcv RPC PIN Set Event Level Vaule:%d is Error #########\n", aubData_p[PM_DRV_DATA_ZERO]);
		LOG_OUTPUT(LOG_APP_ID_PMDR, DEBUGLOG_LEVEL_ERROR,ATAT_LEVEL_OFF,"[%s]Invalid pin level:%d",__func__, aubData_p[PM_DRV_DATA_ZERO]);
		return;
	}

	switch(aubData_p[PM_DRV_DATA_ONE])
	{
		case PM_GPIO_PIN_MUTE:
			PMDRV_DBG("\n********** PMDRV Rcv RPC A-MUTE Set:%d src=%x**********", aubData_p[PM_DRV_DATA_ZERO], astMessage_p->swSrcTaskId);
			gpio_set(PM_A_MUTE,(Type_uWord)aubData_p[PM_DRV_DATA_ZERO]);
			if(gpio_get(PM_A_MUTE) == aubData_p[PM_DRV_DATA_ZERO]){
				aubRet = PM_GPIO_LEVEL_SET_SUCCESS;
			}
			else{
				aubRet = PM_GPIO_LEVEL_SET_FAIL;
				PMDRV_DBG("\n######## PMDRV Failed to set A-MUTE ##########\n");
				LOG_OUTPUT(LOG_APP_ID_PMDR, DEBUGLOG_LEVEL_ERROR,ATAT_LEVEL_OFF,"[%s]Failed to set A-MUTE",__func__);
			}
			nvdPmDrvSendMsg(astMessage_p->swSrcTaskId, EV_INF_GPIO_LEVEL_SET_RES, &aubRet, sizeof(aubRet));
			LOG_OUTPUT(LOG_APP_ID_PMDR, DEBUGLOG_LEVEL_FALAL,ATAT_LEVEL_OFF,"A-MUTE set :%d src=%x",aubData_p[PM_DRV_DATA_ZERO], astMessage_p->swSrcTaskId);
			break;

		case PM_GPIO_PIN_AMP_STBY:
			PMDRV_DBG("\n********** PMDRV Rcv RPC AMP-STBY Set:%d src=%x**********", aubData_p[PM_DRV_DATA_ZERO], astMessage_p->swSrcTaskId);
			gpio_set(PM_AMP_STBY,(Type_uWord)aubData_p[PM_DRV_DATA_ZERO]);
			if(gpio_get(PM_AMP_STBY) == aubData_p[PM_DRV_DATA_ZERO]){
				aubRet = PM_GPIO_LEVEL_SET_SUCCESS;
			}
			else{
				aubRet = PM_GPIO_LEVEL_SET_FAIL;
				PMDRV_DBG("\n######## PMDRV Failed to set AMP-STBY ##########\n");
				LOG_OUTPUT(LOG_APP_ID_PMDR, DEBUGLOG_LEVEL_ERROR,ATAT_LEVEL_OFF,"[%s]Failed to set AMP-STBY",__func__);
			}
			nvdPmDrvSendMsg(astMessage_p->swSrcTaskId, EV_INF_GPIO_LEVEL_SET_RES, &aubRet, sizeof(aubRet));
			LOG_OUTPUT(LOG_APP_ID_PMDR, DEBUGLOG_LEVEL_FALAL,ATAT_LEVEL_OFF,"AMP-STBY set :%d src=%x", aubData_p[PM_DRV_DATA_ZERO], astMessage_p->swSrcTaskId);
			break;

#if (TEMP_COMPILE_SWITCH_05WORK_10WORK != TEMP_COMPILE_SWITCH_05WORK)
		case PM_GPIO_PIN_TP_33V_CONT:
			PMDRV_DBG("\n********** PMDRV Rcv RPC TP_33V_CONT Set:%d src=%x **********", aubData_p[PM_DRV_DATA_ZERO], astMessage_p->swSrcTaskId);
			gpio_set(PM_TP_33V_CONT,(Type_uWord)aubData_p[PM_DRV_DATA_ZERO]);
			if(gpio_get(PM_TP_33V_CONT) == aubData_p[PM_DRV_DATA_ZERO]){
				aubRet = PM_GPIO_LEVEL_SET_SUCCESS;
			}
			else{
				aubRet = PM_GPIO_LEVEL_SET_FAIL;
				PMDRV_DBG("\n######## PMDRV Failed to set TP_33V_CONT ##########\n");
				LOG_OUTPUT(LOG_APP_ID_PMDR, DEBUGLOG_LEVEL_ERROR,ATAT_LEVEL_OFF,"[%s]Failed to set TP-33V",__func__);
			}
			nvdPmDrvSendMsg(astMessage_p->swSrcTaskId, EV_INF_GPIO_LEVEL_SET_RES, &aubRet, sizeof(aubRet));
			LOG_OUTPUT(LOG_APP_ID_PMDR, DEBUGLOG_LEVEL_FALAL,ATAT_LEVEL_OFF,"TP-33V set :%d src=%x", aubData_p[PM_DRV_DATA_ZERO], astMessage_p->swSrcTaskId);
			break;
#endif

		default:
			PMDRV_DBG("\n########### PMDRV Rcv RPC PIN Set Event Name:%d is Error #########\n", aubData_p[PM_DRV_DATA_ONE]);
			LOG_OUTPUT(LOG_APP_ID_PMDR, DEBUGLOG_LEVEL_ERROR,ATAT_LEVEL_OFF,"Invalid Pin Set:pin=%x,src=%x",aubData_p[PM_DRV_DATA_ONE], astMessage_p->swSrcTaskId);
			break;
	}
}

/********************************************************************************************/
/**	\function		nvdPmDrvStandbyOut
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	StandbyOut
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
static void nvdPmDrvStandbyOut(OS_StdQueue_ts *astMessage_p)
{
	wvdPmDrvDetOnStage0();
}

/********************************************************************************************/
/**	\function		nvdPmDrvStandbyIn
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	StandbyOut
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
static void nvdPmDrvStandbyIn(OS_StdQueue_ts *astMessage_p)
{
#if (TEMP_COMPILE_SWITCH_05WORK_10WORK == TEMP_COMPILE_SWITCH_05WORK)
	wvdPmDrvDetOffStage0();
#else
	wvdPmDrvDetOffStage2();
#endif
}

/********************************************************************************************/
/**	\function		nvdPmDrvSendMsg
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	PM Send Message
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
static void nvdPmDrvSendMsg(Type_uByte aubReceiver, Type_uHWord auhEvent, void * auvData_p, Type_uHWord auhLength)
{
	Type_sByte asbRet = RPC_NG;

	asbRet = wsbRPCSendMsg(ARPC_ID_R5_POWERDRV, aubReceiver, auhEvent, auvData_p, auhLength);
	if(RPC_OK != asbRet){
		PMDRV_DBG("\n########## PMDRV Send Rpc Failed ##########\n");
		LOG_OUTPUT(LOG_APP_ID_PMDR, DEBUGLOG_LEVEL_FALAL,ATAT_LEVEL_OFF,"[%s]RPC send failed:%d",__func__, asbRet);
	}
}


