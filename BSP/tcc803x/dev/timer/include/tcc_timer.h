/****************************************************************************
 *   FileName    : tcc_timer.h
 *   Description : to use the timer for Telechips chipset
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
#ifndef _TCC_TIMER_H_
#define _TCC_TIMER_H_

#ifndef ENABLE
#define ENABLE 1
#endif
#ifndef DISABLE
#define DISABLE 0
#endif

#ifndef NULL
#define NULL	(0)
#endif

#define OS_TIMER_ID 		0
#define PMGPIO_TIMER_CH		1
#define PMDRV_TIMER_CH		2

typedef int32 (*timer_handler_t)(int32, void *);
struct tcc_timer {
	uint32	used;
	uint32	num;
	timer_handler_t	handler;
	void		*args;
};

typedef enum
{
	TCC_TIMER0 = 0,
	TCC_TIMER1,
	TCC_TIMER2,
	TCC_TIMER3,
	TCC_TIMER4,	
	TCC_TIMER5,	
	TCC_TIMER_MAX
} tcc_timer_enum_type;	

void tcc_timer_init(void);
int32 tcc_timer_interrupt_clear(int32 ch);
int32 tcc_timer_enable(int32 ch, uint32 usec, timer_handler_t handler, void *args);
void tcc_timer_disable(int32 ch);
#endif
