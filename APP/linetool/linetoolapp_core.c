/********************************************************************************************/
/* File Name	: linetoolapp.h 															*/
/* Date 		: 2021.11.30																*/
/* Author		: Liujie																*/
/* Model		: EDA 																*/
/* Copyright (c)  2021 Alpine Electronics (China) Co. LTD.	All Rights Reserved 			*/
/*------------------------------------------------------------------------------------------*/
/* Revision History 																		*/
/* No		Date		Revised by		Function Name										*/
/* 0001     2021.11.30  Liujie          Create												*/
/*------------------------------------------------------------------------------------------*/
/********************************************************************************************/

/********************************************************************************************/
/*                             Include File Section                                         */
/********************************************************************************************/

/* System Header Files */
#include "os_framework.h"
#include "ProEng.h"
#include "HAL_API.h"
#include "HAL_devices.h"
#include "HAL_LineTool.h"

//#include "srMN103LF26R.h"
// #include "log_Ext.h"

/* Linetool Header Files */
#include "linetoolapp.h"
#include "linetoolapp_core.h"
#include "linetoolapp_common.h"
#include "linetoolapp_os.h"

/* Other Components */
#include "apn_basic_type.h"
#include "rpc_event.h"
#include "rpc_data.h"
#include "sensor_if.h"
#include "Eepromcontrol.h"
#include "logapp.h"
/********************************************************************************************/
/*                           Macro Definition Section                                       */
/********************************************************************************************/

#define LINETOOL_UART_CH                        (2)         /* UART Channel 2*/
#define LINETOOL_UART_BAUD                      (9600UL)    /* baud rate */

#define LINETOOL_CMD_RES_HEADER                 (0x80U)

/* For Process Engine Define start */
#define WTID_0                  (0U)            /* Timer ID(wait time 0)        */
#define WTID_1                  (1U)            /* Timer ID(wait time 1)        */
#define WTID_5                  (2U)            /* Timer ID(wait time 5)        */
#define WTID_50                 (3U)            /* Timer ID(wait time 50)       */
#define WTID_100                (4U)            /* Timer ID(wait time 100)      */
#define WTID_1000               (5U)            /* Timer ID(wait time 1000)     */
#define WTID_5000               (6U)            /* Timer ID(wait time 5000)     */
#define WTID_7000               (7U)            /* Timer ID(wait time 7000)     */
#define WTID_10000              (8U)            /* Timer ID(wait time 10000)    */
#define WTID_15000              (9U)            /* Timer ID(wait time 15000)    */
#define WTID_20000              (10U)           /* Timer ID(wait time 20000)    */
#define WTID_30000              (11U)           /* Timer ID(wait time 30000)    */
#define WTID_60000              (12U)           /* Timer ID(wait time 60000)    */
#define WTID_FEVR               (13U)           /* Timer ID(wait time forever)  */
#define WTID_MAX                (WTID_FEVR + 1) /* Timer ID Max Define          *//* must be consistent with nswLineToolPETimer_Tbl */

/* Timer Value Define */
#define WTVALUE_0               (0)             /* Timer Value(0msec)           */
#define WTVALUE_1               (1)             /* Timer Value(1msec)           */
#define WTVALUE_5               (5)             /* Timer Value(5msec)           */
#define WTVALUE_50              (50)            /* Timer Value(50msec)          */
#define WTVALUE_100             (100)           /* Timer Value(100msec)         */
#define WTVALUE_1000            (1000)          /* Timer Value(1000msec)        */
#define WTVALUE_5000            (5000)          /* Timer Value(5000msec)        */
#define WTVALUE_7000            (7000)          /* Timer Value(7000msec)        */
#define WTVALUE_10000           (10000)         /* Timer Value(10000msec)       */
#define WTVALUE_15000           (15000)         /* Timer Value(15000msec)       */
#define WTVALUE_20000           (20000)         /* Timer Value(20000msec)       */
#define WTVALUE_30000           (30000)         /* Timer Value(30000msec)       */
#define WTVALUE_60000           (60000)         /* Timer Value(60000msec)       */
#define WTVALUE_FEVR            (-1)            /* Timer Value(time forever)    */

#define LINETOOL_PROC_MAX_BUF                   (3U)  /* Process Buffer Size */
#define LINETOOL_Queue_MAX_BUF                  (5U)  /* Process Buffer Size */
#define LINETOOL_TICKCOUNT_MAX                  (0xFFFFFFFFU)   /* Ticket Max */
#define LINETOOL_PROCTIME_MAX                   (0x7FFFFFFFU)   /* Process Max Time */

/* LineTool Security */
#define LINETOOL_SECURITY_CODE                  (0x0A000001U)
#define LINETOOL_MODE_OFF						(0x00U)
#define LINETOOL_MODE_ON					    (0x01U)

/* Read RDS/RBDS Info */
#define LINETOOL_RDS_PI							(0x00U)
#define LINETOOL_RDS_PS							(0x01U)
#define LINETOOL_RDS_PTY						(0x02U)

/* source */
#define LINETOOL_SOURCE_FM                      (0x00U)
#define LINETOOL_SOURCE_AM                      (0x01U)
#define LINETOOL_SOURCE_USB                     (0x03U)
#define LINETOOL_SOURCE_IPOD                    (0x04U)
#define LINETOOL_SOURCE_CPL                     (0x05U)
#define LINETOOL_SOURCE_BT_PHONE                (0x06U)
#define LINETOOL_SOURCE_BT_AUDIO                (0x07U)
#define LINETOOL_SOURCE_AAP                     (0x0AU)
#define LINETOOL_SOURCE_HOME                    (0x0BU)

/* Version Read */
#define LINETOOL_VER_UPDATE_LINUX               (0x01U)
#define LINETOOL_VER_UPDATE_R5                  (0x02U)
#define LINETOOL_VER_A53                        (0x03U)
#define LINETOOL_VER_A7                         (0x04U)
#define LINETOOL_VER_R5                         (0x05U)
#define LINETOOL_VER_SW                         (0x06U)
#define LINETOOL_VER_BT                         (0x07U)

/* Mute Control*/
#define LINETOOL_MUTE_ON                        (0x01U)
#define LINETOOL_MUTE_OFF                       (0x02U)
#define LINETOOL_SOUND_MUTE_ON                  (0x01U)
#define LINETOOL_SOUND_MUTE_OFF                 (0x00U)

/* Loudness */
#define LINETOOL_LOUDNESS_OFF                   (0x00U)
#define LINETOOL_LOUDNESS_ON                    (0x01U)
/* Temperature */
#define LINETOOL_TEMP_LCD						(0x00U)
#define LINETOOL_TEMP_MAIN						(0x01U)
#define LINETOOL_TEMP_INVALID					(0xFFU)

/*EQ*/
#define LINETOOL_EQ_OFF                         (0x00U)
#define LINETOOL_EQ_ON							(0x01U)

/* FM Direct */
#define LINETOOL_FM_MIN						    (875U)
#define LINETOOL_FM_MAX						    (1080U)

/* AM Direct */
#define LINETOOL_AM_MIN						    (531U)
#define LINETOOL_AM_MAX						    (1602U)

/* Track Up/Down*/
#define LINETOOL_TRACK_UP	                    (0x01)
#define LINETOOL_TRACK_DOWN                     (0x02)

/*BTAudio 1KHZ ON OFF*/
#define LINETOOL_BTAUDIO_1KHZ_START             (0x00U)
#define LINETOOL_BTAUDIO_1KHZ_STOP              (0x01U)

/* Camera Chg*/
#define LINETOOL_CAMERA_NORMAL				    (0x00U)
#define LINETOOL_CAMERA_TOP					    (0x01U)
#define LINETOOL_CAMERA_WIDE				    (0x02U)

/*Clock Out*/
#define LINETOOL_SET_CLK_OFF					(0x00U)
#define LINETOOL_SET_CLK_ON					    (0x01U)

/*led on/off define in led_core.h*/
#define LINETOOL_LED_OFF						(0x00U)
#define LINETOOL_LED_ON					    	(0x01U)

/*BT/Carplay MIC On/Off*/
#define LINETOOL_MIC_BT                         (0x00U)
#define LINETOOL_MIC_CPL						(0x01U)

/*Illumination  status*/
#define LINETOOL_ILL_OFF                        (0x00U)
#define LINETOOL_ILL_ON                         (0x01U)

/* Carplay/Ipod select */
#define LINETOOL_CARPLAY                        (0x00U)
#define LINETOOL_IPOD                           (0x01U)

/*WIFI/BT MAC ID READ*/
#define LINETOOL_WIFI_MAC                       (0x00U)
#define LINETOOL_BT_MAC                         (0x01U)

/*Wifi Mode*/
#define LINETOOL_WIFI_MODE_ON                   (0x00U)
#define LINETOOL_WIFI_MODE_OFF                  (0x01U)

/*GNSS GYRO*/
#define LINETOOL_GNSS_GYRO_X                    (0x00U)
#define LINETOOL_GNSS_GYRO_Y                    (0x01U)
#define LINETOOL_GNSS_GYRO_Z                    (0x02U)

/* Key check display */
#define LINETOOL_KEY_QUIT                       (0x00U)
#define LINETOOL_KEY_ENTER                      (0x01U)

/*TP Mode*/
#define LINETOOL_TP_MODE_OFF					(0x00U)
#define LINETOOL_TP_MODE_ON					    (0x01U)

/*LCD Set*/
#define LINETOOL_LCD_VAL_MAX					(100000U)

/* Fan Control */
#define LINETOOL_FAN_HIGH						(0x00U)
// #define LINETOOL_FAN_LOW						(0x01U)	// not used
#define LINETOOL_FAN_OFF						(0x02U)

/*MIC Path State*/
#define LINETOOL_MIC_PATH_OFF					(0x00U)
#define LINETOOL_MIC_PATH_ON					(0x01U)

/*Key Mode*/
#define LINETOOL_KEY_MODE_OFF					(0x00U)
#define LINETOOL_KEY_MODE_ON					(0x01U)
#define LINETOOL_KEY_CHECK						(0x02U)

#define LINETOOL_HOME                           (0x01U)
#define LINETOOL_DAYNIGHT                       (0x02U)
#define LINETOOL_BACK                           (0x03U)
#define LINETOOL_VOLUP                          (0x04U)
#define LINETOOL_VOLDOWN                        (0x05U)
#define LINETOOL_POWER                          (0x06U)
#define LINETOOL_STK_VOLUP                      (0x11U)
#define LINETOOL_STK_VOLDOWN                    (0x12U)
#define LINETOOL_ONHOOK                         (0x13U)
#define LINETOOL_OFFHOOK                        (0x14U)
#define LINETOOL_TALK                           (0x15U)
#define LINETOOL_CHUP                           (0x16U)
#define LINETOOL_CHDOWN                         (0x17U)
#define LINETOOL_MODE                           (0x18U)

/*Camera State*/
#define LINETOOL_CAMERA_OFF					    (0x00U)
#define LINETOOL_CAMERA_ON					    (0x01U)

/*LCD EEPROM Adress*/
#define LINETOOL_LCD_EEP						(0x0202U)

/* TPSensor Check */
#define LINETOOL_TP_SENSOR_CHECK_OK	            (0x01U)
#define LINETOOL_TP_SENSOR_CHECK_NG	            (0x00U)

/*TP TOUCH CHECK*/
#define LINETOOL_TPCHECK_ON                     (0x01U)
#define LINETOOL_TPCHECK_OFF                    (0x00U)

/*MOUDLE NAME*/
#define LINETOOL_RH00488A01                     (0x10U)
#define LINETOOL_RH00545A01                     (0x20U)
#define LINETOOL_RH00485A01                     (0x30U)
//#define LINETOOL_RH00546A01                     (0x40U)   //delete by cxx on 20220412 for #73590
#define LINETOOL_RH00353A01                     (0x50U)     //add RH00353A01\RH00547A01 by cxx on 20220620 for  #73590
#define LINETOOL_RH00547A01                     (0x60U)

/*MIC APPOINT*/
#define LINETOOL_MIC_ONON                       (0x00U)
#define LINETOOL_MIC_ONOFF                      (0x01U)
#define LINETOOL_MIC_OFFON                      (0x02U)
#define LINETOOL_MIC_OFFOFF                     (0x03U)	//add by cxx on 20220428 for MICAPPOINT

/*TEL MUTE*/
#define LINETOOL_TEL_MUTE_ON                    (0x01U)     // H    add by cxx on 20220412 for[Iss.B]#57078
#define LINETOOL_TEL_MUTE_OFF                   (0x00U)     // L    add by cxx on 20220412 for[Iss.B]#57078

/*VOLUME DIRECTION*/
#define SOUND_VOL_DIRECT                        (0x02U)	// add by cxx on 20220617 for #74355
/********************************************************************************************/
/*                           Enumeration Type Definition Section                            */
/********************************************************************************************/

/* Main Process ID Define */
enum {
/* 000 */ PID_LINETOOL_POWEROFF = 0x00U,       /*  */
/* 001 */ PID_LINETOOL_POWERON,                /*  */
/* 002 */ PID_LINETOOL_WARNING_IN,             /*  */
/* 003 */ PID_LINETOOL_LANGUAGE_IN,            /*  */
/* 004 */ PID_LINETOOL_SOURCE_DIRECT,          /*  */
/* 005 */ PID_LINETOOL_VOLUME_DIRECT,          /*  */
/* 006 */ PID_LINETOOL_SOFT_VER_GET,           /*  */
/* 007 */ PID_LINETOOL_MUTE_CONTROL,           /*  */
/* 008 */ PID_LINETOOL_MAIN_TEMP_GET,          /*  */
/* 009 */ PID_LINETOOL_LOUDNESS_SET,           /*  */
/* 010 */ PID_LINETOOL_FAN_CONTROL,            /*  */
/* 011 */ PID_LINETOOL_EQ_SET,                 /*  */
/* 012 */ PID_LINETOOL_FM_DIRECT,              /*  */
/* 013 */ PID_LINETOOL_AM_DIRECT,              /*  */
/*PID_LINETOOL_SEEK_UP_DOWN,*/       /* 0221 delete lj */
/* 014 */ PID_LINETOOL_FM_ALIGNMENT,           /*  */
/* 015 */ PID_LINETOOL_AM_ALIGNMENT,           /*  */
/* 016 */ PID_LINETOOL_RDS_INFO_GET,           /*  */
/* 017 */ PID_LINETOOL_RDS_LEVEL_GET,          /*  */
/* 018 */ PID_LINETOOL_TUNER_SM_GET,           /*  */
/* 019 */ PID_LINETOOL_IPOD_TRACK_CHG,         /*  */
/* 020 */ PID_LINETOOL_USB_TRACK_CHG,          /*  */
/* 021 */ PID_LINETOOL_USB_AUDIO_CHG,          /*  */
/* 022 */ PID_LINETOOL_USB_VIDEO_CHG,          /*  */
/* 023 */ PID_LINETOOL_BT_CHECK,               /*  */
/* 024 */ PID_LINETOOL_BT_AUDIO_1KZ_SET,       /*  */
/*  */
/* 025 */ PID_LINETOOL_BT_LEVEL_DIAG,          /*  */
/* 026 */ PID_LINETOOL_BT_INFO_CLEAR,          /*  */
/* 027 */ PID_LINETOOL_CAMERA_ENTER,           /*  */
/* 028 */ PID_LINETOOL_CAMERA_CHG,             /*  */
/* 029 */ PID_LINETOOL_CAMERA_QUIT,            /*  */
/* 030 */ PID_LINETOOL_CAMERA_CHECK,           /*  */
/* 031 */ PID_LINETOOL_EEP_WRITE,              /*  */
/* 032 */ PID_LINETOOL_EEP_READ,               /*  */
/* 033 */ PID_LINETOOL_EEP_CLEAR,              /*  */
/* 034 */ PID_LINETOOL_EEP_INIT,               /*  */
/* 035 */ PID_LINETOOL_DISPLAY_ADJUST,         /*  */
/* 036 */ PID_LINETOOL_FLICKER_ADJUST,         /*  */
/* 037 */ PID_LINETOOL_FLC_MODE_ON,            /*  */
/* 038 */ PID_LINETOOL_FLC_MODE_OFF,           /*  */
/* 039 */ PID_LINETOOL_CURRENT_TIME_GET,       /*  */
/* 040 */ PID_LINETOOL_TP_TERMINAL_SET,        /*  */
/* 041 */ PID_LINETOOL_LED_ON,                 /*  */
/* 042 */ PID_LINETOOL_LED_OFF,                /*  */
/* 043 */ PID_LINETOOL_LCD_SET,                /*  */
/* 044 */ PID_LINETOOL_TP_CHECK_END,           /*  */
/* 045 */ PID_LINETOOL_TP_CHECK_START,         /*  */
/* 046 */ PID_LINETOOL_TP_CHECK,               /*  */
/* 047 */ PID_LINETOOL_KEY_CHECK,              /*  */
/* 048 */ PID_LINETOOL_KEY_CHECK_ONOFF,        /*  */
/* 049 */ PID_LINETOOL_MIC_PATH_BUILD,         /*  */
/* 050 */ PID_LINETOOL_MIC_PATH_CUT,           /*  */
/* 051 */ PID_LINETOOL_ILL_CHECK,              /*  */
/* 052 */ PID_LINETOOL_ENGLISH_SET,            /*  */
/* 053 */ PID_LINETOOL_CARPLAY_IPOD_CHG,       /*  */
/* 054 */ PID_LINETOOL_MODE_NAME_GET,          /*  */
/* 055 */ PID_LINETOOL_CARPLAY_TRACK,          /*  */
/* 056 */ PID_LINETOOL_CARPLAY_ON_OFF,         /*  */
/* 057 */ PID_LINETOOL_AAP_ON_OFF,             /*  */
/* 058 */ PID_LINETOOL_SPEED_PLUS_GET,         /*  */
/* 059 */ PID_LINETOOL_WIFI_SSID_SET,          /*  */
/* 060 */ PID_LINETOOL_WIFI_PASSWORD_SET,      /*  */
/* 061 */ PID_LINETOOL_WIFI_CLEAR_INFO,        /*  */
/* 062 */ PID_LINETOOL_WIFI_CONNUM_GET,        /*  */
/* 063 */ PID_LINETOOL_WIFI_RESET,             /*  */
/* 064 */ PID_LINETOOL_WIFI_CON_CHECK,         /*  */
/* 065 */ PID_LINETOOL_WIFI_MODE_SET,          /*  */
/* 066 */ PID_LINETOOL_MAC_GET,                /*  */
/* 067 */ PID_LINETOOL_GNSS_GYRO_GET,          /*  */
/* 068 */ PID_LINETOOL_GNSS_NAVI_GET,          /*  */
/* 069 */ PID_LINETOOL_LCD_TEMP_GET,           /*  */
/* 070 */ PID_LINETOOL_SECURITY_ON,            /*  */
/* 071 */ PID_LINETOOL_SECURITY_OFF,           /*  */
/* 072 */ PID_LINETOOL_SCREENTOUCH,            /*  */
/* 073 */ PID_LINETOOL_TP_SENSOR_CHECK,        /*  */
/* 074 */ PID_LINETOOL_MICAPPOINT,             /*  *///0317 cxx
/* 075 */ PID_LINETOOL_TEL_MUTE,               /*  *///0317 cxx
/* *** */ PID_PROC_MAX /* LineToolApp Process ID MAX < 255 */
};

/* Sub Process ID Define */
enum {
/* 000 */SPID_RESULT_FINISH = 0x00U,
/* 001 */SPID_RESULT_ERROR,
/* 002 */SPID_POWEROFF,
/* 003 */SPID_POWEROFF_CB,
/* 004 */SPID_POWERON,
/* 005 */SPID_POWERON_CB,
/* 006 */SPID_WARNING_IN,
/* 007 */SPID_WARNING_IN_CB,
/* 008 */SPID_LANGUAGE_IN,
/* 009 */SPID_LANGUAGE_IN_CB,
/* 010 */SPID_SOURCE_DIRECT,
/* 011 */SPID_SOURCE_DIRECT_CB,
/* 012 */SPID_VOLUME_DIRECT,
/* 013 */SPID_VOLUME_DIRECT_CB,
/* 014 */SPID_SOFT_VER_GET,
/* 015 */SPID_SOFT_VER_GET_CB,
/* 016 */SPID_MUTE_CONTROL,
/* 017 */SPID_MUTE_CONTROL_CB,
/* 018 */SPID_LCD_TEMP_GET,
/* 019 */SPID_LCD_TEMP_GET_CB,
/* 020 */SPID_LOUDNESS_SET,
/* 021 */SPID_LOUDNESS_SET_CB,
/* 022 */SPID_FAN_CONTROL,
/* 023 */SPID_FAN_CONTROL_CB,
/* 024 */SPID_EQ_SET,
/* 025 */SPID_EQ_SET_CB,
/* 026 */SPID_FM_DIRECT,
/* 027 */SPID_FM_DIRECT_CB,
/* 028 */SPID_AM_DIRECT,
/* 029 */SPID_AM_DIRECT_CB,
/*SPID_SEEK_UP_DOWN,     0221 delete lj */
/*SPID_SEEK_UP_DOWN_CB,  0221 delete lj */
/* 030 */SPID_FM_ALIGNMENT,
/* 031 */SPID_FM_ALIGNMENT_CB,
/* 032 */SPID_AM_ALIGNMENT,
/* 033 */SPID_AM_ALIGNMENT_CB,
/* 034 */SPID_RDS_INFO_GET,
/* 035 */SPID_RDS_INFO_GET_CB,
/* 036 */SPID_RDS_LEVEL_GET,
/* 037 */SPID_RDS_LEVEL_GET_CB,
/* 038 */SPID_TUNER_SM_GET,
/* 039 */SPID_TUNER_SM_GET_CB,
/* 040 */SPID_IPOD_TRACK_CHG,
/* 041 */SPID_IPOD_TRACK_CHG_CB,
/* 042 */SPID_USB_TRACK_CHG,
/* 043 */SPID_USB_TRACK_CHG_CB,
/* 044 */SPID_USB_AUDIO_CHG,
/* 045 */SPID_USB_AUDIO_CHG_CB,
/* 046 */SPID_USB_VIDEO_CHG,
/* 047 */SPID_USB_VIDEO_CHG_CB,
/* 048 */SPID_BT_CHECK,
/* 049 */SPID_BT_CHECK_CB,
/* 050 */SPID_BT_AUDIO_1KZ_SET,
/* 051 */SPID_BT_AUDIO_1KZ_SET_CB,
/*  */
/*  */
/* 052 */SPID_BT_LEVEL_DIAG,
/* 053 */SPID_BT_LEVEL_DIAG_CB,
/* 054 */SPID_BT_INFO_CLEAR,
/* 055 */SPID_BT_INFO_CLEAR_CB,
/* 056 */SPID_CAMERA_ENTER,
/* 057 */SPID_CAMERA_ENTER_CB,
/* 058 */SPID_CAMERA_CHG,
/* 059 */SPID_CAMERA_CHG_CB,
/* 060 */SPID_CAMERA_QUIT,
/* 061 */SPID_CAMERA_QUIT_CB,
/* 062 */SPID_CAMERA_CHECK,
/* 063 */SPID_CAMERA_CHECK_CB,
/* 064 */SPID_EEP_WRITE,
/* 065 */SPID_EEP_READ,
/* 066 */SPID_EEP_CLEAR,
/* 067 */SPID_MEMORY_INIT,
/* 068 */SPID_MEMORY_INIT_CB,
/* 069 */SPID_DISPLAY_ADJUST,
/* 070 */SPID_DISPLAY_ADJUST_CB,
/* 071 */SPID_FLICKER_MODE_JUDGE,
/* 072 */SPID_FLICKER_ADJUST,
/* 073 */SPID_FLICKER_ADJUST_CB,
/* 074 */SPID_FLICKER_SET_EEP,
/* 075 */SPID_FLC_MODE_ON,
/* 076 */SPID_FLC_MODE_ON_CB,
/* 077 */SPID_FLC_MODE_OFF,
/* 078 */SPID_FLC_MODE_OFF_CB,
/* 079 */SPID_CURRENT_TIME_GET,
/* 080 */SPID_CURRENT_TIME_GET_CB,
/* 081 */SPID_TP_TERMINAL_SET,
/* 082 */SPID_TP_TERMINAL_SET_CB,
/* 083 */SPID_LED_ON,
/* 084 */SPID_LED_ON_CB,
/* 085 */SPID_LED_OFF,
/* 086 */SPID_LED_OFF_CB,
/* 087 */SPID_LCD_SET_EMMC,
/* 088 */SPID_LCD_SET_EMMC_CB,
/* 087 */SPID_LCD_SET,
/* 088 */SPID_LCD_SET_CB,
/* 089 */SPID_LCD_SET_EEP,
/* 090 */SPID_TP_CHECK_END,
/* 091 */SPID_TP_CHECK_END_CB,
/* 092 */SPID_TP_CHECK_START,
/* 093 */SPID_TP_CHECK_START_CB,
/* 094 */SPID_TP_CHECK,
/* 095 */SPID_TP_CHECK_CB,
/* 096 */SPID_KEY_CHECK,
/* 097 */SPID_KEY_CHECK_CB,
/* 098 */SPID_KEY_CHECK_ONOFF,
/* 099 */SPID_KEY_CHECK_ONOFF_CB,
/* 100 */SPID_MIC_PATH_BUILD,
/* 101 */SPID_MIC_PATH_BUILD_CB,
/* 102 */SPID_MIC_PATH_CUT,
/* 103 */SPID_MIC_PATH_CUT_CB,
/* 104 */SPID_ILL_CHECK,
/* 105 */SPID_ENGLISH_SET,
/* 106 */SPID_ENGLISH_SET_CB,
/* 107 */SPID_CARPLAY_IPOD_CHG,
/* 108 */SPID_CARPLAY_IPOD_CHG_CB,
/* 109 */SPID_MODE_NAME_GET,
/*SPID_MODE_NAME_GET_CB,   0228 lj*/
/* 110 */SPID_CARPLAY_TRACK,
/* 111 */SPID_CARPLAY_TRACK_CB,
/* 112 */SPID_CARPLAY_ON_OFF,
/* 113 */SPID_CARPLAY_ON_OFF_CB,
/* 114 */SPID_AAP_ON_OFF,
/* 115 */SPID_AAP_ON_OFF_CB,
/* 116 */SPID_SPEED_PLUS_GET,
/* 117 */SPID_WIFI_SSID_SET,
/* 118 */SPID_WIFI_SSID_SET_CB,
/* 119 */SPID_WIFI_PASSWORD_SET,
/* 120 */SPID_WIFI_PASSWORD_SET_CB,
/* 121 */SPID_WIFI_CLEAR_INFO,
/* 122 */SPID_WIFI_CLEAR_INFO_CB,
/* 123 */SPID_WIFI_CONNUM_GET,
/* 124 */SPID_WIFI_CONNUM_GET_CB,
/* 125 */SPID_WIFI_RESET,
/* 126 */SPID_WIFI_RESET_CB,
/* 127 */SPID_WIFI_CON_CHECK,
/* 128 */SPID_WIFI_CON_CHECK_CB,
/* 129 */SPID_WIFI_MODE_SET,
/* 130 */SPID_WIFI_MODE_SET_CB,
/* 131 */SPID_MAC_GET,
/* 132 */SPID_MAC_GET_CB,
/* 133 */SPID_GNSS_GYRO_GET,
/* 134 */SPID_GNSS_GYRO_GET_CB,
/* 135 */SPID_GNSS_NAVI_GET,
/* 136 */SPID_GNSS_NAVI_GET_CB,
/* 137 */SPID_SECURITY_ON,
/* 138 */SPID_SECURITY_OFF,
/* 139 */SPID_TEST_SOURCE_ON,
/* 140 */SPID_TEST_SOURCE_ON_CB,
/* 141 */SPID_TEST_SOURCE_OFF,
/* 142 */SPID_TEST_SOURCE_OFF_CB,
/* 143 */SPID_MAIN_TEMP_GET,
/* 144 */SPID_SCREENTOUCH,
/* 145 */SPID_SCREENTOUCH_CB,
/* 146 */SPID_TPSENSORCHECK,
/* 147 */SPID_TPSENSORCHECK_CB,
/* 148 */SPID_MICAPPOINT,           //  0317 cxx
/* 149 */SPID_MICAPPOINT_CB,        //  0317 cxx
/* 150 */SPID_TEL_MUTE,             //  0317 cxx
//SPID_TEL_MUTE_CB,                 //  delete by cxx on 20220428 for tel mute
/* 151 */SPID_MIC_SOUND_ON,         //  add L502-L505 by cxx on 20220428 for MIC MODE ON/OFF
/* 152 */SPID_MIC_SOUND_ON_CB,
/* 153 */SPID_MIC_SOUND_OFF,
/* 154 */SPID_MIC_SOUND_OFF_CB,
/* 155 */SPID_PROC_MAX              /* LineToolApp SubProcess ID MAX */

};

enum {
    LINETOOL_RES_ACK = 0U,    /* ACK Response */
    LINETOOL_RES_FINISH,      /* Finish Response */
    LINETOOL_RES_DATA,        /* Result Data Send Response */
    LINETOOL_RES_NACK,        /* NACK Response */
    LINETOOL_RES_ERROR        /* NACK Response */
};

/********************************************************************************************/
/*                           User type definition section                                   */
/********************************************************************************************/

typedef struct{
    Type_uHWord uhInputEvent;
    Type_uHWord uhPrpcessID;
}LineTool_EventChg_ts;


// typedef struct {     // delete L522-539 by cxx on 20220425 for remove to rpc_date.h
//     Type_uByte  aubType;
//     Type_uByte  aubTemp;
//     Type_uHWord auhGyro;
//     Type_uHWord auhAcc;
// } LineTool_GyroData_ts;

// typedef struct {
//     Type_uByte aubStaNum;
//     Type_uByte aubEL;
//     Type_uByte aubCN;
//     Type_uHWord aubAZI;
// }LineTool_NaviDataSing_ts;

// typedef struct {
//     Type_uByte aubStaCount;
//     LineTool_NaviDataSing_ts astNaviData[LINETOOL_NUM_10];
// } LineTool_NaviData_ts;


/********************************************************************************************/
/*                           Structure/Union Type Definition Section                        */
/********************************************************************************************/

/* not used. */

/********************************************************************************************/
/*                           ROM Table Section                                              */
/********************************************************************************************/

/*************************** DiagApp PE Wait Timer ******************************************/
static const Type_sWord nswLineToolPETimer_Tbl[] = {         /* must be consistent with WTID_MAX */
    WTVALUE_0,
    WTVALUE_1,
    WTVALUE_5,
    WTVALUE_50,
    WTVALUE_100,
    WTVALUE_1000,
    WTVALUE_5000,
    WTVALUE_7000,
    WTVALUE_10000,
    WTVALUE_15000,
    WTVALUE_20000,
    WTVALUE_30000,
    WTVALUE_60000,
    WTVALUE_FEVR
};

/* LineTool Event Exchange Table */
static LineTool_EventChg_ts const nstLineToolAppEvChg_Tbl[] = {          /* must be consistent with LINETOOL_EVENT_DEF */
/*** Input Event ***                   *** Process ID ***/
/* 000 */ { LINETOOL_EV_POWEROFF,           PID_LINETOOL_POWEROFF},
/* 001 */ { LINETOOL_EV_POWERON,            PID_LINETOOL_POWERON},
/* 002 */ { LINETOOL_EV_WARNING_IN,         PID_LINETOOL_WARNING_IN},
/* 003 */ { LINETOOL_EV_LANGUAGE_IN,        PID_LINETOOL_LANGUAGE_IN},
/* 004 */ { LINETOOL_EV_SOURCE_DIRECT,      PID_LINETOOL_SOURCE_DIRECT},
/* 005 */ { LINETOOL_EV_VOLUME_DIRECT,      PID_LINETOOL_VOLUME_DIRECT},
/* 006 */ { LINETOOL_EV_SOFT_VER_GET,       PID_LINETOOL_SOFT_VER_GET},
/* 007 */ { LINETOOL_EV_MUTE_CONTROL,       PID_LINETOOL_MUTE_CONTROL},
/* 008 */ { LINETOOL_EV_MAIN_TEMP_GET,      PID_LINETOOL_MAIN_TEMP_GET},
/* 009 */ { LINETOOL_EV_LOUDNESS_SET,       PID_LINETOOL_LOUDNESS_SET},
/* 010 */ { LINETOOL_EV_FAN_CONTROL,        PID_LINETOOL_FAN_CONTROL},
/* 011 */ { LINETOOL_EV_EQ_SET,             PID_LINETOOL_EQ_SET},
/* 012 */ { LINETOOL_EV_FM_DIRECT,          PID_LINETOOL_FM_DIRECT},
/* 013 */ { LINETOOL_EV_AM_DIRECT,          PID_LINETOOL_AM_DIRECT},
/*{ LINETOOL_EV_SEEK_UP_DOWN,       PID_LINETOOL_SEEK_UP_DOWN}, 0221 delete lj */
/* 014 */ { LINETOOL_EV_FM_ALIGNMENT,       PID_LINETOOL_FM_ALIGNMENT},
/* 015 */ { LINETOOL_EV_AM_ALIGNMENT,       PID_LINETOOL_AM_ALIGNMENT},
/* 016 */ { LINETOOL_EV_RDS_INFO_GET,       PID_LINETOOL_RDS_INFO_GET},
/* 017 */ { LINETOOL_EV_RDS_LEVEL_GET,      PID_LINETOOL_RDS_LEVEL_GET},
/* 018 */ { LINETOOL_EV_TUNER_SM_GET,       PID_LINETOOL_TUNER_SM_GET},
/* 019 */ { LINETOOL_EV_IPOD_TRACK_CHG,     PID_LINETOOL_IPOD_TRACK_CHG},
/* 020 */ { LINETOOL_EV_USB_TRACK_CHG,      PID_LINETOOL_USB_TRACK_CHG},
/* 021 */ { LINETOOL_EV_USB_AUDIO_CHG,      PID_LINETOOL_USB_AUDIO_CHG},
/* 022 */ { LINETOOL_EV_USB_VIDEO_CHG,      PID_LINETOOL_USB_VIDEO_CHG},
/* 023 */ { LINETOOL_EV_BT_CHECK,           PID_LINETOOL_BT_CHECK},
/* 024 */ { LINETOOL_EV_BT_AUDIO_1KZ_SET,   PID_LINETOOL_BT_AUDIO_1KZ_SET},
/*  */
/* 025 */ { LINETOOL_EV_BT_LEVEL_DIAG,      PID_LINETOOL_BT_LEVEL_DIAG},
/* 026 */ { LINETOOL_EV_BT_INFO_CLEAR,      PID_LINETOOL_BT_INFO_CLEAR},
/* 027 */ { LINETOOL_EV_CAMERA_ENTER,       PID_LINETOOL_CAMERA_ENTER},
/* 028 */ { LINETOOL_EV_CAMERA_CHG,         PID_LINETOOL_CAMERA_CHG},
/* 029 */ { LINETOOL_EV_CAMERA_QUIT,        PID_LINETOOL_CAMERA_QUIT},
/* 030 */ { LINETOOL_EV_CAMERA_CHECK,       PID_LINETOOL_CAMERA_CHECK},
/* 031 */ { LINETOOL_EV_EEP_WRITE,          PID_LINETOOL_EEP_WRITE},
/* 032 */ { LINETOOL_EV_EEP_READ,           PID_LINETOOL_EEP_READ},
/* 033 */ { LINETOOL_EV_EEP_CLEAR,          PID_LINETOOL_EEP_CLEAR},
/* 034 */ { LINETOOL_EV_EEP_INIT,           PID_LINETOOL_EEP_INIT},
/* 035 */ { LINETOOL_EV_DISPLAY_ADJUST,     PID_LINETOOL_DISPLAY_ADJUST},
/* 036 */ { LINETOOL_EV_FLICKER_ADJUST,     PID_LINETOOL_FLICKER_ADJUST},
/* 037 */ { LINETOOL_EV_FLC_MODE_ON,        PID_LINETOOL_FLC_MODE_ON},
/* 038 */ { LINETOOL_EV_FLC_MODE_OFF,       PID_LINETOOL_FLC_MODE_OFF},
/* 039 */ { LINETOOL_EV_CURRENT_TIME_GET,   PID_LINETOOL_CURRENT_TIME_GET},
/* 040 */ { LINETOOL_EV_TP_TERMINAL_SET,    PID_LINETOOL_TP_TERMINAL_SET},
/* 041 */ { LINETOOL_EV_LED_ON,             PID_LINETOOL_LED_ON},
/* 042 */ { LINETOOL_EV_LED_OFF,            PID_LINETOOL_LED_OFF},
/* 043 */ { LINETOOL_EV_LCD_SET,            PID_LINETOOL_LCD_SET},
/* 044 */ { LINETOOL_EV_TP_CHECK_END,       PID_LINETOOL_TP_CHECK_END},
/* 045 */ { LINETOOL_EV_TP_CHECK_START,     PID_LINETOOL_TP_CHECK_START},
/* 046 */ { LINETOOL_EV_TP_CHECK,           PID_LINETOOL_TP_CHECK},
/* 047 */ { LINETOOL_EV_KEY_CHECK,          PID_LINETOOL_KEY_CHECK},
/* 048 */ { LINETOOL_EV_KEY_CHECK_ONOFF,    PID_LINETOOL_KEY_CHECK_ONOFF},
/* 049 */ { LINETOOL_EV_MIC_PATH_BUILD,     PID_LINETOOL_MIC_PATH_BUILD},
/* 050 */ { LINETOOL_EV_MIC_PATH_CUT,       PID_LINETOOL_MIC_PATH_CUT},
/* 051 */ { LINETOOL_EV_ILL_CHECK,          PID_LINETOOL_ILL_CHECK},
/* 052 */ { LINETOOL_EV_ENGLISH_SET,        PID_LINETOOL_ENGLISH_SET},
/* 053 */ { LINETOOL_EV_CARPLAY_IPOD_CHG,   PID_LINETOOL_CARPLAY_IPOD_CHG},
/* 054 */ { LINETOOL_EV_MODE_NAME_GET,      PID_LINETOOL_MODE_NAME_GET},
/* 055 */ { LINETOOL_EV_CARPLAY_UP_DOWN,    PID_LINETOOL_CARPLAY_TRACK},
/* 056 */ { LINETOOL_EV_CARPLAY_ON_OFF,     PID_LINETOOL_CARPLAY_ON_OFF},
/* 057 */ { LINETOOL_EV_AAP_ON_OFF,         PID_LINETOOL_AAP_ON_OFF},
/* 058 */ { LINETOOL_EV_SPEED_PLUS_GET,     PID_LINETOOL_SPEED_PLUS_GET},
/* 059 */ { LINETOOL_EV_WIFI_SSID_SET,      PID_LINETOOL_WIFI_SSID_SET},
/* 060 */ { LINETOOL_EV_WIFI_PASSWORD_SET,  PID_LINETOOL_WIFI_PASSWORD_SET},
/* 061 */ { LINETOOL_EV_WIFI_CLEAR_INFO,    PID_LINETOOL_WIFI_CLEAR_INFO},
/* 062 */ { LINETOOL_EV_WIFI_CONNUM_GET,    PID_LINETOOL_WIFI_CONNUM_GET},
/* 063 */ { LINETOOL_EV_WIFI_RESET,         PID_LINETOOL_WIFI_RESET},
/* 064 */ { LINETOOL_EV_WIFI_CON_CHECK,     PID_LINETOOL_WIFI_CON_CHECK},
/* 065 */ { LINETOOL_EV_WIFI_MODE_SET,      PID_LINETOOL_WIFI_MODE_SET},
/* 066 */ { LINETOOL_EV_MAC_GET,            PID_LINETOOL_MAC_GET},
/* 067 */ { LINETOOL_EV_GNSS_GYRO_GET,      PID_LINETOOL_GNSS_GYRO_GET},
/* 068 */ { LINETOOL_EV_GNSS_NAVI_GET,      PID_LINETOOL_GNSS_NAVI_GET},
/* 069 */ { LINETOOL_EV_LCD_TEMP_GET,       PID_LINETOOL_LCD_TEMP_GET},
/* 070 */ { LINETOOL_EV_SECURITY_ON,        PID_LINETOOL_SECURITY_ON},
/* 071 */ { LINETOOL_EV_SECURITY_OFF,       PID_LINETOOL_SECURITY_OFF},
/* 072 */ { LINETOOL_EV_SCREENTOUCH,        PID_LINETOOL_SCREENTOUCH},
/* 073 */ { LINETOOL_EV_TP_SENSOR_CHECK,    PID_LINETOOL_TP_SENSOR_CHECK},
/* 074 */ { LINETOOL_EV_MICAPPOINT,         PID_LINETOOL_MICAPPOINT},   //0317 cxx
/* 075 */ { LINETOOL_EV_TEL_MUTE,           PID_LINETOOL_TEL_MUTE},     //0317 cxx
/* *** */ { LINETOOL_EV_MAX,                PID_PROC_MAX}
};

