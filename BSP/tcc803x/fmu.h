/****************************************************************************
 *   FileName    : fmu.h
 *   Description : Verification of Fault Management Unit Start Entry
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

#ifndef  _FMU_H_
#define  _FMU_H_

//#include <common.h>
#include  <types.h>


#define FMU_PW		0x5afe8030
#define FMU_PW_ERROR		0x5afe8031

#define FAULT_SOURCE		64

//#define FMU_DEBUG
#ifdef FMU_DEBUG
#define FMU_DBG(fmt, args...)	printf("FMU\t: " fmt, ## args)
#else
#define FMU_DBG(fmt, args...)	do {} while(0)
#endif

#ifndef __GNU_C__
#define __func__ __FUNCTION__
#endif

//#define  FMU_FAIL										   0u
//#define  FMU_OK 										   1u


typedef enum{
	FMU_OK = 0, 
	FMU_FAIL,
	FMU_INVALID_ID_ERR,
	FMU_RESERVE_ID_ERR, 
	FMU_INVALID_SEVERITY_ERR, 
	FMU_ISR_HANDLER_ERR,
	FMU_DS_COMPARE_FAIL_ERR,
	FMU_INTSET_ERR,
	FMU_INTEN_ERR,
	FMU_INTDIS_ERR
}FMU_ERR_TYPES;


#define HwMC_FMU			((volatile MC_FMU			*)(MICOM_BASE_ADDR + 0x936400))


typedef  void (*FMU_INT_FNCT_PTR)(void *);



//----------------------------------------------------------------------------------------------------------------------------
// Register structure
//----------------------------------------------------------------------------------------------------------------------------
// FMU_CTRL
typedef struct {
	uint32	FORCE_FAULT 		:  1;	//	[	00], RW
	uint32	SW_RESET			:  1;	//	[	01], RW
	uint32	SOFT_FAULT_TEST_EN	:  1;	//	[	02], RW
	uint32	SOFT_FAULT_EN		:  1;	//	[	03], RW
	uint32	PERIODIC_FAULT_EN	:  1;	//	[	04], RW 
	uint32						: 27;	//	[31:05]
} FMU_CTRL;

typedef union {
	uint32		nReg;
	FMU_CTRL	bReg;
} FMU_CTRL_U;

// FMU_EN
typedef struct {
	uint32	ADC 				:  1;	//	[0][   00], RW
	uint32	GPSB0				:  1;	//	[0][   01], RW
	uint32	GPSB1				:  1;	//	[0][   02], RW
	uint32	GPSB2				:  1;	//	[0][   03], RW
	uint32	GPSB3				:  1;	//	[0][   04], RW
	uint32	PDM0				:  1;	//	[0][   05], RW
	uint32	PDM1				:  1;	//	[0][   06], RW
	uint32	PDM2				:  1;	//	[0][   07], RW
	uint32	MC_CFG				:  1;	//	[0][   08], RW
	uint32	CR5_CFG 			:  1;	//	[0][   09], RW
	uint32	CKC_CFG 			:  1;	//	[0][   10], RW
	uint32	GPIO_CFG			:  1;	//	[0][   11], RW
	uint32	MID_CFG 			:  1;	//	[0][   12], RW
	uint32	CMU_CFG 			:  1;	//	[0][   13], RW
	uint32	SYS_SM_CFG			:  1;	//	[0][   14], RW
	uint32	X2X_MST0			:  1;	//	[0][   15], RW
	uint32	X2X_MST1			:  1;	//	[0][   16], RW
	uint32	X2X_SLV0			:  1;	//	[0][   17], RW
	uint32	X2X_SLV1			:  1;	//	[0][   18], RW
	uint32	X2X_SLV2			:  1;	//	[0][   19], RW
	uint32	SDM0				:  1;	//	[0][   20], RW
	uint32	SDM1				:  1;	//	[0][   21], RW
	uint32	PERI_CLK			:  1;	//	[0][   22], RW
	uint32	PLL1_CLK			:  1;	//	[0][   23], RW
	uint32	SRAM0_ECC			:  1;	//	[0][   24], RW
	uint32	SRAM1_ECC			:  1;	//	[0][   25], RW
	uint32	SFMC_ECC			:  1;	//	[0][   26], RW
	uint32	MBOX0_S_ECC 		:  1;	//	[0][   27], RW
	uint32	MBOX0_NS_ECC		:  1;	//	[0][   28], RW
	uint32	MBOX1_S_ECC 		:  1;	//	[0][   29], RW
	uint32	MBOX1_NS_ECC		:  1;	//	[0][   30], RW
	uint32	MBOX2_ECC			:  1;	//	[0][   31], RW
	uint32	OSC 				:  1;	//	[1][   00], RW
	uint32	VLD_CORE			:  1;	//	[1][   01], RW
	uint32	VLD_OTP 			:  1;	//	[1][   02], RW
	uint32	WDT 				:  1;	//	[1][   03], RW
	uint32	PLL0_CLK			:  1;	//	[1][   04], RW
	uint32	CPU_CLK 			:  1;	//	[1][   05], RW
	uint32	BUS_CLK 			:  1;	//	[1][   06], RW
	uint32	CHIP_CFG_SOFT		:  1;	//	[1][   07], RW
	uint32	CHIP_CFG_CRC		:  1;	//	[1][   08], RW
	uint32	ROM_CRC 			:  1;	//	[1][   09], RW
	uint32						: 21;	//	[1][30:10]
	uint32	FMU_CFG_SOFT		:  1;	//	[1][   31], RW
} FMU_EN;

typedef union {
	uint32	nReg[2];
	FMU_EN	bReg;
} FMU_EN_U;

// FMU_EXT_FAULT_CTRL
typedef struct {
	uint32	POLARITY			:  1;	//	[	00], RW
	uint32						:  3;	//	[03:01], RW
	uint32	LOW 				:  4;	//	[07:04], RW
	uint32	HIGH				:  3;	//	[10:08], RW TODO
	uint32						: 21;	//	[31:11]
} FMU_EXT_FAULT_CTRL;

typedef union {
	uint32				nReg;
	FMU_EXT_FAULT_CTRL	bReg;
} FMU_EXT_FAULT_CTRL_U;

// FMU_SEVERITY_LEVEL
typedef struct {
	uint32	ADC 				:  2;	//	[0][01:00], RW
	uint32	GPSB0				:  2;	//	[0][03:02], RW
	uint32	GPSB1				:  2;	//	[0][05:04], RW
	uint32	GPSB2				:  2;	//	[0][07:06], RW
	uint32	GPSB3				:  2;	//	[0][09:08], RW
	uint32	PDM0				:  2;	//	[0][11:10], RW
	uint32	PDM1				:  2;	//	[0][13:12], RW
	uint32	PDM2				:  2;	//	[0][15:14], RW
	uint32	MC_CFG				:  2;	//	[0][17:16], RW
	uint32	CR5_CFG 			:  2;	//	[0][19:18], RW
	uint32	CKC_CFG 			:  2;	//	[0][21:20], RW
	uint32	GPIO_CFG			:  2;	//	[0][23:22], RW
	uint32	MID_CFG 			:  2;	//	[0][25:24], RW
	uint32	CMU_CFG 			:  2;	//	[0][27:26], RW
	uint32	SYS_SM_CFG			:  2;	//	[0][29:28], RW
	uint32	X2X_MST0			:  2;	//	[0][31:30], RW
	uint32	X2X_MST1			:  2;	//	[1][01:00], RW
	uint32	X2X_SLV0			:  2;	//	[1][03:02], RW
	uint32	X2X_SLV1			:  2;	//	[1][05:04], RW
	uint32	X2X_SLV2			:  2;	//	[1][07:06], RW
	uint32	SDM0				:  2;	//	[1][09:08], RW
	uint32	SDM1				:  2;	//	[1][11:10], RW
	uint32	PERI_CLK			:  2;	//	[1][13:12], RW
	uint32	PLL1_CLK			:  2;	//	[1][15:14], RW
	uint32	SRAM0_ECC			:  2;	//	[1][17:16], RW
	uint32	SRAM1_ECC			:  2;	//	[1][19:18], RW
	uint32	SFMC_ECC			:  2;	//	[1][21:20], RW
	uint32	MBOX0_S_ECC 		:  2;	//	[1][23:22], RW
	uint32	MBOX0_NS_ECC		:  2;	//	[1][25:24], RW
	uint32	MBOX1_S_ECC 		:  2;	//	[1][27:26], RW
	uint32	MBOX1_NS_ECC		:  2;	//	[1][29:28], RW
	uint32	MBOX2_ECC			:  2;	//	[1][31:30], RW
	uint32	OSC 				:  2;	//	[2][01:00], RW
	uint32	VLD_CORE			:  2;	//	[2][03:02], RW
	uint32	VLD_OTP 			:  2;	//	[2][05:04], RW
	uint32	WDT 				:  2;	//	[2][07:06], RW
	uint32	PLL0_CLK			:  2;	//	[2][09:08], RW
	uint32	CPU_CLK 			:  2;	//	[2][11:10], RW
	uint32	BUS_CLK 			:  2;	//	[2][13:12], RW
	uint32	CHIP_CFG_SOFT		:  2;	//	[2][15:14], RW
	uint32	CHIP_CFG_CRC		:  2;	//	[2][17:16], RW
	uint32	ROM_CRC 			:  2;	//	[2][19:18], RW
	uint32						: 12;	//	[2][31:20]
	uint32						: 30;	//	[3][29:00]
	uint32	FMU_CFG_SOFT		:  2;	//	[3][31:30], RW
} FMU_SEVERITY_LEVEL;

typedef union {
	uint32				nReg[4];
	FMU_SEVERITY_LEVEL	bReg;
} FMU_SEVERITY_LEVEL_U;

// FMU_CLK_SEL
typedef struct {
	uint32	CLK_SEL 			:  1;	//	[	00], RW
	uint32						: 31;	//	[31:01]
} FMU_CLK_SEL;

typedef union {
	uint32		nReg;
	FMU_CLK_SEL bReg;
} FMU_CLK_SEL_U;

// FMU_OUT_STATUS
typedef struct {
	uint32	IRQ 				:  1;	//	[	00], RO
	uint32	FIQ 				:  1;	//	[	01], RO
	uint32	FAULT				:  1;	//	[	02], RO
	uint32						: 13;	//	[15:03]
	uint32	FAULT_STATE 		:  2;	//	[17:16], RO
	uint32						: 14;	//	[31:18]
} FMU_OUT_STATUS;

typedef union {
	uint32			nReg;
	FMU_OUT_STATUS	bReg;
} FMU_OUT_STATUS_U;

//----------------------------------------------------------------------------------------------------------------------------
// FMU register structure
//----------------------------------------------------------------------------------------------------------------------------
typedef struct {
	FMU_CTRL_U					uCTRL;				// 0x00
	FMU_EN_U					uEN;				// 0x04 ~ 0x08
	FMU_EXT_FAULT_CTRL_U		uEXT_FAULT_CTRL;	// 0x0c
	FMU_SEVERITY_LEVEL_U		uSEVERITY_LEVEL;	// 0x10 ~ 0x1c
	FMU_EN_U					uIRQ_MASK;			// 0x20 ~ 0x24
	FMU_EN_U					uIRQ_CLR;			// 0x28 ~ 0x2c
	FMU_EN_U					uFIQ_MASK;			// 0x30 ~ 0x34
	FMU_EN_U					uFIQ_CLR;			// 0x38 ~ 0x3c
	FMU_EN_U					uFAULT_MASK;		// 0x40 ~ 0x44
	FMU_EN_U					uFAULT_CLR; 		// 0x48 ~ 0x4c
	FMU_EN_U					uIRQ_STATUS;		// 0x50 ~ 0x54
	FMU_EN_U					uFIQ_STATUS;		// 0x58 ~ 0x5c
	FMU_EN_U					uFAULT_STATUS;		// 0x60 ~ 0x64
	FMU_CLK_SEL_U				uCLK_SEL;			// 0x68
	FMU_OUT_STATUS_U			uOUT_STATUS;		// 0x6c
	uint32						CFG_WR_PW;			// 0x70
} MC_FMU;

 

//#define FMU_DEBUG

// FMU fault ID enumeration
enum {
	FMU_ID_ADC				=  0,
	FMU_ID_GPSB0			=  1,
	FMU_ID_GPSB1			=  2,
	FMU_ID_GPSB2			=  3,
	FMU_ID_GPSB3			=  4,
	FMU_ID_PDM0 			=  5,
	FMU_ID_PDM1 			=  6,
	FMU_ID_PDM2 			=  7,
	FMU_ID_MC_CFG			=  8,
	FMU_ID_CR5_CFG			=  9,
	FMU_ID_CKC_CFG			= 10,
	FMU_ID_GPIO_CFG 		= 11,
	FMU_ID_MID_CFG			= 12,
	FMU_ID_CMU_CFG			= 13,
	FMU_ID_SYS_SM_CFG		= 14,
	FMU_ID_X2X_MST0 		= 15,
	FMU_ID_X2X_MST1 		= 16,
	FMU_ID_X2X_SLV0 		= 17,
	FMU_ID_X2X_SLV1 		= 18,
	FMU_ID_X2X_SLV2 		= 19,
	FMU_ID_SDM_VIDEO		= 20,
	FMU_ID_SDM_AUDIO		= 21,
	FMU_ID_PERI_CLK 		= 22,
	FMU_ID_PLL1_CLK 		= 23,
	FMU_ID_SRAM0_ECC		= 24,
	FMU_ID_SRAM1_ECC		= 25,
	FMU_ID_SFMC_ECC 		= 26,
	FMU_ID_MBOX0_S_ECC		= 27,
	FMU_ID_MBOX0_NS_ECC 	= 28,
	FMU_ID_MBOX1_S_ECC		= 29,
	FMU_ID_MBOX1_NS_ECC 	= 30,
	FMU_ID_MBOX2_ECC		= 31,
	FMU_ID_OSC				= 32,
	FMU_ID_VLD_CORE 		= 33,
	FMU_ID_VLD_OTP			= 34,
	FMU_ID_WDT				= 35,
	FMU_ID_PLL0_CLK 		= 36,
	FMU_ID_CPU_CLK			= 37,
	FMU_ID_BUS_CLK			= 38,
	FMU_ID_CHIP_CFG_SOFT	= 39,
	FMU_ID_CHIP_CFG_CRC 	= 40,
	FMU_ID_ROM_CRC			= 41,
	FMU_ID_FMU_CFG_SOFT 	= 63
};

// FMU fault severity level enumeration
enum {
	FMU_SVL_LOW 			=  0,
	FMU_SVL_MID 			=  1,
	FMU_SVL_HIGH			=  2,
	FMU_SVL_REV 			=  3
};

// FMU operation clock source enumeration
enum {
	FMU_CLK_OSC 			=  0,
	FMU_CLK_XIN 			=  1
};

// External fault signal polarity
enum {
	FAULT_ACTIVE_LOW		=  0,
	FAULT_ACTIVE_HIGH		=  1
};

// External fault signal polarity
enum {
	FMU_DISABLE 			=  0,
	FMU_ENABLE				=  1
};

// FMU_EN
typedef struct {
	uint32	id;
	uint32	cnt;
} FMU_STS;


struct FMU_INT_VECT_PTR {
	uint32 severity_level;
	FMU_INT_FNCT_PTR func;
	void *arg;
};


static struct FMU_INT_VECT_PTR FMU_IntVectTbl[FAULT_SOURCE];



//----------------------------------------------------------------------------------------------------------------------------
// Function definition
//----------------------------------------------------------------------------------------------------------------------------


///////////////////////////////// Internal API 
static void set_fmu_ext_fault		(uint8 polarity, uint32 low_duration, uint32 high_duration);	// External fault configuration
static void fmu_sw_reset			(void); 													// FMU S/W reset assert & release
static void fmu_irq_cb (void *arg);
static void fmu_fiq_cb (void *arg);
static uint8 clr_fmu_isr (uint32 id);
static uint8 DS_Compare(uint32 value, uint32 reg);
static uint8 clr_fmu_isr_mask		(uint8 id); 												// IRQ	 mask clear
static FMU_STS get_fmu_irq_status	(void); 													// Get FMU IRQ	 status
static FMU_STS get_fmu_fiq_status	(void); 													// Get FMU FIQ	 status
//FMU_STS get_fmu_fault_status(void); 													// Get FMU Fault status






///////////////////////////////// ISO26262 Interface API 
uint8 init_fmu(void);
uint8 deinit_fmu(void);
uint8 tcc_fmu_isr_handler(uint8 id, uint8 severity_level, FMU_INT_FNCT_PTR int_fnct, void * arg);
uint8 tcc_fmu_set(uint8 id);
uint8 tcc_fmu_isr_clr (uint8 id);



#endif //_FMU_H_
