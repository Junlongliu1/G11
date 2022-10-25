/****************************************************************************
 *   FileName    : tcc_pmgpio.h
 *   Description : PMGPIO driver
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


#ifndef  __TCC_PMGPIO_H__
#define  __TCC_PMGPIO_H__

#include "temp_compile_switch.h"


//#define FEATURE_PWDN_SOURCE_EXTERNAL

/*
*********************************************************************************************************
*                                                 EXTERNS
*********************************************************************************************************
*/

#define	HwFULL 					0xFFFFFFFFUL
#define	HwZERO 				0x00000000UL

#define	Hw31 					0x80000000UL
#define	Hw30 					0x40000000UL
#define	Hw29 					0x20000000UL
#define	Hw28 					0x10000000UL
#define	Hw27 					0x08000000UL
#define	Hw26 					0x04000000UL
#define	Hw25 					0x02000000UL
#define	Hw24 					0x01000000UL
#define	Hw23 					0x00800000UL
#define	Hw22 					0x00400000UL
#define	Hw21 					0x00200000UL
#define	Hw20 					0x00100000UL
#define	Hw19 					0x00080000UL
#define	Hw18 					0x00040000UL
#define	Hw17 					0x00020000UL
#define	Hw16 					0x00010000UL
#define	Hw15 					0x00008000UL
#define	Hw14 					0x00004000UL
#define	Hw13 					0x00002000UL
#define	Hw12 					0x00001000UL
#define	Hw11 					0x00000800UL
#define	Hw10 					0x00000400UL
#define	Hw9 					0x00000200UL
#define	Hw8 					0x00000100UL
#define	Hw7 					0x00000080UL
#define	Hw6 					0x00000040UL
#define	Hw5 					0x00000020UL
#define	Hw4 					0x00000010UL
#define	Hw3 					0x00000008UL
#define	Hw2 					0x00000004UL
#define	Hw1 					0x00000002UL
#define	Hw0 					0x00000001UL

#if (TEMP_COMPILE_SWITCH_05WORK_10WORK == TEMP_COMPILE_SWITCH_05WORK)
#define 	Hw_BU_DET				Hw1
#define 	Hw_ACC_DET				Hw3
#else
#define 	Hw_HI_DET				Hw0
#define 	Hw_BU_DET				Hw1
#define 	Hw_ACC_DET				Hw2
#endif
#define 	Hw_PCTL_0				Hw0
#define 	Hw_PCTL_1				Hw1

/***********************************************************************************
*
*	GPIO_K[0:31] Register Define			(Base Addr = 0x14200000UL)
*
************************************************************************************/

#define HwGPIOK_DAT 			*(volatile uint32 *)0x1B935680UL 		// R/W, GPK Data Register
#define HwGPIOK_EN 				*(volatile uint32 *)0x1B935684UL 		// R/W, 
#define HwGPIOK_SET 			*(volatile uint32 *)0x1B935688UL 		// R/W, 
#define HwGPIOK_CLR 			*(volatile uint32 *)0x1B93568CUL 		// R/W, 
#define HwGPIOK_XOR 			*(volatile uint32 *)0x1B935690UL 		// R/W, 
#define HwGPIOK_CD0 			*(volatile uint32 *)0x1B935694UL 		// R/W, 
#define HwGPIOK_CD1 			*(volatile uint32 *)0x1B935698UL 		// R/W, 
#define HwGPIOK_PE 				*(volatile uint32 *)0x1B93569CUL 		// R/W, 
#define HwGPIOK_PS 				*(volatile uint32 *)0x1B9356A0UL 		// R/W, 
#define HwGPIOK_IEN 				*(volatile uint32 *)0x1B9356A4UL 		// R/W, 
#define HwGPIOK_IS 				*(volatile uint32 *)0x1B9356A8UL 		// R/W, 
#define HwGPIOK_SR 				*(volatile uint32 *)0x1B9356ACUL 		// R/W, 
#define HwGPIOK_FN0 				*(volatile uint32 *)0x1B9356B0UL 		// R/W, 
#define HwGPIOK_FN1 				*(volatile uint32 *)0x1B9356B4UL 		// R/W, 
#define HwGPIOK_FN2 				*(volatile uint32 *)0x1B9356B8UL 		// R/W, 


