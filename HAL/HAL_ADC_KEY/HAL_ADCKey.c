/*******************************************************************************************/
/**<!-------Copyright (C) 2021 Alpine Electronics, Inc. All rights reserved. --------------->
 * 
 * \file					HAL_ADCKey.c
 * \date					2021/09/16
 * \author					Liyongnan
 * \model					EDA
 * \description				ADC key driver
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

#include "rpc_data.h"

#include "HAL_API.h"
#include "HAL_devices.h"
#include "HAL_ADCKey.h"
#include "key_core.h"

/********************************************************************************************/
/*							Global Variable Definition Section								*/
/********************************************************************************************/

/********************************************************************************************/
/*							Static Variable Definition Section								*/
/********************************************************************************************/
static Hal_AdcKey_Detect_Buff_ts nstAdcKeyDetect;
static Hal_AdcKey_Type_ts nstAdcKeyType;

/********************************************************************************************/
/*							ROM Table Section												*/
/********************************************************************************************/
static const Hal_AdcKey_ADtoKeyData_ts nstAdValueToKey_Tbl[] = {
/* HFT Key */
/* Key data    				A/D Voltage Max	 		A/D Voltage Min */
{ STK_KEY_TYPE_ON_HOOK,		HAL_ADCKEY_AD_HFT_ON_HOOK_MAX,		HAL_ADCKEY_AD_HFT_ON_HOOK_MIN },	/* On Hook */
{ STK_KEY_TYPE_OFF_HOOK,	HAL_ADCKEY_AD_HFT_OFF_HOOK_MAX,		HAL_ADCKEY_AD_HFT_OFF_HOOK_MIN },	/* Off Hook */
{ STK_KEY_TYPE_TALK,		HAL_ADCKEY_AD_HFT_TALK_MAX,			HAL_ADCKEY_AD_HFT_TALK_MIN }, 		/* Talk */
{ STK_KEY_TYPE_OFF,			HAL_ADCKEY_AD_HFT_KEY_OFF_MAX,		HAL_ADCKEY_AD_HFT_KEY_OFF_MIN }, 	/* Key Off */
};
/********************************************************************************************/
/*							Static Prototype Declaration Section							*/
/********************************************************************************************/
static Type_uByte ADCKey_Getkeycodebyscancode(Type_uWord adcdata);
static Type_sWord ADCKey_Scan(Type_uByte mode, Type_uByte loop, Type_uByte *key_val);
static void ADCKey_ResetKeystatus (void);
/********************************************************************************************/
/*							Function Definition Section										*/
/********************************************************************************************/
/********************************************************************************************/
/**	\function		ADCKey_Open
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
int32 ADCKey_Open(uint32 OPTIONS)
{
	LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d>ADCKey_Open", __FUNCTION__, __LINE__);
	int32 alRet;
	
	alRet = HAL_ERR_NOT_OPENED;
	
	nstAdcKeyType.uwStatus = HAL_DEVICE_ST_NO_OPEN;

	if (OPTIONS  != HAL_ADCKEY_OPTION_NORMAL) {
		LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>OPTIONS:%d", __FUNCTION__, __LINE__, OPTIONS);
		alRet = HAL_ERR_NOT_SUPPORTED;
	} else {
		nstAdcKeyType.uwOptions  = (Type_uWord)OPTIONS;
		nstAdcKeyType.uwStatus = HAL_DEVICE_ST_OPENED;
		ADCKey_ResetKeystatus();
		alRet = HAL_OK;
	}
	return alRet;
}
/********************************************************************************************/
/**	\function		ADCKey_Close
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
int32 ADCKey_Close(void)
{
//	LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d>ADCKey_Close", __FUNCTION__, __LINE__);
	int32 alRet;

	alRet = HAL_ERR_NOT_OPENED;

	if (nstAdcKeyType.uwStatus != HAL_DEVICE_ST_OPENED ) {
//		LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>uwStatus:%d", __FUNCTION__, __LINE__, nstAdcKeyType.uwStatus);
		alRet = HAL_ERR_NOT_OPENED;
	} else {
		nstAdcKeyType.uwStatus = HAL_DEVICE_ST_NO_OPEN;
		alRet = HAL_OK;
	}

	return alRet;
}
/********************************************************************************************/
/**	\function		ADCKey_Ioctl
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
int32 ADCKey_Ioctl(uint32 OPT, void* PAR1, void* PAR2, void* PAR3, void* PAR4)
{
	int32 alRet;
	Type_uByte aubMode;
	Type_uByte aubLoop;
	Type_uByte aubKeyVal;
	Type_uByte aubKeyAction;
	Type_sWord aswRetAdc;

	alRet = HAL_ERR_NOT_OPENED;
	aubMode = HAL_ADCKEY_NULL;
	aubLoop = HAL_ADCKEY_NULL;
	aubKeyVal = HAL_ADCKEY_NULL;
	aubKeyAction = HAL_ADCKEY_NULL;
	aswRetAdc = HAL_ADCKEY_NULL;

	if (nstAdcKeyType.uwStatus != HAL_DEVICE_ST_OPENED) {
		LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>Not open uwStatus:%d", __FUNCTION__, __LINE__, nstAdcKeyType.uwStatus);
		alRet = HAL_ERR_NOT_OPENED;
	} else {
		switch (OPT) {
			case HAL_ADCKEY_IOCTL_ADCKEY_READ:
				aubMode = *(Type_uByte *)PAR1;
				aubLoop = *(Type_uByte *)PAR2;
				aswRetAdc = ADCKey_Scan(aubMode, aubLoop, &aubKeyVal);
				if(aswRetAdc == HAL_OK) {
					/* adjust key value */
					nstAdcKeyDetect.ubConfirmKeyCode = aubKeyVal;
					if (aubKeyVal != STK_KEY_TYPE_OFF) {
						/* key action on, backup key code */
//					nstAdcKeyDetect.ubConfirmKeyCode = aubKeyVal;
						aubKeyAction = KEY_ACTION_ON;
					} else {
						aubKeyAction = KEY_ACTION_OFF;
					}
					*((Type_uByte *)PAR3) = nstAdcKeyDetect.ubConfirmKeyCode;
					*((Type_uByte *)PAR4) = aubKeyAction;
					alRet = HAL_OK;
				} else {
					alRet = HAL_ERR_DEVICE_BUSY;
				}
				break;
			case HAL_ADCKEY_IOCTL_ADCKEY_RESET:
				ADCKey_ResetKeystatus();
				break;
			default:
				break;
		}
	}

	return alRet;
}
/********************************************************************************************/
/**	\function		ADCKey_Getkeycodebyscancode
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
static Type_uByte ADCKey_Getkeycodebyscancode(Type_uWord adcdata)
{
	Type_uByte i ;
	Type_uByte aubkey;
	Type_uWord auwdata;

	aubkey = HAL_ADCKEY_NULL;
	auwdata = HAL_ADCKEY_NULL;

	auwdata = (adcdata & 0x00000FFF);
	for (i = 0; i < (sizeof(nstAdValueToKey_Tbl) / sizeof(Hal_AdcKey_ADtoKeyData_ts)); i++) {
		if ((auwdata >= nstAdValueToKey_Tbl[i].uhMin) && (auwdata <= nstAdValueToKey_Tbl[i].uhMax)) {
			aubkey = nstAdValueToKey_Tbl[i].ubKey;
			break;
		}
	}
	
	return aubkey;
}
/********************************************************************************************/
/**	\function		ADCKey_Scan
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
static Type_sWord ADCKey_Scan(Type_uByte mode, Type_uByte loop, Type_uByte *key_val)
{
	Type_uWord auwread_data;
	Type_sWord aswret;
	Type_uByte aubkey;

	auwread_data = HAL_ADCKEY_NULL;
	aubkey = HAL_ADCKEY_NULL;
	aswret = HAL_ERR_NOT_OPENED;

	auwread_data = (Type_uWord)tcc_adc_read(HAL_ADCKEY_CHANNEL);
	aubkey = ADCKey_Getkeycodebyscancode(auwread_data);
	/* check  */
	if (aubkey == HAL_ADCKEY_NULL) {
		/* invalid adc */
	} else {
		if (mode == KEY_STATUS_ON_LINETOOL) {
			if (aubkey != nstAdcKeyDetect.ubKey) {
				LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_DEBUG, ATAT_LEVEL_OFF, "\n <%s:%d>aubkey:%x", __FUNCTION__, __LINE__, aubkey);
				nstAdcKeyDetect.ubKey = aubkey;
				*key_val = nstAdcKeyDetect.ubKey;
				aswret = HAL_OK;
			} else {
				;
			}
		} else if (mode == KEY_STATUS_ON_NORMAL) {
			if (aubkey != nstAdcKeyDetect.ubKey) {
				LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_DEBUG, ATAT_LEVEL_OFF, "\n <%s:%d>aubkey:%x", __FUNCTION__, __LINE__, aubkey);
				nstAdcKeyDetect.ubKey = aubkey;
				nstAdcKeyDetect.ubCnt = HAL_ADCKEY_VALUE_1;
				
			} else {
				if (nstAdcKeyDetect.ubCnt < (loop - 1)) {
					nstAdcKeyDetect.ubCnt++;
				} else if (nstAdcKeyDetect.ubCnt == (loop - 1)) {
					if (nstAdcKeyDetect.ubConfirmKeyValue != nstAdcKeyDetect.ubKey) {
						/* send key */
						nstAdcKeyDetect.ubConfirmKeyValue = nstAdcKeyDetect.ubKey;
						*key_val = nstAdcKeyDetect.ubConfirmKeyValue;
						aswret = HAL_OK;
					} else {
						/* the same as before  */
						;
					}
				} else {
					LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_DEBUG, ATAT_LEVEL_OFF, "\n <%s:%d>ubCnt:%x", __FUNCTION__, __LINE__, nstAdcKeyDetect.ubCnt);
				}
			}
		} else {
			LOG_OUTPUT(LOG_APP_ID_KEY, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d>mode:%d", __FUNCTION__, __LINE__, mode);
		}
	}

	return aswret;
}
/********************************************************************************************/
/**	\function		ADCKey_ResetKeystatus
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
static void ADCKey_ResetKeystatus (void)
{
	nstAdcKeyDetect.ubCnt = HAL_ADCKEY_NULL;
	nstAdcKeyDetect.ubKey = STK_KEY_TYPE_OFF;
	nstAdcKeyDetect.ubConfirmKeyValue = STK_KEY_TYPE_OFF;
	nstAdcKeyDetect.ubConfirmKeyCode = HAL_ADCKEY_NULL;
}
