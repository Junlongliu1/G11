
/****************************************************************************************
 *   FileName    : tcc_ipc_mbox.c
 *   Description :
 ****************************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips Inc.
 *   All rights reserved

This source code contains confidential information of Telechips.
Any unauthorized use without a written permission of Telechips including not limited
to re-distribution in source or binary form is strictly prohibited.
This source code is provided "AS IS" and nothing contained in this source code
shall constitute any express or implied warranty of any kind, including without limitation,
any warranty of merchantability, fitness for a particular purpose or non-infringement of any patent,
copyright or other third party intellectual property right.
No warranty is made, express or implied, regarding the information's accuracy,
completeness, or performance.
In no event shall Telechips be liable for any claim, damages or other liability arising from,
out of or in connection with this source code or the use in the source code.
This source code is provided subject to the terms of a Mutual Non-Disclosure Agreement
between Telechips and Company.
*
****************************************************************************************/
#include <types.h>
#include <bsp_int.h>
#include <osal.h>
#include "tcc_ipc_typedef.h"
#include "tcc_ipc.h"
#include "tcc_ipc_os.h"
#include "tcc_ipc_mbox.h"
#include "debug.h"
#include "rpc_os.h"
typedef struct _mailbox_info{
	volatile PMAILBOX _pMyMBox;
	//volatile PMAILBOX _pOtherMBox; //checking other mbox is unavailable
	Type_uWord 		uwirq;
	ipc_mbox_receive _receive_cb;
	Type_uByte 		ubintLevel;
}mailbox_info_ts;

static mailbox_info_ts mboxInfo[IPC_CH_MAX]= {
#ifdef IPC_CA53_S
	{NULL,NULL,NULL_PTR,0},
#endif
#ifdef IPC_CA53_NS
	{NULL,NULL,NULL_PTR,0},
#endif
#ifdef IPC_CA7_S
	{NULL,NULL,NULL_PTR,0},
#endif
#ifdef IPC_CA7_NS
	{NULL,NULL,NULL_PTR,0},
#endif
#ifdef IPC_CM4_NS
	{NULL,NULL,NULL_PTR,0},
#endif
};

MboxDataReg_ts mboxData[IPC_CH_MAX];

#ifdef IPC_CA53_NS
static void nvdipc_ca53_ns_mailbox_handler(void *arg);
#endif
#ifdef IPC_CA7_NS
static void nvdipc_ca7_ns_mailbox_handler(void *arg);
#endif
#ifdef IPC_CM4_NS
static void ipc_cm4_ns_mailbox_handler(void *arg);
#endif

static void nvdipc_mailbox_put_data(IPC_CH ch, const Type_uByte *aubbuff_p, const Type_uWord auwsize); //QAC

Type_sWord wswipc_mailbox_init(IPC_CH ch, Type_uWord auwmyMBoxBase, Type_uWord auwotherMBoxBase,Type_uWord auwirqNum, Type_uByte aubintLevel, ipc_mbox_receive handler)
{
	Type_sWord aswret=-1;

	if(handler != NULL)
	{		
		mboxInfo[ch]._pMyMBox = (volatile PMAILBOX) auwmyMBoxBase;
		mboxInfo[ch].uwirq = auwirqNum;
		mboxInfo[ch]._receive_cb = handler;
		mboxInfo[ch].ubintLevel = aubintLevel;

		/* FLUSH: 6bit, OEN: 5bit, IEN: 4bit, LEVEL: 1~0bit */
		BITSET(mboxInfo[ch]._pMyMBox->uMBOX_CTL_016.nREG, FLUSH_BIT|D_FLUSH_BIT);

		/* register mbox rx interrupt handler	*/
#ifdef IPC_CA53_NS
		if(ch == IPC_CH_CA53_NS) {
			(void)wswipc_request_irq(auwirqNum, nvdipc_ca53_ns_mailbox_handler);
			aswret = 0;
		}
#endif
#ifdef IPC_CA7_NS
		if(ch == IPC_CH_CA7_NS) {
			(void)wswipc_request_irq(auwirqNum, nvdipc_ca7_ns_mailbox_handler);
			aswret = 0;
		}
#endif
#ifdef IPC_CM4_NS
		if(ch == IPC_CH_CM4_NS) {
			(void)wswipc_request_irq(auwirqNum, ipc_cm4_ns_mailbox_handler);
			aswret = 0;
		}
#endif
		BITSET(mboxInfo[ch]._pMyMBox->uMBOX_CTL_016.nREG, (uint32)(mboxInfo[ch].ubintLevel|IEN_BIT));
	}

	return aswret;
}

