/**********************************************************************************************
*  file name                : letapp_common.c
*  date                     : 2016.06.01
*  autho                    : zhangjinzhu
*  model                    : TBTA
*  description              : let Task execute related file
*
*----------------------------------------------------------------------------------------------
*  revision History
*  No.       date           revised by        description
*  0001      2016.04.01     zhangjinzhu       Create
**********************************************************************************************/

/********************************************************************************************/
/*                             Include File Section                                         */
/********************************************************************************************/

/* System Header Files */
//#include "kernel_id.h"
#include "os_framework.h"
#include "apn_basic_type.h"
#include "generic.h"
/* LET Header Files */
#include "letapp_common.h"
#include "letapp_core.h"
#include "letapp_os.h"
#include "letapp_suba_core.h"
/* Other Components */
//#include "dserial.h"
//#include "rpcapp_event.h"
//#include "eepromcontrol.h"
#include "rpc_data.h"
/********************************************************************************************/
/*                           Macro Definition Section                                       */
/********************************************************************************************/

/* not used. */

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
Type_uByte wubLetAppCmdSize;
Type_uByte wubLetAppSubACmdSize;
Type_uByte wubLetAppSubBCmdSize;
Type_uByte wubLetAppCmdData[LETAPP_NUM_3]; /* LetApp Commond Data */
Type_uByte wubLetAppSubACmdData[LETAPP_NUM_3]; /* LetApp SubA Commond Data */
Type_uByte wubLetAppSubBCmdData[LETAPP_NUM_3]; /* LetApp SubB Commond Data */
Type_uByte wubLetAppTempBuff[LETAPP_NUM_64]; /* LetApp Temp Used Buffer */
Type_uByte wubLetSubATempBuff[LETAPP_NUM_64]; /* Let SubA Temp Used Buffer */
Type_uByte wubLetSubBTempBuff[LETAPP_NUM_64]; /* Let SubB Temp Used Buffer */
/********************************************************************************************/
/*                           ROM Table Section                                              */
/********************************************************************************************/

/* not used. */

/********************************************************************************************/
/*                           Static Variable Definition Section                             */
/********************************************************************************************/

static OS_TimeTick_t nuwOsTimeTick;

/********************************************************************************************/
/*                           Static Function Declaration Section                            */
/********************************************************************************************/

/* not used. */

/********************************************************************************************/
/*                           Global variable definition section                             */
/********************************************************************************************/

/* not used. */

