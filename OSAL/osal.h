/****************************************************************************
 *   FileName    : osal.h
 *   Description : OS Abstraction Layer API
 ****************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips Inc.
 *   All rights reserved

This source code contains confidential information of Telechips.
Any unauthorized use without a written permission of Telechips including not limited to re-
distribution in source or binary form is strictly prohibited.
This source code is provided "AS IS" and nothing contained in this source code shall
constitute any express or implied warranty of any kind, including without limitation, any warranty
of merchantability, fitness for a particular purpose or non-infringement of any patent, copyright
or other third party intellectual property right. No warranty is made, express or implied,
regarding the information's accuracy, completeness, or performance.
In no event shall Telechips be liable for any claim, damages or other liability arising from, out of
or in connection with this source code or the use in the source code.
This source code is provided subject to the terms of a Mutual Non-Disclosure Agreement
between Telechips and Company.
*
****************************************************************************/
#ifndef  _OSAL_H_
#define  _OSAL_H_

#define OSAL_MEM_FUNC_IMPL // In case of using os memory library ex OS_MemCopy(), OS_MemSet()..etc
#define OSAL_STRING_FUN_IMPL //In case of  using os string library. XXX_StrCopy(), XXX_Strlen()...etc

#ifdef OS_NONE
#include <osal_none_impl.h>
#endif
#ifdef OS_UCOS
#include <osal_ucos_impl.h>
#endif
#ifdef OS_FREERTOS
#include <osal_freertos_impl.h>
#endif

#define	OSAL_BIT_31								0x80000000ul
#define	OSAL_BIT_30								0x40000000ul
#define	OSAL_BIT_29								0x20000000ul
#define	OSAL_BIT_28								0x10000000ul
#define	OSAL_BIT_27								0x08000000ul
#define	OSAL_BIT_26								0x04000000ul
#define	OSAL_BIT_25								0x02000000ul
#define	OSAL_BIT_24								0x01000000ul
#define	OSAL_BIT_23								0x00800000ul
#define	OSAL_BIT_22								0x00400000ul
#define	OSAL_BIT_21								0x00200000ul
#define	OSAL_BIT_20								0x00100000ul
#define	OSAL_BIT_19								0x00080000ul
#define	OSAL_BIT_18								0x00040000ul
#define	OSAL_BIT_17								0x00020000ul
#define	OSAL_BIT_16								0x00010000ul
#define	OSAL_BIT_15								0x00008000ul
#define	OSAL_BIT_14								0x00004000ul
#define	OSAL_BIT_13								0x00002000ul
#define	OSAL_BIT_12								0x00001000ul
#define	OSAL_BIT_11								0x00000800ul
#define	OSAL_BIT_10								0x00000400ul
#define	OSAL_BIT_09								0x00000200ul
#define	OSAL_BIT_08								0x00000100ul
#define	OSAL_BIT_07								0x00000080ul
#define	OSAL_BIT_06								0x00000040ul
#define	OSAL_BIT_05								0x00000020ul
#define	OSAL_BIT_04								0x00000010ul
#define	OSAL_BIT_03								0x00000008ul
#define	OSAL_BIT_02								0x00000004ul
#define	OSAL_BIT_01								0x00000002ul
#define	OSAL_BIT_00								0x00000001ul

#ifdef OS_UCOS
#define APP_CFG_TASK_START_PRIO           		2  //APP TASK START task
#define POWER_MANAGER_APP_TASK_PRIO	3  //Power Manager APP task
#define IPC_CONTROL_TASK_PRIO				3  //IPC Control task

