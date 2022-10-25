/****************************************************************************
 *   FileName    : tcc_pmgpio.c
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

#include <types.h>
#include <bsp.h> // for introducing NOP_DELAY : QAC
#include <bsp_int.h>

#include "tcc_pmgpio.h"
#include "tcc_timer.h"
#include "reg_phys.h"

#include "debug.h"
#include "os_framework.h"
#include "pm_pin_define.h"

#include "temp_compile_switch.h"



#define PMGPIO_INTERRUPT_SRC				(INT_PMGPIO)
#define PMGPIO_CHECK_WAIT					(1U)		
#define PMGPIO_CTRLREG_DOWN_SHIFT			(24U)
#define PMGPIO_CTRLREG_DOWN_BATT_MASK		(0xF0000000)
#define PMGPIO_CTRLREG_DOWN_SOFT_MASK		(0x0F000000)


extern void wvdPmBattOff(Type_uByte aubData1, Type_uByte aubData2);

static uint8 powerDownStatus = false; //current power down type

static uint8 setACCTimer = false; //set Timer to wait ACC power down
static uint8 setBUTimer = false; //set Timer to wait BU power down

static uint8 powerDownWaitMode = false; //wait : true, no wait : false
static uint32 powerDownWaitTime = 0;

static powerInterrupt_fp nstPowerInterrupt_fp = NULL;

/*static */
static int32 PMGPIO_WaitPowerDown(int32 dummy, void* pDummy);
static void PMGPIO_Interrupt_Handler(void *arg);
static void SetPowerDownStatus(uint8 status);

static void SetPowerDownStatus(uint8 status)
{
	powerDownStatus = status;
}

uint8 GetPowerDownStatus(void)
{
	return powerDownStatus;
}

void SetPowerDownWaitMode(uint8 mode, uint32 wait_time)
{
	powerDownWaitMode = mode;
	powerDownWaitTime = wait_time;
}

static int32 PMGPIO_WaitPowerDown(int32 dummy, void* pDummy)
{
	static uint32 cnt = 0;
	int32 ret = 0;

	if(setBUTimer == true) {
		if ((HwGPK_IRQST & Hw_BU_DET) == 0) {
			tcc_timer_disable(PMGPIO_TIMER_CH);
			cnt = 0;
			setBUTimer = false;
		} else {
			cnt++;
		}
		
		if (cnt > powerDownWaitTime) {
			HwGPK_IRQEN &= ~Hw_BU_DET;
			tcc_timer_disable(PMGPIO_TIMER_CH);
			SetPowerDownStatus(PM_PWRDN_BU_OFF);
		}
	}
	
	if(setACCTimer == true) {
		if ((HwGPK_IRQST & Hw_ACC_DET) == 0) {
			tcc_timer_disable(PMGPIO_TIMER_CH);
			cnt = 0;
			setACCTimer = false;
		} else {
			cnt++;
		}
		
		if (cnt > powerDownWaitTime) {
			HwGPK_IRQEN &= ~Hw_ACC_DET;
			tcc_timer_disable(PMGPIO_TIMER_CH);
			SetPowerDownStatus(PM_PWRDN_ACC_OFF);
		}
	}

	//(void) ret;
	//(void) dummy;
	//(void) pDummy;
	
	return ret;
}

static void PMGPIO_Interrupt_Handler(void *arg)
{	
	if ((HwGPK_IRQST & Hw_BU_DET) != 0) { 		// BU_DET//MISRA-C:2004 Rule 13.2 
		if(nstPowerInterrupt_fp != NULL){
			nstPowerInterrupt_fp(0, 0);
		}
	}

	/* external source */
}

void SetPMGPIO_Interrupt_Handler(void)
{
	uint8 bool_ret;//MISRA-C:2004 Rule 16.10 
	bool_ret = BSP_IntVectSet(PMGPIO_INTERRUPT_SRC, 0, INT_TYPE_EDGE_FALLING, PMGPIO_Interrupt_Handler, 0);
	if ( bool_ret == 1)
	{
		BSP_IntSrcEn(PMGPIO_INTERRUPT_SRC);
	}
}

void EarlyWakeUp(void)
{
	// Initialize to PMGPIO interrupt register
	InitializePMGPIO(1);

	return;
}

