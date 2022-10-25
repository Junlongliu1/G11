/****************************************************************************************
 *   FileName    : tcc_ipc_cmd.c
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
#include "tcc_ipc_typedef.h"
#include "tcc_ipc.h"
#include "tcc_ipc_os.h"
#include "tcc_ipc_mbox.h"
#include "tcc_ipc_cmd.h"

#define OPEN_MBOX_TIMEOUT		5	//ms
#define MBOX_TIMEOUT		100	//ms
#define ACK_TIMEOUT			500	//ms

static Type_uWord nuwget_sequential_ID(IPC_CH ch);

static Type_uWord nuwget_sequential_ID(IPC_CH ch)
{
	static Type_uWord nuwsendSeqID[IPC_CH_MAX]={
//QAC : Msg(4:0686) Array has fewer initializers than its declared size. Default initialization is applied to the remainder of the array elements. MISRA-C:2004 Rule 9.2; REFERENCE - ISO-6.5.7 Semantics		
#ifdef IPC_CA53_S 
	0,
#endif
#ifdef IPC_CA53_NS
	0,
#endif
#ifdef IPC_CA7_S
	0,
#endif
#ifdef IPC_CA7_NS
	0,
#endif
#ifdef IPC_CM4_NS
	0
#endif
	};
	wvdipc_common_lock(ch);
	nuwsendSeqID[ch]++;
	if(nuwsendSeqID[ch] == 0xFFFFFFFFU)
	{
		nuwsendSeqID[ch] = 1;
	}
	wvdipc_common_unlock(ch);

	return nuwsendSeqID[ch];
}

Type_sWord wswipc_send_open(IPC_CH ch)

{
	Type_sWord aswret = IPC_ERR_COMMON;
	MboxMsgReg sendMsg;

#ifdef IPC_CM4_NS
	if(ch == IPC_CH_CM4_NS)
	{
		aswret = IPC_SUCCESS;    
	}
	else
#endif
	{
		sendMsg.reg[0] = 1;
		sendMsg.reg[1] = nuwget_sequential_ID(ch);
		sendMsg.reg[2] = (CTL_CMD<<16)|(IPC_OPEN);
		sendMsg.reg[3] = 0;
		sendMsg.reg[4] = 0;
		sendMsg.reg[5] = 0;
		sendMsg.reg[6] = 0;
		sendMsg.reg[7] = 0;

		aswret = wswipc_mailbox_send_open(ch, &sendMsg, OPEN_MBOX_TIMEOUT);
	}
	
	return aswret;
}

Type_sWord wswipc_send_close(IPC_CH ch)

{
	Type_sWord aswret = IPC_ERR_COMMON;
	MboxMsgReg sendMsg;

#ifdef IPC_CM4_NS
	if(ch == IPC_CH_CM4_NS)
	{
		aswret = IPC_SUCCESS;    
	}
	else
#endif
	{
		sendMsg.reg[0] = 1;
		sendMsg.reg[1] = nuwget_sequential_ID(ch);
		sendMsg.reg[2] = (CTL_CMD<<16)|(IPC_CLOSE);
		sendMsg.reg[3] = 0;
		sendMsg.reg[4] = 0;
		sendMsg.reg[5] = 0;
		sendMsg.reg[6] = 0;
		sendMsg.reg[7] = 0;

		aswret = wswipc_mailbox_send_close(ch, &sendMsg, MBOX_TIMEOUT);
	}
	
	return aswret;
}
#if 0
Type_sWord wswipc_send_sys_ctrl(IPC_CH ch, Type_uWord *auwcmd_p, Type_uByte *aubbuff_p, Type_uWord auwsize, Type_uWord auwisAck)

{
    Type_sWord aswidx = 0;
    Type_sWord aswret = IPC_ERR_COMMON;
    MboxMsgReg sendMsg;
    
    if(auwisAck == IPC_O_ACK)
    {
        // If you want to wait ack, cmd[1] must be sequence Id. 
        if (auwcmd_p != NULL)
        {
    	    wvdipc_cmd_wake_preset(ch, WRITE_CMD, auwcmd_p[1]);
        }
    }
   	if (auwcmd_p != NULL)
   	{
    	for (aswidx = 0; aswidx < 8; aswidx++)
    	{
        	sendMsg.reg[aswidx] = auwcmd_p[aswidx];
    	}

    	aswret = wswipc_mailbox_send_data(ch, &sendMsg, aubbuff_p, auwsize, MBOX_TIMEOUT);
    
    	if((auwisAck == IPC_O_ACK) && (aswret == IPC_SUCCESS))
    	{
    		aswret = wswipc_cmd_wait_event_timeout(ch, WRITE_CMD,ACK_TIMEOUT);
    	}
   	}
	return aswret;
}
#endif
Type_sWord wswipc_send_write(IPC_CH ch, const Type_uWord *auwcmd_p, const Type_uByte *aubbuff_p, const Type_uWord auwsize, Type_uWord auwisAck) //QAC
{
	Type_sWord aswret = IPC_ERR_COMMON;
	MboxMsgReg sendMsg;

#ifdef IPC_CM4_NS
	if(ch == IPC_CH_CM4_NS)
	{
		const IPC_UCHAR *pCmd = (const IPC_UCHAR *)auwcmd_p; //QAC
		sendMsg.reg[0] = *((const uint32 *)&pCmd[0]); //QAC : const
		sendMsg.reg[1] = *((const uint32 *)&pCmd[4]);
		sendMsg.reg[2] = *((const uint32 *)&pCmd[8]);
		sendMsg.reg[3] = *((const uint32 *)&pCmd[12]);
		sendMsg.reg[4] = *((const uint32 *)&pCmd[16]);
		sendMsg.reg[5] = *((const uint32 *)&pCmd[20]);
		sendMsg.reg[6] = *((const uint32 *)&pCmd[24]);
		sendMsg.reg[7] = *((const uint32 *)&pCmd[28]);

		aswret = wswipc_mailbox_send_data(ch, &sendMsg, aubbuff_p, auwsize, MBOX_TIMEOUT);
	}
	else
#endif
	{
		sendMsg.reg[0] = *(auwcmd_p+0);
		if(sendMsg.reg[0] != IPC_APP_ID_IPC)
		{
			sendMsg.reg[1] = *(auwcmd_p+1);
		}
		else
		{
			sendMsg.reg[1] = nuwget_sequential_ID(ch);
		}
		sendMsg.reg[2] = *(auwcmd_p+2);
		sendMsg.reg[3] = *(auwcmd_p+3);
		sendMsg.reg[4] = *(auwcmd_p+4);
		sendMsg.reg[5] = *(auwcmd_p+5);
		sendMsg.reg[6] = *(auwcmd_p+6);
		sendMsg.reg[7] = *(auwcmd_p+7);

		if(auwisAck == IPC_O_ACK)
		{
			wvdipc_cmd_wake_preset(ch, WRITE_CMD, sendMsg.reg[1]);
		}
		
		aswret = wswipc_mailbox_send_data(ch, &sendMsg, aubbuff_p, auwsize, MBOX_TIMEOUT);
		
		if((auwisAck == IPC_O_ACK) && (aswret == IPC_SUCCESS))
		{
			aswret = wswipc_cmd_wait_event_timeout(ch, WRITE_CMD,ACK_TIMEOUT);
		}
	}

	return aswret;
}

Type_sWord wswipc_send_ping(IPC_CH ch)

{
	Type_sWord aswret = IPC_ERR_COMMON;
	MboxMsgReg sendMsg;

	sendMsg.reg[0] = 1;
	sendMsg.reg[1] = nuwget_sequential_ID(ch);
	sendMsg.reg[2] = (CTL_CMD<<16)|(IPC_SEND_PING);
	sendMsg.reg[3] = 0;
	sendMsg.reg[4] = 0;
	sendMsg.reg[5] = 0;
	sendMsg.reg[6] = 0;
	sendMsg.reg[7] = 0;

	wvdipc_cmd_wake_preset(ch, CTL_CMD, sendMsg.reg[1]);
	aswret = wswipc_mailbox_send(ch, &sendMsg, MBOX_TIMEOUT);
	if(aswret == IPC_SUCCESS)
	{
		aswret = wswipc_cmd_wait_event_timeout(ch, CTL_CMD, ACK_TIMEOUT);

	}
	return aswret;
}

Type_sWord wswipc_send_ack(IPC_CH ch, Type_uWord auwseqID, IpcCmdType cmdType, Type_uWord auwsourcCmd)

{
	Type_sWord aswret = IPC_ERR_COMMON;
	MboxMsgReg sendMsg;

	sendMsg.reg[0] = 1;
	sendMsg.reg[1] = auwseqID;
	sendMsg.reg[2] = (cmdType<<16)|(IPC_ACK);
	sendMsg.reg[3] = auwsourcCmd;
	sendMsg.reg[4] = 0;
	sendMsg.reg[5] = 0;
	sendMsg.reg[6] = 0;
	sendMsg.reg[7] = 0;

	aswret = wswipc_mailbox_send(ch, &sendMsg, MBOX_TIMEOUT);
	return aswret;
}

