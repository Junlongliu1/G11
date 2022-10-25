/****************************************************************************
 *   FileName    : ictc.c
 *   Description : to use the ICTC for Telechips chipset
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
#include "ictc.h"
#include <bsp_int.h>

#define ictc_readl		readl
#define ictc_writel		writel

#define ICTC_TRACE(f, a...) 

#define ICTC_CH_NUM 	3

#define TCC_ICTC0_BASE	TCC_ICTC_BASE
#define TCC_ICTC1_BASE	(TCC_ICTC0_BASE + 0x10000)
#define TCC_ICTC2_BASE	(TCC_ICTC1_BASE + 0x10000)
#define ICTC_CH_OFFSET	0x10000

#define ICTC_F_IN_SEL	0		// see Table 11.2 Source of External Interrupt in TCC8030 Datasheet

// clk offset
#define ICTC0_PCLK_OFFSET	0x68
#define ICTC1_PCLK_OFFSET	0x6C
#define ICTC2_PCLK_OFFSET	0x70

// source clk
#define ICTC_SOURCE_CLK		0x5	// XIN direct

// ICTC_EN register
#define ICTC_EN					(1UL<<31) //Compiler warning : 
#define TCLK_EN					(1UL<<21)
#define TS_CNT_EN				(1UL<<20)
#define FLT_CNT_EN				(1UL<<19)
#define TO_CNT_EN				(1UL<<18)
#define FEDGE_CNT_EN			(1UL<<17)
#define PD_CMP_CNT_EN			(1UL<<16)

Type_uByte nubTimeoutFlag = 0;

static void __iomem *ictc_en_reg;
static void __iomem *ictc_ctrl_reg;
static void __iomem	*ictc_irq_reg;

static void __iomem	*ictc_to_vl_reg;
static void __iomem	*ictc_redge_vl_reg;
static void __iomem	*ictc_fedge_vl_reg;
static void __iomem	*ictc_prd_rnd_vl_reg;
static void __iomem	*ictc_dt_rnd_vl_reg;
//static void __iomem	*ictc_fedge_cnt_reg;	//QAC
// Rule 8.7 - Objects shall be defined at block scope if they are only accessed from within a single function.
static void __iomem	*ictc_prd_cnt_pvl_reg;
static void __iomem	*ictc_prd_cnt_ppvl_reg;
static void __iomem	*ictc_dt_cnt_pvl_reg;
static void __iomem	*ictc_dt_cnt_ppvl_reg;
static void __iomem	*ictc_fedge_det_cnt_cvl_reg;
static void __iomem	*ictc_fedge_det_cnt_pvl_reg;
//static void __iomem	*ictc_redge_ts_cnt_reg;	//QAC
//static void __iomem	*ictc_fedge_ts_cnt_reg;	//QAC
//static uint32	f_in_sel;


typedef struct 
{
	int32 ch;
	ictc_userCb cb;
} ictc_user_data;

static ictc_user_data user_priv[3];

static void ictc_interrupt_handler(void *arg);
static void ictc_clear_counter(uint32 ch);
static void ictc_clear_interrupt(uint32 ch);
static void ictc_set_tclk(uint32 ch);
static void ictc_stop(uint32 ch);

void ictc_set_TimeOutCount_Flag(Type_uByte ubFlag)
{
	nubTimeoutFlag = ubFlag;
}

Type_uByte ictc_get_TimeOutCount_Flag()
{
	return nubTimeoutFlag;
}

static void ictc_interrupt_handler(void *arg)
{
	ictc_user_data *pdata = (ictc_user_data*)arg;
	if ( pdata!=0)
	{
		int32 ch;
		ch = pdata->ch;
		ICTC_TRACE("\x1b[1;35m[%s:%d] ICTC interrupt handler \x1b[0m\n", __func__, __LINE__);

		if (pdata->cb!=0)
		{
			pdata->cb(ch, ictc_get_period_pvl_count(ch), ictc_get_duty_pvl_count(ch));
		}


		// clear interrupt flag & counter
		//ictc_stop(ch);
		ictc_clear_counter(ch);
		ictc_clear_interrupt(ch);
		//ictc_start(ch);
		
		//set timeout counter flag
		ictc_set_TimeOutCount_Flag(1);
	}
}

int32 ictc_get_fedge_det_cnt_cvl(uint32 ch)
{
	void __iomem *reg = ictc_fedge_det_cnt_cvl_reg+(ch*ICTC_CH_OFFSET);
	
	return ictc_readl(reg);
}

int32 ictc_get_fedge_det_cnt_pvl(uint32 ch)
{
	void __iomem *reg = ictc_fedge_det_cnt_pvl_reg+(ch*ICTC_CH_OFFSET);
	
	return ictc_readl(reg);
}

int32 ictc_get_period_pvl_count(uint32 ch)
{
	void __iomem *reg = ictc_prd_cnt_pvl_reg+(ch*ICTC_CH_OFFSET);
	
	return ictc_readl(reg);
}

int32 ictc_get_period_ppvl_count(uint32 ch)
{
	void __iomem *reg = ictc_prd_cnt_ppvl_reg+(ch*ICTC_CH_OFFSET);
	
	return ictc_readl(reg);
}

int32 ictc_get_duty_pvl_count(uint32 ch)
{
	void __iomem *reg = ictc_dt_cnt_pvl_reg+(ch*ICTC_CH_OFFSET);
	
	return ictc_readl(reg);
}

int32 ictc_get_duty_ppvl_count(uint32 ch)
{
	void __iomem *reg = ictc_dt_cnt_ppvl_reg+(ch*ICTC_CH_OFFSET);
	
	return ictc_readl(reg);
}

int32 ictc_set_clk(uint32 ch, uint32 divide_val)
{
	void __iomem *ctrl_reg = ictc_ctrl_reg+(ch*ICTC_CH_OFFSET);
	void __iomem *reg;

	ictc_writel( ictc_readl(ctrl_reg)|(divide_val<<22), ctrl_reg );
	
	switch(ch)
	{
		case 0:
			reg = (void __iomem *)TCC_CKC_BASE+ICTC0_PCLK_OFFSET;
			break;
		case 1:
			reg = (void __iomem *)TCC_CKC_BASE+ICTC1_PCLK_OFFSET;
			break;
		case 2:
			reg = (void __iomem *)TCC_CKC_BASE+ICTC2_PCLK_OFFSET;
			break;
		default:
			reg = NULL;
			break;
	}

	if(reg == NULL)
	{
		return -1;
	}

	// init pclk
	ictc_writel( 0, reg);
	// source clock selection
	ictc_writel( ictc_readl(reg)|(ICTC_SOURCE_CLK<<24), reg );
	// divider-value configuration
	ictc_writel( ictc_readl(reg)|0x1, reg );
	// clock devider enable
	ictc_writel( ictc_readl(reg)|(ENABLE<<29), reg );
	// clock out enable
	ictc_writel( ictc_readl(reg)|(ENABLE<<30), reg );

	ictc_set_tclk(ch);

	return 0;
}

void ictc_set_port(uint32 ch, uint32 gpio_num)
{
	void __iomem *reg = ictc_ctrl_reg+(ch*ICTC_CH_OFFSET);
	static uint32	f_in_sel;
	f_in_sel = gpio_num;

	ictc_writel( 0x1<<16, reg);	// enable only duty intr.
	// set F_IN_SEL
	ictc_writel( ictc_readl(reg)|f_in_sel, reg );
}
void ictc_set_filter_fmode(uint32 ch, uint32 filtermode)
{
	void __iomem *reg = ictc_ctrl_reg+(ch*ICTC_CH_OFFSET);
	volatile uint32 readreg;
	
	readreg = ictc_readl(reg);
	ictc_writel( ((readreg)|((filtermode)<<20)), reg);
}

void ictc_set_filter_rmode(uint32 ch, uint32 filtermode)
{
	void __iomem *reg = ictc_ctrl_reg+(ch*ICTC_CH_OFFSET);
	volatile uint32  readreg;
	
	readreg = ictc_readl(reg);
	ictc_writel( ((readreg)|((filtermode)<<18)), reg);
}

static void ictc_clear_counter(uint32 ch)
{
	void __iomem *en_reg = ictc_en_reg+(ch*ICTC_CH_OFFSET);
	
	ictc_writel( ictc_readl(en_reg)|0xF, en_reg );
	ictc_writel( ictc_readl(en_reg)&(~0xF), en_reg );
}

static void ictc_clear_interrupt(uint32 ch)
{
	uint32 irq_en;
	void __iomem *irq_reg = ictc_irq_reg+(ch*ICTC_CH_OFFSET);

	irq_en = ictc_readl(irq_reg) >> 26;
	ictc_writel( ictc_readl(irq_reg)|0xFC00, irq_reg );
	ictc_writel( 0, irq_reg );

	ictc_set_interrupt(ch, irq_en);
}

void ictc_set_callback(uint32 ch, ictc_userCb cb)
{
	user_priv[ch].ch = ch;
	user_priv[ch].cb = cb;
}
void ictc_set_interrupt(uint32 ch, uint32 irq_en)
{
	void __iomem *irq_reg = ictc_irq_reg+(ch*ICTC_CH_OFFSET);

	// clear interrupt flag
	ictc_writel( ictc_readl(irq_reg)|(0xFF00), irq_reg );
	ictc_writel( 0, irq_reg );

	ictc_writel( (irq_en << 24), irq_reg );

	switch(ch)
	{
		case 0:
			(void)BSP_IntVectSet(INT_IC_TC0, PRIORITY_NO_MEAN, INT_TYPE_LEVEL_HIGH, ictc_interrupt_handler, &user_priv[ch]);
			BSP_IntSrcEn(INT_IC_TC0);
			break;
		case 1:
			(void)BSP_IntVectSet(INT_IC_TC1, PRIORITY_NO_MEAN, INT_TYPE_LEVEL_HIGH, ictc_interrupt_handler, &user_priv[ch]);
			BSP_IntSrcEn(INT_IC_TC1);
			break;
		case 2:
			(void)BSP_IntVectSet(INT_IC_TC2, PRIORITY_NO_MEAN, INT_TYPE_LEVEL_HIGH, ictc_interrupt_handler, &user_priv[ch]);
			BSP_IntSrcEn(INT_IC_TC2);
			break;
		default:
			// error
			break;
	}
}

void ictc_set_to_vl(uint32 ch, uint32 value)
{
	void __iomem *reg = ictc_to_vl_reg+(ch*ICTC_CH_OFFSET);

	ictc_writel( 0x0FFFFFFF&value, reg );
}

void ictc_set_redge_vl(uint32 ch, uint32 value)
{
	void __iomem *reg = ictc_redge_vl_reg+(ch*ICTC_CH_OFFSET);

	ictc_writel( 0x0FFFFFFF&value, reg );
}

void ictc_set_fedge_vl(uint32 ch, uint32 value)
{
	void __iomem *reg = ictc_fedge_vl_reg+(ch*ICTC_CH_OFFSET);

	ictc_writel( 0x0FFFFFFF&value, reg );
}

void ictc_set_prd_rnd_vl(uint32 ch, uint32 value)
{
	void __iomem *reg = ictc_prd_rnd_vl_reg+(ch*ICTC_CH_OFFSET);

	ictc_writel( 0x0FFFFFFF&value, reg );
}

void ictc_set_dt_rnd_vl(uint32 ch, uint32 value)
{
	void __iomem *reg = ictc_dt_rnd_vl_reg+(ch*ICTC_CH_OFFSET);

	ictc_writel( 0x0FFFFFFF&value, reg );
}

void ictc_reset(uint32 ch)
{
	ictc_stop(ch);

	ictc_set_interrupt(ch, 0/*NULL*/); //change NULL to 0 because GHC define NULL to void*.
	ictc_clear_counter(ch);
	
	ictc_start(ch);
}

