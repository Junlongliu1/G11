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
//#include "AppMgr_Cfg.h"
// #include "Log_Ext.h"

/* Linetool Header Files */
#include "linetoolapp.h"
#include "linetoolapp_common.h"
#include "linetoolapp_core.h"

/* Other Components */
#include "uart.h"
//#include "RpcApp_event.h"
#include "rpc_if.h"
#include "rpc_event.h"
#include "bsp.h"
#include "logapp.h"	//add by cxx on 20220421 for LOG
/********************************************************************************************/
/*                           Macro Definition Section                                       */
/********************************************************************************************/

#define LINETOOL_RPC_INIT_RETRY         (3)
/********************************************************************************************/
/*                           Enumeration Type Definition Section                            */
/********************************************************************************************/

/* not used. */

/********************************************************************************************/
/*                           User type definition section                                   */
/********************************************************************************************/

/* not used. */

/********************************************************************************************/
/*                           Structure/Union Type Definition Section                        */
/********************************************************************************************/

typedef struct {
    Type_uHWord uhRpcEvent;
    Type_uHWord uhLinetoolEvent;
}LinetoolResEvent_ts;

/********************************************************************************************/
/*                           ROM Table Section                                              */
/********************************************************************************************/
static const LinetoolResEvent_ts nstLinetoolRpcEventChg[] = {
    { EV_RES_LINETOOL_POWER_OFF,            LINETOOL_INF_POWER_OFF },
    { EV_RES_LINETOOL_POWER_ON,             LINETOOL_INF_POWER_ON },
    { EV_RES_LINETOOL_WARNING_IN,           LINETOOL_INF_WARNING_IN },
    { EV_RES_LINETOOL_LANGUAGE_IN,          LINETOOL_INF_LANGUAGE_IN },
    { EV_RES_LINETOOL_SOURCE_DIRECT,        LINETOOL_INF_SOURCE_DIRECT },
    { EV_RES_LINETOOL_VOLUME_DIRECT,        LINETOOL_INF_VOLUME_DIRECT },
    { UPD_CMD_CURRENT_VER_RES,              LINETOOL_INF_GET_GENERAL_VER },
    { EV_RES_LINETOOL_GET_BT_VER,           LINETOOL_INF_GET_BT_VER },
    { EV_RES_LINETOOL_MUTE_CONTROL,         LINETOOL_INF_MUTE_CONTROL },
    { EV_RES_LINETOOL_LOUDNESS,             LINETOOL_INF_LOUDNESS },
    { EV_RES_LINETOOL_EQ,                   LINETOOL_INF_EQ },
    { EV_RES_LINETOOL_FM_FREQ_DIRECT,       LINETOOL_INF_FM_FREQ_DIRECT },
    { EV_RES_LINETOOL_AM_FREQ_DIRECT,       LINETOOL_INF_AM_FREQ_DIRECT },
/*    { EV_RES_LINETOOL_SEEK_UP,              LINETOOL_INF_SEEK_UP },*/	    /* 0221 delete lj */
/*    { EV_RES_LINETOOL_SEEK_DOWN,            LINETOOL_INF_SEEK_DOWN },*/	/* 0221 delete lj */
    { EV_RES_LINETOOL_FM_ALIGNMENT,         LINETOOL_INF_FM_ALIGNMENT },
    { EV_RES_LINETOOL_AM_ALIGNMENT,         LINETOOL_INF_AM_ALIGNMENT },
    { EV_RES_LINETOOL_GET_RDS_PI,           LINETOOL_INF_GET_RDS_PI },
    { EV_RES_LINETOOL_GET_RDS_PS,           LINETOOL_INF_GET_RDS_PS },
    { EV_RES_LINETOOL_GET_RDS_PTY,          LINETOOL_INF_GET_RDS_PTY },
    { EV_RES_LINETOOL_GET_RDS_LEVEL,        LINETOOL_INF_GET_RDS_LEVEL },
    { EV_RES_LINETOOL_GET_TUNER_SM,         LINETOOL_INF_GET_TUNER_SM },
    { EV_RES_LINETOOL_IPOD_TRACK_CHG,       LINETOOL_INF_IPOD_TRACK_CHG },
    { EV_RES_LINETOOL_USB_TRACK_CHG,        LINETOOL_INF_USB_TRACK_CHG },
    { EV_RES_LINETOOL_USB_AUDIO_CHG,        LINETOOL_INF_USB_AUDIO_CHG },
    { EV_RES_LINETOOL_USB_VEDIO_CHG,        LINETOOL_INF_USB_VEDIO_CHG },
    { EV_RES_LINETOOL_BT_CHECK,             LINETOOL_INF_BT_CHECK },
    { EV_RES_LINETOOL_BT_AUDIO_1KZ_SET,     LINETOOL_INF_BT_AUDIO_1KZ_SET },
    { EV_RES_LINETOOL_BT_LEVEL_DIAG,        LINETOOL_INF_BT_LEVEL_DIAG },
    { EV_RES_LINETOOL_BT_INFO_CLEAR,        LINETOOL_INF_BT_INFO_CLEAR },
    { EV_RES_LINETOOL_CAMERA_ENTER,         LINETOOL_INF_CAMERA_ENTER },
    { EV_RES_LINETOOL_CAMERA_CHG,           LINETOOL_INF_CAMERA_CHG },
    { EV_RES_LINETOOL_CAMERA_QUIT,          LINETOOL_INF_CAMERA_QUIT },
    { EV_RES_LINETOOL_CAMERA_CHECK,         LINETOOL_INF_CAMERA_CHECK },
    { EV_RES_LINETOOL_MEMORY_INIT,          LINETOOL_INF_MEMORY_INIT },
    { EV_RES_LINETOOL_DISPLAY_ADJUST,       LINETOOL_INF_DISPLAY_ADJUST },
    { EV_RES_LINETOOL_FLC_SET,              LINETOOL_INF_FLC_SET },
    { EV_RES_LINETOOL_FLC_MODE_ON,          LINETOOL_INF_FLC_MODE_ON },
    { EV_RES_LINETOOL_FLC_MODE_OFF,         LINETOOL_INF_FLC_MODE_OFF },
    { EV_RES_LINETOOL_GET_CURRENT_TIME,     LINETOOL_INF_GET_CURRENT_TIME },
    { EV_RES_LINETOOL_SET_CLOCK_TERMINAL,   LINETOOL_INF_SET_CLOCK_TERMINAL },
    { EV_RES_LINETOOL_TP_MODE_ON,           LINETOOL_INF_TP_MODE_ON },
    { EV_RES_LINETOOL_TP_MODE_OFF,          LINETOOL_INF_TP_MODE_OFF },
    { EV_INF_TPCTL_EVE_DISPATCH_NTY,        LINETOOL_INF_TP_CHECK },
    { EV_RES_LINETOOL_KEY_CHECK_ONOFF,      LINETOOL_INF_KEY_CHECK_ONOFF },
    { EV_RES_LINETOOL_TEST_SOURCE_ON,       LINETOOL_INF_TEST_SOURCE_ON },
    { EV_RES_LINETOOL_TEST_SOURCE_OFF,      LINETOOL_INF_TEST_SOURCE_OFF },
    { EV_RES_LINETOOL_MIC_PATH_BUILD,       LINETOOL_INF_MIC_PATH_BUILD },
    { EV_RES_LINETOOL_MIC_PATH_CUT,         LINETOOL_INF_MIC_PATH_CUT },
    { EV_RES_LINETOOL_SET_ENGLISH,          LINETOOL_INF_SET_ENGLISH },
    { EV_RES_LINETOOL_CPL_IPOD_SEL,         LINETOOL_INF_CPL_IPOD_SEL },
    { EV_RES_LINETOOL_CPL_TRACK_CHG,        LINETOOL_INF_CPL_TRACK_CHG },
    { EV_RES_LINETOOL_CPL_ON_OFF,           LINETOOL_INF_CPL_ON_OFF },
    { EV_RES_LINETOOL_AAP_ON_OFF,           LINETOOL_INF_AAP_ON_OFF },
    { EV_RES_LINETOOL_WIFI_SET_SSID,        LINETOOL_INF_WIFI_SET_SSID },
    { EV_RES_LINETOOL_WIFI_SET_PASSWORD,    LINETOOL_INF_WIFI_SET_PASSWORD },
    { EV_RES_LINETOOL_WIFI_CLEAR_INFO,      LINETOOL_INF_WIFI_CLEAR_INFO },
    { EV_RES_LINETOOL_WIFI_GETCONNUM,       LINETOOL_INF_WIFI_GETCONNUM },
    { EV_RES_LINETOOL_WIFI_RESET,           LINETOOL_INF_WIFI_RESET },
    { EV_RES_LINETOOL_WIFI_CON_CHECK,       LINETOOL_INF_WIFI_CON_CHECK },
    { EV_RES_LINETOOL_WIFI_MODE_ON_OFF,     LINETOOL_INF_WIFI_MODE_ON_OFF },
    { EV_RES_LINETOOL_WIFI_GET_MAC,         LINETOOL_INF_WIFI_GET_MAC },
    { EV_RES_LINETOOL_BT_GET_MAC,           LINETOOL_INF_BT_GET_MAC },
    { EV_RES_LINETOOL_GNSS_GET_GYRO,        LINETOOL_INF_GNSS_GET_GYRO },
    { EV_RES_LINETOOL_GNSS_GET_NAVI,        LINETOOL_INF_GNSS_GET_NAVI },
    { SNR_EV_INF_FANSET_RES,                LINETOOL_INF_SNR_FANSET_RES },
	{ LINETOOL_LED_SET_RES,					LINETOOL_INF_LED_SET_RES },
	{ EV_INF_LCD_TEMP_RES,					LINETOOL_INF_LCD_TEMP_RES },
	{ EVENT_CODE_LINETOOL_KEY_MODE_ON_RES,	LINETOOL_INF_KEY_MODE_RES },
	{ EVENT_CODE_LINETOOL_KEY_MODE_OFF_RES,	LINETOOL_INF_KEY_MODE_RES },
	{ EVENT_CODE_HARDKEY,                   LINETOOL_INF_KEY_VALUE_RES },
	{ EVENT_CODE_STEERINGKEY,               LINETOOL_INF_KEY_VALUE_RES },
	{ EV_INF_LINETOOL_PWM_CHG_RES,          LINETOOL_INF_PWM_CHG_RES },
	{ EV_INF_TPCTL_TPSIMU_RES,              LINETOOL_INF_SCREENTOUCH_RES },
	{ EV_INF_TPCTL_SELF_TEST_RES,           LINETOOL_INF_TP_SENSOR_CHECK_RES },
	{ EV_RES_LINETOOL_MIC_APPOINT,          LINETOOL_INF_MICAPPOINT_RES },
	// { ,                                  LINETOOL_INF_TEL_MUTE_RES },
	{ EV_NVM_LCD_WRITE_RES,                 LINETOOL_INF_EMMCWRITE_LCD_RES },
};
/********************************************************************************************/
/*                           Static Variable Definition Section                             */
/********************************************************************************************/

