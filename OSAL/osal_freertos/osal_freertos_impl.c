/****************************************************************************
 *   FileName    : osal_freertos_impl.c
 *   Description : implementation layer of osal for freertos
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
#include  <types.h>
#include <osal.h>
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <event_groups.h>
#include <stdio.h> //QAC-<stdio.h> shall not be used in production code, but freertos recommends using <stdio.h>
#include <string.h>
#include "os_framework.h"
/*tasks */
typedef struct
{    
	int32 used;		/* if already used or not */    
	StaticTask_t Tcb;		/* pointer to Task Control Block */
}OSAL_TASK_TABLE_T;

/* queue */
typedef struct
{    
	int32 used;		/* if already used or not */
	StaticQueue_t queue;		/* pointer to Queue control block */
}OSAL_QUEUE_TABLE_T;

/* semaphore */
typedef struct
{    
	int32 used;		/* if already used or not */
	StaticSemaphore_t semaphore;		/* pointer to Semaphore control block */
}OSAL_SEMAPHORE_TABLE_T;

/* event */
typedef struct
{    
	int32 used;		/*if already used or not */
	StaticEventGroup_t event;		/* pointer to Event control block */
}OSAL_EVENT_TABLE_T;

typedef struct {
	OS_u8Bit_t		used;		/* if used or not */
	OS_TmrStData_t	stdata;		/* static data */
} OS_TmrTbl_ts;

static StaticSemaphore_t osal_obj_sem;

const uint32  OSAL_TickRate = (uint32) OS_CFG_TICK_RATE_HZ;
uint32  OSAL_TickTimerInitialized = 0;

/* OS object */
static OSAL_TASK_TABLE_T OSAL_Task_Obj[OSAL_MAX_TASK];
static OSAL_QUEUE_TABLE_T OSAL_Queue_Obj[OSAL_MAX_QUEUE];
static OSAL_SEMAPHORE_TABLE_T OSAL_Sema_Obj[OSAL_MAX_SEMA];
static OSAL_EVENT_TABLE_T OSAL_Event_Obj[OSAL_MAX_EVENT];
static OS_TmrTbl_ts OS_TmrTable[OS_MAX_TMR];

int32 OSAL_InitOS(void)
{
	int32 i;
	int32 ret = OSAL_ERR_NONE;
	BaseType_t err;
	volatile uint32 intNestingCnt = 0;

	heap_init();

	/*OSAL default manage object*/
	(void) xSemaphoreCreateBinaryStatic(&osal_obj_sem); //CS : redundant condition

	/* semaphore must first be given using the xSemaphoreGive() API function before it can subsequently be taken (obtained) using the xSemaphoreTake() function. */
	intNestingCnt = xPortGetInterruptNestingCount();
	if(intNestingCnt == 0)
	{
		err = xSemaphoreGive((SemaphoreHandle_t)&osal_obj_sem);
	}
	else
	{
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		err = xSemaphoreGiveFromISR((SemaphoreHandle_t)&osal_obj_sem, &xHigherPriorityTaskWoken);
	}
	
	if(err == pdTRUE)
	{
		/* Init Object */
		for(i = 0; i < OSAL_MAX_TASK; i++)    
		{        
			//OSAL_MemSet(&OSAL_Task_Obj[i], 0x00, sizeof(OSAL_TASK_TABLE_T));
			//modify for time save
			OSAL_Task_Obj[i].used = 0;
		}
		for(i = 0; i < OSAL_MAX_QUEUE; i++)    
		{        
			//OSAL_MemSet(&OSAL_Queue_Obj[i], 0x00, sizeof(OSAL_QUEUE_TABLE_T));
			//modify for time save
			OSAL_Queue_Obj[i].used = 0;
		}
		for(i = 0; i < OSAL_MAX_SEMA; i++)    
		{        
			//OSAL_MemSet(&OSAL_Sema_Obj[i], 0x00, sizeof(OSAL_SEMAPHORE_TABLE_T));
			//modify for time save
			OSAL_Sema_Obj[i].used = 0;
		}
		for(i = 0; i < OSAL_MAX_EVENT; i++)    
		{        
			//OSAL_MemSet(&OSAL_Event_Obj[i], 0x00, sizeof(OSAL_EVENT_TABLE_T));
			//modify for time save
			OSAL_Event_Obj[i].used = 0;
		}
		
	}
	else
	{
		ret = OSAL_ERR_INIT;
	}
	
	return ret;
}

int32 OSAL_InitOSFuctions(void)
{
	int32 ret = OSAL_ERR_NONE;
	
	return ret;
}

int32 OSAL_StartOS(void)
{
	int32 ret = OSAL_ERR_NONE;
	
	vTaskStartScheduler();
	
	return ret;
}

uint32 OSAL_GetOSTimeTick(void)
{
	TickType_t ticks;

	ticks = xTaskGetTickCount();
	
	return ticks;
}

int32 OSAL_TaskCreate(uint32 *task_id, const uint8 *task_name,
							OSAL_TASK_PTR function_pointer, const uint32 *stack_pointer,
							uint32 stack_size, uint32 priority, void * ftask_param)
{
	BaseType_t err;
	int32 retval = OSAL_ERR_NONE;
	int32 i, search_id = -1;
	volatile uint32 intNestingCnt = 0;

	if ( (task_name == 0) || (function_pointer == 0) ||(stack_pointer ==0))
	{
		retval = OSAL_ERR_INVALID_PARAMETER;
	}
	else
	{

		/* obtain sema. wait forever*/
		intNestingCnt = xPortGetInterruptNestingCount();
		if(intNestingCnt == 0)
		{
			err = xSemaphoreTake((SemaphoreHandle_t)&osal_obj_sem, (TickType_t )portMAX_DELAY);
		}
		else
		{
			BaseType_t xHigherPriorityTaskWoken = pdFALSE;
			err = xSemaphoreTakeFromISR((SemaphoreHandle_t)&osal_obj_sem, &xHigherPriorityTaskWoken);
			portYIELD_FROM_ISR( xHigherPriorityTaskWoken )//;QAC
		}
		if(err == pdTRUE)
		{
			/*search free table*/
			for(i = 0; i < OSAL_MAX_TASK; i++)
			{
				if(OSAL_Task_Obj[i].used == 0)
				{
					search_id = i;
					break;
				}
			}
			if ( search_id >= 0)
			{
				TaskHandle_t task = 0;

				task = xTaskCreateStatic((TaskFunction_t)function_pointer,
															(const int8 * const)task_name, //warning
															stack_size,
															ftask_param,
															(UBaseType_t)( priority | portPRIVILEGE_BIT ),
															(StackType_t * const)stack_pointer, /* QAC-use const */
															(StaticTask_t * const)&OSAL_Task_Obj[search_id].Tcb); /* QAC-use const */
				if(task != 0)
				{
					OSAL_Task_Obj[search_id].used = 1;
					*task_id = search_id;
				}
				else
				{
					retval = OSAL_ERR_INIT_TASK;
				}
			}
			else
			{
				retval = OSAL_ERR_NO_SPACE;
			}

			intNestingCnt = xPortGetInterruptNestingCount();
			if(intNestingCnt == 0)
			{
				err = xSemaphoreGive((SemaphoreHandle_t)&osal_obj_sem);
			}
			else
			{
				BaseType_t xHigherPriorityTaskWoken = pdFALSE;
				err = xSemaphoreGiveFromISR((SemaphoreHandle_t)&osal_obj_sem, &xHigherPriorityTaskWoken);
				portYIELD_FROM_ISR( xHigherPriorityTaskWoken )//;QAC
			}
		}

	}

	return retval;
}

