/*
 * arch/arm/mach-telechips/tcc803x/vioc/lvds_Wrap.c
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
#include "lvds_wrap.h"

#ifndef REG_LVDS_WRAP
#define REG_LVDS_WRAP		(0x1B940000)		// 16 word
#endif

#ifndef printf
extern void IO_DBG_SerialPrintf_(int8 *format, ...);
#define	printf	IO_DBG_SerialPrintf_ 
#endif

#ifndef pr_err
#define pr_err printf
#endif

/* LVDS_WRAP_SetConfigure
 * Set Tx splitter configuration
 * lr : tx splitter output mode - 0: even/odd, 1: left/right
 * bypass : tx splitter bypass mode
 * width : tx splitter width - single port: real width, dual port: half width
 */
void LVDS_WRAP_SetConfigure(unsigned int lr,
			       unsigned int bypass, unsigned int width)
{
	volatile void __iomem *reg = (volatile void __iomem *)REG_LVDS_WRAP;
	if (reg) {
		unsigned int val = (readl(reg + TS_CFG) &
				    ~(TS_CFG_WIDTH_MASK | TS_CFG_MODE_MASK |
				      TS_CFG_LR_MASK | TS_CFG_BP_MASK));
		val |= (((width & 0xFFF) << TS_CFG_WIDTH_SHIFT) |
			((bypass & 0x1) << TS_CFG_BP_SHIFT) |
			((lr & 0x1) << TS_CFG_LR_SHIFT));
		writel(val, reg + TS_CFG);
	}
}

void LVDS_WRAP_Set(uint32 lvds_type, uint32 val, uint32 select, uint32 sel0[TXOUT_MAX_LINE][TXOUT_DATA_PER_LINE], uint32 sel1[TXOUT_MAX_LINE][TXOUT_DATA_PER_LINE])
{
	LVDS_WRAP_SetAccessCode();
	if(lvds_type == 1) {
		int32 idx;
		LVDS_WRAP_SetConfigure(0, 0, val);
		for(idx=0;idx<TS_SWAP_CH_MAX;idx++){
			LVDS_WRAP_SetDataSwap(idx,idx);
		}
		LVDS_WRAP_SetMuxOutput(DISP_MUX_TYPE, 0, select, 1);
		LVDS_WRAP_SetMuxOutput(TS_MUX_TYPE, TS_MUX_IDX0, TS_MUX_PATH_CORE, 1);
		LVDS_WRAP_SetDataArray(TS_MUX_IDX0, sel0);
		LVDS_WRAP_SetMuxOutput(TS_MUX_TYPE, TS_MUX_IDX1, TS_MUX_PATH_CORE, 1);
		LVDS_WRAP_SetDataArray(TS_MUX_IDX1, sel1);
	}
	else{
		LVDS_WRAP_SetMuxOutput(TS_MUX_TYPE, val, select, 1);
		LVDS_WRAP_SetDataArray(val, sel0);
	}
}

/* LVDS_WRAP_SetDataSwap
 * Set Tx splitter output data swap
 * ch : Tx splitter output channel(0, 1, 2, 3)
 * set : Tx splitter data swap mode
 */
void LVDS_WRAP_SetDataSwap(unsigned int ch,
			      unsigned int set)
{
	volatile void __iomem *reg = (volatile void __iomem *)REG_LVDS_WRAP;

	if (reg) {
		unsigned int val;

		switch (ch) {
		case 0:
			val = (readl(reg + TS_CFG) &
			       ~(TS_CFG_SWAP0_MASK));
			val |= ((set & 0x3) << TS_CFG_SWAP0_SHIFT);
			writel(val, reg + TS_CFG);
			break;
		case 1:
			val = (readl(reg + TS_CFG) &
			       ~(TS_CFG_SWAP1_MASK));
			val |= ((set & 0x3) << TS_CFG_SWAP1_SHIFT);
			writel(val, reg + TS_CFG);
			break;
		case 2:
			val = (readl(reg + TS_CFG) &
			       ~(TS_CFG_SWAP2_MASK));
			val |= ((set & 0x3) << TS_CFG_SWAP2_SHIFT);
			writel(val, reg + TS_CFG);
			break;
		case 3:
			val = (readl(reg + TS_CFG) &
			       ~(TS_CFG_SWAP3_MASK));
			val |= ((set & 0x3) << TS_CFG_SWAP3_SHIFT);
			writel(val, reg + TS_CFG);
			break;
		default:
			pr_err("%s: invalid parameter(%d, %d)\n", __func__, ch,
			       set);
			break;
		}
	}
}

