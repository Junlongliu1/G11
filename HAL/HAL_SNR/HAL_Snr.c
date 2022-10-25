/*******************************************************************************************/
/**<!-------Copyright (C) 2021 Alpine Electronics, Inc. All rights reserved. --------------->
 *
 * \file					HAL_Snr.c
 * \date					2021/10/25
 * \author					Guanjiafu
 * \model					EDA
 * \description				SNR driver
 * \version					0.55
 *
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/10/25		Guanjiafu		New
 ********************************************************************************************/
/********************************************************************************************/
/*							Include File Section											*/
/********************************************************************************************/
#include "os_framework.h"

#include "debug.h"
#include "pwm.h"
#include "gpio.h"
#include "ictc.h"
#include "rpc_data.h"

#include "HAL_API.h"
#include "HAL_devices.h"
#include "HAL_Snr.h"
#include "sensor_core.h"
/********************************************************************************************/
/*							Static Variable Definition Section								*/
/********************************************************************************************/
static Hal_Snr_Type_ts nstSNR_Type;
static Hal_Snr_HiTempCheckData_ts nstSNR_HiTempCheck;
static Hal_Snr_GPIOCheckData_ts nstSNR_GPIOCheck[HAL_SNR_VALUE_3];
static Hal_Snr_VspCheckData_ts nstSNR_VspCheck;
static Hal_Snr_TempData_ts nstSNR_TempCheck;

/********************************************************************************************/
/*							ROM Table Section												*/
/********************************************************************************************/
static const Hal_Snr_GPIOCheckDataTbl_ts nstGpioCheck_Tbl[] = {
/* ILL/PARK/TELMUTE */
/* GPIO port		 	Check count */
{HAL_SNR_ILL_PORT,		HAL_SNR_VALUE_6},	/* ILL */
{HAL_SNR_PARK_PORT, 	HAL_SNR_VALUE_5},	/* PARK */
{HAL_SNR_TELMUTE_PORT, 	HAL_SNR_VALUE_10}	/* TELMUTE */
};
/********************************************************************************************/
/*							Static Prototype Declaration Section							*/
/********************************************************************************************/
static void SNR_GpioInit(void);
static void SNR_IctcInit(void);
static void SNR_VariableInit(void);
static int32 SNR_SetFan(void* PAR1);
static int32 SNR_CheckClockTemp(void* PAR1);
static int32 SNR_CheckHiTemp(void* PAR1);
static int32 SNR_CheckVsp(void* PAR1);
static int32 SNR_CheckGpio(Type_uByte index, void* PAR1);

/********************************************************************************************/
/*							Function Definition Section										*/
/********************************************************************************************/
/********************************************************************************************/
/**	\function		Snr_Open
 *	\date			2021/10/25
 *	\author			Guanjiafu
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
 *					0001	2021/10/25		Guanjiafu		New
 ********************************************************************************************/
