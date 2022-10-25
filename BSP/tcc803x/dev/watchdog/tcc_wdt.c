/****************************************************************************
 *   FileName    : tcc_wdt.c
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
#ifndef __FILENAME__
#define __FILENAME__ "tcc_wdt.c"
#endif/*__FILENAME__*/

#include <types.h>
#include <bsp.h>
#include <bsp_int.h>
#include "debug.h"
#include "clock.h"
#include "reg_phys.h"
#include "tcc_timer.h"
#include "tcc_wdt.h"
#ifndef TCC803x_CS
#include "tcc_ckc.h"
#endif

#include "pmwdt.h"

extern void wvdPmHardWdtSend(void);

#define TCC_WDT_DEBUG	0
#if TCC_WDT_DEBUG
#define wdt_pr_dbg	tcc_printf
#else
#define wdt_pr_dbg(fmt,...)	\
	do{}while(0)
#endif /* TCC_WDT_DEBUG */

#ifndef TCC803x_CS
#define COLD_RESET_BASE	0x1B93617C
#endif

#ifdef  TCC803x_CS
#define wdt_time2cnt(t)	(t)*24000000
#else
#define wdt_time2cnt(t)	((t)*(CKC_GET_RATE(PERI_WDT)))	/* t: sec */
#define TCC_WDT_RATE_12	12 /* MHz */
#endif

#define WDT_EN_OFF	0x00
#define WDT_CLR_OFF	0x04
#define WDT_IRQ_CNT_OFF	0x08
#define WDT_RSR_CNT_OFF	0x0C
#ifdef TCC803x_CS
#define WDT_SM_MODE_OFF	0x10
#define WDT_PW_OFF	0x14
#endif /* TCC803x_CS */

#define PMU_WDTCTRL_OFF	0xD8

#define WDT_EN_BIT	(0)
#define WDT_CLR_BIT	(0)

#ifdef TCC803x_CS
#define WDT_SM_CLR	(4)
#define WDT_SM_CNT	(3)
#define WDT_SM_RST_SEL	(2)
#define WDT_SM_RST_PMU	(0x0)
#define WDT_SM_RST_FMU	(0x1)
#define WDT_SM_WDT0	(0x00)
#define WDT_SM_WDT1	(0x01)
#define WDT_SM_WDT2	(0x02)
#define WDT_SM_VOTE	(0x03)

#define WDT_PASSWORD	0x8030ACE5
#endif /* TCC803x_CS */

#define PMU_WDT_EN_BIT		(31)
#define PMU_WDT_RSTEN_BIT	(7)

#define EN_BIT(B,P)		((B)<<(P))

/* WTD time: second */
#define WDT_IRQ_TIME	18
#define WDT_RESET_TIME	24

/*************************** LOCAL VARIABLE ***************************/
static uint32 tcc_wdt_status = 0;
/**********************************************************************/

/*************************** LOCAL FUNCTION ***************************/
#ifdef TCC803x_CS
static uint32 wdt_readl(void __iomem * addr);
static void wdt_writel(uint32 val, void __iomem * addr);
static void tcc_wdt_sm_mode(uint32 clear, uint32 oneshot, uint32 rst_sel, uint32 vote);
#else
#define wdt_readl	readl
#define wdt_writel	writel
#endif
static void __iomem *tcc_wdt_addr(uint32 base, uint32 offset);
static int32 tcc_wdt_set_timeout(uint32 timeout, uint32 pretimeout);
static void tcc_wdt_enable_timer(void);
static void tcc_wdt_disable_timer(void);
static void tcc_wdt_irq_handler(void *args);
static void tcc_wdt_start(void);
/**********************************************************************/

#ifdef TCC803x_CS
static uint32 wdt_readl(void __iomem * addr)
{
	return readl(addr);
}

static void wdt_writel(uint32 val, void __iomem * addr)
{
	writel(WDT_PASSWORD, tcc_wdt_addr(TCC_WDT_BASE,WDT_PW_OFF));
	writel(val, addr);
}

static void tcc_wdt_sm_mode(uint32 clear, uint32 oneshot, uint32 rst_sel, uint32 vote)
{
	uint32 safety_mode = 0x00000;

	safety_mode |= (clear << WDT_SM_CLR);
	safety_mode |= (oneshot << WDT_SM_CNT);
	safety_mode |= (rst_sel << WDT_SM_RST_SEL);
	safety_mode |= vote;

	wdt_writel(safety_mode, tcc_wdt_addr(TCC_WDT_BASE,WDT_SM_MODE_OFF));

	wdt_pr_dbg("safety_mode : 0x%08X", safety_mode);
}
#endif /* TCC803x_CS */

