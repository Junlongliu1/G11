/****************************************************************************
 *   FileName    : HAL_IPC.c
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
*    2018/06/26     0.1            created                      bskim
*******************************************************************************/
#include <types.h>
#include <osal.h>
#include "tcc_ipc_typedef.h"
#include "tcc_ipc.h"
#include "HAL_API.h"
#include "HAL_devices.h"
#include "HAL_IPC.h"
#include "HAL_IPC_CA7.h"
#include "tcc_ipc_buffer.h"
#include "tcc_ipc_os.h"
#include "bsp_int.h"

#include "tcc_ipc_mbox.h"
#include "tcc_ipc_cmd.h"
#include "tcc_ipc_ctl.h"
#include "debug.h"

#ifdef USE_HAL_IPC_CA7

//#define IPCCA7_DEBUG
#ifdef IPCCA7_DEBUG
#define IPCCA7_DBG(fmt, args...)   printf(fmt, ## args)
#else
#define IPCCA7_DBG(fmt, args...)   do {} while(0)
#endif

static Hal_IPCCA7Type HalIPCCA7_Main;

int32 IPCCA7_Open(uint32 OPTIONS)
{
	Type_sWord aswlRet;
	Type_sWord aswret;
	
	OSAL_SR_ALLOC();
	
	OSAL_CRITICAL_ENTER();
	gIPCHandler[IPC_CH_CA7_NS].ipcOption.isAck = (Type_uWord)((OPTIONS & IPC_O_ACK_MASK) >> 8);
	
	if(HalIPCCA7_Main.uwHALIPCCA7IsOpen == 0)
	{
		aswlRet = HAL_OK;
	}
	else
	{
		aswlRet = HAL_ERR_OPENED_ALREADY;
	}
	OSAL_CRITICAL_EXIT();

	if(aswlRet == HAL_OK)
	{
		wvdipc_struct_init(IPC_CH_CA7_NS, &gIPCHandler[IPC_CH_CA7_NS]);

		// open driver
		aswret = wswipc_initialize(IPC_CH_CA7_NS, INT_LEVEL_8);
		if(aswret == IPC_SUCCESS)
		{
			HalIPCCA7_Main.uwHALIPCCA7Status = HAL_DEVICE_ST_OPENED;
			
			OSAL_CRITICAL_ENTER();
			HalIPCCA7_Main.uwHALIPCCA7CallCount = 0;
			HalIPCCA7_Main.uwHALIPCCA7IsOpen = 1;
			OSAL_CRITICAL_EXIT();
		}
		else
		{
			aswlRet = HAL_ERR_HAL_DEV;
		}
	}

	return aswlRet;
}

int32 IPCCA7_Read(uint8 *buff, uint32 size)
{
	Type_sWord aswlRet;
	Type_uWord auwisOpen;
	
	aswlRet = HAL_OK;

	if (HalIPCCA7_Main.uwHALIPCCA7Status != HAL_DEVICE_ST_OPENED)
	{
		aswlRet = HAL_ERR_NOT_OPENED;
	}
	else
	{
		if (buff !=0)
		{
			OSAL_SR_ALLOC();

			OSAL_CRITICAL_ENTER();
			auwisOpen = HalIPCCA7_Main.uwHALIPCCA7IsOpen;
			HalIPCCA7_Main.uwHALIPCCA7CallCount++;
			OSAL_CRITICAL_EXIT();

			if(auwisOpen == 1)
			{
				aswlRet = wswipc_read(IPC_CH_CA7_NS, buff, size);
			}
			else
			{
				aswlRet = HAL_ERR_NOT_INITIALIZED;
			}

			OSAL_CRITICAL_ENTER();
			HalIPCCA7_Main.uwHALIPCCA7CallCount--;
			OSAL_CRITICAL_EXIT();
		}
		else
		{
			aswlRet = HAL_ERR_BAD_PARAMETER;
		}
	}

	return aswlRet;
}

int32 IPCCA7_Write(const uint8 *buff, uint32 size)
{
	Type_sWord aswlRet;
	Type_uWord auwisOpen;
	
	aswlRet = HAL_OK;

	if (HalIPCCA7_Main.uwHALIPCCA7Status != HAL_DEVICE_ST_OPENED)
	{
		aswlRet = HAL_ERR_NOT_OPENED;
	}
	else
	{
		OSAL_SR_ALLOC();

		OSAL_CRITICAL_ENTER();
		auwisOpen = HalIPCCA7_Main.uwHALIPCCA7IsOpen;
		HalIPCCA7_Main.uwHALIPCCA7CallCount++;
		OSAL_CRITICAL_EXIT();

		if(auwisOpen == 1)
		{
			aswlRet = HAL_ERR_NOT_SUPPORTED;
		}
		else
		{
			aswlRet = HAL_ERR_NOT_INITIALIZED;
		}

		OSAL_CRITICAL_ENTER();
		HalIPCCA7_Main.uwHALIPCCA7CallCount--;
		OSAL_CRITICAL_EXIT();
	}

	return aswlRet;
}