static void ictc_set_tclk(uint32 ch)
{
	void __iomem *reg = ictc_en_reg+(ch*ICTC_CH_OFFSET);

	// Pre-Scaler Counter Enable
	ictc_writel( ictc_readl(reg)|TCLK_EN, reg );
}

void ictc_start(uint32 ch)
{
	void __iomem *reg = ictc_en_reg+(ch*ICTC_CH_OFFSET);
	
	ICTC_TRACE("\x1b[1;31m[%s:%d]\x1b[0m\n", __func__, __LINE__);

	// Filter Counter Enable
	ictc_writel( ictc_readl(reg)|FLT_CNT_EN, reg );
	
	// Falling Edge Counter Enable
	ictc_writel( ictc_readl(reg)|FEDGE_CNT_EN, reg );

	ictc_writel( ictc_readl(reg)|TO_CNT_EN, reg );
	
	// Period/Duty CMP Counter Enable
	ictc_writel( ictc_readl(reg)|PD_CMP_CNT_EN, reg );
	
	ictc_writel( ictc_readl(reg)|TS_CNT_EN, reg );

	ictc_set_to_vl(ch, 0x225510);  /*1.5s/(1/(12MHz/8))             1.5s/  1 / (ictc clock /dvided clock) (conversion)  */

	ictc_set_interrupt( ch, 
					TO_CNT_FULL_IRQ
                    );
    
	ictc_set_prd_rnd_vl(ch, 0x0);
	ictc_set_dt_rnd_vl(ch, 0x0);

	// start ICTC
	ictc_writel( ictc_readl(reg)|ICTC_EN, reg );

}