/* not used. */

/********************************************************************************************/
/*                           Static Function Declaration Section                            */
/********************************************************************************************/

static void nvdLineToolAppCallback(const Type_uByte aubSrc, const Type_uHWord auhEvent, void* avdData_p, const Type_uHWord auhLength);

/********************************************************************************************/
/*                           Global variable definition section                             */
/********************************************************************************************/

Type_uByte wubLineToolPowerState;
/********************************************************************************************/
/*                           Function Definition Section                                    */
/********************************************************************************************/
/********************************************************************************************/
/** \function       wubLineTool_Init
 *  \date           2021/01/11
 *  \author         zhangjinzhu
 *  \description    LineTool Init
 *
 *  <!------------- Argument Code ----------------------------------------------------------->
 *
 *  <!------------- Return Code ------------------------------------------------------------->
 *  \return         RES_OK - Success
 *                  RES_NG - Failure
 *
 *********************************************************************************************
 *  \par    Revision History:
 *  <!----- No.     Date        Revised by      Details ------------------------------------->
 *
 ********************************************************************************************/
Type_uByte wubLineTool_Init(void) {

    Type_uByte aubReturn;
    Type_uByte aubCount;
    Type_sByte asbResult;

    aubReturn = RES_NG;
    asbResult = RPC_NG;
    aubCount = LINETOOL_NUM_0;

	// wvdLineToolDrvInit();
    // wvdLineToolAppInit();
    wubLineToolPowerState = LINETOOL_STATE_ACC_OFF;

    for (aubCount = LINETOOL_NUM_0; aubCount < LINETOOL_RPC_INIT_RETRY; aubCount++) {
        asbResult = wsbRPCInit(ARPC_ID_LINETOOLAPP, nvdLineToolAppCallback);
        if (RPC_OK == asbResult) {
            aubReturn = RES_OK;
            break;
        } else {
            /* No Action */
        }
    }
    return aubReturn;
}
/********************************************************************************************/
/** \function       wubLineTool_Standbyin
 *  \date           2016/06/01
 *  \author         zhangjinzhu
 *  \description    UART Unload
 *
 *  <!------------- Argument Code ----------------------------------------------------------->
 *
 *  <!------------- Return Code ------------------------------------------------------------->
 *  \return         RES_OK - Success
 *                  RES_NG - Failure
 *
 *********************************************************************************************
 *  \par    Revision History:
 *  <!----- No.     Date        Revised by      Details ------------------------------------->
 *
 ********************************************************************************************/
