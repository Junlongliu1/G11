/****************************************************************************
 *   FileName    : tcc_ckc.c
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
#include "types.h"
#include <string.h>
#include <osal.h>
#include "reg_phys.h"
#include "tcc_ckc.h"
#include "ckc_reg.h"
#include "div64.h"
#include "clock.h"

/**********************************
 *  Pre Defines
 **********************************/
#define ckc_writel	writel
#define ckc_readl	readl

#define MAX_MICOM_PLL	2
#define MAX_MICOM_CLK_SRC	((MAX_MICOM_PLL * 2 ) + 2)

static addr_t	micom_ckc_base = 0;

static uint32 stMicomClockSource[MAX_MICOM_CLK_SRC];

//static struct tcc_ckc_ops tcc803x_ops; //QAC : hide a more global declaration

static inline void tcc_ckc_reset_clock_source(int32 id);
//QAC 
static inline void tcc_pclkctrl_write(addr_t reg, uint32 md,
				      uint32 en, uint32 sel,
				      uint32 u32_div, uint32 type);//Msg(4:5115) The names of standard library macros, objects and functions shall not be reused. MISRA-C:2004 Rule 20.2
static inline void tcc_micom_clkctrl_write(addr_t reg, uint32 en,
				     uint32 config, uint32 sel);
//static int32 tcc_find_pms_table(tPMSValue *table, uint32 rate, uint32 max);
static inline int32 tcc_find_pms(tPMS *PLL, uint32 srcfreq);
static int32 tcc_ckc_dedicated_pll_set_rate(addr_t reg, uint32 rate);
static uint32 tcc_ckc_dedicated_pll_get_rate(addr_t reg);
static int32 tcc_ckc_micom_pll_div_get(int32 id);
static int32 tcc_ckc_micom_pll_div_set(int32 id, uint32 plldiv);
static uint32 tcc_ckc_micom_pll_get_rate(int32 id);
static int32 tcc_ckc_micom_pll_set_rate(int32 id, uint32 rate);
static inline int32 tcc_find_micom_clkctrl(tCLKCTRL *CLKCTRL);
static int32 tcc_ckc_micom_clkctrl_set_rate(int32 id, uint32 rate);
static uint32 tcc_ckc_micom_clkctrl_get_rate(int32 id);
static inline uint32 tcc_ckc_pclk_divider(const tPCLKCTRL *PCLKCTRL, uint32 *clkdiv,
	const uint32 src_CLK, uint32 div_max);
static int32 tcc_micom_find_pclk(tPCLKCTRL *PCLKCTRL, tPCLKTYPE type);
static int32 tcc_ckc_micom_is_peri_enabled(int32 id);
static int32 tcc_ckc_micom_peri_enable(int32 id);
static int32 tcc_ckc_micom_peri_disable(int32 id);
static uint32 tcc_ckc_micom_peri_get_rate(int32 id);
static int32 tcc_ckc_micom_peri_set_rate(int32 id, uint32 rate);
static int32 tcc_ckc_micom_is_iobus_pwdn(int32 id);
static int32 tcc_ckc_micom_iobus_pwdn(int32 id, bool pwdn);
static int32 tcc_ckc_micom_iobus_swreset(int32 id, bool reset);
static inline void tcc_ckc_reset_clock_source(int32 id);

/* PLL Configuration Macro */
#define tcc_pll_write(reg,en,p,m,s,src) do { \
	if (en) { \
		volatile uint32 i; \
		ckc_writel(0 \
			|(1<<PLL_LOCKEN_SHIFT)|(2<<PLL_CHGPUMP_SHIFT) \
			|(((src)&PLL_SRC_MASK)<<PLL_SRC_SHIFT)|(((s)&PLL_S_MASK)<<PLL_S_SHIFT) \
			|(((m)&PLL_M_MASK)<<PLL_M_SHIFT)|(((p)&PLL_P_MASK)<<PLL_P_SHIFT), reg); \
		/* need to delay at least 1us. */ \
		for (i=100 ; i ; i--);  /* if cpu clokc is 1HGz then loop 100. */ \
		ckc_writel(ckc_readl(reg) | (((en)&1)<<PLL_EN_SHIFT), reg); \
		while((ckc_readl(reg)&(1<<PLL_LOCKST_SHIFT))==0); \
	} else \
		ckc_writel(ckc_readl(reg) & ~(1<<PLL_EN_SHIFT), reg); \
} while(0)

#define tcc_dckc_pll_write(reg,en,p,m,s) do { \
        if (en) { \
                volatile uint32 i; \
                ckc_writel(0 \
                        |(1<<PLL_LOCKEN_SHIFT)|(2<<PLL_CHGPUMP_SHIFT) \
                        |(((s)&PLL_S_MASK)<<PLL_S_SHIFT) \
                        |(((m)&PLL_M_MASK)<<PLL_M_SHIFT)|(((p)&PLL_P_MASK)<<PLL_P_SHIFT), reg); \
                for (i=100 ; i ; i--)									\
                {												\
			;  /* if cpu clokc is 1HGz then loop 100. */ 			\
                }											\
                ckc_writel(ckc_readl(reg) | (((en)&1)<<PLL_EN_SHIFT), reg); \
                while((ckc_readl(reg)&(1<<PLL_LOCKST_SHIFT))==0)	\
                {			\
			i++; 		\
                }			\
        } else { \
                ckc_writel(ckc_readl(reg) & ~(1<<PLL_EN_SHIFT), reg); \
        } \
} while(0)

