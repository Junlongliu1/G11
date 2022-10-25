/****************************************************************************
 *   FileName    : Main.c
 *   Description : Main Application Start Entry
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


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/
#include <types.h>
#include <version.h>
#include <bsp.h>
#include <app_cfg.h>
#include <HAL_API.h>
#include <HAL_devices.h>
#include <HAL_Timer.h>
//Test Gpio
#include <gpio.h>
#include "tcc_pmgpio.h"
#if (SERVICE_CONSOLE_EN == 1)
#include <ConsoleDemoApp.h>
#endif

#include <osal.h>
#include <main.h>
#include <bsp_os.h>
#include "debug.h"

#include <Console.h>
#include "tcc_ipc_protocol.h"
#include "os_framework_cfg.h"
#include "temp_compile_switch.h"
#include "compile_switch.h"

#include "Eepromcontrol.h"

#include "logapp.h" 
#include "pm.h"
#include "pmdrv.h"

#if (SERVICE_TEST_EN == 1u)
#include "clock.h" 
#include "Robust_Test.h" 
#endif


/*
*********************************************************************************************************
*                                             LOCAL DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL VARIABLES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                cmain()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Arguments   : none
*
* Returns     : none
*********************************************************************************************************
*/

void cmain (void)
{
	int32  err = OSAL_ERR_NONE;
	uint32 startType = PM_PWRDN_ACC_OFF;
#ifdef PM_BATT_CHECK_BEFORE_IRQ
	uint8  battStatus = FALSE;
#endif

	(void) OSAL_InitOS();

	BSP_Init(); /* Initialize BSP functions */
	// print out sdk version string.
	OS_DBG("\nSDK Version: V%d.%d.%d\n",SDK_MAJOR_VERSION,SDK_MINOR_VERSION, SDK_PATCH_VERSION);
	#if (OS_START_TEST == 1u)
	PF_TEST_START;
	#endif
	
	// HAL init...
	HAL_InitializeDevices();

	// get cold reset status
	startType = wubPmGetStartType();

	// PMGPIO Init
	InitializePMGPIO(0);

#ifdef PM_HAL_BEFORE_OS	
	// Hal Power Open and Batt INT set
	wvdPmHalOpen();
#else
	//Batt INT set
	wvdPmBattIrqSet();
#endif

#ifdef PM_BATT_CHECK_BEFORE_IRQ
	// check batt status...
	battStatus = wubPmDrvBattStatusChk();
	if(battStatus == RES_NG){
		wvdPmBattSysDown();
		return;
	}
#endif

	// call module cold reset init
	if((startType == PM_PWRDN_NO)  || (startType == PM_PWRDN_UPDATE_RESET) || (startType == PM_PWRDN_FACTORY_RESET)){
		wvdPmModuleColdInit();
	}

	wvdPmModuleInit();

#if (TEMP_COMPILE_SWITCH_05WORK_10WORK == TEMP_COMPILE_SWITCH_05WORK)
	tcc_pr_info("\n******* 0.5 WORK*******\n");
#else
	tcc_pr_info("\n******* 1 WORK*******\n");
#endif
#if (COMPILE_SWITCH_DISP_SIZ == COMPILE_SWITCH_8INCH)
	tcc_pr_info("\n******* 8 inch *******\n");
#else
	tcc_pr_info("\n******* 10 inch *******\n");
#endif

	#if (SERVICE_CONSOLE_EN == 1u)
	InitializeConsole();
	#endif
	
	err = OS_Register();
	
	// start woring os.... never return from this function
	if( OSAL_ERR_NONE == err){
		
		#if (OS_START_TEST == 1u)
		PF_TEST_STOP;
		#endif
		
		(void) OSAL_StartOS();
		// start woring os.... never return from this function
	}else{
		OS_DBG("\n %s OS_Register Create Err: %d",__FUNCTION__,err);
		wvdSysPowerReset(PM_PWRDN_OS_RESET);
	}
}
/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/
#if (SERVICE_TEST_EN == 1u)

void  TSK_APP (void )
{	
	static int32 oldTimeTick = 0;
	static int32 newTimeTick = 0;
	int32 diffTick = 0;
	uint32 clock;

	OS_ErrCode_t aswFlagResult;
	OS_FlgPtn_t auwFlagPattern;

	while (TRUE)
	{
		aswFlagResult = OS_WaitEventFlag(FLG_ID_TEST, TEST_FLGPTN, OS_TRUE, &auwFlagPattern, OS_TOUT_FOREVER);
		if (OS_ERR_OK == aswFlagResult) {
			
			oldTimeTick = newTimeTick;
			newTimeTick = OSAL_GetOSTimeTick();
			diffTick	= newTimeTick - oldTimeTick;
			
			if (TEST_FLGPTN_WDTIRQ == (auwFlagPattern & TEST_FLGPTN)){
				clock = tcc_get_pll(0);
				printf("\n FLG_ID_TEST  TEST_FLGPTN DiffTick =%d    clock = %d!!!\n",diffTick,clock);
				(void)OS_ClearEventFlag(FLG_ID_TEST, ~TEST_FLGPTN_WDTIRQ);
			}else{
				printf("\n FLG_ID_TEST TimeOut DiffTick =%d !!!\n",diffTick);
			}
		}
	}
}
#endif/*SERVICE_TEST_EN*/


void SDK_GetVersionInfo(Sdk_VersionInfoType * pVersion)
{
	static const Sdk_VersionInfoType SDK_Version =
	{
		SDK_MAJOR_VERSION,
		SDK_MINOR_VERSION,
		SDK_PATCH_VERSION,
		0
	};

	if ( pVersion != 0)
	{
		OSAL_MemCopy(pVersion, &SDK_Version, sizeof(Sdk_VersionInfoType));
	}
}
