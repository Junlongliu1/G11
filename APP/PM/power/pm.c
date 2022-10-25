/********************************************************************************************/
/* File Name	: pm.c																		*/
/* Date			: 2021/11/12																*/
/* Author		: wangying																	*/
/* Model		: EDA																		*/
/* Description	: power manager																*/
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
#include "rpc_if.h"
#include "rpc_event.h"
#include "rpc_data.h"
#include "pm.h"
#include "pmdrv.h"
#include "gpio.h"
#include "tcc_pmgpio.h"
#include "HAL_PowerManager.h"
#include <HAL_devices.h>
#include "Eep_core.h"
#include "logapp.h"
#include "temp_compile_switch.h"
#ifdef PM_ST_EN
#include "reg_phys.h"
#include "bsp_int.h"
#endif


/********************************************************************************************/
/*							Function Definition Section										*/
/********************************************************************************************/
extern Type_uByte wubPmSoftWdtStandbyOut(void);
extern Type_uByte wubPmSoftWdtStandbyIn(void);
extern Type_uByte wubPmSoftWdtForceStandby(void);
extern Type_uByte wubLineTool_PowerOn(void);
extern Type_uByte wubLineTool_PowerOff(void);
extern Type_uByte wubLineTool_Standbyout(void);
extern Type_uByte wubLineTool_Standbyin(void);
extern Type_uByte wubLineTool_EnforceStandby(void);
extern Type_uByte wubAgent_Init(void);
extern Type_uByte wubAgent_StandbyOut(void);
extern Type_uByte wubAgent_StandbyIn(void);
extern Type_uByte wubAgent_EnforceStandby(void);
extern Type_uByte wubEEP_Init(void);
extern Type_uByte wubEEP_StandbyOut(void);
extern Type_uByte wubEEP_StandbyIn(void);
extern Type_uByte wubEEP_EnforceStandby(void);
extern Type_uByte wubLed_StandbyOut(void);
extern Type_uByte wubLed_StandbyIn(void);
extern Type_uByte wubLed_EnforceStandby(void);
extern Type_uByte wubLed_PowerOn(void);
extern Type_uByte wubLed_PowerOff(void);
extern Type_uByte wubSnr_StandbyOut(void);
extern Type_uByte wubSnr_StandbyIn(void);
extern Type_uByte wubSnr_EnforceStandby(void);
extern Type_uByte wubKey_StandbyOut(void);
extern Type_uByte wubKey_StandbyIn(void);
extern Type_uByte wubKey_EnforceStandby(void);
extern Type_uByte wubRPC_Standbyout(void);
extern Type_uByte wubRPC_Standbyin(void);
extern Type_uByte wubR5Log_StandbyOut(void);
extern Type_uByte wubR5Log_StandbyIn(void);
extern Type_uByte wubR5Log_EnforceStandby(void);
extern Type_uByte wubKlineDrv_ForceStandby(void);
extern Type_uByte wubKlineDrv_StandbyIn(void);
extern Type_uByte wubKlineDrv_StandbyOut(void);
extern Type_uByte wubLetApp_ForceStandby(void);
extern Type_uByte wubLetApp_StandbyIn(void);
extern Type_uByte wubLetApp_StandbyOut(void);
extern Type_uByte wubBuckupRam_StandbyOut(void);
extern Type_uByte wubBuckupRam_StandbyIn(void);
extern Type_uByte wubBuckupRam_EnforceStandby(void);
extern Type_uByte wubBackup_AllClear(void);


static Type_uByte nubPmColdInit(void);
static void nvdPmRpcRcv(const Type_uByte aubsrc, const Type_uHWord auhCmd, void *avbData_p, const Type_uHWord auhLength);
static void nvdPmInit(void);
#ifndef PM_HAL_BEFORE_OS
extern void nvdPmHalOpen(void);
#endif
static void nvdPmSendStartType(void);
static void nvdPmProcCtrl(OS_StdQueue_ts *astMessage_p);
static void nvdPmUpdateResFlg(OS_StdQueue_ts *astMessage_p);
static void nvdPmSendMsg(Type_uByte aubReceiver, Type_uHWord auhEvent, void * auvData_p, Type_uHWord auhLength);
static void nvdPmDownCtrl(void);
static void nvdPmSysReset(Type_uByte aubType);
static void nvdPmSysDown(Type_uByte aubType);
static void nvdPmErrResetNow(Type_uByte aubType);

static void nvdPmSocStartErrReset(OS_StdQueue_ts *astMessage_p);
static void nvdPmSocStandbyOutErrReset(OS_StdQueue_ts *astMessage_p);
static void nvdPmSysAccResetCtrl(OS_StdQueue_ts *astMessage_p);
static void nvdPmSysUpdateResetCtrl(OS_StdQueue_ts *astMessage_p);
static void nvdPmSysFactoryResetCtrl(OS_StdQueue_ts *astMessage_p);
static void nvdPmSysA53ReqResetCtrl(OS_StdQueue_ts *astMessage_p);
static void nvdPmSysA7ReqResetCtrl(OS_StdQueue_ts *astMessage_p);
static void nvdPmSysA53HeartResetCtrl(OS_StdQueue_ts *astMessage_p);
static void nvdPmSysA7HeartResetCtrl(OS_StdQueue_ts *astMessage_p);
static void nvdPmSysA53WdtResetCtrl(OS_StdQueue_ts *astMessage_p);
static void nvdPmSysA7WdtResetCtrl(OS_StdQueue_ts *astMessage_p);
static void nvdPmA53WdtErrResetNow(OS_StdQueue_ts *astMessage_p);
static void nvdPmA7WdtErrResetNow(OS_StdQueue_ts *astMessage_p);
static void nvdPmSysDownCtrl(OS_StdQueue_ts *astMessage_p);
static void nvdPmStandbyInOver(OS_StdQueue_ts *astMessage_p);
static void nvdPmStandbyOutCtrlStart(OS_StdQueue_ts *astMessage_p);
static void nvdPmStandbyOutMcuCtrlProc(OS_StdQueue_ts *astMessage_p);
static void nvdPmStandbyOutSocCtrlProc(OS_StdQueue_ts *astMessage_p);
static void nvdPmStandbyOutSocCtrlStart(OS_StdQueue_ts *astMessage_p);
static void nvdPmStandbyOutResCtrl(OS_StdQueue_ts *astMessage_p);
static void wvdPmStandbyOutNextCtrl(void);
static void wvdPmStandbyOutOver(OS_StdQueue_ts *astMessage_p);
//static void nvdPmStandbyIningResetCtrl(OS_StdQueue_ts *astMessage_p);
static void nvdPmStandbyInNextCtrl(void);
static void nvdPmSocInitReceive(OS_StdQueue_ts *astMessage_p); 

static void nvdPmAccStandbyInStart(OS_StdQueue_ts *astMessage_p);
static void nvdPmUpdateStandbyInStart(OS_StdQueue_ts *astMessage_p);
static void nvdPmUpdateInitStandbyInStart(OS_StdQueue_ts *astMessage_p);
static void nvdPmFactoryStandbyInStart(OS_StdQueue_ts *astMessage_p);
static void nvdPmWdtA53StandbyInStart(OS_StdQueue_ts *astMessage_p);
static void nvdPmWdtA7StandbyInStart(OS_StdQueue_ts *astMessage_p);
static void nvdPmA53ReqStandbyInStart(OS_StdQueue_ts *astMessage_p);
static void nvdPmA7ReqStandbyInStart(OS_StdQueue_ts *astMessage_p);
static void nvdPmA53HeartStandbyInStart(OS_StdQueue_ts *astMessage_p);
static void nvdPmA7HeartStandbyInStart(OS_StdQueue_ts *astMessage_p);
static void nvdPmStandbyInCtrlStart(OS_StdQueue_ts *astMessage_p);
static void nvdPmStandbyInCtrlProc(OS_StdQueue_ts *astMessage_p);
static void nvdPmStandbyInResCtrl(OS_StdQueue_ts *astMessage_p);

static void nvdPmUpdatePowerKeyResFlg(OS_StdQueue_ts *astMessage_p);
static void nvdPmPowerKeyOnCtrlStart(OS_StdQueue_ts *astMessage_p);
static void nvdPmPowerKeyOffCtrlStart(OS_StdQueue_ts *astMessage_p);
static void nvdPmPowerKeyOnCtrlProc(OS_StdQueue_ts *astMessage_p);
static void nvdPmPowerKeyOnNextCtrl(void);
static void nvdPmPowerKeyOffCtrlProc(OS_StdQueue_ts *astMessage_p);
static void nvdPmPowerKeyOnResCtrl(OS_StdQueue_ts *astMessage_p);
static void nvdPmPowerKeyOffResCtrl(OS_StdQueue_ts *astMessage_p);
static void nvdPmPowerKeyOffNextCtrl(void);
static void nvdPmPowerKeyProcStop(void);

static void nvdPmSocWdtErrCtrl(OS_StdQueue_ts *astMessage_p);
static void nvdPmSocHeartErrCtrl(OS_StdQueue_ts *astMessage_p);
static void nvdPmAccOff(OS_StdQueue_ts *astMessage_p);

#if 0
static void nvdPmUpdateResetCtrlStart(OS_StdQueue_ts *astMessage_p);
static void nvdPmUpdateResetCtrlProc(OS_StdQueue_ts *astMessage_p);
static void nvdPmUpdateResetResCtrl(OS_StdQueue_ts *astMessage_p);
static Type_uByte nvdPmResetCtrlProc(const PmComAsyncIf_ts *astProcTbl_p, Type_uByte aubTblSize);
static void nvdPmUpdateResetNextCtrl(void);

static void nvdPmFactoryResetCtrlStart(OS_StdQueue_ts *astMessage_p);
static void nvdPmFactoryResetCtrlProc(OS_StdQueue_ts *astMessage_p);
static void nvdPmFactoryResetResCtrl(OS_StdQueue_ts *astMessage_p);
static void nvdPmFactoryResetNextCtrl(void);

static Type_uByte nubPmSocUpdateReset(void);
static Type_uByte nubPmSocFactoryReset(void);
#endif

static Type_uByte nubPmSocStandbyOut(void);
static Type_uByte nubPmSocStandbyIn(void);
static Type_uByte nubPmSocPowerOn(void);
static Type_uByte nubPmSocPowerOff(void);

#ifdef PM_ST_EN
static void nvdPmStPortCheck(void);
static void nubPmStPinLevelGet(void);

#endif

/********************************************************************************************/
/*							Macro Definition Section										*/
/********************************************************************************************/
#define ARPC_ID_ANY						(0xFFU)

#define PM_STATE_NONE					(0x1000U)
#define PM_STATE_NO_CHANGE				(0xFFFEU)
#define PM_STATE_ANY					(0xFFFFU)

#define PM_SOC_INIT_OK_WAIT				(0x0001U)
#define PM_SOC_INIT_OK					(0x0002U)

#define	PM_STANDBY_STANDBYIN 			(0x0001U)
#define	PM_STANDBY_STANDBYINING			(0x0002U)
#define	PM_STANDBY_STANDBYOUT			(0x0004U)
#define	PM_STANDBY_STANDBYOUTING		(0x0008U)

#define	PM_POWERKEY_ONING				(0x0002U)
#define	PM_POWERKEY_OFFING				(0x0008U)

#define	PM_UPDATE_RESET_ING	 			(0x0001U)
#define	PM_FACTORY_RESET_ING 			(0x0002U)
#define	PM_A53REQ_RESET_ING				(0x0004U)
#define	PM_A7REQ_RESET_ING				(0x0008U)
#define	PM_A53HEART_RESET_ING			(0x0010U)
#define	PM_A7HEART_RESET_ING			(0x0020U)
#define	PM_A53WDT_RESET_ING				(0x0040U)
#define	PM_A7WDT_RESET_ING				(0x0080U)
#define	PM_RESET_OVER 					(0x0100U)


#define PM_PRIORITY_LINE_MAX			(5U)
#define PM_PRIORITY_ROW_MAX				(8U)
#define PM_PRIORITY_LINE_ARRAY_MAX		(PM_PRIORITY_LINE_MAX - 1)

#define PM_MODULE_NONE					(0xFFU)

#ifdef PM_STANDBYOUT_RETRY
#define PM_SOC_STANDBYOUT_RESTRY_NUM	(0U)
#endif

#define PM_DATA_ZERO					(0x00U)
#define PM_BIT_TRUE						(0x0001U)
#define PM_ARRAY_INDEX_ONE				(1U)

/********************************************************************************************/
/*							ROM Table Section												*/
/********************************************************************************************/
static const PmComSyncIf_ts nstPmColdInit_Tbl[] = {
/*		ubModuleID				proc_fp				*/
	{ARPC_ID_R5_POWERMNG,		nubPmColdInit		},
	{ARPC_ID_R5_AGENT,			wubAgent_Init		},
	{ARPC_ID_EEPROM,			wubEEP_Init			},
	//{ARPC_ID_R5_LOGAPP,			wubLogApp_Init		},
};

static const PmComSyncIf_ts nstPmInit_Tbl[] = {
/*		ubModuleID				proc_fp				*/
	{ARPC_ID_R5_LOGAPP,			wubLogApp_Init		},
};


static const PmStandbyIf_ts nstPmStandby_Tbl[] = {
/*		ubModuleID			uwStandbyOutTime	uwStandbyInTime		uwCallbackTime		standbyOutProc_fp		standbyInProc_fp		forceStandbyProc_fp			callbackProc_fp*/
	{ARPC_ID_R5_POWERDRV,	170,					40,						0,			wubPmDrvStandbyOut,		wubPmDrvStandbyIn,		wubPmDrvForceStandby,		NULL},
	{ARPC_ID_R5_SOFTWDT,	0,						0,						0,			wubPmSoftWdtStandbyOut,	wubPmSoftWdtStandbyIn, 	NULL,						NULL},
	{ARPC_ID_EEPROM,		1000,					10,						0,			wubEEP_StandbyOut,		wubEEP_StandbyIn, 		wubEEP_EnforceStandby,		NULL},
	{ARPC_ID_R5_BACKUPRAM,	0,						0,						0,			wubBuckupRam_StandbyOut,wubBuckupRam_StandbyIn,	wubBuckupRam_EnforceStandby,NULL},
	{ARPC_ID_R5_AGENT,		0,						0,						0,			wubAgent_StandbyOut,	wubAgent_StandbyIn, 	wubAgent_EnforceStandby,	NULL},
	{ARPC_ID_LED,			0,						0,						0,			wubLed_StandbyOut,		wubLed_StandbyIn, 		wubLed_EnforceStandby,		NULL},
	{ARPC_ID_SENSOR,		0,						0,						0,			wubSnr_StandbyOut,		wubSnr_StandbyIn, 		wubSnr_EnforceStandby,		NULL},
	{ARPC_ID_R5_KEY,		0,						0,						0,			wubKey_StandbyOut,		wubKey_StandbyIn, 		wubKey_EnforceStandby,		NULL},
	{ARPC_ID_R5_RPC,		0,						0,						0,			wubRPC_Standbyout,		wubRPC_Standbyin, 		NULL,						NULL},
	{ARPC_ID_LINETOOLAPP,	0,						0,						0,			wubLineTool_Standbyout,	wubLineTool_Standbyin, 	wubLineTool_EnforceStandby,	NULL},
	{ARPC_ID_R5_LOGAPP,		0,						0,						0,			wubR5Log_StandbyOut,	wubR5Log_StandbyIn, 	wubR5Log_EnforceStandby,	NULL},
	{ARPC_ID_KLINEDRV,		0,						0,						0,			wubKlineDrv_StandbyOut,	wubKlineDrv_StandbyIn, 	wubKlineDrv_ForceStandby,	NULL},
	{ARPC_ID_LET,			0,						0,						0,			wubLetApp_StandbyOut,	wubLetApp_StandbyIn, 	wubLetApp_ForceStandby,		NULL},
	{ARPC_ID_A53_POWER,		15000,					15000,					0,			nubPmSocStandbyOut,		nubPmSocStandbyIn,		NULL				,		NULL},
};

static const PmPowerKeyIf_ts nstPmPowerKey_Tbl[] = {
/*		ubModuleID				uwOnTime	uwOffTime			onProc_fp			offProc_fp	*/
	{ARPC_ID_LINETOOLAPP,		0,			0,				wubLineTool_PowerOn,	wubLineTool_PowerOff,	},
	//{ARPC_ID_LED,				0,			0,				wubLed_PowerOn,			wubLed_PowerOff,		}, /* 0919*/
	{ARPC_ID_A53_POWER,			1000,		1000,			nubPmSocPowerOn,		nubPmSocPowerOff,		},
};

#if 0
static const PmComAsyncIf_ts nstPmUpdateReset_Tbl[] = {
/*		ubModuleID				uwTime			proc_fp				*/
	{ARPC_ID_A53_POWER,			1000,			nubPmSocUpdateReset,			},
	{ARPC_ID_LINETOOLAPP,		0,				wubLineTool_Standbyin,			},
	{ARPC_ID_R5_LOGAPP,			0,				wubR5Log_StandbyIn,				},
	{ARPC_ID_R5_BACKUPRAM,		0,				wubBackup_AllClear,				},
};

static const PmComAsyncIf_ts nstPmFactoryReset_Tbl[] = {
/*		ubModuleID				uwTime			proc_fp				*/
	{ARPC_ID_A53_POWER,			1000,			nubPmSocFactoryReset,			},
	{ARPC_ID_LINETOOLAPP,		0,				wubLineTool_Standbyin,			},
	{ARPC_ID_R5_LOGAPP,			0,				wubR5Log_StandbyIn,				},
	{ARPC_ID_R5_BACKUPRAM,		0,				wubBackup_AllClear,				},
};
#endif

static const Type_uWord nuwModulePriority_Tbl[PM_PRIORITY_LINE_MAX][PM_PRIORITY_ROW_MAX] = {
	{ARPC_ID_R5_POWERDRV,	PM_MODULE_NONE,			PM_MODULE_NONE,	PM_MODULE_NONE,	PM_MODULE_NONE,			PM_MODULE_NONE,		PM_MODULE_NONE,			PM_MODULE_NONE},
	{ARPC_ID_EEPROM,		ARPC_ID_R5_BACKUPRAM,	ARPC_ID_R5_RPC,	PM_MODULE_NONE,	PM_MODULE_NONE,			PM_MODULE_NONE,		PM_MODULE_NONE,			PM_MODULE_NONE},
	{ARPC_ID_R5_AGENT,		ARPC_ID_KLINEDRV,		PM_MODULE_NONE,	PM_MODULE_NONE,	PM_MODULE_NONE,			PM_MODULE_NONE,		PM_MODULE_NONE,			PM_MODULE_NONE},
	{ARPC_ID_SENSOR,		ARPC_ID_LED,			ARPC_ID_LET,	ARPC_ID_LINETOOLAPP,	ARPC_ID_R5_LOGAPP,	ARPC_ID_A53_POWER,	PM_MODULE_NONE,		PM_MODULE_NONE},
	{ARPC_ID_R5_KEY,		ARPC_ID_R5_SOFTWDT,		PM_MODULE_NONE,	PM_MODULE_NONE,	PM_MODULE_NONE,			PM_MODULE_NONE,		PM_MODULE_NONE,			PM_MODULE_NONE},
};

