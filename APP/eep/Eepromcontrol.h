#ifndef EEPROMCONTROL_H 
#define EEPROMCONTROL_H 
/********************************************************************************************/
/** <!-- Copyright (C) 2021 Alpine Electronics, Inc. All rights reserved. ------------------------>
 *
 *	\file			EepromControl Global Headder File
 *	\date			2021/11/01
 *	\author			zhangbo
 *	\model			EDA
 *	\description	EepromControl Global Macro
 *	\version		0.550
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *					[Ticket2244] Macro Definition Addition
 ********************************************************************************************/

/********************************************************************************************/
/*							Include File Section											*/
/********************************************************************************************/
#include "apn_basic_type.h"
#include "os_framework.h"
#include "HAL_API.h"

/********************************************************************************************/
/*							Macro Definition Section (Global)								*/
/********************************************************************************************/


/* Eeprom DataBlock Name & Eeprom Adress(Start Adress) */
#define EEP_ADDR_MISC				(0x0000U)
#define EEP_ADDR_LOG				(0x0020U)

#define EEP_ADDR_CARPLAY			(0x0400U)		
#define EEP_ADDR_PART				(0x0420U)		
#define EEP_ADDR_SERIAL				(0x0430U)		
#define EEP_ADDR_DATE				(0x0440U)
#define EEP_ADDR_OSC				(0x0450U)	
#define EEP_ADDR_MODEL				(0x0458U)

#define EEP_ADDR_TRANSIT			(0x0460U)
#define EEP_ADDR_TRACEABILITY		(0x0470U)
#define EEP_ADDR_SELF				(0x0490U)
/* 0210 issO */
#define EEP_ADDR_HAC				(0x04A0U)



#define EEP_ADDR_CLEAR				(0x0000U)
#define EEP_ADDR_END				(0x04FFU)

#define EEP_MISC_SIZE  				(EEP_SIZE_3)
#define EEP_LOG_SIZE  				(EEP_SIZE_32)

#define EEP_CARPLAY_SIZE  			(EEP_SIZE_32)
#define EEP_PART_SIZE  				(EEP_SIZE_16)
#define EEP_SERIAL_SIZE  			(EEP_SIZE_16)
#define EEP_DATE_SIZE  				(EEP_SIZE_16)
#define EEP_OSC_SIZE  				(EEP_SIZE_8)
#define EEP_MODEL_SIZE  			(EEP_SIZE_8)

#define EEP_TRANSIT_SIZE  			(EEP_SIZE_16)
#define EEP_TRACEABILITY_SIZE  		(EEP_SIZE_32)
#define EEP_SELF_SIZE  				(EEP_SIZE_16)
/* 0210 issO */
#define EEP_HAC_SIZE  				(EEP_SIZE_16)



#define EEP_CHECKSUM_SIZE  			(EEP_SIZE_1)


#define EEP_SIZE_MAX				(4096U)
#define EEP_SIZE_32                 (32U)
#define EEP_SIZE_16                 (16U)
#define EEP_SIZE_8                  (8U)
#define EEP_SIZE_3                 (3U)
#define EEP_SIZE_2                  (2U)
#define EEP_SIZE_1                  (1U)

#define EEP_RESET_COUNT				14


/********************************************************************************************/
/*							Macro Definition Section (Local)								*/
/********************************************************************************************/
/*** EepromControl Message(Request) Event ***/

#define EEP_STS_NG         0
#define EEP_STS_OK         1


/*** EepromControl Module State ***/
#define EEP_STATE_STANDY	(0U)		
#define EEP_STATE_IDLE	(1U)		
#define EEP_STATE_BUSY	(2U)		

#define EEP_FORCE_WRITE				(0x01U)


#define EEP_CLEAR					(0U)
#define EEP_RETRY_CONT				(2U)

#define INI_0			0
#define INI_1			1
#define INI_3			3
#define RES_ERROR 		-1
#define RES_SUCCESS		0
#define RES_FAILED		1

#define USELESS_NUM 	0

/*CarType_Variation setting*/
#define CarType_Variation_0x10 			0x10
#define CarType_Variation_0x20 			0x20
#define CarType_Variation_0x30 			0x30
#define CarType_Variation_0x40 			0x40
#define CarType_Variation_0x50 			0x50
#define CarType_Variation_0x60 			0x60


enum EEP_NUM_DEF{
	EEP_NUM_0 = 0, 
	EEP_NUM_1, 
	EEP_NUM_2, 
	EEP_NUM_3, 
	EEP_NUM_4, 
	EEP_NUM_5, 
	EEP_NUM_6, 
	EEP_NUM_7, 
	EEP_NUM_8, 
	EEP_NUM_9, 
	EEP_NUM_10
};



/********************************************************************************************/
/*							Enumeration Type Definition Section								*/
/********************************************************************************************/

/********************************************************************************************/
/*							Structure/Union Type Definition Section							*/
/********************************************************************************************/
typedef struct {
	const Type_uByte		ubVariation_Index;
	const Type_uByte		ubCarType_Variation_0x10;
	const Type_uByte   	    ubCarType_Variation_0x20;
	const Type_uByte 		ubCarType_Variation_0x30;
	const Type_uByte 		ubCarType_Variation_0x40;
	const Type_uByte 		ubCarType_Variation_0x50;
    const Type_uByte 		ubCarType_Variation_0x60;
} Variation_Table_ts;

