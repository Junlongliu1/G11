/*!
* @file       rpc_data.h
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

#ifndef __RPC_DATA_H__
#define __RPC_DATA_H__

#include "apn_basic_type.h"

//let
#define CAMNUMLENTH 12

//KEY
enum {
	STK_KEY_TYPE_ON_HOOK	= (0x20U),
	STK_KEY_TYPE_OFF_HOOK,
	STK_KEY_TYPE_TALK,
	STK_KEY_TYPE_OFF = (0xffU)
};
	
enum {
	STK_KEY_TYPE_VOL_DOWN	= (0x10U),
	STK_KEY_TYPE_VOL_UP,
	STK_KEY_TYPE_CH_DOWN,
	STK_KEY_TYPE_CH_UP,
	STK_KEY_TYPE_MODE
};

enum {
	HKY_KEY_TYPE_DAY_NIGHT	= (0x01U),
	HKY_KEY_TYPE_BACK,
	HKY_KEY_TYPE_HOME,
	HKY_KEY_TYPE_VOL_DOWN,
	HKY_KEY_TYPE_VOL_UP,
	HKY_KEY_TYPE_POWER
};

enum {
	KEY_ACTION_OFF			= (1U),
	KEY_ACTION_ON
};


//SENSOR
enum { 
	DCDC_PWM_6V_L = (1U),
 	DCDC_PWM_6V_H
};
enum {
	SNR_INF_FAN_ON = (1U),
	SNR_INF_FAN_OFF,
	SNR_INF_FAN_CONTROL
};

/**************************************R5 Power Start****************************************/
enum { 												/* EV_INF_STANDBYOUT_NOTIFY Parameter*/
	STANDBYOUT_TYPE_WARM_RESET = (0U),
 	STANDBYOUT_TYPE_COLD_RESET
};

enum { 												/* EV_INF_GPIO_LEVEL_SET_REQ Parameter0*/
	PM_GPIO_LEVEL_LOW = (0U),
 	PM_GPIO_LEVEL_HIGH
};

enum { 												/* EV_INF_GPIO_LEVEL_SET_REQ Parameter1*/
	PM_GPIO_PIN_MUTE = (0U),
	PM_GPIO_PIN_AMP_STBY,
	PM_GPIO_PIN_TP_33V_CONT
};

enum { 												/* EV_INF_GPIO_LEVEL_SET_RES Parameter0*/
	PM_GPIO_LEVEL_SET_SUCCESS = (0U),
 	PM_GPIO_LEVEL_SET_FAIL
};

/***************************************R5 Power End****************************************/


//let
typedef struct {
	Type_uByte ubConncetState;
	Type_uByte ubRomErrorState;
	Type_uByte ubCSensorState;
	Type_uByte ubAmingState;
	Type_uByte ubChipState;
	Type_uByte ubRomReadState;
	Type_uByte ubResult;
	Type_uByte ubErrorState;
	Type_uByte ubCamState;
	Type_uByte ubPitchHigh;
	Type_uByte ubPitchLow;
	Type_uByte ubRollHigh;
	Type_uByte ubRollLow;
	Type_uByte ubYawHigh;
	Type_uByte ubYawLow;
	Type_uByte ubCamPartsNum[CAMNUMLENTH];
	Type_uByte ubCamUnitNum[CAMNUMLENTH];
	Type_uByte ubCamRomNum[CAMNUMLENTH];
}LaneWatchLetStatus_st;

/* EV_LET_SPK_DISPLAY Used Buffer */
enum LETAPP_SPK_DISP_DEF{
/* 000 */ LET_SPK_DISP_INVALID = 0U,
/* 001 */ LET_SPK_DISP_BLANK,
/* 002 */ LET_SPK_DISP_DIAG,
/* 003 */ LET_SPK_DISP_FR_L,
/* 004 */ LET_SPK_DISP_FR_L_OK,
/* 005 */ LET_SPK_DISP_FR_L_NG,
/* 006 */ LET_SPK_DISP_FR_R,
/* 007 */ LET_SPK_DISP_FR_R_OK,
/* 008 */ LET_SPK_DISP_FR_R_NG,
/* 009 */ LET_SPK_DISP_RR_R,
/* 010 */ LET_SPK_DISP_RR_R_OK,
/* 011 */ LET_SPK_DISP_RR_R_NG,
/* 012 */ LET_SPK_DISP_RR_L,
/* 013 */ LET_SPK_DISP_RR_L_OK,
/* 014 */ LET_SPK_DISP_RR_L_NG,
/* 015 */ LET_SPK_DISP_NO_CHK,
/* 016 */ LET_SPK_DISP_BIBIRI,
/* 017 */ LET_SPK_DISP_BIBIRI_OK,
/* 018 */ LET_SPK_DISP_BIBIRI_NG,
/* 019 */ LET_SPK_DISP_BIBIRI_NO_CHK,
/* *** */ LET_SPK_DISP_MAX
};