Type_uByte wubLineTool_Standbyout(void) {

	wvdLineToolDrvInit();
    wvdLineToolAppInit();
    wubLineToolPowerState = LINETOOL_STATE_POWER_ON;

    return RES_OK;
}
/********************************************************************************************/
/** \function       wubLineTool_Standbyin
 *  \date           2016/06/01
 *  \author         zhangjinzhu
 *  \description    UART Unload
 *
 *  <!------------- Argument Code ----------------------------------------------------------->
 *
 *  <!------------- Return Code ------------------------------------------------------------->
 *  \return         RES_OK - Success
 *                  RES_NG - Failure
 *
 *********************************************************************************************
 *  \par    Revision History:
 *  <!----- No.     Date        Revised by      Details ------------------------------------->
 *
 ********************************************************************************************/
Type_uByte wubLineTool_Standbyin(void) {
	wvdLineToolDrvUnInit();
    wvdLineToolAppInit();
    wubLineToolPowerState = LINETOOL_STATE_ACC_OFF;

    return RES_OK;
}

/********************************************************************************************/
/** \function       wubLineTool_EnforceStandby
 *  \date           2021/11/29
 *  \author         liujie
 *  \description    Power On
 *
 *  <!------------- Argument Code ----------------------------------------------------------->
 *
 *  <!------------- Return Code ------------------------------------------------------------->
 *  \return         RES_OK - Success
 *                  RES_NG - Failure
 *
 *********************************************************************************************
 *  \par    Revision History:
 *  <!----- No.     Date        Revised by      Details ------------------------------------->
 *
 ********************************************************************************************/
