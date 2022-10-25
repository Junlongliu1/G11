#ifndef EEP_CORE_H 
#define EEP_CORE_H 
/*******************************************************************************************/
/**<!-------Copyright (C) 2021 Alpine Electronics, Inc. All rights reserved. --------------->
 * 
 * \file					Eep_core.h
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
 *
 ********************************************************************************************/

/********************************************************************************************/
/*							Include File Section											*/
/********************************************************************************************/
#include "HAL_API.h"
#include "pm_pin_define.h"
#include "HAL_Eep.h"


/********************************************************************************************/
/*							Macro Definition Section										*/
/********************************************************************************************/
#define EEP_R			(0x0001U)/* Eeep Read */
#define EEP_W			(0x0010U)/* Eeep Write */
#define EEP_C			(0x0100U)/* Eeep Clear */	
#define EEP_S			(0x1000U)/* No checksum*/

#define HIGH 1
#define LOW  0

#define EEP_SCL_H  gpio_set(PM_EEP_I2C_SCL, HIGH)
#define EEP_SCL_L  gpio_set(PM_EEP_I2C_SCL, LOW)

#define EEP_SDA_H  gpio_set(PM_EEP_I2C_SDA, HIGH)
#define EEP_SDA_L  gpio_set(PM_EEP_I2C_SDA, LOW)

#define	EEP_I2C_CHANNEL		(0)
#define	EEP_I2C_SCL_kHZ		(400)

#define	EEP_I2C_WADDR       	(0xA0U)
#define	EEP_I2C_RADDR       	(0xA1U)


#define	eeprom_write	I2CEEPRom_GetBlockSize_Write
#define	eeprom_read		I2CEEPRom_GetBlockSize_Read



#if (TEMP_COMPILE_SWITCH_05WORK_10WORK == TEMP_COMPILE_SWITCH_05WORK)
#define EEP_ENABLE  gpio_set(PM_EEP_CONT,HIGH)
#define EEP_DISABLE  gpio_set(PM_EEP_CONT,LOW)
#else
#define EEP_ENABLE		do {} while(0)
#define EEP_DISABLE		do {} while(0)
#endif

enum MODULE_ID{
	MODULE_ID_MIN = 0,
	MODULE_ID_MISC,
	MODULE_ID_LOG,
	MODULE_ID_CARPLAY,
	MODULE_ID_PART,	
	MODULE_ID_SERIAL,
	MODULE_ID_DATE,
	MODULE_ID_OSC,
	MODULE_ID_MODEL,
	MODULE_ID_TRANSIT,
	MODULE_ID_TRACEABILITY,
	MODULE_ID_SELF,
	MODULE_ID_HAC,
	MODULE_ID_MAX
};

enum TPYE_ID{
	TPYE_COUNT = 0,
	TYPE_HW,
	TYPE_TASK,
	TYPE_SEAM,
	TYPE_EVENT,
	TYPE_QUEUE,
	TYPE_TIMER,
	TYPE_PMMNG,
	TYPE_PMDRV,
};

enum {
	PM_EEP_ERROR_MNG_HALOPEN = 0x01,
	PM_EEP_ERROR_MNG_HALCTRL_DOWNWAIT = 0x02,
	PM_EEP_ERROR_MNG_HALCTRL_SETINT = 0x04,
	PM_EEP_ERROR_MNG_HALCTRL_SETCALLBACK = 0x08,
	PM_EEP_ERROR_MNG_HALCTRL_POWERDOWN = 0x10,
	PM_EEP_ERROR_MNG_SOCINIT_TIMOUT = 0x20,
	PM_EEP_ERROR_MNG_SOCSTANDBYOUT_TIMOUT = 0x40
};

enum {
	PM_EEP_ERROR_DRV_HALHANDLE = 0x01,
	PM_EEP_ERROR_DRV_HALGETBAT = 0x02,
	PM_EEP_ERROR_DRV_BATTIMER = 0x04
};