/********************************************************************************************/
/*							Extern Declaration												*/
/********************************************************************************************/
enum{
	EN_VARI_FUNC_MIN = INI_0,
	EN_VARI_FUNC_DESTINATION,
	EN_VARI_FUNC_HANDLE,
	EN_VARI_FUNC_DISP_SIZE,
	EN_VARI_FUNC_DEFAULT_LANGUAGE,
	EN_VARI_FUNC_RADIO_NOR,
	EN_VARI_FUNC_RADIO_DBS,
	EN_VARI_FUNC_RADIO_RDS,
	EN_VARI_FUNC_RADIO_FREQ,
	EN_VARI_FUNC_BT_AUDIO,
	EN_VARI_FUNC_BT_HFT,
	EN_VARI_FUNC_BT_RCAPP,
	EN_VARI_FUNC_CARPALY_WIRED,
	EN_VARI_FUNC_CARPALY_WIRELESS,
	EN_VARI_FUNC_AAP_WIRED,
	EN_VARI_FUNC_AAP_WIRELESS,
	EN_VARI_FUNC_WLINK_WIRED,
	EN_VARI_FUNC_WIFI_CH_DUAL,
	EN_VARI_FUNC_WIFI_CH_2,
	EN_VARI_FUNC_WIFI_CH_5,
	EN_VARI_FUNC_WIFI_SETTING,
	EN_VARI_FUNC_RWC_NORMAL,
	EN_VARI_FUNC_RWC_WIDE,
	EN_VARI_FUNC_LWC,
	EN_VARI_FUNC_COM_BCAN,
	EN_VARI_FUNC_COM_KLINE,
	EN_VARI_FUNC_MULT_LAGE_UKENG,
	EN_VARI_FUNC_MULT_LAGE_FRENCH,
	EN_VARI_FUNC_TIME_ZONE,
	EN_VARI_FUNC_MAX,
};

enum{

	EN_VARI_RES_ERROR = INI_0,
	EN_VARI_RES_UNDEFINE,
	EN_VARI_RES_ERROR_MAX,
	
	EN_VARI_RES_FUNC_EXIST,
	EN_VARI_RES_FUNC_NOT_EXIST,

	EN_VARI_RES_FUNC_DEST_ID,
	EN_VARI_RES_FUNC_DEST_KN,

	EN_VARI_RES_FUNC_HANDLE_RH,
	EN_VARI_RES_FUNC_HANDLE_LH,

	EN_VARI_RES_FUNC_DISP_INCH_8,
	EN_VARI_RES_FUNC_DISP_INCH_10,
	
	EN_VARI_RES_FUNC_DFT_LAGE_UKENG,
	EN_VARI_RES_FUNC_DFT_LAGE_MEXSPN,
	EN_VARI_RES_FUNC_DFT_LAGE_BHSINA,

	EN_VARI_RES_FUNC_RADIO_FREQ_THAI,
	EN_VARI_RES_FUNC_RADIO_FREQ_GEN,
	EN_VARI_RES_FUNC_RADIO_FREQ_NAMR,

	EN_VARI_RES_FUNC_WIFI_DFT_ON,
	EN_VARI_RES_FUNC_WIFI_DFT_OFF,

	EN_VARI_RES_FUNC_TIME_P_0200,
	EN_VARI_RES_FUNC_TIME_P_0530,
};


/********************************************************************************************/
/*							Type Definition Section											*/
/********************************************************************************************/

/********************************************************************************************/
/*							Structure/Union Type Definition Section							*/
/********************************************************************************************/

/********************************************************************************************/
/*							Extern Declaration												*/
/********************************************************************************************/

/********************************************************************************************/
/*							Global Function Prototype Declaration							*/
/********************************************************************************************/
extern Type_uByte wubEEP_Init(void);
extern Type_uByte wubEEP_EnforceStandby(void);
extern Type_uByte wubEEP_StandbyOut(void);
extern Type_uByte wubEEP_StandbyIn(void);
extern Type_uByte wubEEP_BlockWrite(Type_uHWord auhStaBlk, Type_uHWord auhWtSize, const Type_uByte *aubBuff_p);
extern Type_uByte wubEEP_BlockRead(Type_uHWord auhStaAdr,Type_uHWord auhRdSize,Type_uByte* aubBuff_p);
extern Type_uByte wubEEP_LineToolReadEEP(Type_uHWord auhStaAdr,Type_uHWord auhRdSize,Type_uByte* aubBuff_p);
extern Type_uByte wubEEP_LineToolWriteEEP(Type_uHWord auhStaBlk, Type_uHWord auhWtSize, const Type_uByte *aubBuff_p);
extern Type_uByte wubEEP_SendMail(const Type_uByte aubsrcId, const Type_uWord auhEventCode, void* avdMailBuffer_p, const Type_uWord aubMailSize);
extern Type_uByte wubEEP_LineToolAllClear(void);
extern Type_uByte wubEEP_DefaultSet(void);
extern void nvdEEP_SW_Reset(void);
extern void nvdEEP_SW_StartCondition(void);

extern Type_uByte wubVariation_CarType_Get(void);
extern Type_uByte wubVariation_Function_Get(Type_uByte aubFuntionIndex);
extern void wvEepHalOpen(void);
extern HAL_HANDLE wvdEepHalHandleGet(void);


#endif	/* EEPROMCONTROL_H */
