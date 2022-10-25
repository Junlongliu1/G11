/****************************************************************************
 *   FileName    : HAL_devices.h
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

#ifndef __HAL_DEVICES_H__
#define __HAL_DEVICES_H__

#define USE_HAL_POWER_MANAGER
#define USE_HAL_ADC_KEY
#define USE_HAL_GPIO_KEY
#define USE_HAL_CONSOLE
#ifdef TCC803x
#define USE_HAL_IPC_CA53
#define USE_HAL_IPC_CA7
#define USE_HAL_IPC_CM4
#endif

#define	HAL_DEVICE_ST_NO_OPEN		0
#define	HAL_DEVICE_ST_OPENED		1
#define	HAL_DEVICE_ST_ENABLED		2
#define	HAL_DEVICE_ST_DISABLED		3
typedef enum
{
	HAL_TIMER = 0,
	HAL_POWER_MANAGER,
	HAL_ADC_KEY,
	HAL_CONSOLE,
	HAL_IPC_CA53,
	HAL_IPC_CA7,
	HAL_IPC_CM4,
	HAL_I2C_KEY,
	HAL_GPIO_KEY,
	HAL_LED,
	HAL_SNR,
	HAL_IIC_EEPROM,
	HAL_LINETOOL,
	HAL_KLINEDRV,
	HAL_MAX_ID
}HAL_DEVICE_ID_Type;

extern void HAL_InitializeDevices(void);

#endif
