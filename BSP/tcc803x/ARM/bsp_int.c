/*
*********************************************************************************************************
*   FileName    : bsp_int.c
*   Description : 
*********************************************************************************************************
*
*   TCC Version : 1.0
*   Copyright (c) Telechips Inc.
*   All rights reserved 
* 
* This source code contains confidential information of Telechips.
* Any unauthorized use without a written permission of Telechips including not limited to re-
* distribution in source or binary form is strictly prohibited.
* This source code is provided "AS IS" and nothing contained in this source code shall 
* constitute any express or implied warranty of any kind, including without limitation, any warranty 
* of merchantability, fitness for a particular purpose or non-infringement of any patent, copyright 
* or other third party intellectual property right. No warranty is made, express or implied, 
* regarding the information's accuracy, completeness, or performance. 
* In no event shall Telechips be liable for any claim, damages or other liability arising from, out of 
* or in connection with this source code or the use in the source code. 
* This source code is provided subject to the terms of a Mutual Non-Disclosure Agreement 
* between Telechips and Company.
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include  <types.h>
#include <osal.h>

#include  <bsp.h>
#include  <bsp_int.h>

#include  <reg_phys.h>
#include  <gpio.h>

#include "tcc_ckc.h"
#include "clock.h"//tcc_get_clkctrl
/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/
//#define HwMC_GICD           ((volatile MC_GICD          *)(MICOM_BASE_ADDR + 0x901000))
//#define HwMC_GICC           ((volatile MC_GICC          *)(MICOM_BASE_ADDR + 0x902000))
#define  BSP_INT_GIC_DIST_REG		((ARM_REG_GIC_DIST_PTR)(MICOM_BASE_ADDR + 0x901000u))
#define  BSP_INT_GIC_IF_REG			((ARM_REG_GIC_IF_PTR)(MICOM_BASE_ADDR + 0x902000u))

#define  BSP_INT_SRC_CNT			224u  /* GIC400 */
//#define  BSP_INT_SWI_SRC_CNT		32
//#define  BSP_INT_PRIO_MASK			0x000000F8U

/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/
//static void BSP_IntPolarity(uint32 id, uint32 val);
static void  BSP_GICIntPrioSet (uint32  int_id, uint32  prio);
static void  BSP_GICIntConfigSet (uint32  int_id, uint8  int_type);
static uint32  BSP_GICIntAck(void);
static void BSP_GICIntEOI(uint32  int_id);
//static uint8 BSP_IntDebounce(uint32 u32_en_fil, uint32 u32_n_mod, uint32 u32_num, uint32 u32_time);  // IntDebounce => BSP_IntDebounce renaming 

/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

struct BSP_INT_VECT_PTR {
	BSP_INT_FNCT_PTR func;
	uint8 IsBothEdge;
	void *arg;
};

static struct BSP_INT_VECT_PTR BSP_IntVectTbl[INT_SRC_CNT]={0};       /* Interrupt vector table.                              */

                                                                /* ------------ GIC DISTRIBUTOR INTERFACE ------------- */
typedef  struct  arm_reg_gic_dist {
    uint32  ICDDCR;                                          /* Distributor Control Register.                        */
    uint32  ICDICTR;                                         /* Interrupt Controller Type Register.                  */
    uint32  ICDIIDR;                                         /* Distributor Implementer Identification Register.     */
    uint32  RSVD1[29];                                       /* Reserved.                                            */
    uint32  ICDISRn[32];                                     /* Interrupt Security Registers.                        */
    uint32  ICDISERn[32];                                    /* Interrupt Set-Enable Registers.                      */
    uint32  ICDICERn[32];                                    /* Interrupt Clear-Enable Registers.                    */
    uint32  ICDISPRn[32];                                    /* Interrupt Set-Pending Registers.                     */
    uint32  ICDICPRn[32];                                    /* Interrupt Clear-Pending Registers.                   */
    uint32  ICDABRn[32];                                     /* Active Bit Registers.                                */
    uint32  RSVD2[32];                                       /* Reserved.                                            */
    uint32  ICDIPRn[255];                                    /* Interrupt Priority Registers.                        */
    uint32  RSVD3[1];                                        /* Reserved.                                            */
    uint32  ICDIPTRn[255];                                   /* Interrupt Processor Target Registers.                */
    uint32  RSVD4[1];                                        /* Reserved.                                            */
    uint32  ICDICFRn[64];                                    /* Interrupt Configuration Registers.                   */
    uint32  RSVD5[128];                                      /* Reserved.                                            */
    uint32  ICDSGIR;                                         /* Software Generate Interrupt Register.                */
    uint32  RSVD6[51];                                       /* Reserved.                                            */
} ARM_REG_GIC_DIST, *ARM_REG_GIC_DIST_PTR;


                                                                /* ---------------- GIC CPU INTERFACE ----------------- */