int32 OSAL_TaskDelete(uint32 task_id)
{
	int32 ret = OSAL_ERR_NOT_SUPPORT;
	
	return ret;
}

void OSAL_TaskDelay(uint32 millisecond)
{
	uint32 ticks = 0;
	
	ticks = (millisecond/((1000 + (OSAL_TickRate-1))/OSAL_TickRate));
	
	vTaskDelay(ticks);
}

int32 OSAL_QueueCreate(uint32 *queue_id, const uint8 *queue_name, uint32 queue_depth, uint32 data_size, uint32 flags)
{
	BaseType_t err;
	int32 retval = OSAL_ERR_NONE;
	int32 i, search_id = -1;// CS : Redundant Condition
	volatile uint32 intNestingCnt = 0;
	
	if ( (queue_name == 0) || (queue_depth < 1) )
	{
		retval = OSAL_ERR_INVALID_PARAMETER;
	}
	else
	{
		/* obtain sema. wait forever*/
		intNestingCnt = xPortGetInterruptNestingCount();
		if(intNestingCnt == 0)
		{
			err = xSemaphoreTake((SemaphoreHandle_t)&osal_obj_sem, (TickType_t )portMAX_DELAY);
		}
		else
		{
			BaseType_t xHigherPriorityTaskWoken = pdFALSE;
			err = xSemaphoreTakeFromISR((SemaphoreHandle_t)&osal_obj_sem, &xHigherPriorityTaskWoken);
		}
		if(err == pdTRUE)
		{
			/*search free table*/
			for(i = 0; i < OSAL_MAX_QUEUE; i++)
			{
				if(OSAL_Queue_Obj[i].used == 0)
				{
					search_id = i;
					break;
				}
			}

			if ( search_id >= 0)// CS : Redundant Condition 
			{
				void * pQueueBuffer;
				QueueHandle_t pQueueHandle = 0;
				
				pQueueBuffer = pvPortMalloc(queue_depth*data_size);

				if ( pQueueBuffer != 0 )
				{
					pQueueHandle = xQueueCreateStatic(queue_depth, 
								data_size, 
								pQueueBuffer, 
								(StaticQueue_t *)&OSAL_Queue_Obj[search_id].queue);
				}

				if(pQueueHandle != 0)
				{
					OSAL_Queue_Obj[search_id].used = 1;
					*queue_id = search_id;
				}
				else
				{
					retval = OSAL_ERR_QUEUE;
				}
			}
			else
			{
				retval = OSAL_ERR_NO_SPACE;
			}
			
			/*release sema*/
			intNestingCnt = xPortGetInterruptNestingCount();
			if(intNestingCnt == 0)
			{
				err = xSemaphoreGive((SemaphoreHandle_t)&osal_obj_sem);
			}
			else
			{
				BaseType_t xHigherPriorityTaskWoken = pdFALSE;
				err = xSemaphoreGiveFromISR((SemaphoreHandle_t)&osal_obj_sem, &xHigherPriorityTaskWoken);
			}
		}
		else
		{
			retval = OSAL_ERR_QUEUE;
		}
	}
	return retval;
}


int32 OSAL_QueueDelete( uint32 queue_id)
{
	int32 ret = OSAL_ERR_NOT_SUPPORT;

	return ret;
}

int32 OSAL_QueueGet( uint32 queue_id, void *data, uint32 size, uint32 *size_copied, int32 timeout, uint32 options)
{
	BaseType_t err;
	int32 ret = 0;
	volatile uint32 intNestingCnt = 0;
	uint32 timeDly; //CS : useless assignment
	
	if ((OSAL_Queue_Obj[queue_id].used == 1) && (data != 0) && (size_copied != 0))
	{
		if( (options & 0x00008000) == OSAL_API_OPT_NON_BLOCKING)  //CS : Redundant Condition
		{
			timeDly = 0; // no block time
		}
		else
		{
			if (timeout == OSAL_NO_TIMEOUT) {
				timeDly = portMAX_DELAY;
			} else {
				timeDly = timeout;
			}
		}

		intNestingCnt = xPortGetInterruptNestingCount();
		if(intNestingCnt == 0)
		{
			err = xQueueReceive((QueueHandle_t)&(OSAL_Queue_Obj[queue_id].queue),
						(void *)data,
						(TickType_t)timeDly);
		}
		else
		{
			BaseType_t xHigherPriorityTaskWoken = pdFALSE;
			err = xQueueReceiveFromISR((QueueHandle_t)&(OSAL_Queue_Obj[queue_id].queue),
						(void *)data,
						&xHigherPriorityTaskWoken);
			portYIELD_FROM_ISR( xHigherPriorityTaskWoken )//;QAC
		}

		if ((err == pdTRUE) && (data != 0)) 
		{
			*size_copied = (uint32)OSAL_Queue_Obj[queue_id].queue.uxDummy4[2];// referenced by uxItemSize on uxItemSize	;
			ret = OSAL_ERR_NONE;
		}
		else
		{
			ret = OSAL_ERR_QUEUE;
		}
	}
	else
	{
		ret = OSAL_ERR_INVALID_PARAMETER;
	}
	
	return ret;
}

int32  OSAL_QueuePut( uint32 queue_id, void *data, uint32 size, uint32 flags, int32 timeout, uint32 options)
{
	BaseType_t err;
	int32 ret;
	volatile uint32 intNestingCnt = 0;
	uint32 timeDly; //CS : useless assignment

	if ((OSAL_Queue_Obj[queue_id].used == 1) && (data != 0) && (size > 0))
	{
		uint32 uxItemSize;
		uint32 i, pushCnt;
		uint8 * queueDataPtr = (uint8 *)data;

		if( (options & 0x00008000) == OSAL_API_OPT_NON_BLOCKING)  //CS : Redundant Condition
		{
			timeDly = 0; // no block time
		}
		else
		{
			if (timeout == OSAL_NO_TIMEOUT) {
				timeDly = portMAX_DELAY;
			} else {
				timeDly = timeout;
			}
		}

		uxItemSize = (uint32)OSAL_Queue_Obj[queue_id].queue.uxDummy4[2];// referenced by uxItemSize on uxItemSize	;

		if(uxItemSize != 0) //CS : Uninitialized Variable
		{
			pushCnt = (size/uxItemSize);
			if(pushCnt != 0)
			{
				for (i =0; i < pushCnt; i++)
				{
					intNestingCnt = xPortGetInterruptNestingCount();
					if(intNestingCnt == 0)
					{
						err = xQueueSend((QueueHandle_t)&(OSAL_Queue_Obj[queue_id].queue),
								(void *)(queueDataPtr + (i*uxItemSize)),
								(TickType_t)timeDly);
					}
					else
					{
						BaseType_t xHigherPriorityTaskWoken = pdFALSE;
						err = xQueueSendFromISR((QueueHandle_t)&(OSAL_Queue_Obj[queue_id].queue),
								(void *)(queueDataPtr + (i*uxItemSize)), 
								&xHigherPriorityTaskWoken);
						portYIELD_FROM_ISR( xHigherPriorityTaskWoken )//;QAC
					}

					if(err != pdTRUE)
					{
						ret = OSAL_ERR_QUEUE;
						break;
					}
					else
					{
						ret = OSAL_ERR_NONE;
					}
				}
			}
			else
			{
				ret = OSAL_ERR_QUEUE;
			}
		}
		else
		{
			ret = OSAL_ERR_QUEUE;	
		}
	}
	else
	{
		ret = OSAL_ERR_INVALID_PARAMETER;
	}
	
	return ret;

}

