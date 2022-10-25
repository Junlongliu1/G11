/****************************************************************************
 *   FileName    : HAL_API.c
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

/******************************************************************************
*  yyyy/mm/dd     ver            descriptions                Author
*	---------	--------   ---------------       -----------------
*    2011/03/23     0.1            created                      Jim
*******************************************************************************/
#include <types.h>
#include <osal.h>
#include "HAL_API.h"
#include "HAL_devices.h"

static pHAL_OBJ GetHALObject(HAL_HANDLE HALHandle);
static int32 HAL_CreateSemaphore(uint32 *semaphore);
static int32 HAL_ObtainSemaphore(uint32 semaphore, uint32 suspend);
static int32 HAL_ReleaseSemaphore(uint32 semaphore);


static HAL_OBJ gHAL_Devices[HAL_MAX_ID];

static pHAL_OBJ GetHALObject(HAL_HANDLE HALHandle) 
{
	pHAL_OBJ ret=0;

	if (HALHandle != 0) {
		ret=(pHAL_OBJ)HALHandle;	// will be implemented more
	}
	return ret;
}

static int32 HAL_CreateSemaphore(uint32 *semaphore)
{
	int32 err;
	int32 retval=HAL_OK;

	if ( semaphore != 0)
	{
		err = OSAL_SemaCreate(semaphore, (const uint8 *)"HAL Sema", 1, 0);
		if ( err != OSAL_ERR_NONE)
		{
			retval = HAL_ERR_OS_API;
		}
	}
	return retval;
}

static int32 HAL_ObtainSemaphore(uint32 semaphore, uint32 suspend)
{
	int32 retval=HAL_OK;

	retval = OSAL_SemaObtain(semaphore, OSAL_NO_TIMEOUT, suspend);
	if ( retval != OSAL_ERR_NONE)
	{
		retval = HAL_ERR_OS_API;
	}
	return retval;
}


static int32 HAL_ReleaseSemaphore(uint32 semaphore)
{
	int32 retval=HAL_OK;

	/*Release Semaphore*/
	retval = OSAL_SemaRelease(semaphore);
	if ( retval != OSAL_ERR_NONE)
	{
		retval = HAL_ERR_OS_API;
	}
	return retval;
}

HAL_HANDLE HAL_Open(HAL_ID handleID, uint32 option)
{
	int32 lHALRet = HAL_OK;
	HAL_OBJ * pHalObj;
	HAL_DEV_FUNCS * funcs;

	pHalObj = &gHAL_Devices[handleID];
	if ( pHalObj != 0)
	{
		if (pHalObj->mHAL_ST == HAL_OBJST_INITIALIZED)
		{
			lHALRet = HAL_CreateSemaphore(&pHalObj->mSemaID);
			if (lHALRet == HAL_OK)
			{
				funcs = pHalObj->mIOFuncs;
			 	if (funcs->fpOpen != 0)
			 	{
					lHALRet = funcs->fpOpen(option);
			 		pHalObj->mHAL_ST = HAL_OBJST_ENABLE;
			 	}
				else
				{
					lHALRet =HAL_ERR_NOT_OPENED;
				}
			}
		}
		else
		{
			lHALRet =HAL_ERR_NOT_INITIALIZED;
		}
	}
	else
	{
		lHALRet = HAL_ERR_BAD_PARAMETER;
	}
	
	if ( lHALRet!= HAL_OK) {
		pHalObj = 0;
	}
	return (HAL_HANDLE)pHalObj;
}

int32 HAL_Write(HAL_HANDLE HALHandle, const uint8 *pBuff, uint32 size)
{
	int32 lHALRet = HAL_OK;
	HAL_OBJ *pHalObj;
	HAL_DEV_FUNCS * funcs;

	pHalObj = GetHALObject(HALHandle);
	if ( pHalObj != 0)
	{
		if (pHalObj->mHAL_ST == HAL_OBJST_ENABLE)
		{
			funcs= (HAL_DEV_FUNCS *)pHalObj->mIOFuncs;
			if (funcs->fpWrite !=0 )
			{
				lHALRet = HAL_ObtainSemaphore(pHalObj->mSemaID, 0);
				if (lHALRet == HAL_OK)
				{
					lHALRet = funcs->fpWrite(pBuff,size);
					(void) HAL_ReleaseSemaphore(pHalObj->mSemaID);
				}
				//(void) HAL_ReleaseSemaphore(pHalObj->mSemaID);
			}
			else
			{
				lHALRet = HAL_ERR_NOT_SUPPORTED;
			}
		}
		else
		{
			lHALRet = HAL_ERR_NOT_OPENED;
		}
	}
	else
	{
		lHALRet = HAL_ERR_BAD_PARAMETER;
	}
	
	return lHALRet;
}