void wvdipc_mailbox_deinit(IPC_CH ch)
{
	mboxInfo[ch]._receive_cb = NULL;
	if(mboxInfo[ch]._pMyMBox != NULL)
	{
		(void)wswipc_free_irq(mboxInfo[ch].uwirq);
		mboxInfo[ch].uwirq =0;
		BITCLR(mboxInfo[ch]._pMyMBox->uMBOX_CTL_016.nREG, IEN_BIT|OEN_BIT|FLUSH_BIT|D_FLUSH_BIT);
	}
	mboxInfo[ch]._pMyMBox = NULL;
}

Type_sWord wswipc_mailbox_send(IPC_CH ch, const MboxMsgReg * Msg_p, Type_uWord auwTimeOut) //SEND CMD
{
	Type_sWord auwret = IPC_ERR_NOTREADY;
	wvdipc_mbox_lock(ch);
	if(mboxInfo[ch]._pMyMBox != NULL)
	{
		Type_uWord  auwtimeOut = auwTimeOut;
		auwret =IPC_SUCCESS;

		/* my mbox ready*/
		if(mboxInfo[ch]._pMyMBox->uMBOX_CTL_016.bREG.LEVEL > 0UL)
		{
			while((mboxInfo[ch]._pMyMBox->uMBOX_STR.bREG.MEMP == 0UL)
				&&(auwtimeOut>0))
			{
				wvdipc_mdelay(1);
				auwtimeOut--;
			}
			
			/* If delay time is too long, ipc may already be release.*/
			if(mboxInfo[ch]._pMyMBox->uMBOX_STR.bREG.MEMP == 0UL) {
				BITSET( mboxInfo[ch]._pMyMBox->uMBOX_CTL_016.nREG, FLUSH_BIT); //Flush CMD FIFO
			}
			
			if(mboxInfo[ch]._pMyMBox->uMBOX_STR.bREG.MEMP != 0)
			{
				BITCLR( mboxInfo[ch]._pMyMBox->uMBOX_CTL_016.nREG, OEN_BIT); //OEN low

				if(mboxInfo[ch].ubintLevel >= INT_LEVEL_2)
				{
					mboxInfo[ch]._pMyMBox->uMBOX_TX0.nREG = Msg_p->reg[0];
					mboxInfo[ch]._pMyMBox->uMBOX_TX1.nREG = Msg_p->reg[1];
				}
				if(mboxInfo[ch].ubintLevel >= INT_LEVEL_4)
				{
					mboxInfo[ch]._pMyMBox->uMBOX_TX2.nREG = Msg_p->reg[2];
					mboxInfo[ch]._pMyMBox->uMBOX_TX3.nREG = Msg_p->reg[3];
				}
				if(mboxInfo[ch].ubintLevel == INT_LEVEL_8)
				{
					mboxInfo[ch]._pMyMBox->uMBOX_TX4.nREG = Msg_p->reg[4];
					mboxInfo[ch]._pMyMBox->uMBOX_TX5.nREG = Msg_p->reg[5];
					mboxInfo[ch]._pMyMBox->uMBOX_TX6.nREG = Msg_p->reg[6];
					mboxInfo[ch]._pMyMBox->uMBOX_TX7.nREG = Msg_p->reg[7];
				}

				BITSET( mboxInfo[ch]._pMyMBox->uMBOX_CTL_016.nREG, OEN_BIT); //OEN high
			}
			else
			{
				auwret = IPC_ERR_RECEIVER_DOWN;
			}
		}
		else
		{
			auwret = IPC_ERR_RECEIVER_NOT_SET;
		}
	}
	wvdipc_mbox_unlock(ch);
	
	return auwret;
}