int32 OSAL_SemaCreate(uint32 *sem_id, const uint8 *sem_name, uint32 sem_initial_value, uint32 options)
{
	BaseType_t err;
	int32 retval = OSAL_ERR_NONE;
	int32 i, search_id = -1;	// CS : Redundant Condition 
	volatile uint32 intNestingCnt = 0;
	
	if (sem_name == 0)
	{
		retval = OSAL_ERR_INVALID_PARAMETER;
	}
	else
	{
		/* obtain sema. wait forever*/
		intNestingCnt = xPortGetInterruptNestingCount();
		if(intNestingCnt == 0)
		{
			err = xSemaphoreTake((SemaphoreHandle_t)&osal_obj_sem, (TickType_t )portMAX_DELAY);
		}
		else
		{
			BaseType_t xHigherPriorityTaskWoken = pdFALSE;
			err = xSemaphoreTakeFromISR((SemaphoreHandle_t)&osal_obj_sem, &xHigherPriorityTaskWoken);
		}
		if(err == pdTRUE)
		{
			/*search free table*/
			for(i = 0; i < OSAL_MAX_SEMA; i++)
			{
				if(OSAL_Sema_Obj[i].used == 0)
				{
					search_id = i;
					break;
				}
			}

			if ( search_id >= 0)
			{
				QueueHandle_t pSemaQueueHandle = 0;

				pSemaQueueHandle = xSemaphoreCreateBinaryStatic((StaticSemaphore_t *)&(OSAL_Sema_Obj[search_id].semaphore));
				if(pSemaQueueHandle != 0)
				{
					OSAL_Sema_Obj[search_id].used = 1;
					*sem_id = search_id;
					
					intNestingCnt = xPortGetInterruptNestingCount();
					if(intNestingCnt == 0)
					{
						err = xSemaphoreGive((SemaphoreHandle_t)&(OSAL_Sema_Obj[search_id].semaphore));
					}
					else
					{
						BaseType_t xHigherPriorityTaskWoken = pdFALSE;
						err = xSemaphoreGiveFromISR((SemaphoreHandle_t)&(OSAL_Sema_Obj[search_id].semaphore), &xHigherPriorityTaskWoken);
					}
				}
				else
				{
					retval = OSAL_ERR_SEMA;
				}
			}
			else
			{
				retval = OSAL_ERR_NO_SPACE;
			}
			/*release sema*/
			intNestingCnt = xPortGetInterruptNestingCount();
			if(intNestingCnt == 0)
			{
				(void) xSemaphoreGive((SemaphoreHandle_t)&osal_obj_sem);
			}
			else
			{
				BaseType_t xHigherPriorityTaskWoken = pdFALSE;
				(void) xSemaphoreGiveFromISR((SemaphoreHandle_t)&osal_obj_sem, &xHigherPriorityTaskWoken);
			}
		}
		else
		{
			retval = OSAL_ERR_SEMA;
		}
	}
	return retval;
}

int32 OSAL_SemaDelete( uint32 sem_id)
{
	int32 ret = OSAL_ERR_NOT_SUPPORT;

	return ret;
}

int32 OSAL_SemaObtain( uint32 sem_id, int32 timeout, uint32 options)
{
	BaseType_t err;
	int32 ret;
	volatile uint32 intNestingCnt = 0;
	uint32 timeDly; // CS : Redundant Condition
	
	if (OSAL_Sema_Obj[sem_id].used == 1)
	{
		if( (options & 0x00008000) == OSAL_API_OPT_NON_BLOCKING)  //CS : Redundant Condition
		{
			timeDly = 0; // no block time
		}
		else
		{
			if (timeout == OSAL_NO_TIMEOUT) {
				timeDly = portMAX_DELAY;
			} else {
				timeDly = timeout;
			}
		}

		/* obtain sema. wait forever*/
		intNestingCnt = xPortGetInterruptNestingCount();
		if(intNestingCnt == 0)
		{
			err = xSemaphoreTake((SemaphoreHandle_t)&(OSAL_Sema_Obj[sem_id].semaphore), (TickType_t)timeDly);
		}
		else
		{
			BaseType_t xHigherPriorityTaskWoken = pdFALSE;
			err = xSemaphoreTakeFromISR((SemaphoreHandle_t)&(OSAL_Sema_Obj[sem_id].semaphore), &xHigherPriorityTaskWoken);
			portYIELD_FROM_ISR( xHigherPriorityTaskWoken )//; QAC
			
		}
		
		if (err == pdTRUE)
		{
			ret = OSAL_ERR_NONE;
		}
		else
		{
			ret = OSAL_ERR_SEMA;
		}
	}
	else
	{
		ret = OSAL_ERR_INVALID_PARAMETER;
	}
	
	return ret;
}

int32 OSAL_SemaRelease( uint32 sem_id )
{
	BaseType_t err;
	int32 ret;
	volatile uint32 intNestingCnt = 0;
	
	if (OSAL_Sema_Obj[sem_id].used == 1)
	{
		intNestingCnt = xPortGetInterruptNestingCount();
		if(intNestingCnt == 0)
		{
			err = xSemaphoreGive((SemaphoreHandle_t)&(OSAL_Sema_Obj[sem_id].semaphore));
		}
		else
		{
			BaseType_t xHigherPriorityTaskWoken = pdFALSE;
			err = xSemaphoreGiveFromISR((SemaphoreHandle_t)&(OSAL_Sema_Obj[sem_id].semaphore), &xHigherPriorityTaskWoken);
			portYIELD_FROM_ISR( xHigherPriorityTaskWoken )//; QAC
		}
		
		if (err == pdPASS)
		{
			ret = OSAL_ERR_NONE;
		}
		else
		{
			ret = OSAL_ERR_SEMA;
		}
	}
	else
	{
		ret = OSAL_ERR_INVALID_PARAMETER;
	}
	
	return ret;
}