/***********************************************************************************
*
*	Backup RAM Register Define			(Base Addr = 0x14800000UL)
*
************************************************************************************/

#define HwBACKUP_RAM 			*(volatile uint32 *)0x1B937000UL 		// R/W, Backup RAM Register


/***********************************************************************************
*
*	Alive GPIO(PMGPIO) Register Define			(Base Addr = 0x1B937000UL)
*
************************************************************************************/

#define HwPMGPIO_DAT 			*(volatile uint32 *)0x1B937800UL 		// R/W, PMGPIO Data Register
#define HwPMGPIO_DOE 			*(volatile uint32 *)0x1B937804UL 		// R/W, PMGPIO Direction Control Register
#define HwPMGPIO_FS0 			*(volatile uint32 *)0x1B937808UL 		// R/W, PMGPIO Function Select 0 Register
#define HwPMGPIO_DIE 			*(volatile uint32 *)0x1B93780CUL 		// R/W, PMGPIO Input Enable Register
#define HwPMGPIO_PE 			*(volatile uint32 *)0x1B937810UL 		// R/W, PMGPIO PULL Down/Up Enable Register
#define HwPMGPIO_PS 			*(volatile uint32 *)0x1B937814UL 		// R/W, PMGPIO PULL Down/Up Select Register
#define HwPMGPIO_CD0 			*(volatile uint32 *)0x1B937818UL 		// R/W, PMGPIO Driver Strength 1 Register
#define HwPMGPIO_CD1 			*(volatile uint32 *)0x1B93781CUL 		// R/W, PMGPIO Driver Strength 1 Register
#define HwPMGPIO_EE0 			*(volatile uint32 *)0x1B937820UL 		// R/W, PMGPIO Event Enable 0 Register
#define HwPMGPIO_EE1 			*(volatile uint32 *)0x1B937824UL 		// R/W, PMGPIO Event Enable 1 Register
#define HwPMGPIO_CTL 			*(volatile uint32 *)0x1B937828UL 		// R/W, PMGPIO Control Register
#define HwPMGPIO_DI 				*(volatile uint32 *)0x1B93782CUL 		// R/W, PMGPIO Input Raw Status Register
#define HwPMGPIO_STR 			*(volatile uint32 *)0x1B937830UL 		// R/W, PMGPIO Rising Event Status Register
#define HwPMGPIO_STF 			*(volatile uint32 *)0x1B937834UL 		// R/W, PMGPIO Falling Event Status Register
#define HwPMGPIO_POL 			*(volatile uint32 *)0x1B937838UL 		// R/W, PMGPIO Event Polarity Register
#define HwPMGPIO_PROT 			*(volatile uint32 *)0x1B93783CUL 		// R/W, PMGPIO Protect Register
#define HwPMGPIO_DAT_H 			*(volatile uint32 *)0x1B937840UL 		// R/W, PMGPIO Data Register
#define HwPMGPIO_DOE_H 		*(volatile uint32 *)0x1B937844UL 		// R/W, PMGPIO Direction Control Register
#define HwPMGPIO_FS0_H 			*(volatile uint32 *)0x1B937848UL 		// R/W, PMGPIO Function Select 0 Register
#define HwPMGPIO_DIE_H 			*(volatile uint32 *)0x1B93784CUL 		// R/W, PMGPIO Input Enable Register
#define HwPMGPIO_PE_H 			*(volatile uint32 *)0x1B937850UL 		// R/W, PMGPIO PULL Down/Up Enable Register
#define HwPMGPIO_PS_H 			*(volatile uint32 *)0x1B937854UL 		// R/W, PMGPIO PULL Down/Up Register
#define HwPMGPIO_CD0_H 			*(volatile uint32 *)0x1B937858UL 		// R/W, PMGPIO Driver Strength 0 Register
#define HwPMGPIO_CD1_H 			*(volatile uint32 *)0x1B93785CUL 		// R/W, PMGPIO Driver Strength 1 Register

#define HwPMGPIO_APB 			*(volatile uint32 *)0x1B937900UL 		// R/W, PMGPIO & PMRAM APB Timing Register

