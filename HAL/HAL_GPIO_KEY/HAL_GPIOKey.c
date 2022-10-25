/*******************************************************************************************/
/**<!-------Copyright (C) 2021 Alpine Electronics, Inc. All rights reserved. --------------->
 * 
 * \file					HAL_GPIOKey.c
 * \date					2021/09/16
 * \author					Liyongnan
 * \model					EDA
 * \description				GPIO key driver
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
#include "os_framework.h"

#include "gpio.h"
#include "rpc_event.h"
#include "rpc_data.h"

#include "HAL_API.h"
#include "HAL_devices.h"
#include "HAL_GPIOKey.h"
#include "key_core.h"

/********************************************************************************************/
/*							Global Variable Definition Section								*/
/********************************************************************************************/

/********************************************************************************************/
/*							Static Variable Definition Section								*/
/********************************************************************************************/
static Hal_GpioKey_Type_ts nstGpioKeyType;
static Hal_GpioKey_Detect_Buff_ts nstGpioKeyDetect[HAL_GPIOKEY_VALUE_6];
/********************************************************************************************/
/*							ROM Table Section												*/
/********************************************************************************************/
static const Hal_GpioKeyDataTbl_ts nstHARDKey_Tbl[HAL_GPIOKEY_VALUE_6] = { /* Hard Key */
/* Key data    				GPIO port */
{HKY_KEY_TYPE_HOME,			HAL_GPIOKEY_HOME_KEY_PORT}, 		/* HOME_KEY */
{HKY_KEY_TYPE_DAY_NIGHT,	HAL_GPIOKEY_DAY_NIGHT_KEY_PORT},	/* DAY/NIGHT_KEY */
{HKY_KEY_TYPE_BACK, 		HAL_GPIOKEY_BACK_KEY_PORT},			/* BACK_KEY */
{HKY_KEY_TYPE_VOL_DOWN, 	HAL_GPIOKEY_VOL_DOWN_KEY_PORT},		/* VOL-_KEY */
{HKY_KEY_TYPE_VOL_UP,		HAL_GPIOKEY_VOL_UP_KEY_PORT},		/* VOL+_KEY */
{HKY_KEY_TYPE_POWER, 		HAL_GPIOKEY_POWER_KEY_PORT},		/* POWER_KEY */
};

/********************************************************************************************/
/*							Static Prototype Declaration Section							*/
/********************************************************************************************/
static Type_sWord GPIOKey_Scan(Type_uByte mode, Type_uByte loop, Type_uByte *key_val, Type_uByte *key_action);
static void GPIOKey_ResetKeystatus (void);

/********************************************************************************************/
/*							Function Definition Section										*/
/********************************************************************************************/
/********************************************************************************************/
/**	\function		GPIOKey_Open
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
int32 GPIOKey_Open(uint32 OPTIONS)
{
	LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d>GPIOKey_Open", __FUNCTION__, __LINE__);
	int32 alRet;

	alRet = HAL_ERR_NOT_OPENED;
	
	nstGpioKeyType.uwStatus = HAL_DEVICE_ST_NO_OPEN;
	
	if (OPTIONS != HAL_GPIOKEY_OPTION_NORMAL) {
		LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>OPTIONS:%d", __FUNCTION__, __LINE__, OPTIONS);
		alRet = HAL_ERR_NOT_SUPPORTED;
	} else {
		nstGpioKeyType.uwOptions = OPTIONS;
		nstGpioKeyType.uwStatus = HAL_DEVICE_ST_OPENED;
		GPIOKey_ResetKeystatus();
		alRet = HAL_OK;
	}
	return alRet;
}
/********************************************************************************************/
/**	\function		GPIOKey_Close
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
int32 GPIOKey_Close(void)
{
//	LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d>GPIOKey_Close", __FUNCTION__, __LINE__);
	int32 alRet;

	alRet = HAL_ERR_NOT_OPENED;

	if (nstGpioKeyType.uwStatus != HAL_DEVICE_ST_OPENED) {
//		LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>uwStatus:%d", __FUNCTION__, __LINE__, nstGpioKeyType.uwStatus);
		alRet = HAL_ERR_NOT_OPENED;
	} else {
		nstGpioKeyType.uwStatus = HAL_DEVICE_ST_NO_OPEN;
		alRet = HAL_OK;
	}

	return alRet;
}
/********************************************************************************************/
/**	\function		GPIOKey_Ioctl
 *	\date			2021/09/16
 *	\author			liyongnan
 *	\description	HAL IOctl
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		OPT
 *	\param[in]		PAR1
 *	\param[in]		PAR2
 *	\param[out]		PAR3
 *	\param[out]		PAR4
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			result
 *					
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/09/16		liyongnan		New
 ********************************************************************************************/
