/****************************************************************************
 *   FileName    : mpu.c
 *   Description : memory protect unit driver
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
#include <types.h>
#include <cache.h>
#include <osal.h>
#include <reg_phys.h>
#include <mpu.h>


#ifdef MPU_EN

/*****************************************************************************/
/*                               USER SETTING                                */
/*****************************************************************************/

/*** 1. SELECT DMA SIZE - CHOICE ONLY ONE ***/

#define REGION_DMA  REGION_8K


#if (REGION_DMA == REGION_4K)
    #define MPU_DMA_ADDR  MPU_SRAM0_ADDR+0x3F000
#elif (REGION_DMA == REGION_8K)
    #define MPU_DMA_ADDR  MPU_SRAM0_ADDR+0x3E000
#elif (REGION_DMA == REGION_16K)
    #define MPU_DMA_ADDR  MPU_SRAM0_ADDR+0x3C000
#elif (REGION_DMA == REGION_32K)
    #define MPU_DMA_ADDR  MPU_SRAM0_ADDR+0x38000
#elif (REGION_DMA == REGION_64K)
    #define MPU_DMA_ADDR  MPU_SRAM0_ADDR+0x30000
#endif


/*** 2. SET RESION ATTRIBUTES ***/
//Msg(4:3408) 'mpu_table' has external linkage but is being defined without any previous declaration. MISRA-C:2004 Rule 8.8
#if 0
MPU_CONFIG mpu_table[MPU_MAX_REGION] = {
	{ REGION_ENABLE,  MPU_BOOT_ADDR,    REGION_4G,      EXECUTE_NEVER | PRIV_RW_USER_NA | STRONG_ORDERD_SHARED },
	{ REGION_ENABLE,  MPU_BOOT_ADDR,    REGION_256M,    NORM_SHARED_WB_WA | PRIV_RW_USER_RW },
	{ REGION_ENABLE,  MPU_PERI_ADDR,    REGION_16M,     STRONG_ORDERD_SHARED | PRIV_RW_USER_RW },
	{ REGION_ENABLE,  MPU_SRAM0_ADDR,   REGION_256K,    NORM_SHARED_WB_WA | PRIV_RW_USER_RW },
	{ REGION_ENABLE,  MPU_SRAM1_ADDR,   REGION_64K,     NORM_SHARED_WB_WA | PRIV_RW_USER_RW },
	{ REGION_ENABLE,  MPU_DMA_ADDR,     REGION_DMA,     STRONG_ORDERD_SHARED | PRIV_RW_USER_RW },
	{ REGION_ENABLE,  MPU_SNOR_ADDR,    REGION_128M,    NORM_NSHARED_WB_NWA | PRIV_RO_USER_RO },
	{ REGION_ENABLE,  MPU_VECTOR_ADDR,  REGION_1K,      NORM_SHARED_WB_WA | PRIV_RW_USER_RW },
	{ REGION_ENABLE,  MPU_A7S_BOOT_ADDR,REGION_256M,    NORM_SHARED_WB_WA | PRIV_RW_USER_RW },
	{ REGION_ENABLE,  MPU_A7S_BACKUP_ADDR,  REGION_256M,     NORM_SHARED_WB_WA | PRIV_RO_USER_RO },
	{ REGION_ENABLE,  MPU_CBUS_ADDR,    REGION_4M,      STRONG_ORDERD_SHARED | PRIV_RW_USER_RW },
	{ REGION_ENABLE,  MPU_VIOC_ADDR,    REGION_4M,     STRONG_ORDERD_SHARED | PRIV_RW_USER_RW },
};
#endif

static MPU_CONFIG mpu_config[MPU_MAX_REGION]; //QAC : has external linkage and has multiple definitions.

static void mpu_update_config(uint32 reg_num, uint32 addr, uint32 size, uint32 attr);
//static void mpu_disable_region(uint32 reg_num);//Compile Warning
static void mpu_enable_region(uint32 reg_num, uint32 addr, uint32 size, uint32 attr);

