/****************************************************************************
 *   FileName    : bsp_os.c
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

#include <types.h>
#include <osal.h>
#include  <bsp.h>
#include  <bsp_int.h>
#include  <bsp_os.h>


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

/* ARM exception IDs                                      */
#define  BSP_ARM_EXCEPT_RESET                      0x00u
#define  BSP_ARM_EXCEPT_UNDEF_INSTR                0x01u
#define  BSP_ARM_EXCEPT_SWI                        0x02u
#define  BSP_ARM_EXCEPT_PREFETCH_ABORT             0x03u
#define  BSP_ARM_EXCEPT_DATA_ABORT                 0x04u
#define  BSP_ARM_EXCEPT_ADDR_ABORT                 0x05u
#define  BSP_ARM_EXCEPT_IRQ                        0x06u
#define  BSP_ARM_EXCEPT_FIQ                        0x07u
#define  BSP_ARM_EXCEPT_NBR                        0x08u

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


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*********************************************************************************************************
**                                         GLOBAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/


/*
 *********************************************************************************************************
 *                                          OS_CPU_ExceptHndlr()
 *
 * Description : Handle any exceptions.
 *
 * Argument(s) : except_id     ARM exception type:
 *
 *                                  OS_CPU_ARM_EXCEPT_RESET             0x00
 *                                  OS_CPU_ARM_EXCEPT_UNDEF_INSTR       0x01
 *                                  OS_CPU_ARM_EXCEPT_SWI               0x02
 *                                  OS_CPU_ARM_EXCEPT_PREFETCH_ABORT    0x03
 *                                  OS_CPU_ARM_EXCEPT_DATA_ABORT        0x04
 *                                  OS_CPU_ARM_EXCEPT_ADDR_ABORT        0x05
 *                                  OS_CPU_ARM_EXCEPT_IRQ               0x06
 *                                  OS_CPU_ARM_EXCEPT_FIQ               0x07
 *
 * Return(s)   : none.
 *
 * Caller(s)   : OS_CPU_ARM_EXCEPT_HANDLER(), which is declared in os_cpu_a.s.
 *
 * Note(s)     : (1) Only OS_CPU_ARM_EXCEPT_FIQ and OS_CPU_ARM_EXCEPT_IRQ exceptions handler are implemented.
 *                   For the rest of the exception a infinite loop is implemented for debuging pruposes. This behavior
 *                   should be replaced with another behavior (reboot, etc).
 *********************************************************************************************************
 */

void  OS_CPU_ExceptHndlr (uint32 except_id)
{
    switch (except_id) {
        case BSP_ARM_EXCEPT_FIQ:
             BSP_IntHandler();
             break;


        case BSP_ARM_EXCEPT_IRQ:
             BSP_IntHandler();
             break;


        case BSP_ARM_EXCEPT_RESET:
             /* $$$$ Insert code to handle a Reset exception               */

        case BSP_ARM_EXCEPT_UNDEF_INSTR:
             /* $$$$ Insert code to handle a Undefine Instruction exception */

        case BSP_ARM_EXCEPT_SWI:
             /* $$$$ Insert code to handle a Software exception             */

        case BSP_ARM_EXCEPT_PREFETCH_ABORT:
             /* $$$$ Insert code to handle a Prefetch Abort exception       */

        case BSP_ARM_EXCEPT_DATA_ABORT:
             /* $$$$ Insert code to handle a Data Abort exception           */

        case BSP_ARM_EXCEPT_ADDR_ABORT:
             /* $$$$ Insert code to handle a Address Abort exception        */
        default:

            while (1) { /* Infinite loop on other exceptions. (see note #1)          */
                CPU_WaitForEvent();
            }
    }
}