static inline void tcc_pclkctrl_write(addr_t reg, uint32 md,
				      uint32 en, uint32 sel,
				      uint32 u32_div, uint32 type) 
{
	if (type == PCLKCTRL_TYPE_XXX) {
		ckc_writel(ckc_readl(reg) & ~(1<<PCLKCTRL_OUTEN_SHIFT), reg);
		ckc_writel(ckc_readl(reg) & ~(1<<PCLKCTRL_EN_SHIFT), reg);
		ckc_writel((ckc_readl(reg) & ~(PCLKCTRL_DIV_XXX_MASK<<PCLKCTRL_DIV_SHIFT)) |
				((u32_div&PCLKCTRL_DIV_XXX_MASK)<<PCLKCTRL_DIV_SHIFT), reg);
		ckc_writel((ckc_readl(reg) & ~(PCLKCTRL_SEL_MASK<<PCLKCTRL_SEL_SHIFT)) |
				((sel&PCLKCTRL_SEL_MASK)<<PCLKCTRL_SEL_SHIFT), reg);
		ckc_writel((ckc_readl(reg) & ~(1<<PCLKCTRL_EN_SHIFT))
			| ((en&1)<<PCLKCTRL_EN_SHIFT), reg);
		ckc_writel((ckc_readl(reg) & ~(1<<PCLKCTRL_OUTEN_SHIFT))
			| ((en&1)<<PCLKCTRL_OUTEN_SHIFT), reg);
	} else if (type == PCLKCTRL_TYPE_YYY) {
		ckc_writel(ckc_readl(reg) & ~(1<<PCLKCTRL_EN_SHIFT), reg);
		ckc_writel((ckc_readl(reg) & ~(PCLKCTRL_DIV_YYY_MASK<<PCLKCTRL_DIV_SHIFT)) |
				((u32_div&PCLKCTRL_DIV_YYY_MASK)<<PCLKCTRL_DIV_SHIFT), reg);
		ckc_writel((ckc_readl(reg) & ~(PCLKCTRL_SEL_MASK<<PCLKCTRL_SEL_SHIFT)) |
				((sel&PCLKCTRL_SEL_MASK)<<PCLKCTRL_SEL_SHIFT), reg);
		ckc_writel((ckc_readl(reg) & ~(1<<PCLKCTRL_MD_SHIFT)) |
				((md&1)<<PCLKCTRL_MD_SHIFT), reg);
		ckc_writel((ckc_readl(reg) & ~(1<<PCLKCTRL_EN_SHIFT))
			| ((en&1)<<PCLKCTRL_EN_SHIFT), reg);
	}
	else
	{
		; //empty statement
	}
}

static inline void tcc_micom_clkctrl_write(addr_t reg, uint32 en,
				     uint32 config, uint32 sel)
{
	volatile uint32 i = 0;
	uint32 cur_config;

	cur_config = (ckc_readl(reg) >> MCLKCTRL_CONFIG_SHIFT) & MCLKCTRL_CONFIG_MASK;

	if (config >= cur_config) {
		ckc_writel((ckc_readl(reg)&(~(MCLKCTRL_CONFIG_MASK<<MCLKCTRL_CONFIG_SHIFT)))
			   |((config&MCLKCTRL_CONFIG_MASK)<<MCLKCTRL_CONFIG_SHIFT), reg);
		while((ckc_readl(reg) & (1UL<<MCLKCTRL_CLKCHG_SHIFT)) !=0)
			{
				// dummy value for Codesonar
				i++;	//do nothing
			}
		ckc_writel((ckc_readl(reg)&(~(MCLKCTRL_SEL_MASK<<MCLKCTRL_SEL_SHIFT)))
			   |((sel&MCLKCTRL_SEL_MASK)<<MCLKCTRL_SEL_SHIFT), reg);
		while((ckc_readl(reg) & (1UL<<MCLKCTRL_CLKCHG_SHIFT)) !=0)
			{
				// dummy value for Codesonar
				i++;	//do nothing
			}
	}
	else {
		ckc_writel((ckc_readl(reg)&(~(MCLKCTRL_SEL_MASK<<MCLKCTRL_SEL_SHIFT)))
			   |((sel&MCLKCTRL_SEL_MASK)<<MCLKCTRL_SEL_SHIFT), reg);
		while((ckc_readl(reg) & (1UL<<MCLKCTRL_CLKCHG_SHIFT)) != 0)
			{
				// dummy value for Codesonar
				i++;	//do nothing
			}
		ckc_writel((ckc_readl(reg)&(~(MCLKCTRL_CONFIG_MASK<<MCLKCTRL_CONFIG_SHIFT)))
			   |((config&MCLKCTRL_CONFIG_MASK)<<MCLKCTRL_CONFIG_SHIFT), reg);
		while((ckc_readl(reg) & (1UL<<MCLKCTRL_CLKCHG_SHIFT)) !=0)
			{
				// dummy value for Codesonar
				i++;	//do nothing
			}
	}
	if (en != 0) {//Msg(4:3344) Value of control expression is not derived from an explicit logical operation. MISRA-C:2004 Rule 13.2
		ckc_writel((ckc_readl(reg)&(~(1<<MCLKCTRL_EN_SHIFT)))
				|((en&1)<<MCLKCTRL_EN_SHIFT), reg);
		while((ckc_readl(reg) & (1 << MCLKCTRL_DIVSTS_SHIFT)) !=0)
			{
				// dummy value for Codesonar
				i++;	//do nothing
			}
	}
}

