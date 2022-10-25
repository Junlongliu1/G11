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

#ifndef _LINETOOLAPP_H_
#define _LINETOOLAPP_H_

/**********************************************************************************************/
/*                           Include file section                                             */
/**********************************************************************************************/

/* not used. */

/**********************************************************************************************/
/*                           Macro definition section                                         */
/**********************************************************************************************/

/* not used. */

/**********************************************************************************************/
/*                           Enumeration Type Definition Section                              */
/**********************************************************************************************/

/* SendMessageID(for Other Components Request) */

/*** Main Event Requset ***/
enum LINETOOL_EVENT_DEF{                      /* must be consistent with nstLineToolAppEvChg_Tbl */
/* 000 */ LINETOOL_EV_INVALID = 0x00U,
/* 001 */ LINETOOL_EV_POWEROFF,               /* Power Off */
/* 002 */ LINETOOL_EV_POWERON,                /* Power On */
/* 003 */ LINETOOL_EV_WARNING_IN,             /* Warning In */
/* 004 */ LINETOOL_EV_LANGUAGE_IN,            /* Language In */
/* 005 */ LINETOOL_EV_SOURCE_DIRECT,          /* Source Direct Change */
/* 006 */ LINETOOL_EV_VOLUME_DIRECT,          /* Volume Direct Change */
/* 007 */ LINETOOL_EV_SOFT_VER_GET,           /* Soft Ver Read */
/* 008 */ LINETOOL_EV_MUTE_CONTROL,           /* Mute Control */
/* 009 */ LINETOOL_EV_MAIN_TEMP_GET,          /* Get Current Main Temperature */
/* 010 */ LINETOOL_EV_LOUDNESS_SET,           /* Loudness Set */
/* 011 */ LINETOOL_EV_FAN_CONTROL,            /* FAN ON/OF Control */
/* 012 */ LINETOOL_EV_EQ_SET,                 /* EQ Set */
/* 013 */ LINETOOL_EV_FM_DIRECT,              /* FM Freq Set */
/* 014 */ LINETOOL_EV_AM_DIRECT,              /* AM Freq Set */
/* 015 */ /*LINETOOL_EV_SEEK_UP_DOWN,*/       /* Tuner Seek Up/Down *//* 0221 delete lj */
/* 016 */ LINETOOL_EV_FM_ALIGNMENT,           /* FM Level(S/M) Alignment */
/* 017 */ LINETOOL_EV_AM_ALIGNMENT,           /* AM Level(S/M) Alignment */
/* 018 */ LINETOOL_EV_RDS_INFO_GET,           /* Get RDS Info */
/* 019 */ LINETOOL_EV_RDS_LEVEL_GET,          /* Get RDS Level */
/* 020 */ LINETOOL_EV_TUNER_SM_GET,           /* Get Tuner Input signal value */
/* 021 */ LINETOOL_EV_IPOD_TRACK_CHG,         /* iPod Track up/down */
/* 022 */ LINETOOL_EV_USB_TRACK_CHG,          /* USB Track up/down */
/* 023 */ LINETOOL_EV_USB_AUDIO_CHG,          /* Set USB Audio */
/* 024 */ LINETOOL_EV_USB_VIDEO_CHG,          /* Set USB Vedio */
/* 025 */ LINETOOL_EV_BT_CHECK,               /* BT Check */
/* 026 */ LINETOOL_EV_BT_AUDIO_1KZ_SET,       /* Set BT Audio 1KHz On/Off */
/* 027 */ //LINETOOL_EV_HCIENABLE_CHG,          /*  */
/* 028 */ LINETOOL_EV_BT_LEVEL_DIAG,          /*  */
/* 029 */ LINETOOL_EV_BT_INFO_CLEAR,          /*  */
/* 030 */ LINETOOL_EV_CAMERA_ENTER,           /*  */
/* 031 */ LINETOOL_EV_CAMERA_CHG,             /*  */
/* 032 */ LINETOOL_EV_CAMERA_QUIT,            /*  */
/* 033 */ LINETOOL_EV_CAMERA_CHECK,           /*  */
/* 034 */ LINETOOL_EV_EEP_WRITE,              /*  */
/* 035 */ LINETOOL_EV_EEP_READ,               /*  */
/* 036 */ LINETOOL_EV_EEP_CLEAR,              /*  */
/* 037 */ LINETOOL_EV_EEP_INIT,               /*  */
/* 038 */ LINETOOL_EV_DISPLAY_ADJUST,         /*  */
/* 039 */ LINETOOL_EV_FLICKER_ADJUST,         /*  */
/* 040 */ LINETOOL_EV_FLC_MODE_ON,            /*  */
/* 041 */ LINETOOL_EV_FLC_MODE_OFF,           /*  */
/* 042 */ LINETOOL_EV_CURRENT_TIME_GET,       /*  */
/* 043 */ LINETOOL_EV_TP_TERMINAL_SET,        /*  */
/* 044 */ LINETOOL_EV_LED_ON,                 /*  */
/* 045 */ LINETOOL_EV_LED_OFF,                /*  */
/* 046 */ LINETOOL_EV_LCD_SET,                /*  */
/* 047 */ LINETOOL_EV_TP_CHECK_END,           /*  */
/* 048 */ LINETOOL_EV_TP_CHECK_START,         /*  */
/* 049 */ LINETOOL_EV_TP_CHECK,               /*  */
/* 050 */ LINETOOL_EV_KEY_CHECK,              /*  */
/* 051 */ LINETOOL_EV_KEY_CHECK_ONOFF,        /*  */
/* 052 */ LINETOOL_EV_MIC_PATH_BUILD,         /*  */
/* 053 */ LINETOOL_EV_MIC_PATH_CUT,           /*  */
/* 054 */ LINETOOL_EV_ILL_CHECK,              /*  */
/* 055 */ LINETOOL_EV_ENGLISH_SET,            /*  */
/* 056 */ LINETOOL_EV_CARPLAY_IPOD_CHG,       /*  */
/* 057 */ LINETOOL_EV_MODE_NAME_GET,          /*  */
/* 058 */ LINETOOL_EV_CARPLAY_UP_DOWN,        /*  */
/* 059 */ LINETOOL_EV_CARPLAY_ON_OFF,         /*  */
/* 060 */ LINETOOL_EV_AAP_ON_OFF,             /*  */
/* 061 */ LINETOOL_EV_SPEED_PLUS_GET,         /*  */
/* 062 */ LINETOOL_EV_WIFI_SSID_SET,          /*  */
/* 063 */ LINETOOL_EV_WIFI_PASSWORD_SET,      /*  */
/* 064 */ LINETOOL_EV_WIFI_CLEAR_INFO,        /*  */
/* 065 */ LINETOOL_EV_WIFI_CONNUM_GET,        /*  */
/* 066 */ LINETOOL_EV_WIFI_RESET,             /*  */
/* 067 */ LINETOOL_EV_WIFI_CON_CHECK,         /*  */
/* 068 */ LINETOOL_EV_WIFI_MODE_SET,          /*  */
/* 069 */ LINETOOL_EV_MAC_GET,                /*  */
/* 070 */ LINETOOL_EV_GNSS_GYRO_GET,          /*  */
/* 071 */ LINETOOL_EV_GNSS_NAVI_GET,          /*  */
/* 072 */ LINETOOL_EV_SECURITY_ON,            /*  */
/* 073 */ LINETOOL_EV_SECURITY_OFF,           /*  */
/* 074 */ LINETOOL_EV_LCD_TEMP_GET,           /* Get Current LCD Temperature */
/* 075 */ /*LINETOOL_EV_WIFI_TETHER_SET,  0223 lj*/
/* 076 */ LINETOOL_EV_SCREENTOUCH,
/* 077 */ LINETOOL_EV_TP_SENSOR_CHECK,
/* 078 */ LINETOOL_EV_MICAPPOINT,           // 0317 cxx
/* 079 */ LINETOOL_EV_TEL_MUTE,             // 0317 cxx
/* *** */ LINETOOL_EV_MAX,
/* *** */ LINETOOL_EV_UART_DATA,
/* *** */ LINETOOL_EV_TIMEOUT
};

