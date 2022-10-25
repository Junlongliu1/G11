/****************************************************************************
 *   FileName    : tcc_timer.c
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

#include <types.h>
#include "tcc_ckc.h"
#include "reg_phys.h"
#include "tcc_timer.h"
#include "clock.h"
#include <bsp_int.h>
#include <debug.h>

#define timer_readl		readl
#define timer_writel	writel

#define TMR_OP_EN_CFG 		0x000
#define TMR_MAIN_CNT_LVD 	0x004
#define TMR_CMP_VALUE0 		0x008
#define TMR_CMP_VALUE1 		0x00C
#define TMR_PSCL_CNT 		0x010
#define TMR_MAIN_CNT 		0x014
#define TMR_IRQ_CTRL 		0x018

#define TMR_EN_OP_MODE_FREE_RUN 	0
#define TMR_EN_OP_CNT_EN 			1
#define TMR_EN_OP_LDZERO_ON			1
#define TMR_EN_OP_LDM0_ON			1
#define GPT_TC32_PRESCALE			11

#define TMR_IRQ_CTRL_IRQ_EN0 			(1<<16)
#define TMR_IRQ_CTRL_IRQ_EN1 			(2<<16)
#define TMR_IRQ_CTRL_IRQ_EN2 			(4<<16)
#define TMR_IRQ_CTRL_IRQ_EN3 			(8<<16)

#define TMR_IRQ_CLR_CTRL		 		(1<<31)		/* 0:read, 1:write */
#define TMR_IRQ_MASK			 		0x1F


#define MAX_TCKSEL	6

static void __iomem	*timer_base_reg = NULL;
//static uint32 clk_rate; //QAC
static int32 timer_initialized = 0;
static struct tcc_timer timer_res[TCC_TIMER_MAX];

#define TIMER_CLK_RATE	12000000

static void tcc_timer_handler(void *args);

static void tcc_timer_handler(void *args)
{
	uint32 irq_reg;
	struct tcc_timer *timer = (struct tcc_timer*)args;

	void __iomem *reg = timer_base_reg+(timer->num*0x100);
	irq_reg = timer_readl(reg+TMR_IRQ_CTRL);

	if ((irq_reg&0x001f0000) != 0) {
		if (timer->used != 0) {
			timer_writel((irq_reg|TMR_IRQ_MASK), reg+TMR_IRQ_CTRL); //clear irq

			if (timer->handler != 0) {
				(void)timer->handler(timer->num, timer->args);
			}
		}
	}
}
int32 tcc_timer_interrupt_clear(int32 ch)
{
	uint32 clr_ctl;
	void __iomem *reg = timer_base_reg+(ch*0x100);

	clr_ctl = timer_readl(reg+TMR_IRQ_CTRL);

	if ((clr_ctl & 0x80000000) != 0) {
		timer_writel(clr_ctl|TMR_IRQ_MASK, reg+TMR_IRQ_CTRL);
	}
	return 0;
}

/*
 * usec: timer tick vaule.  if usec value is 1000 then timer counter tick is 1 msec.
 * handler
 */