#if (TEMP_COMPILE_SWITCH_05WORK_10WORK == TEMP_COMPILE_SWITCH_05WORK)
void PowerDown(int32 mode)
{
	uint32 outputLowPort = 0, outputHighPort = 0, inputPort = 0, pullDownPort = 0, pullUpPort = 0;
	
	HwGPK_IRQEN &= ~(Hw_ACC_DET | Hw_BU_DET/* | external source */);  //Interrupt disable

	if (mode == Hw_ACC_DET)
	{
		HwPMGPIO_CTL &= ~PMGPIO_CTRLREG_DOWN_SOFT_MASK;
		HwPMGPIO_CTL |= (PM_PWRDN_ACC_OFF<<PMGPIO_CTRLREG_DOWN_SHIFT); //save power down reason for ACC OFF
	}
	else{
		HwPMGPIO_CTL &= ~PMGPIO_CTRLREG_DOWN_SOFT_MASK;
		HwPMGPIO_CTL |= (PM_PWRDN_BU_OFF<<PMGPIO_CTRLREG_DOWN_SHIFT); //save power down reason for ACC OFF
	}

	tcc_pr_info("******Power Down*******\n");

	/* check ACC port - If ACC goes on and event is cleared and powered down after setting the wakeup event, system cannot wake up. */
	if (((HwPMGPIO_DAT & (Hw_ACC_DET/* | external source */)) != 0) && (gpio_get(PM_HI_DET) == GPIO_LOW)) {  //MISRA-C:2004 Rule 13.2 
		//HwGPK_IRQEN |= (Hw_ACC_DET | Hw_BU_DET/* | external source */);  // interrupt enable
		OS_SleepTask(PMGPIO_CHECK_WAIT);
		if (((HwPMGPIO_DAT & (Hw_ACC_DET/* | external source */)) != 0) && (gpio_get(PM_HI_DET) == GPIO_LOW)){
			wvdSysPowerReset(PM_PWRDN_ACC_OFF);
			return;
		}
	}

	/* set wake-up event */
	HwPMGPIO_POL &= ~(Hw_ACC_DET | Hw_BU_DET/* | external source */);  //use HwPMGPIO_STR to control PCTL[1:0]
	HwPMGPIO_STR |= (Hw_ACC_DET | Hw_BU_DET/* | external source */);  // Clear evnet status

	/* clear PMGPIO(Alive GPIO) Port to reduce power leakage current on power down mode */
	/* Power leakage current must be checked on user's hardware environment */
	/********************************************
	Hw18	//CAN2_RX
	Hw17	//CAN2_TX
	Hw16	//CAN1_RX
	Hw15	//CAN1_TX
	Hw14	//CAN0_RX
	Hw13	//CAN0_TX
	Hw12	//TRUNK_OPEN_DET
	Hw11	//LIN_SLP
	Hw10	//LIN_RX
	Hw9		//LIN_TX
	Hw8		//CAN0_STB
	Hw7		//DOOR_OPEN_DET
	Hw6		//CAN0_INH
	Hw5		//EMG_WAKE_IN
	Hw4		//IGN_DET
	Hw3		//ACC_DET
	Hw2		//GPIO_K02
	Hw1		//BU_DET0
	Hw0		//GPIO_K00
	*********************************************/ //QAC

	HwPMGPIO_FS0 = HwZERO;  // GPIOK pin controlled by PMGPIO_DAT & PMGPIO_DOE 

	inputPort = (Hw18 | Hw16 | Hw14 | Hw12 | Hw10 | Hw7 | Hw6 | Hw5 | Hw4 | Hw3 | Hw1 | Hw0);
	outputLowPort = (Hw17 | Hw15 | Hw13 | Hw11 | Hw9);
	outputHighPort = (Hw8 | Hw2);
	pullDownPort = (Hw18 | Hw16 | Hw12 | Hw10 | Hw7 | Hw6 | Hw4);
	pullUpPort = (Hw14);
	
	HwPMGPIO_DOE &= ~(inputPort);  // Set to input mode
	HwPMGPIO_DIE |= (inputPort);  // Enable to input mode
					
	HwPMGPIO_DOE |= (outputLowPort | outputHighPort);  // Set to output mode

	HwPMGPIO_DAT &= ~(outputLowPort);  // Set to low
	HwPMGPIO_DAT |= (outputHighPort);  // Set to high

	HwPMGPIO_PS |= (pullUpPort);  // Set to pull-up
					
	HwPMGPIO_PS &= ~(pullDownPort); // Set to pull-down

	HwPMGPIO_PE = (pullDownPort | pullUpPort);  // Set pull up/down enable

 	/* set PCTL */
	HwPMGPIO_FS0_H = ~(Hw_PCTL_1 | Hw_PCTL_0);  //0: PCTL[1:0] pin controlled by PMGPIO_DAT_H/DOE_H
	HwPMGPIO_DOE_H = (Hw_PCTL_1 | Hw_PCTL_0);  //PCTL[1:0] output

	tcc_pr_info("******Power Down:HwPMGPIO_CTL=%x*******\n", HwPMGPIO_CTL);
 
	HwPMGPIO_CTL |= Hw0;  //disable Latch
	HwPMGPIO_CTL |= Hw1;  //disable LVS
	
	HwPMGPIO_EE0 = (Hw_ACC_DET | Hw_BU_DET/* | external source */);  //Enabled pin can change PCTL[1] output from low to high 
	
	HwPMGPIO_DAT_H = (~Hw_PCTL_1 | Hw_PCTL_0); // Power on state > PCTL[1] = HIGH , PCTL[0] = LOW

	while (1) {
		NOP_DELAY(); // Codesonar : Empty while Statement
	}

	return;
}

