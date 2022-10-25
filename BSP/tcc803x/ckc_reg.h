/****************************************************************************
 *   FileName    : ckc_reg.h
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
#ifndef TCC_CKC_REG_H
#define TCC_CKC_REG_H

/*
 * CKC Register Offsets
 */
#define CKC_CLKCTRL	0x000
#define CKC_PLLPMS	0x040
#define CKC_PLLCON	0x060
#define CKC_PLLMON	0x080
#define CKC_CLKDIVC	0x0A0
#define CKC_SWRESET	0x0B0
#define CKC_PCLKCTRL	0x0D0

// Dedicated CLKCTRL Register Offsets
#define DCKC_CLKCTRL	0x000
#define DCKC_CLKCTRL2	0x004
#define DCKC_PLLPMS	0x008
#define DCKC_PLLCON	0x00C
#define DCKC_PLLMON	0x010
#define DCKC_PLLDIVC	0x014

// MICOM Register Offsets
#define MCKC_PLL0PMS	0x000
#define MCKC_PLL1PMS	0x00C
#define MCKC_CLKDIVC	0x018
#define MCKC_CLKCTRL	0x01C
#define MCKC_PCLKCTRL	0x024

// MICOM Subsystem Register Offsets
#define MCKC_HCLK0	0x000
#define MCKC_HCLK1	0x004
#define MCKC_HCLK2	0x008
#define MCKC_HCLKSWR0	0x00C
#define MCKC_HCLKSWR1	0x010
#define MCKC_HCLKSWR2	0x014

/*
 * MICOM CLKCTRL Configuration Register
 */
#define MCLKCTRL_SEL_MIN	0
#define MCLKCTRL_SEL_MAX	7
#define MCLKCTRL_SEL_SHIFT	0
#define MCLKCTRL_SEL_MASK	0x7
#define MCLKCTRL_CONFIG_MIN	1
#define MCLKCTRL_CONFIG_MAX	15
#define MCLKCTRL_CONFIG_SHIFT	5
#define MCLKCTRL_CONFIG_MASK	0xF
#define MCLKCTRL_EN_SHIFT	22	// Caution : Do NOT change this value to LOW.
#define MCLKCTRL_DIVSTS_SHIFT	29
#define MCLKCTRL_CLKCHG_SHIFT	31

enum { /* CLKCTRL SEL */
	MCLKCTRL_SEL_XIN=0,
	MCLKCTRL_SEL_PLL0,
	MCLKCTRL_SEL_PLL1,
	MCLKCTRL_SEL_XINDIV,
	MCLKCTRL_SEL_PLL0DIV,
	MCLKCTRL_SEL_PLL1DIV,
	MCLKCTRL_SEL_EXTIN0,
	MCLKCTRL_SEL_EXTIN1
};

/*
 * PLL Configuration Register
 */
#define PLL_MAX_RATE		((3200*1000)*1000UL)	// Max. 3200MHz
#define PLL_MIN_RATE		((25*1000)*1000UL)	// Min.   25MHz
#define PLL_VCO_MAX		((3200*1000)*1000UL)	// Max. 3200MHz
#define PLL_VCO_MIN		((1600*1000)*1000UL)	// Min. 1600MHz
#define PLL_P_MAX		6	// 63	FREF = FIN/p  (4MHz ~ 12MHz)
#define PLL_P_MIN		2	// 1	FREF = FIN/p  (4MHz ~ 12MHz)
#define PLL_P_SHIFT		0
#define PLL_P_MASK		0x3F
#define PLL_M_MAX		1023
#define PLL_M_MIN		64
#define PLL_M_SHIFT		6
#define PLL_M_MASK		0x3FF
#define PLL_S_MAX		6
#define PLL_S_MIN		0
#define PLL_S_SHIFT		16
#define PLL_S_MASK		0x7
#define PLL_SRC_SHIFT		19
#define PLL_SRC_MASK		0x3
#define PLL_BYPASS_SHIFT	21
#define PLL_LOCKST_SHIFT	23
#define PLL_CHGPUMP_SHIFT	24
#define PLL_CHGPUMP_MASK	0x3
#define PLL_LOCKEN_SHIFT	26
#define PLL_RSEL_SHIFT		27
#define PLL_RSEL_MASK		0xF
#define PLL_EN_SHIFT		31

/* PLL Clock Source */
enum{
	PLLSRC_XIN=0,
	PLLSRC_HDMIXI,
	PLLSRC_EXTCLK0,
	PLLSRC_EXTCLK1,
	PLLSRC_MAX
};

/*
 * PCLKCTRL Configuration Register
 */
#define PCLKCTRL_MAX_FCKS	(1600*1000*1000)
#define PCLKCTRL_DIV_MIN	0
#define PCLKCTRL_DIV_DCO_MIN	1
#define PCLKCTRL_DIV_SHIFT	0
#define PCLKCTRL_DIV_XXX_MAX	0xFFF
#define PCLKCTRL_DIV_XXX_MASK	PCLKCTRL_DIV_XXX_MAX
#define PCLKCTRL_DIV_YYY_MAX	0xFFFFFF
#define PCLKCTRL_DIV_YYY_MASK	PCLKCTRL_DIV_YYY_MAX
#define PCLKCTRL_SEL_MIN	0
#define PCLKCTRL_SEL_MAX	26
#define PCLKCTRL_SEL_SHIFT	24
#define PCLKCTRL_SEL_MASK	0x1F
#define PCLKCTRL_EN_SHIFT	29
#define PCLKCTRL_OUTEN_SHIFT	30
#define PCLKCTRL_MD_SHIFT	31

/* PCLK Type */
typedef enum {
	PCLKCTRL_TYPE_XXX=0,
	PCLKCTRL_TYPE_YYY,
	PCLKCTRL_TYPE_MAX
} tPCLKTYPE;

/* PCLK Mode Selection */
enum {
	PCLKCTRL_MODE_DCO=0,
	PCLKCTRL_MODE_DIVIDER,
	PCLKCTRL_MODE_MAX
};

enum {
	MPCLKCTRL_SEL_PLL0 = 0,
	MPCLKCTRL_SEL_PLL1,
	MPCLKCTRL_SEL_XIN = 5,
	MPCLKCTRL_SEL_PLL0DIV = 10,
	MPCLKCTRL_SEL_PLL1DIV = 11,
	MPCLKCTRL_SEL_XINDIV = 23,
};

#endif