int32 OSAL_EventCreate(uint32 *event_id, const uint8 *event_name, uint32 event_initial_value,  int32* native_err)
{
	BaseType_t err;
	int32 retval = OSAL_ERR_NONE;
	int32 i, search_id = -1;	// CS : Redundant Condition
	volatile uint32 intNestingCnt = 0;
	
	if (event_name == 0)
	{
		retval = OSAL_ERR_INVALID_PARAMETER;
	}
	else
	{
		/* obtain sema. wait forever*/
		intNestingCnt = xPortGetInterruptNestingCount();
		if(intNestingCnt == 0)
		{
			err = xSemaphoreTake((SemaphoreHandle_t)&osal_obj_sem, (TickType_t )portMAX_DELAY);
		}
		else
		{
			BaseType_t xHigherPriorityTaskWoken = pdFALSE;
			err = xSemaphoreTakeFromISR((SemaphoreHandle_t)&osal_obj_sem, &xHigherPriorityTaskWoken);
		}
		if(err == pdTRUE)
		{
			/*search free table*/
			for(i = 0; i < OSAL_MAX_EVENT; i++)
			{
				if(OSAL_Event_Obj[i].used == 0)
				{
					search_id = i;
					break;
				}
			}

			if ( search_id >= 0)// CS : Redundant Condition 
			{
				EventGroupHandle_t pEventGroupHandle = 0;

				pEventGroupHandle = xEventGroupCreateStatic((StaticEventGroup_t *)&(OSAL_Event_Obj[search_id].event));
				
				
				if(pEventGroupHandle != 0)
				{
					OSAL_Event_Obj[search_id].used = 1;
					*event_id = search_id;
				}
				else
				{
					retval = OSAL_ERR_EVENT;
				}
				
				if ( native_err != 0)
				{
					*native_err = 0; //Actually there is no native err for FreeRTOS
				}
			}
			else
			{
				retval = OSAL_ERR_NO_SPACE;
			}
			/*release sema*/
			intNestingCnt = xPortGetInterruptNestingCount();
			if(intNestingCnt == 0)
			{
				(void) xSemaphoreGive((SemaphoreHandle_t)&osal_obj_sem);
			}
			else
			{
				BaseType_t xHigherPriorityTaskWoken = pdFALSE;
				(void) xSemaphoreGiveFromISR((SemaphoreHandle_t)&osal_obj_sem, &xHigherPriorityTaskWoken);
			}
		}
		else
		{
			retval = OSAL_ERR_EVENT;
		}
	}
	
	return retval;
}

int32 OSAL_EventDelete(uint32 event_id,  int32* native_err)
{
	int32 ret = OSAL_ERR_NOT_SUPPORT;

	return ret;
}

int32 OSAL_SetEvent(uint32 event_id, uint32 event, uint32 options,  int32* native_err)
{
	BaseType_t err;
	int32 ret; // CS : Redundant Condition
	volatile uint32 intNestingCnt = 0;
	
	if (OSAL_Event_Obj[event_id].used == 1)
	{
		intNestingCnt = xPortGetInterruptNestingCount();
		
		if ( (options & 0x00000001ul) ==OSAL_EVENT_SET_OPT_FLAG_SET)
		{
			if(intNestingCnt == 0)
			{
				(void) xEventGroupSetBits((EventGroupHandle_t)&(OSAL_Event_Obj[event_id].event), (EventBits_t)event); //QAC-Not use return value
				ret = OSAL_ERR_NONE;
			}
			else
			{
				BaseType_t xHigherPriorityTaskWoken = pdFALSE;
				err = xEventGroupSetBitsFromISR((EventGroupHandle_t)&(OSAL_Event_Obj[event_id].event), (EventBits_t)event, &xHigherPriorityTaskWoken);
				if (err == pdTRUE)
				{
					ret = OSAL_ERR_NONE;
					portYIELD_FROM_ISR( xHigherPriorityTaskWoken )
				}
				else
				{
					ret = OSAL_ERR_EVENT;
				}
			}
		}
		else if( (options & 0x00000001ul) ==OSAL_EVENT_SET_OPT_FLAG_CLR)
		{
			if(intNestingCnt == 0)
			{
				(void) xEventGroupClearBits((EventGroupHandle_t)&(OSAL_Event_Obj[event_id].event), (EventBits_t)event); //QAC-Not use return value
				ret = OSAL_ERR_NONE;
			}
			else
			{
				err = xEventGroupClearBitsFromISR((EventGroupHandle_t)&(OSAL_Event_Obj[event_id].event), (EventBits_t)event);
				if (err == pdTRUE)
				{
					ret = OSAL_ERR_NONE;
				}
				else
				{
					ret = OSAL_ERR_EVENT;
				}
			}
		}
		else
		{
			ret = OSAL_ERR_INVALID_PARAMETER;
		}
	}
	else
	{
		ret = OSAL_ERR_EVENT;
	}
	
	return ret;
}

uint32 OSAL_GetEvent(uint32 event_id, uint32 event, uint32 timeout, uint32 options, int32 *err, int32* native_err)
{
	uint32 flags= 0;
	if (OSAL_Event_Obj[event_id].used == 1)
	{
		uint32 uxBits;
		uint32 is_Consume = pdFALSE, is_All = pdFALSE;
		uint32 timeDly; // CS : Redundant Condition

		if ( (options & OSAL_EVENT_OPT_SET_ALL) != 0)
		{
			is_All = pdTRUE;
		}
		else if ( (options & OSAL_EVENT_OPT_CONSUME)!= 0)
		{
			is_Consume = pdTRUE;
		}
		else
		{
			; //QAC-Add else statement
		}
		
		if( (options & 0x00008000) == OSAL_API_OPT_NON_BLOCKING)  //CS : Redundant Condition
		{
			timeDly = 0; // no block time
		}
		else
		{
			if (timeout == 0) {
				timeDly = portMAX_DELAY;
			} else {
				timeDly = timeout;
			}
		}
		
		uxBits = xEventGroupWaitBits((EventGroupHandle_t)&(OSAL_Event_Obj[event_id].event),
								event,
								is_Consume,
								is_All,
								(TickType_t)timeDly);
		if ( uxBits == pdFALSE)
		{
			*err = OSAL_ERR_EVENT;  
		}
		else
		{
			flags = uxBits;
			*err = OSAL_ERR_NONE;  
		}
		if ( native_err != 0)
		{
			*native_err = 0; //There is no native err for FreeRTOS
		}
	}
	else
	{
		*err = OSAL_ERR_INIT;  
	}
	return flags;
}


void OSAL_MemSet( void *pmem, uint8 val, int32   size)
{
	if ( (pmem != 0) && (size > 0))
	{
		(void) memset(pmem, (int32)val, (size_t)size); //QAC-Not use return value
	}
}

void OSAL_MemCopy(  void *pdest, const  void *psrc, uint32   size)
{
	if ( (pdest != 0) && (psrc !=0 ) &&(size > 0))
	{
		(void) memcpy(pdest, (const  void *)psrc, (size_t)size); //QAC-Not use return value
	}
}

int32 OSAL_MemCmp(const  void * pmem1, const  void *pmem2, uint32 size)
{
	int32 iRet = OSAL_ERR_NONE;
	if ( (pmem1 != 0) && (pmem2 !=0 ) &&(size > 0))
	{
		iRet = memcmp((const  void *)pmem1,(const  void *)pmem2, size);
	}
	return iRet;
}


/*String*/
int8 * OSAL_StrCopy(int8 *dest_str, const int8 *src_srt)
{
	int8 * ret = 0;
	if ( (dest_str !=0) && (src_srt !=0))
	{
		uint32 str_len = 0;
		str_len = strnlen((const int8 *)src_srt, OSAL_DEF_INT_32U_MAX_VAL);
		if ( str_len != 0)
		{
			ret = strncpy((int8*)dest_str, (const int8*)src_srt, str_len); //Codesonar Security Warning
		}
	}
	return ret;
}

