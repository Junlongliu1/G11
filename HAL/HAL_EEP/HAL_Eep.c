/*******************************************************************************************/
/**<!-------Copyright (C) 2021 Alpine Electronics, Inc. All rights reserved. --------------->
 * 
 * \file					HAL_Eep.c
 * \date					2021/11/01
 * \author					zhangbo
 * \model					EDA
 * \description				I2C eep driver
 * \version					0.55
 *
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/11/01		zhangbo		New
 ********************************************************************************************/
/********************************************************************************************/
/*							Include File Section											*/
/********************************************************************************************/
#include <types.h>
//#include <osal.h>
#include "generic.h"
#include "i2c.h"
#include "os_framework.h"


#include "HAL_API.h"
#include "HAL_devices.h"
#include "HAL_Eep.h"
#include "i2c.h"
#include "bsp.h"
#include "debug.h"

#if (SERVICE_TEST_EN == 1u)
#include "Robust_Test.h" 
#endif


/********************************************************************************************/
/*							Global Variable Definition Section								*/
/********************************************************************************************/
/********************************************************************************************/
/*							Static Variable Definition Section								*/
/********************************************************************************************/

static Hal_Eep_Type_ts nstEepType;
/********************************************************************************************/
/*							ROM Table Section												*/
/********************************************************************************************/
/********************************************************************************************/
/*							Static Prototype Declaration Section							*/
/********************************************************************************************/
/********************************************************************************************/
/*							Function Definition Section										*/
/********************************************************************************************/
/********************************************************************************************/
/**	\function		IIC0EEPRom_Open
 *	\date			2021/11/01
 *	\author			zhangbo
 *	\description	HAL Open
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		OPTIONS
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			result
 *					
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/11/01		zhangbo		New
 ********************************************************************************************/
int32 I2CEEPRom_Open(uint32 OPTIONS)
{
	int32 alRet;
	
	alRet = BSP_ERR_NONE;
	
	nstEepType.uwStatus = HAL_DEVICE_ST_NO_OPEN;
	if (OPTIONS != 0) {
		alRet = HAL_ERR_NOT_SUPPORTED;
	} else {
		nstEepType.uwOptions  = OPTIONS;
		nstEepType.uwStatus = HAL_DEVICE_ST_OPENED;

		alRet = tcc_i2c_open(HAL_EEP_I2C_CHANNEL,
			HAL_EEP_I2C_GPIO_SCL,
			HAL_EEP_I2C_GPIO_SDA,
			HAL_EEP_I2C_SCL_kHZ,
			BSP_NULL,
			BSP_NULL);

		if(alRet != BSP_ERR_NONE) {
			alRet = HAL_ERR_NOT_INITIALIZED;
		} else {
			alRet = HAL_OK;
		}
	}

	return alRet;
}

/********************************************************************************************/
/**	\function		IIC0EEPRom_Close
 *	\date			2021/11/01
 *	\author			zhangbo
 *	\description	HAL Close
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		none
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			result
 *					
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/11/01		zhangbo		New
 ********************************************************************************************/
int32 I2CEEPRom_Close(void)
{
	int32 alRet;

	alRet = BSP_ERR_NONE;

	if (nstEepType.uwStatus != HAL_DEVICE_ST_OPENED) {
		alRet = HAL_ERR_NOT_OPENED;
	} else {
		alRet = tcc_i2c_close(HAL_EEP_I2C_CHANNEL);
		if(alRet != BSP_ERR_NONE) {
			alRet = HAL_ERR_RESOURCE_CRASH;
		} else {
			alRet = HAL_OK;
		}

		if (alRet == HAL_OK) {
			nstEepType.uwStatus = HAL_DEVICE_ST_NO_OPEN;
			//close driver
		}
	}

	return alRet;
}
/********************************************************************************************/
/**	\function		IIC0EEPRom_Ioctl
 *	\date			2021/11/01
 *	\author			zhangbo
 *	\description	HAL Close
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		none
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			result
 *					
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/11/01		zhangbo		New
 ********************************************************************************************/