/* LVDS_WRAP_SetMuxOutput
 * Set Tx splitter MUX output selection
 * mux: the type of mux (DISP_MUX_TYPE, TS_MUX_TYPE)
 * select : the select
 */
void LVDS_WRAP_SetMuxOutput(MUX_TYPE mux, unsigned int ch,
			       unsigned int select, unsigned int enable)
{
	volatile void __iomem *reg = (volatile void __iomem *)REG_LVDS_WRAP;
	unsigned int val;

	if (reg) {
		switch (mux) {
		case DISP_MUX_TYPE:
			val = (readl(reg + DISP_MUX_SEL) &
					~(DISP_MUX_SEL_SEL_MASK));
			val |= ((select & 0x3) << DISP_MUX_EN_EN_SHIFT);
			writel(val, reg + DISP_MUX_SEL);
			val = (readl(reg + DISP_MUX_EN) &
					~(DISP_MUX_EN_EN_MASK));
			val |= ((enable & 0x1) << DISP_MUX_EN_EN_SHIFT);
			writel(val, reg + DISP_MUX_EN);
			break;
		case TS_MUX_TYPE:
			switch (ch) {
			case 0:
				val = (readl(reg + TS_MUX_SEL0) &
				       ~(TS_MUX_SEL_SEL_MASK));
				val |= ((select & 0x7) << TS_MUX_SEL_SEL_SHIFT);
				writel(val, reg + TS_MUX_SEL0);
				val = (readl(reg + TS_MUX_EN0) &
				       ~(TS_MUX_EN_EN_MASK));
				val |= ((enable & 0x1) << TS_MUX_EN_EN_SHIFT);
				writel(val, reg + TS_MUX_EN0);
				break;
			case 1:
				val = (readl(reg + TS_MUX_SEL1) &
				       ~(TS_MUX_SEL_SEL_MASK));
				val |= ((select & 0x7) << TS_MUX_SEL_SEL_SHIFT);
				writel(val, reg + TS_MUX_SEL1);
				val = (readl(reg + TS_MUX_EN1) &
				       ~(TS_MUX_EN_EN_MASK));
				val |= ((enable & 0x1) << TS_MUX_EN_EN_SHIFT);
				writel(val, reg + TS_MUX_EN1);
				break;
			case 2:
				val = (readl(reg + TS_MUX_SEL2) &
				       ~(TS_MUX_SEL_SEL_MASK));
				val |= ((select & 0x7) << TS_MUX_SEL_SEL_SHIFT);
				writel(val, reg + TS_MUX_SEL2);
				val = (readl(reg + TS_MUX_EN2) &
				       ~(TS_MUX_EN_EN_MASK));
				val |= ((enable & 0x1) << TS_MUX_EN_EN_SHIFT);
				writel(val, reg + TS_MUX_EN2);
				break;
			case 3:
				val = (readl(reg + TS_MUX_SEL3) &
				       ~(TS_MUX_SEL_SEL_MASK));
				val |= ((select & 0x7) << TS_MUX_SEL_SEL_SHIFT);
				writel(val, reg + TS_MUX_SEL3);
				val = (readl(reg + TS_MUX_EN3) &
				       ~(TS_MUX_EN_EN_MASK));
				val |= ((enable & 0x1) << TS_MUX_EN_EN_SHIFT);
				writel(val, reg + TS_MUX_EN3);
				break;
			default:
				goto error_mux_output;
			}
			break;
		case MUX_TYPE_MAX:
		default:
			goto error_mux_output;
		}
	}
	return;
error_mux_output:
	pr_err("%s in error, invalid parameter(mux: %d, ch: %d) \n", __func__,
	       mux, ch);
}

