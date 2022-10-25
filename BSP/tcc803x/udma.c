/****************************************************************************
 *   FileName    : udma.c
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
#include <bsp_int.h>
#include "udma.h"
#include "clock.h"
#include "tcc_ckc.h"
#include "reg_phys.h"

// UART DMA Base Address
#define UDMA_CON0_BASE	(TCC_UDMA_BASE)
#define UDMA_CON1_BASE	(TCC_UDMA_BASE+0x10000)
#define UDMA_CON2_BASE	(TCC_UDMA_BASE+0x20000)
#define UDMA_CON3_BASE	(TCC_UDMA_BASE+0x30000)

#define UDMA_CON_BASE(x)	(TCC_UDMA_BASE + ((x) * 0x10000))

// UART DMA Register offsets
#define UDMA_ISR	(0x00)	// Interrupt Status Register
#define UDMA_ITCSR	(0x04)	// Interrupt Terminal Count Status Register
#define UDMA_ITCCR	(0x08)	// Interrupt Terminal Count Clear Register
#define UDMA_IESR	(0x0C)	// Interrupt Error Status Register
#define UDMA_IECR	(0x10)	// Interrupt Error Clear Register
#define UDMA_RITCSR	(0x14)	// Raw Interrupt Terminal Count Status Register
#define UDMA_REISR	(0x18)	// Raw Error Interrupt Status Register
#define UDMA_ECR	(0x1C)	// Enabled Channel Register
#define UDMA_SBRR	(0x20)	// Software Burst Request Register
#define UDMA_SSRR	(0x24)	// Software Single Request Register
#define UDMA_SLBRR	(0x28)	// Software Last Burst Request Register
#define UDMA_SLSRR	(0x2C)	// Software Last Single Request Register
#define UDMA_CR		(0x30)	// Configuration Register
#define UDMA_SR		(0x34)	// Reserved

#define UDMA_CH_SRC_ADDR(x)	((0x100 + ((x) * 0x20)))
#define UDMA_CH_DST_ADDR(x)	((0x104 + ((x) * 0x20)))
#define UDMA_CH_LLI(x)		((0x108 + ((x) * 0x20)))
#define UDMA_CH_CON(x)		((0x10C + ((x) * 0x20)))
#define UDMA_CH_CONFIG(x)	((0x110 + ((x) * 0x20)))

#define CH0_FLAG 	(1 << 0)
#define CH1_FLAG 	(1 << 1)

struct udma_con_data
{
	uint32 con;
	struct udma_data *ch[2];
};
#if 0 //Msg(4:3218) File scope static, 'udma_con_table', only accessed in one function. MISRA-C:2004 Rule 8.7
static struct udma_con_data udma_con_table[4] =
{
	{0, {NULL, NULL}},
	{1, {NULL, NULL}},
	{2, {NULL, NULL}},
	{3, {NULL, NULL}}
};
#endif
static void udma_isr(void *arg);

void udma_tcint_enable(const struct udma_data *dmacon)
{
	// Enable terminal count interrupt
	writel(readl(UDMA_CON_BASE(dmacon->con)+UDMA_CH_CON(dmacon->ch)) | (1UL << 31),
	       UDMA_CON_BASE(dmacon->con)+UDMA_CH_CON(dmacon->ch));
	// Mask terminal count interrupt
	writel(readl(UDMA_CON_BASE(dmacon->con)+UDMA_CH_CONFIG(dmacon->ch)) | (1UL << 15),
	       UDMA_CON_BASE(dmacon->con)+UDMA_CH_CONFIG(dmacon->ch));
}

void udma_tcint_disable(const struct udma_data *dmacon)
{
	// Disable terminal count interrupt
	writel(readl(UDMA_CON_BASE(dmacon->con)+UDMA_CH_CON(dmacon->ch)) & ~(1UL << 31),
	       UDMA_CON_BASE(dmacon->con)+UDMA_CH_CON(dmacon->ch));
	// Unask terminal count interrupt
	writel(readl(UDMA_CON_BASE(dmacon->con)+UDMA_CH_CONFIG(dmacon->ch)) & ~(1UL << 15),
	       UDMA_CON_BASE(dmacon->con)+UDMA_CH_CONFIG(dmacon->ch));
}

void udma_set_inc(const struct udma_data *dmacon, uint8 dest_inc, uint32 src_inc)
{
	uint32 inc;
	uint32 temp;

	temp = readl(UDMA_CON_BASE(dmacon->con)+UDMA_CH_CON(dmacon->ch));
	//clear desc, src increment
	temp &= ~(0x3UL << 26);

	inc = ((dest_inc & 0x1UL) << 1) | (src_inc & 0x1);
	temp |= (inc << 26);

	writel(temp, UDMA_CON_BASE(dmacon->con)+UDMA_CH_CON(dmacon->ch));
}

void udma_set_transferwidth(const struct udma_data *dmacon, uint8 dest_width, uint32 src_width)
{
	uint32 transfer_width;
	uint32 temp;

	temp = readl(UDMA_CON_BASE(dmacon->con)+UDMA_CH_CON(dmacon->ch));
	//clear desc, src transfer width
	temp &= ~(0x3FUL << 18);
	transfer_width = ((dest_width & 0x7UL) << 3) | (src_width & 0x7UL);
	temp |= (transfer_width << 18);

	writel(temp, UDMA_CON_BASE(dmacon->con)+UDMA_CH_CON(dmacon->ch));
}

void udma_set_burstsize(const struct udma_data *dmacon, uint8 dest_burst, uint32 src_burst)
{
	uint32 burst_size;
	uint32 temp;

	temp = readl(UDMA_CON_BASE(dmacon->con)+UDMA_CH_CON(dmacon->ch));
	//clear desc, src burst size
	temp &= ~(0x3FUL << 12);
	burst_size = ((dest_burst & 0x7UL) << 3) | (src_burst & 0x7UL);
	temp |= (burst_size << 12);

	writel(temp, UDMA_CON_BASE(dmacon->con)+UDMA_CH_CON(dmacon->ch));
}

void udma_set_transfersize(const struct udma_data *dmacon, uint32 transfer_size)
{
	uint32 temp;

	temp = readl(UDMA_CON_BASE(dmacon->con)+UDMA_CH_CON(dmacon->ch));
	//clear transfer size
	temp &= ~(0xFFF);
	temp |= transfer_size & 0xFFF;
	writel(temp, UDMA_CON_BASE(dmacon->con)+UDMA_CH_CON(dmacon->ch));
}

void udma_set_flowcontrol(const struct udma_data *dmacon, uint32 flow)
{
	uint32 temp;

	temp = readl(UDMA_CON_BASE(dmacon->con)+UDMA_CH_CONFIG(dmacon->ch));
	temp &= ~(0x7UL << 11);
	temp |= ((flow & 0x7UL) << 11);
	writel(temp, UDMA_CON_BASE(dmacon->con)+UDMA_CH_CONFIG(dmacon->ch));
}

void udma_set_peri(const struct udma_data *dmacon, uint8 dest_peri, uint8 src_peri)
{
	uint32 temp;

	temp = readl(UDMA_CON_BASE(dmacon->con)+UDMA_CH_CONFIG(dmacon->ch));
	temp &= ~(0x7FEUL);
	temp |= (dest_peri & 0xFUL)<<6;
	temp |= (src_peri & 0xFUL)<<1;
	writel(temp, UDMA_CON_BASE(dmacon->con)+UDMA_CH_CONFIG(dmacon->ch));
}

uint32 udma_get_transfersize(const struct udma_data *dmacon)
{
	uint32 size;

	size = (readl(UDMA_CON_BASE(dmacon->con)+UDMA_CH_CON(dmacon->ch)) & (0xFFF));

	return size;
}

static void udma_isr(void *arg)
{
	uint32 status, udma_cnt=0;
	struct udma_con_data *dmacon_table = (struct udma_con_data *)arg;
	struct udma_data *dmacon = NULL;

	status = readl(UDMA_CON_BASE(dmacon_table->con) + UDMA_ITCSR);
	if (status != 0) {
		if ((status & CH0_FLAG) != 0) {
			dmacon = (struct udma_data *)dmacon_table->ch[0];
			// Clear terminal count interrupt
			writel(CH0_FLAG, UDMA_CON_BASE(dmacon->con) + UDMA_ITCCR);
		}
		if ((status & CH1_FLAG) != 0) {
			dmacon = (struct udma_data *)dmacon_table->ch[1];
			if (dmacon == NULL) //Codesonar : Null Test After Dereference
			{
				return;
			}
			
			// Clear terminal count interrupt
			writel(CH1_FLAG, UDMA_CON_BASE(dmacon->con) + UDMA_ITCCR);
			//if (dmacon == NULL) //Codesonar : Null Test After Dereference
			//{
			//	return;
			//}
			
			udma_ch_disable(dmacon); // channel disabled

			// break loop if the FIFO is not empty after 2ms. time out.
			// 6250 loops for 1ms can be changed depending on Cortex-R5 Single Core clock speed.
			// 6250 loops for 1ms is based on 600MHz which is max clock speed for Cortex-R5
			// clock speed lower than 600MHz is okay because 1ms is guaranteed for 6250 loop to have enough time that isr is complete after FIFO is empty.
			// if it is higher than 600MHz, isr can be complete too fast before FIFO is empty.
			while (udma_cnt <=12500) //// 12500 : 2ms, 6250 : 1ms
			{
				// check the Active flag
				// 0 = there is no data in the FIFO of the channel
				if ((readl(UDMA_CON_BASE(dmacon->con) + UDMA_CH_CONFIG(dmacon->ch)) & (0x20000)) == 0)
				{
					break;
				}
				#if 0
				else if(udma_cnt==12500) // 12500 : 2ms, 6250 : 1ms
				{
					break;
				}
				#endif
				udma_cnt+=1;
			}
			dmacon->is_run = 0;

			return;


		}
	}

	if (dmacon == NULL)
	{
		return;
	}

	udma_ch_disable(dmacon); // channel disabled

	// break loop if the FIFO is not empty after 2ms. time out.
	// 6250 loops for 1ms can be changed depending on Cortex-R5 Single Core clock speed.
	// 6250 loops for 1ms is based on 600MHz which is max clock speed for Cortex-R5
	// clock speed lower than 600MHz is okay because 1ms is guaranteed for 6250 loop to have enough time that isr is complete after FIFO is empty.
	// if it is higher than 600MHz, isr can be complete too fast before FIFO is empty.
	while (udma_cnt<=12500) // 12500 : 2ms, 6250 : 1ms
	{
		// check the Active flag
		// 0 = there is no data in the FIFO of the channel
		if ((readl(UDMA_CON_BASE(dmacon->con) + UDMA_CH_CONFIG(dmacon->ch)) & (0x20000)) == 0)
		{
			break;
		}
		#if 0 //Msg(4:0771) More than one break statement is being used to terminate an iteration statement. MISRA-C:2004 Rule 14.6
		else if(udma_cnt==12500) // 12500 : 2ms, 6250 : 1ms
		{
			break;
		}
		#endif
		udma_cnt+=1;
	}
	udma_set_src_addr(dmacon, (uint32)dmacon->src_addr);
	udma_set_dest_addr(dmacon, (uint32)dmacon->dest_addr);
	udma_tcint_enable(dmacon);
	udma_set_transferwidth(dmacon, UDMA_TRANSFER_SIZE_BYTE, UDMA_TRANSFER_SIZE_BYTE);
	udma_set_transfersize(dmacon, dmacon->buff_size);

	// Run DMA
	udma_ch_enable(dmacon);

	dmacon->is_run = 0;
}

void udma_ch_enable(const struct udma_data *dmacon)
{
	uint32 ch_conf;

	ch_conf = readl(UDMA_CON_BASE(dmacon->con)+UDMA_CH_CONFIG(dmacon->ch));
	ch_conf &= ~(1 << 18);	// enable DMA requests
	ch_conf |= (1 << 0);	// channel enabled

	writel(ch_conf, UDMA_CON_BASE(dmacon->con)+UDMA_CH_CONFIG(dmacon->ch));
}

void udma_ch_disable(const struct udma_data *dmacon)
{
	uint32 ch_conf;

	ch_conf = readl(UDMA_CON_BASE(dmacon->con)+UDMA_CH_CONFIG(dmacon->ch));
	ch_conf |= (1 << 18);	// ignore subsequent source DMA requests
	ch_conf &= ~(1 << 0);	// channel disabled

	writel(ch_conf, UDMA_CON_BASE(dmacon->con)+UDMA_CH_CONFIG(dmacon->ch));
}

void udma_set_src_addr(const struct udma_data *dmacon, uint32 addr)
{
	writel(addr, UDMA_CON_BASE(dmacon->con)+UDMA_CH_SRC_ADDR(dmacon->ch));
}

void udma_set_dest_addr(const struct udma_data *dmacon, uint32 addr)
{
	writel(addr, UDMA_CON_BASE(dmacon->con)+UDMA_CH_DST_ADDR(dmacon->ch));
}

void udma_set_lli_addr(const struct udma_data *dmacon, uint32 lli_addr)
{
	writel(lli_addr, UDMA_CON_BASE(dmacon->con)+UDMA_CH_LLI(dmacon->ch));
}

uint32 udma_get_dest_addr(const struct udma_data *dmacon)
{
	return readl(UDMA_CON_BASE(dmacon->con)+UDMA_CH_DST_ADDR(dmacon->ch));
}

void udma_transfer_m2m(const struct udma_data *dmacon, uint32 src_addr,
		       uint32 dest_addr, uint32 len)
{
	// DMA Flow control set to memory to memory transfer
	udma_set_flowcontrol(dmacon, UDMA_FLOW_TYPE_M2M);
	udma_set_inc(dmacon, UDMA_INC, UDMA_INC);
	udma_set_burstsize(dmacon, UDMA_BURST_SIZE_256, UDMA_BURST_SIZE_256);
	udma_set_src_addr(dmacon, src_addr);
	udma_set_dest_addr(dmacon, dest_addr);
	udma_tcint_enable(dmacon);
	udma_set_transferwidth(dmacon, UDMA_TRANSFER_SIZE_WORD, UDMA_TRANSFER_SIZE_WORD);
	udma_set_transfersize(dmacon, len);
	udma_ch_enable(dmacon);
}

int32 udma_init(struct udma_data *dmacon, uint32 int_prio)
{
	 //Msg(4:3218) File scope static, 'udma_con_table', only accessed in one function. MISRA-C:2004 Rule 8.7
	static struct udma_con_data udma_con_table[4] =
	{
		{0, {NULL, NULL}},
		{1, {NULL, NULL}},
		{2, {NULL, NULL}},
		{3, {NULL, NULL}}
	};

	(void)tcc_iobus_enable(IOBUS_UDMA0 + dmacon->con);

	udma_con_table[dmacon->con].con = dmacon->con;
	udma_con_table[dmacon->con].ch[dmacon->ch] = dmacon;

	// DMA Controller Already initialized.
	if ((readl(UDMA_CON_BASE(dmacon->con) + UDMA_CR) & 0x1) != 0)
	{
		return 0;
	}

	// Disable DMA Contorller
	writel(readl(UDMA_CON_BASE(dmacon->con)+UDMA_CR) & ~(0x1), UDMA_CON_BASE(dmacon->con) + UDMA_CR);

	// Clear Interrupt
	writel(0xFF, UDMA_CON_BASE(dmacon->con) + UDMA_ITCCR);
	writel(0xFF, UDMA_CON_BASE(dmacon->con) + UDMA_IECR);

	(void)BSP_IntVectSet(INT_UT_DMA0 + dmacon->con, int_prio, INT_TYPE_LEVEL_HIGH,
		udma_isr, &udma_con_table[dmacon->con]);
	BSP_IntSrcEn(INT_UT_DMA0 + dmacon->con);

	// Enable DMA Controller
	writel(0x1, UDMA_CON_BASE(dmacon->con) + UDMA_CR);

	return 0;
}
