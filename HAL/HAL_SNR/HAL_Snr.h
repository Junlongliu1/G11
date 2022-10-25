/*******************************************************************************************/
/**<!-------Copyright (C) 2021 Alpine Electronics, Inc. All rights reserved. --------------->
 *
 * \file					HAL_Snr.h
 * \date					2021/10/25
 * \author					Guanjiafu
 * \model					EDA
 * \description				External declarations
 * \version					0.55
 *
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/10/25		Guanjiafu		New
 ********************************************************************************************/
#ifndef __HAL_SNR_H
#define __HAL_SNR_H
/********************************************************************************************/
/*							Include File Section											*/
/********************************************************************************************/
#include "apn_basic_type.h"
#include "mt_adc.h"
#include "temp_compile_switch.h"
#include "pm_pin_define.h"
/********************************************************************************************/
/*							Macro Definition Section										*/
/********************************************************************************************/
//#define SNR_DEBUG
#ifdef SNR_DEBUG
#include "debug.h"
#define SNR_DBG(fmt, args...)   printf(fmt, ## args)
#else
#define SNR_DBG(fmt, args...)   do {} while(0)
#endif
#define HAL_SNR_NULL			(0U)
#define HAL_SNR_VALUE_0			(0U)
#define HAL_SNR_VALUE_1			(1U)
#define HAL_SNR_VALUE_2			(2U)
#define HAL_SNR_VALUE_3			(3U)
#define HAL_SNR_VALUE_5			(5U)
#define HAL_SNR_VALUE_6			(6U)
#define HAL_SNR_VALUE_10		(10U)
#define HAL_SNR_BYTE_INVALID	(0xffU)

/* Temp Port info */
#define HAL_SNR_ADC_TEMP_CH		(ADC_CHANNEL0)
#define HAL_SNR_FAN_PORT		(PM_FAN_CONT)

/* VSP Port info */
#define HAL_SNR_VSP_ICTC_PORT			(PM_VSP)
#define HAL_SNR_VSP_ICTC_CH				(0U)
#define HAL_SNR_VSP_ICTC_BASE_CLK		(12000000U) 	//12Mhz
#define HAL_SNR_VSP_ICTC_DIV_CLK_SEL	(2U) 			//divided clock:8
#define HAL_SNR_VSP_ICTC_DIV_CLK		(8U)
#define HAL_SNR_VSP_SPEED_PER_MK_NS		(1412000000U) 	//1.412s=0.708Hz
#define HAL_SNR_VSP_VALUE_MAX			(500U)			//real speed can not up to 500	

#define HAL_SNR_ILL_PORT		(PM_ILLM_ON)
#define HAL_SNR_TELMUTE_PORT	(PM_TEL_MUTE)

#define HAL_SNR_TIMEOUT_FLAG    (1U)

#if (TEMP_COMPILE_SWITCH_05WORK_10WORK == TEMP_COMPILE_SWITCH_05WORK)
#define HAL_SNR_PARK_PORT		(0xffU) //in A53
#else
#define HAL_SNR_PARK_PORT		(PM_VTR_PARK)
#endif
/* Read Count */
#define HAL_SNR_CLOCK_TEMP_READ_COUNT		(3600U)
#define HAL_SNR_FAN_TEMP_READ_COUNT			(10U)
#define HAL_SNR_VSP_READ_COUNT				(5U)

/********************************************************************************************/
/*							Type Definition Section											*/
/********************************************************************************************/
typedef struct {
	Type_uWord uwStatus;
	Type_uWord uwOptions;
} Hal_Snr_Type_ts;

typedef struct{
	uint32 port;             /* GPIO port */
	Type_uByte chk_cnt;     /* check count */
} Hal_Snr_GPIOCheckDataTbl_ts;

typedef struct{
	 Type_uHWord uhDataBuff[HAL_SNR_FAN_TEMP_READ_COUNT];
	 Type_uByte ubCnt;
} Hal_Snr_HiTempCheckData_ts;

typedef struct{
	Type_uByte ubStatus;
	Type_uByte ubCnt;
} Hal_Snr_GPIOCheckData_ts;

typedef struct{
	Type_uHWord uhDataBuff[HAL_SNR_VSP_READ_COUNT];
	Type_uByte ubCnt;
} Hal_Snr_VspCheckData_ts;

typedef struct{
	Type_uWord uwTotalADValue;
	Type_uHWord uhCnt;
} Hal_Snr_TempData_ts;

/********************************************************************************************/
/*							Enumeration Type Definition Section								*/
/********************************************************************************************/
enum {	
	HAL_SNR_OPTION_NORMAL = (1U)
};

enum {
	HAL_SNR_IOCTL_CHECK_CLOCKTEMP = (1U),
	HAL_SNR_IOCTL_CHECK_HiTEMP,
	HAL_SNR_IOCTL_CHECK_PARK,
	HAL_SNR_IOCTL_CHECK_VSP,
	HAL_SNR_IOCTL_CHECK_ILLM,
	HAL_SNR_IOCTL_SET_FAN,
	HAL_SNR_IOCTL_CHECK_TELMUTE
};

enum {
	HAL_SNR_GPIO_INDEX_ILL = 0,
	HAL_SNR_GPIO_INDEX_PARK,
	HAL_SNR_GPIO_INDEX_TELMUTE
};
	
/********************************************************************************************/
/*							Extern Declaration												*/
/********************************************************************************************/
extern int32 SNR_Open(uint32 OPTIONS);
extern int32 SNR_Close(void);
extern int32 SNR_Ioctl(uint32 OPT, void* PAR1, void* PAR2, void* PAR3, void* PAR4);
#endif /* __HAL_SNR_H__ */