extern Type_uByte wubLineTool_EnforceStandby(void) {
    Type_uByte aubReturn;

    aubReturn = RES_OK;
    wubLineToolPowerState = LINETOOL_STATE_ACC_OFF;

	wvdLineToolDrvUnInit();
    wvdLineToolAppInit();

    return aubReturn;
}

/********************************************************************************************/
/** \function       wubLineTool_PowerOn
 *  \date           2021/11/29
 *  \author         liujie
 *  \description    Power On
 *
 *  <!------------- Argument Code ----------------------------------------------------------->
 *
 *  <!------------- Return Code ------------------------------------------------------------->
 *  \return         RES_OK - Success
 *                  RES_NG - Failure
 *
 *********************************************************************************************
 *  \par    Revision History:
 *  <!----- No.     Date        Revised by      Details ------------------------------------->
 *
 ********************************************************************************************/

Type_uByte wubLineTool_PowerOn(void) {
    Type_uByte aubReturn;

    aubReturn = RES_OK;
    wubLineToolPowerState = LINETOOL_STATE_POWER_ON;

    return aubReturn;
}

/********************************************************************************************/
/** \function       wubLineTool_PowerOff
 *  \date           2021/11/29
 *  \author         liujie
 *  \description    Power Off
 *
 *  <!------------- Argument Code ----------------------------------------------------------->
 *
 *  <!------------- Return Code ------------------------------------------------------------->
 *  \return         RES_OK - Success
 *                  RES_NG - Failure
 *
 *********************************************************************************************
 *  \par    Revision History:
 *  <!----- No.     Date        Revised by      Details ------------------------------------->
 *
 ********************************************************************************************/