/* GHC Warning : declared but never referenced
static int32 tcc_find_pms_table(tPMSValue *table, uint32 rate, uint32 max)
{
	uint32 i;

	for(i = 0; i < max; i++) {
		if(table[i].fpll == rate) {
			return i;
		}
	}

	return -1;
}
*/

static inline int32 tcc_find_pms(tPMS *PLL, uint32 srcfreq)
{
	uint64	u64_pll, u64_src, fvco, srch_p, srch_m, u64_tmp;
	uint32 srch_pll, err, srch_err;
	int32	srch_s;

	if (PLL->fpll ==0) {
		PLL->en = 0;
		return 0;
	}

	u64_pll = (uint64)PLL->fpll;
	u64_src = (uint64)srcfreq;

	err = 0xFFFFFFFF;
	srch_err = 0xFFFFFFFF;
	for (srch_s=PLL_S_MIN; srch_s <= PLL_S_MAX ; srch_s++) {
		fvco = u64_pll << srch_s;

		if ((fvco >= PLL_VCO_MIN) && (fvco <= PLL_VCO_MAX)) {
			for (srch_p=PLL_P_MIN ; srch_p<=PLL_P_MAX ; srch_p++) {
				srch_m = fvco*srch_p;
				do_div(srch_m, srcfreq);
		                if ((srch_m < PLL_M_MIN) || (srch_m > PLL_M_MAX))
		                {
		                        continue;
		                }
				u64_tmp = srch_m*u64_src;
				do_div(u64_tmp, srch_p);
		                srch_pll = (uint32)(u64_tmp>>srch_s);
				if ((srch_pll < PLL_MIN_RATE )|| (srch_pll > PLL_MAX_RATE))
				{
					continue;
				}
		                srch_err = (srch_pll > u64_pll) ? (srch_pll - u64_pll) : (u64_pll - srch_pll);
		                if (srch_err < err) {
		                        err = srch_err;
		                        PLL->p = (uint32)srch_p;
		                        PLL->m = (uint32)srch_m;
					PLL->s = (uint32)srch_s;
		                }
			}
		}
	}
	if (err == 0xFFFFFFFF)
	{
		return -1;
	}

	u64_tmp = u64_src*(uint64)PLL->m;
	do_div(u64_tmp, PLL->p);
	PLL->fpll = (uint32)(u64_tmp>>PLL->s);
	PLL->en = 1;
	return 0;
}

static int32 tcc_ckc_dedicated_pll_set_rate(addr_t reg, uint32 rate)
{
	tPMS		nPLL;
	int32 err = 0;
	nPLL.fpll = rate;

	if (tcc_find_pms(&nPLL, XIN_CLK_RATE) != 0)//Msg(4:3344) Value of control expression is not derived from an explicit logical operation. MISRA-C:2004 Rule 13.2
	{
		//goto tcc_ckc_setpll2_failed; //Msg(4:2001) A 'goto' statement has been used. MISRA-C:2004 Rule 14.4
		tcc_dckc_pll_write(reg, 0, PLL_P_MIN, ((PLL_P_MIN*PLL_VCO_MIN)+XIN_CLK_RATE)/XIN_CLK_RATE, PLL_S_MIN);
		err = -1;
	}
	else
	{
		tcc_dckc_pll_write(reg, nPLL.en, nPLL.p, nPLL.m, nPLL.s);
		err = 0;
	}
	//return 0;

//tcc_ckc_setpll2_failed: //Msg(4:2001) A 'goto' statement has been used. MISRA-C:2004 Rule 14.4
//	tcc_dckc_pll_write(reg, 0, PLL_P_MIN, ((PLL_P_MIN*PLL_VCO_MIN)+XIN_CLK_RATE)/XIN_CLK_RATE, PLL_S_MIN);
	return err;
}

static uint32 tcc_ckc_dedicated_pll_get_rate(addr_t reg)
{
	uint32	reg_values = ckc_readl(reg);
	tPMS		nPLLCFG;
	uint64		u64_tmp;

	nPLLCFG.p = (reg_values>>PLL_P_SHIFT)&(PLL_P_MASK);
	nPLLCFG.m = (reg_values>>PLL_M_SHIFT)&(PLL_M_MASK);
	nPLLCFG.s = (reg_values>>PLL_S_SHIFT)&(PLL_S_MASK);
	nPLLCFG.en = (reg_values>>PLL_EN_SHIFT)&(1);
	nPLLCFG.src = (reg_values>>PLL_SRC_SHIFT)&(PLL_SRC_MASK);

	u64_tmp = (uint64)XIN_CLK_RATE*(uint64)nPLLCFG.m;
	do_div(u64_tmp, nPLLCFG.p);
	return (uint32)((u64_tmp)>>nPLLCFG.s);
}

