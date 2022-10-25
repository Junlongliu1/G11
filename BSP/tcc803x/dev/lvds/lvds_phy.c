/*
 * kernel/drivers/video/fbdev/tcc-fb/vioc/vioc_lvds.c
 * Author:  <linux@telechips.com>
 * Created: June 10, 2008
 * Description: TCC VIOC h/w block
 *
 * Copyright (C) 2008-2009 Telechips
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see the file COPYING, or write
 * to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <types.h>
#include <bsp.h>
#include "reg_phys.h"
#include "lvds_phy.h"
#include "debug.h"
#include <osal.h>

#ifndef HwLVDS_PHY_DUAL
#define HwLVDS_PHY_DUAL		(0x1B942000)
#endif

#ifndef HwLVDS_PHY_SINGLE
#define HwLVDS_PHY_SINGLE		(0x1B941000)
#endif

#define REG_LVDS_PORT_DUAL(port)		(volatile void __iomem *)(HwLVDS_PHY_DUAL + (port << 10))
#define REG_LVDS_PORT_SINGLE(port)		(volatile void __iomem *)(HwLVDS_PHY_SINGLE + (port << 10))

#define LVDS_PHY_VCO_RANGE_MIN		(560000000)			// 560Mhz
#define LVDS_PHY_VCO_RANGE_MAX		(1120000000)		// 1120Mhz
#define LVDS_PHY_UPSAMPLE_RATIO_MAX		(0x4)			// 0~4

#ifndef printf
extern void IO_DBG_SerialPrintf_(int8 *format, ...);
#define	printf	IO_DBG_SerialPrintf_ 
#endif

#ifndef pr_err
#define pr_err printf
#endif

#define LVDS_TYPE_SINGLE 0
#define LVDS_TYPE_DUAL 1

static volatile void __iomem *lvds_phy_regs[] = {
	REG_LVDS_PORT_SINGLE(0),
	NULL,
	REG_LVDS_PORT_DUAL(2),
	REG_LVDS_PORT_DUAL(3)
};

/* for upsample ratio calculation
 * n = upsample ratio
 * X = 2^n (X = 1, 2, 4, 8 16)
 */
static unsigned int ref_ratio_arr[5][2] = {
	{ 0, 1},
	{ 1, 2},
	{ 2, 4},
	{ 3, 8},
	{ 4, 16}
};

static void LVDS_PHY_LaneSwap(uint32 lvds_type, uint32 lvds_main, uint32 lvds_sub, uint32* lane_main, uint32 *lane_sub);
static void LVDS_PHY_GetCalibrationLevel(unsigned int vcm, unsigned int vsw, unsigned int *vcmcal, unsigned int *swingcal);
static void lvds_phy_udelay(unsigned int n);

static void lvds_phy_udelay(unsigned int n)
{
	unsigned int i;
	for (i = 0; i < (n * 500); i++)
		NOP_DELAY();
}

static volatile void __iomem* LVDS_PHY_GetAddress(unsigned int port)
{
	if(port >= LVDS_PHY_PORT_MAX)
		return NULL;

	return lvds_phy_regs[port];
}

void LVDS_PHY_Init(uint32 lvds_type, uint32 lvds_main, uint32 lvds_sub, uint32 upsample_ratio, uint32 ref_cnt, uint32 vcm, uint32 vsw, uint32 *LVDS_LANE_MAIN, uint32 *LVDS_LANE_SUB)
{
	uint32 status;

	if((lvds_type != LVDS_TYPE_SINGLE) && (lvds_type != LVDS_TYPE_DUAL)){
		printf("%s : wrong lvds type\n");
	}else{

		LVDS_PHY_ClockEnable(lvds_main, 1);
		if(lvds_type == LVDS_TYPE_DUAL) {
			LVDS_PHY_ClockEnable(lvds_sub, 1);
		}

		LVDS_PHY_SWReset(lvds_main, 1);
		if(lvds_type == LVDS_TYPE_DUAL) {
			LVDS_PHY_SWReset(lvds_sub, 1);
		}

		OSAL_TaskDelay(1);

		LVDS_PHY_SWReset(lvds_main, 0);
		if(lvds_type == LVDS_TYPE_DUAL) {
			LVDS_PHY_SWReset(lvds_sub, 0);
		}

		/* LVDS PHY Strobe setup */
		LVDS_PHY_SetStrobe(lvds_main, 1, 1);
		if(lvds_type == LVDS_TYPE_DUAL) {
			LVDS_PHY_SetStrobe(lvds_sub, 1, 1);
		}

		LVDS_PHY_StrobeConfig(lvds_main, lvds_sub, upsample_ratio,
				LVDS_PHY_INIT, vcm, vsw);

		LVDS_PHY_LaneEnable(lvds_main, 0);
		if(lvds_type == LVDS_TYPE_DUAL) {
			LVDS_PHY_LaneEnable(lvds_sub, 0);
		}

		LVDS_PHY_SetPortOption(lvds_main, 0, 0, 0, 0x0, 0x0);
		if(lvds_type == LVDS_TYPE_DUAL) {
			LVDS_PHY_SetPortOption(lvds_sub, 1, 0, 1, 0x0, 0x7);
		}

		LVDS_PHY_LaneSwap(lvds_type, lvds_main, lvds_sub, LVDS_LANE_MAIN, LVDS_LANE_SUB);

		LVDS_PHY_StrobeConfig(lvds_main, lvds_sub, upsample_ratio,
				LVDS_PHY_READY, vcm, vsw);

		LVDS_PHY_SetFcon(lvds_main, LVDS_PHY_FCON_AUTOMATIC, 0, 0, ref_cnt);		// fcon value, for 44.1Mhz
		if(lvds_type == LVDS_TYPE_DUAL) {
			LVDS_PHY_SetFcon(lvds_sub, LVDS_PHY_FCON_AUTOMATIC, 0, 0, ref_cnt);			// fcon value, for 44.1Mhz
		}

		LVDS_PHY_FConEnable(lvds_main, 1);
		if(lvds_type == LVDS_TYPE_DUAL) {
			LVDS_PHY_FConEnable(lvds_sub, 1);
		}

		LVDS_PHY_SetCFcon(lvds_main, LVDS_PHY_FCON_AUTOMATIC, 1);
		if(lvds_type == LVDS_TYPE_DUAL) {
			LVDS_PHY_SetCFcon(lvds_sub, LVDS_PHY_FCON_AUTOMATIC, 1);
		}

		LVDS_PHY_CheckPLLStatus(lvds_main, lvds_sub);
		OSAL_TaskDelay(10);

		LVDS_PHY_StrobeConfig(lvds_main, lvds_sub, upsample_ratio,
				LVDS_PHY_START, vcm, vsw);

		/* LVDS PHY digital setup */
		LVDS_PHY_SetFormat(lvds_main, 0, 1, 0, upsample_ratio);
		if(lvds_type == LVDS_TYPE_DUAL) {
			LVDS_PHY_SetFormat(lvds_sub, 0, 1, 0, upsample_ratio);
		}

		LVDS_PHY_SetFifoEnableTiming(lvds_main, 0x3);
		if(lvds_type == LVDS_TYPE_DUAL) {
			LVDS_PHY_SetFifoEnableTiming(lvds_sub, 0x3);
		}

		/* LVDS PHY Main/Sub Lane Disable */
		LVDS_PHY_LaneEnable(lvds_main, 0);
		if(lvds_type == LVDS_TYPE_DUAL) {
			LVDS_PHY_LaneEnable(lvds_sub, 0);
		}

		/* LVDS PHY Main port FIFO Disable */
		LVDS_PHY_FifoEnable(lvds_main, 0);
		if(lvds_type == LVDS_TYPE_DUAL) {
			LVDS_PHY_FifoEnable(lvds_sub, 0);
		}

		LVDS_PHY_FifoReset(lvds_main, 1);
		if(lvds_type == LVDS_TYPE_DUAL) {
			LVDS_PHY_FifoReset(lvds_sub, 1);
		}

		OSAL_TaskDelay(1);

		LVDS_PHY_FifoReset(lvds_main, 0);
		if(lvds_type == LVDS_TYPE_DUAL) {
			LVDS_PHY_FifoReset(lvds_sub, 0);
		}

		/* LVDS PHY Main/Sub port FIFO Enable */
		LVDS_PHY_FifoEnable(lvds_main, 1);
		if(lvds_type == LVDS_TYPE_DUAL) {
			LVDS_PHY_FifoEnable(lvds_sub, 1);
		}

		/* LVDS PHY Main/Sub port Lane Enable */
		LVDS_PHY_LaneEnable(lvds_main, 1);
		if(lvds_type == LVDS_TYPE_DUAL) {
			LVDS_PHY_LaneEnable(lvds_sub, 1);
		}

		status = LVDS_PHY_CheckStatus(lvds_main, lvds_sub);
		if(lvds_main != LVDS_PHY_PORT_MAX){
			if((status & 0x1) == 0){
				printf("%s: LVDS_PHY Primary port(%d) is in death \n", __func__, lvds_main);
			}else{
				printf("%s: LVDS_PHY Primary port (%s) is enabled\n",__func__, (lvds_type == LVDS_TYPE_DUAL) ? "Dual LVDS":"Single LVDS");
			}
		}
		if(lvds_sub != LVDS_PHY_PORT_MAX){
			if((status & 0x2) == 0){
				printf("%s: LVDS_PHY Secondary port(%d) is in death \n", __func__, lvds_sub);
			}else{
				printf("%s: LVDS_PHY Secondary port (%s) is enabled\n",__func__, (lvds_type == LVDS_TYPE_DUAL) ? "Dual LVDS":"Single LVDS");
			}
		}
	}
}