#define SYSTEM_MON_APP_TASK_PRIO			4  //System Monitoring APP task
#define VEHICLE_SIGNAL_DEMO_APP_PRIORITY  6  //Vehicle Signal DEMO APP task
#define VSDR_MONITOR_TASK_PRIORITY  		(VEHICLE_SIGNAL_DEMO_APP_PRIORITY -1)  //VSDR Receive Monitor Task
#define IPC_PARSER_TASK_PRIO				6  //IPC Parser task
#define KEY_APP_TASK_PRIO					6  //KEY APP task
#define UART_TEST_TASK_PRIO				6  //Uart Test APP task
#define CONSOLE_DEMO_APP_PRIORITY		6  //Console DEMO APP task
#define NVM_MAMAGER_APP_TASK_PRIO		8  //NVM Manager APP task
#define OSAL_TASK_LOWEST_PRIO 			(32-5) //for ucOS-III  APP_TASK_LOWEST_PRIO => (OS_CFG_PRIO_MAX-5u)
#else //FreeRTOS
#define FREERTOS_MAX_PRIORITY				16 //configMAX_PRIORITIES
#define APP_CFG_TASK_START_PRIO           		(FREERTOS_MAX_PRIORITY - 3)  //APP TASK START task
#define POWER_MANAGER_APP_TASK_PRIO	(FREERTOS_MAX_PRIORITY - 3)  //Power Manager APP task
#define IPC_CONTROL_TASK_PRIO				(FREERTOS_MAX_PRIORITY - 3)  //IPC Control task
#define SYSTEM_MON_APP_TASK_PRIO			(FREERTOS_MAX_PRIORITY - 4)  //System Monitoring APP task
#define VEHICLE_SIGNAL_DEMO_APP_PRIORITY  (FREERTOS_MAX_PRIORITY - 6)  //Vehicle Signal DEMO APP task
#define VSDR_MONITOR_TASK_PRIORITY  		(VEHICLE_SIGNAL_DEMO_APP_PRIORITY + 1)  //VSDR Receive Monitor Task
#define IPC_PARSER_TASK_PRIO				(FREERTOS_MAX_PRIORITY - 6)  //IPC Parser task
#define KEY_APP_TASK_PRIO					(FREERTOS_MAX_PRIORITY - 6)  //KEY APP task
#define UART_TEST_TASK_PRIO				(FREERTOS_MAX_PRIORITY - 6)  //Uart Test APP task
#define CONSOLE_DEMO_APP_PRIORITY		(FREERTOS_MAX_PRIORITY - 6)  //Console DEMO APP task
#define NVM_MAMAGER_APP_TASK_PRIO		(FREERTOS_MAX_PRIORITY - 8)  //NVM Manager APP task
#define OSAL_TASK_LOWEST_PRIO 			(FREERTOS_MAX_PRIORITY - 12)
#endif


#define OSAL_TIMEOUT_FOREVER		(0)

/*OSAL Error Define*/
#define OSAL_ERR_NONE				(0)		// ok
#define OSAL_ERR_INIT				(-1)		// initialization error
#define OSAL_ERR_NO_SPACE			(-2)		// need more space(memory, channel.etc.)
#define OSAL_ERR_INVALID_PARAMETER	(-3)		// passed invalid parameter
#define OSAL_ERR_INIT_TASK			(-4)		// error occurred in task api
#define OSAL_ERR_DEL				(-5)		// error occurred deleting loop
#define OSAL_ERR_QUEUE				(-6)		// error occurred in queue api
#define OSAL_ERR_SEMA				(-7)		// error occurred in semaphore api
#define OSAL_ERR_EVENT				(-8)		// error occurred in event api
#define OSAL_ERR_NOT_SUPPORT		(-9)		// not supported
#define OSAL_ERR_TIMEOUT			(-10)	// timed out error
#define OSAL_ERR_INVALID_HANDLE		(-11)	// invalid handle
#define OSAL_ERR_NO_DATA			(-12)	// no data
#define OSAL_ERR_UNDEF_STATE		(-13)	// not defined state

/* Configuration for the number of using os resources*/
#define OSAL_MAX_TASK	32
#define OSAL_MAX_QUEUE	32
#define OSAL_MAX_SEMA	32
#define OSAL_MAX_EVENT	16

