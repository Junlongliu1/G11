/****************************************************************************************
 *   FileName    : tcc_ipc_ctl.c
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
#include <types.h>
#include <osal.h>
#include <bsp_int.h>
//#include "tcc_ipc_typedef.h"
#include "tcc_ipc.h"
//#include "tcc_ipc_buffer.h"
#include "tcc_ipc_os.h"
//#include "tcc_ipc_mbox.h"
#include "tcc_ipc_cmd.h"
#include "tcc_ipc_ctl.h"
#include "tcc_ipc_protocol.h"
#include "debug.h"
//#include <tcc_robust.h>
#include "fwug.h"
//#include <lvds_initializer.h>
#include "rpc_os.h"
#include "rpc_if.h"

//#define IPC_DBG
#ifdef IPC_DBG
#define IPC_DEBUG(fmt, args...)   printf(fmt, ## args)
#else
#define IPC_DEBUG(fmt, args...)   do {} while(0)
#endif

#define DECODE_ID(val) ((val & 0xffff0000) >> 16)
#define DECODE_EVENT(val) ((val & 0x0000ffff) >> 0)

static Type_uByte gIPCRingBuffer[IPC_CH_MAX][MAX_IPC_RING_BUFF_SIZE];

IpcHandler_ts 	gIPCHandler[IPC_CH_MAX];

//extern MboxDataReg mboxData[IPC_CH_MAX];//Msg(4:3447) 'mboxData' has external linkage but this declaration is not in a header file. MISRA-C:2004 Rule 8.8

static struct ipc_ctrtask_info_ts ipcCtrTaskInfo[IPC_CH_MAX];

static Type_sWord nswipc_write_data(IPC_CH ch, const Type_uWord *auwcmd_p, const Type_uByte *aubbuff_p, const Type_uWord auwsize); //QAC
static Type_sWord nswipc_read_data(IPC_CH ch, Type_uByte *aubbuff_p, Type_uWord auwsize);
static void nvdipc_init_stack(IPC_CH ch);
//static void nvdipc_control_task_del(IPC_CH ch);
static void nvdipc_set_status(IPC_CH ch, IpcStatus status);
static void nvdipc_set_buffer(IPC_CH ch);
static void nvdipc_receive_ctlcmd(IPC_CH ch, const MboxMsgReg  * pMsg);
static void nvdipc_receive_writecmd(IPC_CH ch, const MboxMsgReg  * pMsg);
#ifdef IPC_CA53_NS
static void nvdipc_ca53_ns_receive_message(IPC_CH ch, const MboxMsgReg *mboxMsg);
#endif
#ifdef IPC_CA7_NS
static void nvdipc_ca7_ns_receive_message(IPC_CH ch, const MboxMsgReg *mboxMsg);
#endif
#ifdef IPC_CM4_NS
static void ipc_cm4_ns_receive_message(IPC_CH ch, const MboxMsgReg *mboxMsg);
static void ipc_cm4_ns_control_task (void *p_arg);
#endif

static struct ipc_mbox_ts ipc_ctr_mbox[IPC_CH_MAX] = {
#ifdef IPC_CA53_S
	{ IPC_CH_CA53_S, HwMBox0_CA53_S_BASE, HwMBox0_CR5_S_BASE, INT_MBOX0_SLV_S, /* make callback function */},
#endif
#ifdef IPC_CA53_NS
	{ IPC_CH_CA53_NS, HwMBox0_CA53_NS_BASE, HwMBox0_CR5_NS_BASE, INT_MBOX0_SLV_NS, nvdipc_ca53_ns_receive_message},
#endif
#ifdef IPC_CA7_S
	{ IPC_CH_CA7_S, HwMBox1_CA7_S_BASE, HwMBox1_CR5_S_BASE, INT_MBOX1_SLV_S, /* make callback function */},
#endif
#ifdef IPC_CA7_NS
	{ IPC_CH_CA7_NS, HwMBox1_CA7_NS_BASE, HwMBox1_CR5_NS_BASE, INT_MBOX1_SLV_NS, nvdipc_ca7_ns_receive_message},
#endif
};

Type_sWord wswipc_initialize(IPC_CH ch, Type_uByte aubintLevel)

{
	Type_sWord ret = IPC_SUCCESS;

	gIPCHandler[ch].ipcStatus = IPC_INIT;
	nvdipc_set_buffer(ch);

	IPC_DEBUG("%s, ipc %d initialized\n", __func__, ch);
	(void)wswipc_mailbox_init(ch, gIPCHandler[ch].mboxReg.uwmbox1_base,gIPCHandler[ch].mboxReg.uwmbox0_base,gIPCHandler[ch].mboxReg.uwirq, aubintLevel, ipc_ctr_mbox[ch].receive_cb);
	(void)wswipc_send_open(ch);
	gIPCHandler[ch].ipcStatus = IPC_OPENED;
#ifdef IPC_CM4_NS
	if(ch == IPC_CH_CM4_NS)
	{
		nvdipc_set_status(ch, IPC_READY);
	}
#endif
	
	return ret;
}

void wvdipc_release(IPC_CH ch)
{
	(void)wswipc_send_close(ch);
	wvdipc_mailbox_deinit(ch);
	wvdipc_os_resouce_release(ch);
}