Type_sWord wswipc_mailbox_send_open(IPC_CH ch, const MboxMsgReg * Msg_p, Type_uWord auwTimeOut) //for only open
{
	Type_sWord aswret = IPC_ERR_NOTREADY;

	wvdipc_mbox_lock(ch);
	if(mboxInfo[ch]._pMyMBox != NULL)
	{
		Type_uWord  auwtimeOut = auwTimeOut;
		aswret =IPC_SUCCESS;

		/* my mbox ready*/
		if(mboxInfo[ch]._pMyMBox->uMBOX_CTL_016.bREG.LEVEL > 0UL) 
		{
			/* AP might be reading data */
			while(((mboxInfo[ch]._pMyMBox->uMBOX_STR.bREG.MEMP == 0) && (mboxInfo[ch]._pMyMBox->uMBOX_STR.bREG.MFUL == 0))
				&&(auwtimeOut>0))
			{
				wvdipc_mdelay(1);
				auwtimeOut--;
			}

			/* If delay time is too long, ipc may already be release.*/
			if(mboxInfo[ch]._pMyMBox->uMBOX_STR.bREG.MEMP == 0UL)
			{
				BITSET( mboxInfo[ch]._pMyMBox->uMBOX_CTL_016.nREG, FLUSH_BIT); //Flush CMD FIFO
			}
			
			if(mboxInfo[ch]._pMyMBox->uMBOX_STR.bREG.MEMP != 0UL)
			{
				BITCLR( mboxInfo[ch]._pMyMBox->uMBOX_CTL_016.nREG, OEN_BIT); //OEN low
				
				if(mboxInfo[ch].ubintLevel >= INT_LEVEL_2)
				{
					mboxInfo[ch]._pMyMBox->uMBOX_TX0.nREG = Msg_p->reg[0];
					mboxInfo[ch]._pMyMBox->uMBOX_TX1.nREG = Msg_p->reg[1];
				}
				if(mboxInfo[ch].ubintLevel >= INT_LEVEL_4)
				{
					mboxInfo[ch]._pMyMBox->uMBOX_TX2.nREG = Msg_p->reg[2];
					mboxInfo[ch]._pMyMBox->uMBOX_TX3.nREG = Msg_p->reg[3];
				}
				if(mboxInfo[ch].ubintLevel == INT_LEVEL_8)
				{
					mboxInfo[ch]._pMyMBox->uMBOX_TX4.nREG = Msg_p->reg[4];
					mboxInfo[ch]._pMyMBox->uMBOX_TX5.nREG = Msg_p->reg[5];
					mboxInfo[ch]._pMyMBox->uMBOX_TX6.nREG = Msg_p->reg[6];
					mboxInfo[ch]._pMyMBox->uMBOX_TX7.nREG = Msg_p->reg[7];
				}

				BITSET( mboxInfo[ch]._pMyMBox->uMBOX_CTL_016.nREG, OEN_BIT); //OEN high
			}
			else
			{
				aswret = IPC_ERR_RECEIVER_DOWN;
			}
		}
		else
		{
			aswret = IPC_ERR_COMMON;
		}
	}
	wvdipc_mbox_unlock(ch);
	
	return aswret;
}

