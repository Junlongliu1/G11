/********************************************************************************************/
/* File Name	: os_timer_cfg.h														*/
/* Date		: 2020/12/16																*/
/* Author		: plj																*/
/* Model		: EDA																	*/
/* Description	: OS Framework - Header File for type definition.							*/
/* Copyright (c)  2020 Alpine Electronics (China) Co. LTD.  All Rights Reserved				*/
/*------------------------------------------------------------------------------------------*/
/* Revision History																			*/
/* No		Date		Revised by		Function Name										*/
/* 																							*/
/*------------------------------------------------------------------------------------------*/
/********************************************************************************************/

#ifndef OS_TIMER_CFG_H
#define OS_TIMER_CFG_H

/********************************************************************************************/
/*							Include File Section											*/
/********************************************************************************************/
#include "types.h"
#include <osal.h>
#include "apn_basic_type.h"
#include "os_framework_type.h"
#include "temp_compile_switch.h"

/********************************************************************************************/
/*							Macro Definition Section										*/
/********************************************************************************************/

typedef struct
{
	OS_ObjId_t				ubTmrId;
	OS_FuncPtr_t			vdTmrFunc_p;
	OS_CVoidPtr_t			vdTmrName_p;
	OS_TimeTick_t			uwTmrPeriod;
	OS_u8Bit_t				ubAutoStart;		/* if auto start */
	OS_u8Bit_t				ubAutoReload; 
} OS_TmrCfgTbl_ts;

typedef enum
{
	OS_TIMER_ID_PMDRVCYC = 0,
	OS_TIMER_ID_PMWDTCYCHEART,
	OS_TIMER_ID_OSWDTCYCHEART,
	OS_TIMER_ID_PMSOCSTARTGT,
	OS_TIMER_ID_PMSTANDBYOUTGT,
	OS_TIMER_ID_PMSTANDBYINGT,
	OS_TIMER_ID_PMPOWERKEYONGT,
	OS_TIMER_ID_PMPOWERKEYOFFGT,
	OS_TIMER_ID_KLINEDRV,
#if (TEMP_COMPILE_SWITCH_05WORK_10WORK != TEMP_COMPILE_SWITCH_05WORK)
	OS_TIMER_ID_PMDRVPINSETUP1,
#endif
#if (SERVICE_TEST_EN == 1u)
	OS_TIMER_ID_TEST,
#endif
#if (TEMP_COMPILE_SWITCH_05WORK_10WORK != TEMP_COMPILE_SWITCH_05WORK)
	OS_TIMER_ID_PMDRVPINSETUP2,
	OS_TIMER_ID_PMDRVPINSETUP3,
	OS_TIMER_ID_PMDRVPINSETUP4,
	OS_TIMER_ID_PMDRVPINSETUP5,
	OS_TIMER_ID_PMDRVPINSETDN1,
	OS_TIMER_ID_PMDRVPINSETDN3,
	OS_TIMER_ID_PMDRVPINSETDN4,
#endif
	OS_TIMER_ID_MAX
}OS_TIMER_ID_Type;

OS_ErrCode_t OS_Timer_Initialize(void);

/********************************************************************************************/
/*							Global Function Prototype Declaration							*/
/*****************************************************************************************r***/

#endif /* OS_TIMER_CFG_H */
/* End of File */