static const PmCtrlProc_ts nstPmProcCtrl_Tbl[] = {
	/*	ModuleID			Event							SocStateMask			StandbyMask										ResetMask				PowerKeyMask										NextSocState			NextStandbyState			NextResetState			NextPowerKeyState		proc_fp				*/

	/* StanbyOut */
	{ARPC_ID_A53_POWER, 	EV_INF_A53_TO_R5_START, 		PM_STATE_NONE,			PM_STATE_ANY,									PM_STATE_ANY,			PM_STATE_ANY,										PM_SOC_INIT_OK, 		PM_STATE_NO_CHANGE, 		PM_STATE_NO_CHANGE, 	PM_STATE_NO_CHANGE, 	nvdPmSocInitReceive},		/*-*/
	{ARPC_ID_R5_POWERDRV,	EV_INF_PM_STANDBYOUT_START, 	PM_STATE_ANY,			PM_STATE_NONE, 									PM_STATE_NONE,			PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STANDBY_STANDBYOUTING, 	PM_STATE_NO_CHANGE, 	PM_STATE_NONE,			nvdPmStandbyOutCtrlStart},	/*1*/
	{ARPC_ID_R5_POWERMNG,	EV_INF_PM_STANDBYOUT_ING_MCU, 	PM_STATE_ANY,			PM_STANDBY_STANDBYOUTING,						PM_STATE_NONE,			PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE,			PM_STATE_NO_CHANGE, 	PM_STATE_NONE,			nvdPmStandbyOutMcuCtrlProc},/*2*/
	{ARPC_ID_R5_POWERMNG,	EV_INF_PM_STANDBYOUT_ING_SOC, 	PM_SOC_INIT_OK,			PM_STANDBY_STANDBYOUTING,						PM_STATE_NONE,			PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE,			PM_STATE_NO_CHANGE, 	PM_STATE_NONE,			nvdPmStandbyOutSocCtrlProc},/*3*/
	{ARPC_ID_R5_POWERMNG,	EV_INF_PM_STANDBYOUT_ING_SOC, 	PM_STATE_NONE,			PM_STANDBY_STANDBYOUTING,						PM_STATE_NONE,			PM_STATE_ANY,										PM_SOC_INIT_OK_WAIT,	PM_STATE_NO_CHANGE,			PM_STATE_NO_CHANGE, 	PM_STATE_NONE,			NULL},						/*-*/
	{ARPC_ID_ANY,			EV_INF_STANDBYOUT_FINISH, 		PM_STATE_ANY,			PM_STANDBY_STANDBYOUTING, 						PM_STATE_NONE,			PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE, 		PM_STATE_NO_CHANGE, 	PM_STATE_NONE,			nvdPmStandbyOutResCtrl},	/*4*/
	{ARPC_ID_R5_POWERMNG,	EV_INF_PM_STANDBYOUT_OVER, 		PM_SOC_INIT_OK,			PM_STANDBY_STANDBYOUTING, 						PM_STATE_NONE,			PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STANDBY_STANDBYOUT, 		PM_STATE_NO_CHANGE, 	PM_STATE_NONE,			wvdPmStandbyOutOver},		/*5*/
	{ARPC_ID_A53_POWER,		EV_INF_A53_TO_R5_START, 		PM_SOC_INIT_OK_WAIT,	PM_STANDBY_STANDBYOUTING,						PM_STATE_NONE,			PM_STATE_ANY,										PM_SOC_INIT_OK,			PM_STATE_NO_CHANGE,			PM_STATE_NO_CHANGE, 	PM_STATE_NONE,			nvdPmStandbyOutSocCtrlStart},/*-*/
#if 0
	{ARPC_ID_R5_POWERDRV,	EV_INF_PM_STANDBYOUT_START, 	PM_STATE_ANY,			PM_STANDBY_STANDBYINING, 						PM_STATE_NONE,			PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STATE_NONE, 				PM_STATE_NO_CHANGE, 	PM_STATE_NONE,			nvdPmStandbyIningResetCtrl},
#endif
	{ARPC_ID_R5_POWERDRV,	EV_INF_PM_STANDBYOUT_START, 	PM_STATE_ANY,			PM_STANDBY_STANDBYIN, 							PM_STATE_NONE,			PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STATE_NONE, 				PM_STATE_NO_CHANGE, 	PM_STATE_NONE,			nvdPmSysAccResetCtrl},/*-STANDBYINING STANDBYOUT RES*/

	/* StanbyIn  */
	{ARPC_ID_R5_POWERDRV,	EV_INF_PM_STANDBYIN_START, 		PM_STATE_ANY,			PM_STATE_NONE, 									PM_STATE_NONE,			PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STATE_NONE, 				PM_STATE_NONE, 			PM_STATE_NONE,			nvdPmSysDownCtrl},
	{ARPC_ID_R5_POWERDRV,	EV_INF_PM_STANDBYIN_START, 		PM_STATE_ANY,			PM_STANDBY_STANDBYOUTING,						PM_STATE_NONE,			PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STATE_NONE, 				PM_STATE_NONE, 			PM_STATE_NONE,			nvdPmAccOff},
	{ARPC_ID_R5_POWERDRV,	EV_INF_PM_STANDBYIN_START, 		PM_STATE_ANY,			PM_STANDBY_STANDBYOUT, 							PM_STATE_NONE,			PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STANDBY_STANDBYINING,	PM_STATE_NONE, 			PM_STATE_NONE,			nvdPmAccStandbyInStart},/*1*/
	{ARPC_ID_R5_POWERMNG,	EV_INF_PM_STANDBYIN_ING, 		PM_STATE_ANY,			PM_STANDBY_STANDBYINING, 						PM_STATE_NONE,			PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE, 		PM_STATE_NONE, 			PM_STATE_NONE,			nvdPmStandbyInCtrlProc},/*2*/
	{ARPC_ID_ANY,			EV_INF_STANDBYIN_FINISH, 		PM_STATE_ANY,			PM_STANDBY_STANDBYINING, 						PM_STATE_NONE,			PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE, 		PM_STATE_NONE, 			PM_STATE_NONE,			nvdPmStandbyInResCtrl}, /*3*/
	{ARPC_ID_R5_POWERMNG,	EV_INF_PM_STANDBYIN_OVER, 		PM_STATE_ANY,			PM_STANDBY_STANDBYINING, 						PM_STATE_NONE,			PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STANDBY_STANDBYIN, 		PM_STATE_NONE, 			PM_STATE_NONE,			nvdPmStandbyInOver},	/*4*/

	/* Update Reset */
	{ARPC_ID_UPDATASTM,		EV_INF_UPDATE_UPDATE_RESET_REQ,	PM_SOC_INIT_OK,			PM_STANDBY_STANDBYOUT,							PM_STATE_NONE,			PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE,			PM_UPDATE_RESET_ING,	PM_STATE_NONE,			nvdPmUpdateStandbyInStart},
	{ARPC_ID_R5_POWERMNG,	EV_INF_PM_STANDBYIN_ING,		PM_SOC_INIT_OK,			PM_STANDBY_STANDBYOUT,							PM_UPDATE_RESET_ING,	PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE,			PM_STATE_NO_CHANGE,		PM_STATE_NONE,			nvdPmStandbyInCtrlProc},
	{ARPC_ID_ANY,			EV_INF_STANDBYIN_FINISH,		PM_SOC_INIT_OK,			PM_STANDBY_STANDBYOUT,							PM_UPDATE_RESET_ING,	PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE,			PM_STATE_NO_CHANGE,		PM_STATE_NONE,			nvdPmStandbyInResCtrl},
	{ARPC_ID_R5_POWERMNG,	EV_INF_PM_STANDBYIN_OVER,		PM_SOC_INIT_OK,			PM_STANDBY_STANDBYOUT,							PM_UPDATE_RESET_ING,	PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE,			PM_RESET_OVER,			PM_STATE_NONE,			nvdPmSysUpdateResetCtrl},

	{ARPC_ID_HMI,			EV_INF_HMI_UPDATE_RESET_REQ,	PM_SOC_INIT_OK,			PM_STANDBY_STANDBYOUTING|PM_STANDBY_STANDBYOUT,	PM_STATE_NONE,			PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE,			PM_UPDATE_RESET_ING,	PM_STATE_NONE,			nvdPmUpdateInitStandbyInStart},
	{ARPC_ID_R5_POWERMNG,	EV_INF_PM_STANDBYIN_ING,		PM_SOC_INIT_OK,			PM_STANDBY_STANDBYOUTING|PM_STANDBY_STANDBYOUT,	PM_UPDATE_RESET_ING,	PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE,			PM_STATE_NO_CHANGE,		PM_STATE_NONE,			nvdPmStandbyInCtrlProc},
	{ARPC_ID_ANY,			EV_INF_STANDBYIN_FINISH,		PM_SOC_INIT_OK,			PM_STANDBY_STANDBYOUTING|PM_STANDBY_STANDBYOUT,	PM_UPDATE_RESET_ING,	PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE,			PM_STATE_NO_CHANGE,		PM_STATE_NONE,			nvdPmStandbyInResCtrl},
	{ARPC_ID_R5_POWERMNG,	EV_INF_PM_STANDBYIN_OVER,		PM_SOC_INIT_OK,			PM_STANDBY_STANDBYOUTING|PM_STANDBY_STANDBYOUT,	PM_UPDATE_RESET_ING,	PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE,			PM_RESET_OVER,			PM_STATE_NONE,			nvdPmSysUpdateResetCtrl},

	/* Factory Reset */
	{ARPC_ID_HMI,			EV_INF_HMI_FACTORY_RESET_REQ,	PM_SOC_INIT_OK,			PM_STANDBY_STANDBYOUT,							PM_STATE_NONE,			PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE,			PM_FACTORY_RESET_ING,	PM_STATE_NONE,			nvdPmFactoryStandbyInStart},
	{ARPC_ID_R5_POWERMNG,	EV_INF_PM_STANDBYIN_ING,		PM_SOC_INIT_OK,			PM_STANDBY_STANDBYOUT,							PM_FACTORY_RESET_ING,	PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE,			PM_STATE_NO_CHANGE,		PM_STATE_NONE,			nvdPmStandbyInCtrlProc},
	{ARPC_ID_ANY,			EV_INF_STANDBYIN_FINISH,		PM_SOC_INIT_OK,			PM_STANDBY_STANDBYOUT,							PM_FACTORY_RESET_ING,	PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE,			PM_STATE_NO_CHANGE,		PM_STATE_NONE,			nvdPmStandbyInResCtrl},
	{ARPC_ID_R5_POWERMNG,	EV_INF_PM_STANDBYIN_OVER,		PM_SOC_INIT_OK,			PM_STANDBY_STANDBYOUT,							PM_FACTORY_RESET_ING,	PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE,			PM_RESET_OVER,			PM_STATE_NONE,			nvdPmSysFactoryResetCtrl},
#if 0
		/* Update Reset */
		{ARPC_ID_UPDATASTM, 	EV_INF_UPDATE_UPDATE_RESET_REQ, PM_SOC_INIT_OK, 		PM_STANDBY_STANDBYOUT,					PM_STATE_NONE,			PM_STATE_ANY,										PM_STATE_NO_CHANGE, 	PM_STATE_NO_CHANGE, 		PM_RESET_ING,			PM_STATE_NONE,			nvdPmUpdateResetCtrlStart},
		{ARPC_ID_R5_POWERMNG,	EV_INF_PM_UPDATE_RESET_ING, 	PM_SOC_INIT_OK, 		PM_STANDBY_STANDBYOUT,					PM_RESET_ING,			PM_STATE_ANY,										PM_STATE_NO_CHANGE, 	PM_STATE_NO_CHANGE, 		PM_STATE_NO_CHANGE, 	PM_STATE_NONE,			nvdPmUpdateResetCtrlProc},
		{ARPC_ID_ANY,			EV_INF_UPDATE_RESET_FINISH, 	PM_SOC_INIT_OK, 		PM_STANDBY_STANDBYOUT,					PM_RESET_ING,			PM_STATE_ANY,										PM_STATE_NO_CHANGE, 	PM_STATE_NO_CHANGE, 		PM_STATE_NO_CHANGE, 	PM_STATE_NONE,			nvdPmUpdateResetResCtrl},
		{ARPC_ID_R5_POWERMNG,	EV_INF_PM_UPDATE_RESET_OVER,	PM_SOC_INIT_OK, 		PM_STANDBY_STANDBYOUT,					PM_RESET_ING,			PM_STATE_ANY,										PM_STATE_NO_CHANGE, 	PM_STATE_NO_CHANGE, 		PM_STATE_NONE,			PM_STATE_NONE,			nvdPmSysResetCtrl},
	
		/* Factory Reset */
		{ARPC_ID_HMI,			EV_INF_HMI_FACTORY_RESET_REQ,	PM_SOC_INIT_OK, 		PM_STANDBY_STANDBYOUT,					PM_STATE_NONE,			PM_STATE_ANY,										PM_STATE_NO_CHANGE, 	PM_STATE_NO_CHANGE, 		PM_RESET_ING,			PM_STATE_NONE,			nvdPmFactoryResetCtrlStart},
		{ARPC_ID_R5_POWERMNG,	EV_INF_PM_FACTORY_RESET_ING,	PM_SOC_INIT_OK, 		PM_STANDBY_STANDBYOUT,					PM_RESET_ING,			PM_STATE_ANY,										PM_STATE_NO_CHANGE, 	PM_STATE_NO_CHANGE, 		PM_STATE_NO_CHANGE, 	PM_STATE_NONE,			nvdPmFactoryResetCtrlProc},
		{ARPC_ID_ANY,			EV_INF_FACTORY_RESET_FINISH,	PM_SOC_INIT_OK, 		PM_STANDBY_STANDBYOUT,					PM_RESET_ING,			PM_STATE_ANY,										PM_STATE_NO_CHANGE, 	PM_STATE_NO_CHANGE, 		PM_STATE_NO_CHANGE, 	PM_STATE_NONE,			nvdPmFactoryResetResCtrl},
		{ARPC_ID_R5_POWERMNG,	EV_INF_PM_FACTORY_RESET_OVER,	PM_SOC_INIT_OK, 		PM_STANDBY_STANDBYOUT,					PM_RESET_ING,			PM_STATE_ANY,										PM_STATE_NO_CHANGE, 	PM_STATE_NO_CHANGE, 		PM_STATE_NONE,			PM_STATE_NONE,			nvdPmSysResetCtrl},
#endif

	/* PowerKey ON/OFF */
	{ARPC_ID_HMI,			EV_INF_HMI_POWERON_REQ,			PM_SOC_INIT_OK,			PM_STANDBY_STANDBYOUT,					PM_STATE_NONE,			PM_STATE_NONE|PM_POWERKEY_OFFING,					PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE,			PM_STATE_NO_CHANGE,		PM_POWERKEY_ONING,		nvdPmPowerKeyOnCtrlStart},
	{ARPC_ID_R5_POWERMNG,	EV_INF_PM_POWERON_ING, 			PM_SOC_INIT_OK,			PM_STANDBY_STANDBYOUT, 					PM_STATE_NONE,			PM_POWERKEY_ONING,									PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE, 		PM_STATE_NO_CHANGE, 	PM_STATE_NO_CHANGE,		nvdPmPowerKeyOnCtrlProc},
	{ARPC_ID_ANY,			EV_INF_POWERON_FINISH, 			PM_SOC_INIT_OK,			PM_STANDBY_STANDBYOUT, 					PM_STATE_NONE,			PM_POWERKEY_ONING,									PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE, 		PM_STATE_NO_CHANGE, 	PM_STATE_NO_CHANGE,		nvdPmPowerKeyOnResCtrl},
	{ARPC_ID_R5_POWERMNG,	EV_INF_PM_POWERON_OVER, 		PM_SOC_INIT_OK,			PM_STANDBY_STANDBYOUT, 					PM_STATE_NONE,			PM_POWERKEY_ONING,									PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE, 		PM_STATE_NO_CHANGE, 	PM_STATE_NONE,			NULL},

	{ARPC_ID_HMI,			EV_INF_HMI_POWEROFF_REQ, 		PM_SOC_INIT_OK,			PM_STANDBY_STANDBYOUT, 					PM_STATE_NONE,			PM_STATE_NONE|PM_POWERKEY_ONING,					PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE, 		PM_STATE_NO_CHANGE, 	PM_POWERKEY_OFFING,		nvdPmPowerKeyOffCtrlStart},
	{ARPC_ID_R5_POWERMNG,	EV_INF_PM_POWEROFF_ING,			PM_SOC_INIT_OK,			PM_STANDBY_STANDBYOUT, 					PM_STATE_NONE,			PM_POWERKEY_OFFING,									PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE, 		PM_STATE_NO_CHANGE, 	PM_STATE_NO_CHANGE, 	nvdPmPowerKeyOffCtrlProc},
	{ARPC_ID_ANY,			EV_INF_POWEROFF_FINISH,			PM_SOC_INIT_OK,			PM_STANDBY_STANDBYOUT, 					PM_STATE_NONE,			PM_POWERKEY_OFFING,									PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE, 		PM_STATE_NO_CHANGE, 	PM_STATE_NO_CHANGE, 	nvdPmPowerKeyOffResCtrl},
	{ARPC_ID_R5_POWERMNG,	EV_INF_PM_POWEROFF_OVER, 		PM_SOC_INIT_OK,			PM_STANDBY_STANDBYOUT, 					PM_STATE_NONE,			PM_POWERKEY_OFFING,									PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE, 		PM_STATE_NO_CHANGE, 	PM_STATE_NONE,			NULL},


	/* GTimout */
	{ARPC_ID_R5_POWERMNG,	EV_INF_PM_SOCINIT_GTIMEROUT,		PM_SOC_INIT_OK_WAIT, 	PM_STANDBY_STANDBYOUTING,			PM_STATE_NONE,			PM_STATE_ANY,										PM_STATE_NO_CHANGE, 	PM_STATE_NO_CHANGE, 		PM_STATE_NO_CHANGE,		PM_STATE_NONE,			nvdPmSocStartErrReset},
	{ARPC_ID_R5_POWERMNG,	EV_INF_PM_SOCSTANDBYOUT_GTIMEROUT,	PM_SOC_INIT_OK, 		PM_STANDBY_STANDBYOUTING,			PM_STATE_NONE,			PM_STATE_ANY,										PM_STATE_NO_CHANGE, 	PM_STATE_NO_CHANGE, 		PM_STATE_NO_CHANGE,		PM_STATE_NONE,			nvdPmSocStandbyOutErrReset},

	
	/* Soft WatchDog Reset */
	/* System A53 WatchDog Reset */
	{ARPC_ID_OS,			EV_INF_PM_A53WDT_REBOOT_REQ,		PM_SOC_INIT_OK,			PM_STANDBY_STANDBYOUT,					PM_STATE_NONE,			PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE,			PM_A53WDT_RESET_ING,	PM_STATE_NONE,			nvdPmWdtA53StandbyInStart},
	{ARPC_ID_R5_POWERMNG,	EV_INF_PM_STANDBYIN_ING,			PM_SOC_INIT_OK,			PM_STANDBY_STANDBYOUT,					PM_A53WDT_RESET_ING,	PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE,			PM_STATE_NO_CHANGE,		PM_STATE_NONE,			nvdPmStandbyInCtrlProc},
	{ARPC_ID_ANY,			EV_INF_STANDBYIN_FINISH,			PM_SOC_INIT_OK,			PM_STANDBY_STANDBYOUT,					PM_A53WDT_RESET_ING,	PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE,			PM_STATE_NO_CHANGE,		PM_STATE_NONE,			nvdPmStandbyInResCtrl},
	{ARPC_ID_R5_POWERMNG,	EV_INF_PM_STANDBYIN_OVER,			PM_SOC_INIT_OK,			PM_STANDBY_STANDBYOUT,					PM_A53WDT_RESET_ING,	PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE,			PM_RESET_OVER,			PM_STATE_NONE,			nvdPmSysA53WdtResetCtrl},

	{ARPC_ID_OS,			EV_INF_PM_A53WDT_REBOOT_REQ,		PM_STATE_ANY, 			PM_STATE_NONE|PM_STANDBY_STANDBYOUTING,	PM_STATE_NONE,			PM_STATE_ANY,										PM_STATE_NO_CHANGE, 	PM_STATE_NO_CHANGE, 		PM_STATE_NO_CHANGE,		PM_STATE_NONE,			nvdPmA53WdtErrResetNow},

	/* System A7 WatchDog Reset */
	{ARPC_ID_OS,			EV_INF_PM_A7WDT_REBOOT_REQ,			PM_SOC_INIT_OK,			PM_STANDBY_STANDBYOUT,					PM_STATE_NONE,			PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE,			PM_A7WDT_RESET_ING,		PM_STATE_NONE,			nvdPmWdtA7StandbyInStart},
	{ARPC_ID_R5_POWERMNG,	EV_INF_PM_STANDBYIN_ING,			PM_SOC_INIT_OK,			PM_STANDBY_STANDBYOUT,					PM_A7WDT_RESET_ING,		PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE,			PM_STATE_NO_CHANGE,		PM_STATE_NONE,			nvdPmStandbyInCtrlProc},
	{ARPC_ID_ANY,			EV_INF_STANDBYIN_FINISH,			PM_SOC_INIT_OK,			PM_STANDBY_STANDBYOUT,					PM_A7WDT_RESET_ING,		PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE,			PM_STATE_NO_CHANGE,		PM_STATE_NONE,			nvdPmStandbyInResCtrl},
	{ARPC_ID_R5_POWERMNG,	EV_INF_PM_STANDBYIN_OVER,			PM_SOC_INIT_OK,			PM_STANDBY_STANDBYOUT,					PM_A7WDT_RESET_ING,		PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE,			PM_RESET_OVER,			PM_STATE_NONE,			nvdPmSysA7WdtResetCtrl},

	{ARPC_ID_OS,			EV_INF_PM_A7WDT_REBOOT_REQ,			PM_STATE_ANY,			PM_STATE_NONE|PM_STANDBY_STANDBYOUTING, PM_STATE_NONE,			PM_STATE_ANY,										PM_STATE_NO_CHANGE, 	PM_STATE_NO_CHANGE, 		PM_STATE_NO_CHANGE, 	PM_STATE_NONE,			nvdPmA7WdtErrResetNow},

	/* A53 Heart Reset Req*/
	{ARPC_ID_R5_SOFTWDT,	EV_INF_PM_A53HEAT_REBOOT_REQ,		PM_SOC_INIT_OK,			PM_STANDBY_STANDBYOUT,					PM_STATE_NONE,			PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE,			PM_A53REQ_RESET_ING,	PM_STATE_NONE,			nvdPmA53ReqStandbyInStart},
	{ARPC_ID_R5_POWERMNG,	EV_INF_PM_STANDBYIN_ING,			PM_SOC_INIT_OK,			PM_STANDBY_STANDBYOUT,					PM_A53REQ_RESET_ING,	PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE,			PM_STATE_NO_CHANGE,		PM_STATE_NONE,			nvdPmStandbyInCtrlProc},
	{ARPC_ID_ANY,			EV_INF_STANDBYIN_FINISH,			PM_SOC_INIT_OK,			PM_STANDBY_STANDBYOUT,					PM_A53REQ_RESET_ING,	PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE,			PM_STATE_NO_CHANGE,		PM_STATE_NONE,			nvdPmStandbyInResCtrl},
	{ARPC_ID_R5_POWERMNG,	EV_INF_PM_STANDBYIN_OVER,			PM_SOC_INIT_OK,			PM_STANDBY_STANDBYOUT,					PM_A53REQ_RESET_ING,	PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE,			PM_RESET_OVER,			PM_STATE_NONE,			nvdPmSysA53ReqResetCtrl},
	/* A7 Heart Reset Req*/
	{ARPC_ID_R5_SOFTWDT,	EV_INF_PM_A7HEART_REBOOT_REQ,		PM_SOC_INIT_OK,			PM_STANDBY_STANDBYOUT,					PM_STATE_NONE,			PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE,			PM_A7REQ_RESET_ING,		PM_STATE_NONE,			nvdPmA7ReqStandbyInStart},
	{ARPC_ID_R5_POWERMNG,	EV_INF_PM_STANDBYIN_ING,			PM_SOC_INIT_OK,			PM_STANDBY_STANDBYOUT,					PM_A7REQ_RESET_ING,		PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE,			PM_STATE_NO_CHANGE,		PM_STATE_NONE,			nvdPmStandbyInCtrlProc},
	{ARPC_ID_ANY,			EV_INF_STANDBYIN_FINISH,			PM_SOC_INIT_OK,			PM_STANDBY_STANDBYOUT,					PM_A7REQ_RESET_ING,		PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE,			PM_STATE_NO_CHANGE,		PM_STATE_NONE,			nvdPmStandbyInResCtrl},
	{ARPC_ID_R5_POWERMNG,	EV_INF_PM_STANDBYIN_OVER,			PM_SOC_INIT_OK,			PM_STANDBY_STANDBYOUT,					PM_A7REQ_RESET_ING,		PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE,			PM_RESET_OVER,			PM_STATE_NONE,			nvdPmSysA7ReqResetCtrl},
	/* A53 Heart Timout Reset */
	{ARPC_ID_R5_SOFTWDT,	EV_INF_PM_A53HEART_TIMOUT_REBOOT,	PM_SOC_INIT_OK,			PM_STANDBY_STANDBYOUT,					PM_STATE_NONE,			PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE,			PM_A53HEART_RESET_ING,	PM_STATE_NONE,			nvdPmA53HeartStandbyInStart},
	{ARPC_ID_R5_POWERMNG,	EV_INF_PM_STANDBYIN_ING,			PM_SOC_INIT_OK,			PM_STANDBY_STANDBYOUT,					PM_A53HEART_RESET_ING,	PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE,			PM_STATE_NO_CHANGE,		PM_STATE_NONE,			nvdPmStandbyInCtrlProc},
	{ARPC_ID_ANY,			EV_INF_STANDBYIN_FINISH,			PM_SOC_INIT_OK,			PM_STANDBY_STANDBYOUT,					PM_A53HEART_RESET_ING,	PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE,			PM_STATE_NO_CHANGE,		PM_STATE_NONE,			nvdPmStandbyInResCtrl},
	{ARPC_ID_R5_POWERMNG,	EV_INF_PM_STANDBYIN_OVER,			PM_SOC_INIT_OK,			PM_STANDBY_STANDBYOUT,					PM_A53HEART_RESET_ING,	PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE,			PM_RESET_OVER,			PM_STATE_NONE,			nvdPmSysA53HeartResetCtrl},
	/* A7 Heart Timout Reset */
	{ARPC_ID_R5_SOFTWDT,	EV_INF_PM_A7HEART_TIMOUT_REBOOT,	PM_SOC_INIT_OK,			PM_STANDBY_STANDBYOUT,					PM_STATE_NONE,			PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE,			PM_A7HEART_RESET_ING,	PM_STATE_NONE,			nvdPmA7HeartStandbyInStart},
	{ARPC_ID_R5_POWERMNG,	EV_INF_PM_STANDBYIN_ING,			PM_SOC_INIT_OK,			PM_STANDBY_STANDBYOUT,					PM_A7HEART_RESET_ING,	PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE,			PM_STATE_NO_CHANGE,		PM_STATE_NONE,			nvdPmStandbyInCtrlProc},
	{ARPC_ID_ANY,			EV_INF_STANDBYIN_FINISH,			PM_SOC_INIT_OK,			PM_STANDBY_STANDBYOUT,					PM_A7HEART_RESET_ING,	PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE,			PM_STATE_NO_CHANGE,		PM_STATE_NONE,			nvdPmStandbyInResCtrl},
	{ARPC_ID_R5_POWERMNG,	EV_INF_PM_STANDBYIN_OVER,			PM_SOC_INIT_OK,			PM_STANDBY_STANDBYOUT,					PM_A7HEART_RESET_ING,	PM_STATE_ANY,										PM_STATE_NO_CHANGE,		PM_STATE_NO_CHANGE,			PM_RESET_OVER,			PM_STATE_NONE,			nvdPmSysA7HeartResetCtrl},

};


