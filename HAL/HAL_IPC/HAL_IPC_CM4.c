/****************************************************************************
 *   FileName    : HAL_IPC_CM4.c
 *   Description : HAL IPC device using mailbox
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
/******************************************************************************
*  yyyy/mm/dd     ver            descriptions                Author
*	---------	--------   ---------------       -----------------
*    2018/08/23     0.1            created                    Jihoon Lim
*******************************************************************************/
#include <types.h>
#include <osal.h>
#include "tcc_ipc_typedef.h"
#include "tcc_ipc.h"
#include "HAL_API.h"
#include "HAL_devices.h"
#include "HAL_IPC.h"
#include "HAL_IPC_CM4.h"
#include "bsp_int.h"
#include "tcc_ipc_mbox.h"
#include "tcc_ipc_buffer.h"
#include "tcc_ipc_os.h"
#include "tcc_ipc_cmd.h"
#include "tcc_ipc_ctl.h"
#include "debug.h"

#ifdef USE_HAL_IPC_CM4

//#define IPCCM4_DEBUG_ENABLE
#ifdef IPCCM4_DEBUG_ENABLE
#define IPCCM4_DBG(fmt, args...)   printf(fmt, ## args)
#else
#define IPCCM4_DBG(fmt, args...)   do {} while(0)
#endif

static Hal_IPCCM4Type HalIPCCM4_Main;
//IpcHandler 	gIPCHandler;

int32 IPCCM4_Open(uint32 OPTIONS)
{
	int32 lRet;
	int32 ret;

	OSAL_SR_ALLOC();
	
	OSAL_CRITICAL_ENTER();
	gIPCHandler[IPC_CH_CM4_NS].ipcOption.isAck = (uint32)((OPTIONS & IPC_O_ACK_MASK) >> 8);
	
	if(HalIPCCM4_Main.gHALIPCCM4IsOpen == 0)
	{
		lRet = HAL_OK;
	}
	else
	{
		lRet = HAL_ERR_OPENED_ALREADY;
	}
	OSAL_CRITICAL_EXIT();

	if(lRet == HAL_OK)
	{
		wvdipc_struct_init(IPC_CH_CM4_NS, &gIPCHandler[IPC_CH_CM4_NS]);

		// open driver
		ret = wswipc_initialize(IPC_CH_CM4_NS, INT_LEVEL_8);
		if(ret == IPC_SUCCESS)
		{
			HalIPCCM4_Main.gHALIPCCM4Status = HAL_DEVICE_ST_OPENED;
			
			OSAL_CRITICAL_ENTER();
			HalIPCCM4_Main.gHALIPCCM4CallCount = 0;
			HalIPCCM4_Main.gHALIPCCM4IsOpen = 1;
			OSAL_CRITICAL_EXIT();
		}
		else
		{
			lRet = HAL_ERR_HAL_DEV;
		}
	}

	return lRet;
}

int32 IPCCM4_Read(uint8 *buff, uint32 size)
{
	int32 lRet;
	uint32 isOpen;
	
	lRet = HAL_OK;

	if (HalIPCCM4_Main.gHALIPCCM4Status != HAL_DEVICE_ST_OPENED)
	{
		lRet = HAL_ERR_NOT_OPENED;
	}
	else
	{
		if (buff !=0)
		{
			OSAL_SR_ALLOC();

			OSAL_CRITICAL_ENTER();
			isOpen = HalIPCCM4_Main.gHALIPCCM4IsOpen;
			HalIPCCM4_Main.gHALIPCCM4CallCount++;
			OSAL_CRITICAL_EXIT();

			if(isOpen == 1)
			{
				lRet = wswipc_read(IPC_CH_CM4_NS, buff, size);
			}
			else
			{
				lRet = HAL_ERR_NOT_INITIALIZED;
			}

			OSAL_CRITICAL_ENTER();
			HalIPCCM4_Main.gHALIPCCM4CallCount--;
			OSAL_CRITICAL_EXIT();
		}
		else
		{
			lRet = HAL_ERR_BAD_PARAMETER;
		}
	}

	return lRet;
}

