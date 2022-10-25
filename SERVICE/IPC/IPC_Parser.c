/****************************************************************************
 *   FileName    : IPC_Parser.c
 *   Description : IPC Management
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
#include <app_cfg.h>
#include <types.h>
#include <osal.h>
#include "HAL_API.h"
#include "HAL_IPC.h"
#include "HAL_devices.h"
#include "IPC_Parser.h"
#include "tcc_ipc_protocol.h"
#include "bsp.h"
#include "debug.h"
#include "rpc_os.h"
#include "rpc_if.h"

#ifdef IPC_CA53_NS
void TSK_MAIN_PARSER(void)
{
	Type_sWord aswipc_ret;

	while (1u) {
		if(wubRPCStandbyFlag == RPC_STANDBYOUT)
		{
			aswipc_ret = wswipc_receive_packet(IPC_SVC_CH_MAIN);
			//printf("aswipc_ret:%d [%s]: <%s:%d>\n",aswipc_ret, __func__, __FILE__, __LINE__);
			if ( aswipc_ret > DUMMY_DATA_SIZE) 
			{
				//printf("aswipc_ret:%d [%s]: <%s:%d>\n",aswipc_ret, __func__, __FILE__, __LINE__);
				(void) wswipc_packet_parser(IPC_SVC_CH_MAIN);
				(void)wswipc_packet_parser1(IPC_SVC_CH_MAIN);
			}
			else
			{
				OSAL_TaskDelay(5);
			}
			
		}
		else
		{
			OSAL_TaskDelay(5);
		}
		
	}
	
}
#endif
#ifdef IPC_CA7_NS
void TSK_SUB_PARSER(void)
{
	Type_sWord aswipc_ret;

	while (1u) {
		if(wubRPCStandbyFlag == RPC_STANDBYOUT)
		{
			aswipc_ret = wswipc_receive_packet(IPC_SVC_CH_SUB);
			if ( aswipc_ret > DUMMY_DATA_SIZE) 
			{
				(void) wswipc_packet_parser(IPC_SVC_CH_SUB);
				(void)wswipc_packet_parser1(IPC_SVC_CH_SUB);
			}
			else
			{
				OSAL_TaskDelay(5);
			}
			
		}
		else
		{
			OSAL_TaskDelay(5);
		}
		
	}
	
}
#endif

void wvdIPCOpen(void)
{
	Type_sWord aswindex;
	Type_uByte aubisHalHandle = false;
#if defined(TCC803x)
#ifdef IPC_CA53_S
	//not supported yet
#endif
#ifdef IPC_CA53_NS
	ipc_ch_info[IPC_SVC_CH_MAIN].halHandle = HAL_Open(HAL_IPC_CA53, IPC_MODE_0);
	
#endif
#ifdef IPC_CA7_S
	//not supported yet
#endif
#ifdef IPC_CA7_NS
	ipc_ch_info[IPC_SVC_CH_SUB].halHandle = HAL_Open(HAL_IPC_CA7, IPC_MODE_0);

#endif
#ifdef IPC_CM4_NS
	//not supported yet
#endif
#endif

	for(aswindex = INDEX_INIT; aswindex < IPC_SVC_CH_MAX; aswindex++)
	{
		if(ipc_ch_info[aswindex].halHandle != NULL)
		{
			aubisHalHandle = true;
		}
		
	}
	if (aubisHalHandle != true)
	{
		printf("IPC OPEN FAIL[%s]: [%d]<%s:%d>\n", __func__, __FILE__, __LINE__);
	}
	else
	{
		;
	}	
}

