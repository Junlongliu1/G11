/****************************************************************************
 *   FileName    : ca7sp_reset.c
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
//#ifndef __FILENAME__
//#define __FILENAME__ "watchdog_reboot.c"
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

//#include <tcc_ipc_typedef.h>
//#include <tcc_ipc.h>
//#include <tcc_ipc_buffer.h>
//#include <tcc_ipc_os.h>
//#include <tcc_ipc_mbox.h>
//#include <tcc_ipc_ctl.h>
//#include <tcc_ipc_cmd.h>
#include "tcc_robust.h"
#include "tcc_pmgpio.h"
#include "rpc_if.h"
#include "pm.h"


//#define CA7SP_RESET_DEBUG_ENABLE
#ifdef CA7SP_RESET_DEBUG_ENABLE
#define CA7_RESET_DEBUG(fmt, args...)   printf(fmt, ## args)
#else
#define CA7_RESET_DEBUG(fmt, args...)   do {} while(0)
#endif

#ifndef __GNU_C__
#define __func__ __FUNCTION__
#endif


#define TMP_DELAY(count) do { \
	volatile uint32 i = 0; \
	for( ; i<(count); i++ ) {}; \
} while(0)

#define WATCHDOG_RESET	0x99FF99FF

#ifdef TCC803x_CS
#define HLC_HEADER_BASE (0x07000040)
#else
#define HLC_HEADER_BASE (0xC1000040)
#endif

#define MBOX_CMD_BUFFER		8
#define MBOX_DATA_BUFFER 	32

#define A53_MCP_FLAG	0x8030ca53

static void restore_a7s_image(uint32 flags);//Msg(4:3408) 'restore_a7s_image' has external linkage but is being defined without any previous declaration. MISRA-C:2004 Rule 8.8
static void tcc_cbus_wdt_irq_handler(void *args);//Msg(4:3450) Function 'tcc_cbus_wdt_irq_handler', with internal linkage, is being defined without a previous declaration. MISRA-C:2004 Rule 8.1

static void tcc_cbus_wdt_irq_handler(void *args)
{
	(void)wsbRPCSendMsg(ARPC_ID_OS, ARPC_ID_R5_POWERMNG, EV_INF_PM_A7WDT_REBOOT_REQ,STD_NULL, STD_NULL);
}

/********************************************************************************************/
/**	\function		tcc_robust_ca7s_watchdog_init
 *	\date			2021/11/04
 *	\author			liyongnan
 *	\description		robust a7 wdt init
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
int32 tcc_robust_ca7s_watchdog_init(void)
{
	int32 ret = BSP_ERR_NONE;

	if (BSP_IntVectSet(INT_CBUS_WDT, 0x2,
				INT_TYPE_EDGE_RISING,
				tcc_cbus_wdt_irq_handler,
				0) != BSP_OK)
	{
		ret = BSP_ERR_INIT;
		CA7_RESET_DEBUG("[%s] irq hanlder set failed! [%d]",__func__, ret);
		return ret;
	}

	BSP_IntSrcEn(INT_CBUS_WDT);

	return BSP_ERR_NONE;
}