static void LVDS_PHY_LaneSwap(uint32 lvds_type, uint32 lvds_main, uint32 lvds_sub, uint32* lane_main, uint32 *lane_sub)
{
	uint32 idx;
	for(idx=0;idx<5;idx++){
		LVDS_PHY_SetLaneSwap(lvds_main, idx, lane_main[idx]);
		if(lvds_type == LVDS_TYPE_DUAL) {
			LVDS_PHY_SetLaneSwap(lvds_sub, idx, lane_sub[idx]);
		}
	}
}

/* LVDS_PHY_GetCalibrationLevel
 * Get setting value for VCM/VSW calibration
 * vcm : typical vcm level of lvds panel
 * vsw : typical vsw level of lvds panel
 */
static void LVDS_PHY_GetCalibrationLevel(unsigned int vcm, unsigned int vsw, unsigned int *vcmcal, unsigned int *swingcal)
{
	unsigned int swing_max, swing_min;
	unsigned int index, step;

	if (vcm < 770) {		/* 0 : 190~370*/
		*vcmcal = 0;
		swing_min = 190;
		swing_max = 370;
	} else if (vcm >= 770 && vcm < 870) {		/* 1 : 210 ~ 470*/
		*vcmcal = 1;
		swing_min = 210;
		swing_max = 470;
	} else if (vcm >= 870 && vcm < 960) {		/* 2 : 210 ~ 540 */
		*vcmcal = 2;
		swing_min = 210;
		swing_max = 540;
	} else if (vcm >= 960 && vcm < 1050) {		/* 3 : 210 ~ 570 */
		*vcmcal = 3;
		swing_min =210;
		swing_max = 540;
	} else if (vcm >= 1050 && vcm < 1130) {		/* 4 : 210 ~ 560 */
		*vcmcal = 4;
		swing_min = 210;
		swing_max = 560;
	} else if (vcm >= 1130 && vcm < 1210) {		/* 5 : 210 ~ 530 */
		*vcmcal = 5;
		swing_min = 210;
		swing_max = 530;
	} else if (vcm >= 1130 && vcm < 1210) {		/* 6 : 210 ~ 500 */
		*vcmcal = 6;
		swing_min = 210;
		swing_max = 500;
	} else  {		/* 7 : 210 ~ 460 */
		*vcmcal = 7;
		swing_min = 210;
		swing_max = 560;
	}

	step = (swing_max - swing_min)/16;
	for (index = 0; index < 16; index++) {
		*swingcal = index;
		if (vsw <= (swing_min + step * index))
			break;
	}
}

/* LVDS_PHY_GetUpsampleRatio
 * Get upsample ratio value for Automatic FCON
 * p_port : the primary port number of lvds phy
 * s_port : the secondary port number of lvds phy
 * freq : lvds pixel clock
 */
int LVDS_PHY_GetUpsampleRatio(unsigned int p_port, unsigned int s_port, unsigned int freq)
{
	volatile void __iomem *p_reg = LVDS_PHY_GetAddress(p_port);
	volatile void __iomem *s_reg = LVDS_PHY_GetAddress(s_port);
	int idx = -1;
	if(p_reg) {
		unsigned int pxclk;
		if(s_reg)
			pxclk = freq/2;
		else
			pxclk = freq;

		for(idx = 0; idx < (int)(sizeof(ref_ratio_arr) / sizeof(ref_ratio_arr[0])); idx++) {
			if(((pxclk * 7) * ref_ratio_arr[idx][1]) > LVDS_PHY_VCO_RANGE_MIN)
				goto end_func;
		}

		if(idx < (int)(sizeof(ref_ratio_arr) / sizeof(ref_ratio_arr[0]))) {
			pr_err("error in %s: can not get upsample ratio (%dMhz)\n",
				__func__, (pxclk/1000000));
			idx = -1;
		}
	} else {
		pr_err("error in %s can not get hw address \n",
			__func__);
	}
end_func:
	return idx;
}