void InitializePMGPIO(int32 boot)
{
	if (boot != 0) { //MISRA-C:2004 Rule 13.2  
		HwPMGPIO_FS0_H = ~(Hw_PCTL_1 | Hw_PCTL_0);  //0: PCTL[1:0] pin controlled by PMGPIO_DAT_H/DOE_H
		HwPMGPIO_DOE_H = (Hw_PCTL_1 | Hw_PCTL_0);  //PCTL[1:0] output
		HwPMGPIO_DAT_H = (Hw_PCTL_1 | ~Hw_PCTL_0); // Power on state > PCTL[1] = HIGH , PCTL[0] = LOW
		
		HwPMGPIO_CTL &= ~Hw0;  //enable Latch
		HwPMGPIO_CTL &= ~Hw1;  //enable LVS
		
		HwPMGPIO_FS0 = 0xFFFFFFFF;  //normal GPIOK port

		#if 0 //check the wake up source
		{  
			uint32 rigingEvent = 0, fallingEvent;
			
			rigingEvent = HwPMGPIO_STR;
			fallingEvent = HwPMGPIO_STF;
			if((rigingEvent & Hw_ACC_DET) != 0)
			{
			    //wake up source is ACC
			}
			if((fallingEvent & Hw_ACC_DET) != 0)
			{
			    //wake up source is ACC
			}
		} 
		#endif

	}
	else {
		HwPMGPIO_CTL |= Hw5;  //OFFCTL[1:0], Once PCTL[1:0] output change from high to low state, keep low until PRST# active

		//HwPMGPIO_CTL &= ~(PM_PWRDN_ACC_OFF<<24); //Clear power down reason information
		HwPMGPIO_CTL &= ~(PMGPIO_CTRLREG_DOWN_SOFT_MASK); //Clear power down reason information

		//Set PMGPIO port
		HwPMGPIO_FS0 &= ~(Hw_ACC_DET|Hw_BU_DET/*|external source*/);  //GPIOK[1:0] pin controlled by PMGPIO_DAT/DOE
		HwPMGPIO_DOE &= ~(Hw_ACC_DET|Hw_BU_DET/*|external source*/);  //GPIOK[1:0] pin input

		//Clear Event
		HwPMGPIO_STR = 0xFFFFFFFF;
		HwPMGPIO_STF = 0xFFFFFFFF;

		//Set Interrupt
		HwGPK_FBP &= ~(Hw_ACC_DET|Hw_BU_DET/*|external source*/);  //enable interrupt source & filter
		HwGPK_IRQST |= (Hw_ACC_DET|Hw_BU_DET/*|external source*/);  // clear interrupt
		HwGPK_IRQPOL |= (Hw_ACC_DET|Hw_BU_DET/*|external source*/);  //interrupt polarity inverted (when changed high->low)
		HwGPK_IRQTM0 |= (Hw_ACC_DET|Hw_BU_DET/*|external source*/); // 0: edge trigger    1: level trigger
		HwGPK_IRQEN |= (Hw_ACC_DET | Hw_BU_DET/*|external source*/); // interrupt enable
	}

	return;
}