static Type_sWord nswipc_write_data(IPC_CH ch, const Type_uWord *auwcmd_p, const Type_uByte *aubbuff_p, const Type_uWord auwsize) //QAC
{
	Type_sWord aswret = IPC_ERR_COMMON;

	if((auwcmd_p != NULL) && (auwsize <= MAX_IPC_FIFO_SIZE))
	{
		if(auwcmd_p[0] == IPC_APP_ID_A53MR){
			aswret = wswipc_send_write(ch, auwcmd_p, aubbuff_p, auwsize, 0);
		}else{
			aswret = wswipc_send_write(ch, auwcmd_p, aubbuff_p, auwsize, gIPCHandler[ch].ipcOption.isAck);
		}
		if(aswret ==0 )
		{
			aswret = auwsize;
		}
		else
		{
			aswret = IPC_ERR_TIMEOUT;
		}
	}
	return aswret;
}

Type_sWord wswipca53_write(const Type_uWord *auwcmd_p, const Type_uByte *aubbuff_p, const Type_uWord auwsize) //QAC
{
	Type_sWord aswret = IPC_ERR_COMMON;
	Type_uHWord auhcmd1 = 0; 
	Type_uHWord auhcmd2 = 0;
	Type_uWord auwpacket_size = 0;
	Type_uByte* aubpackage = NULL; 
	static Type_uWord nuwcmd[CMDSIZE];

	
	if(auwcmd_p != NULL)
	{
		
		if((*auwcmd_p == IPC_APP_ID_IPC) && (gIPCHandler[IPC_CH_CA53_NS].ipcStatus < IPC_READY)) //USER IPC
		{
			aswret = IPC_ERR_NOTREADY;
		}
		else
		{
			wvdipc_write_lock(IPC_CH_CA53_NS);
			wvdGen_MemCpy(nuwcmd, auwcmd_p, CMDSIZE*sizeof(Type_uWord));

			auhcmd1 = (Type_uHWord)(DECODE_ID(nuwcmd[4]));
			auhcmd2 = (Type_uHWord)(DECODE_EVENT(nuwcmd[4]));
			
			nuwcmd[4] = VALUE_0;
			aubpackage = wubpackage(auhcmd1, auhcmd2, aubbuff_p, auwsize);
			if ((auwsize == LENGTH_INIT) || (aubbuff_p == NULL)) 
			{
				auwpacket_size = DUMMY_DATA_PACKET;
			} 
			else 
			{
				auwpacket_size = IPC_PACKET_PREFIX_SIZE + auwsize;
			}
			nuwcmd[3] = auwpacket_size;		
			aswret = nswipc_write_data(IPC_CH_CA53_NS, nuwcmd, aubpackage, auwpacket_size);
			wvdipc_write_unlock(IPC_CH_CA53_NS);
		}
	}
	else
	{
		aswret = IPC_ERR_COMMON;
	}

	return aswret;
}

Type_sWord wswipca7_write(const Type_uWord *auwcmd_p, const Type_uByte *aubbuff_p, const Type_uWord auwsize) //QAC
{
	Type_sWord aswret = IPC_ERR_COMMON;
	Type_uHWord auhcmd1 = 0; 
	Type_uHWord auhcmd2 = 0;
	Type_uWord auwpacket_size = 0;
	Type_uByte* aubpackage = NULL; 
	static Type_uWord nuwcmd[CMDSIZE];

	
	if(auwcmd_p != NULL)
	{
		
		if((*auwcmd_p == IPC_APP_ID_IPC) && (gIPCHandler[IPC_CH_CA7_NS].ipcStatus < IPC_READY)) //USER IPC
		{
			aswret = IPC_ERR_NOTREADY;
		}
		else
		{
			wvdipc_write_lock(IPC_CH_CA7_NS);
			wvdGen_MemCpy(nuwcmd, auwcmd_p, CMDSIZE*sizeof(Type_uWord));

			auhcmd1 = (Type_uHWord)(DECODE_ID(nuwcmd[4]));
			auhcmd2 = (Type_uHWord)(DECODE_EVENT(nuwcmd[4]));

			
			nuwcmd[4] = VALUE_0;
			aubpackage = wubpackage(auhcmd1, auhcmd2, aubbuff_p, auwsize);
			if ((auwsize == LENGTH_INIT) || (aubbuff_p == NULL)) 
			{
				auwpacket_size = DUMMY_DATA_PACKET;
			} 
			else 
			{
				auwpacket_size = IPC_PACKET_PREFIX_SIZE + auwsize;
			}
			nuwcmd[3] = auwpacket_size;		
			aswret = nswipc_write_data(IPC_CH_CA7_NS, nuwcmd, aubpackage, auwpacket_size);
			wvdipc_write_unlock(IPC_CH_CA7_NS);
		}
	}
	else
	{
		aswret = IPC_ERR_COMMON;
	}

	return aswret;
}

