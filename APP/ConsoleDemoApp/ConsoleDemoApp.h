/****************************************************************************
 *   FileName    : ConsoleDemoApp.h
 *   Description : Demo Application for console function
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


#ifndef  _CONSOLE_DEMO_APP_H_
#define  _CONSOLE_DEMO_APP_H_

/*
*********************************************************************************************************
*                                                 EXTERNS
*********************************************************************************************************
*/

#define TASK_STA_TITLE "\r\nname                            ticks           percent\r\n"
#define TASK_LIST_TITLE "\r\nname                        state   priority    stack   num\r\n"


#define	EEPROM_LEN	2
#ifdef   APP_MODULE
#define  APP_EXT
#else
#define  APP_EXT  extern
#endif

APP_EXT void ConsoleDemoTask(void *p_arg);
#if (SERVICE_TEST_EN == 1u)
extern void CYC_TEST(void);
#endif


#endif