static void __iomem *tcc_wdt_addr(uint32 base, uint32 offset)
{
	return (void __iomem *)(base + offset);
}

static int32 tcc_wdt_set_timeout(uint32 timeout, uint32 pretimeout)
{
	int32 res = BSP_ERR_NONE;

	if (pretimeout > timeout)
	{
		wdt_pr_dbg("timeout value is less than pretimeout value : %d, %d", timeout, pretimeout);
		res = BSP_ERR_INVALID_PARAMETER;
	}

	wdt_pr_dbg("timeout : %d, pretimeout : %d", timeout, pretimeout);
	wdt_writel(wdt_time2cnt(pretimeout), tcc_wdt_addr(TCC_WDT_BASE,WDT_IRQ_CNT_OFF));
	wdt_writel(wdt_time2cnt(timeout), tcc_wdt_addr(TCC_WDT_BASE,WDT_RSR_CNT_OFF));

	return res;
}

static void tcc_wdt_enable_timer(void)
{
	wdt_pr_dbg("%s",__func__);

	/* enable watchdog timer interrupt */
	BSP_IntSrcEn(INT_WATCHDOG);
}

static void tcc_wdt_disable_timer(void)
{
	wdt_pr_dbg("%s",__func__);

	/* disable watchdog timer interrupt */
	BSP_IntSrcDis(INT_WATCHDOG);
}

void tcc_wdt_ping(void)
{
	wdt_writel(EN_BIT(1,WDT_CLR_BIT), tcc_wdt_addr(TCC_WDT_BASE,WDT_CLR_OFF));
}

static void tcc_wdt_irq_handler(void *args)
{
	wdt_pr_dbg("%s",__func__);

	wvdPmHardWdtSend();
}

static void tcc_wdt_start(void)
{
	uint32 wdt_en = 0;
	uint32 wdt_rsten = 0;
	tcc_wdt_enable_timer();

	/* enable watchdog timer */
	wdt_writel(EN_BIT(1,WDT_EN_BIT), tcc_wdt_addr(TCC_WDT_BASE,WDT_EN_OFF));


	wdt_en = EN_BIT(1UL,PMU_WDT_EN_BIT); //GHC Warning
	wdt_rsten = EN_BIT(1UL,PMU_WDT_RSTEN_BIT);

	/* enable pmu watchdog */
	wdt_writel((wdt_en | wdt_rsten)
		| wdt_readl(tcc_wdt_addr(TCC_PMU_BASE_MICOM,PMU_WDTCTRL_OFF)),
		tcc_wdt_addr(TCC_PMU_BASE_MICOM,PMU_WDTCTRL_OFF));

	tcc_wdt_status = 1;
}

void tcc_wdt_stop(void)
{
	wdt_writel(EN_BIT(0,WDT_EN_BIT), tcc_wdt_addr(TCC_WDT_BASE,WDT_EN_OFF));
	tcc_wdt_disable_timer();

	tcc_wdt_status = 0;
}

void tcc_wdt_reset(void)
{
	wdt_pr_dbg("%s",__func__);

	tcc_wdt_disable_timer();

#ifdef TCC803x_CS
	/* set the watchdog timer reset request count to zero */
	wdt_writel(0x00, tcc_wdt_addr(TCC_WDT_BASE,WDT_RSR_CNT_OFF));
#else
	wdt_writel(0x01, COLD_RESET_BASE);
#endif
}

int32 tcc_wdt_init(void)
{
	int32 ret = BSP_ERR_NONE;

	if (tcc_wdt_status == 0){

		//tcc_pr_info("%s",__func__);

#ifdef TCC803x_CS
		/* WDT clock rate is fixed to 24 MHz
		 * Unnecessary to set clock */
#else
		/* set wdt clk */
		(void)CKC_SET_EN(PERI_WDT, CKC_MHZ(TCC_WDT_RATE_12));
#endif

#ifdef TCC803x_CS
		tcc_wdt_sm_mode(1, 1, WDT_SM_RST_PMU, WDT_SM_VOTE);
#endif

		(void) tcc_wdt_set_timeout(WDT_RESET_TIME, WDT_IRQ_TIME);

		if (BSP_IntVectSet(INT_WATCHDOG, PRIORITY_NO_MEAN,
					INT_TYPE_EDGE_RISING,
					tcc_wdt_irq_handler,
					0) != BSP_OK)
		{
			ret = BSP_ERR_INIT;
			tcc_pr_err("irq hanlder set failed! [%d]", ret);
		}
		else
		{
			tcc_wdt_start();
		}
	}

	return ret;
}
