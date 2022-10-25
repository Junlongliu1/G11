/****************************************************************************
 *   FileName    : gpio.c
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
#include <gpio.h>
#include <reg_phys.h>
#include <bsp.h>
#include "debug.h"
#include <app_cfg.h>


#ifndef ENABLE
#define ENABLE 1
#endif
#ifndef DISABLE
#define DISABLE 0
#endif


static const uint32 nstGPIOTBL[GPIO_PORT_TYPE] = {TCC_GPA(1),TCC_GPB(1),TCC_GPC(8),
													TCC_GPE(1),TCC_GPG(1),TCC_GPH(6),
													TCC_GPK(3),TCC_GPMA(3)};

#define reg_base(x)	(TCC_GPIO_BASE + ((((x)&GPIO_PORT_MASK)>>GPIO_PORT_SHIFT)*0x40))

#define gpioa_sel_base (TCC_PMU_BASE_MICOM + TCC_PMU_GPIOA_SEL_OFFSET)
#define gpiob_sel_base (TCC_PMU_BASE_MICOM + TCC_PMU_GPIOB_SEL_OFFSET)
#define gpioc_sel_base (TCC_PMU_BASE_MICOM + TCC_PMU_GPIOC_SEL_OFFSET)
#define gpioe_sel_base (TCC_PMU_BASE_MICOM + TCC_PMU_GPIOE_SEL_OFFSET)
#define gpiog_sel_base (TCC_PMU_BASE_MICOM + TCC_PMU_GPIOG_SEL_OFFSET)
#define gpioh_sel_base (TCC_PMU_BASE_MICOM + TCC_PMU_GPIOH_SEL_OFFSET)
#define gpioma_sel_base (TCC_PMU_BASE_MICOM + TCC_PMU_GPIOMA_SEL_OFFSET)

#define is_gpiok(x)	((((x)&GPIO_PORT_MASK) == GPIO_PORT_K) ? 1 : 0)

#define reg_data(x)	(reg_base(x) + 0x00)
#define reg_outen(x)	(reg_base(x) + 0x04)
#define reg_data_or(x)	(reg_base(x) + 0x08)
#define reg_data_bic(x)	(reg_base(x) + 0x0C)
#define reg_pullen(x)	(is_gpiok(x) ? (TCC_PMGPIO_BASE + 0x10) : (reg_base(x) + 0x1C))
#define reg_pullsel(x)	(is_gpiok(x) ? (TCC_PMGPIO_BASE + 0x14) : (reg_base(x) + 0x20))
#define reg_cd(x,pin)	((is_gpiok(x) ? (TCC_PMGPIO_BASE + 0x18) : (reg_base(x) + 0x14)) + (0x4*((pin)/16)))
#define reg_ien(x)	(is_gpiok(x) ? (TCC_PMGPIO_BASE + 0x0C) :( reg_base(x) + 0x24))
#define reg_is(x)	(reg_base(x) + 0x28)
#define reg_sr(x)	(reg_base(x) + 0x2C)
#define reg_fn(x,pin)	((reg_base(x) + 0x30) + (0x4*((pin)/8)))


#define PMGPIO_SEL	(TCC_PMGPIO_BASE + 0x8)

#if (SERVICE_TEST_EN == 1u)
#define GPIO_SCAN_DEBUG
#endif

#ifdef GPIO_SCAN_DEBUG
static const int8 scan_list[GPIO_LIST_NUM][GPIO_MAX_STRING] =
        { "DATA",
          "OUTPUT ENABLE",
          "INPUT ENABLE",
          "PULL UP/DOWN",
          "DRIVE STRENGH",
          "SCHMITT"
        };

static int32 scan_offset[] = { 0x0, 0x04, 0x24, 0x1C, 0x14, 0x28 };
#endif

/*
 * gpio_config() function descripiton.
 * Parameters
 *  - port  : gpio index
 *  - config: default setting values are below when config is 0.
 *            * slew rate          : fast (gpio_k is not support)
 *            * input type         : cmos (gpio_k is not support)
 *            * input buffer       : no change
 *            * direction          : input
 *            * driver strength    : no change
 *            * pull up/down       : no pull (pull disable)
 *            * function selection : normal gpio
 */