int32 tcc_timer_enable(int32 ch, uint32 usec, timer_handler_t handler, void *args)
{
	uint32 rate, set_rate, clk_id, Tck, cmpval0;

	void __iomem *reg = timer_base_reg+(ch*0x100);
	uint32 reg_val;

	if ( usec < 1000) {
		rate = TIMER_CLK_RATE*2;
	} else {
		rate = TIMER_CLK_RATE;
	}
	Tck = GPT_TC32_PRESCALE + 1;

	switch (ch)
	{
		case TCC_TIMER0:
			clk_id = PERI_TIMER0;
			break;
		case TCC_TIMER1:
			clk_id = PERI_TIMER1;
			break;
		case TCC_TIMER2:
			clk_id = PERI_TIMER2;
			break;
		case TCC_TIMER3:
			clk_id = PERI_TIMER3;
			break;
		case TCC_TIMER4:
			clk_id = PERI_TIMER4;
			break;
		case TCC_TIMER5:
			clk_id = PERI_TIMER5;
			break;
		default:
			// TODO: set error id
			//printf("timer id is wrong!\n");
			return -1;	// removed codesonar warning
	}
	(void)tcc_set_peri(clk_id, ENABLE, rate);
	set_rate = tcc_get_peri(clk_id); // MHz
	
	cmpval0 = (usec * ((set_rate/1000)  / (Tck * 1000))) - 1; 
//	cmpval0 = 60; //todo check why? i want to 1ms
	if (!timer_initialized)	{
		return -1;
	}
	//reset main cnt load value
	timer_writel(0xffffffff, reg+TMR_MAIN_CNT_LVD);

	timer_writel(cmpval0, reg+TMR_CMP_VALUE0);
	timer_writel(0x00000000, reg+TMR_CMP_VALUE1);

	reg_val = 0;
	//reg_val |= (TMR_EN_OP_MODE_FREE_RUN<<26); // removed codesonar warning
	reg_val |= (GPT_TC32_PRESCALE<<0);
	reg_val |= (TMR_EN_OP_CNT_EN<<24);
	// set mode 5
	reg_val |= (TMR_EN_OP_LDZERO_ON<<25);
	reg_val |= (TMR_EN_OP_LDM0_ON<<28);

	timer_writel(reg_val, reg+TMR_OP_EN_CFG);

	reg_val = timer_readl(reg+TMR_IRQ_CTRL)|(TMR_IRQ_CTRL_IRQ_EN0);
	timer_writel(reg_val, reg+TMR_IRQ_CTRL);

	timer_res[ch].used = 1;
	timer_res[ch].handler = handler;
	timer_res[ch].args = args;

	if (ch != 0) { // timer 0 specially used by os timer
		(void)BSP_IntVectSet(INT_TIMER_0 +ch,
		PRIORITY_NO_MEAN,
		INT_TYPE_LEVEL_HIGH,
		tcc_timer_handler,
		(void*)&timer_res[ch]);
	}
	return 0;
}

void tcc_timer_disable(int32 ch)
{
	void __iomem *reg = timer_base_reg+(ch*0x100);
	uint32 clr_ctl;

	if (!timer_initialized)	{
		return;
	}

	clr_ctl = timer_readl(reg+TMR_IRQ_CTRL);

	timer_writel((clr_ctl & (~(1<<24))), reg+TMR_OP_EN_CFG);

	clr_ctl = 0;
	//clr_ctl &= ~(0x001f0000);	// removed codesonar warning
	timer_writel(clr_ctl, reg+TMR_IRQ_CTRL);

	timer_res[ch].used = 0;
	timer_res[ch].handler = NULL;
	timer_res[ch].args = NULL;
}

void tcc_timer_init(void)
{
	//static uint32 clk_rate; //warning. set but not used
	uint32 i;
	timer_base_reg	= (void __iomem *)TCC_TIMER_BASE;

	// TODO: check after ckc working
	//(void)tcc_set_peri(PERI_MICOM_TIMER0, ENABLE, TIMER_CLK_RATE);
	//clk_rate = tcc_get_peri(PERI_MICOM_TIMER0);
	(void)tcc_set_peri(PERI_TIMER0, ENABLE, TIMER_CLK_RATE);
	//clk_rate = tcc_get_peri(PERI_TIMER0);

	for (i=0 ; i<TCC_TIMER_MAX ; i++) {
		void __iomem *reg = timer_base_reg+(i*0x100);
		timer_res[i].used = 0;
		timer_res[i].num = i;
		timer_res[i].handler = NULL;
		timer_res[i].args = NULL;

		timer_writel(0x7FFF, reg+TMR_OP_EN_CFG);
		timer_writel(0x0, reg+TMR_MAIN_CNT_LVD);
		timer_writel(0x0, reg+TMR_CMP_VALUE0);
		timer_writel(0x0, reg+TMR_CMP_VALUE1);

		timer_writel(TMR_IRQ_CLR_CTRL|TMR_IRQ_MASK, reg+TMR_IRQ_CTRL);
	}

	timer_initialized = 1;
}