int16 OSAL_StrCmp(const int8 *p1, const int8 *p2)
{
	int16 ret = -1;
	if ( (p1 !=0) && (p2 !=0))
	{
		ret = (int16)strncmp((const int8 *)p1, (const int8 *)p2, OSAL_DEF_INT_32U_MAX_VAL); //Codesonar Security Warning
	}
	return ret;
}

int16 OSAL_StrNCmp(const int8 *p1, const int8 *p2, int32 len)
{
	int16 ret = -1;
	if ( (p1 !=0) && (p2 !=0) && (len>0))
	{
		ret = (int16)strncmp((const int8 *)p1, (const int8 *)p2, (uint32)len);
	}
	return ret;
}

uint32 OSAL_StrLen(const int8 *pstr)
{
	uint32 ret = 0;
	if ( pstr !=0)
	{
		ret = strnlen((const int8 *)pstr, OSAL_DEF_INT_32U_MAX_VAL); //Codesonar Security Warning
	}
	return ret;
}

void  OSTimeTick (void)
{
	FreeRTOS_Tick_Handler();
}



OS_ErrCode_t OS_CreateTask(OS_TskId_t		*auwTskId_p,
						OS_FuncPtr_t				avdTskFunc_p,
						 OS_CVoidPtr_t				avdTskName_p,
						 OS_ObjPri_t				auwTskPriority,
						 OS_MemSize_t				auwTskStackDepth,
						 OS_MemSize_t * const		auwTskStack_p)
{
	OS_ErrCode_t aswErrCode = OS_ERR_OK;
	uint32  auwTskId;

	auwTskId = OS_MAX_TSK;

	if ((NULL == avdTskFunc_p) || (NULL == avdTskName_p)
		|| (configMAX_PRIORITIES < auwTskPriority) || (0 == auwTskPriority)
		|| (0 == auwTskStackDepth) || (NULL == auwTskStack_p)||(NULL == auwTskId_p)) 
	{
		aswErrCode = OS_ERR_PAR;
		return aswErrCode;
	} 
	else
	{
		aswErrCode = OSAL_TaskCreate(
			&auwTskId,
			avdTskName_p,
			(TaskFunction_t)avdTskFunc_p,
			(uint32 *)auwTskStack_p,
			auwTskStackDepth,
			auwTskPriority,
			NULL);
		if(aswErrCode != OSAL_ERR_NONE)
		{
			return aswErrCode;
		}else{
			*auwTskId_p = auwTskId;
		}
		
	}
	return aswErrCode;

}

OS_ErrCode_t OS_GetTaskId(OS_TskId_t *auwCurrentTskId_p)
{
	OS_ErrCode_t aswErrCode;
	OS_TskId_t auwTskId;
	StaticTask_t *tcb_p;
	OS_u32Bit_t i;

	if (NULL == auwCurrentTskId_p) {
		aswErrCode = OS_ERR_PAR;
	} else {
		tcb_p = (StaticTask_t*)xTaskGetCurrentTaskHandle();
		auwTskId = OS_INVALID_ID;

		for (i = 0; (i < OS_MAX_TSK - 1) && (0 != OS_cfg_tsk[i].uwTskId_p); i++) {
			if (OS_cfg_tsk[i].uwTskStack_p == (uint32_t*)tcb_p->pxDummy6) {
				auwTskId = *OS_cfg_tsk[i].uwTskId_p;
				break;
			}
		}

		if (OS_INVALID_ID == auwTskId) {
			aswErrCode = OS_ERR_NG;
			//OSFW_E("tcb_p %x, pxDummy6 %x", tcb_p, (uint32*)tcb_p->pxDummy6);
		} else {
			aswErrCode = OS_ERR_OK;
		}
		*auwCurrentTskId_p = auwTskId;
	}

	return aswErrCode;
}

void OS_SleepTask(OS_TimeTick_t auwTimeout)
{
	OSAL_TaskDelay(auwTimeout);
}

OS_ErrCode_t OS_CreateQueue(OS_QueueId_t	*auwQueueId_p,
							OS_MemSize_t	auwQueueLength,
						  	OS_CVoidPtr_t	avdQueueName_p)
{
	OS_ErrCode_t aswErrCode = OSAL_ERR_NONE;
	uint32  auwMbxId;

	auwMbxId = OS_MAX_MBX;

	if ((0 == auwQueueLength)||(NULL == auwQueueId_p)) 
	{
		aswErrCode = OS_ERR_PAR;
		return aswErrCode;
	} 
	else 
	{
		aswErrCode = OSAL_QueueCreate(&auwMbxId,
			avdQueueName_p,
			auwQueueLength,
			sizeof(OS_StdQueue_ts),
			0);
		if(aswErrCode == OSAL_ERR_NONE)
		{
			*auwQueueId_p = auwMbxId;
		}else{
			return aswErrCode;
		}
	}
	return aswErrCode;
}

OS_ErrCode_t OS_SendQueue(OS_QueueId_t	auwQueueId,  OS_ObjPri_t auwQueuePriority, OS_StdQueue_ts	 *astStdQueue_p)
{
	OS_ErrCode_t aswErrCode;
	volatile OS_u32Bit_t auwIsISR;
	OS_VoidPtr_t avdQueueHdl;
	OS_u32Bit_t auwQueueRet;

	if ((OS_MAX_MBX <= auwQueueId) || (OSAL_Queue_Obj[auwQueueId].used == 0))
	{
		aswErrCode = OS_ERR_ID;
	} 
	else if (NULL == astStdQueue_p) 
	{
		aswErrCode = OS_ERR_PAR;
	} 
	else 
	{
		avdQueueHdl = (QueueHandle_t)&OSAL_Queue_Obj[auwQueueId].queue;
		auwIsISR = xPortGetInterruptNestingCount();
		if (auwIsISR) 
		{
			BaseType_t xHigherPriorityTaskWoken = pdFALSE;
			auwQueueRet = xQueueSendFromISR((QueueHandle_t)avdQueueHdl, astStdQueue_p, &xHigherPriorityTaskWoken);
			portYIELD_FROM_ISR( xHigherPriorityTaskWoken )
		} 
		else 
		{
			auwQueueRet = xQueueSend((QueueHandle_t)avdQueueHdl, astStdQueue_p, 0);
		}
		if(pdPASS == auwQueueRet){
			aswErrCode = OS_ERR_OK;
		} else {
			aswErrCode = OS_ERR_NG;
		}
	}
	return aswErrCode;
}