/* LVDS_PHY_GetRefCnt
 * Get Reference count value for Automatic FCON
 * p_port : the primary port number of lvds phy
 * s_port : the secondary port number of lvds phy
 * freq : lvds pixel clock
 * upsample_ratio : upsample_ratio
 * Formula : REF_CNT = ((pixel clk * 7 * (2^upsample_ratio))/20)/24*16*16
 */
unsigned int LVDS_PHY_GetRefCnt(unsigned int p_port, unsigned int s_port, unsigned int freq, int upsample_ratio)
{
	volatile void __iomem *p_reg = LVDS_PHY_GetAddress(p_port);
	volatile void __iomem *s_reg = LVDS_PHY_GetAddress(s_port);

	if(p_reg) {
		unsigned int pxclk;
		if(s_reg)
			pxclk = freq/2;
		else
			pxclk = freq;

		if((upsample_ratio > LVDS_PHY_UPSAMPLE_RATIO_MAX) || (upsample_ratio < 0)) {
			pr_err("error in %s: invaild parameter (pxclk:%dMhz / upsample_ratio:%d)\n",
					__func__, (pxclk/1000000), upsample_ratio);
			return 0;
		}

		return (((pxclk * 7 * ref_ratio_arr[upsample_ratio][1])/20)/24*16*16)/1000000;
	} else {
		pr_err("error in %s can not get hw address \n", __func__);
	}

	return 0;
}

/* LVDS_PHY_SetFormat
 * Set LVDS phy format information
 * port : the port number of lvds phy
 * balance : balanced mode enable (0-disable, 1-enable)
 * depth : color depth(0-6bit, 1-8bit)
 * format : 0-VESA, 1-JEIDA
 * freq : 0~4 video frequency
 */
void LVDS_PHY_SetFormat(unsigned int port, unsigned int balance,
			     unsigned int depth, unsigned int format, unsigned int freq)
{
	volatile void __iomem *reg = LVDS_PHY_GetAddress(port);

	if (reg) {
		unsigned int value;
		value = (readl(reg + LVDS_FORMAT) &
			 ~(LVDS_FORMAT_BALANCED_EN_MASK |
			   LVDS_FORMAT_COLOR_DEPTH_MASK |
			   LVDS_FORMAT_COLOR_FORMAT_MASK |
			   LVDS_FORMAT_UPSAMPLE_RATIO_MASK));
		value |= (((balance & 0x1) << LVDS_FORMAT_BALANCED_EN_SHIFT) |
			  ((depth & 0x1) << LVDS_FORMAT_COLOR_DEPTH_SHIFT) |
			  ((format & 0x1) << LVDS_FORMAT_COLOR_FORMAT_SHIFT) |
			  ((freq & 0x7) << LVDS_FORMAT_UPSAMPLE_RATIO_SHIFT));
		writel(value, reg + LVDS_FORMAT);
	}
}

/* LVDS_PHY_SetUserMode
 * Control lane skew and p/n swap
 * port : the port number of lvds phy
 * lane : lane type
 * skew : lane skew value
 * swap : lane p/n swap (0-Normal, 1-Swap p/n)
 */
void LVDS_PHY_SetUserMode(unsigned int port, unsigned int lane,
			       unsigned int skew, unsigned int swap)
{
	volatile void __iomem *reg = LVDS_PHY_GetAddress(port);

	if (reg) {
		unsigned int value;
		switch (lane) {
		case LVDS_PHY_CLK_LANE:
			value = (readl(reg + LVDS_USER_MODE_PHY_IF_SET0) &
				 ~(LVDS_USER_MODE_PHY_IF_SET0_CLK_PN_SWAP_MASK |
				   LVDS_USER_MODE_PHY_IF_SET0_CLK_LANE_SKEW_MASK));
			value |=
				(((skew & 0x7)
				  << LVDS_USER_MODE_PHY_IF_SET0_CLK_LANE_SKEW_SHIFT) |
				 ((swap & 0x1)
				  << LVDS_USER_MODE_PHY_IF_SET0_CLK_PN_SWAP_SHIFT));
			writel(value, reg + LVDS_USER_MODE_PHY_IF_SET0);
			break;
		case LVDS_PHY_DATA0_LANE:
			value = (readl(reg + LVDS_USER_MODE_PHY_IF_SET0) &
				 ~(LVDS_USER_MODE_PHY_IF_SET0_DATA0_PN_SWAP_MASK |
				   LVDS_USER_MODE_PHY_IF_SET0_DATA0_LANE_SKEW_MASK));
			value |=
				(((skew & 0x7)
				  << LVDS_USER_MODE_PHY_IF_SET0_DATA0_LANE_SKEW_SHIFT) |
				 ((swap & 0x1)
				  << LVDS_USER_MODE_PHY_IF_SET0_DATA0_PN_SWAP_SHIFT));
			writel(value, reg + LVDS_USER_MODE_PHY_IF_SET0);
			break;
		case LVDS_PHY_DATA1_LANE:
			value = (readl(reg + LVDS_USER_MODE_PHY_IF_SET0) &
				 ~(LVDS_USER_MODE_PHY_IF_SET0_DATA1_PN_SWAP_MASK |
				   LVDS_USER_MODE_PHY_IF_SET0_DATA1_LANE_SKEW_MASK));
			value |=
				(((skew & 0x7)
				  << LVDS_USER_MODE_PHY_IF_SET0_DATA1_LANE_SKEW_SHIFT) |
				 ((swap & 0x1)
				  << LVDS_USER_MODE_PHY_IF_SET0_DATA1_PN_SWAP_SHIFT));
			writel(value, reg + LVDS_USER_MODE_PHY_IF_SET0);
			break;
		case LVDS_PHY_DATA2_LANE:
			value = (readl(reg + LVDS_USER_MODE_PHY_IF_SET0) &
				 ~(LVDS_USER_MODE_PHY_IF_SET0_DATA2_PN_SWAP_MASK |
				   LVDS_USER_MODE_PHY_IF_SET0_DATA2_LANE_SKEW_MASK));
			value |=
				(((skew & 0x7)
				  << LVDS_USER_MODE_PHY_IF_SET0_DATA2_LANE_SKEW_SHIFT) |
				 ((swap & 0x1)
				  << LVDS_USER_MODE_PHY_IF_SET0_DATA2_PN_SWAP_SHIFT));
			writel(value, reg + LVDS_USER_MODE_PHY_IF_SET0);
			break;
		case LVDS_PHY_DATA3_LANE:
			value = (readl(reg + LVDS_USER_MODE_PHY_IF_SET0) &
				 ~(LVDS_USER_MODE_PHY_IF_SET0_DATA3_PN_SWAP_MASK |
				   LVDS_USER_MODE_PHY_IF_SET0_DATA3_LANE_SKEW_MASK));
			value |=
				(((skew & 0x7)
				  << LVDS_USER_MODE_PHY_IF_SET0_DATA3_LANE_SKEW_SHIFT) |
				 ((swap & 0x1)
				  << LVDS_USER_MODE_PHY_IF_SET0_DATA3_PN_SWAP_SHIFT));
			writel(value, reg + LVDS_USER_MODE_PHY_IF_SET0);
			break;
		case LVDS_PHY_LANE_MAX:
		default:
			pr_err("%s in error, invaild parameter(lane:%d)\n",
			       __func__, lane);
			break;
		}
	}
}