#define HwGPK_FDAT 				*(volatile uint32 *)0x1B937A00UL 		// R/W, GPK Filtered Data Port
#define HwGPK_IRQST 			*(volatile uint32 *)0x1B937A10UL 		// R/W, GPK Interrupt Status Register
#define HwGPK_IRQEN 			*(volatile uint32 *)0x1B937A14UL 		// R/W, GPK Interrupt Enable Register
#define HwGPK_IRQPOL 			*(volatile uint32 *)0x1B937A18UL 		// R/W, GPK Interrupt Polarity Register
#define HwGPK_IRQTM0 			*(volatile uint32 *)0x1B937A1CUL 		// R/W, GPK Interrupt Trigger Mode 0 Register
#define HwGPK_IRQTM1 			*(volatile uint32 *)0x1B937A20UL 		// R/W, GPK Interrupt Trigger Mode 1 Register
#define HwGPK_FCK 				*(volatile uint32 *)0x1B937A24UL 		// R/W, GPK Filter Clock Configuration Register
#define HwGPK_FBP 				*(volatile uint32 *)0x1B937A28UL 		// R/W, GPK Filter Bypass Register
#define HwGPK_CTL 				*(volatile uint32 *)0x1B937A2CUL 		// R/W, GPK Miscellaneous Control Register


#define PMGPIO_CTL_REG			0x1B937828  //PMGPIO Controll Register
#define RST_STS0				0xC003F400 //Reset Status 0 Register (0x14400130)
#define RST_STS1				0xC003F404 //Reset Status 1 Register (0x14400134)
#define RST_STS2				0xC003F408 //Reset Status 2 Register (0x14400138)

#define PM_SYS_WARM_RESET		(0U)
#define PM_SYS_COLD_RESET		(1U)

#define WARM_RESET_REG			0x1B936184
#define COLD_RESET_REG			0x1B93617C

#define PM_GPIO_STS_NG			0xFF

enum
{
	SYS_POWERPIN_BATT = 0,
	SYS_POWERPIN_ACC,
	SYS_POWERPIN_HI
};


typedef enum {
	PM_PWRDN_NO 					= 0x00,
	PM_PWRDN_BU_OFF					= 0x01,
	PM_PWRDN_ACC_OFF				= 0x02,
	PM_PWRDN_WDT					= 0x03,
	PM_PWRDN_UPDATE_RESET			= 0x04,
	PM_PWRDN_FACTORY_RESET			= 0x05,
	PM_PWRDN_A53_REBOOT_REQ			= 0x06,
	PM_PWRDN_A7_REBOOT_REQ			= 0x07,
	PM_PWRDN_A53_HAERT_TIMOUT		= 0x08,
	PM_PWRDN_A7_HAERT_TIMOUT		= 0x09,
	PM_PWRDN_OS_A53WDT				= 0x0A,
	PM_PWRDN_OS_A7WDT				= 0x0B,
	PM_PWRDN_SOC_START_TIMOUT		= 0x0C,
	PM_PWRDN_SOC_STANDBYOUT_TIMOUT	= 0x0D,
	PM_PWRDN_OS_RESET				= 0x0E,
#if defined(FEATURE_PWDN_SOURCE_EXTERNAL) //QAC
	//PM_PWRDN_EXT_SRC			= 0x0E,
#endif
	PM_PWRDN_MAX				= 0x0F,
} PM_PWRDN_TYPE;

typedef  void (*powerInterrupt_fp)(uint8 data, uint8 error);

extern uint8 GetPowerDownStatus(void);
extern void SetPowerDownWaitMode(uint8 mode, uint32 wait_time);
extern void SetPMGPIO_Interrupt_Handler(void);
extern void EarlyWakeUp(void);
extern void PowerDown(int32 mode);
extern void InitializePMGPIO(int32 boot);
extern void checkBootReason(void);
extern uint8 PmGetBattDat(void);

#if 0
int32 PMIO_sel_config(uint32 mode, uint32 gpio, uint32 pmgp_dir);
int32 PMIO_port_config(uint32 config);
int32 PMIO_GPK_f_read(uint32 gpio);
#endif
extern void wvdSysPowerReset(uint8 type);
extern uint32 wubGetStartInfo(void *pSts0, void *pSts1, void *pSts2);
extern void wvdPowerSetCallback(powerInterrupt_fp astFunc_fp);
extern void wvdSysPowerSetWarmFlag(void);
extern uint8 wubSysPowerGetPinStstus(uint8 pin);
#endif

