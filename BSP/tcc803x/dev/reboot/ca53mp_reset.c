 /****************************************************************************
  *   FileName	  : ca53mp_reset.c
  *   Description :
  * \date						 2021/11/04
  * \author 				 xulei
  * \model					 EDA
  * \description			 a7 reset
  * \version				 0.55
  *
  *****************************************************************************

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
//#ifndef __FILENAME__ //QAC  The macro identifier '%s' is reserved
//#define __FILENAME__ "ca53mp_reboot.c"
//#endif/*__FILENAME__*/

#include <types.h>
#include <bsp.h>
#include <bsp_int.h>
#include <osal.h>
#include "debug.h"
#include "clock.h"
#include "reg_phys.h"
#include "tcc_timer.h"
#include "os_framework_type.h"
#include "os_framework.h"
#ifndef TCC803x_CS
#include "tcc_ckc.h"
#endif

#include <tcc_ipc.h>
#include "tcc_robust.h" // robust_a53_reboot
#include "tcc_pmgpio.h"
#include "rpc_if.h"
#include "pm.h"


//#define CA53MP_RESET_DEBUG_ENABLE
#ifdef CA53MP_RESET_DEBUG_ENABLE
#define CA53_RESET_DEBUG(fmt, args...)   printf(fmt, ## args)
#else
#define CA53_RESET_DEBUG(fmt, args...)   do {} while(0)
#endif

#ifndef __GNU_C__
#define __func__ __FUNCTION__
#endif

#define TMP_DELAY(count) do { \
	volatile uint32 i = 0; \
	for( ; i<(count); i++ ) {}; \
}while(0)

static void tcc_pmu_wdt_irq_handler(void *args);

/********************************************************************************************/
/**	\function		tcc_pmu_wdt_irq_handler
 *	\date			2021/11/04
 *	\author			xulei
 *	\description		pmu watchdog irq handler
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		none
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			none
 *					
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/11/4		xulei		New
 ********************************************************************************************/
static void tcc_pmu_wdt_irq_handler(void *args)
{
	(void)wsbRPCSendMsg(ARPC_ID_OS, ARPC_ID_R5_POWERMNG, EV_INF_PM_A53WDT_REBOOT_REQ,STD_NULL, STD_NULL);
}
/********************************************************************************************/
/**	\function		tcc_robust_ca53_watchdog_init
 *	\date			2021/11/04
 *	\author			liyongnan
 *	\description		robust a53 wdt init
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		none
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			ret 
 *					
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/11/4		xulei		New
 ********************************************************************************************/
int32 tcc_robust_ca53_watchdog_init(void)
{
	int32 ret = BSP_ERR_NONE;

	if (BSP_IntVectSet(INT_PMU_WDT, 0x2,
				INT_TYPE_EDGE_RISING,
				tcc_pmu_wdt_irq_handler,
				0) != BSP_OK)
	{
		ret = BSP_ERR_INIT;
		CA53_RESET_DEBUG("[%s] irq hanlder set failed! [%d]", __func__, ret);
		return ret;
	}

	BSP_IntSrcEn(INT_PMU_WDT);

	return BSP_ERR_NONE;
}