static int32 tcc_ckc_micom_pll_div_get(int32 id)
{
	addr_t reg = micom_ckc_base + MCKC_CLKDIVC;
	uint32 offset = 0;
	uint32 reg_values = 0;

	switch(id) {
	case MPLL_0:
	case MPLL_1:
		offset = (3-id) * 8;
		break;
	case MPLL_XIN:
		offset = 8;
		break;
	default:
		return 0;
	}

	reg_values = ckc_readl(reg);

	return ((reg_values >> offset) & 0x3F);
}

static int32 tcc_ckc_micom_pll_div_set(int32 id, uint32 plldiv)
{
	addr_t reg = micom_ckc_base + MCKC_CLKDIVC;
	uint32 offset = 0;
	uint32 reg_values = 0;

	switch(id) {
	case MPLL_0:
	case MPLL_1:
		offset = (3-id) * 8;
		break;
	case MPLL_XIN:
		offset = 8;
		break;
	default:
		return -1;
	}
	/* Reset pll_0 Pll_1 XIN*/
	reg_values = ckc_readl(reg) & ~((uint32)(0xFFU << offset));
	ckc_writel(reg_values, reg);
	if (plldiv != 0) {
		reg_values |= (0x80 | (plldiv & 0x3F)) << offset;
	}
	else {
		reg_values |= 1 << offset;
	}
	ckc_writel(reg_values, reg);

	return 0;
}

static uint32 tcc_ckc_micom_pll_get_rate(int32 id)
{
	addr_t reg = (addr_t)NULL;

	if (id == PLL_MICOM_0) {
		reg = micom_ckc_base + MCKC_PLL0PMS;
	}
	else if (id == PLL_MICOM_1) {
		reg = micom_ckc_base + MCKC_PLL1PMS;
	}
	else {
		return 0;
	}

	return tcc_ckc_dedicated_pll_get_rate(reg);
}

static int32 tcc_ckc_micom_pll_set_rate(int32 id, uint32 rate)
{
        addr_t reg = (addr_t)NULL;
        int32 idx = -1;

        if (id == PLL_MICOM_0) {
                reg = micom_ckc_base + MCKC_PLL0PMS;
                idx = MPCLKCTRL_SEL_PLL0;
        }
        else if (id == PLL_MICOM_1) {
                reg = micom_ckc_base + MCKC_PLL1PMS;
                idx = MPCLKCTRL_SEL_PLL1;
        }
	else {
		return 0;
	}

        (void)tcc_ckc_dedicated_pll_set_rate(reg, rate);//QAC unused return
        stMicomClockSource[idx] = tcc_ckc_dedicated_pll_get_rate(reg);

        return 0;
}