/* LineTool Communition Data Table */
static Type_uByte const nubLineToolComCMD_Tbl[][LINETOOL_NUM_8] = {
/** DataLength *** DataHeader ** Data1 ******* Data2 ******* Data3 ******* Data4 ******* Data5 ******* Engine PID ***/
    {0x02U,        0x10U,        0x00U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_POWEROFF},
    {0x02U,        0x10U,        0x01U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_POWERON},
    {0x02U,        0x10U,        0x02U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_WARNING_IN},
    {0x02U,        0x10U,        0x04U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_LANGUAGE_IN},
    {0x02U,        0x30U,        0x00U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_SOURCE_DIRECT},
    {0x02U,        0x30U,        0x03U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_VOLUME_DIRECT},
    {0x02U,        0x50U,        0x00U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_SOFT_VER_GET},
    {0x02U,        0x50U,        0x01U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_MUTE_CONTROL},
    {0x03U,        0x50U,        0x02U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_LCD_TEMP_GET},
    {0x03U,        0x50U,        0x02U,        0x01U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_MAIN_TEMP_GET},
    {0x02U,        0x50U,        0x03U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_LOUDNESS_SET},
    {0x02U,        0x50U,        0x04U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_FAN_CONTROL},
    {0x02U,        0x50U,        0x0AU,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_EQ_SET},
    {0x02U,        0x30U,        0x04U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_FM_DIRECT},
    {0x02U,        0x30U,        0x05U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_AM_DIRECT},
/*    {0x02U,        0x30U,        0x06U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_SEEK_UP_DOWN}, 0221 delete lj */
    {0x02U,        0x90U,        0x00U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_FM_ALIGNMENT},
    {0x02U,        0x90U,        0x01U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_AM_ALIGNMENT},
    {0x02U,        0x60U,        0x02U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_RDS_INFO_GET},
    {0x02U,        0x60U,        0x03U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_RDS_LEVEL_GET},
    {0x02U,        0x70U,        0x09U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_TUNER_SM_GET},
    {0x02U,        0x30U,        0x10U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_IPOD_TRACK_CHG},
    {0x02U,        0x30U,        0x11U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_USB_TRACK_CHG},
    {0x03U,        0x50U,        0x21U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_USB_AUDIO_CHG},
    {0x03U,        0x50U,        0x21U,        0x01U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_USB_VIDEO_CHG},
    {0x02U,        0x50U,        0x08U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_BT_CHECK},
    {0x02U,        0x50U,        0x10U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_BT_AUDIO_1KZ_SET},
    {0x02U,        0x60U,        0x08U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_BT_LEVEL_DIAG},
    {0x02U,        0x70U,        0x06U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_BT_INFO_CLEAR},
    {0x02U,        0x50U,        0x0CU,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_CAMERA_ENTER},
    {0x02U,        0x50U,        0x0DU,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_CAMERA_CHG},
    {0x02U,        0x50U,        0x0EU,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_CAMERA_QUIT},
    {0x02U,        0x60U,        0x0AU,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_CAMERA_CHECK},
    {0x02U,        0x70U,        0x00U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_EEP_WRITE},
    {0x02U,        0x70U,        0x01U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_EEP_READ},
    {0x02U,        0x70U,        0x02U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_EEP_CLEAR},
    {0x02U,        0x70U,        0x05U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_EEP_INIT},
    {0x02U,        0x90U,        0x03U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_DISPLAY_ADJUST},
    {0x03U,        0x90U,        0x04U,        0x01U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_FLC_MODE_ON},
    {0x03U,        0x90U,        0x04U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_FLC_MODE_OFF},
    {0x02U,        0x30U,        0x12U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_CURRENT_TIME_GET},
    {0x02U,        0x50U,        0x20U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_TP_TERMINAL_SET},
    {0x02U,        0x30U,        0x21U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_LED_ON},
    {0x02U,        0x30U,        0x22U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_LED_OFF},
    {0x02U,        0x90U,        0x10U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_LCD_SET},
    {0x06U,        0x50U,        0x22U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_TP_CHECK_END},
    {0x06U,        0x50U,        0x22U,        0x01U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_TP_CHECK_START},
    {0x06U,        0x50U,        0x22U,        0x02U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_TP_CHECK},
    {0x02U,        0x50U,        0x23U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_KEY_CHECK},
    {0x02U,        0x50U,        0x29U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_KEY_CHECK_ONOFF},
    {0x02U,        0x50U,        0x24U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_MIC_PATH_BUILD},
    {0x02U,        0x50U,        0x25U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_MIC_PATH_CUT},
    {0x02U,        0x60U,        0x09U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_ILL_CHECK},
    {0x02U,        0x70U,        0x07U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_ENGLISH_SET},
    {0x02U,        0x70U,        0x08U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_CARPLAY_IPOD_CHG},
    {0x02U,        0x70U,        0x0AU,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_MODE_NAME_GET},
    {0x02U,        0x60U,        0x00U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_CARPLAY_UP_DOWN},
    {0x02U,        0x60U,        0x01U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_CARPLAY_ON_OFF},
    {0x02U,        0x60U,        0x06U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_AAP_ON_OFF},
    {0x02U,        0x60U,        0x07U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_SPEED_PLUS_GET},
    {0x02U,        0x70U,        0x0EU,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_WIFI_SSID_SET},
    {0x02U,        0x70U,        0x0FU,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_WIFI_PASSWORD_SET},
    {0x02U,        0x70U,        0x10U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_WIFI_CLEAR_INFO},
    {0x02U,        0x70U,        0x11U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_WIFI_CONNUM_GET},
    {0x02U,        0x70U,        0x12U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_WIFI_RESET},
    {0x02U,        0x70U,        0x13U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_WIFI_CON_CHECK},
    {0x02U,        0x70U,        0x15U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_WIFI_MODE_SET},
    {0x02U,        0x70U,        0x14U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_MAC_GET},
    {0x02U,        0x70U,        0x17U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_GNSS_GYRO_GET},
    {0x02U,        0x70U,        0x18U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_GNSS_NAVI_GET},
    {0x02U,        0xABU,        0x20U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_SCREENTOUCH},
    {0x02U,        0x70U,        0x1CU,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_TP_SENSOR_CHECK},
    {0x02U,        0x50U,        0x12U,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_MICAPPOINT},
    {0x02U,        0x70U,        0x1AU,        0x00U,        0x00U,        0x00U,        0x00U,        LINETOOL_EV_TEL_MUTE},   //0317 cxx
};
static Type_uByte const nubLineToolComCMDSec_Tbl[][LINETOOL_NUM_8] = {
	{0x02U, 	   0x30U,		 0x23U, 	   0x00U,		 0x00U, 	   0x00U,		 0x00U, 	   LINETOOL_EV_SECURITY_ON},
	{0x02U, 	   0x30U,		 0x24U, 	   0x00U,		 0x00U, 	   0x00U,		 0x00U, 	   LINETOOL_EV_SECURITY_OFF},

};
#define LINETOOL_CMD_TAB_SIZE    (sizeof(nubLineToolComCMD_Tbl) / sizeof(nubLineToolComCMD_Tbl[LINETOOL_NUM_0]))
#define LINETOOL_CMDSEC_TAB_SIZE    (sizeof(nubLineToolComCMDSec_Tbl) / sizeof(nubLineToolComCMDSec_Tbl[LINETOOL_NUM_0]))

/********************************************************************************************/
/*                           Static Variable Definition Section                             */
/********************************************************************************************/

/* For LineTool Task Process Engine */
static ProcBuf_ts nstLineToolProcBuf_a[LINETOOL_PROC_MAX_BUF];    /* Process Buffer */
static Type_uWord nuwLineToolStartTime;   /* Main Process Time */
static ProcCTB_ts nstLineToolAppCTB;    /* Process Control Table */
static Type_uByte nubLineToolTempData[LINETOOL_NUM_8];
static OS_StdQueue_ts nstLineToolAppStdQueue;

static Type_uByte nubLineToolTPMode;   /* TP Mode */
static Type_uByte nubLineToolKeyMode;   /* KEY Mode */
static Type_uByte nubLineToolCameraState;   /* Camera Mode */
static Type_uByte nubLineToolMICPathState;   /* MIC Path Mode */
static Linetool_Flicker_ts wstLineToolFlickerData;   /* Flicker Value */

static Type_uByte nubLineToolMode;
static HAL_HANDLE nvdLineToolHalHandle = STD_NULL;

/********************************************************************************************/
/*                           Static Function Declaration Section                            */
/********************************************************************************************/
static Type_uHWord nuhLineToolAppEvPIDChg(void);
static Type_uWord nswLineToolAppGetProcTime(Type_uWord auwStartTime, Type_uWord auwEndTime);
static void nvdLineToolAppDataAnalylis(void);
static void nvdLineToolAppCmdReseponse(Type_uByte aubResKind, const Type_uByte *aubResData, Type_uByte aubResLen);
static void nvdLineToolDataSend(const Type_uByte *aubBuffer_p, const Type_uByte aubBufferSize);
static void nvdLinetoolUARTWrite(const Type_uByte *aubBuffer_p, const Type_uByte aubBufferSize);
static void nvdLineToolAppUartInterrupt(Type_uByte aubState, Type_uByte aubData);

static Type_uByte nubLineToolResFinish(void);
static Type_uByte nubLineToolResError(void);
static Type_uByte nubLineToolFmAlignment(void);
static Type_uByte nubLineToolFmAlignmentCB(void);
static Type_uByte nubLineToolAmAlignment(void);
static Type_uByte nubLineToolAmAlignmentCB(void);
static Type_uByte nubLineToolEepWrite(void);
static Type_uByte nubLineToolEepRead(void);
static Type_uByte nubLineToolGetCurrentTime(void);
static Type_uByte nubLineToolFMDirect(void);
static Type_uByte nubLineToolFMDirectCB(void);
static Type_uByte nubLineToolAMDirect(void);
static Type_uByte nubLineToolAMDirectCB(void);
static Type_uByte nubLineToolPowerOffReq(void);
static Type_uByte nubLineToolPowerOffCB(void);
static Type_uByte nubLineToolPowerOnReq(void);
static Type_uByte nubLineToolPowerOnCB(void);
static Type_uByte nubLineToolWarningInReq(void);
static Type_uByte nubLineToolWarningInCB(void);
static Type_uByte nubLineToolLEDOn(void);
static Type_uByte nubLineToolLEDOnCB(void);
static Type_uByte nubLineToolLEDOff(void);
static Type_uByte nubLineToolLEDOffCB(void);
static Type_uByte nubLineToolSourceDirect(void);
static Type_uByte nubLineToolSourceDirectCB(void);
static Type_uByte nubLineToolLCDSetEMMC(void);
static Type_uByte nubLineToolLCDSetEMMCCB(void);
static Type_uByte nubLineToolLCDSet(void);
static Type_uByte nubLineToolLCDSetCB(void);
static Type_uByte nubLineToolLCDSetEep(void);
static Type_uByte nubLineToolFlickerAdjust(void);
static Type_uByte nubLineToolFlickerAdjustCB(void);
static Type_uByte nubLineToolVolumeDirect(void);
static Type_uByte nubLineToolVolumeDirectCB(void);
static Type_uByte nubLineToolMuteControl(void);
static Type_uByte nubLineToolMuteControlCB(void);
static Type_uByte nubLineToolLoudnessSet(void);
static Type_uByte nubLineToolLoudnessSetCB(void);
static Type_uByte nubLineToolEQSet(void);
static Type_uByte nubLineToolEQSetCB(void);
static Type_uByte nubLineToolMemoryInit(void);
static Type_uByte nubLineToolMemoryInitCB(void);
static Type_uByte nubLineToolEepClear(void);
static Type_uByte nubLineToolGetLcdTemp(void);
static Type_uByte nubLineToolGetLcdTempCB(void);
static Type_uByte nubLineToolGetVerReq(void);
static Type_uByte nubLineToolGetVerCB(void);
/*static Type_uByte nubLineToolSeekUpDownReq(void); 0221 delete lj */
/*static Type_uByte nubLineToolSeekUpDownCB(void);  0221 delete lj */
static Type_uByte nubLineToolIpodTrackChgReq(void);
static Type_uByte nubLineToolIpodTrackChgCB(void);
static Type_uByte nubLineToolUSBTrackChgReq(void);
static Type_uByte nubLineToolUSBTrackChgCB(void);
static Type_uByte nubLineToolCameraEnterReq(void);
static Type_uByte nubLineToolCameraEnterCB(void);
static Type_uByte nubLineToolCameraChgReq(void);
static Type_uByte nubLineToolCameraChgCB(void);
static Type_uByte nubLineToolCameraQuitReq(void);
static Type_uByte nubLineToolCameraQuitCB(void);
static Type_uByte nubLineToolFanControl(void);
static Type_uByte nubLineToolFanControlCB(void);
static Type_uByte nubLineToolBTCheckReq(void);
static Type_uByte nubLineToolBTCheckCB(void);
static Type_uByte nubLineToolBTAudio1kzSetReq(void);
static Type_uByte nubLineToolBTAudio1kzSetCB(void);
static Type_uByte nubLineToolKeyCheck(void);
static Type_uByte nubLineToolKeyCheckCB(void);
static Type_uByte nubLineToolUSBAudioChgReq(void);
static Type_uByte nubLineToolUSBAudioChgCB(void);
static Type_uByte nubLineToolUSBVideoChgReq(void);
static Type_uByte nubLineToolUSBVideoChgCB(void);
static Type_uByte nubLineToolTPCheckEndReq(void);
static Type_uByte nubLineToolTPCheckEndCB(void);
static Type_uByte nubLineToolTPCheckStartReq(void);
static Type_uByte nubLineToolTPCheckStartCB(void);
static Type_uByte nubLineToolTPCheckReq(void);
static Type_uByte nubLineToolTPCheckCB(void);
static Type_uByte nubLineToolFlickerModeJudge(void);
static Type_uByte nubLineToolFlickerModeOn(void);
static Type_uByte nubLineToolFlickerModeOnCB(void);
static Type_uByte nubLineToolFlickerModeOff(void);
static Type_uByte nubLineToolFlickerModeOffCB(void);
static Type_uByte nubLineToolMICPathBuild(void);
static Type_uByte nubLineToolMICPathBuildCB(void);
static Type_uByte nubLineToolMICPathCut(void);
static Type_uByte nubLineToolMICPathCutCB(void);
static Type_uByte nubLineToolDisplayAdjust(void);
static Type_uByte nubLineToolDisplayAdjustCB(void);
static Type_uByte nubLineToolFlickerSetEep(void);
static Type_uByte nubLineToolLCDSetEep(void);
static Type_uByte nubLineToolKeyCheckOnOff(void);
static Type_uByte nubLineToolKeyCheckOnOffCB(void);
static Type_uByte nubLineToolSecurityOn(void);
static Type_uByte nubLineToolSecurityOff(void);
static Type_uByte nubLineToolSecurityTimeout(void);
static Type_uByte nubLineToolIpodCarplayChg(void);
static Type_uByte nubLineToolIpodCarplayChgCB(void);
static Type_uByte nubLineToolCarplayTrack(void);
static Type_uByte nubLineToolCarplayTrackCB(void);
static Type_uByte nubLineToolSecurityOnStart(void);
static Type_uByte nubLineToolSecurityOnStartCB(void);
static Type_uByte nubLineToolGetCurrentTimeCB(void);
static Type_uByte nubLineToolLanguageIn(void);
static Type_uByte nubLineToolLanguageInCB(void);
static Type_uByte nubLineToolReadRDSInfo(void);
static Type_uByte nubLineToolReadRDSInfoCB(void);
static Type_uByte nubLineToolGetRDSLevel(void);
static Type_uByte nubLineToolGetRDSLevelCB(void);
static Type_uByte nubLineToolGetTunerSM(void);
static Type_uByte nubLineToolGetTunerSMCB(void);
static Type_uByte nubLineToolBTLevelDiag(void);
static Type_uByte nubLineToolBTLevelDiagCB(void);
static Type_uByte nubLineToolBTInfoClear(void);
static Type_uByte nubLineToolBTInfoClearCB(void);
static Type_uByte nubLineToolCameraCheck(void);
static Type_uByte nubLineToolCameraCheckCB(void);
static Type_uByte nubLineToolTPTerminalSet(void);
static Type_uByte nubLineToolTPTerminalSetCB(void);
static Type_uByte nubLineToolILLCheck(void);
static Type_uByte nubLineToolEnglishSet(void);
static Type_uByte nubLineToolEnglishSetCB(void);
static Type_uByte nubLineToolVariationGet(void);
/*static Type_uByte nubLineToolVariationGetCB(void);    0228 lj*/
static Type_uByte nubLineToolCarplayOnOff(void);
static Type_uByte nubLineToolCarplayOnOffCB(void);
static Type_uByte nubLineToolAAPOnOff(void);
static Type_uByte nubLineToolAAPOnOffCB(void);
static Type_uByte nubLineToolSpeedPlus(void);
static Type_uByte nubLineToolWifiSSIDSet(void);
static Type_uByte nubLineToolWifiSSIDSetCB(void);
static Type_uByte nubLineToolWifiPassWordSet(void);
static Type_uByte nubLineToolWifiPassWordSetCB(void);
static Type_uByte nubLineToolWifiClearInfo(void);
static Type_uByte nubLineToolWifiClearInfoCB(void);
static Type_uByte nubLineToolWifiConnNumGet(void);
static Type_uByte nubLineToolWifiConnNumGetCB(void);
static Type_uByte nubLineToolWifiReset(void);
static Type_uByte nubLineToolWifiResetCB(void);
static Type_uByte nubLineToolWifiConnectCheck(void);
static Type_uByte nubLineToolWifiConnectCheckCB(void);
static Type_uByte nubLineToolWifiModeSet(void);
static Type_uByte nubLineToolWifiModeSetCB(void);
static Type_uByte nubLineToolMacGet(void);
static Type_uByte nubLineToolMacGetCB(void);
static Type_uByte nubLineToolGNSSGyroDataGet(void);
static Type_uByte nubLineToolGNSSGyroDataGetCB(void);
static Type_uByte nubLineToolGNSSNaviDataGet(void);
static Type_uByte nubLineToolGNSSNaviDataGetCB(void);
static Type_uByte nubLineToolTestSourceOn(void);
static Type_uByte nubLineToolTestSourceOnCB(void);
static Type_uByte nubLineToolTestSourceOff(void);
static Type_uByte nubLineToolTestSourceOffCB(void);
static Type_uByte nubLineToolGetMainTemp(void);
static Type_uByte nubLineToolScreenTouch(void);
static Type_uByte nubLineToolScreenTouchCB(void);
static Type_uByte nubLineToolTPSensorCheck(void);
static Type_uByte nubLineToolTPSensorCheckCB(void);
static Type_uByte nubLineToolMicAppoint(void);      //0317 cxx
static Type_uByte nubLineToolMicAppointCB(void);    //0317 cxx
static Type_uByte nubLineToolTelMute(void);         //0317 cxx
// static Type_uByte nubLineToolTelMuteCB(void);    // delete by cxx on 20220412 for [Iss.B]#57078.
static Type_uByte nubLineToolMicSoundOn(void);        // add L932-L935 by cxx on 20220428 for MIC MODE ON/OFF
static Type_uByte nubLineToolMicSoundOnCB(void);
static Type_uByte nubLineToolMicSoundOff(void);
static Type_uByte nubLineToolMicSoundOffCB(void);
/********************************************************************************************/
/*                           Global variable definition section                             */
/********************************************************************************************/


/********************************************************************************************/
/*                           Function Definition Section                                    */
/********************************************************************************************/
/*************************** LineTool Main Process Table ************************************/
static ElementOfProcTbl_ts const  nstLineToolFMAlignment_Tbl[]={ /* PID_LINETOOL_FMALIGNMENT */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_FM_ALIGNMENT,			STAGE1,			WTID_5000,		C_STAGE3,			WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_FM_ALIGNMENT_CB,		C_STAGE2,		WTID_0,			C_STAGE3,			WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR)
};

static ElementOfProcTbl_ts const nstLineToolAMAlignment_Tbl[]={ /* PID_LINETOOL_AMALIGNMENT */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_AM_ALIGNMENT,			STAGE1,			WTID_5000,		C_STAGE3,			WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_AM_ALIGNMENT_CB,		C_STAGE2,		WTID_0,			C_STAGE3,			WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR)
};

static ElementOfProcTbl_ts const nstLineToolEepWrite_Tbl[]={ /* PID_LINETOOL_EEP_WRITE */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_EEP_WRITE,				C_STAGE1,		WTID_0,			C_STAGE2,			WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR)
};

static ElementOfProcTbl_ts const nstLineToolEepRead_Tbl[]={ /* PID_LINETOOL_EEP_READ */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_EEP_READ,				C_STAGE1,		WTID_0,			C_STAGE2,			WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR)
};

static ElementOfProcTbl_ts const nstLineToolGetCurrentTime_Tbl[]={ /* PID_LINETOOL_GETCURRENTTM */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_CURRENT_TIME_GET,		STAGE1,			WTID_5000,		C_STAGE3,			WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_CURRENT_TIME_GET_CB,	C_STAGE2,		WTID_0,			C_STAGE3,			WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR)
};

static ElementOfProcTbl_ts const nstLineToolFMDirect_Tbl[]={ /* PID_LINETOOL_FMDIRECT */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_FM_DIRECT,				STAGE1,			WTID_5000,		C_STAGE3,			WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_FM_DIRECT_CB,			C_STAGE2,		WTID_0,			C_STAGE3,			WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR)
};

static ElementOfProcTbl_ts const nstLineToolAMDirect_Tbl[]={ /* PID_LINETOOL_AMDIRECT */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_AM_DIRECT,				STAGE1,			WTID_5000,		C_STAGE3,			WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_AM_DIRECT_CB,			C_STAGE2,		WTID_0,			C_STAGE3,			WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR)
};

static ElementOfProcTbl_ts const nstLineToolPowerOff_Tbl[]={ /* PID_LINETOOL_POWEROFF */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_POWEROFF,		    	STAGE1,			WTID_5000,		C_STAGE3,			WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_POWEROFF_CB,			C_STAGE2,		WTID_0,			C_STAGE3,			WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR)
};

static ElementOfProcTbl_ts const nstLineToolPowerOn_Tbl[]={ /* PID_LINETOOL_POWERON */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_POWERON,		    	STAGE1,			WTID_5000,		C_STAGE3,			WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_POWERON_CB,			C_STAGE2,		WTID_0,			C_STAGE3,			WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR)
};

static ElementOfProcTbl_ts const nstLineToolWarningIn_Tbl[]={ /* PID_LINETOOL_WARNINGIN */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_WARNING_IN,	    	STAGE1,			WTID_5000,		C_STAGE3,			WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_WARNING_IN_CB,			C_STAGE2,		WTID_0,			C_STAGE3,			WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR)
};

static ElementOfProcTbl_ts const nstLineToolLEDOn_Tbl[]={ /* PID_LINETOOL_LED_ON */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_LED_ON,	    	    STAGE1,			WTID_5000,		C_STAGE3,			WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_LED_ON_CB,			    C_STAGE2,		WTID_0,			C_STAGE3,			WTID_0),    /*0308 lj add*/
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR)
};

static ElementOfProcTbl_ts const nstLineToolLEDOff_Tbl[]={ /* PID_LINETOOL_LED_OFF */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_LED_OFF,	    	    STAGE1,			WTID_5000,		C_STAGE3,			WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_LED_OFF_CB,			C_STAGE2,		WTID_0,			C_STAGE3,			WTID_0),    /*0308 lj add*/
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR)
};

static ElementOfProcTbl_ts const nstLineToolSourceDirect_Tbl[]={ /* PID_LINETOOL_SOURCE_DIRECT */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_SOURCE_DIRECT,			STAGE1,			WTID_5000,		C_STAGE3,			WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_SOURCE_DIRECT_CB,		C_STAGE2,		WTID_0,			C_STAGE3,			WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR)
};
#if 0
static ElementOfProcTbl_ts const nstLineToolLCDSet_Tbl[]={ /* PID_LINETOOL_LCD_SET */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_LCD_SET,				STAGE1,			WTID_5000,		C_STAGE4,			WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_LCD_SET_CB,		    C_STAGE2,		WTID_0,			C_STAGE4,			WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_LCD_SET_EEP,			C_STAGE3,		WTID_0,			C_STAGE4,			WTID_0),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE04 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR)
};
#else
static ElementOfProcTbl_ts const nstLineToolLCDSet_Tbl[]={ /* PID_LINETOOL_LCD_SET */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_LCD_SET_EMMC,			STAGE1,			WTID_5000,		C_STAGE5,			WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_LCD_SET_EMMC_CB,		C_STAGE2,		WTID_0,			C_STAGE5,			WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_LCD_SET,				STAGE3,			WTID_5000,		C_STAGE5,			WTID_0),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_LCD_SET_CB,		    C_STAGE4,		WTID_0,			C_STAGE5,			WTID_0),
/* STAGE04 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE05 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR)
};
#endif
static ElementOfProcTbl_ts const nstLineToolFlickerAdjust_Tbl[]={ /* PID_LINETOOL_FLICKER_ADJUST */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_FLICKER_MODE_JUDGE,	C_STAGE1,		WTID_0,			C_STAGE5,			WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_FLICKER_ADJUST,		STAGE2,			WTID_5000,		C_STAGE5,			WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_FLICKER_ADJUST_CB,		C_STAGE3,		WTID_0,			C_STAGE5,			WTID_0),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_FLICKER_SET_EEP,		C_STAGE4,		WTID_0,			C_STAGE5,			WTID_0),
/* STAGE04 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE05 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR)
};

static ElementOfProcTbl_ts const nstLineToolVolumeDirect_Tbl[]={ /* PID_LINETOOL_VOLUME_DIRECT */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_VOLUME_DIRECT,			STAGE1,			WTID_5000,		C_STAGE3,			WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_VOLUME_DIRECT_CB,		C_STAGE2,		WTID_0,			C_STAGE3,			WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR)
};

static ElementOfProcTbl_ts const nstLineToolMuteControl_Tbl[]={ /* PID_LINETOOL_MUTE_CONTROL */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_MUTE_CONTROL,			STAGE1,			WTID_5000, 		C_STAGE3,			WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_MUTE_CONTROL_CB,		C_STAGE2,		WTID_0,			C_STAGE3,			WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR)
};

static ElementOfProcTbl_ts const nstLineToolLoudnessSet_Tbl[]={ /* PID_LINETOOL_LOUDNESS_SET */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_LOUDNESS_SET,			STAGE1,			WTID_5000,		C_STAGE3,			WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_LOUDNESS_SET_CB,		C_STAGE2,		WTID_0,			C_STAGE3,			WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR)
};

static ElementOfProcTbl_ts const nstLineToolEQSet_Tbl[]={ /* PID_LINETOOL_EQ_SET */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_EQ_SET,				STAGE1,			WTID_5000,		C_STAGE3,			WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_EQ_SET_CB,				C_STAGE2,		WTID_0,			C_STAGE3,			WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR)
};

static ElementOfProcTbl_ts const nstLineToolMemoryInit_Tbl[]={ /* PID_LINETOOL_MEMORY_INIT */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_MEMORY_INIT,			STAGE1,			WTID_10000,		C_STAGE3,			WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_MEMORY_INIT_CB,		C_STAGE2,		WTID_0,			C_STAGE3,		   	WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,		    WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
};

static ElementOfProcTbl_ts const nstLineToolEepClear_Tbl[]={ /* PID_LINETOOL_EEP_CLEAR */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_EEP_CLEAR,				C_STAGE1,		WTID_0,			C_STAGE2,			WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR)
};

static ElementOfProcTbl_ts const nstLineToolGetLcdTemp_Tbl[]={ /* PID_LINETOOL_LCD_TEMP_GET */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_LCD_TEMP_GET,	        STAGE1,			WTID_5000,		C_STAGE3,			WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_LCD_TEMP_GET_CB,		C_STAGE2,		WTID_0,			C_STAGE3,			WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR)
};

static ElementOfProcTbl_ts const nstLineToolGetMainTemp_Tbl[]={ /* PID_LINETOOL_MAIN_TEMP_GET */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_MAIN_TEMP_GET,		    C_STAGE1,		WTID_0,			C_STAGE2,			WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR)
};

static ElementOfProcTbl_ts const nstLineToolSoftVerRead_Tbl[]={ /* PID_LINETOOL_DA_SOFT_VER_READ */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_SOFT_VER_GET,	        STAGE1,			WTID_5000,		C_STAGE3,			WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_SOFT_VER_GET_CB,		C_STAGE2,		WTID_0,			C_STAGE3,			WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR)
};

// static ElementOfProcTbl_ts const nstLineToolSeekUpDown_Tbl[]={ /* PID_LINETOOL_SEEK_UP_DOWN */                                                       /* 0221 delete lj */
// /* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_SEEK_UP_DOWN,		STAGE1,			WTID_5000,		C_STAGE3,			WTID_0),
// /* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_SEEK_UP_DOWN_CB,		C_STAGE2,		WTID_0,			C_STAGE3,			WTID_0),
// /* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
// /* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR)
// };

static ElementOfProcTbl_ts const nstLineToolIpodTrackChg_Tbl[]={ /* PID_LINETOOL_IPOD_TRACK_CHG */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_IPOD_TRACK_CHG,	    STAGE1,			WTID_5000,		C_STAGE3,			WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_IPOD_TRACK_CHG_CB,		C_STAGE2,		WTID_0,			C_STAGE3,			WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR)
};

static ElementOfProcTbl_ts const nstLineToolUSBTrackChg_Tbl[]={ /* PID_LINETOOL_USB_TRACK_CHG */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_USB_TRACK_CHG,		    STAGE1,			WTID_5000,		C_STAGE3,			WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_USB_TRACK_CHG_CB,		C_STAGE2,		WTID_0,			C_STAGE3,			WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR)
};

static ElementOfProcTbl_ts const nstLineToolCameraEnter_Tbl[]={ /* PID_LINETOOL_CAMERA_ENTER */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_CAMERA_ENTER,		    STAGE1,			WTID_5000,		C_STAGE3,			WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_CAMERA_ENTER_CB,		C_STAGE2,		WTID_0,			C_STAGE3,			WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR)
};

static ElementOfProcTbl_ts const nstLineToolCameraChg_Tbl[]={ /* PID_LINETOOL_CAMERA_CHG */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_CAMERA_CHG,		    STAGE1,			WTID_5000,		C_STAGE3,			WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_CAMERA_CHG_CB,			C_STAGE2,		WTID_0,			C_STAGE3,			WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR)
};

static ElementOfProcTbl_ts const nstLineToolCameraQuit_Tbl[]={ /* PID_LINETOOL_CAMERA_QUIT */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_CAMERA_QUIT,		    STAGE1,			WTID_5000,		C_STAGE3,			WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_CAMERA_QUIT_CB,		C_STAGE2,		WTID_0,			C_STAGE3,			WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR)
};

static ElementOfProcTbl_ts const nstLineToolFanControl_Tbl[]={ /* PID_LINETOOL_FAN_CONTROL */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_FAN_CONTROL,			STAGE1,		    WTID_5000,		C_STAGE3,			WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_FAN_CONTROL_CB,		C_STAGE2,		WTID_0,			C_STAGE3,			WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR)
};

static ElementOfProcTbl_ts const nstLineToolBTCheck_Tbl[]={ /* PID_LINETOOL_BT_CHECK */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_BT_CHECK,			    STAGE1,			WTID_10000,		C_STAGE3,			WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_BT_CHECK_CB,			C_STAGE2,		WTID_0,			C_STAGE3,			WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR)
};

static ElementOfProcTbl_ts const nstLineToolBTAudio1kzSet_Tbl[]={ /* PID_LINETOOL_BT_AUDIO_1KZ_SET */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_BT_AUDIO_1KZ_SET,	    STAGE1,			WTID_5000,		C_STAGE3,			WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_BT_AUDIO_1KZ_SET_CB,	C_STAGE2,		WTID_0,			C_STAGE3,			WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR)
};

static ElementOfProcTbl_ts const nstLineToolUSBAudioChg_Tbl[]={ /* PID_LINETOOL_USB_AUDIO_CHG */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_USB_AUDIO_CHG,		STAGE1,			WTID_5000,		C_STAGE3,			WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_USB_AUDIO_CHG_CB,		C_STAGE2,		WTID_0,			C_STAGE3,			WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR)
};

static ElementOfProcTbl_ts const nstLineToolUSBVideoChg_Tbl[]={ /* PID_LINETOOL_USB_VIDEO_CHG */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_USB_VIDEO_CHG,		STAGE1,			WTID_5000,		C_STAGE3,			WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_USB_VIDEO_CHG_CB,		C_STAGE2,		WTID_0,			C_STAGE3,			WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR)
};

static ElementOfProcTbl_ts const nstLineToolTPCheckEnd_Tbl[]={ /* PID_LINETOOL_TP_CHECK_END */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_TP_CHECK_END,		    STAGE1,			WTID_5000,		C_STAGE3,			WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_TP_CHECK_END_CB,		C_STAGE2,		WTID_0,			C_STAGE3,			WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR)
};

static ElementOfProcTbl_ts const nstLineToolTPCheckStart_Tbl[]={ /* PID_LINETOOL_TP_CHECK_START */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_TP_CHECK_START,	STAGE1,			WTID_5000,		C_STAGE3,			WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_TP_CHECK_START_CB,		C_STAGE2,		WTID_0,			C_STAGE3,			WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR)
};