/********************************************************************************************/
/*                           Function Definition Section                                    */
/********************************************************************************************/
/*********************************************************************************************
*  function                 : wvdLetAppInitial
*  date                     : 2016.07.01
*  autho                    : zhangjinzhu
*  description              : -
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
void wvdLetAppInitial(void)
{
    /* Let Variation Init */
    wubLetSpkIndex = LETAPP_NUM_0;
    wubLetSpkBtnInfo = LET_SPK_BTN_INVALID;
    wuhLetSpkConnectInfo = LETAPP_NUM_0;
    nuwOsTimeTick = LETAPP_NUM_0;
    wubLetAppCmdSize = LETAPP_NUM_0;
    wubLetAppSubACmdSize = LETAPP_NUM_0;
    wubLetAppSubBCmdSize = LETAPP_NUM_0;
    wuhLetBibiriSong = LETAPP_NUM_0;
    wubLetSndBass = LETAPP_NUM_0;
    wubLetSndTreble = LETAPP_NUM_0;
    wvdGen_MemSet(&wubLetAppCmdData[LETAPP_NUM_0], LETAPP_NUM_0, sizeof(wubLetAppCmdData));
    wvdGen_MemSet(&wubLetAppSubACmdData[LETAPP_NUM_0], LETAPP_NUM_0, sizeof(wubLetAppSubACmdData));
    wvdGen_MemSet(&wubLetAppSubBCmdData[LETAPP_NUM_0], LETAPP_NUM_0, sizeof(wubLetAppSubBCmdData));
    wvdGen_MemSet(&wubLetAppTempBuff[LETAPP_NUM_0], LETAPP_NUM_0, sizeof(wubLetAppTempBuff));
    wvdGen_MemSet(&wubLetSubATempBuff[LETAPP_NUM_0], LETAPP_NUM_0, sizeof(wubLetSubATempBuff));
    wvdGen_MemSet(&wubLetSubBTempBuff[LETAPP_NUM_0], LETAPP_NUM_0, sizeof(wubLetSubBTempBuff));
    nubBackState = LETAPP_NUM_0;

    wvdLetAppComInit(); /* Let Mode Status Init */
}
/*********************************************************************************************
*  function                 : wvdLetAppComInit
*  date                     : 2016.07.01
*  autho                    : zhangjinzhu
*  description              : -
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
void wvdLetAppComInit(void)
{
    /* Let Mode Status Init */
    wstLetAmIndex_p = OS_NULL;
    wstLetFmIndex_p = OS_NULL;
    wstLetTunerIndex_p = OS_NULL;
    wstLetUsbIndex_p = OS_NULL;
    wstLetBeepIndex_p = OS_NULL;
    wstLetMemIndex_p = OS_NULL;
    wstLetBsmIndex_p = OS_NULL;
    wubLetAppComModeSts = LET_MODE_OFFED;
    wubLetAppHighIndexMode = H_INDEX_MODE_WAIT;
    wubLetAppAmIndexMode = MODE_L_AM_OFF;
    wubLetAppFmIndexMode = MODE_L_FM_OFF;
    wubLetAppTunerIndexMode = MODE_L_TUNER_TEST_OFF;
    wubLetAppUsbIndexMode = MODE_L_USB_TEST_OFF;
    wubLetAppBeepIndexMode = MODE_L_SPK_BEEP_OFF;
    wubLetAppMemIndexMode = MODE_L_MEM_CLEAR_OFF;
    wubLetAppBsmIndexMode = MODE_L_BSM_TEST_OFF;
}
/*********************************************************************************************
*  function                 : wubLetAppFmAmIsChk
*  date                     : 2016.07.01
*  autho                    : zhangjinzhu
*  description              : -
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
Type_uByte wubLetAppFmAmIsChk(void)
{
    Type_uByte aubFmAmIsChk;

    aubFmAmIsChk = LETAPP_NOMAL_NG;

    if ((MODE_L_AM_OFF == wubLetAppAmIndexMode) && (MODE_L_FM_OFF == wubLetAppFmIndexMode) && (MODE_L_TUNER_TEST_OFF == wubLetAppTunerIndexMode)){
        aubFmAmIsChk = LETAPP_NOMAL_OK; /* Let FM AM No Check */
    }
    else {
        aubFmAmIsChk = LETAPP_NOMAL_NG; /* Let FM AM Checking */
    }

    return aubFmAmIsChk;
}
/*********************************************************************************************
*  function                 : wvdLetAppSysWorkTime
*  date                     : 2016.07.01
*  autho                    : zhangjinzhu
*  description              : -
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
void wvdLetAppSysWorkTime(Type_uByte aubWorkSts)
{
    OS_TimeTick_t auwOsCurTime;
    OS_TimeTick_t auwOsPassTime;
    Type_uHWord auhWorkTime;
    Type_uHWord auhEepData;
    
    auwOsCurTime = LETAPP_NUM_0;
    auwOsPassTime = LETAPP_NUM_0;
    auhWorkTime = LETAPP_NUM_0;
    auhEepData = LETAPP_NUM_0;

    if (LETAPP_NOMAL_OK == aubWorkSts){
        nuwOsTimeTick = OSAL_GetOSTimeTick(); /* Time Caculate Start */
    }
    else {
        auwOsCurTime = OSAL_GetOSTimeTick(); /* Time Caculate Finish */
        if (auwOsCurTime > nuwOsTimeTick){
            auwOsPassTime = auwOsCurTime - nuwOsTimeTick;
        }
        else {
            auwOsPassTime = LETAPP_TICK_TIME_MAX - nuwOsTimeTick + auwOsCurTime;
        }

        auhWorkTime = ((auwOsPassTime / LETAPP_NUM_1000) / LETAPP_NUM_60) / LETAPP_NUM_10; /* 10 minute Per 1 count */

        //LXQADD (void)wubEEP_BlockRead(LET_EEP_ADD_TOTAL_TIME, LETAPP_NUM_2, (Type_uByte*)(&auhEepData)); /* Time Count Eeprom Read */

        if ((Type_uWord)auhWorkTime + (Type_uWord)auhEepData > LETAPP_NUM_FFFF){
            auhWorkTime = (Type_uHWord)(((Type_uWord)auhWorkTime + (Type_uWord)auhEepData) - LETAPP_NUM_FFFF);
        }
        else {
            auhWorkTime = auhWorkTime + auhEepData;
        }

       // (void)wubEEP_BlockWrite(LET_EEP_ADD_TOTAL_TIME, LETAPP_NUM_2, (Type_uByte*)(&auhWorkTime), EEP_FORCE_WRITE); /* Time Count Eeprom Write */
    }
}

/* End of File */