/*****************************************************************************
*
* Update attributes of mpu_config from the mpu_table
*
* @param	reg_num specifies the number for that memory region.
* @param	addr is the address for which attributes are to be set.
* @param	size specifies the size for that memory region.
* @param	attr specifies the attributes for that memory region.
* @return	None.
*
******************************************************************************/
static void mpu_update_config(uint32 reg_num, uint32 addr, uint32 size, uint32 attr)
{
    if((size != 0) && (attr != 0))
    {
        mpu_config[reg_num].region_enable = REGION_ENABLE;
        mpu_config[reg_num].region_base = addr;
        mpu_config[reg_num].region_size = size;
        mpu_config[reg_num].region_attr = attr;
    }
    else
    {
        mpu_config[reg_num].region_enable = REGION_DISABLE;
        mpu_config[reg_num].region_base = 0U;
        mpu_config[reg_num].region_size = 0U;
        mpu_config[reg_num].region_attr = 0U;
    }
}

/*****************************************************************************
*
* Disable all the MPU regions if any of them is enabled
*
* @param	reg_num specifies the number for that memory region.*
* @return	None.
*
******************************************************************************/
/* GHC Warning : declared but never referenced
static void mpu_disable_region(uint32 reg_num)
{
	uint32  CtrlReg = 0;
	volatile uint32 Attrib;

	if(reg_num < MPU_MAX_REGION)
	{
	    //Disable I/D caches
    	Attrib = mmrc(ARM_CP15_SYS_CONTROL);
    	Attrib &= (~ARM_CP15_CONTROL_I); //disable I
    	Attrib &= (~ARM_CP15_CONTROL_C); // disable D
    	mmcr(ARM_CP15_SYS_CONTROL,Attrib);
    	mmcr(ARM_CP15_INVAL_BTAC,0); // Invalidate BTAC
    	ARM_DSB();

	    mmcr(ARM_CP15_MPU_MEMORY_REG_NUMBER, reg_num);
	    CtrlReg = mmrc(ARM_CP15_MPU_REG_SIZE_EN);
	    CtrlReg &= ~(REGION_EN);

	    ARM_DSB();
	    mmcr(ARM_CP15_MPU_REG_SIZE_EN, CtrlReg);
	    ARM_DSB();
	}
}
*/
/*****************************************************************************
*
* Set the memory attributes for a section of memory with starting address addr
* of the region size defined by reg_size having attributes attrib of region number
* reg_num
*
* @param	reg_num specifies the number for that memory region.
* @param	addr is the address for which attributes are to be set.
* @param	size specifies the size for that memory region.
* @param	attr specifies the attributes for that memory region.
* @return	None.
*
*
******************************************************************************/
static void mpu_enable_region(uint32 reg_num, uint32 addr, uint32 size, uint32 attr)
{
	uint32  address = addr;
	uint32  Regionsize = 0;

	//if((mpu_config[reg_num].region_enable != REGION_ENABLE) && (reg_num < MPU_MAX_REGION))
	if(((reg_num < MPU_MAX_REGION) && (mpu_config[reg_num].region_enable != REGION_ENABLE))) //QAC : Dereference of an invalid pointer value of reg_num
	{
		mpu_update_config(reg_num, addr, size, attr);

		mmcr(ARM_CP15_MPU_MEMORY_REG_NUMBER, reg_num);

		Regionsize = size;
		Regionsize <<= 1;
		Regionsize |= REGION_EN;
		ARM_DSB();

		mmcr(ARM_CP15_MPU_REG_BASEADDR, address);
		mmcr(ARM_CP15_MPU_REG_ACCESS_CTRL, attr);
		mmcr(ARM_CP15_MPU_REG_SIZE_EN, Regionsize);

		ARM_DSB();
	}
	else
	{
		// Error Print
	}
}