static void gpio_select(uint32 port, uint32 flag);
int32 gpio_config(uint32 port, uint32 config)
{
	uint32 pin = port&GPIO_PIN_MASK;
	uint32 bit = 1<<pin;
	uint32 func = config & GPIO_FUNC_MASK;
	uint32 pull = config & (GPIO_PULL_MASK<<GPIO_PULL_SHIFT);
	uint32 cd = config & (GPIO_CD_MASK<<GPIO_CD_SHIFT);
	uint32 ien = config & (GPIO_INPUTBUF_MASK<<GPIO_INPUTBUF_SHIFT);
	uint32 base_val;
	uint32 comp_val, set_val;

	gpio_select(port, DISABLE); // gpio select change to micom

	/* function */
	base_val = readl(reg_fn(port,pin)) & (~(0xF<<((pin%8)*4)));
	set_val = base_val | (func<<((pin%8)*4));
	writel(set_val, reg_fn(port,pin));
	/* configuration check */
	comp_val = readl(reg_fn(port,pin));
	if(comp_val != set_val)
	{
		return BSP_ERR_INIT;
	}

	/* pull-up/down */
	if (pull == GPIO_PULLUP) {
		base_val = readl(reg_pullen(port));
		set_val = (base_val | bit);
		writel(set_val, reg_pullen(port));
		comp_val = readl(reg_pullen(port));
		if(comp_val != set_val)
		{
			return BSP_ERR_INIT;
		}

		base_val = readl(reg_pullsel(port));
		set_val = base_val | bit;
		writel(set_val, reg_pullsel(port));
		comp_val = readl(reg_pullsel(port));
	    if(comp_val != set_val)
	    {
	      	return BSP_ERR_INIT;
	    }
	}
	else if (pull == GPIO_PULLDN) {
		base_val = readl(reg_pullen(port));
		set_val = base_val | bit;
		writel(set_val, reg_pullen(port));
		comp_val = readl(reg_pullen(port));
		if(comp_val != set_val)
		{
			return BSP_ERR_INIT;
		}

		base_val = readl(reg_pullsel(port));
		set_val = base_val & ~bit;
		writel(set_val, reg_pullsel(port));
        comp_val = readl(reg_pullsel(port));
        if(comp_val != set_val)
		{
			return BSP_ERR_INIT;
		}
	}
	else {
		base_val = readl(reg_pullen(port));
		set_val = base_val & ~bit;
		/* set pull disable. */
		writel(set_val, reg_pullen(port));
        comp_val = readl(reg_pullen(port));
        if(comp_val != set_val)
		{
			return BSP_ERR_INIT;
		}
	}
	/* drive strength */
	if (cd !=0) {
		cd = cd >> GPIO_CD_SHIFT;
		base_val = readl(reg_cd(port,pin)) & ~(3<<((pin%16)*2));
		set_val = base_val | ((cd&0x3)<<((pin%16)*2));
		writel(set_val, reg_cd(port,pin));
        comp_val = readl(reg_cd(port,pin));
        if(comp_val != set_val)
		{
			return BSP_ERR_INIT;
		}
	}
	/* direction */
	if ((config&GPIO_OUTPUT)!=0) {
		base_val = readl(reg_outen(port));
		set_val = base_val | bit;
		writel(set_val, reg_outen(port));
		comp_val = readl(reg_outen(port));
		if(comp_val != set_val)
		{
			return BSP_ERR_INIT;
		}
	}
	else {
		base_val = readl(reg_outen(port));
		set_val = base_val & ~bit;
		writel(set_val, reg_outen(port));
        comp_val = readl(reg_outen(port));
        if(comp_val != set_val)
		{
			return BSP_ERR_INIT;
		}
	}

	/* input buffer enable */
	if (ien == GPIO_INPUTBUF_EN) {
		base_val = readl(reg_ien(port));
		set_val = base_val | bit;
		writel(set_val, reg_ien(port));
        comp_val = readl(reg_ien(port));
        if(comp_val != set_val)
		{
			return BSP_ERR_INIT;
		}
	}
	else if (ien == GPIO_INPUTBUF_DIS) {
		base_val = readl(reg_ien(port));
		set_val = base_val & ~bit;
		writel(set_val, reg_ien(port));
        comp_val = readl(reg_ien(port));
        if(comp_val != set_val)
		{
			return BSP_ERR_INIT;
		}
	}
	else //QAC
	{
		; // no statement
	}
	/* input type */
	if ((config & GPIO_INPUT_SCHMITT) != 0) {
		base_val = readl(reg_is(port));
		set_val = base_val | bit;
		writel(set_val, reg_is(port));
        comp_val = readl(reg_is(port));
        if(comp_val != set_val)
		{
			return BSP_ERR_INIT;
		}
	}
	else {
		base_val = readl(reg_is(port));
		set_val =  base_val & ~bit;
		writel(set_val, reg_is(port));
        comp_val = readl(reg_is(port));
        if(comp_val != set_val)
		{
			return BSP_ERR_INIT;
		}
	}

	/* slew rate */
	if ((config & GPIO_SLEW_RATE_SLOW) != 0) {
		base_val = readl(reg_sr(port));
		set_val = base_val | set_val;
		writel(set_val, reg_sr(port));
        comp_val = readl(reg_sr(port));
        if(comp_val != set_val)
		{
			return BSP_ERR_INIT;
		}
	}
	else {
		base_val = readl(reg_sr(port));
		set_val = base_val & ~bit;
		writel(set_val, reg_sr(port));
        comp_val = readl(reg_sr(port));
        if(comp_val != set_val)
		{
			return BSP_ERR_INIT;
		}
	}
	return 0;
}