void LVDS_PHY_SetLaneSwap(unsigned int port, unsigned int lane, unsigned int select)
{
	volatile void __iomem *reg = LVDS_PHY_GetAddress(port);
	unsigned int value;
	if(reg) {
		switch(lane) {
		case LVDS_PHY_CLK_LANE:
		case LVDS_PHY_DATA0_LANE:
		case LVDS_PHY_DATA1_LANE:
		case LVDS_PHY_DATA2_LANE:
		case LVDS_PHY_DATA3_LANE:
			break;
		case LVDS_PHY_LANE_MAX:
		default:
			pr_err("%s in error, invaild parameter(lane:%d)\n",
			       __func__, lane);
			break;
		}
		value = (readl(reg+LVDS_USER_MODE_PHY_IF_SET1) &
			~(0x7 << (LVDS_USER_MODE_PHY_IF_SET1_SET_LANE0_SHIFT + (lane * 0x4))));
		value |= ((select & 0x7) << (LVDS_USER_MODE_PHY_IF_SET1_SET_LANE0_SHIFT + (lane*0x4)));
		writel(value, reg+LVDS_USER_MODE_PHY_IF_SET1);
	}
}

/* LVDS_PHY_SetFifoEnableTiming
 * Select FIFO2 enable timing
 * port : the port number of lvds phy
 * cycle : FIFO enable after n clock cycle (0~3 cycle)
 */
void LVDS_PHY_SetFifoEnableTiming(unsigned int port, unsigned int cycle)
{
	volatile void __iomem *reg = LVDS_PHY_GetAddress(port);

	if (reg) {
		unsigned int value;
		writel(0x00000000, reg+LVDS_STARTUP_MODE);

		value = (readl(reg + LVDS_STARTUP_MODE) &
			 ~(LVDS_STARTUP_MODE_FIFO2_RD_EN_TIMING_MASK));
		value |= ((cycle & 0x3)
			  << LVDS_STARTUP_MODE_FIFO2_RD_EN_TIMING_SHIFT);
		writel(value, reg + LVDS_STARTUP_MODE);
	}
}

/* LVDS_PHY_SetPortOption
 * Selects a port option for dual pixel mode
 * port : the port number of lvds phy
 * port_mode : the mode of this port(0-main port, 1-sub port)
 * sync_swap : swap vsync/hsync position (0-do not swap, 1-swap)
 * use_other_port : 0-normal, 1-use sync from other port
 * lane_en : lane enable (CLK, DATA0~3)
 * sync_transmit_src : sync transmit source (0-normal, 1-other port)
 */
void LVDS_PHY_SetPortOption(unsigned int port, unsigned int port_mode,
				 unsigned int sync_swap,
				 unsigned int use_other_port,
				 unsigned int lane_en,
				 unsigned int sync_transmit_src)
{
	volatile void __iomem *reg = LVDS_PHY_GetAddress(port);

	if (reg) {
		unsigned int value;
		value = (readl(reg + LVDS_PORT) &
			 ~(LVDS_PORT_SET_SECONDARY_PORT_MASK |
			   LVDS_PORT_VSYNC_HSYNC_SWAP_MASK |
			   LVDS_PORT_USE_SYNC_FROM_OP_MASK |
			   LVDS_PORT_LANE_EN_MASK |
			   LVDS_PORT_SYNC_TRANSMITTED_MASK));

		value |= (((port_mode & 0x1)
			   << LVDS_PORT_SET_SECONDARY_PORT_SHIFT) |
			  ((sync_swap & 0x1)
			   << LVDS_PORT_VSYNC_HSYNC_SWAP_SHIFT) |
			  ((use_other_port & 0x1)
			   << LVDS_PORT_USE_SYNC_FROM_OP_SHIFT) |
			  ((lane_en & 0x1F) << LVDS_PORT_LANE_EN_SHIFT) |
			  ((sync_transmit_src & 0x7)
			   << LVDS_PORT_SYNC_TRANSMITTED_SHIFT));
		writel(value, reg + LVDS_PORT);
	}
}

/* LVDS_PHY_LaneEnable
 * Set lvds lane enable/disable
 * port : the port number of lvds phy
 */
void LVDS_PHY_LaneEnable(unsigned int port, unsigned int enable)
{
	volatile void __iomem *reg = LVDS_PHY_GetAddress(port);

	if (reg) {
		unsigned int value;
		value = (readl(reg + LVDS_PORT) &
			 ~(LVDS_PORT_LANE_EN_MASK));

		if(enable)
			value |= (0x1F << LVDS_PORT_LANE_EN_SHIFT);
		writel(value, reg + LVDS_PORT);
	}
}

/* LVDS_PHY_FifoEnable
 * Set lvds phy fifo enable
 * port : the port number of lvds phy
 */
