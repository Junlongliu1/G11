/****************************************************************************
 *   FileName    : HAL_PowerManager.c
 *   Description : HAL PowerManager device using PMGPIO
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
#include <types.h>
#include <osal.h>
#include "HAL_API.h"
#include "HAL_devices.h"
#include "HAL_PowerManager.h"
#include "gpio.h"
#include "pm_pin_define.h"
#include "debug.h"


#ifdef USE_HAL_POWER_MANAGER

static HalPmMng_ts halPmMng;

int32 PowerManager_Open(uint32 OPTIONS)
{
	int32 lRet = HAL_OK;
	
	halPmMng.gHALPMStatus = HAL_DEVICE_ST_NO_OPEN; //CS : Useless Assignment - Modify initial value

	if (OPTIONS  != NULL)
	{
		lRet = HAL_ERR_NOT_SUPPORTED;
	}
	else
	{
		halPmMng.gHALPMOpenOptions = OPTIONS;
		halPmMng.gHALPMStatus = HAL_DEVICE_ST_OPENED;
		halPmMng.gHALPMPowerDown = false;
	}

	return lRet;
}

int32 PowerManager_Read(uint8 *buff, uint32 size)
{
	int32 lRet = HAL_OK;

	if (halPmMng.gHALPMStatus != HAL_DEVICE_ST_OPENED)
	{
		lRet = HAL_ERR_NOT_OPENED;
	}
	else
	{
		if ( buff !=0)
		{
			;
		}
		else
		{
			lRet = HAL_ERR_BAD_PARAMETER;
		}
	}

	return lRet;
}

int32 PowerManager_Close(void)
{
	int32 lRet;

	if (halPmMng.gHALPMStatus != HAL_DEVICE_ST_OPENED)
	{
		lRet = HAL_ERR_NOT_OPENED;
	}
	else
	{
		//CS : Redundant Condition - Remove redundant if statement.
		halPmMng.gHALPMStatus = HAL_DEVICE_ST_NO_OPEN;
		//close driver
		lRet = HAL_OK;
	}

	return lRet;
}

int32 PowerManager_Ioctl(uint32 OPT, void* PAR1, void* PAR2, void* PAR3, void* PAR4)
{
	int32 lRet;

	lRet = HAL_OK;

	if (halPmMng.gHALPMStatus != HAL_DEVICE_ST_OPENED)
	{
		lRet = HAL_ERR_NOT_OPENED;
		tcc_pr_err("\n####### PowerManager_Ioctl not Open #####\n");
	}
	else
	{
		switch (OPT)
		{
			case HAL_IO_PM_SET_INTERRUPT:
				SetPMGPIO_Interrupt_Handler();
				break;

			case HAL_IO_PM_GET_POWERDOWN_STATUS:
				if(PAR3 != NULL)
				{
					uint8 value;
					value = GetPowerDownStatus();
					*((uint8 *)PAR3) = value;
				}
				break;

			case HAL_IO_PM_ENTER_POWERDOWN:
				if(PAR1 != NULL)
				{
					uint8 value;
					value = *((uint8 *)PAR1); //QAC
					if(value == PM_PWRDN_ACC_OFF) {
						PowerDown(Hw_ACC_DET);
					} 
					else if(value == PM_PWRDN_BU_OFF) {
						PowerDown(Hw_BU_DET);
#if defined(FEATURE_PWDN_SOURCE_EXTERNAL) //QAC
					} 
					else if(value == PM_PWRDN_EXT_SRC) {
						PowerDown(Hw_ACC_DET);
#endif
					} 
					else {
						tcc_pr_err("###### Failed to ENTER_POWERDOWN:%d ######\n", value);
					}
				}
				break;

			case HAL_IO_PM_SET_POWERDOWN_WAIT_INFO:
				if((PAR1 != NULL) && (PAR2 != NULL))
				{
					uint8 mode = *((uint8 *)PAR1); //QAC
					uint32 wait_time = *((uint32 *)PAR2);//QAC
					SetPowerDownWaitMode(mode, wait_time);//QAC
				}
				break;

			case HAL_IO_PM_ENTER_POWERRESET:
				if(PAR1 != NULL)
				{
					uint8 value;
					value = *((uint8 *)PAR1);
					if(value < PM_PWRDN_MAX){
						wvdSysPowerReset(value);
					} 
					else {
						tcc_pr_err("##### Failed to ENTER_POWERRESET:%d ######\n", value);
					}
				}

				break;

			case HAL_IO_PM_GET_POWERPIN_STATUS:
#if (TEMP_COMPILE_SWITCH_05WORK_10WORK == TEMP_COMPILE_SWITCH_05WORK)
				if((PAR1 != NULL) && (PAR2 != NULL))
				{
					uint8 pin = *((uint32 *)PAR1);
					
					if(pin == HAL_POWERPIN_BATT){
						*((uint8 *)PAR2) = gpio_get(PM_B_DET_BUFF);
					}
					else if(pin == HAL_POWERPIN_ACC){
						*((uint8 *)PAR2) = gpio_get(PM_A_DET);
					}
					else if(pin == HAL_POWERPIN_HI){
						*((uint8 *)PAR2) = gpio_get(PM_HI_DET);
					}
					else {
						tcc_pr_err("###### Unsupport pin number:%d #####\n", pin);
					}
				}

#else
				if((PAR1 != NULL) && (PAR2 != NULL))
				{
					uint8 pin = *((uint8 *)PAR1);				
					*((uint8 *)PAR2) = wubSysPowerGetPinStstus(pin);
				}
#endif
				break;

			case HAL_IO_PM_SET_WARM_START_FLAG:
				wvdSysPowerSetWarmFlag();
				break;
				
			default:
				break;
		}
	}

	return lRet;
}

int32 PowerManager_CallBack(powerInterrupt_fp func){

    int32 alRet = HAL_OK;

	if (halPmMng.gHALPMStatus != HAL_DEVICE_ST_OPENED)
	{
		alRet = HAL_ERR_NOT_OPENED;
	}
	else {
       wvdPowerSetCallback(func);
    }

	return alRet;
}

#endif