#else
/********************************************************************************************/
/**	\function		PowerDown
 *	\date			2021/11/11
 *	\author			xulei
 *	\description	PowerDown fun
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *		 0001		 - 	 		wangying		 New
 *		 0002	 20220826 		 xulei		 	 Robust Question No.7
 *												　1.Hw_ACC_DET and Hw_HI_DET Interrupt are not used，Interrupt disable is not required，Remove the Hw_ACC_DET and Hw_HI_DET interruption prohibitions.
 *											 	 2.During the power-down, additional judgment was found to be Hw_BU_DET detected when power-up again.
 ********************************************************************************************/

void PowerDown(int32 mode)
{
	uint32 outputLowPort = 0, outputHighPort = 0, inputPort = 0, pullDownPort = 0, pullUpPort = 0;

	//HwGPK_IRQEN &= ~(Hw_ACC_DET | Hw_BU_DET | Hw_HI_DET/* | external source */);  //Interrupt disable
	HwGPK_IRQEN &= ~(Hw_BU_DET /* | external source */);  //Interrupt disable /*20220826 xulei No.0002-1*/

	if (mode == Hw_BU_DET){
		HwPMGPIO_CTL &= ~PMGPIO_CTRLREG_DOWN_SOFT_MASK;
		HwPMGPIO_CTL |= (PM_PWRDN_BU_OFF<<PMGPIO_CTRLREG_DOWN_SHIFT); //save power down reason for ACC OFF
		
		/* check ACC port - If ACC goes on and event is cleared and powered down after setting the wakeup event, system cannot wake up. */
		//if (((HwPMGPIO_DAT & (Hw_ACC_DET/* | external source */)) != 0) && ((HwPMGPIO_DAT & (Hw_HI_DET/* | external source */)) != 0)&&((HwPMGPIO_DAT & (Hw_BU_DET/* | external source */)) != 0)) {  //MISRA-C:2004 Rule 13.2 
		if (HwPMGPIO_DAT == (Hw_ACC_DET|Hw_HI_DET|Hw_BU_DET)){				 /*20220826 xulei No.0002-2*/
			wvdSysPowerReset(PM_PWRDN_BU_OFF);
			return;
		}
	}
	else{
		HwPMGPIO_CTL &= ~PMGPIO_CTRLREG_DOWN_SOFT_MASK;
		HwPMGPIO_CTL |= (PM_PWRDN_ACC_OFF<<PMGPIO_CTRLREG_DOWN_SHIFT); //save power down reason for ACC OFF

		/* check ACC port - If ACC goes on and event is cleared and powered down after setting the wakeup event, system cannot wake up. */
		if (((HwPMGPIO_DAT & (Hw_ACC_DET/* | external source */)) != 0) && ((HwPMGPIO_DAT & (Hw_HI_DET/* | external source */)) != 0)) {  //MISRA-C:2004 Rule 13.2 
			//HwGPK_IRQEN |= (Hw_ACC_DET | Hw_BU_DET/* | external source */);  // interrupt enable
			OS_WaitTime(PMGPIO_CHECK_WAIT);
			if (((HwPMGPIO_DAT & (Hw_ACC_DET/* | external source */)) != 0) && ((HwPMGPIO_DAT & (Hw_HI_DET/* | external source */)) != 0)){
				wvdSysPowerReset(PM_PWRDN_ACC_OFF);
				return;
			}
		}
	}

	/* set wake-up event */
	HwPMGPIO_POL &= ~(Hw_ACC_DET | Hw_BU_DET | Hw_HI_DET/* | external source */);  //use HwPMGPIO_STR to control PCTL[1:0]
	HwPMGPIO_STR |= (Hw_ACC_DET | Hw_BU_DET | Hw_HI_DET/* | external source */);  // Clear evnet status

	/* clear PMGPIO(Alive GPIO) Port to reduce power leakage current on power down mode */
	/* Power leakage current must be checked on user's hardware environment */
	/********************************************
	Hw18	//CAN2_RX
	Hw17	//CAN2_TX
	Hw16	//CAN1_RX
	Hw15	//CAN1_TX
	Hw14	//CAN0_RX
	Hw13	//CAN0_TX
	Hw12	//TRUNK_OPEN_DET
	Hw11	//LIN_SLP
	Hw10	//LIN_RX
	Hw9		//LIN_TX
	Hw8		//CAN0_STB
	Hw7		//DOOR_OPEN_DET
	Hw6		//CAN0_INH
	Hw5		//EMG_WAKE_IN
	Hw4		//IGN_DET
	Hw3		//ACC_DET	-> GPIO_K03
	Hw2		//GPIO_K02	-> ACC_DET
	Hw1		//BU_DET0	
	Hw0		//GPIO_K00 	-> HI_DET
	*********************************************/ //QAC

	HwPMGPIO_FS0 = HwZERO;  // GPIOK pin controlled by PMGPIO_DAT & PMGPIO_DOE 

	inputPort = (Hw2 | Hw1 | Hw0);
	outputLowPort = (Hw18 | Hw17 | Hw16 | Hw15 | Hw14 | Hw13 | Hw12 | Hw11| Hw10 | Hw9| Hw8 | Hw7 | Hw6 | Hw5 | Hw4 | Hw3);
	
	HwPMGPIO_DOE &= ~(inputPort);  // Set to input mode
	HwPMGPIO_DIE |= (inputPort);  // Enable to input mode
					
	HwPMGPIO_DOE |= (outputLowPort | outputHighPort);  // Set to output mode

	HwPMGPIO_DAT &= ~(outputLowPort);  // Set to low
	HwPMGPIO_DAT |= (outputHighPort);  // Set to high

	HwPMGPIO_PS |= (pullUpPort);  // Set to pull-up
					
	HwPMGPIO_PS &= ~(pullDownPort); // Set to pull-down

	HwPMGPIO_PE = (pullDownPort | pullUpPort);  // Set pull up/down enable

 	/* set PCTL */
	HwPMGPIO_FS0_H = ~(Hw_PCTL_1 | Hw_PCTL_0);  //0: PCTL[1:0] pin controlled by PMGPIO_DAT_H/DOE_H
	HwPMGPIO_DOE_H = (Hw_PCTL_1 | Hw_PCTL_0);  //PCTL[1:0] output

	//tcc_pr_info("******Power Down:HwPMGPIO_CTL=%x******\n", HwPMGPIO_CTL);
 
	HwPMGPIO_CTL |= Hw0;  //disable Latch
	HwPMGPIO_CTL |= Hw1;  //disable LVS
	
	HwPMGPIO_EE0 = (Hw_ACC_DET | Hw_BU_DET | Hw_HI_DET/* | external source */);  //Enabled pin can change PCTL[1] output from low to high 
	
	HwPMGPIO_DAT_H = (~Hw_PCTL_1 | Hw_PCTL_0); // Power on state > PCTL[1] = HIGH , PCTL[0] = LOW

	while (1) {
		NOP_DELAY(); // Codesonar : Empty while Statement
	}

	return;
}

