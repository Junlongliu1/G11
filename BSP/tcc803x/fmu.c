/****************************************************************************
 *   FileName    : fmu.c
 *   Description : Telechip Fault Manager Unit Driver.
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


#ifdef FMU_DRIVER

#include <fmu.h>
#include <bsp_int.h>
#include <bsp.h>
#include <osal.h>
#include "debug.h"
#include "reg_phys.h"



//----------------------------------------------------------------------------------------------------------------------------
// Function Definition for ISO26262
//----------------------------------------------------------------------------------------------------------------------------
static void fmu_irq_cb (void *arg) {

	FMU_STS status;
    FMU_INT_FNCT_PTR  p_isr;
    void *int_arg;	
	
	FMU_DBG("[%s] IN!!\n", __func__);

	status = get_fmu_irq_status();

	//range check 
	if((status.id) > FMU_ID_FMU_CFG_SOFT){
		FMU_DBG("[%s]IRQ id(%d) is invalid\n",__func__, status.id); 
	}	
	else if ((status.id > FMU_ID_ROM_CRC) && (status.id < FMU_ID_FMU_CFG_SOFT)){
		FMU_DBG("[%s]IRQ id(%d) is reserved \n",__func__, status.id); 
	}
	else{
		p_isr = FMU_IntVectTbl[status.id].func;						/* Fetch ISR handler.									*/
		int_arg = FMU_IntVectTbl[status.id].arg;

		if(p_isr != NULL) {
			FMU_DBG("[%s] Call Safety Mechanism IRQ handler[%d]\n",__func__,status.id);
			(*p_isr)(int_arg);										/* Call ISR handler.									*/
		}
	}

}


static void fmu_fiq_cb (void *arg) {
	
	FMU_STS status;
    FMU_INT_FNCT_PTR  p_isr;
    void *int_arg;	

	FMU_DBG("[%s] IN!!\n", __func__);
	
	status = get_fmu_fiq_status();

	//range check 
	if((status.id) > FMU_ID_FMU_CFG_SOFT){
		FMU_DBG("[%s]FIQ id(%d) is invalid\n",__func__, status.id);
	}	
	else if ((status.id > FMU_ID_ROM_CRC) && (status.id < FMU_ID_FMU_CFG_SOFT)) {
		FMU_DBG("[%s]FIQ id(%d) is reserved \n",__func__, status.id);
	}
	else{
		p_isr = FMU_IntVectTbl[status.id].func; 					/* Fetch ISR handler.									*/
		int_arg = FMU_IntVectTbl[status.id].arg;
		
		if(p_isr != NULL) {
			FMU_DBG("[%s] Call Safety Mechanism FIQ handler[%d]\n",__func__,status.id);
			(*p_isr)(int_arg);										/* Call ISR handler.									*/
		}
	}
}

uint8 tcc_fmu_isr_handler (uint8 id,
						  	uint8 severity_level,
						  	FMU_INT_FNCT_PTR int_fnct,
						  	void * arg) {

	uint8 ret = FMU_OK;

	if(id > FMU_ID_FMU_CFG_SOFT){
		ret =  FMU_INVALID_ID_ERR;
	}
	else if ((id > FMU_ID_ROM_CRC) && (id < FMU_ID_FMU_CFG_SOFT)) {
		ret =  FMU_RESERVE_ID_ERR;
	}
	else if (severity_level > FMU_SVL_HIGH) {
		ret =  FMU_INVALID_SEVERITY_ERR;
	}
	else if (int_fnct == NULL) 
	{
		ret =  FMU_ISR_HANDLER_ERR;
	}
	else
	{
		FMU_IntVectTbl[id].severity_level= severity_level;						  
		FMU_IntVectTbl[id].func = int_fnct;
		FMU_IntVectTbl[id].arg = arg;
	}	
	return ret;		
}