OS_ErrCode_t OS_ReceiveQueue(OS_QueueId_t auwQueueId, OS_TimeTick_t auwTimeout,OS_StdQueue_ts	**astStdQueue_pp)
{
	OS_ErrCode_t aswErrCode;
	OS_VoidPtr_t avdQueueHdl;
	volatile OS_u32Bit_t auwIsISR;

	if ((OS_MAX_MBX <= auwQueueId) || (0 == OSAL_Queue_Obj[auwQueueId].used)) {
		aswErrCode = OS_ERR_ID;
	} else if (NULL == astStdQueue_pp) {
		aswErrCode = OS_ERR_PAR;
	} else {
		avdQueueHdl = (QueueHandle_t)&OSAL_Queue_Obj[auwQueueId].queue;
		auwIsISR = xPortGetInterruptNestingCount();
		if (auwIsISR) {
			BaseType_t xHigherPriorityTaskWoken = pdFALSE;
			if (pdPASS == xQueueReceiveFromISR((QueueHandle_t)avdQueueHdl, *astStdQueue_pp, &xHigherPriorityTaskWoken)) {
				aswErrCode = OS_ERR_OK;
			} else {
				aswErrCode = OS_ERR_TMOUT;
			}
			portYIELD_FROM_ISR( xHigherPriorityTaskWoken )
		} else {
			if (pdPASS == xQueueReceive((QueueHandle_t)avdQueueHdl, *astStdQueue_pp, pdMS_TO_TICKS(auwTimeout))) {
				aswErrCode = OS_ERR_OK;
			} else {
				aswErrCode = OS_ERR_TMOUT;
			}
		}
	}
	
	return aswErrCode;
}

OS_ErrCode_t OS_MessagesWaitingQueue(OS_QueueId_t auwQueueId,Type_uWord *auwqueue_p)
{
	OS_ErrCode_t aswErrCode = OS_ERR_OK;
	OS_VoidPtr_t avdQueueHdl;

	if ((OS_MAX_MBX <= auwQueueId) || (0 == OSAL_Queue_Obj[auwQueueId].used)) {
		aswErrCode = OS_ERR_ID;
	} else {
		avdQueueHdl = (QueueHandle_t)&OSAL_Queue_Obj[auwQueueId].queue;
		*auwqueue_p = uxQueueMessagesWaiting(avdQueueHdl);
	}
	return aswErrCode;
}


OS_ErrCode_t OS_CreateSemaphore(OS_ObjId_t aubSemId,OS_SemCnt_t	auwSemMaxCnt, OS_SemCnt_t	auwSemInitialCnt)
{
	OS_ErrCode_t aswErrCode = OS_ERR_OK;
	SemaphoreHandle_t avdSemHdl;

	avdSemHdl = NULL;

	if ((0 == auwSemMaxCnt) || (auwSemInitialCnt > auwSemMaxCnt)) {
		aswErrCode = OS_ERR_PAR;
	} else {
		if (OS_MAX_SEM > aubSemId) {
			avdSemHdl = xSemaphoreCreateCountingStatic(
				auwSemMaxCnt,
				auwSemInitialCnt,
				&OSAL_Sema_Obj[aubSemId].semaphore);
			if (NULL != avdSemHdl) {
				OSAL_Sema_Obj[aubSemId].used = 1;
			} else {
				aswErrCode = OS_ERR_NG;
			}
		} else {
			aswErrCode = OS_ERR_NOMEM;
		}
	}

	return aswErrCode;
}

OS_ErrCode_t OS_WaitSemaphore(OS_ObjId_t	aubSemId, OS_TimeTick_t	auwTimeout)
{
	OS_ErrCode_t aswErrCode;
	OS_VoidPtr_t avdSemHdl;
	volatile OS_u32Bit_t auwIsISR;

	if ((OS_MAX_SEM <= aubSemId) || (0 == OSAL_Sema_Obj[aubSemId].used)) {
		aswErrCode = OS_ERR_ID;
	} else {
		avdSemHdl = (SemaphoreHandle_t)&OSAL_Sema_Obj[aubSemId].semaphore;
		auwIsISR = xPortGetInterruptNestingCount();
		if (auwIsISR) {
			if (pdPASS == xSemaphoreTakeFromISR((SemaphoreHandle_t)avdSemHdl, NULL)) {
				aswErrCode = OS_ERR_OK;
			} else {
				aswErrCode = OS_ERR_TMOUT;
			}
		} else {
			if (pdPASS == xSemaphoreTake((SemaphoreHandle_t)avdSemHdl, pdMS_TO_TICKS(auwTimeout))) {
				aswErrCode = OS_ERR_OK;
			} else {
				aswErrCode = OS_ERR_TMOUT;
			}
		}
	}

	return aswErrCode;
}

OS_ErrCode_t OS_SignalSemaphore(OS_ObjId_t	aubSemId)
{
	OS_ErrCode_t aswErrCode;
	OS_VoidPtr_t avdSemHdl;
	volatile OS_u32Bit_t auwIsISR;

	if ((OS_MAX_SEM <= aubSemId) || (0 == OSAL_Sema_Obj[aubSemId].used)) {
		aswErrCode = OS_ERR_ID;
	} else {
		avdSemHdl = (SemaphoreHandle_t)&OSAL_Sema_Obj[aubSemId].semaphore;
		auwIsISR = xPortGetInterruptNestingCount();
		if (auwIsISR) {
			if (pdPASS == xSemaphoreGiveFromISR((SemaphoreHandle_t)avdSemHdl, NULL)) {
				aswErrCode = OS_ERR_OK;
			} else {
				aswErrCode = OS_ERR_NG;
			}
		} else {
			if (pdPASS == xSemaphoreGive((SemaphoreHandle_t)avdSemHdl)) {
				aswErrCode = OS_ERR_OK;
			} else {
				aswErrCode = OS_ERR_NG;
			}
		}
	}

	return aswErrCode;
}
OS_ErrCode_t OS_CreateEventFlag(OS_ObjId_t aubFlgId)
{
	OS_ErrCode_t aswErrCode = OS_ERR_OK;
	EventGroupHandle_t avdFlgHdl;
	
	avdFlgHdl = NULL;
	
	if (OS_MAX_FLG > aubFlgId) {
		avdFlgHdl = xEventGroupCreateStatic(&OSAL_Event_Obj[aubFlgId].event);
		if (NULL != avdFlgHdl) {
			OSAL_Event_Obj[aubFlgId].used = 1;
		} else {
			aswErrCode = OS_ERR_NG;
		}
	} else {
		aswErrCode = OS_ERR_NOMEM;
	}
	
	return aswErrCode;
}

OS_ErrCode_t OS_SetEventFlag(OS_ObjId_t aubFlgId, OS_FlgPtn_t auwFlgPtn)
{
	OS_ErrCode_t aswErrCode;
	OS_VoidPtr_t avdFlgHdl;
	volatile OS_u32Bit_t auwIsISR;

	if ((OS_MAX_FLG <= aubFlgId) || (0 == OSAL_Event_Obj[aubFlgId].used)) {
		aswErrCode = OS_ERR_ID;
	} else {
		avdFlgHdl = (EventGroupHandle_t)&OSAL_Event_Obj[aubFlgId].event;
		auwIsISR = xPortGetInterruptNestingCount();
		if (auwIsISR) {
			if (pdPASS == xEventGroupSetBitsFromISR((EventGroupHandle_t)avdFlgHdl, auwFlgPtn & OS_FLAG_MASK, NULL)) {
				aswErrCode = OS_ERR_OK;
			} else {
				aswErrCode = OS_ERR_NG;
			}
		} else {
			xEventGroupSetBits((EventGroupHandle_t)avdFlgHdl, auwFlgPtn & OS_FLAG_MASK);
			aswErrCode = OS_ERR_OK;
		}
	}

	return aswErrCode;
}