/* EV_LET_BUTTON_INF Used Buffer */
enum LETAPP_SPK_BUTTON_DEF{
/* 000 */ LET_SPK_BTN_INVALID = 0U,
/* 001 */ LET_SPK_BTN_NG,
/* 002 */ LET_SPK_BTN_OK,
/* 003 */ LET_SPK_BTN_BACK,
/* *** */ LET_SPK_BTN_MAX
};

/* VideoSrcMgr Start*/
enum VideoSrcID
{
	V_SOURCE_ID_NONE = 1U,                  /* 01  */ /* Source ID: VIDEO Source is Home */
	V_SOURCE_ID_USB  ,  					/* 02  */ /*Source ID: VIDEO Source is USB */ /* A53 source */
	V_SOURCE_ID_CARPLAY_VIDEO   ,  			/* 03  */ /*Source ID: VIDEO Source is CARPLAY_VIDEO */ /* A53 source */
	V_SOURCE_ID_AAP_VIDEO  ,  				/* 04  */ /*Source ID: VIDEO Source is AAP_VIDEO */ /* A53 source */
	V_SOURCE_ID_WEBLINK_VIDEO  ,  			/* 05  */ /*Source ID: VIDEO Source is WEBLINK_VIDEO */ /* A53 source */
	V_SOURCE_ID_RWC  ,						/* 06	*/ /*Source ID: VIDEO Source is RWC */ /* A7 source */
	V_SOURCE_ID_LWC	,  						/* 07  */ /*Source ID: VIDEO Source is LWC */ /* A7 source */
	V_SOURCE_ID_NO_GL_RWC  ,                /* 08  */ /*Source ID: VIDEO Source is NO GL */	 /* A7 source */ 				 	
	V_SOURCE_ID_TOUCH_LWC  , 				/* 09  */ /*Source ID: VIDEO Source is TOUCH LWC */ /* A7 source */
	V_SOURCE_ID_OPENING  ,  				/* 10  */ /*Source ID: VIDEO Source is OPENING */ /* A53 source */
	V_SOURCE_ID_DIAG_RWC  ,					/* 11  */ /*Source ID: VIDEO Source is DIAG RWC */ /* A7 source */
	V_SOURCE_ID_AVM  ,						/* 12  */ /*Source ID: VIDEO Source is AVM */ /* reserve */ /* A7 source */
	V_SOURCE_ID_NUM,	
};

typedef struct {
	Type_uHWord auhNextSrc ;
	Type_uHWord auhParam1 ;/* When change start this is ON;when change end this's APP req Source ON (NG OR OK) */
}VSrcChg_Info_st;

typedef struct {
	Type_uHWord auhSrcID ;
	Type_uHWord auhParam1 ;
}VSrcOpt_st;

enum VSrcOpt_auhParam1
{
//REQ
	VIDEO_APP_NOMAL_SRC_ON = 0X01U,
	VIDEO_APP_SKIP_SRC_ON,
	VIDEO_APP_NOMAL_SRC_OFF = 0X03U,
	VIDEO_APP_SKIP_SRC_OFF,
// RES
	VIDEO_APP_SRC_ON_OK = 0X11U,
	VIDEO_APP_SRC_ON_NG ,
	VIDEO_APP_SRC_OFF_OK = 0X13U,
	VIDEO_APP_SRC_OFF_NG ,
};

enum VIDEO_CHANGE_RES
{
	VIDEO_CHANGE_OK ,
	VIDEO_CHANGE_NG,
};