static ElementOfProcTbl_ts const nstLineToolTPCheck_Tbl[]={ /* PID_LINETOOL_TP_CHECK */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_TP_CHECK,			STAGE1,			WTID_5000,		C_STAGE3,			WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_TP_CHECK_CB,			C_STAGE2,		WTID_0,			C_STAGE3,			WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
};

static ElementOfProcTbl_ts const nstLineToolFlickerModeOn_Tbl[]={ /* PID_LINETOOL_FLICKER_MODE_ON */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_FLC_MODE_ON,		STAGE1,			WTID_5000,		C_STAGE3,			WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_FLC_MODE_ON_CB,	C_STAGE2,		WTID_0,			C_STAGE3,			WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
};

static ElementOfProcTbl_ts const nstLineToolFlickerModeOff_Tbl[]={ /* PID_LINETOOL_FLICKER_MODE_OFF */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_FLC_MODE_OFF,		STAGE1,			WTID_5000,		C_STAGE3,			WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_FLC_MODE_OFF_CB,	C_STAGE2,		WTID_0,			C_STAGE3,			WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
};

static ElementOfProcTbl_ts const nstLineToolMICPathBuild_Tbl[]={ /* PID_LINETOOL_MIC_PATH_BUILD */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_TEST_SOURCE_ON,		STAGE1, 		WTID_5000,		C_STAGE7,			WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_TEST_SOURCE_ON_CB, 	C_STAGE2,		WTID_0, 		C_STAGE7,			WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_MIC_SOUND_ON,		    STAGE3,			WTID_5000,		C_STAGE7,			WTID_0),	//add by cxx on 20220428 for MIC MODE ON
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_MIC_SOUND_ON_CB,		C_STAGE4,		WTID_0,			C_STAGE7,			WTID_0),	//add by cxx on 20220428 for MIC MODE ON
/* STAGE04 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_MIC_PATH_BUILD,		STAGE5,			WTID_5000,		C_STAGE7,			WTID_0),
/* STAGE05 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_MIC_PATH_BUILD_CB,		C_STAGE6,		WTID_0,			C_STAGE7,			WTID_0),
/* STAGE06 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE07 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
};

static ElementOfProcTbl_ts const nstLineToolMICPathCut_Tbl[]={ /* PID_LINETOOL_MIC_PATH_CUT */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_MIC_PATH_CUT,			STAGE1,			WTID_5000,		C_STAGE7,			WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_MIC_PATH_CUT_CB,		C_STAGE2,		WTID_0,			C_STAGE7,			WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_MIC_SOUND_OFF,			STAGE3, 		WTID_5000,		C_STAGE7,			WTID_0),	//add by cxx on 20220428 for MIC MODE OFF
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_MIC_SOUND_OFF_CB,		C_STAGE4,		WTID_0, 		C_STAGE7,			WTID_0),	//add by cxx on 20220428 for MIC MODE OFF
/* STAGE04 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_TEST_SOURCE_OFF,		STAGE5, 		WTID_5000,		C_STAGE7,			WTID_0),
/* STAGE05 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_TEST_SOURCE_OFF_CB,	C_STAGE6,		WTID_0, 		C_STAGE7,			WTID_0),
/* STAGE06 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE07 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
};

static ElementOfProcTbl_ts const nstLineToolDisplayAdjust_Tbl[]={ /* PID_LINETOOL_DISPLAY_ADJUST */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_DISPLAY_ADJUST,		STAGE1,			WTID_5000,		C_STAGE3,			WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_DISPLAY_ADJUST_CB,		C_STAGE2,		WTID_0,			C_STAGE3,			WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
};

static ElementOfProcTbl_ts const nstLineToolL_SecurityOn_Tbl[]={ /* PID_LINETOOL_SECURITY_ON */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_SECURITY_ON,		    C_STAGE1,		WTID_5000,		C_STAGE2,			WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
};
static ElementOfProcTbl_ts const nstLineToolL_SecurityOFF_Tbl[]={ /* PID_LINETOOL_SECURITY_OFF */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_SECURITY_OFF,			C_STAGE1,		WTID_0,		    C_STAGE2,		    WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
};

static ElementOfProcTbl_ts const nstLineToolCarPlayIpodChg_Tbl[]={ /* PID_LINETOOL_CARPLAY_IPOD_CHG */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_CARPLAY_IPOD_CHG,		STAGE1,			WTID_5000,		C_STAGE3,			WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_CARPLAY_IPOD_CHG_CB,	C_STAGE2,		WTID_0,			C_STAGE3,			WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
};

static ElementOfProcTbl_ts const nstLineToolCarPlayUpDown_Tbl[]={ /* PID_LINETOOL_CARPLAY_TRACK */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_CARPLAY_TRACK,		STAGE1,			WTID_5000,		C_STAGE3,			WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_CARPLAY_TRACK_CB,	C_STAGE2,		WTID_0,			C_STAGE3,			WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
};

static ElementOfProcTbl_ts const nstLineToolLanguageIn_Tbl[]={ /* PID_LINETOOL_LANGUAGE_IN */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_LANGUAGE_IN,		    STAGE1,		    WTID_5000,		C_STAGE3,		    WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_LANGUAGE_IN_CB,		C_STAGE2,	    WTID_0,			C_STAGE3,		    WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
};

static ElementOfProcTbl_ts const nstLineToolReadRDSInfo_Tbl[] = /* PID_LINETOOL_READ_RDS_INFO */
{
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RDS_INFO_GET,         STAGE1, 	    WTID_5000,		C_STAGE3,		    WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RDS_INFO_GET_CB,	    C_STAGE2,	    WTID_0, 		C_STAGE3,		    WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH, 	    PE_PEC0,	    WTID_FEVR,		PE_PEC1,		    WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,		    PE_PEC0,	    WTID_FEVR,		PE_PEC1,		    WTID_FEVR),
};

static ElementOfProcTbl_ts const nstLineToolGetRDSLevel_Tbl[] = /* PID_LINETOOL_GET_RDS_LEVEL */
{
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RDS_LEVEL_GET,         STAGE1, 	    WTID_5000,		C_STAGE3,		    WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RDS_LEVEL_GET_CB,	    C_STAGE2,	    WTID_0, 		C_STAGE3,		    WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH, 	    PE_PEC0,	    WTID_FEVR,		PE_PEC1,		    WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,		    PE_PEC0,	    WTID_FEVR,		PE_PEC1,		    WTID_FEVR),
};

static ElementOfProcTbl_ts const nstLineToolGetTunerSM_Tbl[]={ /* PID_LINETOOL_GET_TUNER_SM */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_TUNER_SM_GET,		    STAGE1,		    WTID_5000,		C_STAGE3,		    WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_TUNER_SM_GET_CB,		C_STAGE2,	    WTID_0,			C_STAGE3,		    WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
};

static ElementOfProcTbl_ts const nstLineToolBTLevelDiag_Tbl[] =      /* PID_LINETOOL_BT_LEVEL_DIAG */
{
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_BT_LEVEL_DIAG,         STAGE1,	        WTID_5000,		C_STAGE3,		    WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_BT_LEVEL_DIAG_CB,      C_STAGE2,	    WTID_0, 		C_STAGE3,		    WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH, 	    PE_PEC0,	    WTID_FEVR,		PE_PEC1,		    WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,		    PE_PEC0,	    WTID_FEVR,		PE_PEC1,		    WTID_FEVR),
};

static ElementOfProcTbl_ts const nstLineToolBTInfoClear_Tbl[]={ /* PID_LINETOOL_BT_INFO_CLEAR */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_BT_INFO_CLEAR,		    STAGE1,		    WTID_10000,		C_STAGE3,		    WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_BT_INFO_CLEAR_CB,		C_STAGE2,	    WTID_0,			C_STAGE3,		    WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
};

static ElementOfProcTbl_ts const nstLineToolCameraCheck_Tbl[]={ /* PID_LINETOOL_CAMERA_CHECK */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_CAMERA_CHECK,		    STAGE1,		    WTID_5000,		C_STAGE3,		    WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_CAMERA_CHECK_CB,		C_STAGE2,	    WTID_0,			C_STAGE3,		    WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
};

static ElementOfProcTbl_ts const nstLineToolTPTerminalSet_Tbl[]={ /* PID_LINETOOL_TP_TERMINAL_SET */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_TP_TERMINAL_SET,		STAGE1,         WTID_5000,		C_STAGE3,		    WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_TP_TERMINAL_SET_CB,	C_STAGE2,	    WTID_0,			C_STAGE3,		    WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR)
};

static ElementOfProcTbl_ts const nstLineToolKeyCheck_Tbl[]={ /* PID_LINETOOL_KEY_CHECK */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_KEY_CHECK,				STAGE1,		    WTID_5000,		C_STAGE2,		    WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_KEY_CHECK_CB,			C_STAGE2,	    WTID_0,			C_STAGE3,		    WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR)
};

static ElementOfProcTbl_ts const nstLineToolKeyCheckOnOff_Tbl[]={ /* PID_LINETOOL_KEY_CHECK_ONOFF */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_KEY_CHECK_ONOFF,		STAGE1, 		WTID_5000,		C_STAGE3,		    WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_KEY_CHECK_ONOFF_CB,    C_STAGE2,	    WTID_0,			C_STAGE3,		    WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
};

static ElementOfProcTbl_ts const nstLineToolILLCheck_Tbl[]={ /* PID_LINETOOL_ILL_CHECK */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_ILL_CHECK,	            C_STAGE1,		WTID_0,		    C_STAGE2,		    WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
};

static ElementOfProcTbl_ts const nstLineToolEnglishSet_Tbl[]={ /* PID_LINETOOL_ENGLISH_SET */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_ENGLISH_SET,		    STAGE1,		    WTID_10000,		C_STAGE3,		    WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_ENGLISH_SET_CB,		C_STAGE2,	    WTID_0,			C_STAGE3,		    WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
};

static ElementOfProcTbl_ts const nstLineToolCarPlayOnOff_Tbl[] = /* PID_LINETOOL_CARPLAY_ON_OFF */
{
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_CARPLAY_ON_OFF,         STAGE1, 	    WTID_5000,		C_STAGE3,		    WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_CARPLAY_ON_OFF_CB,	        C_STAGE2,	    WTID_0, 		C_STAGE3,		    WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH, 	    PE_PEC0,        WTID_FEVR,		PE_PEC1,		    WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,		    PE_PEC0,	    WTID_FEVR,		PE_PEC1,		    WTID_FEVR),
};

static ElementOfProcTbl_ts const nstLineToolAAPOnOff_Tbl[] =      /* PID_LINETOOL_AAP_ON_OFF */
{
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_AAP_ON_OFF,            STAGE1,         WTID_5000,		C_STAGE3,		    WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_AAP_ON_OFF_CB,         C_STAGE2,	    WTID_0, 		C_STAGE3,		    WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH, 	    PE_PEC0,	    WTID_FEVR,		PE_PEC1,		    WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,		    PE_PEC0,	    WTID_FEVR,		PE_PEC1,		    WTID_FEVR),
};

static ElementOfProcTbl_ts const nstLineToolSpeedPlus_Tbl[]={ /* PID_LINETOOL_SPEED_PLUS_GET */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_SPEED_PLUS_GET,	    C_STAGE1,		WTID_0,		    C_STAGE2,		    WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
};

static ElementOfProcTbl_ts const nstLineToolWifiSetSSID_Tbl[]={ /* PID_LINETOOL_WIFI_SET_SSID */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,  SPID_WIFI_SSID_SET,               STAGE1,         WTID_5000,      C_STAGE3,           WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,  SPID_WIFI_SSID_SET_CB,            C_STAGE2,       WTID_0,         C_STAGE3,           WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,  SPID_RESULT_FINISH,               PE_PEC0,        WTID_FEVR,      PE_PEC1,            WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,  SPID_RESULT_ERROR,                PE_PEC0,        WTID_FEVR,      PE_PEC1,            WTID_FEVR),
};

static ElementOfProcTbl_ts const nstLineToolWifiSetPassWord_Tbl[]={ /* PID_LINETOOL_WIFI_SET_PASSWORD */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,  SPID_WIFI_PASSWORD_SET,           STAGE1,         WTID_5000,      C_STAGE3,           WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,  SPID_WIFI_PASSWORD_SET_CB,        C_STAGE2,       WTID_0,         C_STAGE3,           WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,  SPID_RESULT_FINISH,               PE_PEC0,        WTID_FEVR,      PE_PEC1,            WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,  SPID_RESULT_ERROR,                PE_PEC0,        WTID_FEVR,      PE_PEC1,            WTID_FEVR),
};

static ElementOfProcTbl_ts const nstLineToolWifiClearInfo_Tbl[]={ /* PID_LINETOOL_WIFI_CLEAR_INFO */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,  SPID_WIFI_CLEAR_INFO,             STAGE1,         WTID_5000,      C_STAGE3,           WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,  SPID_WIFI_CLEAR_INFO_CB,          C_STAGE2,       WTID_0,         C_STAGE3,           WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,  SPID_RESULT_FINISH,               PE_PEC0,        WTID_FEVR,      PE_PEC1,            WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,  SPID_RESULT_ERROR,                PE_PEC0,        WTID_FEVR,      PE_PEC1,            WTID_FEVR),
};

static ElementOfProcTbl_ts const nstLineToolWifiGetConNum_Tbl[]={ /* PID_LINETOOL_WIFI_GETCONNUM */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,  SPID_WIFI_CONNUM_GET,              STAGE1,         WTID_5000,      C_STAGE3,           WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,  SPID_WIFI_CONNUM_GET_CB,           C_STAGE2,       WTID_0,         C_STAGE3,           WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,  SPID_RESULT_FINISH,               PE_PEC0,        WTID_FEVR,      PE_PEC1,            WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,  SPID_RESULT_ERROR,                PE_PEC0,        WTID_FEVR,      PE_PEC1,            WTID_FEVR),
};

static ElementOfProcTbl_ts const nstLineToolWifiReset_Tbl[]={ /* PID_LINETOOL_WIFI_RESET */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,  SPID_WIFI_RESET,                  STAGE1,         WTID_5000,      C_STAGE3,           WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,  SPID_WIFI_RESET_CB,               C_STAGE2,       WTID_0,         C_STAGE3,           WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,  SPID_RESULT_FINISH,               PE_PEC0,        WTID_FEVR,      PE_PEC1,            WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,  SPID_RESULT_ERROR,                PE_PEC0,        WTID_FEVR,      PE_PEC1,            WTID_FEVR),
};

static ElementOfProcTbl_ts const nstLineToolWifiConCheck_Tbl[]={ /* PID_LINETOOL_WIFI_CON_CHECK */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,  SPID_WIFI_CON_CHECK,              STAGE1,         WTID_5000,      C_STAGE3,           WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,  SPID_WIFI_CON_CHECK_CB,           C_STAGE2,       WTID_0,         C_STAGE3,           WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,  SPID_RESULT_FINISH,               PE_PEC0,        WTID_FEVR,      PE_PEC1,            WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,  SPID_RESULT_ERROR,                PE_PEC0,        WTID_FEVR,      PE_PEC1,            WTID_FEVR),
};

static ElementOfProcTbl_ts const nstLineToolWifiSetMode_Tbl[]={ /* PID_LINETOOL_WIFI_SET_MODE */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,  SPID_WIFI_MODE_SET,               STAGE1,         WTID_5000,      C_STAGE3,           WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,  SPID_WIFI_MODE_SET_CB,            C_STAGE2,       WTID_0,         C_STAGE3,           WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,  SPID_RESULT_FINISH,               PE_PEC0,        WTID_FEVR,      PE_PEC1,            WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,  SPID_RESULT_ERROR,                PE_PEC0,        WTID_FEVR,      PE_PEC1,            WTID_FEVR),
};

static ElementOfProcTbl_ts const nstLineToolGetMac_Tbl[]={ /* PID_LINETOOL_GET_MAC */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,  SPID_MAC_GET,                     STAGE1,         WTID_5000,      C_STAGE3,           WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,  SPID_MAC_GET_CB,                  C_STAGE2,       WTID_0,         C_STAGE3,           WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,  SPID_RESULT_FINISH,               PE_PEC0,        WTID_FEVR,      PE_PEC1,            WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,  SPID_RESULT_ERROR,                PE_PEC0,        WTID_FEVR,      PE_PEC1,            WTID_FEVR),
};

static ElementOfProcTbl_ts const nstLineToolGnssGetGyroData_Tbl[]={ /* PID_LINETOOL_GNSS_GET_GYRO */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,  SPID_GNSS_GYRO_GET,               STAGE1,         WTID_5000,      C_STAGE3,           WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,  SPID_GNSS_GYRO_GET_CB,            C_STAGE2,       WTID_0,         C_STAGE3,           WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,  SPID_RESULT_FINISH,               PE_PEC0,        WTID_FEVR,      PE_PEC1,            WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,  SPID_RESULT_ERROR,                PE_PEC0,        WTID_FEVR,      PE_PEC1,            WTID_FEVR),
};

static ElementOfProcTbl_ts const nstLineToolGnssGetNaviData_Tbl[]={ /* PID_LINETOOL_GNSS_GET_NAVI */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,  SPID_GNSS_NAVI_GET,               STAGE1,         WTID_5000,      C_STAGE3,           WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,  SPID_GNSS_NAVI_GET_CB,            C_STAGE2,       WTID_0,         C_STAGE3,           WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,  SPID_RESULT_FINISH,               PE_PEC0,        WTID_FEVR,      PE_PEC1,            WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,  SPID_RESULT_ERROR,                PE_PEC0,        WTID_FEVR,      PE_PEC1,            WTID_FEVR),
};

static ElementOfProcTbl_ts const nstLineToolVariationGet_Tbl[]={ /* PID_LINETOOL_MODE_NAME_GET */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_MODE_NAME_GET,	        C_STAGE1,		WTID_0,		    C_STAGE2,		    WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
};
// static ElementOfProcTbl_ts const nstLineToolVariationGet_Tbl[]={ /* PID_LINETOOL_MODE_NAME_GET */
// /* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,  SPID_MODE_NAME_GET,               STAGE1,         WTID_5000,      C_STAGE3,           WTID_0),
// /* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,  SPID_MODE_NAME_GET_CB,            C_STAGE2,       WTID_0,         C_STAGE3,           WTID_0),
// /* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,  SPID_RESULT_FINISH,               PE_PEC0,        WTID_FEVR,      PE_PEC1,            WTID_FEVR),
// /* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,  SPID_RESULT_ERROR,                PE_PEC0,        WTID_FEVR,      PE_PEC1,            WTID_FEVR),
// };

static ElementOfProcTbl_ts const nstLineToolL_ScreenTouch_Tbl[]={ /* PID_LINETOOL_SCREENTOUCH */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_SCREENTOUCH,		    STAGE1,		    WTID_5000,		C_STAGE3,		    WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_SCREENTOUCH_CB,		C_STAGE2,	    WTID_0,			C_STAGE3,		    WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
};

static ElementOfProcTbl_ts const nstLineTool_TPSensorCheck_Tbl[]={ /* PID_LINETOOL_TP_SENSOR_CHECK */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_TPSENSORCHECK,		    STAGE1,		    WTID_5000,		C_STAGE3,		    WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_TPSENSORCHECK_CB,		C_STAGE2,	    WTID_0,			C_STAGE3,		    WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
};

static ElementOfProcTbl_ts const nstLineTool_MicAppoint_Tbl[]={ /* PID_LINETOOL_MICAPPOINT */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_MICAPPOINT,		    STAGE1,		    WTID_5000,		C_STAGE3,		    WTID_0),
/* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_MICAPPOINT_CB,		    C_STAGE2,	    WTID_0,			C_STAGE3,		    WTID_0),
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
};
static ElementOfProcTbl_ts const nstLineTool_TelMute_Tbl[]={ /* PID_LINETOOL_TELMUTE */
/* STAGE00 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_TEL_MUTE,		        C_STAGE1,		WTID_0,		    C_STAGE2,		    WTID_0), // chaged by cxx on 20220412 for [Iss.B]#57078. */
// /* STAGE01 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_TEL_MUTE_CB,		    C_STAGE2,	    WTID_0,			C_STAGE3,		    WTID_0), // delete by cxx on 20220412 for [Iss.B]#57078. */
/* STAGE02 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_FINISH,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
/* STAGE03 */PE_MAKE_PROC_TABLE(PE_NO_CHILD,		SPID_RESULT_ERROR,			PE_PEC0,		WTID_FEVR,		PE_PEC1,			WTID_FEVR),
};
/*************************** LineTool Main Process ******************************************/
static ElementOfProcTbl_ts const (*const nstLineToolMainProc_Tbl[PID_PROC_MAX]) = {
/* 000 */ nstLineToolPowerOff_Tbl,            /*  PID_LINETOOL_POWEROFF  */
/* 001 */ nstLineToolPowerOn_Tbl,             /*  PID_LINETOOL_POWERON  */
/* 002 */ nstLineToolWarningIn_Tbl,           /*  PID_LINETOOL_WARNING_IN  */
/* 003 */ nstLineToolLanguageIn_Tbl,          /*  PID_LINETOOL_LANGUAGE_IN  */
/* 004 */ nstLineToolSourceDirect_Tbl,        /*  PID_LINETOOL_SOURCE_DIRECT  */
/* 005 */ nstLineToolVolumeDirect_Tbl,        /*  PID_LINETOOL_VOLUME_DIRECT  */
/* 006 */ nstLineToolSoftVerRead_Tbl,         /*  PID_LINETOOL_SOFT_VER_GET  */
/* 007 */ nstLineToolMuteControl_Tbl,         /*  PID_LINETOOL_MUTE_CONTROL  */
/* 008 */ nstLineToolGetMainTemp_Tbl,         /*  PID_LINETOOL_MAIN_TEMP_GET  */
/* 009 */ nstLineToolLoudnessSet_Tbl,         /*  PID_LINETOOL_LOUDNESS_SET  */
/* 010 */ nstLineToolFanControl_Tbl,          /*  PID_LINETOOL_FAN_CONTROL  */
/* 011 */ nstLineToolEQSet_Tbl,               /*  PID_LINETOOL_EQ_SET  */
/* 012 */ nstLineToolFMDirect_Tbl,            /*  PID_LINETOOL_FM_DIRECT  */
/* 013 */ nstLineToolAMDirect_Tbl,            /*  PID_LINETOOL_AM_DIRECT  */
/*nstLineToolSeekUpDown_Tbl,*/      /*  PID_LINETOOL_SEEK_UP_DOWN  */     /* 0221 delete lj */
/* 014 */ nstLineToolFMAlignment_Tbl,         /*  PID_LINETOOL_FM_ALIGNMENT  */
/* 015 */ nstLineToolAMAlignment_Tbl,         /*  PID_LINETOOL_AM_ALIGNMENT  */
/* 016 */ nstLineToolReadRDSInfo_Tbl,         /*  PID_LINETOOL_RDS_INFO_GET  */
/* 017 */ nstLineToolGetRDSLevel_Tbl,         /*  PID_LINETOOL_RDS_LEVEL_GET  */
/* 018 */ nstLineToolGetTunerSM_Tbl,          /*  PID_LINETOOL_TUNER_SM_GET  */
/* 019 */ nstLineToolIpodTrackChg_Tbl,        /*  PID_LINETOOL_IPOD_TRACK_CHG  */
/* 020 */ nstLineToolUSBTrackChg_Tbl,         /*  PID_LINETOOL_USB_TRACK_CHG  */
/* 021 */ nstLineToolUSBAudioChg_Tbl,         /*  PID_LINETOOL_USB_AUDIO_CHG  */
/* 022 */ nstLineToolUSBVideoChg_Tbl,         /*  PID_LINETOOL_USB_VIDEO_CHG  */
/* 023 */ nstLineToolBTCheck_Tbl,             /*  PID_LINETOOL_BT_CHECK  */
/* 024 */ nstLineToolBTAudio1kzSet_Tbl,       /*  PID_LINETOOL_BT_AUDIO_1KZ_SET  */
/*  PID_LINETOOL_HCIENABLE_CHG  */
/* 025 */ nstLineToolBTLevelDiag_Tbl,         /*  PID_LINETOOL_BT_LEVEL_DIAG  */
/* 026 */ nstLineToolBTInfoClear_Tbl,         /*  PID_LINETOOL_BT_INFO_CLEAR  */
/* 027 */ nstLineToolCameraEnter_Tbl,         /*  PID_LINETOOL_CAMERA_ENTER  */
/* 028 */ nstLineToolCameraChg_Tbl,           /*  PID_LINETOOL_CAMERA_CHG  */
/* 029 */ nstLineToolCameraQuit_Tbl,          /*  PID_LINETOOL_CAMERA_QUIT  */
/* 030 */ nstLineToolCameraCheck_Tbl,         /*  PID_LINETOOL_CAMERA_CHECK  */
/* 031 */ nstLineToolEepWrite_Tbl,            /*  PID_LINETOOL_EEP_WRITE  */
/* 032 */ nstLineToolEepRead_Tbl,             /*  PID_LINETOOL_EEP_READ  */
/* 033 */ nstLineToolEepClear_Tbl,            /*  PID_LINETOOL_EEP_CLEAR  */
/* 034 */ nstLineToolMemoryInit_Tbl,          /*  PID_LINETOOL_EEP_INIT  */
/* 035 */ nstLineToolDisplayAdjust_Tbl,       /*  PID_LINETOOL_DISPLAY_ADJUST  */
/* 036 */ nstLineToolFlickerAdjust_Tbl,       /*  PID_LINETOOL_FLICKER_ADJUST  */
/* 037 */ nstLineToolFlickerModeOn_Tbl,       /*  PID_LINETOOL_FLC_MODE_ON  */
/* 038 */ nstLineToolFlickerModeOff_Tbl,      /*  PID_LINETOOL_FLC_MODE_OFF  */
/* 039 */ nstLineToolGetCurrentTime_Tbl,      /*  PID_LINETOOL_CURRENT_TIME_GET  */
/* 040 */ nstLineToolTPTerminalSet_Tbl,       /*  PID_LINETOOL_TP_TERMINAL_SET  */
/* 041 */ nstLineToolLEDOn_Tbl,               /*  PID_LINETOOL_LED_ON  */
/* 042 */ nstLineToolLEDOff_Tbl,              /*  PID_LINETOOL_LED_OFF  */
/* 043 */ nstLineToolLCDSet_Tbl,              /*  PID_LINETOOL_LCD_SET  */
/* 044 */ nstLineToolTPCheckEnd_Tbl,          /*  PID_LINETOOL_TP_CHECK_END  */
/* 045 */ nstLineToolTPCheckStart_Tbl,        /*  PID_LINETOOL_TP_CHECK_START  */
/* 046 */ nstLineToolTPCheck_Tbl,             /*  PID_LINETOOL_TP_CHECK  */
/* 047 */ nstLineToolKeyCheck_Tbl,            /*  PID_LINETOOL_KEY_CHECK  */
/* 048 */ nstLineToolKeyCheckOnOff_Tbl,       /*  PID_LINETOOL_KEY_CHECK_ONOFF  */
/* 049 */ nstLineToolMICPathBuild_Tbl,        /*  PID_LINETOOL_MIC_PATH_BUILD  */
/* 050 */ nstLineToolMICPathCut_Tbl,          /*  PID_LINETOOL_MIC_PATH_CUT  */
/* 051 */ nstLineToolILLCheck_Tbl,            /*  PID_LINETOOL_ILL_CHECK  */
/* 052 */ nstLineToolEnglishSet_Tbl,          /*  PID_LINETOOL_ENGLISH_SET  */
/* 053 */ nstLineToolCarPlayIpodChg_Tbl,      /*  PID_LINETOOL_CARPLAY_IPOD_CHG  */
/* 054 */ nstLineToolVariationGet_Tbl,        /*  PID_LINETOOL_MODE_NAME_GET  */
/* 055 */ nstLineToolCarPlayUpDown_Tbl,       /*  PID_LINETOOL_CARPLAY_TRACK  */
/* 056 */ nstLineToolCarPlayOnOff_Tbl,        /*  PID_LINETOOL_CARPLAY_ON_OFF  */
/* 057 */ nstLineToolAAPOnOff_Tbl,            /*  PID_LINETOOL_AAP_ON_OFF  */
/* 058 */ nstLineToolSpeedPlus_Tbl,           /*  PID_LINETOOL_SPEED_PLUS_GET  */
/* 059 */ nstLineToolWifiSetSSID_Tbl,         /*  PID_LINETOOL_WIFI_SSID_SET  */
/* 060 */ nstLineToolWifiSetPassWord_Tbl,     /*  PID_LINETOOL_WIFI_PASSWORD_SET  */
/* 061 */ nstLineToolWifiClearInfo_Tbl,       /*  PID_LINETOOL_WIFI_CLEAR_INFO  */
/* 062 */ nstLineToolWifiGetConNum_Tbl,       /*  PID_LINETOOL_WIFI_CONNUM_GET  */
/* 063 */ nstLineToolWifiReset_Tbl,           /*  PID_LINETOOL_WIFI_RESET  */
/* 064 */ nstLineToolWifiConCheck_Tbl,        /*  PID_LINETOOL_WIFI_CON_CHECK  */
/* 065 */ nstLineToolWifiSetMode_Tbl,         /*  PID_LINETOOL_WIFI_MODE_SET  */
/* 066 */ nstLineToolGetMac_Tbl,              /*  PID_LINETOOL_MAC_GET  */
/* 067 */ nstLineToolGnssGetGyroData_Tbl,     /*  PID_LINETOOL_GNSS_GYRO_GET  */
/* 068 */ nstLineToolGnssGetNaviData_Tbl,     /*  PID_LINETOOL_GNSS_NAVI_GET  */
/* 069 */ nstLineToolGetLcdTemp_Tbl,          /*  PID_LINETOOL_LCD_TEMP_GET  */
/* 070 */ nstLineToolL_SecurityOn_Tbl,        /*  PID_LINETOOL_SECURITY_ON  */
/* 071 */ nstLineToolL_SecurityOFF_Tbl,       /*  PID_LINETOOL_SECURITY_OFF  */
/*  PID_LINETOOL_WIFI_TETHER_SET  */
/* 072 */ nstLineToolL_ScreenTouch_Tbl,       /*  PID_LINETOOL_SCREENTOUCH  */
/* 073 */ nstLineTool_TPSensorCheck_Tbl,      /*  PID_LINETOOL_TP_SENSOR_CHECK  */
/* 074 */ nstLineTool_MicAppoint_Tbl,         /*  PID_LINETOOL_MICAPPOINT  */   //0317 cxx
/* 075 */ nstLineTool_TelMute_Tbl,            /*  PID_LINETOOL_TELMUTE  */      //0317 cxx
};


