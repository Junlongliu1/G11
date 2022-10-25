/****************************************************************************
 *   FileName    : bsp_os_timer.c
 *   Description :
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

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include  <types.h>
#include <osal.h>
#include  <bsp.h>
#include  <bsp_os.h>
#include  <bsp_int.h>
#include  <tcc_ckc.h>
#include  <clock.h>
#include  <bsp_os_timer.h>
#include  <tcc_timer.h>

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/
static uint64 system_main_tick_cnt = 26; //(BOOT_CHECK_ELAPSED_TIME) + (18)


/*
*********************************************************************************************************
*                                     LOCAL & EXTERN FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static void  BSP_OS_TmrTickHandler (void *arg);

/*
*********************************************************************************************************
*********************************************************************************************************
**                                         GLOBAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                       BSP_OS_TmrTickHandler()
*
* Description : Interrupt handler for the tick timer
*
* Argument(s) : data
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static void  BSP_OS_TmrTickHandler (void *arg)
{

	OS_TIME_TICK_HANDLER();
	#ifdef OS_UCOS
	(void)tcc_timer_interrupt_clear(OS_TIMER_ID);
	#endif
	system_main_tick_cnt ++;		
}

/*
 *********************************************************************************************************
 *                                            BSP_OS_TmrTickInit()
 *
 * Description : Initialize uC/OS-III's tick source
 *
 * Argument(s) : tick_rate              Number of ticks per second.
 *
 * Return(s)   : none.
 *
 * Caller(s)   : Application.
 *
 * Note(s)     : none.
 *********************************************************************************************************
 */

void  BSP_OS_TmrTickInit (uint32 tick_rate)
{
	(void)BSP_IntVectSet(INT_TIMER_0 +OS_TIMER_ID,
						0xau,
						INT_TYPE_LEVEL_HIGH,
						BSP_OS_TmrTickHandler,
						BSP_NULL);
	BSP_IntSrcEn(INT_TIMER_0 + OS_TIMER_ID);
	(void)tcc_timer_enable(OS_TIMER_ID,tick_rate, 0, 0);

	BSP_IntPrioSet(PPI_VIRT_TIMER, PRIORITY_NO_MEAN-2);
	BSP_IntSrcEn(PPI_VIRT_TIMER);
	CPU_MB();
}

/*
 *********************************************************************************************************
 *                                            BSP_OS_TmrIntClear()
 *
 * Description : 
 *
 * Argument(s) : tick_rate              Number of ticks per second.
 *
 * Return(s)   : none.
 *
 * Caller(s)   : Application.
 *
 * Note(s)     : none.
 *********************************************************************************************************
 */

void  BSP_OS_TmrIntClear(void)
{
	(void)tcc_timer_interrupt_clear(OS_TIMER_ID);
}

uint64  BSP_OS_GetTmrTickCnt (void)
{
	return system_main_tick_cnt;
}