enum VIDEO_SUB_SRC_ID
{
	V_SUB_SOURCE_ID_NONE=V_SOURCE_ID_NUM+1,
	V_SUB_SOURCE_ID_RWC, /* A7 source */
	V_SUB_SOURCE_ID_LWC, /* A7 source */
	V_SUB_SOURCE_ID_AVM, /* reserve */ /* A7 source */
	V_SUB_SOURCE_ID_NUM, /* A7 source */
};

typedef struct{
	Type_uByte ubBrightness;
	Type_uByte ubContrast;
	Type_uByte ubBlackLevel;
}VSrc_LUT_Info_st;

typedef struct{
	VSrc_LUT_Info_st stGraphic;
	VSrc_LUT_Info_st stVideo;
	VSrc_LUT_Info_st stLWC;
	VSrc_LUT_Info_st stRWC;
}VSrc_LUTSRC_Info_st;

typedef struct{
	VSrc_LUTSRC_Info_st stDay;
	VSrc_LUTSRC_Info_st stNight;
}VSrc_SetDayNight_Info_st;

typedef struct{
 	VSrc_SetDayNight_Info_st stQuality;
}VSrc_VQA_Info_st;

enum DAY_NIGHT_MODE
{
	VIDEOAPP_DAY_MODE=0x01U,
	VIDEOAPP_NIGHT_MODE,
};

typedef struct{
	Type_uHWord uhType;		/* param:	enum VQAINFO */
	Type_uHWord uhValue;
}VLUTReq_Info_st;

enum VQAINFO
{
  VIDEO_BRIGHTNESS=0X01,                                 
  VIDEO_CONTRAST,
  VIDEO_BLACK_LEVEL,
};

typedef struct{
     Type_uHWord uhDayNightMode;
     Type_uHWord uhType;	/* param:	enum VQAINFO */
     Type_uHWord uhValue;
}VLUTRES_Info_st;

typedef struct{
    Type_uHWord uhMuteOnOff;
    Type_uHWord uhMuteTimeOut;
}VSrc_Mute_Info_st;

enum VIDEO_HMI_MUTE_STATUS
{
	VIDEO_HMI_MUTE_OFF = 0U,
	VIDEO_HMI_MUTE_ON,
};

typedef struct{
	Type_uByte ubContrast;
	Type_uByte ubBlack_Level;
	Type_uByte ubWhite_Level;
}VLUTDiag_Info_st;
/* VideoSrcMgr End*/


/*********** DiagApp Start ***********/
/* Speaker */
#define	SPEAKER_INFO_FR_L        	(0x00U)
#define	SPEAKER_INFO_FR_R        	(0x01U)
#define	SPEAKER_INFO_RR_L        	(0x02U)
#define	SPEAKER_INFO_RR_R        	(0x03U)
#define	SPEAKER_INFO_ALL         	(0x08U)
#define	SPEAKER_INFO_QUIT         	(0x09U)
#define SPEAKER_INFO_UNSUPPORT   	(0xFFU)

/* Usb Test */
#define DIAGAPP_USB_TEST_PACKET     (0x00)
#define DIAGAPP_USB_TEST_SE_NAK     (0x01)
#define DIAGAPP_USB_TEST_J          (0x02)
#define DIAGAPP_USB_TEST_K          (0x03)
#define DIAGAPP_USB_TEST_HOSTMODE   (0x04)
#define DIAGAPP_USB_TEST_DEVMODE    (0x05)
#define DIAGAPP_USB_TEST_HUFSMODE   (0x06)
#define DIAGAPP_USB_TEST_RESET      (0x07)

/*********** DiagApp End ***********/


/*********** LogMgr Start ****************/
#define LOG_RET_OK                  (0U)
#define LOG_RET_NG                  (1U)
#define LOG_FILETYPE_A53DLT         (1U)
#define LOG_FILETYPE_A53MSG         (2U)
#define LOG_FILETYPE_A7DLT          (3U)
#define LOG_FILETYPE_A7MSG          (4U)
#define LOG_FILETYPE_R5DLT          (5U)
#define LOG_FILENAME_LEN_MAX        (50U)
#define LOG_PATH_LEN_MAX            (255U)

typedef struct {
    Type_uByte  ubFileType;   /* LOG_FILETYPE_A7DLT / LOG_FILETYPE_A7MSG */
    Type_sWord  swSMKey;
    Type_Char   chFileName[LOG_FILENAME_LEN_MAX+1];
    Type_uWord  uwFileSize;
} LogMgr_SMInfo_ts;