/*************************** LineTool Main SubProcess ***************************************/
static Type_uByte (*const nstLineToolSubProc_Tbl[SPID_PROC_MAX])(void) = {
/* 000 */ nubLineToolResFinish,           /*	SPID_RESULT_FINISH	 */
/* 001 */ nubLineToolResError,            /*	SPID_RESULT_ERROR	 */
/* 002 */ nubLineToolPowerOffReq,         /*	SPID_POWEROFF	 */
/* 003 */ nubLineToolPowerOffCB,          /*	SPID_POWEROFF_CB	 */
/* 004 */ nubLineToolPowerOnReq,          /*	SPID_POWERON	 */
/* 005 */ nubLineToolPowerOnCB,           /*	SPID_POWERON_CB	 */
/* 006 */ nubLineToolWarningInReq,        /*	SPID_WARNING_IN	 */
/* 007 */ nubLineToolWarningInCB,         /*	SPID_WARNING_IN_CB	 */
/* 008 */ nubLineToolLanguageIn,          /*	SPID_LANGUAGE_IN	 */
/* 009 */ nubLineToolLanguageInCB,        /*	SPID_LANGUAGE_IN_CB	 */
/* 010 */ nubLineToolSourceDirect,        /*	SPID_SOURCE_DIRECT	 */
/* 011 */ nubLineToolSourceDirectCB,      /*	SPID_SOURCE_DIRECT_CB	 */
/* 012 */ nubLineToolVolumeDirect,        /*	SPID_VOLUME_DIRECT	 */
/* 013 */ nubLineToolVolumeDirectCB,      /*	SPID_VOLUME_DIRECT_CB	 */
/* 014 */ nubLineToolGetVerReq,           /*	SPID_SOFT_VER_GET	 */
/* 015 */ nubLineToolGetVerCB,            /*	SPID_SOFT_VER_GET_CB	 */
/* 016 */ nubLineToolMuteControl,         /*	SPID_MUTE_CONTROL	 */
/* 017 */ nubLineToolMuteControlCB,       /*	SPID_MUTE_CONTROL_CB	 */
/* 018 */ nubLineToolGetLcdTemp,          /*	SPID_LCD_TEMP_GET	 */
/* 019 */ nubLineToolGetLcdTempCB,        /*	SPID_LCD_TEMP_GET_CB	 */
/* 020 */ nubLineToolLoudnessSet,         /*	SPID_LOUDNESS_SET	 */
/* 021 */ nubLineToolLoudnessSetCB,       /*	SPID_LOUDNESS_SET_CB	 */
/* 022 */ nubLineToolFanControl,          /*	SPID_FAN_CONTROL	 */
/* 023 */ nubLineToolFanControlCB,        /*	SPID_FAN_CONTROL_CB	 */
/* 024 */ nubLineToolEQSet,               /*	SPID_EQ_SET	 */
/* 025 */ nubLineToolEQSetCB,             /*	SPID_EQ_SET_CB	 */
/* 026 */ nubLineToolFMDirect,            /*	SPID_FM_DIRECT	 */
/* 027 */ nubLineToolFMDirectCB,          /*	SPID_FM_DIRECT_CB	 */
/* 028 */ nubLineToolAMDirect,            /*	SPID_AM_DIRECT	 */
/* 029 */ nubLineToolAMDirectCB,          /*	SPID_AM_DIRECT_CB	 */
/*nubLineToolSeekUpDownReq,*/   /*	SPID_SEEK_UP_DOWN	 */    /* 0221 delete lj */
/*nubLineToolSeekUpDownCB, */   /*	SPID_SEEK_UP_DOWN_CB	 *//* 0221 delete lj */
/* 030 */ nubLineToolFmAlignment,         /*	SPID_FM_ALIGNMENT	 */
/* 031 */ nubLineToolFmAlignmentCB,       /*	SPID_FM_ALIGNMENT_CB	 */
/* 032 */ nubLineToolAmAlignment,         /*	SPID_AM_ALIGNMENT	 */
/* 033 */ nubLineToolAmAlignmentCB,       /*	SPID_AM_ALIGNMENT_CB	 */
/* 034 */ nubLineToolReadRDSInfo,         /*	SPID_RDS_INFO_GET	 */
/* 035 */ nubLineToolReadRDSInfoCB,       /*	SPID_RDS_INFO_GET_CB	 */
/* 036 */ nubLineToolGetRDSLevel,         /*	SPID_RDS_LEVEL_GET	 */
/* 037 */ nubLineToolGetRDSLevelCB,       /*	SPID_RDS_LEVEL_GET_CB	 */
/* 038 */ nubLineToolGetTunerSM,          /*	SPID_TUNER_SM_GET	 */
/* 039 */ nubLineToolGetTunerSMCB,        /*	SPID_TUNER_SM_GET_CB	 */
/* 040 */ nubLineToolIpodTrackChgReq,     /*	SPID_IPOD_TRACK_CHG	 */
/* 041 */ nubLineToolIpodTrackChgCB,      /*	SPID_IPOD_TRACK_CHG_CB	 */
/* 042 */ nubLineToolUSBTrackChgReq,      /*	SPID_USB_TRACK_CHG	 */
/* 043 */ nubLineToolUSBTrackChgCB,       /*	SPID_USB_TRACK_CHG_CB	 */
/* 044 */ nubLineToolUSBAudioChgReq,      /*	SPID_USB_AUDIO_CHG	 */
/* 045 */ nubLineToolUSBAudioChgCB,       /*	SPID_USB_AUDIO_CHG_CB	 */
/* 046 */ nubLineToolUSBVideoChgReq,      /*	SPID_USB_VIDEO_CHG	 */
/* 047 */ nubLineToolUSBVideoChgCB,       /*	SPID_USB_VIDEO_CHG_CB	 */
/* 048 */ nubLineToolBTCheckReq,          /*	SPID_BT_CHECK	 */
/* 049 */ nubLineToolBTCheckCB,           /*	SPID_BT_CHECK_CB	 */
/* 050 */ nubLineToolBTAudio1kzSetReq,    /*	SPID_BT_AUDIO_1KZ_SET	 */
/* 051 */ nubLineToolBTAudio1kzSetCB,     /*	SPID_BT_AUDIO_1KZ_SET_CB	 */
//*	SPID_HCIENABLE_CHG	 */
//*	SPID_HCIENABLE_CHG_CB	 */
/* 052 */ nubLineToolBTLevelDiag,         /*	SPID_BT_LEVEL_DIAG	 */
/* 053 */ nubLineToolBTLevelDiagCB,       /*	SPID_BT_LEVEL_DIAG_CB	 */
/* 054 */ nubLineToolBTInfoClear,         /*	SPID_BT_INFO_CLEAR	 */
/* 055 */ nubLineToolBTInfoClearCB,       /*	SPID_BT_INFO_CLEAR_CB	 */
/* 056 */ nubLineToolCameraEnterReq,      /*	SPID_CAMERA_ENTER	 */
/* 057 */ nubLineToolCameraEnterCB,       /*	SPID_CAMERA_ENTER_CB	 */
/* 058 */ nubLineToolCameraChgReq,        /*	SPID_CAMERA_CHG	 */
/* 059 */ nubLineToolCameraChgCB,         /*	SPID_CAMERA_CHG_CB	 */
/* 060 */ nubLineToolCameraQuitReq,       /*	SPID_CAMERA_QUIT	 */
/* 061 */ nubLineToolCameraQuitCB,        /*	SPID_CAMERA_QUIT_CB	 */
/* 062 */ nubLineToolCameraCheck,         /*	SPID_CAMERA_CHECK	 */
/* 063 */ nubLineToolCameraCheckCB,       /*	SPID_CAMERA_CHECK_CB	 */
/* 064 */ nubLineToolEepWrite,            /*	SPID_EEP_WRITE	 */
/* 065 */ nubLineToolEepRead,             /*	SPID_EEP_READ	 */
/* 066 */ nubLineToolEepClear,            /*	SPID_EEP_CLEAR	 */
/* 067 */ nubLineToolMemoryInit,          /*	SPID_MEMORY_INIT	 */
/* 068 */ nubLineToolMemoryInitCB,        /*	SPID_MEMORY_INIT_CB	 */
/* 069 */ nubLineToolDisplayAdjust,       /*	SPID_DISPLAY_ADJUST	 */
/* 070 */ nubLineToolDisplayAdjustCB,     /*	SPID_DISPLAY_ADJUST_CB	 */
/* 071 */ nubLineToolFlickerModeJudge,    /*	SPID_FLICKER_MODE_JUDGE	 */
/* 072 */ nubLineToolFlickerAdjust,       /*	SPID_FLICKER_ADJUST	 */
/* 073 */ nubLineToolFlickerAdjustCB,     /*	SPID_FLICKER_ADJUST_CB	 */
/* 074 */ nubLineToolFlickerSetEep,       /*	SPID_FLICKER_SET_EEP	 */
/* 075 */ nubLineToolFlickerModeOn,       /*	SPID_FLC_MODE_ON	 */
/* 076 */ nubLineToolFlickerModeOnCB,     /*	SPID_FLC_MODE_ON_CB	 */
/* 077 */ nubLineToolFlickerModeOff,      /*	SPID_FLC_MODE_OFF	 */
/* 078 */ nubLineToolFlickerModeOffCB,    /*	SPID_FLC_MODE_OFF_CB	 */
/* 079 */ nubLineToolGetCurrentTime,      /*	SPID_CURRENT_TIME_GET	 */
/* 080 */ nubLineToolGetCurrentTimeCB,    /*	SPID_CURRENT_TIME_GET_CB	 */
/* 081 */ nubLineToolTPTerminalSet,       /*	SPID_TP_TERMINAL_SET	 */
/* 082 */ nubLineToolTPTerminalSetCB,     /*	SPID_TP_TERMINAL_SET_CB	 */
/* 083 */ nubLineToolLEDOn,               /*	SPID_LED_ON	 */
/* 084 */ nubLineToolLEDOnCB,             /*	SPID_LED_ON_CB	 */
/* 085 */ nubLineToolLEDOff,              /*	SPID_LED_OFF	 */
/* 086 */ nubLineToolLEDOffCB,            /*	SPID_LED_OFF_CB	 */
/* 087 */ nubLineToolLCDSetEMMC,          /*	SPID_LCD_SET_EMMC	 */
/* 088 */ nubLineToolLCDSetEMMCCB,        /*	SPID_LCD_SET_EMMC_CB	 */
/* 087 */ nubLineToolLCDSet,              /*	SPID_LCD_SET	 */
/* 088 */ nubLineToolLCDSetCB,            /*	SPID_LCD_SET_CB	 */
/* 089 */ nubLineToolLCDSetEep,           /*	SPID_LCD_SET_EEP	 */
/* 090 */ nubLineToolTPCheckEndReq,       /*	SPID_TP_CHECK_END	 */
/* 091 */ nubLineToolTPCheckEndCB,        /*	SPID_TP_CHECK_END_CB	 */
/* 092 */ nubLineToolTPCheckStartReq,     /*	SPID_TP_CHECK_START	 */
/* 093 */ nubLineToolTPCheckStartCB,      /*	SPID_TP_CHECK_START_CB	 */
/* 094 */ nubLineToolTPCheckReq,          /*	SPID_TP_CHECK	 */
/* 095 */ nubLineToolTPCheckCB,           /*	SPID_TP_CHECK_CB	 */
/* 096 */ nubLineToolKeyCheck,            /*	SPID_KEY_CHECK	 */
/* 097 */ nubLineToolKeyCheckCB,          /*	SPID_KEY_CHECK_CB	 */
/* 098 */ nubLineToolKeyCheckOnOff,       /*	SPID_KEY_CHECK_ONOFF	 */
/* 099 */ nubLineToolKeyCheckOnOffCB,     /*	SPID_KEY_CHECK_ONOFF_CB	 */
/* 100 */ nubLineToolMICPathBuild,        /*	SPID_MIC_PATH_BUILD	 */
/* 101 */ nubLineToolMICPathBuildCB,      /*	SPID_MIC_PATH_BUILD_CB	 */
/* 102 */ nubLineToolMICPathCut,          /*	SPID_MIC_PATH_CUT	 */
/* 103 */ nubLineToolMICPathCutCB,        /*	SPID_MIC_PATH_CUT_CB	 */
/* 104 */ nubLineToolILLCheck,            /*	SPID_ILL_CHECK	 */
/* 105 */ nubLineToolEnglishSet,          /*	SPID_ENGLISH_SET	 */
/* 106 */ nubLineToolEnglishSetCB,        /*	SPID_ENGLISH_SET_CB	 */
/* 107 */ nubLineToolIpodCarplayChg,      /*	SPID_CARPLAY_IPOD_CHG	 */
/* 108 */ nubLineToolIpodCarplayChgCB,    /*	SPID_CARPLAY_IPOD_CHG_CB	 */
/* 109 */ nubLineToolVariationGet,        /*	SPID_MODE_NAME_GET	 */
/*nubLineToolVariationGetCB,*/      /*	SPID_MODE_NAME_GET_CB	 */
/* 110 */ nubLineToolCarplayTrack,        /*	SPID_CARPLAY_TRACK	 */
/* 111 */ nubLineToolCarplayTrackCB,      /*	SPID_CARPLAY_TRACK_CB	 */
/* 112 */ nubLineToolCarplayOnOff,        /*	SPID_CARPLAY_ON_OFF	 */
/* 113 */ nubLineToolCarplayOnOffCB,      /*	SPID_CARPLAY_ON_OFF_CB	 */
/* 114 */ nubLineToolAAPOnOff,            /*	SPID_AAP_ON_OFF	 */
/* 115 */ nubLineToolAAPOnOffCB,          /*	SPID_AAP_ON_OFF_CB	 */
/* 116 */ nubLineToolSpeedPlus,           /*	SPID_SPEED_PLUS_GET	 */
/* 117 */ nubLineToolWifiSSIDSet,         /*	SPID_WIFI_SSID_SET	 */
/* 118 */ nubLineToolWifiSSIDSetCB,       /*	SPID_WIFI_SSID_SET_CB	 */
/* 119 */ nubLineToolWifiPassWordSet,     /*	SPID_WIFI_PASSWORD_SET	 */
/* 120 */ nubLineToolWifiPassWordSetCB,   /*	SPID_WIFI_PASSWORD_SET_CB	 */
/* 121 */ nubLineToolWifiClearInfo,       /*	SPID_WIFI_CLEAR_INFO	 */
/* 122 */ nubLineToolWifiClearInfoCB,     /*	SPID_WIFI_CLEAR_INFO_CB	 */
/* 123 */ nubLineToolWifiConnNumGet,      /*	SPID_WIFI_CONNUM_GET	 */
/* 124 */ nubLineToolWifiConnNumGetCB,    /*	SPID_WIFI_CONNUM_GET_CB	 */
/* 125 */ nubLineToolWifiReset,           /*	SPID_WIFI_RESET	 */
/* 126 */ nubLineToolWifiResetCB,         /*	SPID_WIFI_RESET_CB	 */
/* 127 */ nubLineToolWifiConnectCheck,    /*	SPID_WIFI_CON_CHECK	 */
/* 128 */ nubLineToolWifiConnectCheckCB,  /*	SPID_WIFI_CON_CHECK_CB	 */
/* 129 */ nubLineToolWifiModeSet,         /*	SPID_WIFI_MODE_SET	 */
/* 130 */ nubLineToolWifiModeSetCB,       /*	SPID_WIFI_MODE_SET_CB	 */
/* 131 */ nubLineToolMacGet,              /*	SPID_MAC_GET	 */
/* 132 */ nubLineToolMacGetCB,            /*	SPID_MAC_GET_CB	 */
/* 133 */ nubLineToolGNSSGyroDataGet,     /*	SPID_GNSS_GYRO_GET	 */
/* 134 */ nubLineToolGNSSGyroDataGetCB,   /*	SPID_GNSS_GYRO_GET_CB	 */
/* 135 */ nubLineToolGNSSNaviDataGet,     /*	SPID_GNSS_NAVI_GET	 */
/* 136 */ nubLineToolGNSSNaviDataGetCB,   /*	SPID_GNSS_NAVI_GET_CB	 */
/* 137 */ nubLineToolSecurityOn,          /*	SPID_SECURITY_ON	 */
/* 138 */ nubLineToolSecurityOff,         /*	SPID_SECURITY_OFF	 */
/* 139 */ nubLineToolTestSourceOn,		  /*	SPID_TEST_SOURCE_ON	 */
/* 140 */ nubLineToolTestSourceOnCB,	  /*	SPID_TEST_SOURCE_ON_CB	 */
/* 141 */ nubLineToolTestSourceOff,		  /*	SPID_TEST_SOURCE_OFF	 */
/* 142 */ nubLineToolTestSourceOffCB,     /*	SPID_TEST_SOURCE_OFF_CB	 */
/* 143 */ nubLineToolGetMainTemp,         /*	SPID_MAIN_TEMP_GET	 */
/*	SPID_WIFI_TETHING_SET	 */
/*	SPID_WIFI_TETHING_SET_CB	 */
/* 144 */ nubLineToolScreenTouch,         /*	SPID_SCREENTOUCH	 */
/* 145 */ nubLineToolScreenTouchCB,       /*	SPID_SCREENTOUCH_CB	 */
/* 146 */ nubLineToolTPSensorCheck,       /*	SPID_TPSENSORCHECK	 */
/* 147 */ nubLineToolTPSensorCheckCB,     /*	SPID_TPSENSORCHECK_CB	 */
/* 148 */ nubLineToolMicAppoint,          /*	SPID_MICAPPOINT	 */	//0317 cxx
/* 149 */ nubLineToolMicAppointCB,        /*	SPID_MICAPPOINT_CB	 */
/* 150 */ nubLineToolTelMute,             /*	SPID_TEL_MUTE	 */
/*	SPID_TEL_MUTE_CB	 */   // delete by cxx on 20220412 for [Iss.B]#57078. */
/* 151 */nubLineToolMicSoundOn,           /*	SPID_MIC_SOUND_ON	     */		//add L1743-L1746 by cxx on 20220428 for MIC MODE ON/OFF
/* 152 */nubLineToolMicSoundOnCB,         /*	SPID_MIC_SOUND_ON_CB	 */
/* 153 */nubLineToolMicSoundOff,          /*	SPID_MIC_SOUND_OFF	     */
/* 154 */nubLineToolMicSoundOffCB,        /*	SPID_MIC_SOUND_OFF_CB	 */
};


/*********************************************************************************************
*  function                 : wvdLineToolAppInit
*  date                     : 2016.06.01
*  autho                    : liujie
*  description              : LineToolApp initial
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
void wvdLineToolAppInit(void)
{
    nubLineToolTPMode = LINETOOL_TP_MODE_OFF;
    nubLineToolKeyMode = LINETOOL_KEY_MODE_OFF;
    nubLineToolCameraState = LINETOOL_CAMERA_OFF;
    nubLineToolMICPathState = LINETOOL_MIC_PATH_OFF;
    nuwLineToolStartTime = LINETOOL_NUM_0;
	nubLineToolMode = LINETOOL_MODE_OFF;
}

/*********************************************************************************************
*  function                 : wvdLineToolDrvInit
*  date                     : 2021.01.11
*  autho                    : liujie
*  description              : LineToolDrv initial
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
void wvdLineToolDrvInit(void) {
    /* Initialize global variables */
    nvdLineToolHalHandle = STD_NULL;

    wstLineToolDrvVar.ubDataRecLength = LINETOOL_NUM_0;
    wvdGen_MemSet(wstLineToolDrvVar.ubRevData_a, LINETOOL_NUM_0, sizeof(wstLineToolDrvVar.ubRevData_a));
    wstLineToolDrvVar.ubSaveData_p = wstLineToolDrvVar.ubRevData_a;
    nvdLineToolHalHandle = HAL_Open(HAL_LINETOOL, HAL_LINETOOL_UART);
	HAL_SetCallbackFunction(nvdLineToolHalHandle,(pfCallback)wvdLineToolUartInt);
    // (void)wubLineToolUartOpen(LINETOOL_UART_CH, LINETOOL_UART_BAUD, STD_FALSE, nvdLineToolUartInt);
}

/*********************************************************************************************
*  function                 : wvdLineToolDrvUnInit
*  date                     : 2021.01.11
*  autho                    : liujie
*  description              : LineToolDrv initial
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
void wvdLineToolDrvUnInit(void)
{
    /* Initialize global variables */
    wstLineToolDrvVar.ubDataRecLength = LINETOOL_NUM_0;
    wvdGen_MemSet(wstLineToolDrvVar.ubRevData_a, LINETOOL_NUM_0, sizeof(wstLineToolDrvVar.ubRevData_a));
    wstLineToolDrvVar.ubSaveData_p = wstLineToolDrvVar.ubRevData_a;
    // wubLineToolUartClose(LINETOOL_UART_CH);
    if (OS_NULL != nvdLineToolHalHandle) {
        HAL_Close(nvdLineToolHalHandle);
    }
}


/********************************************************************************************/
/** \function       nvdLineToolUartInt
 *  \date           2010/08/11
 *  \author         Yoshinori Iihoshi
 *  \description    RX interrupt handler of UART0
 *
 *  <!------------- Argument Code ----------------------------------------------------------->
 *
 *  <!------------- Return Code ------------------------------------------------------------->
 *  \return         OS_RETURN_INTERRUPT - return code for interrupt handler
 *
 *********************************************************************************************
 *  \par    Revision History:
 *  <!----- No.     Date        Revised by      Details ------------------------------------->
 *
 ********************************************************************************************/
static void nvdLineToolUartInt(Type_uByte aubState, Type_uByte aubData) {
    if (STD_TRUE == aubState) {

        *wstLineToolDrvVar.ubSaveData_p = aubData;
        wstLineToolDrvVar.ubDataRecLength++;
        if (wstLineToolDrvVar.ubSaveData_p == &wstLineToolDrvVar.ubRevData_a[LINETOOL_CMDLENGTH])
        {
            wstLineToolDrvVar.ubSaveData_p = wstLineToolDrvVar.ubRevData_a;
        } else {
            wstLineToolDrvVar.ubSaveData_p++;
        }
        (void)OS_SetEventFlag(FLG_ID_LINETOOLDRV, LINETOOLDRV_FLGPTN_RECDATA);
    } else {
        (void)OS_SetEventFlag(FLG_ID_LINETOOLDRV, LINETOOLDRV_FLGPTN_ERR);
    }
}

/*********************************************************************************************
*  function                 : wvdLineToolAppControl
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : LineTool Main function
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
void wvdLineToolAppControl(OS_StdQueue_ts *astStdQueue_p)
{
    /* Auto variable Define */
    Type_uWord auwNowTime;          /* <AUTO> Now Time */
    Type_uWord auwPassTime;         /* <AUTO> Pass Time */
    Type_uHWord auhMainProcPID;     /* <AUTO> Procedure PID */
    Type_uByte aubProcIsExecute;    /* <AUTO> Procedure Status */
    Type_uByte aubProcResult;       /* <AUTO> Procedure Result */

    /* Initalize */
    auwNowTime          = LINETOOL_NUM_0;
    auwPassTime         = LINETOOL_NUM_0;
    aubProcIsExecute    = STD_FALSE;
    aubProcResult       = PERC_NO_EXECUTE_PROC;
    auhMainProcPID      = PID_PROC_MAX;
    wvdGen_MemSet(&nstLineToolAppStdQueue, LINETOOL_NUM_0, sizeof(nstLineToolAppStdQueue));

    /* Setting Request for Stand-by Process */
    wvdPESetReq(&nstLineToolAppCTB);

    if (OS_NULL != astStdQueue_p) {
        wvdGen_MemCpy(&nstLineToolAppStdQueue, astStdQueue_p, sizeof(OS_StdQueue_ts));
        if (LINETOOL_EV_UART_DATA == nstLineToolAppStdQueue.uhEventCode) {
            nvdLineToolAppDataAnalylis();
        } else {
            /* No Action */
        }

        if ((LINETOOL_EV_INVALID < nstLineToolAppStdQueue.uhEventCode) && (LINETOOL_EV_MAX > nstLineToolAppStdQueue.uhEventCode)) {
            /* Process Execute status check */
			// LINETOOLAPP_D("nstLineToolAppStdQueue.uhEventCode = %d", nstLineToolAppStdQueue.uhEventCode);
            aubProcIsExecute = wubPEIsExecuteing(&nstLineToolAppCTB);

            if (STD_TRUE == aubProcIsExecute){
                nvdLineToolAppCmdReseponse(LINETOOL_RES_NACK, OS_NULL, LINETOOL_NUM_0);
            } else { /* Process Execute */

                auhMainProcPID = nuhLineToolAppEvPIDChg();

                if(PID_PROC_MAX > auhMainProcPID) {

                    wvdPEResetProcess(&nstLineToolAppCTB);

                    wvdPESetProcess(auhMainProcPID,  &nstLineToolAppCTB);

                    aubProcResult = wubPEExecute(&nstLineToolAppCTB);

                    /* Getting wait time */
                    if ((PERC_NO_EXECUTE_PROC != aubProcResult) && (WTID_MAX > aubProcResult)){

                        wswLineToolAppTmout = nswLineToolPETimer_Tbl[aubProcResult];

                        nuwLineToolStartTime = OSAL_GetOSTimeTick();
                    } else {
						/* No Action */
                    }
					aubProcIsExecute = wubPEIsExecuteing(&nstLineToolAppCTB);

					if (STD_FALSE == aubProcIsExecute) {
						wswLineToolAppTmout = WTVALUE_60000;
					} else {
						/* No Action */
					}

                } else {
                    /* No Action */
                }
            }
        } else if ((LINETOOL_EV_TIMEOUT <= nstLineToolAppStdQueue.uhEventCode) && (LINETOOL_INF_MAX > nstLineToolAppStdQueue.uhEventCode)){
            aubProcIsExecute = wubPEIsExecuteing(&nstLineToolAppCTB);
            if (STD_TRUE == aubProcIsExecute) {
                aubProcResult = wubPEExecute(&nstLineToolAppCTB);
                /* Getting wait time */
                if ((PERC_NO_EXECUTE_PROC != aubProcResult) && (WTID_MAX > aubProcResult)) {

                    wswLineToolAppTmout = nswLineToolPETimer_Tbl[aubProcResult];

                    nuwLineToolStartTime = OSAL_GetOSTimeTick();
                } else {
                    /* No Action */
                }
            } else if(LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode){
				(void)nubLineToolSecurityTimeout();
                /* No Action */
            } else {
                    /* No Action */
            }
        } else {
            /* No Action */
        }

        /* Timeout time Caculate */
        if (wswLineToolAppTmout != OS_TOUT_FOREVER ) {
            /* Get Current Time */
            auwNowTime = OSAL_GetOSTimeTick();
            /*  */
            auwPassTime = nswLineToolAppGetProcTime(nuwLineToolStartTime, auwNowTime );
            if ( auwPassTime < wswLineToolAppTmout ) {
                /* Timeout time caculate */
                wswLineToolAppTmout = ( wswLineToolAppTmout - auwPassTime );
            } else {
                /* Timeout time 0 */
                wswLineToolAppTmout = LINETOOL_NUM_0;
            }
        } else {
            /* No Action */
        }

        /* Timeout time caculate Start */
        nuwLineToolStartTime = OSAL_GetOSTimeTick();
    } else {
        /* No Action */
    }
}

/*********************************************************************************************
*  function                 : wvdLineToolApp_SetupProEng
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : LineToolApp engine setup
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
void wvdLineToolApp_SetupProEng(void)
{
    /* Auto variable Define */
    Type_uByte aubPESetupRes; /* <AUTO> Return Code   */

    /* Initalize */
    aubPESetupRes = RES_NG;
    wvdGen_MemSet(&wstLineToolFlickerData, LINETOOL_NUM_0, sizeof(Linetool_Flicker_ts));
    wvdGen_MemSet(&nubLineToolTempData[LINETOOL_NUM_0], LINETOOL_NUM_0, sizeof(nubLineToolTempData));
    wvdGen_MemSet(&nstLineToolAppStdQueue, LINETOOL_NUM_0, sizeof(nstLineToolAppStdQueue));

    aubPESetupRes = wubPESetUp(&nstLineToolAppCTB, &nstLineToolProcBuf_a[LINETOOL_NUM_0], nstLineToolMainProc_Tbl, nstLineToolSubProc_Tbl);

    if (RES_OK == aubPESetupRes){
        /* No Action */
    }
    else {
        /* No Action */
    }
}

/*********************************************************************************************
*  function                 : nswLineToolAppGetProcTime
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : DiagApp Event
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uWord nswLineToolAppGetProcTime(Type_uWord auwStartTime, Type_uWord auwEndTime)
{
    /* Auto variable Define */
    Type_uWord auwProcTime;

    /* Initalize */
    auwProcTime = LINETOOL_NUM_0;

    /* Process Execute Time */
    if (auwEndTime >= auwStartTime){
        auwProcTime = auwEndTime - auwStartTime;
    }
    else{
        auwProcTime = (LINETOOL_TICKCOUNT_MAX - auwStartTime) + auwEndTime;
        auwProcTime++;
    }

    /* Process Execute Time Max */
    if (auwProcTime > LINETOOL_PROCTIME_MAX){
        auwProcTime = LINETOOL_PROCTIME_MAX;
    }
    else {
        /* No Action */
    }

    /* Return Process Execute Time Max */

    return auwProcTime;
}

/*********************************************************************************************
*  function                 : nvdLineToolAppDataAnalylis
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : LineTool Event
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static void nvdLineToolAppDataAnalylis(void)
{
    Type_sWord aswStrCmpRes[LINETOOL_NUM_2];
    Type_uByte aubTempNum;
    Type_uByte aubCmdCheckLen;
    Type_uByte aubCmdData[LINETOOL_CMDLENGTH];
    Type_uByte aubCmdDataLen;
    Type_uHWord auhCmdChkSum;

    aubTempNum = LINETOOL_NUM_0;
    aswStrCmpRes[LINETOOL_NUM_0] = GEN_CMP_INVALID;
    aswStrCmpRes[LINETOOL_NUM_1] = GEN_CMP_INVALID;
    aubCmdCheckLen = LINETOOL_NUM_0;
    aubCmdDataLen = LINETOOL_NUM_0;
    auhCmdChkSum = LINETOOL_NUM_0;
    wvdGen_MemSet(&aubCmdData[LINETOOL_NUM_0], LINETOOL_NUM_0, sizeof(aubCmdData));

    if (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p) {											/* check if pointer is NULL*/
        nstLineToolAppStdQueue.uhEventCode = LINETOOL_EV_INVALID;

        if (nstLineToolAppStdQueue.uwQueueSize < LINETOOL_CMDLENGTH) {																			/* check the size of Queue buffer*/
            aubCmdDataLen = (Type_uByte)(nstLineToolAppStdQueue.uwQueueSize) - LINETOOL_NUM_1;
            wvdGen_MemCpy(&aubCmdData[LINETOOL_NUM_0], nstLineToolAppStdQueue.vdQueueBuffer_p, nstLineToolAppStdQueue.uwQueueSize);			/* if no pro*/

            for (aubTempNum = LINETOOL_NUM_0; aubTempNum < aubCmdDataLen; aubTempNum++) {
                auhCmdChkSum = auhCmdChkSum + aubCmdData[aubTempNum];
            }
            auhCmdChkSum = auhCmdChkSum & LINETOOL_NUM_255;
			// printf("CheckSum:0x%02x \n", auhCmdChkSum);
            if (aubCmdData[aubTempNum] == auhCmdChkSum) {
                aubTempNum = LINETOOL_NUM_0;

				while ((aubTempNum < LINETOOL_CMDSEC_TAB_SIZE) && (aswStrCmpRes[LINETOOL_NUM_0] != GEN_CMP_EQUAL)) {
					   aubCmdCheckLen = nubLineToolComCMDSec_Tbl[aubTempNum][LINETOOL_NUM_0];
					   aswStrCmpRes[LINETOOL_NUM_0] = wswGen_MemCmp(&aubCmdData[LINETOOL_NUM_0], &nubLineToolComCMDSec_Tbl[aubTempNum][LINETOOL_NUM_1], aubCmdCheckLen);
					   aubTempNum++;
				}

				if((nubLineToolMode == LINETOOL_MODE_ON)&&(aswStrCmpRes[LINETOOL_NUM_0] != GEN_CMP_EQUAL)) {

					aubTempNum = LINETOOL_NUM_0;
					while ((aubTempNum < LINETOOL_CMD_TAB_SIZE) && (aswStrCmpRes[LINETOOL_NUM_0] != GEN_CMP_EQUAL)){
                        aubCmdCheckLen = nubLineToolComCMD_Tbl[aubTempNum][LINETOOL_NUM_0];
                        aswStrCmpRes[LINETOOL_NUM_0] = wswGen_MemCmp(&aubCmdData[LINETOOL_NUM_0], &nubLineToolComCMD_Tbl[aubTempNum][LINETOOL_NUM_1], aubCmdCheckLen);
                        aubTempNum++;
               		}
					// LINETOOLAPP_D("aubCmdCheckLen:0x%02x, aswStrCmpRes[0]:0x%08x", aubCmdCheckLen, aswStrCmpRes[LINETOOL_NUM_0]);
				} else {
					aswStrCmpRes[LINETOOL_NUM_1] = GEN_CMP_EQUAL;
				}

				if (aswStrCmpRes[LINETOOL_NUM_0] == GEN_CMP_EQUAL) {
					if((nubLineToolMode == LINETOOL_MODE_ON)&&(aswStrCmpRes[LINETOOL_NUM_1] != GEN_CMP_EQUAL))
					{
                    	nstLineToolAppStdQueue.uhEventCode = (Type_uHWord)nubLineToolComCMD_Tbl[aubTempNum - LINETOOL_NUM_1][LINETOOL_NUM_7];
                    	nvdLineToolAppCmdReseponse(LINETOOL_RES_ACK, OS_NULL, LINETOOL_NUM_0);
					}
					else {
                    	nstLineToolAppStdQueue.uhEventCode = (Type_uHWord)nubLineToolComCMDSec_Tbl[aubTempNum - LINETOOL_NUM_1][LINETOOL_NUM_7];
                    	nvdLineToolAppCmdReseponse(LINETOOL_RES_ACK, OS_NULL, LINETOOL_NUM_0);
					}
                }
                else {
                    nvdLineToolAppCmdReseponse(LINETOOL_RES_NACK, OS_NULL, LINETOOL_NUM_0);
                    LINETOOLAPP_D("NACK");
                }
            }
            else {
                nvdLineToolAppCmdReseponse(LINETOOL_RES_ERROR, OS_NULL, LINETOOL_NUM_0);
            }
        }
        else {
            nvdLineToolAppCmdReseponse(LINETOOL_RES_NACK, OS_NULL, LINETOOL_NUM_0);
            LINETOOLAPP_D("NACK");
        }
    }
    else {
        /* No Action */
    }
}

/*********************************************************************************************
*  function                 : nuhLineToolAppEvPIDChg
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : LineTool Event
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uHWord nuhLineToolAppEvPIDChg(void)
{
    /* Auto variable Define */
    Type_uHWord auhProcPID;
    Type_uByte  aubTempNum;

    /* Initalize */
    auhProcPID = PID_PROC_MAX;
    aubTempNum = LINETOOL_NUM_0;

    /* Check Event Code */
    if (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p) {
        while((nstLineToolAppEvChg_Tbl[aubTempNum].uhInputEvent != nstLineToolAppStdQueue.uhEventCode) && (aubTempNum < (LINETOOL_EV_MAX - LINETOOL_NUM_1))){
            aubTempNum++;
        }
        auhProcPID = nstLineToolAppEvChg_Tbl[aubTempNum].uhPrpcessID;
		// LINETOOLAPP_D("auhProcPID = %d, aubTempNum = %d", auhProcPID, aubTempNum);
    } else {
        /* No Action */
    }

    return auhProcPID;
}

/*********************************************************************************************
*  function                 : nvdLineToolAppDataCheckSum
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : LineTool Event
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static void nvdLineToolAppCmdReseponse(Type_uByte aubResKind, const Type_uByte *aubResData, Type_uByte aubResLen)
{
	LOG_OUTPUT(LOG_APP_ID_LINE, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "[LineTool]nvdLineToolAppCmdReseponse:(%d).",aubResKind);

    Type_uByte aubResDataBuf[LINETOOL_CMDLENGTH];

    wvdGen_MemSet(&aubResDataBuf[LINETOOL_NUM_0], LINETOOL_NUM_0, sizeof(aubResDataBuf));

    if (aubResLen > LINETOOL_CMDLENGTH - LINETOOL_NUM_2)
    {
        LOG_OUTPUT(LOG_APP_ID_LINE, DEBUGLOG_LEVEL_WARN, ATAT_LEVEL_OFF, "[LineTool]nvdLineToolAppCmdReseponse:aubResLen(%d) exceeds the upper limit value.",aubResLen);
        aubResLen = LINETOOL_CMDLENGTH - LINETOOL_NUM_2;
    }
    if (aubResData != OS_NULL){
        wvdGen_MemCpy(&aubResDataBuf[LINETOOL_NUM_2], aubResData, aubResLen);
    }
    else {
        /* No Action */
    }
    // if ((LINETOOL_NUM_7 >= aubResLen) && (aubResData != OS_NULL)){		//0301 lj
    //     OS_CopyMemory(&aubResDataBuf[LINETOOL_NUM_2], aubResData, aubResLen);
    // }
    // else {
    //     /* No Action */
    // }

    switch (aubResKind){
        case LINETOOL_RES_ACK:
            LINETOOLAPP_D("ACK");
            aubResDataBuf[LINETOOL_NUM_0] = LINETOOL_CMD_RES_HEADER;
            aubResDataBuf[LINETOOL_NUM_1] = LINETOOL_RES_ACK;
            nvdLineToolDataSend(&aubResDataBuf[LINETOOL_NUM_0], LINETOOL_NUM_6);
            break;
        case LINETOOL_RES_FINISH:
            LINETOOLAPP_D("FINISH");
            aubResDataBuf[LINETOOL_NUM_0] = LINETOOL_CMD_RES_HEADER;
            aubResDataBuf[LINETOOL_NUM_1] = LINETOOL_RES_FINISH;
            nvdLineToolDataSend(&aubResDataBuf[LINETOOL_NUM_0], LINETOOL_NUM_6);
            break;
        case LINETOOL_RES_DATA:
            LINETOOLAPP_D("DATA");
           aubResDataBuf[LINETOOL_NUM_0] = LINETOOL_CMD_RES_HEADER;
            aubResDataBuf[LINETOOL_NUM_1] = LINETOOL_RES_DATA;
            nvdLineToolDataSend(&aubResDataBuf[LINETOOL_NUM_0], (aubResLen + LINETOOL_NUM_2));
            break;
        case LINETOOL_RES_NACK:
            LINETOOLAPP_D("NACK");
            aubResDataBuf[LINETOOL_NUM_0] = LINETOOL_CMD_RES_HEADER;
            aubResDataBuf[LINETOOL_NUM_1] = LINETOOL_RES_NACK;
            nvdLineToolDataSend(&aubResDataBuf[LINETOOL_NUM_0], LINETOOL_NUM_6);
            break;
        case LINETOOL_RES_ERROR:
            LINETOOLAPP_D("ERROR");
            aubResDataBuf[LINETOOL_NUM_0] = LINETOOL_CMD_RES_HEADER;
            aubResDataBuf[LINETOOL_NUM_1] = LINETOOL_RES_ERROR;
            nvdLineToolDataSend(&aubResDataBuf[LINETOOL_NUM_0], LINETOOL_NUM_6);
            break;
        default:
            break;
    }
}

/*********************************************************************************************
*  function                 : nvdLineToolDataSend
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : LineTool Event
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static void nvdLineToolDataSend(const Type_uByte *aubBuffer_p, const Type_uByte aubBufferSize)
{
    Type_uHWord auhDataChkSum;
    Type_uByte aubTempNum;
    Type_uByte aubResDataBuf[LINETOOL_CMDLENGTH];

    aubTempNum = LINETOOL_NUM_0;
    auhDataChkSum = LINETOOL_NUM_0;
    wvdGen_MemSet(&aubResDataBuf[LINETOOL_NUM_0], LINETOOL_NUM_0, sizeof(aubResDataBuf));


    if ((aubBuffer_p != OS_NULL) && (aubBufferSize < LINETOOL_CMDLENGTH)) {
        /* calculate checksum */
        for (aubTempNum = LINETOOL_NUM_0; aubTempNum < aubBufferSize; aubTempNum++){
            aubResDataBuf[aubTempNum] = aubBuffer_p[aubTempNum];
            auhDataChkSum = auhDataChkSum + aubBuffer_p[aubTempNum];
        }

        aubResDataBuf[aubTempNum] = (Type_uByte)auhDataChkSum & LINETOOL_NUM_255;
        if (OS_NULL != nvdLineToolHalHandle) {
            HAL_Write(nvdLineToolHalHandle, &aubResDataBuf[LINETOOL_NUM_0], (aubBufferSize + LINETOOL_NUM_1));
        }
        // (void)nvdLinetoolUARTWrite(&aubResDataBuf[LINETOOL_NUM_0], aubBufferSize + LINETOOL_NUM_1);
    } else {
        /* No Action */
    }
}

