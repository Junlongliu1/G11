/****************************************************************************
 *   FileName    : udma.h
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

#ifndef _TCC_UDMA_H_
#define _TCC_UDMA_H_

// UDMA address starts from 0xC003E000 and occupies 0x3FF for each of tx and rx.
// UDMA has 4 channels whose range of address is from 0xC003E000 to 0xC003FFFF(8KB).
// Valid address range of SRAM(micom) is only defined from 0xC0000000 to 0xC003FFFF(256KB).
// If it is needed to increase DMA buffer area in SRAM, refer to 4.6 Use fixed DMA buffer
// in TCC803x MICOM RTOS SDK-User Guide
#define UDMA_BUFF_SIZE	0x3ff // 1023.
#define UDMA_BUFF_MARGIN 0x0

#define UDMA_CH0		(0)
#define UDMA_CH1		(1)

#define UDMA_INC		(1)
#define UDMA_NO_INC		(0)

#define UDMA_TRANSFER_SIZE_BYTE		(0)
#define UDMA_TRANSFER_SIZE_HALF		(1)
#define UDMA_TRANSFER_SIZE_WORD		(2)

#define UDMA_MAX_LLI_NUM			(64)

#define UDMA_BURST_SIZE_1			(0)
#define UDMA_BURST_SIZE_4			(1)
#define UDMA_BURST_SIZE_8			(2)
#define UDMA_BURST_SIZE_16			(3)
#define UDMA_BURST_SIZE_32			(4)
#define UDMA_BURST_SIZE_64			(5)
#define UDMA_BURST_SIZE_128			(6)
#define UDMA_BURST_SIZE_256			(7)

#define UDMA_FLOW_TYPE_M2M			(0)
#define UDMA_FLOW_TYPE_M2P			(1)
#define UDMA_FLOW_TYPE_P2M			(2)
#define UDMA_FLOW_TYPE_P2P			(3)
#define UDMA_FLOW_TYPE_P2P_BY_DP	(4)
#define UDMA_FLOW_TYPE_M2P_BY_P		(5)
#define UDMA_FLOW_TYPE_P2M_BY_P		(6)
#define UDMA_FLOW_TYPE_P2P_BY_SP	(7)

// DMA Request Peripheral
enum {
	UDMA_PERI_RX = 0,
	UDMA_PERI_TX = 1
};

typedef volatile struct
{
	uint32 src_addr;
	uint32 dest_addr;
	uint32 lli_addr;
	uint32 control;
} DMA_LLI_Type;

struct udma_data
{
	uint32 con;
	uint32 ch;
	int8 *src_addr;
	int8 *dest_addr;
	uint32 buff_size;
	uint32 transfer_size;
	uint32 is_run;
};

int32 udma_init(struct udma_data *dmacon, uint32 int_prio);
void udma_ch_enable(const struct udma_data *dmacon);
void udma_ch_disable(const struct udma_data *dmacon);
void udma_set_flowcontrol(const struct udma_data *dmacon, uint32 flow);
void udma_set_inc(const struct udma_data *dmacon, uint8 dest_inc, uint32 src_inc);
void udma_set_transferwidth(const struct udma_data *dmacon, uint8 dest_width, uint32 src_width);
void udma_set_burstsize(const struct udma_data *dmacon, uint8 dest_burst, uint32 src_burst);
void udma_set_transfersize(const struct udma_data *dmacon, uint32 transfer_size);
void udma_tcint_enable(const struct udma_data *dmacon);
void udma_tcint_disable(const struct udma_data *dmacon);
void udma_set_peri(const struct udma_data *dmacon, uint8 dest_peri, uint8 src_peri);
void udma_set_src_addr(const struct udma_data *dmacon, uint32 addr);
void udma_set_dest_addr(const struct udma_data *dmacon, uint32 addr);
void udma_set_lli_addr(const struct udma_data *dmacon, uint32 lli_addr);
uint32 udma_get_dest_addr(const struct udma_data *dmacon);
void udma_transfer_m2m(const struct udma_data *dmacon, uint32 src_addr,
		uint32 dest_addr, uint32 len);
uint32 udma_get_transfersize(const struct udma_data *dmacon);
#endif	/* _TCC_UDMA_H_ */
