/****************************************************************************
 *   FileName    : HAL_PowerManager.h
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
*    2017/03/10     0.1            created                      bskim
*******************************************************************************/

#ifndef __HAL_POWERMANAGER_H__
#define __HAL_POWERMANAGER_H__

#include "tcc_pmgpio.h"

#define HAL_PM_OPTION_NORMAL		(0U)

#define	HAL_POWERPIN_BATT	SYS_POWERPIN_BATT
#define	HAL_POWERPIN_ACC	SYS_POWERPIN_ACC
#define	HAL_POWERPIN_HI		SYS_POWERPIN_HI

typedef struct
{
	uint32 gHALPMStatus;
	uint32 gHALPMOpenOptions;
	uint32 gHALPMPowerDown;
} HalPmMng_ts;

enum
{
	HAL_IO_PM_SET_INTERRUPT = 0,
	HAL_IO_PM_GET_POWERDOWN_STATUS,			//not used
	HAL_IO_PM_ENTER_POWERDOWN,
	HAL_IO_PM_SET_POWERDOWN_WAIT_INFO,
	HAL_IO_PM_ENTER_POWERRESET,
	HAL_IO_PM_GET_POWERRESET,
	HAL_IO_PM_GET_POWERPIN_STATUS,
	HAL_IO_PM_SET_WARM_START_FLAG,
};

extern int32 PowerManager_Open(uint32 OPTIONS);
extern int32 PowerManager_Read(uint8 *buff, uint32 size);
extern int32 PowerManager_Close(void);
extern int32 PowerManager_Ioctl(uint32 OPT, void* PAR1, void* PAR2, void* PAR3, void* PAR4);
extern int32 PowerManager_CallBack(powerInterrupt_fp func);

#endif /* __HAL_POWERMANAGER_H__ */