Type_sWord wswipcm4_write(const Type_uWord *auwcmd_p, const Type_uByte *aubbuff_p, const Type_uWord auwsize) //QAC
{
	Type_sWord aswret = IPC_ERR_COMMON;
	Type_uHWord auhcmd1 = 0; 
	Type_uHWord auhcmd2 = 0;
	Type_uWord auwpacket_size = 0;
	Type_uByte* aubpackage = NULL; 
	static Type_uWord nuwcmd[CMDSIZE];

	
	if(auwcmd_p != NULL)
	{
		
		if((*auwcmd_p == IPC_APP_ID_IPC) && (gIPCHandler[IPC_CH_CM4_NS].ipcStatus < IPC_READY)) //USER IPC
		{
			aswret = IPC_ERR_NOTREADY;
		}
		else
		{
			wvdipc_write_lock(IPC_CH_CM4_NS);
			wvdGen_MemCpy(nuwcmd, auwcmd_p, CMDSIZE*sizeof(Type_uWord));

			auhcmd1 = (Type_uHWord)(DECODE_ID(nuwcmd[4]));
			auhcmd2 = (Type_uHWord)(DECODE_EVENT(nuwcmd[4]));

			
			nuwcmd[4] = VALUE_0;
			aubpackage = wubpackage(auhcmd1, auhcmd2, aubbuff_p, auwsize);
			if ((auwsize == LENGTH_INIT) || (aubbuff_p == NULL)) 
			{
				auwpacket_size = DUMMY_DATA_PACKET;
			} 
			else 
			{
				auwpacket_size = IPC_PACKET_PREFIX_SIZE + auwsize;
			}
			nuwcmd[3] = auwpacket_size;		
			aswret = nswipc_write_data(IPC_CH_CM4_NS, nuwcmd, aubpackage, auwpacket_size);
			wvdipc_write_unlock(IPC_CH_CM4_NS);
		}
	}
	else
	{
		aswret = IPC_ERR_COMMON;
	}

	return aswret;
}
#if 0
Type_sWord wswipc_write(IPC_CH ch, const Type_uWord *auwcmd_p, const Type_uByte *aubbuff_p, const Type_uWord auwsize) //QAC
{
	Type_sWord aswret = IPC_ERR_COMMON;
	Type_uHWord auhcmd1 = 0; 
	Type_uHWord auhcmd2 = 0;
	Type_uWord auwpacket_size = 0;
	Type_uByte* aubpackage = NULL; 
	static Type_uWord nuwcmd[CMDSIZE];

	
	if(auwcmd_p != NULL)
	{
		
		if((*auwcmd_p == IPC_APP_ID_IPC) && (gIPCHandler[ch].ipcStatus < IPC_READY)) //USER IPC
		{
			aswret = IPC_ERR_NOTREADY;
		}
		else
		{
			wvdipc_write_lock(ch);
			wvdGen_MemCpy(nuwcmd, auwcmd_p, CMDSIZE*sizeof(Type_uWord));

			auhcmd1 = (Type_uHWord)(DECODE_ID(nuwcmd[4]));
			auhcmd2 = (Type_uHWord)(DECODE_EVENT(nuwcmd[4]));
			
			nuwcmd[4] = VALUE_0;
			aubpackage = wubpackage(auhcmd1, auhcmd2, aubbuff_p, auwsize);
			if ((auwsize == LENGTH_INIT) || (aubbuff_p == NULL)) 
			{
				auwpacket_size = DUMMY_DATA_PACKET;
			} 
			else 
			{
				auwpacket_size = IPC_PACKET_PREFIX_SIZE + auwsize;
			}
			nuwcmd[3] = auwpacket_size;		
			aswret = nswipc_write_data(ch, nuwcmd, aubpackage, auwpacket_size);
			wvdipc_write_unlock(ch);
		}
	}
	else
	{
		aswret = IPC_ERR_COMMON;
	}

	return aswret;
}
#endif
static Type_sWord nswipc_read_data(IPC_CH ch, Type_uByte *aubbuff_p, Type_uWord auwsize)
{
	Type_sWord aswret = 0;	//return read size

	if((aubbuff_p != NULL)&&(auwsize > 0))
	{
		Type_uWord auwdataSize, auwreadSize;

		wvdipc_rbuffer_lock(ch);
		auwdataSize = wswipc_buffer_data_available(&gIPCHandler[ch].readRingBuffer);
		wvdipc_rbuffer_unlock(ch);
		
		if(auwdataSize < auwsize)
		{
			auwreadSize = auwdataSize;
		}
		else
		{
			auwreadSize = auwsize;
		}

		if(auwreadSize !=0)
		{
			wvdipc_rbuffer_lock(ch);
			aswret = wswipc_pop_buffer(&gIPCHandler[ch].readRingBuffer, aubbuff_p, auwreadSize);
			wvdipc_rbuffer_unlock(ch);
			if(aswret == IPC_BUFFER_OK)
			{
				aswret = auwreadSize;
			}
			else
			{
				aswret = IPC_ERR_READ;
			}
		}
	}

	return aswret;
}


Type_sWord wswipc_read(IPC_CH ch, Type_uByte *aubbuff_p, Type_uWord auwsize)
{
	Type_sWord aswret = IPC_ERR_COMMON;

	if((aubbuff_p != NULL)&&(auwsize > 0))
	{
		wvdipc_read_lock(ch);
		if(gIPCHandler[ch].ipcStatus < IPC_READY)
		{
			aswret = IPC_ERR_NOTREADY;
		}
		else
		{
			aswret = nswipc_read_data(ch, aubbuff_p, auwsize);
		}
		wvdipc_read_unlock(ch);
	}

	return aswret;
}