/********************************************************************************************/
/*							Static Variable Definition Section								*/
/********************************************************************************************/
static Type_uByte		nubStartType = STANDBYOUT_TYPE_WARM_RESET;
static Type_uByte 		nubPmPriorityLineIndex = PM_DATA_ZERO;
static Type_uWord 		nuwPmModuleResFlg = PM_DATA_ZERO;
static Type_uByte 		nubPmPowerKeyPriorityLineIndex = PM_DATA_ZERO;
static Type_uWord 		nuwPmPowerKeyModuleResFlg = PM_DATA_ZERO;
#ifdef PM_STANDBYOUT_RETRY
static Type_uByte 		nubPmSocStandbyOutRetryNum = PM_DATA_ZERO;
#endif
static Type_uByte		nubPmSocPriorityLine = PM_PRIORITY_LINE_ARRAY_MAX;
static PmStateMng_ts	nstPmStateMng;
static HAL_HANDLE 		nvbPmHalHandle = STD_NULL;
static PmStartInfo_ts	nstPmStartInfo;
#ifdef PM_ST_EN
static Type_sWord		nubPmStNo = PM_ST_INIT_NO;
#endif

/********************************************************************************************/
/*							Table Definition Section										*/
/********************************************************************************************/


/********************************************************************************************/
/********************************************************************************************/
/**	\function		TSK_POWER
 *	\date			2021/11/11
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
void TSK_POWER(void)
{
	OS_ErrCode_t 	auwRst;
	OS_StdQueue_ts 	astMsg; 
	OS_StdQueue_ts * astMsg_p = &astMsg;
	Type_sByte 		asbRet;
	
	nvdPmInit();
#ifndef PM_HAL_BEFORE_OS
	nvdPmHalOpen();
#endif
	asbRet = wsbRPCInit(ARPC_ID_R5_POWERMNG, (wvdRPCCallback)nvdPmRpcRcv);
	if(asbRet != RPC_OK){
		PM_DBG("\n######## PM RPC Init Failed:%d#######\n", asbRet);
		LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL,ATAT_LEVEL_OFF,"RPC Init Failed:%d", asbRet);
	}

	for( ; ; )
	{		
		auwRst = OS_ReceiveQueue(MBX_ID_POWER, OS_TOUT_FOREVER, &astMsg_p);
		
		if (auwRst == OS_ERR_OK)
		{	
			nvdPmProcCtrl(&astMsg);
		
			if((astMsg.uwQueueSize > PM_DATA_ZERO) && (NULL != astMsg.vdQueueBuffer_p))
			{
				auwRst = OS_ReleaseFMemoryBlock(astMsg.vdQueueBuffer_p);
				if (auwRst != OS_ERR_OK){
					PM_DBG("\n######## PM Failed to release memory:%d#######\n", auwRst);
					LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL,ATAT_LEVEL_OFF,"Failed to release memory:%d", auwRst);
				}

			}
		}else{
			LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL,ATAT_LEVEL_OFF,"Failed to OS_ReceiveQueue:%d", auwRst);
		}
	}
 
}

/********************************************************************************************/
/**	\function		wvdPmHalOpen
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	Power Hal Open
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
Type_uWord wubPmGetStartType(void)
{
	wvdGen_MemSet(&nstPmStartInfo, PM_DATA_ZERO, sizeof(nstPmStartInfo));
	nstPmStartInfo.uwType = wubGetStartInfo(&nstPmStartInfo.rstSts0, &nstPmStartInfo.rstSts1, &nstPmStartInfo.rstSts2);
	return nstPmStartInfo.uwType;
}

/********************************************************************************************/
/**	\function		wvdPmHalOpen
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	Power Hal Open
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
void wvdPmHalOpen(void)
{
	Type_uByte aubWaitMode = FALSE;
	Type_uWord auwWaitTime = PM_DATA_ZERO;
	Type_sWord aswRet = HAL_OK;

	nvbPmHalHandle = HAL_Open(HAL_POWER_MANAGER, HAL_PM_OPTION_NORMAL);
	if(NULL == nvbPmHalHandle){
		PM_DBG("\n######## PM Fail to open Hal#######\n");
		wvdEEPLOGWrite( TYPE_PMMNG, PM_EEP_ERROR_MNG_HALOPEN );
		return;
	}

	aswRet = HAL_Ioctl(nvbPmHalHandle, HAL_IO_PM_SET_POWERDOWN_WAIT_INFO, &aubWaitMode, &auwWaitTime, 0, 0);
	if(HAL_OK != aswRet){
		PM_DBG("\n######## PM Fail to HAL_Ioctl（POWERDOWN_WAIT_INFO）:%d#######\n", aswRet);
		wvdEEPLOGWrite( TYPE_PMMNG, PM_EEP_ERROR_MNG_HALCTRL_DOWNWAIT );
	}

	aswRet = HAL_SetCallbackFunction(nvbPmHalHandle, (pfCallback)wvdPmBattOff);
	if(HAL_OK != aswRet){
		PM_DBG("\n######## PM Fail to HAL_Ioctl（SetCallback）:%d#######\n", aswRet);
		wvdEEPLOGWrite( TYPE_PMMNG, PM_EEP_ERROR_MNG_HALCTRL_SETCALLBACK );
	}

	aswRet = HAL_Ioctl(nvbPmHalHandle, HAL_IO_PM_SET_INTERRUPT, 0, 0, 0, 0);	
	if(HAL_OK != aswRet){
		PM_DBG("\n######## PM Fail to HAL_Ioctl（SET_INTERRUPT）:%d#######\n", aswRet);
		wvdEEPLOGWrite( TYPE_PMMNG, PM_EEP_ERROR_MNG_HALCTRL_SETINT );
	}

}

/********************************************************************************************/
/**	\function		wvdPmBattSysDown
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	Batt PowerDown
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
void wvdPmBattSysDown(void)
{
	Type_uByte nubType = PM_PWRDN_BU_OFF;
	Type_sWord aswRet = HAL_OK;
		
	//PM_DBG("\n********************Batt PowerDown*********************\n");
	
	aswRet = HAL_Ioctl(nvbPmHalHandle, HAL_IO_PM_ENTER_POWERDOWN, &nubType, 0, 0, 0);
	if(HAL_OK != aswRet){
		PM_DBG("\n######## PM Fail to HAL_Ioctl（ENTER_POWERDOWN）:%d#######\n", aswRet);
		wvdEEPLOGWrite( TYPE_PMMNG, PM_EEP_ERROR_MNG_HALCTRL_POWERDOWN );
	}

	/* Close Hal Power */
	HAL_Close(nvbPmHalHandle);

}

/********************************************************************************************/
/**	\function		wvdPmBattOff
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	Batt Off Interrupt
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
void wvdPmBattOff(Type_uByte aubData1, Type_uByte aubData2)
{
	Type_uByte aubProcIndex = PM_DATA_ZERO;
	Type_uByte aubType = PM_PWRDN_BU_OFF;

	for(aubProcIndex = PM_DATA_ZERO; aubProcIndex < (sizeof(nstPmStandby_Tbl)/sizeof(PmStandbyIf_ts)); aubProcIndex++){
		if(nstPmStandby_Tbl[aubProcIndex].forceStandbyProc_fp != NULL){
			(void)nstPmStandby_Tbl[aubProcIndex].forceStandbyProc_fp();
		}
	}

#ifdef PM_ST_EN
			if(nubPmStNo == PM_ST_NO_2120){
				gpio_set(PM_A2_NC, GPIO_HIGH);
				printf("\n@@@@@[%s] ST NO is %x \n", __func__, nubPmStNo);
			}
#endif

	(void)HAL_Ioctl(nvbPmHalHandle, HAL_IO_PM_ENTER_POWERDOWN, &aubType, 0, 0, 0);
	(void)HAL_Close(nvbPmHalHandle);

}

#else
void nvdPmHalOpen(void)
{
	Type_uByte aubWaitMode = FALSE;
	Type_uWord auwWaitTime = PM_DATA_ZERO;
	Type_sWord aswRet = HAL_OK;

	nvbPmHalHandle = HAL_Open(HAL_POWER_MANAGER, HAL_PM_OPTION_NORMAL);
	if(NULL == nvbPmHalHandle){
		PM_DBG("\n######## PM Fail to open Hal#######\n");
		wvdEEPLOGWrite( TYPE_PMMNG, PM_EEP_ERROR_MNG_HALOPEN );
		return;
	}

	aswRet = HAL_Ioctl(nvbPmHalHandle, HAL_IO_PM_SET_POWERDOWN_WAIT_INFO, &aubWaitMode, &auwWaitTime, 0, 0);
	if(HAL_OK != aswRet){
		PM_DBG("\n######## PM Fail to HAL_Ioctl（POWERDOWN_WAIT_INFO）:%d#######\n", aswRet);
		wvdEEPLOGWrite( TYPE_PMMNG, PM_EEP_ERROR_MNG_HALCTRL_DOWNWAIT );
	}

}

/********************************************************************************************/
/**	\function		wvdPmBattIrqSet
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	Batt INT Set
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
void wvdPmBattIrqSet(void)
{
	wvdPowerSetCallback((powerInterrupt_fp)wvdPmBattOff);
	SetPMGPIO_Interrupt_Handler();
	
#ifdef PM_ST_EN
	if(nubPmStNo == PM_ST_NO_1140){
		gpio_set(PM_A24_NC, GPIO_HIGH);
		BSP_IntSrcDis(INT_PMGPIO);
		printf("\n@@@@@[%s] ST NO is %x \n", __func__, nubPmStNo);
	}
#endif
}

/********************************************************************************************/
/**	\function		wvdPmBattSysDown
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	Batt PowerDown
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
void wvdPmBattSysDown(void)
{
	//PM_DBG("\n********************Batt PowerDown*********************\n");
	
	PowerDown(Hw_BU_DET);
}

/********************************************************************************************/
/**	\function		wvdPmBattOff
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	Batt Off Interrupt
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
 *		 0002	 20220826 		 xulei		 	 Robust Question No.8
 												After the interruption is generated, it is necessary to judge 
 												the interruption again to prevent incorrect actions
 ********************************************************************************************/