static inline int32 tcc_find_micom_clkctrl(tCLKCTRL *CLKCTRL)
{
	uint32 i, divid_param[MAX_MICOM_CLK_SRC], err[MAX_MICOM_CLK_SRC], searchsrc, clk_rate;
	//int32 xin_freq = CLKCTRL->en ? (XIN_CLK_RATE/2) : XIN_CLK_RATE; //QAC Rule 13.2
	uint32 xin_freq = 0;

	if( CLKCTRL->en != 0)
	{
		xin_freq = (XIN_CLK_RATE/2);
	}
	else
	{
		xin_freq= XIN_CLK_RATE;
	}
	searchsrc = 0xFFFFFFFF;

	if (CLKCTRL->freq <= xin_freq) {
		CLKCTRL->sel = MCLKCTRL_SEL_XIN;
		CLKCTRL->freq = xin_freq;
		//CLKCTRL->config = CLKCTRL->en ? 1 : 0;
		if( CLKCTRL->en != 0)
		{
			CLKCTRL->config = 1;
		}
		else
		{
			CLKCTRL->config= 0;
		}
	}
	else {
		OSAL_MemSet((void *) divid_param, 0x00, sizeof(divid_param)); // QAC 9.1 : Using value of uninitialized 
		for (i=0 ; i<MAX_MICOM_CLK_SRC ; i++) {
			if (stMicomClockSource[i] == 0)
			{
				continue;
			}
			if (CLKCTRL->en != 0) {//Msg(4:3344) Value of control expression is not derived from an explicit logical operation. MISRA-C:2004 Rule 13.2
				divid_param[i] = (stMicomClockSource[i]
					+ (CLKCTRL->freq - 1)) / CLKCTRL->freq;
				if (divid_param[i] > (MCLKCTRL_CONFIG_MAX+1))
				{
					divid_param[i] = MCLKCTRL_CONFIG_MAX+1;
				}
				else if (divid_param[i] < (MCLKCTRL_CONFIG_MIN+1))
				{
					divid_param[i] = MCLKCTRL_CONFIG_MIN+1;
				}
				else
				{
					; //empty statement
				}
				clk_rate = stMicomClockSource[i]/divid_param[i];
			} else {
				clk_rate = stMicomClockSource[i];
			}
			if (CLKCTRL->freq < clk_rate)
			{
				continue;
			}
			err[i] = CLKCTRL->freq - clk_rate;
			if (searchsrc == 0xFFFFFFFF)
			{
				searchsrc = i;
			}
			else {
				/* find similar clock */
				if (err[i] < err[searchsrc])
				{
					searchsrc = i;
				}
				/* find even division vlaue */
				else if(err[i] == err[searchsrc]) {
					if (divid_param[i]%2 == 0)
					{
						searchsrc = i;
					}
				}
				else
				{
					;
				}
			}
			if (err[searchsrc] == 0)
			{
				break;
			}
		}
		if (searchsrc == 0xFFFFFFFF)
		{
			return -1;
		}
		switch(searchsrc) {
		case MPLL_0:	CLKCTRL->sel = MCLKCTRL_SEL_PLL0; break;
		case MPLL_1:	CLKCTRL->sel = MCLKCTRL_SEL_PLL1; break;
		case MPLL_DIV_0:	CLKCTRL->sel = MCLKCTRL_SEL_PLL0DIV; break;
		case MPLL_DIV_1:	CLKCTRL->sel = MCLKCTRL_SEL_PLL1DIV; break;
		case MPLL_XIN:	CLKCTRL->sel = MCLKCTRL_SEL_XIN; break;
		default: return -1;
		}
		if (CLKCTRL->en != 0)  //QAC
		{
			if (divid_param[searchsrc] > (MCLKCTRL_CONFIG_MAX + 1))
			{
				divid_param[searchsrc] = MCLKCTRL_CONFIG_MAX+1;
			}
			else if (divid_param[searchsrc] <= MCLKCTRL_CONFIG_MIN)
			{
				divid_param[searchsrc] = MCLKCTRL_CONFIG_MIN+1;
			}
			else
			{
				; //else
			}
			CLKCTRL->freq = stMicomClockSource[searchsrc]/divid_param[searchsrc];
			CLKCTRL->config = divid_param[searchsrc] - 1;
		} else {
			CLKCTRL->freq = stMicomClockSource[searchsrc];
			CLKCTRL->config = 0;
		}
	}
	return 0;
}

static int32 tcc_ckc_micom_clkctrl_set_rate(int32 id, uint32 rate)
{
	addr_t	reg = micom_ckc_base+MCKC_CLKCTRL+(id - FBUS_CPU)*4;
	tCLKCTRL	nCLKCTRL;
	//int32 idx;//warning. unused variable

	//nCLKCTRL.en = (ckc_readl(reg) & (1<<MCLKCTRL_EN_SHIFT)) ? 1 : 0;
	if((ckc_readl(reg) & (1<<MCLKCTRL_EN_SHIFT)) != 0)
	{
		nCLKCTRL.en = 1;
	}
	else
	{
		nCLKCTRL.en= 0;
	}

	nCLKCTRL.freq = rate;
	if (tcc_find_micom_clkctrl(&nCLKCTRL) != 0) //QAC : 13.2
	{
		return -1;
	}

	tcc_micom_clkctrl_write(reg, nCLKCTRL.en, nCLKCTRL.config, nCLKCTRL.sel);
	return 0;
}

static uint32 tcc_ckc_micom_clkctrl_get_rate(int32 id)
{
	addr_t	reg = micom_ckc_base+MCKC_CLKCTRL+(id - FBUS_CPU)*4;
	tCLKCTRL	nCLKCTRL;
	uint32 reg_values = 0;
	uint32 src_freq = 0;

	reg_values = ckc_readl(reg);

	nCLKCTRL.sel = (reg_values & (MCLKCTRL_SEL_MASK<<MCLKCTRL_SEL_SHIFT)) >> MCLKCTRL_SEL_SHIFT;
	switch (nCLKCTRL.sel) {
	case MCLKCTRL_SEL_XIN:
		src_freq = XIN_CLK_RATE;
		break;
	case MCLKCTRL_SEL_PLL0:
		src_freq = tcc_ckc_micom_pll_get_rate(PLL_MICOM_0);
		break;
	case MCLKCTRL_SEL_PLL1:
		src_freq = tcc_ckc_micom_pll_get_rate(PLL_MICOM_1);
		break;
	case MCLKCTRL_SEL_PLL0DIV:
		src_freq = tcc_ckc_micom_pll_get_rate(PLL_MICOM_0) /
			(tcc_ckc_micom_pll_div_get(PLL_MICOM_0)+1);
		break;
	case MCLKCTRL_SEL_PLL1DIV:
		src_freq = tcc_ckc_micom_pll_get_rate(PLL_MICOM_1) /
			(tcc_ckc_micom_pll_div_get(PLL_MICOM_1)+1);
		break;
	default:
		return 0;
	}

	nCLKCTRL.config = (reg_values & (MCLKCTRL_CONFIG_MASK<<MCLKCTRL_CONFIG_SHIFT)) >> MCLKCTRL_CONFIG_SHIFT;
	nCLKCTRL.freq = src_freq / (nCLKCTRL.config+1);

	return (uint32)nCLKCTRL.freq;
}

