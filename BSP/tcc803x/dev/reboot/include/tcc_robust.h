/****************************************************************************
 *   FileName    : tcc_robust.h
 *   Description : to use the watchdog for Telechips chipset
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
#ifndef _TCC_ROBUST_H_
#define _TCC_ROBUST_H_

//void robust_a7s_reboot(uint32 appid);
//void robust_a53_reboot(uint32 appid);
int32 tcc_robust_ca53_watchdog_init(void);
int32 tcc_robust_ca7s_watchdog_init(void);
//void a7s_wakeup(void);
#endif