/*********************************************************************************************
*  function                 : nubLineToolResFinish
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : Enginer Function
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolResFinish(void)
{
    nvdLineToolAppCmdReseponse(LINETOOL_RES_FINISH, OS_NULL, LINETOOL_NUM_0);
    return PE_SEC0;
}

/*********************************************************************************************
*  function                 : nubLineToolResError
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : Enginer Function
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolResError(void)
{
    nvdLineToolAppCmdReseponse(LINETOOL_RES_ERROR, OS_NULL, LINETOOL_NUM_0);
    return PE_SEC0;
}

/*********************************************************************************************
*  function                 : nubLineToolFmAlignment
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : Enginer Function
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolFmAlignment(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

	/* Initalize */
    aubResult = RES_NG;
    aubReturn = PE_SEC1;

    /* send data to RPC*/
    if (LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) {
		aubResult = wubLineToolApp_RPCSend(ARPC_ID_TUNER, EV_REQ_LINETOOL_FM_ALIGNMENT, OS_NULL, LINETOOL_NUM_0);

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            /* No Action */
        }
    } else {
        /* No Action */
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolFmAlignmentCB
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : Enginer Function
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolFmAlignmentCB(void) {
    //NC FmAlignmentCB
    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_FM_ALIGNMENT == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
    	wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }

    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode){
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolAmAlignment
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : Enginer Function
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolAmAlignment(void) {
    //NC AmAlignment
    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

	/* Initalize */
    aubResult = RES_NG;
    aubReturn = PE_SEC1;

    /* send data to RPC*/
    if (LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) {
		aubResult = wubLineToolApp_RPCSend(ARPC_ID_TUNER, EV_REQ_LINETOOL_AM_ALIGNMENT, OS_NULL, LINETOOL_NUM_0);

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            /* No Action */
        }
    } else {
        /* No Action */
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolAmAlignmentCB
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : Enginer Function
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolAmAlignmentCB(void) {
    //NC AmAlignmentCB
    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_AM_ALIGNMENT == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
    	wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }

    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode){
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolEepWrite
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : EEP ROM Write
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolEepWrite(void) {

    Type_uByte* aubDataBuff_p;
    Type_uHWord auhWriteAdd;
    Type_uHWord auhWriteSize;
    Type_uByte aubReturn;
    Type_uByte aubWriteRes;

    aubDataBuff_p = OS_NULL;
    auhWriteAdd = LINETOOL_NUM_0;
    auhWriteSize = LINETOOL_NUM_0;
    aubReturn = PE_SEC1;
    aubWriteRes = RES_NG;
    wvdGen_MemSet(&nubLineToolTempData[LINETOOL_NUM_0], LINETOOL_NUM_0, sizeof(nubLineToolTempData));

    if ((nstLineToolAppStdQueue.uwQueueSize == LINETOOL_NUM_10) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        aubDataBuff_p = (Type_uByte * )nstLineToolAppStdQueue.vdQueueBuffer_p;
        /* Eeprom Write Addresss */
        auhWriteAdd = aubDataBuff_p[LINETOOL_NUM_2];
        auhWriteAdd = (auhWriteAdd << LINETOOL_NUM_8) | aubDataBuff_p[LINETOOL_NUM_3];
        /* Eeprom Write Size */
        auhWriteSize = aubDataBuff_p[LINETOOL_NUM_4];

        if (LINETOOL_NUM_1 == auhWriteSize){
            nubLineToolTempData[LINETOOL_NUM_0] = aubDataBuff_p[LINETOOL_NUM_8];
        }
        else if(LINETOOL_NUM_2 == auhWriteSize){
            nubLineToolTempData[LINETOOL_NUM_0] = aubDataBuff_p[LINETOOL_NUM_7];
            nubLineToolTempData[LINETOOL_NUM_1] = aubDataBuff_p[LINETOOL_NUM_8];
        }
        else if(LINETOOL_NUM_3 == auhWriteSize){
            nubLineToolTempData[LINETOOL_NUM_0] = aubDataBuff_p[LINETOOL_NUM_6];
            nubLineToolTempData[LINETOOL_NUM_1] = aubDataBuff_p[LINETOOL_NUM_7];
            nubLineToolTempData[LINETOOL_NUM_2] = aubDataBuff_p[LINETOOL_NUM_8];
        }
        else if(LINETOOL_NUM_4 == auhWriteSize){
            nubLineToolTempData[LINETOOL_NUM_0] = aubDataBuff_p[LINETOOL_NUM_5];
            nubLineToolTempData[LINETOOL_NUM_1] = aubDataBuff_p[LINETOOL_NUM_6];
            nubLineToolTempData[LINETOOL_NUM_2] = aubDataBuff_p[LINETOOL_NUM_7];
            nubLineToolTempData[LINETOOL_NUM_3] = aubDataBuff_p[LINETOOL_NUM_8];
        }
        else {
            /* No Action */
        }

        if (auhWriteSize <= LINETOOL_NUM_4){
            aubWriteRes = wubEEP_LineToolWriteEEP(auhWriteAdd, auhWriteSize, &nubLineToolTempData[LINETOOL_NUM_0]);
            if (RES_OK == aubWriteRes) {
                aubReturn = PE_SEC0;
            } else {
                /* No Action */
            }
        } else {
            /* No Action */
        }
    } else {
        /* No Action */
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolEepRead
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : EEP ROM Read
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolEepRead(void) {

    Type_uByte* aubDataBuff_p;
    Type_uHWord auhReadAdd;
    Type_uHWord auhReadSize;
    Type_uByte aubReadBuff[LINETOOL_NUM_4];
    Type_uByte aubReturn;
    Type_uByte aubReadRes;

    aubDataBuff_p = OS_NULL;
    aubReturn = PE_SEC1;
    auhReadAdd = LINETOOL_NUM_0;
    auhReadSize = LINETOOL_NUM_0;
    aubReadRes = RES_NG;
    wvdGen_MemSet(&nubLineToolTempData[LINETOOL_NUM_0], LINETOOL_NUM_0, sizeof(nubLineToolTempData));
    wvdGen_MemSet(&aubReadBuff[LINETOOL_NUM_0], LINETOOL_NUM_0, sizeof(aubReadBuff));

    if ((nstLineToolAppStdQueue.uwQueueSize == LINETOOL_NUM_7) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        aubDataBuff_p = (Type_uByte * )nstLineToolAppStdQueue.vdQueueBuffer_p;
        /* Eeprom Read Addresss */
        auhReadAdd = aubDataBuff_p[LINETOOL_NUM_2];
        auhReadAdd = (auhReadAdd << LINETOOL_NUM_8) | aubDataBuff_p[LINETOOL_NUM_3];
        /* Eeprom Read Size */
        auhReadSize = aubDataBuff_p[LINETOOL_NUM_4];
        if (auhReadSize <= LINETOOL_NUM_4) {
            aubReadRes = wubEEP_LineToolReadEEP(auhReadAdd, auhReadSize, &nubLineToolTempData[LINETOOL_NUM_0]);
            if (RES_OK == aubReadRes) {
                if (LINETOOL_NUM_1 == auhReadSize) {
                    aubReadBuff[LINETOOL_NUM_3] = nubLineToolTempData[LINETOOL_NUM_0];
                } else if(LINETOOL_NUM_2 == auhReadSize) {
                    aubReadBuff[LINETOOL_NUM_2] = nubLineToolTempData[LINETOOL_NUM_0];
                    aubReadBuff[LINETOOL_NUM_3] = nubLineToolTempData[LINETOOL_NUM_1];
                } else if(LINETOOL_NUM_3 == auhReadSize) {
                    aubReadBuff[LINETOOL_NUM_1] = nubLineToolTempData[LINETOOL_NUM_0];
                    aubReadBuff[LINETOOL_NUM_2] = nubLineToolTempData[LINETOOL_NUM_1];
                    aubReadBuff[LINETOOL_NUM_3] = nubLineToolTempData[LINETOOL_NUM_2];
                } else if(LINETOOL_NUM_4 == auhReadSize) {
                    aubReadBuff[LINETOOL_NUM_0] = nubLineToolTempData[LINETOOL_NUM_0];
                    aubReadBuff[LINETOOL_NUM_1] = nubLineToolTempData[LINETOOL_NUM_1];
                    aubReadBuff[LINETOOL_NUM_2] = nubLineToolTempData[LINETOOL_NUM_2];
                    aubReadBuff[LINETOOL_NUM_3] = nubLineToolTempData[LINETOOL_NUM_3];
                } else {
                    /* No Action */
                }
                nvdLineToolAppCmdReseponse(LINETOOL_RES_DATA, &aubReadBuff[LINETOOL_NUM_0], LINETOOL_NUM_4);
                aubReturn = PE_SEC0;
            } else {
                /* No Action */
            }
        } else {
            /* No Action */
        }
    } else {
        /* No Action */
    }
    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolGetCurrentTime
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : Get Current Time
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolGetCurrentTime(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

	/* Initalize */
    aubResult = RES_NG;
    aubReturn = PE_SEC1;

    /* send data to RPC*/
    if (LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) {
		aubResult = wubLineToolApp_RPCSend(ARPC_ID_CLOCK, EV_REQ_LINETOOL_GET_CURRENT_TIME, OS_NULL, LINETOOL_NUM_0);

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            /* No Action */
        }
    } else {
        /* No Action */
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolGetCurrentTimeCB
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : Get Current Time CB
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolGetCurrentTimeCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubData[LINETOOL_NUM_5]; // chaged by cxx on 20220608 for send current time

    /* Initalize */
    aubReturn = PE_SEC1;
    wvdGen_MemSet(&aubData,LINETOOL_NUM_0,sizeof(aubData)); // chaged by cxx on 20220608 for send current time

    if (LINETOOL_INF_GET_CURRENT_TIME == nstLineToolAppStdQueue.uhEventCode) {

        if ((OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p) && (LINETOOL_NUM_4 == nstLineToolAppStdQueue.uwQueueSize)) {
            wvdGen_MemCpy(aubData, nstLineToolAppStdQueue.vdQueueBuffer_p, LINETOOL_NUM_4);
            if (RES_OK == aubData[LINETOOL_NUM_0]) {

                nvdLineToolAppCmdReseponse(LINETOOL_RES_DATA,&aubData[LINETOOL_NUM_1],LINETOOL_NUM_4); // chaged by cxx on 20220608 for send current time
                aubReturn = PE_SEC0;
            } else {
                /* No Action */
            }
        } else {
            /* No Action */
        }
    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode){
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}


/*********************************************************************************************
*  function                 : nubLineToolFMDirect
*  date                     : 2021.12.03
*  autho                    : liujie
*  description              : FM Direct
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolFMDirect(void) {

    Type_uByte aubData[LINETOOL_NUM_7];
    Type_uHWord auhFrequency;
    Type_uByte aubReturn;
    Type_uByte aubResult;
    Type_uByte aubSendData[LINETOOL_NUM_2];     /* add by cxx on 20220406 for #66259. */

    aubReturn = PE_SEC1;
    auhFrequency = LINETOOL_NUM_0;
    aubResult = RES_NG;
    wvdGen_MemSet(&aubData[LINETOOL_NUM_0], LINETOOL_NUM_0, sizeof(aubData));
    wvdGen_MemSet(aubSendData, LINETOOL_NUM_0, sizeof(aubSendData));     /* add by cxx on 20220406 for #66259. */

    if ((OS_NULL!=nstLineToolAppStdQueue.vdQueueBuffer_p)&&(LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize)){
        wvdGen_MemCpy(&aubData[LINETOOL_NUM_0], nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(aubData));
        /* FM Frequency Direct */
        auhFrequency  = (((aubData[LINETOOL_NUM_3] >> LINETOOL_NUM_4) & 0x0F) * LINETOOL_NUM_1000);
        auhFrequency += ((aubData[LINETOOL_NUM_3] & 0x0F) * LINETOOL_NUM_100);
        auhFrequency += (((aubData[LINETOOL_NUM_4] >> LINETOOL_NUM_4) & 0x0F) * LINETOOL_NUM_10);
        auhFrequency += (aubData[LINETOOL_NUM_4] & 0x0F);
        LINETOOLAPP_D("FM Direct Freq = %d",auhFrequency);
       
        /* Swap High Byte <-> Low Byte */
        aubSendData[LINETOOL_NUM_0] = (Type_uByte)((auhFrequency & 0xFF00) >> LINETOOL_NUM_8);     /* add by cxx on 20220406 for #66259. */
        aubSendData[LINETOOL_NUM_1] = (Type_uByte)(auhFrequency & 0x00FF);                         /* add by cxx on 20220406 for #66259. */
        if ((LINETOOL_FM_MAX >= auhFrequency) && (LINETOOL_FM_MIN <= auhFrequency)) {
            // aubResult = wubLineToolApp_RPCSend(ARPC_ID_TUNER, EV_REQ_LINETOOL_FM_FREQ_DIRECT, &auhFrequency, LINETOOL_NUM_2);     /* delete by cxx on 20220406 for #66259. */
            aubResult = wubLineToolApp_RPCSend(ARPC_ID_TUNER, EV_REQ_LINETOOL_FM_FREQ_DIRECT, &aubSendData, LINETOOL_NUM_2);             /* add by cxx on 20220406 for #66259. */
            if (RES_OK == aubResult){
                aubReturn = PE_SEC0;
            }
            else {
                /* No Actioon */
            }
        } else {
            /* No Actioon */
        }
    } else {
        /* No Action */
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolFMDirectCB
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : FM Direct CB
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolFMDirectCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_FM_FREQ_DIRECT == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
    	wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }

    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode){
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolAMDirect
*  date                     : 2021.12.03
*  autho                    : liujie
*  description              : AM Direct
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolAMDirect(void) {
    //CD AMD Direct
    Type_uByte aubData[LINETOOL_NUM_7];
    Type_uHWord auhFrequency;
    Type_uByte aubReturn;
    Type_uByte aubResult;
    Type_uByte aubSendData[LINETOOL_NUM_2];     /* add by cxx on 20220411 for #66262. */

    aubReturn = PE_SEC1;
    auhFrequency = LINETOOL_NUM_0;
    aubResult = RES_NG;

    wvdGen_MemSet(aubData, LINETOOL_NUM_0, sizeof(aubData));
    wvdGen_MemSet(aubSendData, LINETOOL_NUM_0, sizeof(aubSendData));     /* add by cxx on 20220411 for #66262. */

    if ((OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)&&(LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize)) {
        wvdGen_MemCpy(&aubData[LINETOOL_NUM_0], nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(aubData));

        /* AM Frequency Direct */
        auhFrequency  = (((aubData[LINETOOL_NUM_3] >> LINETOOL_NUM_4) & 0x0F) * LINETOOL_NUM_1000);
        auhFrequency += ((aubData[LINETOOL_NUM_3] & 0x0F) * LINETOOL_NUM_100);
        auhFrequency += (((aubData[LINETOOL_NUM_4] >> LINETOOL_NUM_4) & 0x0F) * LINETOOL_NUM_10);
        auhFrequency += (aubData[LINETOOL_NUM_4] & 0x0F);
        LINETOOLAPP_D("AM Direct Freq = %d",auhFrequency);

       /* Swap High Byte <-> Low Byte */
        aubSendData[LINETOOL_NUM_0] = (Type_uByte)((auhFrequency & 0xFF00) >> LINETOOL_NUM_8);     /* add by cxx on 202204011 for #66262. */
        aubSendData[LINETOOL_NUM_1] = (Type_uByte)(auhFrequency & 0x00FF);                         /* add by cxx on 202204011 for #66262. */
        if ((LINETOOL_AM_MAX >= auhFrequency) && (LINETOOL_AM_MIN <= auhFrequency)) {
            // aubResult = wubLineToolApp_RPCSend(ARPC_ID_TUNER, EV_REQ_LINETOOL_AM_FREQ_DIRECT, &auhFrequency, LINETOOL_NUM_2);     /* delete by cxx on 20220411 for #66262. */
            aubResult = wubLineToolApp_RPCSend(ARPC_ID_TUNER, EV_REQ_LINETOOL_AM_FREQ_DIRECT, &aubSendData, LINETOOL_NUM_2);
            if (RES_OK == aubResult) {
                aubReturn = PE_SEC0;
            } else {
                /* No Actioon */
            }
        } else {
            /* No Actioon */
        }
    } else {
        /* No Action */
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolAMDirectCB
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : AM Direct CB
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolAMDirectCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_AM_FREQ_DIRECT == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
    	wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }

    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode){
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolPowerOffReq
*  date                     : 2021.12.03
*  autho                    : liujie
*  description              : Audio Power Off Req
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolPowerOffReq(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

	/* Initalize */
    aubResult = RES_NG;
    aubReturn = PE_SEC1;

    if (LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) {
		aubResult = wubLineToolApp_RPCSend(ARPC_ID_HMI, EV_REQ_LINETOOL_POWER_OFF, OS_NULL, LINETOOL_NUM_0 ); /* send data to RPC*/

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            /* No Action */
        }
    } else {
        /* No Action */
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolPowerOffCB
*  date                     : 2021.12.03
*  autho                    : liujie
*  description              : Audio Power Off Req
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolPowerOffCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_POWER_OFF == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
    	wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }

    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolPowerOnReq
*  date                     : 2021.12.03
*  autho                    : liujie
*  description              : Audio Power On Req
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolPowerOnReq(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

	/* Initalize */
    aubResult = RES_NG;
    aubReturn = PE_SEC1;

    /* send data to RPC*/
    if (LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) {
		aubResult = wubLineToolApp_RPCSend(ARPC_ID_HMI, EV_REQ_LINETOOL_POWER_ON, OS_NULL, LINETOOL_NUM_0);

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            /* No Action */
        }
    } else {
        /* No Action */
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolPowerOnCB
*  date                     : 2021.12.03
*  autho                    : liujie
*  description              : Audio Power On Req
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolPowerOnCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_POWER_ON == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
    	wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }

    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolWarningInReq
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : Warning In Request
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolWarningInReq(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubResult = RES_NG;
    aubReturn = PE_SEC1;

    /* send data to RPC*/
    if (LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) {
        aubResult = wubLineToolApp_RPCSend(ARPC_ID_HMI, EV_REQ_LINETOOL_WARNING_IN, OS_NULL, LINETOOL_NUM_0);

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            /* No Action */
        }
    } else {
        /* No Action */
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolWarningInCB
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : Warning In Callback
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolWarningInCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_WARNING_IN == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }

    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode){
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;

}

/*********************************************************************************************
*  function                 : nubLineToolLEDOn
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : LED On
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolLEDOn(void) {
    Type_uByte aubReturn;
    Type_uByte aubResult;
	Type_uByte aubData;

    aubReturn = PE_SEC1;
    aubResult = RES_NG;
	aubData = LINETOOL_NUM_0;

	/* send data to RPC*/
    if (LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) {
		aubData = LINETOOL_LED_ON;
		aubResult = wubLineToolApp_RPCSend(ARPC_ID_LED, LINETOOL_LED_SET_REQ, &aubData, LINETOOL_NUM_1);

        if (RES_OK == aubResult){
            aubReturn = PE_SEC0;
        } else {
            /* No Action */
        }

    } else {
        /* No Action */
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolLEDOnCB
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : LED On
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolLEDOnCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;

    /* Initalize */
    aubReturn = PE_SEC1;

    if (LINETOOL_INF_LED_SET_RES == nstLineToolAppStdQueue.uhEventCode) {
		aubReturn = PE_SEC0;
    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolLEDOff
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : LED Off
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolLEDOff(void) {
    Type_uByte aubReturn;
    Type_uByte aubResult;
	Type_uByte aubData;

    aubReturn = PE_SEC1;
    aubResult = RES_NG;
	aubData = LINETOOL_NUM_0;

    if (LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) {

		aubData = LINETOOL_LED_OFF;
		aubResult = wubLineToolApp_RPCSend(ARPC_ID_LED, LINETOOL_LED_SET_REQ, &aubData, LINETOOL_NUM_1);

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            /* No Action */
        }

    } else {
        /* No Action */
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolLEDOffCB
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : LED Off
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolLEDOffCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;

    /* Initalize */
    aubReturn = PE_SEC1;

    if (LINETOOL_INF_LED_SET_RES == nstLineToolAppStdQueue.uhEventCode) {
		aubReturn = PE_SEC0;
    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }
    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLineToolSourceDirect
*  date                     : 2021.12.10
*  autho                    : liujie
*  description              : Source Direct
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolSourceDirect(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;
    Type_uByte aubData;
    Type_uByte* aubDataBuff_p;

    /* Initalize */
    aubResult = RES_NG;
    aubReturn = PE_SEC1;
    aubData = LINETOOL_NUM_0;
    aubDataBuff_p = OS_NULL;

    /* send data to RPC*/
    if ((OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p) && (LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize)) {
        aubDataBuff_p = (Type_uByte*)nstLineToolAppStdQueue.vdQueueBuffer_p;
        aubData = aubDataBuff_p[LINETOOL_NUM_2];

        switch (aubData) {
            case LINETOOL_SOURCE_FM:
            case LINETOOL_SOURCE_AM:
            case LINETOOL_SOURCE_USB:
            case LINETOOL_SOURCE_IPOD:
            case LINETOOL_SOURCE_CPL:
            case LINETOOL_SOURCE_BT_PHONE:
            case LINETOOL_SOURCE_BT_AUDIO:
            case LINETOOL_SOURCE_AAP:
            case LINETOOL_SOURCE_HOME:
            {
                aubResult = wubLineToolApp_RPCSend(ARPC_ID_HMI, EV_REQ_LINETOOL_SOURCE_DIRECT, &aubData, LINETOOL_NUM_1);
                if (RES_OK == aubResult) {
                    aubReturn = PE_SEC0;
                } else {
                    /* No Action */
                }
            }
                break;
            default:
                /* No Action */
                break;
        }
    } else {
        /* No Action */
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolSourceDirectCB
*  date                     : 2021.12.10
*  autho                    : liujie
*  description              : Source Direct Callback
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolSourceDirectCB(void) {
    //NC SourceDirectCB
    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_SOURCE_DIRECT == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }

    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode){
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;

}

/*********************************************************************************************
*  function                 : nubLineToolLCDSetEMMC
*  date                     : 2021.12.10
*  autho                    : liujie
*  description              : LCD Set EMMC
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolLCDSetEMMC(void)
{
	Type_uByte aubReturn;
	Type_uByte aubResult;
	Type_uByte *aubDataBuff_p;
	NvmResLineToolWData_ts LinetoolNvmWriteData = {};
	
	aubResult = RES_NG;
	aubDataBuff_p = OS_NULL;
	aubReturn = PE_SEC1;
	
	if ((LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p))
	{
		aubDataBuff_p = (Type_uByte * )nstLineToolAppStdQueue.vdQueueBuffer_p;
		LinetoolNvmWriteData.ubadd = LINETOOL_NUM_0;
		LinetoolNvmWriteData.ublen = LINETOOL_NUM_2;
		LinetoolNvmWriteData.ubdata[0] = aubDataBuff_p[LINETOOL_NUM_2];
		LinetoolNvmWriteData.ubdata[1] = aubDataBuff_p[LINETOOL_NUM_3];
		aubResult = wubLineToolApp_RPCSend(ARPC_ID_A53_NVM, EV_NVM_LCD_WRITE_REQ, &LinetoolNvmWriteData, sizeof(LinetoolNvmWriteData));
		if (RES_OK == aubResult)
		{
			aubReturn = PE_SEC0;
		}
		else
		{
			/* No Action */
		}
	}
	else
	{
		/* No Action */
	}
	
	return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolLCDSetEMMCCB
*  date                     : 2021.12.10
*  autho                    : liujie
*  description              : LCD Set Callback
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolLCDSetEMMCCB(void)
{
	/* Auto variable Define */
	Type_uByte aubReturn;
	Type_uByte aubResult;
	
	/* Initalize */
	aubReturn = PE_SEC1;
	aubResult = LINETOOL_NUM_0;
	
	if ((LINETOOL_INF_EMMCWRITE_LCD_RES == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p))
	{
		wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));
		if (RES_OK == aubResult)
		{
			aubReturn = PE_SEC0;
		}
		else
		{
			aubReturn = PE_SEC1;
		}
	}
	else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode)
	{
		aubReturn = PE_SEC1;
	}
	else 
	{
		aubReturn = PE_SEC_CONTINUE;
	}
	
	return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolLCDSet
*  date                     : 2021.12.10
*  autho                    : liujie
*  description              : LCD Set
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
#if 0
static Type_uByte nubLineToolLCDSet(void) {
    Type_uByte aubReturn;
    Type_uByte aubResult;
    Type_uWord auwLCDBrightData;
	Type_uByte *aubDataBuff_p;

    aubResult = RES_NG;
    aubDataBuff_p = OS_NULL;
    aubReturn = PE_SEC1;

    if ((LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {

    	aubDataBuff_p = (Type_uByte * )nstLineToolAppStdQueue.vdQueueBuffer_p;
		auwLCDBrightData = (((Type_uHWord)aubDataBuff_p[LINETOOL_NUM_2] * LINETOOL_NUM_100) + aubDataBuff_p[LINETOOL_NUM_3]) * LINETOOL_NUM_10;

        if (LINETOOL_LCD_VAL_MAX >= auwLCDBrightData) {
            aubResult = wubLineToolApp_RPCSend(ARPC_ID_LCD, EV_INF_LINETOOL_PWM_CHG_REQ, &auwLCDBrightData, sizeof(auwLCDBrightData));
	        if (RES_OK == aubResult) {
                aubReturn = PE_SEC0;
            } else {
                /* No Action */
            }
        } else {
            /* No Action */
        }
    } else {
        /* No Action */
    }

    return aubReturn;
}
#else
static Type_uByte nubLineToolLCDSet(void)
{
	Type_uByte aubReturn;
	Type_uByte aubResult;
	
	aubResult = RES_NG;
	aubReturn = PE_SEC1;
	
	aubResult = wubLineToolApp_RPCSend(ARPC_ID_LCD, EV_INF_LINETOOL_PWM_CHG_REQ, OS_NULL, LINETOOL_NUM_0);
	aubReturn = (RES_OK == aubResult) ? PE_SEC0 : PE_SEC1;
	
	return aubReturn;
}
#endif

/*********************************************************************************************
*  function                 : nubLineToolLCDSetCB
*  date                     : 2021.12.10
*  autho                    : liujie
*  description              : LCD Set Callback
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolLCDSetCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_PWM_CHG_RES == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }

    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode){
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;

}

/*********************************************************************************************
*  function                 : nubLineToolLCDSetEep
*  date                     : 2017.03.14
*  autho                    : liujie
*  description              : LCD Value Write Eep
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolLCDSetEep(void)
{
    // TODO:nubLineToolLCDSetEep
    Type_uByte aubReturn;
    Type_uByte aubResult;

    aubReturn = PE_SEC1;
    aubResult = RES_OK;

    // aubResult = wubEepWriteData(LINETOOL_LCD_EEP, LINETOOL_NUM_2, &nuhLineToolLCDData[0]);

    if (RES_OK == aubResult) {
        aubReturn = PE_SEC0;
    }
    else{
        aubReturn = PE_SEC1;
    }

    return aubReturn;
}


/*********************************************************************************************
*  function                 : nubLineToolFlickerAdjust
*  date                     : 2021.12.10
*  autho                    : liujie
*  description              : Flicker Adjust
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolFlickerAdjust(void) {
    // not used
    // Type_uByte aubReturn;
    // Type_uByte aubResult;
    // Type_uHWord auhFlicker;
    // Type_uByte aubFlicker[LINETOOL_NUM_7];
	// Type_uByte aubSendData[LINETOOL_NUM_3];

    // aubResult = RES_NG;
    // aubReturn = PE_SEC1;
    // auhFlicker = LINETOOL_NUM_0;
    // OS_SetMemory(&aubFlicker[LINETOOL_NUM_0], LINETOOL_NUM_0, sizeof(aubFlicker));
    // OS_SetMemory(&wstLineToolFlickerData, LINETOOL_NUM_0, sizeof(Linetool_Flicker_ts));

    // if ((LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) && (LINETOOL_FLICKER_MODE_ON == nubLineToolFlickerMode)){

    //     wvdGen_MemCpy(&aubFlicker[LINETOOL_NUM_0], nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(aubFlicker));
    //     auhFlicker = ((Type_uHWord)aubFlicker[LINETOOL_NUM_3] << LINETOOL_NUM_8) + ((Type_uHWord)aubFlicker[LINETOOL_NUM_4]);

    //     if ((LINETOOL_FLICKER_MIN <= auhFlicker) && (LINETOOL_FLICKER_MAX >= auhFlicker)){
	// 		aubSendData[LINETOOL_NUM_0] = LINETOOL_NUM_0;
	// 		aubSendData[LINETOOL_NUM_1] = aubFlicker[LINETOOL_NUM_3];
	// 		aubSendData[LINETOOL_NUM_2] = aubFlicker[LINETOOL_NUM_4];
	// 		wstLineToolFlickerData.ubhighPosition = aubFlicker[LINETOOL_NUM_3];
	// 		wstLineToolFlickerData.ublowPosition = aubFlicker[LINETOOL_NUM_4];

    //         aubResult = wubLineToolApp_RPCSend(ARPC_ID_HMI, EV_REQ_LINETOOL_FLC_SET, aubSendData, LINETOOL_NUM_3 ); /* send data to RPC*/

    //         if (RES_OK == aubResult){
    //             aubReturn = PE_SEC0;
    //         }
    //         else {
    //             /* No Action */
    //         }
    //     }
    //     else{
    //         /* No Action */
    //     }
    // }
    // else{
    //     /* No Action */
    // }
    // return aubReturn;
    return PE_SEC0;
}

/*********************************************************************************************
*  function                 : nubLineToolFlickerAdjustCB
*  date                     : 2021.12.10
*  autho                    : liujie
*  description              : Flicker Adjust Callback
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolFlickerAdjustCB(void) {
    /* not used */
    // /* Auto variable Define */
    // Type_uByte aubReturn;
    // Type_uByte aubResult;

    // /* Initalize */
    // aubReturn = PE_SEC1;
    // aubResult = LINETOOL_NUM_0;

    // if ((LINETOOL_INF_FLC_SET == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
    //     wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

    //     if (RES_OK == aubResult) {
    //         aubReturn = PE_SEC0;
    //     } else {
    //         aubReturn = PE_SEC1;
    //     }

    // } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
    //     aubReturn = PE_SEC1;
    // } else {
    //     aubReturn = PE_SEC_CONTINUE;
    // }

    // return aubReturn;
    return PE_SEC0;
}


/*********************************************************************************************
*  function                 : nubLineToolVolumeDirect
*  date                     : 2021.12.03
*  autho                    : liujie
*  description              : Volume Direct
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolVolumeDirect(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;
    Type_uByte aubData[LINETOOL_NUM_2];	// changed by cxx on 20220617 for #74355
    Type_uByte* aubDataBuff_p;

    /* Initalize */
    aubResult = RES_NG;
    aubReturn = PE_SEC1;
    // aubData = LINETOOL_NUM_0;
    wvdGen_MemSet(aubData, LINETOOL_NUM_0, sizeof(aubData));

    /* send data to RPC*/
    if ((OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p) && (LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize)) {
        aubDataBuff_p = (Type_uByte*)nstLineToolAppStdQueue.vdQueueBuffer_p;
        aubData[LINETOOL_NUM_0] = SOUND_VOL_DIRECT;		// add by cxx on 20220617 for #74355
        aubData[LINETOOL_NUM_1] = aubDataBuff_p[LINETOOL_NUM_2];
        aubResult = wubLineToolApp_RPCSend(ARPC_ID_SOUNDAPP, EV_REQ_LINETOOL_VOLUME_DIRECT, &aubData, LINETOOL_NUM_2);	// changed by cxx on 20220617 for #74355

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            /* No Action */
        }
    } else {
        /* No Action */
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolVolumeDirectCB
*  date                     : 2021.12.03
*  autho                    : liujie
*  description              : Volume Direct Callback
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolVolumeDirectCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_VOLUME_DIRECT == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }

    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;

}


/*********************************************************************************************
*  function                 : nubLineToolMuteControl
*  date                     : 2021.12.02
*  autho                    : liujie
*  description              : Mute Control
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolMuteControl(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;
    // Type_uByte aubData[LINETOOL_NUM_2];	// delete by cxx 0n 20220614 for A-MUTE to Sound
    Type_uByte aubData;
    Type_uByte aubSendData;	//add by cxx on 20220614 for A-MUTE to Sound
    Type_uByte *aubDataBuff_p;

    /* Initalize */
    aubResult = RES_NG;
    aubReturn = PE_SEC1;
    aubDataBuff_p = OS_NULL;
    // wvdGen_MemSet(aubData, LINETOOL_NUM_0, sizeof(aubData));
    aubData = LINETOOL_NUM_0;
    aubSendData = LINETOOL_NUM_0;

    /* send data to RPC*/
    if ((OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p) && (LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize)) {
		
		aubDataBuff_p = (Type_uByte *)nstLineToolAppStdQueue.vdQueueBuffer_p;
        // aubData[LINETOOL_NUM_0] = aubDataBuff_p[LINETOOL_NUM_2];	// delete by cxx 0n 20220614 for A-MUTE to Sound
        aubData = aubDataBuff_p[LINETOOL_NUM_2];

        // if ((LINETOOL_MUTE_OFF == aubData[LINETOOL_NUM_0]) || (LINETOOL_MUTE_ON == aubData[LINETOOL_NUM_0])) {
        if ((LINETOOL_MUTE_OFF == aubData) || (LINETOOL_MUTE_ON == aubData)) {
            aubSendData = (aubData == LINETOOL_MUTE_OFF) ? LINETOOL_SOUND_MUTE_OFF : LINETOOL_SOUND_MUTE_ON;
            // if(LINETOOL_MUTE_OFF == aubData[LINETOOL_NUM_0]){	// delete L3947-L3954 by cxx 0n 20220614 for A-MUTE to Sound
            //     aubData[LINETOOL_NUM_0] = PM_GPIO_LEVEL_LOW;
            // }
            // else{
                /* No Action */
            // }
            // aubData[LINETOOL_NUM_1] = PM_GPIO_PIN_MUTE;
            // aubResult = wubLineToolApp_RPCSend(ARPC_ID_R5_POWERDRV, EV_INF_GPIO_LEVEL_SET_REQ, &aubData, LINETOOL_NUM_2);
            aubResult = wubLineToolApp_RPCSend(ARPC_ID_SOUNDAPP, EV_REQ_LINETOOL_MUTE_CONTROL, &aubSendData, LINETOOL_NUM_1);
            if (RES_OK == aubResult) {
                aubReturn = PE_SEC0;
            } else {
                /* No Action */
            }
        } else {
           /* No Action */
        }
    } else {
        /* No Action */
    }
    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolMuteControlCB
*  date                     : 2021.12.02
*  autho                    : liujie
*  description              : Mute Control
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolMuteControlCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_MUTE_CONTROL == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }

    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;

}

/*********************************************************************************************
*  function                 : nubLineToolLoudnessSet
*  date                     : 2021.12.03
*  autho                    : liujie
*  description              : Loudness Set
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolLoudnessSet(void) {
    Type_uByte aubReturn;
    Type_uByte aubResult;
    Type_uByte aubData;
    Type_uByte *aubDataBuff_p;

    aubResult = RES_NG;
    aubReturn = PE_SEC1;
    aubData = LINETOOL_NUM_0;
    aubDataBuff_p = OS_NULL;

    if ((OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p) && (LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize)) {

        aubDataBuff_p = (Type_uByte *)nstLineToolAppStdQueue.vdQueueBuffer_p;
        aubData = aubDataBuff_p[LINETOOL_NUM_2];

        if ((LINETOOL_LOUDNESS_OFF == aubData) || (LINETOOL_LOUDNESS_ON == aubData)) {

            aubResult = wubLineToolApp_RPCSend(ARPC_ID_SOUNDAPP, EV_REQ_LINETOOL_LOUDNESS, &aubData, LINETOOL_NUM_1);
            if (RES_OK == aubResult) {
                aubReturn = PE_SEC0;
            } else {
                /* No Action */
            }
        } else {
            /* No Action */
        }
    } else {
        /* No Action */
    }
    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolLoudnessSetCB
*  date                     : 2021.12.03
*  autho                    : liujie
*  description              : Loudness Set Callback
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolLoudnessSetCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_LOUDNESS == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }

    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;

}

/*********************************************************************************************
*  function                 : nubLineToolEQSet
*  date                     : 2021.12.03
*  autho                    : liujie
*  description              : EQ Set
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolEQSet(void) {
    Type_uByte aubReturn;
    Type_uByte aubResult;
    Type_uByte aubEQState;
    Type_uByte *aubDataBuff_p;

    aubResult = RES_NG;
    aubReturn = PE_SEC1;
    aubEQState = LINETOOL_EQ_OFF;
    aubDataBuff_p = OS_NULL;

    if ((OS_NULL!= nstLineToolAppStdQueue.vdQueueBuffer_p) && (LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize)) {

        aubDataBuff_p = (Type_uByte*)nstLineToolAppStdQueue.vdQueueBuffer_p;
        aubEQState = aubDataBuff_p[LINETOOL_NUM_2];
        if ((LINETOOL_EQ_OFF == aubEQState) || (LINETOOL_EQ_ON == aubEQState)) {

            aubResult = wubLineToolApp_RPCSend(ARPC_ID_SOUNDAPP, EV_REQ_LINETOOL_EQ, &aubEQState, LINETOOL_NUM_1);
            if (RES_OK == aubResult) {
                aubReturn = PE_SEC0;
            } else {
                /* No Action */
            }
        } else {
            /* No Action */
        }
    } else {
        /* No Action */
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolEQSetCB
*  date                     : 2021.12.03
*  autho                    : liujie
*  description              : EQ Set Callback
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolEQSetCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_EQ == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }

    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;

}

/*********************************************************************************************
*  function                 : nubLineToolMemoryInit
*  date                     : 2021.12.03
*  autho                    : liujie
*  description              : Memory Init
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolMemoryInit(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubResult = RES_NG;
    aubReturn = PE_SEC1;

    /* send data to RPC*/
    if (LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) {
        aubResult = wubLineToolApp_RPCSend(ARPC_ID_HMI, EV_REQ_LINETOOL_MEMORY_INIT, OS_NULL, LINETOOL_NUM_0);

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            /* No Action */
        }
    } else {
        /* No Action */
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolMemoryInitCB
*  date                     : 2020.04.07
*  autho                    : lixiaoqiang
*  description              : Memory Init CB
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolMemoryInitCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_MEMORY_INIT == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }

    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;

}