typedef  struct  arm_reg_gic_if {
    uint32  ICCICR;                                          /* CPU Interface Control Register.                      */
    uint32  ICCPMR;                                          /* Interrupt Priority Mask Register.                    */
    uint32  ICCBPR;                                          /* Binary Point Register.                               */
    uint32  ICCIAR;                                          /* Interrupt Acknowledge Register.                      */
    uint32  ICCEOIR;                                         /* End Interrupt Register.                              */
    uint32  ICCRPR;                                          /* Running Priority Register.                           */
    uint32  ICCHPIR;                                         /* Highest Pending Interrupt Register.                  */
    uint32  ICCABPR;                                         /* Aliased Binary Point Register.                       */
    uint32  RSVD[55];                                        /* Reserved.                                            */
    uint32  ICCIIDR;                                         /* CPU Interface Identification Register.               */
} ARM_REG_GIC_IF, *ARM_REG_GIC_IF_PTR;


                                                                /* ----------- DISTRIBUTOR CONTROL REGISTER ----------- */
#define  ARM_BIT_GIC_DIST_ICDDCR_EN            BSP_BIT_00       /* Global GIC enable.                                   */



                                                                /* ---------- CPU_INTERFACE_CONTROL_REGISTER ---------- */
#define  ARM_BIT_GIC_IF_ICCICR_ENS             BSP_BIT_00       /* Enable secure interrupts.                            */
#define  ARM_BIT_GIC_IF_ICCICR_ENNS            BSP_BIT_01       /* Enable non-secure interrupts.                        */
#define  ARM_BIT_GIC_IF_ICCICR_ACKCTL          BSP_BIT_02       /* Secure ack of NS interrupts.                         */