void InitializePMGPIO(int32 boot)
{
	if (boot != 0) { //MISRA-C:2004 Rule 13.2  
		HwPMGPIO_FS0_H = ~(Hw_PCTL_1 | Hw_PCTL_0);  //0: PCTL[1:0] pin controlled by PMGPIO_DAT_H/DOE_H
		HwPMGPIO_DOE_H = (Hw_PCTL_1 | Hw_PCTL_0);  //PCTL[1:0] output
		HwPMGPIO_DAT_H = (Hw_PCTL_1 | ~Hw_PCTL_0); // Power on state > PCTL[1] = HIGH , PCTL[0] = LOW
		
		HwPMGPIO_CTL &= ~Hw0;  //enable Latch
		HwPMGPIO_CTL &= ~Hw1;  //enable LVS
		
		HwPMGPIO_FS0 = 0xFFFFFFFF;  //normal GPIOK port

		#if 0 //check the wake up source
		{  
			uint32 rigingEvent = 0, fallingEvent;
			
			rigingEvent = HwPMGPIO_STR;
			fallingEvent = HwPMGPIO_STF;
			if((rigingEvent & Hw_ACC_DET) != 0)
			{
			    //wake up source is ACC
			}
			if((fallingEvent & Hw_ACC_DET) != 0)
			{
			    //wake up source is ACC
			}
		} 
		#endif

	}
	else {
		HwPMGPIO_CTL |= Hw5;  //OFFCTL[1:0], Once PCTL[1:0] output change from high to low state, keep low until PRST# active

		//HwPMGPIO_CTL &= ~(PM_PWRDN_ACC_OFF<<24); //Clear power down reason information
		HwPMGPIO_CTL &= ~(PMGPIO_CTRLREG_DOWN_SOFT_MASK); //Clear power down reason information

		//Set PMGPIO port
		HwPMGPIO_FS0 &= ~(Hw_ACC_DET|Hw_BU_DET|Hw_HI_DET/*|external source*/);  //GPIOK[1:0] pin controlled by PMGPIO_DAT/DOE
		HwPMGPIO_DOE &= ~(Hw_ACC_DET|Hw_BU_DET|Hw_HI_DET/*|external source*/);  //GPIOK[1:0] pin input

		//Clear Event
		HwPMGPIO_STR = 0xFFFFFFFF;
		HwPMGPIO_STF = 0xFFFFFFFF;

		//Set Interrupt
		HwGPK_FBP &= ~(Hw_BU_DET/*|external source*/);  //enable interrupt source & filter
		HwGPK_IRQST &= ~(Hw_BU_DET/*|external source*/);  // clear interrupt
		HwGPK_IRQPOL |= (Hw_BU_DET/*|external source*/);  //interrupt polarity inverted (when changed high->low)
		HwGPK_IRQTM0 |= (Hw_BU_DET/*|external source*/); // 0: edge trigger	1: level trigger
		HwGPK_IRQEN |= (Hw_BU_DET/*|external source*/); // interrupt enable
	}

	return;
}
#endif