/*********************************************************************************************
*  function                 : nubLineToolEepClear
*  date                     : 2021.12.03
*  autho                    : liujie
*  description              : EEP Clear
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolEepClear(void) {

    Type_uByte aubReturn;
    Type_uByte aubResult;

    aubReturn = PE_SEC1;
    aubResult = RES_NG;

    if (LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) {

        aubResult = wubEEP_LineToolAllClear();
        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            /* No Action */
        }

    } else {
        /* No Action */
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolGetMainTemp
*  date                     : 2021.12.03
*  autho                    : liujie
*  description              : Get Current Temp
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolGetMainTemp(void) {

    Type_uByte aubReturn;
    Type_uHWord auhTemp;
    Type_uByte aubReadBuff[LINETOOL_NUM_4];  //0314 cxx

    aubReturn = PE_SEC1;
    auhTemp = LINETOOL_NUM_0;
    wvdGen_MemSet(aubReadBuff, LINETOOL_NUM_0, sizeof(aubReadBuff));

    if (LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) {
        auhTemp = wuhSnr_GetTemp();
        LINETOOLAPP_D("Main temperature = %d", auhTemp);
        aubReadBuff[LINETOOL_NUM_0] = (Type_uByte)((auhTemp & 0xFF00) >> LINETOOL_NUM_8);
        aubReadBuff[LINETOOL_NUM_1] = (Type_uByte)(auhTemp & 0x00FF);
        nvdLineToolAppCmdReseponse(LINETOOL_RES_DATA, &aubReadBuff[LINETOOL_NUM_0], LINETOOL_NUM_4);  //0314 cxx
        aubReturn = PE_SEC0;
    } else {
        /* No Action */
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolGetLcdTemp
*  date                     : 2021.12.03
*  autho                    : liujie
*  description              : Get Lcd Temp
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolGetLcdTemp(void) {
    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

	/* Initalize */
    aubResult = RES_NG;
    aubReturn = PE_SEC1;

    if (LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) {
		aubResult = wubLineToolApp_RPCSend(ARPC_ID_LCD, EV_INF_LCD_TEMP_REQ, OS_NULL, LINETOOL_NUM_0 ); /* send data to RPC*/

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            /* No Action */
        }
    } else {
        /* No Action */
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolGetLcdTempCB
*  date                     : 2021.12.03
*  autho                    : liujie
*  description              : Get Lcd Temp CB
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolGetLcdTempCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubSendData[LINETOOL_NUM_4];
    Type_uHWord aubData;    // add by cxx on 20220616 for lcd temperature

    /* Initalize */
    aubReturn = PE_SEC1;
    wvdGen_MemSet(aubSendData, LINETOOL_NUM_0, sizeof(aubSendData));
    aubData = LINETOOL_NUM_0;

	/* send data to RPC*/
    if (LINETOOL_INF_LCD_TEMP_RES == nstLineToolAppStdQueue.uhEventCode) {

        if ((OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p) && (LINETOOL_NUM_2 == nstLineToolAppStdQueue.uwQueueSize)) {

            wvdGen_MemCpy(&aubData, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(aubData));    // change by cxx on 20220616 for lcd temperature*/
            /* add by cxx on 20220616 for Swap High Byte <-> Low Byte */
            aubSendData[LINETOOL_NUM_0] = (Type_uByte)((aubData & 0xFF00) >> LINETOOL_NUM_8);
            aubSendData[LINETOOL_NUM_1] = (Type_uByte)(aubData & 0x00FF);
            nvdLineToolAppCmdReseponse(LINETOOL_RES_DATA, aubSendData, sizeof(aubSendData));
            aubReturn = PE_SEC0;
        } else {
            /* No Action */
        }
    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolGetVerReq
*  date                     : 2021.12.02
*  autho                    : liujie
*  description              : Get DA Version request
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolGetVerReq(void)
{
    Type_uByte aubReturn;
    Type_uByte aubResult;
    Type_uByte aubData;
    Type_uByte* aubDataBuff_p;

    aubReturn = PE_SEC0;
    aubData = LINETOOL_NUM_0;
    aubDataBuff_p = OS_NULL;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = RES_NG;

    if ((LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        aubDataBuff_p = (Type_uByte*) nstLineToolAppStdQueue.vdQueueBuffer_p;
        aubData = aubDataBuff_p[LINETOOL_NUM_2];
		//0315 cxx
        if((aubData >= LINETOOL_VER_UPDATE_LINUX)&&(aubData <= LINETOOL_VER_BT)){
            if(aubData == LINETOOL_VER_BT){
                aubResult = wubLineToolApp_RPCSend(ARPC_ID_BT, EV_REQ_LINETOOL_GET_BT_VER, OS_NULL, LINETOOL_NUM_0);    // add by cxx on20220524 for #71038
                // aubResult = wubLineToolApp_RPCSend(ARPC_ID_BTAPP_TO_STM, EV_REQ_LINETOOL_GET_BT_VER, OS_NULL, LINETOOL_NUM_0);
            }
            else{
                switch (aubData) {
                    case LINETOOL_VER_UPDATE_LINUX : aubData = VERSION_MAINCORE_UPDATE; break;
                    case LINETOOL_VER_UPDATE_R5    : aubData = VERSION_MICOM_UPDATE;    break;
                    case LINETOOL_VER_R5           : aubData = VERSION_MICOM_NORMAL;    break;
                    case LINETOOL_VER_A53          : aubData = VERSION_MAINCORE_NORMAL; break;
                    case LINETOOL_VER_A7           : aubData = VERSION_SUBCORE_NORMAL;  break;
                    case LINETOOL_VER_SW           : aubData = VERSION_VECHILE;         break;
                    default:/* No Action */break;
                }
                aubResult = wubLineToolApp_RPCSend(ARPC_ID_UPDATASTM, UPD_CMD_CURRENT_VER_REQ, &aubData, LINETOOL_NUM_1);
            }

            if (RES_OK == aubResult) {
                aubReturn = PE_SEC0;
            } else {
            /* No Action */
            }
        }
    } else {
            /* No Action */
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolGetVerCB
*  date                     : 2021.12.02
*  autho                    : liujie
*  description              : Get DA Version CB
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolGetVerCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubReadBuf[LINETOOL_NUM_10];
    Type_uByte aubResult[LINETOOL_NUM_8];
    Type_uByte aubData[LINETOOL_NUM_4]; 
    Type_uByte aubi; 
    Type_uByte aubj;
    //Type_uByte aubCnt;    //delete by cxx on 20220421 for LOG
    Type_uByte *aubDataBuff_p;  // add by cxx on 20220525 for #71038

    /* Initalize */
    aubReturn = PE_SEC1;
    aubj = LINETOOL_NUM_0;
    aubi = LINETOOL_NUM_0;
    //aubCnt = LINETOOL_NUM_0;  //delete by cxx on 20220421 for LOG
    aubDataBuff_p = OS_NULL;
    wvdGen_MemSet(&aubReadBuf[LINETOOL_NUM_0], LINETOOL_NUM_0, sizeof(aubReadBuf));
    wvdGen_MemSet(&aubResult[LINETOOL_NUM_0], LINETOOL_NUM_0, sizeof(aubResult));
    wvdGen_MemSet(&aubData[LINETOOL_NUM_0], LINETOOL_NUM_0, sizeof(aubData));

    if ((LINETOOL_INF_GET_GENERAL_VER == nstLineToolAppStdQueue.uhEventCode) || (LINETOOL_INF_GET_BT_VER == nstLineToolAppStdQueue.uhEventCode)) {
  		// add L4567 - L4580 by cxx on 20220525 for #71038
        if ((OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p) && (LINETOOL_NUM_11 >= nstLineToolAppStdQueue.uwQueueSize)) {
            aubDataBuff_p = (Type_uByte * )nstLineToolAppStdQueue.vdQueueBuffer_p;
            
            if(LINETOOL_INF_GET_GENERAL_VER == nstLineToolAppStdQueue.uhEventCode){
                wvdGen_MemCpy(aubReadBuf, &aubDataBuff_p[LINETOOL_NUM_0], LINETOOL_NUM_10);
                for (aubi=LINETOOL_NUM_0; aubi< LINETOOL_NUM_10; aubi++)
                {       
                    if((aubReadBuf[aubi] != LINETOOL_ASCII_NUM_POINT)
                    &&(aubj < LINETOOL_NUM_8)
                    &&(aubReadBuf[aubi] <= LINETOOL_ASCII_NUM_9)
                    &&(aubReadBuf[aubi] >= LINETOOL_ASCII_NUM_0))
                    {
                        aubResult[aubj] = aubReadBuf[aubi] - LINETOOL_ASCII_NUM_0;
                        aubj++;
                    }
                }
                aubData[LINETOOL_NUM_0] = ((aubResult[LINETOOL_NUM_0] & 0x0f) << LINETOOL_NUM_4 | (aubResult[LINETOOL_NUM_1] & 0x0f));
                aubData[LINETOOL_NUM_1] = ((aubResult[LINETOOL_NUM_2] & 0x0f) << LINETOOL_NUM_4 | (aubResult[LINETOOL_NUM_3] & 0x0f));
                aubData[LINETOOL_NUM_2] = ((aubResult[LINETOOL_NUM_4] & 0x0f) << LINETOOL_NUM_4 | (aubResult[LINETOOL_NUM_5] & 0x0f));
                aubData[LINETOOL_NUM_3] = ((aubResult[LINETOOL_NUM_6] & 0x0f) << LINETOOL_NUM_4 | (aubResult[LINETOOL_NUM_7] & 0x0f));

                nvdLineToolAppCmdReseponse(LINETOOL_RES_DATA, aubData, LINETOOL_NUM_4);
                aubReturn = PE_SEC0;
            
            }else if(LINETOOL_INF_GET_BT_VER == nstLineToolAppStdQueue.uhEventCode){    // add by cxx on 20220525 for bt reply result
                
                if( LINETOOL_NUM_0 == aubDataBuff_p[LINETOOL_NUM_0]){
                    wvdGen_MemCpy(aubData, &aubDataBuff_p[LINETOOL_NUM_1], LINETOOL_NUM_4);
                    nvdLineToolAppCmdReseponse(LINETOOL_RES_DATA, aubData, LINETOOL_NUM_4);    // add by cxx on 20220610 for #71038
                    aubReturn = PE_SEC0;
                }else{
                    aubReturn = PE_SEC1;
                }
            }else{
                /* No Action */
            }
        } else {
            /* No Action */
        }
    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode){
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}


/*********************************************************************************************
*  function                 : nubLineToolSeekUpDownReq
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : Seek Up/Down Req
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
// static Type_uByte nubLineToolSeekUpDownReq(void) {               /* 0221 delete lj */

//     /* Auto variable Define */
//     Type_uByte aubReturn;
//     Type_uByte aubResult;
//     Type_uByte aubData;
//     Type_uByte* aubDataBuffer_p;

//     /* Initalize */
//     aubResult = RES_NG;
//     aubReturn = PE_SEC1;
//     aubData = LINETOOL_NUM_0;
//     aubDataBuffer_p = OS_NULL;

//     /* send data to RPC*/
//     if ((OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p) && (LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize)) {
//         aubDataBuffer_p = (Type_uByte*)nstLineToolAppStdQueue.vdQueueBuffer_p;
//         aubData = aubDataBuffer_p[LINETOOL_NUM_2];

//         if (LINETOOL_TRACK_UP == aubData) {
//             aubResult = wubLineToolApp_RPCSend(ARPC_ID_TUNER, EV_REQ_LINETOOL_SEEK_UP, OS_NULL, LINETOOL_NUM_0);
//         } else if (LINETOOL_TRACK_DOWN == aubData) {
//             aubResult = wubLineToolApp_RPCSend(ARPC_ID_TUNER, EV_REQ_LINETOOL_SEEK_DOWN, OS_NULL, LINETOOL_NUM_0);
//         } else {
//             /* No Action */
//         }

//         if (RES_OK == aubResult) {
//             aubReturn = PE_SEC0;
//         } else {
//             /* No Action */
//         }
//     } else {
//         /* No Action */
//     }

//     return aubReturn;
// }
/*********************************************************************************************
*  function                 : nubLineToolSeekUpDownCB
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : Seek Up/Down CB
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
// static Type_uByte nubLineToolSeekUpDownCB(void) {        /* 0221 delete lj */

//     /* Auto variable Define */
//     Type_uByte aubReturn;
//     Type_uByte aubResult;

//     /* Initalize */
//     aubReturn = PE_SEC1;
//     aubResult = LINETOOL_NUM_0;

//     if (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p) {
//         if ((LINETOOL_INF_SEEK_UP == nstLineToolAppStdQueue.uhEventCode) || (LINETOOL_INF_SEEK_DOWN == nstLineToolAppStdQueue.uhEventCode)) {
//             wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));
//             if (RES_OK == aubResult){
//                 aubReturn = PE_SEC0;
//             }
//             else{
//                 aubReturn = PE_SEC1;
//             }
//         }
//     }
//     else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode){
//         aubReturn = PE_SEC1;
//     }
//     else {
//         aubReturn = PE_SEC_CONTINUE;
//     }

//     return aubReturn;
// }

/*********************************************************************************************
*  function                 : nubLineToolIpodTrackChgReq
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : Ipod Track Up/Down Req
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolIpodTrackChgReq(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;
    Type_uByte *aubDataBuff_p;
    Type_uByte aubData;

    /* Initalize */
    aubResult = RES_NG;
    aubReturn = PE_SEC1;
    aubDataBuff_p = OS_NULL;
    aubData = LINETOOL_NUM_0;

    if ((OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p) && (LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize)) {

        aubDataBuff_p = (Type_uByte *)nstLineToolAppStdQueue.vdQueueBuffer_p;
        aubData = aubDataBuff_p[LINETOOL_NUM_2];

        if ((LINETOOL_TRACK_UP == aubData) || (LINETOOL_TRACK_DOWN == aubData)) {

            aubResult = wubLineToolApp_RPCSend(ARPC_ID_HMI, EV_REQ_LINETOOL_IPOD_TRACK_CHG, &aubData, LINETOOL_NUM_1);
            if (RES_OK == aubResult) {
                aubReturn = PE_SEC0;
            } else {
                /* No Action */
            }
        } else {
            /* No Action */
        }
    } else {
        /* No Action */
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolIpodTrackChgCB
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : Ipod Track Up/Down CB
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolIpodTrackChgCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_IPOD_TRACK_CHG == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }

    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;

}

/*********************************************************************************************
*  function                 : nubLineToolUSBTrackChgReq
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : USB Track Up/Down Req
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolUSBTrackChgReq(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;
    Type_uByte *aubDataBuff_p;
    Type_uByte aubData;

    /* Initalize */
    aubResult = RES_NG;
    aubReturn = PE_SEC1;
    aubDataBuff_p = OS_NULL;
    aubData = LINETOOL_NUM_0;

    if ((OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p) && (LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize)) {

        aubDataBuff_p = (Type_uByte *)nstLineToolAppStdQueue.vdQueueBuffer_p;
        aubData = aubDataBuff_p[LINETOOL_NUM_2];

        if ((LINETOOL_TRACK_UP == aubData) || (LINETOOL_TRACK_DOWN == aubData)) {
            aubResult = wubLineToolApp_RPCSend(ARPC_ID_HMI, EV_REQ_LINETOOL_USB_TRACK_CHG, &aubData, LINETOOL_NUM_1);

            if (RES_OK == aubResult) {
                aubReturn = PE_SEC0;
            } else {
                /* No Action */
            }
        } else {
            /* No Action */
        }
    }
    else{
        /* No Action */
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolUSBTrackChgCB
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : USB Track Up/Down CB
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolUSBTrackChgCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_USB_TRACK_CHG == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }

    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolCameraEnterReq
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : Camera Enter Req
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolCameraEnterReq(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubResult = RES_NG;
    aubReturn = PE_SEC1;

    /* send data to RPC*/
    if (LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) {
        aubResult = wubLineToolApp_RPCSend(ARPC_ID_HMI, EV_REQ_LINETOOL_CAMERA_ENTER, OS_NULL, LINETOOL_NUM_0);

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            /* No Action */
        }
    } else {
        /* No Action */
    }
    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolCameraEnterCB
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : Camera Enter CB
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolCameraEnterCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_CAMERA_ENTER == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

        if (RES_OK == aubResult) {
            nubLineToolCameraState = LINETOOL_CAMERA_ON;
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }

    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;

}

/*********************************************************************************************
*  function                 : nubLineToolCameraChgReq
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : Camera Chg Req
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolCameraChgReq(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;
    Type_uByte aubData;
    Type_uByte *aubDataBuff_p;

    /* Initalize */
    aubResult = RES_NG;
    aubReturn = PE_SEC1;
    aubData = LINETOOL_NUM_0;
    aubDataBuff_p = OS_NULL;

    if ((LINETOOL_CAMERA_ON == nubLineToolCameraState) &&
        (LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) &&
        (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {

        aubDataBuff_p = (Type_uByte *)nstLineToolAppStdQueue.vdQueueBuffer_p;
        aubData = aubDataBuff_p[LINETOOL_NUM_2];
        if((LINETOOL_CAMERA_NORMAL == aubData)||(LINETOOL_CAMERA_TOP == aubData)||(LINETOOL_CAMERA_WIDE == aubData)) {

            aubResult = wubLineToolApp_RPCSend(ARPC_ID_REARCAMERA, EV_REQ_LINETOOL_CAMERA_CHG, &aubData, LINETOOL_NUM_1);
            if (RES_OK == aubResult) {
                aubReturn = PE_SEC0;
            } else {
                /* No Action */
            }
        } else {
            /* No Action */
        }
    } else {
        /* No Action */
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolCameraChgCB
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : Camera Chg CB
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolCameraChgCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_CAMERA_CHG == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }

    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;

}

/*********************************************************************************************
*  function                 : nubLineToolCameraQuitReq
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : Camera Quit Req
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolCameraQuitReq(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubResult = RES_NG;
    aubReturn = PE_SEC1;

    /* send data to RPC*/
    if (LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) {
        aubResult = wubLineToolApp_RPCSend(ARPC_ID_HMI, EV_REQ_LINETOOL_CAMERA_QUIT, OS_NULL, LINETOOL_NUM_0);
        wubLineToolApp_RPCSend(ARPC_ID_REARCAMERA, EV_REQ_LINETOOL_CAMERA_QUIT, OS_NULL, LINETOOL_NUM_0);

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            /* No Action */
        }
    } else {
        /* No Action */
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolCameraQuitCB
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : Camera Quit CB
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolCameraQuitCB(void) {
    Type_uByte aubReturn;
    Type_uByte aubResult;

    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_CAMERA_QUIT == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
    	wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

        if (RES_OK == aubResult) {
            nubLineToolCameraState = LINETOOL_CAMERA_OFF;
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }

    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode){
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolFanControl
*  date                     : 2021.12.03
*  autho                    : liujie
*  description              : Fan Control
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolFanControl(void)
{
    Type_uByte aubReturn;
    Type_uByte aubResult;
    Type_uByte aubFanState;
    Type_uByte aubData;
    Type_uByte *aubDataBuff_p;

    aubReturn = PE_SEC1;
    aubResult = RES_NG;
    aubFanState = LINETOOL_FAN_OFF;
    aubData = SNR_INF_FAN_CONTROL;
    aubDataBuff_p = OS_NULL;

    if ((OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p) && (LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize)) {

        aubDataBuff_p = (Type_uByte *)nstLineToolAppStdQueue.vdQueueBuffer_p;
        aubFanState = aubDataBuff_p[LINETOOL_NUM_2];

        if (LINETOOL_FAN_HIGH == aubFanState) {

            aubData = SNR_INF_FAN_ON;
            aubResult = wubLineToolApp_RPCSend(ARPC_ID_SENSOR, SNR_EV_INF_FANSET_REQ, &aubData, LINETOOL_NUM_1);

        } else if (LINETOOL_FAN_OFF == aubFanState) {

            aubData = SNR_INF_FAN_OFF;
            aubResult = wubLineToolApp_RPCSend(ARPC_ID_SENSOR, SNR_EV_INF_FANSET_REQ, &aubData, LINETOOL_NUM_1);

        } else {
            /* No Action */
        }

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            /* No Action */
        }
    } else {
        /* No Action */
    }
    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolFanControlCB
*  date                     : 2021.12.03
*  autho                    : liujie
*  description              : Fan Control
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolFanControlCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;

    /* Initalize */
    aubReturn = PE_SEC1;

    if (LINETOOL_INF_SNR_FANSET_RES == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC0;
    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolBTCheckReq
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : BT Check Req
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolBTCheckReq(void) {
    /* Auto variable Define */
    Type_uByte aubResult;
	Type_uByte aubReturn;
	Type_uByte aubData[LINETOOL_NUM_10];

    /* Initalize */
	aubResult = RES_NG;
	aubReturn = PE_SEC1;
	wvdGen_MemSet(aubData, LINETOOL_NUM_0, sizeof(aubData));

	if (LINETOOL_NUM_10 == nstLineToolAppStdQueue.uwQueueSize && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {

        wvdGen_MemCpy(aubData, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(aubData));
        aubResult = wubLineToolApp_RPCSend(ARPC_ID_BT, EV_REQ_LINETOOL_BT_CHECK, &aubData[LINETOOL_NUM_2], LINETOOL_NUM_6);
        if(RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            /* No Action */
        }
	} else {
		/* No Action */
	}

	return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolBTCheckCB
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : BT Check CB
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolBTCheckCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_BT_CHECK == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }

    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;

}

/*********************************************************************************************
*  function                 : nubLineToolBTAudio1kzSetReq
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : BT Audio 1kz Set Req
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolBTAudio1kzSetReq(void) {

    /* Auto variable Define */
    Type_uByte aubResult;
	Type_uByte aubReturn;
	Type_uByte aubData[LINETOOL_NUM_7];

    /* Initalize */
	aubResult = RES_NG;
	aubReturn = PE_SEC1;
	wvdGen_MemSet(aubData, LINETOOL_NUM_0, sizeof(aubData));

	if (LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        wvdGen_MemCpy(aubData, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(aubData));
        if((LINETOOL_BTAUDIO_1KHZ_START == aubData[LINETOOL_NUM_2]) || (LINETOOL_BTAUDIO_1KHZ_STOP == aubData[LINETOOL_NUM_2])) {

			aubResult = wubLineToolApp_RPCSend(ARPC_ID_BT, EV_REQ_LINETOOL_BT_AUDIO_1KZ_SET, &aubData[LINETOOL_NUM_2], LINETOOL_NUM_1);
            if(RES_OK == aubResult) {
				aubReturn = PE_SEC0;
			} else {
				/* No Action */
			}
		} else {
			/* No Action */
		}
	} else {
		/* No Action */
	}

	return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolBTAudio1kzSetCB
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : BT Audio 1kz Set CB
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolBTAudio1kzSetCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_BT_AUDIO_1KZ_SET == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }

    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolUSBAudioChgReq
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : USB Audio Chg Req
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolUSBAudioChgReq(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubResult = RES_NG;
    aubReturn = PE_SEC1;

    /* send data to RPC*/
    if (LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) {
        aubResult = wubLineToolApp_RPCSend(ARPC_ID_HMI, EV_REQ_LINETOOL_USB_AUDIO_CHG, OS_NULL, LINETOOL_NUM_0);

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            /* No Action */
        }
    } else {
        /* No Action */
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolUSBAudioChgCB
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : USB Audio Chg CB
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolUSBAudioChgCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_USB_AUDIO_CHG == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }

    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;

}

/*********************************************************************************************
*  function                 : nubLineToolUSBVideoChgReq
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : USB Video Chg Req
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolUSBVideoChgReq(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubResult = RES_NG;
    aubReturn = PE_SEC1;

    /* send data to RPC*/
    if (LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) {
        aubResult = wubLineToolApp_RPCSend(ARPC_ID_HMI, EV_REQ_LINETOOL_USB_VEDIO_CHG, OS_NULL, LINETOOL_NUM_0);

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            /* No Action */
        }
    } else {
        /* No Action */
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolUSBVideoChgCB
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : USB Video Chg CB
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolUSBVideoChgCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_USB_VEDIO_CHG == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }

    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;

}

/*********************************************************************************************
*  function                 : nubLineToolTPCheckEndReq
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : TP Check End Req
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolTPCheckEndReq(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubResult       = RES_NG;
    aubReturn       = PE_SEC1;

    if ((LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) && (OS_NULL!= nstLineToolAppStdQueue.vdQueueBuffer_p)) {

        aubResult = wubLineToolApp_RPCSend(ARPC_ID_HMI, EV_REQ_LINETOOL_TP_MODE_OFF, OS_NULL, LINETOOL_NUM_0);

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            /* No Action */
        }


    } else {
        /* No Action */
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolTPCheckEndCB
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : TP Check End CB
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolTPCheckEndCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_TP_MODE_OFF == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)){

        wvdGen_MemCpy(&aubResult,nstLineToolAppStdQueue.vdQueueBuffer_p,LINETOOL_NUM_1);

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
            nubLineToolTPMode = LINETOOL_TP_MODE_OFF;
        } else {
	         aubReturn = PE_SEC1;
	    }
	}
    else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolTPCheckStartReq
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : TP Check Start Req
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolTPCheckStartReq(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubResult     = RES_NG;
    aubReturn     = PE_SEC1;

    if ((LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) && (OS_NULL!= nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        aubResult = wubLineToolApp_RPCSend(ARPC_ID_HMI, EV_REQ_LINETOOL_TP_MODE_ON, OS_NULL, LINETOOL_NUM_0);
        if (RES_OK == aubResult){
            aubReturn = PE_SEC0;
        }
        else {
            /* No Action */
        }

    } else {
        /* No Action */
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolTPCheckStartCB
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : TP Check Start CB
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolTPCheckStartCB(void) {
    Type_uByte aubReturn;
    Type_uByte aubResult;

    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_1;

    if ((LINETOOL_INF_TP_MODE_ON == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)){

        wvdGen_MemCpy(&aubResult,nstLineToolAppStdQueue.vdQueueBuffer_p,LINETOOL_NUM_1);

        if(aubResult == RES_OK) {
            aubReturn = PE_SEC0;
            nubLineToolTPMode = LINETOOL_TP_MODE_ON;
        } else {
            aubReturn = PE_SEC1;
        }
    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolTPCheckReq
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : TP Check Req
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolTPCheckReq(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;
    Type_uByte aubData;

    /* Initalize */
    aubResult = RES_NG;
    aubReturn = PE_SEC1;
    aubData = LINETOOL_NUM_0;

    /* send data to RPC*/
    if (LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) {
        aubData = LINETOOL_TPCHECK_ON;
        aubResult = wubLineToolApp_RPCSend(ARPC_ID_TP, EV_INF_TPCTL_POS_DISPATCH_REQ, &aubData, LINETOOL_NUM_1);
        
        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            /* No Action */
        }
    } else {
        /* No Action */
    }
    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolTPCheckCB
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : TP Check CB
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolTPCheckCB(void)
{
    Type_uByte aubReturn;
    Type_uByte *aubDataBuff_p;
    Type_uByte aubData[LINETOOL_NUM_4];
    TpctlTouchInfo astLineToolTouchData;    // changed by cxx on 20220609 for tp check
    Type_uByte aubTPCheckOFF;

    aubReturn = PE_SEC1;
    aubDataBuff_p = OS_NULL;
    wvdGen_MemSet(&aubData[LINETOOL_NUM_0], LINETOOL_NUM_0, sizeof(aubData));
    wvdGen_MemSet(&astLineToolTouchData, LINETOOL_NUM_0, sizeof(TpctlTouchInfo));    // changed by cxx on 20220609 for tp check
    aubTPCheckOFF = LINETOOL_TPCHECK_OFF;

    if ((LINETOOL_INF_TP_CHECK == nstLineToolAppStdQueue.uhEventCode)&&(OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)){
        //TODO if receive data -> TP Check OFF
        wubLineToolApp_RPCSend(ARPC_ID_TP, EV_INF_TPCTL_POS_DISPATCH_REQ, &aubTPCheckOFF, LINETOOL_NUM_1);
        
        aubDataBuff_p = (Type_uByte *)nstLineToolAppStdQueue.vdQueueBuffer_p;          
        wvdGen_MemCpy(&astLineToolTouchData, &aubDataBuff_p[LINETOOL_NUM_0], sizeof(TpctlTouchInfo));    // changed Line5918~5923 by cxx on 20220609 for tp check

        aubData[LINETOOL_NUM_0] = astLineToolTouchData.stTouchPoint->uhTouchPos_x / LINETOOL_NUM_100;
        aubData[LINETOOL_NUM_1] = astLineToolTouchData.stTouchPoint->uhTouchPos_x  % LINETOOL_NUM_100;
        aubData[LINETOOL_NUM_2] = astLineToolTouchData.stTouchPoint->uhTouchPos_y  / LINETOOL_NUM_100;
        aubData[LINETOOL_NUM_3] = astLineToolTouchData.stTouchPoint->uhTouchPos_y  % LINETOOL_NUM_100;

        nvdLineToolAppCmdReseponse(LINETOOL_RES_DATA, &aubData[LINETOOL_NUM_0], LINETOOL_NUM_4);
        aubReturn = PE_SEC0;    //0315 cxx
    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolFlickerModeJudge
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : Flicker Mode Judge
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolFlickerModeJudge(void)
{
    Type_uByte aubReturn;

    aubReturn = PE_SEC1;
    /* not used */
    // if (LINETOOL_FLICKER_MODE_ON == nubLineToolFlickerMode) {
    //     aubReturn = PE_SEC0;
    // }
    // else{
    //     /* No Action */
    // }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolFlickerModeOn
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : Flicker Mode Start
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolFlickerModeOn(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubResult = RES_NG;
    aubReturn = PE_SEC1;

    /* send data to RPC*/
    if ((LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)){
        aubResult = wubLineToolApp_RPCSend(ARPC_ID_HMI, EV_REQ_LINETOOL_FLC_MODE_ON, OS_NULL, LINETOOL_NUM_0);
            if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            /* No Action */
        }
    } else {
        /* No Action */
    }
    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolFlickerModeOnCB
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : Flicker Mode Start CB
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolFlickerModeOnCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_FLC_MODE_ON == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }

    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;

}

/*********************************************************************************************
*  function                 : nubLineToolFlickerModeOff
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : Flicker Mode End
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolFlickerModeOff(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubResult = RES_NG;
    aubReturn = PE_SEC1;

    /* send data to RPC*/
   if ((LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {

        aubResult = wubLineToolApp_RPCSend(ARPC_ID_HMI, EV_REQ_LINETOOL_FLC_MODE_OFF, OS_NULL, LINETOOL_NUM_0);
        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            /* No Action */
        }
    } else {
        /* No Action */
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolFlickerModeOffCB
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : Flicker Mode End CB
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolFlickerModeOffCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_FLC_MODE_OFF == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }

    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolMICPathBuild
*  date                     : 2021.12.06
*  autho                    : liujie
*  description              : MIC Path Build
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolMICPathBuild(void) {

	/* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

	/* Initalize */
	aubResult = RES_NG;
    aubReturn = PE_SEC1;

	/* send data to RPC*/
        aubResult = wubLineToolApp_RPCSend(ARPC_ID_BT, EV_REQ_LINETOOL_MIC_PATH_BUILD, OS_NULL, LINETOOL_NUM_0);

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            /* No Action */
        }
    return aubReturn;

}

/*********************************************************************************************
*  function                 : nubLineToolMICPathBuildCB
*  date                     : 2021.12.06
*  autho                    : liujie
*  description              : MIC Path Build
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolMICPathBuildCB(void) {
    // printf("[6] Build BT Callback\n");
    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_MIC_PATH_BUILD == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }

    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;

}

/*********************************************************************************************
*  function                 : nubLineToolMICPathCut
*  date                     : 2021.12.06
*  autho                    : liujie
*  description              : MIC Path Cut
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolMICPathCut(void) {
    // printf("[1] BT Off\n");
	/* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;
    Type_uByte aubData;
    Type_uByte *aubDataBuff_p;

	/* Initalize */
	aubResult = RES_NG;
    aubReturn = PE_SEC1;
    aubData = LINETOOL_NUM_0;
    aubDataBuff_p = OS_NULL;

	/* send data to RPC*/
        if ((OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p) && (LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize)) {

        aubDataBuff_p = (Type_uByte *)nstLineToolAppStdQueue.vdQueueBuffer_p;
        aubData = aubDataBuff_p[LINETOOL_NUM_2];

        if (LINETOOL_MIC_BT == aubData) {
        aubResult = wubLineToolApp_RPCSend(ARPC_ID_BT, EV_REQ_LINETOOL_MIC_PATH_CUT, OS_NULL, LINETOOL_NUM_0);
        } else {
            /* No Action */
        }
    } else {
        /* No Action */
    }
    if (RES_OK == aubResult) {
        aubReturn = PE_SEC0;
    } else {
        /* No Action */
    }
    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolMICPathCutCB
*  date                     : 2021.12.06
*  autho                    : liujie
*  description              : MIC Path Cut CB
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolMICPathCutCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_MIC_PATH_CUT == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }

    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;

}

/*********************************************************************************************
*  function                 : nubLineToolTestSourceOn
*  date                     : 2021.12.06
*  autho                    : liujie
*  description              : Test Source On
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolTestSourceOn(void) {
    // printf("[1] Build HMI Source On\n");
	/* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;
    Type_uByte aubData;
    Type_uByte *aubDataBuff_p;


	/* Initalize */
	aubResult = RES_NG;
    aubReturn = PE_SEC1;
    aubData = LINETOOL_NUM_0;
    aubDataBuff_p = OS_NULL;

	/* send data to RPC*/
    if ((OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p) && (LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize)) {

        aubDataBuff_p = (Type_uByte *)nstLineToolAppStdQueue.vdQueueBuffer_p;
        aubData = aubDataBuff_p[LINETOOL_NUM_2];

        if (LINETOOL_MIC_BT == aubData){

            aubResult = wubLineToolApp_RPCSend(ARPC_ID_HMI, EV_REQ_LINETOOL_TEST_SOURCE_ON, &aubData, LINETOOL_NUM_1);
            if (RES_OK == aubResult) {
                aubReturn = PE_SEC0;
            } else {
                /* No Action */
            }
        } else {
            /* No Action */
        }
    } else {
        /* No Action */
    }
    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolTestSourceOnCB
*  date                     : 2021.12.06
*  autho                    : liujie
*  description              : Test Source On CB
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolTestSourceOnCB(void) {
    // printf("[2] Build HMI Source On Callback\n");
    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_TEST_SOURCE_ON == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }

    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;

}

/*********************************************************************************************
*  function                 : nubLineToolTestSourceOff
*  date                     : 2021.12.06
*  autho                    : liujie
*  description              : Test Source Off
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolTestSourceOff(void) {
    // printf("[5] HMI Off\n");
    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;
    Type_uByte aubData;
   
    /* Initalize */
    aubResult = RES_NG;
    aubReturn = PE_SEC1;
    aubData = LINETOOL_NUM_0;
	
    /* send data to RPC*/
    aubData = LINETOOL_MIC_BT;
    aubResult = wubLineToolApp_RPCSend(ARPC_ID_HMI, EV_REQ_LINETOOL_TEST_SOURCE_OFF, &aubData, LINETOOL_NUM_1);
    if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
     } else {
                /* No Action */
     }

    return aubReturn;

}

/*********************************************************************************************
*  function                 : nubLineToolTestSourceOffCB
*  date                     : 2021.12.06
*  autho                    : liujie
*  description              : Test Source Off CB
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolTestSourceOffCB(void) {
    // printf("[5] HMI Off Callback\n");
    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_TEST_SOURCE_OFF == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }

    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;

}


/*********************************************************************************************
*  function                 : nubLineToolDisplayAdjust
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : Display Adjust
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolDisplayAdjust(void)
{
    /* Auto variable Define */
    Type_uByte aubResult;
	Type_uByte aubReturn;
    Type_uByte aubData;
	Type_uByte *aubDataBuff_p;

    /* Initalize */
	aubResult = RES_NG;
	aubReturn = PE_SEC1;
    aubData = LINETOOL_NUM_0;
	aubDataBuff_p = OS_NULL;

	if (LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {

        aubDataBuff_p = (Type_uByte *)nstLineToolAppStdQueue.vdQueueBuffer_p;
        aubData = aubDataBuff_p[LINETOOL_NUM_2];

		if(LINETOOL_NUM_10 >= aubData) {

			aubResult = wubLineToolApp_RPCSend(ARPC_ID_HMI, EV_REQ_LINETOOL_DISPLAY_ADJUST, &aubData, LINETOOL_NUM_1);
            if(RES_OK == aubResult) {
				aubReturn = PE_SEC0;
			} else {
				/* No Action */
			}
		} else {
			/* No Action */
		}
	} else {
		/* No Action */
	}

	return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolDisplayAdjustCB
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : Display Adjust CB
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolDisplayAdjustCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_DISPLAY_ADJUST == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }

    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;

}

/*********************************************************************************************
*  function                 : nubLineToolFlickerSetEep
*  date                     : 2017.03.08
*  autho                    : liujie
*  description              : Flicker Value Write Eep
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolFlickerSetEep(void) {
    Type_uByte aubReturn;
    // Type_uByte aubResult;

    aubReturn = PE_SEC1;
    // aubResult = EEP_ERR_PAR;

    wvdGen_MemSet(&nubLineToolTempData[LINETOOL_NUM_0], LINETOOL_NUM_0, sizeof(nubLineToolTempData));

    nubLineToolTempData[LINETOOL_NUM_0] = wstLineToolFlickerData.ubhighPosition;
    nubLineToolTempData[LINETOOL_NUM_1] = wstLineToolFlickerData.ublowPosition;

    // aubResult = wubEepWriteData(LINETOOL_FLICKER_EEP, LINETOOL_NUM_2, &nubLineToolTempData[LINETOOL_NUM_0]);

    // if (EEP_OK == aubResult){
    //     aubReturn = PE_SEC0;
    // }
    // else{
    //     aubReturn = PE_SEC1;
    // }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLineToolSecurityOn
*  date                     : 2021.10.26
*  autho                    : -
*  description              : Security ON
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolSecurityOn(void)
{
	Type_uByte aubReturn;
	Type_uByte* aubDataBuff_p;
	Type_uWord auwLineToolSecurityCode;
	aubDataBuff_p =OS_NULL;
    aubReturn = PE_SEC1;
	 if (nstLineToolAppStdQueue.uwQueueSize == LINETOOL_NUM_7){
        aubDataBuff_p = (Type_uByte * )nstLineToolAppStdQueue.vdQueueBuffer_p;
        auwLineToolSecurityCode = ((Type_uWord)aubDataBuff_p[LINETOOL_NUM_2] << LINETOOL_NUM_24) | (aubDataBuff_p[LINETOOL_NUM_3] << LINETOOL_NUM_16) | (aubDataBuff_p[LINETOOL_NUM_4] << LINETOOL_NUM_8) | aubDataBuff_p[LINETOOL_NUM_5];
		if (LINETOOL_SECURITY_CODE == auwLineToolSecurityCode) {
			aubReturn = PE_SEC0;
			nubLineToolMode = LINETOOL_MODE_ON;
		}
	}
	else {
        /* No Action */
    }

	return aubReturn;
}

 /*********************************************************************************************
 *	function				 : nubLineToolSecurityOff
 *	date					 : 2018.07.19
 *	autho					 : -
 *	description 			 : Security OFF
 *
 *	<!-------------- Argument Code ---------------------------------------------------------->
 *	param[in]
 *
 *	<!-------------- Return Code ------------------------------------------------------------>
 *	return
 *
 *********************************************************************************************
 *	revision History
 *	No. 	  date			 revised by 	   description
 *
 *********************************************************************************************/