uint8 tcc_fmu_set (uint8 id) {

	uint32	nth_reg;
	uint32	nth_bit;

	uint32 ds_reg_128[4];
	uint32 ds_reg_64[2];
	
	uint8   ret = FMU_OK;
	uint8   ds_ret;
	
	if(id > FMU_ID_FMU_CFG_SOFT){
		ret = FMU_INVALID_ID_ERR;
	}
	else if ((id > FMU_ID_ROM_CRC) && (id < FMU_ID_FMU_CFG_SOFT)) {
		ret =  FMU_RESERVE_ID_ERR;
	}
	else
	{
		// Set FMU severity level for given FMU ID
		nth_reg =	(id >> 4) & 0x3;												// 0 ~ 3
		nth_bit = (((id 	) & 0xf) << 1); 										// (0 ~ 15) x 2

		// for double storage 
		ds_reg_128[nth_reg] = HwMC_FMU->uSEVERITY_LEVEL.nReg[nth_reg];
		
		HwMC_FMU->CFG_WR_PW 					= FMU_PW;
		HwMC_FMU->uSEVERITY_LEVEL.nReg[nth_reg] = ((HwMC_FMU->uSEVERITY_LEVEL.nReg[nth_reg] & (0xFFFFFFFFU - ((uint32)0x3 << nth_bit))) | (FMU_IntVectTbl[id].severity_level << nth_bit));

		if(FMU_OK != DS_Compare(ds_reg_128[nth_reg], HwMC_FMU->uSEVERITY_LEVEL.nReg[nth_reg]))
		{
			ret = FMU_DS_COMPARE_FAIL_ERR;
		}

		ret = clr_fmu_isr_mask(id);
		
		// Set FMU enable for given FMU ID
		nth_reg = (id >> 5) & 0x1;													// 0 ~ 1
		nth_bit = (id	  ) & 0x1f; 												// 0 ~ 31

		//for double storage 
		ds_reg_64[nth_reg] = HwMC_FMU->uEN.nReg[nth_reg] ;

		HwMC_FMU->CFG_WR_PW 					= FMU_PW;
		HwMC_FMU->uEN.nReg[nth_reg] 			= (HwMC_FMU->uEN.nReg[nth_reg] | ((uint32)0x1 << nth_bit));

		ds_reg_64[nth_reg] = (ds_reg_64[nth_reg] | ((uint32)0x1 << nth_bit));
		if(FMU_OK != DS_Compare(ds_reg_64[nth_reg], HwMC_FMU->uEN.nReg[nth_reg]))
		{
			ret = FMU_DS_COMPARE_FAIL_ERR;
		}
		
		
	}
	return ret;	
}

uint8 tcc_fmu_isr_clr (uint8 id){

	uint8 ret = FMU_OK;

	if(id > FMU_ID_FMU_CFG_SOFT){
		ret = FMU_INVALID_ID_ERR;
	}
	else if ((id > FMU_ID_ROM_CRC) && (id < FMU_ID_FMU_CFG_SOFT)) {
		ret = FMU_RESERVE_ID_ERR;
	}
	else
	{
		ret = clr_fmu_isr(id);
	}	
	return ret;
}


uint8 init_fmu(void){

	uint8 ret = FMU_OK;
	int32 vret, eret; 

	fmu_sw_reset();

	set_fmu_ext_fault(FAULT_ACTIVE_LOW, 0, 0);

	vret = BSP_IntVectSet(INT_FMU_IRQ, PRIORITY_NO_MEAN, INT_TYPE_EDGE_RISING, fmu_irq_cb, BSP_NULL);
	if(vret != BSP_OK) 
	{ 
		ret =  FMU_INTSET_ERR;
	}
	else
	{
		BSP_IntSrcEn(INT_FMU_IRQ);
	}


	vret = BSP_IntVectSet(INT_FMU_FIQ, PRIORITY_NO_MEAN, INT_TYPE_LEVEL_HIGH, fmu_fiq_cb, BSP_NULL);
	if(vret != BSP_OK) 
	{ 
		ret =  FMU_INTSET_ERR;
	}
	else
	{
		BSP_IntSrcEn(INT_FMU_FIQ);
	}

	return ret;	
}

uint8 deinit_fmu(void)
{
	uint8 ret = FMU_OK;
	int32 int_ret; 
	
	BSP_IntSrcDis(INT_FMU_IRQ);
	BSP_IntSrcDis(INT_FMU_FIQ);

	return ret;
}

//----------------------------------------------------------------------------------------------------------------------------
// Set external fault control
//----------------------------------------------------------------------------------------------------------------------------
// External fault configuration
//----------------------------------------------------------------------------------------------------------------------------
static void set_fmu_ext_fault (uint8 polarity, uint32 low_duration, uint32 high_duration) {

	FMU_DBG("[%s] IN!! polarity %d low_duration %d high_duration %d\n", __func__, polarity, low_duration,high_duration);

	if (polarity > 2) {
		FMU_DBG("Polarity type(%d) is invalid @ set_fmu_ext_fault()\n", polarity);
		//return FMU_FAIL;
	}
	else if (low_duration > 16) {
		FMU_DBG("Fault low duration(%d) is invalid @ set_fmu_ext_fault()\n", low_duration);
		//return FMU_FAIL;
	}
	else if (high_duration > 2) {
		FMU_DBG("Fault high duration(%d) is invalid @ set_fmu_ext_fault()\n", high_duration);
		//return FMU_FAIL;		
	}
	else
	{
		HwMC_FMU->CFG_WR_PW 			= FMU_PW;
		HwMC_FMU->uEXT_FAULT_CTRL.nReg	= ((high_duration << 8) | (low_duration << 4) | (polarity << 0));
	}

}


