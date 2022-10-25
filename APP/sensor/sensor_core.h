/*******************************************************************************************/
/**<!-------Copyright (C) 2021 Alpine Electronics, Inc. All rights reserved. --------------->
 *
 * \file					sensor_core.h
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
 *
 ********************************************************************************************/
#ifndef SNR_CORE_H
#define SNR_CORE_H
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


/* COMMON */
#define SNR_BYTE_INVALID	(0xffU)
#define SNR_HWORD_INVALID	(0xffffU)
#define SNR_VALUE_0		(0U)
#define SNR_VALUE_2		(2U)
#define SNR_VALUE_3		(3U)
#define SNR_VALUE_5		(5U)
#define SNR_VALUE_6		(6U)

#define SNR_CTRL_LOAD	(0x01U)
#define SNR_CTRL_UNLOAD	(0x02U)
/* CYC time */
#define SNR_CYC_TIME 			(2U)
#define SNR_CHKCNT_CLOCK_TEMP	(1000U / SNR_CYC_TIME)
#define SNR_CHKCNT_FAN_TEMP		(1000U / SNR_CYC_TIME)
#define SNR_CHKCNT_PARK			(20U / SNR_CYC_TIME)
#define SNR_CHKCNT_VSP			(20U / SNR_CYC_TIME)
#define SNR_CHKCNT_ILLM			(8U / SNR_CYC_TIME)
#define SNR_CHKCNT_TELMUTE		(6U / SNR_CYC_TIME)

/* FAN TEMP ADC */
#define SNR_FAN_TEMP_HIGH		(0x0719U)
#define SNR_FAN_TEMP_LOW		(0x075FU)

/********************************************************************************************/
/*							Type Definition Section											*/
/********************************************************************************************/
/********************************************************************************************/
/*							Enumeration Type Definition Section								*/
/********************************************************************************************/
enum {
	SNR_STATUS_UNLOAD = (0U),
	SNR_STATUS_LOAD	
};
enum {
	SNR_STATUS_OFF = (0x00),
	SNR_STATUS_ON,
	SNR_STATUS_INIT = (0xff)
};
enum {
	SNR_STATUS_TEMP_NORMAL = (1U),
	SNR_STATUS_TEMP_HI,
	SNR_STATUS_TEMP_KEEP
};
/********************************************************************************************/
/*							Structure/Union Type Definition Section							*/
/********************************************************************************************/

/********************************************************************************************/
/*							Extern Declaration												*/
/********************************************************************************************/
extern Type_uByte nubSnr_State;
extern HAL_HANDLE nvbSnr_HalHandle;
extern Type_uHWord nuhSnr_VspValue;
extern Type_uHWord nuhSnr_FanTempValue;
extern Type_uByte nubSnr_IllmStatus;
extern Type_uByte nubSnr_TempStatus;
extern Type_uByte nubSnr_ParkStatus;
extern Type_uByte nubSnr_FanTempMode;
extern Type_uByte nubSnr_TelMuteStatus;
/********************************************************************************************/
/*							Global Function Prototype Declaration							*/
/********************************************************************************************/
extern void wvdSnr_VariableInit(void);

#endif	/* SNR_CORE_H  */