void LVDS_PHY_FifoEnable(unsigned int port, unsigned int enable)
{
	volatile void __iomem *reg = LVDS_PHY_GetAddress(port);

	if (reg) {
		unsigned int value;
		value = (readl(reg + LVDS_EN) &
			 ~(LVDS_EN_FIFO2_EN_MASK | LVDS_EN_FIFO1_EN_MASK |
			   LVDS_EN_FIFO0_EN_MASK | LVDS_EN_DATA_EN_MASK));

		if (enable) {
			value |= ((0x1 << LVDS_EN_FIFO2_EN_SHIFT) |
				  (0x1 << LVDS_EN_FIFO1_EN_SHIFT) |
				  (0x1 << LVDS_EN_FIFO0_EN_SHIFT) |
				  (0x1 << LVDS_EN_DATA_EN_SHIFT));
		}
		writel(value, reg + LVDS_EN);
	}
}

/* LVDS_PHY_FifoReset
 * Reset LVDS PHY Fifo
 * port : the port number of lvds phy
 * reset : 0-release, 1-reset
 */
void LVDS_PHY_FifoReset(unsigned int port, unsigned reset)
{
	volatile void __iomem *reg = LVDS_PHY_GetAddress(port);
	if(reg) {
		if(reset)
			writel(0x0000118F, reg + LVDS_RESETB);
		else
			writel(0x00001FFF, reg + LVDS_RESETB);
	}
}

/* LVDS_PHY_SWReset
 * Control lvds phy swreset
 * port : the port number of lvds phy
 * reset : 0-release, 1-reset
 */
void LVDS_PHY_SWReset(unsigned int port, unsigned int reset)
{
	volatile void __iomem *reg = LVDS_PHY_GetAddress(port);

	if (reg) {
		if (reset)
			writel(0x00000000, reg + LVDS_RESETB);
		else
			writel(0x00001FFF, reg + LVDS_RESETB);
	}
}

/* LVDS_PHY_ClockEnable
 * Control lvds phy clock enable
 * port : the port number of lvds phy
 * enable : 0-disable, 1-enable
 */
void LVDS_PHY_ClockEnable(unsigned int port, unsigned int enable)
{
	volatile void __iomem *reg = LVDS_PHY_GetAddress(port);

	if (reg) {
		if (enable)
			writel(0x000100FF, reg + LVDS_CLK_SET);
		else
			writel(0x00000000, reg + LVDS_CLK_SET);
	}
}

/* LVDS_PHY_SetStrobe
 * Sets LVDS strobe registers
 * port : the port number of lvds phy
 * mode : 0-Manual, 1- Auto
 * enable : 0-disable, 1-enable
 */
void LVDS_PHY_SetStrobe(unsigned int port, unsigned int mode, unsigned int enable)
{
	volatile void __iomem *reg = LVDS_PHY_GetAddress(port);

	if(reg) {
		unsigned int value;
		value = (readl(reg+LVDS_STB_EN) & ~(LVDS_STB_EN_STB_EN_MASK));
		value |= ((enable & 0x1) << LVDS_STB_EN_STB_EN_SHIFT);
		writel(value, reg+LVDS_STB_EN);

		value = (readl(reg+LVDS_AUTO_STB_SET) & ~(LVDS_AUTO_STB_SET_STB_AUTO_EN_MASK));
		value |= ((mode & 0x1) << LVDS_AUTO_STB_SET_STB_AUTO_EN_SHIFT);
		writel(value, reg+LVDS_AUTO_STB_SET);
	}
}

/* LVDS_PHY_SetFcon
 * Setup LVDS PHY Manual/Automatic Coarse tunning
 * port : the port number of lvds phy
 * mode : coarse tunning method 0-manual, 1-automatic
 * loop : feedback loop mode 0-closed loop, 1-open loop
 * fcon : frequency control value
 */
void LVDS_PHY_SetFcon(unsigned int port, unsigned int mode, unsigned int loop, unsigned int division, unsigned int fcon)
{
	volatile void __iomem *reg = LVDS_PHY_GetAddress(port);
	unsigned int value;

	if(reg) {
		unsigned int target_th = 0x2020;		/* Alphachips Guide for FCON */
		switch(mode) {
		case LVDS_PHY_FCON_MANUAL:
			value = (readl(reg+LVDS_CTSET1) & ~(LVDS_CTSET1_MPLL_CTLCK_MASK|
						LVDS_CTSET1_MPLL_DIVN_MASK| LVDS_CTSET1_MPLL_FCON_MASK));
			value |= (((loop & 0x1) << LVDS_CTSET1_MPLL_CTLCK_SHIFT) |
					((division & 0x3) << LVDS_CTSET1_MPLL_DIVN_SHIFT) |
					((fcon & 0x3FF) << LVDS_CTSET1_MPLL_FCON_SHIFT));
			writel(value, reg+LVDS_CTSET1);         // offset: 0x094

			value = (readl(reg+LVDS_FCOPT) & ~(LVDS_FCOPT_CLK_DET_SEL_MASK |
						LVDS_FCOPT_CT_SEL_MASK));
			value |= ((mode & 0x1) << LVDS_FCOPT_CT_SEL_SHIFT);
			writel(value, reg+LVDS_FCOPT);          // offset: 0x09C
			break;
		case LVDS_PHY_FCON_AUTOMATIC:
			value = (readl(reg+LVDS_FCCNTR1) & ~(LVDS_FCCNTR1_CONTIN_TARGET_TH_MASK |
						LVDS_FCCNTR1_REF_CNT_MASK));
			value |= (((target_th & 0xFFFF) << LVDS_FCCNTR1_CONTIN_TARGET_TH_SHIFT) |
					(fcon & 0xFFFF) << LVDS_FCCNTR1_REF_CNT_SHIFT);
			writel(value, reg+LVDS_FCCNTR1);                // offset: 0x0B0

			value = (readl(reg+LVDS_CTSET1) & ~(LVDS_CTSET1_MPLL_CTLCK_MASK|
						LVDS_CTSET1_MPLL_DIVN_MASK| LVDS_CTSET1_MPLL_FCON_MASK));
			value |= (((loop & 0x1) << LVDS_CTSET1_MPLL_CTLCK_SHIFT) |		/* Shoulb be set to 'Open' loop */
					((division & 0x3) << LVDS_CTSET1_MPLL_DIVN_SHIFT));
			writel(value, reg+LVDS_CTSET1);         // offset: 0x094

			value = (readl(reg+LVDS_CTSET0) & ~(LVDS_CTSET0_ENABLE_MASK |
						LVDS_CTSET0_RUN_MASK));
			value |= (0x1 << LVDS_CTSET0_ENABLE_SHIFT);
			writel(value, reg+LVDS_CTSET0);         // offset: 0x090

			value = (readl(reg+LVDS_FCOPT) & ~(LVDS_FCOPT_CLK_DET_SEL_MASK |
						LVDS_FCOPT_CT_SEL_MASK));
			value |= (((mode & 0x1) << LVDS_FCOPT_CT_SEL_SHIFT) |
					(0x1 << LVDS_FCOPT_CLK_DET_SEL_SHIFT));
			writel(value, reg+LVDS_FCOPT);          // offset: 0x09C

			writel(0x10101010, reg+LVDS_FCCNTR0);           /* Alphachips Guide for FCON */         // offset: 0x098
			break;
		case LVDS_PHY_FCON_MAX:
		default:
			pr_err("%s in error, invaild parameter(mode: %d)\n",
					__func__, mode);
			break;
		}
	}
}

