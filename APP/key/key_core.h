/*******************************************************************************************/
/**<!-------Copyright (C) 2021 Alpine Electronics, Inc. All rights reserved. --------------->
 * 
 * \file					Key_core.h
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
#ifndef KEY_CORE_H 
#define KEY_CORE_H 
/********************************************************************************************/
/*							Include File Section											*/
/********************************************************************************************/
#include "apn_basic_type.h"
#include "HAL_API.h"
/********************************************************************************************/
/*							Macro Definition Section										*/
/********************************************************************************************/
//#define KEY_DEBUG
#ifdef KEY_DEBUG
#define KEY_DBG(fmt, args...)   printf(fmt, ## args)
#else
#define KEY_DBG(fmt, args...)   do {} while(0)
#endif

#define SYSLOG_SUPPORT
#ifdef SYSLOG_SUPPORT
#include "logapp.h"
#else
#define LOG_OUTPUT(aubAppID, aubLogLevel, aubATATLevel, achFormat,...)   do {} while(0)
#endif


#define KEY_VALUE_0 (0U)
#define KEY_VALUE_2 (2U)
#define KEY_VALUE_3 (3U)
#define KEY_VALUE_4 (4U)

#define KEY_GPIOKEY_CNT (6U)
#define KEY_ADCKEY_WAIT_TIME (4U)
#define KEY_I2CKEY_WAIT_TIME (4U)
#define KEY_GPIOKEY_WAIT_TIME (8U)
#define KEY_ADCKEY_LOOP_CNT (5U)
#define KEY_I2CKEY_LOOP_CNT (5U)
#define KEY_GPIOKEY_LOOP_CNT (6U)

/********************************************************************************************/
/*							Type Definition Section											*/
/********************************************************************************************/

/********************************************************************************************/
/*							Enumeration Type Definition Section								*/
/********************************************************************************************/
enum {
	KEY_STATUS_OFF         = (1U),
	KEY_STATUS_ON_NORMAL,
	KEY_STATUS_ON_LINETOOL
};

/*ad_key*/
typedef struct {
	 Type_uByte ubKey;		/* KeyData */
	 Type_uByte ubKeyState;		/* ubKeyState */

} Key_ADState_ts;



/*i2c_key*/
typedef struct {
	 Type_uByte ubKey;		/* KeyData */
	 Type_uByte ubKeyState;		/* ubKeyState */

} Key_I2CState_ts;


/********************************************************************************************/
/*							Structure/Union Type Definition Section							*/
/********************************************************************************************/

/********************************************************************************************/
/*							Extern Declaration												*/
/********************************************************************************************/
extern Type_uByte	nubKEY_State;
extern HAL_HANDLE	nvbKEY_AdcHalHandle;
extern HAL_HANDLE	nvbKEY_I2cHalHandle;
extern HAL_HANDLE	nvbKEY_GpioHalHandle;
/********************************************************************************************/
/*							Global Function Prototype Declaration							*/
/********************************************************************************************/
extern void wvdKey_VariableInit(void);

#endif /* KEY_CORE_H  */
