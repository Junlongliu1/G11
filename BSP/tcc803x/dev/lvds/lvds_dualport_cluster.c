/****************************************************************************************
 *   FileName    : lvds_dualport_cluster.c
 *   Description :
 ****************************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips Inc.
 *   All rights reserved

 This source code contains confidential information of Telechips.
 Any unauthorized use without a written permission of Telechips including not limited
 to re-distribution in source or binary form is strictly prohibited.
 This source code is provided AS IS and nothing contained in this source code
 shall constitute any express or implied warranty of any kind, including without limitation,
 any warranty of merchantability, fitness for a particular purpose or non-infringement of any patent,
 copyright or other third party intellectual property right.
 No warranty is made, express or implied, regarding the information accuracy,
 completeness, or performance.
 In no event shall Telechips be liable for any claim, damages or other liability arising from,
 out of or in connection with this source code or the use in the source code.
 This source code is provided subject to the terms of a Mutual Non-Disclosure Agreement
 between Telechips and Company.
 *
 ****************************************************************************************/

#include "lvds_dualport_cluster.h"
#include <osal.h>

#include <bsp.h>
#include "reg_phys.h"

#include "debug.h"
#include "lvds_wrap.h"
#include "lvds_phy.h"
#include "gpio.h"

#define DISP0_STATUS 0x12000050
#define DISP1_STATUS 0x12000150
#define DISP2_STATUS 0x12000250

#define DDI_SOURCE DISP0_STATUS

#define DISP1_LCD_ON TCC_GPB(17)
#define DISP1_LCD_RESET TCC_GPB(18)
#define DISP2_LCD_ON TCC_GPC(8)
#define DISP2_LCD_RESET TCC_GPC(9)


#define MUXSRC_BL_PWON	DISP1_LCD_ON
#define MUXSRC_BL_RESET DISP1_LCD_RESET
#define MUXSRC_BL_ON    TCC_GPMA(26)

#define TM123XDHP90_LVDS_PORT_MAIN  LVDS_PHY_PORT_D0
#define TM123XDHP90_LVDS_PORT_SUB   LVDS_PHY_PORT_D1

void BL_On(void)
{
	(void)gpio_config(MUXSRC_BL_PWON, (GPIO_FUNC(0) | GPIO_OUTPUT));//DISP2_LCD_ON//R289
	(void)gpio_set(MUXSRC_BL_PWON,1);
//	OSAL_TaskDelay(1);//recomended 20 us
	(void)gpio_config(MUXSRC_BL_RESET, (GPIO_FUNC(0) | GPIO_OUTPUT));//DISP2_LCD_ON//R289
	(void)gpio_set(MUXSRC_BL_RESET,1);
	(void)gpio_config(MUXSRC_BL_ON, (GPIO_FUNC(0) | GPIO_OUTPUT));//DISP2_LCD_ON//R289
	(void)gpio_set(MUXSRC_BL_ON,1);
}


