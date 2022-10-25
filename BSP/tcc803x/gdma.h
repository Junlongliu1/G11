/****************************************************************************
 *   FileName    : gdma.h
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

#ifndef _TCC_GDMA_H_
#define _TCC_GDMA_H_

#define DMA_CH_MAX		(7)

#define DMA_INC			(1)
#define DMA_NO_INC		(0)

#define DMA_TRANSFER_SIZE_BYTE	(0)
#define DMA_TRANSFER_SIZE_HALF	(1)
#define DMA_TRANSFER_SIZE_WORD	(2)

#define DMA_MAX_LLI_NUM		(64)

#define DMA_BURST_SIZE_1	(0)
#define DMA_BURST_SIZE_4	(1)
#define DMA_BURST_SIZE_8	(2)
#define DMA_BURST_SIZE_16	(3)
#define DMA_BURST_SIZE_32	(4)
#define DMA_BURST_SIZE_64	(5)
#define DMA_BURST_SIZE_128	(6)
#define DMA_BURST_SIZE_256	(7)

#define DMA_FLOW_TYPE_M2M		(0)
#define DMA_FLOW_TYPE_M2P		(1)
#define DMA_FLOW_TYPE_P2M		(2)
#define DMA_FLOW_TYPE_P2P		(3)
#define DMA_FLOW_TYPE_P2P_BY_DP	(4)
#define DMA_FLOW_TYPE_M2P_BY_P	(5)
#define DMA_FLOW_TYPE_P2M_BY_P	(6)
#define DMA_FLOW_TYPE_P2P_BY_SP	(7)

// DMA Request Peripheral
enum {
	DMA_CAN0 = 0,
	DMA_CAN1,
	DMA_CAN2,
	DMA_GPSB0_RX,
	DMA_GPSB1_RX,
	DMA_GPSB2_RX,
	DMA_GPSB0_TX,
	DMA_GPSB1_TX,
	DMA_GPSB2_TX,
	DMA_ICTC0,
	DMA_ICTC1,
	DMA_ICTC2,
	DMA_ADC
};

typedef volatile struct
{
	uint32 src_addr;
	uint32 dest_addr;
	uint32 lli_addr;
	uint32 control;
}GDMA_LLI_Type;

struct gdma_data
{
	uint32 con;
	uint32 ch;
	int8 *p_addr;
	int8 *nc_addr;
};

int32 init_gdma(struct gdma_data *dmacon, uint32 int_prio);
//void gdma_ch_enable(const struct gdma_data *dmacon);//Msg(4:1505) The function 'gdma_ch_enable' is only referenced in the translation unit where it is defined. MISRA-C:2004 Rule 8.10
//void gdma_ch_disable(const struct gdma_data *dmacon);//Msg(4:1505) The function 'gdma_ch_disable' is only referenced in the translation unit where it is defined. MISRA-C:2004 Rule 8.10
//void gdma_set_flowcontrol(const struct gdma_data *dmacon, uint32 flow);//Msg(4:1505) The function 'gdma_set_flowcontrol' is only referenced in the translation unit where it is defined. MISRA-C:2004 Rule 8.10
//void gdma_set_inc(const struct gdma_data *dmacon, uint8 dest_inc, uint32 src_inc);//Msg(4:1505) The function 'gdma_set_inc' is only referenced in the translation unit where it is defined. MISRA-C:2004 Rule 8.10
//void gdma_set_transferwidth(const struct gdma_data *dmacon, uint8 dest_width, uint32 src_width);//Msg(4:1505) The function 'gdma_set_transferwidth' is only referenced in the translation unit where it is defined. MISRA-C:2004 Rule 8.10
//void gdma_set_burstsize(const struct gdma_data *dmacon, uint8 dest_burst, uint32 src_burst);//Msg(4:1505) The function 'gdma_set_burstsize' is only referenced in the translation unit where it is defined. MISRA-C:2004 Rule 8.10
//void gdma_set_transfersize(const struct gdma_data *dmacon, uint32 transfer_size);//Msg(4:1505) The function 'gdma_set_transfersize' is only referenced in the translation unit where it is defined. MISRA-C:2004 Rule 8.10
//void gdma_tcint_enable(const struct gdma_data *dmacon);//Msg(4:1505) The function 'gdma_tcint_enable' is only referenced in the translation unit where it is defined. MISRA-C:2004 Rule 8.10
//void gdma_tcint_disable(const struct gdma_data *dmacon);//Msg(4:1505) The function 'gdma_set_flowcontrol' is only referenced in the translation unit where it is defined. MISRA-C:2004 Rule 8.10
//void gdma_set_peri(const struct gdma_data *dmacon, uint8 dest_peri, uint8 src_peri);//Msg(4:1505) The function 'gdma_set_flowcontrol' is only referenced in the translation unit where it is defined. MISRA-C:2004 Rule 8.10
//void gdma_set_src_addr(const struct gdma_data *dmacon, uint32 addr);//Msg(4:1505) The function 'gdma_set_src_addr' is only referenced in the translation unit where it is defined. MISRA-C:2004 Rule 8.10
//void gdma_set_dest_addr(const struct gdma_data *dmacon, uint32 addr);//Msg(4:1505) The function 'gdma_set_dest_addr' is only referenced in the translation unit where it is defined. MISRA-C:2004 Rule 8.10
//void gdma_set_lli_addr(const struct gdma_data *dmacon, uint32 lli_addr);//Msg(4:1505) The function 'gdma_set_lli_addr' is only referenced in the translation unit where it is defined. MISRA-C:2004 Rule 8.10
int32 gdma_transfer_m2m(const struct gdma_data *dmacon, uint32 src_addr,
		       uint32 dest_addr, uint32 len);

#endif	/* _TCC_DMA_H_ */