OS_ErrCode_t OS_ClearEventFlag(OS_ObjId_t aubFlgId, OS_FlgPtn_t	auwFlgPtn)
{
	OS_ErrCode_t aswErrCode;
	OS_VoidPtr_t avdFlgHdl;
	volatile OS_u32Bit_t auwIsISR;

	if ((OS_MAX_FLG <= aubFlgId) || (0 == OSAL_Event_Obj[aubFlgId].used)) {
		aswErrCode = OS_ERR_ID;
	} else {
		avdFlgHdl = (EventGroupHandle_t)&OSAL_Event_Obj[aubFlgId].event;
		auwIsISR = xPortGetInterruptNestingCount();
		if (auwIsISR) {
			if (pdPASS == xEventGroupClearBitsFromISR((EventGroupHandle_t)avdFlgHdl, ~auwFlgPtn & OS_FLAG_MASK)) {
				aswErrCode = OS_ERR_OK;
			} else {
				aswErrCode = OS_ERR_NG;
			}
		} else {
			xEventGroupClearBits((EventGroupHandle_t)avdFlgHdl, ~auwFlgPtn & OS_FLAG_MASK);
			aswErrCode = OS_ERR_OK;
		}
	}

	return aswErrCode;
}
/********************************************************************************************/
/**	\function		OS_WaitEventFlag
 *	\date			2021/11/03
 *	\author			xulei
 *	\description	Free RTOS Wait Evert Flag
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			OS_ErrCode_t
 *					
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001		-			-					New
 ********************************************************************************************/

OS_ErrCode_t OS_WaitEventFlag(OS_ObjId_t		aubFlgId,
							  OS_FlgPtn_t		auwWaitFlgPtn,
							  OS_SvcMode_t		auwSvcCallMode,
							  OS_FlgPtn_t		*auwClrFlgPtn_p,
							  OS_TimeTick_t		auwTimeout)
{
	OS_ErrCode_t aswErrCode;
	OS_VoidPtr_t avdFlgHdl;

	if ((OS_MAX_FLG <= aubFlgId) || (0 == OSAL_Event_Obj[aubFlgId].used)) {
		aswErrCode = OS_ERR_ID;
	} else if ((0 == (auwWaitFlgPtn & OS_FLAG_MASK)) || (NULL == auwClrFlgPtn_p)) {
		aswErrCode = OS_ERR_PAR;
	} else {
		avdFlgHdl = (EventGroupHandle_t)&OSAL_Event_Obj[aubFlgId].event;
		*auwClrFlgPtn_p = xEventGroupWaitBits((EventGroupHandle_t)avdFlgHdl, auwWaitFlgPtn & OS_FLAG_MASK, pdTRUE, auwSvcCallMode, pdMS_TO_TICKS(auwTimeout));
		aswErrCode = OS_ERR_OK;
	}

	return aswErrCode;
}
/********************************************************************************************/
/**	\function		OS_CreateTimer
 *	\date			2021/11/03
 *	\author			xulei
 *	\description	Free RTOS Timer Create
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		OS_ObjId_t *aubTimerId_p
 *	\param[in]		OS_FuncPtr_t	avdTmrFunc_p
 *	\param[in]		OS_CVoidPtr_t	avdTmrName_p
 *	\param[in]		OS_TimeTick_t	auwTmrPeriod
 *	\param[in]		OS_ReloadMode_t aubReloadMode
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			OS_ErrCode_t
 *					
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001		-			-					New
 ********************************************************************************************/

OS_ErrCode_t OS_CreateTimer(OS_ObjId_t aubTmrId,
						  OS_FuncPtr_t		avdTmrFunc_p,
						  OS_CVoidPtr_t		avdTmrName_p,
						  OS_TimeTick_t		auwTmrPeriod,
						  OS_ReloadMode_t 	aubReloadMode)
{
	OS_ErrCode_t aswErrCode = OS_ERR_OK;
	TimerHandle_t avdTmrHdl;

	avdTmrHdl = NULL;

	if ((NULL == avdTmrFunc_p) || (0 == auwTmrPeriod)) {
		aswErrCode = OS_ERR_PAR;
	} else {
		if (OS_MAX_TMR > aubTmrId) {
			avdTmrHdl = xTimerCreateStatic(
				avdTmrName_p,							/* Timer Name */
				pdMS_TO_TICKS(auwTmrPeriod),			/* Timer Period in ticks */
				aubReloadMode,							/* Auto-reload */
				(void *) 0, 							/* Not used */
				(TimerCallbackFunction_t)avdTmrFunc_p,	/* Timer Callback Function */
				&OS_TmrTable[aubTmrId].stdata);			/* Timer Static Data Pointer */
			if (NULL != avdTmrHdl) {
				OS_TmrTable[aubTmrId].used = 1;
			} else {
				aswErrCode = OS_ERR_NG;
			}
		} else {
			aswErrCode = OS_ERR_NOMEM;
		}
	}

	return aswErrCode;
}
/********************************************************************************************/
/**	\function		OS_StartTimer
 *	\date			2021/11/03
 *	\author			xulei
 *	\description	Free RTOS Timer Start
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		OS_ObjId_t aubTmrId
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			OS_ErrCode_t
 *					
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001		-			-					New
 ********************************************************************************************/

OS_ErrCode_t OS_StartTimer(OS_ObjId_t aubTmrId)
{
	OS_ErrCode_t aswErrCode;
	OS_VoidPtr_t avdTmrHdl;
	volatile OS_u32Bit_t auwIsISR;

	if ((OS_MAX_TMR <= aubTmrId) || (0 == OS_TmrTable[aubTmrId].used)) {
		aswErrCode = OS_ERR_ID;
	} else {
		avdTmrHdl = (TimerHandle_t)&OS_TmrTable[aubTmrId].stdata;
		auwIsISR = xPortGetInterruptNestingCount();
		if (auwIsISR) {
			if (pdPASS == xTimerStartFromISR((TimerHandle_t)avdTmrHdl, NULL)) {
				aswErrCode = OS_ERR_OK;
			} else {
				aswErrCode = OS_ERR_NG;
			}
		} else {
			if (pdPASS == xTimerStart((TimerHandle_t)avdTmrHdl, 0)) {
				aswErrCode = OS_ERR_OK;
			} else {
				aswErrCode = OS_ERR_NG;
			}
		}
	}

	return aswErrCode;
}
/********************************************************************************************/
/* Function Name	: OS_TimerChangePeriod													*/
/* Date				: 																		*/
/* Author			: 																		*/
/* Description		: 																		*/
/* Argument Code	:																		*/
/* Return Code		:																		*/
/*------------------------------------------------------------------------------------------*/
/* Revision History																			*/
/* No		Date		Revised by		Explanation											*/
/*==========================================================================================*/
/* 																							*/
/********************************************************************************************/
OS_ErrCode_t OS_TimerChangePeriod(OS_ObjId_t aubTmrId,OS_TimeTick_t auwTmrNewPeriod)
{
	OS_ErrCode_t aswErrCode;
	OS_VoidPtr_t avdTmrHdl;
	volatile OS_u32Bit_t auwIsISR;

	do{
		if ((OS_MAX_TMR <= aubTmrId) || (0 == OS_TmrTable[aubTmrId].used)) {
			aswErrCode = OS_ERR_ID;
			break;
		}

		if( 0 == auwTmrNewPeriod ){
			aswErrCode = OS_ERR_PAR;
			break;
		}
		
		avdTmrHdl = (TimerHandle_t)&OS_TmrTable[aubTmrId].stdata;
		auwIsISR = xPortGetInterruptNestingCount();
		
		if (auwIsISR) {
			if (pdPASS == xTimerChangePeriodFromISR((TimerHandle_t)avdTmrHdl,pdMS_TO_TICKS(auwTmrNewPeriod),0)){
				aswErrCode = OS_ERR_OK;
			} else {
				aswErrCode = OS_ERR_NG;
			}
		} else {
			if (pdPASS == xTimerChangePeriod((TimerHandle_t)avdTmrHdl,pdMS_TO_TICKS(auwTmrNewPeriod),0)) {
				aswErrCode = OS_ERR_OK;
			} else {
				aswErrCode = OS_ERR_NG;
			}
		}
	}while(0);
		
	return aswErrCode;
}