static void ictc_stop(uint32 ch)
{
	void __iomem *reg = ictc_en_reg+(ch*ICTC_CH_OFFSET);
	
	ICTC_TRACE("\x1b[1;34m[%s:%d]\x1b[0m\n", __func__, __LINE__);

	// stop ICTC
	ictc_writel( ictc_readl(reg)&(~ICTC_EN), reg );
}

void ictc_init(void)
{
	//static void __iomem	*ictc_fedge_cnt_reg;	//warning. set but not used
	//static void __iomem	*ictc_redge_ts_cnt_reg;	//warning. set but not used
	//static void __iomem	*ictc_fedge_ts_cnt_reg;	//warning. set but not used

	ICTC_TRACE("\x1b[1;32m[%s:%d]ICTC driver init \x1b[0m\n", __func__, __LINE__);

	ictc_en_reg		= (void __iomem *)TCC_ICTC_BASE + EN_CTRL;
	ictc_ctrl_reg	= (void __iomem *)TCC_ICTC_BASE + MODE_CTRL;
	ictc_irq_reg	= (void __iomem *)TCC_ICTC_BASE + IRQ_CTRL;

	ictc_to_vl_reg		= (void __iomem *)TCC_ICTC_BASE + TIME_OUT;
	ictc_redge_vl_reg	= (void __iomem *)TCC_ICTC_BASE + RISING_EDGE;
	ictc_fedge_vl_reg	= (void __iomem *)TCC_ICTC_BASE + FALLING_EDGE;
	ictc_prd_rnd_vl_reg	= (void __iomem *)TCC_ICTC_BASE + PRD_CMP_RND;
	ictc_dt_rnd_vl_reg	= (void __iomem *)TCC_ICTC_BASE + DUTY_CMP_RND;
	//ictc_fedge_cnt_reg	= (void __iomem *)TCC_ICTC_BASE + FALLING_EDGE_CNT;

	ictc_prd_cnt_pvl_reg	= (void __iomem *)TCC_ICTC_BASE + PRD_CNT_PVL;
	ictc_prd_cnt_ppvl_reg	= (void __iomem *)TCC_ICTC_BASE + PRD_CNT_PPVL;
	ictc_dt_cnt_pvl_reg		= (void __iomem *)TCC_ICTC_BASE + DT_CNT_PVL;
	ictc_dt_cnt_ppvl_reg	= (void __iomem *)TCC_ICTC_BASE + DT_CNT_PPVL;
	ictc_fedge_det_cnt_cvl_reg	= (void __iomem *)TCC_ICTC_BASE + FEDGE_DET_CNT_CVL;
	ictc_fedge_det_cnt_pvl_reg	= (void __iomem *)TCC_ICTC_BASE + FEDGE_DET_CNT_PVL;
	//ictc_redge_ts_cnt_reg	= (void __iomem *)TCC_ICTC_BASE + REDGE_TS_CNT;
	//ictc_fedge_ts_cnt_reg	= (void __iomem *)TCC_ICTC_BASE + FEDGE_TS_CNT;

#ifdef ICTC_TEST // for test
{
	uint32 i = 0;

	for(i=0;i<3;i++)
	{
	ictc_set_port(i, ICTC_F_IN_SEL+i);		// set f_in_sel
	ictc_set_clk(i, 8);					// set clk
	
	ictc_set_to_vl(i, 0x00ffffff);
	ictc_set_redge_vl(i, 0x00000300);
	ictc_set_fedge_vl(i, 0x00000300);

	ictc_start(i);
	}
}

#endif

}

