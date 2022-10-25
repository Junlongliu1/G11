/*******************************************************************************************/
/**<!-------Copyright (C) 2021 Alpine Electronics, Inc. All rights reserved. --------------->
 * 
 * \file					HAL_GPIOKey.h
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
#ifndef __HAL_GPIOKEY_H__
#define __HAL_GPIOKEY_H__
/********************************************************************************************/
/*							Include File Section											*/
/********************************************************************************************/
#include "types.h"
#include "apn_basic_type.h"
#include "temp_compile_switch.h"
#include "pm_pin_define.h"
/********************************************************************************************/
/*							Macro Definition Section										*/
/********************************************************************************************/
//#define GPIOKEY_DEBUG (1)
#ifdef GPIOKEY_DEBUG
#include "debug.h"
#define GPIOKEY_DBG(fmt, args...)   printf(fmt, ## args)
#else
#define GPIOKEY_DBG(fmt, args...)   do {} while(0)
#endif

#define HAL_GPIOKEY_NULL			(0x00U)
#define HAL_GPIOKEY_VALUE_1			(1U)
#define HAL_GPIOKEY_VALUE_6			(6U)
#define HAL_GPIOKEY_GPIO_HIGH		(1U)
#define HAL_GPIOKEY_GPIO_LOW		(0U)

#if (TEMP_COMPILE_SWITCH_05WORK_10WORK == TEMP_COMPILE_SWITCH_10WORK)
#define HAL_GPIOKEY_HOME_KEY_PORT		(PM_HOME_KEY)
#define HAL_GPIOKEY_POWER_KEY_PORT		(PM_POWER_KEY)
#define HAL_GPIOKEY_DAY_NIGHT_KEY_PORT	(PM_DAY_NIGHT_KEY)
#define HAL_GPIOKEY_BACK_KEY_PORT		(PM_BACK_KEY)
#define HAL_GPIOKEY_VOL_DOWN_KEY_PORT	(PM_VOL_DOWN_KEY)
#define HAL_GPIOKEY_VOL_UP_KEY_PORT		(PM_VOL_UP_KEY)
#else
/* unused in 0.5wk */
#define HAL_GPIOKEY_HOME_KEY_PORT		(0xff)
#define HAL_GPIOKEY_POWER_KEY_PORT		(0xff)
#define HAL_GPIOKEY_DAY_NIGHT_KEY_PORT	(0xff)
#define HAL_GPIOKEY_BACK_KEY_PORT		(0xff)
#define HAL_GPIOKEY_VOL_DOWN_KEY_PORT	(0xff)
#define HAL_GPIOKEY_VOL_UP_KEY_PORT		(0xff)
#endif

#define HAL_GPIOKEY_DETECT_MODE_LINETOOL (1)
#define HAL_GPIOKEY_DETECT_MODE_NORMAL (2)

/********************************************************************************************/
/*							Type Definition Section											*/
/********************************************************************************************/
typedef struct {
	Type_uWord uwStatus;
	Type_uWord uwOptions;
} Hal_GpioKey_Type_ts;

typedef struct{
	Type_uHWord uhKey;	/* KeyData */
	uint32 port;		/* GPIO port */ 
} Hal_GpioKeyDataTbl_ts;

typedef struct {
	Type_uByte ubAction;
	Type_uByte ubCnt;
	Type_uByte ubConfirmAction;
} Hal_GpioKey_Detect_Buff_ts;

/********************************************************************************************/
/*							Enumeration Type Definition Section								*/
/********************************************************************************************/
enum {	
	HAL_GPIOKEY_OPTION_NORMAL = 1
};

enum {
	HAL_GPIOKEY_IOCTL_GPIOKEY_READ = 1,
	HAL_GPIOKEY_IOCTL_GPIOKEY_RESET,
};

/********************************************************************************************/
/*							Extern Declaration												*/
/********************************************************************************************/
extern int32 GPIOKey_Open(uint32 OPTIONS);
extern int32 GPIOKey_Read(uint8 *buff, uint32 size);
extern int32 GPIOKey_Close(void);
extern int32 GPIOKey_Ioctl(uint32 OPT, void* PAR1, void* PAR2, void* PAR3, void* PAR4);
#endif /* __HAL_GPIOKEY_H__ */
