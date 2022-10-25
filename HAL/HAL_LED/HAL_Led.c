/*******************************************************************************************/
/**<!-------Copyright (C) 2021 Alpine Electronics, Inc. All rights reserved. --------------->
 * 
 * \file					HAL_Led.c
 * \date					2021/09/16
 * \author					Liyongnan
 * \model					EDA
 * \description				LED driver
 * \version					0.55
 *
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/09/16		liyongnan		New
 ********************************************************************************************/
/********************************************************************************************/
/*							Include File Section											*/
/********************************************************************************************/
#include "apn_basic_type.h"

#include "gpio.h"
#include "pwm.h"

#include "HAL_API.h"
#include "HAL_devices.h"
#include "HAL_Led.h"
#include "led_core.h"

/********************************************************************************************/
/*							Global Variable Definition Section								*/
/********************************************************************************************/

/********************************************************************************************/
/*							Static Variable Definition Section								*/
/********************************************************************************************/
static Hal_Led_Type_ts nstLedType;
/********************************************************************************************/
/*							Static Prototype Declaration Section							*/
/********************************************************************************************/
static void LED_SetPort(void);
/********************************************************************************************/
/*							Function Definition Section										*/
/********************************************************************************************/
/********************************************************************************************/
/**	\function		LED_SetPort
 *	\date			2021/09/16
 *	\author			liyongnan
 *	\description	set LED port GPIO_G10
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		OPTIONS
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			result
 *					
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/09/16		liyongnan		New
 ********************************************************************************************/
static void LED_SetPort(void)
{
	gpio_config(HAL_LED_PWM_PORT, GPIO_FUNC(10) | GPIO_OUTPUT);
	set_pwm_port(HAL_LED_PWM_CH, HAL_LED_PWM_PORT_NUM);
}
/********************************************************************************************/
/**	\function		LED_Open
 *	\date			2021/09/16
 *	\author			liyongnan
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
 *					0001	2021/09/16		liyongnan		New
 ********************************************************************************************/
int32 LED_Open(uint32 OPTIONS)
{
	int32 alRet;
	alRet = HAL_OK;

	if (OPTIONS != HAL_LED_OPTION_NORMAL) {
		alRet = HAL_ERR_NOT_SUPPORTED;
		LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>OPTIONS:%d", __FUNCTION__, __LINE__, OPTIONS);
	} else {
		nstLedType.uwOptions = (Type_uWord)OPTIONS;
		nstLedType.uwStatus = HAL_DEVICE_ST_OPENED;
		/* set pwm out port */
		LED_SetPort();
	}
	return alRet;
}
/********************************************************************************************/
/**	\function		LED_Close
 *	\date			2021/09/16
 *	\author			liyongnan
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
 *					0001	2021/09/16		liyongnan		New
 ********************************************************************************************/
int32 LED_Close(void)
{
	int32 alRet;

	alRet = HAL_OK;
	if (nstLedType.uwStatus == HAL_DEVICE_ST_NO_OPEN) {
		alRet = HAL_ERR_NOT_OPENED;
	//	LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>HAL_DEVICE_ST_NO_OPEN", __FUNCTION__, __LINE__);
	} else {
		nstLedType.uwStatus = HAL_DEVICE_ST_NO_OPEN;
		/* close pwm driver */
		pwm_disable(HAL_LED_PWM_CH);
	}

	return alRet;
}
/********************************************************************************************/
/**	\function		LED_Ioctl
 *	\date			2021/09/16
 *	\author			liyongnan
 *	\description	HAL IOctl
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		OPT
 *	\param[in]		PAR1
 *	\param[in/out]	PAR2
 *	\param[in/out]	PAR3
 *	\param[in/out]	PAR4
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			result
 *					
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/09/16		liyongnan		New
 ********************************************************************************************/
int32 LED_Ioctl(uint32 OPT,  void* PAR1, void* PAR2, void* PAR3, void* PAR4)
{
	int32 alRet;
	Type_uWord auwDuty_ns;
	Type_uHWord auhDuty;

	alRet= HAL_OK;
	auhDuty = HAL_LED_NULL;
	auwDuty_ns = HAL_LED_NULL;

	if (nstLedType.uwStatus == HAL_DEVICE_ST_NO_OPEN) {
		alRet = HAL_ERR_NOT_OPENED;
		LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>HAL_DEVICE_ST_NO_OPEN", __FUNCTION__, __LINE__);

	} else {
		switch (OPT) {
			case HAL_LED_IOCTL_SET_DUTY:
				if (PAR1 != STD_NULL) {
					auhDuty = *(Type_uHWord *)PAR1;
					if (auhDuty <= 10000) {
						LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d>auhDuty:%d", __FUNCTION__, __LINE__, auhDuty);
						/* Convert Duty To ns */
						auwDuty_ns = HAL_LED_PWM_FREQUENCY / 10000 * auhDuty;
						(void) pwm_config(HAL_LED_PWM_CH, auwDuty_ns, HAL_LED_PWM_FREQUENCY);
					} else {	
						alRet = HAL_ERR_BAD_PARAMETER;
						LOG_OUTPUT(LOG_APP_ID_LED, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>auhDuty:%d", __FUNCTION__, __LINE__, auhDuty);
					}
				}
				break;
			default:
				break;
		}
	}

	return alRet;
}