Type_uByte wubLineTool_PowerOff(void) {
    Type_uByte aubReturn;

    aubReturn = RES_OK;
    wubLineToolPowerState = LINETOOL_STATE_POWER_OFF;

    return aubReturn;

}


/********************************************************************************************/
/* Function Name		: nvdLineToolAppCallback				    						*/
/* Date				    : 2021.11.01														*/
/* Author			    : liujie    														*/
/* Description			: Rpc CallBack									                	*/
/* Argument Code		: aubSrc		[IN]	Source ID									*/
/* 				 	      auhEvent		[IN]	Event Code			    					*/
/* 					      avdData_p		[IN]	Buffer		            					*/
/* 				 	      auhLength		[IN]	Buffer Size				            		*/
/* Return Code		: 																		*/
/*------------------------------------------------------------------------------------------*/
/* Revision History																			*/
/* No		Date		Revised by		Explanation											*/
/*==========================================================================================*/
/********************************************************************************************/
void nvdLineToolAppCallback(const Type_uByte aubSrc, const Type_uHWord auhEvent, void* avdData_p, const Type_uHWord auhLength) {
    LOG_OUTPUT(LOG_APP_ID_LINE, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n Rpc rcv aubSrc  = %x auhEvent = %x auhLength = %d", aubSrc, auhEvent, auhLength);    //add by cxx on 20220421 for LOG
    //printf("Rpc rcv aubSrc  = %x auhEvent = %x auhLength = %d", aubSrc, auhEvent, auhLength);       //delete by cxx on 20220421 for LOG
#ifndef LINETOOLAPP_DEBUG           //add by cxx on 20220421 for LOG
    // TODO: for test
    printf(" data = [ ");
    Type_uByte * test = avdData_p;
    for (int i = 0; i < auhLength; i++) {
        printf("%02x ", test[i]);
    }
    printf("] \n");
    // TODO: for test end
#endif
    Type_uHWord auhLinetoolEvent;
    Type_uHWord auhChkNum;
    Type_uHWord auhChkMax;
    Type_uByte aubInitRes = RES_NG; //0329 cxx for PCLintResult 534

    auhLinetoolEvent = LINETOOL_INF_INVALID;
    auhChkNum = LINETOOL_NUM_0;
    auhChkMax = sizeof(nstLinetoolRpcEventChg) / sizeof(LinetoolResEvent_ts);
    while (auhChkNum < auhChkMax) {
        if (nstLinetoolRpcEventChg[auhChkNum].uhRpcEvent == auhEvent) {
            auhLinetoolEvent = nstLinetoolRpcEventChg[auhChkNum].uhLinetoolEvent;
            aubInitRes = wubLineToolAppSendQueue(auhLinetoolEvent, avdData_p, auhLength); //0329 cxx for PCLintResult 534
            if(RES_NG == aubInitRes){
                /* No Action */
            }
            break;
        } else {
            /* No Action */
        }
        auhChkNum++;
    }
}

/********************************************************************************************/
/** \function       wvdLineToolUartInt
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
void wvdLineToolUartInt(Type_uByte aubData, Type_uByte aubState) {

   if (BSP_ERR_NONE == aubState) {

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
/* End of File */