uint8 PmGetBattDat(void)
{
	uint8  aubRet = RES_NG;

	if ((HwPMGPIO_DAT & (Hw_BU_DET)) == 0){
		aubRet = RES_OK;
	}

	return aubRet;
	
}

#if 0 /* misiyo*/
/**************************************
int32 PMIO_sel_config(uint32 mode, uint32 gpio, uint32 pmgp_dir)

- mode (gpio_11 is fixed to 1)
1 : GPIO_K
0 : PMGPIO

- gpio (gpio_11 is not able to be controlled)
range : 0~18

-pmgp_dir (pmgpio_11 is always output mode)
0 : input
1 : output

**************************************/
int32 PMIO_sel_config(uint32 mode, uint32 gpio, uint32 pmgp_dir)
{
	int32 ret=-1;
	
	if(gpio<19)
	{
		if(mode==1)
		{	
			HwPMGPIO_FS0=(HwPMGPIO_FS0|(1<<gpio));
			return 0;
		} 
		else if(mode==0) 
		{
			if(pmgp_dir==0)
			{
				HwPMGPIO_DOE=(HwPMGPIO_DOE&~(1<<gpio));
			}
			else if(pmgp_dir==1)
			{
				HwPMGPIO_DOE=(HwPMGPIO_DOE|(1<<gpio));
			}
			else
			{
				; // Empty Statement
			}
			HwPMGPIO_FS0=(HwPMGPIO_FS0&~(1<<gpio));
			return 0;
		}
		else
		{
			; // Empty Statement
		}		
	}

	return ret;
}