/* LVDS_PHY_SetCFcon
 * Check fcon status and setup cfcon value
 * port : the port number of lvds phy
 * mode : fcon control mode 0-Manual, 1-Automatic
 * enable : 0-cfcon disable 1-cfcon enable
 */
void LVDS_PHY_SetCFcon(unsigned int port, unsigned int mode, unsigned int enable)
{
	volatile void __iomem *reg = LVDS_PHY_GetAddress(port);
	unsigned int value;
	unsigned int time_out = 0;
	if(reg) {
		if(mode == LVDS_PHY_FCON_AUTOMATIC) {
			while(time_out++ < 10000) {
				unsigned int pd_fcstat;//,pd_fccntval1, pd_fcresval;
				pd_fcstat = (readl(reg+LVDS_FCSTAT) & (LVDS_FCST_CLK_OK_MASK|LVDS_FCST_DONE_MASK|LVDS_FCST_ERROR_MASK));
				//pd_fccntval1 = readl(reg+LVDS_FCCNTVAL1); //warning. set but not used
				//pd_fcresval = readl(reg+LVDS_FCRESEVAL); //warning. set but not used
				if(pd_fcstat == (LVDS_FCST_CLK_OK_MASK|LVDS_FCST_DONE_MASK)) {
					if(enable) {
						/* Alphachips Guide For CFCON */
						writel(0x0000002C, reg+LVDS_FCCONTINSET0);              // offset: 0x0B4
						writel(0x00000960, reg+LVDS_FCCONTINSET1);              // offset: 0x0B8
						writel(0x003FF005, reg+LVDS_FCCONTINSET2);              // offset: 0x0BC
						writel(0x0000002D, reg+LVDS_FCCONTINSET0);              // offset: 0x0B4
					}
					goto closed_loop;
				} else {
					lvds_phy_udelay(1000);
				}
			}
		}
		pr_err("%s time out\n" ,__func__);
closed_loop:
		/* Change loop mode to 'Closed' loop */
		value = (readl(reg+LVDS_CTSET1) & ~(LVDS_CTSET1_MPLL_CTLCK_MASK));
		value |= (0x1 << LVDS_CTSET1_MPLL_CTLCK_SHIFT);
		writel(value, reg+LVDS_CTSET1);         // offset: 0x094
	}
}

void LVDS_PHY_CheckPLLStatus(unsigned int p_port, unsigned int s_port)
{
	volatile void __iomem *p_reg = LVDS_PHY_GetAddress(p_port);
	volatile void __iomem *s_reg = LVDS_PHY_GetAddress(s_port);
	unsigned int offset = LVDS_MONITOR_DEBUG1;
	unsigned int time_out = 0;

	if(p_reg || s_reg) {
		while(time_out++ < 10000) {
			unsigned int p_pllstatus = (readl(p_reg+offset) & LVDS_MONITOR_DEBUG1_PLL_STATUS_MASK);
			if(s_reg) {
				unsigned int s_pllstatus = (readl(s_reg+offset) & LVDS_MONITOR_DEBUG1_PLL_STATUS_MASK);
				if((p_pllstatus == LVDS_MONITOR_DEBUG1_PLL_STATUS_MASK) && (s_pllstatus == LVDS_MONITOR_DEBUG1_PLL_STATUS_MASK))
					return;
			} else {
				if(p_pllstatus == LVDS_MONITOR_DEBUG1_PLL_STATUS_MASK)
					return;
			}
			lvds_phy_udelay(1000);
		}
	}
	pr_err("%s time out\n",__func__);
}

/* LVDS_PHY_FConEnable
 * Controls FCON running enable
 * port : the port number of lvds phy
 * enable : 0-disable, 1-enable
 */
void LVDS_PHY_FConEnable(unsigned int port, unsigned int enable)
{
	volatile void __iomem *reg = LVDS_PHY_GetAddress(port);
	unsigned int value;

	if(reg) {
		value = (readl(reg+LVDS_CTSET0) & ~(LVDS_CTSET0_ENABLE_MASK |
					LVDS_CTSET0_RUN_MASK));
		value |= (((enable & 0x1) << LVDS_CTSET0_ENABLE_SHIFT) |
				((enable & 0x1) << LVDS_CTSET0_RUN_SHIFT));
		writel(value, reg+LVDS_CTSET0);
	}
}

/* LVDS_PHY_StrobeWrite
 * Write LVDS PHY Strobe register
 * reg : LVDS PHY Strobe register address
 * offset : LVDS PHY register offset
 * value : the value you want
 */
void LVDS_PHY_StrobeWrite(volatile void __iomem *reg, unsigned int offset, unsigned int value)
{
	unsigned int time_out = 0;
	writel(value, reg+offset);
	while(time_out++ < 1000){
		if(readl(reg+LVDS_AUTO_STB_DONE) == 0x1) return;
		lvds_phy_udelay(1000);
	}
	pr_err("%s time out\n",__func__);
}

/* LVDS_PHY_Config
 * Setup LVDS PHY Strobe. (Alphachips Guide Value Only)
 * p_port : the primary port number
 * s_port : the secondary port number
 * upsample_ratio : division ratio (0: fcon automatic, 0~4: fcon manual)
 * step : the step of lvds phy configure
 * vcm : typical vcm level in mV
 * vsw : typical vsw level in mV
 */