int32 gpio_get(uint32 port)
{
	if ((readl(reg_data(port)) & (1<<(port&GPIO_PIN_MASK)))!=0) {
		return 1;
	}
	else {
		return 0;
	}
}

int32 gpio_set(uint32 port, uint32 data)
{
	uint32 bit = 1<<(port&GPIO_PIN_MASK);

	gpio_select(port, DISABLE);
	if (data > 1) {
		return -1;
	}

	/* set data */
	if (data!=0) {
		writel(bit, reg_data_or(port));
	}
	else {
		writel(bit, reg_data_bic(port));
	}
	//gpio_select(port, ENABLE);

	return 0;
}

int32 gpio_to_num(uint32 port)
{
	uint32 pin = port&GPIO_PIN_MASK;
	int32 num =0; //GHC Warning

	switch (port & GPIO_PORT_MASK) {
		case GPIO_PORT_A:
			//if (pin <= 31) {	//removed codesonar warning
				//	return pin;
				num = (int32)pin;
			//}
			break;
		case GPIO_PORT_B:
			if (pin <= 28) {
				//return (0x20+pin);
				num = (int32)(0x20+pin);
			}
			break;
		case GPIO_PORT_C:
			if (pin <= 29) {
				//return (0x3d+pin);
				num = (int32)(0x3d+pin);
			}
			break;
		case GPIO_PORT_E:
			if (pin <= 19) {
				//return (0x5b+pin);
				num = (int32)(0x5b+pin);				
			}
			break;
		case GPIO_PORT_G:
			if (pin <= 10) {
				//return (0x6f+pin);
				num = (int32)(0x6f+pin);				
			}
			break;
		case GPIO_PORT_H:
			if (pin <= 11) {
				//return (0x7a+pin);
				num = (int32)(0x7a+pin);				
			}
			break;
		case GPIO_PORT_K:
			if (pin <= 18) {
				//return (0x86+pin);
				num = (int32)(0x86+pin);
			}
			break;
		case GPIO_PORT_MA:
			if (pin <= 29) {
				//return (0x99+pin);
				num = (int32)(0x99+pin);
			}
			break;
		default: // GHC Warning : swich statement must contain a default case
			num = -1;
			break;
	}

	return num;
}

uint32 gpio_get_strength(uint32 port)
{
	uint32 val;
	uint32 pin = port&GPIO_PIN_MASK;

	val = ((readl(reg_cd(port,pin))) >> ((pin%16)*2)) & 0x03;

	return val;
}

