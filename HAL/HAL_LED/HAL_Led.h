/*******************************************************************************************/
/**<!-------Copyright (C) 2021 Alpine Electronics, Inc. All rights reserved. --------------->
 * 
 * \file					HAL_Led.h
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
#ifndef __HAL_LED_H__
#define __HAL_LED_H__
/********************************************************************************************/
/*							Include File Section											*/
/********************************************************************************************/
#include "apn_basic_type.h"
#include "temp_compile_switch.h"
#include "pm_pin_define.h"
/********************************************************************************************/
/*							Macro Definition Section										*/
/********************************************************************************************/
#define HAL_LED_PWM_CH (1U)
#define HAL_LED_PWM_PORT (PM_LED_PWM)
#if (TEMP_COMPILE_SWITCH_05WORK_10WORK == TEMP_COMPILE_SWITCH_05WORK)
#define HAL_LED_PWM_PORT_NUM (72U)
#else
#define HAL_LED_PWM_PORT_NUM (95U)
#endif
#define HAL_LED_PWM_FREQUENCY (5 * 1000 * 1000)		// 200hz

#define HAL_LED_NULL (0x00U)

/********************************************************************************************/
/*							Type Definition Section											*/
/********************************************************************************************/
typedef struct {
	Type_uWord uwStatus;
	Type_uWord uwOptions;
} Hal_Led_Type_ts;
/********************************************************************************************/
/*							Enumeration Type Definition Section								*/
/********************************************************************************************/
enum {	
	HAL_LED_OPTION_NORMAL = 1
};

enum {	
	HAL_LED_IOCTL_SET_DUTY = 1
};
/********************************************************************************************/
/*							Extern Declaration												*/
/********************************************************************************************/
extern int32 LED_Open(uint32 OPTIONS);
extern int32 LED_Close(void);
extern int32 LED_Ioctl(uint32 OPT, void* PAR1, void* PAR2, void* PAR3, void* PAR4);
#endif /* __HAL_LED_H__ */