#define  OSAL_EVENT_OPT_CLR_ALL		(0x00000001ul)  /* Wait for ALL    the bits specified to be CLR       */
#define  OSAL_EVENT_OPT_CLR_ANY		(0x00000002ul)  /* Wait for ANY of the bits specified to be CLR       */
#define  OSAL_EVENT_OPT_SET_ALL		(0x00000004ul)  /* Wait for ALL    the bits specified to be SET       */
#define  OSAL_EVENT_OPT_SET_ANY		(0x00000008ul)  /* Wait for ANY of the bits specified to be SET       */
#define  OSAL_EVENT_OPT_CONSUME		(0x00000100ul)  /* Consume the flags if condition(s) satisfied        */


#define  OSAL_EVENT_SET_OPT_FLAG_SET                (0x00000000ul)
#define  OSAL_EVENT_SET_OPT_FLAG_CLR                (0x00000001ul)
#define  OSAL_EVENT_OPT_NON_BLOCKING	(0x00008000ul)

#define  OSAL_API_OPT_BLOCKING			(0x00000000ul)
#define  OSAL_API_OPT_NON_BLOCKING		(0x00008000ul)

#define  OSAL_NO_TIMEOUT		0

extern const uint32 OSAL_TickRate; // os tick rate in hz. if it has 1000. then it means 1ms
extern uint32  OSAL_TickTimerInitialized;

typedef  void (*OSAL_TASK_PTR)(void *);

int32 OSAL_InitOS(void);
int32 OSAL_StartOS(void);
int32 OSAL_InitOSFuctions(void);
uint32 OSAL_GetOSTimeTick(void);

/* Task */
int32 OSAL_TaskCreate(uint32 *task_id, const uint8 *task_name,
							OSAL_TASK_PTR function_pointer, const uint32 *stack_pointer,
							uint32 stack_size, uint32 priority, void * ftask_param);
int32 OSAL_TaskDelete(uint32 task_id);
void OSAL_TaskDelay(uint32 millisecond);

/* Queue */
int32 OSAL_QueueCreate(uint32 *queue_id, const uint8 *queue_name, uint32 queue_depth, uint32 data_size, uint32 flags);
int32 OSAL_QueueDelete(uint32 queue_id);
int32 OSAL_QueueGet(uint32 queue_id, void *data, uint32 size, uint32 *size_copied, int32 timeout, uint32 options);
int32  OSAL_QueuePut(uint32 queue_id, void *data, uint32 size, uint32 flags, int32 timeout, uint32 options);

/* Semaphore */
int32 OSAL_SemaCreate(uint32 *sem_id, const uint8 *sem_name, uint32 sem_initial_value, uint32 options);
int32 OSAL_SemaDelete(uint32 sem_id );
int32 OSAL_SemaObtain(uint32 sem_id, int32 timeout, uint32 options);
int32 OSAL_SemaRelease(uint32 sem_id );

/* Event Flags */
int32 OSAL_EventCreate(uint32 *event_id, const uint8 *event_name, uint32 event_initial_value,  int32* native_err);
int32 OSAL_EventDelete(uint32 event_id,  int32* native_err);
int32 OSAL_SetEvent(uint32 event_id, uint32 event, uint32 options,  int32* native_err);
uint32 OSAL_GetEvent(uint32 event_id, uint32 event, uint32 timeout, uint32 options, int32 *err, int32* native_err);

/*Mem*/
void OSAL_MemSet( void *pmem, uint8 val, int32   size);
void OSAL_MemCopy(  void *pdest, const  void *psrc, uint32   size);
int32 OSAL_MemCmp(const  void * pmem1, const  void *pmem2, uint32 size);

/* String*/
int8 * OSAL_StrCopy(int8 *dest_str, const int8 *src_srt);
int16 OSAL_StrCmp(const int8 *p1, const int8 *p2);
int16 OSAL_StrNCmp(const int8 *p1, const int8 *p2, int32 len);
uint32 OSAL_StrLen(const int8 *pstr);
#endif