void init_lvds_preset(void)
{
	uint32 idx;
	uint32 txout_sel0[TXOUT_MAX_LINE][TXOUT_DATA_PER_LINE] = {
		{TXOUT_G_D(0), TXOUT_R_D(5), TXOUT_R_D(4), TXOUT_R_D(3), TXOUT_R_D(2), TXOUT_R_D(1), TXOUT_R_D(0)},
		{TXOUT_B_D(1), TXOUT_B_D(0), TXOUT_G_D(5), TXOUT_G_D(4), TXOUT_G_D(3), TXOUT_G_D(2), TXOUT_G_D(1)},
		{TXOUT_DE, TXOUT_VS, TXOUT_HS, TXOUT_B_D(5), TXOUT_B_D(4), TXOUT_B_D(3), TXOUT_B_D(2)},
		{TXOUT_DUMMY, TXOUT_B_D(7), TXOUT_B_D(6), TXOUT_G_D(7), TXOUT_G_D(6), TXOUT_R_D(7), TXOUT_R_D(6)}
	};
	uint32 txout_sel1[TXOUT_MAX_LINE][TXOUT_DATA_PER_LINE] = {
		{TXOUT_G_D(0), TXOUT_R_D(5), TXOUT_R_D(4), TXOUT_R_D(3), TXOUT_R_D(2), TXOUT_R_D(1), TXOUT_R_D(0)},
		{TXOUT_B_D(1), TXOUT_B_D(0), TXOUT_G_D(5), TXOUT_G_D(4), TXOUT_G_D(3), TXOUT_G_D(2), TXOUT_G_D(1)},
		{TXOUT_DE, TXOUT_DUMMY, TXOUT_DUMMY, TXOUT_B_D(5), TXOUT_B_D(4), TXOUT_B_D(3), TXOUT_B_D(2)},
		{TXOUT_DUMMY, TXOUT_B_D(7), TXOUT_B_D(6), TXOUT_G_D(7), TXOUT_G_D(6), TXOUT_R_D(7), TXOUT_R_D(6)}
	};

	volatile uint32 vl_col = 1920;
	volatile uint32 lcdc_mux = 2;

	LVDS_WRAP_SetAccessCode();
	LVDS_WRAP_SetConfigure(0, 0, vl_col);
	for(idx = TS_SWAP_CH0; idx < TS_SWAP_CH_MAX; idx++){
		LVDS_WRAP_SetDataSwap(idx, idx);
	}
	LVDS_WRAP_SetMuxOutput(DISP_MUX_TYPE, 0, lcdc_mux, 1); //2 == LCDC_MUX_IDX2
	LVDS_WRAP_SetMuxOutput(TS_MUX_TYPE, TS_MUX_IDX0, TS_MUX_PATH_CORE, 1);
	LVDS_WRAP_SetDataArray(TS_MUX_IDX0, txout_sel0);
	LVDS_WRAP_SetMuxOutput(TS_MUX_TYPE, TS_MUX_IDX1, TS_MUX_PATH_CORE, 1);
	LVDS_WRAP_SetDataArray(TS_MUX_IDX1, txout_sel1);

}

