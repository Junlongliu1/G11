/*!
* @file       rpc_event.h
* @date       2021/09/09
* @author     Zhenwang.zhang
* @version    0.1
* @brief      EventID define file.
* @par Copyright (c) 2021 Alpine Electronics, Inc. All Rights Reserved.
*/
/*********************************************************************************
 * Revision History                                                              *
 * No    Date        Revised by        Description                               *
 * --    ----        ----------        -----------                               *
 * 01    2021/09/09  Zhenwang.zhang      New                                     *
 *********************************************************************************/

#ifndef __RPC_EVENT_H__
#define __RPC_EVENT_H__

#include "rpc_module.h"

//LED
enum {
	HMI_LED_VALUE_GET_REQ = ((ARPC_ID_LED<<8) + 0x00),
	HMI_LED_VALUE_SET,
	HMI_LED_PWM_SET,
	LINETOOL_LED_SET_REQ,
	HMI_LED_VALUE_NOTICE,
	LINETOOL_LED_SET_RES
};

//KEY
enum {
	EVENT_CODE_HARDKEY = ((ARPC_ID_R5_KEY<<8) + 0x00),
	EVENT_CODE_STEERINGKEY,
	EVENT_CODE_LINETOOL_KEY_MODE_ON,
	EVENT_CODE_LINETOOL_KEY_MODE_OFF,
	EVENT_CODE_LINETOOL_KEY_MODE_CHECK,
	EVENT_CODE_LINETOOL_KEY_MODE_ON_RES,
	EVENT_CODE_LINETOOL_KEY_MODE_OFF_RES,
	EVENT_CODE_LINETOOL_KEY_MODE_CHECK_RES
};

//POWER Mng R5
enum {
	EV_INF_HMI_POWERON_REQ = ((ARPC_ID_R5_POWERMNG << 8)  + 0x00),
	EV_INF_HMI_POWERON_RES,
	EV_INF_HMI_POWEROFF_REQ,
	EV_INF_HMI_POWEROFF_RES,
	EV_INF_UPDATE_UPDATE_RESET_REQ,
	EV_INF_UPDATE_UPDATE_RESET_RES,
	EV_INF_HMI_FACTORY_RESET_REQ,
	EV_INF_HMI_FACTORY_RESET_RES,
	EV_INF_STANDBYOUT_NOTIFY,
	EV_INF_STANDBYOUT_FINISH,
	EV_INF_STANDBYIN_NOTIFY,
	EV_INF_STANDBYIN_FINISH,
	EV_INF_POWERON_NOTIFY,
	EV_INF_POWERON_FINISH,
	EV_INF_POWEROFF_NOTIFY,
	EV_INF_POWEROFF_FINISH,
	EV_INF_PRERESET_WDT_NOTIFY,			//not used
	EV_INF_PRERESET_WDT_FINISH,			//not used
	EV_INF_A53_TO_R5_START,
	EV_INF_UPDATE_RESET_NOTIFY,
	EV_INF_UPDATE_RESET_FINISH,
	EV_INF_FACTORY_RESET_NOTIFY,
	EV_INF_FACTORY_RESET_FINISH,
	EV_INF_HMI_UPDATE_RESET_REQ,
	EV_INF_A53_TO_R5_START_RES,
};

//POWER Drv R5
enum {
	EV_INF_GPIO_LEVEL_SET_REQ = ((ARPC_ID_R5_POWERDRV << 8)  + 0x00),
	EV_INF_GPIO_LEVEL_SET_RES,
};

//POWER Wdt R5
enum {
	EV_INF_R5_TO_A53_HEARTBEAT_CMD = ((ARPC_ID_R5_SOFTWDT << 8)  + 0x00),
	EV_INF_R5_TO_A53_HEARTBEAT_RES,
	EV_INF_R5_TO_A7_HEARTBEAT_CMD,
	EV_INF_R5_TO_A7_HEARTBEAT_RES,
	EV_INF_A53_TO_R5_REBOOT_REQ,
	EV_INF_A7_TO_R5_REBOOT_REQ,
};


//power A53
enum{	
    EV_INF_A53_TO_R5_STBYOUT_OK = ((ARPC_ID_A53_POWER << 8)  + 0x00),			
    EV_INF_A53_TO_R5_STBYIN_OK,		
    EV_INF_A53_TO_R5_POWERON_OK,		
    EV_INF_A53_TO_R5_POWEROFF_OK,	
    EV_INF_A53_TO_R5_HEARTBEAT_RES,
	
	EV_INF_A53_TO_PREREBOOT_CMD,			
	EV_INF_A53_TO_MOUDLE_STBYOUT_CMD,	
	EV_INF_A53_TO_MOUDLE_STBYIN_CMD,	
	EV_INF_A53_TO_MOUDLE_POWERON_CMD,	
	EV_INF_A53_TO_MOUDLE_POWEROFF_CMD,	
	EV_INF_A53_TO_MOUDLE_HEARTBEAT_CMD,	
	EV_INF_A53_TO_MOUDLE_HEARTBEAT_RES,
	EV_INF_A53_TO_MOUDLE_STBYOUT_RES,	
	EV_INF_A53_TO_MOUDLE_STBYIN_RES,		
	EV_INF_A53_TO_MOUDLE_POWERON_RES,	
	EV_INF_A53_TO_MOUDLE_POWEROFF_RES,
	EV_INF_A53_TO_PREREBOOT_RES,
	
	EV_INF_A53_TO_MOUDLE_UPDATE_RESET_CMD,
	EV_INF_A53_TO_MOUDLE_UPDATE_RESET_RES,
	EV_INF_A53_TO_MOUDLE_FACTORY_RESET_CMD,
	EV_INF_A53_TO_MOUDLE_FACTORY_RESET_RES,
	
};

