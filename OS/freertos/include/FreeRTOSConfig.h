/*
    FreeRTOS V9.0.0 - Copyright (C) 2016 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/
/*******************************************************************************
*   Modified by Telechips Inc.
 
 
*   Modified date :
 
 
*   Description :
*******************************************************************************/
 #ifndef INC_FREERTOS_CONFIG_H
#define INC_FREERTOS_CONFIG_H

#include <app_cfg.h>

/*
 * The FreeRTOS Cortex-A port implements a full interrupt nesting model.
 *
 * Interrupts that are assigned a priority at or below
 * configMAX_API_CALL_INTERRUPT_PRIORITY (which counter-intuitively in the ARM
 * generic interrupt controller [GIC] means a priority that has a numerical
 * value above configMAX_API_CALL_INTERRUPT_PRIORITY) can call FreeRTOS safe API
 * functions and will nest.
 *
 * Interrupts that are assigned a priority above
 * configMAX_API_CALL_INTERRUPT_PRIORITY (which in the GIC means a numerical
 * value below configMAX_API_CALL_INTERRUPT_PRIORITY) cannot call any FreeRTOS
 * API functions, will nest, and will not be masked by FreeRTOS critical
 * sections (although it is necessary for interrupts to be globally disabled
 * extremely briefly as the interrupt mask is updated in the GIC).
 *
 * FreeRTOS functions that can be called from an interrupt are those that end in
 * "FromISR".  FreeRTOS maintains a separate interrupt safe API to enable
 * interrupt entry to be shorter, faster, simpler and smaller.
 *
 * Setting this value to zero means that nesting interrupts are not used.
 */
#define configMAX_API_CALL_INTERRUPT_PRIORITY	0//(16 -2)

#define configCPU_CLOCK_HZ						100000000UL
#define configUSE_PORT_OPTIMISED_TASK_SELECTION	0
#define configUSE_TICKLESS_IDLE					0
#define configTICK_RATE_HZ						( ( TickType_t ) 1000 )
#define configPERIPHERAL_CLOCK_HZ  				( 33333000UL )
#define configUSE_PREEMPTION					0
#define configUSE_IDLE_HOOK						0
#define configUSE_TICK_HOOK						0
#define configMAX_PRIORITIES					( 16 )
#define configMINIMAL_STACK_SIZE				( ( unsigned short ) 250 ) /* Large in case configUSE_TASK_FPU_SUPPORT is 2 in which case all tasks have an FPU context. */
#if (SERVICE_CONSOLE_EN == 1u)
#define configTOTAL_HEAP_SIZE					( 20 * 1024 )/* 2021/12/20 xulei  1024->16383*/
#else
#define configTOTAL_HEAP_SIZE					( 10 * 1024 )/* 2021/12/20 xulei  1024->16383*/
#endif

#define configMAX_TASK_NAME_LEN					( 30 )
#if (SERVICE_TEST_EN == 1u)
#define configUSE_TRACE_FACILITY				1		/* 2022/02/09 xulei test 0->1*/
#else
#define configUSE_TRACE_FACILITY				0
#endif
#define configUSE_16_BIT_TICKS					0
#define configIDLE_SHOULD_YIELD					1
#define configUSE_MUTEXES						1
#define configQUEUE_REGISTRY_SIZE				8
#define configCHECK_FOR_STACK_OVERFLOW			2
#define configUSE_RECURSIVE_MUTEXES				1
#define configUSE_MALLOC_FAILED_HOOK			0
#define configUSE_APPLICATION_TASK_TAG			0
#define configUSE_COUNTING_SEMAPHORES			1
#define configUSE_QUEUE_SETS					1
#define configSUPPORT_STATIC_ALLOCATION			1 // for MICOM system. use static allocation for os resource
#define configSUPPORT_DYNAMIC_ALLOCATION        1

/* Include the query-heap CLI command to query the free heap space. */
#define configINCLUDE_QUERY_HEAP_COMMAND		1

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES 					0
#define configMAX_CO_ROUTINE_PRIORITIES 		( 2 )

/* Software timer definitions. */
#define configUSE_TIMERS						1
#define configTIMER_TASK_PRIORITY				( configMAX_PRIORITIES - 1 )
#define configTIMER_QUEUE_LENGTH				200
#define configTIMER_TASK_STACK_DEPTH			( configMINIMAL_STACK_SIZE * 4 )

