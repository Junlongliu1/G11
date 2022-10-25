/****************************************************************************
 *   FileName    : bsp.c
 *   Description : Board Support Package for MICOM
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

#include "types.h"
#include <osal.h>
#include "clock.h"
#include "tcc_ckc.h"
#include "debug.h"
#include <bsp_int.h>
#if defined(TCC_AUD_PATH)
#include "tca_i2s.h"
#endif

#include "uart.h"
#include "pwm.h"
#include "bsp.h"
#include <i2c.h>
#include <spi.h>
#include <mt_adc.h>
#include <tcc_timer.h>
#include <ictc.h>
#include "tcc_pmgpio.h"
#include <tcc_wdt.h>
#include <tcc_robust.h>

//extern int32 mmu_init(void);
//extern void BSP_Int_Init(void);
//extern void init_i2c(void);
//extern void init_spi(void);
//extern void init_mt_adc(void);

extern void wvdPmGpioConfig( void );

static void init_clock(void);

void init_arm(void)
{
	uint32 remap;
	#ifdef SNOR_STANDALONE
	//remap on sram
	remap = *((volatile uint32 *)(0x1B936014 ));
	remap |= ((1)<<21);
	*((volatile uint32 *)(0x1B936014 )) = remap;
	#else
	//remap on sram
	remap = *((volatile uint32 *)(0x1B936014 ));
	remap |= ((1)<<21);
	*((volatile uint32 *)(0x1B936014 )) = remap;
	#endif
	/* set vector base */
	//ARM_Set_VBAR((uint32)&_start);
#ifdef MMU_EN
	mmu_init();
#endif
}

static void init_clock(void)
{
	tcc_ckc_init();

	/* DO NOT Change. PLL0 Already set by BL1 */
	//(void)tcc_set_pll(MPLL_0, ENABLE, 1200000000, 3);

	/* DO NOT Change. PLL1 Already set by BL1 */
	//(void)tcc_set_pll(MPLL_1, ENABLE,  960000000, 2);

	/* DO NOT Change. CPU Clock Already set by BL1 */
	//(void)tcc_set_clkctrl(FBUS_CPU, ENABLE, 600000000);
}

void BSP_Init(void)
{
	//init_arm(); // call this funciton in startup.S file

	
	init_clock();

	BSP_Int_Init();
	tcc_timer_init();
	(void)uart_init();
	(void)tcc_wdt_init();

	//(void)init_i2c();
	//(void)init_spi();
	init_pwm();
	ictc_init(); // init input capture
	(void)init_adc(0);
	#ifdef FMU_DRIVER
	(void)init_fmu();
	#endif 

	(void)tcc_robust_ca53_watchdog_init();
	(void)tcc_robust_ca7s_watchdog_init();

	wvdPmGpioConfig();

	//WriteDeviceShareConfig(SRAM_DEVICE_SHARE_CONFIG_BASE);
	//writel(0x4B4F434D, SRAM_BOOT_SEQ_CHECK_BASE);// Write "MCOK"

	//tcc_pr_info("Welcome to Telechips MICOM\n");

}

/* R0 : ARM Exception ID, R1 : Dummy , R2 : Link Register(Return PC)*/
void BSP_Undef_Abort_Exception_Hook(uint32 except_id, uint32 dumy,uint32 LR)
{
/* Unused function by TCC803X
	writel(except_id, SRAM_EXCEPTION_SAVE_BASE);
	writel(LR, (SRAM_EXCEPTION_SAVE_BASE+4));
*/
}

/* 
Warning !!!  After calliing this function. it will never return from exception except H/W reseting. 
This function is designed for very restricted use case such as firmware upgrading.
*/
void System_Halt(void)
{
	OSAL_SR_ALLOC();
	OSAL_CRITICAL_ENTER(); // Interrupt disable(I/F)
	arm_reserved(); // move ARM p.c on the sram.
	// can not reach to this line
	OSAL_CRITICAL_EXIT(); //warning. has no effect. 
}