int32 I2CEEPRom_Ioctl(uint32 OPT, void* PAR1, void* PAR2, void* PAR3, void* PAR4)
{
	int32 lRet;
	Type_uByte 	aubRetry;
	Type_uHWord aubAddr = HAL_EEP_NULL;
	Type_uHWord aubSize = HAL_EEP_NULL;
	Type_uByte *aubOutData_P = STD_NULL;
	Type_uByte *aubInData_P = STD_NULL;

	aubRetry = 0;
 	aubAddr = *(Type_uHWord *)PAR1;
 	aubSize = *(Type_uHWord *)PAR2;
	aubOutData_P = (Type_uByte *)PAR3;
	aubInData_P = (Type_uByte *)PAR4;	
	
	lRet = BSP_ERR_NONE;
	
	if (nstEepType.uwStatus != HAL_DEVICE_ST_OPENED )
	{
		lRet = HAL_ERR_NOT_OPENED;
	}
	else
	{
		switch (OPT)
		{
			case HAL_IO_EEP_WRITE:
				do{
					lRet = I2CEEPRom_GetBlockSize_Write(aubAddr,aubSize,aubInData_P);
					aubRetry++;
				#if EEP_1_1_7
					lRet = BSP_ERR_INVALID_HANDLE;
				#endif 
						
				}while((aubRetry < HAL_EEP_RETRY_CONT)&&(lRet != BSP_ERR_NONE));

				if( lRet != BSP_ERR_NONE ){
					lRet = HAL_ERR_RESOURCE_CRASH;
				}
				break;
			case HAL_IO_EEP_READ:
				do{
					lRet = I2CEEPRom_GetBlockSize_Read(aubAddr,aubSize,aubOutData_P);
					aubRetry++;
				#if EEP_1_1_7
					lRet = BSP_ERR_INVALID_HANDLE;
				#endif 
					
				}while((aubRetry < HAL_EEP_RETRY_CONT)&&(lRet != BSP_ERR_NONE));

				if( lRet != BSP_ERR_NONE ){
					lRet = HAL_ERR_RESOURCE_CRASH;
				}
				break;
			default:
				lRet = HAL_ERR_NOT_SUPPORTED;
				break;
		}
	}
	EEP_DBG("\n %s: : return=%d Retry=%d", __FUNCTION__,lRet,aubRetry);
	return lRet;
}

/********************************************************************************************/
/**	\function		I2CEEPRom_GetBlockSize_Write
 *	\date			2021/11/01
 *	\author			zhangbo
 *	\description	HAL Close
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		auhStaAdr	auhSize  *aubData_P
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			result
 *					
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/11/01		zhangbo		New
 ********************************************************************************************/
