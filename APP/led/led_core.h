/*******************************************************************************************/
/**<!-------Copyright (C) 2021 Alpine Electronics, Inc. All rights reserved. --------------->
 * 
 * \file					Led_core.h
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
 *
 ********************************************************************************************/
#ifndef LED_CORE_H 
#define LED_CORE_H 
/********************************************************************************************/
/*							Include File Section											*/
/********************************************************************************************/
#include "apn_basic_type.h"
#include "HAL_API.h"

/********************************************************************************************/
/*							Macro Definition Section										*/
/********************************************************************************************/
#define SYSLOG_SUPPORT
#ifdef SYSLOG_SUPPORT
#include "logapp.h"
#else
#define LOG_OUTPUT(aubAppID, aubLogLevel, aubATATLevel, achFormat,...)   do {} while(0)
#endif

/* Value */
#define LED_VALUE_0						(0U)
#define LED_VALUE_1						(1U)
#define LED_VALUE_3						(3U)

/* eeprom add */
#define LED_KEY_LIGHT_VALUE_ADDRESS	(0x00U)
#define LED_KEY_LIGHT_VALUE_SIZE	(0x01U)

/* duty array value */
#define LED_DUTY_SIZE						(2U)			
#define LED_DUTY_ARRAY_0					(0U)	
#define LED_DUTY_ARRAY_1					(1U)

/********************************************************************************************/
/*							Type Definition Section											*/
/********************************************************************************************/

/********************************************************************************************/
/*							Enumeration Type Definition Section								*/
/********************************************************************************************/
/* key light type */
enum{
/* -5 */	LED_KEY_LIGHT_VALUE_NEGATIVE_5 = (0U),
/* -4 */	LED_KEY_LIGHT_VALUE_NEGATIVE_4,
/* -3 */	LED_KEY_LIGHT_VALUE_NEGATIVE_3,
/* -2 */	LED_KEY_LIGHT_VALUE_NEGATIVE_2,
/* -1 */	LED_KEY_LIGHT_VALUE_NEGATIVE_1,
/* 0 */		LED_KEY_LIGHT_VALUE_POSITIVE_0,
/* +1 */	LED_KEY_LIGHT_VALUE_POSITIVE_1,
/* +2 */	LED_KEY_LIGHT_VALUE_POSITIVE_2,
/* +3 */	LED_KEY_LIGHT_VALUE_POSITIVE_3,
/* +4 */	LED_KEY_LIGHT_VALUE_POSITIVE_4,
/* +5 */	LED_KEY_LIGHT_VALUE_POSITIVE_5,
};

enum{
	LED_STATUS_ACC_OFF         = (1U),
	LED_STATUS_ACC_ON_POWER_ON,
	LED_STATUS_ACC_ON_POWER_OFF
};

enum{
	LED_STATUS_OFF         = (1U),
	LED_STATUS_ON
};
	
enum{
	LED_OFF         = (0U),
	LED_ON
};
	
/********************************************************************************************/
/*							Structure/Union Type Definition Section							*/
/********************************************************************************************/
extern const Type_uHWord nstLED_KeyDutySave_T[];

#if (COMPILE_SWITCH_DISP_SIZ == COMPILE_SWITCH_8INCH)
#define LED_ILL_OFF 0
#else
#define LED_ILL_OFF 0x09F6
#endif
/********************************************************************************************/
/*							Extern Declaration												*/
/********************************************************************************************/
extern Type_uByte	nubLED_State;
extern Type_uByte	nubLED_KeyLight;
extern Type_uByte	nubLED_ILL;
extern HAL_HANDLE	nvbLED_HalHandle;
/********************************************************************************************/
/*							Global Function Prototype Declaration							*/
/********************************************************************************************/
extern void wubLed_VariableInit(void);
extern Type_uByte wubLed_GetLedValue(void);
#endif	/* LED_CORE_H  */