//----------------------------------------------------------------------------------------------------------------------------
// FMU S/W reset
//----------------------------------------------------------------------------------------------------------------------------
// FMU S/W reset assert & release
//----------------------------------------------------------------------------------------------------------------------------
static void fmu_sw_reset (void) { 

	HwMC_FMU->CFG_WR_PW 			= FMU_PW;
	HwMC_FMU->uCTRL.bReg.SW_RESET	= 0x1;

	HwMC_FMU->CFG_WR_PW 			= FMU_PW;
	HwMC_FMU->uCTRL.bReg.SW_RESET	= 0x0;
}

// isr mask clear for given ID
static uint8 clr_fmu_isr_mask (uint8 id) {

	uint32	nth_reg;
	uint32	nth_bit;
	uint32  IRQ_reg_64[2];
	uint32  FIQ_reg_64[2];
	uint32  FOULT_reg_64[2];
	uint8 	ret = FMU_OK; 

	nth_reg = (id >> 5) & 0x1;													// 0 ~ 1
	nth_bit = (id	  ) & 0x1f; 												// 0 ~ 31

	if (FMU_IntVectTbl[id].severity_level == FMU_SVL_LOW) {

		// for Double storage test
		IRQ_reg_64[nth_reg]	= HwMC_FMU->uIRQ_MASK.nReg[nth_reg];
		
		// Clear IRQ mask enable for given FMU ID					
		HwMC_FMU->CFG_WR_PW 					= FMU_PW;
		HwMC_FMU->uIRQ_MASK.nReg[nth_reg]		= (HwMC_FMU->uIRQ_MASK.nReg[nth_reg] | ((uint32)0x1 << nth_bit));

		// for Double storage test
		IRQ_reg_64[nth_reg] = (IRQ_reg_64[nth_reg] | ((uint32)0x1 << nth_bit));
		if(FMU_OK != DS_Compare(IRQ_reg_64[nth_reg], HwMC_FMU->uIRQ_MASK.nReg[nth_reg]))
		{
			ret = FMU_DS_COMPARE_FAIL_ERR;
		}
		
	}
	else if(FMU_IntVectTbl[id].severity_level == FMU_SVL_MID){

		// for Double storage test
		FIQ_reg_64[nth_reg]	= HwMC_FMU->uFIQ_MASK.nReg[nth_reg];
				
		// Clear FIQ mask enable for given FMU ID												// 0 ~ 31
		HwMC_FMU->CFG_WR_PW 					= FMU_PW;
		HwMC_FMU->uFIQ_MASK.nReg[nth_reg]		= (HwMC_FMU->uFIQ_MASK.nReg[nth_reg] | ((uint32)0x1 << nth_bit));

		// for Double storage test
		FIQ_reg_64[nth_reg] = (FIQ_reg_64[nth_reg] | ((uint32)0x1 << nth_bit));
		if(FMU_OK != DS_Compare(FIQ_reg_64[nth_reg], HwMC_FMU->uFIQ_MASK.nReg[nth_reg]))
		{
			ret = FMU_DS_COMPARE_FAIL_ERR;
		}	
		
	}
	else if(FMU_IntVectTbl[id].severity_level == FMU_SVL_HIGH){

		// for Double storage test
		FIQ_reg_64[nth_reg] = HwMC_FMU->uFIQ_MASK.nReg[nth_reg];

		// Clear FIQ mask enable for given FMU ID												// 0 ~ 31
		HwMC_FMU->CFG_WR_PW 					= FMU_PW;
		HwMC_FMU->uFIQ_MASK.nReg[nth_reg]		= (HwMC_FMU->uFIQ_MASK.nReg[nth_reg] | ((uint32)0x1 << nth_bit));

		// for Double storage test
		FIQ_reg_64[nth_reg] = (FIQ_reg_64[nth_reg] | ((uint32)0x1 << nth_bit));
		if(FMU_OK != DS_Compare(FIQ_reg_64[nth_reg], HwMC_FMU->uFIQ_MASK.nReg[nth_reg]))
		{
			ret = FMU_DS_COMPARE_FAIL_ERR;
		}			

		// for Double storage test
		FOULT_reg_64[nth_reg] = HwMC_FMU->uFAULT_MASK.nReg[nth_reg];
		
		// Clear Fault mask enable for given FMU ID											// 0 ~ 31
		HwMC_FMU->CFG_WR_PW 					= FMU_PW;
		HwMC_FMU->uFAULT_MASK.nReg[nth_reg] 	= (HwMC_FMU->uFAULT_MASK.nReg[nth_reg] | ((uint32)0x1 << nth_bit));

		// for Double storage test
		FOULT_reg_64[nth_reg] = (FOULT_reg_64[nth_reg] | ((uint32)0x1 << nth_bit));
		if(FMU_OK != DS_Compare(FOULT_reg_64[nth_reg], HwMC_FMU->uFAULT_MASK.nReg[nth_reg]))
		{
			ret = FMU_DS_COMPARE_FAIL_ERR;
		}		
		
	}
	else {
		ret = FMU_INVALID_SEVERITY_ERR;
	}

	return ret;
 }