/* If configUSE_TASK_FPU_SUPPORT is set to 1 (or undefined) then each task will
be created without an FPU context, and a task must call vTaskUsesFPU() before
making use of any FPU registers.  If configUSE_TASK_FPU_SUPPORT is set to 2 then
tasks are created with an FPU context by default, and calling vTaskUsesFPU() has
no effect. */
//#define configUSE_TASK_FPU_SUPPORT				2
#define configUSE_TASK_FPU_SUPPORT				1

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
#define INCLUDE_vTaskPrioritySet				1
#define INCLUDE_uxTaskPriorityGet				1
#define INCLUDE_vTaskDelete						1
#define INCLUDE_vTaskCleanUpResources			1
#define INCLUDE_vTaskSuspend					1
#define INCLUDE_vTaskDelayUntil					1
#define INCLUDE_vTaskDelay						1
#define INCLUDE_xTimerPendFunctionCall			1
#define INCLUDE_eTaskGetState					1
#define INCLUDE_xTaskAbortDelay					1
#define INCLUDE_xTaskGetTaskHandle				1
#define INCLUDE_xTaskGetHandle					1

/* This demo makes use of one or more example stats formatting functions.  These
format the raw data provided by the uxTaskGetSystemState() function in to human
readable ASCII form.  See the notes in the implementation of vTaskList() within
FreeRTOS/Source/tasks.c for limitations. */
#define configUSE_STATS_FORMATTING_FUNCTIONS	1

/* The private watchdog is used to generate run time stats. */
//#include "xscuwdt.h"
//extern XScuWdt xWatchDogInstance;
extern void vInitialiseTimerForRunTimeStats( void );

#if (SERVICE_TEST_EN == 1u)
#define configGENERATE_RUN_TIME_STATS 1 // because use sprintf  /* 2022/02/09 xulei test 0->1*/
#else
#define configGENERATE_RUN_TIME_STATS 0 // because use sprintf 
#endif

#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() vInitialiseTimerForRunTimeStats()

#if (SERVICE_TEST_EN == 1u)
extern volatile unsigned long long FreeRTOSRunTimeTicks;
#define portGET_RUN_TIME_COUNTER_VALUE() (FreeRTOSRunTimeTicks)
#else
#define portGET_RUN_TIME_COUNTER_VALUE() 1
#endif


/* The size of the global output buffer that is available for use when there
are multiple command interpreters running at once (for example, one on a UART
and one on TCP/IP).  This is done to prevent an output buffer being defined by
each implementation - which would waste RAM.  In this case, there is only one
command interpreter running. */
#define configCOMMAND_INT_MAX_OUTPUT_SIZE 2096

/* Normal assert() semantics without relying on the provision of an assert.h
header file. */
//void vAssertCalled( const char * pcFile, unsigned long ulLine );
//#define configASSERT( x ) if( ( x ) == 0 ) vAssertCalled( __FILE__, __LINE__ );
#define configASSERT( x )	if( ( x ) == 0 ) { /*taskDISABLE_INTERRUPTS();*/ for( ;; ); }

/* If configTASK_RETURN_ADDRESS is not defined then a task that attempts to
return from its implementing function will end up in a "task exit error"
function - which contains a call to configASSERT().  However this can give GCC
some problems when it tries to unwind the stack, as the exit error function has
nothing to return to.  To avoid this define configTASK_RETURN_ADDRESS to 0.  */
#define configTASK_RETURN_ADDRESS	NULL


/****** Hardware specific settings. *******************************************/

/*
 * The application must provide a function that configures a peripheral to
 * create the FreeRTOS tick interrupt, then define configSETUP_TICK_INTERRUPT()
 * in FreeRTOSConfig.h to call the function.  This file contains a function
 * that is suitable for use on the Zynq MPU.  FreeRTOS_Tick_Handler() must
 * be installed as the peripheral's interrupt handler.
 */
void vConfigureTickInterrupt( void );
#define configSETUP_TICK_INTERRUPT() vConfigureTickInterrupt()

void vClearTickInterrupt( void );
#define configCLEAR_TICK_INTERRUPT() vClearTickInterrupt()

//#define  BSP_INT_GIC_CBUS_DIST_REG    ((ARM_REG_GIC_DIST_PTR)(0x17300000 + 0x1000u))
//#define  BSP_INT_GIC_CBUS_IF_REG      ((ARM_REG_GIC_IF_PTR)(0x17300000 + 0x2000u))
//#define  BSP_INT_GIC_DIST_REG         ((ARM_REG_GIC_DIST_PTR)(0x17F00000 + 0x1000u))
//#define  BSP_INT_GIC_IF_REG           ((ARM_REG_GIC_IF_PTR)(0x17F00000 + 0x2000u))
#define configINTERRUPT_CONTROLLER_BASE_ADDRESS 		(0x1B900000 + 0x1000u) //hskim temp
#define configINTERRUPT_CONTROLLER_CPU_INTERFACE_OFFSET ( 0x1000u )
#define configUNIQUE_INTERRUPT_PRIORITIES				32 // tcc8030 32 supported levels bit[2.0]= 0b000 on GICC_PMR

#define pdMS_TO_TICKS( xTimeInMs ) ( xTimeInMs )

#endif /* FREERTOS_CONFIG_H */