static inline uint32 tcc_ckc_pclk_divider(const tPCLKCTRL *PCLKCTRL, uint32 *clkdiv,
	const uint32 src_CLK, uint32 div_max)
{
	uint32	clk_rate1, clk_rate2, err1, err2;

	if (src_CLK <= PCLKCTRL->freq)
	{
		*clkdiv = 1;
	}
	else
	{
		*clkdiv = src_CLK/PCLKCTRL->freq;
	}

	if ((*clkdiv) > div_max)
	{
		*clkdiv = div_max;
	}

	clk_rate1 = (src_CLK)/(*clkdiv);
	clk_rate2 = (src_CLK)/(((*clkdiv) < div_max) ? ((*clkdiv)+1) : (*clkdiv));
	err1 = (clk_rate1 > PCLKCTRL->freq)?(clk_rate1 - PCLKCTRL->freq):(PCLKCTRL->freq - clk_rate1);
	err2 = (clk_rate2 > PCLKCTRL->freq)?(clk_rate2 - PCLKCTRL->freq):(PCLKCTRL->freq - clk_rate2);

	if (err1 > err2)
	{
		*clkdiv += 1;
	}

	return (err1 < err2) ? err1 : err2;
}

static int32 tcc_micom_find_pclk(tPCLKCTRL *PCLKCTRL, tPCLKTYPE type)
{
	int32 i;
	uint32 div_max, searchsrc, err_div, md;
	uint32 u32_div[MAX_MICOM_CLK_SRC], err[MAX_MICOM_CLK_SRC];
	uint32 div_div = PCLKCTRL_DIV_MIN;

	PCLKCTRL->md = PCLKCTRL_MODE_DIVIDER;
	div_max = PCLKCTRL_DIV_XXX_MAX;

	OSAL_MemSet((void *)u32_div, 0x00, sizeof(u32_div)); // QAC 9.1 : Using value of uninitialized 

	searchsrc = 0xFFFFFFFF;
	for (i=(MAX_MICOM_CLK_SRC-1) ; i>=0 ; i--) {
		if (stMicomClockSource[i] == 0) {
			continue;
		}
		if ((stMicomClockSource[i] >= PCLKCTRL_MAX_FCKS) && (type == PCLKCTRL_TYPE_XXX)) {
			continue;
		}

		/* divider mode */
		err_div = tcc_ckc_pclk_divider(PCLKCTRL, &div_div, stMicomClockSource[i], div_max+1);

		err[i] = err_div;
		u32_div[i] = div_div;
		md = PCLKCTRL_MODE_DIVIDER;

		if (searchsrc == 0xFFFFFFFF) {
			searchsrc = i;
			PCLKCTRL->md = md;
		}
		else {
			/* find similar clock */
			if (err[i] < err[searchsrc]) {
				searchsrc = i;
				PCLKCTRL->md = md;
			}
		}
	}

	switch(searchsrc) {
	case MPLL_0:	PCLKCTRL->sel = MPCLKCTRL_SEL_PLL0; break;
	case MPLL_1:	PCLKCTRL->sel = MPCLKCTRL_SEL_PLL1; break;
	case MPLL_DIV_0:	PCLKCTRL->sel = MPCLKCTRL_SEL_PLL0DIV; break;
	case MPLL_DIV_1:	PCLKCTRL->sel = MPCLKCTRL_SEL_PLL1DIV; break;
	case MPLL_XIN:	PCLKCTRL->sel = MPCLKCTRL_SEL_XIN; break;
	default: return -1;
	}

	PCLKCTRL->div_val = u32_div[searchsrc];
	if ((PCLKCTRL->div_val >= (PCLKCTRL_DIV_MIN+1)) && (PCLKCTRL->div_val <= (div_max+1)))
	{
		PCLKCTRL->div_val -= 1;
	}
	else
	{
		return -1;
	}
	PCLKCTRL->freq = stMicomClockSource[searchsrc]/(PCLKCTRL->div_val+1);

	return 0;
}

static int32 tcc_ckc_micom_is_peri_enabled(int32 id)
{
	addr_t reg = micom_ckc_base+MCKC_PCLKCTRL+(id - PERI_SFMC) * 4;

	return ((ckc_readl(reg) & (1 << PCLKCTRL_EN_SHIFT)) != 0) ? 1 : 0;
}

static int32 tcc_ckc_micom_peri_enable(int32 id)
{
	addr_t reg = micom_ckc_base+MCKC_PCLKCTRL+(id - PERI_SFMC) * 4;

	ckc_writel(ckc_readl(reg) | 1 << PCLKCTRL_EN_SHIFT, reg);

	return 0;
}

static int32 tcc_ckc_micom_peri_disable(int32 id)
{
	addr_t reg = micom_ckc_base+MCKC_PCLKCTRL+(id - PERI_SFMC) * 4;

	ckc_writel(ckc_readl(reg) & ~(1 << PCLKCTRL_EN_SHIFT), reg);

	return 0;
}