#if 0 //Msg(4:3218) File scope static, 'ExtIntr', only accessed in one function. MISRA-C:2004 Rule 8.7
static const uint32 ExtIntr[] = {
	/*A : 0 ~31 GPIO_A[31:00] */
	TCC_GPA(0) ,TCC_GPA(1) ,TCC_GPA(2) ,TCC_GPA(3) ,TCC_GPA(4) ,TCC_GPA(5) ,TCC_GPA(6) ,TCC_GPA(7) ,  // 0~
	TCC_GPA(8) ,TCC_GPA(9) ,TCC_GPA(10),TCC_GPA(11),TCC_GPA(12),TCC_GPA(13),TCC_GPA(14),TCC_GPA(15),
	TCC_GPA(16),TCC_GPA(17) ,TCC_GPA(18),TCC_GPA(18),TCC_GPA(20),TCC_GPA(21),TCC_GPA(22),TCC_GPA(23),
	TCC_GPA(24),TCC_GPA(25) ,TCC_GPA(26),TCC_GPA(27),TCC_GPA(28),TCC_GPA(29),TCC_GPA(30),TCC_GPA(31),
	/*B :  32 ~ 60 GPIO_B[28:00]*/
	TCC_GPB(0) ,TCC_GPB(1) ,TCC_GPB(2) ,TCC_GPB(3) ,TCC_GPB(4) ,TCC_GPB(5) ,TCC_GPB(6) ,TCC_GPB(7) ,  // ~ 32
	TCC_GPB(8) ,TCC_GPB(9) ,TCC_GPB(10),TCC_GPB(11),TCC_GPB(12),TCC_GPB(13),TCC_GPB(14), TCC_GPB(15),
	TCC_GPB(16),TCC_GPB(17),TCC_GPB(18),TCC_GPB(19),TCC_GPB(20),TCC_GPB(21),TCC_GPB(22), TCC_GPB(23),
	TCC_GPB(24),TCC_GPB(25),TCC_GPB(26),TCC_GPB(27),TCC_GPB(28),
	/*C :  61 ~ 90 GPIO_C[29:00]*/
	TCC_GPC(0),TCC_GPC(1) ,TCC_GPC(2) ,TCC_GPC(3) ,TCC_GPC(4) ,TCC_GPC(5) ,TCC_GPC(6) ,TCC_GPC(7) ,	// ~ 61
	TCC_GPC(8), TCC_GPC(9) ,TCC_GPC(10),TCC_GPC(11),TCC_GPC(12),TCC_GPC(13),TCC_GPC(14),TCC_GPC(15),
	TCC_GPC(16),TCC_GPC(17),TCC_GPC(18),TCC_GPC(19),TCC_GPC(20),TCC_GPC(21),TCC_GPC(22),TCC_GPC(23),
	TCC_GPC(24), TCC_GPC(25),TCC_GPC(26),TCC_GPC(27),TCC_GPC(28),TCC_GPC(29),
	/*E :  91 ~ 110 GPIO_E[19:00]*/
	TCC_GPE(0) ,TCC_GPE(1) ,TCC_GPE(2) ,TCC_GPE(3) ,TCC_GPE(4) ,TCC_GPE(5) ,TCC_GPE(6) ,TCC_GPE(7) , // ~ 91
	TCC_GPE(8) ,TCC_GPE(9) ,TCC_GPE(10),TCC_GPE(11),TCC_GPE(12),TCC_GPE(13),TCC_GPE(14),TCC_GPE(15),
	TCC_GPE(16),TCC_GPE(17),TCC_GPE(18),TCC_GPE(19),
	/*E :  111~121 GPIO_G[10:00]*/
	TCC_GPG(0) ,TCC_GPG(1) ,TCC_GPG(2) ,TCC_GPG(3) ,TCC_GPG(4),TCC_GPG(5) ,TCC_GPG(6) ,TCC_GPG(7), // ~ 111
	TCC_GPG(8) ,TCC_GPG(9) ,TCC_GPG(10), 
	/*E :  122~133 GPIO_H[11:00]*/
	TCC_GPH(0) ,TCC_GPH(1) ,TCC_GPH(2) ,TCC_GPH(3) ,TCC_GPH(4) ,TCC_GPH(5) ,TCC_GPH(6) ,TCC_GPH(7) , // ~ 122
	TCC_GPH(8) ,TCC_GPH(9) ,TCC_GPH(10),TCC_GPH(11),
	/*K :  134~152 GPIO_K[18:00]*/
	TCC_GPK(0) ,TCC_GPK(1) ,TCC_GPK(2) ,TCC_GPK(3) ,TCC_GPK(4) ,TCC_GPK(5) ,TCC_GPK(6) ,TCC_GPK(7) ,  // ~ 134
	TCC_GPK(8), TCC_GPK(9), TCC_GPK(10),TCC_GPK(11),TCC_GPK(12),TCC_GPK(13),TCC_GPK(14),TCC_GPK(15),
	TCC_GPK(16),TCC_GPK(17),TCC_GPK(18),  
	/*MA:  153~182 GPIO_MA[29:00*/
	TCC_GPMA(0),TCC_GPMA(1),TCC_GPMA(2),TCC_GPMA(3),TCC_GPMA(4), TCC_GPMA(5),TCC_GPMA(6),TCC_GPMA(7), // ~ 153
	TCC_GPMA(8),TCC_GPMA(9),TCC_GPMA(10),TCC_GPMA(11),TCC_GPMA(12),TCC_GPMA(13),TCC_GPMA(14),TCC_GPMA(15),
	TCC_GPMA(16),TCC_GPMA(17),TCC_GPMA(18),TCC_GPMA(19),TCC_GPMA(20),TCC_GPMA(21),TCC_GPMA(22),TCC_GPMA(23),
	TCC_GPMA(24),TCC_GPMA(25),TCC_GPMA(26),TCC_GPMA(27),TCC_GPMA(28),TCC_GPMA(29)
};
#endif
/*
*********************************************************************************************************
*********************************************************************************************************
**                                      LOCAL & GLOBAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/
/* GHC Warning : declared but never referenced
static void BSP_IntPolarity(uint32 id, uint32 val)
{
	// TODO: check if polarity can be supported by VPIC
	#if 0
	uint32 reg;
	uint32 bitmask = (1<<(id%32));

	if (id < GIC_PPI_START) {
		return;
	}
	else if (id < (GIC_PPI_START+32)) {
		reg = TCC_VIC_BASE + 0x38;
	}
	else if (id < (GIC_PPI_START+64)) {
		reg = TCC_VIC_BASE + 0x3C;
	}
	else if (id < (GIC_PPI_START+96)) {
		reg = TCC_SMU_CFG_BASE + 0x0C;
	}
	else if (id < (GIC_PPI_START+128)) {
		reg = TCC_SMU_CFG_BASE + 0x10;
	}
	else if (id < (GIC_PPI_START+160)) {
		reg = TCC_SMU_CFG_BASE + 0x14;
	}
	else {
		return;
	}
	if (val != 0) {  //QAC - Rule 13.2
		writel(readl(reg) | bitmask, reg);
	}
	else {
		writel(readl(reg) & ~bitmask, reg);
	}
	#endif
}
*/
/*
*********************************************************************************************************
*                                          BSP_GICIntPrioSet()
*
* Description : Set interrupt priority.
*
* Argument(s) : int_id      Interrupt id.
*
*               prio        Interrupt priority.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_IntVectSet().
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

static void  BSP_GICIntPrioSet (uint32  int_id,
                         uint32  prio)
{
	uint32  reg_off;
	uint32  reg_byte;
	uint32  temp_reg;

	if (prio >= PRIORITY_NO_MEAN) {
		return;
	}

	/* PPI3 INTs does not set priority */
	if (int_id >= (GIC_SPI_START + BSP_INT_SRC_CNT)) {
		return;
	}

	reg_off = int_id >> 2u;
	reg_byte = int_id & 0x03;

	temp_reg = BSP_INT_GIC_DIST_REG->ICDIPRn[reg_off];
	temp_reg = temp_reg & ~(0xFF << (reg_byte * 8u));
	temp_reg = temp_reg | (((prio<<4) & 0xF0u) << (reg_byte * 8u));

	BSP_INT_GIC_DIST_REG->ICDIPRn[reg_off] = temp_reg;
}


