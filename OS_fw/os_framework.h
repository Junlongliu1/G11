/********************************************************************************************/
/* File Name	: os_framework.h														*/
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

#ifndef OS_FRAMEWORK_H
#define OS_FRAMEWORK_H

/********************************************************************************************/
/*							Include File Section											*/
/********************************************************************************************/
#include "types.h"
#include "debug.h"
#include <osal.h>
#include "apn_basic_type.h"
#include "generic.h"
#include "os_framework_type.h"
#include "os_framework_cfg.h"
/********************************************************************************************/
/*							Macro Definition Section										*/
/********************************************************************************************/


/********************************************************************************************/
/*							Type Definition Section											*/
/********************************************************************************************/

/********************************************************************************************/
/*							Structure/Union Type Definition Section							*/
/********************************************************************************************/

/********************************************************************************************/
/*							Extern Declaration												*/
/********************************************************************************************/

/********************************************************************************************/
/*							Global Function Prototype Declaration							*/
/********************************************************************************************/

OS_ErrCode_t OS_CreateTask(OS_TskId_t				*auwTskId_p,
							OS_FuncPtr_t				avdTskFunc_p,
						 OS_CVoidPtr_t				avdTskName_p,
						 OS_ObjPri_t				auwTskPriority,
						 OS_MemSize_t				auwTskStackDepth,
						 OS_MemSize_t * const		auwTskStack_p);

OS_ErrCode_t OS_GetTaskId(OS_TskId_t *auwCurrentTskId_p);
void OS_SleepTask(OS_TimeTick_t auwTimeout);

OS_ErrCode_t OS_CreateQueue(OS_QueueId_t	*auwQueueId_p, OS_MemSize_t	auwQueueLength,	OS_CVoidPtr_t	avdQueueName_p);
OS_ErrCode_t OS_SendQueue(OS_QueueId_t	auwQueueId,  OS_ObjPri_t auwQueuePriority, OS_StdQueue_ts	 *astStdQueue_p);
OS_ErrCode_t OS_ReceiveQueue(OS_QueueId_t auwQueueId, OS_TimeTick_t	auwTimeout, OS_StdQueue_ts **astStdQueue_pp);
OS_ErrCode_t OS_CreateSemaphore(OS_ObjId_t aubSemId,OS_SemCnt_t	auwSemMaxCnt, OS_SemCnt_t	auwSemInitialCnt);
OS_ErrCode_t OS_WaitSemaphore(OS_ObjId_t	aubSemId, OS_TimeTick_t	auwTimeout);
OS_ErrCode_t OS_SignalSemaphore(OS_ObjId_t	aubSemId);
OS_ErrCode_t OS_CreateEventFlag(OS_ObjId_t aubFlgId);
OS_ErrCode_t OS_SetEventFlag(OS_ObjId_t aubFlgId, OS_FlgPtn_t auwFlgPtn);
OS_ErrCode_t OS_ClearEventFlag(OS_ObjId_t aubFlgId, OS_FlgPtn_t	auwFlgPtn);
OS_ErrCode_t OS_WaitEventFlag(OS_ObjId_t		aubFlgId,
							  OS_FlgPtn_t		auwWaitFlgPtn,
							  OS_SvcMode_t		auwSvcCallMode,
							  OS_FlgPtn_t		*auwClrFlgPtn_p,
							  OS_TimeTick_t		auwTimeout);
OS_ErrCode_t OS_CreateTimer(OS_ObjId_t 		aubTmrId,
						  OS_FuncPtr_t		avdTmrFunc_p,
						  OS_CVoidPtr_t		avdTmrName_p,
						  OS_TimeTick_t		auwTmrPeriod,
						  OS_ReloadMode_t 	aubReloadMode);
OS_ErrCode_t OS_StartTimer(OS_ObjId_t aubTmrId);
OS_ErrCode_t OS_TimerChangePeriod(OS_ObjId_t aubTmrId,OS_TimeTick_t auwTmrNewPeriod);
OS_ErrCode_t OS_StopTimer(OS_ObjId_t aubTmrId);
OS_ErrCode_t OS_ResetTimer(OS_ObjId_t aubTmrId);
OS_ErrCode_t OS_GetFMemoryBlock(OS_VoidPtr_t	*avdMemBlk_pp, OS_MemSize_t	auwMemBlkSize);
OS_ErrCode_t OS_ReleaseFMemoryBlock(OS_VoidPtr_t avdMemBlk_p);
void  OS_WaitTime(Type_uWord auwMaxWaitTime);
OS_ErrCode_t OS_MessagesWaitingQueue(OS_QueueId_t auwQueueId,Type_uWord *auwqueue_p);

#endif /* OS_FRAMEWORK_H */
/* End of File */