/**************************************
int32 PMIO_port_config(uint32 config)

- PEmode 
0 : disable
1 : enable

- PEgpio 
PMGPIO PULL Up/Down Enable. 0~18

- PSmode
0 : pull-down
1 : pull-up

- PSgpio 
PMGPIO PULL Up/Down. 0~18

- CD0_mode
0 : setting '0' at CD0gpio
1 : setting '1' at CD0gpio

- CD1_mode
0 : setting '0' at CD1gpio
1 : setting '1' at CD1gpio

- CD0_gpio
PMGPIO Driver Strength0 0~18

- CD1_gpio
PMGPIO Driver Strength1 0~18

------------------------------------------
CD0_gpio | CD1_gpio |  description
    0    |     0    |  x1 Driving Strength
    0    |     1    |  x2 Driving Strength
    1    |     0    |  x4 Driving Strength
    1    |     1    |  x8 Driving Strength
------------------------------------------

uint32 config :
0000 0000 0000 0000 0000 0000 0000 0000
          ||     ||      ||     ||
          ||     ||      ||     |PSgpio
          ||     ||      ||     PSmode
          ||     ||      |PEgpio
          ||     ||      PEmode
          ||     |CD0gpio
          ||     CD0mode
          |CD1gpio
          CD1mode
           
**************************************/
int32 PMIO_port_config(uint32 config)
{
	
	//parsing parameter

	uint32 PE_mode=(config&(1<<11)>>11);
	uint32 PE_mask=0x1F<<6;
	uint32 PS_mode=(config&(1<<5)>>5);
	uint32 PS_mask=0x1F;
	uint32 PE_gpio=((config&PE_mask)>>6);
	uint32 PS_gpio=(config&PS_mask);

	uint32 CD0_mask=0x1F<<12;
	uint32 CD0_mode=(config&(1<<17)>>17);
	uint32 CD0_gpio=((config&CD0_mask)>>12);

	uint32 CD1_mask=0x1F<<18;
	uint32 CD1_mode=(config&(1<<23)>>23);
	uint32 CD1_gpio=((config&CD1_mask)>>18);

	//setting registers

	if(PE_mode==1)
	{
		HwGPIOK_PE |= 1<<PE_gpio;
	}
	else //if(PE_mode==0) // QAC : the result of this logical operation is always true
	{
		HwGPIOK_PE &= ~(1<<PE_gpio);
	}

	if(PS_mode==1)
	{
		HwGPIOK_PS |= 1<<PS_gpio;
	}
	else //if(PS_mode==0)// QAC : the result of this logical operation is always true
	{
		HwGPIOK_PS &= ~(1<<PS_gpio);
	}

	if(CD0_mode==1)
	{
		HwPMGPIO_CD0 |= 1<<CD0_gpio;
	}
	else //if(CD0_mode==0)// QAC : the result of this logical operation is always true
	{
		HwPMGPIO_CD0 &= ~(1<<CD0_gpio);
	}

	if(CD1_mode==1)
	{
		HwPMGPIO_CD1 |= 1<<CD1_gpio;
	}
	else //if(CD1_mode==0)// QAC : the result of this logical operation is always true
	{
		HwPMGPIO_CD1 &= ~(1<<CD1_gpio);
	}

	return 0; // qac : non-void return
}


/**************************************
int32 PMIO_GPK_f_read(uint32 gpio)
: Filtered GPIOK[15:0] input data. 
The corresponding bit of GPK_FBP register 
should be ??0?? (filter enabled)

- gpio
range : 0~15

**************************************/
int32 PMIO_GPK_f_read(uint32 gpio)
{
	int32 ret=-1;
	uint32 backup;
	uint32 f_val_mask=(1<<gpio);
	uint32 f_val;

	if(gpio<16)
	{
		backup=HwGPK_FBP;
		HwGPK_FBP &= ~(1<<gpio);
		f_val=HwGPK_FDAT&f_val_mask;
		HwGPK_FBP=backup;

		return f_val;
	}
	return ret;
}
#endif

uint8 wubSysPowerGetPinStstus(uint8 pin)
{
	uint8 level = PM_GPIO_STS_NG;
	
	if(pin == SYS_POWERPIN_BATT){
		if((HwPMGPIO_DAT & Hw_BU_DET) != 0){
			level = GPIO_HIGH;
		}
		else{
			level = GPIO_LOW;
		}
	}
	else if(pin == SYS_POWERPIN_ACC){
		if((HwPMGPIO_DAT & Hw_ACC_DET) != 0){
			level = GPIO_HIGH;
		}
		else{
			level = GPIO_LOW;
		}
	}
	else if(pin == SYS_POWERPIN_HI){
		if((HwPMGPIO_DAT & Hw_HI_DET) != 0){
			level = GPIO_HIGH;
		}
		else{
			level = GPIO_LOW;
		}
	}
	else{
		level = PM_GPIO_STS_NG;
	}

	return level;

}


void wvdSysPowerReset(uint8 type)
{
	HwPMGPIO_CTL &= ~PMGPIO_CTRLREG_DOWN_SOFT_MASK;
	HwPMGPIO_CTL |= (type << PMGPIO_CTRLREG_DOWN_SHIFT); //save power down reason for Soft Reset

	writel(0x01, WARM_RESET_REG);
}