static uint32 tcc_ckc_micom_peri_get_rate(int32 id)
{
	addr_t reg = micom_ckc_base+MCKC_PCLKCTRL+(id - PERI_SFMC) * 4;
	uint32 reg_values = ckc_readl(reg);
	uint32 src_freq = 0;
	tPCLKCTRL nPCLKCTRL;

	nPCLKCTRL.sel = (reg_values & (PCLKCTRL_SEL_MASK << PCLKCTRL_SEL_SHIFT)) >> PCLKCTRL_SEL_SHIFT;

	switch(nPCLKCTRL.sel) {
	case PCLKCTRL_SEL_PLL0:
		src_freq = tcc_ckc_micom_pll_get_rate(PLL_MICOM_0); break;
	case PCLKCTRL_SEL_PLL1:
		src_freq = tcc_ckc_micom_pll_get_rate(PLL_MICOM_1); break;
	case PCLKCTRL_SEL_PLL0DIV:
		src_freq = tcc_ckc_micom_pll_get_rate(PLL_MICOM_0) /
			(tcc_ckc_micom_pll_div_get(PLL_MICOM_0)+1);
			break;
	case PCLKCTRL_SEL_PLL1DIV:
		src_freq = tcc_ckc_micom_pll_get_rate(PLL_MICOM_1) /
			(tcc_ckc_micom_pll_div_get(PLL_MICOM_1)+1);
		break;
	case PCLKCTRL_SEL_XIN:
		src_freq = XIN_CLK_RATE; break;
	default :
		return 0;
	}

	nPCLKCTRL.freq = 0;
	nPCLKCTRL.div_val = (reg_values & (PCLKCTRL_DIV_XXX_MASK << PCLKCTRL_DIV_SHIFT)) >> PCLKCTRL_DIV_SHIFT;

	nPCLKCTRL.freq = src_freq / (nPCLKCTRL.div_val + 1);

	return (uint32)nPCLKCTRL.freq;
}

static int32 tcc_ckc_micom_peri_set_rate(int32 id, uint32 rate)
{
	addr_t	reg = micom_ckc_base+MCKC_PCLKCTRL+(id - PERI_SFMC) * 4;
	int32 err = 0;
	tPCLKCTRL	nPCLKCTRL;

	nPCLKCTRL.freq = rate;
	nPCLKCTRL.periname = id;
	nPCLKCTRL.div_val = 0;
	nPCLKCTRL.md = PCLKCTRL_MODE_DIVIDER;
	nPCLKCTRL.sel = MPCLKCTRL_SEL_XIN;

	if (tcc_micom_find_pclk(&nPCLKCTRL, PCLKCTRL_TYPE_XXX) != 0)
	{ //QAC : brace
		//goto tcc_ckc_setperi_failed; //Msg(4:2001) A 'goto' statement has been used. MISRA-C:2004 Rule 14.4
		tcc_pclkctrl_write(reg, PCLKCTRL_MODE_DIVIDER, CKC_DISABLE, MPCLKCTRL_SEL_XIN, 1, PCLKCTRL_TYPE_XXX);
		err = -1;
	}
	else
	{
		//nPCLKCTRL.en = (ckc_readl(reg) & (1<<PCLKCTRL_EN_SHIFT)) ? 1 : 0; //QAC
		if ((ckc_readl(reg) & (1<<PCLKCTRL_EN_SHIFT)) != 0)
		{
			nPCLKCTRL.en = 1;
		}
		else
		{
			nPCLKCTRL.en = 0;
		}

		tcc_pclkctrl_write(reg, nPCLKCTRL.md, nPCLKCTRL.en, nPCLKCTRL.sel, nPCLKCTRL.div_val, PCLKCTRL_TYPE_XXX);
		err= 0;
	}
	
//tcc_ckc_setperi_failed: //Msg(4:2001) A 'goto' statement has been used. MISRA-C:2004 Rule 14.4
//	tcc_pclkctrl_write(reg, PCLKCTRL_MODE_DIVIDER, CKC_DISABLE, MPCLKCTRL_SEL_XIN, 1, PCLKCTRL_TYPE_XXX);
	return err;
}

static int32 tcc_ckc_micom_is_iobus_pwdn(int32 id)
{
	addr_t reg;

	if (id < (32*1)) {
		reg = TCC_SUBSYS_BASE + MCKC_HCLK0;
	}
	else if (id < (32*2)) {
		reg = TCC_SUBSYS_BASE + MCKC_HCLK1;
	}
	else if (id < (32*3)) {
		reg = TCC_SUBSYS_BASE + MCKC_HCLK2;
	}
	else {
		return -1;
	}

	id %= 32;

	return ((ckc_readl(reg) & (1<<id)) != 0) ? 0 : 1;
}