/*
*********************************************************************************************************
*                                         BSP_GICIntConfigSet()
*
* Description : Set interrupt Config.
*
* Argument(s) : int_id              Interrupt id.
*
*               int_config          Interrupt trigger type.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_IntVectSet().
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

static void  BSP_GICIntConfigSet (uint32  int_id,
                           uint8  int_type)
{
	uint32  reg_off;
	uint32  reg_mask;
	uint32  temp_reg;

	reg_off = int_id >> 4u;
	reg_mask = 0x2u << ((int_id & 0xf) << 1u);

	temp_reg = BSP_INT_GIC_DIST_REG->ICDICFRn[reg_off];

	if ((int_type == INT_TYPE_LEVEL_HIGH) || (int_type == INT_TYPE_LEVEL_LOW)) {
		temp_reg = temp_reg & ~reg_mask;
	}
	else {
		temp_reg = temp_reg | reg_mask;
	}

	BSP_INT_GIC_DIST_REG->ICDICFRn[reg_off] = temp_reg;

	/* set polarity */
	// TODO: check if polarity can be supported by VPIC
	#if 0
	if ((int_type == INT_TYPE_LEVEL_LOW) || (int_type == INT_TYPE_EDGE_FALLING)) {
		BSP_IntPolarity(int_id, 1);
	}
	else {
		BSP_IntPolarity(int_id, 0);
	}
	#endif
}


/*
*********************************************************************************************************
*                                            BSP_GICIntAck()
*
* Description : Interrupt acknowledge.
*
* Argument(s) : none.
*
* Return(s)   : Current interrupt.
*
* Caller(s)   : BSP_IntHandler().
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

static uint32  BSP_GICIntAck(void)
{
	uint32 int_id;

	int_id = BSP_INT_GIC_IF_REG->ICCIAR;

	return (int_id);
}


/*
*********************************************************************************************************
*                                            BSP_GICIntEOI()
*
* Description : End of interrupt.
*
* Argument(s) : int_id  Interrupt id.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_IntHandler().
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

static void BSP_GICIntEOI(uint32  int_id)
{
	BSP_INT_GIC_IF_REG->ICCEOIR = int_id;
}


/*
*********************************************************************************************************
*                                            BSP_GICPreInit()
*
* Description : Pre-Initialise interrupts.
*               This function called in secure states. (ARM_TRUSTZONE)
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_GICPreInit().
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

void BSP_GICPreInit(void)
{
	uint32 offset;

	/* dedicated gic */
	for (offset=0 ; offset<8 ; offset++) {
		BSP_INT_GIC_DIST_REG->ICDIPRn[offset] = 0xA0A0A0A0;
	}

	BSP_INT_GIC_IF_REG->ICCPMR = 0xFFu;

	for (offset=0 ; offset<8 ; offset++) {
		BSP_INT_GIC_DIST_REG->ICDISRn[offset] = 0xFFFFFFFF;
	}
}