int32 IPCCA7_Close(void)
{
	Type_sWord aswlRet;
	Type_uWord auwisOpen;
	Type_uWord auwcallCount;

	aswlRet = HAL_OK;

	if (HalIPCCA7_Main.uwHALIPCCA7Status != HAL_DEVICE_ST_OPENED)
	{
		aswlRet = HAL_ERR_NOT_OPENED;
	}
	else
	{
		OSAL_SR_ALLOC();
		
		HalIPCCA7_Main.uwHALIPCCA7Status = HAL_DEVICE_ST_NO_OPEN;
		
		//close driver
		OSAL_CRITICAL_ENTER();
		auwisOpen = HalIPCCA7_Main.uwHALIPCCA7IsOpen;
		auwcallCount = HalIPCCA7_Main.uwHALIPCCA7CallCount;
		OSAL_CRITICAL_EXIT();

		if((auwisOpen == 1 ) &&(auwcallCount ==0))
		{
			wvdipc_release(IPC_CH_CA7_NS);
			OSAL_CRITICAL_ENTER();
			HalIPCCA7_Main.uwHALIPCCA7IsOpen = 0;
			OSAL_CRITICAL_EXIT();
		}
		else if (auwcallCount !=0)
		{
			aswlRet = HAL_ERR_DEVICE_BUSY;
		}
		else
		{
			aswlRet = HAL_ERR_HAL_DEV;
		}
	}

	return aswlRet;
}

int32 IPCCA7_Ioctl(uint32 OPT, void* PAR1, void* PAR2, void* PAR3, void* PAR4) //QAC
{
	Type_sWord aswlRet;
	Type_uWord auwisOpen;
	Type_sWord aswret;
	
	aswlRet = HAL_OK;

	if (HalIPCCA7_Main.uwHALIPCCA7Status != HAL_DEVICE_ST_OPENED)
	{
		aswlRet = HAL_ERR_NOT_OPENED;
	}
	else
	{
		OSAL_SR_ALLOC();

		OSAL_CRITICAL_ENTER();
		auwisOpen = HalIPCCA7_Main.uwHALIPCCA7IsOpen;
		HalIPCCA7_Main.uwHALIPCCA7CallCount++;
		OSAL_CRITICAL_EXIT();

		if(auwisOpen == 1)
		{
			switch (OPT)
			{
				case IOCTL_IPC_WRITE:
					{
						if((PAR1 != NULL))
						{
							const Type_uWord *auwcmd_p = (const Type_uWord *)PAR1;
							const Type_uByte *aubbuff_p = (const Type_uByte *)PAR2;
							const Type_uWord auwsize = *((const Type_uWord *)PAR3);

							aswret = wswipca7_write(auwcmd_p, aubbuff_p, auwsize);
							if(aswret < IPC_SUCCESS) {
								aswlRet = HAL_ERR_HAL_DEV;
							}
						}
						else
						{
							aswlRet = HAL_ERR_BAD_PARAMETER;
						}
					}
					break;
				case IOCTL_IPC_READ:
					break;
				case IOCTL_IPC_SET_PARAM:
					break;
				case IOCTL_IPC_GET_PARAM:
					break;
				case IOCTL_IPC_PING_TEST:
					{
						ipc_ping_info pingInfo;

						pingInfo.pingResult = IPC_PING_ERR_INIT;
						pingInfo.sendByte =0;
						pingInfo.responseTime =0;
						pingInfo.compareResult = 0;	//codesonar
						(void)wswipc_ping_test(IPC_CH_CA7_NS, &pingInfo);
						
						if(pingInfo.pingResult == IPC_SUCCESS)
						{
							IPCCA7_DBG("IPC CA7 result value : %d\n", pingInfo.pingResult);
							IPCCA7_DBG("IPC CA7 response time : %d\n", pingInfo.responseTime);
						}
						else
						{
							aswlRet = HAL_ERR_HAL_DEV;
						}
					}
					break;
				case IOCTL_IPC_ISREADY:
					{
						if(PAR1 != NULL)
						{
							Type_uWord * auwisReady_p = (Type_uWord *)PAR1; //QAC

							if(gIPCHandler[IPC_CH_CA7_NS].ipcStatus < IPC_READY)
							{
								*auwisReady_p = 0;
							}
							else
							{
								*auwisReady_p = 1;
							}
						}
						else
						{
							aswlRet = HAL_ERR_BAD_PARAMETER;
						}
					}
					break;

				default:
					{
						aswlRet = HAL_ERR_BAD_PARAMETER;
					}
					break;
			}
		}
		else
		{
			aswlRet = HAL_ERR_NOT_INITIALIZED;
		}

		OSAL_CRITICAL_ENTER();
		HalIPCCA7_Main.uwHALIPCCA7CallCount--;
		OSAL_CRITICAL_EXIT();
	}

	return aswlRet;
}

void wvdPCCA7_MainInit(void)
{
	HalIPCCA7_Main.uwHALIPCCA7CallCount = VALUE_0;
	HalIPCCA7_Main.uwHALIPCCA7IsOpen = IPC_NO;
	HalIPCCA7_Main.uwHALIPCCA7Status = HAL_DEVICE_ST_NO_OPEN;
}

#endif