Type_sWord wswipc_ping_test(IPC_CH ch, ipc_ping_info * pingInfo)
{
	Type_sWord aswret = IPC_ERR_COMMON;
	IpcHandler_ts *pIPCHandler = &gIPCHandler[ch];

	if(pingInfo != NULL)
	{
		aswret = IPC_SUCCESS;
		/* Check IPC */
		if(pIPCHandler->ipcStatus <= IPC_INIT)
		{
			pingInfo->pingResult = IPC_PING_ERR_INIT;
			aswret = IPC_ERR_COMMON;
		}

		if(aswret == IPC_SUCCESS)
		{
			if(pIPCHandler->ipcStatus < IPC_READY)
			{
				aswret = IPC_ERR_NOTREADY;
			}
		}

		if(aswret == IPC_SUCCESS)
		{
			Type_sWord aswstartTime, aswendTime;
			
			wvdipc_write_lock(ch);

			aswstartTime = ipc_get_msec();

			aswret = wswipc_send_ping(ch);
			if(aswret == IPC_SUCCESS)
			{
				pingInfo->pingResult = IPC_PING_SUCCESS;
			}
			else
			{
				if(aswret == IPC_ERR_TIMEOUT)
				{
					pingInfo->pingResult = IPC_PING_ERR_RESPOND;
				}
				else if (aswret == IPC_ERR_RECEIVER_NOT_SET)
				{
					pingInfo->pingResult = IPC_PING_ERR_RECEIVER_MBOX;
				}
				else if (aswret == IPC_ERR_RECEIVER_DOWN)
				{
					pingInfo->pingResult = IPC_PING_ERR_SEND;
				}
				else if (aswret == IPC_ERR_NOTREADY)
				{
					pingInfo->pingResult = IPC_PING_ERR_NOT_READY;
				}
				else
				{
					pingInfo->pingResult = IPC_PING_ERR_SENDER_MBOX;
				}
				aswret = IPC_ERR_NOTREADY;
			}

			wvdipc_write_unlock(ch);
			
			aswendTime =  ipc_get_msec();

			pingInfo->responseTime = (Type_uWord)(aswendTime - aswstartTime); //msec
		}
	}
	return aswret;
}
void wvdipc_struct_init(IPC_CH ch, IpcHandler_ts *ipcHandler)
{
	ipcHandler->ipcStatus = IPC_NULL;

	ipcHandler->mboxReg.uwmbox0_base = ipc_ctr_mbox[ch].base0; //AP
	ipcHandler->mboxReg.uwmbox1_base = ipc_ctr_mbox[ch].base1; //MICOM
	ipcHandler->mboxReg.uwirq = ipc_ctr_mbox[ch].uwirq;

}

void wvdIPCHandler_Init(IPC_CH ch, IpcHandler_ts *ipcHandler)
{
	ipcHandler->ipcStatus = IPC_NULL;

	ipcHandler->mboxReg.uwmbox0_base = ipc_ctr_mbox[ch].base0; //AP
	ipcHandler->mboxReg.uwmbox1_base = ipc_ctr_mbox[ch].base1; //MICOM
	ipcHandler->mboxReg.uwirq = ipc_ctr_mbox[ch].uwirq;


	ipcHandler->ipcOption.isAck = 0;

}

static void nvdipc_set_status(IPC_CH ch, IpcStatus status)
{
	gIPCHandler[ch].ipcStatus = status;
}

IpcStatus wswipc_get_status(IPC_CH ch)
{
	return gIPCHandler[ch].ipcStatus;
}

static void nvdipc_set_buffer(IPC_CH ch)
{	
	wvdipc_wbuffer_lock(ch);
	wvdipc_buffer_init(&gIPCHandler[ch].readRingBuffer, (Type_uByte*)&(gIPCRingBuffer[ch]), MAX_IPC_RING_BUFF_SIZE);
	wvdipc_wbuffer_unlock(ch);
}


static void nvdipc_receive_ctlcmd(IPC_CH ch, const MboxMsgReg  * pMsg)
{
	if(pMsg != NULL)
	{
		Type_uWord 	auwseqID = pMsg->reg[1];
		IpcCmdID	cmdID = (IpcCmdID)(pMsg->reg[2] & CMD_ID_MASK); //GHC Warning : enumerated type mixed with another type
		Type_uWord	auwack = gIPCHandler[ch].ipcOption.isAck;

		switch(cmdID)
		{
			case IPC_OPEN:
				//printf("<%s:%d>\n", __func__, __LINE__);
				nvdipc_set_status(ch, IPC_READY);
				if(auwack == IPC_O_ACK)
				{
					(void)wswipc_send_ack(ch, auwseqID, CTL_CMD, pMsg->reg[2]);
				}
				break;
			case IPC_CLOSE:
				gIPCHandler[ch].ipcStatus = IPC_INIT;
				OSAL_MemSet((Type_uByte*)&(gIPCRingBuffer[ch]), 0, MAX_IPC_RING_BUFF_SIZE);
				break;
			case IPC_SEND_PING:
				if(auwack == IPC_O_ACK)
				{
					(void)wswipc_send_ack(ch, auwseqID, CTL_CMD, pMsg->reg[2]);
				}
				break;
			case IPC_ACK:
			default:
				break;
		}
	}
}

