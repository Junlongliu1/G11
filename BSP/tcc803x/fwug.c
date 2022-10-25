/****************************************************************************
 *   FileName    : FWUG_SysFunc.c
 *   Description : FWUG main procedure
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
#include <app_cfg.h>

#include <types.h>
#include <osal.h>
#include "bsp.h"
#include "bsp_int.h"
#include "tcc_wdt.h"
#include "fwug.h"
#include "debug.h"

#define FWUG_DEBUG
#ifdef FWUG_DEBUG
#define FWUG_DBG(fmt, args...)   printf(fmt, ## args)
#else
#define FWUG_DBG(fmt, args...)   do {} while(0)
#endif


void FWUG_Start(uint32 cmd)
{
	if(cmd == FWUG_CMD_UPDATE_START)
	{
		FWUG_CopyAndJump();
	}
	else
	{
		FWUG_DBG("%s, Invalid command \n", __func__);
	}
}

void FWUG_CopyAndJump(void)
{
	snor_rom_info_t snor_info;
	uint32 image_offset;//, image_size;
	uint32 i;

	update_fw_header_x updater_header = {0,0,0}; //Codesonar Uninitialized Variable
	register void (*func)(void);

	/* disable interrupt */
	OSAL_SR_ALLOC();
	BSP_IntSrcDis(PPI_VIRT_TIMER);
	OSAL_CRITICAL_ENTER();

	/* disable watchdog timer */
	tcc_wdt_stop();
	
	/* search for update.bin */
	OSAL_MemCopy((void *)&snor_info, (void *)SNOR_BASEADDR+SNOR_ROM_INFO_OFFSET, sizeof(snor_rom_info_t));

	if (snor_info.signature == SNOR_SIGNATURE)
	{
		FWUG_DBG("%s, find F/W upgrade code \n", __func__);
		for (i = 0; i < SNOR_SECTION_MAX_COUNT; i++)
		{
		    if (snor_info.section_info[i].section_id == SNOR_UPDATE_BINARY_0_ID)
		    { //Msg(4:2212) Body of control statement is not enclosed within braces. MISRA-C:2004 Rules 14.8, 14.9
		        break;
		    }
		}	

		image_offset = snor_info.section_info[i].offset;
		//image_size = snor_info.section_info[i].image_size; /* update.bin header + image size */ //Codesonar Unused Value


		/* update.bin copy & jump */
		OSAL_MemCopy((void *)&updater_header, (void *)SNOR_BASEADDR+image_offset, sizeof(update_fw_header_x)); /* search for update.bin header */

		OSAL_MemCopy((void *)updater_header.address, (void *)SNOR_BASEADDR+image_offset+0x100, updater_header.size); /* copy update.bin to SRAM1 */

		FWUG_DBG("%s, copy from snor address :0x%08x\n", (SNOR_BASEADDR+image_offset+0x100));
		FWUG_DBG("%s, jump to F/W upgrade code address : 0x%08x, size: %d\n", __func__, updater_header.address, updater_header.size);

		func= ((void (*)(void))(updater_header.address)); //qac : rule1.1 
		if ( func !=  NULL_PTR) // Codesonar : Null Pointer Dereference 
		{
			(func)();
		}
		
		while (1) { //Never Reach
			NOP_DELAY(); // Empty while Statement
		}
	}
	else
	{
		FWUG_DBG("%s, can't find F/W upgrade code \n", __func__);
	}
	OSAL_CRITICAL_EXIT();// has no effect. added only for compile warning related with unused variable of cpu_sr

}