Type_sByte I2CEEPRom_GetBlockSize_Write(Type_uHWord auhStaAdr, Type_uByte auhSize, Type_uByte *aubData_P)
{
	Type_sByte		asbRtn;                 /* Result */
	Type_uHWord		auhBlockOffset;         /* Block Position */
	Type_uHWord		auhFirBlockLength;      /* First Block Length */
	Type_uHWord		auhNextBlockLength;     /* Next Block Length */
	Type_uHWord		auhBlockNum;         	/* Block Start Adress */
	Type_uHWord		auhAddr;
	Type_uByte		aubAddr[HAL_EEP_I2C_ADDR_LENGTH];
	Type_uByte		aubDataBuff[32];
	Type_uByte		aubBlockCount;

	//OSAL_MemSet(aubDataBuff, HAL_EEP_NULL, sizeof(aubDataBuff));
	wvdGen_MemSet(&aubDataBuff[HAL_EEP_NULL], HAL_EEP_NULL, sizeof(aubDataBuff));

	asbRtn = BSP_ERR_NONE;        /* Result NG Set */
	auhAddr = auhStaAdr;
	
	/* Start Adress in the Block Position */
	auhBlockOffset	= auhStaAdr % HAL_EEP_I2C_LENGTH;
	/* The Length of From Start Adress to the Block End Adress */
	auhFirBlockLength	= HAL_EEP_I2C_LENGTH - auhBlockOffset;
	/* The First Adress of tne Start Adress In */
	auhBlockNum		= auhStaAdr/HAL_EEP_I2C_LENGTH;
	/* multiple Block count*/
	aubBlockCount		=0;

	aubAddr[0] = (auhAddr >> 8) & 0xFF;
	aubAddr[1] = (auhAddr ) & 0xFF;

	EEP_DBG("\n %s: : Write  Adr=%d  Size=%d", __FUNCTION__,auhStaAdr,auhSize);

	/* The Write Data only in One Block */
	if (auhSize <= auhFirBlockLength){
		wvdGen_MemCpy(&aubDataBuff[HAL_EEP_NULL], aubData_P, auhSize);
		/* Direct Write from IIC */
		asbRtn = tcc_i2c_xfer_w_cmd(HAL_EEP_I2C_CHANNEL, HAL_EEP_I2C_WADDR,
									HAL_EEP_I2C_ADDR_LENGTH, aubAddr, 
									auhSize, &aubDataBuff[HAL_EEP_NULL],
									0, NULL, 
									HAL_EEP_I2C_OPT, HAL_EEP_I2C_SYNC);
		/* wait 5ms*/
		OS_WaitTime(5000);
	}else {
		/* The Write Data in multiple Block */
		wvdGen_MemCpy(&aubDataBuff[HAL_EEP_NULL], aubData_P, auhFirBlockLength);

		EEP_DBG("\n write Data in multiple start Adr=%d  Size=%d",auhStaAdr,auhFirBlockLength);

		/* Write the First Block The Data In From IIC */
		asbRtn = tcc_i2c_xfer_w_cmd(HAL_EEP_I2C_CHANNEL, HAL_EEP_I2C_WADDR,
									HAL_EEP_I2C_ADDR_LENGTH, aubAddr, 
									auhFirBlockLength, &aubDataBuff[HAL_EEP_NULL],
									0, NULL, 
									HAL_EEP_I2C_OPT, HAL_EEP_I2C_SYNC);

		auhNextBlockLength = auhSize - auhFirBlockLength;
		OS_WaitTime(5000);
		
		while ((auhNextBlockLength > 0)&&(asbRtn == BSP_ERR_NONE)){
			/* Next Block Start Adress */
			auhBlockNum++;
			/* This Block Start Adress */
			auhAddr = auhBlockNum * HAL_EEP_I2C_LENGTH;
			
			aubAddr[0] = (auhAddr >> 8) & 0xFF;
			aubAddr[1] = (auhAddr ) & 0xFF;
					
			/* The Data End Of This Block */
			if (auhNextBlockLength <= HAL_EEP_I2C_LENGTH){
				EEP_DBG("\n write Data in End  start Adr=%d  Size=%d ",auhAddr,auhNextBlockLength);
				wvdGen_MemCpy(&aubDataBuff[HAL_EEP_NULL], (aubData_P + auhFirBlockLength + aubBlockCount*HAL_EEP_I2C_LENGTH), auhNextBlockLength);

				/* Write This Block Data to IIC */
				asbRtn = tcc_i2c_xfer_w_cmd(HAL_EEP_I2C_CHANNEL, HAL_EEP_I2C_WADDR, 
											HAL_EEP_I2C_ADDR_LENGTH,aubAddr,
											auhNextBlockLength, &aubDataBuff[HAL_EEP_NULL],
											0, NULL,
											HAL_EEP_I2C_OPT, HAL_EEP_I2C_SYNC);

				auhNextBlockLength = 0;
			}else{
				EEP_DBG("\n write Data in multiple	Adr=%d	Size=%d ",auhAddr,HAL_EEP_I2C_LENGTH);
				
				wvdGen_MemCpy(&aubDataBuff[HAL_EEP_NULL], (aubData_P + auhFirBlockLength + aubBlockCount*HAL_EEP_I2C_LENGTH), HAL_EEP_I2C_LENGTH);
						
				/* Write This Block Data to IIC */
				asbRtn = tcc_i2c_xfer_w_cmd(HAL_EEP_I2C_CHANNEL, HAL_EEP_I2C_WADDR, 
											HAL_EEP_I2C_ADDR_LENGTH, aubAddr,
											HAL_EEP_I2C_LENGTH, &aubDataBuff[HAL_EEP_NULL],
											0, NULL, 
											HAL_EEP_I2C_OPT, HAL_EEP_I2C_SYNC);
				/* The Remaining Block Length */
				auhNextBlockLength = auhNextBlockLength - HAL_EEP_I2C_LENGTH;

				/* Next Block Start Buff Adress */
				aubBlockCount++;
			}
			OS_WaitTime(5000);
		}
	}

	return asbRtn;
}

/********************************************************************************************/
/**	\function		I2CEEPRom_GetBlockSize_Read
 *	\date			2021/11/01
 *	\author			zhangbo
 *	\description	HAL Close
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		none
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			result
 *					
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/11/01		zhangbo		New
 ********************************************************************************************/

