/********************************************************************************************/
/* File Name	: os_framework_type.h														*/
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

#ifndef OS_FRAMEWORK_TYPE_H
#define OS_FRAMEWORK_TYPE_H

/********************************************************************************************/
/*							Include File Section											*/
/********************************************************************************************/
#include "apn_basic_type.h"

#include "FreeRTOS.h"

/********************************************************************************************/
/*							Macro Definition Section										*/
/********************************************************************************************/

/*==========================================================================================*/
/*	Common Macros																			*/
/*==========================================================================================*/
#define OS_NULL						(0)						/*!< Null Value					*/

#define OS_TRUE						(1)						/*!< Boolean True				*/
#define OS_FALSE					(0)						/*!< Boolean False				*/

#define OS_TOUT_INSTANT				(0U)					/*!< Timeout Immediately		*/
#define OS_TOUT_FOREVER				portMAX_DELAY			/*!< Timeout Forever			*/

#define OS_INVALID_ID				(0xFFU)					/*!< Invalid Object ID			*/

#define OS_RETURN_INTERRUPT			(0xFFU)					/*!< return interrupt handler	*/
/*==========================================================================================*/
/*	OS Error Codes																			*/
/*==========================================================================================*/
#define OS_ERR_OK					(0)						/*!< No Error					*/
/* OSAL Err ID*/
#define OS_ERR_INIT				(-1)		// initialization error
#define OS_ERR_NO_SPACE			(-2)		// need more space(memory, channel.etc.)
#define OS_ERR_INVALID_PARAMETER	(-3)		// passed invalid parameter
#define OS_ERR_INIT_TASK			(-4)		// error occurred in task api
#define OS_ERR_DEL				(-5)		// error occurred deleting loop
#define OS_ERR_QUEUE				(-6)		// error occurred in queue api
#define OS_ERR_SEMA				(-7)		// error occurred in semaphore api
#define OS_ERR_EVENT				(-8)		// error occurred in event api
#define OS_ERR_NOT_SUPPORT		(-9)		// not supported
#define OS_ERR_TIMEOUT			(-10)	// timed out error
#define OS_ERR_INVALID_HANDLE		(-11)	// invalid handle
#define OS_ERR_NO_DATA			(-12)	// no data
#define OS_ERR_UNDEF_STATE		(-13)	// not defined state
#define OS_ERR_NG					(-14)					/*!< General Error				*/
#define OS_ERR_ID					(-15)					/*!< Invalid ID					*/
#define OS_ERR_PAR					(-16)					/*!< Parameter Error			*/
#define OS_ERR_NOMEM				(-17)					/*!< No Memory					*/
#define OS_ERR_TMOUT				(-18)					/*!< Timeout					*/

/*==========================================================================================*/
/*	EventFlag Macros																		*/
/*==========================================================================================*/
#if configUSE_16_BIT_TICKS == 1
	#define OS_FLAG_MASK			(0x00FFU)				/*!< EventFlag 16 Bit Mask		*/
#else
	#define OS_FLAG_MASK			(0x00FFFFFFUL)			/*!< EventFlag 32 Bit Mask		*/
#endif
/*==========================================================================================*/
/*	Memory Macros																			*/
/*==========================================================================================*/
#define OS_MEM_SAFE_MARGIN			(64U)					/*!< Memory Safe Margin			*/

/********************************************************************************************/
/*							Type Definition Section											*/
/********************************************************************************************/
/*==========================================================================================*/
/*	Common Typedefs																			*/
/*==========================================================================================*/
typedef Type_sByte					OS_s8Bit_t;				/*!< Signed 8 Bit Integer		*/
typedef Type_sHWord					OS_s16Bit_t;			/*!< Signed 16 Bit Integer		*/
typedef Type_sWord					OS_sInt_t;				/*!< Signed Integer				*/
typedef Type_sWord					OS_s32Bit_t;			/*!< Signed 32 Bit Integer		*/
typedef Type_uByte					OS_u8Bit_t;				/*!< Unsigned 8 Bit Integer		*/
typedef Type_uHWord					OS_u16Bit_t;			/*!< Unsigned 16 Bit Integer	*/
typedef Type_uWord					OS_uInt_t;				/*!< Unsigned Integer			*/
typedef Type_uWord					OS_u32Bit_t;			/*!< Unsigned 32 Bit Integer	*/

typedef void *						OS_VoidPtr_t;			/*!< Void Pointer				*/
typedef const void *				OS_CVoidPtr_t;			/*!< Const Void Pointer			*/
typedef void						(*OS_FuncPtr_t)(void);	/*!< Function Pointer			*/