/*
*********************************************************************************************************
*                                            BSP_Int_Init()
*
* Description : Initialise interrupts.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_Init().
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

void BSP_Int_Init (void)
{
	uint32 offset;

	BSP_INT_GIC_DIST_REG->ICDDCR &= ~ARM_BIT_GIC_DIST_ICDDCR_EN;
	BSP_INT_GIC_DIST_REG->ICDDCR |= ARM_BIT_GIC_DIST_ICDDCR_EN;

	/* set default priority value to 0xA0 */
	for (offset=0 ; offset<((INT_PMGPIO+4)/4) ; offset++) 
	{
		BSP_INT_GIC_DIST_REG->ICDIPRn[offset] = 0xA0A0A0A0;
	}

	BSP_INT_GIC_IF_REG->ICCPMR = 0xFFu;
	BSP_INT_GIC_IF_REG->ICCICR |= ARM_BIT_GIC_IF_ICCICR_ENS;

	CPU_MB();
}



/*
*********************************************************************************************************
*                                            BSP_IntSrcEn()
*
* Description : Enable interrupt source int_id.
*
* Argument(s) : int_id        Interrupt to enable.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_OS_TmrTickInit().
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

void  BSP_IntSrcEn (uint32 int_id)
{
	uint32  reg_off;
	uint32  reg_bit;

	reg_off = int_id >> 5u;                                     /* Calculate the register offset.                       */
	reg_bit = int_id & 0x1F;                                    /* Mask bit ID.                                         */

	BSP_INT_GIC_DIST_REG->ICDISERn[reg_off] = 1u << reg_bit;

	if (BSP_IntVectTbl[int_id].IsBothEdge == BSP_TRUE) {
		reg_off = (int_id+10) >> 5u;                            /* Calculate the register offset.                       */
		reg_bit = (int_id+10) & 0x1F;                           /* Mask bit ID.                                         */

		BSP_INT_GIC_DIST_REG->ICDISERn[reg_off] = 1u << reg_bit;
	}

	CPU_MB();
}


/*
*********************************************************************************************************
*                                            BSP_IntSrcDis()
*
* Description : Disable interrupt source int_id.
*
* Argument(s) : int_id      Interrupt to disable.
*
* Return(s)   : none.
*
* Caller(s)   : none.
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

void  BSP_IntSrcDis (uint32 int_id)
{
	uint32  reg_off;
	uint32  reg_bit;

	reg_off = int_id >> 5u;                                     /* Calculate the register offset.                       */
	reg_bit = int_id & 0x1F;                                    /* Mask bit ID.                                         */

	BSP_INT_GIC_DIST_REG->ICDICERn[reg_off] = 1u << reg_bit;

	if (BSP_IntVectTbl[int_id].IsBothEdge == BSP_TRUE) {
		reg_off = (int_id+10) >> 5u;                            /* Calculate the register offset.                       */
		reg_bit = (int_id+10) & 0x1F;                           /* Mask bit ID.                                         */

		BSP_INT_GIC_DIST_REG->ICDICERn[reg_off] = 1u << reg_bit;
	}
}