static Type_uByte nubLineToolSecurityOff(void)
{
	nubLineToolMode = LINETOOL_MODE_OFF;

	return PE_SEC0;
}

 /*********************************************************************************************
 *	function				 : nubLineToolSecurityTimeout
 *	date					 : 2018.07.19
 *	autho					 : -
 *	description 			 : Security TIMEOUT
 *
 *	<!-------------- Argument Code ---------------------------------------------------------->
 *	param[in]
 *
 *	<!-------------- Return Code ------------------------------------------------------------>
 *	return
 *
 *********************************************************************************************
 *	revision History
 *	No. 	  date			 revised by 	   description
 *
 *********************************************************************************************/
static Type_uByte nubLineToolSecurityTimeout(void)
{
	Type_uByte aubResult;
	Type_uByte aubReturn;
	aubResult = RES_NG;
    aubReturn = PE_SEC1;
	nubLineToolMode = LINETOOL_MODE_OFF;
    wswLineToolAppTmout = OS_TOUT_FOREVER;
//	aubResult = wubRPC_SendDataToGerdaC(MODULE_ID_GERDA_HMI, EV_REQ_LINETOOL_MODE_OFF, STD_NULL, LINETOOL_NUM_0);

	if(RES_OK != aubResult)
	{
		aubReturn = PE_SEC1;
	}
	else{
		aubReturn = PE_SEC0;
	}


	return aubReturn;
}

/*********************************************************************************************
*  function 				: nubLineToolIpodCarplayChg
*  date 					: 2021.10.26
*  autho					: -
*  description				: Ipod/Carplay Chg
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.		 date			revised by		  description
*
*********************************************************************************************/
static Type_uByte nubLineToolIpodCarplayChg(void) {

    /* Auto variable Define */
    Type_uByte aubResult;
	Type_uByte aubReturn;
	Type_uByte aubData;
    Type_uByte *aubDataBuff_p;

     /* Initalize */
	aubResult = RES_NG;
	aubReturn = PE_SEC1;
    aubData = LINETOOL_NUM_0;
    aubDataBuff_p = OS_NULL;

	/* send data to RPC*/
	if ((LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {

		aubDataBuff_p = (Type_uByte*) nstLineToolAppStdQueue.vdQueueBuffer_p;
        aubData = aubDataBuff_p[LINETOOL_NUM_2];
        if((LINETOOL_CARPLAY == aubData) || (LINETOOL_IPOD == aubData)) {

			aubResult = wubLineToolApp_RPCSend(ARPC_ID_LINKMGR, EV_REQ_LINETOOL_CPL_IPOD_SEL, &aubData, LINETOOL_NUM_1);
            if(RES_OK == aubResult) {
				aubReturn = PE_SEC0;
			} else {
                aubReturn = PE_SEC1;
			}
		} else {
			/* No Action */
		}
	} else {
		/* No Action */
	}
	return aubReturn;
}

/*********************************************************************************************
*  function 				: nubLineToolIpodCarplayChgCB
*  date 					: 2021.12.08
*  autho					: -
*  description				: Ipod/Carplay Chg CB
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.		 date			revised by		  description
*
*********************************************************************************************/
static Type_uByte nubLineToolIpodCarplayChgCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_CPL_IPOD_SEL == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }

    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;

}

/*********************************************************************************************
*  function 				: nubLineToolCarplayTrack
*  date 					: 2021.10.26
*  autho					: -
*  description				: Carplay Up/Down
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.		 date			revised by		  description
*
*********************************************************************************************/
static Type_uByte nubLineToolCarplayTrack(void) {

    /* Auto variable Define */
    Type_uByte aubResult;
	Type_uByte aubReturn;
	Type_uByte aubData;
    Type_uByte *aubDataBuff_p;

     /* Initalize */
	aubResult = RES_NG;
	aubReturn = PE_SEC1;
    aubData = LINETOOL_NUM_0;
    aubDataBuff_p = OS_NULL;

	/* send data to RPC*/
	if ((LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {

		aubDataBuff_p = (Type_uByte*) nstLineToolAppStdQueue.vdQueueBuffer_p;
        aubData = aubDataBuff_p[LINETOOL_NUM_2];
        if((LINETOOL_TRACK_DOWN == aubData) || (LINETOOL_TRACK_UP == aubData)) {

			aubResult = wubLineToolApp_RPCSend(ARPC_ID_HMI, EV_REQ_LINETOOL_CPL_TRACK_CHG, &aubData, LINETOOL_NUM_1);
            if(RES_OK == aubResult) {
				aubReturn = PE_SEC0;
			} else {
                aubReturn = PE_SEC1;
			}
		} else {
			/* No Action */
		}
	} else {
		/* No Action */
	}
	return aubReturn;
}

/*********************************************************************************************
*  function 				: nubLineToolCarplayTrackCB
*  date 					: 2021.12.08
*  autho					: -
*  description				: Carplay Up/Down CB
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.		 date			revised by		  description
*
*********************************************************************************************/
static Type_uByte nubLineToolCarplayTrackCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_CPL_TRACK_CHG == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }

    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;

}

/*********************************************************************************************
*  function                 : nubLineToolLanguageIn
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              :
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolLanguageIn(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubResult = RES_NG;
    aubReturn = PE_SEC1;

    /* send data to RPC*/
    if (LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) {
        aubResult = wubLineToolApp_RPCSend(ARPC_ID_HMI, EV_REQ_LINETOOL_LANGUAGE_IN, OS_NULL, LINETOOL_NUM_0);

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            /* No Action */
        }
    } else {
        /* No Action */
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolLanguageInCB
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              :
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolLanguageInCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_LANGUAGE_IN == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }

    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;

}

/*********************************************************************************************
*  function                 : nubLineToolReadRDSInfo
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              :
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/

static Type_uByte nubLineToolReadRDSInfo(void) {
    /* Auto variable Define */
    Type_uByte aubResult;
	Type_uByte aubReturn;
	Type_uByte aubData;
    Type_uByte *aubDataBuff_p;

     /* Initalize */
	aubResult = RES_NG;
	aubReturn = PE_SEC1;
    aubData = LINETOOL_NUM_0;
    aubDataBuff_p = OS_NULL;

	if ((LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {

		aubDataBuff_p = (Type_uByte*) nstLineToolAppStdQueue.vdQueueBuffer_p;
        aubData = aubDataBuff_p[LINETOOL_NUM_2];
        if(LINETOOL_RDS_PI == aubData) {
            aubResult = wubLineToolApp_RPCSend(ARPC_ID_TUNER, EV_REQ_LINETOOL_GET_RDS_PI, OS_NULL, LINETOOL_NUM_0);
             if(RES_OK == aubResult) {
				aubReturn = PE_SEC0;
			} else {
                aubReturn = PE_SEC1;
			}
        }else if (LINETOOL_RDS_PS == aubData){
            aubResult = wubLineToolApp_RPCSend(ARPC_ID_TUNER, EV_REQ_LINETOOL_GET_RDS_PS, OS_NULL, LINETOOL_NUM_0);
            if(RES_OK == aubResult) {
				aubReturn = PE_SEC0;
			} else {
                aubReturn = PE_SEC1;
			}
        }else if(LINETOOL_RDS_PTY == aubData) {
			aubResult = wubLineToolApp_RPCSend(ARPC_ID_TUNER, EV_REQ_LINETOOL_GET_RDS_PTY, OS_NULL, LINETOOL_NUM_0);
            if(RES_OK == aubResult) {
				aubReturn = PE_SEC0;
			} else {
                aubReturn = PE_SEC1;
			}
		} else {
			/* No Action */
		}
	} else {
		/* No Action */
	}
	return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolReadRDSInfoCB
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              :
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/

static Type_uByte nubLineToolReadRDSInfoCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubData[LINETOOL_NUM_4];
    Type_uByte aubDataBuffer[LINETOOL_NUM_9];

    /* Initalize */
    aubReturn = PE_SEC1;
    wvdGen_MemSet(aubData, LINETOOL_NUM_0, sizeof(aubData));
    wvdGen_MemSet(aubDataBuffer, LINETOOL_NUM_0, sizeof(aubDataBuffer));

    if (LINETOOL_INF_GET_RDS_PI == nstLineToolAppStdQueue.uhEventCode) {
        if ((OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p) && (LINETOOL_NUM_3 == nstLineToolAppStdQueue.uwQueueSize)) {
            wvdGen_MemCpy(&aubDataBuffer, nstLineToolAppStdQueue.vdQueueBuffer_p,LINETOOL_NUM_3);
            if (RES_OK == aubDataBuffer[LINETOOL_NUM_0]) {
                aubData[LINETOOL_NUM_2] = aubDataBuffer[LINETOOL_NUM_1];
                aubData[LINETOOL_NUM_3] = aubDataBuffer[LINETOOL_NUM_2];
                nvdLineToolAppCmdReseponse(LINETOOL_RES_DATA, &aubData[LINETOOL_NUM_0], LINETOOL_NUM_4);
                aubReturn = PE_SEC0;
            } else {
                /* No Action */
            }
        } else {
            /* No Action */
        }
    } else if (LINETOOL_INF_GET_RDS_PS == nstLineToolAppStdQueue.uhEventCode) {
        if ((OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p) && (LINETOOL_NUM_9 == nstLineToolAppStdQueue.uwQueueSize)) {
            wvdGen_MemCpy(&aubDataBuffer, nstLineToolAppStdQueue.vdQueueBuffer_p, LINETOOL_NUM_9);
            if (RES_OK == aubDataBuffer[LINETOOL_NUM_0]) {
                nvdLineToolAppCmdReseponse(LINETOOL_RES_DATA, &aubDataBuffer[LINETOOL_NUM_1], LINETOOL_NUM_8);
                aubReturn = PE_SEC0;
            } else {
                /* No Action */
            }
        } else {
            /* No Action */
        }
    } else if (LINETOOL_INF_GET_RDS_PTY == nstLineToolAppStdQueue.uhEventCode) {
        if ((OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p) && (LINETOOL_NUM_2 == nstLineToolAppStdQueue.uwQueueSize)) {
            wvdGen_MemCpy(&aubDataBuffer, nstLineToolAppStdQueue.vdQueueBuffer_p, LINETOOL_NUM_2);
            if (RES_OK == aubDataBuffer[LINETOOL_NUM_0]) {              
                aubData[LINETOOL_NUM_3] = aubDataBuffer[LINETOOL_NUM_1];                
                nvdLineToolAppCmdReseponse(LINETOOL_RES_DATA, &aubData[LINETOOL_NUM_0], LINETOOL_NUM_4);
                aubReturn = PE_SEC0;
            } else {
                /* No Action */
            }
        } else {
            /* No Action */
        }
    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolGetRDSLevel
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              :
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/

static Type_uByte nubLineToolGetRDSLevel(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubResult = RES_NG;
    aubReturn = PE_SEC1;

    /* send data to RPC*/
    if (LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) {
        aubResult = wubLineToolApp_RPCSend(ARPC_ID_TUNER, EV_REQ_LINETOOL_GET_RDS_LEVEL, OS_NULL, LINETOOL_NUM_0);

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            /* No Action */
        }
    } else {
        /* No Action */
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolGetRDSLevelCB
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              :
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolGetRDSLevelCB(void) {
    Type_uByte aubReturn;
	Type_uByte aubData[LINETOOL_NUM_5];    // changed by cxx on 20220620 for add 00

	aubReturn = PE_SEC1;
	wvdGen_MemSet(&aubData[LINETOOL_NUM_0], LINETOOL_NUM_0, sizeof(aubData));

	if(LINETOOL_INF_GET_RDS_LEVEL == nstLineToolAppStdQueue.uhEventCode) {

        if ((OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p) && (LINETOOL_NUM_2 == nstLineToolAppStdQueue.uwQueueSize)) {

            wvdGen_MemCpy(&aubData[LINETOOL_NUM_0], nstLineToolAppStdQueue.vdQueueBuffer_p, LINETOOL_NUM_2);

            if (RES_OK == aubData[LINETOOL_NUM_0]) {

                nvdLineToolAppCmdReseponse(LINETOOL_RES_DATA, &aubData[LINETOOL_NUM_1], LINETOOL_NUM_4);    // changed by cxx on 20220620 for add 00
                aubReturn = PE_SEC0;
            } else {
                /* No Action */
            }
        } else {
            /* No Action */
        }
	} else if(LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
		aubReturn = PE_SEC1;
	} else {
		aubReturn = PE_SEC_CONTINUE;
	}
	return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolGetTunerSM
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              :
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolGetTunerSM(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubResult = RES_NG;
    aubReturn = PE_SEC1;

    /* send data to RPC*/
    if (LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) {
        aubResult = wubLineToolApp_RPCSend(ARPC_ID_HMI, EV_REQ_LINETOOL_GET_TUNER_SM, OS_NULL, LINETOOL_NUM_0);

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            /* No Action */
        }
    } else {
        /* No Action */
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolGetTunerSMCB
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              :
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolGetTunerSMCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
	Type_uByte aubData[LINETOOL_NUM_5]; // changed by cxx on 20220609 for send db data

    /* Initalize */
	aubReturn = PE_SEC1;
	wvdGen_MemSet(&aubData[LINETOOL_NUM_0], LINETOOL_NUM_0, sizeof(aubData));

	if((LINETOOL_INF_GET_TUNER_SM == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {

        if ((OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p) && (LINETOOL_NUM_2 == nstLineToolAppStdQueue.uwQueueSize)) {

            wvdGen_MemCpy(&aubData[LINETOOL_NUM_0], nstLineToolAppStdQueue.vdQueueBuffer_p, LINETOOL_NUM_2);
            if (RES_OK == aubData[LINETOOL_NUM_0]) {
                nvdLineToolAppCmdReseponse(LINETOOL_RES_DATA, &aubData[LINETOOL_NUM_1], LINETOOL_NUM_4); // changed by cxx on 20220609 for send db data
                aubReturn = PE_SEC0;
            } else {
                /* No Action */
            }
        } else {
            /* No Action */
        }
	} else if(LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
		aubReturn = PE_SEC1;
	} else {
		aubReturn = PE_SEC_CONTINUE;
	}

	return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolBTLevelDiag
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              :
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolBTLevelDiag(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubResult = RES_NG;
    aubReturn = PE_SEC1;

    /* send data to RPC*/
    if (LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) {
        aubResult = wubLineToolApp_RPCSend(ARPC_ID_BT, EV_REQ_LINETOOL_BT_LEVEL_DIAG, OS_NULL, LINETOOL_NUM_0);

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            /* No Action */
        }
    } else {
        /* No Action */
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolBTLevelDiagCB
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              :
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolBTLevelDiagCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubData[LINETOOL_NUM_5];// changed by cxx on 20220607 for #71055

    /* Initalize */
    aubReturn = PE_SEC1;
    wvdGen_MemSet(&aubData[LINETOOL_NUM_0],LINETOOL_NUM_0,sizeof(aubData));

    if (LINETOOL_INF_BT_LEVEL_DIAG == nstLineToolAppStdQueue.uhEventCode) {

        if ((OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p) && (LINETOOL_NUM_2 == nstLineToolAppStdQueue.uwQueueSize)) {    // changed by cxx on 20220607 for #71055
        // if ((OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p) && (LINETOOL_NUM_3 == nstLineToolAppStdQueue.uwQueueSize)) {

            wvdGen_MemCpy(aubData, nstLineToolAppStdQueue.vdQueueBuffer_p, LINETOOL_NUM_2);// changed by cxx on 20220607 for #71055
            if (RES_OK == aubData[LINETOOL_NUM_0]) {
                nvdLineToolAppCmdReseponse(LINETOOL_RES_DATA, &aubData[LINETOOL_NUM_1], LINETOOL_NUM_4);// changed by cxx on 20220607 for #71055
                aubReturn = PE_SEC0;
            } else {
                /* No Action */
            }
        } else {
            /* No Action */
        }
    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolBTInfoClear
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              :
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolBTInfoClear(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubResult = RES_NG;
    aubReturn = PE_SEC1;

    /* send data to RPC*/
    if (LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) {
        aubResult = wubLineToolApp_RPCSend(ARPC_ID_BT, EV_REQ_LINETOOL_BT_INFO_CLEAR, OS_NULL, LINETOOL_NUM_0);

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            /* No Action */
        }
    } else {
        /* No Action */
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolBTInfoClearCB
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              :
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolBTInfoClearCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_BT_INFO_CLEAR == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }

    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;

}

/*********************************************************************************************
*  function                 : nubLineToolCameraCheck
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              :
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolCameraCheck(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubResult = RES_NG;
    aubReturn = PE_SEC1;

    /* send data to RPC*/
    if (LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) {
        aubResult = wubLineToolApp_RPCSend(ARPC_ID_REARCAMERA, EV_REQ_LINETOOL_CAMERA_CHECK, OS_NULL, LINETOOL_NUM_0);

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            /* No Action */
        }
    } else {
        /* No Action */
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolCameraCheckCB
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              :
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolCameraCheckCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_CAMERA_CHECK == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {

        wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));
        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }

    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolTPTerminalSet
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : TP Terminal Set
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolTPTerminalSet(void) {
    Type_uByte aubReturn;
    Type_uByte aubResult;
    Type_uByte aubData;
    Type_uByte *aubDataBuff_p;

    aubReturn = PE_SEC1;
    aubResult = RES_NG;
    aubData = LINETOOL_NUM_0;
    aubDataBuff_p = OS_NULL;

    if ((LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {

        aubDataBuff_p = (Type_uByte *)nstLineToolAppStdQueue.vdQueueBuffer_p;
        aubData = aubDataBuff_p[LINETOOL_NUM_2];

        if ((LINETOOL_SET_CLK_OFF == aubData) || (LINETOOL_SET_CLK_ON == aubData)) {
            aubResult = wubLineToolApp_RPCSend(ARPC_ID_CLOCK, EV_REQ_LINETOOL_SET_CLOCK_TERMINAL, &aubData, LINETOOL_NUM_1);
            if (RES_OK == aubResult) {
                aubReturn = PE_SEC0;
            } else {
                /* No Action */
            }
        }
        else{
            /* No Action */
        }
    }
    else{
        /* No Action */
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolTPTerminalSetCB
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : TP Terminal Set
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolTPTerminalSetCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_SET_CLOCK_TERMINAL == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }

    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;

}

/*********************************************************************************************
*  function                 : nubLineToolKeyCheckOnOff
*  date                     : 2021.12.10
*  autho                    : liujie
*  description              : Key Check Mode On Off Change
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolKeyCheckOnOff(void) {

    /* Auto variable Define */
    Type_uByte aubResult;
	Type_uByte aubReturn;
	Type_uByte aubData;
    Type_uByte *aubDataBuff_p;

     /* Initalize */
	aubResult = RES_NG;
	aubReturn = PE_SEC1;
    aubData = LINETOOL_NUM_0;
    aubDataBuff_p = OS_NULL;

	/* send data to RPC*/
	if ((LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {

		aubDataBuff_p = (Type_uByte*) nstLineToolAppStdQueue.vdQueueBuffer_p;
        aubData = aubDataBuff_p[LINETOOL_NUM_2];
        if((LINETOOL_KEY_QUIT == aubData) || (LINETOOL_KEY_ENTER == aubData)) {

			aubResult = wubLineToolApp_RPCSend(ARPC_ID_HMI, EV_REQ_LINETOOL_KEY_CHECK_ONOFF, &aubData, LINETOOL_NUM_1);
            if(RES_OK == aubResult) {
				aubReturn = PE_SEC0;
			} else {
                aubReturn = PE_SEC1;
			}
		} else {
			/* No Action */
		}
	} else {
		/* No Action */
	}
	return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolKeyCheckOnOffCB
*  date                     : 2021.12.10
*  autho                    : liujie
*  description              : Key Check Mode On Off Change
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolKeyCheckOnOffCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_KEY_CHECK_ONOFF == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }

    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolILLCheck
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              :
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolILLCheck(void) {
    Type_uByte aubReturn;
	Type_uByte aubData;

	aubData = LINETOOL_NUM_0;
    aubReturn = PE_SEC1;

    if (LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize){

		aubData = wubSnr_GetILL();
		if(LINETOOL_ILL_ON == aubData)
		{
			aubReturn = PE_SEC0;
		}
		else
		{
			aubReturn = PE_SEC1;
		}
    }
    else{
        /* No Action */
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolEnglishSet
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              :
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolEnglishSet(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubResult = RES_NG;
    aubReturn = PE_SEC1;

    /* send data to RPC*/
    if (LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) {
        aubResult = wubLineToolApp_RPCSend(ARPC_ID_HMI, EV_REQ_LINETOOL_SET_ENGLISH, OS_NULL, LINETOOL_NUM_0);

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            /* No Action */
        }
    } else {
        /* No Action */
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolEnglishSetCB
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              :
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolEnglishSetCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_SET_ENGLISH == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }

    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;

}
/*********************************************************************************************
*  function                 : nubLineToolVariationGet
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              :
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolVariationGet(void)
{
	//TODO:nubLineToolVariationGet
    Type_uByte aubReturn;
	Type_uByte auhLineToolVarData;
	Type_uByte aubReadBuff[LINETOOL_NUM_10];

	aubReturn = PE_SEC1;
	auhLineToolVarData = LINETOOL_NUM_0;
	wvdGen_MemSet(&aubReadBuff[LINETOOL_NUM_0], LINETOOL_NUM_0, sizeof(aubReadBuff));
    
	if (LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) {
        auhLineToolVarData = wubVariation_CarType_Get();
        //printf("auhLineToolVarData = [%02X]\n",auhLineToolVarData); //delete by cxx on 20220421 for LOG
		aubReturn = PE_SEC0;
	}
	if (PE_SEC0 == aubReturn) {
        switch (auhLineToolVarData){
            
            case LINETOOL_RH00488A01:
            //printf("LINETOOL_RH00488A01\n");     //delete by cxx on 20220421 for LOG
                aubReadBuff[LINETOOL_NUM_0] = LINETOOL_ASCII_NUM_R;
                aubReadBuff[LINETOOL_NUM_1] = LINETOOL_ASCII_NUM_H;
                aubReadBuff[LINETOOL_NUM_2] = LINETOOL_ASCII_NUM_0;
                aubReadBuff[LINETOOL_NUM_3] = LINETOOL_ASCII_NUM_0;
                aubReadBuff[LINETOOL_NUM_4] = LINETOOL_ASCII_NUM_4;
                aubReadBuff[LINETOOL_NUM_5] = LINETOOL_ASCII_NUM_8;
                aubReadBuff[LINETOOL_NUM_6] = LINETOOL_ASCII_NUM_8;
                aubReadBuff[LINETOOL_NUM_7] = LINETOOL_ASCII_NUM_A;
                aubReadBuff[LINETOOL_NUM_8] = LINETOOL_ASCII_NUM_0;
                aubReadBuff[LINETOOL_NUM_9] = LINETOOL_ASCII_NUM_1;
                break;
            case LINETOOL_RH00545A01:
            //printf("LINETOOL_RH00545A01\n");   //delete by cxx on 20220421 for LOG
                aubReadBuff[LINETOOL_NUM_0] = LINETOOL_ASCII_NUM_R;
                aubReadBuff[LINETOOL_NUM_1] = LINETOOL_ASCII_NUM_H;
                aubReadBuff[LINETOOL_NUM_2] = LINETOOL_ASCII_NUM_0;
                aubReadBuff[LINETOOL_NUM_3] = LINETOOL_ASCII_NUM_0;
                aubReadBuff[LINETOOL_NUM_4] = LINETOOL_ASCII_NUM_5;
                aubReadBuff[LINETOOL_NUM_5] = LINETOOL_ASCII_NUM_4;
                aubReadBuff[LINETOOL_NUM_6] = LINETOOL_ASCII_NUM_5;
                aubReadBuff[LINETOOL_NUM_7] = LINETOOL_ASCII_NUM_A;
                aubReadBuff[LINETOOL_NUM_8] = LINETOOL_ASCII_NUM_0;
                aubReadBuff[LINETOOL_NUM_9] = LINETOOL_ASCII_NUM_1;
                break;
            case LINETOOL_RH00485A01:
                //printf("LINETOOL_RH00485A01\n");   //delete by cxx on 20220421 for LOG
                aubReadBuff[LINETOOL_NUM_0] = LINETOOL_ASCII_NUM_R;
                aubReadBuff[LINETOOL_NUM_1] = LINETOOL_ASCII_NUM_H;
                aubReadBuff[LINETOOL_NUM_2] = LINETOOL_ASCII_NUM_0;
                aubReadBuff[LINETOOL_NUM_3] = LINETOOL_ASCII_NUM_0;
                aubReadBuff[LINETOOL_NUM_4] = LINETOOL_ASCII_NUM_4;
                aubReadBuff[LINETOOL_NUM_5] = LINETOOL_ASCII_NUM_8;
                aubReadBuff[LINETOOL_NUM_6] = LINETOOL_ASCII_NUM_5;
                aubReadBuff[LINETOOL_NUM_7] = LINETOOL_ASCII_NUM_A;
                aubReadBuff[LINETOOL_NUM_8] = LINETOOL_ASCII_NUM_0;
                aubReadBuff[LINETOOL_NUM_9] = LINETOOL_ASCII_NUM_1;
                break;
            // case LINETOOL_RH00546A01:    //delete by cxx on 20220412 for #73590
            //     //printf("LINETOOL_RH00546A01\n");   //delete by cxx on 20220421 for LOG              
            //     aubReadBuff[LINETOOL_NUM_0] = LINETOOL_ASCII_NUM_R;
            //     aubReadBuff[LINETOOL_NUM_1] = LINETOOL_ASCII_NUM_H;
            //     aubReadBuff[LINETOOL_NUM_2] = LINETOOL_ASCII_NUM_0;
            //     aubReadBuff[LINETOOL_NUM_3] = LINETOOL_ASCII_NUM_0;
            //     aubReadBuff[LINETOOL_NUM_4] = LINETOOL_ASCII_NUM_5;
            //     aubReadBuff[LINETOOL_NUM_5] = LINETOOL_ASCII_NUM_4;
            //     aubReadBuff[LINETOOL_NUM_6] = LINETOOL_ASCII_NUM_6;
            //     aubReadBuff[LINETOOL_NUM_7] = LINETOOL_ASCII_NUM_A;
            //     aubReadBuff[LINETOOL_NUM_8] = LINETOOL_ASCII_NUM_0;
            //     aubReadBuff[LINETOOL_NUM_9] = LINETOOL_ASCII_NUM_1;
            //     break;
            case LINETOOL_RH00353A01:    //add RH00353A01 by cxx on 20220620 for #73590      
                aubReadBuff[LINETOOL_NUM_0] = LINETOOL_ASCII_NUM_R;
                aubReadBuff[LINETOOL_NUM_1] = LINETOOL_ASCII_NUM_H;
                aubReadBuff[LINETOOL_NUM_2] = LINETOOL_ASCII_NUM_0;
                aubReadBuff[LINETOOL_NUM_3] = LINETOOL_ASCII_NUM_0;
                aubReadBuff[LINETOOL_NUM_4] = LINETOOL_ASCII_NUM_3;
                aubReadBuff[LINETOOL_NUM_5] = LINETOOL_ASCII_NUM_5;
                aubReadBuff[LINETOOL_NUM_6] = LINETOOL_ASCII_NUM_3;
                aubReadBuff[LINETOOL_NUM_7] = LINETOOL_ASCII_NUM_A;
                aubReadBuff[LINETOOL_NUM_8] = LINETOOL_ASCII_NUM_0;
                aubReadBuff[LINETOOL_NUM_9] = LINETOOL_ASCII_NUM_1;
                break;
            case LINETOOL_RH00547A01:   //add RH00547A01 by cxx on 20220620 for #73590
                aubReadBuff[LINETOOL_NUM_0] = LINETOOL_ASCII_NUM_R;
                aubReadBuff[LINETOOL_NUM_1] = LINETOOL_ASCII_NUM_H;
                aubReadBuff[LINETOOL_NUM_2] = LINETOOL_ASCII_NUM_0;
                aubReadBuff[LINETOOL_NUM_3] = LINETOOL_ASCII_NUM_0;
                aubReadBuff[LINETOOL_NUM_4] = LINETOOL_ASCII_NUM_5;
                aubReadBuff[LINETOOL_NUM_5] = LINETOOL_ASCII_NUM_4;
                aubReadBuff[LINETOOL_NUM_6] = LINETOOL_ASCII_NUM_7;
                aubReadBuff[LINETOOL_NUM_7] = LINETOOL_ASCII_NUM_A;
                aubReadBuff[LINETOOL_NUM_8] = LINETOOL_ASCII_NUM_0;
                aubReadBuff[LINETOOL_NUM_9] = LINETOOL_ASCII_NUM_1;
                break;
            
            default:
                break;
        }
        nvdLineToolAppCmdReseponse(LINETOOL_RES_DATA, &aubReadBuff[LINETOOL_NUM_0], LINETOOL_NUM_10);
	} else {
	    /* No Action */
	}


    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolVariationGetCB
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              :
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
// static Type_uByte nubLineToolVariationGetCB(void)    /*0228 lj*/
// {
// 	// TODO:nubLineToolVariationGetCB
//     Type_uByte aubReturn;

// 	aubReturn = PE_SEC1;

// 	return aubReturn;
// }


/*********************************************************************************************
*  function                 : nubLineToolCarplayOnOff
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              :
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolCarplayOnOff(void) {

    /* Auto variable Define */
    Type_uByte aubResult;
	Type_uByte aubReturn;
	Type_uByte aubData;
    Type_uByte *aubDataBuff_p;

     /* Initalize */
	aubResult = RES_NG;
	aubReturn = PE_SEC1;
    aubData = LINETOOL_NUM_0;
    aubDataBuff_p = OS_NULL;

	/* send data to RPC*/
	if ((LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {

		aubDataBuff_p = (Type_uByte*) nstLineToolAppStdQueue.vdQueueBuffer_p;
        aubData = aubDataBuff_p[LINETOOL_NUM_2];
        if((LINETOOL_NUM_0 == aubData) || (LINETOOL_NUM_1 == aubData)) {

			aubResult = wubLineToolApp_RPCSend(ARPC_ID_HMI, EV_REQ_LINETOOL_CPL_ON_OFF, &aubData, LINETOOL_NUM_1);
            if(RES_OK == aubResult) {
				aubReturn = PE_SEC0;
			} else {
                aubReturn = PE_SEC1;
			}
		} else {
			/* No Action */
		}
	} else {
		/* No Action */
	}
	return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolCarplayOnOffCB
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              :
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/

static Type_uByte nubLineToolCarplayOnOffCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_CPL_ON_OFF == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }

    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolAAPOnOff
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              :
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolAAPOnOff(void) {

    /* Auto variable Define */
    Type_uByte aubResult;
	Type_uByte aubReturn;
	Type_uByte aubData;
    Type_uByte *aubDataBuff_p;

     /* Initalize */
	aubResult = RES_NG;
	aubReturn = PE_SEC1;
    aubData = LINETOOL_NUM_0;
    aubDataBuff_p = OS_NULL;

	/* send data to RPC*/
	if ((LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {

		aubDataBuff_p = (Type_uByte*) nstLineToolAppStdQueue.vdQueueBuffer_p;
        aubData = aubDataBuff_p[LINETOOL_NUM_2];
        if((LINETOOL_NUM_0 == aubData) || (LINETOOL_NUM_1 == aubData)) {

			aubResult = wubLineToolApp_RPCSend(ARPC_ID_HMI, EV_REQ_LINETOOL_AAP_ON_OFF, &aubData, LINETOOL_NUM_1);
            if(RES_OK == aubResult) {
				aubReturn = PE_SEC0;
			} else {
                aubReturn = PE_SEC1;
			}
		} else {
			/* No Action */
		}
	} else {
		/* No Action */
	}
	return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolAAPOnOffCB
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              :
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/

static Type_uByte nubLineToolAAPOnOffCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_AAP_ON_OFF == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }

    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;

}

/*********************************************************************************************
*  function                 : nubLineToolSpeedPlus
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              :
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolSpeedPlus(void) {

    Type_uByte aubReturn;
	Type_uHWord auhLineToolVspData;
	Type_uByte aubReadBuff[LINETOOL_NUM_4];

	aubReturn = PE_SEC1;
	auhLineToolVspData = LINETOOL_NUM_0;
	wvdGen_MemSet(&aubReadBuff[LINETOOL_NUM_0], LINETOOL_NUM_0, sizeof(aubReadBuff));

	if (LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) {

        auhLineToolVspData = wuhSnr_GetVsp();
		aubReturn = PE_SEC0;
	}

	if (PE_SEC0 == aubReturn) {

		aubReadBuff[LINETOOL_NUM_0] = (Type_uByte)((auhLineToolVspData & 0xFF00) >> LINETOOL_NUM_8);
		aubReadBuff[LINETOOL_NUM_1] = (Type_uByte)(auhLineToolVspData & 0x00FF);
		nvdLineToolAppCmdReseponse(LINETOOL_RES_DATA, &aubReadBuff[LINETOOL_NUM_0], LINETOOL_NUM_4);

	} else {
	    /* No Action */
	}

	return aubReturn;
}


/*********************************************************************************************
*  function 				: nubLineToolWifiSSIDSet
*  date 					: 2021.12.08
*  autho					: -
*  description				: Wifi SSID Change
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.		 date			revised by		  description
*
*********************************************************************************************/
static Type_uByte nubLineToolWifiSSIDSet(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;
    Type_uByte *aubDataBuff_p;
    Type_uByte aubSendData[LINETOOL_NUM_8];

    /* Initalize */
    aubResult = RES_NG;
    aubReturn = PE_SEC1;
    aubDataBuff_p =OS_NULL;
    wvdGen_MemSet(aubSendData, LINETOOL_NUM_0, sizeof(aubSendData));

    /* send data to RPC*/
    if((LINETOOL_NUM_11 == nstLineToolAppStdQueue.uwQueueSize) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        aubDataBuff_p = (Type_uByte * )nstLineToolAppStdQueue.vdQueueBuffer_p;
        wvdGen_MemCpy(aubSendData, &aubDataBuff_p[LINETOOL_NUM_2], sizeof(aubSendData));

		aubResult = wubLineToolApp_RPCSend(ARPC_ID_WIFI_MW, EV_REQ_LINETOOL_WIFI_SET_SSID, aubSendData, LINETOOL_NUM_8);
        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            /* No Action */
        }
    } else {
        /* No Action */
    }
    return aubReturn;
}

/*********************************************************************************************
*  function 				: nubLineToolWifiSSIDSetCB
*  date 					: 2021.12.08
*  autho					: -
*  description				: Wifi SSID Change CB
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.		 date			revised by		  description
*
*********************************************************************************************/
static Type_uByte nubLineToolWifiSSIDSetCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_WIFI_SET_SSID == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }

    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;

}

/*********************************************************************************************
*  function 				: nubLineToolWifiPassWordSet
*  date 					: 2021.12.08
*  autho					: -
*  description				: Wifi SSID Change
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.		 date			revised by		  description
*
*********************************************************************************************/
static Type_uByte nubLineToolWifiPassWordSet(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;
    Type_uByte *aubDataBuff_p;
    Type_uByte aubSendData[LINETOOL_NUM_8];

    /* Initalize */
    aubResult = RES_NG;
    aubReturn = PE_SEC1;
    aubDataBuff_p =OS_NULL;
    wvdGen_MemSet(aubSendData, LINETOOL_NUM_0, sizeof(aubSendData));

    /* send data to RPC*/
    if((LINETOOL_NUM_11 == nstLineToolAppStdQueue.uwQueueSize) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        aubDataBuff_p = (Type_uByte * )nstLineToolAppStdQueue.vdQueueBuffer_p;
        wvdGen_MemCpy(aubSendData, &aubDataBuff_p[LINETOOL_NUM_2], sizeof(aubSendData));

		aubResult = wubLineToolApp_RPCSend(ARPC_ID_WIFI_MW, EV_REQ_LINETOOL_WIFI_SET_PASSWORD, aubSendData, LINETOOL_NUM_8);
        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            /* No Action */
        }
    } else {
        /* No Action */
    }
    return aubReturn;
}

/*********************************************************************************************
*  function 				: nubLineToolWifiPassWordSetCB
*  date 					: 2021.12.08
*  autho					: -
*  description				: Wifi SSID Change CB
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.		 date			revised by		  description
*
*********************************************************************************************/
static Type_uByte nubLineToolWifiPassWordSetCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_WIFI_SET_PASSWORD == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }

    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;

}


/*********************************************************************************************
*  function 				: nubLineToolWifiClearInfo
*  date 					: 2021.10.26
*  autho					: -
*  description				:
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.		 date			revised by		  description
*
*********************************************************************************************/
static Type_uByte nubLineToolWifiClearInfo(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubResult = RES_NG;
    aubReturn = PE_SEC1;

    /* send data to RPC*/
    if (LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) {
        aubResult = wubLineToolApp_RPCSend(ARPC_ID_WIFI_MW, EV_REQ_LINETOOL_WIFI_CLEAR_INFO, OS_NULL, LINETOOL_NUM_0);

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            /* No Action */
        }
    } else {
        /* No Action */
    }

    return aubReturn;
}

/*********************************************************************************************
*  function 				: nubLineToolWifiClearInfoCB
*  date 					: 2021.12.08
*  autho					: -
*  description				:
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.		 date			revised by		  description
*
*********************************************************************************************/
static Type_uByte nubLineToolWifiClearInfoCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_WIFI_CLEAR_INFO == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }

    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;

}

/*********************************************************************************************
*  function 				: nubLineToolWifiReset
*  date 					: 2021.10.26
*  autho					: -
*  description				:
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.		 date			revised by		  description
*
*********************************************************************************************/
static Type_uByte nubLineToolWifiReset(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubResult = RES_NG;
    aubReturn = PE_SEC1;

    /* send data to RPC*/
    if (LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) {
        aubResult = wubLineToolApp_RPCSend(ARPC_ID_WIFI_MW, EV_REQ_LINETOOL_WIFI_RESET, OS_NULL, LINETOOL_NUM_0);

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            /* No Action */
        }
    } else {
        /* No Action */
    }

    return aubReturn;
}