int32 HAL_Read(HAL_HANDLE HALHandle, uint8 *pBuff, uint32 size)
{
	int32 lHALRet = HAL_OK;
	HAL_OBJ *pHalObj;
	HAL_DEV_FUNCS * funcs;

	pHalObj = GetHALObject(HALHandle);
	if ( pHalObj != 0)
	{
		if (pHalObj->mHAL_ST == HAL_OBJST_ENABLE)
		{
			funcs= (HAL_DEV_FUNCS *)pHalObj->mIOFuncs;
			if (funcs->fpRead !=0 )
			{
				lHALRet = HAL_ObtainSemaphore(pHalObj->mSemaID, 0);
				if ( lHALRet == HAL_OK)
				{
					lHALRet = funcs->fpRead(pBuff,size);
					(void) HAL_ReleaseSemaphore(pHalObj->mSemaID);
				}
				//(void) HAL_ReleaseSemaphore(pHalObj->mSemaID);
			}
			else
			{
				lHALRet = HAL_ERR_NOT_SUPPORTED;
			}
		}
		else
		{
			lHALRet = HAL_ERR_NOT_OPENED;
		}
	}
	else
	{
		lHALRet = HAL_ERR_BAD_PARAMETER;
	}
	
	return lHALRet;
}

int32 HAL_Close(HAL_HANDLE HALHandle)
{
	int32 lHALRet = HAL_OK;
	HAL_OBJ *pHalObj;
	HAL_DEV_FUNCS * funcs;

	pHalObj = GetHALObject(HALHandle);
	if ( pHalObj != 0)
	{
		if (pHalObj->mHAL_ST == HAL_OBJST_ENABLE)
		{
			funcs= (HAL_DEV_FUNCS *)pHalObj->mIOFuncs;
			if (funcs->fpClose !=0 )
			{
				lHALRet = HAL_ObtainSemaphore(pHalObj->mSemaID, 0);
				if ( lHALRet == HAL_OK)
				{
					lHALRet = funcs->fpClose();
					pHalObj->mHAL_ST = HAL_OBJST_DISABLE;
					(void)HAL_ReleaseSemaphore(pHalObj->mSemaID);
				}
				//lHALRet = HAL_ReleaseSemaphore(pHalObj->mSemaID); // CS: Unused Value 
			}
			else
			{
				lHALRet = HAL_ERR_NOT_SUPPORTED;
			}
		}
		else
		{
			lHALRet = HAL_ERR_NOT_OPENED;
		}
	}
	else
	{
		lHALRet = HAL_ERR_BAD_PARAMETER;
	}
	
	return lHALRet;

}

int32 HAL_Ioctl(HAL_HANDLE HALHandle, uint32 ulCommand, void* param1, void* param2, void* param3, void* param4)
{
	int32 lHALRet = HAL_OK;
	HAL_OBJ *pHalObj;
	HAL_DEV_FUNCS * funcs;

	pHalObj = GetHALObject(HALHandle);
	if ( pHalObj != 0)
	{
		if (pHalObj->mHAL_ST == HAL_OBJST_ENABLE)
		{
			funcs= (HAL_DEV_FUNCS *)pHalObj->mIOFuncs;
			if (funcs->fpIoctl !=0 )
			{
				lHALRet= HAL_ObtainSemaphore(pHalObj->mSemaID, 0);
				if ( lHALRet == HAL_OK)
				{
					lHALRet = funcs->fpIoctl(ulCommand, param1, param2, param3, param4);
					(void)HAL_ReleaseSemaphore(pHalObj->mSemaID);
				}
				//lHALRet = HAL_ReleaseSemaphore(pHalObj->mSemaID); // CS: Unused Value 
			}
			else
			{
				lHALRet = HAL_ERR_NOT_SUPPORTED;
			}
		}
		else
		{
			lHALRet = HAL_ERR_NOT_OPENED;
		}
	}
	else
	{
		lHALRet = HAL_ERR_BAD_PARAMETER;
	}
	return lHALRet;

}

int32 HAL_SetCallbackFunction(HAL_HANDLE HALHandle, pfCallback halCBFunc)
{
#if 0
	int32 lHALRet;
	lHALRet = HAL_ERR_NOT_SUPPORTED;
	return lHALRet;
#endif
#if 1
	int32 lHALRet = HAL_OK;
	HAL_OBJ *pHalObj;
	HAL_DEV_FUNCS * funcs;

	pHalObj = GetHALObject(HALHandle);
	if ( pHalObj != 0)
	{
		if (pHalObj->mHAL_ST == HAL_OBJST_ENABLE)
		{
			funcs= (HAL_DEV_FUNCS *)pHalObj->mIOFuncs;
			if (funcs->fpCall !=0 )
			{
				lHALRet= HAL_ObtainSemaphore(pHalObj->mSemaID, 0);
				if ( lHALRet == HAL_OK)
				{
					lHALRet = funcs->fpCall(halCBFunc);
					(void)HAL_ReleaseSemaphore(pHalObj->mSemaID);
				}
			}
			else
			{
				lHALRet = HAL_ERR_NOT_SUPPORTED;
			}
		}
		else
		{
			lHALRet = HAL_ERR_NOT_OPENED;
		}
	}
	else
	{
		lHALRet = HAL_ERR_BAD_PARAMETER;
	}
	return lHALRet;
	#endif
}

void HAL_InitHALObject(uint32 deviceID, const HAL_DEV_FUNCS * pIOFuncs)
{
	if ( (deviceID <HAL_MAX_ID) && (pIOFuncs != 0))
	{
		gHAL_Devices[deviceID].mHAL_ID = deviceID;
		gHAL_Devices[deviceID].mHAL_ST = HAL_OBJST_INITIALIZED;
		gHAL_Devices[deviceID].mSemaID = 0xFFFFFFFF;
		OSAL_MemCopy((void *)&(gHAL_Devices[deviceID].mIOFuncs), (const void *)&pIOFuncs, 4);
	}
}