/*********** LogMgr End ******************/


/*********** LinetoolApp Start ***********/
/* Gnss Gyro Get*/
typedef struct {
    Type_uByte  aubResult;
    Type_uByte  aubType;
    Type_uByte  aubTemp;
    Type_uHWord auhGyro;
    Type_uHWord auhAcc;
} LineTool_GyroData_ts;

/* Navi Set */
typedef struct {
    Type_uByte aubStaNum;
    Type_uByte aubEL;
    Type_uByte aubCN;
    Type_uHWord aubAZI;
}LineTool_NaviDataSing_ts;

typedef struct {
    Type_uByte  aubResult;
    Type_uByte aubStaCount;
    LineTool_NaviDataSing_ts astNaviData[10];
} LineTool_NaviData_ts;
/*********** LinetoolApp End ***********/
/*	updatestm version type define	*/
enum VERSION_TYPE_ID
{
	VERSION_VECHILE = 0x00,
	VERSION_MAINCORE_NORMAL,
	VERSION_SUBCORE_NORMAL,
	VERSION_MICOM_NORMAL,
	VERSION_MAINCORE_UPDATE,
	VERSION_MICOM_UPDATE,
	VERSION_TOTAL,
};

/****USBFS Write File****/

typedef struct{
	Type_Char		chWritePath[254]; 
	Type_uWord		uwWriteSize;
	Type_sWord		swWriteShareMemID;
}USBFS_REQ_WRITEFILE_t;


/****USBFS Read File****/
typedef struct{
	Type_uByte		ubReadState;
	Type_uWord		uwReadSize;
	Type_sWord		swReadShareMemID;
}USBFS_RES_READFILE_t;

typedef struct{
	Type_Char		chReadPath[254]; 
	Type_uWord		uwReadSize;
}USBFS_REQ_READFILE_t;


/****DGL FILE EXIST****/
typedef struct{
	Type_Char		chDglFileName[110]; 
	Type_uWord		uwFileNum;
}USBFS_REQ_DGLFILE_t;

/****tpctl event data define****/
enum TPCTL_OPER_RESULT{
	TPCTL_OPER_RESULT_OK 		= 0,
	TPCTL_OPER_RESULT_NG 		= -1,
	TPCTL_OPER_RESULT_NO_OPER	= -2
};

typedef struct tpctl_touch_simu_info
{
	Type_uHWord	auhTouchPos_x;
	Type_uHWord	auhTouchPos_y;
	Type_uHWord	auhTouchState;
}TpctlTouchSimuInfo;

enum TPCTL_TOUCH_STATE_TYPE{
	TPCTL_TOUCH_STATE_TYPE_UP 		= 0,
	TPCTL_TOUCH_STATE_TYPE_DOWN 	= 1,
	TPCTL_TOUCH_STATE_TYPE_NO_OPER	= 2
};
enum TPCTL_TOUCH_POINT_INDEX{
	TPCTL_TOUCH_POINT_INDEX_0 		= 0,
	TPCTL_TOUCH_POINT_INDEX_1 		= 1,
	TPCTL_TOUCH_POINT_INDEX_2		= 2,
	TPCTL_TOUCH_POINT_INDEX_3		= 3,
	TPCTL_TOUCH_POINT_INDEX_4		= 4,
	TPCTL_TOUCH_POINT_MAX			= 5
};
typedef struct
{
	Type_uByte	ubTouchPointId;
	Type_uHWord	uhTouchPos_x;
	Type_uHWord	uhTouchPos_y;
	Type_uHWord	uhTouchState;
}TpctlTouchPointInfo;

typedef struct
{
	Type_uByte	ubTouchPointNum;
	TpctlTouchPointInfo	stTouchPoint[TPCTL_TOUCH_POINT_MAX];
}TpctlTouchInfo;

typedef struct
{
	Type_uHWord	ubadd;
	Type_uByte	ublen;
	Type_uByte	ubdata[4];
}NvmResLineToolWData_ts;
typedef struct
{
	Type_uHWord	ubadd;
	Type_uByte	ublen;
}NvmResLineToolRData_ts;

#endif // __RPC_DATA_H__
