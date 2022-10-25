/********************************************************************************************/
/* File Name	: os_framework_cfg.h														*/
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

#ifndef OS_FRAMEWORK_CFG_H
#define OS_FRAMEWORK_CFG_H

/********************************************************************************************/
/*							Include File Section											*/
/********************************************************************************************/
#include "types.h"
#include <osal.h>
#include "apn_basic_type.h"
#include "os_framework_type.h"
#include "os_timer_cfg.h"
#include "app_cfg.h"

/********************************************************************************************/
/*							Macro Definition Section										*/
/********************************************************************************************/

#define OS_MAX_TSK								OSAL_MAX_TASK				/*!< Max Task Number			*/
#define OS_MAX_SEM								OSAL_MAX_SEMA				/*!< Max Semaphore Number		*/
#define OS_MAX_FLG								OSAL_MAX_EVENT				/*!< Max EventFlag Number		*/
#define OS_MAX_MBX								OSAL_MAX_QUEUE				/*!< Max Mailbox Number			*/
#define OS_MAX_TMR								50							/*!< Max Timer Number			*/


//#define OS_DEBUG
#ifdef OS_DEBUG
#define OS_DBG(fmt, args...)   printf(fmt, ## args)
#else
#define OS_DBG(fmt, args...)   do {} while(0)
#endif


/********************************************************************************************/
/*							Type Definition Section											*/
/********************************************************************************************/

/********************************************************************************************/
/*							Structure/Union Type Definition Section							*/
/********************************************************************************************/

/********************************************************************************************/
/*							Extern Declaration												*/
/********************************************************************************************/

extern const OS_TskCfgTbl_ts OS_cfg_tsk[];
/* Task ID */

/* Semphore ID */
typedef enum
{
	SEM_ID_MAIN_RECIVER_MBOX = 0,
	SEM_ID_MAIN_RECIVER_READ,
	SEM_ID_MAIN_RECIVER_WRITE,
	SEM_ID_MAIN_RECIVER_RBUF,
	SEM_ID_MAIN_RECIVER_WBUF,
	SEM_ID_MAIN_RECIVER_COMMON,
	SEM_ID_SUB_RECIVER_MBOX,
	SEM_ID_SUB_RECIVER_READ,
	SEM_ID_SUB_RECIVER_WRITE,
	SEM_ID_SUB_RECIVER_RBUF,
	SEM_ID_SUB_RECIVER_WBUF,
	SEM_ID_SUB_RECIVER_COMMON,
	SEM_ID_MAX
}OS_SEM_ID_Type;

/* Mailbox ID */
extern OS_QueueId_t MBX_ID_POWER;
extern OS_QueueId_t MBX_ID_POWERDRV;
extern OS_QueueId_t MBX_ID_POWERSOFTWDT;
extern OS_QueueId_t MBX_ID_AGENT;
extern OS_QueueId_t MBX_ID_EEP;

extern OS_QueueId_t MBX_ID_KLINEDRV; /* LXQ ADD START */
extern OS_QueueId_t MBX_ID_LETAPP;
extern OS_QueueId_t MBX_ID_LETAPP_SUBA;
extern OS_QueueId_t MBX_ID_LETAPP_SUBB;
extern OS_QueueId_t MBX_ID_LINETOOL; /* LXQ ADD END */
extern OS_QueueId_t MBX_ID_LED;
extern OS_QueueId_t MBX_ID_KEY;
extern OS_QueueId_t MBX_ID_SNR;
extern OS_QueueId_t MBX_ID_LOGAPP;
extern OS_QueueId_t MBX_ID_RPC; 
extern OS_QueueId_t MBX_ID_MAIN_RECIVER;
extern OS_QueueId_t MBX_ID_SUB_RECIVER;
#if (SERVICE_CONSOLE_EN ==1)
extern OS_QueueId_t MBX_ID_TEST;
#endif

typedef enum
{
	FLG_ID_KLINEDRV = 0,
	FLG_ID_LINETOOLDRV,
	FLG_ID_MAIN_RECIVER_ACK,
	FLG_ID_SUB_RECIVER_ACK,
	#if (SERVICE_TEST_EN == 1u)
	FLG_ID_TEST,
	#endif
	FLG_ID_MAX
}OS_FLG_ID_Type;

/********************************************************************************************/
/*							Global Function Prototype Declaration							*/
/********************************************************************************************/
extern OS_ErrCode_t OS_Register(void);
extern OS_ErrCode_t OS_fw_cfg_Init_Timer(OS_u8Bit_t timerID,const OS_TmrCfgTbl_ts *ptmrtable);

#endif /* OS_FRAMEWORK_CFG_H */
/* End of File */
