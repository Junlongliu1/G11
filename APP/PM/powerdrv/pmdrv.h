/********************************************************************************************/
/** <!-- Copyright (C) Alpine Electronics, Inc. All rights reserved. ------------------------>
 *
 *	\file			pmdrv.h
 *	\date			2021/11/12
 *	\author			wangying
 *	\model			G11
 *	\description	power driver
 *	\version

 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/

#ifndef PMDRV_H
#define PMDRV_H

/********************************************************************************************/
/* 						 Include File Section												*/
/********************************************************************************************/
#include "apn_basic_type.h"
#include "os_framework_type.h"
#include "os_framework.h"
#include "generic.h"
#include "temp_compile_switch.h"


/********************************************************************************************/
/*							Macro Definition Section										*/
/********************************************************************************************/
//#define PMDRV_DEBUG
#ifdef PMDRV_DEBUG
#define PMDRV_DBG(fmt, args...)   printf(fmt, ## args)
#else
#define PMDRV_DBG(fmt, args...)   do {} while(0)
#endif


/********************************************************************************************/
/*							Type Definition Section											*/
/********************************************************************************************/
typedef void (*exPmDrvPinProc_fp)(void);
typedef void (*exPmDrvProcCtrl_fp)(OS_StdQueue_ts *astMessage_p);

typedef struct{
	Type_uByte	ubPort;
	Type_uByte	ubCount;
	Type_uByte	ubRawLast;
	Type_uByte	ubRawCurrent;
	Type_uByte	ubFilteredLast;
	Type_uByte	ubFilteredCurrent;
}PmDrvPortInfo_ts;


typedef struct{
	Type_uByte			ubAdetStatus;
	Type_uByte 			ubHdetStatus;
	OS_EventCode_t		uhEvent;
}PmDrvPowerEvtCtrl_ts;

typedef struct{
	Type_uByte 			ubModuleID;
	OS_EventCode_t		uhEvent;
	exPmDrvProcCtrl_fp	proc_fp;
}PmDrvProcCtrl_ts;

/********************************************************************************************/
/*							Enumeration Type Definition Section								*/
/********************************************************************************************/
enum{
	PM_DRV_PORT_ADET_INDEX = (0U),
	PM_DRV_PORT_HDET_INDEX,
	PM_DRV_PORT_NUM
};
/********************************************************************************************/
/*							Structure/Union Type Definition Section							*/
/********************************************************************************************/


/********************************************************************************************/
/*							Extern Declaration												*/
/********************************************************************************************/


/********************************************************************************************/
/*							Global Function Prototype Declaration							*/
/********************************************************************************************/
extern void TSK_POWERDRV(void);
extern Type_uByte wubPmDrvStandbyOut(void);
extern Type_uByte wubPmDrvStandbyIn(void);
extern void wvdPmDrvRequestCurrentEvent(void);
extern Type_uByte wubPmDrvForceStandby(void);
extern void wvdPmDrvCycCtrl(void);
extern Type_uByte wubPmDrvBattStatusChk(void);
extern Type_sWord wswPmDrvBattCheckWait(Type_sWord dummy, void* pDummy);
extern OS_EventCode_t wubPmDrvGetStandbyDetSts(void);
void wvdPmDrvDetOnStage0(void);
#if (TEMP_COMPILE_SWITCH_05WORK_10WORK == TEMP_COMPILE_SWITCH_05WORK)
extern void wvdPmDrvDetOffStage0(void);
#else
extern void wvdPmDrvDetOnStage1(void);
extern void wvdPmDrvDetOnStage2(void);
extern void wvdPmDrvDetOnStage3(void);
extern void wvdPmDrvDetOnStage4(void);
extern void wvdPmDrvDetOnStage5(void);
extern void wvdPmDrvDetOffStage0(void);
extern void wvdPmDrvDetOffStage1(void);
extern void wvdPmDrvDetOffStage2(void);
extern void wvdPmDrvDetOffStage3(void);
extern void wvdPmDrvDetOffStage4(void);
#endif

#endif /* PMDRV.H */
/* End of File */