//power A7
enum{
/* 00 */	EV_INF_A7_CMD_MIN					=	((ARPC_ID_A7_POWER << 8)  + 0x00)		,
/* 01 */	EV_INF_A7_TO_A53_STBYOUT_OK			=	EV_INF_A53_TO_MOUDLE_STBYOUT_RES		,
/* 02 */	EV_INF_A7_TO_A53_STBYIN_OK			=	EV_INF_A53_TO_MOUDLE_STBYIN_RES			,
/* 03 */	EV_INF_A7_TO_A53_POWERON_OK			=	EV_INF_A53_TO_MOUDLE_POWERON_RES		,
/* 04 */	EV_INF_A7_TO_A53_POWEROFF_OK		=	EV_INF_A53_TO_MOUDLE_POWEROFF_RES		,
/* 05 */	EV_INF_A7_TO_A53_HEARTBEAT_RES		=	EV_INF_A53_TO_MOUDLE_HEARTBEAT_RES		,
/* 06 */	EV_INF_A7_TO_A53_UPDATE_RESET_OK	=	EV_INF_A53_TO_MOUDLE_UPDATE_RESET_RES	,
/* 07 */	EV_INF_A7_TO_A53_FACTORY_RESET_OK	=	EV_INF_A53_TO_MOUDLE_FACTORY_RESET_RES	,
/* 08 */	EV_INF_A7_TO_A53_PREREBOOT_OK		=	EV_INF_A53_TO_PREREBOOT_RES				,

/* 09 */	EV_INF_A7_TO_MOUDLE_STBYOUT_CMD		=	(EV_INF_A7_CMD_MIN + 0x09)				,
/* 0A */	EV_INF_A7_TO_MOUDLE_STBYIN_CMD		,
/* 0B */	EV_INF_A7_TO_MOUDLE_POWERON_CMD		,
/* 0C */	EV_INF_A7_TO_MOUDLE_POWEROFF_CMD	,
/* 0D */	EV_INF_A7_TO_MOUDLE_HEARTBEAT_CMD	,
/* 0E */	EV_INF_A7_TO_MOUDLE_STBYOUT_RES		,
/* 0F */	EV_INF_A7_TO_MOUDLE_STBYIN_RES		,
/* 10 */	EV_INF_A7_TO_MOUDLE_POWERON_RES		,
/* 11 */	EV_INF_A7_TO_MOUDLE_POWEROFF_RES	,
/* 12 */	EV_INF_A7_TO_MOUDLE_HEARTBEAT_RES	,


/* 13 */	EV_INF_A7_TO_MOUDLE_UPDATE_RESET_CMD	,
/* 14 */	EV_INF_A7_TO_MOUDLE_FACTORY_RESET_CMD	,
/* 15 */	EV_INF_A7_TO_MOUDLE_UPDATE_RESET_RES	,
/* 16 */	EV_INF_A7_TO_MOUDLE_FACTORY_RESET_RES	,

/* 17 */	EV_INF_A7_TO_PREREBOOT_CMD			,
/* 18 */	EV_INF_A7_TO_PREREBOOT_RES			,

/* 19 */	EV_INF_A7_CMD_MAX
};

//SENSOR
enum {
	SNR_EV_INF_DCDC_FREQSET_REQ = ((ARPC_ID_SENSOR<<8) + 0x00),
	SNR_EV_INF_FANSET_REQ,
	SNR_EV_INF_HMI_READY_REQ,
	SNR_EV_INF_ILLUMI_GET_REQ,
	SNR_EV_INF_PARK_CHG,
	SNR_EV_INF_ILLUMI_CHG,
	SNR_EV_INF_VSP_CHG,
	SNR_EV_INF_TEMP_1H,
	SNR_EV_INF_FANSET_RES,
	SNR_EV_INF_ILLUMI_GET_RES,
	SNR_EV_INF_TELMUTE_CHG
};


/* LOG EVENT ID*/
enum {
	EVT_LOG_MIN = ((ARPC_ID_R5_LOGAPP<<8) + 0x00),		/* do not delete */

	/* A53 TO R5 */
	EVT_LOG_A53TOR5_ATAT_ON,
	EVT_LOG_A53TOR5_ATAT_OFF,
	EVT_LOG_A53TOR5_POWER_ON,
	EVT_LOG_A53TOR5_POWER_OFF,
	
	/* R5 TO A53 */
	EVT_LOG_R5_OUTPUT_LOG_INFO,
	
	EVT_LOG_MAX						/* do not delete */
};
enum {
	/* A53 TO A7 */
	EVT_LOG_A53TOA7_CLEAR_REQ = ((ARPC_ID_A7_LOGAPP<<8) + 0x00),
	EVT_LOG_A53TOA7_SAVE_REQ,
	EVT_LOG_A53TOA7_ATAT_ON,
	EVT_LOG_A53TOA7_ATAT_OFF,
	EVT_LOG_A53TOA7_POWER_ON,

	/* A7 TO A53 */
	EVT_LOG_A7_CLEAR_RES_OK,
	EVT_LOG_A7_CLEAR_RES_NG,
	EVT_LOG_A7_LOG_SAVE_TO_EMMC_OK,
	EVT_LOG_A7_LOG_SAVE_TO_EMMC_NG,
	EVT_LOG_A7_LOG_SAVESM_INFO,
	EVT_LOG_A7_LOG_SAVE_RES,

	/* TO A7 */
	EVT_LOG_TOA7_ATAT_OUTPUT
};
enum {
	/* TO A53 */
	EVT_LOG_TOA53_ATAT_OUTPUT = ((ARPC_ID_A53_LOGAPP<<8) + 0x00)
};

//usbtest
enum
{
	EVT_USBTEST_HOST_COMMODE = ((ARPC_ID_USB_TEST<<8) + 0x00),
	EVT_USBTEST_DEVICE_COMMODE,
	EVT_USBTEST_TEST_J_SUBMODE,
	EVT_USBTEST_TEST_K_SUBMODE,
	EVT_USBTEST_TEST_SE0_NAK_SUBMODE,
	EVT_USBTEST_TEST_PACKET_SUBMODE,
	EVT_USBTEST_TEST_FORCE_ENABLE,
	EVT_USBTEST_HIGHSPEEDMODE,
	EVT_USBTEST_RETURN,
	EVT_USBTEST_FULLSPEEDMODE,
};

/* DiagApp Start */

/* HMI to Diagapp */
#define DIAGAPP_INF_USB_TEST                ((ARPC_ID_DIAGAPP<<8) + 0x00)
#define DIAGAPP_INF_USB_TEST_RECOVER        ((ARPC_ID_DIAGAPP<<8) + 0x01)
#define DIAGAPP_INF_ALL_CLEAR               ((ARPC_ID_DIAGAPP<<8) + 0x02)
#define DIAGAPP_INF_UPDATE_CLEAR            ((ARPC_ID_DIAGAPP<<8) + 0x03)
#define DIAGAPP_INF_FACTORY_RESET           ((ARPC_ID_DIAGAPP<<8) + 0x04)
#define DIAGAPP_INF_SPEAK_CHK_CHANGE        ((ARPC_ID_DIAGAPP<<8) + 0x05)
#define DIAGAPP_INF_ECNR_TEST_START         ((ARPC_ID_DIAGAPP<<8) + 0x06)
#define DIAGAPP_INF_ECNR_TEST_STOP          ((ARPC_ID_DIAGAPP<<8) + 0x07)