/*receive read cmd : other prosseor write my read buffer */
static void nvdipc_receive_writecmd(IPC_CH ch, const MboxMsgReg  * pMsg)
{
	Type_sWord aswret =IPC_BUFFER_ERROR;
	
	if(pMsg != NULL)
	{
		Type_uWord 	auwseqID = pMsg->reg[1];
		IpcCmdID	cmdID = (IpcCmdID)(pMsg->reg[2] & CMD_ID_MASK);//GHC Warning : enumerated type mixed with another type
		Type_uWord	auwack = gIPCHandler[ch].ipcOption.isAck;

		switch(cmdID)
		{
			case IPC_WRITE:
				{
					Type_uWord auwreadSize;
					Type_uWord auwidx;

					auwreadSize = pMsg->reg[3];
					auwidx = pMsg->sel_dataIdx;
					//printf("<%s:%d>\n", __func__, __LINE__);
					if(auwidx < MAX_MBOX_DATA_IDX)
					{
						if((auwreadSize > 0) && (auwreadSize <= MAX_IPC_FIFO_SIZE))
						{
							//printf("<%s:%d>\n", __func__, __LINE__);
							Type_uWord auwfreeSpace;
							Type_uByte * aubbuffer_p = (Type_uByte *) &(mboxData[ch].data[auwidx]);
							IPC_RINGBUF_st* pRingBuffer = &gIPCHandler[ch].readRingBuffer;
							wvdipc_rbuffer_lock(ch);
							auwfreeSpace = wswipc_buffer_free_space(&gIPCHandler[ch].readRingBuffer);
							if(auwfreeSpace > auwreadSize)
							{
								//printf("<%s:%d>\n", __func__, __LINE__);
								aswret = wswipc_push_buffer(pRingBuffer, aubbuffer_p,auwreadSize);

								if(aswret  > 0)
								{
									aswret = IPC_SUCCESS;
								}
								else
								{
									aswret = IPC_ERR_BUFFER;
								}
							}
							else
							{
								aswret = wswipc_push_buffer_overwrite(pRingBuffer, aubbuffer_p, auwreadSize);
								if(aswret  > 0)
								{
									aswret = IPC_SUCCESS;
								}
								else
								{
									aswret = IPC_ERR_BUFFER;
								}
							}
							wvdipc_rbuffer_unlock(ch);
						}
					}
					if(aswret ==IPC_BUFFER_OK)
					{
						if(auwack == IPC_O_ACK)
						{
							aswret = wswipc_send_ack(ch, auwseqID, WRITE_CMD, pMsg->reg[2]);
						}
						
						if((auwack == IPC_O_ACK) && (aswret <0))
						{
							aswret =IPC_ERR_RECEIVER_DOWN;
						}
						else
						{

						}
					}
				}
				break;
			default:
				break;
		}
	}
}

void wvdRingBufferInit(IPC_CH ch)
{
	(void)OSAL_MemSet(&gIPCRingBuffer[ch][VALUE_0], VALUE_0, MAX_IPC_RING_BUFF_SIZE);
}

void wvdIPCHandlerInit(IPC_CH ch)
{
	wvdIPCHandler_Init(ch, &gIPCHandler[ch]);
	wvdipc_buffer_flush(&gIPCHandler[ch].readRingBuffer);

}

Type_sWord nswReciver_SendQueue(OS_QueueId_t auwMbxId,
						   const Type_uHWord auhEventCode,
						   void	*avdBuffer_p,
						   const Type_uWord	auwBufferSize,
						   const OS_ObjPri_t aubPriority)
{
	Type_sWord			aswReturnCode = RES_NG;					/* Return Code						*/
	OS_StdQueue_ts		astStdQueue;					/* Mail Buffer						*/
	OS_VoidPtr_t		avdMemAddr_p;					/* void Pointer Address				*/
	OS_ErrCode_t		aswOSErrorCode;					/* OS Error code					*/

	avdMemAddr_p = STD_NULL;
	wvdGen_MemSet(&astStdQueue, RPC_CLEAR_UB , sizeof(astStdQueue));
	avdMemAddr_p = avdBuffer_p;

	astStdQueue.swSrcTaskId	 = ARPC_ID_R5_RPC;
	astStdQueue.uhEventCode	 = auhEventCode;
	if((avdBuffer_p != STD_NULL) && (auwBufferSize != RPC_CLEAR_UB))
	{
		astStdQueue.vdQueueBuffer_p = avdMemAddr_p;
		astStdQueue.uwQueueSize	 = auwBufferSize;

		aswOSErrorCode	= OS_SendQueue(auwMbxId, aubPriority, &astStdQueue);

		if (OS_ERR_OK == aswOSErrorCode)
		{
			aswReturnCode = RES_OK;
		}
		else
		{
			aswReturnCode = RES_NG;
		}
	}
	else
	{
		aswReturnCode = RES_NG;
		printf("Send data NULL[%s]: <%s:%d>\n",__func__, __FILE__, __LINE__);

	}

	return aswReturnCode;

}