#define EEP_ADDR_COUNT			(0x0020U)
#define EEP_ADDR_HW				(0x0021U)
#define EEP_ADDR_TASK			(0x0022U)
#define EEP_ADDR_SEAM			(0x0023U)
#define EEP_ADDR_EVENT			(0x0024U)
#define EEP_ADDR_QUEUE			(0x0025U)
#define EEP_ADDR_TIMER			(0x0026U)
#define EEP_ADDR_PMMNG			(0x0027U)
#define EEP_ADDR_PMDRV			(0x0028U)
#define HAL_EEP_OPTION_NORMAL	0


/*** Semaphore ***/
#define UPPER_MASK_DATA			(0xFF00U)		/* UP 8bit Mask */
#define LOWER_MASK_DATA			(0x00FFU)		/* LOW 8bit Mask */
/* Eeprom sw Use */

#define EEP_WRITABLE_OFF			(0x00U)
#define EEP_WRITABLE_ON				(0x01U)

#define EEP_RETRY_CNT		(3U)

#define EEP_VALUE_0                (0U)
#define EEP_VALUE_1                (1U)
#define EEP_VALUE_2                (2U)
#define EEP_VALUE_3                (3U)
#define EEP_VALUE_4                (4U)

#define EEP_BLOCK_LENGTH			(32U)

/********************************************************************************************/
/*							Type Definition Section											*/
/********************************************************************************************/
typedef void (*EEPFunc) (OS_StdQueue_ts* pstMsg);
typedef struct
{
	Type_uWord         uhEvent;
	EEPFunc             Proc;
}EEPProc_st;
typedef	struct	
{
	const OS_ObjId_t* tsk_id_p;
	Type_uByte Eep_MN_Moudle_ID; /* Eep source moudle ID */
}Eep_SourceTsk_ts;

/********************************************************************************************/
/*							Structure/Union Type Definition Section							*/
/********************************************************************************************/
typedef struct{
	Type_uByte	    aubModleId;			/* ModleId*/
	Type_uByte  	*ubData_P;
	Type_uByte  	*ubDefaultData_P;
	Type_uHWord		uhBlockStrA;		/* Block Start  (Adress) */
	Type_uHWord		uhBlockSize;		/* Block Length (byte) */
	Type_uHWord		ubDataType;	    	/* DataType*/
}EEP_AllDataBlockTbl_ts;


/********************************************************************************************/
/*							Extern Declaration												*/
/********************************************************************************************/
extern Type_uByte		wubEEPModuleState;			/* EepromControl Module State */
extern HAL_HANDLE 		nvbEEP_I2cHalHandle;


/********************************************************************************************/
/*							Global Function Prototype Declaration							*/
/********************************************************************************************/
extern Type_uByte wubDirectReadRAM(Type_uHWord auhStaAdr, Type_uHWord auhRdSize, Type_uByte* aubBuff_p);
extern Type_uByte wubLineToolReadEEP(Type_uHWord auhStaAdr, Type_uHWord auhRdSize, Type_uByte* aubBuff_p);
extern Type_uByte wubLineToolWriteEEP(Type_uHWord auhStaAdr, Type_uHWord auhRdSize, const Type_uByte* aubBuff_p);
extern Type_uByte wubBlockReadCheck(Type_uHWord auhStaAdr, Type_uHWord auhRdSize);
extern Type_uByte wubBlockWriteCheck(Type_uHWord auhStaAdr, Type_uHWord auhRdSize, Type_uByte aubWriteTpye);
extern Type_uByte wubEEPBlockWrite(Type_uHWord auhStaBlk, Type_uHWord auhWtSize, const Type_uByte *aubBuff_p);
extern Type_uByte wubEEPLINETOOLAPPAllClear(void);
extern void wvdEEPLOGWrite(Type_uByte	tpye,Type_uByte	data);

extern Type_uByte wubEEPDataDefaultSet(void);
#endif	/* EEP_CORE_H */