void wvdPmBattOff(Type_uByte aubData1, Type_uByte aubData2)
{
	Type_uByte aubProcIndex = PM_DATA_ZERO;

	if ( RES_OK == PmGetBattDat( ) ){
		for(aubProcIndex = PM_DATA_ZERO; aubProcIndex < (sizeof(nstPmStandby_Tbl)/sizeof(PmStandbyIf_ts)); aubProcIndex++){
			if(nstPmStandby_Tbl[aubProcIndex].forceStandbyProc_fp != NULL){
				(void)nstPmStandby_Tbl[aubProcIndex].forceStandbyProc_fp();
			}
		}

#ifdef PM_ST_EN
			if(nubPmStNo == PM_ST_NO_2120){
				gpio_set(PM_A2_NC, GPIO_HIGH);
				printf("\n@@@@@[%s] ST NO is %x \n", __func__, nubPmStNo);
			}
#endif
		//PM_DBG("\n********************Batt PowerDown*********************\n");
		PowerDown(Hw_BU_DET);
	}
}

#endif

/********************************************************************************************/
/**	\function		nvdPmAccOff
 *	\date			2022/8/10
 *	\author			xulei
 *	\description	A53 Before booting，Fast power-down demand
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
 *		 0001	 20220826 		 xulei		 	 New
 ********************************************************************************************/

void nvdPmAccOff(OS_StdQueue_ts *astMessage_p)
{
	Type_uByte aubProcIndex = PM_DATA_ZERO;

	for(aubProcIndex = PM_DATA_ZERO; aubProcIndex < (sizeof(nstPmStandby_Tbl)/sizeof(PmStandbyIf_ts)); aubProcIndex++){
		if(nstPmStandby_Tbl[aubProcIndex].forceStandbyProc_fp != NULL){
			(void)nstPmStandby_Tbl[aubProcIndex].forceStandbyProc_fp();
		}
	}

	PM_DBG("\n********************ACC PowerDown*********************\n");
	PowerDown(Hw_ACC_DET);
}

/********************************************************************************************/
/**	\function		wvdPmHalOpen
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	Power Hal Open
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
HAL_HANDLE wvdPmHalHandleGet(void)
{
	return nvbPmHalHandle;
}

/********************************************************************************************/
/**	\function		wvdPmModuleColdInit
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	App Cold Init
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
void wvdPmModuleColdInit(void)
{
	Type_uByte aubIndex = PM_DATA_ZERO;
	Type_uByte aubRet = RES_OK;

	PM_DBG("\n************************************Cold Init*************************\n");

	for(aubIndex = PM_DATA_ZERO; aubIndex < (sizeof(nstPmColdInit_Tbl)/sizeof(PmComSyncIf_ts)); aubIndex++){
		if(nstPmColdInit_Tbl[aubIndex].proc_fp != NULL ){
			aubRet = nstPmColdInit_Tbl[aubIndex].proc_fp();
			if(RES_NG == aubRet){
				PM_DBG("\n############ Cold Init Error:%d ############\n", aubIndex);
			}
		}
	}
}
/********************************************************************************************/
/**	\function		wvdPmModuleColdInit
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	App Cold Init
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
void wvdPmModuleInit(void)
{
	Type_uByte aubIndex = PM_DATA_ZERO;
	Type_uByte aubRet = RES_OK;

	PM_DBG("\n************************************ Init*************************\n");

	for(aubIndex = PM_DATA_ZERO; aubIndex < (sizeof(nstPmInit_Tbl)/sizeof(PmComSyncIf_ts)); aubIndex++){
		if(nstPmInit_Tbl[aubIndex].proc_fp != NULL ){
			aubRet = nstPmInit_Tbl[aubIndex].proc_fp();
			if(RES_NG == aubRet){
				PM_DBG("\n############ TASK Init Error:%d ############\n", aubIndex);
			}
		}
	}
}

/********************************************************************************************/
/**	\function		wvdPmSocInitGTimout
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	Soc Init OK Timeout
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
void wvdPmSocInitGTimout(void)
{	
	PM_DBG("\n############ %s ############\n", __func__);
	nvdPmSendMsg(ARPC_ID_R5_POWERMNG, EV_INF_PM_SOCINIT_GTIMEROUT, NULL, PM_DATA_ZERO);
}

/********************************************************************************************/
/**	\function		wvdPmStandbyOutGTimout
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	StandbyOut Async Guart Timer
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
void wvdPmStandbyOutGTimout(void)
{
	OS_EventCode_t aubSts = EV_INF_PM_STANDBYOUT_START;
	
	PM_DBG("\n########## PM StandbyOut Timeout CurIndex = %d ##########\n",nubPmPriorityLineIndex);
	LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_ERROR,ATAT_LEVEL_OFF,"StandbyOut CurIndex=%d", nubPmPriorityLineIndex);

	nuwPmModuleResFlg = PM_DATA_ZERO;
	
	if(nubPmPriorityLineIndex == nubPmSocPriorityLine){
		aubSts = wubPmDrvGetStandbyDetSts();
		if(aubSts == EV_INF_PM_STANDBYIN_START){
			wvdPmStandbyOutNextCtrl();
		}
		else{
			#ifdef PM_STANDBYOUT_RETRY
				if(nubPmSocStandbyOutRetryNum < PM_SOC_STANDBYOUT_RESTRY_NUM){
					nubPmSocStandbyOutRetryNum++;
					nvdPmSendMsg(ARPC_ID_R5_POWERMNG, EV_INF_PM_STANDBYOUT_ING_SOC, NULL, PM_DATA_ZERO);
				}
				else{
					PM_DBG("\n########## PM Soc StandbyOut Restry Faild to Reset ##########\n");
					nvdPmSendMsg(ARPC_ID_R5_POWERMNG, EV_INF_PM_SOCSTANDBYOUT_GTIMEROUT, NULL, PM_DATA_ZERO);
				}
			#else
					PM_DBG("\n########## PM Soc StandbyOut Faild to Reset ##########\n");
					nvdPmSendMsg(ARPC_ID_R5_POWERMNG, EV_INF_PM_SOCSTANDBYOUT_GTIMEROUT, NULL, PM_DATA_ZERO);
			#endif
#ifdef PM_ST_EN
			if(nubPmStNo == PM_ST_NO_2223){
				gpio_set(PM_A2_NC, GPIO_HIGH);
				printf("\n@@@@@[%s] ST NO is %x \n", __func__, nubPmStNo);
			}
#endif
		}
	}
	else{
		wvdPmStandbyOutNextCtrl();
	}
}

/********************************************************************************************/
/**	\function		wvdPmStandbyInGTimout
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	StandbyOut Async Guart Timer
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
void wvdPmStandbyInGTimout(void)
{
	PM_DBG("\n########## PM StandbyIn Timeout CurIndex = %d ##########\n",nubPmPriorityLineIndex);
	LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_ERROR,ATAT_LEVEL_OFF,"StandbyIn CurIndex=%d", nubPmPriorityLineIndex);

	nuwPmModuleResFlg = PM_DATA_ZERO;
	nvdPmStandbyInNextCtrl();
}

#if 0
/********************************************************************************************/
/**	\function		wvdPmUpdateResetGTimout
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	Update Reset Async Guart Timer
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
void wvdPmUpdateResetGTimout(void)
{
	PM_DBG("\n***********%d:PM UpdateReset Timeout*************\n",OSAL_GetOSTimeTick());
	
	nvdPmUpdateResetNextCtrl();
}

/********************************************************************************************/
/**	\function		wvdPmFactoryResetGTimout
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	Factory Reset Async Guart Timer
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
void wvdPmFactoryResetGTimout(void)
{
	PM_DBG("\n***********%d:PM FactoryReset Timeout*************\n",OSAL_GetOSTimeTick());
	
	nvdPmFactoryResetNextCtrl();
}
#endif

/********************************************************************************************/
/**	\function		wvdPmPowerKeyOnGTimout
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	Power Key On Async Guart Timer
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
void wvdPmPowerKeyOnGTimout(void)
{
	PM_DBG("\n######## %d:PM PowerKeyOn Timeout ###########\n",OSAL_GetOSTimeTick());
	LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_ERROR,ATAT_LEVEL_OFF,"PowerKey On CurIndex=%d", nubPmPowerKeyPriorityLineIndex);

	nuwPmPowerKeyModuleResFlg = PM_DATA_ZERO;
	nvdPmPowerKeyOnNextCtrl();
}

/********************************************************************************************/
/**	\function		wvdPmPowerKeyOffGTimout
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	Power Key Off Async Guart Timer
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
void wvdPmPowerKeyOffGTimout(void)
{
	PM_DBG("\n########## %d:PM PowerKeyOff Timeout ###########\n",OSAL_GetOSTimeTick());
	LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_ERROR,ATAT_LEVEL_OFF,"PowerKey Off CurIndex=%d", nubPmPowerKeyPriorityLineIndex);

	nuwPmPowerKeyModuleResFlg = PM_DATA_ZERO;
	nvdPmPowerKeyOffNextCtrl();
}

#ifdef PM_ST_EN
/********************************************************************************************/
/**	\function		wvdPmStNoSet
 *	\date			2022/5/20
 *	\author			wangying
 *	\description	ST NO Set
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
void wvdPmStNoSet(Type_sWord stNo)
{
	nubPmStNo = stNo;
	printf("\n @@@@@@[%s] ST NO is %x\n", __func__, nubPmStNo);
	gpio_set(PM_A2_NC, GPIO_LOW);

    switch(nubPmStNo)
    {
		case PM_ST_NO_5130:
		case PM_ST_NO_5132:
			nvdPmStPortCheck();
			break;
		case PM_ST_NO_9000:
			nubPmStPinLevelGet();
			break;

		default:
			break;
	}
}

/********************************************************************************************/
/**	\function		wvdPmStNoGet
 *	\date			2022/5/20
 *	\author			wangying
 *	\description	ST NO Set
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
Type_uWord wvdPmStNoGet(void)
{
	return nubPmStNo;
}

/********************************************************************************************/
/**	\function		nvdPmStPortCheck
 *	\date			2022/5/20
 *	\author			wangying
 *	\description	Output port register value
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
static void nvdPmStPortCheck(void)
{
	Type_uByte nubIndex = 0;
	Type_uWord nuwData = 0;
	Type_uWord nuwRegGpioA = TCC_GPIO_BASE ;
	Type_uWord nuwRegGpioB = TCC_GPIO_BASE + 0x40*1;
	Type_uWord nuwRegGpioC = TCC_GPIO_BASE + 0x40*2;
	Type_uWord nuwRegGpioE = TCC_GPIO_BASE + 0x40*4;
	Type_uWord nuwRegGpioG = TCC_GPIO_BASE + 0x40*6;
	Type_uWord nuwRegGpioH = TCC_GPIO_BASE + 0x40*25;
	Type_uWord nuwRegGpioK = TCC_GPIO_BASE + 0x40*26;	//Input,Output,FUNC
	Type_uWord nuwRegGpioMA = TCC_GPIO_BASE + 0x40*27;

	Type_uByte nubOffsetOutputEn = 0x04;		//1:Enable 0:Disable
	Type_uByte nubOffsetDriveStrengthL = 0x14;	//00:x1 01:x4 10:x2 11:x6	(port0~15)
	Type_uByte nubOffsetDriveStrengthH = 0x18;	//00:x1 01:x4 10:x2 11:x6	(port16~31)
	Type_uByte nubOffsetPullEn = 0x1C;			//1:Enable 0:Disable
	Type_uByte nubOffsetInputEn = 0x24;		//1:Enable 0:Disable
	Type_uByte nubOffsetFunc7 = 0x30;		//port0~7
	Type_uByte nubOffsetFunc15 = 0x34;		//port8~15
	Type_uByte nubOffsetFunc23 = 0x38;		//port16~23
	Type_uByte nubOffsetFunc31 = 0x3C;		//port24~31

	Type_uWord nuwRegPMGpioPullEn = TCC_PMGPIO_BASE + 0x10;

	Type_uWord nuwRegGpioTbl[] = {nuwRegGpioA, nuwRegGpioB, nuwRegGpioC, nuwRegGpioE, nuwRegGpioG, nuwRegGpioH, nuwRegGpioMA, nuwRegGpioK};
	Type_Char  *nucString[] = {"GPIO_A", "GPIO_B", "GPIO_C", "GPIO_E", "GPIO_G", "GPIO_H", "GPIO_MA", "GPIO_K"};
	Type_uByte nubNum = sizeof(nuwRegGpioTbl)/sizeof(Type_uWord);

	//PullEn Check
	for(nubIndex = 0; nubIndex < (nubNum-1); nubIndex++){
		nuwData = readl(nuwRegGpioTbl[nubIndex] + nubOffsetPullEn);
		printf("[Power ST PullEn]%s:%x\n", nucString[nubIndex], nuwData);
	}
	nuwData = readl(nuwRegPMGpioPullEn);
	printf("[Power ST PullEn]GPIO_K:%x\n", nuwData);

	//DriverStrength Check
	for(nubIndex = 0; nubIndex < (nubNum-1); nubIndex++){
		nuwData = readl(nuwRegGpioTbl[nubIndex] + nubOffsetDriveStrengthL);
		printf("[Power ST DriverStrength]%s(port0~15):%x\n", nucString[nubIndex], nuwData);
		nuwData = readl(nuwRegGpioTbl[nubIndex] + nubOffsetDriveStrengthH);
		printf("[Power ST DriverStrength]%s(port16~31):%x\n", nucString[nubIndex], nuwData);
	}		

	//func check
	for(nubIndex = 0; nubIndex < nubNum; nubIndex++){
		nuwData = readl(nuwRegGpioTbl[nubIndex] + nubOffsetFunc7);
		printf("[Power ST Func]%s(port0~7):%x\n", nucString[nubIndex], nuwData);
		nuwData = readl(nuwRegGpioTbl[nubIndex] + nubOffsetFunc15);
		printf("[Power ST Func]%s(port8~15):%x\n", nucString[nubIndex], nuwData);
		nuwData = readl(nuwRegGpioTbl[nubIndex] + nubOffsetFunc23);
		printf("[Power ST Func]%s(port16~23):%x\n", nucString[nubIndex], nuwData);
		nuwData = readl(nuwRegGpioTbl[nubIndex] + nubOffsetFunc31);
		printf("[Power ST Func]%s(port24~31):%x\n", nucString[nubIndex], nuwData);
	}

	//Output check
	for(nubIndex = 0; nubIndex < nubNum-1; nubIndex++){
		nuwData = readl(nuwRegGpioTbl[nubIndex] + nubOffsetOutputEn);
		printf("[Power ST OutEn]%s:%x\n", nucString[nubIndex], nuwData);
		nuwData = readl(nuwRegGpioTbl[nubIndex]);
		printf("[Power ST Level]%s:%x\n", nucString[nubIndex], nuwData);
	}

	nuwData = readl(nuwRegGpioTbl[nubNum-1]);
	printf("[Power ST Level]%s:%x\n", nucString[nubNum-1], nuwData);

	//Input check
	for(nubIndex = 0; nubIndex < nubNum-1; nubIndex++){
		nuwData = readl(nuwRegGpioTbl[nubIndex] + nubOffsetInputEn);
		printf("[Power ST InEn]%s:%x\n", nucString[nubIndex], nuwData);
	}

	printf("\n@@@@@[%s] ST NO is %x \n", __func__, nubPmStNo);
}

/********************************************************************************************/
/**	\function		nubPmStPinLevelGet
 *	\date			2022/5/20
 *	\author			wangying
 *	\description	Output pin level
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
static void nubPmStPinLevelGet(void)
{
	printf("[ST] Level A23 is : %d\n", gpio_get(PM_A23_NC));
	printf("[ST] Level B21 is : %d\n", gpio_get(PM_PMIC_SCL));
	printf("[ST] Level B22 is : %d\n", gpio_get(PM_PMIC_SDA));
	printf("[ST] Level B26 is : %d\n", gpio_get(PM_B26_NC));
	printf("[ST] Level B26 is : %d\n", gpio_get(PM_B27_NC));
	printf("[ST] Level B26 is : %d\n", gpio_get(PM_B28_NC));
	printf("[ST] Level C8 is : %d\n", gpio_get(PM_C08_NC));
	printf("[ST] Level C9 is : %d\n", gpio_get(PM_C09_NC));
	printf("[ST] Level H6 is : %d\n", gpio_get(PM_H06_NC));
	printf("[ST] Level H7 is : %d\n", gpio_get(PM_H07_NC));
	printf("[ST] Level H10 is : %d\n", gpio_get(PM_H10_NC));
	printf("[ST] Level H11 is : %d\n", gpio_get(PM_H11_NC));
	printf("[ST] Level K11 is : %d\n", gpio_get(PM_GPIO_K11));

}
#endif

/********************************************************************************************/
/**	\function		nubPmColdInit
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	Cold Init
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
 static Type_uByte nubPmColdInit(void)
{
	nubStartType = STANDBYOUT_TYPE_COLD_RESET; 

	return RES_OK;
}

/********************************************************************************************/
/**	\function		wvdPmRpcRcv
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
static void nvdPmRpcRcv(const Type_uByte aubsrc, const Type_uHWord auhCmd, void *avdData_p, const Type_uHWord auhLength)
{
	OS_ErrCode_t auwExeRslt = OS_ERR_TMOUT;
	OS_ErrCode_t auwGetRslt = OS_ERR_NOMEM;		
	OS_VoidPtr_t avdMsgData_p = NULL;
	OS_StdQueue_ts astMsg;

	PM_DBG("\n*********PM Rpc Rcv*********\n");
	LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF,"Rpc Receiver Callback");

	wvdGen_MemSet(&astMsg, PM_DATA_ZERO, sizeof(OS_StdQueue_ts));
	astMsg.swSrcTaskId = aubsrc;
	astMsg.uhEventCode = auhCmd;
	
	if((avdData_p != NULL) && (auhLength > PM_DATA_ZERO)){
		auwGetRslt = OS_GetFMemoryBlock(&avdMsgData_p, auhLength);
		if(auwGetRslt == OS_ERR_OK)
		{
			wvdGen_MemSet(avdMsgData_p, PM_DATA_ZERO, auhLength);
			wvdGen_MemCpy(avdMsgData_p , avdData_p, auhLength); 	
			astMsg.vdQueueBuffer_p = avdMsgData_p ;						
			astMsg.uwQueueSize = auhLength;	
		}
		else{
			PM_DBG("\n########## PM Faild to Get Queue Memmory ##########\n");
			LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]Get Memory Failed",__func__);
		}
	}
		
	auwExeRslt = OS_SendQueue(MBX_ID_POWER, OS_MAIL_PRI_NORMAL, &astMsg); 
	if(auwExeRslt != OS_ERR_OK){
		PM_DBG("\n########## PM Faild to Send Queue ##########\n");
		LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]Send Queue Failed",__func__);
		if(NULL != astMsg.vdQueueBuffer_p){
			auwExeRslt = OS_ReleaseFMemoryBlock(astMsg.vdQueueBuffer_p);
			if(auwExeRslt != OS_ERR_OK){
				PM_DBG("\n########## PM Faild to realease memory:%d ##########\n", auwExeRslt);
				LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]Faild to realease memory:%d",__func__, auwExeRslt);
			}
		}
	}
	
}

/********************************************************************************************/
/**	\function		nvdPmInit
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	Task Init
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
 static void nvdPmInit(void)
{
	Type_uByte aubLineIndex = PM_DATA_ZERO;
	Type_uByte aubRowIndex = PM_DATA_ZERO;
	OS_ErrCode_t aswRet = OS_ERR_OK;

	nubPmPriorityLineIndex = PM_DATA_ZERO;
	nubPmPowerKeyPriorityLineIndex = PM_DATA_ZERO;
	nuwPmModuleResFlg = PM_DATA_ZERO;
	nuwPmPowerKeyModuleResFlg = PM_DATA_ZERO;
#ifdef PM_STANDBYOUT_RETRY
	nubPmSocStandbyOutRetryNum = PM_DATA_ZERO;
#endif
	nstPmStateMng.uhSocState = PM_STATE_NONE;
	nstPmStateMng.uhPowerKeyState = PM_STATE_NONE;
	nstPmStateMng.uhResetState = PM_STATE_NONE;
	nstPmStateMng.uhStandbyState = PM_STATE_NONE;
	nubPmSocPriorityLine = PM_PRIORITY_LINE_MAX; 
	
	for(aubLineIndex = PM_DATA_ZERO; aubLineIndex < PM_PRIORITY_LINE_MAX;aubLineIndex++){
		for(aubRowIndex = PM_DATA_ZERO; aubRowIndex < PM_PRIORITY_ROW_MAX;aubRowIndex++){
			if(nuwModulePriority_Tbl[aubLineIndex][aubRowIndex] == ARPC_ID_A53_POWER){
				nubPmSocPriorityLine = aubLineIndex;
				break;
			}
		}

		if(nubPmSocPriorityLine != PM_PRIORITY_LINE_MAX){
			break;
		}
	}


	/* Start Soc Init GTimer */
	aswRet = OS_StartTimer(OS_TIMER_ID_PMSOCSTARTGT);
	if(aswRet != OS_ERR_OK){
		PM_DBG("\n########## PM Soc Start Timer Start Failed:%d ##########\n", aswRet);
		LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]Soc Start Timer Start Failed:%d",__func__, aswRet);
	}

	//nvdPmSendStartType();
	
}

