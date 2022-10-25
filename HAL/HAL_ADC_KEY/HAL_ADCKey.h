/*******************************************************************************************/
/**<!-------Copyright (C) 2021 Alpine Electronics, Inc. All rights reserved. --------------->
 * 
 * \file					HAL_ADCKey.h
 * \date					2021/09/16
 * \author					Liyongnan
 * \model					EDA
 * \description				External declarations
 * \version					0.55
 *
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/09/16		liyongnan		New
 ********************************************************************************************/
#ifndef __HAL_ADCKEY_H__
#define __HAL_ADCKEY_H__
/********************************************************************************************/
/*							Include File Section											*/
/********************************************************************************************/
#include "apn_basic_type.h"
#include "mt_adc.h"
/********************************************************************************************/
/*							Macro Definition Section										*/
/********************************************************************************************/
//#define ADCKEY_DEBUG (1)
#ifdef ADCKEY_DEBUG
#include "debug.h"
#define ADCKEY_DBG(fmt, args...)   printf(fmt, ## args)
#else
#define ADCKEY_DBG(fmt, args...)   do {} while(0)
#endif

#define HAL_ADCKEY_NULL		(0x00U)
#define HAL_ADCKEY_VALUE_1	(1U)
#define HAL_ADCKEY_CHANNEL	(ADC_CHANNEL5)

#define HAL_ADCKEY_AD_HFT_ON_HOOK_MIN	(0x0901U)
#define HAL_ADCKEY_AD_HFT_ON_HOOK_MAX	(0x0AA1U)
#define HAL_ADCKEY_AD_HFT_OFF_HOOK_MIN	(0x0B0EU)
#define HAL_ADCKEY_AD_HFT_OFF_HOOK_MAX	(0x0C7AU)
#define HAL_ADCKEY_AD_HFT_TALK_MIN		(0x0ECBU)
#define HAL_ADCKEY_AD_HFT_TALK_MAX		(0x0F55U)
#define HAL_ADCKEY_AD_HFT_KEY_OFF_MIN	(0x0FA5U)
#define HAL_ADCKEY_AD_HFT_KEY_OFF_MAX	(0x0FE3U)
/********************************************************************************************/
/*							Type Definition Section											*/
/********************************************************************************************/
typedef struct {
	Type_uByte ubKey;	/* detected key code */
	Type_uByte ubCnt;
	Type_uByte ubConfirmKeyValue; /* if ubKey detected >= 5 times, set it to ubConfirmKeyValue */
	Type_uByte ubConfirmKeyCode; /* when keyvalue is STK_KEY_TYPE_OFF, this is the last pressed key */
} Hal_AdcKey_Detect_Buff_ts;

typedef struct {
	 Type_uByte ubKey;		/* KeyData */
	 Type_uHWord uhMax;		/* A/D value Max */
	 Type_uHWord uhMin;		/* A/D value Min */
} Hal_AdcKey_ADtoKeyData_ts;

typedef struct {
	Type_uWord uwStatus;
	Type_uWord uwOptions;
} Hal_AdcKey_Type_ts;

/********************************************************************************************/
/*							Enumeration Type Definition Section								*/
/********************************************************************************************/
enum {	
	HAL_ADCKEY_OPTION_NORMAL = 1
};

enum {
	HAL_ADCKEY_IOCTL_ADCKEY_READ = 1,
	HAL_ADCKEY_IOCTL_ADCKEY_RESET,
};

/********************************************************************************************/
/*							Extern Declaration												*/
/********************************************************************************************/
extern int32 ADCKey_Open(uint32 OPTIONS);
extern int32 ADCKey_Close(void);
extern int32 ADCKey_Ioctl(uint32 OPT, void* PAR1, void* PAR2, void* PAR3, void* PAR4);
#endif /* __HAL_ADCKEY_H__ */

