/*******************************************************************************************/
/**<!-------Copyright (C) 2021 Alpine Electronics, Inc. All rights reserved. --------------->
 * 
 * \file					HAL_Eep.h
 * \date					2021/11/01
 * \author					zhangbo
 * \model					EDA
 * \description				External declarations
 * \version					0.55
 *
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/11/01		zhangbo		New
 ********************************************************************************************/
#ifndef __HAL_EEP_H__
#define __HAL_EEP_H__
/********************************************************************************************/
/*							Include File Section											*/
/********************************************************************************************/
#include "apn_basic_type.h"
#include "gpio.h"
/********************************************************************************************/
/*							Macro Definition Section										*/
/********************************************************************************************/
//#define EEP_DEBUG
#ifdef EEP_DEBUG
#define EEP_DBG(fmt, args...)   printf(fmt, ## args)
#else
#define EEP_DBG(fmt, args...)   do {} while(0)
#endif


#define HAL_EEP_NULL				(0x00U)
#define HAL_EEP_I2C_CHANNEL			(0)
#define HAL_EEP_I2C_GPIO_SCL		(TCC_GPMA(22))
#define HAL_EEP_I2C_GPIO_SDA		(TCC_GPMA(23))
#define HAL_EEP_I2C_SCL_kHZ			(400)
#define HAL_EEP_I2C_OPT				(0x0)
#define HAL_EEP_I2C_SYNC			(0x0)

#define	HAL_EEP_I2C_WADDR       	(0xA0U)
#define	HAL_EEP_I2C_RADDR       	(0xA1U)

#define HAL_EEP_I2C_LENGTH			(32U)		/* Block Sum Length */

#define HAL_EEP_I2C_ADDR_LENGTH		(2U)

#define HAL_EEP_RETRY_CONT			(3U)



/********************************************************************************************/
/*							Type Definition Section											*/
/********************************************************************************************/
typedef struct {
	Type_uWord uwStatus;
	Type_uWord uwOptions;
} Hal_Eep_Type_ts;

enum
{
	HAL_IO_EEP_READ = 1,
	HAL_IO_EEP_WRITE,
	HAL_IO_EEP_CLEAR
};




/********************************************************************************************/
/*							Enumeration Type Definition Section								*/
/********************************************************************************************/

/********************************************************************************************/
/*							Extern Declaration												*/
/********************************************************************************************/
extern int32 I2CEEPRom_Open(uint32 OPTIONS);
extern int32 I2CEEPRom_Close(void);
extern int32 I2CEEPRom_Ioctl(uint32 OPT, void* PAR1, void* PAR2, void* PAR3, void* PAR4);
extern Type_sByte I2CEEPRom_GetBlockSize_Write(Type_uHWord auhStaAdr, Type_uByte auhSize, Type_uByte *aubData_P);
extern Type_sByte I2CEEPRom_GetBlockSize_Read(Type_uHWord auhStaAdr, Type_uByte auhSize, Type_uByte *aubData_P);



#endif /* __HAL_EEP_H__ */