static int32 tcc_ckc_micom_iobus_pwdn(int32 id, bool pwdn)
{
	addr_t reg;

	if (id < (32*1)) {
		reg = TCC_SUBSYS_BASE + MCKC_HCLK0;
	}
	else if (id < (32*2)) {
		reg = TCC_SUBSYS_BASE + MCKC_HCLK1;
	}
	else if (id < (32*3)) {
		reg = TCC_SUBSYS_BASE + MCKC_HCLK2;
	}
	else {
		return -1;
	}

	id %= 32;

	if (pwdn == 1)
	{
		ckc_writel(ckc_readl(reg) & ~(1<<id), reg);
	}
	else
	{
		ckc_writel(ckc_readl(reg) | (1<<id), reg);
	}

	return 0;
}

static int32 tcc_ckc_micom_iobus_swreset(int32 id, bool reset)
{
	addr_t reg;

	if      (id < (32*1)) {
		reg = TCC_SUBSYS_BASE + MCKC_HCLKSWR0;
	}
	else if (id < (32*2)) {
		reg = TCC_SUBSYS_BASE + MCKC_HCLKSWR1;
	}
	else if (id < (32*3)) {
		reg = TCC_SUBSYS_BASE + MCKC_HCLKSWR2;
	}
	else {
		return -1;
	}

	id %= 32;

	if (reset == 1)
	{
		ckc_writel(ckc_readl(reg) & ~(1<<id), reg);
	}
	else
	{
		ckc_writel(ckc_readl(reg) | (1<<id), reg);
	}

	return 0;
}


static inline void tcc_ckc_reset_clock_source(int32 id)
{
	if (id >= MAX_MICOM_CLK_SRC)
	{ //QAC : brace
		return;
	}

	if (id < MAX_MICOM_PLL) {
		stMicomClockSource[id] = tcc_ckc_micom_pll_get_rate(id);
		stMicomClockSource[MAX_MICOM_PLL+id] = stMicomClockSource[id] /
			(tcc_ckc_micom_pll_div_get(id)+1);
	}
}


//static int32 initialized = 0;
void tcc_ckc_init(void)
{
	static int32 initialized = 0;//QAC
#ifdef __GNU_C__//QAC : file scope
static struct tcc_ckc_ops tcc803x_ops = {
	.ckc_swreset			= NULL,
	.ckc_pll_set_rate		= &tcc_ckc_micom_pll_set_rate,
	.ckc_pll_get_rate		= &tcc_ckc_micom_pll_get_rate,
	.ckc_plldiv_set			= &tcc_ckc_micom_pll_div_set,
	.ckc_clkctrl_set_rate		= &tcc_ckc_micom_clkctrl_set_rate,
	.ckc_clkctrl_get_rate		= &tcc_ckc_micom_clkctrl_get_rate,
	.ckc_peri_enable		= &tcc_ckc_micom_peri_enable,
	.ckc_peri_disable		= &tcc_ckc_micom_peri_disable,
	.ckc_peri_set_rate		= &tcc_ckc_micom_peri_set_rate,
	.ckc_peri_get_rate		= &tcc_ckc_micom_peri_get_rate,
	.ckc_is_peri_enabled		= &tcc_ckc_micom_is_peri_enabled,
	.ckc_iobus_pwdn			= &tcc_ckc_micom_iobus_pwdn,
	.ckc_is_iobus_pwdn		= &tcc_ckc_micom_is_iobus_pwdn,
	.ckc_iobus_swreset		= &tcc_ckc_micom_iobus_swreset,
};
#else
static struct tcc_ckc_ops tcc803x_ops = {
	NULL, // int32 (*ckc_swreset)(int32 id, bool reset);
	&tcc_ckc_micom_pll_set_rate,
	&tcc_ckc_micom_pll_get_rate,
	&tcc_ckc_micom_pll_div_set,
	&tcc_ckc_micom_clkctrl_set_rate,
	&tcc_ckc_micom_clkctrl_get_rate,
	&tcc_ckc_micom_peri_enable,
	&tcc_ckc_micom_peri_disable,
	&tcc_ckc_micom_peri_set_rate,
	&tcc_ckc_micom_peri_get_rate,
	&tcc_ckc_micom_is_peri_enabled,
	&tcc_ckc_micom_iobus_pwdn,
	&tcc_ckc_micom_is_iobus_pwdn,
	&tcc_ckc_micom_iobus_swreset,
};
#endif
	
	int32 i;

	if (initialized == 1) {
		return;
	}
	initialized = 1;

	micom_ckc_base = (addr_t )TCC_CKC_BASE;
	/* */
	for (i = 0; i < (MAX_MICOM_PLL * 2); i++) {
		stMicomClockSource[i] = 0;
	}
	stMicomClockSource[MAX_MICOM_PLL * 2] = XIN_CLK_RATE;
	stMicomClockSource[(MAX_MICOM_PLL * 2) + 1] = 0;

	tcc_ckc_set_ops(&tcc803x_ops);
	#ifndef TCC803x_CS
	(void)tcc_ckc_micom_pll_div_set(MPLL_0, 1);
	#endif
	(void)tcc_ckc_micom_pll_div_set(MPLL_1, 1);

	for (i=0 ; i< MAX_MICOM_PLL ; i++) {
		tcc_ckc_reset_clock_source(i);
	}

	//tcc_set_clkctrl(FBUS_CPU, ENABLE, XIN_CLK_RATE);
}