/*
*********************************************************************************************************
*                                           BSP_IntPrioSet()
*
* Description : Set interrupt priority.
*
* Argument(s) : int_id  Interrupt id.
*
*               prio    Interrupt priority. (0~15)
*
* Return(s)   : none.
*
* Caller(s)   : none.
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

void  BSP_IntPrioSet(uint32  int_id,
                     uint32  prio)
{
	OSAL_SR_ALLOC();

	OSAL_CRITICAL_ENTER();
	BSP_GICIntPrioSet(int_id, prio);
	OSAL_CRITICAL_EXIT();
}


/*
*********************************************************************************************************
*                                           BSP_IntVectSet()
*
* Description : Configure interrupt vector.
*
* Argument(s) : int_id              Interrupt ID.
*
*               int_prio            Interrupt priority. (0~15)
*
*               int_type            Interrupt trigger type.
*
*               int_fnct            ISR function pointer.
*
* Return(s)   : Interrupt configuration result.
*                                DEF_YES                   Interrupt successfully set.
*                                DEF_NO                    Error setting interrupt.
*
* Caller(s)   : Application.
*
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

uint8  BSP_IntVectSet (uint32       int_id,
                             uint32       int_prio,
                             uint8       int_type,
                             BSP_INT_FNCT_PTR int_fnct,
                             void         *int_arg)
{
	uint32 rev_int_id;

	OSAL_SR_ALLOC();

	if(int_prio > PRIORITY_NO_MEAN) {
		return BSP_NO;
	}

	if(int_id > 1022u) {
		return BSP_NO;
	}

	OSAL_CRITICAL_ENTER();                                       /* Prevent partially configured interrupts.             */

	BSP_GICIntPrioSet(int_id, int_prio);
	BSP_GICIntConfigSet(int_id, int_type);

	BSP_IntVectTbl[int_id].func = int_fnct;
	BSP_IntVectTbl[int_id].arg = int_arg;
	BSP_IntVectTbl[int_id].IsBothEdge = BSP_FALSE;

	if ((int_id >= EINT_START_INT) && (int_id <= EINT_END_INT)  /* Set reversed external interrupt                      */
		&& (int_type == INT_TYPE_EDGE_BOTH)) {                     /*        for supporting both edge.                     */
		// add offset of IRQn
		rev_int_id = int_id + EINT_NUM;

		BSP_GICIntPrioSet(rev_int_id, int_prio);
		BSP_GICIntConfigSet(rev_int_id, int_type);

		BSP_IntVectTbl[rev_int_id].func = int_fnct;
		BSP_IntVectTbl[rev_int_id].arg = int_arg;
		BSP_IntVectTbl[int_id].IsBothEdge = BSP_TRUE;
	}

	OSAL_CRITICAL_EXIT();

	return (BSP_OK);
}


/*
*********************************************************************************************************
*                                           BSP_IntHandler()
*
* Description : Generic interrupt handler.
*
* Argument(s) : Interrupt type.
*
* Return(s)   : none.
*
* Caller(s)   : OS_CPU_ExceptHndlr().
*
* Note(s)     : none.
*
*********************************************************************************************************
*/
void  BSP_IntHandlerWithParam (uint32 ulICCIAR )
{
	uint32        int_ack;
	uint32        int_id;
	BSP_INT_FNCT_PTR  p_isr;
	void          *int_arg;

	int_ack = ulICCIAR;

	int_id = int_ack & OSAL_BIT_FIELD(10u, 0u);                  /* Mask away the CPUID.                                 */

	if(int_id == 1023u) {                                       /* Spurious interrupt.                                  */
		return;
	}

	p_isr = BSP_IntVectTbl[int_id].func;                        /* Fetch ISR handler.                                   */
	int_arg = BSP_IntVectTbl[int_id].arg;

	if(p_isr != BSP_NULL) {
		(*p_isr)(int_arg);                                      /* Call ISR handler.                                    */
	}
	CPU_MB();                                                   /* Memory barrier before ending the interrupt.          */
}

void  BSP_IntHandler (void)
{
	uint32        int_ack;
	uint32        int_id;
	BSP_INT_FNCT_PTR  p_isr;
	void          *int_arg;

	int_ack = BSP_GICIntAck();

	int_id = int_ack & OSAL_BIT_FIELD(10u, 0u);                  /* Mask away the CPUID.                                 */

	if(int_id == 1023u) {                                       /* Spurious interrupt.                                  */
		return;
	}

	p_isr = BSP_IntVectTbl[int_id].func;                        /* Fetch ISR handler.                                   */
	int_arg = BSP_IntVectTbl[int_id].arg;

	if(p_isr != BSP_NULL) {
		(*p_isr)(int_arg);                                      /* Call ISR handler.                                    */
	}

	CPU_MB();                                                   /* Memory barrier before ending the interrupt.          */
	BSP_GICIntEOI(int_ack);
}