// Clear FMU ISR for given ID
static uint8 clr_fmu_isr (uint32 id) {

	uint32	nth_reg;
	uint32	nth_bit;
	uint32  IRQ_reg_64[2];
	uint32  FIQ_reg_64[2];
	uint32  FOULT_reg_64[2];
	uint8 	ret = FMU_OK;	
	
	FMU_DBG("[%s] IN!! id %d \n", __func__, id);

	nth_reg = (id >> 5) & 0x1;													// 0 ~ 1
	nth_bit = (id	  ) & 0x1f; 												// 0 ~ 31

	if (FMU_IntVectTbl[id].severity_level == FMU_SVL_LOW) {

		// for Double storage test
		IRQ_reg_64[nth_reg] = HwMC_FMU->uIRQ_CLR.nReg[nth_reg];

		// Clear IRQ clear for given FMU ID
		HwMC_FMU->CFG_WR_PW 					= FMU_PW;
		HwMC_FMU->uIRQ_CLR.nReg[nth_reg]		= ((uint32)0x1 << nth_bit);

		// for Double storage test
		IRQ_reg_64[nth_reg] = (IRQ_reg_64[nth_reg] | ((uint32)0x1 << nth_bit));
		if(FMU_OK != DS_Compare(IRQ_reg_64[nth_reg], HwMC_FMU->uIRQ_CLR.nReg[nth_reg]))
		{
			ret = FMU_DS_COMPARE_FAIL_ERR;
		}	

		// for Double storage test
		HwMC_FMU->CFG_WR_PW 					= FMU_PW;
		HwMC_FMU->uIRQ_CLR.nReg[nth_reg]		= 0;
		
	}
	else if(FMU_IntVectTbl[id].severity_level == FMU_SVL_MID){

		// for Double storage test
		FIQ_reg_64[nth_reg] = HwMC_FMU->uFIQ_CLR.nReg[nth_reg];

		// Clear FIQ for given FMU ID		
		HwMC_FMU->CFG_WR_PW 					= FMU_PW;
		HwMC_FMU->uFIQ_CLR.nReg[nth_reg]		= ((uint32)0x1 << nth_bit);

		// for Double storage test
		FIQ_reg_64[nth_reg] = (FIQ_reg_64[nth_reg] | ((uint32)0x1 << nth_bit));
		if(FMU_OK != DS_Compare(FIQ_reg_64[nth_reg], HwMC_FMU->uFIQ_CLR.nReg[nth_reg]))
		{
			ret = FMU_DS_COMPARE_FAIL_ERR;
		}	
		
		HwMC_FMU->CFG_WR_PW 					= FMU_PW;
		HwMC_FMU->uFIQ_CLR.nReg[nth_reg]		= 0;
		
	}
	else if(FMU_IntVectTbl[id].severity_level == FMU_SVL_HIGH){

		// for Double storage test
		FIQ_reg_64[nth_reg] = HwMC_FMU->uFIQ_CLR.nReg[nth_reg];

		// Clear FIQ for given FMU ID		
		HwMC_FMU->CFG_WR_PW 					= FMU_PW;
		HwMC_FMU->uFIQ_CLR.nReg[nth_reg]		= ((uint32)0x1 << nth_bit);

		// for Double storage test
		FIQ_reg_64[nth_reg] = (FIQ_reg_64[nth_reg] | ((uint32)0x1 << nth_bit));
		if(FMU_OK != DS_Compare(FIQ_reg_64[nth_reg], HwMC_FMU->uFIQ_CLR.nReg[nth_reg]))
		{
			ret = FMU_DS_COMPARE_FAIL_ERR;
		}	
		
		HwMC_FMU->CFG_WR_PW 					= FMU_PW;
		HwMC_FMU->uFIQ_CLR.nReg[nth_reg]		= 0;

		// for Double storage test
		FOULT_reg_64[nth_reg] = HwMC_FMU->uFAULT_CLR.nReg[nth_reg];

		// Clear Fault clear for given FMU ID
		HwMC_FMU->CFG_WR_PW 					= FMU_PW;
		HwMC_FMU->uFAULT_CLR.nReg[nth_reg]		= ((uint32)0x1 << nth_bit);

		// for Double storage test
		FOULT_reg_64[nth_reg] = (FOULT_reg_64[nth_reg] | ((uint32)0x1 << nth_bit));
		if(FMU_OK != DS_Compare(FOULT_reg_64[nth_reg], HwMC_FMU->uFAULT_CLR.nReg[nth_reg]))
		{
			ret = FMU_DS_COMPARE_FAIL_ERR;
		}	
		
		HwMC_FMU->CFG_WR_PW 					= FMU_PW;
		HwMC_FMU->uFAULT_CLR.nReg[nth_reg]		= 0;
		
	}
	else {
		ret = FMU_INVALID_SEVERITY_ERR;
	}
	return ret;
}