int32 GPIOKey_Ioctl(uint32 OPT, void* PAR1, void* PAR2, void* PAR3, void* PAR4)
{
	Type_uByte aubKeyVal[HAL_GPIOKEY_VALUE_6];
	Type_uByte aubKeyAction[HAL_GPIOKEY_VALUE_6];
	Type_sWord aswRetGpio;
	int32 alRet;
	Type_uByte aubMode;
	Type_uByte aubLoop;

	alRet = HAL_OK;
	aubMode = HAL_GPIOKEY_NULL;
	aubLoop = HAL_GPIOKEY_NULL;
	aswRetGpio = HAL_GPIOKEY_NULL;
	OSAL_MemSet(aubKeyVal, KEY_VALUE_0, sizeof(aubKeyVal));
	OSAL_MemSet(aubKeyAction, KEY_VALUE_0, sizeof(aubKeyAction));

	if (nstGpioKeyType.uwStatus != HAL_DEVICE_ST_OPENED) {
		LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>Not open uwStatus:%d", __FUNCTION__, __LINE__, nstGpioKeyType.uwStatus);
		alRet = HAL_ERR_NOT_OPENED;
	} else {
		switch (OPT) {
			case HAL_GPIOKEY_IOCTL_GPIOKEY_READ:
				aubMode = *(Type_uByte *)PAR1;
				aubLoop = *(Type_uByte *)PAR2;
				aswRetGpio = GPIOKey_Scan(aubMode, aubLoop, aubKeyVal, aubKeyAction);
				if(aswRetGpio == HAL_OK) {
					/* adjust key value */
					wvdGen_MemCpy((Type_uByte *)PAR3, &aubKeyVal, HAL_GPIOKEY_VALUE_6);
					wvdGen_MemCpy((Type_uByte *)PAR4, &aubKeyAction, HAL_GPIOKEY_VALUE_6);
					alRet = HAL_OK;
				} else {
					alRet = HAL_ERR_DEVICE_BUSY;
				}
				break;

			case HAL_GPIOKEY_IOCTL_GPIOKEY_RESET:
				GPIOKey_ResetKeystatus();
				break;

			default:
				break;
		}
	}

	return alRet;
}
/********************************************************************************************/
/**	\function		GPIOKey_Scan
 *	\date			2021/09/16
 *	\author			liyongnan
 *	\description	Scan Key
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		mode (linetool/mormal)
 *	\param[in]		loop (noise reduce count)
 *	\param[out]		key_val
 *	\param[out]		key_action
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			result
 *					
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/09/16		liyongnan		New
 ********************************************************************************************/
static Type_sWord GPIOKey_Scan(Type_uByte mode, Type_uByte loop, Type_uByte *key_val, Type_uByte *key_action)
{
	Type_sWord aswread_data;
	Type_sWord aubret;
	Type_uByte aubaction;
	Type_uByte i;
	Type_uByte j;

	aswread_data = HAL_GPIOKEY_NULL;
	aubaction = HAL_GPIOKEY_NULL;
	aubret = HAL_ERR_NOT_OPENED;
	i = HAL_GPIOKEY_NULL;
	j = HAL_GPIOKEY_NULL;

	for (i = 0; i < HAL_GPIOKEY_VALUE_6; i++) {
		aswread_data = gpio_get(nstHARDKey_Tbl[i].port);

		if (aswread_data == HAL_GPIOKEY_GPIO_HIGH) {
			GPIOKEY_DBG("\n  <%s:%d> idx:%d port:%x aswread_data:%d", __FUNCTION__, __LINE__, i, nstHARDKey_Tbl[i].port, aswread_data);
			aubaction = KEY_ACTION_ON;
		} else {
			aubaction = KEY_ACTION_OFF;
		}

		if (mode == KEY_STATUS_ON_LINETOOL) {
			if (aubaction != nstGpioKeyDetect[i].ubAction) {
				LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_DEBUG, ATAT_LEVEL_OFF, "\n <%s:%d>aubaction:%x", __FUNCTION__, __LINE__, aubaction);
				nstGpioKeyDetect[i].ubAction = aubaction;
				*(key_val + j) = nstHARDKey_Tbl[i].uhKey;
				*(key_action + j) = nstGpioKeyDetect[i].ubAction;
				j++;
			} else {
				;
			}
		} else if (mode == KEY_STATUS_ON_NORMAL) {
			if (aubaction != nstGpioKeyDetect[i].ubAction) {
				LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_DEBUG, ATAT_LEVEL_OFF, "\n <%s:%d>aubaction:%x", __FUNCTION__, __LINE__, aubaction);
				nstGpioKeyDetect[i].ubCnt = HAL_GPIOKEY_VALUE_1;
				nstGpioKeyDetect[i].ubAction = aubaction;
			} else {
				if (nstGpioKeyDetect[i].ubCnt < (loop - 1)) {
					nstGpioKeyDetect[i].ubCnt++;
				} else if (nstGpioKeyDetect[i].ubCnt == (loop - 1)) {
					if (nstGpioKeyDetect[i].ubConfirmAction != nstGpioKeyDetect[i].ubAction) {
						/* send key */
						nstGpioKeyDetect[i].ubConfirmAction = nstGpioKeyDetect[i].ubAction;
						*(key_val + j) = nstHARDKey_Tbl[i].uhKey;
						*(key_action + j) = nstGpioKeyDetect[i].ubAction;
						j++;
					} else {
						/*	do nothing */
					}
				} else {
					LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_DEBUG, ATAT_LEVEL_OFF, "\n <%s:%d>[%d] ubCnt:%x", __FUNCTION__, __LINE__, i, nstGpioKeyDetect[i].ubCnt);
				}
			}
		
		} else {
			LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>mode:%d", __FUNCTION__, __LINE__, mode);
		}
	}
	/* judge if send  */
	if (j != HAL_GPIOKEY_NULL) {
		aubret = HAL_OK;
	} else {
		aubret = HAL_ERR_NOT_OPENED;
	}

	return aubret;
}
/********************************************************************************************/
/**	\function		GPIOKey_ResetKeystatus
 *	\date			2021/09/16
 *	\author			liyongnan
 *	\description	Reset variable
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		none
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			none
 *					
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/09/16		liyongnan		New
 ********************************************************************************************/
static void GPIOKey_ResetKeystatus (void)
{
	Type_uByte i;

	for (i = 0; i < HAL_GPIOKEY_VALUE_6; i++) {
		nstGpioKeyDetect[i].ubCnt = HAL_GPIOKEY_NULL;
		nstGpioKeyDetect[i].ubAction = KEY_ACTION_OFF;
		nstGpioKeyDetect[i].ubConfirmAction = KEY_ACTION_OFF;
	}
}