void wvdSysPowerSetWarmFlag(void)
{
	HwPMGPIO_CTL |= PMGPIO_CTRLREG_DOWN_BATT_MASK;
}

uint32 wubGetStartInfo(void *pSts0, void *pSts1, void *pSts2)
{
	uint32 softRet, batRest, rstSts0, rstSts1, rstSts2;
	uint32 * pRstStat0 = (uint32 *)pSts0;
	uint32 * pRstStat1 = (uint32 *)pSts1;
	uint32 * pRstStat2 = (uint32 *)pSts2;

	rstSts0 = readl(RST_STS0);
	rstSts1 = readl(RST_STS1);
	rstSts2 = readl(RST_STS2);
	
	if((pRstStat0 != NULL) && (pRstStat1 != NULL) && (pRstStat2 != NULL)){
		*pRstStat0 = rstSts0;
		*pRstStat1 = rstSts1;
		*pRstStat2 = rstSts2;
	}

	softRet = (readl(PMGPIO_CTL_REG) & PMGPIO_CTRLREG_DOWN_SOFT_MASK) >> PMGPIO_CTRLREG_DOWN_SHIFT;
	batRest = (readl(PMGPIO_CTL_REG) & PMGPIO_CTRLREG_DOWN_BATT_MASK) >> PMGPIO_CTRLREG_DOWN_SHIFT;

	if(batRest == 0){
		tcc_pr_info("Boot reason is Batt Long\n");
		return PM_PWRDN_NO;
	}
	tcc_pr_info("Boot reason is watchdog:sts0=%x, sts1=%x, sts2=%x\n", rstSts0, rstSts1, rstSts2); //watchdog reset

	switch(softRet)
	{
		case PM_PWRDN_ACC_OFF:
			tcc_pr_info("Boot reason is Acc or HDET\n");
			break;

		case PM_PWRDN_BU_OFF:
			tcc_pr_info("Boot reason is Batt Short\n");
			break;

		case PM_PWRDN_UPDATE_RESET:
			tcc_pr_info("Boot reason is update\n");
			break;

		case PM_PWRDN_FACTORY_RESET:
			tcc_pr_info("Boot reason is factory recover\n");
			break;

		case PM_PWRDN_A53_REBOOT_REQ:
			tcc_pr_info("Boot reason is A53 Reboot Req\n");
			break;

		case PM_PWRDN_A7_REBOOT_REQ:
			tcc_pr_info("Boot reason is A7 Reboot Req\n");
			break;

		case PM_PWRDN_A53_HAERT_TIMOUT:
			tcc_pr_info("Boot reason is A53 Heart Timout\n");
			break;

		case PM_PWRDN_A7_HAERT_TIMOUT:
			tcc_pr_info("Boot reason is A7 Heart Timout\n");
			break;

		case PM_PWRDN_OS_A53WDT:
			tcc_pr_info("Boot reason is A53 WatchDog:sts0=%x, sts1=%x, sts2=%x\n", rstSts0, rstSts1, rstSts2);
			break;

		case PM_PWRDN_OS_A7WDT:
			tcc_pr_info("Boot reason is A7 WatchDog:sts0=%x, sts1=%x, sts2=%x\n", rstSts0, rstSts1, rstSts2);
			break;

		case PM_PWRDN_SOC_START_TIMOUT:
			tcc_pr_info("Boot reason is Soc Start Timout\n");
			break;

		case PM_PWRDN_SOC_STANDBYOUT_TIMOUT:
			tcc_pr_info("Boot reason is Soc StanbyOut Timout\n");
			break;

		case PM_PWRDN_OS_RESET:
			tcc_pr_info("Boot reason is R5 OS error\n");
			break;

		default:
			if((rstSts0 != 0) || (rstSts1 != 0) || (rstSts2 != 0))
			{
				softRet = PM_PWRDN_WDT;
				tcc_pr_info("Boot reason is watchdog:sts0=%x, sts1=%x, sts2=%x\n", rstSts0, rstSts1, rstSts2); //watchdog reset
			}
			else
			{
				softRet = PM_PWRDN_MAX;
				tcc_pr_info("Boot reason is unknow\n");
			}
			break;

	}

	return softRet;
}

void wvdPowerSetCallback(powerInterrupt_fp astFunc_fp)
{
	nstPowerInterrupt_fp = astFunc_fp;
}