/* DiagApp to HMI */
#define DIAGAPP_EV_USB_TEST                 ((ARPC_ID_DIAGAPP<<8) + 0x08)
#define DIAGAPP_EV_ALL_CLEAR                ((ARPC_ID_DIAGAPP<<8) + 0x09)
#define DIAGAPP_EV_UPDATE_CLEAR             ((ARPC_ID_DIAGAPP<<8) + 0x0A)
#define DIAGAPP_EV_FACTORY_RESET            ((ARPC_ID_DIAGAPP<<8) + 0x0B)
#define DIAGAPP_EV_SPEAKER_CHECK            ((ARPC_ID_DIAGAPP<<8) + 0x0C)

/* DiagApp to Other */
#define DIAGAPP_EV_ALL_CLEAR_NOTIFY         ((ARPC_ID_DIAGAPP<<8) + 0x0D)
#define DIAGAPP_EV_FACTORY_RESET_NOTIFY     ((ARPC_ID_DIAGAPP<<8) + 0x0E)
#define DIAGAPP_EV_UPDATE_CLEAR_NOTIFY      ((ARPC_ID_DIAGAPP<<8) + 0x0F)

/* Other to DiagApp */
#define DIAGAPP_EV_ALL_CLEAR_CB             ((ARPC_ID_DIAGAPP<<8) + 0x10)
#define DIAGAPP_EV_FACTORY_RESET_CB         ((ARPC_ID_DIAGAPP<<8) + 0x11)
#define DIAGAPP_EV_UPDATE_CLEAR_CB          ((ARPC_ID_DIAGAPP<<8) + 0x12)
/* DiagApp end */