#ifdef IPC_CA53_NS
static void nvdipc_ca53_ns_receive_message(IPC_CH ch, const MboxMsgReg *mboxMsg)
{
	static MboxMsgReg  ReceivedMsg[MAX_CMD_TYPE]; //QAC
	
	if(mboxMsg != NULL)
	{
		IpcCmdType	cmdType;
		IpcCmdID	cmdID ;
		IpcCmdID	srcCmdID;
		Type_uWord	auwappID;
		Type_uWord	auwack;
		Type_uByte	aubsendMsg = false;
		//int aubReturn = 100;

		auwappID = mboxMsg->reg[0];
		cmdType = (IpcCmdType)((mboxMsg->reg[2] & CMD_TYPE_MASK ) >>16);//GHC Warning : enumerated type mixed with another type
		cmdID = (IpcCmdID)(mboxMsg->reg[2] & CMD_ID_MASK);//GHC Warning : enumerated type mixed with another type
		srcCmdID = (IpcCmdID)(mboxMsg->reg[3]);//GHC Warning : enumerated type mixed with another type

		auwack = gIPCHandler[IPC_CH_CA53_NS].ipcOption.isAck;

		if((auwappID == IPC_APP_ID_UBOOT) || (auwappID == IPC_APP_ID_ARMTF) || (auwappID == IPC_APP_ID_A53MR))//Msg(4:3397) Binary operator other than + - * / % used with binary operator of different precedence. Extra parentheses recommended. MISRA-C:2004 Rule 12.1
		{
			cmdType = (IpcCmdType)(0); //GHC Warning : enumerated type mixed with another type
			aubsendMsg = true;
		}
		else if(auwappID == IPC_APP_ID_FWUG)
		{
			cmdType = (IpcCmdType)(0); //GHC Warning : enumerated type mixed with another type
			aubsendMsg = true;
		}
		else if(auwappID == IPC_APP_ID_IPC)
		{
			if(cmdType < MAX_CMD_TYPE)
			{
				if((auwack == IPC_O_ACK) && (cmdID == IPC_ACK) && (srcCmdID != IPC_OPEN))
				{
					wvdipc_cmd_wake_up(ch, cmdType,mboxMsg->reg[1]) ;
				}
				else if((auwack == IPC_O_ACK) && (cmdID == IPC_ACK))
				{
					
					nvdipc_set_status(ch, IPC_READY);
				}
				else
				{
					aubsendMsg = true;
				}
			}
		}
		else
		{
			/* unknown app id */
		}

		if(aubsendMsg == true)
		{
			ReceivedMsg[cmdType].reg[0] = mboxMsg->reg[0];
			ReceivedMsg[cmdType].reg[1] = mboxMsg->reg[1];
			ReceivedMsg[cmdType].reg[2] = mboxMsg->reg[2];
			ReceivedMsg[cmdType].reg[3] = mboxMsg->reg[3];
			ReceivedMsg[cmdType].reg[4] = mboxMsg->reg[4];
			ReceivedMsg[cmdType].reg[5] = mboxMsg->reg[5];
			ReceivedMsg[cmdType].reg[6] = mboxMsg->reg[6];
			ReceivedMsg[cmdType].reg[7] = mboxMsg->reg[7];
			ReceivedMsg[cmdType].sel_dataIdx = mboxMsg->sel_dataIdx;
			
			(void)nswReciver_SendQueue(MBX_ID_MAIN_RECIVER, cmdType, (void *)&ReceivedMsg[cmdType], sizeof(ReceivedMsg), OS_MAIL_PRI_NORMAL);
		}
	}
}

void TSK_MAIN_RECIVER(void)
{
	MboxMsgReg	cpyMsg;
	MboxMsgReg*	cpyMsg_p;
	Type_sWord aswerr = OS_ERR_OK;
	OS_StdQueue_ts astMessage;
	OS_StdQueue_ts* astMessage_p;

	wvdGen_MemSet(&astMessage, 0x0, sizeof(astMessage));
	wvdGen_MemSet(&cpyMsg, 0x0, sizeof(cpyMsg));

	astMessage_p = &astMessage;
	cpyMsg_p = &cpyMsg;

	while(1)
	{
		aswerr = OS_ReceiveQueue(MBX_ID_MAIN_RECIVER, OS_TOUT_FOREVER, (void *)&astMessage_p);
		if(wubRPCStandbyFlag == RPC_STANDBYOUT)
		{
			cpyMsg_p = astMessage_p->vdQueueBuffer_p;
			
			if((aswerr == OS_ERR_OK) && ((astMessage_p->uwQueueSize) > 0))
			{
				IpcCmdType cmdType = 0;
				Type_uWord auwappIden = 0;

				auwappIden = cpyMsg_p->reg[0];
				cmdType = (IpcCmdType)((cpyMsg_p->reg[2] & CMD_TYPE_MASK ) >> 16); //fix casting error

				if(auwappIden == IPC_APP_ID_UBOOT)
				{

				}
				else if(auwappIden == IPC_APP_ID_FWUG)
				{
					Type_uWord auwcmd;
					auwcmd = cpyMsg_p->reg[1];

					FWUG_Start(auwcmd);
				}
				else if(auwappIden == IPC_APP_ID_IPC)
				{
					switch(cmdType)
					{
						case CTL_CMD:
							//printf("<%s:%d>\n", __func__, __LINE__);
							nvdipc_receive_ctlcmd(IPC_CH_CA53_NS, (const MboxMsgReg *)cpyMsg_p);
							break;
						case WRITE_CMD:
							//printf("<%s:%d>\n", __func__, __LINE__);
							nvdipc_receive_writecmd(IPC_CH_CA53_NS, (const MboxMsgReg *)cpyMsg_p);
							break;
						default:
							break;
					}
				}
				else
				{
				/* unknown app id */
				}
			}
		}
	}
}