void init_lvds(void)
{
	volatile uint32 clk_freq = 88200000; //95000000;
	volatile uint32 vcm = 1200;
	volatile uint32 vsw = 500;

	volatile uint32 upsample_ratio = LVDS_PHY_GetUpsampleRatio(
			TM123XDHP90_LVDS_PORT_MAIN, TM123XDHP90_LVDS_PORT_SUB,
			clk_freq);
	volatile uint32 ref_cnt = LVDS_PHY_GetRefCnt(
			TM123XDHP90_LVDS_PORT_MAIN, TM123XDHP90_LVDS_PORT_SUB,
			clk_freq, upsample_ratio);
	//printf("%s: LVDS_WRAP clk_freq %d\n", __func__, clk_freq);

//	printf("%s: LVDS_PHY\n", __func__);
//	OSAL_TaskDelay(10);

	/* LVDS PHY Clock Enable */
	LVDS_PHY_ClockEnable(TM123XDHP90_LVDS_PORT_MAIN, 1);
	LVDS_PHY_ClockEnable(TM123XDHP90_LVDS_PORT_SUB, 1);

	LVDS_PHY_SWReset(TM123XDHP90_LVDS_PORT_MAIN, 1);
	LVDS_PHY_SWReset(TM123XDHP90_LVDS_PORT_SUB, 1);
//	OSAL_TaskDelay(1);
	LVDS_PHY_SWReset(TM123XDHP90_LVDS_PORT_MAIN, 0);
	LVDS_PHY_SWReset(TM123XDHP90_LVDS_PORT_SUB, 0);

	/* LVDS PHY Strobe setup */
	LVDS_PHY_SetStrobe(TM123XDHP90_LVDS_PORT_MAIN, 1, 1);
	LVDS_PHY_SetStrobe(TM123XDHP90_LVDS_PORT_SUB, 1, 1);

	LVDS_PHY_StrobeConfig(TM123XDHP90_LVDS_PORT_MAIN, TM123XDHP90_LVDS_PORT_SUB, upsample_ratio,
			LVDS_PHY_INIT, vcm, vsw);

	LVDS_PHY_LaneEnable(TM123XDHP90_LVDS_PORT_MAIN, 0);
	LVDS_PHY_LaneEnable(TM123XDHP90_LVDS_PORT_SUB, 0);
	LVDS_PHY_SetPortOption(TM123XDHP90_LVDS_PORT_MAIN, 0, 0, 0, 0x0, 0x0);
	LVDS_PHY_SetPortOption(TM123XDHP90_LVDS_PORT_SUB, 1, 0, 1, 0x0, 0x7);

	LVDS_PHY_SetLaneSwap(TM123XDHP90_LVDS_PORT_MAIN, LVDS_PHY_CLK_LANE, LVDS_PHY_DATA3_LANE);
	LVDS_PHY_SetLaneSwap(TM123XDHP90_LVDS_PORT_MAIN, LVDS_PHY_DATA0_LANE, LVDS_PHY_CLK_LANE);
	LVDS_PHY_SetLaneSwap(TM123XDHP90_LVDS_PORT_MAIN, LVDS_PHY_DATA1_LANE, LVDS_PHY_DATA2_LANE);
	LVDS_PHY_SetLaneSwap(TM123XDHP90_LVDS_PORT_MAIN, LVDS_PHY_DATA2_LANE, LVDS_PHY_DATA1_LANE);
	LVDS_PHY_SetLaneSwap(TM123XDHP90_LVDS_PORT_MAIN, LVDS_PHY_DATA3_LANE, LVDS_PHY_DATA0_LANE);

	LVDS_PHY_SetLaneSwap(TM123XDHP90_LVDS_PORT_SUB, LVDS_PHY_CLK_LANE, LVDS_PHY_DATA0_LANE);
	LVDS_PHY_SetLaneSwap(TM123XDHP90_LVDS_PORT_SUB, LVDS_PHY_DATA0_LANE, LVDS_PHY_DATA1_LANE);
	LVDS_PHY_SetLaneSwap(TM123XDHP90_LVDS_PORT_SUB, LVDS_PHY_DATA1_LANE, LVDS_PHY_DATA2_LANE);
	LVDS_PHY_SetLaneSwap(TM123XDHP90_LVDS_PORT_SUB, LVDS_PHY_DATA2_LANE, LVDS_PHY_CLK_LANE);
	LVDS_PHY_SetLaneSwap(TM123XDHP90_LVDS_PORT_SUB, LVDS_PHY_DATA3_LANE, LVDS_PHY_DATA3_LANE);

	LVDS_PHY_StrobeConfig(TM123XDHP90_LVDS_PORT_MAIN, TM123XDHP90_LVDS_PORT_SUB, upsample_ratio,
			LVDS_PHY_READY, vcm, vsw);
	LVDS_PHY_SetFcon(TM123XDHP90_LVDS_PORT_MAIN, LVDS_PHY_FCON_AUTOMATIC, 0, 0, ref_cnt);	// fcon value, for 44.1Mhz
	LVDS_PHY_SetFcon(TM123XDHP90_LVDS_PORT_SUB, LVDS_PHY_FCON_AUTOMATIC, 0, 0, ref_cnt);	  // fcon value, for 44.1Mhz
	LVDS_PHY_FConEnable(TM123XDHP90_LVDS_PORT_MAIN, 1);
	LVDS_PHY_FConEnable(TM123XDHP90_LVDS_PORT_SUB, 1);
	LVDS_PHY_SetCFcon(TM123XDHP90_LVDS_PORT_MAIN, LVDS_PHY_FCON_AUTOMATIC, 1);
	LVDS_PHY_SetCFcon(TM123XDHP90_LVDS_PORT_SUB, LVDS_PHY_FCON_AUTOMATIC, 1);
	LVDS_PHY_CheckPLLStatus(TM123XDHP90_LVDS_PORT_MAIN, TM123XDHP90_LVDS_PORT_SUB);
//	OSAL_TaskDelay(20);

	LVDS_PHY_StrobeConfig(TM123XDHP90_LVDS_PORT_MAIN, TM123XDHP90_LVDS_PORT_SUB, upsample_ratio,
			LVDS_PHY_START, vcm, vsw);


	/* LVDS PHY digital setup */
	LVDS_PHY_SetFormat(TM123XDHP90_LVDS_PORT_MAIN, 0, 1, 0, upsample_ratio);
	LVDS_PHY_SetFormat(TM123XDHP90_LVDS_PORT_SUB, 0, 1, 0, upsample_ratio);

	LVDS_PHY_SetFifoEnableTiming(TM123XDHP90_LVDS_PORT_MAIN, 0x3);
	LVDS_PHY_SetFifoEnableTiming(TM123XDHP90_LVDS_PORT_SUB, 0x3);

	/* LVDS PHY Main/Sub Lane Disable */
	LVDS_PHY_LaneEnable(TM123XDHP90_LVDS_PORT_MAIN, 0);
	LVDS_PHY_LaneEnable(TM123XDHP90_LVDS_PORT_SUB, 0);

	/* LVDS PHY Main port FIFO Disable */
	LVDS_PHY_FifoEnable(TM123XDHP90_LVDS_PORT_MAIN, 0);
	LVDS_PHY_FifoEnable(TM123XDHP90_LVDS_PORT_SUB, 0);

	LVDS_PHY_FifoReset(TM123XDHP90_LVDS_PORT_MAIN, 1);
	LVDS_PHY_FifoReset(TM123XDHP90_LVDS_PORT_SUB, 1);
//	OSAL_TaskDelay(1);
	LVDS_PHY_FifoReset(TM123XDHP90_LVDS_PORT_MAIN, 0);
	LVDS_PHY_FifoReset(TM123XDHP90_LVDS_PORT_SUB, 0);

	/* LVDS PHY Main/Sub port FIFO Enable */
	LVDS_PHY_FifoEnable(TM123XDHP90_LVDS_PORT_MAIN, 1);
	LVDS_PHY_FifoEnable(TM123XDHP90_LVDS_PORT_SUB, 1);

	/* LVDS PHY Main/Sub port Lane Enable */
	LVDS_PHY_LaneEnable(TM123XDHP90_LVDS_PORT_MAIN, 1);
	LVDS_PHY_LaneEnable(TM123XDHP90_LVDS_PORT_SUB, 1);

	if((LVDS_PHY_CheckStatus(TM123XDHP90_LVDS_PORT_MAIN, TM123XDHP90_LVDS_PORT_SUB) & 0x1) == 0) {
//		printf("%s: LVDS_PHY Primary port(%d) is in death \n",
	//			__func__, TM123XDHP90_LVDS_PORT_MAIN);
	}
	if((LVDS_PHY_CheckStatus(TM123XDHP90_LVDS_PORT_MAIN, TM123XDHP90_LVDS_PORT_SUB) & 0x2) == 0) {
	//	printf("%s: LVDS_PHY Secondary port(%d) is in death \n",
		//		__func__, TM123XDHP90_LVDS_PORT_SUB);
	}

//	printf("%s: LVDS_PHY Finished\n", __func__);

}

int32 get_lvds_signal_ready(void)
{
    int32 ret = 0;
    ret = ((readl(DDI_SOURCE) >> 30) & 1);
    return ret;
}

int32 get_reset_is_fail(void)
{
    int32 ret = 0;
    ret = (readl(0x1B9401F4) & 0x1);
    return ret;
}

void reset_tx_splitter(void)
{
	writel(0x10000, 0x1b9361fc);
	OSAL_TaskDelay(1);
	writel(0x10001, 0x1b9361fc);
	
	init_lvds_preset();
	writel(0x1A190504 , 0x1b940154);
	writel(0xffffffff , 0x1b9401f0);

	init_lvds();

	writel(0 		  , 0x1b9401f0);
//	masking_lvds_err_interrupt();

}

void masking_lvds_err_interrupt(void)
{
	writel(0xffffffff, 0x1b940208);//TS_IREQ_MASK
	writel(0xffffffff, 0x1b94020c);//LVDS_IREQ_MASK
}