Type_sByte I2CEEPRom_GetBlockSize_Read(Type_uHWord auhStaAdr, Type_uByte auhSize, Type_uByte *aubData_P)
{
	Type_sByte		asbRtn;                 /* Result */
	Type_uHWord		auhBlockOffset;         /* Block Position */
	Type_uHWord		auhFirBlockLength;      /* First Block Length */
	Type_uHWord		auhNextBlockLength;      /* Second Block Length */
	Type_uHWord		auhBlockNum;         /* Block Start Adress */  
	Type_uHWord		auhAddr;
	Type_uByte		aubAddr[HAL_EEP_I2C_ADDR_LENGTH];
	Type_uByte		aubDataBuff[32];
	Type_uByte		aubBlockNum;

	wvdGen_MemSet(&aubDataBuff[HAL_EEP_NULL], HAL_EEP_NULL, sizeof(aubDataBuff));
	
	asbRtn = BSP_ERR_NONE;
	auhAddr = auhStaAdr;
	aubBlockNum		=0;

	/* Start Adress in the Block Position */
	auhBlockOffset	= auhAddr % HAL_EEP_I2C_LENGTH;
	/* The Length of From Start Adress to the Block End Adress */
	auhFirBlockLength	= HAL_EEP_I2C_LENGTH - auhBlockOffset;
	/* The First Adress of tne Start Adress In */
	
	auhBlockNum = auhAddr/HAL_EEP_I2C_LENGTH;
	
	aubAddr[0] = (auhAddr >> 8) & 0xFF;
	aubAddr[1] = (auhAddr ) & 0xFF;
	
	EEP_DBG("\n %s: : Read start Adr=%d Read Size=%d", __FUNCTION__,auhStaAdr,auhSize);

	/* The Read Data only In One Block */
	if (auhSize <= auhFirBlockLength){
		
		/* Direct Read from IIC */
		asbRtn = tcc_i2c_xfer_w_cmd(HAL_EEP_I2C_CHANNEL, HAL_EEP_I2C_WADDR,
									HAL_EEP_I2C_ADDR_LENGTH, aubAddr,
									0, NULL,
									auhSize, aubData_P,
									HAL_EEP_I2C_OPT, HAL_EEP_I2C_SYNC);
		/* wait 5ms*/
		//OS_WaitTime(5000);
		EEP_DBG("\n %s: : The Read Data only In One Block Read start Adr=%d Read Size=%d", __FUNCTION__,auhAddr,auhSize);
	
	}else {	/* The Read Data in multiple Block */

		/* Read the First Block The Data In From IIC */
		asbRtn = tcc_i2c_xfer_w_cmd(HAL_EEP_I2C_CHANNEL, HAL_EEP_I2C_WADDR, 
									HAL_EEP_I2C_ADDR_LENGTH, aubAddr, 
									0, NULL,
									auhFirBlockLength, &aubDataBuff[HAL_EEP_NULL],
									HAL_EEP_I2C_OPT, HAL_EEP_I2C_SYNC);
		
		EEP_DBG("\n The Read Data in multiple start Adr=%d Read Size=%d,data = 0x%x",auhAddr,auhFirBlockLength,aubDataBuff[HAL_EEP_NULL]);

		wvdGen_MemCpy(aubData_P,&aubDataBuff[HAL_EEP_NULL], auhFirBlockLength);
			
		auhNextBlockLength = auhSize - auhFirBlockLength;
		
		while ((auhNextBlockLength > 0)&&(asbRtn == BSP_ERR_NONE)){
			 /* Next Block Start Adress */
			auhBlockNum++;
			/* This Block Start Adress */
			auhAddr = auhBlockNum * HAL_EEP_I2C_LENGTH;
		
			aubAddr[0] = (auhAddr >> 8) & 0xFF;
			aubAddr[1] = (auhAddr ) & 0xFF;
			
			//OS_WaitTime(5000);
			/* The Data End Of This Block */
			if (auhNextBlockLength <= HAL_EEP_I2C_LENGTH){
				
				/* Read This Block Data to IIC */
				asbRtn = tcc_i2c_xfer_w_cmd(HAL_EEP_I2C_CHANNEL, HAL_EEP_I2C_WADDR,
											HAL_EEP_I2C_ADDR_LENGTH, aubAddr, 
											0, NULL, 
											auhNextBlockLength, &aubDataBuff[HAL_EEP_NULL],
											HAL_EEP_I2C_OPT, HAL_EEP_I2C_SYNC);

				EEP_DBG("\n The Read Data in End Block Addr=%d  Size=%d,data = 0x%x",auhAddr,auhNextBlockLength,aubDataBuff[HAL_EEP_NULL]);
				
				wvdGen_MemCpy((aubData_P + auhFirBlockLength + aubBlockNum*HAL_EEP_I2C_LENGTH),&aubDataBuff[HAL_EEP_NULL],auhNextBlockLength);
				
				auhNextBlockLength = 0;
				
				
			}else{
				/* Read This Block Data to IIC */
				asbRtn = tcc_i2c_xfer_w_cmd(HAL_EEP_I2C_CHANNEL, HAL_EEP_I2C_WADDR, 
											HAL_EEP_I2C_ADDR_LENGTH, aubAddr, 
											0, NULL, 
											HAL_EEP_I2C_LENGTH, &aubDataBuff[HAL_EEP_NULL],
											HAL_EEP_I2C_OPT, HAL_EEP_I2C_SYNC);

				EEP_DBG("\n The Read Data in  multiple Block Addr=%d  Size=%d,data = 0x%x",auhAddr,HAL_EEP_I2C_LENGTH,aubDataBuff[HAL_EEP_NULL]);

				wvdGen_MemCpy((aubData_P + auhFirBlockLength + aubBlockNum*HAL_EEP_I2C_LENGTH),&aubDataBuff[HAL_EEP_NULL],  HAL_EEP_I2C_LENGTH);
				
				/* The Remaining Block Length */
				auhNextBlockLength = auhNextBlockLength - HAL_EEP_I2C_LENGTH;

				aubBlockNum++;
			} 
		}

	}

	return asbRtn;
}