/********************************************************************************************/
/**	\function		nvdPmProcCtrl
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	PM Process Control
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
static void nvdPmSendStartType(void)
{
	switch(nstPmStartInfo.uwType)
	{
		case PM_PWRDN_NO:
			LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"Boot reason is Batt Long");
			break;
			
		case PM_PWRDN_ACC_OFF:
			LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"Boot reason is Acc or HDET");
			break;

		case PM_PWRDN_BU_OFF:
			LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"Boot reason is Batt Short");
			break;

		case PM_PWRDN_UPDATE_RESET:
			LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"Boot reason is update");
			break;

		case PM_PWRDN_FACTORY_RESET:
			LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"Boot reason is factory recover");
			break;

		case PM_PWRDN_A53_REBOOT_REQ:
			LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"Boot reason is A53 Reboot Req");
			break;

		case PM_PWRDN_A7_REBOOT_REQ:
			LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"Boot reason is A7 Reboot Req");
			break;

		case PM_PWRDN_A53_HAERT_TIMOUT:
			LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"Boot reason is A53 Heart Timout");
			break;

		case PM_PWRDN_A7_HAERT_TIMOUT:
			LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"Boot reason is A7 Heart Timout");
			break;

		case PM_PWRDN_OS_A53WDT:
			LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"Boot reason is A53 WatchDog:sts0=%x,sts1=%x,sts2=%x", nstPmStartInfo.rstSts0, nstPmStartInfo.rstSts1, nstPmStartInfo.rstSts2);
			break;

		case PM_PWRDN_OS_A7WDT:
			LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"Boot reason is A7 WatchDog:sts0=%x,sts1=%x,sts2=%x", nstPmStartInfo.rstSts0, nstPmStartInfo.rstSts1, nstPmStartInfo.rstSts2);
			break;

		case PM_PWRDN_SOC_START_TIMOUT:
			LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"Boot reason is Soc Start Timout");
			break;

		case PM_PWRDN_SOC_STANDBYOUT_TIMOUT:
			LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"Boot reason is Soc StanbyOut Timout");
			break;

		case PM_PWRDN_OS_RESET:
			LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"Boot reason is R5 OS error");
			break;

		case PM_PWRDN_WDT:
			LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"Boot reason is watchdog:sts0=%x,sts1=%x,sts2=%x", nstPmStartInfo.rstSts0, nstPmStartInfo.rstSts1, nstPmStartInfo.rstSts2);
			break;

		case PM_PWRDN_MAX:
		default:
			LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"Boot reason is unknow:sts0=%x,sts1=%x,sts2=%x", nstPmStartInfo.rstSts0, nstPmStartInfo.rstSts1, nstPmStartInfo.rstSts2);
			break;

	}
	
}

/********************************************************************************************/
/**	\function		nvdPmProcCtrl
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	PM Process Control
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
static void nvdPmProcCtrl(OS_StdQueue_ts *astMessage_p)
{
	Type_uByte aubIndex = PM_DATA_ZERO;
	Type_uByte aubFlag = FALSE;

	PM_DBG("\n********PM Managger Event Rcv RPCID = %X**\n, Event = %X**\n, SocSts = %X**\n, StandbySts = %X**\n, resetSts = %X**\n, PowerKeySts = %X**\n *****************************\n",astMessage_p->swSrcTaskId, astMessage_p->uhEventCode, nstPmStateMng.uhSocState, nstPmStateMng.uhStandbyState, nstPmStateMng.uhResetState, nstPmStateMng.uhPowerKeyState);

	for(aubIndex = PM_DATA_ZERO; aubIndex < (sizeof(nstPmProcCtrl_Tbl)/sizeof(PmCtrlProc_ts)); aubIndex++){
		if(((nstPmProcCtrl_Tbl[aubIndex].ubModuleID == ARPC_ID_ANY)  || (astMessage_p->swSrcTaskId == nstPmProcCtrl_Tbl[aubIndex].ubModuleID) )
			&& (astMessage_p->uhEventCode == nstPmProcCtrl_Tbl[aubIndex].uhEvent)
			&& ((nstPmProcCtrl_Tbl[aubIndex].uhSocMask == PM_STATE_ANY)  || ((nstPmStateMng.uhSocState & nstPmProcCtrl_Tbl[aubIndex].uhSocMask) != PM_DATA_ZERO))
			&& ((nstPmProcCtrl_Tbl[aubIndex].uhStandbyMask == PM_STATE_ANY)  || ((nstPmStateMng.uhStandbyState & nstPmProcCtrl_Tbl[aubIndex].uhStandbyMask) != PM_DATA_ZERO))
			&& ((nstPmProcCtrl_Tbl[aubIndex].uhResetMask == PM_STATE_ANY)  || ((nstPmStateMng.uhResetState & nstPmProcCtrl_Tbl[aubIndex].uhResetMask) != PM_DATA_ZERO))
			&& ((nstPmProcCtrl_Tbl[aubIndex].uhPowerKeyMask == PM_STATE_ANY)  ||((nstPmStateMng.uhPowerKeyState & nstPmProcCtrl_Tbl[aubIndex].uhPowerKeyMask) != PM_DATA_ZERO))){
			
			PM_DBG("\n*******************PM Proc Run*****************\n");
			LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_WARN,ATAT_LEVEL_OFF,"[%s]Valid ID=%X,Eve=%X",__func__,astMessage_p->swSrcTaskId, astMessage_p->uhEventCode);
			LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_WARN,ATAT_LEVEL_OFF,"Soc=%4X,Stby=%4X, rst=%4X, PmKey=%4X",nstPmStateMng.uhSocState, nstPmStateMng.uhStandbyState, nstPmStateMng.uhResetState, nstPmStateMng.uhPowerKeyState);


			aubFlag = TRUE;
			
			if(nstPmProcCtrl_Tbl[aubIndex].pmCtrl_fp != NULL){
				nstPmProcCtrl_Tbl[aubIndex].pmCtrl_fp(astMessage_p);
			}

			if(nstPmProcCtrl_Tbl[aubIndex].uhNextStandbyState != PM_STATE_NO_CHANGE){
				nstPmStateMng.uhStandbyState = nstPmProcCtrl_Tbl[aubIndex].uhNextStandbyState;
				if(nstPmStateMng.uhStandbyState == PM_STANDBY_STANDBYOUT){
					wvdPmDrvRequestCurrentEvent();
				}
			}
			
			if(nstPmProcCtrl_Tbl[aubIndex].uhNextSocState != PM_STATE_NO_CHANGE){
				nstPmStateMng.uhSocState = nstPmProcCtrl_Tbl[aubIndex].uhNextSocState;
			}

			if(nstPmProcCtrl_Tbl[aubIndex].uhNextResetState != PM_STATE_NO_CHANGE){
				nstPmStateMng.uhResetState = nstPmProcCtrl_Tbl[aubIndex].uhNextResetState;
			}
			if(nstPmProcCtrl_Tbl[aubIndex].uhNextPowerKeyState != PM_STATE_NO_CHANGE){
				nstPmStateMng.uhPowerKeyState = nstPmProcCtrl_Tbl[aubIndex].uhNextPowerKeyState;
				if(nstPmStateMng.uhPowerKeyState == PM_STATE_NONE){
					nvdPmPowerKeyProcStop();
				}
			}

			break;
		}

	}

	if(aubFlag == FALSE){
		PM_DBG("\n################ PM Invalid event ############\n");
		LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_ERROR,ATAT_LEVEL_OFF,"[%s]Invalid ID=%X,Eve=%X",__func__,astMessage_p->swSrcTaskId, astMessage_p->uhEventCode);
		LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_ERROR,ATAT_LEVEL_OFF,"Soc=%4X,Stby=%4X, rst=%4X, PmKey=%4X",nstPmStateMng.uhSocState, nstPmStateMng.uhStandbyState, nstPmStateMng.uhResetState, nstPmStateMng.uhPowerKeyState);
	}

}

/********************************************************************************************/
/**	\function		nvdPmUpdateResFlg
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	Update Response Flag
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
static void nvdPmUpdateResFlg(OS_StdQueue_ts *astMessage_p)
{
	Type_uByte aubPriorityIndex = PM_DATA_ZERO;
	
	if(nubPmPriorityLineIndex >= PM_PRIORITY_LINE_MAX){
		PM_DBG("\n########### PM nubPmPriorityLineIndex:%d is bigger than Max priority ##############\n", nubPmPriorityLineIndex);
		LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_ERROR,ATAT_LEVEL_OFF,"[%s]Priority=%2d is too big",__func__,nubPmPriorityLineIndex);
		return;
	}

	if(nuwPmModuleResFlg == PM_DATA_ZERO){
		PM_DBG("\n########### PM Invalid nuwPmModuleResFlg is also zero ##############\n");
		LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_ERROR,ATAT_LEVEL_OFF,"[%s]All Response is also received",__func__);
		return;
	}

	for(aubPriorityIndex = PM_DATA_ZERO; aubPriorityIndex < PM_PRIORITY_ROW_MAX; aubPriorityIndex++){
		if(astMessage_p->swSrcTaskId == nuwModulePriority_Tbl[nubPmPriorityLineIndex][aubPriorityIndex]){
			nuwPmModuleResFlg &= ~(PM_BIT_TRUE << aubPriorityIndex);
			break;
		}
	}
}

/********************************************************************************************/
/**	\function		nvdPmStandbyOutCtrlStart
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	StandbyOut Event Operate
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
static void nvdPmStandbyOutCtrlStart(OS_StdQueue_ts *astMessage_p)
{
	PM_DBG("\n*********************PM StandbyOut Start********************\n");
	LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]StandbyOut Start",__func__);

	nubPmPriorityLineIndex = PM_DATA_ZERO;
	nuwPmModuleResFlg = PM_DATA_ZERO;
	nvdPmStandbyOutMcuCtrlProc(astMessage_p);

	nvdPmSendStartType();

}

/********************************************************************************************/
/**	\function		nvdPmStandbyOutMcuCtrlProc
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	Power Key On Process
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
static void nvdPmStandbyOutMcuCtrlProc(OS_StdQueue_ts *astMessage_p)
{
	Type_uByte aubPriorityIndex = PM_DATA_ZERO;
	Type_uByte aubProcIndex = PM_DATA_ZERO;
	Type_uWord auwGTimer = PM_DATA_ZERO;
	Type_uByte aubFlag = FALSE;
	OS_ErrCode_t aswRet = OS_ERR_OK;


	PM_DBG("\n*************Mcu StandbyOut Proc: CurIndex = %d SocIndex = %d****************\n",nubPmPriorityLineIndex, nubPmSocPriorityLine);
	LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_WARN, ATAT_LEVEL_OFF,"Mcu StandbyOuting Index=%d", nubPmPriorityLineIndex);

	if(nubPmPriorityLineIndex >= nubPmSocPriorityLine){
		PM_DBG("\n########### PM Mcu Index:%d is bigger than Soc priority:%d ##############\n", nubPmPriorityLineIndex, nubPmSocPriorityLine);
		LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_ERROR,ATAT_LEVEL_OFF,"Mcu StandbyOuting Priority=%2d is too big",nubPmPriorityLineIndex);
		return;
	}

	for(;nubPmPriorityLineIndex < nubPmSocPriorityLine;nubPmPriorityLineIndex++){
		for(aubPriorityIndex = PM_DATA_ZERO; aubPriorityIndex < PM_PRIORITY_ROW_MAX; aubPriorityIndex++){
			if(nuwModulePriority_Tbl[nubPmPriorityLineIndex][aubPriorityIndex] != PM_MODULE_NONE){
				for(aubProcIndex = PM_DATA_ZERO; aubProcIndex < (sizeof(nstPmStandby_Tbl)/sizeof(PmStandbyIf_ts)); aubProcIndex++){
					if(nstPmStandby_Tbl[aubProcIndex].ubModuleID == nuwModulePriority_Tbl[nubPmPriorityLineIndex][aubPriorityIndex]){
						if(nstPmStandby_Tbl[aubProcIndex].standbyOutProc_fp != NULL){
							aubFlag = TRUE;
							(void)nstPmStandby_Tbl[aubProcIndex].standbyOutProc_fp();
							if(nstPmStandby_Tbl[aubProcIndex].uwStandbyOutTime > PM_DATA_ZERO){
								nuwPmModuleResFlg |= (PM_BIT_TRUE << aubPriorityIndex);
							}
							if(auwGTimer < nstPmStandby_Tbl[aubProcIndex].uwStandbyOutTime){
								auwGTimer = nstPmStandby_Tbl[aubProcIndex].uwStandbyOutTime;
							}
						}
						break;
					}
				}

			}
		}

		if(aubFlag == TRUE){
			break;
		}
	}

	if(auwGTimer == PM_DATA_ZERO){
		if(nubPmPriorityLineIndex < (nubPmSocPriorityLine - PM_ARRAY_INDEX_ONE)){
			nubPmPriorityLineIndex++;
			nvdPmSendMsg(ARPC_ID_R5_POWERMNG, EV_INF_PM_STANDBYOUT_ING_MCU, NULL, PM_DATA_ZERO);
		}
		else if(nubPmSocPriorityLine < PM_PRIORITY_LINE_MAX){
			nubPmPriorityLineIndex++;
			nvdPmSendMsg(ARPC_ID_R5_POWERMNG, EV_INF_PM_STANDBYOUT_ING_SOC, NULL, PM_DATA_ZERO);
		}
		else{
			nvdPmSendMsg(ARPC_ID_R5_POWERMNG, EV_INF_PM_STANDBYOUT_OVER, NULL, PM_DATA_ZERO);
		}
	}
	else{
		/* Start StandbyOut GTimer */
		aswRet = OS_TimerChangePeriod(OS_TIMER_ID_PMSTANDBYOUTGT, auwGTimer);
		if(aswRet != OS_ERR_OK){
			PM_DBG("########## Timer Start Failed:%d ##########\n", aswRet);
			LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]Timer Start Failed:%d",__func__, aswRet);
		}
	}
	
}

/********************************************************************************************/
/**	\function		nvdPmStandbyOutSocCtrlStart
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	Power Key On Process
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
static void nvdPmStandbyOutSocCtrlStart(OS_StdQueue_ts *astMessage_p)
{
	OS_ErrCode_t aswRet = OS_ERR_OK;

	/* Stop Soc Init GTimer */
	aswRet = OS_StopTimer(OS_TIMER_ID_PMSOCSTARTGT);
	if(aswRet != OS_ERR_OK){
		PM_DBG("########## Timer Stop Failed:%d ##########\n", aswRet);
		LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]Timer Stop Failed:%d",__func__, aswRet);
	}

	//PM_DBG("##########timer %d nvdPmStandbyOutSocCtrlStart ##########\n",OSAL_GetOSTimeTick());
	
	nvdPmSendMsg(ARPC_ID_A53_POWER, EV_INF_A53_TO_R5_START_RES, NULL, PM_DATA_ZERO);

	nvdPmStandbyOutSocCtrlProc(astMessage_p);

}

/********************************************************************************************/
/**	\function		nvdPmStandbyOutSocCtrlProc
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	Power Key On Process
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
static void nvdPmStandbyOutSocCtrlProc(OS_StdQueue_ts *astMessage_p)
{
	Type_uByte aubPriorityIndex = PM_DATA_ZERO;
	Type_uByte aubProcIndex = PM_DATA_ZERO;
	Type_uWord auwGTimer = PM_DATA_ZERO;
	Type_uByte aubFlag = FALSE;	
	OS_ErrCode_t aswRet = OS_ERR_OK;

	PM_DBG("\n*************Soc StandbyOut Proc: CurIndex = %d****************\n", nubPmPriorityLineIndex);
	LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_WARN, ATAT_LEVEL_OFF,"SoC StandbyOuting Index=%d", nubPmPriorityLineIndex);

	for(;nubPmPriorityLineIndex < PM_PRIORITY_LINE_MAX;nubPmPriorityLineIndex++){
		for(aubPriorityIndex = PM_DATA_ZERO; aubPriorityIndex < PM_PRIORITY_ROW_MAX; aubPriorityIndex++){
			if(nuwModulePriority_Tbl[nubPmPriorityLineIndex][aubPriorityIndex] != PM_MODULE_NONE){
				for(aubProcIndex = PM_DATA_ZERO; aubProcIndex < (sizeof(nstPmStandby_Tbl)/sizeof(PmStandbyIf_ts)); aubProcIndex++){
					if(nstPmStandby_Tbl[aubProcIndex].ubModuleID == nuwModulePriority_Tbl[nubPmPriorityLineIndex][aubPriorityIndex]){
						if(nstPmStandby_Tbl[aubProcIndex].standbyOutProc_fp != NULL){
							aubFlag = TRUE;
							(void)nstPmStandby_Tbl[aubProcIndex].standbyOutProc_fp();
							if(nstPmStandby_Tbl[aubProcIndex].uwStandbyOutTime > PM_DATA_ZERO){
								nuwPmModuleResFlg |= (PM_BIT_TRUE << aubPriorityIndex);
							}
							if(auwGTimer < nstPmStandby_Tbl[aubProcIndex].uwStandbyOutTime){
								auwGTimer = nstPmStandby_Tbl[aubProcIndex].uwStandbyOutTime;
							}
						}
						break;
					}
				}

			}
		}

		if(aubFlag == TRUE){
			break;
		}
	}

	if(auwGTimer == PM_DATA_ZERO){
		if(nubPmPriorityLineIndex < PM_PRIORITY_LINE_ARRAY_MAX){
			nubPmPriorityLineIndex++;
			nvdPmSendMsg(ARPC_ID_R5_POWERMNG, EV_INF_PM_STANDBYOUT_ING_SOC, NULL, PM_DATA_ZERO);
		}
		else{
			nvdPmSendMsg(ARPC_ID_R5_POWERMNG, EV_INF_PM_STANDBYOUT_OVER, NULL, PM_DATA_ZERO);
		}
	}
	else{
		/* Start StandbyOut GTimer */
		aswRet = OS_TimerChangePeriod(OS_TIMER_ID_PMSTANDBYOUTGT, auwGTimer);
		if(aswRet != OS_ERR_OK){
			PM_DBG("########## Timer Start Failed:%d ##########\n", aswRet);
			LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]Timer Start Failed:%d",__func__, aswRet);
		}

	}

}