/*
*********************************************************************************************************
*                                           BSP_IntExtSet()
*
* Description : Configure External Interrupts.
*
* Argument(s) : int_id              Interrupt ID.
*
*               gpio                gpio id (ex: TCC_GPX())
*
* Return(s)   : Configuration result.
*                                DEF_YES                   Externel Interrupt successfully set.
*                                DEF_NO                    Error setting interrupt.
*
* Caller(s)   : Application.
*
*
* Note(s)     : none.
*
*********************************************************************************************************
*/
uint8 BSP_IntExtSet(uint32 int_id, uint32 gpio)
{
	uint32 *reg = (uint32 *)((TCC_GPIO_BASE + 0x280) + (4*((int_id-INT_EXT0)/4)));//QAC : cast between a pointer to void and an integral type.

	uint32 data, mask, shift, idx;
	//Msg(4:3218) File scope static, 'ExtIntr', only accessed in one function. MISRA-C:2004 Rule 8.7
	static const uint32 ExtIntr[] = {
		/*A : 0 ~31 GPIO_A[31:00] */
		TCC_GPA(0) ,TCC_GPA(1) ,TCC_GPA(2) ,TCC_GPA(3) ,TCC_GPA(4) ,TCC_GPA(5) ,TCC_GPA(6) ,TCC_GPA(7) ,  // 0~
		TCC_GPA(8) ,TCC_GPA(9) ,TCC_GPA(10),TCC_GPA(11),TCC_GPA(12),TCC_GPA(13),TCC_GPA(14),TCC_GPA(15),
		TCC_GPA(16),TCC_GPA(17) ,TCC_GPA(18),TCC_GPA(18),TCC_GPA(20),TCC_GPA(21),TCC_GPA(22),TCC_GPA(23),
		TCC_GPA(24),TCC_GPA(25) ,TCC_GPA(26),TCC_GPA(27),TCC_GPA(28),TCC_GPA(29),TCC_GPA(30),TCC_GPA(31),
		/*B :  32 ~ 60 GPIO_B[28:00]*/
		TCC_GPB(0) ,TCC_GPB(1) ,TCC_GPB(2) ,TCC_GPB(3) ,TCC_GPB(4) ,TCC_GPB(5) ,TCC_GPB(6) ,TCC_GPB(7) ,  // ~ 32
		TCC_GPB(8) ,TCC_GPB(9) ,TCC_GPB(10),TCC_GPB(11),TCC_GPB(12),TCC_GPB(13),TCC_GPB(14), TCC_GPB(15),
		TCC_GPB(16),TCC_GPB(17),TCC_GPB(18),TCC_GPB(19),TCC_GPB(20),TCC_GPB(21),TCC_GPB(22), TCC_GPB(23),
		TCC_GPB(24),TCC_GPB(25),TCC_GPB(26),TCC_GPB(27),TCC_GPB(28),
		/*C :  61 ~ 90 GPIO_C[29:00]*/
		TCC_GPC(0),TCC_GPC(1) ,TCC_GPC(2) ,TCC_GPC(3) ,TCC_GPC(4) ,TCC_GPC(5) ,TCC_GPC(6) ,TCC_GPC(7) ,	// ~ 61
		TCC_GPC(8), TCC_GPC(9) ,TCC_GPC(10),TCC_GPC(11),TCC_GPC(12),TCC_GPC(13),TCC_GPC(14),TCC_GPC(15),
		TCC_GPC(16),TCC_GPC(17),TCC_GPC(18),TCC_GPC(19),TCC_GPC(20),TCC_GPC(21),TCC_GPC(22),TCC_GPC(23),
		TCC_GPC(24), TCC_GPC(25),TCC_GPC(26),TCC_GPC(27),TCC_GPC(28),TCC_GPC(29),
		/*E :  91 ~ 110 GPIO_E[19:00]*/
		TCC_GPE(0) ,TCC_GPE(1) ,TCC_GPE(2) ,TCC_GPE(3) ,TCC_GPE(4) ,TCC_GPE(5) ,TCC_GPE(6) ,TCC_GPE(7) , // ~ 91
		TCC_GPE(8) ,TCC_GPE(9) ,TCC_GPE(10),TCC_GPE(11),TCC_GPE(12),TCC_GPE(13),TCC_GPE(14),TCC_GPE(15),
		TCC_GPE(16),TCC_GPE(17),TCC_GPE(18),TCC_GPE(19),
		/*E :  111~121 GPIO_G[10:00]*/
		TCC_GPG(0) ,TCC_GPG(1) ,TCC_GPG(2) ,TCC_GPG(3) ,TCC_GPG(4),TCC_GPG(5) ,TCC_GPG(6) ,TCC_GPG(7), // ~ 111
		TCC_GPG(8) ,TCC_GPG(9) ,TCC_GPG(10), 
		/*E :  122~133 GPIO_H[11:00]*/
		TCC_GPH(0) ,TCC_GPH(1) ,TCC_GPH(2) ,TCC_GPH(3) ,TCC_GPH(4) ,TCC_GPH(5) ,TCC_GPH(6) ,TCC_GPH(7) , // ~ 122
		TCC_GPH(8) ,TCC_GPH(9) ,TCC_GPH(10),TCC_GPH(11),
		/*K :  134~152 GPIO_K[18:00]*/
		TCC_GPK(0) ,TCC_GPK(1) ,TCC_GPK(2) ,TCC_GPK(3) ,TCC_GPK(4) ,TCC_GPK(5) ,TCC_GPK(6) ,TCC_GPK(7) ,  // ~ 134
		TCC_GPK(8), TCC_GPK(9), TCC_GPK(10),TCC_GPK(11),TCC_GPK(12),TCC_GPK(13),TCC_GPK(14),TCC_GPK(15),
		TCC_GPK(16),TCC_GPK(17),TCC_GPK(18),  
		/*MA:  153~182 GPIO_MA[29:00*/
		TCC_GPMA(0),TCC_GPMA(1),TCC_GPMA(2),TCC_GPMA(3),TCC_GPMA(4), TCC_GPMA(5),TCC_GPMA(6),TCC_GPMA(7), // ~ 153
		TCC_GPMA(8),TCC_GPMA(9),TCC_GPMA(10),TCC_GPMA(11),TCC_GPMA(12),TCC_GPMA(13),TCC_GPMA(14),TCC_GPMA(15),
		TCC_GPMA(16),TCC_GPMA(17),TCC_GPMA(18),TCC_GPMA(19),TCC_GPMA(20),TCC_GPMA(21),TCC_GPMA(22),TCC_GPMA(23),
		TCC_GPMA(24),TCC_GPMA(25),TCC_GPMA(26),TCC_GPMA(27),TCC_GPMA(28),TCC_GPMA(29)
	};

	if ((int_id < EINT_START_INT) || (int_id > EINT_END_INT)) {
		return BSP_NO;
	}

	for (idx = 0 ; idx < ARRAY_SIZE(ExtIntr) ; idx++) {
		if (ExtIntr[idx] == gpio) {
			break;
		}
	}

	if (idx >= ARRAY_SIZE(ExtIntr)) {
		return BSP_NO;
	}

	shift = 8*((int_id-INT_EXT0)%4);
	mask = 0x7F << shift;

	data = readl(reg);
	data = (data & ~mask) | (idx << shift);
	writel(data, reg);

	return BSP_YES;
}

