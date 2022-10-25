/*******************************************************************************************/
/**<!-------Copyright (C) 2021 Alpine Electronics, Inc. All rights reserved. --------------->
 * 
 * \file					HAL_I2CKey.c
 * \date					2021/09/16
 * \author					Liyongnan
 * \model					EDA
 * \description				I2C key driver
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
#include <types.h>
#include <osal.h>
#include "i2c.h"

#include  "rpc_data.h"

#include "HAL_API.h"
#include "HAL_devices.h"
#include "HAL_I2CKey.h"
#include "key_core.h"

/********************************************************************************************/
/*							Global Variable Definition Section								*/
/********************************************************************************************/

/********************************************************************************************/
/*							Static Variable Definition Section								*/
/********************************************************************************************/
static Hal_I2cKey_Detect_Buff_ts nstI2cKeyDetect;
static Hal_I2cKey_Type_ts nstI2cKeyType;
/********************************************************************************************/
/*							ROM Table Section												*/
/********************************************************************************************/
static const Hal_I2cKey_ADtoKeyData_ts nstAdValueToKey_Tbl[] = {
/* AUDIO Key */
/* Key data    				A/D Voltage Max	 					A/D Voltage Min */
{ STK_KEY_TYPE_VOL_DOWN,	HAL_I2CKEY_AD_AUDIO_VOL_DOWN_MAX,	HAL_I2CKEY_AD_AUDIO_VOL_DOWN_MIN },	/* Vol- */
{ STK_KEY_TYPE_VOL_UP,		HAL_I2CKEY_AD_AUDIO_VOL_UP_MAX,		HAL_I2CKEY_AD_AUDIO_VOL_UP_MIN },	/* Vol+ */
{ STK_KEY_TYPE_CH_DOWN,		HAL_I2CKEY_AD_AUDIO_CH_DOWN_MAX,	HAL_I2CKEY_AD_AUDIO_CH_DOWN_MIN }, 	/* Ch- */
{ STK_KEY_TYPE_CH_UP,		HAL_I2CKEY_AD_AUDIO_CH_UP_MAX,		HAL_I2CKEY_AD_AUDIO_CH_UP_MIN }, 	/* Ch+ */
{ STK_KEY_TYPE_MODE,		HAL_I2CKEY_AD_AUDIO_MODE_MAX,		HAL_I2CKEY_AD_AUDIO_MODE_MIN },		/* Mode */
{ STK_KEY_TYPE_OFF, 		HAL_I2CKEY_AD_HFT_KEY_OFF_MAX,		HAL_I2CKEY_AD_HFT_KEY_OFF_MIN },	/* Key Off */
};
/********************************************************************************************/
/*							Static Prototype Declaration Section							*/
/********************************************************************************************/
static Type_uByte I2CKey_Getkeycodebyscancode(Type_uWord adcdata);
static Type_sWord I2CKey_Scan(Type_uByte mode, Type_uByte loop, Type_uByte *key_val);
static void I2CKey_ResetKeystatus (void);
/********************************************************************************************/
/*							Function Definition Section										*/
/********************************************************************************************/
/********************************************************************************************/
/**	\function		I2CKey_Open
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
int32 I2CKey_Open(uint32 OPTIONS)
{
	LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d>I2CKey_Open", __FUNCTION__, __LINE__);
	int32 alRet;
	
	alRet = OSAL_ERR_NONE;
	
	nstI2cKeyType.uwStatus = HAL_DEVICE_ST_NO_OPEN;
	if (OPTIONS != HAL_I2CKEY_OPTION_NORMAL) {
		LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>OPTIONS:%d", __FUNCTION__, __LINE__, OPTIONS);
		alRet = HAL_ERR_NOT_SUPPORTED;
	} else {
		nstI2cKeyType.uwOptions  = OPTIONS;
		nstI2cKeyType.uwStatus = HAL_DEVICE_ST_OPENED;
		// open driver
		alRet = tcc_i2c_open(HAL_I2CKEY_CHANNEL,
							HAL_I2CKEY_GPIO_SCL,
							HAL_I2CKEY_GPIO_SDA,
							HAL_I2CKEY_SCL_kHZ,
							HAL_I2CKEY_NULL,
							HAL_I2CKEY_NULL);
		I2CKEY_DBG("\n\nHAL_KEY_I2C%s:%d+++++++tcc_i2c_open : %d", __FUNCTION__, __LINE__,alRet);
		if(alRet != OSAL_ERR_NONE) {
			LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>tcc_i2c_open:%d", __FUNCTION__, __LINE__, alRet);
			alRet = HAL_ERR_NOT_INITIALIZED;
		} else {
			alRet = HAL_OK;
		}
		I2CKey_ResetKeystatus();
	}

	return alRet;
}
/********************************************************************************************/
/**	\function		I2CKey_Close
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
int32 I2CKey_Close(void)
{
//	LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d>I2CKey_Close", __FUNCTION__, __LINE__);
	int32 alRet;

	alRet = OSAL_ERR_NONE;

	if (nstI2cKeyType.uwStatus != HAL_DEVICE_ST_OPENED) {
//		LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>uwStatus:%d", __FUNCTION__, __LINE__, nstI2cKeyType.uwStatus);
		alRet = HAL_ERR_NOT_OPENED;
	} else {
		//close driver
		alRet = tcc_i2c_close(HAL_I2CKEY_CHANNEL);
		if(alRet != OSAL_ERR_NONE) {
//			LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>tcc_i2c_close:%d", __FUNCTION__, __LINE__, alRet);
			alRet = HAL_ERR_RESOURCE_CRASH;
		} else {
			alRet = HAL_OK;
		}

		if (alRet == HAL_OK) {
			nstI2cKeyType.uwStatus = HAL_DEVICE_ST_NO_OPEN;
		}
	}

	return alRet;
}
/********************************************************************************************/
/**	\function		I2CKey_Ioctl
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
int32 I2CKey_Ioctl(uint32 OPT, void* PAR1, void* PAR2, void* PAR3, void* PAR4)
{
	int32 alRet;
	Type_sWord aswRetI2c;
	Type_uByte aubMode;
	Type_uByte aubLoop;
	Type_uByte aubKeyVal;
	Type_uByte aubKeyAction;

	alRet = HAL_OK;
	aubMode = HAL_I2CKEY_NULL;
	aubLoop = HAL_I2CKEY_NULL;
	aubKeyVal = HAL_I2CKEY_NULL;
	aubKeyAction = HAL_I2CKEY_NULL;
	aswRetI2c = HAL_I2CKEY_NULL;

	if (nstI2cKeyType.uwStatus != HAL_DEVICE_ST_OPENED) {
		I2CKEY_DBG("\n\n%s:%d", __FUNCTION__, __LINE__);
		LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>Not open uwStatus:%d", __FUNCTION__, __LINE__, nstI2cKeyType.uwStatus);
		alRet = HAL_ERR_NOT_OPENED;
	} else {
		switch (OPT) {
			case HAL_I2CKEY_IOCTL_I2CKEY_READ:
				aubMode = *(Type_uByte *)PAR1;
				aubLoop = *(Type_uByte *)PAR2;

				aswRetI2c = I2CKey_Scan(aubMode, aubLoop, &aubKeyVal);
				if(aswRetI2c == HAL_OK) {
					/* adjust key value */
					nstI2cKeyDetect.ubConfirmKeyCode = aubKeyVal;
					if (aubKeyVal  != STK_KEY_TYPE_OFF) {
						/* key action on, backup key code */
//						nstI2cKeyDetect.ubConfirmKeyCode = aubKeyVal;
						aubKeyAction = KEY_ACTION_ON;
					} else {
						aubKeyAction = KEY_ACTION_OFF;
					}
					*((Type_uByte *)PAR3) = nstI2cKeyDetect.ubConfirmKeyCode;
					*((Type_uByte *)PAR4) = aubKeyAction;
					alRet = HAL_OK;
				} else {
					alRet = HAL_ERR_DEVICE_BUSY;
				}
				break;
			case HAL_I2CKEY_IOCTL_I2CKEY_RESET:
				I2CKey_ResetKeystatus();
				break;
			default:
				I2CKEY_DBG("\n\n%s:%d++++++OPT:%d", __FUNCTION__, __LINE__, OPT);
				break;
		}
	}

	return alRet;
}
/********************************************************************************************/
/**	\function		I2CKey_Getkeycodebyscancode
 *	\date			2021/09/16
 *	\author			liyongnan
 *	\description	Get key value from AD
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		adcdata
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			result
 *					
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/09/16		liyongnan		New
 ********************************************************************************************/