/********************************************************************************************/
/**	\function		nvdPmStandbyOutResCtrl
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	StandbyOut Response
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
static void nvdPmStandbyOutResCtrl(OS_StdQueue_ts *astMessage_p)
{
	OS_ErrCode_t aswRet = OS_ERR_OK;
	PM_DBG("\n ##########[%s]##########\n",__func__);
#ifdef PM_ST_EN
	if((nubPmStNo == PM_ST_NO_2221) && (astMessage_p->swSrcTaskId == ARPC_ID_R5_POWERDRV)){
		printf("\n@@@@@[%s] ST NO is %x \n", __func__, nubPmStNo);
		return;
	}
	else if((nubPmStNo == PM_ST_NO_2222) && (astMessage_p->swSrcTaskId == ARPC_ID_EEPROM)){
		printf("\n@@@@@[%s] ST NO is %x \n", __func__, nubPmStNo);
		return;
	}
	else if((nubPmStNo == PM_ST_NO_2223) && (astMessage_p->swSrcTaskId == ARPC_ID_A53_POWER)){
		printf("\n@@@@@[%s] ST NO is %x \n", __func__, nubPmStNo);
		return;
	}
	else{
	}
#endif

	nvdPmUpdateResFlg(astMessage_p);

	if(nuwPmModuleResFlg == PM_DATA_ZERO){

		/* Stop StandbyOut GTimer */
		aswRet = OS_StopTimer(OS_TIMER_ID_PMSTANDBYOUTGT);
		if(aswRet != OS_ERR_OK){
			PM_DBG("########## Timer Stop Failed:%d ##########\n", aswRet);
			LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]Timer Stop Failed:%d",__func__, aswRet);
		}

		wvdPmStandbyOutNextCtrl();
	
		LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF,"[%s]All response id received",__func__);
	}

}

/********************************************************************************************/
/**	\function		wvdPmStandbyOutNextCtrl
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	StandbyOut Next Level Module Run
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
static void wvdPmStandbyOutNextCtrl(void)
{
	if(nubPmPriorityLineIndex < (nubPmSocPriorityLine - PM_ARRAY_INDEX_ONE)){
		nubPmPriorityLineIndex++;
		nvdPmSendMsg(ARPC_ID_R5_POWERMNG, EV_INF_PM_STANDBYOUT_ING_MCU, NULL, PM_DATA_ZERO);
	}
	else if(nubPmPriorityLineIndex < PM_PRIORITY_LINE_ARRAY_MAX){
		nubPmPriorityLineIndex++;
		nvdPmSendMsg(ARPC_ID_R5_POWERMNG, EV_INF_PM_STANDBYOUT_ING_SOC, NULL, PM_DATA_ZERO);
	}
	else{
		nvdPmSendMsg(ARPC_ID_R5_POWERMNG, EV_INF_PM_STANDBYOUT_OVER, NULL, PM_DATA_ZERO);
	}
}

/********************************************************************************************/
/**	\function		wvdPmStandbyOutOver
 *	\date			2022/03/10
 *	\author			wangying
 *	\description	StandbyOut Finish
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
static void wvdPmStandbyOutOver(OS_StdQueue_ts *astMessage_p)
{
	Type_sWord aswRet = HAL_OK;

	aswRet = HAL_Ioctl(nvbPmHalHandle, HAL_IO_PM_SET_WARM_START_FLAG, 0, 0, 0, 0);
	if(HAL_OK != aswRet){
		PM_DBG("\n######## PM Fail to HAL_Ioctl（SET_WARM_START_FLAG）:%d#######\n", aswRet);
		LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]Falid to set warm flag",__func__);
	}

	LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]StandbyOut Finish!",__func__);

#ifdef PM_ST_EN
	if((nubPmStNo == PM_ST_NO_2220) || (nubPmStNo == PM_ST_NO_2221) || (nubPmStNo == PM_ST_NO_2222)){
		gpio_set(PM_A2_NC, GPIO_HIGH);
		printf("\n@@@@@[%s] ST NO is %x \n", __func__, nubPmStNo);
	}
#endif

}


/********************************************************************************************/
/**	\function		nvdPmSocInitReceive
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	Soc Init OK Set
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
static void nvdPmSocInitReceive(OS_StdQueue_ts *astMessage_p)
{
	OS_ErrCode_t aswRet = OS_ERR_OK;
	
	PM_DBG("\n*******PM Soc Init OK receive begin Mcu StandbyOut end ******\n");

	/* Stop Soc Init GTimer */
	aswRet = OS_StopTimer(OS_TIMER_ID_PMSOCSTARTGT);
	if(aswRet != OS_ERR_OK){
		PM_DBG("\n######## PM Timer Stop Failed:%d #########\n", aswRet);
		LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]Timer Stop Failed:%d",__func__, aswRet);
	}

	nvdPmSendMsg(ARPC_ID_A53_POWER, EV_INF_A53_TO_R5_START_RES, NULL, PM_DATA_ZERO);
}

#if 0
/********************************************************************************************/
/**	\function		nvdPmStandbyIningResetCtrl
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	StandbyIning Reset
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
static void nvdPmStandbyIningResetCtrl(OS_StdQueue_ts *astMessage_p)
{
	Type_uByte aubProcIndex = PM_DATA_ZERO;

	for(aubProcIndex = PM_DATA_ZERO; aubProcIndex < (sizeof(nstPmStandby_Tbl)/sizeof(PmStandbyIf_ts)); aubProcIndex++){
		if(nstPmStandby_Tbl[aubProcIndex].forceStandbyProc_fp != NULL){
			nstPmStandby_Tbl[aubProcIndex].forceStandbyProc_fp();
		}
	}

	nvdPmSysAccResetCtrl(astMessage_p);

}
#endif

/********************************************************************************************/
/**	\function		nvdPmStandbyInNextCtrl
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	StandbyIn Next Level Module Run
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
static void nvdPmStandbyInNextCtrl(void)
{
	if(nubPmPriorityLineIndex > 0){
		nvdPmSendMsg(ARPC_ID_R5_POWERMNG, EV_INF_PM_STANDBYIN_ING, NULL, PM_DATA_ZERO);
	}
	else{
		nvdPmSendMsg(ARPC_ID_R5_POWERMNG, EV_INF_PM_STANDBYIN_OVER, NULL, PM_DATA_ZERO);
	}
}

/********************************************************************************************/
/**	\function		nvdPmStandbyInOver
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	StandbyIn Reset
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
static void nvdPmStandbyInOver(OS_StdQueue_ts *astMessage_p)
{
	OS_EventCode_t aubSts = EV_INF_PM_STANDBYOUT_START;

#ifdef PM_ST_EN
	if((nubPmStNo == PM_ST_NO_2320) || (nubPmStNo == PM_ST_NO_2321) || (nubPmStNo == PM_ST_NO_2322) || (nubPmStNo == PM_ST_NO_2323)){
		gpio_set(PM_A2_NC, GPIO_HIGH);
		printf("\n@@@@@[%s] ST NO is %x \n", __func__, nubPmStNo);
	}

	if(nubPmStNo == PM_ST_NO_5131){
		nvdPmStPortCheck();
		printf("\n@@@@@[%s] ST NO is %x \n", __func__, nubPmStNo);
	}
#endif
	gpio_set(PM_VIDEO_DEC_12V_CONT,GPIO_LOW);
	gpio_set(PM_VIDEO_DEC_33V_CONT,GPIO_LOW);

	aubSts = wubPmDrvGetStandbyDetSts();
	if(aubSts == EV_INF_PM_STANDBYIN_START){
		nvdPmSysDown(PM_PWRDN_ACC_OFF);
	}
	else{
		nvdPmSysReset(PM_PWRDN_ACC_OFF);
	}
}

/********************************************************************************************/
/**	\function		nvdPmSysDownCtrl
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	StandbyIn Reset
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
static void nvdPmSysDownCtrl(OS_StdQueue_ts *astMessage_p)
{
	nvdPmSysDown(PM_PWRDN_ACC_OFF);
}

/********************************************************************************************/
/**	\function		nvdPmSysAccResetCtrl
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	StandbyIn Reset
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
static void nvdPmSysAccResetCtrl(OS_StdQueue_ts *astMessage_p)
{
	nvdPmSysReset(PM_PWRDN_ACC_OFF);
}

/********************************************************************************************/
/**	\function		nvdPmSysUpdateResetCtrl
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	StandbyIn Reset
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
static void nvdPmSysUpdateResetCtrl(OS_StdQueue_ts *astMessage_p)
{
	OS_EventCode_t aubSts = EV_INF_PM_STANDBYOUT_START;
	
	aubSts = wubPmDrvGetStandbyDetSts();
	if(aubSts == EV_INF_PM_STANDBYIN_START){
		nvdPmSysDown(PM_PWRDN_ACC_OFF);
	}
	else{
		nvdPmSysReset(PM_PWRDN_UPDATE_RESET);
	}
}

/********************************************************************************************/
/**	\function		nvdPmSysFactoryResetCtrl
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	StandbyIn Reset
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
static void nvdPmSysFactoryResetCtrl(OS_StdQueue_ts *astMessage_p)
{
	OS_EventCode_t aubSts = EV_INF_PM_STANDBYOUT_START;
	
	aubSts = wubPmDrvGetStandbyDetSts();
	if(aubSts == EV_INF_PM_STANDBYIN_START){
		nvdPmSysDown(PM_PWRDN_ACC_OFF);
	}
	else{
		nvdPmSysReset(PM_PWRDN_FACTORY_RESET);
	}
}

/********************************************************************************************/
/**	\function		nvdPmSysA53ReqResetCtrl
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	StandbyIn Reset
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
static void nvdPmSysA53ReqResetCtrl(OS_StdQueue_ts *astMessage_p)
{
	OS_EventCode_t aubSts = EV_INF_PM_STANDBYOUT_START;
	
	aubSts = wubPmDrvGetStandbyDetSts();
	if(aubSts == EV_INF_PM_STANDBYIN_START){
		nvdPmSysDown(PM_PWRDN_ACC_OFF);
	}
	else{
		nvdPmSysReset(PM_PWRDN_A53_REBOOT_REQ);
	}
}

/********************************************************************************************/
/**	\function		nvdPmSysA7ReqResetCtrl
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	StandbyIn Reset
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
static void nvdPmSysA7ReqResetCtrl(OS_StdQueue_ts *astMessage_p)
{
	OS_EventCode_t aubSts = EV_INF_PM_STANDBYOUT_START;
	
	aubSts = wubPmDrvGetStandbyDetSts();
	if(aubSts == EV_INF_PM_STANDBYIN_START){
		nvdPmSysDown(PM_PWRDN_ACC_OFF);
	}
	else{
		nvdPmSysReset(PM_PWRDN_A7_REBOOT_REQ);
	}
}

/********************************************************************************************/
/**	\function		nvdPmSysA53HeartResetCtrl
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	StandbyIn Reset
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
static void nvdPmSysA53HeartResetCtrl(OS_StdQueue_ts *astMessage_p)
{
	OS_EventCode_t aubSts = EV_INF_PM_STANDBYOUT_START;
	
	aubSts = wubPmDrvGetStandbyDetSts();
	if(aubSts == EV_INF_PM_STANDBYIN_START){
		nvdPmSysDown(PM_PWRDN_ACC_OFF);
	}
	else{
		nvdPmSysReset(PM_PWRDN_A53_HAERT_TIMOUT);
	}
}

/********************************************************************************************/
/**	\function		nvdPmSysA7HeartResetCtrl
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	StandbyIn Reset
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
static void nvdPmSysA7HeartResetCtrl(OS_StdQueue_ts * astMessage_p)
{
	OS_EventCode_t aubSts = EV_INF_PM_STANDBYOUT_START;
	
	aubSts = wubPmDrvGetStandbyDetSts();
	if(aubSts == EV_INF_PM_STANDBYIN_START){
		nvdPmSysDown(PM_PWRDN_ACC_OFF);
	}
	else{
		nvdPmSysReset(PM_PWRDN_A7_HAERT_TIMOUT);
	}
}

/********************************************************************************************/
/**	\function		nvdPmSysA53WdtResetCtrl
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	StandbyIn Reset
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
static void nvdPmSysA53WdtResetCtrl(OS_StdQueue_ts *astMessage_p)
{
	OS_EventCode_t aubSts = EV_INF_PM_STANDBYOUT_START;
	
	aubSts = wubPmDrvGetStandbyDetSts();
	if(aubSts == EV_INF_PM_STANDBYIN_START){
		nvdPmSysDown(PM_PWRDN_ACC_OFF);
	}
	else{
		nvdPmSysReset(PM_PWRDN_OS_A53WDT);
	}
}

/********************************************************************************************/
/**	\function		nvdPmSysA7WdtResetCtrl
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	StandbyIn Reset
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
static void nvdPmSysA7WdtResetCtrl(OS_StdQueue_ts *astMessage_p)
{
	OS_EventCode_t aubSts = EV_INF_PM_STANDBYOUT_START;
	
	aubSts = wubPmDrvGetStandbyDetSts();
	if(aubSts == EV_INF_PM_STANDBYIN_START){
		nvdPmSysDown(PM_PWRDN_ACC_OFF);
	}
	else{
		nvdPmSysReset(PM_PWRDN_OS_A7WDT);
	}
}

/********************************************************************************************/
/**	\function		nvdPmSocStartErrReset
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	Soc Start Timout Ctrl
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
static void nvdPmSocStartErrReset(OS_StdQueue_ts *astMessage_p)
{
	wvdEEPLOGWrite( TYPE_PMMNG, PM_EEP_ERROR_MNG_SOCINIT_TIMOUT );
	//LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"Soc start timout");
	nvdPmErrResetNow(PM_PWRDN_SOC_START_TIMOUT);
}

/********************************************************************************************/
/**	\function		nvdPmSocStandbyOutErrReset
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	Soc Start Timout Ctrl
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
static void nvdPmSocStandbyOutErrReset(OS_StdQueue_ts *astMessage_p)
{
	wvdEEPLOGWrite( TYPE_PMMNG, PM_EEP_ERROR_MNG_SOCSTANDBYOUT_TIMOUT );
	//LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF, "Soc StandbyOut Timout");
	nvdPmErrResetNow(PM_PWRDN_SOC_STANDBYOUT_TIMOUT);
}

/********************************************************************************************/
/**	\function		nvdPmA53WdtErrResetNow
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	Soc Start Timout Ctrl
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
static void nvdPmA53WdtErrResetNow(OS_StdQueue_ts *astMessage_p)
{
	//LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF,"OS A53 Watchdog reset");
	nvdPmErrResetNow(PM_PWRDN_OS_A53WDT);
}

/********************************************************************************************/
/**	\function		nvdPmA7WdtErrResetNow
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	Soc Start Timout Ctrl
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
static void nvdPmA7WdtErrResetNow(OS_StdQueue_ts *astMessage_p)
{
	//LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF,"OS A7 Watchdog reset");
	nvdPmErrResetNow(PM_PWRDN_OS_A7WDT);
}

/********************************************************************************************/
/**	\function		nvdPmErrResetNow
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	Soc Start Timout
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
static void nvdPmErrResetNow(Type_uByte aubType)
{
	Type_uByte aubProcIndex = PM_DATA_ZERO;
	OS_EventCode_t aubSts = EV_INF_PM_STANDBYOUT_START;

	for(aubProcIndex = PM_DATA_ZERO; aubProcIndex < (sizeof(nstPmStandby_Tbl)/sizeof(PmStandbyIf_ts)); aubProcIndex++){
		if(nstPmStandby_Tbl[aubProcIndex].forceStandbyProc_fp != NULL){
			(void)nstPmStandby_Tbl[aubProcIndex].forceStandbyProc_fp();
		}
	}

	aubSts = wubPmDrvGetStandbyDetSts();
	if(aubSts == EV_INF_PM_STANDBYIN_START){
		nvdPmSysDown(PM_PWRDN_ACC_OFF);
	}
	else{
		nvdPmSysReset(aubType);
	}

}

/********************************************************************************************/
/**	\function		nvdPmSysDownCtrl
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	StandbyIn Reset
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
static void nvdPmSysDown(Type_uByte aubType)
{
	Type_sWord aswRet = HAL_OK;
	
	PM_DBG("\n*******PM PowerDown ******\n");
	
	nvdPmDownCtrl();

	aswRet = HAL_Ioctl(nvbPmHalHandle, HAL_IO_PM_ENTER_POWERDOWN, &aubType, 0, 0, 0);
	if(HAL_OK != aswRet){
		PM_DBG("\n######## PM HAL_Ioctl POWERDOWN Failed:%d ########\n", aswRet);
		//LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]HAL_Ioctl Failed:%d", __func__, aswRet);
	}

	/* Close Hal Power */
	aswRet = HAL_Close(nvbPmHalHandle);
	if(HAL_OK != aswRet){
		PM_DBG("\n######## PM HAL_Close Failed:%d ########\n", aswRet);
		//LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]HAL_Ioctl Failed:%d", __func__, aswRet);
	}

}

/********************************************************************************************/
/**	\function		nvdPmSysReset
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	StandbyIn Reset
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
static void nvdPmSysReset(Type_uByte aubType)
{
	Type_sWord aswRet = HAL_OK;
	
	nvdPmDownCtrl();

	aswRet = HAL_Ioctl(nvbPmHalHandle, HAL_IO_PM_ENTER_POWERRESET, &aubType, 0, 0, 0);
	if(HAL_OK != aswRet){
		PM_DBG("\n######## PM HAL_Ioctl POWERRESET Failed:%d ########\n", aswRet);
		//LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]HAL_Ioctl Failed:%d", __func__, aswRet);
	}

	
	/* Close Hal Power */
	aswRet = HAL_Close(nvbPmHalHandle);
	if(HAL_OK != aswRet){
		PM_DBG("\n######## PM HAL_Close Failed:%d ########\n", aswRet);
		//LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]HAL_Ioctl Failed:%d", __func__, aswRet);
	}

}