/* LineTool Start */
#define EV_REQ_LINETOOL_POWER_OFF           ((ARPC_ID_LINETOOLAPP<<8) + 0x00)
#define EV_REQ_LINETOOL_POWER_ON            ((ARPC_ID_LINETOOLAPP<<8) + 0x01)
#define EV_REQ_LINETOOL_WARNING_IN          ((ARPC_ID_LINETOOLAPP<<8) + 0x02)
#define EV_REQ_LINETOOL_LANGUAGE_IN         ((ARPC_ID_LINETOOLAPP<<8) + 0x03)
#define EV_REQ_LINETOOL_SOURCE_DIRECT       ((ARPC_ID_LINETOOLAPP<<8) + 0x04)
#define EV_REQ_LINETOOL_VOLUME_DIRECT       ((ARPC_ID_LINETOOLAPP<<8) + 0x05)
#define EV_REQ_LINETOOL_GET_GENERAL_VER     ((ARPC_ID_LINETOOLAPP<<8) + 0x06)
#define EV_REQ_LINETOOL_GET_BT_VER          ((ARPC_ID_LINETOOLAPP<<8) + 0x07)
#define EV_REQ_LINETOOL_MUTE_CONTROL        ((ARPC_ID_LINETOOLAPP<<8) + 0x08)
#define EV_REQ_LINETOOL_LOUDNESS            ((ARPC_ID_LINETOOLAPP<<8) + 0x09)
#define EV_REQ_LINETOOL_EQ                  ((ARPC_ID_LINETOOLAPP<<8) + 0x0A)
#define EV_REQ_LINETOOL_FM_FREQ_DIRECT      ((ARPC_ID_LINETOOLAPP<<8) + 0x0B)
#define EV_REQ_LINETOOL_AM_FREQ_DIRECT      ((ARPC_ID_LINETOOLAPP<<8) + 0x0C)
#define EV_REQ_LINETOOL_SEEK_UP             ((ARPC_ID_LINETOOLAPP<<8) + 0x0D)
#define EV_REQ_LINETOOL_SEEK_DOWN           ((ARPC_ID_LINETOOLAPP<<8) + 0x0E)
#define EV_REQ_LINETOOL_FM_ALIGNMENT        ((ARPC_ID_LINETOOLAPP<<8) + 0x0F)
#define EV_REQ_LINETOOL_AM_ALIGNMENT        ((ARPC_ID_LINETOOLAPP<<8) + 0x10)
#define EV_REQ_LINETOOL_GET_RDS_INFO        ((ARPC_ID_LINETOOLAPP<<8) + 0x11)
#define EV_REQ_LINETOOL_GET_RDS_LEVEL       ((ARPC_ID_LINETOOLAPP<<8) + 0x12)
#define EV_REQ_LINETOOL_GET_TUNER_SM        ((ARPC_ID_LINETOOLAPP<<8) + 0x13)
#define EV_REQ_LINETOOL_IPOD_TRACK_CHG      ((ARPC_ID_LINETOOLAPP<<8) + 0x14)
#define EV_REQ_LINETOOL_USB_TRACK_CHG       ((ARPC_ID_LINETOOLAPP<<8) + 0x15)
#define EV_REQ_LINETOOL_USB_AUDIO_CHG       ((ARPC_ID_LINETOOLAPP<<8) + 0x16)
#define EV_REQ_LINETOOL_USB_VEDIO_CHG       ((ARPC_ID_LINETOOLAPP<<8) + 0x17)
#define EV_REQ_LINETOOL_BT_CHECK            ((ARPC_ID_LINETOOLAPP<<8) + 0x18)
#define EV_REQ_LINETOOL_BT_AUDIO_1KZ_SET    ((ARPC_ID_LINETOOLAPP<<8) + 0x19)
#define EV_REQ_LINETOOL_HCIENABLE_CHG       ((ARPC_ID_LINETOOLAPP<<8) + 0x1A)
#define EV_REQ_LINETOOL_BT_LEVEL_DIAG       ((ARPC_ID_LINETOOLAPP<<8) + 0x1B)
#define EV_REQ_LINETOOL_BT_INFO_CLEAR       ((ARPC_ID_LINETOOLAPP<<8) + 0x1C)
#define EV_REQ_LINETOOL_CAMERA_ENTER        ((ARPC_ID_LINETOOLAPP<<8) + 0x1D)
#define EV_REQ_LINETOOL_CAMERA_CHG          ((ARPC_ID_LINETOOLAPP<<8) + 0x1E)
#define EV_REQ_LINETOOL_CAMERA_QUIT         ((ARPC_ID_LINETOOLAPP<<8) + 0x1F)
#define EV_REQ_LINETOOL_CAMERA_CHECK        ((ARPC_ID_LINETOOLAPP<<8) + 0x20)
#define EV_REQ_LINETOOL_MEMORY_INIT         ((ARPC_ID_LINETOOLAPP<<8) + 0x21)
#define EV_REQ_LINETOOL_DISPLAY_ADJUST      ((ARPC_ID_LINETOOLAPP<<8) + 0x22)
#define EV_REQ_LINETOOL_FLC_SET             ((ARPC_ID_LINETOOLAPP<<8) + 0x23)
#define EV_REQ_LINETOOL_FLC_MODE_ON         ((ARPC_ID_LINETOOLAPP<<8) + 0x24)
#define EV_REQ_LINETOOL_FLC_MODE_OFF        ((ARPC_ID_LINETOOLAPP<<8) + 0x25)
#define EV_REQ_LINETOOL_GET_CURRENT_TIME    ((ARPC_ID_LINETOOLAPP<<8) + 0x26)
#define EV_REQ_LINETOOL_SET_CLOCK_TERMINAL  ((ARPC_ID_LINETOOLAPP<<8) + 0x27)
#define EV_REQ_LINETOOL_TP_MODE_ON          ((ARPC_ID_LINETOOLAPP<<8) + 0x28)
#define EV_REQ_LINETOOL_TP_MODE_OFF         ((ARPC_ID_LINETOOLAPP<<8) + 0x29)
#define EV_REQ_LINETOOL_TP_CHECK            ((ARPC_ID_LINETOOLAPP<<8) + 0x2A)
#define EV_REQ_LINETOOL_KEY_CHECK_ONOFF     ((ARPC_ID_LINETOOLAPP<<8) + 0x2B)
#define EV_REQ_LINETOOL_TEST_SOURCE_ON      ((ARPC_ID_LINETOOLAPP<<8) + 0x2C)
#define EV_REQ_LINETOOL_TEST_SOURCE_OFF     ((ARPC_ID_LINETOOLAPP<<8) + 0x2D)
#define EV_REQ_LINETOOL_MIC_PATH_BUILD      ((ARPC_ID_LINETOOLAPP<<8) + 0x2E)
#define EV_REQ_LINETOOL_MIC_PATH_CUT        ((ARPC_ID_LINETOOLAPP<<8) + 0x2F)
#define EV_REQ_LINETOOL_SET_ENGLISH         ((ARPC_ID_LINETOOLAPP<<8) + 0x30)
#define EV_REQ_LINETOOL_CPL_IPOD_SEL        ((ARPC_ID_LINETOOLAPP<<8) + 0x31)
#define EV_REQ_LINETOOL_CPL_TRACK_CHG       ((ARPC_ID_LINETOOLAPP<<8) + 0x32)
#define EV_REQ_LINETOOL_CPL_ON_OFF          ((ARPC_ID_LINETOOLAPP<<8) + 0x33)
#define EV_REQ_LINETOOL_AAP_ON_OFF          ((ARPC_ID_LINETOOLAPP<<8) + 0x34)
#define EV_REQ_LINETOOL_WIFI_SET_SSID       ((ARPC_ID_LINETOOLAPP<<8) + 0x35)
#define EV_REQ_LINETOOL_WIFI_SET_PASSWORD   ((ARPC_ID_LINETOOLAPP<<8) + 0x36)
#define EV_REQ_LINETOOL_WIFI_CLEAR_INFO     ((ARPC_ID_LINETOOLAPP<<8) + 0x37)
#define EV_REQ_LINETOOL_WIFI_GETCONNUM      ((ARPC_ID_LINETOOLAPP<<8) + 0x38)
#define EV_REQ_LINETOOL_WIFI_RESET          ((ARPC_ID_LINETOOLAPP<<8) + 0x39)
#define EV_REQ_LINETOOL_WIFI_CON_CHECK      ((ARPC_ID_LINETOOLAPP<<8) + 0x3A)
#define EV_REQ_LINETOOL_WIFI_MODE_ON_OFF    ((ARPC_ID_LINETOOLAPP<<8) + 0x3B)
#define EV_REQ_LINETOOL_WIFI_GET_MAC        ((ARPC_ID_LINETOOLAPP<<8) + 0x3C)
#define EV_REQ_LINETOOL_BT_GET_MAC          ((ARPC_ID_LINETOOLAPP<<8) + 0x3D)
#define EV_REQ_LINETOOL_GNSS_GET_GYRO       ((ARPC_ID_LINETOOLAPP<<8) + 0x3E)
#define EV_REQ_LINETOOL_GNSS_GET_NAVI       ((ARPC_ID_LINETOOLAPP<<8) + 0x3F)
#define EV_RES_LINETOOL_POWER_OFF           ((ARPC_ID_LINETOOLAPP<<8) + 0x40)
#define EV_RES_LINETOOL_POWER_ON            ((ARPC_ID_LINETOOLAPP<<8) + 0x41)
#define EV_RES_LINETOOL_WARNING_IN          ((ARPC_ID_LINETOOLAPP<<8) + 0x42)
#define EV_RES_LINETOOL_LANGUAGE_IN         ((ARPC_ID_LINETOOLAPP<<8) + 0x43)
#define EV_RES_LINETOOL_SOURCE_DIRECT       ((ARPC_ID_LINETOOLAPP<<8) + 0x44)
#define EV_RES_LINETOOL_VOLUME_DIRECT       ((ARPC_ID_LINETOOLAPP<<8) + 0x45)
#define EV_RES_LINETOOL_GET_GENERAL_VER     ((ARPC_ID_LINETOOLAPP<<8) + 0x46)
#define EV_RES_LINETOOL_GET_BT_VER          ((ARPC_ID_LINETOOLAPP<<8) + 0x47)
#define EV_RES_LINETOOL_MUTE_CONTROL        ((ARPC_ID_LINETOOLAPP<<8) + 0x48)
#define EV_RES_LINETOOL_LOUDNESS            ((ARPC_ID_LINETOOLAPP<<8) + 0x49)
#define EV_RES_LINETOOL_EQ                  ((ARPC_ID_LINETOOLAPP<<8) + 0x4A)
#define EV_RES_LINETOOL_FM_FREQ_DIRECT      ((ARPC_ID_LINETOOLAPP<<8) + 0x4B)
#define EV_RES_LINETOOL_AM_FREQ_DIRECT      ((ARPC_ID_LINETOOLAPP<<8) + 0x4C)
#define EV_RES_LINETOOL_SEEK_UP             ((ARPC_ID_LINETOOLAPP<<8) + 0x4D)
#define EV_RES_LINETOOL_SEEK_DOWN           ((ARPC_ID_LINETOOLAPP<<8) + 0x4E)
#define EV_RES_LINETOOL_FM_ALIGNMENT        ((ARPC_ID_LINETOOLAPP<<8) + 0x4F)
#define EV_RES_LINETOOL_AM_ALIGNMENT        ((ARPC_ID_LINETOOLAPP<<8) + 0x50)
#define EV_RES_LINETOOL_GET_RDS_INFO        ((ARPC_ID_LINETOOLAPP<<8) + 0x51)
#define EV_RES_LINETOOL_GET_RDS_LEVEL       ((ARPC_ID_LINETOOLAPP<<8) + 0x52)
#define EV_RES_LINETOOL_GET_TUNER_SM        ((ARPC_ID_LINETOOLAPP<<8) + 0x53)
#define EV_RES_LINETOOL_IPOD_TRACK_CHG      ((ARPC_ID_LINETOOLAPP<<8) + 0x54)
#define EV_RES_LINETOOL_USB_TRACK_CHG       ((ARPC_ID_LINETOOLAPP<<8) + 0x55)
#define EV_RES_LINETOOL_USB_AUDIO_CHG       ((ARPC_ID_LINETOOLAPP<<8) + 0x56)
#define EV_RES_LINETOOL_USB_VEDIO_CHG       ((ARPC_ID_LINETOOLAPP<<8) + 0x57)
#define EV_RES_LINETOOL_BT_CHECK            ((ARPC_ID_LINETOOLAPP<<8) + 0x58)
#define EV_RES_LINETOOL_BT_AUDIO_1KZ_SET    ((ARPC_ID_LINETOOLAPP<<8) + 0x59)
#define EV_RES_LINETOOL_HCIENABLE_CHG       ((ARPC_ID_LINETOOLAPP<<8) + 0x5A)
#define EV_RES_LINETOOL_BT_LEVEL_DIAG       ((ARPC_ID_LINETOOLAPP<<8) + 0x5B)
#define EV_RES_LINETOOL_BT_INFO_CLEAR       ((ARPC_ID_LINETOOLAPP<<8) + 0x5C)
#define EV_RES_LINETOOL_CAMERA_ENTER        ((ARPC_ID_LINETOOLAPP<<8) + 0x5D)
#define EV_RES_LINETOOL_CAMERA_CHG          ((ARPC_ID_LINETOOLAPP<<8) + 0x5E)
#define EV_RES_LINETOOL_CAMERA_QUIT         ((ARPC_ID_LINETOOLAPP<<8) + 0x5F)
#define EV_RES_LINETOOL_CAMERA_CHECK        ((ARPC_ID_LINETOOLAPP<<8) + 0x60)
#define EV_RES_LINETOOL_MEMORY_INIT         ((ARPC_ID_LINETOOLAPP<<8) + 0x61)
#define EV_RES_LINETOOL_DISPLAY_ADJUST      ((ARPC_ID_LINETOOLAPP<<8) + 0x62)
#define EV_RES_LINETOOL_FLC_SET             ((ARPC_ID_LINETOOLAPP<<8) + 0x63)
#define EV_RES_LINETOOL_FLC_MODE_ON         ((ARPC_ID_LINETOOLAPP<<8) + 0x64)
#define EV_RES_LINETOOL_FLC_MODE_OFF        ((ARPC_ID_LINETOOLAPP<<8) + 0x65)
#define EV_RES_LINETOOL_GET_CURRENT_TIME    ((ARPC_ID_LINETOOLAPP<<8) + 0x66)
#define EV_RES_LINETOOL_SET_CLOCK_TERMINAL  ((ARPC_ID_LINETOOLAPP<<8) + 0x67)
#define EV_RES_LINETOOL_TP_MODE_ON          ((ARPC_ID_LINETOOLAPP<<8) + 0x68)
#define EV_RES_LINETOOL_TP_MODE_OFF         ((ARPC_ID_LINETOOLAPP<<8) + 0x69)
#define EV_RES_LINETOOL_TP_CHECK            ((ARPC_ID_LINETOOLAPP<<8) + 0x6A)
#define EV_RES_LINETOOL_KEY_CHECK_ONOFF     ((ARPC_ID_LINETOOLAPP<<8) + 0x6B)
#define EV_RES_LINETOOL_TEST_SOURCE_ON      ((ARPC_ID_LINETOOLAPP<<8) + 0x6C)
#define EV_RES_LINETOOL_TEST_SOURCE_OFF     ((ARPC_ID_LINETOOLAPP<<8) + 0x6D)
#define EV_RES_LINETOOL_MIC_PATH_BUILD      ((ARPC_ID_LINETOOLAPP<<8) + 0x6E)
#define EV_RES_LINETOOL_MIC_PATH_CUT        ((ARPC_ID_LINETOOLAPP<<8) + 0x6F)
#define EV_RES_LINETOOL_SET_ENGLISH         ((ARPC_ID_LINETOOLAPP<<8) + 0x70)
#define EV_RES_LINETOOL_CPL_IPOD_SEL        ((ARPC_ID_LINETOOLAPP<<8) + 0x71)
#define EV_RES_LINETOOL_CPL_TRACK_CHG       ((ARPC_ID_LINETOOLAPP<<8) + 0x72)
#define EV_RES_LINETOOL_CPL_ON_OFF          ((ARPC_ID_LINETOOLAPP<<8) + 0x73)
#define EV_RES_LINETOOL_AAP_ON_OFF          ((ARPC_ID_LINETOOLAPP<<8) + 0x74)
#define EV_RES_LINETOOL_WIFI_SET_SSID       ((ARPC_ID_LINETOOLAPP<<8) + 0x75)
#define EV_RES_LINETOOL_WIFI_SET_PASSWORD   ((ARPC_ID_LINETOOLAPP<<8) + 0x76)
#define EV_RES_LINETOOL_WIFI_CLEAR_INFO     ((ARPC_ID_LINETOOLAPP<<8) + 0x77)
#define EV_RES_LINETOOL_WIFI_GETCONNUM      ((ARPC_ID_LINETOOLAPP<<8) + 0x78)
#define EV_RES_LINETOOL_WIFI_RESET          ((ARPC_ID_LINETOOLAPP<<8) + 0x79)
#define EV_RES_LINETOOL_WIFI_CON_CHECK      ((ARPC_ID_LINETOOLAPP<<8) + 0x7A)
#define EV_RES_LINETOOL_WIFI_MODE_ON_OFF    ((ARPC_ID_LINETOOLAPP<<8) + 0x7B)
#define EV_RES_LINETOOL_WIFI_GET_MAC        ((ARPC_ID_LINETOOLAPP<<8) + 0x7C)
#define EV_RES_LINETOOL_BT_GET_MAC          ((ARPC_ID_LINETOOLAPP<<8) + 0x7D)
#define EV_RES_LINETOOL_GNSS_GET_GYRO       ((ARPC_ID_LINETOOLAPP<<8) + 0x7E)
#define EV_RES_LINETOOL_GNSS_GET_NAVI       ((ARPC_ID_LINETOOLAPP<<8) + 0x7F)
#define EV_RES_LINETOOL_WIFI_TETHER_SET     ((ARPC_ID_LINETOOLAPP<<8) + 0x80)
#define EV_REQ_LINETOOL_WIFI_TETHER_SET     ((ARPC_ID_LINETOOLAPP<<8) + 0x81)
#define EV_REQ_LINETOOL_GET_RDS_PI          ((ARPC_ID_LINETOOLAPP<<8) + 0x82)
#define EV_REQ_LINETOOL_GET_RDS_PS          ((ARPC_ID_LINETOOLAPP<<8) + 0x83)
#define EV_REQ_LINETOOL_GET_RDS_PTY         ((ARPC_ID_LINETOOLAPP<<8) + 0x84)
#define EV_RES_LINETOOL_GET_RDS_PI          ((ARPC_ID_LINETOOLAPP<<8) + 0x85)
#define EV_RES_LINETOOL_GET_RDS_PS          ((ARPC_ID_LINETOOLAPP<<8) + 0x86)
#define EV_RES_LINETOOL_GET_RDS_PTY         ((ARPC_ID_LINETOOLAPP<<8) + 0x87)
#define EV_REQ_LINETOOL_MIC_APPOINT			((ARPC_ID_LINETOOLAPP<<8) + 0x88)
#define EV_RES_LINETOOL_MIC_APPOINT			((ARPC_ID_LINETOOLAPP<<8) + 0x89)
/* LineTool End */

