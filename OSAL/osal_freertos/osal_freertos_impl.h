/****************************************************************************
 *   FileName    : osal_freertos_impl.h
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
#ifndef _OSAL_FREERTOS_IMPL_H_
#define _OSAL_FREERTOS_IMPL_H_
#define  OSAL_CPU_WORD_SIZE_32                          4u   /* 32-bit word size (in octets).                                */
#define  OSAL_DEF_OCTET_NBR_BITS                                8u
#define  OSAL_DEF_INT_32U_MAX_VAL                      4294967295u
#define  OSAL_DEF_BIT(bit)                                                   (1u << (bit))

#define  OSAL_DEF_INT_CPU_NBR_BITS                           (OSAL_CPU_WORD_SIZE_32     * OSAL_DEF_OCTET_NBR_BITS)
#define  OSAL_BIT_FIELD(bit_field, bit_shift)                                 ((((bit_field) >= OSAL_DEF_INT_CPU_NBR_BITS) ? (OSAL_DEF_INT_32U_MAX_VAL)     \
                                                                                                                     : (OSAL_DEF_BIT(bit_field) - 1uL)) \
                                                                                                                            << (bit_shift))
extern void        CPU_WaitForEvent (void); //QAC 17.3 : No function declearatin.
extern void __DSB(void);//QAC 17.3 : No function declearatin.
extern uint32 CPU_SR_Save(void); //QAC 17.3 : No function declearatin.
extern void CPU_SR_Restore(uint32); //QAC 17.3 : No function declearatin.

#define  OSAL_SR_ALLOC()             uint32  cpu_sr = (uint32)0

#define  OSAL_INT_DIS()         do { cpu_sr = CPU_SR_Save(); } while (0) /* Save    CPU status word & disable interrupts.*/
#define  OSAL_INT_EN()          do { CPU_SR_Restore(cpu_sr); } while (0) /* Restore CPU status word.                     */


#define  OSAL_CRITICAL_ENTER()  do { OSAL_INT_DIS(); } while (0)          /* Disable   interrupts.                        */
#define  OSAL_CRITICAL_EXIT()   do { OSAL_INT_EN();  } while (0)          /* Re-enable interrupts.                        */


#define  CPU_MB()       __DSB()
#define  CPU_RMB()      __DSB()
#define  CPU_WMB()      __DSB()

#define OS_CMP_EQUAL				0						/*!< Compare Result is Equal	*/
#define OS_CMP_INVALID				(-1)					         /*!< Invalid Compare			*/


void  OSTimeTick (void);//QAC 17.3 : No function declearatin.

#define OS_TIME_TICK_HANDLER 	OSTimeTick

#define  OS_CFG_TICK_RATE_HZ            1000u               /* Tick rate in Hertz (10 to 1000 Hz)                     */

#endif