#endif

#ifdef IPC_CA7_NS
static void nvdipc_ca7_ns_receive_message(IPC_CH ch, const MboxMsgReg *mboxMsg)
{
	static MboxMsgReg  ReceivedMsg[MAX_CMD_TYPE]; //QAC
	if(mboxMsg != NULL)
	{
		IpcCmdType	cmdType;
		IpcCmdID	cmdID ;
		IpcCmdID	srcCmdID;
		Type_uWord	auwappID;
		Type_uWord	auwack;

		auwappID = mboxMsg->reg[0];
		cmdType = (IpcCmdType)((mboxMsg->reg[2] & CMD_TYPE_MASK ) >>16);//GHC Warning : enumerated type mixed with another type
		cmdID = (IpcCmdID)(mboxMsg->reg[2] & CMD_ID_MASK);//GHC Warning : enumerated type mixed with another type
		srcCmdID = (IpcCmdID)(mboxMsg->reg[3]);//GHC Warning : enumerated type mixed with another type

		auwack = gIPCHandler[IPC_CH_CA7_NS].ipcOption.isAck;


		if((auwappID == IPC_APP_ID_UBOOT) || (auwappID == IPC_APP_ID_A7SR))
		{
			ReceivedMsg[0].reg[0] = mboxMsg->reg[0];
			ReceivedMsg[0].reg[1] = mboxMsg->reg[1];
			ReceivedMsg[0].reg[2] = mboxMsg->reg[2];
			ReceivedMsg[0].reg[3] = mboxMsg->reg[3];
			ReceivedMsg[0].reg[4] = mboxMsg->reg[4];
			ReceivedMsg[0].reg[5] = mboxMsg->reg[5];
			ReceivedMsg[0].reg[6] = mboxMsg->reg[6];
			ReceivedMsg[0].reg[7] = mboxMsg->reg[7];
			ReceivedMsg[0].sel_dataIdx = mboxMsg->sel_dataIdx;

			(void)OSAL_QueuePut(ipcCtrTaskInfo[ch].ipcControlQID,  (void *)&ReceivedMsg[0], sizeof(MboxMsgReg), 0, OSAL_NO_TIMEOUT, OSAL_API_OPT_BLOCKING);
		}
		else if(auwappID == IPC_APP_ID_IPC)
		{
			if(cmdType < MAX_CMD_TYPE)
			{
				if((auwack == IPC_O_ACK) && (cmdID == IPC_ACK) && (srcCmdID != IPC_OPEN))
				{
					wvdipc_cmd_wake_up(ch, cmdType,mboxMsg->reg[1]) ;
				}
				else if((auwack == IPC_O_ACK) && (cmdID == IPC_ACK))
				{
					nvdipc_set_status(ch, IPC_READY);
				}
				else
				{
					ReceivedMsg[cmdType].reg[0] = mboxMsg->reg[0];
					ReceivedMsg[cmdType].reg[1] = mboxMsg->reg[1];
					ReceivedMsg[cmdType].reg[2] = mboxMsg->reg[2];
					ReceivedMsg[cmdType].reg[3] = mboxMsg->reg[3];
					ReceivedMsg[cmdType].reg[4] = mboxMsg->reg[4];
					ReceivedMsg[cmdType].reg[5] = mboxMsg->reg[5];
					ReceivedMsg[cmdType].reg[6] = mboxMsg->reg[6];
					ReceivedMsg[cmdType].reg[7] = mboxMsg->reg[7];
					ReceivedMsg[cmdType].sel_dataIdx = mboxMsg->sel_dataIdx;

					(void)nswReciver_SendQueue(MBX_ID_SUB_RECIVER, cmdType, (void *)&ReceivedMsg[cmdType], sizeof(ReceivedMsg), OS_MAIL_PRI_NORMAL);
				}
			}
		}
		else
		{
			/* unknown app id */
		}
	}
}