/* Bluetooth start */
#define EV_REQ_BT_TO_LINKMGR_CLEAR_INFO			((ARPC_ID_BT<<8) + 0x00)
#define EV_RES_BT_TO_LINKMGR_CLEAR_INFO			((ARPC_ID_BT<<8) + 0x01)
/* Bluetooth End */

//mediadetect start
#define EV_INF_USB_DEVICE_CONNNECT ((ARPC_ID_MEDIA_DETECT << 8) + 0x00)
#define EV_INF_USB_DEVICE_DISCONNNECT ((ARPC_ID_MEDIA_DETECT << 8) + 0x01)
//mediadetect end


//DGL Start
enum {
	//HMI->DGL
	HMI_EVT_DGL_REQ_RWC_SGL_SET = ((ARPC_ID_DGLAPP<<8) + 0x00),		
	HMI_EVT_DGL_REQ_RWC_SGL_VALUE,
	HMI_EVT_DGL_REQ_GL_ADJ_OPER,
	HMI_EVT_DGL_REQ_COORDINATE_XY,
	//DGL->HMI
	DGL_EVT_HMI_RES_RWC_SGL_SET,
	DGL_EVT_HMI_RES_RWC_SGL_VALUE,
	DGL_EVT_HMI_RES_SAVE_STATE,
	DGL_EVT_HMI_RES_LOAD_STATE,
	DGL_EVT_HMI_RES_USB_FILE_EXIST,
	DGL_EVT_HMI_RES_COORDINATE_CHG,
	//RWC->DGL
	CAM_EVT_DGL_REQ_KEY_VIEWMODE_CHANGE,
	//DGL->RWC
	DGL_EVT_CAM_RES_KEY_VIEWMODE_CHANGE,
	DGL_EVT_CAM_REQ_VIEW_MODE_SET,
	//VIDEOSRC->DGL
	CAM_EVT_DGL_REQ_GL_ON,
	CAM_EVT_DGL_REQ_GL_OFF,
	//DGL->VIDEOSRC
	DGL_EVT_CAM_RES_GL_ON,
	DGL_EVT_CAM_RES_GL_OFF
};
//DGL End