void LVDS_PHY_StrobeConfig(unsigned int p_port, unsigned int s_port, unsigned int upsample_ratio, unsigned int step, unsigned int vcm, unsigned int vsw)
{
	volatile void __iomem *p_reg = LVDS_PHY_GetAddress(p_port);
	volatile void __iomem *s_reg = LVDS_PHY_GetAddress(s_port);
	unsigned int vcmcal, swingcal;
	unsigned int value;

	if(p_reg) {
		switch(step) {
		case LVDS_PHY_INIT:
			LVDS_PHY_GetCalibrationLevel(vcm, vsw, &vcmcal, &swingcal);
			value = ((swingcal & 0xF)|((vcmcal & 0x7) << 4));

			LVDS_PHY_StrobeWrite(p_reg, 0x380, 0x000000FF);		// STB_PLL ADDR 0x0
			LVDS_PHY_StrobeWrite(p_reg, 0x384, 0x0000001F);		// STB_PLL ADDR 0x1
			LVDS_PHY_StrobeWrite(p_reg, 0x388, 0x00000032);		// STB_PLL ADDR 0x2
			LVDS_PHY_StrobeWrite(p_reg, 0x38C, 0x0000000F);		// STB_PLL ADDR 0x3
			LVDS_PHY_StrobeWrite(p_reg, 0x390, 0x00000000);		// STB_PLL ADDR 0x4
			LVDS_PHY_StrobeWrite(p_reg, 0x394, 0x00000013);		// STB_PLL ADDR 0x5
			LVDS_PHY_StrobeWrite(p_reg, 0x398, 0x00000029);		// STB_PLL ADDR 0x6
			LVDS_PHY_StrobeWrite(p_reg, 0x39C, 0x00000025);		// STB_PLL ADDR 0x7
			LVDS_PHY_StrobeWrite(p_reg, 0x3A0, 0x000000D4);		// STB_PLL ADDR 0x8
			LVDS_PHY_StrobeWrite(p_reg, 0x3A4, 0x00000000);		// STB_PLL ADDR 0x9
			LVDS_PHY_StrobeWrite(p_reg, 0x3A8, 0x0000000C);		// STB_PLL ADDR 0xA
			LVDS_PHY_StrobeWrite(p_reg, 0x3AC, 0x00000000);		// STB_PLL ADDR 0xB
			LVDS_PHY_StrobeWrite(p_reg, 0x3B0, 0x00000000);		// STB_PLL ADDR 0xC
			if(s_reg)
				LVDS_PHY_StrobeWrite(p_reg, 0x3B4, 0x00000007);		// STB_PLL ADDR 0xD
			else
				LVDS_PHY_StrobeWrite(p_reg, 0x3B4, 0x00000001);		// STB_PLL ADDR 0xD
			LVDS_PHY_StrobeWrite(p_reg, 0x3B8, 0x00000001);		// STB_PLL ADDR 0xE

			if(s_reg) {
				LVDS_PHY_StrobeWrite(s_reg, 0x380, 0x000000FF);		// STB_PLL ADDR 0x0
				LVDS_PHY_StrobeWrite(s_reg, 0x384, 0x0000001F);		// STB_PLL ADDR 0x1
				LVDS_PHY_StrobeWrite(s_reg, 0x388, 0x00000032);		// STB_PLL ADDR 0x2
				LVDS_PHY_StrobeWrite(s_reg, 0x38C, 0x0000000F);		// STB_PLL ADDR 0x3
				LVDS_PHY_StrobeWrite(s_reg, 0x390, 0x00000000);		// STB_PLL ADDR 0x4
				LVDS_PHY_StrobeWrite(s_reg, 0x394, 0x00000013);		// STB_PLL ADDR 0x5
				LVDS_PHY_StrobeWrite(s_reg, 0x398, 0x00000029);		// STB_PLL ADDR 0x6
				LVDS_PHY_StrobeWrite(s_reg, 0x39C, 0x00000025);		// STB_PLL ADDR 0x7
				LVDS_PHY_StrobeWrite(s_reg, 0x3A0, 0x000000D4);		// STB_PLL ADDR 0x8
				LVDS_PHY_StrobeWrite(s_reg, 0x3A4, 0x00000000);		// STB_PLL ADDR 0x9
				LVDS_PHY_StrobeWrite(s_reg, 0x3A8, 0x0000000C);		// STB_PLL ADDR 0xA
				LVDS_PHY_StrobeWrite(s_reg, 0x3AC, 0x00000000);		// STB_PLL ADDR 0xB
				LVDS_PHY_StrobeWrite(s_reg, 0x3B0, 0x00000000);		// STB_PLL ADDR 0xC
				LVDS_PHY_StrobeWrite(s_reg, 0x3B4, 0x0000000C);		// STB_PLL ADDR 0xD
				LVDS_PHY_StrobeWrite(s_reg, 0x3B8, 0x00000001);		// STB_PLL ADDR 0xE
			}

			LVDS_PHY_StrobeWrite(p_reg, 0x204, value);		// STB_LANE0 ADDR 0x1
			LVDS_PHY_StrobeWrite(p_reg, 0x244, value);		// STB_LANE1 ADDR 0x1
			LVDS_PHY_StrobeWrite(p_reg, 0x284, value);		// STB_LANE2 ADDR 0x1
			LVDS_PHY_StrobeWrite(p_reg, 0x2C4, value);		// STB_LANE3 ADDR 0x1
			LVDS_PHY_StrobeWrite(p_reg, 0x304, value);		// STB_LANE4 ADDR 0x1
			if(s_reg) {
				LVDS_PHY_StrobeWrite(s_reg, 0x204, value);		// STB_LANE0 ADDR 0x1
				LVDS_PHY_StrobeWrite(s_reg, 0x244, value);		// STB_LANE1 ADDR 0x1
				LVDS_PHY_StrobeWrite(s_reg, 0x284, value);		// STB_LANE2 ADDR 0x1
				LVDS_PHY_StrobeWrite(s_reg, 0x2C4, value);		// STB_LANE3 ADDR 0x1
				LVDS_PHY_StrobeWrite(s_reg, 0x304, value);		// STB_LANE4 ADDR 0x1
			}
			break;
		case LVDS_PHY_READY:
			value = (upsample_ratio & 0x7);
			LVDS_PHY_StrobeWrite(p_reg, 0x380, 0x00000000);		// STB_PLL ADDR 0x0
			LVDS_PHY_StrobeWrite(p_reg, 0x380, 0x000000FF);		// STB_PLL ADDR 0x0
			LVDS_PHY_StrobeWrite(p_reg, 0x384, 0x0000001F);		// STB_PLL ADDR 0x1
			LVDS_PHY_StrobeWrite(p_reg, 0x3A4, value);		// STB_PLL ADDR 0x9
			LVDS_PHY_StrobeWrite(p_reg, 0x3AC, value);		// STB_PLL ADDR 0xB
			if(s_reg) {
				LVDS_PHY_StrobeWrite(s_reg, 0x380, 0x00000000);		// STB_PLL ADDR 0x0
				LVDS_PHY_StrobeWrite(s_reg, 0x380, 0x000000FF);		// STB_PLL ADDR 0x0
				LVDS_PHY_StrobeWrite(s_reg, 0x384, 0x0000001F);		// STB_PLL ADDR 0x1
				LVDS_PHY_StrobeWrite(s_reg, 0x3A4, value);		// STB_PLL ADDR 0x9
				LVDS_PHY_StrobeWrite(s_reg, 0x3AC, value);		// STB_PLL ADDR 0xB
			}
			break;
		case LVDS_PHY_START:
			if(s_reg) {
				//unsigned int value = (LVDS_PHY_GetRegValue(p_port, 0x3B4) & ~(0x000000F0)); //GHC Warning
				value = (LVDS_PHY_GetRegValue(p_port, 0x3B4) & ~(0x000000F0));
				LVDS_PHY_StrobeWrite(p_reg, 0x3B4, value);		// STB_PLL ADDR 0xD
				value = (LVDS_PHY_GetRegValue(s_port, 0x3B4) & ~(0x000000F0));
				LVDS_PHY_StrobeWrite(s_reg, 0x3B4, value);		// STB_PLL ADDR 0xD

				LVDS_PHY_StrobeWrite(p_reg, 0x3B8, 0x00000000);		// STB_PLL ADDR 0xE
				LVDS_PHY_StrobeWrite(s_reg, 0x3B8, 0x00000000);		// STB_PLL ADDR 0xE

				LVDS_PHY_StrobeWrite(s_reg, 0x3B8, 0x00000001);   // STB_PLL ADDR 0xE
				LVDS_PHY_StrobeWrite(p_reg, 0x3B8, 0x00000001);   // STB_PLL ADDR 0xE clk_gen

				LVDS_PHY_StrobeWrite(s_reg, 0x3B4, 0x0000003C);   // STB_PLL ADDR 0xD
				LVDS_PHY_StrobeWrite(p_reg, 0x3B4, 0x00000037);   // STB_PLL ADDR 0xD startup_enable

			} else {
				//unsigned int value = (LVDS_PHY_GetRegValue(p_port, 0x3B4) & ~(0x000000F0));//GHC Warning
				value = (LVDS_PHY_GetRegValue(p_port, 0x3B4) & ~(0x000000F0));
				LVDS_PHY_StrobeWrite(p_reg, 0x3B4, value);		// STB_PLL ADDR 0xD
				LVDS_PHY_StrobeWrite(p_reg, 0x3B8, 0x00000000);		// STB_PLL ADDR 0xE
				LVDS_PHY_StrobeWrite(p_reg, 0x3B8, 0x00000001);		// STB_PLL ADDR 0xE
				value = (LVDS_PHY_GetRegValue(p_port, 0x3B4) & ~(0x000000F0));
				value |= (0x1 << 4);
				LVDS_PHY_StrobeWrite(p_reg, 0x3B4, value);		// STB_PLL ADDR 0xD
			}

			LVDS_PHY_StrobeWrite(p_reg, 0x200, 0x00000079);		// STB_LANE0 ADDR 0x0
			LVDS_PHY_StrobeWrite(p_reg, 0x240, 0x00000079);		// STB_LANE1 ADDR 0x0
			LVDS_PHY_StrobeWrite(p_reg, 0x280, 0x00000079);		// STB_LANE2 ADDR 0x0
			LVDS_PHY_StrobeWrite(p_reg, 0x2C0, 0x00000079);		// STB_LANE3 ADDR 0x0
			LVDS_PHY_StrobeWrite(p_reg, 0x300, 0x00000079);		// STB_LANE4 ADDR 0x0
			if(s_reg) {
				LVDS_PHY_StrobeWrite(s_reg, 0x200, 0x00000079);		// STB_LANE0 ADDR 0x0
				LVDS_PHY_StrobeWrite(s_reg, 0x240, 0x00000079);		// STB_LANE1 ADDR 0x0
				LVDS_PHY_StrobeWrite(s_reg, 0x280, 0x00000079);		// STB_LANE2 ADDR 0x0
				LVDS_PHY_StrobeWrite(s_reg, 0x2C0, 0x00000079);		// STB_LANE3 ADDR 0x0
				LVDS_PHY_StrobeWrite(s_reg, 0x300, 0x00000079);		// STB_LANE4 ADDR 0x0
			}			break;
		case LVDS_PHY_CONFIG_MAX:
		default:
			pr_err("%s in error, invaild parameter(step: %d)\n",
					__func__, step);
			break;
		}
	}
}

