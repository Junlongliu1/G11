/*******************************************************************************************/
/**<!-------Copyright (C) 2021 Alpine Electronics, Inc. All rights reserved. --------------->
 * 
 * \file					HAL_I2CKey.h
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
#ifndef __HAL_I2CKEY_H__
#define __HAL_I2CKEY_H__
/********************************************************************************************/
/*							Include File Section											*/
/********************************************************************************************/
#include "apn_basic_type.h"
#include "temp_compile_switch.h"
#include "pm_pin_define.h"
/********************************************************************************************/
/*							Macro Definition Section										*/
/********************************************************************************************/
//#define I2CKEY_DEBUG
#ifdef I2CKEY_DEBUG
#include "debug.h"
#define I2CKEY_DBG(fmt, args...)   printf(fmt, ## args)
#else
#define I2CKEY_DBG(fmt, args...)   do {} while(0)
#endif

#define HAL_I2CKEY_NULL				(0U)
#define HAL_I2CKEY_VALUE_0			(0U)
#define HAL_I2CKEY_VALUE_1			(1U)
#define HAL_I2CKEY_VALUE_2			(2U)
#define HAL_I2CKEY_VALUE_3			(3U)
#define HAL_I2CKEY_VALUE_6			(6U)

#define HAL_I2CKEY_CHANNEL			(1U)
#define HAL_I2CKEY_GPIO_SCL			(PM_ADC_I2C_SCL)
#define HAL_I2CKEY_GPIO_SDA			(PM_ADC_I2C_SDA)
#define HAL_I2CKEY_SCL_kHZ			(70U)
#if (TEMP_COMPILE_SWITCH_05WORK_10WORK == TEMP_COMPILE_SWITCH_05WORK)
#define HAL_I2CKEY_SLAVE_ADDRESS	(0x9aU)//(0x54U)
#else
#define HAL_I2CKEY_SLAVE_ADDRESS	(0xa8U)
#endif
#define HAL_I2CKEY_REGISTER_ADDRESS	(0x00U)
#define HAL_I2CKEY_OPT				(0U)
#define HAL_I2CKEY_SYNC				(0U)

#define HAL_I2CKEY_AD_AUDIO_VOL_DOWN_MIN	(0x0656U)
#define HAL_I2CKEY_AD_AUDIO_VOL_DOWN_MAX	(0x0778U)
#define HAL_I2CKEY_AD_AUDIO_VOL_UP_MIN		(0x08D7U)
#define HAL_I2CKEY_AD_AUDIO_VOL_UP_MAX		(0x0A03U)
#define HAL_I2CKEY_AD_AUDIO_CH_DOWN_MIN		(0x0AF6U)
#define HAL_I2CKEY_AD_AUDIO_CH_DOWN_MAX		(0x0BF2U)
#define HAL_I2CKEY_AD_AUDIO_CH_UP_MIN		(0x0CECU)
#define HAL_I2CKEY_AD_AUDIO_CH_UP_MAX		(0x0DA0U)
#define HAL_I2CKEY_AD_AUDIO_MODE_MIN		(0x0E57U)
#define HAL_I2CKEY_AD_AUDIO_MODE_MAX		(0x0EC2U)
#define HAL_I2CKEY_AD_HFT_KEY_OFF_MIN	(0x0F52U)
#define HAL_I2CKEY_AD_HFT_KEY_OFF_MAX	(0x0F7DU)
/********************************************************************************************/
/*							Type Definition Section											*/
/********************************************************************************************/
typedef struct {
	Type_uWord uwStatus;
	Type_uWord uwOptions;
} Hal_I2cKey_Type_ts;

typedef struct {
	 Type_uByte ubKey;		/* KeyData */
	 Type_uHWord uhMax;		/* A/D value Max */
	 Type_uHWord uhMin;		/* A/D value Min */
} Hal_I2cKey_ADtoKeyData_ts;

typedef struct {
	Type_uByte ubKey;
	Type_uByte ubCnt;
	Type_uByte ubConfirmKeyValue;
	Type_uByte ubConfirmKeyCode;
} Hal_I2cKey_Detect_Buff_ts;

/********************************************************************************************/
/*							Enumeration Type Definition Section								*/
/********************************************************************************************/
enum {	
	HAL_I2CKEY_OPTION_NORMAL = 1
};

enum {
	HAL_I2CKEY_IOCTL_I2CKEY_READ = 1,
	HAL_I2CKEY_IOCTL_I2CKEY_RESET,
};

/********************************************************************************************/
/*							Extern Declaration												*/
/********************************************************************************************/
extern int32 I2CKey_Open(uint32 OPTIONS);
extern int32 I2CKey_Close(void);
extern int32 I2CKey_Ioctl(uint32 OPT, void* PAR1, void* PAR2, void* PAR3, void* PAR4);
#endif /* __HAL_I2CKEY_H__ */