//Rear Camera App Start
enum {
	//HMI->Rear Camera App
	EV_REQ_RWC_VIEWMODE_CHG	= ((ARPC_ID_REARCAMERA<<8) + 0x00),
	EV_REQ_RWC_VIEWSFT_MODE,
	EV_REQ_RWC_SPLIT_SCREEN_STS,
	EV_REQ_SET_RWC_DEFAULT,
	EV_REQ_RWC_SETUP_VALUE,
	EV_INF_RWC_CTRL_MODULE,
	EV_REQ_RWC_ERROR_STS,
	EV_REQ_RWC_DIAG_STATUS,
	EV_REQ_RWC_DGL_ADJ_OFF,//for hmi
	EV_REQ_RWC_DGL_ADJ_ON,//for hmi		
	//Rear Camera App->HMI
	EV_RES_RWC_VIEWMODE_CHG = ((ARPC_ID_REARCAMERA<<8) + 0x70),
	EV_RES_RWC_VIEWSFT_MODE,
	EV_RES_RWC_SPLIT_SCREEN_STS,
	EV_RES_SET_RWC_DEFAULT,
	EV_RES_RWC_SETUP_VALUE,
	EV_INF_RWC_ERROR_STS,
	EV_INF_RWC_ON,
	EV_INF_RWC_OFF,
	EV_INF_REVERSE_STS,
	EV_INF_RWC_SOURCE_STS,
	EV_REQ_DGL_REQ_DGL_ADJ_ON,//for dgl
	EV_REQ_DGL_REQ_DGL_ADJ_OFF//for dgl	
};
//Rear Camera App Stop

