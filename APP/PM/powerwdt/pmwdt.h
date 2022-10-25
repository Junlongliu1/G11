/********************************************************************************************/
/** <!-- Copyright (C) Alpine Electronics, Inc. All rights reserved. ------------------------>
 *
 *	\file			pmwdt.h
 *	\date			2021/11/12
 *	\author			wangying
 *	\model			G11
 *	\description	power watchDog
 *	\version

 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/

#ifndef PMWDT_H
#define PMWDT_H

/********************************************************************************************/
/* 						 Include File Section												*/
/********************************************************************************************/
#include "apn_basic_type.h"
#include "os_framework_type.h"
#include "os_framework.h"
#include "generic.h"
#include "pm.h"

/********************************************************************************************/
/*							Macro Definition Section										*/
/********************************************************************************************/
#define PMWDT_FLGPTN                (0x01U)
#define PMWDT_FLGPTN_WDTIRQ         (0x01U)

//#define PMWDT_DEBUG
#ifdef PMWDT_DEBUG
#define PMWDT_DBG(fmt, args...)   printf(fmt, ## args)
#else
#define PMWDT_DBG(fmt, args...)   do {} while(0)
#endif


/********************************************************************************************/
/*							Type Definition Section											*/
/********************************************************************************************/
typedef void (*exPmWdtProcCtrl_fp)(OS_StdQueue_ts *astMessage_p);


/********************************************************************************************/
/*							Enumeration Type Definition Section								*/
/********************************************************************************************/
enum {
	EV_INF_PMWDT_CYC_TIMOUT = ((ARPC_ID_R5_SOFTWDT << 8)  + 0x50),
};


/********************************************************************************************/
/*							Structure/Union Type Definition Section							*/
/********************************************************************************************/
typedef struct{
	Type_uByte		 	ubModuleID;
	exPmAsyncProc_fp	proc_fp;
}PmWdtPreResetIf_ts;

typedef struct{
	Type_uByte 			ubModuleID;
	OS_EventCode_t		uhEvent;
	exPmWdtProcCtrl_fp	proc_fp;
}PmWdtProcCtrl_ts;

/********************************************************************************************/
/*							Extern Declaration												*/
/********************************************************************************************/


/********************************************************************************************/
/*							Global Function Prototype Declaration							*/
/********************************************************************************************/
extern void wvdPmHardWdtSend(void);
extern void wvdOsWdtCycCtrl(void);


extern void TSK_POWERSOFTWDT(void);
extern Type_uByte wubPmSoftWdtStandbyOut(void);
extern Type_uByte wubPmSoftWdtStandbyIn(void);
extern Type_uByte wubPmSoftWdtForceStandby(void);
extern void wvdPmSoftWdtCycCtrl(void);

#endif /* PMDRV.H */
/* End of File */


