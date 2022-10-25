/****************************************************************************
 *   FileName    : debug.h
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

#ifndef	_DEBUG_H__
#define _DEBUG_H__ 

#define	DEBUG_ENABLE	1

extern void IO_DBG_SerialPrintf_(int8 *format, ...);
#define	printf	IO_DBG_SerialPrintf_ 

#ifndef __FILENAME__
#define __FILENAME__ __FILE__
#endif/*__FILENAME__*/

#define TCC_PR_ERR
#define TCC_PR_INFO
//#define TCC_PR_WARN
//#define TCC_PR_DBG

#define tcc_printf(fmt,...)	\
	printf("[%s] "fmt"\n", __FUNCTION__, ##__VA_ARGS__)
	//printf("%s:%d	"fmt"\n", __FILENAME__, __LINE__, ##__VA_ARGS__)

#ifdef TCC_PR_ERR
#define tcc_pr_err	tcc_printf
#else
#define tcc_pr_err(fmt,...)	\
	do{}while(0)
#endif /* FEATURE_TCC_MSG_ERROR */

#ifdef TCC_PR_INFO
#define tcc_pr_info	tcc_printf
#else
#define tcc_pr_info(fmt,...)	\
	do{}while(0)
#endif /* TCC_PR_INFO */

#ifdef TCC_PR_WARN
#define tcc_pr_warn	tcc_printf
#else
#define tcc_pr_warn(fmt,...)	\
	do{}while(0)
#endif /* TCC_PR_WARN */

#ifdef TCC_PR_DBG
#define tcc_pr_dbg	tcc_printf
#else
#define tcc_pr_dbg(fmt,...)	\
	do{}while(0)
#endif /* TCC_PR_DBG */

#endif