Type_sWord wswipc_mailbox_send_close(IPC_CH ch, const MboxMsgReg * Msg_p, Type_uWord auwTimeOut)
{
	Type_sWord aswret = IPC_ERR_NOTREADY;
	wvdipc_mbox_lock(ch);
	if(mboxInfo[ch]._pMyMBox != NULL)
	{
		Type_uWord  auwtimeOut = auwTimeOut;
		aswret =IPC_SUCCESS;

		/* my mbox ready*/
		if(mboxInfo[ch]._pMyMBox->uMBOX_CTL_016.bREG.LEVEL > 0UL)
		{
			while((mboxInfo[ch]._pMyMBox->uMBOX_STR.bREG.MEMP == 0UL)
				&&(auwtimeOut>0))
			{
				wvdipc_mdelay(1);
				auwtimeOut--;
			}

			/* If delay time is too long, ipc may already be release.*/
			if(mboxInfo[ch]._pMyMBox->uMBOX_STR.bREG.MEMP == 0UL)
			{
				BITSET( mboxInfo[ch]._pMyMBox->uMBOX_CTL_016.nREG, FLUSH_BIT); //Flush CMD FIFO
			}
			
			if(mboxInfo[ch]._pMyMBox->uMBOX_STR.bREG.MEMP != 0UL)
			{
				BITCLR( mboxInfo[ch]._pMyMBox->uMBOX_CTL_016.nREG, OEN_BIT); //OEN low

				if(mboxInfo[ch].ubintLevel >= INT_LEVEL_2)
				{
					mboxInfo[ch]._pMyMBox->uMBOX_TX0.nREG = Msg_p->reg[0];
					mboxInfo[ch]._pMyMBox->uMBOX_TX1.nREG = Msg_p->reg[1];
				}
				if(mboxInfo[ch].ubintLevel >= INT_LEVEL_4)
				{
					mboxInfo[ch]._pMyMBox->uMBOX_TX2.nREG = Msg_p->reg[2];
					mboxInfo[ch]._pMyMBox->uMBOX_TX3.nREG = Msg_p->reg[3];
				}
				if(mboxInfo[ch].ubintLevel == INT_LEVEL_8)
				{
					mboxInfo[ch]._pMyMBox->uMBOX_TX4.nREG = Msg_p->reg[4];
					mboxInfo[ch]._pMyMBox->uMBOX_TX5.nREG = Msg_p->reg[5];
					mboxInfo[ch]._pMyMBox->uMBOX_TX6.nREG = Msg_p->reg[6];
					mboxInfo[ch]._pMyMBox->uMBOX_TX7.nREG = Msg_p->reg[7];
				}

				BITSET( mboxInfo[ch]._pMyMBox->uMBOX_CTL_016.nREG, OEN_BIT); //OEN high
			}
			else
			{
				aswret = IPC_ERR_RECEIVER_DOWN;
			}
		}
		else
		{
			aswret = IPC_ERR_COMMON;
		}
	}
	wvdipc_mbox_unlock(ch);
	
	return aswret;
}

Type_sWord wswipc_mailbox_send_data(IPC_CH ch, const MboxMsgReg * Msg_p, const Type_uByte *aubbuff_p, const Type_uWord auwsize, Type_uWord auwTimeOut) //QAC
{
	Type_sWord aswret = IPC_ERR_NOTREADY;
	wvdipc_mbox_lock(ch);
	if(mboxInfo[ch]._pMyMBox != NULL)
	{
		Type_uWord  auwtimeOut = auwTimeOut;
		aswret =IPC_SUCCESS;

		/* my mbox ready*/
		if(mboxInfo[ch]._pMyMBox->uMBOX_CTL_016.bREG.LEVEL > 0UL)
		{
			while(((mboxInfo[ch]._pMyMBox->uMBOX_STR.bREG.MEMP == 0) || (mboxInfo[ch]._pMyMBox->uMBOX_TXFIFO_STR.bREG.MEMP == 0))
				&&(auwtimeOut>0))
			{
				wvdipc_mdelay(1);
				auwtimeOut--;
			}
			
			/* If delay time is too long, ipc may already be release.*/
			if(mboxInfo[ch]._pMyMBox->uMBOX_STR.bREG.MEMP == 0UL) {
				BITSET( mboxInfo[ch]._pMyMBox->uMBOX_CTL_016.nREG, FLUSH_BIT); //Flush CMD FIFO
			}
			if(mboxInfo[ch]._pMyMBox->uMBOX_TXFIFO_STR.bREG.MEMP == 0) {
				BITSET( mboxInfo[ch]._pMyMBox->uMBOX_CTL_016.nREG, D_FLUSH_BIT); //Flush DATA FIFO
			}
			
			if((mboxInfo[ch]._pMyMBox->uMBOX_STR.bREG.MEMP != 0UL)&&(mboxInfo[ch]._pMyMBox->uMBOX_TXFIFO_STR.bREG.MEMP != 0UL))
			{
				BITCLR( mboxInfo[ch]._pMyMBox->uMBOX_CTL_016.nREG, OEN_BIT); //OEN low

				if(auwsize != 0)
				{
					nvdipc_mailbox_put_data(ch, aubbuff_p, auwsize); //put data to DATA FIFO
				}

				/* put data to CMD FIFO */
				if(mboxInfo[ch].ubintLevel >= INT_LEVEL_2)
				{
					mboxInfo[ch]._pMyMBox->uMBOX_TX0.nREG = Msg_p->reg[0];
					mboxInfo[ch]._pMyMBox->uMBOX_TX1.nREG = Msg_p->reg[1];
				}
				if(mboxInfo[ch].ubintLevel >= INT_LEVEL_4)
				{
					mboxInfo[ch]._pMyMBox->uMBOX_TX2.nREG = Msg_p->reg[2];
					mboxInfo[ch]._pMyMBox->uMBOX_TX3.nREG = Msg_p->reg[3];
				}
				if(mboxInfo[ch].ubintLevel == INT_LEVEL_8)
				{
					mboxInfo[ch]._pMyMBox->uMBOX_TX4.nREG = Msg_p->reg[4];
					mboxInfo[ch]._pMyMBox->uMBOX_TX5.nREG = Msg_p->reg[5];
					mboxInfo[ch]._pMyMBox->uMBOX_TX6.nREG = Msg_p->reg[6];
					mboxInfo[ch]._pMyMBox->uMBOX_TX7.nREG = Msg_p->reg[7];
				}

				BITSET( mboxInfo[ch]._pMyMBox->uMBOX_CTL_016.nREG, OEN_BIT); //OEN high
			}
			else
			{
				aswret = IPC_ERR_RECEIVER_DOWN;
			}
		}
		else
		{
			aswret = IPC_ERR_RECEIVER_NOT_SET;
		}
	}
	wvdipc_mbox_unlock(ch);
	
	return aswret;
}

