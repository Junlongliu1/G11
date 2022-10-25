/****************************************************************************
 *   FileName    : pwm.c
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
#include <types.h>
#include "reg_phys.h"
#include "tcc_ckc.h"
#include "clock.h"
#include <pwm.h>

#define PWM_TOTAL_MODULES			(3)    // The PWM controller consist of 3 PWM modules (PWM-0/1/2).
#define PWM_TOTAL_CH_PER_MODULE		(4)    // Each PWM module has four separate PWM blocks (PWM-A/B/C/D).
#define PWM_TOTAL_CHANNELS			(PWM_TOTAL_MODULES * PWM_TOTAL_CH_PER_MODULE)

#define CONV_MODULE_ID(CH)			((CH) / PWM_TOTAL_CH_PER_MODULE)
#define CONV_MODULE_CH(CH)			(uint32)((CH) % PWM_TOTAL_CH_PER_MODULE)

#define PWM_MODULE_REG_BASE(CH)		(TCC_PWM_MODULEx_BASE(CONV_MODULE_ID(CH)))
#define PWM_REG_BASE(CH)			(PWM_MODULE_REG_BASE(CH))

//// IO bus register Offset
//#define OFFSET_PWM		(0x4C)

// PWM Register Offset
#define OFFSET_ST			(0x00)				// PWM block status
#define OFFSET_EN			(0x04)				// PWM enable & register update end & Trigger
#define OFFSET_MODE			(0x08)				// PWM output mode & PWMCLKDiv
#define OFFSET_LOOP			(0x0C)				// PWM block operation steps
#define PWMPSTN1(X)         (0x10 + (0x10*(X)))
#define PWMPSTN2(X)         (0x14 + (0x10*(X)))
#define PWMPSTN3(X)         (0x18 + (0x10*(X)))
#define PWMPSTN4(X)         (0x1C + (0x10*(X)))

#define OFFSET_OutReg1(X)		(0x50 + (0x10* (X)))	// PWM X1 block Reg mode Output value
#define OFFSET_OutReg2(X)		(0x54 + (0x10* (X)))	// PWM X2 block Reg mode Output value
#define OFFSET_OutReg3(X)		(0x58 + (0x10* (X)))	// PWM X3 block Reg mode Output value
#define OFFSET_OutReg4(X)		(0x5c + (0x10* (X)))	// PWM X4 block Reg mode Output value
#define OFFSET_CntMax			(0x90)		// PWM Maximum Count value of Register Out Mode3

#define PHASE_MODE  		(1)
#define REGISTER_OUT_MODE  	(2)

#define PWM_DIVID_MAX	3 			// clock divide max value 3(divide 16)

#define PWM_CLOCK		(25* 1000*1000)		// 25Mhz

#define PWM_TOTAL_NUM	(PWM_TOTAL_CHANNELS)

#define pwm_writel	writel
#define pwm_readl	readl

static void pwm_wait(uint32 channel);
static void pwm_register_mode_set(uint32 channel, uint32 regist_value);

void pwm_enable(uint32 channel)
{
	uint32 reg_base = 0;
	uint32 reg = 0;

	reg_base = pwm_readl(PWM_REG_BASE(channel) + OFFSET_EN);

	reg = reg_base | ((uint32)0x00010<<(CONV_MODULE_CH(channel)));
	pwm_writel(reg, PWM_REG_BASE(channel) + OFFSET_EN);

	reg_base = pwm_readl(PWM_REG_BASE(channel) + OFFSET_EN);
	reg = reg_base | ((uint32)0x00011<<(CONV_MODULE_CH(channel)));
	pwm_writel(reg, PWM_REG_BASE(channel) + OFFSET_EN);

	reg_base = pwm_readl(PWM_REG_BASE(channel) + OFFSET_EN);
	reg = reg_base | ((uint32)0x10011<<(CONV_MODULE_CH(channel)));
	pwm_writel(reg, PWM_REG_BASE(channel) + OFFSET_EN);

	//pwm_writel(0x000, (TCC_IOBUSCFG_BASE + OFFSET_PWM));
}


void pwm_disable(uint32 channel)
{
	uint32 reg_base = 0;
	uint32 reg = 0;

	reg_base = pwm_readl(PWM_REG_BASE(channel) + OFFSET_EN);
	reg = reg_base & (~((uint32)0x01<<(CONV_MODULE_CH(channel))));
	pwm_writel(reg, PWM_REG_BASE(channel)+OFFSET_EN);

	reg_base = pwm_readl(PWM_REG_BASE(channel) + OFFSET_EN);
	reg = reg_base | ((uint32)0x10010<<(CONV_MODULE_CH(channel)));
	pwm_writel(reg, PWM_REG_BASE(channel)+OFFSET_EN);
}

static void pwm_wait(uint32 channel)
{
	volatile uint32 delay_cnt  = 0x3FFFF;
	volatile uint32 busy = 0;
	while(delay_cnt>0) //QAC
	{	
		delay_cnt --;
		busy = pwm_readl(PWM_REG_BASE(channel) + OFFSET_ST);
		//if(!(busy  & (0x1 << CONV_MODULE_CH(channel))))
		if(((busy)  & (uint32)(0x1 << CONV_MODULE_CH(channel)))==0)
		{
			break;
		}
	}
}

static void pwm_register_mode_set(uint32 channel, uint32 regist_value)
{
	//volatile uint32 reg = 0, bit_shift = 0; //QAC
	uint32 reg_base = 0;
	uint32 reg = 0;
	uint32 bit_shift = 0;


	bit_shift = 4 * CONV_MODULE_CH(channel);
	reg_base = pwm_readl(PWM_REG_BASE(channel)+OFFSET_MODE);
	
	if(((reg_base >> bit_shift) & 0xF) != REGISTER_OUT_MODE) {
		pwm_disable(channel);
		pwm_wait(channel);
	}
	
         reg = (uint32)(reg_base& ~(0xF << bit_shift)) | (uint32)(REGISTER_OUT_MODE << bit_shift);
         pwm_writel(reg, PWM_REG_BASE(channel)+OFFSET_MODE);  //phase mode
 
         reg_base = pwm_readl(PWM_REG_BASE(channel)+OFFSET_MODE);
         bit_shift = (2 * CONV_MODULE_CH(channel)) + 24;
         reg = (reg_base & ~(0x3 << bit_shift));		//divide by 2 : default value
         pwm_writel(reg, PWM_REG_BASE(channel)+OFFSET_MODE);
         
         pwm_writel(regist_value, PWM_REG_BASE(channel) + OFFSET_OutReg1(CONV_MODULE_CH(channel)));
}

int32 pwm_config(uint32 channel, uint32 duty_ns, uint32 period_ns)
{
	uint32 reg_base = 0;
	uint32 reg = 0;//volatile uint32 reg = 0; //QAC
	uint32 k = 0, bit_shift = 0;
	uint32 clk_freq, clk_period_ns;
	uint32 total_cnt = 0, hi_cnt = 0, low_cnt = 0, divide = 0;
	uint32 cal_duty = 0, cal_period = 0;
	uint32 level_hi = 0, level_low = 0;
	
	clk_freq = tcc_get_peri(PERI_PWM0);

	if((clk_freq == 0) || (duty_ns > period_ns))
	{
		return -1;
	}

	clk_period_ns = (1000 * 1000 *1000) /  clk_freq;

	if(duty_ns == 0)
	{
		level_low = 1;
		//goto pwm_low_out;
	}
	else if(duty_ns == period_ns)
	{
		level_hi = 1;
		//goto pwm_hi_out;
	}
	else
	{
		while(1)
		{
			clk_period_ns = clk_period_ns * (2);
			total_cnt = (period_ns / clk_period_ns);
			if(total_cnt >= 4)
			{ // QAC
				total_cnt = total_cnt -4;
			}
			else
			{
				total_cnt = 0;
			}
			
			if((total_cnt <= 1) || (k == PWM_DIVID_MAX) || (total_cnt <= 0xFFFF))
			{
				if(total_cnt <= 1)
				{
					if(duty_ns > (period_ns/2)) 
					{
						level_hi = 1;
					}
					else
					{
						level_low = 1;
					}
				}
				break;
			}
			k++;
		}
	}

			
	if(level_hi != 0) //QAC
	{
		pwm_register_mode_set(channel, 0xFFFFFFFF);
		pwm_enable(channel);
	}
	else if(level_low != 0)//QAC
	{
		pwm_register_mode_set(channel, 0x00000000);
		pwm_enable(channel);
	}
	else
	{
		//prevent over flow.
		for(divide = 1; divide < 0xFFFF; divide++)
		{
			// 0xFFFFFFFF > total_cnt * duty / divide		
			if((0xFFFFFFFF/duty_ns) > (total_cnt/divide))
			{
				break;
			}
		}
		cal_duty = duty_ns / divide;
		cal_period = period_ns / divide;
			
		hi_cnt = (total_cnt * (cal_duty ))  / (cal_period);
		low_cnt = total_cnt - hi_cnt;
		

	//pwm_result: //warning. defined but not used
		reg_base = pwm_readl(PWM_REG_BASE(channel)+OFFSET_MODE);

		bit_shift = 4 * CONV_MODULE_CH(channel);
		if(((reg_base >> bit_shift) & 0xF) != PHASE_MODE) {
			pwm_disable(channel);
			pwm_wait(channel);
		}
		
		reg = (reg_base & ~(0xF << bit_shift)) | (PHASE_MODE << bit_shift);  //phase mode
		pwm_writel(reg, PWM_REG_BASE(channel)+OFFSET_MODE);  //phase mode

		bit_shift = (2 * CONV_MODULE_CH(channel)) + 24;
		reg = ((reg & ~(0x3 << bit_shift)) | (k  << bit_shift));
		pwm_writel(reg, PWM_REG_BASE(channel)+OFFSET_MODE);  //divide

		pwm_writel(low_cnt, PWM_REG_BASE(channel) + PWMPSTN1(CONV_MODULE_CH(channel))); //low count setting
		pwm_writel(hi_cnt,  PWM_REG_BASE(channel) + PWMPSTN2(CONV_MODULE_CH(channel))); //hight count setting

		reg_base = pwm_readl(PWM_REG_BASE(channel)+OFFSET_MODE);
		bit_shift = (1 << CONV_MODULE_CH(channel)) + 16;
		reg = (reg_base & ~(0x1 << bit_shift)) | (0x0 << bit_shift);//signal inverse clear
		pwm_writel(reg, PWM_REG_BASE(channel)+OFFSET_MODE);  
		pwm_enable(channel);
	}
	
	return 0;

}

void set_pwm_port(uint32 channel, uint32 pwm_port_num)
{
	volatile uint32 reg = TCC_PWM_PORTCFG_BASE + 0x10 + (CONV_MODULE_ID(channel) * 4);
	uint32 val;
	val = pwm_readl(reg);
	val &= (~((uint32)(0xFF) << (CONV_MODULE_CH(channel) * 8)));
	val |= (pwm_port_num << (CONV_MODULE_CH(channel) * 8));
	pwm_writel(val, reg);
}

void init_pwm(void)
{
	(void)tcc_set_peri(PERI_PWM0, ENABLE, PWM_CLOCK);
	(void)tcc_set_peri(PERI_PWM1, ENABLE, PWM_CLOCK);
	(void)tcc_set_peri(PERI_PWM2, ENABLE, PWM_CLOCK);
}

void deinit_pwm(void)
{
	(void)tcc_set_peri(PERI_PWM0, DISABLE, PWM_CLOCK);
	(void)tcc_set_peri(PERI_PWM1, DISABLE, PWM_CLOCK);
	(void)tcc_set_peri(PERI_PWM2, DISABLE, PWM_CLOCK);
}