/********************************************************************************************/
/**	\function		nvdPmDownCtrl
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	PM Module Down
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
static void nvdPmDownCtrl(void)
{
	OS_ErrCode_t aswRet = OS_ERR_OK;
	
	/* Stop Soc Init GTimer */
	aswRet = OS_StopTimer(OS_TIMER_ID_PMSOCSTARTGT);
	if(aswRet != OS_ERR_OK){
		PM_DBG("\n######## [%s]PM SocInit Timer Stop Failed:%d ########\n", __func__, aswRet);
		//LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]SocInit Timer Stop Failed:%d",__func__, aswRet);
	}

	/* Stop StandbyOut GTimer */
	aswRet = OS_StopTimer(OS_TIMER_ID_PMSTANDBYOUTGT);
	if(aswRet != OS_ERR_OK){
		PM_DBG("\n######## [%s]PM StandbyOut Timer Stop Failed:%d ########\n", __func__, aswRet);
		//LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]StandbyOut Timer Stop Failed:%d",__func__, aswRet);
	}

	/* Stop StandbyIn GTimer */
	aswRet = OS_StopTimer(OS_TIMER_ID_PMSTANDBYINGT);
	if(aswRet != OS_ERR_OK){
		PM_DBG("\n######## [%s]PM StandbyIn Timer Stop Failed:%d ########\n", __func__, aswRet);
		//LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]StandbyIn Timer Stop Failed:%d",__func__, aswRet);
	}

	/* Stop Power On Timer */
	aswRet = OS_StopTimer(OS_TIMER_ID_PMPOWERKEYONGT);
	if(aswRet != OS_ERR_OK){
		PM_DBG("\n######## [%s]PM PowerOn Timer Stop Failed:%d ########\n", __func__, aswRet);
		//LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]PowerOn Timer Stop Failed:%d",__func__, aswRet);
	}

	/* Stop Power Off Timer */
	aswRet = OS_StopTimer(OS_TIMER_ID_PMPOWERKEYOFFGT);
	if(aswRet != OS_ERR_OK){
		PM_DBG("\n######## [%s]PM PowerOff Timer Stop Failed:%d ########\n", __func__, aswRet);
		//LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]PowerOff Timer Stop Failed:%d",__func__, aswRet);
	}


#if 0
	/* Stop Update Reset Timer */
	OS_StopTimer(OS_TIMER_ID_PMUPDATERESETGT);

	/* Stop Update Resest Timer */
	OS_StopTimer(OS_TIMER_ID_PMFACTORYRESETGT);
#endif
}

/********************************************************************************************/
/**	\function		nvdPmAccStandbyInStart
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	StandbyIn Event Operate
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
static void nvdPmAccStandbyInStart(OS_StdQueue_ts *astMessage_p)
{
	LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"ACC or H-DET StandbyIn Start");
	nvdPmStandbyInCtrlStart(astMessage_p);
}

/********************************************************************************************/
/**	\function		nvdPmUpdateStandbyInStart
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	StandbyIn Event Operate
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
static void nvdPmUpdateStandbyInStart(OS_StdQueue_ts *astMessage_p)
{
	LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"Update StandbyIn Start");
	nvdPmStandbyInCtrlStart(astMessage_p);
}

/********************************************************************************************/
/**	\function		nvdPmUpdateInitStandbyInStart
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	StandbyIn Event Operate
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
static void nvdPmUpdateInitStandbyInStart(OS_StdQueue_ts *astMessage_p)
{
	LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"Update Init StandbyIn Start");
	nvdPmStandbyInCtrlStart(astMessage_p);
}

/********************************************************************************************/
/**	\function		nvdPmFactoryStandbyInStart
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	StandbyIn Event Operate
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
static void nvdPmFactoryStandbyInStart(OS_StdQueue_ts *astMessage_p)
{
	LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"Factory Init StandbyIn Start");
	nvdPmStandbyInCtrlStart(astMessage_p);
}

/********************************************************************************************/
/**	\function		nvdPmWdtA53StandbyInStart
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	StandbyIn Event Operate
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
static void nvdPmWdtA53StandbyInStart(OS_StdQueue_ts *astMessage_p)
{
	LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"System A53 Watchdog StandbyIn Start");
	nvdPmStandbyInCtrlStart(astMessage_p);
}

/********************************************************************************************/
/**	\function		nvdPmWdtA7StandbyInStart
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	StandbyIn Event Operate
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
static void nvdPmWdtA7StandbyInStart(OS_StdQueue_ts *astMessage_p)
{
	LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"System A7 Watchdog StandbyIn Start");
	nvdPmStandbyInCtrlStart(astMessage_p);
}

/********************************************************************************************/
/**	\function		nvdPmA53ReqStandbyInStart
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	StandbyIn Event Operate
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
static void nvdPmA53ReqStandbyInStart(OS_StdQueue_ts *astMessage_p)
{
	LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"A53 Reboot Req StandbyIn Start");
	nvdPmStandbyInCtrlStart(astMessage_p);
}

/********************************************************************************************/
/**	\function		nvdPmA7ReqStandbyInStart
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	StandbyIn Event Operate
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
static void nvdPmA7ReqStandbyInStart(OS_StdQueue_ts *astMessage_p)
{
	LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"A7 Reboot Req StandbyIn Start");
	nvdPmStandbyInCtrlStart(astMessage_p);
}

/********************************************************************************************/
/**	\function		nvdPmA53HeartStandbyInStart
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	StandbyIn Event Operate
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
static void nvdPmA53HeartStandbyInStart(OS_StdQueue_ts *astMessage_p)
{
	LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"A53 Heart Timout StandbyIn Start");
	nvdPmStandbyInCtrlStart(astMessage_p);
}

/********************************************************************************************/
/**	\function		nvdPmA7HeartStandbyInStart
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	StandbyIn Event Operate
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
static void nvdPmA7HeartStandbyInStart(OS_StdQueue_ts *astMessage_p)
{
	LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"A7 Heart Timout StandbyIn Start");
	nvdPmStandbyInCtrlStart(astMessage_p);
}

/********************************************************************************************/
/**	\function		nvdPmStandbyInCtrlStart
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	StandbyIn Event Operate
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
static void nvdPmStandbyInCtrlStart(OS_StdQueue_ts *astMessage_p)
{
	OS_ErrCode_t aswRet = OS_ERR_OK;
	
	/* Stop StandbyOut GTimer */
	aswRet = OS_StopTimer(OS_TIMER_ID_PMSTANDBYOUTGT);
	if(aswRet != OS_ERR_OK){
		PM_DBG("\n######## PM Timer Stop Failed:%d ########\n", aswRet);
		LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]Timer Stop Failed:%d",__func__, aswRet);
	}

	nubPmPriorityLineIndex = PM_PRIORITY_LINE_MAX;
	nuwPmModuleResFlg = PM_DATA_ZERO;
	
#if (TEMP_COMPILE_SWITCH_05WORK_10WORK != TEMP_COMPILE_SWITCH_05WORK)
	wvdPmDrvDetOffStage0();
#endif
	nvdPmStandbyInCtrlProc(astMessage_p);
}

/********************************************************************************************/
/**	\function		nvdPmStandbyInCtrlProc
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	StanbyIn Process
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
static void nvdPmStandbyInCtrlProc(OS_StdQueue_ts *astMessage_p)
{
	Type_uByte aubPriorityIndex = PM_DATA_ZERO;
	Type_uByte aubProcIndex = PM_DATA_ZERO;
	Type_uWord auwGTimer = PM_DATA_ZERO;
	Type_uByte aubFlag = FALSE;

	PM_DBG("\n************ StandbyIn Start:%d ************\n",nubPmPriorityLineIndex);

	if(nubPmPriorityLineIndex > PM_PRIORITY_LINE_MAX){
		PM_DBG("\n###########PM nubPmPriorityLineIndex:%d is bigger than Max ###########\n", nubPmPriorityLineIndex);
		return;
	}
	
	while((nubPmPriorityLineIndex > 0) && (aubFlag != TRUE)){
		nubPmPriorityLineIndex--;
		for(aubPriorityIndex = PM_DATA_ZERO; aubPriorityIndex < PM_PRIORITY_ROW_MAX; aubPriorityIndex++){
			if(nuwModulePriority_Tbl[nubPmPriorityLineIndex][aubPriorityIndex] != PM_MODULE_NONE){
				for(aubProcIndex = PM_DATA_ZERO; aubProcIndex < (sizeof(nstPmStandby_Tbl)/sizeof(PmStandbyIf_ts)); aubProcIndex++){
					if(nstPmStandby_Tbl[aubProcIndex].ubModuleID == nuwModulePriority_Tbl[nubPmPriorityLineIndex][aubPriorityIndex]){
						if(nstPmStandby_Tbl[aubProcIndex].standbyInProc_fp != NULL){
							aubFlag = TRUE;
							(void)nstPmStandby_Tbl[aubProcIndex].standbyInProc_fp();
							if(nstPmStandby_Tbl[aubProcIndex].uwStandbyInTime > PM_DATA_ZERO){
								nuwPmModuleResFlg |= (PM_BIT_TRUE << aubPriorityIndex);
							}
							if(auwGTimer < nstPmStandby_Tbl[aubProcIndex].uwStandbyInTime){
								auwGTimer = nstPmStandby_Tbl[aubProcIndex].uwStandbyInTime;
							}
						}

						break;
					}
				}

			}
		}

	}

	if(auwGTimer == PM_DATA_ZERO){
		if(nubPmPriorityLineIndex > PM_DATA_ZERO){
			nvdPmSendMsg(ARPC_ID_R5_POWERMNG, EV_INF_PM_STANDBYIN_ING, NULL, PM_DATA_ZERO);
		}
		else{
			nvdPmSendMsg(ARPC_ID_R5_POWERMNG, EV_INF_PM_STANDBYIN_OVER, NULL, PM_DATA_ZERO);
		}
	}
	else{
		/* Start StandbyIn Timer */
		OS_TimerChangePeriod(OS_TIMER_ID_PMSTANDBYINGT, auwGTimer);
	}
	
}

/********************************************************************************************/
/**	\function		nvdPmStandbyResInCtrl
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	StandbyIn Response
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
static void nvdPmStandbyInResCtrl(OS_StdQueue_ts *astMessage_p)
{
#ifdef PM_ST_EN
		if((nubPmStNo == PM_ST_NO_2321) && (astMessage_p->swSrcTaskId == ARPC_ID_R5_POWERDRV)){
			printf("\n@@@@@[%s] ST NO is %x \n", __func__, nubPmStNo);
			return;
		}
		else if((nubPmStNo == PM_ST_NO_2322) && (astMessage_p->swSrcTaskId == ARPC_ID_EEPROM)){
			printf("\n@@@@@[%s] ST NO is %x \n", __func__, nubPmStNo);
			return;
		}
		else if((nubPmStNo == PM_ST_NO_2323) && (astMessage_p->swSrcTaskId == ARPC_ID_A53_POWER)){
			printf("\n@@@@@[%s] ST NO is %x \n", __func__, nubPmStNo);
			return;
		}
		else{
		}
#endif

	nvdPmUpdateResFlg(astMessage_p);

	if(nuwPmModuleResFlg == PM_DATA_ZERO){

		/* Stop StandbyIn Timer */
		OS_StopTimer(OS_TIMER_ID_PMSTANDBYINGT);
		
		nvdPmStandbyInNextCtrl();
	}
}

#if 0
/********************************************************************************************/
/**	\function		nvdPmPowerUpdateResetCtrlStart
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	Update Reset Event Operate
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 ********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
static void nvdPmUpdateResetCtrlStart(OS_StdQueue_ts *astMessage_p)
{
	Type_uWord auwGTimer = PM_DATA_ZERO;
	Type_uByte aubSize = sizeof(nstPmUpdateReset_Tbl)/sizeof(PmComAsyncIf_ts);

	nubPmPriorityLineIndex = PM_PRIORITY_LINE_MAX;
	nuwPmModuleResFlg = PM_DATA_ZERO;

	nvdPmSendMsg(ARPC_ID_HMI, EV_INF_UPDATE_UPDATE_RESET_RES, NULL, 0);

	auwGTimer = nvdPmResetCtrlProc(&nstPmUpdateReset_Tbl[0], aubSize);
	if(auwGTimer == PM_DATA_ZERO){
		if(nubPmPriorityLineIndex > PM_DATA_ZERO){
			nvdPmSendMsg(ARPC_ID_R5_POWERMNG, EV_INF_PM_UPDATE_RESET_ING, NULL, PM_DATA_ZERO);
		}
		else{
			nvdPmSendMsg(ARPC_ID_R5_POWERMNG, EV_INF_PM_UPDATE_RESET_OVER, NULL, PM_DATA_ZERO);
		}
	}
	else{
		/* Start Update Reset Timer */
		OS_TimerChangePeriod(OS_TIMER_ID_PMUPDATERESETGT, auwGTimer);
		
	}
}

/********************************************************************************************/
/**	\function		nvdPmUpdateResetCtrlProc
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	Power Key Off Process
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
static void nvdPmUpdateResetCtrlProc(OS_StdQueue_ts *astMessage_p)
{
	Type_uWord auwGTimer = PM_DATA_ZERO;
	Type_uByte aubSize = sizeof(nstPmUpdateReset_Tbl)/sizeof(PmComAsyncIf_ts);

	auwGTimer = nvdPmResetCtrlProc(&nstPmUpdateReset_Tbl[0], aubSize);
	if(auwGTimer == PM_DATA_ZERO){
		if(nubPmPriorityLineIndex > PM_DATA_ZERO){
			nvdPmSendMsg(ARPC_ID_R5_POWERMNG, EV_INF_PM_UPDATE_RESET_ING, NULL, PM_DATA_ZERO);
		}
		else{
			nvdPmSendMsg(ARPC_ID_R5_POWERMNG, EV_INF_PM_UPDATE_RESET_OVER, NULL, PM_DATA_ZERO);
		}
	}
	else{
		/* Start Update Reset Timer */
		OS_TimerChangePeriod(OS_TIMER_ID_PMUPDATERESETGT, auwGTimer);
	}
}

/********************************************************************************************/
/**	\function		nvdPmUpdateResetResCtrl
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	PowerKey On Response
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
static void nvdPmUpdateResetResCtrl(OS_StdQueue_ts *astMessage_p)
{
	nvdPmUpdateResFlg(astMessage_p);

	if(nuwPmModuleResFlg == PM_DATA_ZERO){

		/* Stop Update Reset Timer */
		OS_StopTimer(OS_TIMER_ID_PMUPDATERESETGT);

		nvdPmUpdateResetNextCtrl();
	}

}

/********************************************************************************************/
/**	\function		nvdPmUpdateResetNextCtrl
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	Update Reset Next Level Module  Run
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
static void nvdPmUpdateResetNextCtrl(void)
{
	if(nubPmPriorityLineIndex > PM_DATA_ZERO){
		nvdPmSendMsg(ARPC_ID_R5_POWERMNG, EV_INF_PM_UPDATE_RESET_ING, NULL, PM_DATA_ZERO);
	}
	else{
		nvdPmSendMsg(ARPC_ID_R5_POWERMNG, EV_INF_PM_UPDATE_RESET_OVER, NULL, PM_DATA_ZERO);
	}
}


/********************************************************************************************/
/**	\function		nvdPmResetCtrlProc
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	Power Key Off Process
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
static Type_uByte nvdPmResetCtrlProc(const PmComAsyncIf_ts *astProcTbl_p, Type_uByte aubTblSize)
{
	Type_uByte aubPriorityIndex = PM_DATA_ZERO;
	Type_uByte aubProcIndex = PM_DATA_ZERO;
	Type_uWord auwGTimer = PM_DATA_ZERO;
	Type_uByte aubFlag = FALSE;

	PM_DBG("\n###########Reset Start:%d##########\n",nubPmPriorityLineIndex);

	if(nubPmPriorityLineIndex > PM_PRIORITY_LINE_MAX){
		return auwGTimer;
	}
	
	while((nubPmPriorityLineIndex > 0) && (aubFlag != TRUE)){
		nubPmPriorityLineIndex--;
		for(aubPriorityIndex = PM_DATA_ZERO; aubPriorityIndex < PM_PRIORITY_ROW_MAX; aubPriorityIndex++){
			if(nuwModulePriority_Tbl[nubPmPriorityLineIndex][aubPriorityIndex] != PM_MODULE_NONE){
				for(aubProcIndex = PM_DATA_ZERO; aubProcIndex < aubTblSize; aubProcIndex++){
					if(astProcTbl_p[aubProcIndex].ubModuleID == nuwModulePriority_Tbl[nubPmPriorityLineIndex][aubPriorityIndex]){
						if(astProcTbl_p[aubProcIndex].proc_fp != NULL){
							aubFlag = TRUE;
							astProcTbl_p[aubProcIndex].proc_fp();
							if(astProcTbl_p[aubProcIndex].uwTime != PM_DATA_ZERO){
								nuwPmModuleResFlg |= (PM_BIT_TRUE << aubPriorityIndex);
							}
							if(auwGTimer < astProcTbl_p[aubProcIndex].uwTime){
								auwGTimer = astProcTbl_p[aubProcIndex].uwTime;
							}
						}

						break;
					}
				}

			}
		}

	}

	return auwGTimer;
	
}

/********************************************************************************************/
/**	\function		nvdPmFactoryResetCtrlStart
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	Factory Reset Event Operate
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 ********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
static void nvdPmFactoryResetCtrlStart(OS_StdQueue_ts *astMessage_p)
{
	Type_uWord auwGTimer = PM_DATA_ZERO;
	Type_uByte aubSize = sizeof(nstPmFactoryReset_Tbl)/sizeof(PmComAsyncIf_ts);

	nubPmPriorityLineIndex = PM_PRIORITY_LINE_ARRAY_MAX;
	nuwPmModuleResFlg = PM_DATA_ZERO;

	nvdPmSendMsg(ARPC_ID_HMI, EV_INF_HMI_FACTORY_RESET_RES, NULL, 0);

	auwGTimer = nvdPmResetCtrlProc(&nstPmFactoryReset_Tbl[PM_DATA_ZERO], aubSize);
	if(auwGTimer == PM_DATA_ZERO){
		if(nubPmPriorityLineIndex > PM_DATA_ZERO){
			nvdPmSendMsg(ARPC_ID_R5_POWERMNG, EV_INF_PM_FACTORY_RESET_ING, NULL, PM_DATA_ZERO);
		}
		else{
			nvdPmSendMsg(ARPC_ID_R5_POWERMNG, EV_INF_PM_FACTORY_RESET_OVER, NULL, PM_DATA_ZERO);
		}
	}
	else{
		/* Start Factory Reset Timer */
		OS_TimerChangePeriod(OS_TIMER_ID_PMFACTORYRESETGT, auwGTimer);
	}
}

/********************************************************************************************/
/**	\function		nvdPmFactoryResetCtrlProc
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	Factory Reset Process
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
static void nvdPmFactoryResetCtrlProc(OS_StdQueue_ts *astMessage_p)
{
	Type_uWord auwGTimer = PM_DATA_ZERO;
	Type_uByte aubSize = sizeof(nstPmUpdateReset_Tbl)/sizeof(PmComAsyncIf_ts);

	auwGTimer = nvdPmResetCtrlProc(&nstPmFactoryReset_Tbl[PM_DATA_ZERO], aubSize);
	if(auwGTimer == PM_DATA_ZERO){
		if(nubPmPriorityLineIndex > PM_DATA_ZERO){
			nubPmPriorityLineIndex--;
			nvdPmSendMsg(ARPC_ID_R5_POWERMNG, EV_INF_PM_FACTORY_RESET_ING, NULL, PM_DATA_ZERO);
		}
		else{
			nvdPmSendMsg(ARPC_ID_R5_POWERMNG, EV_INF_PM_FACTORY_RESET_OVER, NULL, PM_DATA_ZERO);
		}
	}
	else{
		/* Start Factory Reset Timer */
		OS_TimerChangePeriod(OS_TIMER_ID_PMFACTORYRESETGT, auwGTimer);
	}

}