static void gpio_select(uint32 port, uint32 flag)
{
	uint32 pin = port&GPIO_PIN_MASK;

	switch (port & GPIO_PORT_MASK) {
		case GPIO_PORT_A:
			if(flag == ENABLE)
			{ //QAC : brace
				writel(readl(gpioa_sel_base)|(0x1<<pin), gpioa_sel_base);
			}
			else
			{
				writel(readl(gpioa_sel_base)& ~(0x1<<pin), gpioa_sel_base);
			}
			break;
		case GPIO_PORT_B:
			if(flag == ENABLE)
			{
				writel(readl(gpiob_sel_base)|(0x1<<pin), gpiob_sel_base);
			}
			else
			{
				writel(readl(gpiob_sel_base)& ~(0x1<<pin), gpiob_sel_base);
			}
			break;
		case GPIO_PORT_C:
			if(flag == ENABLE)
			{
				writel(readl(gpioc_sel_base)|(0x1<<pin), gpioc_sel_base);
			}
			else
			{
				writel(readl(gpioc_sel_base)& ~(0x1<<pin), gpioc_sel_base);
			}
			break;
		case GPIO_PORT_E:
			if(flag == ENABLE)
			{
				writel(readl(gpioe_sel_base)|(0x1<<pin), gpioe_sel_base);
			}
			else
			{
				writel(readl(gpioe_sel_base)& ~(0x1<<pin), gpioe_sel_base);
			}
			break;
		case GPIO_PORT_G:
			if(flag == ENABLE)
			{
				writel(readl(gpiog_sel_base)|(0x1<<pin), gpiog_sel_base);
			}
			else
			{
				writel(readl(gpiog_sel_base)& ~(0x1<<pin), gpiog_sel_base);
			}
			break;
		case GPIO_PORT_H:
			if(flag == ENABLE)
			{
				writel(readl(gpioh_sel_base)|(0x1<<pin), gpioh_sel_base);
			}
			else
			{
				writel(readl(gpioh_sel_base)& ~(0x1<<pin), gpioh_sel_base);
			}
			break;
		case GPIO_PORT_MA:
			if(flag == ENABLE)
			{
				writel(readl(gpioma_sel_base)|(0x1<<pin), gpioma_sel_base);
			}
			else
			{
				writel(readl(gpioma_sel_base)& ~(0x1<<pin), gpioma_sel_base);
			}
			break;
		default:
			break;
	}
}

uint8 GetGpioTableSize(void)
{
	
	uint8 aubIndex = GPIO_PORT_TYPE;
	return aubIndex;
}

uint32 GetGpioTablePort(uint8 id)
{
	
	return nstGPIOTBL[id];
}

#ifdef GPIO_SCAN_DEBUG
void gpio_print_out(uint32 reg, uint32 flag)
{
	uint32 gpio_reg_data;
	int8 gpio_reg_buf[32]={0,};
	int32 i;

	gpio_reg_data=readl(reg+scan_offset[flag]);
	printf("%s : ", scan_list[flag]);
	for(i=0; i < 32 ; i++){
		gpio_reg_buf[31-i] = (gpio_reg_data&0x1)+'0';
		gpio_reg_data = (gpio_reg_data>>1);
	}
	printf("%s\n", gpio_reg_buf);
}


void gpio_scan(uint32 port, uint32 flag){

	uint32 gpio_reg = reg_base(port);
	int32 i;

	switch(port & GPIO_PORT_MASK) {
		case GPIO_PORT_A:
			printf("\nGPIO A REG DATA \n");
			break;
		case GPIO_PORT_B:
			printf("\nGPIO B REG DATA \n");
			break;
		case GPIO_PORT_C:
			printf("\nGPIO C REG DATA \n");
			break;
		case GPIO_PORT_E:
			printf("\nGPIO E REG DATA \n");
			break;
		case GPIO_PORT_G:
			printf("\nGPIO G REG DATA \n");
			break;
		case GPIO_PORT_H:
			printf("\nGPIO H REG DATA \n");
			break;
		case GPIO_PORT_K:
			printf("\nGPIO K REG DATA \n");
			break;
		case GPIO_PORT_MA:
			printf("\nGPIO MA REG DATA \n");
			break;
	}

        if(flag != GPIO_SCAN_ALL){
                gpio_print_out(gpio_reg, flag);
        } else {
                for(i=0; i < GPIO_LIST_NUM; i++)
                {
                        gpio_print_out(gpio_reg, i);
                }
        }

}
#endif
