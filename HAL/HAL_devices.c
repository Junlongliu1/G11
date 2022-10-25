/****************************************************************************
 *   FileName    : HAL_devices.c
 *   Description : Register HAL devices to HAL object list.
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
*    2016/10/31     0.1            created                      hskim
*******************************************************************************/
#include <types.h>
#include <osal.h>
#include <HAL_API.h>
#include <HAL_devices.h>

#include "HAL_TIMER/HAL_Timer.h"
#include "HAL_CONSOLE/HAL_Console.h"
#include "HAL_ADC_KEY/HAL_ADCKey.h"
#include "HAL_GPIO_KEY/HAL_GPIOKey.h"
#include "HAL_POWER_MANAGER/HAL_PowerManager.h"
#include "HAL_IPC/HAL_IPC_CA53.h"
#include "HAL_IPC/HAL_IPC_CA7.h"
#include "HAL_IPC/HAL_IPC_CM4.h"
#include "HAL_I2C_KEY/HAL_I2CKey.h"
#include "HAL_LED/HAL_Led.h"
#include "HAL_SNR/HAL_Snr.h"
#include "HAL_EEP/HAL_Eep.h"
#include "HAL_LineTool.h"
#include "HAL_KLINEDRV/HAL_KlineDrv.h"



	static const HAL_DEV_FUNCS HAL_DEVICE_FUNCTIONS_TABLE[HAL_MAX_ID] =
	{
	/* OPEN,				READ,				WRITE,			CLOSE,					IOCTL*/
	{Timer_Open,			0,					0,				Timer_Close,			Timer_Ioctl,			0},
	{PowerManager_Open, 	PowerManager_Read,	0,				PowerManager_Close, 	PowerManager_Ioctl,		PowerManager_CallBack},
	{ADCKey_Open,			0,					0,				ADCKey_Close,			ADCKey_Ioctl,			0},
	{Console_Open,			Console_Read,		Console_Write,	Console_Close,			Console_Ioctl,			0},
	{IPCCA53_Open,			IPCCA53_Read,		IPCCA53_Write,	IPCCA53_Close,			IPCCA53_Ioctl,			0},
	{IPCCA7_Open,			IPCCA7_Read,		IPCCA7_Write,	IPCCA7_Close,			IPCCA7_Ioctl,			0},
	{IPCCM4_Open,			IPCCM4_Read,		IPCCM4_Write,	IPCCM4_Close,			IPCCM4_Ioctl,			0},
	{I2CKey_Open,			0,					0,				I2CKey_Close,			I2CKey_Ioctl,			0},
	{GPIOKey_Open,			0,					0,				GPIOKey_Close,			GPIOKey_Ioctl,			0},
	{LED_Open,				0,					0,				LED_Close,				LED_Ioctl,				0},
	{SNR_Open,				0,					0,				SNR_Close,				SNR_Ioctl,				0},
	{I2CEEPRom_Open,		0,					0,				I2CEEPRom_Close,		I2CEEPRom_Ioctl,		0},
	{LineTool_Open,			0,					LineTool_Write,	LineTool_Close,			0,						LineTool_CallBACK},
	{KlineDrv_Open,			0,					KlineDrv_Write,	KlineDrv_Close,			0,						KlineDrv_CallBACK},
	/*HAL_devices.h HAL_DEVICE_ID_Type*/
};

void	HAL_InitializeDevices(void)
		{
	uint32 i;
	
	for (i = 0; i <HAL_MAX_ID; i++) {
		HAL_InitHALObject(i, &HAL_DEVICE_FUNCTIONS_TABLE[i]);
	}
}

