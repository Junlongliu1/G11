/****************************************************************************************
 *   FileName    : tcc_ipc_ctl.h
 *   Description : 
 ****************************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips Inc.
 *   All rights reserved 
 
This source code contains confidential information of Telechips.
Any unauthorized use without a written permission of Telechips including not limited 
to re-distribution in source or binary form is strictly prohibited.
This source code is provided ��AS IS�� and nothing contained in this source code 
shall constitute any express or implied warranty of any kind, including without limitation, 
any warranty of merchantability, fitness for a particular purpose or non-infringement of any patent, 
copyright or other third party intellectual property right. 
No warranty is made, express or implied, regarding the information��s accuracy, 
completeness, or performance. 
In no event shall Telechips be liable for any claim, damages or other liability arising from, 
out of or in connection with this source code or the use in the source code. 
This source code is provided subject to the terms of a Mutual Non-Disclosure Agreement 
between Telechips and Company.
*
****************************************************************************************/

#ifndef __TCC_IPC_CTL_H__
#define __TCC_IPC_CTL_H__
#include "apn_basic_type.h"
#include "tcc_ipc_typedef.h"
#include "tcc_ipc_buffer.h"
#include "tcc_ipc_mbox.h"
#include "os_framework.h"

#define MAX_READ_TIMEOUT       50      //ms

#define PING_DATA_SIZE			64
#define PING_RESPONSE_SIGN		0x5A

#define MAX_IPC_RING_BUFF_SIZE	2048
#define MAX_IPC_FIFO_SIZE	512

#define IPC_TASK_START_STK_SIZE 256

#define MAX_QUEUE_DEPTH     4

typedef struct _IpcMsgInfo{
	Type_uWord uwseqID;		/* sequential ID */
	Type_uWord uwcmdType;
	Type_uWord uwcmdID;
	Type_uWord uwdata1;
	Type_uWord uwdata2;
	Type_uWord uwdata3;
	Type_uWord uwdata4;
	Type_uWord uwdata5;
	Type_uWord uwdata6;
}IpcMsgInfo_ts;

typedef enum {
	IPC_NULL =0,
	IPC_INIT,
	IPC_OPENED,
	IPC_READY,			/*Buffer setting completed*/
	IPC_MAX_STATUS,
} IpcStatus;

typedef enum {
	IPC_BUF_NULL =0,
	IPC_BUF_READY,
	IPC_BUF_BUSY,
	IPC_BUF_MAX_STATUS,
} IpcBufferStatus;

typedef struct _IpcBufferInfo{
	Type_uWord	uwstartAddr;
	Type_uWord 	uwbufferSize;
	IpcBufferStatus	status;
}IpcBufferInfo_ts;

typedef struct _IpcMboxReg {
    Type_uWord uwmbox0_base;
    Type_uWord uwmbox1_base;
    Type_uWord uwirq;
} IpcMboxReg_ts;

typedef struct _IpcHandler{
	IpcStatus		ipcStatus;

	IPC_RINGBUF_st readRingBuffer;

	IpcMboxReg_ts	mboxReg;

	//Type_uWord	uwisWait;
	//Type_uWord	uwvTime;
	//Type_uWord	uwvMin;

	//ipc_ctl_param	setParam;

	IPC_OPTION ipcOption;
}IpcHandler_ts;

struct ipc_ctrtask_info_ts {
	Type_uWord ipcControlQID;
	Type_uWord IPC_ControlTask_ID;
	Type_uWord IPC_ControlTaskStk[IPC_TASK_START_STK_SIZE];
};

struct ipc_mbox_ts {
	Type_uWord ch;		// MBOX Channel
	addr_t base0;		// MBOX0 BASE
	addr_t base1;		// MBOX1 BASE
	Type_uWord uwirq;		// irq number
	ipc_mbox_receive receive_cb;
};

extern IpcHandler_ts 	gIPCHandler[IPC_CH_MAX];

void wvdipc_struct_init(IPC_CH ch, IpcHandler_ts *ipcHandler);
IpcStatus wswipc_get_status(IPC_CH ch);
Type_sWord wswipc_initialize(IPC_CH ch, Type_uByte aubintLevel);
void wvdipc_release(IPC_CH ch);
Type_sWord wswipc_write(IPC_CH ch, const Type_uWord *auwcmd_p, const Type_uByte *aubbuff_p, const Type_uWord auwsize); //QAC
Type_sWord wswipc_read(IPC_CH ch, Type_uByte *aubbuff_p, Type_uWord auwsize);
Type_sWord wswipc_ping_test(IPC_CH ch, ipc_ping_info * pingInfo);
void wvdRingBufferInit(IPC_CH ch);
void wvdIPCHandlerInit(IPC_CH ch);
void wvdIPCHandler_Init(IPC_CH ch, IpcHandler_ts *ipcHandler);
extern Type_sWord nswReciver_SendQueue(OS_QueueId_t auwMbxId, const Type_uHWord auhEventCode, void* avdBuffer_p, const Type_uWord auwBufferSize, const OS_ObjPri_t aubPriority);
Type_sWord wswipca53_write(const Type_uWord *auwcmd_p, const Type_uByte *aubbuff_p, const Type_uWord auwsize);
Type_sWord wswipca7_write(const Type_uWord *auwcmd_p, const Type_uByte *aubbuff_p, const Type_uWord auwsize);
Type_sWord wswipcm4_write(const Type_uWord *auwcmd_p, const Type_uByte *aubbuff_p, const Type_uWord auwsize);
#endif /* __TCC_IPC_CTL_H__ */