static void nvdipc_mailbox_put_data(IPC_CH ch, const Type_uByte *aubbuff_p, const Type_uWord auwsize) //QAC //PUT to DATA FIFO
{
	const Type_uByte *aubBuff_p = (const Type_uByte *)aubbuff_p;
	
	Type_uWord auwtempBuff = 0;
	Type_uByte auboffset = 0;
	Type_uWord auwindex;
	Type_uWord auwwsize = sizeof(Type_uWord);
	Type_uWord auwmask = (auwwsize - 1);
	
	for(auwindex=(auwsize/auwwsize); auwindex>0; auwindex--)
	{
		mboxInfo[ch]._pMyMBox->uMBOX_TXFIFO.nREG = *(const Type_uWord *)aubBuff_p;
		aubBuff_p += auwwsize;
	}
	
	if((auwsize&auwmask) != 0)
	{
		for(auwindex=(auwsize&auwmask); auwindex>0; auwindex--)
		{
			auwtempBuff |= (((Type_uWord)(*aubBuff_p++)) & 0x000000FF) << auboffset;
			auboffset += 8; //one byte
		}
		mboxInfo[ch]._pMyMBox->uMBOX_TXFIFO.nREG = auwtempBuff;
	}
}

void wvdipc_mailbox_get_data(IPC_CH ch, Type_uByte * aubbuff_p, Type_uWord auwsize) //GET from DATA FIFO
{
	Type_uWord *auwBuff_p = (Type_uWord *)aubbuff_p;
	
	Type_uWord auwtemp = 0;
	Type_uWord auwoffset = 0;
	Type_uWord auwindex;
	Type_uWord auwwsize = sizeof(Type_uWord);
	Type_uWord auwmask = (auwwsize - 1);

	for(auwindex=0; auwindex<(auwsize/auwwsize); auwindex++)
	{
		auwBuff_p[auwindex] = mboxInfo[ch]._pMyMBox->uMBOX_RXFIFO.nREG;
	}
	
	auwoffset=auwindex*auwwsize;
	
	if((auwsize&auwmask) != 0)
	{
		auwtemp = mboxInfo[ch]._pMyMBox->uMBOX_RXFIFO.nREG;

		for(auwindex=0; auwindex<(auwsize&auwmask); auwindex++)
		{
			aubbuff_p[auwindex+auwoffset] = (auwtemp&0xff);
			auwtemp = (auwtemp>>8);
		}
	}
}