/* SendMessageID(for Other Components Infernotice) */
enum LINETOOL_INFORNOTICE_DEF {
/* 000 */ LINETOOL_INF_INVALID = LINETOOL_EV_TIMEOUT,
/* 001 */ LINETOOL_INF_POWER_OFF,             /*  */
/* 002 */ LINETOOL_INF_POWER_ON,              /*  */
/* 003 */ LINETOOL_INF_WARNING_IN,            /*  */
/* 004 */ LINETOOL_INF_LANGUAGE_IN,           /*  */
/* 005 */ LINETOOL_INF_SOURCE_DIRECT,         /*  */
/* 006 */ LINETOOL_INF_VOLUME_DIRECT,         /*  */
/* 007 */ LINETOOL_INF_GET_GENERAL_VER,       /*  */
/* 008 */ LINETOOL_INF_GET_BT_VER,            /*  */
/* 009 */ LINETOOL_INF_MUTE_CONTROL,          /*  */
/* 010 */ LINETOOL_INF_LOUDNESS,              /*  */
/* 011 */ LINETOOL_INF_EQ,                    /*  */
/* 012 */ LINETOOL_INF_FM_FREQ_DIRECT,        /*  */
/* 013 */ LINETOOL_INF_AM_FREQ_DIRECT,        /*  */
/* 014 */ /*LINETOOL_INF_SEEK_UP,*/           /*  */	/* 0221 delete lj */
/* 015 */ /*LINETOOL_INF_SEEK_DOWN,*/         /*  */	/* 0221 delete lj */
/* 016 */ LINETOOL_INF_FM_ALIGNMENT,          /*  */
/* 017 */ LINETOOL_INF_AM_ALIGNMENT,          /*  */
/* 018 */ LINETOOL_INF_GET_RDS_INFO,          /*  *///delete
		  LINETOOL_INF_GET_RDS_PI,
          LINETOOL_INF_GET_RDS_PS,
          LINETOOL_INF_GET_RDS_PTY,
/* 019 */ LINETOOL_INF_GET_RDS_LEVEL,         /*  */
/* 020 */ LINETOOL_INF_GET_TUNER_SM,          /*  */
/* 021 */ LINETOOL_INF_IPOD_TRACK_CHG,        /*  */
/* 022 */ LINETOOL_INF_USB_TRACK_CHG,         /*  */
/* 023 */ LINETOOL_INF_USB_AUDIO_CHG,         /*  */
/* 024 */ LINETOOL_INF_USB_VEDIO_CHG,         /*  */
/* 025 */ LINETOOL_INF_BT_CHECK,              /*  */
/* 026 */ LINETOOL_INF_BT_AUDIO_1KZ_SET,      /*  */
/* 027 */ LINETOOL_INF_HCIENABLE_CHG,         /*  */
/* 028 */ LINETOOL_INF_BT_LEVEL_DIAG,         /*  */
/* 029 */ LINETOOL_INF_BT_INFO_CLEAR,         /*  */
/* 030 */ LINETOOL_INF_CAMERA_ENTER,          /*  */
/* 031 */ LINETOOL_INF_CAMERA_CHG,            /*  */
/* 032 */ LINETOOL_INF_CAMERA_QUIT,           /*  */
/* 033 */ LINETOOL_INF_CAMERA_CHECK,          /*  */
/* 034 */ LINETOOL_INF_MEMORY_INIT,           /*  */
/* 035 */ LINETOOL_INF_DISPLAY_ADJUST,        /*  */
/* 036 */ LINETOOL_INF_FLC_SET,               /*  */
/* 037 */ LINETOOL_INF_FLC_MODE_ON,           /*  */
/* 038 */ LINETOOL_INF_FLC_MODE_OFF,          /*  */
/* 039 */ LINETOOL_INF_GET_CURRENT_TIME,      /*  */
/* 040 */ LINETOOL_INF_SET_CLOCK_TERMINAL,    /*  */
/* 041 */ LINETOOL_INF_TP_MODE_ON,            /*  */
/* 042 */ LINETOOL_INF_TP_MODE_OFF,           /*  */
/* 043 */ LINETOOL_INF_TP_CHECK,              /*  */
/* 044 */ LINETOOL_INF_KEY_CHECK_ONOFF,       /*  */
/* 045 */ LINETOOL_INF_TEST_SOURCE_ON,        /*  */
/* 046 */ LINETOOL_INF_TEST_SOURCE_OFF,       /*  */
/* 047 */ LINETOOL_INF_MIC_PATH_BUILD,        /*  */
/* 048 */ LINETOOL_INF_MIC_PATH_CUT,          /*  */
/* 049 */ LINETOOL_INF_SET_ENGLISH,           /*  */
/* 050 */ LINETOOL_INF_CPL_IPOD_SEL,          /*  */
/* 051 */ LINETOOL_INF_CPL_TRACK_CHG,         /*  */
/* 052 */ LINETOOL_INF_CPL_ON_OFF,            /*  */
/* 053 */ LINETOOL_INF_AAP_ON_OFF,            /*  */
/* 054 */ LINETOOL_INF_WIFI_SET_SSID,         /*  */
/* 055 */ LINETOOL_INF_WIFI_SET_PASSWORD,     /*  */
/* 056 */ LINETOOL_INF_WIFI_CLEAR_INFO,       /*  */
/* 057 */ LINETOOL_INF_WIFI_GETCONNUM,        /*  */
/* 058 */ LINETOOL_INF_WIFI_RESET,            /*  */
/* 059 */ LINETOOL_INF_WIFI_CON_CHECK,        /*  */
/* 060 */ LINETOOL_INF_WIFI_MODE_ON_OFF,      /*  */
/* 061 */ LINETOOL_INF_WIFI_GET_MAC,          /*  */
/* 062 */ LINETOOL_INF_BT_GET_MAC,            /*  */
/* 063 */ LINETOOL_INF_GNSS_GET_GYRO,         /*  */
/* 064 */ LINETOOL_INF_GNSS_GET_NAVI,         /*  */
/* 065 */ LINETOOL_INF_SNR_FANSET_RES,        /*  */
/* 066 */ LINETOOL_INF_LED_SET_RES,		  	  /*  */
/* 067 */ LINETOOL_INF_LCD_TEMP_RES,		  /*  */
/* 068 */ LINETOOL_INF_KEY_MODE_RES,		  /*  */
/* 069 */ LINETOOL_INF_KEY_VALUE_RES,		  /*  */
/* 070 */ LINETOOL_INF_PWM_CHG_RES,           /*  */
/* 071 */ /*LINETOOL_INF_WIFI_TETHER_SET_RES,*/	  /*  */    /*0223 lj delete*/
/* 071 */ LINETOOL_INF_SCREENTOUCH_RES,	      /*  */
/* 072 */ LINETOOL_INF_TP_SENSOR_CHECK_RES,	      /*  */
/* 073 */ LINETOOL_INF_MICAPPOINT_RES,	      /*  */	//0317 cxx
/* 074 */ LINETOOL_INF_TEL_MUTE_RES,	      /*  */	//0317 cxx
/* 075 */ LINETOOL_INF_EMMCWRITE_LCD_RES,     /*  */
/* *** */ LINETOOL_INF_MAX                    /* <255 */
};

/**********************************************************************************************/
/*                           User type definition section                                     */
/**********************************************************************************************/

/* not used. */

/**********************************************************************************************/
/*                           Structure/Union type definition section                          */
/**********************************************************************************************/

/* not used. */

/**********************************************************************************************/
/*                           Global variable definition section                               */
/**********************************************************************************************/


/**********************************************************************************************/
/*                           Global Function Prototype Declaration                            */
/**********************************************************************************************/
extern void TSK_LINETOOLAPP(void);
extern void TSK_LINETOOLDRV(void);
extern Type_uByte wubLineTool_Init(void);
extern Type_uByte wubLineTool_Standbyout(void);
extern Type_uByte wubLineTool_Standbyin(void);
extern Type_uByte wubLineTool_EnforceStandby(void);
extern Type_uByte wubLineTool_PowerOn(void);
extern Type_uByte wubLineTool_PowerOff(void);
extern Type_uByte wubLineToolAppSendQueue(const Type_uHWord auhEventCode, const void *avdBuffer_p, const Type_uWord auwSize);
extern void wvdLineToolUartInt(Type_uByte aubData, Type_uByte aubState);

#endif
/* end of file */