/********************************************************************************************/
/**	\function		nvdPmFactoryResetResCtrl
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	Factory Reset Response
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
static void nvdPmFactoryResetResCtrl(OS_StdQueue_ts *astMessage_p)
{
	nvdPmUpdateResFlg(astMessage_p);

	if(nuwPmModuleResFlg == PM_DATA_ZERO){

		/* Stop Factory Reset Timer */
		OS_StopTimer(OS_TIMER_ID_PMFACTORYRESETGT);

		nvdPmFactoryResetNextCtrl();
	}

}

/********************************************************************************************/
/**	\function		nvdPmFactoryResetNextCtrl
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	Factory Reset Next Level Module Run
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
static void nvdPmFactoryResetNextCtrl(void)
{
	if(nubPmPriorityLineIndex > 0){
		nvdPmSendMsg(ARPC_ID_R5_POWERMNG, EV_INF_PM_FACTORY_RESET_ING, NULL, PM_DATA_ZERO);
	}
	else{
		nvdPmSendMsg(ARPC_ID_R5_POWERMNG, EV_INF_PM_FACTORY_RESET_OVER, NULL, PM_DATA_ZERO);
	}
}

/********************************************************************************************/
/**	\function		nubPmSocUpdateReset
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	Send Update Reset Message to Soc
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
static Type_uByte nubPmSocUpdateReset(void)
{
	PM_DBG("\n************Send UpdateReset to A53***************\n");
	LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF,"[%s]Send Update to A53",__func__);
	
	nvdPmSendMsg(ARPC_ID_A53_POWER, EV_INF_UPDATE_RESET_NOTIFY, NULL, 0);
	
	return RES_OK;
}

/********************************************************************************************/
/**	\function		nvdPmSocUpdateFactoryReset
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	Send Factory Reset Message to Soc
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
static Type_uByte nubPmSocFactoryReset(void)
{
	PM_DBG("\n************Send FactoryReset to A53***************\n");
	LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF,"[%s]Send Factory to A53",__func__);
	
	nvdPmSendMsg(ARPC_ID_A53_POWER, EV_INF_FACTORY_RESET_NOTIFY, NULL, 0);
	
	return RES_OK;
}
#endif

/********************************************************************************************/
/**	\function		nvdPmUpdatePowerKeyResFlg
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	Update Response Flag
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
static void nvdPmUpdatePowerKeyResFlg(OS_StdQueue_ts *astMessage_p)
{
	Type_uByte aubPriorityIndex = PM_DATA_ZERO;
	
	if(nubPmPowerKeyPriorityLineIndex >= PM_PRIORITY_LINE_MAX){
		PM_DBG("\n########### PM nubPmPowerKeyPriorityLineIndex:%d is bigger than Max priority ##############\n", nubPmPowerKeyPriorityLineIndex);
		LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_ERROR,ATAT_LEVEL_OFF,"PowerKey Priority=%2d is too big",nubPmPowerKeyPriorityLineIndex);
		return;
	}

	if(nuwPmPowerKeyModuleResFlg == PM_DATA_ZERO){
		PM_DBG("\n########### PM Invalid nuwPmPowerKeyModuleResFlg is also zero ##############\n");
		LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_ERROR,ATAT_LEVEL_OFF,"PowerKey All Response is also received");
		return;
	}

	for(aubPriorityIndex = PM_DATA_ZERO; aubPriorityIndex < PM_PRIORITY_ROW_MAX; aubPriorityIndex++){
		if(astMessage_p->swSrcTaskId == nuwModulePriority_Tbl[nubPmPowerKeyPriorityLineIndex][aubPriorityIndex]){
			nuwPmPowerKeyModuleResFlg &= ~(PM_BIT_TRUE << aubPriorityIndex);
			break;
		}
	}
}

/********************************************************************************************/
/**	\function		nvdPmPowerKeyOnCtrlStart
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	Power Key On Event Operate
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
static void nvdPmPowerKeyOnCtrlStart(OS_StdQueue_ts *astMessage_p)
{
	LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"PowerKey On Start");
	
	nubPmPowerKeyPriorityLineIndex = PM_DATA_ZERO;
	nuwPmPowerKeyModuleResFlg = PM_DATA_ZERO;
	
	nvdPmSendMsg(ARPC_ID_HMI, EV_INF_HMI_POWERON_RES, NULL, PM_DATA_ZERO);
	nvdPmPowerKeyOnCtrlProc(astMessage_p);
}

/********************************************************************************************/
/**	\function		nvdPmPowerKeyOnCtrlProc
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	Power Key On Process
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
static void nvdPmPowerKeyOnCtrlProc(OS_StdQueue_ts *astMessage_p)
{
	Type_uByte aubPriorityIndex = PM_DATA_ZERO;
	Type_uByte aubProcIndex = PM_DATA_ZERO;
	Type_uWord auwGTimer = PM_DATA_ZERO;
	Type_uByte aubFlag = FALSE;
	OS_ErrCode_t aswRet = OS_ERR_OK;

	LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"PowerKey On Index=%d", nubPmPowerKeyPriorityLineIndex);

	if(nubPmPowerKeyPriorityLineIndex >= PM_PRIORITY_LINE_MAX){
		PM_DBG("\n########### PM nubPmPowerKeyPriorityLineIndex:%d is bigger than Max priority ##############\n", nubPmPowerKeyPriorityLineIndex);
		LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_ERROR,ATAT_LEVEL_OFF,"PowerKey On Priority=%2d is too big",nubPmPowerKeyPriorityLineIndex);
		return;
	}

	for(;nubPmPowerKeyPriorityLineIndex < PM_PRIORITY_LINE_MAX;nubPmPowerKeyPriorityLineIndex++){
		for(aubPriorityIndex = PM_DATA_ZERO; aubPriorityIndex < PM_PRIORITY_ROW_MAX; aubPriorityIndex++){
			if(nuwModulePriority_Tbl[nubPmPowerKeyPriorityLineIndex][aubPriorityIndex] != PM_MODULE_NONE){
				for(aubProcIndex = PM_DATA_ZERO; aubProcIndex < (sizeof(nstPmPowerKey_Tbl)/sizeof(PmPowerKeyIf_ts)); aubProcIndex++){
					if(nstPmPowerKey_Tbl[aubProcIndex].ubModuleID == nuwModulePriority_Tbl[nubPmPowerKeyPriorityLineIndex][aubPriorityIndex]){
						if(nstPmPowerKey_Tbl[aubProcIndex].onProc_fp != NULL){
							aubFlag = TRUE;
							(void)nstPmPowerKey_Tbl[aubProcIndex].onProc_fp();
							if(nstPmPowerKey_Tbl[aubProcIndex].uwOnTime != PM_DATA_ZERO){
								nuwPmPowerKeyModuleResFlg |= (PM_BIT_TRUE << aubPriorityIndex);
							}
							if(auwGTimer < nstPmPowerKey_Tbl[aubProcIndex].uwOnTime){
								auwGTimer = nstPmPowerKey_Tbl[aubProcIndex].uwOnTime;
							}
						}

						break;
					}
				}

			}
		}

		if(aubFlag == TRUE){
			break;
		}
	}

	if(auwGTimer == PM_DATA_ZERO){
		if(nubPmPowerKeyPriorityLineIndex < PM_PRIORITY_LINE_ARRAY_MAX){
			nubPmPowerKeyPriorityLineIndex++;
			nvdPmSendMsg(ARPC_ID_R5_POWERMNG, EV_INF_PM_POWERON_ING, NULL, PM_DATA_ZERO);
		}
		else{
			nvdPmSendMsg(ARPC_ID_R5_POWERMNG, EV_INF_PM_POWERON_OVER, NULL, PM_DATA_ZERO);
		}
	}
	else{
		/* Start Power On Timer */
		aswRet = OS_TimerChangePeriod(OS_TIMER_ID_PMPOWERKEYONGT, auwGTimer);
		if(aswRet != OS_ERR_OK){
			PM_DBG("\n######## PM Timer Stop Failed:%d ########\n", aswRet);
			LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]Timer Start Failed:%d",__func__, aswRet);
		}
	}
	
}

/********************************************************************************************/
/**	\function		nvdPmPowerKeyOnResCtrl
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	PowerKey On Response
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
static void nvdPmPowerKeyOnResCtrl(OS_StdQueue_ts *astMessage_p)
{
	OS_ErrCode_t aswRet = OS_ERR_OK;
	
	nvdPmUpdatePowerKeyResFlg(astMessage_p);

	if(nuwPmPowerKeyModuleResFlg == PM_DATA_ZERO){

		/* Stop Power On Timer */
		aswRet = OS_StopTimer(OS_TIMER_ID_PMPOWERKEYONGT);
		if(aswRet != OS_ERR_OK){
			PM_DBG("\n######## PM Timer Stop Failed:%d ########\n", aswRet);
			LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]Timer Stop Failed:%d",__func__, aswRet);
		}

		nvdPmPowerKeyOnNextCtrl();
	}

}

/********************************************************************************************/
/**	\function		nvdPmPowerKeyOnNextCtrl
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	Power Key On Next Level Module Run
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
static void nvdPmPowerKeyOnNextCtrl(void)
{
	if(nubPmPowerKeyPriorityLineIndex < PM_PRIORITY_LINE_ARRAY_MAX){
		nubPmPowerKeyPriorityLineIndex++;
		nvdPmSendMsg(ARPC_ID_R5_POWERMNG, EV_INF_PM_POWERON_ING, NULL, PM_DATA_ZERO);
	}
	else{
		nvdPmSendMsg(ARPC_ID_R5_POWERMNG, EV_INF_PM_POWERON_OVER, NULL, PM_DATA_ZERO);
	}
}


/********************************************************************************************/
/**	\function		nvdPmPowerKeyOffCtrlStart
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	Power Key On Event Operate
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
static void nvdPmPowerKeyOffCtrlStart(OS_StdQueue_ts *astMessage_p)
{
	LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"PowerKey Off Start");
	
	nubPmPowerKeyPriorityLineIndex = PM_PRIORITY_LINE_MAX;
	nuwPmPowerKeyModuleResFlg = PM_DATA_ZERO;
	
	nvdPmSendMsg(ARPC_ID_HMI, EV_INF_HMI_POWEROFF_RES, NULL, PM_DATA_ZERO);
	nvdPmPowerKeyOffCtrlProc(astMessage_p);
}

/********************************************************************************************/
/**	\function		nvdPmPowerKeyOffCtrlProc
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	Power Key Off Process
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
static void nvdPmPowerKeyOffCtrlProc(OS_StdQueue_ts *astMessage_p)
{
	Type_uByte aubPriorityIndex = PM_DATA_ZERO;
	Type_uByte aubProcIndex = PM_DATA_ZERO;
	Type_uWord auwGTimer = PM_DATA_ZERO;
	Type_uByte aubFlag = FALSE;
	OS_ErrCode_t aswRet = OS_ERR_OK;

	PM_DBG("\n*********** PM PowerOff Start:%d *************\n",nubPmPowerKeyPriorityLineIndex);
	LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF,"PowerKey Off Index=%d", nubPmPowerKeyPriorityLineIndex);

	if(nubPmPowerKeyPriorityLineIndex > PM_PRIORITY_LINE_MAX){
		PM_DBG("\n########### PM nubPmPowerKeyPriorityLineIndex:%d is bigger than Max priority ##############\n", nubPmPowerKeyPriorityLineIndex);
		LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_ERROR,ATAT_LEVEL_OFF,"PowerKey Off Priority=%2d is too big",nubPmPowerKeyPriorityLineIndex);
		return;
	}
	
	while((nubPmPowerKeyPriorityLineIndex > 0) && (aubFlag != TRUE)){
		nubPmPowerKeyPriorityLineIndex--;
		for(aubPriorityIndex = PM_DATA_ZERO; aubPriorityIndex < PM_PRIORITY_ROW_MAX; aubPriorityIndex++){
			if(nuwModulePriority_Tbl[nubPmPowerKeyPriorityLineIndex][aubPriorityIndex] != PM_MODULE_NONE){
				for(aubProcIndex = PM_DATA_ZERO; aubProcIndex < (sizeof(nstPmPowerKey_Tbl)/sizeof(PmPowerKeyIf_ts)); aubProcIndex++){
					if(nstPmPowerKey_Tbl[aubProcIndex].ubModuleID == nuwModulePriority_Tbl[nubPmPowerKeyPriorityLineIndex][aubPriorityIndex]){
						if(nstPmPowerKey_Tbl[aubProcIndex].offProc_fp != NULL){
							aubFlag = TRUE;
							(void)nstPmPowerKey_Tbl[aubProcIndex].offProc_fp();
							if(nstPmPowerKey_Tbl[aubProcIndex].uwOnTime != PM_DATA_ZERO){
								nuwPmPowerKeyModuleResFlg |= (PM_BIT_TRUE << aubPriorityIndex);
							}
							if(auwGTimer < nstPmPowerKey_Tbl[aubProcIndex].uwOffTime){
								auwGTimer = nstPmPowerKey_Tbl[aubProcIndex].uwOffTime;
							}
						}

						break;
					}
				}

			}
		}

	}

	if(auwGTimer == PM_DATA_ZERO){
		if(nubPmPowerKeyPriorityLineIndex > PM_DATA_ZERO){
			nubPmPowerKeyPriorityLineIndex--;
			nvdPmSendMsg(ARPC_ID_R5_POWERMNG, EV_INF_PM_POWEROFF_ING, NULL, PM_DATA_ZERO);
		}
		else{
			nvdPmSendMsg(ARPC_ID_R5_POWERMNG, EV_INF_PM_POWEROFF_OVER, NULL, PM_DATA_ZERO);
		}
	}
	else{
		/* Start Power Off Timer */
		aswRet = OS_TimerChangePeriod(OS_TIMER_ID_PMPOWERKEYOFFGT, auwGTimer);
		if(aswRet != OS_ERR_OK){
			PM_DBG("\n######## PM Timer Stop Failed:%d ########\n", aswRet);
			LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]Timer Start Failed:%d",__func__, aswRet);
		}
	}
	
}

/********************************************************************************************/
/**	\function		nvdPmPowerKeyOffResCtrl
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	PowerKey On Response
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
static void nvdPmPowerKeyOffResCtrl(OS_StdQueue_ts *astMessage_p)
{
	OS_ErrCode_t aswRet = OS_ERR_OK;
	
	nvdPmUpdatePowerKeyResFlg(astMessage_p);

	if(nuwPmPowerKeyModuleResFlg == PM_DATA_ZERO){

		/* Stop Power Off Timer */
		aswRet = OS_StopTimer(OS_TIMER_ID_PMPOWERKEYOFFGT);
		if(aswRet != OS_ERR_OK){
			PM_DBG("\n######## PM Timer Stop Failed:%d ########\n", aswRet);
			LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]Timer Stop Failed:%d",__func__, aswRet);
		}

		nvdPmPowerKeyOffNextCtrl();
	}

}

/********************************************************************************************/
/**	\function		nvdPmPowerKeyOffNextCtrl
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	Power Key Next Level Module Run
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
static void nvdPmPowerKeyOffNextCtrl(void)
{
	if(nubPmPowerKeyPriorityLineIndex > 0){
		nvdPmSendMsg(ARPC_ID_R5_POWERMNG, EV_INF_PM_POWEROFF_ING, NULL, PM_DATA_ZERO);
	}
	else{
		nvdPmSendMsg(ARPC_ID_R5_POWERMNG, EV_INF_PM_POWEROFF_OVER, NULL, PM_DATA_ZERO);
	}
}

/********************************************************************************************/
/**	\function		nvdPmPowerKeyProcStop
 *	\date			2022/02/25
 *	\author			wangying
 *	\description	Power Key Next Level Module Run
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
static void nvdPmPowerKeyProcStop(void)
{
	OS_ErrCode_t aswRet = OS_ERR_OK;
	
	nubPmPowerKeyPriorityLineIndex = PM_DATA_ZERO;
	nuwPmPowerKeyModuleResFlg = PM_DATA_ZERO;

	/* Stop Power On Timer */
	aswRet = OS_StopTimer(OS_TIMER_ID_PMPOWERKEYONGT);
	if(aswRet != OS_ERR_OK){
		PM_DBG("\n######## [%s]PM PowerOn Timer Stop Failed:%d ########\n", __func__, aswRet);
		LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]On Timer Stop Failed:%d",__func__, aswRet);
	}

	/* Stop Power Off Timer */
	aswRet = OS_StopTimer(OS_TIMER_ID_PMPOWERKEYOFFGT);
	if(aswRet != OS_ERR_OK){
		PM_DBG("\n######## [%s]PM PowerOff Timer Stop Failed:%d ########\n", __func__, aswRet);
		LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]Off Timer Stop Failed:%d",__func__, aswRet);
	}
}

/********************************************************************************************/
/**	\function		nvdPmSendMsg
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
static void nvdPmSendMsg(Type_uByte aubReceiver, Type_uHWord auhEvent, void * auvData_p, Type_uHWord auhLength)
{
	Type_sByte asbRet = RPC_NG;
		
	asbRet = wsbRPCSendMsg(ARPC_ID_R5_POWERMNG, aubReceiver, auhEvent, auvData_p, auhLength);
	if(RPC_OK != asbRet){
		PM_DBG("\n########## PM Send Rpc Failed ##########\n");
		LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]RPC Send Failed",__func__);
	}
}

/********************************************************************************************/
/**	\function		nubPmSocStandbyOut
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	Send StandbyOut Message to Soc
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
static Type_uByte nubPmSocStandbyOut(void)
{
	PM_DBG("\n************ PM Send StanbyOut to A53:%d ***************\n",nubStartType);
	LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"Send StanbyOut to A53:%d", nubStartType);
	
	nvdPmSendMsg(ARPC_ID_A53_POWER, EV_INF_STANDBYOUT_NOTIFY, &nubStartType, sizeof(nubStartType));

	return RES_OK;
}

/********************************************************************************************/
/**	\function		nubPmSocStandbyIn
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	Send StandbyIn Message to Soc
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
static Type_uByte nubPmSocStandbyIn(void)
{
	PM_DBG("\n************%d: Send StanbyIn to A53***************\n", OSAL_GetOSTimeTick());
	LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"Send StanbyIn to A53");

	nvdPmSendMsg(ARPC_ID_A53_POWER, EV_INF_STANDBYIN_NOTIFY, NULL, PM_DATA_ZERO);
	
	return RES_OK;
}

/********************************************************************************************/
/**	\function		nubPmSocPowerOn
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	Send PowerOn Message to Soc
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
static Type_uByte nubPmSocPowerOn(void)
{
	PM_DBG("\n************Send PowerOn to A53***************\n");
	LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"Send PowerKey On to A53");
	
	nvdPmSendMsg(ARPC_ID_A53_POWER, EV_INF_POWERON_NOTIFY, NULL, PM_DATA_ZERO);

	return RES_OK;
}

/********************************************************************************************/
/**	\function		nubPmSocPowerOff
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	Send PowerOff Message to Soc
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
static Type_uByte nubPmSocPowerOff(void)
{
	PM_DBG("\n************Send PowerOff to A53***************\n");
	LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"Send PowerKey Off to A53");
	
	nvdPmSendMsg(ARPC_ID_A53_POWER, EV_INF_POWEROFF_NOTIFY, NULL, PM_DATA_ZERO);
	
	return RES_OK;
}