int32 IPCCM4_Write(const uint8 *buff, uint32 size)
{
	int32 lRet;
	uint32 isOpen;
	
	lRet = HAL_OK;

	if (HalIPCCM4_Main.gHALIPCCM4Status != HAL_DEVICE_ST_OPENED)
	{
		lRet = HAL_ERR_NOT_OPENED;
	}
	else
	{
		OSAL_SR_ALLOC();

		OSAL_CRITICAL_ENTER();
		isOpen = HalIPCCM4_Main.gHALIPCCM4IsOpen;
		HalIPCCM4_Main.gHALIPCCM4CallCount++;
		OSAL_CRITICAL_EXIT();

		if(isOpen == 1)
		{
			lRet = HAL_ERR_NOT_SUPPORTED;
		}
		else
		{
			lRet = HAL_ERR_NOT_INITIALIZED;
		}

		OSAL_CRITICAL_ENTER();
		HalIPCCM4_Main.gHALIPCCM4CallCount--;
		OSAL_CRITICAL_EXIT();
	}

	return lRet;
}

int32 IPCCM4_Close(void)
{
	int32 lRet;
	uint32 isOpen;
	uint32 callCount;

	lRet = HAL_OK;

	if (HalIPCCM4_Main.gHALIPCCM4Status != HAL_DEVICE_ST_OPENED)
	{
		lRet = HAL_ERR_NOT_OPENED;
	}
	else
	{
		OSAL_SR_ALLOC();
		
		HalIPCCM4_Main.gHALIPCCM4Status = HAL_DEVICE_ST_NO_OPEN;
		
		//close driver
		OSAL_CRITICAL_ENTER();
		isOpen = HalIPCCM4_Main.gHALIPCCM4IsOpen;
		callCount = HalIPCCM4_Main.gHALIPCCM4CallCount;
		OSAL_CRITICAL_EXIT();

		if((isOpen == 1 ) &&(callCount ==0))
		{
			wvdipc_release(IPC_CH_CM4_NS);
			OSAL_CRITICAL_ENTER();
			HalIPCCM4_Main.gHALIPCCM4IsOpen = 0;
			OSAL_CRITICAL_EXIT();
		}
		else if (callCount !=0)
		{
			lRet = HAL_ERR_DEVICE_BUSY;
		}
		else
		{
			lRet = HAL_ERR_HAL_DEV;
		}
	}

	return lRet;
}

int32 IPCCM4_Ioctl(uint32 OPT,  void* PAR1, void* PAR2, void* PAR3, void* PAR4) //QAC
{
	int32 lRet;
	uint32 isOpen;
	int32 ret;
	
	lRet = HAL_OK;

	if (HalIPCCM4_Main.gHALIPCCM4Status != HAL_DEVICE_ST_OPENED)
	{
		lRet = HAL_ERR_NOT_OPENED;
	}
	else
	{
		OSAL_SR_ALLOC();

		OSAL_CRITICAL_ENTER();
		isOpen = HalIPCCM4_Main.gHALIPCCM4IsOpen;
		HalIPCCM4_Main.gHALIPCCM4CallCount++;
		OSAL_CRITICAL_EXIT();

		if(isOpen == 1)
		{
			switch (OPT)
			{
				case IOCTL_IPC_WRITE:
					{
					if ((PAR1 != NULL)) {
						const uint32 *cmd = (const uint32 *)PAR1;
						const uint8 *buff = (const uint8 *)PAR2;
						const uint32 size = *((const uint32 *)PAR3);

						ret = wswipcm4_write((Type_uWord*)cmd, buff, size);
						if (ret < IPC_SUCCESS) {
							lRet = HAL_ERR_HAL_DEV;
						}
						}
						else
						{
							lRet = HAL_ERR_BAD_PARAMETER;
						}
					}
					break;
				case IOCTL_IPC_READ:
					break;
				default:
					{
						lRet = HAL_ERR_BAD_PARAMETER;
					}
					break;
			}
					
		}
		else
		{
			lRet = HAL_ERR_NOT_INITIALIZED;
		}

		OSAL_CRITICAL_ENTER();
		HalIPCCM4_Main.gHALIPCCM4CallCount--;
		OSAL_CRITICAL_EXIT();
	}

	return lRet;
}
#endif