static uint8 DS_Compare(uint32 value, uint32 reg)
{
	uint8 ret=FMU_OK;
	if(value != reg)
	{ 
		ret = FMU_DS_COMPARE_FAIL_ERR;
	}
	else
	{
		FMU_DBG("[%s]DS compare result sucess\n",__func__);
	}
	return ret;	
}



//----------------------------------------------------------------------------------------------------------------------------
// Get FMU IRQ/FIQ/Fault status
//----------------------------------------------------------------------------------------------------------------------------
// Get FMU IRQ status
static FMU_STS get_fmu_irq_status (void) {

	uint8	id_cnt;
	uint32	reg0, reg1;
	uint32	id_status;
	uint32  t_status;

	FMU_STS status;
	uint32	check_out; 

	status.id	= 64;
	status.cnt	= 0;
	check_out	= 0;
	reg0		= HwMC_FMU->uIRQ_STATUS.nReg[0];
	reg1		= HwMC_FMU->uIRQ_STATUS.nReg[1];

	for(id_cnt=0; id_cnt<64; id_cnt++) {
		id_status = reg0 & (uint32)0x1;    

		if (id_status == 1) {
			if (check_out == 0) {
				status.id = id_cnt;
				check_out = 1;
			} else {
				status.cnt++;
				FMU_DBG("[%s]status.cnt = %d\n",status.cnt);
			}
		}

		reg0 = (((reg1 & 0x1) << 31) | (reg0 >> 1));
		reg1 =						   (reg1 >> 1);
	}

	return status;
}

// Get FMU FIQ status
static FMU_STS get_fmu_fiq_status (void) {

	uint8	id_cnt;
	uint32	reg0, reg1;
	uint32	id_status;

	FMU_STS status;
	uint32	check_out; 

	status.id	= 64;
	status.cnt	= 0;
	check_out	= 0;
	reg0		= HwMC_FMU->uFIQ_STATUS.nReg[0];
	reg1		= HwMC_FMU->uFIQ_STATUS.nReg[1];

	for(id_cnt=0; id_cnt<64; id_cnt++) {
		id_status = reg0 & (uint32)0x1;    

		if (id_status == 1) {
			if (check_out == 0) {
				status.id = id_cnt;
				check_out = 1;
			} else {
				status.cnt++;
			}
		}

		reg0 = (((reg1 & 0x1) << 31) | (reg0 >> 1));
		reg1 =						   (reg1 >> 1);
	}

	return status;
}

#if 0
// Get FMU Fault status
FMU_STS get_fmu_fault_status (void) {

	uint8	id_cnt;
	uint32	reg0, reg1;
	uint32	id_status;

	FMU_STS status;
	uint32	check_out; 

	status.id	= 64;
	status.cnt	= 0;
	check_out	= 0;
	reg0		= HwMC_FMU->uFAULT_STATUS.nReg[0];
	reg1		= HwMC_FMU->uFAULT_STATUS.nReg[1];

	for(id_cnt=0; id_cnt<64; id_cnt++) {
		id_status = reg0 & (uint32)0x1;    

		if (id_status == 1) {
			if (check_out == 0) {
				status.id = id_cnt;
				check_out = 1;
			} else {
				status.cnt++;
			}
		}

		reg0 = (((reg1 & 0x1) << 31) | (reg0 >> 1));
		reg1 =						   (reg1 >> 1);
	}

	return status;
}
#endif

#endif