Type_uWord wuwipc_mailbox_get_data_size(IPC_CH ch)
{
	Type_uWord auwsize;

	auwsize = mboxInfo[ch]._pMyMBox->uMBOX_RXFIFO_STR.bREG.SCOUNT;

	return auwsize;
}

void wvdMboxDataInit(IPC_CH ch)
{
	mboxData[ch].uwcur_dataIdx = VALUE_0;
	mboxData[ch].uwdata_size = VALUE_0;
	(void)OSAL_MemSet(&mboxData[ch].data[VALUE_0][VALUE_0], INIT_VALUE, MAX_MBOX_DATA_FIFO_CNT);
	(void)OSAL_MemSet(&mboxData[ch].data[VALUE_1][VALUE_0], INIT_VALUE, MAX_MBOX_DATA_FIFO_CNT);
}

#ifdef IPC_CA53_NS
static void nvdipc_ca53_ns_mailbox_handler(void *arg)
{
	MboxMsgReg mboxMsg;
	Type_uWord	auwdataCount;
	Type_uWord	auwidx = 0;
	Type_uWord auwindex;

	/* get data fifo count */
	auwdataCount = mboxInfo[IPC_CH_CA53_NS]._pMyMBox->uMBOX_RXFIFO_STR.bREG.SCOUNT;
	mboxData[IPC_CH_CA53_NS].uwdata_size = auwdataCount;

	/* get data fifo */
	if((auwdataCount > 0) && (auwdataCount <= MAX_MBOX_DATA_FIFO_CNT))
	{
		if(mboxData[IPC_CH_CA53_NS].uwcur_dataIdx < (MAX_MBOX_DATA_IDX-1)) {
			mboxData[IPC_CH_CA53_NS].uwcur_dataIdx++;
		} else {
			mboxData[IPC_CH_CA53_NS].uwcur_dataIdx = 0;
		}
		auwidx = mboxData[IPC_CH_CA53_NS].uwcur_dataIdx;
		
		for(auwindex=0; auwindex<auwdataCount; auwindex++)
		{
			mboxData[IPC_CH_CA53_NS].data[auwidx][auwindex] = mboxInfo[IPC_CH_CA53_NS]._pMyMBox->uMBOX_RXFIFO.nREG;

		}

	}

	/* get cmd fifo */
	mboxMsg.reg[0] = mboxInfo[IPC_CH_CA53_NS]._pMyMBox->uMBOX_RX0.nREG;
	mboxMsg.reg[1] = mboxInfo[IPC_CH_CA53_NS]._pMyMBox->uMBOX_RX1.nREG;
	mboxMsg.reg[2] = mboxInfo[IPC_CH_CA53_NS]._pMyMBox->uMBOX_RX2.nREG;
	mboxMsg.reg[3] = mboxInfo[IPC_CH_CA53_NS]._pMyMBox->uMBOX_RX3.nREG;
	mboxMsg.reg[4] = mboxInfo[IPC_CH_CA53_NS]._pMyMBox->uMBOX_RX4.nREG;
	mboxMsg.reg[5] = mboxInfo[IPC_CH_CA53_NS]._pMyMBox->uMBOX_RX5.nREG;
	mboxMsg.reg[6] = mboxInfo[IPC_CH_CA53_NS]._pMyMBox->uMBOX_RX6.nREG;
	mboxMsg.reg[7] = mboxInfo[IPC_CH_CA53_NS]._pMyMBox->uMBOX_RX7.nREG;
	mboxMsg.sel_dataIdx = auwidx;
	
	if((mboxInfo[IPC_CH_CA53_NS]._receive_cb!= NULL) && (wubRPCStandbyFlag == RPC_STANDBYOUT))
	{
		mboxInfo[IPC_CH_CA53_NS]._receive_cb(IPC_CH_CA53_NS, &mboxMsg);
		
	}
}
#endif
#ifdef IPC_CA7_NS
static void nvdipc_ca7_ns_mailbox_handler(void *arg)
{
	MboxMsgReg mboxMsg;
	Type_uWord	auwdataCount;
	Type_uWord	auwidx = 0;
	Type_uWord auwindex;

	/* get data fifo count */
	auwdataCount = mboxInfo[IPC_CH_CA7_NS]._pMyMBox->uMBOX_RXFIFO_STR.bREG.SCOUNT;

	/* get data fifo */
	if((auwdataCount > 0) && (auwdataCount <= MAX_MBOX_DATA_FIFO_CNT))
	{
		if(mboxData[IPC_CH_CA7_NS].uwcur_dataIdx < (MAX_MBOX_DATA_IDX-1)) {
			mboxData[IPC_CH_CA7_NS].uwcur_dataIdx++;
		} else {
			mboxData[IPC_CH_CA7_NS].uwcur_dataIdx = 0;
		}
		auwidx = mboxData[IPC_CH_CA7_NS].uwcur_dataIdx;
		
		for(auwindex=0; auwindex<auwdataCount; auwindex++)
		{
			mboxData[IPC_CH_CA7_NS].data[auwidx][auwindex] = mboxInfo[IPC_CH_CA7_NS]._pMyMBox->uMBOX_RXFIFO.nREG;
		}
	}

	/* get cmd fifo */
	mboxMsg.reg[0] = mboxInfo[IPC_CH_CA7_NS]._pMyMBox->uMBOX_RX0.nREG;
	mboxMsg.reg[1] = mboxInfo[IPC_CH_CA7_NS]._pMyMBox->uMBOX_RX1.nREG;
	mboxMsg.reg[2] = mboxInfo[IPC_CH_CA7_NS]._pMyMBox->uMBOX_RX2.nREG;
	mboxMsg.reg[3] = mboxInfo[IPC_CH_CA7_NS]._pMyMBox->uMBOX_RX3.nREG;
	mboxMsg.reg[4] = mboxInfo[IPC_CH_CA7_NS]._pMyMBox->uMBOX_RX4.nREG;
	mboxMsg.reg[5] = mboxInfo[IPC_CH_CA7_NS]._pMyMBox->uMBOX_RX5.nREG;
	mboxMsg.reg[6] = mboxInfo[IPC_CH_CA7_NS]._pMyMBox->uMBOX_RX6.nREG;
	mboxMsg.reg[7] = mboxInfo[IPC_CH_CA7_NS]._pMyMBox->uMBOX_RX7.nREG;
	mboxMsg.sel_dataIdx = auwidx;

	if((mboxInfo[IPC_CH_CA7_NS]._receive_cb!= NULL) && (wubRPCStandbyFlag == RPC_STANDBYOUT))
	{
		mboxInfo[IPC_CH_CA7_NS]._receive_cb(IPC_CH_CA7_NS, &mboxMsg);
	}
}
#endif
#ifdef IPC_CM4_NS
static void ipc_cm4_ns_mailbox_handler(void *arg)
{
	MboxMsgReg mboxMsg;

	mboxMsg.reg[0] = mboxInfo[IPC_CH_CM4_NS]._pMyMBox->uMBOX_RX0.nREG;		/* cmd */
	mboxMsg.reg[1] = mboxInfo[IPC_CH_CM4_NS]._pMyMBox->uMBOX_RX1.nREG;  	/* msg_len */
	mboxMsg.reg[2] = mboxInfo[IPC_CH_CM4_NS]._pMyMBox->uMBOX_RX2.nREG;
	mboxMsg.reg[3] = mboxInfo[IPC_CH_CM4_NS]._pMyMBox->uMBOX_RX3.nREG;
	mboxMsg.reg[4] = mboxInfo[IPC_CH_CM4_NS]._pMyMBox->uMBOX_RX4.nREG;
	mboxMsg.reg[5] = mboxInfo[IPC_CH_CM4_NS]._pMyMBox->uMBOX_RX5.nREG;
	mboxMsg.reg[6] = mboxInfo[IPC_CH_CM4_NS]._pMyMBox->uMBOX_RX6.nREG;
	mboxMsg.reg[7] = mboxInfo[IPC_CH_CM4_NS]._pMyMBox->uMBOX_RX7.nREG;

	if(mboxInfo[IPC_CH_CM4_NS]._receive_cb!= NULL)
	{
		mboxInfo[IPC_CH_CM4_NS]._receive_cb(IPC_CH_CM4_NS, &mboxMsg);
	}
}
#endif