//LWC Camera App Start
enum {
	//HMI->LWC Camera App
	EV_REQ_LWC_DELAY_DISPLAY_SET = ((ARPC_ID_LANEWATCH<<8) + 0x00),
	EV_REQ_LWC_TURN_SIGNAL_SET,		
	EV_REQ_LWC_GUIDELINE_SET,		
	EV_REQ_SET_LWC_DEFAULT,		
	EV_REQ_LWC_SETUP_VALUE,			
	EV_INF_SERVICE_AMING,			
	EV_REQ_LWC_SPLIT_SCREEN_STS,		
	EV_REQ_LWC_DAYNIGHT_SET,			
	EV_INF_LWC_ORIGINAL_ON,			
	EV_INF_LWC_ORIGINAL_OFF,		
	EV_INF_LWC_CTRL_MODULE,			
	EV_INF_LWC_CLOSE,	
	EV_REQ_LWC_ERROR_STS,
	//LET->LWC Camera App
	EV_REQ_LET_BSM_INITIAL,			
	EV_REQ_LET_BSM_START_TP_SET,		
	EV_REQ_LET_BSM_END,				
	EV_REQ_LET_BSM_RESULT,			
    //LWC Camera App->HMI
	EV_RES_LWC_SPLIT_SCREEN_STS = ((ARPC_ID_LANEWATCH<<8) + 0x70),
	EV_RES_LWC_DAYNIGHT_SET,		
	EV_RES_LWC_TURN_SIGNAL_SET,	
	EV_RES_LWC_DELAY_DISPLAY_SET,	
	EV_RES_LWC_GUIDELINE_SET,		
	EV_RES_SET_LWC_DEFAULT,			
	EV_RES_LWC_SETUP_VALUE,			
	EV_INF_LWC_ERROR_STS,			
	EV_INF_LWC_ON,					
	EV_INF_LWC_OFF,					
	EV_INF_LWC_AMING_START,			
	EV_INF_LWC_AMING_COMPLETE,	
	EV_INF_LWC_SOURCE_STS,
    //LWC Camera App->LET	
	EV_RES_LET_BSM_INITIAL,			
	EV_RES_LET_BSM_START_TP_SET,	
	EV_RES_LET_BSM_END,				
	EV_RES_LET_BSM_RESULT,
};	
//LWC Camera App Stop

//LET Start
enum {
/* 000 */ EV_LET_MIN = ((ARPC_ID_LET<<8) + 0x00),
/* 001 */ EV_LET_INVALID,
/* 002 */ EV_INF_LET_AUDIO_DIAG_START_REQ,
/* 003 */ EV_INF_LET_AUDIO_DIAG_START_RES,
/* 004 */ EV_INF_LET_AUDIO_DIAG_END_REQ,
/* 005 */ EV_INF_LET_AUDIO_DIAG_END_RES,
/* 006 */ EV_LET_USB_SPK_SRC_ON,//reserve
/* 007 */ EV_LET_USB_SPK_SRC_ON_RES,//reserve
/* 008 */ EV_LET_USB_SPK_SRC_OFF,//reserve
/* 009 */ EV_LET_USB_SPK_SRC_OFF_RES,//reserve
/* 010 */ EV_LET_BEEP_SPK_SRC_ON,//reserve
/* 011 */ EV_LET_BEEP_SPK_SRC_ON_RES,//reserve
/* 012 */ EV_LET_BEEP_SPK_SRC_OFF,//reserve
/* 013 */ EV_LET_BEEP_SPK_SRC_OFF_RES,//reserve
/* 014 */ EV_LET_SPK_DISPLAY,//reserve
/* 015 */ EV_LET_SPK_DISPLAY_RES,//reserve
/* 016 */ EV_LET_USB_PLAY,//reserve
/* 017 */ EV_LET_USB_PLAY_RES,//reserve
/* 018 */ EV_LET_USB_PLAY_STOP,//reserve
/* 019 */ EV_LET_USB_PLAY_STOP_RES,//reserve
/* 020 */ EV_LET_BUTTON_INF,//reserve
/* 021 */ EV_LET_USB_DISCONNECT,//reserve
/* 022 */ EV_LET_INLINE_AGENT_CLR,//reserve
/* *** */ EV_LET_MAX /* do not delete */
};

//LET End

//AGENT start
enum {
	//A53 -> R5
	EVENT_CODE_REQ_DATA_WRITE = ((ARPC_ID_A53_AGENT<<8) + 0x00),
	//R5 - >A53
	EVENT_CODE_RES_DATA_WRITE,
	EVENT_CODE_REQ_DATA_READ,
	EVENT_CODE_RES_DATA_READ,
	// Agent event req
	EVT_MOUDLE_REQ_MIN,
    EVT_CARPLAYMAC_REQ,
    EVT_DANUMBER_REQ,
    EVT_SERIALNUM_REQ,
    EVT_MANUFACTURE_DATA_REQ,
    EVT_CRYSTAL_OSCILLATOR_REQ,
    EVT_MOUDLE_DIFFERENCE_REQ,
    EVT_RPOJECT_REQ,
    EVT_TRACEABILITY_REQ,
    EVT_SELF_AGING_REQ,
    EVT_HAC_AO_PN_REQ,
	EVT_USB_MUSIC_REQ,
    EVT_MOUDLE_REQ_MAX,
	//Agent event res
    EVT_MOUDLE_RES_MIN,
    EVT_CARPLAYMAC_RES,
    EVT_DANUMBER_RES,
    EVT_SERIALNUM_RES,
    EVT_MANUFACTURE_DATA_RES,
    EVT_CRYSTAL_OSCILLATOR_RES,
    EVT_MOUDLE_DIFFERENCE_RES,
    EVT_RPOJECT_RES,
    EVT_TRACEABILITY_RES,
    EVT_SELF_AGING_RES,
    EVT_HAC_AO_PN_RES,
	EVT_USB_MUSIC_RES,
    EVT_MOUDLE_RES_MAX,
};
//AGENT End
#if 0
//EEPROM start
enum {
	//PM -> EEPROM
	EVENT_CODE_REQ_LOAD = ((ARPC_ID_EEPROM<<8) + 0x00),
	//EEPROM - >PM
	EVENT_CODE_RES_LOAD,
	EVENT_CODE_REQ_UNLOAD,
	EVENT_CODE_RES_UNLOAD,
};
//EEPROM End
#endif

