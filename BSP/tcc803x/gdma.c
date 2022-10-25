/****************************************************************************
 *   FileName    : gdma.c
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
#include <osal.h>

#include "gdma.h"
#include "clock.h"
#include "tcc_ckc.h"
#include "reg_phys.h"
#include <bsp_int.h>
#include "device_share_config.h"
#include <debug.h>

// GDMA Register offsets.
//
#define DMAC_ISR        (0x00UL)
#define DMAC_ITCSR      (0x04UL)
#define DMAC_ITCCR      (0x08UL)
#define DMAC_IESR       (0x0CUL)
#define DMAC_IECR       (0x10UL)
#define DMAC_RITCSR     (0x14UL)
#define DMAC_REISR      (0x18UL)
#define DMAC_ECR        (0x1CUL)
#define DMAC_SBRR       (0x20UL)
#define DMAC_SSRR       (0x24UL)
#define DMAC_SLBRR      (0x28UL)
#define DMAC_SLSRR      (0x2CUL)
#define DMAC_CR         (0x30UL)
#define DMAC_SR         (0x34UL)

#define DMA_CH_SRC_ADDR(x)      ((0x100UL + ((x) * 0x20UL)))
#define DMA_CH_DST_ADDR(x)      ((0x104UL + ((x) * 0x20UL)))
#define DMA_CH_LLI(x)           ((0x108UL + ((x) * 0x20UL)))
#define DMA_CH_CON(x)           ((0x10CUL + ((x) * 0x20UL)))
#define DMA_CH_CONFIG(x)        ((0x110UL + ((x) * 0x20UL)))

#define DMA_MAX_XFER_SIZE   (0xFFFUL)

// GDMA Interrupt Flag Group
// TCC803X Has 1 GDMA Controllers
static uint32 GDMAEvtFlgGrpID;
#define GDMA_EVT_FLAG_GRP_NAME          "GDMA_Event_Group"
#define GDMA_EVT_FLAG_GRP_TIMEOUT       1000

static void gdma_isr(void *arg);
static int32 gdma_wait_intr(const struct gdma_data *dmacon);
static void gdma_tcint_enable(const struct gdma_data *dmacon);
static void gdma_set_inc(const struct gdma_data *dmacon, uint8 dest_inc, uint32 src_inc);
static void gdma_set_transferwidth(const struct gdma_data *dmacon, uint8 dest_width, uint32 src_width);
static void gdma_set_burstsize(const struct gdma_data *dmacon, uint8 dest_burst, uint32 src_burst);
static void gdma_set_transfersize(const struct gdma_data *dmacon, uint32 transfer_size);
static void gdma_ch_enable(const struct gdma_data *dmacon);
static void gdma_ch_disable(const struct gdma_data *dmacon);
static void gdma_set_src_addr(const struct gdma_data *dmacon, uint32 addr);
static void gdma_set_dest_addr(const struct gdma_data *dmacon, uint32 addr);
static void gdma_set_lli_addr(const struct gdma_data *dmacon, uint32 lli_addr);
static void gdma_set_flowcontrol(const struct gdma_data *dmacon, uint32 flow);
static void gdma_set_peri(const struct gdma_data *dmacon, uint8 dest_peri, uint8 src_peri);
static void gdma_tcint_disable(const struct gdma_data *dmacon);

static void gdma_tcint_enable(const struct gdma_data *dmacon)
{
	// Enable terminal count interrupt
	writel(readl(TCC_GDMA_BASE+DMA_CH_CON(dmacon->ch)) | (1UL<<31),
	       TCC_GDMA_BASE+DMA_CH_CON(dmacon->ch));
	// Mask terminal count interrupt
	writel(readl(TCC_GDMA_BASE+DMA_CH_CONFIG(dmacon->ch)) | (1UL<<15),
	       TCC_GDMA_BASE+DMA_CH_CONFIG(dmacon->ch));
}

static void gdma_tcint_disable(const struct gdma_data *dmacon)
{
	// Disable terminal count interrupt
	writel(readl(TCC_GDMA_BASE+DMA_CH_CON(dmacon->ch)) & ~(1UL<<31),
	       TCC_GDMA_BASE+DMA_CH_CON(dmacon->ch));
	// Unask terminal count interrupt
	writel(readl(TCC_GDMA_BASE+DMA_CH_CONFIG(dmacon->ch)) & ~(1UL<<15),
	       TCC_GDMA_BASE+DMA_CH_CONFIG(dmacon->ch));
}

static void gdma_set_inc(const struct gdma_data *dmacon, uint8 dest_inc, uint32 src_inc)
{
	uint32 inc;
	uint32 temp;

	temp = readl(TCC_GDMA_BASE+DMA_CH_CON(dmacon->ch));
	//clear desc, src increment
	temp &= ~(0x3UL<<26);

	inc = ((dest_inc&0x1UL)<<1) | (src_inc&0x1UL);
	temp |= (inc<<26);

	writel(temp, TCC_GDMA_BASE+DMA_CH_CON(dmacon->ch));
}

static void gdma_set_transferwidth(const struct gdma_data *dmacon, uint8 dest_width, uint32 src_width)
{
	uint32 transfer_width;
	uint32 temp;

	temp = readl(TCC_GDMA_BASE+DMA_CH_CON(dmacon->ch));
	//clear desc, src transfer width
	temp &= ~(0x3fUL<<18);

	transfer_width = ((dest_width&0x7UL)<<3) | (src_width&0x7UL);
	temp |= (transfer_width<<18);

	writel(temp, TCC_GDMA_BASE+DMA_CH_CON(dmacon->ch));
}

static void gdma_set_burstsize(const struct gdma_data *dmacon, uint8 dest_burst, uint32 src_burst)
{
	uint32 burst_size;
	uint32 temp;

	temp = readl(TCC_GDMA_BASE+DMA_CH_CON(dmacon->ch));
	//clear desc, src burst size
	temp &= ~(0x3fUL<<12);
	burst_size = ((dest_burst&0x7UL)<<3) | (src_burst&0x7UL);
	temp |= (burst_size<<12);

	writel(temp, TCC_GDMA_BASE+DMA_CH_CON(dmacon->ch));
}

static void gdma_set_transfersize(const struct gdma_data *dmacon, uint32 transfer_size)
{
	uint32 temp;

	temp = readl(TCC_GDMA_BASE+DMA_CH_CON(dmacon->ch));
	//clear transfer size
	temp &= ~(0x0fffUL);
	temp |= transfer_size & 0x0fffUL;
	writel(temp, TCC_GDMA_BASE+DMA_CH_CON(dmacon->ch));
}

static void gdma_set_flowcontrol(const struct gdma_data *dmacon, uint32 flow)
{
	uint32 temp;

	temp = readl(TCC_GDMA_BASE+DMA_CH_CONFIG(dmacon->ch));
	temp &= ~(0x7UL<<11);
	temp |= (flow & 0x7UL)<<11;
	writel(temp, TCC_GDMA_BASE+DMA_CH_CONFIG(dmacon->ch));
}

static void gdma_set_peri(const struct gdma_data *dmacon, uint8 dest_peri, uint8 src_peri)
{
	uint32 temp;

	temp = readl(TCC_GDMA_BASE+DMA_CH_CONFIG(dmacon->ch));
	temp &= ~(0x7feUL);
	temp |= (dest_peri & 0xfUL)<<6;
	temp |= (src_peri & 0xfUL)<<1;
	writel(temp, TCC_GDMA_BASE+DMA_CH_CONFIG(dmacon->ch));
}

static void gdma_isr(void *arg)
{
	uint32 status;
	uint32 flags;
	int32 err;
	uint32 i;

	status = readl(TCC_GDMA_BASE+DMAC_ITCSR);
	if (status!=0UL) {
		for (i = 0; i < (uint32)DMA_CH_MAX; i++) {
			if ((status & (1UL<<i)) != 0UL) {
				flags = (1UL<<i);

				err = OSAL_SetEvent(GDMAEvtFlgGrpID, flags, (uint32)OSAL_EVENT_SET_OPT_FLAG_SET, 0);
				if (err != OSAL_ERR_NONE)
				{
					return;
				}

				// Clear terminal count interrupt
				writel((1UL<<i), TCC_GDMA_BASE + DMAC_ITCCR);
			}
		}
	}
}

static int32 gdma_wait_intr(const struct gdma_data *dmacon)
{
	int32 err = 0;
	uint32 flags, retFlags;

	flags = (1UL<<dmacon->ch);

	retFlags= OSAL_GetEvent(GDMAEvtFlgGrpID, flags, 0,
		(OSAL_EVENT_OPT_SET_ALL|OSAL_EVENT_OPT_CONSUME|OSAL_API_OPT_BLOCKING), &err, 0);
	// dummy for codesonar
	if(err != OSAL_ERR_NONE)
	{
		return err;
	}
	if((retFlags & flags) != 0UL) //Msg(4:3344) Value of control expression is not derived from an explicit logical operation.MISRA-C:2004 Rule 13.2
	{
		(void)OSAL_SetEvent(GDMAEvtFlgGrpID, flags, (uint32)OSAL_EVENT_SET_OPT_FLAG_CLR, 0); //Msg(4:3200) 'OSAL_SetEvent' returns a value which is not being used. MISRA-C:2004 Rule 16.10
	} else 
	{
		retFlags = 1;
	}

	return 0;
}

static void gdma_ch_enable(const struct gdma_data *dmacon)
{
	writel(readl(TCC_GDMA_BASE+DMA_CH_CONFIG(dmacon->ch)) | 0x1UL,
	       TCC_GDMA_BASE+DMA_CH_CONFIG(dmacon->ch));
	(void)gdma_wait_intr(dmacon);
}

static void gdma_ch_disable(const struct gdma_data *dmacon)
{
	writel(readl(TCC_GDMA_BASE+DMA_CH_CONFIG(dmacon->ch)) & ~(0x1UL),
	       TCC_GDMA_BASE+DMA_CH_CONFIG(dmacon->ch));
}

static void gdma_set_src_addr(const struct gdma_data *dmacon, uint32 addr)
{
	writel(addr, TCC_GDMA_BASE+DMA_CH_SRC_ADDR(dmacon->ch));
}

static void gdma_set_dest_addr(const struct gdma_data *dmacon, uint32 addr)
{
	writel(addr, TCC_GDMA_BASE+DMA_CH_DST_ADDR(dmacon->ch));
}

static void gdma_set_lli_addr(const struct gdma_data *dmacon, uint32 lli_addr)
{
	writel(lli_addr, TCC_GDMA_BASE+DMA_CH_LLI(dmacon->ch));
}

int32 gdma_transfer_m2m(const struct gdma_data *dmacon, uint32 src_addr,
		       uint32 dest_addr, uint32 len)
{
	int32 ret;
	uint32 word_len, word_len_rest;
	uint32 part_num, part_rest, i;
	uint32 src_addr_rest, dest_addr_rest;

	if (((src_addr % sizeof(int32)) != 0UL) || ((dest_addr % sizeof(int32)) != 0UL))  //Msg(4:5013) Use of basic type 'int'.MISRA-C:2004 Rule 6.3 Msg(4:4107) Both operands of || operator are arithmetic or bitwise expressions. MISRA-C:2004 Rule 12.6; REFERENCE - ISO 6.3.14
	{
		/* align check */
		ret = OSAL_ERR_INVALID_PARAMETER;
		return ret;
	}

	word_len = (len / 4UL);
	word_len_rest = len % 4UL;
	part_num = word_len / DMA_MAX_XFER_SIZE;
	part_rest = word_len % DMA_MAX_XFER_SIZE;
	src_addr_rest = src_addr;
	dest_addr_rest = dest_addr;

	gdma_set_flowcontrol(dmacon, DMA_FLOW_TYPE_M2M);
	gdma_set_inc(dmacon, DMA_INC, DMA_INC);
	gdma_set_burstsize(dmacon, DMA_BURST_SIZE_256, DMA_BURST_SIZE_256);

	/* max transfer */
	for (i = 0; i < part_num; i++) {
		gdma_set_src_addr(dmacon, src_addr_rest);
		gdma_set_dest_addr(dmacon, dest_addr_rest);
		gdma_tcint_enable(dmacon);
		gdma_set_transferwidth(dmacon, DMA_TRANSFER_SIZE_WORD, DMA_TRANSFER_SIZE_WORD);
		gdma_set_transfersize(dmacon, (uint32)DMA_MAX_XFER_SIZE);
		gdma_ch_enable(dmacon);
		src_addr_rest += (DMA_MAX_XFER_SIZE * 4UL);
		dest_addr_rest += (DMA_MAX_XFER_SIZE * 4UL);
	}

	/* word transfer */
	if (part_rest != 0UL) { //Msg(4:3344) Value of control expression is not derived from an explicit logical operation. MISRA-C:2004 Rule 13.
		gdma_set_src_addr(dmacon, src_addr_rest);
		gdma_set_dest_addr(dmacon, dest_addr_rest);
		gdma_tcint_enable(dmacon);
		gdma_set_transferwidth(dmacon, DMA_TRANSFER_SIZE_WORD, DMA_TRANSFER_SIZE_WORD);
		gdma_set_transfersize(dmacon, part_rest);
		gdma_ch_enable(dmacon);
		src_addr_rest += (part_rest * 4UL);
		dest_addr_rest += (part_rest * 4UL);
	}

	/* byte transfer */
	if (word_len_rest != 0UL) {//Msg(4:3344) Value of control expression is not derived from an explicit logical operation. MISRA-C:2004 Rule 13.
		gdma_set_src_addr(dmacon, src_addr_rest);
		gdma_set_dest_addr(dmacon, dest_addr_rest);
		gdma_tcint_enable(dmacon);
		gdma_set_transferwidth(dmacon, DMA_TRANSFER_SIZE_BYTE, DMA_TRANSFER_SIZE_BYTE);
		gdma_set_transfersize(dmacon, word_len_rest);
		gdma_ch_enable(dmacon);
	}

	ret = OSAL_ERR_NONE;
	return ret;
}