/* GHC Warning : decleared but never referenced
//bus_clock : cpu_clock/2
//time unit(s) : time*(1/bus_clock)

static uint8 BSP_IntDebounce(uint32 u32_en_fil, uint32 u32_n_mod, uint32 u32_num, uint32 u32_time) // IntDebounce => BSP_IntDebounce renaming 
{

	uint32 *reg = (uint32 *)((TCC_GPIO_BASE + 0x700)+(0x4*u32_num)); //QAC : cast between a pointer to void and an integral type.
       uint32 data, time_p, fcpu, p_freq;
	   
	fcpu=tcc_get_clkctrl(FBUS_CPU);
	if ( fcpu > 2) // codesonar  : divide by zero (p_freq)
	{
		p_freq=fcpu/2;

		data=(1<<25);
		writel(data, reg);
		data=(0<<25);
		writel(data, reg);

		//if(u32_n_mod==0) //Codesonar Useless Assignment
		//{
		//	data&=~(1<<26);
		//} else 
		if(u32_n_mod==1)
		{
			data|=(1<<26);
		} 

		//if(u32_en_fil==0) //Codesonar Useless Assignment
		//{
	       //	 data&=~(1<<24);
		//} 
		//else 
		if(u32_en_fil==1)
		{
			data|=(1<<26);
		}

		time_p=(u32_time/p_freq)-1;
		data|=time_p;

		writel(data, reg);	
	}
	return 0; //qac Rule 17.4

}
*/
