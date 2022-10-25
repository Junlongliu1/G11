/****************************************************************************************
 *   FileName    : tcc_ipc_os.c
 *   Description : 
 ****************************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips Inc.
 *   All rights reserved 
 
This source code contains confidential information of Telechips.
Any unauthorized use without a written permission of Telechips including not limited 
to re-distribution in source or binary form is strictly prohibited.
This source code is provided ?$)A!cAS IS?!@ and nothing contained in this source code 
shall constitute any express or implied warranty of any kind, including without limitation, 
any warranty of merchantability, fitness for a particular purpose or non-infringement of any patent, 
copyright or other third party intellectual property right. 
No warranty is made, express or implied, regarding the information??s accuracy, 
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
//#include "tcc_ipc.h"
#include "tcc_ipc_os.h"
#include "tcc_ipc_mbox.h"
#include "os_framework_cfg.h"
#include "os_framework.h"
//#include "os_framework_type.h"

#define CTL_CMD_FLAG			0x00000001ul
#define WRITE_CMD_FLAG		0x00000002ul
#define CMD_FLAG			0x00000003ul
#define READ_WAIT_FLAG		0x00000001ul

typedef struct _IPC_ACK_INFO{
	Type_uWord 		uwseqID;
}IPC_ACK_INFO_ts;

typedef struct _IPC_ACK_FLAGS{
	//Type_uWord 	uweventFlagID;
	IPC_ACK_INFO_ts 	_wait_ack[MAX_CMD_TYPE];
}IPC_ACK_FLAGS_ts;

typedef struct _IPC_OS_FLAGS{
	IPC_ACK_FLAGS_ts	ipc_AckFlags;

	//Type_uWord uwipc_ReadWaitFlagID;
	//Type_uWord uwipc_BufOverflowFlagID;

	//Type_uWord uwCommonSemaID;
	//Type_uWord uwMboxSemaID, uwReadSemaID, uwWriteSemaID;
	//Type_uWord uwRbufSemaID, uwWbufSemaID;
}IPC_OS_FLAGS_ts;

static IPC_OS_FLAGS_ts ipc_OSFlags[IPC_CH_MAX];


IPC_INT64S ipc_get_msec(void)
{
	Type_uWord ret;
	ret =  OSAL_GetOSTimeTick();
	return (IPC_INT64S)(ret* TICK_PER_MS);
}

void wvdipc_mdelay(Type_uWord auwdly)
{
	OSAL_TaskDelay(auwdly);
}

void wvdipc_udelay(Type_uWord auwdly)
{
	/* to do*/
}


void wvdipc_common_lock(IPC_CH ch)
{
	/*Acquire Mutex*/
	if(ch == IPC_CH_CA53_NS)
	{
		(void)OS_WaitSemaphore(SEM_ID_MAIN_RECIVER_COMMON, OS_TOUT_FOREVER);
	}
	else if(ch == IPC_CH_CA7_NS)
	{
		(void)OS_WaitSemaphore(SEM_ID_SUB_RECIVER_COMMON, OS_TOUT_FOREVER);
	}
	else
	{
		;
	}

}

void wvdipc_common_unlock(IPC_CH ch)
{
	/*Release Mutex*/
	if(ch == IPC_CH_CA53_NS)
	{
		(void)OS_SignalSemaphore(SEM_ID_MAIN_RECIVER_COMMON);
	}
	else if(ch == IPC_CH_CA7_NS)
	{
		(void)OS_SignalSemaphore(SEM_ID_SUB_RECIVER_COMMON);
	}
	else
	{
		;
	}
	
}
void wvdipc_mbox_lock(IPC_CH ch)
{
	/*Acquire Mutex*/
	if(ch == IPC_CH_CA53_NS)
	{
		(void)OS_WaitSemaphore(SEM_ID_MAIN_RECIVER_MBOX, OS_TOUT_FOREVER);
	}
	else if(ch == IPC_CH_CA7_NS)
	{
		(void)OS_WaitSemaphore(SEM_ID_SUB_RECIVER_MBOX, OS_TOUT_FOREVER);
	}
	else
	{
		;
	}
}

void wvdipc_mbox_unlock(IPC_CH ch)
{
	/*Release Mutex*/
	if(ch == IPC_CH_CA53_NS)
	{
		(void)OS_SignalSemaphore(SEM_ID_MAIN_RECIVER_MBOX);
	}
	else if(ch == IPC_CH_CA7_NS)
	{
		(void)OS_SignalSemaphore(SEM_ID_SUB_RECIVER_MBOX);
	}
	else
	{
		;
	}
}