/* LVDS_PHY_CheckStatus
 * Check the status of lvds phy
 * p_port : the primary port number of lvds phy
 * s_port : the secondary port number of lvds phy
 */
unsigned int LVDS_PHY_CheckStatus(unsigned int p_port, unsigned int s_port)
{
	volatile void __iomem *p_reg = LVDS_PHY_GetAddress(p_port);
	volatile void __iomem *s_reg = LVDS_PHY_GetAddress(s_port);
	unsigned int ret = 0, value;

	if(p_reg) {
		value = (readl(p_reg+LVDS_MONITOR_DEBUG1) &
				(LVDS_MONITOR_DEBUG1_PLL_STATUS_MASK));
		if(value & (LVDS_MONITOR_DEBUG1_LKVDETLOW_MASK |
					LVDS_MONITOR_DEBUG1_LKVDETHIGH_MASK))
			ret |= 0x1;		/* primary port status b[0]*/
	}

	if(s_reg) {
		value = (readl(s_reg+LVDS_MONITOR_DEBUG1) &
				(LVDS_MONITOR_DEBUG1_PLL_STATUS_MASK));
		if(value & (LVDS_MONITOR_DEBUG1_LKVDETLOW_MASK |
					LVDS_MONITOR_DEBUG1_LKVDETHIGH_MASK))
			ret |= 0x2;		/* secondary port status b[1] */
	}
	return ret;
}

/* LVDS_PHY_GetRegValue
 * Read the register corresponding to 'offset'
 * port : the port number of lvds phy
 * offset : the register offset
 */
unsigned int LVDS_PHY_GetRegValue(unsigned int port, unsigned int offset)
{
	volatile void __iomem *reg = LVDS_PHY_GetAddress(port);
	unsigned int ret;
	unsigned int time_out = 0;
	ret = readl(reg+offset);
	if(offset > 0x1FF) {	/* Write Only Registers */
		while(time_out++ < 10) {
			if(readl(reg+LVDS_AUTO_STB_DONE) & 0x1) goto end_func;
			lvds_phy_udelay(1000);
		}
	}
	pr_err("%s time out\n",__func__);
	end_func:
		ret = readl(reg+LVDS_AUTO_STB_RDATA);
		return ret;
}