void TSK_SUB_RECIVER(void)
{
	MboxMsgReg	cpyMsg;
	MboxMsgReg*	cpyMsg_p;
	Type_sWord aswerr = OS_ERR_OK;
	OS_StdQueue_ts astMessage;
	OS_StdQueue_ts* astMessage_p;
	
	wvdGen_MemSet(&astMessage, 0x0, sizeof(astMessage));
	wvdGen_MemSet(&cpyMsg, 0x0, sizeof(cpyMsg));

	astMessage_p = &astMessage;	
	cpyMsg_p = &cpyMsg;

	while(1)
	{
		aswerr = OS_ReceiveQueue(MBX_ID_SUB_RECIVER, OS_TOUT_FOREVER, (void *)&astMessage_p);
		if(wubRPCStandbyFlag == RPC_STANDBYOUT)
		{
			cpyMsg_p = astMessage_p->vdQueueBuffer_p;
			
			if((aswerr == OS_ERR_OK)&&((astMessage_p->uwQueueSize) > 0))
			{
				IpcCmdType cmdType;
				Type_uWord auwappID;
			
				cmdType = (IpcCmdType)((cpyMsg_p->reg[2] & CMD_TYPE_MASK ) >>16);//GHC Warning : enumerated type mixed with another type
				auwappID = cpyMsg_p->reg[0];

				if(auwappID == IPC_APP_ID_UBOOT)
				{
					;
				}
				else if(auwappID == IPC_APP_ID_IPC)
				{
					switch(cmdType)
					{
						case CTL_CMD:
							nvdipc_receive_ctlcmd(IPC_CH_CA7_NS, (const MboxMsgReg *)cpyMsg_p);
							break;
						case WRITE_CMD:
							nvdipc_receive_writecmd(IPC_CH_CA7_NS, (const MboxMsgReg *)cpyMsg_p);
							break;
						default:
							break;
					}
				}
				else
				{
				/* unknown app id */
				}
			}
		}
	}
}

#endif
#ifdef IPC_CM4_NS
static void ipc_cm4_ns_receive_message(IPC_CH ch, const MboxMsgReg *mboxMsg)
{
	static MboxMsgReg  ReceivedMsg; //QAC

	if(mboxMsg != NULL)
	{
		ReceivedMsg.reg[0] = mboxMsg->reg[0];
		ReceivedMsg.reg[1] = mboxMsg->reg[1];
		ReceivedMsg.reg[2] = mboxMsg->reg[2];
		ReceivedMsg.reg[3] = mboxMsg->reg[3];
		ReceivedMsg.reg[4] = mboxMsg->reg[4];
		ReceivedMsg.reg[5] = mboxMsg->reg[5];
		ReceivedMsg.reg[6] = mboxMsg->reg[6];
		ReceivedMsg.reg[7] = mboxMsg->reg[7];

		(void)OSAL_QueuePut(ipcCtrTaskInfo[ch].ipcControlQID,  (void *)&ReceivedMsg, sizeof(MboxMsgReg), 0, OSAL_NO_TIMEOUT, OSAL_API_OPT_BLOCKING);
	}
}

static void ipc_cm4_ns_control_task(void *p_arg)
{
    IPC_CH ch = IPC_CH_CM4_NS;
	MboxMsgReg	cpyMsg;
	uint32 msgSize = 0;
	int32 err = OSAL_ERR_NONE;

	cpyMsg.reg[0] = 0x0;
	cpyMsg.reg[1] = 0x0;
	cpyMsg.reg[2] = 0x0;
	cpyMsg.reg[3] = 0x0;
	cpyMsg.reg[4] = 0x0;
	cpyMsg.reg[5] = 0x0;
	cpyMsg.reg[6] = 0x0;
	cpyMsg.reg[7] = 0x0;

	while(1)
	{
		err = OSAL_QueueGet(ipcCtrTaskInfo[IPC_CH_CM4_NS].ipcControlQID, (void *)&cpyMsg, sizeof(MboxMsgReg), (uint32 *)&msgSize, OSAL_NO_TIMEOUT, OSAL_API_OPT_BLOCKING);

		if((err == OSAL_ERR_NONE)&&(msgSize > 0))
		{
	        IPC_INT32 ret =IPC_BUFFER_ERROR;
			IPC_UINT32 readSize;

			readSize = cpyMsg.reg[4];
			if((readSize > 0) && (readSize <= MAX_IPC_FIFO_SIZE))
			{
				IPC_UINT32 freeSpace = 0;
				//IPC_UCHAR buffer[MAX_IPC_FIFO_SIZE] = {0, }; //QAC Msg(4:0686) Array has fewer initializers than its declared size.
				IPC_UCHAR buffer[MAX_IPC_FIFO_SIZE];
				IPC_RINGBUF_st * pRingBuffer = &gIPCHandler[ch].readRingBuffer;

				OSAL_MemSet((void *)buffer, 0x00, MAX_IPC_FIFO_SIZE);
				wvdipc_rbuffer_lock(ch);
				freeSpace = wswipc_buffer_free_space(&gIPCHandler[ch].readRingBuffer);
				if(freeSpace > readSize)
				{
					wvdipc_mailbox_get_data(ch, buffer, readSize);
					ret = wswipc_push_buffer(pRingBuffer, buffer, readSize);
					if(ret  > 0)
					{
						ret = IPC_SUCCESS;
					}
					else
					{
						ret = IPC_ERR_BUFFER;
					}
				}
				else
				{
					//wvdipc_buffer_overflow_wake_up(ch, READ_BUF_OVERFLOW_BIT);
					ret = wswipc_push_buffer_overwrite(pRingBuffer, buffer, readSize);
					if(ret  > 0)
					{
						ret = IPC_SUCCESS;
					}
					else
					{
						ret = IPC_ERR_BUFFER;
					}
				}
				wvdipc_rbuffer_unlock(ch);
			}
		}
	}
}

#endif

