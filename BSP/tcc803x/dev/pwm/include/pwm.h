/****************************************************************************
 *   FileName    : pwm.h
 *   Description : header file of pwm driver
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
#ifndef _PWM_H_
#define _PWM_H_

extern void pwm_enable(uint32 channel);
extern void pwm_disable(uint32 channel);
extern int32 pwm_config(uint32 channel, uint32 duty_ns, uint32 period_ns);
extern void init_pwm(void);
extern void deinit_pwm(void);
extern void set_pwm_port(uint32 channel, uint32 pwm_port_num);
#endif