int32 init_gdma(struct gdma_data *dmacon, uint32 int_prio)
{
	int32 err;

	/*if (!ConfirmDeviceChannel(SHARE_DEVICE_UDMA, dmacon->con)) {
		return -1;
	}*/

	(void)tcc_iobus_disable(IOBUS_DMA_GRP);
	(void)tcc_iobus_enable(IOBUS_DMA_GRP);

	// DMA Controller Already initialized.
	/*if((readl(TCC_GDMA_BASE+DMAC_CR) & 0x1)!=0)
	{
		return 0;
	}*/

	// Disable DMA Contorller
	writel(readl(TCC_GDMA_BASE+DMAC_CR) & ~(0x1UL), TCC_GDMA_BASE + DMAC_CR);
	// Clear Interrupt
	writel(0x7F, TCC_GDMA_BASE + DMAC_ITCCR);
	writel(0x7F, TCC_GDMA_BASE + DMAC_IECR);

	err = OSAL_EventCreate((uint32 *)&GDMAEvtFlgGrpID, (const uint8 *)GDMA_EVT_FLAG_GRP_NAME, 0, 0);
	if(err != OSAL_ERR_NONE)
	{
		return err;
	}

	(void)BSP_IntVectSet((uint32)INT_DMA_PL080, int_prio, INT_TYPE_LEVEL_HIGH, gdma_isr, dmacon);
	BSP_IntSrcEn((uint32)INT_DMA_PL080);

	// Enable DMA Controller
	writel(0x1, TCC_GDMA_BASE + DMAC_CR);

	return 0;
}