void wvdipc_write_lock(IPC_CH ch)
{
	/*Acquire Mutex*/
	if(ch == IPC_CH_CA53_NS)
	{
		(void)OS_WaitSemaphore(SEM_ID_MAIN_RECIVER_WRITE, OS_TOUT_FOREVER);
	}
	else if(ch == IPC_CH_CA7_NS)
	{
		(void)OS_WaitSemaphore(SEM_ID_SUB_RECIVER_WRITE, OS_TOUT_FOREVER);
	}
	else
	{
		;
	}
}

void wvdipc_write_unlock(IPC_CH ch)
{
	/*Release Mutex*/
	if(ch == IPC_CH_CA53_NS)
	{
		(void)OS_SignalSemaphore(SEM_ID_MAIN_RECIVER_WRITE);
	}
	else if(ch == IPC_CH_CA7_NS)
	{
		(void)OS_SignalSemaphore(SEM_ID_SUB_RECIVER_WRITE);
	}
	else
	{
		;
	}
}

void wvdipc_read_lock(IPC_CH ch)
{
	/*Acquire Mutex*/
	if(ch == IPC_CH_CA53_NS)
	{
		(void)OS_WaitSemaphore(SEM_ID_MAIN_RECIVER_READ, OS_TOUT_FOREVER);
	}
	else if(ch == IPC_CH_CA7_NS)
	{
		(void)OS_WaitSemaphore(SEM_ID_SUB_RECIVER_READ, OS_TOUT_FOREVER);
	}
	else
	{
		;
	}
}

void wvdipc_read_unlock(IPC_CH ch)
{
	/*Release Mutex*/
	if(ch == IPC_CH_CA53_NS)
	{
		(void)OS_SignalSemaphore(SEM_ID_MAIN_RECIVER_READ);
	}
	else if(ch == IPC_CH_CA7_NS)
	{
		(void)OS_SignalSemaphore(SEM_ID_SUB_RECIVER_READ);
	}
	else
	{
		;
	}
	
}

void wvdipc_rbuffer_lock(IPC_CH ch)
{
	/*Acquire Mutex*/
	if(ch == IPC_CH_CA53_NS)
	{
		(void)OS_WaitSemaphore(SEM_ID_MAIN_RECIVER_RBUF, OS_TOUT_FOREVER);
	}
	else if(ch == IPC_CH_CA7_NS)
	{
		(void)OS_WaitSemaphore(SEM_ID_SUB_RECIVER_RBUF, OS_TOUT_FOREVER);
	}
	else
	{
		;
	}
	
}

void wvdipc_rbuffer_unlock(IPC_CH ch)
{
	/*Release Mutex*/
	if(ch == IPC_CH_CA53_NS)
	{
		(void)OS_SignalSemaphore(SEM_ID_MAIN_RECIVER_RBUF);
		
	}
	else if(ch == IPC_CH_CA7_NS)
	{
		(void)OS_SignalSemaphore(SEM_ID_SUB_RECIVER_RBUF);
	}
	else
	{
		;
	}
	
}

void wvdipc_wbuffer_lock(IPC_CH ch)
{
	/*Acquire Mutex*/
	if(ch == IPC_CH_CA53_NS)
	{
		(void)OS_WaitSemaphore(SEM_ID_MAIN_RECIVER_WBUF, OS_TOUT_FOREVER);
	}
	else if(ch == IPC_CH_CA7_NS)
	{
		(void)OS_WaitSemaphore(SEM_ID_SUB_RECIVER_WBUF, OS_TOUT_FOREVER);
	}
	else
	{
		;
	}
}

void wvdipc_wbuffer_unlock(IPC_CH ch)
{
	/*Release Mutex*/
	if(ch == IPC_CH_CA53_NS)
	{
		(void)OS_SignalSemaphore(SEM_ID_MAIN_RECIVER_WBUF);
	}
	else if(ch == IPC_CH_CA7_NS)
	{
		(void)OS_SignalSemaphore(SEM_ID_SUB_RECIVER_WBUF);
	}
	else
	{
		;
	}
	
}
Type_sWord wswipc_cmd_wait_event_timeout(IPC_CH ch, IpcCmdType cmdType, Type_uWord auwtimeOut)
{
	Type_sWord aswret;
	OS_ErrCode_t aswretFlag;
	Type_uWord	auwwaitFlag = (Type_uWord)0x0;
	OS_FlgPtn_t auwFlagPattern;

	auwFlagPattern = 0;

	if ((ch < IPC_CH_MAX) && (cmdType < MAX_CMD_TYPE)) //warning. comparison is always true. ch and cmdTyp is always bigger than -1
	{
		switch(cmdType)
		{
			case CTL_CMD:
				auwwaitFlag = CTL_CMD_FLAG;
				break;
			case WRITE_CMD:
				auwwaitFlag = WRITE_CMD_FLAG;
				break;
			default:
				break;
		}

		if(ch == IPC_CH_CA53_NS)
		{
			aswretFlag = OS_WaitEventFlag(FLG_ID_MAIN_RECIVER_ACK, CMD_FLAG, OS_FALSE, &auwFlagPattern, auwtimeOut);
			
			if(aswretFlag == OS_ERR_OK)
			{
				if((auwFlagPattern & auwwaitFlag) == auwwaitFlag)
				{
					(void)OS_ClearEventFlag(FLG_ID_MAIN_RECIVER_ACK, ~auwwaitFlag);
					aswret = IPC_SUCCESS;
				}
				else
				{	
					aswret = IPC_ERR_TIMEOUT;
				}
			}
		}
		else if(ch == IPC_CH_CA7_NS)
		{
			aswretFlag = OS_WaitEventFlag(FLG_ID_SUB_RECIVER_ACK, CMD_FLAG, OS_FALSE, &auwFlagPattern, auwtimeOut);
			if(aswretFlag == OS_ERR_OK)
			{
				if((auwFlagPattern & auwwaitFlag) == auwwaitFlag)
				{
					(void)OS_ClearEventFlag(FLG_ID_MAIN_RECIVER_ACK, ~auwwaitFlag);
					aswret = IPC_SUCCESS;
				}
				else
				{	
					aswret = IPC_ERR_TIMEOUT;
				}
			}
		}
		else
		{
			;
		}
	}
	else
	{
		aswret = IPC_ERR_ARGUMENT;
	}
	return aswret;
}