/* LVDS_WRAP_SetDataPath
 * Set Data output format of tx splitter
 * ch : channel number of tx splitter mux
 * path : path number of tx splitter mux
 * set : data output format of tx splitter mux
 */
void LVDS_WRAP_SetDataPath(unsigned int ch, unsigned int path,
			      unsigned int set)
{
	volatile void __iomem *reg = (volatile void __iomem *)REG_LVDS_WRAP;
	unsigned int offset;

	//if ((path < 0) || (path >= TS_TXOUT_SEL_MAX))
	if (path >= TS_TXOUT_SEL_MAX) //warning. comparison of unsigned experssin <0 is always false
		goto error_data_path;

	if (reg) {
		switch (ch) {
		case 0:
			offset = TXOUT_SEL0_0;
			break;
		case 1:
			offset = TXOUT_SEL0_1;
			break;
		case 2:
			offset = TXOUT_SEL0_2;
			break;
		case 3:
			offset = TXOUT_SEL0_3;
			break;
		default:
			goto error_data_path;
		}

		writel((set & 0xFFFFFFFF), reg + (offset + (0x4 * path)));
	}
	return;
error_data_path:
	pr_err("%s in error, invalid parameter(ch: %d, path: %d) \n", __func__,
	       ch, path);
}

/* LVDS_WRAP_SetDataArray
 * Set the data output format of tx splitter mux
 * ch : channel number of tx splitter mux
 * data : the array included the data output format
 */
void LVDS_WRAP_SetDataArray(unsigned int ch, unsigned int data[TXOUT_MAX_LINE][TXOUT_DATA_PER_LINE])
{
	volatile void __iomem *reg = (volatile void __iomem *)REG_LVDS_WRAP;
	unsigned int *lvdsdata = (unsigned int *)data;
	unsigned int idx, value, path;
	unsigned int data0, data1, data2, data3;

	//if ((ch < 0) || (ch >= TS_MUX_IDX_MAX))
	if (ch >= TS_MUX_IDX_MAX) //warning. comparison of unsigned experssin <0 is always false
		goto error_data_array;

	if(reg) {
		for(idx = 0; idx < (TXOUT_MAX_LINE * TXOUT_DATA_PER_LINE);  idx +=4)
		{
			data0 = TXOUT_GET_DATA(idx);
			data1 = TXOUT_GET_DATA(idx + 1);
			data2 = TXOUT_GET_DATA(idx + 2);
			data3 = TXOUT_GET_DATA(idx + 3);

			path = idx / 4;
			value = ((lvdsdata[data3] << 24) | (lvdsdata[data2] << 16)| (lvdsdata[data1] << 8)| (lvdsdata[data0]));
			LVDS_WRAP_SetDataPath(ch, path, value);
		}
	}
	return;
error_data_array:
	pr_err("%s in error, invalid parameter(ch: %d) \n",
			__func__, ch);
}

/* LVDS_WRAP_SetAccessCode
 * Set the access code of LVDS Wrapper
 */
void LVDS_WRAP_SetAccessCode(void)
{
	void __iomem *reg = (void __iomem *)REG_LVDS_WRAP;
	if(readl(reg+CFG_PROTECT) == LVDS_WRAP_ACCESS_CODE)
		return;

	/* Please delete this code, after making a decision about safety mechanism */
	writel(LVDS_WRAP_ACCESS_CODE, reg+CFG_PROTECT);
}

/* LVDS_WRAP_REsetPHY
 * software reset for PHY port
 */
void LVDS_WRAP_ResetPHY(uint32 port, uint32 reset)
{
	volatile void __iomem *reg = (volatile void __iomem *)REG_LVDS_WRAP + TS_SWRESET;

	if(reset == 1){
		switch(port){
			case TS_MUX_IDX0:
			case TS_MUX_IDX1:
				writel(readl(reg) | (0x1 << 4), reg); //LVDS_PHY_2PORT_SYS
				break;
			case TS_MUX_IDX2:
				writel(readl(reg) | (0x1 << 3), reg); //LVDS_PHY_1PORT_SYS
				break;
			default:
				break;
		}
	}
	writel(0x0,reg);
}