/*********************************************************************************************
*  function 				: nubLineToolWifiResetCB
*  date 					: 2021.12.08
*  autho					: -
*  description				:
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.		 date			revised by		  description
*
*********************************************************************************************/
static Type_uByte nubLineToolWifiResetCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_WIFI_RESET == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }

    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;

}

/*********************************************************************************************
*  function 				: nubLineToolWifiConnNumGet
*  date 					: 2021.10.26
*  autho					: -
*  description				:
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.		 date			revised by		  description
*
*********************************************************************************************/
static Type_uByte nubLineToolWifiConnNumGet(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubResult = RES_NG;
    aubReturn = PE_SEC1;

    /* send data to RPC*/
    if (LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) {
        aubResult = wubLineToolApp_RPCSend(ARPC_ID_WIFI_MW, EV_REQ_LINETOOL_WIFI_GETCONNUM, OS_NULL, LINETOOL_NUM_0);

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            /* No Action */
        }
    } else {
        /* No Action */
    }

    return aubReturn;
}

/*********************************************************************************************
*  function 				: nubLineToolWifiConnNumGetCB
*  date 					: 2021.12.08
*  autho					: -
*  description				:
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.		 date			revised by		  description
*
*********************************************************************************************/
static Type_uByte nubLineToolWifiConnNumGetCB(void)
{
    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubData[LINETOOL_NUM_5]; //changed by cxx on 20220608 for send wifi connected number

    /* Initalize */
    aubReturn = PE_SEC1;
    wvdGen_MemSet(aubData,LINETOOL_NUM_0,sizeof(aubData));

    /* send data to RPC*/
    if (LINETOOL_INF_WIFI_GETCONNUM == nstLineToolAppStdQueue.uhEventCode) {

        if ((OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p) && (LINETOOL_NUM_2 == nstLineToolAppStdQueue.uwQueueSize)) {

            wvdGen_MemCpy(aubData, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(aubData));
            if (RES_OK == aubData[LINETOOL_NUM_0]) {
                nvdLineToolAppCmdReseponse(LINETOOL_RES_DATA,&aubData[LINETOOL_NUM_1],LINETOOL_NUM_4); //changed by cxx on 20220608 for send wifi connected number(4byte)
                aubReturn = PE_SEC0;
            } else {
                /* No Action */
            }
        } else {
            /* No Action */
        }
    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode){
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;

}

/*********************************************************************************************
*  function 				: nubLineToolWifiConnectCheck
*  date 					: 2021.10.26
*  autho					: -
*  description				: WIFI Connect
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.		 date			revised by		  description
*
*********************************************************************************************/
static Type_uByte nubLineToolWifiConnectCheck(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;
    Type_uByte aubData[LINETOOL_NUM_6];
	Type_uByte *aubDataBuff_p;

    /* Initalize */
    aubResult = RES_NG;
    aubReturn = PE_SEC1;
    aubDataBuff_p = OS_NULL;
    wvdGen_MemSet(aubData, LINETOOL_NUM_0, sizeof(aubData));

    /* send data to RPC*/
    if ((LINETOOL_NUM_9 == nstLineToolAppStdQueue.uwQueueSize) && (OS_NULL !=nstLineToolAppStdQueue.vdQueueBuffer_p )) {
        aubDataBuff_p = (Type_uByte*)nstLineToolAppStdQueue.vdQueueBuffer_p;
        /* Wifi Mac Id*/
        wvdGen_MemCpy(aubData, &aubDataBuff_p[LINETOOL_NUM_2], sizeof(aubData));
        aubResult = wubLineToolApp_RPCSend(ARPC_ID_WIFI_MW, EV_REQ_LINETOOL_WIFI_CON_CHECK, aubData, sizeof(aubData));

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            /* No Action */
        }
    } else {
        /* No Action */
    }
    return aubReturn;
}

/*********************************************************************************************
*  function 				: nubLineToolWifiConnectCheckCB
*  date 					: 2021.12.08
*  autho					: -
*  description				: WIFI Connect CB
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.		 date			revised by		  description
*
*********************************************************************************************/
static Type_uByte nubLineToolWifiConnectCheckCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_WIFI_CON_CHECK == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }

    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}

/*********************************************************************************************
*  function 				: nubLineToolWifiModeSet
*  date 					: 2021.10.26
*  autho					: -
*  description				: WIFI Mode On/Off
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.		 date			revised by		  description
*
*********************************************************************************************/
static Type_uByte nubLineToolWifiModeSet(void) {

    /* Auto variable Define */
    Type_uByte aubResult;
	Type_uByte aubReturn;
	Type_uByte aubData;
    Type_uByte *aubDataBuff_p;

     /* Initalize */
	aubResult = RES_NG;
	aubReturn = PE_SEC1;
    aubData = LINETOOL_NUM_0;
    aubDataBuff_p = OS_NULL;

	/* send data to RPC*/
	if ((LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {

		aubDataBuff_p = (Type_uByte*) nstLineToolAppStdQueue.vdQueueBuffer_p;
        aubData = aubDataBuff_p[LINETOOL_NUM_2];
        if((LINETOOL_WIFI_MODE_ON == aubData) || (LINETOOL_WIFI_MODE_OFF == aubData)) {

			aubResult = wubLineToolApp_RPCSend(ARPC_ID_WIFI_MW, EV_REQ_LINETOOL_WIFI_MODE_ON_OFF, &aubData, LINETOOL_NUM_1);
            if(RES_OK == aubResult) {
				aubReturn = PE_SEC0;
			} else {
                aubReturn = PE_SEC1;
			}
		} else {
			/* No Action */
		}
	} else {
		/* No Action */
	}
	return aubReturn;
}

/*********************************************************************************************
*  function 				: nubLineToolWifiModeSetCB
*  date 					: 2021.12.08
*  autho					: -
*  description				: WIFI Mode On/Off CB
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.		 date			revised by		  description
*
*********************************************************************************************/
static Type_uByte nubLineToolWifiModeSetCB(void) {

    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_WIFI_MODE_ON_OFF == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }

    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}


/*********************************************************************************************
*  function 				: nubLineToolMacGet
*  date 					: 2021.10.26
*  autho					: -
*  description				: WIFI/BT MAC Read
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.		 date			revised by		  description
*
*********************************************************************************************/
static Type_uByte nubLineToolMacGet(void) {

    Type_uByte aubReturn;
    Type_uByte aubResult;
    Type_uByte aubData;
	Type_uByte *aubDataBuff_p;

    aubResult = RES_NG;
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;
    aubDataBuff_p =OS_NULL;

	if ((LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {    //0329 cxx for PCLintResult 514

		aubDataBuff_p = (Type_uByte * )nstLineToolAppStdQueue.vdQueueBuffer_p;
		aubData = aubDataBuff_p[LINETOOL_NUM_2];

        if(LINETOOL_WIFI_MAC == aubData) {
            aubResult = wubLineToolApp_RPCSend(ARPC_ID_WIFI_MW, EV_REQ_LINETOOL_WIFI_GET_MAC, OS_NULL, LINETOOL_NUM_0);
            if (RES_OK == aubResult) {
                aubReturn = PE_SEC0;
            } else {
                 /* No Action */
            }
        } else if(LINETOOL_BT_MAC == aubData) {
            aubResult = wubLineToolApp_RPCSend(ARPC_ID_BT, EV_REQ_LINETOOL_BT_GET_MAC, OS_NULL, LINETOOL_NUM_0);
            if (RES_OK == aubResult) {
                aubReturn = PE_SEC0;
            } else {
                 /* No Action */
            }
        } else {
            /* No Action */
        }
    } else {
        /* No Action */
    }
    return aubReturn;

}

/*********************************************************************************************
*  function 				: nubLineToolMacGetCB
*  date 					: 2021.12.08
*  autho					: -
*  description				: WIFI/BT MAC Read CB
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.		 date			revised by		  description
*
*********************************************************************************************/
static Type_uByte nubLineToolMacGetCB(void) {
	Type_uByte aubReturn;
	Type_uByte *aubDataBuff_p;

	aubReturn = PE_SEC1;
	aubDataBuff_p =OS_NULL;

	if ((LINETOOL_INF_WIFI_GET_MAC == nstLineToolAppStdQueue.uhEventCode) || (LINETOOL_INF_BT_GET_MAC == nstLineToolAppStdQueue.uhEventCode)) { // chaged by cxx on 20220426 for #68467. && -> ||

        if((OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p) && (LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize)) {

            aubDataBuff_p = (Type_uByte *)nstLineToolAppStdQueue.vdQueueBuffer_p;
			if (LINETOOL_NUM_0 == aubDataBuff_p[LINETOOL_NUM_0]) {	// add by cxx on 20220525 for get bt mac
				nvdLineToolAppCmdReseponse(LINETOOL_RES_DATA, &aubDataBuff_p[LINETOOL_NUM_1], LINETOOL_NUM_6);	//changed by cxx on 20220617 for send mac/bt data
				aubReturn = PE_SEC0;
			} else {
				aubReturn = PE_SEC1;
			}
		}
	} else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
		aubReturn = PE_SEC1;
	} else {
		aubReturn = PE_SEC_CONTINUE;
	}
	return aubReturn;
}

/*********************************************************************************************
*  function 				: nubLineToolGNSSGyroDataGet
*  date 					: 2021.12.08
*  autho					: -
*  description				: GYRO DATA OUTPUT
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.		 date			revised by		  description
*
*********************************************************************************************/
static Type_uByte nubLineToolGNSSGyroDataGet(void) {

    /* Auto variable Define */
    Type_uByte aubResult;
	Type_uByte aubReturn;
	Type_uByte aubData;
    Type_uByte *aubDataBuff_p;

     /* Initalize */
	aubResult = RES_NG;
	aubReturn = PE_SEC1;
    aubData = LINETOOL_NUM_0;
    aubDataBuff_p = OS_NULL;

	/* send data to RPC*/
	if ((LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {

		aubDataBuff_p = (Type_uByte*) nstLineToolAppStdQueue.vdQueueBuffer_p;
        aubData = aubDataBuff_p[LINETOOL_NUM_2];
        if((LINETOOL_GNSS_GYRO_X == aubData) || (LINETOOL_GNSS_GYRO_Y == aubData) || (LINETOOL_GNSS_GYRO_Z == aubData)) {

			aubResult = wubLineToolApp_RPCSend(ARPC_ID_GNSS, EV_REQ_LINETOOL_GNSS_GET_GYRO, &aubData, LINETOOL_NUM_1);	// changed by cxx on 20220505 for #69278
            if(RES_OK == aubResult) {
				aubReturn = PE_SEC0;
			} else {
                aubReturn = PE_SEC1;
			}
		} else {
			/* No Action */
		}
	} else {
		/* No Action */
	}
	return aubReturn;
}

/*********************************************************************************************
*  function 				: nubLineToolGNSSGyroDataGetCB
*  date 					: 2021.12.08
*  autho					: -
*  description				: GYRO DATA OUTPUT CB
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.		 date			revised by		  description
*
*********************************************************************************************/
static Type_uByte nubLineToolGNSSGyroDataGetCB(void)
{
    Type_uByte aubReturn;
    Type_uByte *aubDataBuff_p;
    Type_uByte aubSendData[LINETOOL_NUM_6];	// changed by cxx on 20220505 for #69278
    LineTool_GyroData_ts astGyroData;

    aubReturn = PE_SEC1;
    aubDataBuff_p =OS_NULL;
    wvdGen_MemSet(aubSendData, LINETOOL_NUM_0, sizeof(aubSendData));
    wvdGen_MemSet(&astGyroData, LINETOOL_NUM_0, sizeof(LineTool_GyroData_ts));	// changed by cxx on 20220601 for gnss

    if(LINETOOL_INF_GNSS_GET_GYRO == nstLineToolAppStdQueue.uhEventCode) {
        if((OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p) && (LINETOOL_NUM_8 == nstLineToolAppStdQueue.uwQueueSize)) {	// changed by cxx on 20220601 for gnss
            aubDataBuff_p = (Type_uByte *)nstLineToolAppStdQueue.vdQueueBuffer_p;
            wvdGen_MemCpy(&astGyroData, &aubDataBuff_p[LINETOOL_NUM_0], sizeof(LineTool_GyroData_ts));	// changed by cxx on 20220601 for gnss
            if (RES_OK == astGyroData.aubResult) {  // changed by cxx on 20220601 for gnss
                aubSendData[LINETOOL_NUM_0] = (Type_uByte)((astGyroData.auhGyro & 0xFF00) >> LINETOOL_NUM_8);
                aubSendData[LINETOOL_NUM_1] = (Type_uByte)(astGyroData.auhGyro & 0xFF);
                aubSendData[LINETOOL_NUM_3] = astGyroData.aubTemp;	// changed L9183-L9185 by cxx on 20220505 for #69278
                aubSendData[LINETOOL_NUM_4] = (Type_uByte)(astGyroData.auhAcc / LINETOOL_NUM_10);
                aubSendData[LINETOOL_NUM_5] = (Type_uByte)(astGyroData.auhAcc % LINETOOL_NUM_10);
                nvdLineToolAppCmdReseponse(LINETOOL_RES_DATA, aubSendData, sizeof(aubSendData));
                aubReturn = PE_SEC0;
            } else {
                aubReturn = PE_SEC1;
            }
        }
    }
    else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }
    return aubReturn;
}

/*********************************************************************************************
*  function 				: nubLineToolGNSSNaviDataGet
*  date 					: 2021.12.08
*  autho					: -
*  description				:
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.		 date			revised by		  description
*
*********************************************************************************************/
static Type_uByte nubLineToolGNSSNaviDataGet(void) {

    /* Auto variable Define */
    Type_uByte aubResult;
	Type_uByte aubReturn;
	Type_uByte aubData;
    Type_uByte *aubDataBuff_p;

     /* Initalize */
	aubResult = RES_NG;
	aubReturn = PE_SEC1;
    aubData = LINETOOL_NUM_0;
    aubDataBuff_p = OS_NULL;

	/* send data to RPC*/
	if ((LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {

		aubDataBuff_p = (Type_uByte*) nstLineToolAppStdQueue.vdQueueBuffer_p;
        aubData = aubDataBuff_p[LINETOOL_NUM_2];
        /* 00: get GPS detail info */
        if(LINETOOL_NUM_0 == aubData) {

			aubResult = wubLineToolApp_RPCSend(ARPC_ID_GNSS, EV_REQ_LINETOOL_GNSS_GET_NAVI, OS_NULL, LINETOOL_NUM_0);	// changed by cxx on 20220505 for #69281
            if(RES_OK == aubResult) {
				aubReturn = PE_SEC0;
			} else {
                aubReturn = PE_SEC1;
			}
		} else {
			/* No Action */
		}
	} else {
		/* No Action */
	}
	return aubReturn;
}

/*********************************************************************************************
*  function 				: nubLineToolGNSSNaviDataGetCB
*  date 					: 2021.12.08
*  autho					: -
*  description				:
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.		 date			revised by		  description
*
*********************************************************************************************/
static Type_uByte nubLineToolGNSSNaviDataGetCB(void)
{
    Type_uByte aubReturn;
    Type_uByte *aubDataBuff_p;
    Type_uByte aubSendData[LINETOOL_NUM_50];
    Type_uByte aubCnt;
    Type_uByte aubIndex;

    LineTool_NaviData_ts astNaviData;   //add by cxx on 20220601 for gnss

    aubReturn = PE_SEC1;
    aubDataBuff_p =OS_NULL;
    aubIndex = LINETOOL_NUM_0;
    aubCnt = LINETOOL_NUM_0;
    wvdGen_MemSet(&astNaviData, LINETOOL_NUM_0, sizeof(LineTool_NaviData_ts));	// add by cxx on 20220601 for gnss
    wvdGen_MemSet(&aubSendData, LINETOOL_NUM_0, sizeof(aubSendData));	// add by cxx on 20220601 for gnss
    if(LINETOOL_INF_GNSS_GET_NAVI == nstLineToolAppStdQueue.uhEventCode) {
        if((OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p) && (LINETOOL_NUM_62 == nstLineToolAppStdQueue.uwQueueSize)){	// changed by cxx on 20220601 for gnss
            aubDataBuff_p = (Type_uByte * )nstLineToolAppStdQueue.vdQueueBuffer_p;
            wvdGen_MemCpy(&astNaviData, &aubDataBuff_p[LINETOOL_NUM_0], sizeof(astNaviData));	// add by cxx on 20220601 for gnss
            if (RES_OK == astNaviData.aubResult) {// changed by cxx on 20220601 for gnss
                for(aubCnt = 0; aubCnt < 10; aubCnt++){	// add by cxx on 20220607 for #69278
                    aubSendData[aubIndex++] = astNaviData.astNaviData[aubCnt].aubStaNum;
                    aubSendData[aubIndex++] = astNaviData.astNaviData[aubCnt].aubEL;
                    aubSendData[aubIndex++] = astNaviData.astNaviData[aubCnt].aubCN;
                    aubSendData[aubIndex++] = astNaviData.astNaviData[aubCnt].aubAZI >> LINETOOL_NUM_8;
                    aubSendData[aubIndex++] = astNaviData.astNaviData[aubCnt].aubAZI;                    
                }
                nvdLineToolAppCmdReseponse(LINETOOL_RES_DATA, aubSendData, LINETOOL_NUM_50);	// changed by cxx on 20220601 for gnss
                aubReturn = PE_SEC0;
            } else {
                aubReturn = PE_SEC1;
            }
        }
    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode){
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }
    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolKeyCheck
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : Key Check
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolKeyCheck(void)
{

    Type_uByte aubReturn;
    Type_uByte* aubDataBuff_p;
    Type_uByte aubKeyCheckState;
    Type_uByte aubResult;

    aubReturn = PE_SEC1;
    aubDataBuff_p = OS_NULL;
    aubKeyCheckState = LINETOOL_KEY_MODE_OFF;
    aubResult = RES_NG;

    if (nstLineToolAppStdQueue.uwQueueSize == LINETOOL_NUM_7) {
        aubDataBuff_p = (Type_uByte * )nstLineToolAppStdQueue.vdQueueBuffer_p;
        aubKeyCheckState = aubDataBuff_p[LINETOOL_NUM_2];
        if (LINETOOL_KEY_MODE_OFF == aubKeyCheckState) {

            nubLineToolKeyMode = LINETOOL_KEY_MODE_OFF;
			aubResult = wubLineToolApp_RPCSend(ARPC_ID_R5_KEY, EVENT_CODE_LINETOOL_KEY_MODE_OFF, OS_NULL, LINETOOL_NUM_0);

        } else if (LINETOOL_KEY_MODE_ON == aubKeyCheckState) {

            nubLineToolKeyMode = LINETOOL_KEY_MODE_ON;
			aubResult = wubLineToolApp_RPCSend(ARPC_ID_R5_KEY, EVENT_CODE_LINETOOL_KEY_MODE_ON, OS_NULL, LINETOOL_NUM_0);

        } else if ((LINETOOL_KEY_CHECK == aubKeyCheckState) && (LINETOOL_KEY_MODE_ON == nubLineToolKeyMode)) {

			aubResult = wubLineToolApp_RPCSend(ARPC_ID_R5_KEY, EVENT_CODE_LINETOOL_KEY_MODE_CHECK, OS_NULL, LINETOOL_NUM_0);

        } else {
            /* No Action */
        }
        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            /* No Action */
        }
    } else {
        /* No Action */
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolKeyCheckCB
*  date                     : 2021.12.04
*  autho                    : liujie
*  description              : BT Audio 1kz Set CB
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolKeyCheckCB(void)
{
    Type_uByte aubReturn;
    Type_uByte aubKeyValue;
    Type_uByte aubData[LINETOOL_NUM_4];

    aubReturn = PE_SEC1;
    aubKeyValue = LINETOOL_NUM_0;
    wvdGen_MemSet(aubData, LINETOOL_NUM_0, sizeof(aubData));

    if (LINETOOL_INF_KEY_MODE_RES == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC0;
    } else if (LINETOOL_INF_KEY_VALUE_RES == nstLineToolAppStdQueue.uhEventCode) {
        if (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p) {
            wvdGen_MemCpy(&aubKeyValue, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(aubKeyValue));
            if (((HKY_KEY_TYPE_DAY_NIGHT <= aubKeyValue) && (HKY_KEY_TYPE_POWER >= aubKeyValue)) ||
                ((STK_KEY_TYPE_VOL_DOWN <= aubKeyValue) && (STK_KEY_TYPE_MODE >= aubKeyValue)) ||
                ((STK_KEY_TYPE_ON_HOOK <= aubKeyValue) && (STK_KEY_TYPE_TALK >= aubKeyValue))) {
                switch(aubKeyValue){
                    case HKY_KEY_TYPE_HOME       : aubData[LINETOOL_NUM_0] = LINETOOL_HOME;        break;
                    case HKY_KEY_TYPE_DAY_NIGHT  : aubData[LINETOOL_NUM_0] = LINETOOL_DAYNIGHT;    break;
                    case HKY_KEY_TYPE_BACK       : aubData[LINETOOL_NUM_0] = LINETOOL_BACK;        break;
                    case HKY_KEY_TYPE_VOL_UP     : aubData[LINETOOL_NUM_0] = LINETOOL_VOLUP;       break;
                    case HKY_KEY_TYPE_VOL_DOWN   : aubData[LINETOOL_NUM_0] = LINETOOL_VOLDOWN;     break;
                    case HKY_KEY_TYPE_POWER      : aubData[LINETOOL_NUM_0] = LINETOOL_POWER;       break;
                    case STK_KEY_TYPE_VOL_UP     : aubData[LINETOOL_NUM_0] = LINETOOL_STK_VOLUP;   break;
                    case STK_KEY_TYPE_VOL_DOWN   : aubData[LINETOOL_NUM_0] = LINETOOL_STK_VOLDOWN; break;
                    case STK_KEY_TYPE_ON_HOOK    : aubData[LINETOOL_NUM_0] = LINETOOL_ONHOOK;      break;
                    case STK_KEY_TYPE_OFF_HOOK   : aubData[LINETOOL_NUM_0] = LINETOOL_OFFHOOK;     break;
                    case STK_KEY_TYPE_TALK       : aubData[LINETOOL_NUM_0] = LINETOOL_TALK;        break;
                    case STK_KEY_TYPE_CH_UP      : aubData[LINETOOL_NUM_0] = LINETOOL_CHUP;        break;
                    case STK_KEY_TYPE_CH_DOWN    : aubData[LINETOOL_NUM_0] = LINETOOL_CHDOWN;      break;
                    case STK_KEY_TYPE_MODE       : aubData[LINETOOL_NUM_0] = LINETOOL_MODE;        break;
                    default: break;
                }
                nvdLineToolAppCmdReseponse(LINETOOL_RES_DATA, aubData, sizeof(aubData));
                aubReturn = PE_SEC0;
            } else {
                /* No Action */
            }
        } else {
            /* No Action */
        }
    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}

/*********************************************************************************************
*  function 				: nubLineToolScreenTouch
*  date 					: 2022.02.23
*  autho					: -
*  description				:
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.		 date			revised by		  description
*
*********************************************************************************************/
static Type_uByte nubLineToolScreenTouch(void) {

    /* Auto variable Define */
    Type_uByte aubResult;
	Type_uByte aubReturn;
    Type_uByte *aubDataBuff_p;
    TpctlTouchSimuInfo astLineToolTouchData;

     /* Initalize */
	aubResult = RES_NG;
	aubReturn = PE_SEC1;
    aubDataBuff_p = OS_NULL;

	/* send data to RPC*/
	if ((LINETOOL_NUM_10 == nstLineToolAppStdQueue.uwQueueSize) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {

		aubDataBuff_p = (Type_uByte*) nstLineToolAppStdQueue.vdQueueBuffer_p;
        astLineToolTouchData.auhTouchPos_x = aubDataBuff_p[LINETOOL_NUM_2] << LINETOOL_NUM_8 | aubDataBuff_p[LINETOOL_NUM_3];
        astLineToolTouchData.auhTouchPos_y = aubDataBuff_p[LINETOOL_NUM_4] << LINETOOL_NUM_8 | aubDataBuff_p[LINETOOL_NUM_5];
        astLineToolTouchData.auhTouchState = aubDataBuff_p[LINETOOL_NUM_6] << LINETOOL_NUM_8 | aubDataBuff_p[LINETOOL_NUM_7];
#ifndef LINETOOLAPP_DEBUG   //add by cxx on 20220421 for LOG
//TODO CHECK
        printf("x = [%02X]\n",astLineToolTouchData.auhTouchPos_x);
        printf("y = [%02X]\n",astLineToolTouchData.auhTouchPos_y);
        printf("s = [%02X]\n",astLineToolTouchData.auhTouchState);
//TODO END
#endif
		aubResult = wubLineToolApp_RPCSend(ARPC_ID_TP, EV_INF_TPCTL_TPSIMU_REQ, &astLineToolTouchData, sizeof(astLineToolTouchData));
        if(RES_OK == aubResult) {
			aubReturn = PE_SEC0;
		} else {
            aubReturn = PE_SEC1;
		}
	} else {
		/* No Action */
	}
	return aubReturn;
}

/*********************************************************************************************
*  function 				: nubLineToolScreenTouchCB
*  date 					: 2022.02.23
*  autho					: -
*  description				:
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.		 date			revised by		  description
*
*********************************************************************************************/
static Type_uByte nubLineToolScreenTouchCB(void)
{
    Type_uByte aubReturn;
    Type_uByte aubResult;

    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if((LINETOOL_INF_SCREENTOUCH_RES == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)){
        wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }
    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode){
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }
    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolTPSensorCheck
*  date                     : 2022.02.23
*  autho                    : liujie
*  description              : TP Sensor Check
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolTPSensorCheck(void) {
    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

	/* Initalize */
    aubResult = RES_NG;
    aubReturn = PE_SEC1;

    /* send data to RPC*/
    if (LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) {
		aubResult = wubLineToolApp_RPCSend(ARPC_ID_TP, EV_INF_TPCTL_SELF_TEST_REQ, OS_NULL, LINETOOL_NUM_0);

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            /* No Action */
        }
    } else {
        /* No Action */
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolTPSensorCheckCB
*  date                     : 2022.02.23
*  autho                    : liujie
*  description              : TP Sensor Check Callback
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolTPSensorCheckCB(void) {
    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;
    Type_uByte aubData[LINETOOL_NUM_4];

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;
    wvdGen_MemSet(aubData, LINETOOL_NUM_0, sizeof(aubData));

    if ((LINETOOL_INF_TP_SENSOR_CHECK_RES == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));    	
        if ((LINETOOL_TP_SENSOR_CHECK_OK == aubResult)||(LINETOOL_TP_SENSOR_CHECK_NG == aubResult)){
            aubData[LINETOOL_NUM_0] = (aubResult == RES_OK) ? LINETOOL_TP_SENSOR_CHECK_OK : LINETOOL_TP_SENSOR_CHECK_NG;
            nvdLineToolAppCmdReseponse(LINETOOL_RES_DATA,aubData,sizeof(aubData));
            aubReturn = PE_SEC0;
        }
    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode){
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLineToolMicAppoint
*  date                     : 2022.03.17
*  autho                    : liujie
*  description              : MIC Appoint
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolMicAppoint(void) {
    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;
    Type_uByte *aubDataBuff_p;
    Type_uByte aubData;

	/* Initalize */
    aubResult = RES_NG;
    aubReturn = PE_SEC1;
    aubDataBuff_p = OS_NULL;
    aubData = LINETOOL_NUM_0;

    /* send data to RPC*/
    if ((LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {

        aubDataBuff_p = (Type_uByte*) nstLineToolAppStdQueue.vdQueueBuffer_p;
        aubData = aubDataBuff_p[LINETOOL_NUM_2];
        if((LINETOOL_MIC_ONON == aubData) || (LINETOOL_MIC_ONOFF == aubData) || (LINETOOL_MIC_OFFON == aubData)) {
		    aubResult = wubLineToolApp_RPCSend(ARPC_ID_SOUNDAPP, EV_REQ_LINETOOL_MIC_APPOINT, &aubData, LINETOOL_NUM_1);

            if (RES_OK == aubResult) {
                aubReturn = PE_SEC0;
            } else {
                /* No Action */
            }
        }
    } else {
        /* No Action */
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolMicAppointCB
*  date                     : 2022.03.17
*  autho                    : liujie
*  description              : MIC Appoint Callback
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolMicAppointCB(void) {
    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_MICAPPOINT_RES == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));    	
        if (RES_OK == aubResult){
            aubReturn = PE_SEC0;
        }
    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode){
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLineToolTelMute
*  date                     : 2022.03.17
*  autho                    : liujie
*  description              : Tel Mute
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolTelMute(void) {
    //TODO:nubLineToolTelMute
    //printf("nubLineToolTelMute\n"); //delete by cxx on 20220421 for LOG
    /* Auto variable Define */
    Type_uByte aubReturn;
    // Type_uByte aubResult;
    Type_uByte aubData[LINETOOL_NUM_4];  // chaged by cxx on 20220412 for [Iss.B]#57078.

	/* Initalize */
    //aubResult = RES_NG;     // delete by cxx on 20220411 for [Iss.B]#57078.
    aubReturn = PE_SEC1;
    wvdGen_MemSet(aubData, LINETOOL_NUM_0, sizeof(aubData));      // chaged by cxx on 20220412 for [Iss.B]#57078.

    /* send data to RPC*/
    if (LINETOOL_NUM_7 == nstLineToolAppStdQueue.uwQueueSize) {

        aubData[LINETOOL_NUM_0] = wubSnr_GetTelMute();            // add by cxx on 20220412 for [Iss.B]#57078.
        // aubResult = wubLineToolApp_RPCSend(ARPC_ID_SENSOR, , &aubData, LINETOOL_NUM_1);  // delete by cxx on 20220411 for [Iss.B]#57078.
        // if (RES_OK == aubResult) {                                                       // delete by cxx on 20220411 for [Iss.B]#57078.
        if ((LINETOOL_TEL_MUTE_ON == aubData[LINETOOL_NUM_0])||(LINETOOL_TEL_MUTE_OFF == aubData[LINETOOL_NUM_0])) {    // add by cxx on 20220412 for [Iss.B]#57078.
            nvdLineToolAppCmdReseponse(LINETOOL_RES_DATA,aubData,sizeof(aubData));                                      // add by cxx on 20220412 for [Iss.B]#57078.
            aubReturn = PE_SEC0;
        } else {
            /* No Action */
        }   
    } else {
        /* No Action */
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLineToolTelMuteCB
*  date                     : 2022.03.17
*  autho                    : liujie
*  description              : Tel Mute Callback
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
// static Type_uByte nubLineToolTelMuteCB(void) {  // delete by cxx on 20220411 for [Iss.B]#57078.
    /* Auto variable Define */
    // Type_uByte aubReturn;
    // Type_uByte aubResult;
    // Type_uByte aubData[LINETOOL_NUM_4];

    /* Initalize */
    // aubReturn = PE_SEC0;
    // aubReturn = PE_SEC1;
    // aubResult = LINETOOL_NUM_0;
    // wvdGen_MemSet(aubData,LINETOOL_NUM_0, sizeof(aubData));

    // if ((LINETOOL_INF_TEL_MUTE_RES == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
    //     wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));    
    //     aubData[LINETOOL_NUM_0] = aubResult;
    //     nvdLineToolAppCmdReseponse(LINETOOL_RES_DATA,aubData,sizeof(aubData));
    //     if (RES_OK == aubResult){
    //         aubReturn = PE_SEC0;
    //     }
    // } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode){
    //     aubReturn = PE_SEC1;
    // } else {
    //     aubReturn = PE_SEC_CONTINUE;
    // }

    // return aubReturn;
// }

/*********************************************************************************************
*  function                 : nubLineToolMicSoundOn
*  date                     : 2022.04.28
*  autho                    : chen xiaoxu
*  description              : MIC MODE ON
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolMicSoundOn(void){
    // printf("[3] Sound On\n");
	/* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;
    Type_uByte aubData;

	/* Initalize */
	aubResult = RES_NG;
    aubReturn = PE_SEC1;
    aubData = LINETOOL_MIC_ONON;
	
    /* send data to RPC*/
        aubResult = wubLineToolApp_RPCSend(ARPC_ID_SOUNDAPP, EV_REQ_LINETOOL_MIC_APPOINT, &aubData, LINETOOL_NUM_1);

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            /* No Action */
        }
        
    return aubReturn; 
}
/*********************************************************************************************
*  function                 : nubLineToolMicSoundOnCB
*  date                     : 2022.04.28
*  autho                    : chen xiaoxu
*  description              : MIC MODE ON CB
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolMicSoundOnCB(void){
    // printf("[4] Sound On Callback\n");
    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_MICAPPOINT_RES == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }

    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }
   

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLineToolMicSoundOff
*  date                     : 2022.04.28
*  autho                    : chen xiaoxu
*  description              : MIC MODE OFF
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolMicSoundOff(void){
    // printf("[3] Sound Off\n");
	/* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;
    Type_uByte aubData;

	/* Initalize */
	aubResult = RES_NG;
    aubReturn = PE_SEC1;
    aubData = LINETOOL_MIC_OFFOFF;
	
    /* send data to RPC*/
        aubResult = wubLineToolApp_RPCSend(ARPC_ID_SOUNDAPP, EV_REQ_LINETOOL_MIC_APPOINT, &aubData, LINETOOL_NUM_1);

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            /* No Action */
        }
        
    return aubReturn; 
}
/*********************************************************************************************
*  function                 : nubLineToolMicSoundOffCB
*  date                     : 2022.04.28
*  autho                    : chen xiaoxu
*  description              : MIC MODE OFF CB
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
static Type_uByte nubLineToolMicSoundOffCB(void){
    // printf("[4] Sound Off Callback\n");
    /* Auto variable Define */
    Type_uByte aubReturn;
    Type_uByte aubResult;

    /* Initalize */
    // aubReturn = PE_SEC0;
    aubReturn = PE_SEC1;
    aubResult = LINETOOL_NUM_0;

    if ((LINETOOL_INF_MICAPPOINT_RES == nstLineToolAppStdQueue.uhEventCode) && (OS_NULL != nstLineToolAppStdQueue.vdQueueBuffer_p)) {
        wvdGen_MemCpy(&aubResult, nstLineToolAppStdQueue.vdQueueBuffer_p, sizeof(Type_uByte));

        if (RES_OK == aubResult) {
            aubReturn = PE_SEC0;
        } else {
            aubReturn = PE_SEC1;
        }

    } else if (LINETOOL_EV_TIMEOUT == nstLineToolAppStdQueue.uhEventCode) {
        aubReturn = PE_SEC1;
    } else {
        aubReturn = PE_SEC_CONTINUE;
    }
   

    return aubReturn;
}
/* End of File */