static Type_uByte I2CKey_Getkeycodebyscancode(Type_uWord adcdata)
{
	Type_uByte i;
	Type_uByte aubkey;
	Type_uWord auwdata;

	aubkey = HAL_I2CKEY_NULL;
	auwdata = HAL_I2CKEY_NULL;

	auwdata = (adcdata & 0x00000FFF);
	for (i = 0; i < (sizeof(nstAdValueToKey_Tbl) / sizeof(Hal_I2cKey_ADtoKeyData_ts)); i++) {
		if ((auwdata >= nstAdValueToKey_Tbl[i].uhMin) && (auwdata <= nstAdValueToKey_Tbl[i].uhMax)) {
			aubkey = nstAdValueToKey_Tbl[i].ubKey;
			break;
		}
	}
	
	return aubkey;
}

/********************************************************************************************/
/**	\function		I2CKey_Scan
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
static Type_sWord I2CKey_Scan(Type_uByte mode, Type_uByte loop, Type_uByte *key_val)
{
	Type_uByte aubread_data[HAL_I2CKEY_VALUE_2];
	Type_uByte aubReadReg;
	Type_uWord auwadc_data;
	Type_uByte aubkey;
	Type_sWord aswret;
	Type_sWord aswI2c_ret;

	OSAL_MemSet(aubread_data, HAL_I2CKEY_NULL, sizeof(aubread_data));
	aubReadReg = HAL_I2CKEY_REGISTER_ADDRESS;
	auwadc_data = HAL_I2CKEY_NULL;
	aubkey = STK_KEY_TYPE_OFF;
	aswret = HAL_ERR_NOT_OPENED;
	aswI2c_ret = OSAL_ERR_INVALID_HANDLE;

	aswI2c_ret = tcc_i2c_xfer(HAL_I2CKEY_CHANNEL, HAL_I2CKEY_SLAVE_ADDRESS, 
							HAL_I2CKEY_VALUE_1, &aubReadReg, 
							HAL_I2CKEY_VALUE_2, aubread_data, HAL_I2CKEY_OPT, HAL_I2CKEY_SYNC);

	if (aswI2c_ret != OSAL_ERR_NONE) {
		LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>tcc_i2c_xfer:%d", __FUNCTION__, __LINE__, aswI2c_ret);
		I2CKEY_DBG("\n read error\n");
	} else {
		auwadc_data = (((Type_uWord)aubread_data[0]) << 8) |aubread_data[1];
		aubkey = I2CKey_Getkeycodebyscancode(auwadc_data);
		/* check  */
		if (aubkey == HAL_I2CKEY_NULL) {
			/* invalid adc */
			I2CKEY_DBG("\n  <%s:%d>invalid adc auwadc_data:%x", __FUNCTION__, __LINE__, auwadc_data);
		} else {
			if (mode == KEY_STATUS_ON_LINETOOL) {
				if (aubkey != nstI2cKeyDetect.ubKey) {
					LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_DEBUG, ATAT_LEVEL_OFF, "\n <%s:%d>aubkey:%x", __FUNCTION__, __LINE__, aubkey);
					nstI2cKeyDetect.ubKey = aubkey;
					*key_val = nstI2cKeyDetect.ubKey;
					aswret = HAL_OK;
				} else {
					;
				}
			} else if (mode == KEY_STATUS_ON_NORMAL) {
				if (aubkey != nstI2cKeyDetect.ubKey) {
					LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_DEBUG, ATAT_LEVEL_OFF, "\n <%s:%d>aubkey:%x", __FUNCTION__, __LINE__, aubkey);
					nstI2cKeyDetect.ubKey = aubkey;
					nstI2cKeyDetect.ubCnt = HAL_I2CKEY_VALUE_1;
				} else {
					if (nstI2cKeyDetect.ubCnt < (loop - 1)) {
						nstI2cKeyDetect.ubCnt++;
					} else if (nstI2cKeyDetect.ubCnt == (loop - 1)) {
						if (nstI2cKeyDetect.ubConfirmKeyValue != nstI2cKeyDetect.ubKey) {
							/* send key */
							nstI2cKeyDetect.ubConfirmKeyValue = nstI2cKeyDetect.ubKey;
							*key_val = nstI2cKeyDetect.ubConfirmKeyValue;
							aswret = HAL_OK;
						} else {
							/* the same as before  */
							;
						}
					} else {
						LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>ubCnt:%d", __FUNCTION__, __LINE__, nstI2cKeyDetect.ubCnt);
					}
				}
			} else {
				LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>mode:%d", __FUNCTION__, __LINE__, mode);
			}
		}
	}
	return aswret;
}
/********************************************************************************************/
/**	\function		I2CKey_ResetKeystatus
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
static void I2CKey_ResetKeystatus (void)
{
	nstI2cKeyDetect.ubCnt = HAL_I2CKEY_NULL;
	nstI2cKeyDetect.ubKey = STK_KEY_TYPE_OFF;
	nstI2cKeyDetect.ubConfirmKeyValue = STK_KEY_TYPE_OFF;
	nstI2cKeyDetect.ubConfirmKeyCode = HAL_I2CKEY_NULL;
}
