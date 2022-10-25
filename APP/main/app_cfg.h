/****************************************************************************
 *   FileName    : app_cfg.h
 *   Description : configuration of micom application
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
#ifndef  _APP_CFG_H_
#define  _APP_CFG_H_



/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*                             Size of the task stacks (# of WARD entries)
*********************************************************************************************************
*/
#define TEST_FLGPTN                (0x01U)
#define TEST_FLGPTN_WDTIRQ         (0x01U)

/*
*********************************************************************************************************
*                                    SYSTEM SERVICE CONFIGURE:
*********************************************************************************************************
*/
#define SERVICE_CONSOLE_EN		0u  //for console service
#define SERVICE_TEST_EN			0u  //for TEST service

/*
*********************************************************************************************************
*                                     SYSTEM FUNCTIONS
*********************************************************************************************************
*/
#if (SERVICE_TEST_EN == 1u)
#define PF_TEST_START   gpio_set(TCC_GPA(02), 1)
#define PF_TEST_STOP	gpio_set(TCC_GPA(02), 0)
#endif

#endif