/*****************************************************************************
*
* Set attributes of all regions.
*
* @param	None.
* @return	None.
*
******************************************************************************/
void mpu_init(void)
{
	uint32 idx;
	volatile uint32 Attrib;
	MPU_CONFIG mpu_table[MPU_MAX_REGION] = {
		{ REGION_ENABLE,  MPU_BOOT_ADDR,    REGION_4G,      EXECUTE_NEVER | PRIV_RW_USER_NA | STRONG_ORDERD_SHARED },
		{ REGION_ENABLE,  MPU_BOOT_ADDR,    REGION_256M,    NORM_SHARED_WB_WA | PRIV_RW_USER_RW },
		{ REGION_ENABLE,  MPU_PERI_ADDR,    REGION_16M,     STRONG_ORDERD_SHARED | PRIV_RW_USER_RW },
		{ REGION_ENABLE,  MPU_SRAM0_ADDR,   REGION_256K,    NORM_SHARED_WB_WA | PRIV_RW_USER_RW },
		{ REGION_ENABLE,  MPU_SRAM1_ADDR,   REGION_64K,     NORM_SHARED_WB_WA | PRIV_RW_USER_RW },
		{ REGION_ENABLE,  MPU_DMA_ADDR,     REGION_DMA,     STRONG_ORDERD_SHARED | PRIV_RW_USER_RW },
		{ REGION_ENABLE,  MPU_SNOR_ADDR,    REGION_128M,    NORM_NSHARED_WB_NWA | PRIV_RO_USER_RO },
		{ REGION_ENABLE,  MPU_VECTOR_ADDR,  REGION_1K,      NORM_SHARED_WB_WA | PRIV_RW_USER_RW },
		{ REGION_ENABLE,  MPU_A7S_BOOT_ADDR,REGION_256M,    NORM_SHARED_WB_WA | PRIV_RW_USER_RW },
		{ REGION_ENABLE,  MPU_A7S_BACKUP_ADDR,  REGION_256M,     NORM_SHARED_WB_WA | PRIV_RO_USER_RO },
		{ REGION_ENABLE,  MPU_CBUS_ADDR,    REGION_4M,      STRONG_ORDERD_SHARED | PRIV_RW_USER_RW },
		{ REGION_ENABLE,  MPU_VIOC_ADDR,    REGION_4M,     STRONG_ORDERD_SHARED | PRIV_RW_USER_RW }//Msg(4:0686) Array has fewer initializers than its declared size. Default initialization is applied to the remainder of the array elements. MISRA-C:2004 Rule 9.2; REFERENCE - ISO-6.5.7 Semantics
	};

	OSAL_MemSet(mpu_config, 0, sizeof(mpu_config)); //QAC

	//Disable I/D caches
	Attrib = mmrc(ARM_CP15_SYS_CONTROL);
	Attrib &= (~ARM_CP15_CONTROL_I); //disable I
	Attrib &= (~ARM_CP15_CONTROL_C); // disable D
	mmcr(ARM_CP15_SYS_CONTROL,Attrib);
	mmcr(ARM_CP15_INVAL_BTAC,0); // Invalidate BTAC
	ARM_DSB();

	//Program Flow Prediction Enable
	Attrib = mmrc(ARM_CP15_SYS_CONTROL);
	Attrib |= (ARM_CP15_CONTROL_Z); // Branch Prediction Enable bit
	mmcr(ARM_CP15_SYS_CONTROL,Attrib);
	ARM_DSB();

	// Invalidate L1 Caches, Invalidate Instruction cache
	mmcr(ARM_CP15_INVAL_IC_POU,0);
	ARM_DSB();

	//Instruction cache Enable prior to Data Cache Invalidation
	Attrib = mmrc(ARM_CP15_SYS_CONTROL);
	Attrib |= (ARM_CP15_CONTROL_I); // Enable I Cache
	mmcr(ARM_CP15_SYS_CONTROL,Attrib);

	ARM_DSB();

	for(idx = 0; idx < (sizeof(mpu_table) / sizeof(mpu_table[0])); idx++)
	{
		mpu_enable_region(idx, mpu_table[idx].region_base, mpu_table[idx].region_size, mpu_table[idx].region_attr);
	}

	// Enable MPU and Data Cache
	Attrib = mmrc(ARM_CP15_SYS_CONTROL);
	Attrib |= (ARM_CP15_CONTROL_C); // Enable D Cache
	mmcr(ARM_CP15_SYS_CONTROL,Attrib);
	ARM_DSB();
}

uint32 mem_get_dma_base(void)
{
	#ifdef TCC803x_CS
	// 1. Ths "__nc_dmastart" value already contains the physical base(0xC0000000). Please reference the linker(your) script file(.ld)
	// 2. The dma address is valid only physical address(memory and peripheral point of view).
	uint32 dma_start = 0x00000000; // Physical Offset for 256K SRAM for execution zero base
	#else
	uint32 dma_start = 0xC0000000; // Physical Offset for 256K SRAM for execution zero base
	#endif

	dma_start += (uint32)(&__nc_dmastart[0]); //modify for codesonar warning
	return dma_start;
}

#endif