void wvdipc_cmd_wake_preset(IPC_CH ch, IpcCmdType cmdType, Type_uWord auwseqID)
{
	Type_uWord auwwaitFlag = 0x0;
	if ((ch < IPC_CH_MAX) && (cmdType < MAX_CMD_TYPE)) //warning. comparison is always true. ch and cmdTyp is always bigger than -1
	{
		switch(cmdType)
		{
			case CTL_CMD:
				auwwaitFlag = CTL_CMD_FLAG;
				break;
			case WRITE_CMD:
				auwwaitFlag = WRITE_CMD_FLAG;
				break;
			default:
				break;
		}
		if(ch == IPC_CH_CA53_NS)
		{
			(void)OS_ClearEventFlag(FLG_ID_MAIN_RECIVER_ACK, ~auwwaitFlag);
		}
		else if(ch == IPC_CH_CA7_NS)
		{
			(void)OS_ClearEventFlag(FLG_ID_SUB_RECIVER_ACK, ~auwwaitFlag);
		}
		else
		{
			;
		}

		ipc_OSFlags[ch].ipc_AckFlags._wait_ack[cmdType].uwseqID = auwseqID;
	}
}

void wvdipc_cmd_wake_up(IPC_CH ch, IpcCmdType cmdType, Type_uWord auwseqID)
{
	Type_uWord auwwaitFlag = (Type_uWord)0x0;
	
	if ((ch < IPC_CH_MAX) && (cmdType < MAX_CMD_TYPE)) //warning. comparison is always true. ch and cmdTyp is always bigger than -1
	{
		if(ipc_OSFlags[ch].ipc_AckFlags._wait_ack[cmdType].uwseqID == auwseqID)
		{
			switch(cmdType)
			{
				case CTL_CMD:
					auwwaitFlag = CTL_CMD_FLAG;
					break;
				case WRITE_CMD:
					auwwaitFlag = WRITE_CMD_FLAG;
					break;
				default:
					break;
			}
			if(ch == IPC_CH_CA53_NS)
			{
				(void)OS_SetEventFlag(FLG_ID_MAIN_RECIVER_ACK, auwwaitFlag);
			}
			else if(ch == IPC_CH_CA7_NS)
			{
				(void)OS_SetEventFlag(FLG_ID_SUB_RECIVER_ACK, auwwaitFlag);
			}
			else
			{
				;
			}
		}
		else
		{
			;/* Receive unknown ACK */
		}
	}

}


Type_sWord wswipc_request_irq(Type_uWord auwirq, BSP_INT_FNCT_PTR func)
{
	/* register mbox rx interrupt handler	*/
	
	(void)BSP_IntVectSet(auwirq, 0x3, INT_TYPE_LEVEL_LOW, func, 0);
	BSP_IntSrcEn(auwirq);
	return 0;
}

Type_sWord wswipc_free_irq(Type_uWord auwirq)
{
	BSP_IntSrcDis(auwirq);
	return  0;
}

void wvdipc_os_resouce_release(IPC_CH ch)
{
	//nvdipc_common_lock_delete(ch);
	//nvdipc_delete_mutex(ch);
	//nvdipc_cmd_event_delete(ch);
	//nvdipc_read_event_delete(ch);
	//wvdipc_buffer_overflow_wake_up(ch, READ_BUF_CLOSE_BIT);
	//nvdipc_buffer_overflow_event_delete(ch);
}