typedef OS_u8Bit_t					OS_Bool_t;				/*!< Boolean Integer			*/
typedef OS_u32Bit_t					OS_ObjPri_t;			/*!< Object Priority			*/
typedef OS_u8Bit_t					OS_ObjId_t;				/*!< Object ID					*/
typedef OS_u32Bit_t				OS_TskId_t;				/*!< TaskID					*/
typedef OS_u32Bit_t				OS_QueueId_t;				/*!< QueueID					*/
/* start 	OS_ErrCode_t	OS_u32Bit_t  ->	OS_s32Bit_t	 xulei 2021/11/5*/
typedef  OS_s32Bit_t				OS_ErrCode_t;			/*!< OS Error Code  OS_u32Bit_t			*/
/* end	*/
typedef Type_uWord     				OS_Timeout_t;
/*==========================================================================================*/
/*! \ingroup Task					Task Related Typedefs									*/
/*==========================================================================================*/
typedef StaticTask_t				OS_TskStData_t;			/*!< Task Static Data			*/

/*==========================================================================================*/
/*! \ingroup Mailbox				Mailbox Related Typedefs								*/
/*==========================================================================================*/
typedef OS_u16Bit_t					OS_EventCode_t;			/*!< Event Code					*/
typedef OS_u32Bit_t					OS_QueueSize_t;			/*!< Mail Size					*/
typedef struct OS_StdQueue			OS_StdQueue_ts;			/*!< Standard Mail Structure	*/
typedef StaticQueue_t				OS_QueueStData_t;			/*!< Mailbox Static Data		*/

/*==========================================================================================*/
/*! \ingroup EventFlag				Event Flag Related Typedefs								*/
/*==========================================================================================*/
typedef OS_u32Bit_t					OS_FlgPtn_t;			/*!< Flag Pattern				*/
typedef OS_u32Bit_t					OS_SvcMode_t;			/*!< Service Call Mode			*/
typedef StaticEventGroup_t			OS_FlgStData_t;			/*!< Event Flag Static Data		*/

/*==========================================================================================*/
/*! \ingroup Semaphore				Semaphore Related Typedefs								*/
/*==========================================================================================*/
typedef OS_u32Bit_t					OS_SemCnt_t;			/*!< Semaphore Count			*/
typedef StaticSemaphore_t			OS_SemStData_t;			/*!< Semaphore Static Data		*/

/*==========================================================================================*/
/*! \ingroup Memory					Memory Related Typedefs									*/
/*==========================================================================================*/
typedef OS_u32Bit_t					OS_MemSize_t;			/*!< Memory Size				*/

/*==========================================================================================*/
/*! \ingroup Time					Time Related Typedefs									*/
/*==========================================================================================*/
typedef OS_u32Bit_t					OS_TimeTick_t;			/*!< Time Tick					*/
typedef StaticTimer_t				OS_TmrStData_t;			/*!< Timer Static Data			*/
typedef OS_u8Bit_t					OS_ReloadMode_t;

/********************************************************************************************/
/*							Enumeration Type Definition Section								*/
/********************************************************************************************/
/*==========================================================================================*/
/*! \ingroup Mailbox				Mailbox Related Enumeration Types						*/
/*==========================================================================================*/

enum OS_MailPriority {
	OS_MAIL_PRI_RESERVED =			0,						/*!< System Reserved			*/
	OS_MAIL_PRI_FRONT				,						/*!< HIGH						*/
	OS_MAIL_PRI_NORMAL										/*!< Normal						*/
};
/********************************************************************************************/
/*							Structure/Union Type Definition Section							*/
/********************************************************************************************/
/*==========================================================================================*/
/*! \ingroup Mailbox				Mailbox Related Structure/Union Types					*/
/*==========================================================================================*/
struct OS_StdQueue {
	OS_ObjId_t		swSrcTaskId;							/*!< Source Task ID				*/
	OS_EventCode_t	uhEventCode;							/*!< Event Code					*/
	OS_VoidPtr_t	vdQueueBuffer_p;							/*!< Mail Data Pointer			*/
	OS_QueueSize_t	uwQueueSize;								/*!< Mail Size					*/
};
typedef struct {
	OS_TskId_t				*uwTskId_p;
	OS_FuncPtr_t			vdTskFunc_p;
	OS_CVoidPtr_t			vdTskName_p;
	OS_ObjPri_t				uwTskPriority;
	OS_MemSize_t			uwTskStackDepth;
	OS_MemSize_t * const	uwTskStack_p;
} OS_TskCfgTbl_ts;

typedef struct {
	OS_ObjId_t				ubSemId;
	OS_SemCnt_t				uwSemMaxCnt;
	OS_SemCnt_t				uwSemInitialCnt;
} OS_SemCfgTbl_ts;

typedef struct {
	OS_ObjId_t				ubFlgId;
} OS_FlgCfgTbl_ts;

typedef struct {
	OS_QueueId_t			*uwQueueId_p;
	OS_CVoidPtr_t			vdQueueName_p;
	OS_MemSize_t			uwQueueLength;
} OS_QueueCfgTbl_ts;

/********************************************************************************************/
/*							Extern Declaration												*/
/********************************************************************************************/

/********************************************************************************************/
/*							Global Function Prototype Declaration							*/
/********************************************************************************************/

#endif /* OS_FRAMEWORK_TYPE_H */
/* End of File */