/********************************************************************************************/
/**	\function		OS_StopTimer
 *	\date			2021/11/03
 *	\author			xulei
 *	\description	Free RTOS Timer Stop
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		OS_ObjId_t aubTmrId
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			OS_ErrCode_t
 *					
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001		-			-					New
 ********************************************************************************************/

OS_ErrCode_t OS_StopTimer(OS_ObjId_t aubTmrId)
{
	OS_ErrCode_t aswErrCode;
	OS_VoidPtr_t avdTmrHdl;
	volatile OS_u32Bit_t auwIsISR;

	aswErrCode = OS_ERR_OK;

	if ((OS_MAX_TMR <= aubTmrId) || (0 == OS_TmrTable[aubTmrId].used)) {
		aswErrCode = OS_ERR_ID;
	} else {
		avdTmrHdl = (TimerHandle_t)&OS_TmrTable[aubTmrId].stdata;
		if( pdFAIL == xTimerIsTimerActive((TimerHandle_t)avdTmrHdl)){
			/* xtimer is not Active*/
		}else{
			auwIsISR = xPortGetInterruptNestingCount();
			if (auwIsISR) {
				if (pdPASS == xTimerStopFromISR((TimerHandle_t)avdTmrHdl, NULL)) {
					aswErrCode = OS_ERR_OK;
				} else {
					aswErrCode = OS_ERR_NG;
				}
			} else {
				if (pdPASS == xTimerStop((TimerHandle_t)avdTmrHdl, 0)) {
					aswErrCode = OS_ERR_OK;
				} else {
					aswErrCode = OS_ERR_NG;
				}
			}
		}
	}

	return aswErrCode;
}
/********************************************************************************************/
/**	\function		OS_ResetTimer
 *	\date			2021/11/03
 *	\author			xulei
 *	\description	Free RTOS Timer Reset
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		OS_ObjId_t aubTmrId
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			OS_ErrCode_t
 *					
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001		-			-					New
 ********************************************************************************************/

OS_ErrCode_t OS_ResetTimer(OS_ObjId_t aubTmrId)
{
	OS_ErrCode_t aswErrCode;
	OS_VoidPtr_t avdTmrHdl;
	volatile OS_u32Bit_t auwIsISR;
	if ((OS_MAX_TMR <= aubTmrId) || (0 == OS_TmrTable[aubTmrId].used)) {
		aswErrCode = OS_ERR_ID;
	} else {
		avdTmrHdl = (TimerHandle_t)&OS_TmrTable[aubTmrId].stdata;
		auwIsISR = xPortGetInterruptNestingCount();
		if (auwIsISR) {
			if (pdPASS == xTimerResetFromISR((TimerHandle_t)avdTmrHdl, NULL)) {
				aswErrCode = OS_ERR_OK;
			} else {
				aswErrCode = OS_ERR_NG;
			}
		} else {
			if (pdPASS == xTimerReset((TimerHandle_t)avdTmrHdl, 0)) {
				aswErrCode = OS_ERR_OK;
			} else {
				aswErrCode = OS_ERR_NG;
			}
		}
	}

	return aswErrCode;
}

/********************************************************************************************/
/**	\function		OS_GetFMemoryBlock
 *	\date			2021/11/03
 *	\author			xulei
 *	\description	OS Malloc Memory
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		OS_VoidPtr_t	*avdMemBlk_pp, OS_MemSize_t	auwMemBlkSize
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			OS_ErrCode_t
 *					
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001		-			-					New
 ********************************************************************************************/

OS_ErrCode_t OS_GetFMemoryBlock(OS_VoidPtr_t	*avdMemBlk_pp, OS_MemSize_t	auwMemBlkSize)
{
	OS_ErrCode_t aswErrCode;
	OS_MemSize_t auwHeapSize = xPortGetFreeHeapSize();

	if ((NULL == avdMemBlk_pp) || (0 == auwMemBlkSize)) {
		aswErrCode = OS_ERR_PAR;
	} else if ((0 != auwHeapSize)
		&& ((OS_MEM_SAFE_MARGIN > auwHeapSize)
		|| (auwMemBlkSize > (auwHeapSize - OS_MEM_SAFE_MARGIN)))) {
		aswErrCode = OS_ERR_NOMEM;
	} else {
		*avdMemBlk_pp = pvPortMalloc(auwMemBlkSize);
		if (NULL != *avdMemBlk_pp) {
			aswErrCode = OS_ERR_OK;
		} else {
			aswErrCode = OS_ERR_NOMEM;
		}
	}

	return aswErrCode;
}
/********************************************************************************************/
/**	\function		OS_ReleaseFMemoryBlock
 *	\date			2021/11/03
 *	\author			xulei
 *	\description	OS Release Memory
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		void
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			void
 *					
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001		-			-					New
 ********************************************************************************************/

OS_ErrCode_t OS_ReleaseFMemoryBlock(OS_VoidPtr_t avdMemBlk_p)
{
	OS_ErrCode_t aswErrCode;

	if (NULL == avdMemBlk_p) {
		aswErrCode = OS_ERR_PAR;
	} else {
		vPortFree(avdMemBlk_p);
		aswErrCode = OS_ERR_OK;
	}

	return aswErrCode;
}
/*********************************************************************************************
*  function                 : nvdOSWaitTime
*  date                     : 2022.01.26
*  author                   : xulei
*  description              : I2C Software Driver WaitTime Process.
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  \param[in]		auhMaxWaitTime
					1 = 1usec
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  \return			none
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*  
*********************************************************************************************/
void OS_WaitTime(Type_uWord auwMaxWaitTime)
{
	/********************************************************/
	/* [nop] Count		= [usec] * Clock600[MHz]			*/
	/* 					= auhMaxWaitTime					*/
	/********************************************************/
	Type_uWord		auwNopCounter;
	Type_uWord		ticks = 0;
	
	ticks = auwMaxWaitTime * 8;
	
	for (auwNopCounter = 0; auwNopCounter < ticks; auwNopCounter++){
		asm(" nop ");
	}
}

