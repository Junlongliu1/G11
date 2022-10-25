/********************************************************************************************/
/** <!-- Copyright (C) Alpine Electronics, Inc. All rights reserved. ------------------------>
 *
 *	\file			pm.h
 *	\date			2021/11/12
 *	\author			wangying
 *	\model			G11
 *	\description	power manager
 *	\version

 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
#ifndef PM_H
#define PM_H 

/********************************************************************************************/
/* 						 Include File Section												*/
/********************************************************************************************/
#include "apn_basic_type.h"
#include "stdlib.h"
#include "os_framework_type.h"
#include "os_framework.h"
#include "generic.h"
#include "rpc_event.h"
#include <HAL_API.h>

/********************************************************************************************/
/*							Macro Definition Section										*/
/********************************************************************************************/
//#define PM_DEBUG
#ifdef PM_DEBUG
#define PM_DBG(fmt, args...)   printf(fmt, ## args)
#else
#define PM_DBG(fmt, args...)   do {} while(0)
#endif

//#define PM_HAL_BEFORE_OS
//#define PM_ST_EN
#ifdef PM_ST_EN
#define PM_ST_NO_1140		0x1140	//4416:get time between reset pin to A02 pin(TestMethod:Change PM_ST_INIT_NO)
#define PM_ST_NO_1420		0x1420	//5152:StandbyOut Timing(TestMethod:Change PM_ST_INIT_NO)
#define PM_ST_NO_1421		0x1421	//5153:StandbyIn(ACC) Timing(TestMethod:Change PM_ST_INIT_NO)
#define PM_ST_NO_2120		0x2120	//8480:get time between ADET(LOW) pin to A02 pin(HIGH)(TestMethod:Change PM_ST_INIT_NO or Console Cmd ST_POWER)
#define PM_ST_NO_2220		0x2220	//8736:get time between ADET(HIGH) pin to A02 pin(HIGH)(TestMethod:Change PM_ST_INIT_NO)
#define PM_ST_NO_2221		0x2221	//8737: time between ADET(HIGH) pin to A02 pin(HIGH)(TestMethod:Change PM_ST_INIT_NO)
#define PM_ST_NO_2222		0x2222	//8738:get time between ADET(HIGH) pin to A02 pin(HIGH)(TestMethod:Change PM_ST_INIT_NO)
#define PM_ST_NO_2223		0x2223	//8739:get time between ADETT(HIGH) pin to A02 pin(HIGH) and run reset(TestMethod:Change PM_ST_INIT_NO)
#define PM_ST_NO_2320		0x2320	//8992:get time between ADET(LOW) pin to A02 pin(HIGH)(TestMethod:Change PM_ST_INIT_NO or Console Cmd ST_POWER)
#define PM_ST_NO_2321		0x2321	//8993:get time between ADET(LOW) pin to A02 pin(HIGH)(TestMethod:Change PM_ST_INIT_NO or Console Cmd ST_POWER)
#define PM_ST_NO_2322		0x2322	//8994:get time between ADET(LOW) pin to A02 pin(HIGH)(TestMethod:Change PM_ST_INIT_NO or Console Cmd ST_POWER)
#define PM_ST_NO_2323		0x2323	//8995:get time between ADET(LOW) pin to A02 pin(HIGH)(TestMethod:Change PM_ST_INIT_NO or Console Cmd ST_POWER)
#define PM_ST_NO_5130		0x5130	//20784:output pin register under acc on(TestMethod:Console Cmd ST_POWER)
#define PM_ST_NO_5131		0x5131	//20785:output pin register under acc off(TestMethod:Change PM_ST_INIT_NO or Console Cmd ST_POWER)
#define PM_ST_NO_5132		0x5132	//20786:output pin register under power off(TestMethod:Console Cmd ST_POWER)
#define PM_ST_NO_5320		0x5320	//21280:acc on chattering(TestMethod:Change PM_ST_INIT_NO)
#define PM_ST_NO_5321		0x5321	//21281:hi-det on chattering(TestMethod:Change PM_ST_INIT_NO)
#define PM_ST_NO_5322		0x5322	//21282:acc off chattering(TestMethod:Change PM_ST_INIT_NO or Console Cmd ST_POWER)
#define PM_ST_NO_5323		0x5323	//21283:hi-det off chattering(TestMethod:Change PM_ST_INIT_NO or Console Cmd ST_POWER)
//#define PM_ST_NO_5324		0x5324	//21284:batt on chattering(TestMethod:Change PM_ST_INIT_NO)
//#define PM_ST_NO_5325		0x5325	//21285:batt off chattering(TestMethod:Change PM_ST_INIT_NO)
#define PM_ST_NO_6102		0x6102	//24834:StandbyIn(Hi-Det) Timing(TestMethod:Change PM_ST_INIT_NO)
#define PM_ST_NO_6420		0x6420	//25632:get time between A02 pin(HIGH) pin to A02 pin(LOW)(TestMethod:Change PM_ST_INIT_NO or Console Cmd ST_POWER)

#define PM_ST_NO_9000		0x9000	//36864:read NC pin level under StanbyOut(TestMethod:Console Cmd ST_POWER) required by hanrd part

#define PM_ST_INIT_NO		PM_ST_NO_6420
#endif


/********************************************************************************************/
/*							Type Definition Section											*/
/********************************************************************************************/
typedef Type_uByte (*exPmAsyncProc_fp)(void);
typedef void (*exPmSyncProc_fp)(void);
typedef void (*exPmCtrl_fp)(OS_StdQueue_ts *astMessage_p);