int32 SNR_Open(uint32 OPTIONS)
{
	LOG_OUTPUT(LOG_APP_ID_SNR, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d>", __FUNCTION__, __LINE__);
	int32 alRet;

	alRet = HAL_OK;

	if (OPTIONS != HAL_SNR_OPTION_NORMAL) {
		LOG_OUTPUT(LOG_APP_ID_SNR, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d> OPTIONS:%d", __FUNCTION__, __LINE__, OPTIONS);
		alRet = HAL_ERR_NOT_SUPPORTED;
	} else {
		nstSNR_Type.uwOptions = (Type_uWord)OPTIONS;
		nstSNR_Type.uwStatus = HAL_DEVICE_ST_OPENED;
		SNR_VariableInit();
		SNR_GpioInit();
		/* init port for VSP */
		SNR_IctcInit();
		alRet = HAL_OK;
	}
	return alRet;
}
/********************************************************************************************/
/**	\function		Snr_Close
 *	\date			2021/10/25
 *	\author			Guanjiafu
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
 *					0001	2021/10/25		Guanjiafu		New
 ********************************************************************************************/
int32 SNR_Close(void)
{
	// LOG_OUTPUT(LOG_APP_ID_SNR, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n <%s:%d>", __FUNCTION__, __LINE__);
	int32 alRet;

	alRet = HAL_OK;
	if (nstSNR_Type.uwStatus == HAL_DEVICE_ST_NO_OPEN) {
	//	LOG_OUTPUT(LOG_APP_ID_SNR, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d> HAL_DEVICE_ST_NO_OPEN", __FUNCTION__, __LINE__);
		alRet = HAL_ERR_NOT_OPENED;
	} else {
		nstSNR_Type.uwStatus = HAL_DEVICE_ST_NO_OPEN;
	}

	return alRet;
}
/********************************************************************************************/
/**	\function		SNR_Ioctl
 *	\date			2021/10/25
 *	\author			Guanjiafu
 *	\description	HAL IOctl
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		OPT
 *	\param[in/out]		PAR1
 *	\param[in/out]	PAR2
 *	\param[in/out]	PAR3
 *	\param[in/out]	PAR4
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			result
 *
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/10/25		Guanjiafu		New
 ********************************************************************************************/
int32 SNR_Ioctl(uint32 OPT,  void* PAR1, void* PAR2, void* PAR3, void* PAR4)
{
	int32 alRet;

	alRet = HAL_OK;

	if (nstSNR_Type.uwStatus == HAL_DEVICE_ST_NO_OPEN) {
		alRet = HAL_ERR_NOT_OPENED;
	} else {
		switch (OPT) {
			case HAL_SNR_IOCTL_SET_FAN:
				if (PAR1 != STD_NULL) {
					alRet = SNR_SetFan(PAR1);
				}else {
					LOG_OUTPUT(LOG_APP_ID_SNR, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d> parameter error!!!!", __FUNCTION__, __LINE__);
					alRet = HAL_ERR_BAD_PARAMETER;
				}
				break;

			case HAL_SNR_IOCTL_CHECK_CLOCKTEMP:
				if (PAR1 != STD_NULL) {
					alRet = SNR_CheckClockTemp(PAR1);
				} else {
					/* paramater error */
					LOG_OUTPUT(LOG_APP_ID_SNR, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d> parameter error!!!!", __FUNCTION__, __LINE__);
					alRet = HAL_ERR_BAD_PARAMETER;
				}
				break;

			case HAL_SNR_IOCTL_CHECK_HiTEMP:
				if (PAR1 != STD_NULL) {
					alRet = SNR_CheckHiTemp(PAR1);
				} else {
					/* paramater error */
					LOG_OUTPUT(LOG_APP_ID_SNR, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d> parameter error!!!!", __FUNCTION__, __LINE__);
					alRet = HAL_ERR_BAD_PARAMETER;
				}

				break;

			case HAL_SNR_IOCTL_CHECK_VSP:
				if (PAR1 != STD_NULL) {
					alRet = SNR_CheckVsp(PAR1);
				} else {
					/* paramater error */
					LOG_OUTPUT(LOG_APP_ID_SNR, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d> parameter error!!!!", __FUNCTION__, __LINE__);
					alRet = HAL_ERR_BAD_PARAMETER;
				}
				break;

			case HAL_SNR_IOCTL_CHECK_PARK:
				if (PAR1 != STD_NULL) {			
					alRet = SNR_CheckGpio(HAL_SNR_GPIO_INDEX_PARK, PAR1);
				} else {
					/* paramater error */
					LOG_OUTPUT(LOG_APP_ID_SNR, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d> parameter error!!!!", __FUNCTION__, __LINE__);
					alRet = HAL_ERR_BAD_PARAMETER;
				}
				break;

			case HAL_SNR_IOCTL_CHECK_ILLM:
				if (PAR1 != STD_NULL) {
					alRet = SNR_CheckGpio(HAL_SNR_GPIO_INDEX_ILL, PAR1);
				} else {
					/* paramater error */
					LOG_OUTPUT(LOG_APP_ID_SNR, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d> parameter error!!!!", __FUNCTION__, __LINE__);
					alRet = HAL_ERR_BAD_PARAMETER;
				}
				break;	

			case HAL_SNR_IOCTL_CHECK_TELMUTE:
				if (PAR1 != STD_NULL) {
					alRet = SNR_CheckGpio(HAL_SNR_GPIO_INDEX_TELMUTE, PAR1);
				} else {
					/* paramater error */
					LOG_OUTPUT(LOG_APP_ID_SNR, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d> parameter error!!!!", __FUNCTION__, __LINE__);
					alRet = HAL_ERR_BAD_PARAMETER;
				}
				break;	

			default:
				break;
			}
	}
	return alRet;
}
/********************************************************************************************/
/**	\function		SNR_IctcInit
 *	\date			2021/10/25
 *	\author			Guanjiafu
 *	\description	config VSP ictc port
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		none
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			none
 *
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/10/25		Guanjiafu		New
 ********************************************************************************************/
static void SNR_IctcInit(void)
{
	int32 gfb_port_num;

	(void)gpio_config(HAL_SNR_VSP_ICTC_PORT, GPIO_FUNC(0)|GPIO_INPUT|GPIO_INPUTBUF_EN);

	gfb_port_num = gpio_to_num(HAL_SNR_VSP_ICTC_PORT);
	ictc_set_port(HAL_SNR_VSP_ICTC_CH, gfb_port_num);
	ictc_set_clk(HAL_SNR_VSP_ICTC_CH, HAL_SNR_VSP_ICTC_DIV_CLK_SEL); //set clk

	ictc_set_redge_vl(HAL_SNR_VSP_ICTC_CH, 1000);
	ictc_set_fedge_vl(HAL_SNR_VSP_ICTC_CH, 1000);

	ictc_set_filter_fmode(HAL_SNR_VSP_ICTC_CH, 3);
	ictc_set_filter_rmode(HAL_SNR_VSP_ICTC_CH, 3);

	ictc_start(HAL_SNR_VSP_ICTC_CH);
}
/********************************************************************************************/
/**	\function		SNR_GpioInit
 *	\date			2021/10/25
 *	\author			Guanjiafu
 *	\description	config gpio port(ill/park/reverse/FAN)
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		none
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			none
 *
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/10/25		Guanjiafu		New
 ********************************************************************************************/
static void SNR_GpioInit(void)
{
	Type_uByte i;

	/* ill/park/reverse */
	for (i = 0; i < HAL_SNR_VALUE_3; i++) {
		gpio_config(nstGpioCheck_Tbl[i].port, (GPIO_FUNC(0)|GPIO_INPUT|GPIO_INPUTBUF_EN));
	}
	/* fan */
	gpio_config(HAL_SNR_FAN_PORT, (GPIO_FUNC(0)|GPIO_OUTPUT));

}
/********************************************************************************************/
/**	\function		SNR_VariableInit
 *	\date			2021/10/25
 *	\author			Guanjiafu
 *	\description	init variable of HAL_SNR
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		none
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			none
 *
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/10/25		Guanjiafu		New
 ********************************************************************************************/
static void SNR_VariableInit(void)
{
	Type_uByte i;

	OSAL_MemSet(&nstSNR_HiTempCheck, HAL_SNR_VALUE_0, sizeof(Hal_Snr_HiTempCheckData_ts));
	for (i = 0; i < HAL_SNR_VALUE_3; i++) {
		nstSNR_GPIOCheck[i].ubCnt = HAL_SNR_VALUE_0;
		nstSNR_GPIOCheck[i].ubStatus = HAL_SNR_BYTE_INVALID;
	}
	OSAL_MemSet(&nstSNR_VspCheck, HAL_SNR_VALUE_0, sizeof(Hal_Snr_VspCheckData_ts));
	OSAL_MemSet(&nstSNR_TempCheck, HAL_SNR_VALUE_0, sizeof(Hal_Snr_TempData_ts));

}
/********************************************************************************************/
/**	\function		SNR_SetFan
 *	\date			2021/10/25
 *	\author			Guanjiafu
 *	\description	set fan gpio
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		value
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			result
 *
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/10/25		Guanjiafu		New
 ********************************************************************************************/
static int32 SNR_SetFan(void* PAR1)
{
	int32 alRet;
	int32 alGpioRet;
	Type_uByte aubFan;

	alRet = HAL_OK;
	alGpioRet = HAL_SNR_VALUE_0;
	aubFan = HAL_SNR_VALUE_0;

	aubFan = *(Type_uByte *)PAR1;
	if (SNR_INF_FAN_ON	== aubFan) {
		alGpioRet = gpio_set(HAL_SNR_FAN_PORT, 1);	
		if (alGpioRet == 0) {
			alRet = HAL_OK;
		} else {
			LOG_OUTPUT(LOG_APP_ID_SNR, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d> gpio_set error!!!!", __FUNCTION__, __LINE__);
			alRet = HAL_ERR_HAL_DEV;
		}
	} else if (SNR_INF_FAN_OFF	== aubFan) {
		alGpioRet = gpio_set(HAL_SNR_FAN_PORT, 0);
		if (alGpioRet == 0) {
			alRet = HAL_OK;
		} else {
			LOG_OUTPUT(LOG_APP_ID_SNR, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d> gpio_set error!!!!", __FUNCTION__, __LINE__);
			alRet = HAL_ERR_HAL_DEV;
		}
	} else {
		LOG_OUTPUT(LOG_APP_ID_SNR, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d> parameter error!!!!", __FUNCTION__, __LINE__);
		//accord temp control fan on/off
		alRet = HAL_ERR_BAD_PARAMETER;
	}

	return alRet;
}
/********************************************************************************************/
/**	\function		SNR_CheckClockTemp
 *	\date			2021/10/25
 *	\author			Guanjiafu
 *	\description	check sys temp of 1H for clock
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[out]		temp value
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			result
 *
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/10/25		Guanjiafu		New
 ********************************************************************************************/
static int32 SNR_CheckClockTemp(void* PAR1)
{
	int32 alRet;
	Type_uWord auhAdcValue;
	Type_uHWord auhTempArg;

	alRet = HAL_OK;
	auhAdcValue = HAL_SNR_VALUE_0;
	auhTempArg = HAL_SNR_VALUE_0;

	auhAdcValue = tcc_adc_read(HAL_SNR_ADC_TEMP_CH);
	/* save data */
	if (nstSNR_TempCheck.uhCnt < (HAL_SNR_CLOCK_TEMP_READ_COUNT - 1)) {
		nstSNR_TempCheck.uwTotalADValue += auhAdcValue;
		nstSNR_TempCheck.uhCnt++;
		alRet = HAL_ERR_DEVICE_BUSY;
	} else {
		nstSNR_TempCheck.uwTotalADValue += auhAdcValue;
		auhTempArg = (Type_uHWord)(nstSNR_TempCheck.uwTotalADValue / HAL_SNR_CLOCK_TEMP_READ_COUNT);
		nstSNR_TempCheck.uhCnt = HAL_SNR_VALUE_0;
		nstSNR_TempCheck.uwTotalADValue = HAL_SNR_VALUE_0;
		*(Type_uHWord *)PAR1 = auhTempArg;
		alRet = HAL_OK;
	}

	return alRet;
}
/********************************************************************************************/
/**	\function		SNR_CheckHiTemp
 *	\date			2021/10/25
 *	\author			Guanjiafu
 *	\description	check sys temp
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[out]		temp value
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			result
 *
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/10/25		Guanjiafu		New
 ********************************************************************************************/
static int32 SNR_CheckHiTemp(void* PAR1)
{
	Type_uHWord auhTempBuff[HAL_SNR_FAN_TEMP_READ_COUNT - 1];
	int32 alRet;
	Type_uWord auhAdcValue;
	Type_uWord auwFanTempTotal;
	Type_uHWord auhFanTempArg;
	Type_uByte i;

	alRet = HAL_OK;
	auhAdcValue = HAL_SNR_VALUE_0;
	auhFanTempArg = HAL_SNR_VALUE_0;
	auwFanTempTotal = HAL_SNR_VALUE_0;
	OSAL_MemSet(auhTempBuff, HAL_SNR_VALUE_0, sizeof(auhTempBuff));

	auhAdcValue = tcc_adc_read(HAL_SNR_ADC_TEMP_CH);
	/* save data */
	if (nstSNR_HiTempCheck.ubCnt < (HAL_SNR_FAN_TEMP_READ_COUNT - 1)) {
		nstSNR_HiTempCheck.uhDataBuff[nstSNR_HiTempCheck.ubCnt] = auhAdcValue;
		nstSNR_HiTempCheck.ubCnt++;
		alRet = HAL_ERR_DEVICE_BUSY;
	} else {
		nstSNR_HiTempCheck.uhDataBuff[HAL_SNR_FAN_TEMP_READ_COUNT - 1] = auhAdcValue;
		/* get average, check AD value */
		for (i = 0; i < HAL_SNR_FAN_TEMP_READ_COUNT; i++) {
			auwFanTempTotal += nstSNR_HiTempCheck.uhDataBuff[i];
		}
		auhFanTempArg = (Type_uHWord)(auwFanTempTotal / HAL_SNR_FAN_TEMP_READ_COUNT);
		wvdGen_MemCpy(auhTempBuff, &nstSNR_HiTempCheck.uhDataBuff[1], sizeof(auhTempBuff));
		wvdGen_MemCpy(nstSNR_HiTempCheck.uhDataBuff, auhTempBuff, sizeof(auhTempBuff));
		*(Type_uHWord *)PAR1 = auhFanTempArg;
		alRet = HAL_OK;
	}

	return alRet;
}
/********************************************************************************************/
/**	\function		SNR_CheckVsp
 *	\date			2021/10/25
 *	\author			Guanjiafu
 *	\description	check speed
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[out]		Vsp value
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			result
 *
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/10/25		Guanjiafu		New
 *                  0002    2022/09/29      Zhangruishan    Added timeout counter interrupts for #78099
 ********************************************************************************************/
static int32 SNR_CheckVsp(void* PAR1)
{
	Type_uHWord auhVspBuff[HAL_SNR_VSP_READ_COUNT - 1];
	int32 alRet;
	int32 alIctcRet;
	Type_uWord auwPeriod_ns;
	Type_uWord auwVspTotal;
	Type_uHWord auhVsp;
	Type_uHWord auhVspArg;
	Type_uByte i;
	Type_uByte aubflag;

	alRet = HAL_OK;
	OSAL_MemSet(auhVspBuff, HAL_SNR_VALUE_0, sizeof(auhVspBuff));
	alIctcRet = HAL_SNR_VALUE_0;
	auwPeriod_ns = HAL_SNR_VALUE_0;
	auwVspTotal = HAL_SNR_VALUE_0;
	auhVsp = HAL_SNR_VALUE_0;
	auhVspArg = HAL_SNR_VALUE_0;

	alIctcRet = ictc_get_period_pvl_count(HAL_SNR_VSP_ICTC_CH);
	/* 1,convert ictc ret to ns 	ICTC base CLK 12mHz */
	/* 2,convert ns to speed   (1.412ms/Period_ns) */
	auwPeriod_ns =	(1000000000 / (HAL_SNR_VSP_ICTC_BASE_CLK / HAL_SNR_VSP_ICTC_DIV_CLK)) * alIctcRet;
	auhVsp =  HAL_SNR_VSP_SPEED_PER_MK_NS / auwPeriod_ns;
	
	/* check data is valid or not */
	if (auhVsp > HAL_SNR_VSP_VALUE_MAX) {
		// LOG_OUTPUT(LOG_APP_ID_SNR, DEBUGLOG_LEVEL_ERROR, ATAT_LEVEL_OFF, "\n <%s:%d> auhVsp:%d", __FUNCTION__, __LINE__, auhVsp);
        auhVsp = HAL_SNR_VALUE_0;
	}

	/*No.0002 New additions start*/
	/*set timeout counter interrupt*/
	aubflag = ictc_get_TimeOutCount_Flag();
	if(aubflag == HAL_SNR_TIMEOUT_FLAG)
	{
		ictc_set_TimeOutCount_Flag(HAL_SNR_VALUE_0);
		
		ictc_reset(HAL_SNR_VSP_ICTC_CH);

		ictc_set_interrupt(HAL_SNR_VSP_ICTC_CH, TO_CNT_FULL_IRQ);

		alIctcRet = HAL_SNR_VALUE_0;
		auwPeriod_ns = HAL_SNR_VALUE_0;
		auhVsp = HAL_SNR_VALUE_0;
	}
	/*No.0002 New additions end*/

	/* save data */
	if (nstSNR_VspCheck.ubCnt < (HAL_SNR_VSP_READ_COUNT - 1)) {
		nstSNR_VspCheck.uhDataBuff[nstSNR_VspCheck.ubCnt] = auhVsp;
		nstSNR_VspCheck.ubCnt++;
		alRet = HAL_ERR_DEVICE_BUSY;
	} else {
		nstSNR_VspCheck.uhDataBuff[HAL_SNR_VSP_READ_COUNT - 1] = auhVsp;
	
		/* get VSP average */
		for (i = 0; i < HAL_SNR_VSP_READ_COUNT; i++) {
			auwVspTotal += nstSNR_VspCheck.uhDataBuff[i];
		}
		auhVspArg = (Type_uHWord)(auwVspTotal / HAL_SNR_VSP_READ_COUNT);
		wvdGen_MemCpy(auhVspBuff, &nstSNR_VspCheck.uhDataBuff[1], sizeof(auhVspBuff));
		wvdGen_MemCpy(nstSNR_VspCheck.uhDataBuff, auhVspBuff, sizeof(auhVspBuff));
		*(Type_uHWord *)PAR1 = auhVspArg;
		alRet = HAL_OK;
	}

	return alRet;
}
/********************************************************************************************/
/**	\function		SNR_CheckGpio
 *	\date			2021/10/25
 *	\author			Guanjiafu
 *	\description	check GPIO data
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		gpio index
 *	\param[out]		gpio value
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			result
 *
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/10/25		Guanjiafu		New
 ********************************************************************************************/
static int32 SNR_CheckGpio(Type_uByte index, void* PAR1)
{
	int32 alRet;
	Type_sWord aswValue;
	Type_uByte aubRet;

	alRet = HAL_OK;
	aubRet = STD_FALSE;	
	aswValue = STD_FALSE;	

	aswValue = gpio_get(nstGpioCheck_Tbl[index].port);
	if (nstSNR_GPIOCheck[index].ubStatus != aswValue) {
		nstSNR_GPIOCheck[index].ubCnt = HAL_SNR_VALUE_1;
		nstSNR_GPIOCheck[index].ubStatus = aswValue;
		aubRet = STD_FALSE; 
	} else {
		if (nstSNR_GPIOCheck[index].ubCnt < (nstGpioCheck_Tbl[index].chk_cnt - 1)) {
			nstSNR_GPIOCheck[index].ubCnt++;
		} else if (nstSNR_GPIOCheck[index].ubCnt == (nstGpioCheck_Tbl[index].chk_cnt - 1)) {
			aubRet = STD_TRUE;
		} else {
			aubRet = STD_FALSE; 
		}
	}

	if (aubRet == STD_TRUE) {
		*((Type_uByte *)PAR1) = aswValue;
		alRet = HAL_OK;
	} else {
		alRet = HAL_ERR_DEVICE_BUSY;
	}
	return alRet;
}