/* VideoSrcMgr Start*/
enum Video_Event{
		EV_INF_VIDEO_SOURCE_ON_REQ = ((ARPC_ID_VIDEOSOURCE<<8)+0x01),	/* param:	VSrcOpt_st */	
		EV_INF_VIDEO_SOURCE_ON_RES,				/* param:	VSrcOpt_st */
		EV_INF_VIDEO_SOURCE_OFF_REQ,			/* param:	VSrcOpt_st */	
		EV_INF_VIDEO_SOURCE_OFF_RES,			/* param:	VSrcOpt_st */	
		EV_INF_VIDEO_CHANGE_END,				/* param:	VSrcChg_Info_st */	/* for HMI ,while carplay source change NG ,return last screen */	
		EV_INF_VIDEO_LUT_SET_REQ,				/* param:	VLUTReq_Info_st */
		EV_INF_VIDEO_LUT_SET_RES,				/* param:	VLUTRES_Info_st */
		EV_INF_VIDEO_SOURCE_CHANGE_REQ,			/* param:	VideoSrcID */
		EV_INF_VIDEO_SOURCE_STREAM_START,		/* no param */	/* reserve */
		EV_INF_VIDEO_CHANGE_SUB_SCREEN_REQ,		/* param:	VIDEO_SUB_SRC_ID */		
		EV_INF_VIDEO_DAYNIGHT_STATUS_REQ,		/* param:	DAY_NIGHT_MODE */				
		EV_INF_VIDEO_MUTE_REQ,					/* param:	VSrc_Mute_Info_st */		
		EV_INF_VIDEO_GET_RGB,					/* no param */	
		EV_INF_VIDEO_RGB_NOTICE,				/* param:	VSrc_VQA_Info_st */		
		EV_INF_VIDEO_DIAG_LUT_SET_REQ,			/* param:	VLUTDiag_Info_st */		
		EV_INF_VIDEO_DIAG_LUT_SET_RES,			/* param:	VLUTDiag_Info_st */	
		EV_INF_VIDEO_DIAG_GET_LUT,				/* no param */
		EV_INF_VIDEO_DIAG_LUT_NOTICE,			/* param:	VLUTDiag_Info_st */
		EV_INF_VIDEO_CURRENT_SOURCE_NOTICE,		/* param:	VideoSrcID:V_SOURCE_ID_NONE  V_SOURCE_ID_RWC  V_SOURCE_ID_LWC */
		EV_INF_VIDEO_SUB_HMI_OFF_REQ,			/* param:	VideoSrcID */
		EV_INF_VIDEO_CAM_MODE_ON_REQ,			/* param:	VideoSrcID */
};										
/* VideoSrcMgr End*/
enum lcd_rpc_event{
	EV_INF_LCD_TEMP_REQ						=	((ARPC_ID_LCD << 8)  + 0x00),				//	lcd temperature request
	EV_INF_LCD_TEMP_RES,																								//	lcd temperature response
	EV_INF_PWM_LEVEL_CHG_REQ,																					//	change pwm by level request
	EV_INF_PWM_LEVEL_CHG_RES,																					//	change pwm by level response
	EV_INF_PWM_DUTY_CHG_REQ,																					//	change pwm by duty request
	EV_INF_PWM_DUTY_CHG_RES,																						//	change pwm by duty response
	EV_INF_LINETOOL_PWM_CHG_REQ,										//?????? ???RH00488A_???????????????_40_LCD??????????????????IssF.xlsx??? LINETOOL??????
	EV_INF_LINETOOL_PWM_CHG_RES,										//?????? ???RH00488A_???????????????_40_LCD??????????????????IssF.xlsx??? LINETOOL??????
	EV_INF_LCD_INTER_EVENT_MAX,																				//	the max event value between core-core rpc
};

/*	updatestm rpc event define	*/
enum updatestm_rpc_event{
	UPD_CMD_MIN									=	((ARPC_ID_UPDATASTM << 8)  + 0x00),
	UPD_CMD_INTER_EVENT_MIN						=	UPD_CMD_MIN,
	UPD_CMD_CURRENT_VER_REQ,
	UPD_CMD_CURRENT_VER_RES,
	UPD_CMD_INTER_EVENT_MAX,
};

enum UsbFs_RPC_Event{
	EVT_USBFS_READ_FILE_REQ		=	((ARPC_ID_USBFILE << 8)  + 0x00),
	EVT_USBFS_WRITE_FILE_REQ,
	EVT_USBFS_FILE_EXIST_REQ,
	
	EVT_USBFS_READ_FILE_RES,
	EVT_USBFS_WRITE_FILE_RES,
	EVT_USBFS_FILE_EXIST_RES,
};

/*	tpctl rpc event define	*/
enum tpctl_rpc_event{
	EV_INF_TPCTL_CMD_MIN						=	((ARPC_ID_TP << 8)  + 0x00),
	EV_INF_TPCTL_INTER_EVENT_MIN				=	EV_INF_TPCTL_CMD_MIN,
	EV_INF_TPCTL_SELF_TEST_REQ,										//touch panel self test request
	EV_INF_TPCTL_SELF_TEST_RES,										//touch panel self test response
	EV_INF_TPCTL_TPSIMU_REQ,										//touch simulate request
	EV_INF_TPCTL_TPSIMU_RES,										//touch simulate response
	EV_INF_TPCTL_POS_DISPATCH_REQ,									//touch dispatch request
	EV_INF_TPCTL_POS_DISPATCH_RES,									//touch dispatch response
	EV_INF_TPCTL_EVE_DISPATCH_NTY,									//touch event notify
	EV_INF_TPCTL_INTER_EVENT_MAX,
};

/*	ecnrtest	*/
enum ecnrtest_rpc_event{
	EV_INF_ECNRTEST_RECORD_START			=	((ARPC_ID_ECNRTEST << 8)  + 0x00),
	EV_INF_ECNRTEST_RECORD_STOP	,
};
enum LineTool_nvm_rpc_event{
 	EV_LINETOOL_NVM_READ_REQ                                                        =           ((ARPC_ID_A53_NVM << 8)  + 0x00),  
	EV_LINETOOL_NVM_WRITE_REQ,
	EV_LINETOOL_NVM_READ_RES,
	EV_LINETOOL_NVM_WRITE_RES,
              
	LED_STANDBYOUT_RES,
	LED_VALUE_GET_REQ,
	LED_VALUE_GET_RES,
	LED_VALUE_SET_REQ,
	LED_VALUE_SET_RES,
	
	EV_NVM_LCD_WRITE_REQ,
	EV_NVM_LCD_WRITE_RES,
};

#endif // __RPC_EVENT_H__