typedef struct{
	Type_uHWord		uhSocState;
	Type_uHWord 	uhStandbyState;
	Type_uHWord 	uhResetState;
	Type_uHWord 	uhPowerKeyState;
}PmStateMng_ts;

typedef struct{
	Type_uByte 			ubModuleID;
	exPmAsyncProc_fp	proc_fp;
}PmComSyncIf_ts;

typedef struct{
	Type_uByte		 	ubModuleID;
	Type_uWord			uwTime;
	exPmAsyncProc_fp	proc_fp;
}PmComAsyncIf_ts;

typedef struct{
	Type_uByte 			ubModuleID;
	Type_uWord			uwOnTime;
	Type_uWord			uwOffTime;
	exPmAsyncProc_fp	onProc_fp;
	exPmAsyncProc_fp	offProc_fp;
}PmPowerKeyIf_ts;

typedef struct{
	Type_uByte 			ubModuleID;
	Type_uWord			uwStandbyOutTime;
	Type_uWord			uwStandbyInTime;
	Type_uWord			uwCallbackTime;
	exPmAsyncProc_fp	standbyOutProc_fp;
	exPmAsyncProc_fp	standbyInProc_fp;
	exPmAsyncProc_fp	forceStandbyProc_fp;
	exPmAsyncProc_fp	callbackProc_fp;
}PmStandbyIf_ts;

typedef struct{
	Type_uByte 			ubModuleID;
	OS_EventCode_t		uhEvent;
	Type_uHWord			uhSocMask;
	Type_uHWord			uhStandbyMask;
	Type_uHWord			uhResetMask;
	Type_uHWord			uhPowerKeyMask;
	Type_uHWord			uhNextSocState;
	Type_uHWord			uhNextStandbyState;
	Type_uHWord			uhNextResetState;
	Type_uHWord			uhNextPowerKeyState;
	exPmCtrl_fp			pmCtrl_fp;
}PmCtrlProc_ts;

typedef struct{
	Type_uWord			uwType;
	Type_uWord			rstSts0;
	Type_uWord			rstSts1;
	Type_uWord			rstSts2;
}PmStartInfo_ts;

/********************************************************************************************/
/*							Enumeration Type Definition Section								*/
/********************************************************************************************/


/********************************************************************************************/
/*							Structure/Union Type Definition Section							*/
/********************************************************************************************/


/********************************************************************************************/
enum {
	EV_INF_PM_POWERON_ING = ((ARPC_ID_R5_POWERMNG << 8)  + 0x50),
	EV_INF_PM_POWEROFF_ING,
	EV_INF_PM_POWERON_OVER,
	EV_INF_PM_POWEROFF_OVER,
	EV_INF_PM_UPDATE_RESET_ING,
	EV_INF_PM_UPDATE_RESET_OVER,
	EV_INF_PM_FACTORY_RESET_ING,
	EV_INF_PM_FACTORY_RESET_OVER,
	EV_INF_PM_PRERESET_WDT_ING,
	EV_INF_PM_PRERESET_WDT_OVER,
	
	EV_INF_PM_STANDBYIN_START,
	EV_INF_PM_STANDBYIN_ING,
	EV_INF_PM_STANDBYIN_OVER,
	EV_INF_PM_STANDBYOUT_START,
	EV_INF_PM_STANDBYOUT_ING_MCU,
	EV_INF_PM_STANDBYOUT_ING_SOC,
	EV_INF_PM_STANDBYOUT_OVER,

	EV_INF_PM_SOCINIT_GTIMEROUT,
	EV_INF_PM_SOCSTANDBYOUT_GTIMEROUT,

	EV_INF_PM_A53HEAT_REBOOT_REQ,
	EV_INF_PM_A7HEART_REBOOT_REQ,
	EV_INF_PM_A53HEART_TIMOUT_REBOOT,
	EV_INF_PM_A7HEART_TIMOUT_REBOOT,
	EV_INF_PM_A53WDT_REBOOT_REQ,
	EV_INF_PM_A7WDT_REBOOT_REQ,
};
/********************************************************************************************/
/*							Extern Declaration												*/
/********************************************************************************************/




/********************************************************************************************/
/*							Global Function Prototype Declaration							*/
/********************************************************************************************/
extern void TSK_POWER(void);
extern void wvdPmModuleColdInit(void);
extern void wvdPmModuleInit(void);

extern void wvdPmBattOff(Type_uByte aubData1, Type_uByte aubData2);

extern void wvdPmSocInitGTimout(void);
extern void wvdPmStandbyOutGTimout(void);
extern void wvdPmStandbyInGTimout(void);
#if 0
extern void wvdPmUpdateResetGTimout(void);
extern void wvdPmFactoryResetGTimout(void);
#endif
extern void wvdPmPowerKeyOnGTimout(void);
extern void wvdPmPowerKeyOffGTimout(void);
#ifdef PM_HAL_BEFORE_OS
extern void wvdPmHalOpen(void);
#else
extern void wvdPmBattIrqSet(void);
#endif
extern HAL_HANDLE wvdPmHalHandleGet(void);
extern void wvdPmBattSysDown(void);
extern Type_uWord wubPmGetStartType(void);
#ifdef PM_ST_EN
extern void wvdPmStNoSet(Type_sWord stNo);
extern Type_uWord wvdPmStNoGet(void);
#endif

#endif /* PM.H */
/* End of File */
