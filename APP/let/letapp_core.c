/**********************************************************************************************
*  file name                : letapp_core.c
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
/* Linetool Header Files */
#include "letapp.h"
#include "letapp_os.h"
#include "letapp_common.h"
#include "letapp_core.h"
/* Other Components */
#include "model.h"
//#include "dserial.h"
#include "klinedrv.h"
//#include "eepromcontrol.h"
//#include "soundstm_def.h"
#include "model.h"
#include "rpc_module.h"
#include "rpc_if.h"

/********************************************************************************************/
/*                           Macro Definition Section                                       */
/********************************************************************************************/


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

/* not used. */

/********************************************************************************************/
/*                           Static Function Declaration Section                            */
/********************************************************************************************/
static void nvdLetAppCmdSave(Type_uByte* aubDataBuff_p, Type_uByte aubSize);
static void nvdLetAppHighProcContrl(Type_uByte aubHighEvent);
static void nvdLetAppLowProcContrl(Type_uByte aubProcId, Type_uByte aubLowEvent);
static void nvdLetAppProcContrl(Type_uByte aubProcId);
static void nvdLetIndexModeUpdate(void);
static void nvdLetAppLetStart(void);
static void nvdLetAppLetEnd(void);
static void nvdLetAppAmStart(void);
static void nvdLetAppAmEnd(void);
static void nvdLetAppAmAnt(void);
static void nvdLetAppAmAntEnd(void);
static void nvdLetAppFmStart(void);
static void nvdLetAppFmEnd(void);
static void nvdLetAppFmAnt(void);
static void nvdLetAppFmAntEnd(void);
static void nvdLetAppUsbTestStart(void);
static void nvdLetAppUsbTestEnd(void);
static void nvdLetAppUsbSpkStart(void);
static void nvdLetAppUsbSpkEnd(void);
static void nvdLetAppBibiriStart(void);
static void nvdLetAppBibiriSet(void);
static void nvdLetAppBibiriEnd(void);
static void nvdLetAppBsmInitial(void);
static void nvdLetAppBsmTpSetStart(void);
static void nvdLetAppBsmTestEnd(void);
static void nvdLetAppTunerStart(void);
static void nvdLetAppTunerSet(void);
static void nvdLetAppTunerEnd(void);
static void nvdLetAppBeepChkStart(void);
static void nvdLetAppBeepChkChUp(void);
static void nvdLetAppBeepChkEnd(void);
static void nvdLetAppBeepChkButton(void);
static void nvdLetAppMemClearStart(void);
static void nvdLetAppLetMemClear(void);
static void nvdLetAppInitialClear(void);
static void nvdLetAppMemClearEnd(void);
static void nvdLetAppAmAntRest(void);
static void nvdLetAppFmAntRest(void);
static void nvdLetAppUsbSpkRest(void);
static void nvdLetAppBsmRest(void);
static void nvdLetAppBibiriRest(void);
static void nvdLetAppVolVspRest(void);
static void nvdLetAppVinCompRest(void);
static void nvdLetAppVinNoRest(void);
static void nvdLetAppTotalTimeRest(void);
static void nvdLetAppModeTimeRest(void);
static void nvdLetAppAllLetRest(void);
static void nvdLetAllRestFlame1(void);
static void nvdLetAllRestFlame2(void);
static void nvdLetAllRestFlame3(void);
static void nvdLetAllRestFlame4(void);
static void nvdLetAppDwgRest(void);
static void nvdLetAppDateRest(void);
static void nvdLetAppSerialRest(void);
static void nvdLetAppTunerTestRest(void);
static void nvdLetAppSpkBeepRest(void);
static void nvdLetAppInlineRest(void);
static void nvdLetAppInlineClear(void);
static void nvdLetAppSndVolumeSet(void);
static void nvdLetAppSndBassSet(void);
static void nvdLetAppSndTrebleSet(void);
static void nvdLetAppHotErrRest(void);
static void nvdLetAppResOkSend(void);
static void nvdLetAppResNgBusy(void);
static void nvdLetAppNonCondition(void);
static void nvdLetAppSpkButtonInfo(void);
static void nvdLetAppERRMemClear(void);
/* Let Response Function */
static void nvdLetAppLetStartRes(void);
static void nvdLetAppLetEndRes(void);
static void nvdLetAppAmStartRes(void);
static void nvdLetAppAmAnt1Res(void);
static void nvdLetAppAmAnt2Res(void);
static void nvdLetAppAmAnt3Res(void);
static void nvdLetAppAmAnt1EndRes(void);
static void nvdLetAppAmAnt2EndRes(void);
static void nvdLetAppAmAnt3EndRes(void);
static void nvdLetAppAmAntEndRes(void);
static void nvdLetAppFmStartRes(void);
static void nvdLetAppFmAnt1Res(void);
static void nvdLetAppFmAnt2Res(void);
static void nvdLetAppFmAnt3Res(void);
static void nvdLetAppFmAnt1EndRes(void);
static void nvdLetAppFmAnt2EndRes(void);
static void nvdLetAppFmAnt3EndRes(void);
static void nvdLetAppFmAntEndRes(void);
static void nvdLetAppUsbTestStartRes(void);
static void nvdLetAppUsbSpkStartRes(void);
static void nvdLetAppUsbSpkEndRes(void);
static void nvdLetAppUsbTestEndRes(void);
static void nvdLetAppBibiriStartRes(void);
static void nvdLetAppBibiriSetRes(void);
static void nvdLetAppBibiriEndRes(void);
static void nvdLetAppBsmInitialRes(void);
static void nvdLetAppBsmTpSetStartRes(void);
static void nvdLetAppBsmTestEndRes(void);
static void nvdLetAppBsmRestRes(void);
static void nvdLetAppTunerStartRes(void);
static void nvdLetAppTunerSetRes(void);
static void nvdLetAppTunerEndRes(void);
static void nvdLetAppBeepChkStartRes(void);
static void nvdLetAppBeepChkChUpRes(void);
static void nvdLetAppBeepChkEndRes(void);
static void nvdLetAppBeepDispRes(void);
static void nvdLetAppMemClearStartRes(void);
static void nvdLetAppLetMemClearRes(void);
static void nvdLetAppInitialClearRes(void);
static void nvdLetAppInlineClearRes(void);
static void nvdLetAppMemClearEndRes(void);
static void nvdLetAppVolumeSetRes(void);
static void nvdLetAppBassSetRes(void);
static void nvdLetAppTrebleSetRes(void);
static void nvdLetAppBusyRes(void);
static void nvdLetAppERRMemClearRes(void);

static void nvdLetInlineDiagRes(const Type_uByte * aubEepData_p);
static void nvdLetAppSpkRestChk(Type_uByte* aubEepData_p, Type_uByte aubSize);
/********************************************************************************************/
/*                           ROM Table Section                                              */
/********************************************************************************************/

static const LetModeMatrix_ts nstLetStateCtrlMatrix_a[LET_MODE_MAX][LETAPP_RES_MAX] = {
{ /* LET_MODE_OFFED */
    /** ProcessId                          NextMode                        HighIndexEventNo          **/
    {   PID_LETDIAG_START,                 LET_MODE_WAIT,                  EV_H_INDEX_NO_CHG           },  /* 000 : LETAPP_EV_AUDIO_DIAG_START */
    {   PID_OK_SEND,                       LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 001 : LETAPP_EV_AUDIO_DIAG_END */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 002 : LETAPP_EV_AM_ANT_START */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 003 : LETAPP_EV_AM_ANT1 */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 004 : LETAPP_EV_AM_ANT2 */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 005 : LETAPP_EV_AM_ANT3 */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 006 : LETAPP_EV_AM_ANT1_END */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 007 : LETAPP_EV_AM_ANT2_END */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 008 : LETAPP_EV_AM_ANT3_END */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 009 : LETAPP_EV_AM_ANT_END */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 010 : LETAPP_EV_AM_ANT_REST */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 011 : LETAPP_EV_FM_ANT_START */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 012 : LETAPP_EV_FM_ANT1 */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 013 : LETAPP_EV_FM_ANT2 */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 014 : LETAPP_EV_FM_ANT3 */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 015 : LETAPP_EV_FM_ANT1_END */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 016 : LETAPP_EV_FM_ANT2_END */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 017 : LETAPP_EV_FM_ANT3_END */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 018 : LETAPP_EV_FM_ANT_END */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 019 : LETAPP_EV_FM_ANT_REST */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 020 : LETAPP_EV_USB_TEST_START */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 021 : LETAPP_EV_SPK_CHK_START */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 022 : LETAPP_EV_SPK_CHK_END */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 023 : LETAPP_EV_USB_TEST_END */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 024 : LETAPP_EV_SPK_CHECK_REST */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 025 : LETAPP_EV_SPK_BIBIRI_START */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 026 : LETAPP_EV_SET_BIBIRI_CHECK */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 027 : LETAPP_EV_BIBIRI_CHECK_END */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 028 : LETAPP_EV_BIBIRI_CHK_REST */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 029 : LETAPP_EV_BSM_INITIAL */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 030 : LETAPP_EV_BSM_START_TP_SET */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 031 : LETAPP_EV_BSM_END */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 032 : LETAPP_EV_BSM_REST */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 033 : LETAPP_EV_AUTO_VOL_VSP_REST */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 034 : LETAPP_EV_VIN_COMP_REST */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 035 : LETAPP_EV_AUDIO_VIN_REST */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 036 : LETAPP_EV_TOTAL_TIME_REST */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 037 : LETAPP_EV_MODE_TIME_REST */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 038 : LETAPP_EV_AUDIO_DIAG_REST */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 039 : LETAPP_EV_AUDIO_DWG_REST */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 040 : LETAPP_EV_AUDIO_DATE_REST */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 041 : LETAPP_EV_AUDIO_SERIAL_REST */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 042 : LETAPP_EV_TUNER_TEST_START */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 043 : LETAPP_EV_SET_TUNER_TEST */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 044 : LETAPP_EV_TUNER_TEST_END */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 045 : LETAPP_EV_TUNER_TEST_REST */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 046 : LETAPP_EV_SPK_BEEP_START */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 047 : LETAPP_EV_SPK_BEEP_CH_UP */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 048 : LETAPP_EV_SPK_BEEP_END */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 049 : LETAPP_EV_SPK_BEEP_REST */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 050 : LETAPP_EV_MEMORY_CLEAR_START */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 051 : LETAPP_EV_LET_MEMORY_CLEAR */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 052 : LETAPP_EV_INITIAL_CLEAR */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 053 : LETAPP_EV_MEMORY_CLEAR_END */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 054 : LETAPP_EV_INLINE_DIAG_REST */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 055 : LETAPP_EV_INLINE_DIAG_CLEAR */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 056 : LETAPP_EV_VOL_SET */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 057 : LETAPP_EV_BASS_SET */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 058 : LETAPP_EV_TREBLE_SET */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 059 : LETAPP_EV_HOT_ERR_REST */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 060 : LETAPP_EV_BUTTON_INFO */
    {   PID_NG_NON_CONDITIONS,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 061 : LETAPP_EV_LET_ERR_MEMORY_CLEAR */
    /* LET Response PID Change */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 000 : LETAPP_RES_AUDIO_DIAG_START */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 001 : LETAPP_RES_AUDIO_DIAG_END */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 002 : LETAPP_RES_AM_ANT_START */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 003 : LETAPP_RES_AM_ANT1 */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 004 : LETAPP_RES_AM_ANT2 */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 005 : LETAPP_RES_AM_ANT3 */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 006 : LETAPP_RES_AM_ANT1_END */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 007 : LETAPP_RES_AM_ANT2_END */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 008 : LETAPP_RES_AM_ANT3_END */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 009 : LETAPP_RES_AM_ANT_END */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 010 : LETAPP_RES_FM_ANT_START */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 011 : LETAPP_RES_FM_ANT1 */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 012 : LETAPP_RES_FM_ANT2 */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 013 : LETAPP_RES_FM_ANT3 */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 014 : LETAPP_RES_FM_ANT1_END */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 015 : LETAPP_RES_FM_ANT2_END */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 016 : LETAPP_RES_FM_ANT3_END */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 017 : LETAPP_RES_FM_ANT_END */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 018 : LETAPP_RES_USB_TEST_START */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 019 : LETAPP_RES_SPK_CHK_START */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 020 : LETAPP_RES_SPK_CHK_END */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 021 : LETAPP_RES_USB_TEST_END */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 022 : LETAPP_RES_SPK_BIBIRI_START */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 023 : LETAPP_RES_SET_BIBIRI_CHECK */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 024 : LETAPP_RES_BIBIRI_CHECK_END */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 025 : LETAPP_RES_BSM_INITIAL */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 026 : LETAPP_RES_BSM_START_TP_SET */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 027 : LETAPP_RES_BSM_END */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 028 : LETAPP_RES_BSM_REST */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 029 : LETAPP_RES_TUNER_TEST_START */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 030 : LETAPP_RES_SET_TUNER_TEST */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 031 : LETAPP_RES_TUNER_TEST_END */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 032 : LETAPP_RES_SPK_BEEP_START */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 033 : LETAPP_RES_SPK_BEEP_CH_UP */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 034 : LETAPP_RES_SPK_BEEP_END */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 035 : LETAPP_RES_SPK_DISP */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 036 : LETAPP_RES_MEMORY_CLEAR_START */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 037 : LETAPP_RES_LET_MEMORY_CLEAR */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 038 : LETAPP_RES_INITIAL_CLEAR */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 039 : LETAPP_RES_INLINE_CLEAR */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 040 : LETAPP_RES_MEMORY_CLEAR_END */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 041 : LETAPP_RES_VOLUME_SET */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 042 : LETAPP_RES_BASS_SET */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 043 : LETAPP_RES_TREBLE_SET */    
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 044 : LETAPP_RES_BUSY */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 045 : LETAPP_RES_ERR_MEMORY_CLEAR */
},
{ /* LET_MODE_ONED */
    /** ProcessId                          NextMode                        HighIndexEventNo          **/
    {   PID_OK_SEND,                       LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 000 : LETAPP_EV_AUDIO_DIAG_START */
    {   PID_LETDIAG_END,                   LET_MODE_WAIT,                  EV_H_INDEX_NO_CHG           },  /* 001 : LETAPP_EV_AUDIO_DIAG_END */
    {   PID_HIGH_INDEX,                    LET_MODE_NO_CHG,                EV_H_INDEX_AM_ANT_START     },  /* 002 : LETAPP_EV_AM_ANT_START */
    {   PID_HIGH_INDEX,                    LET_MODE_NO_CHG,                EV_H_INDEX_AM_ANT1          },  /* 003 : LETAPP_EV_AM_ANT1 */
    {   PID_HIGH_INDEX,                    LET_MODE_NO_CHG,                EV_H_INDEX_AM_ANT2          },  /* 004 : LETAPP_EV_AM_ANT2 */
    {   PID_HIGH_INDEX,                    LET_MODE_NO_CHG,                EV_H_INDEX_AM_ANT3          },  /* 005 : LETAPP_EV_AM_ANT3 */
    {   PID_HIGH_INDEX,                    LET_MODE_NO_CHG,                EV_H_INDEX_AM_ANT1_END      },  /* 006 : LETAPP_EV_AM_ANT1_END */
    {   PID_HIGH_INDEX,                    LET_MODE_NO_CHG,                EV_H_INDEX_AM_ANT2_END      },  /* 007 : LETAPP_EV_AM_ANT2_END */
    {   PID_HIGH_INDEX,                    LET_MODE_NO_CHG,                EV_H_INDEX_AM_ANT3_END      },  /* 008 : LETAPP_EV_AM_ANT3_END */
    {   PID_HIGH_INDEX,                    LET_MODE_NO_CHG,                EV_H_INDEX_AM_ANT_END       },  /* 009 : LETAPP_EV_AM_ANT_END */
    {   PID_AM_ANT_REST,                   LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 010 : LETAPP_EV_AM_ANT_REST */
    {   PID_HIGH_INDEX,                    LET_MODE_NO_CHG,                EV_H_INDEX_FM_ANT_START     },  /* 011 : LETAPP_EV_FM_ANT_START */
    {   PID_HIGH_INDEX,                    LET_MODE_NO_CHG,                EV_H_INDEX_FM_ANT1          },  /* 012 : LETAPP_EV_FM_ANT1 */
    {   PID_HIGH_INDEX,                    LET_MODE_NO_CHG,                EV_H_INDEX_FM_ANT2          },  /* 013 : LETAPP_EV_FM_ANT2 */
    {   PID_HIGH_INDEX,                    LET_MODE_NO_CHG,                EV_H_INDEX_FM_ANT3          },  /* 014 : LETAPP_EV_FM_ANT3 */
    {   PID_HIGH_INDEX,                    LET_MODE_NO_CHG,                EV_H_INDEX_FM_ANT1_END      },  /* 015 : LETAPP_EV_FM_ANT1_END */
    {   PID_HIGH_INDEX,                    LET_MODE_NO_CHG,                EV_H_INDEX_FM_ANT2_END      },  /* 016 : LETAPP_EV_FM_ANT2_END */
    {   PID_HIGH_INDEX,                    LET_MODE_NO_CHG,                EV_H_INDEX_FM_ANT3_END      },  /* 017 : LETAPP_EV_FM_ANT3_END */
    {   PID_HIGH_INDEX,                    LET_MODE_NO_CHG,                EV_H_INDEX_FM_ANT_END       },  /* 018 : LETAPP_EV_FM_ANT_END */
    {   PID_FM_ANT_REST,                   LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 019 : LETAPP_EV_FM_ANT_REST */
    {   PID_HIGH_INDEX,                    LET_MODE_NO_CHG,                EV_H_INDEX_USB_TEST_START   },  /* 020 : LETAPP_EV_USB_TEST_START */
    {   PID_HIGH_INDEX,                    LET_MODE_NO_CHG,                EV_H_INDEX_SPK_CHK_START    },  /* 021 : LETAPP_EV_SPK_CHK_START */
    {   PID_HIGH_INDEX,                    LET_MODE_NO_CHG,                EV_H_INDEX_SPK_CHK_END      },  /* 022 : LETAPP_EV_SPK_CHK_END */
    {   PID_HIGH_INDEX,                    LET_MODE_NO_CHG,                EV_H_INDEX_USB_TEST_END     },  /* 023 : LETAPP_EV_USB_TEST_END */
    {   PID_SPK_CHECK_REST,                LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 024 : LETAPP_EV_SPK_CHECK_REST */
    {   PID_HIGH_INDEX,                    LET_MODE_NO_CHG,                EV_H_INDEX_SPK_BIBIRI_START },  /* 025 : LETAPP_EV_SPK_BIBIRI_START */
    {   PID_HIGH_INDEX,                    LET_MODE_NO_CHG,                EV_H_INDEX_SET_BIBIRI_CHECK },  /* 026 : LETAPP_EV_SET_BIBIRI_CHECK */
    {   PID_HIGH_INDEX,                    LET_MODE_NO_CHG,                EV_H_INDEX_BIBIRI_CHECK_END },  /* 027 : LETAPP_EV_BIBIRI_CHECK_END */
    {   PID_BIBIRI_CHK_REST,               LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 028 : LETAPP_EV_BIBIRI_CHK_REST */
    {   PID_BSM_INITIAL,                   LET_MODE_WAIT,                  EV_H_INDEX_NO_CHG           },  /* 029 : LETAPP_EV_BSM_INITIAL */
    {   PID_HIGH_INDEX,                    LET_MODE_NO_CHG,                EV_H_INDEX_BSM_START_TP_SET },  /* 030 : LETAPP_EV_BSM_START_TP_SET */
    {   PID_HIGH_INDEX,                    LET_MODE_NO_CHG,                EV_H_INDEX_BSM_END          },  /* 031 : LETAPP_EV_BSM_END */
    {   PID_BSM_CHECK_REST,                LET_MODE_WAIT,                  EV_H_INDEX_NO_CHG           },  /* 032 : LETAPP_EV_BSM_REST */
    {   PID_AUTO_VOL_VSP_REST,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 033 : LETAPP_EV_AUTO_VOL_VSP_REST */
    {   PID_VIN_COMP_REST,                 LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 034 : LETAPP_EV_VIN_COMP_REST */
    {   PID_AUDIO_VIN_REST,                LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 035 : LETAPP_EV_AUDIO_VIN_REST */
    {   PID_TOTAL_TIME_REST,               LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 036 : LETAPP_EV_TOTAL_TIME_REST */
    {   PID_MODE_TIME_REST,                LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 037 : LETAPP_EV_MODE_TIME_REST */
    {   PID_AUDIO_DIAG_REST,               LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 038 : LETAPP_EV_AUDIO_DIAG_REST */
    {   PID_AUDIO_DWG_REST,                LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 039 : LETAPP_EV_AUDIO_DWG_REST */
    {   PID_AUDIO_DATE_REST,               LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 040 : LETAPP_EV_AUDIO_DATE_REST */
    {   PID_AUDIO_SERIAL_REST,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 041 : LETAPP_EV_AUDIO_SERIAL_REST */
    {   PID_HIGH_INDEX,                    LET_MODE_NO_CHG,                EV_H_INDEX_TUNER_TEST_START },  /* 042 : LETAPP_EV_TUNER_TEST_START */
    {   PID_HIGH_INDEX,                    LET_MODE_NO_CHG,                EV_H_INDEX_SET_TUNER_TEST   },  /* 043 : LETAPP_EV_SET_TUNER_TEST */
    {   PID_HIGH_INDEX,                    LET_MODE_NO_CHG,                EV_H_INDEX_TUNER_TEST_END   },  /* 044 : LETAPP_EV_TUNER_TEST_END */
    {   PID_TUNER_TEST_REST,               LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 045 : LETAPP_EV_TUNER_TEST_REST */
    {   PID_HIGH_INDEX,                    LET_MODE_NO_CHG,                EV_H_INDEX_SPK_BEEP_START   },  /* 046 : LETAPP_EV_SPK_BEEP_START */
    {   PID_HIGH_INDEX,                    LET_MODE_NO_CHG,                EV_H_INDEX_SPK_BEEP_CH_UP   },  /* 047 : LETAPP_EV_SPK_BEEP_CH_UP */
    {   PID_HIGH_INDEX,                    LET_MODE_NO_CHG,                EV_H_INDEX_SPK_BEEP_END     },  /* 048 : LETAPP_EV_SPK_BEEP_END */
    {   PID_SPK_BEEP_REST,                 LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 049 : LETAPP_EV_SPK_BEEP_REST */
    {   PID_HIGH_INDEX,                    LET_MODE_NO_CHG,                EV_H_INDEX_MEM_CLEAR_START  },  /* 050 : LETAPP_EV_MEMORY_CLEAR_START */
    {   PID_HIGH_INDEX,                    LET_MODE_NO_CHG,                EV_H_INDEX_LET_MEM_CLEAR    },  /* 051 : LETAPP_EV_LET_MEMORY_CLEAR */
    {   PID_HIGH_INDEX,                    LET_MODE_NO_CHG,                EV_H_INDEX_INITIAL_CLEAR    },  /* 052 : LETAPP_EV_INITIAL_CLEAR */
    {   PID_HIGH_INDEX,                    LET_MODE_NO_CHG,                EV_H_INDEX_MEM_CLEAR_END    },  /* 053 : LETAPP_EV_MEMORY_CLEAR_END */
    {   PID_INLINE_DIAG_REST,              LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 054 : LETAPP_EV_INLINE_DIAG_REST */
    {   PID_HIGH_INDEX,                    LET_MODE_NO_CHG,                EV_H_INDEX_INLINE_CLEAR     },  /* 055 : LETAPP_EV_INLINE_DIAG_CLEAR */
    {   PID_SND_VOLUME_SET,                LET_MODE_WAIT,                  EV_H_INDEX_NO_CHG           },  /* 056 : LETAPP_EV_VOL_SET */
    {   PID_SND_BASS_SET,                  LET_MODE_WAIT,                  EV_H_INDEX_NO_CHG           },  /* 057 : LETAPP_EV_BASS_SET */
    {   PID_SND_TREBLE_SET,                LET_MODE_WAIT,                  EV_H_INDEX_NO_CHG           },  /* 058 : LETAPP_EV_TREBLE_SET */
    {   PID_HOT_ERR_REST,                  LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 059 : LETAPP_EV_HOT_ERR_REST */
    {   PID_HIGH_INDEX,                    LET_MODE_NO_CHG,                EV_H_INDEX_SPK_BUTTON       },  /* 060 : LETAPP_EV_BUTTON_INFO */
    {   PID_HIGH_INDEX,                    LET_MODE_NO_CHG,                EV_H_INDEX_EER_MEMORY_CLEAR },  /* 061 : LETAPP_EV_LET_ERR_MEMORY_CLEAR */
    /* LET Response PID Change */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 000 : LETAPP_RES_AUDIO_DIAG_START */
    {   PID_RES_AUDIO_DIAG_END,            LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 001 : LETAPP_RES_AUDIO_DIAG_END */
    {   PID_RES_AM_ANT_START,              LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 002 : LETAPP_RES_AM_ANT_START */
    {   PID_RES_AM_ANT1,                   LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 003 : LETAPP_RES_AM_ANT1 */
    {   PID_RES_AM_ANT2,                   LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 004 : LETAPP_RES_AM_ANT2 */
    {   PID_RES_AM_ANT3,                   LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 005 : LETAPP_RES_AM_ANT3 */
    {   PID_RES_AM_ANT1_END,               LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 006 : LETAPP_RES_AM_ANT1_END */
    {   PID_RES_AM_ANT2_END,               LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 007 : LETAPP_RES_AM_ANT2_END */
    {   PID_RES_AM_ANT3_END,               LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 008 : LETAPP_RES_AM_ANT3_END */
    {   PID_RES_AM_ANT_END,                LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 009 : LETAPP_RES_AM_ANT_END */
    {   PID_RES_FM_ANT_START,              LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 010 : LETAPP_RES_FM_ANT_START */
    {   PID_RES_FM_ANT1,                   LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 011 : LETAPP_RES_FM_ANT1 */
    {   PID_RES_FM_ANT2,                   LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 012 : LETAPP_RES_FM_ANT2 */
    {   PID_RES_FM_ANT3,                   LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 013 : LETAPP_RES_FM_ANT3 */
    {   PID_RES_FM_ANT1_END,               LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 014 : LETAPP_RES_FM_ANT1_END */
    {   PID_RES_FM_ANT2_END,               LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 015 : LETAPP_RES_FM_ANT2_END */
    {   PID_RES_FM_ANT3_END,               LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 016 : LETAPP_RES_FM_ANT3_END */
    {   PID_RES_FM_ANT_END,                LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 017 : LETAPP_RES_FM_ANT_END */
    {   PID_RES_USB_TEST_START,            LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 018 : LETAPP_RES_USB_TEST_START */
    {   PID_RES_SPK_CHK_START,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 019 : LETAPP_RES_SPK_CHK_START */
    {   PID_RES_SPK_CHK_END,               LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 020 : LETAPP_RES_SPK_CHK_END */
    {   PID_RES_USB_TEST_END,              LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 021 : LETAPP_RES_USB_TEST_END */
    {   PID_RES_SPK_BIBIRI_START,          LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 022 : LETAPP_RES_SPK_BIBIRI_START */
    {   PID_RES_SET_BIBIRI_CHECK,          LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 023 : LETAPP_RES_SET_BIBIRI_CHECK */
    {   PID_RES_BIBIRI_CHECK_END,          LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 024 : LETAPP_RES_BIBIRI_CHECK_END */
    {   PID_RES_BSM_INITIAL,               LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 025 : LETAPP_RES_BSM_INITIAL */
    {   PID_RES_BSM_START_TP_SET,          LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 026 : LETAPP_RES_BSM_START_TP_SET */
    {   PID_RES_BSM_END,                   LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 027 : LETAPP_RES_BSM_END */
    {   PID_RES_BSM_REST,                  LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 028 : LETAPP_RES_BSM_REST */
    {   PID_RES_TUNER_TEST_START,          LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 029 : LETAPP_RES_TUNER_TEST_START */
    {   PID_RES_SET_TUNER_TEST,            LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 030 : LETAPP_RES_SET_TUNER_TEST */
    {   PID_RES_TUNER_TEST_END,            LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 031 : LETAPP_RES_TUNER_TEST_END */
    {   PID_RES_SPK_BEEP_START,            LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 032 : LETAPP_RES_SPK_BEEP_START */
    {   PID_RES_SPK_BEEP_CH_UP,            LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 033 : LETAPP_RES_SPK_BEEP_CH_UP */
    {   PID_RES_SPK_BEEP_END,              LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 034 : LETAPP_RES_SPK_BEEP_END */
    {   PID_RES_SPK_DISP,                  LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 035 : LETAPP_RES_SPK_DISP */
    {   PID_RES_MEMORY_CLEAR_START,        LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 036 : LETAPP_RES_MEMORY_CLEAR_START */
    {   PID_RES_LET_MEMORY_CLEAR,          LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 037 : LETAPP_RES_LET_MEMORY_CLEAR */
    {   PID_RES_INITIAL_CLEAR,             LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 038 : LETAPP_RES_INITIAL_CLEAR */
    {   PID_RES_INLINE_CLEAR,              LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 039 : LETAPP_RES_INLINE_CLEAR */
    {   PID_RES_MEMORY_CLEAR_END,          LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 040 : LETAPP_RES_MEMORY_CLEAR_END */
    {   PID_RES_VOLUME_SET,                LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 041 : LETAPP_RES_VOLUME_SET */
    {   PID_RES_BASS_SET,                  LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 042 : LETAPP_RES_BASS_SET */
    {   PID_RES_TREBLE_SET,                LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 043 : LETAPP_RES_TREBLE_SET */    
    {   PID_RES_BUSY,                      LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 044 : LETAPP_RES_BUSY */
    {   PID_RES_EER_MEMORY_CLEAR,          LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 045 : LETAPP_RES_ERR_MEMORY_CLEAR */
},
{ /* LET_MODE_WAIT */
    /** ProcessId                          NextMode                        HighIndexEventNo          **/
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 000 : LETAPP_EV_AUDIO_DIAG_START */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 001 : LETAPP_EV_AUDIO_DIAG_END */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 002 : LETAPP_EV_AM_ANT_START */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 003 : LETAPP_EV_AM_ANT1 */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 004 : LETAPP_EV_AM_ANT2 */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 005 : LETAPP_EV_AM_ANT3 */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 006 : LETAPP_EV_AM_ANT1_END */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 007 : LETAPP_EV_AM_ANT2_END */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 008 : LETAPP_EV_AM_ANT3_END */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 009 : LETAPP_EV_AM_ANT_END */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 010 : LETAPP_EV_AM_ANT_REST */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 011 : LETAPP_EV_FM_ANT_START */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 012 : LETAPP_EV_FM_ANT1 */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 013 : LETAPP_EV_FM_ANT2 */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 014 : LETAPP_EV_FM_ANT3 */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 015 : LETAPP_EV_FM_ANT1_END */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 016 : LETAPP_EV_FM_ANT2_END */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 017 : LETAPP_EV_FM_ANT3_END */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 018 : LETAPP_EV_FM_ANT_END */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 019 : LETAPP_EV_FM_ANT_REST */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 020 : LETAPP_EV_USB_TEST_START */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 021 : LETAPP_EV_SPK_CHK_START */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 022 : LETAPP_EV_SPK_CHK_END */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 023 : LETAPP_EV_USB_TEST_END */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 024 : LETAPP_EV_SPK_CHECK_REST */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 025 : LETAPP_EV_SPK_BIBIRI_START */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 026 : LETAPP_EV_SET_BIBIRI_CHECK */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 027 : LETAPP_EV_BIBIRI_CHECK_END */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 028 : LETAPP_EV_BIBIRI_CHK_REST */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 029 : LETAPP_EV_BSM_INITIAL */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 030 : LETAPP_EV_BSM_START_TP_SET */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 031 : LETAPP_EV_BSM_END */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 032 : LETAPP_EV_BSM_REST */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 033 : LETAPP_EV_AUTO_VOL_VSP_REST */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 034 : LETAPP_EV_VIN_COMP_REST */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 035 : LETAPP_EV_AUDIO_VIN_REST */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 036 : LETAPP_EV_TOTAL_TIME_REST */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 037 : LETAPP_EV_MODE_TIME_REST */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 038 : LETAPP_EV_AUDIO_DIAG_REST */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 039 : LETAPP_EV_AUDIO_DWG_REST */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 040 : LETAPP_EV_AUDIO_DATE_REST */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 041 : LETAPP_EV_AUDIO_SERIAL_REST */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 042 : LETAPP_EV_TUNER_TEST_START */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 043 : LETAPP_EV_SET_TUNER_TEST */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 044 : LETAPP_EV_TUNER_TEST_END */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 045 : LETAPP_EV_TUNER_TEST_REST */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 046 : LETAPP_EV_SPK_BEEP_START */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 047 : LETAPP_EV_SPK_BEEP_CH_UP */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 048 : LETAPP_EV_SPK_BEEP_END */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 049 : LETAPP_EV_SPK_BEEP_REST */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 050 : LETAPP_EV_MEMORY_CLEAR_START */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 051 : LETAPP_EV_LET_MEMORY_CLEAR */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 052 : LETAPP_EV_INITIAL_CLEAR */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 053 : LETAPP_EV_MEMORY_CLEAR_END */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 054 : LETAPP_EV_INLINE_DIAG_REST */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 055 : LETAPP_EV_INLINE_DIAG_CLEAR */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 056 : LETAPP_EV_VOL_SET */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 057 : LETAPP_EV_BASS_SET */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 058 : LETAPP_EV_TREBLE_SET */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 059 : LETAPP_EV_HOT_ERR_REST */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 060 : LETAPP_EV_BUTTON_INFO */
    {   PID_NO_ACTION,                     LET_MODE_NO_CHG,                EV_H_INDEX_NO_CHG           },  /* 061 : LETAPP_EV_LET_ERR_MEMORY_CLEAR */
    /* LET Response PID Change */
    {   PID_RES_AUDIO_DIAG_START,          LET_MODE_ONED,                  EV_H_INDEX_NO_CHG           },  /* 000 : LETAPP_RES_AUDIO_DIAG_START */
    {   PID_RES_AUDIO_DIAG_END,            LET_MODE_OFFED,                 EV_H_INDEX_NO_CHG           },  /* 001 : LETAPP_RES_AUDIO_DIAG_END */
    {   PID_RES_AM_ANT_START,              LET_MODE_ONED,                  EV_H_INDEX_NO_CHG           },  /* 002 : LETAPP_RES_AM_ANT_START */
    {   PID_RES_AM_ANT1,                   LET_MODE_ONED,                  EV_H_INDEX_NO_CHG           },  /* 003 : LETAPP_RES_AM_ANT1 */
    {   PID_RES_AM_ANT2,                   LET_MODE_ONED,                  EV_H_INDEX_NO_CHG           },  /* 004 : LETAPP_RES_AM_ANT2 */
    {   PID_RES_AM_ANT3,                   LET_MODE_ONED,                  EV_H_INDEX_NO_CHG           },  /* 005 : LETAPP_RES_AM_ANT3 */
    {   PID_RES_AM_ANT1_END,               LET_MODE_ONED,                  EV_H_INDEX_NO_CHG           },  /* 006 : LETAPP_RES_AM_ANT1_END */
    {   PID_RES_AM_ANT2_END,               LET_MODE_ONED,                  EV_H_INDEX_NO_CHG           },  /* 007 : LETAPP_RES_AM_ANT2_END */
    {   PID_RES_AM_ANT3_END,               LET_MODE_ONED,                  EV_H_INDEX_NO_CHG           },  /* 008 : LETAPP_RES_AM_ANT3_END */
    {   PID_RES_AM_ANT_END,                LET_MODE_ONED,                  EV_H_INDEX_NO_CHG           },  /* 009 : LETAPP_RES_AM_ANT_END */
    {   PID_RES_FM_ANT_START,              LET_MODE_ONED,                  EV_H_INDEX_NO_CHG           },  /* 010 : LETAPP_RES_FM_ANT_START */
    {   PID_RES_FM_ANT1,                   LET_MODE_ONED,                  EV_H_INDEX_NO_CHG           },  /* 011 : LETAPP_RES_FM_ANT1 */
    {   PID_RES_FM_ANT2,                   LET_MODE_ONED,                  EV_H_INDEX_NO_CHG           },  /* 012 : LETAPP_RES_FM_ANT2 */
    {   PID_RES_FM_ANT3,                   LET_MODE_ONED,                  EV_H_INDEX_NO_CHG           },  /* 013 : LETAPP_RES_FM_ANT3 */
    {   PID_RES_FM_ANT1_END,               LET_MODE_ONED,                  EV_H_INDEX_NO_CHG           },  /* 014 : LETAPP_RES_FM_ANT1_END */
    {   PID_RES_FM_ANT2_END,               LET_MODE_ONED,                  EV_H_INDEX_NO_CHG           },  /* 015 : LETAPP_RES_FM_ANT2_END */
    {   PID_RES_FM_ANT3_END,               LET_MODE_ONED,                  EV_H_INDEX_NO_CHG           },  /* 016 : LETAPP_RES_FM_ANT3_END */
    {   PID_RES_FM_ANT_END,                LET_MODE_ONED,                  EV_H_INDEX_NO_CHG           },  /* 017 : LETAPP_RES_FM_ANT_END */
    {   PID_RES_USB_TEST_START,            LET_MODE_ONED,                  EV_H_INDEX_NO_CHG           },  /* 018 : LETAPP_RES_USB_TEST_START */
    {   PID_RES_SPK_CHK_START,             LET_MODE_ONED,                  EV_H_INDEX_NO_CHG           },  /* 019 : LETAPP_RES_SPK_CHK_START */
    {   PID_RES_SPK_CHK_END,               LET_MODE_ONED,                  EV_H_INDEX_NO_CHG           },  /* 020 : LETAPP_RES_SPK_CHK_END */
    {   PID_RES_USB_TEST_END,              LET_MODE_ONED,                  EV_H_INDEX_NO_CHG           },  /* 021 : LETAPP_RES_USB_TEST_END */
    {   PID_RES_SPK_BIBIRI_START,          LET_MODE_ONED,                  EV_H_INDEX_NO_CHG           },  /* 022 : LETAPP_RES_SPK_BIBIRI_START */
    {   PID_RES_SET_BIBIRI_CHECK,          LET_MODE_ONED,                  EV_H_INDEX_NO_CHG           },  /* 023 : LETAPP_RES_SET_BIBIRI_CHECK */
    {   PID_RES_BIBIRI_CHECK_END,          LET_MODE_ONED,                  EV_H_INDEX_NO_CHG           },  /* 024 : LETAPP_RES_BIBIRI_CHECK_END */
    {   PID_RES_BSM_INITIAL,               LET_MODE_ONED,                  EV_H_INDEX_NO_CHG           },  /* 025 : LETAPP_RES_BSM_INITIAL */
    {   PID_RES_BSM_START_TP_SET,          LET_MODE_ONED,                  EV_H_INDEX_NO_CHG           },  /* 026 : LETAPP_RES_BSM_START_TP_SET */
    {   PID_RES_BSM_END,                   LET_MODE_ONED,                  EV_H_INDEX_NO_CHG           },  /* 027 : LETAPP_RES_BSM_END */
    {   PID_RES_BSM_REST,                  LET_MODE_ONED,                  EV_H_INDEX_NO_CHG           },  /* 028 : LETAPP_RES_BSM_REST */
    {   PID_RES_TUNER_TEST_START,          LET_MODE_ONED,                  EV_H_INDEX_NO_CHG           },  /* 029 : LETAPP_RES_TUNER_TEST_START */
    {   PID_RES_SET_TUNER_TEST,            LET_MODE_ONED,                  EV_H_INDEX_NO_CHG           },  /* 030 : LETAPP_RES_SET_TUNER_TEST */
    {   PID_RES_TUNER_TEST_END,            LET_MODE_ONED,                  EV_H_INDEX_NO_CHG           },  /* 031 : LETAPP_RES_TUNER_TEST_END */
    {   PID_RES_SPK_BEEP_START,            LET_MODE_ONED,                  EV_H_INDEX_NO_CHG           },  /* 032 : LETAPP_RES_SPK_BEEP_START */
    {   PID_RES_SPK_BEEP_CH_UP,            LET_MODE_ONED,                  EV_H_INDEX_NO_CHG           },  /* 033 : LETAPP_RES_SPK_BEEP_CH_UP */
    {   PID_RES_SPK_BEEP_END,              LET_MODE_ONED,                  EV_H_INDEX_NO_CHG           },  /* 034 : LETAPP_RES_SPK_BEEP_END */
    {   PID_RES_SPK_DISP,                  LET_MODE_ONED,                  EV_H_INDEX_NO_CHG           },  /* 035 : LETAPP_RES_SPK_DISP */
    {   PID_RES_MEMORY_CLEAR_START,        LET_MODE_ONED,                  EV_H_INDEX_NO_CHG           },  /* 036 : LETAPP_RES_MEMORY_CLEAR_START */
    {   PID_RES_LET_MEMORY_CLEAR,          LET_MODE_ONED,                  EV_H_INDEX_NO_CHG           },  /* 037 : LETAPP_RES_LET_MEMORY_CLEAR */
    {   PID_RES_INITIAL_CLEAR,             LET_MODE_ONED,                  EV_H_INDEX_NO_CHG           },  /* 038 : LETAPP_RES_INITIAL_CLEAR */
    {   PID_RES_INLINE_CLEAR,              LET_MODE_ONED,                  EV_H_INDEX_NO_CHG           },  /* 039 : LETAPP_RES_INLINE_CLEAR */
    {   PID_RES_MEMORY_CLEAR_END,          LET_MODE_ONED,                  EV_H_INDEX_NO_CHG           },  /* 040 : LETAPP_RES_MEMORY_CLEAR_END */
    {   PID_RES_VOLUME_SET,                LET_MODE_ONED,                  EV_H_INDEX_NO_CHG           },  /* 041 : LETAPP_RES_VOLUME_SET */
    {   PID_RES_BASS_SET,                  LET_MODE_ONED,                  EV_H_INDEX_NO_CHG           },  /* 042 : LETAPP_RES_BASS_SET */
    {   PID_RES_TREBLE_SET,                LET_MODE_ONED,                  EV_H_INDEX_NO_CHG           },  /* 043 : LETAPP_RES_TREBLE_SET */    
    {   PID_RES_BUSY,                      LET_MODE_ONED,                  EV_H_INDEX_NO_CHG           },  /* 044 : LETAPP_RES_BUSY */
    {   PID_RES_EER_MEMORY_CLEAR,          LET_MODE_ONED,                  EV_H_INDEX_NO_CHG           },  /* 045 : LETAPP_RES_ERR_MEMORY_CLEAR */
}
};

static const HighIndexMatrix_ts nstHighIndexMatrix_a[H_INDEX_MODE_MAX][EV_H_INDEX_MAX] = {
{ /* H_INDEX_MODE_WAIT */
    /** ProcessId                          NextMode                        LowIndexEventNo          **/
    {   PID_LET_AM_TEST,                   H_INDEX_MODE_FMAM,              EV_L_INDEX_AM_ANT_START     },  /* 000 : EV_H_INDEX_AM_ANT_START */
    {   PID_NO_ACTION,                     H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 001 : EV_H_INDEX_AM_ANT1 */
    {   PID_NO_ACTION,                     H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 002 : EV_H_INDEX_AM_ANT2 */
    {   PID_NO_ACTION,                     H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 003 : EV_H_INDEX_AM_ANT3 */
    {   PID_NO_ACTION,                     H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 004 : EV_H_INDEX_AM_ANT1_END */
    {   PID_NO_ACTION,                     H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 005 : EV_H_INDEX_AM_ANT2_END */
    {   PID_NO_ACTION,                     H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 006 : EV_H_INDEX_AM_ANT3_END */
    {   PID_NO_ACTION,                     H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 007 : EV_H_INDEX_AM_ANT_END */
    {   PID_LET_FM_TEST,                   H_INDEX_MODE_FMAM,              EV_L_INDEX_FM_ANT_START     },  /* 008 : EV_H_INDEX_FM_ANT_START */
    {   PID_NO_ACTION,                     H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 009 : EV_H_INDEX_FM_ANT1 */
    {   PID_NO_ACTION,                     H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 010 : EV_H_INDEX_FM_ANT2 */
    {   PID_NO_ACTION,                     H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 011 : EV_H_INDEX_FM_ANT3 */
    {   PID_NO_ACTION,                     H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 012 : EV_H_INDEX_FM_ANT1_END */
    {   PID_NO_ACTION,                     H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 013 : EV_H_INDEX_FM_ANT2_END */
    {   PID_NO_ACTION,                     H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 014 : EV_H_INDEX_FM_ANT3_END */
    {   PID_NO_ACTION,                     H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 015 : EV_H_INDEX_FM_ANT_END */
    {   PID_LET_USB_SPK_TEST,              H_INDEX_MODE_USB_SPK,           EV_L_INDEX_USB_TEST_START   },  /* 016 : EV_H_INDEX_USB_TEST_START */
    {   PID_NO_ACTION,                     H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 017 : EV_H_INDEX_SPK_CHK_START */
    {   PID_NO_ACTION,                     H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 018 : EV_H_INDEX_SPK_CHK_END */
    {   PID_NO_ACTION,                     H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 019 : EV_H_INDEX_USB_TEST_END */
    {   PID_LET_USB_SPK_TEST,              H_INDEX_MODE_USB_SPK,           EV_L_INDEX_SPK_BIBIRI_START },  /* 020 : EV_H_INDEX_SPK_BIBIRI_START */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 021 : EV_H_INDEX_SET_BIBIRI_CHECK */
    {   PID_NO_ACTION,                     H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 022 : EV_H_INDEX_BIBIRI_CHECK_END */
    {   PID_LET_BSM_TEST,                  H_INDEX_MODE_BSM,               EV_L_INDEX_BSM_START_TP_SET },  /* 023 : EV_H_INDEX_BSM_START_TP_SET */
    {   PID_LET_BSM_TEST,                  H_INDEX_MODE_NO_CHG,            EV_L_INDEX_BSM_END            },  /* 024 : EV_H_INDEX_BSM_END */
    {   PID_LET_TUNER_TEST,                H_INDEX_MODE_FMAM,              EV_L_INDEX_TUNER_TEST_START },  /* 025 : EV_H_INDEX_TUNER_TEST_START */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 026 : EV_H_INDEX_SET_TUNER_TEST */
    {   PID_NO_ACTION,                     H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 027 : EV_H_INDEX_TUNER_TEST_END */
    {   PID_LET_SPK_BEEP_TEST,             H_INDEX_MODE_SPK_BEEP,          EV_L_INDEX_SPK_BEEP_START   },  /* 028 : EV_H_INDEX_SPK_BEEP_START */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 029 : EV_H_INDEX_SPK_BEEP_CH_UP */
    {   PID_NO_ACTION,                     H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 030 : EV_H_INDEX_SPK_BEEP_END */
    {   PID_LET_MEM_CLEAR_TEST,            H_INDEX_MODE_MEM_CLEAR,         EV_L_INDEX_MEM_CLEAR_START  },  /* 031 : EV_H_INDEX_MEM_CLEAR_START */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 032 : EV_H_INDEX_LET_MEM_CLEAR */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 033 : EV_H_INDEX_INITIAL_CLEAR */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 034 : EV_H_INDEX_INLINE_CLEAR */
    {   PID_NO_ACTION,                     H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 035 : EV_H_INDEX_MEM_CLEAR_END */
    {   PID_NO_ACTION,                     H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 036 : EV_H_INDEX_SPK_BUTTON */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 037 : EV_H_INDEX_EER_MEMORY_CLEAR */
},
{ /* H_INDEX_MODE_FMAM */
    /** ProcessId                          NextMode                        LowIndexEventNo          **/
    {   PID_LET_AM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_AM_ANT_START     },  /* 000 : EV_H_INDEX_AM_ANT_START */
    {   PID_LET_AM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_AM_ANT1          },  /* 001 : EV_H_INDEX_AM_ANT1 */
    {   PID_LET_AM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_AM_ANT2          },  /* 002 : EV_H_INDEX_AM_ANT2 */
    {   PID_LET_AM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_AM_ANT3          },  /* 003 : EV_H_INDEX_AM_ANT3 */
    {   PID_LET_AM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_AM_ANT1_END      },  /* 004 : EV_H_INDEX_AM_ANT1_END */
    {   PID_LET_AM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_AM_ANT2_END      },  /* 005 : EV_H_INDEX_AM_ANT2_END */
    {   PID_LET_AM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_AM_ANT3_END      },  /* 006 : EV_H_INDEX_AM_ANT3_END */
    {   PID_LET_AM_TEST,                   H_INDEX_MODE_WAIT,              EV_L_INDEX_AM_ANT_END       },  /* 007 : EV_H_INDEX_AM_ANT_END */
    {   PID_LET_FM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_FM_ANT_START     },  /* 008 : EV_H_INDEX_FM_ANT_START */
    {   PID_LET_FM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_FM_ANT1          },  /* 009 : EV_H_INDEX_FM_ANT1 */
    {   PID_LET_FM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_FM_ANT2          },  /* 010 : EV_H_INDEX_FM_ANT2 */
    {   PID_LET_FM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_FM_ANT3          },  /* 011 : EV_H_INDEX_FM_ANT3 */
    {   PID_LET_FM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_FM_ANT1_END      },  /* 012 : EV_H_INDEX_FM_ANT1_END */
    {   PID_LET_FM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_FM_ANT2_END      },  /* 013 : EV_H_INDEX_FM_ANT2_END */
    {   PID_LET_FM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_FM_ANT3_END      },  /* 014 : EV_H_INDEX_FM_ANT3_END */
    {   PID_LET_FM_TEST,                   H_INDEX_MODE_WAIT,              EV_L_INDEX_FM_ANT_END       },  /* 015 : EV_H_INDEX_FM_ANT_END */
    {   PID_LET_USB_SPK_TEST,              H_INDEX_MODE_USB_SPK,           EV_L_INDEX_USB_TEST_START   },  /* 016 : EV_H_INDEX_USB_TEST_START */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 017 : EV_H_INDEX_SPK_CHK_START */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 018 : EV_H_INDEX_SPK_CHK_END */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 019 : EV_H_INDEX_USB_TEST_END */
    {   PID_LET_USB_SPK_TEST,              H_INDEX_MODE_USB_SPK,           EV_L_INDEX_SPK_BIBIRI_START },  /* 020 : EV_H_INDEX_SPK_BIBIRI_START */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 021 : EV_H_INDEX_SET_BIBIRI_CHECK */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 022 : EV_H_INDEX_BIBIRI_CHECK_END */
    {   PID_LET_BSM_TEST,                  H_INDEX_MODE_BSM,               EV_L_INDEX_BSM_START_TP_SET },  /* 023 : EV_H_INDEX_BSM_START_TP_SET */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 024 : EV_H_INDEX_BSM_END */
    {   PID_LET_TUNER_TEST,                H_INDEX_MODE_NO_CHG,            EV_L_INDEX_TUNER_TEST_START },  /* 025 : EV_H_INDEX_TUNER_TEST_START */
    {   PID_LET_TUNER_TEST,                H_INDEX_MODE_NO_CHG,            EV_L_INDEX_SET_TUNER_TEST   },  /* 026 : EV_H_INDEX_SET_TUNER_TEST */
    {   PID_LET_TUNER_TEST,                H_INDEX_MODE_WAIT,              EV_L_INDEX_TUNER_TEST_END   },  /* 027 : EV_H_INDEX_TUNER_TEST_END */
    {   PID_LET_SPK_BEEP_TEST,             H_INDEX_MODE_SPK_BEEP,          EV_L_INDEX_SPK_BEEP_START   },  /* 028 : EV_H_INDEX_SPK_BEEP_START */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 029 : EV_H_INDEX_SPK_BEEP_CH_UP */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 030 : EV_H_INDEX_SPK_BEEP_END */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 031 : EV_H_INDEX_MEM_CLEAR_START */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 032 : EV_H_INDEX_LET_MEM_CLEAR */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 033 : EV_H_INDEX_INITIAL_CLEAR */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 034 : EV_H_INDEX_INLINE_CLEAR */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 035 : EV_H_INDEX_MEM_CLEAR_END */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 036 : EV_H_INDEX_SPK_BUTTON */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 037 : EV_H_INDEX_EER_MEMORY_CLEAR */
},
{ /* H_INDEX_MODE_USB_SPK */
    /** ProcessId                          NextMode                        LowIndexEventNo          **/
    {   PID_LET_AM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_AM_ANT_START     },  /* 000 : EV_H_INDEX_AM_ANT_START */
    {   PID_LET_AM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_AM_ANT1          },  /* 001 : EV_H_INDEX_AM_ANT1 */
    {   PID_LET_AM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_AM_ANT2          },  /* 002 : EV_H_INDEX_AM_ANT2 */
    {   PID_LET_AM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_AM_ANT3          },  /* 003 : EV_H_INDEX_AM_ANT3 */
    {   PID_LET_AM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_AM_ANT1_END      },  /* 004 : EV_H_INDEX_AM_ANT1_END */
    {   PID_LET_AM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_AM_ANT2_END      },  /* 005 : EV_H_INDEX_AM_ANT2_END */
    {   PID_LET_AM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_AM_ANT3_END      },  /* 006 : EV_H_INDEX_AM_ANT3_END */
    {   PID_LET_AM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_AM_ANT_END       },  /* 007 : EV_H_INDEX_AM_ANT_END */
    {   PID_LET_FM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_FM_ANT_START     },  /* 008 : EV_H_INDEX_FM_ANT_START */
    {   PID_LET_FM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_FM_ANT1          },  /* 009 : EV_H_INDEX_FM_ANT1 */
    {   PID_LET_FM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_FM_ANT2          },  /* 010 : EV_H_INDEX_FM_ANT2 */
    {   PID_LET_FM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_FM_ANT3          },  /* 011 : EV_H_INDEX_FM_ANT3 */
    {   PID_LET_FM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_FM_ANT1_END      },  /* 012 : EV_H_INDEX_FM_ANT1_END */
    {   PID_LET_FM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_FM_ANT2_END      },  /* 013 : EV_H_INDEX_FM_ANT2_END */
    {   PID_LET_FM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_FM_ANT3_END      },  /* 014 : EV_H_INDEX_FM_ANT3_END */
    {   PID_LET_FM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_FM_ANT_END       },  /* 015 : EV_H_INDEX_FM_ANT_END */
    {   PID_LET_USB_SPK_TEST,              H_INDEX_MODE_NO_CHG,            EV_L_INDEX_USB_TEST_START   },  /* 016 : EV_H_INDEX_USB_TEST_START */
    {   PID_LET_USB_SPK_TEST,              H_INDEX_MODE_NO_CHG,            EV_L_INDEX_SPK_CHK_START    },  /* 017 : EV_H_INDEX_SPK_CHK_START */
    {   PID_LET_USB_SPK_TEST,              H_INDEX_MODE_NO_CHG,            EV_L_INDEX_SPK_CHK_END      },  /* 018 : EV_H_INDEX_SPK_CHK_END */
    {   PID_LET_USB_SPK_TEST,              H_INDEX_MODE_WAIT,              EV_L_INDEX_USB_TEST_END     },  /* 019 : EV_H_INDEX_USB_TEST_END */
    {   PID_LET_USB_SPK_TEST,              H_INDEX_MODE_NO_CHG,            EV_L_INDEX_SPK_BIBIRI_START },  /* 020 : EV_H_INDEX_SPK_BIBIRI_START */
    {   PID_LET_USB_SPK_TEST,              H_INDEX_MODE_NO_CHG,            EV_L_INDEX_SET_BIBIRI_CHECK },  /* 021 : EV_H_INDEX_SET_BIBIRI_CHECK */
    {   PID_LET_USB_SPK_TEST,              H_INDEX_MODE_NO_CHG,            EV_L_INDEX_BIBIRI_CHECK_END },  /* 022 : EV_H_INDEX_BIBIRI_CHECK_END */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 023 : EV_H_INDEX_BSM_START_TP_SET */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 024 : EV_H_INDEX_BSM_END */
    {   PID_LET_TUNER_TEST,                H_INDEX_MODE_NO_CHG,            EV_L_INDEX_TUNER_TEST_START },  /* 025 : EV_H_INDEX_TUNER_TEST_START */
    {   PID_LET_TUNER_TEST,                H_INDEX_MODE_NO_CHG,            EV_L_INDEX_SET_TUNER_TEST   },  /* 026 : EV_H_INDEX_SET_TUNER_TEST */
    {   PID_LET_TUNER_TEST,                H_INDEX_MODE_NO_CHG,            EV_L_INDEX_TUNER_TEST_END   },  /* 027 : EV_H_INDEX_TUNER_TEST_END */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 028 : EV_H_INDEX_SPK_BEEP_START */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 029 : EV_H_INDEX_SPK_BEEP_CH_UP */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 030 : EV_H_INDEX_SPK_BEEP_END */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 031 : EV_H_INDEX_MEM_CLEAR_START */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 032 : EV_H_INDEX_LET_MEM_CLEAR */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 033 : EV_H_INDEX_INITIAL_CLEAR */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 034 : EV_H_INDEX_INLINE_CLEAR */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 035 : EV_H_INDEX_MEM_CLEAR_END */
    {   PID_SPK_BUTTON_INFO,               H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 036 : EV_H_INDEX_SPK_BUTTON */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 037 : EV_H_INDEX_EER_MEMORY_CLEAR */
},
{ /* H_INDEX_MODE_SPK_BEEP */
    /** ProcessId                          NextMode                        LowIndexEventNo          **/
    {   PID_LET_AM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_AM_ANT_START     },  /* 000 : EV_H_INDEX_AM_ANT_START */
    {   PID_LET_AM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_AM_ANT1          },  /* 001 : EV_H_INDEX_AM_ANT1 */
    {   PID_LET_AM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_AM_ANT2          },  /* 002 : EV_H_INDEX_AM_ANT2 */
    {   PID_LET_AM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_AM_ANT3          },  /* 003 : EV_H_INDEX_AM_ANT3 */
    {   PID_LET_AM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_AM_ANT1_END      },  /* 004 : EV_H_INDEX_AM_ANT1_END */
    {   PID_LET_AM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_AM_ANT2_END      },  /* 005 : EV_H_INDEX_AM_ANT2_END */
    {   PID_LET_AM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_AM_ANT3_END      },  /* 006 : EV_H_INDEX_AM_ANT3_END */
    {   PID_LET_AM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_AM_ANT_END       },  /* 007 : EV_H_INDEX_AM_ANT_END */
    {   PID_LET_FM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_FM_ANT_START     },  /* 008 : EV_H_INDEX_FM_ANT_START */
    {   PID_LET_FM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_FM_ANT1          },  /* 009 : EV_H_INDEX_FM_ANT1 */
    {   PID_LET_FM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_FM_ANT2          },  /* 010 : EV_H_INDEX_FM_ANT2 */
    {   PID_LET_FM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_FM_ANT3          },  /* 011 : EV_H_INDEX_FM_ANT3 */
    {   PID_LET_FM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_FM_ANT1_END      },  /* 012 : EV_H_INDEX_FM_ANT1_END */
    {   PID_LET_FM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_FM_ANT2_END      },  /* 013 : EV_H_INDEX_FM_ANT2_END */
    {   PID_LET_FM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_FM_ANT3_END      },  /* 014 : EV_H_INDEX_FM_ANT3_END */
    {   PID_LET_FM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_FM_ANT_END       },  /* 015 : EV_H_INDEX_FM_ANT_END */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 016 : EV_H_INDEX_USB_TEST_START */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 017 : EV_H_INDEX_SPK_CHK_START */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 018 : EV_H_INDEX_SPK_CHK_END */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 019 : EV_H_INDEX_USB_TEST_END */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 020 : EV_H_INDEX_SPK_BIBIRI_START */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 021 : EV_H_INDEX_SET_BIBIRI_CHECK */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 022 : EV_H_INDEX_BIBIRI_CHECK_END */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 023 : EV_H_INDEX_BSM_START_TP_SET */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 024 : EV_H_INDEX_BSM_END */
    {   PID_LET_TUNER_TEST,                H_INDEX_MODE_NO_CHG,            EV_L_INDEX_TUNER_TEST_START },  /* 025 : EV_H_INDEX_TUNER_TEST_START */
    {   PID_LET_TUNER_TEST,                H_INDEX_MODE_NO_CHG,            EV_L_INDEX_SET_TUNER_TEST   },  /* 026 : EV_H_INDEX_SET_TUNER_TEST */
    {   PID_LET_TUNER_TEST,                H_INDEX_MODE_NO_CHG,            EV_L_INDEX_TUNER_TEST_END   },  /* 027 : EV_H_INDEX_TUNER_TEST_END */
    {   PID_LET_SPK_BEEP_TEST,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_SPK_BEEP_START   },  /* 028 : EV_H_INDEX_SPK_BEEP_START */
    {   PID_LET_SPK_BEEP_TEST,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_SPK_BEEP_CH_UP   },  /* 029 : EV_H_INDEX_SPK_BEEP_CH_UP */
    {   PID_LET_SPK_BEEP_TEST,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_SPK_BEEP_END     },  /* 030 : EV_H_INDEX_SPK_BEEP_END */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 031 : EV_H_INDEX_MEM_CLEAR_START */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 032 : EV_H_INDEX_LET_MEM_CLEAR */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 033 : EV_H_INDEX_INITIAL_CLEAR */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 034 : EV_H_INDEX_INLINE_CLEAR */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 035 : EV_H_INDEX_MEM_CLEAR_END */
    {   PID_LET_SPK_BEEP_TEST,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_SPK_BEEP_BUTTON  },  /* 036 : EV_H_INDEX_SPK_BUTTON */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 037 : EV_H_INDEX_EER_MEMORY_CLEAR */
},
{ /* H_INDEX_MODE_BSM */
    /** ProcessId                          NextMode                        LowIndexEventNo          **/
    {   PID_LET_AM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_AM_ANT_START     },  /* 000 : EV_H_INDEX_AM_ANT_START */
    {   PID_LET_AM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_AM_ANT1          },  /* 001 : EV_H_INDEX_AM_ANT1 */
    {   PID_LET_AM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_AM_ANT2          },  /* 002 : EV_H_INDEX_AM_ANT2 */
    {   PID_LET_AM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_AM_ANT3          },  /* 003 : EV_H_INDEX_AM_ANT3 */
    {   PID_LET_AM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_AM_ANT1_END      },  /* 004 : EV_H_INDEX_AM_ANT1_END */
    {   PID_LET_AM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_AM_ANT2_END      },  /* 005 : EV_H_INDEX_AM_ANT2_END */
    {   PID_LET_AM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_AM_ANT3_END      },  /* 006 : EV_H_INDEX_AM_ANT3_END */
    {   PID_LET_AM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_AM_ANT_END       },  /* 007 : EV_H_INDEX_AM_ANT_END */
    {   PID_LET_FM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_FM_ANT_START     },  /* 008 : EV_H_INDEX_FM_ANT_START */
    {   PID_LET_FM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_FM_ANT1          },  /* 009 : EV_H_INDEX_FM_ANT1 */
    {   PID_LET_FM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_FM_ANT2          },  /* 010 : EV_H_INDEX_FM_ANT2 */
    {   PID_LET_FM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_FM_ANT3          },  /* 011 : EV_H_INDEX_FM_ANT3 */
    {   PID_LET_FM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_FM_ANT1_END      },  /* 012 : EV_H_INDEX_FM_ANT1_END */
    {   PID_LET_FM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_FM_ANT2_END      },  /* 013 : EV_H_INDEX_FM_ANT2_END */
    {   PID_LET_FM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_FM_ANT3_END      },  /* 014 : EV_H_INDEX_FM_ANT3_END */
    {   PID_LET_FM_TEST,                   H_INDEX_MODE_NO_CHG,            EV_L_INDEX_FM_ANT_END       },  /* 015 : EV_H_INDEX_FM_ANT_END */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 016 : EV_H_INDEX_USB_TEST_START */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 017 : EV_H_INDEX_SPK_CHK_START */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 018 : EV_H_INDEX_SPK_CHK_END */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 019 : EV_H_INDEX_USB_TEST_END */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 020 : EV_H_INDEX_SPK_BIBIRI_START */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 021 : EV_H_INDEX_SET_BIBIRI_CHECK */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 022 : EV_H_INDEX_BIBIRI_CHECK_END */
    {   PID_LET_BSM_TEST,                  H_INDEX_MODE_NO_CHG,            EV_L_INDEX_BSM_START_TP_SET },  /* 023 : EV_H_INDEX_BSM_START_TP_SET */
    {   PID_LET_BSM_TEST,                  H_INDEX_MODE_WAIT,              EV_L_INDEX_BSM_END          },  /* 024 : EV_H_INDEX_BSM_END */
    {   PID_LET_TUNER_TEST,                H_INDEX_MODE_NO_CHG,            EV_L_INDEX_TUNER_TEST_START },  /* 025 : EV_H_INDEX_TUNER_TEST_START */
    {   PID_LET_TUNER_TEST,                H_INDEX_MODE_NO_CHG,            EV_L_INDEX_SET_TUNER_TEST   },  /* 026 : EV_H_INDEX_SET_TUNER_TEST */
    {   PID_LET_TUNER_TEST,                H_INDEX_MODE_NO_CHG,            EV_L_INDEX_TUNER_TEST_END   },  /* 027 : EV_H_INDEX_TUNER_TEST_END */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 028 : EV_H_INDEX_SPK_BEEP_START */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 029 : EV_H_INDEX_SPK_BEEP_CH_UP */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 030 : EV_H_INDEX_SPK_BEEP_END */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 031 : EV_H_INDEX_MEM_CLEAR_START */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 032 : EV_H_INDEX_LET_MEM_CLEAR */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 033 : EV_H_INDEX_INITIAL_CLEAR */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 034 : EV_H_INDEX_INLINE_CLEAR */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 035 : EV_H_INDEX_MEM_CLEAR_END */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 036 : EV_H_INDEX_SPK_BUTTON */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 037 : EV_H_INDEX_EER_MEMORY_CLEAR */
},
{ /* H_INDEX_MODE_MEM_CLEAR */
    /** ProcessId                          NextMode                        LowIndexEventNo          **/
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 000 : EV_H_INDEX_AM_ANT_START */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 001 : EV_H_INDEX_AM_ANT1 */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 002 : EV_H_INDEX_AM_ANT2 */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 003 : EV_H_INDEX_AM_ANT3 */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 004 : EV_H_INDEX_AM_ANT1_END */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 005 : EV_H_INDEX_AM_ANT2_END */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 006 : EV_H_INDEX_AM_ANT3_END */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 007 : EV_H_INDEX_AM_ANT_END */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 008 : EV_H_INDEX_FM_ANT_START */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 009 : EV_H_INDEX_FM_ANT1 */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 010 : EV_H_INDEX_FM_ANT2 */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 011 : EV_H_INDEX_FM_ANT3 */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 012 : EV_H_INDEX_FM_ANT1_END */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 013 : EV_H_INDEX_FM_ANT2_END */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 014 : EV_H_INDEX_FM_ANT3_END */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 015 : EV_H_INDEX_FM_ANT_END */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 016 : EV_H_INDEX_USB_TEST_START */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 017 : EV_H_INDEX_SPK_CHK_START */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 018 : EV_H_INDEX_SPK_CHK_END */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 019 : EV_H_INDEX_USB_TEST_END */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 020 : EV_H_INDEX_SPK_BIBIRI_START */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 021 : EV_H_INDEX_SET_BIBIRI_CHECK */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 022 : EV_H_INDEX_BIBIRI_CHECK_END */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 023 : EV_H_INDEX_BSM_START_TP_SET */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 024 : EV_H_INDEX_BSM_END */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 025 : EV_H_INDEX_TUNER_TEST_START */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 026 : EV_H_INDEX_SET_TUNER_TEST */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 027 : EV_H_INDEX_TUNER_TEST_END */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 028 : EV_H_INDEX_SPK_BEEP_START */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 029 : EV_H_INDEX_SPK_BEEP_CH_UP */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 030 : EV_H_INDEX_SPK_BEEP_END */
    {   PID_LET_MEM_CLEAR_TEST,            H_INDEX_MODE_NO_CHG,            EV_L_INDEX_MEM_CLEAR_START  },  /* 031 : EV_H_INDEX_MEM_CLEAR_START */
    {   PID_LET_MEM_CLEAR_TEST,            H_INDEX_MODE_NO_CHG,            EV_L_INDEX_LET_MEM_CLEAR    },  /* 032 : EV_H_INDEX_LET_MEM_CLEAR */
    {   PID_LET_MEM_CLEAR_TEST,            H_INDEX_MODE_NO_CHG,            EV_L_INDEX_INITIAL_CLEAR    },  /* 033 : EV_H_INDEX_INITIAL_CLEAR */
    {   PID_LET_MEM_CLEAR_TEST,            H_INDEX_MODE_NO_CHG,            EV_L_INDEX_INLINE_CLEAR     },  /* 034 : EV_H_INDEX_INLINE_CLEAR */
    {   PID_LET_MEM_CLEAR_TEST,            H_INDEX_MODE_WAIT,              EV_L_INDEX_MEM_CLEAR_END    },  /* 035 : EV_H_INDEX_MEM_CLEAR_END */
    {   PID_NG_NON_CONDITIONS,             H_INDEX_MODE_NO_CHG,            EV_L_INDEX_NO_CHG           },  /* 036 : EV_H_INDEX_SPK_BUTTON */
    {   PID_LET_MEM_CLEAR_TEST,            H_INDEX_MODE_NO_CHG,            EV_L_INDEX_EER_MEM_CLEAR    },  /* 037 : EV_H_INDEX_EER_MEMORY_CLEAR */
}
};

static const LowIndexMatrix_ts nstAmIndexMatrix_a[MODE_L_AM_MAX][EV_L_INDEX_AM_MAX] = {
{ /* MODE_L_AM_OFF */
    /** ProcessId                          NextMode                      **/
    {   PID_AM_START,                      MODE_L_AM_START                 },  /* 000 : EV_L_INDEX_AM_ANT_START */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 001 : EV_L_INDEX_AM_ANT1 */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 002 : EV_L_INDEX_AM_ANT2 */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 003 : EV_L_INDEX_AM_ANT3 */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 004 : EV_L_INDEX_AM_ANT1_END */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 005 : EV_L_INDEX_AM_ANT2_END */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 006 : EV_L_INDEX_AM_ANT3_END */
    {   PID_OK_SEND,                       MODE_L_NO_CHG                   },  /* 007 : EV_L_INDEX_AM_ANT_END */
},
{ /* MODE_L_AM_START */
    /** ProcessId                          NextMode                      **/
    {   PID_OK_SEND,                       MODE_L_NO_CHG                   },  /* 000 : EV_L_INDEX_AM_ANT_START */
    {   PID_AM_ANT,                        MODE_L_AM_ANT1_CHK              },  /* 001 : EV_L_INDEX_AM_ANT1 */
    {   PID_AM_ANT,                        MODE_L_AM_ANT2_CHK              },  /* 002 : EV_L_INDEX_AM_ANT2 */
    {   PID_AM_ANT,                        MODE_L_AM_ANT3_CHK              },  /* 003 : EV_L_INDEX_AM_ANT3 */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 004 : EV_L_INDEX_AM_ANT1_END */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 005 : EV_L_INDEX_AM_ANT2_END */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 006 : EV_L_INDEX_AM_ANT3_END */
    {   PID_AM_END,                        MODE_L_AM_OFF                   },  /* 007 : EV_L_INDEX_AM_ANT_END */
},
{ /* MODE_L_AM_ANT1_CHK */
    /** ProcessId                          NextMode                      **/
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 000 : EV_L_INDEX_AM_ANT_START */
    {   PID_OK_SEND,                       MODE_L_NO_CHG                   },  /* 001 : EV_L_INDEX_AM_ANT1 */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 002 : EV_L_INDEX_AM_ANT2 */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 003 : EV_L_INDEX_AM_ANT3 */
    {   PID_AM_ANT_END,                    MODE_L_AM_START                 },  /* 004 : EV_L_INDEX_AM_ANT1_END */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 005 : EV_L_INDEX_AM_ANT2_END */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 006 : EV_L_INDEX_AM_ANT3_END */
    {   PID_NG_BUSY,                       MODE_L_NO_CHG                   },  /* 007 : EV_L_INDEX_AM_ANT_END */
},
{ /* MODE_L_AM_ANT2_CHK */
    /** ProcessId                          NextMode                      **/
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 000 : EV_L_INDEX_AM_ANT_START */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 001 : EV_L_INDEX_AM_ANT1 */
    {   PID_OK_SEND,                       MODE_L_NO_CHG                   },  /* 002 : EV_L_INDEX_AM_ANT2 */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 003 : EV_L_INDEX_AM_ANT3 */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 004 : EV_L_INDEX_AM_ANT1_END */
    {   PID_AM_ANT_END,                    MODE_L_AM_START                 },  /* 005 : EV_L_INDEX_AM_ANT2_END */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 006 : EV_L_INDEX_AM_ANT3_END */
    {   PID_NG_BUSY,                       MODE_L_NO_CHG                   },  /* 007 : EV_L_INDEX_AM_ANT_END */
},
{ /* MODE_L_AM_ANT3_CHK */
    /** ProcessId                          NextMode                      **/
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 000 : EV_L_INDEX_AM_ANT_START */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 001 : EV_L_INDEX_AM_ANT1 */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 002 : EV_L_INDEX_AM_ANT2 */
    {   PID_OK_SEND,                       MODE_L_NO_CHG                   },  /* 003 : EV_L_INDEX_AM_ANT3 */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 004 : EV_L_INDEX_AM_ANT1_END */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 005 : EV_L_INDEX_AM_ANT2_END */
    {   PID_AM_ANT_END,                    MODE_L_AM_START                 },  /* 006 : EV_L_INDEX_AM_ANT3_END */
    {   PID_NG_BUSY,                       MODE_L_NO_CHG                   },  /* 007 : EV_L_INDEX_AM_ANT_END */
}
};

static const LowIndexMatrix_ts nstFmIndexMatrix_a[MODE_L_FM_MAX][EV_L_INDEX_FM_MAX] = {
{ /* MODE_L_FM_OFF */
    /** ProcessId                          NextMode                      **/
    {   PID_FM_START,                      MODE_L_FM_START                 },  /* 000 : EV_L_INDEX_FM_ANT_START */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 001 : EV_L_INDEX_FM_ANT1 */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 002 : EV_L_INDEX_FM_ANT2 */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 003 : EV_L_INDEX_FM_ANT3 */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 004 : EV_L_INDEX_FM_ANT1_END */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 005 : EV_L_INDEX_FM_ANT2_END */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 006 : EV_L_INDEX_FM_ANT3_END */
    {   PID_OK_SEND,                       MODE_L_NO_CHG                   },  /* 007 : EV_L_INDEX_FM_ANT_END */
},
{ /* MODE_L_FM_START */
    /** ProcessId                          NextMode                      **/
    {   PID_OK_SEND,                       MODE_L_NO_CHG                   },  /* 000 : EV_L_INDEX_FM_ANT_START */
    {   PID_FM_ANT,                        MODE_L_FM_ANT1_CHK              },  /* 001 : EV_L_INDEX_FM_ANT1 */
    {   PID_FM_ANT,                        MODE_L_FM_ANT2_CHK              },  /* 002 : EV_L_INDEX_FM_ANT2 */
    {   PID_FM_ANT,                        MODE_L_FM_ANT3_CHK              },  /* 003 : EV_L_INDEX_FM_ANT3 */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 004 : EV_L_INDEX_FM_ANT1_END */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 005 : EV_L_INDEX_FM_ANT2_END */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 006 : EV_L_INDEX_FM_ANT3_END */
    {   PID_FM_END,                        MODE_L_FM_OFF                   },  /* 007 : EV_L_INDEX_FM_ANT_END */
},
{ /* MODE_L_FM_ANT1_CHK */
    /** ProcessId                          NextMode                      **/
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 000 : EV_L_INDEX_FM_ANT_START */
    {   PID_OK_SEND,                       MODE_L_NO_CHG                   },  /* 001 : EV_L_INDEX_FM_ANT1 */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 002 : EV_L_INDEX_FM_ANT2 */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 003 : EV_L_INDEX_FM_ANT3 */
    {   PID_FM_ANT_END,                    MODE_L_FM_START                 },  /* 004 : EV_L_INDEX_FM_ANT1_END */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 005 : EV_L_INDEX_FM_ANT2_END */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 006 : EV_L_INDEX_FM_ANT3_END */
    {   PID_NG_BUSY,                       MODE_L_NO_CHG                   },  /* 007 : EV_L_INDEX_FM_ANT_END */
},
{ /* MODE_L_FM_ANT2_CHK */
    /** ProcessId                          NextMode                      **/
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 000 : EV_L_INDEX_FM_ANT_START */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 001 : EV_L_INDEX_FM_ANT1 */
    {   PID_OK_SEND,                       MODE_L_NO_CHG                   },  /* 002 : EV_L_INDEX_FM_ANT2 */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 003 : EV_L_INDEX_FM_ANT3 */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 004 : EV_L_INDEX_FM_ANT1_END */
    {   PID_FM_ANT_END,                    MODE_L_FM_START                 },  /* 005 : EV_L_INDEX_FM_ANT2_END */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 006 : EV_L_INDEX_FM_ANT3_END */
    {   PID_NG_BUSY,                       MODE_L_NO_CHG                   },  /* 007 : EV_L_INDEX_FM_ANT_END */
},
{ /* MODE_L_FM_ANT3_CHK */
    /** ProcessId                          NextMode                      **/
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 000 : EV_L_INDEX_FM_ANT_START */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 001 : EV_L_INDEX_FM_ANT1 */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 002 : EV_L_INDEX_FM_ANT2 */
    {   PID_OK_SEND,                       MODE_L_NO_CHG                   },  /* 003 : EV_L_INDEX_FM_ANT3 */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 004 : EV_L_INDEX_FM_ANT1_END */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 005 : EV_L_INDEX_FM_ANT2_END */
    {   PID_FM_ANT_END,                    MODE_L_FM_START                 },  /* 006 : EV_L_INDEX_FM_ANT3_END */
    {   PID_NG_BUSY,                       MODE_L_NO_CHG                   },  /* 007 : EV_L_INDEX_FM_ANT_END */
}
};

static const LowIndexMatrix_ts nstUsbSpkIndexMatrix_a[MODE_L_USB_MAX][EV_L_INDEX_USB_MAX] = {
{ /* MODE_L_USB_TEST_OFF */
    /** ProcessId                          NextMode                      **/
    {   PID_USB_TEST_START,                MODE_L_USB_TEST_START           },  /* 000 : EV_L_INDEX_USB_TEST_START */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 001 : EV_L_INDEX_SPK_CHK_START */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 002 : EV_L_INDEX_SPK_CHK_END */
    {   PID_OK_SEND,                       MODE_L_NO_CHG                   },  /* 003 : EV_L_INDEX_USB_TEST_END */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 004 : EV_L_INDEX_SPK_BIBIRI_START */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 005 : EV_L_INDEX_SET_BIBIRI_CHECK */
    {   PID_OK_SEND,                       MODE_L_NO_CHG                   },  /* 006 : EV_L_INDEX_BIBIRI_CHECK_END */
},
{ /* MODE_L_USB_TEST_START */
    /** ProcessId                          NextMode                      **/
    {   PID_OK_SEND,                       MODE_L_NO_CHG                   },  /* 000 : EV_L_INDEX_USB_TEST_START */
    {   PID_USB_SPK_CHK_START,             MODE_L_SPK_CHK_START            },  /* 001 : EV_L_INDEX_SPK_CHK_START */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 002 : EV_L_INDEX_SPK_CHK_END */
    {   PID_USB_TEST_END,                  MODE_L_USB_TEST_OFF             },  /* 003 : EV_L_INDEX_USB_TEST_END */
    {   PID_USB_BIBIRI_START,              MODE_L_SPK_BIBIRI_START         },  /* 004 : EV_L_INDEX_SPK_BIBIRI_START */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 005 : EV_L_INDEX_SET_BIBIRI_CHECK */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 006 : EV_L_INDEX_BIBIRI_CHECK_END */
},
{ /* MODE_L_SPK_CHK_START */
    /** ProcessId                          NextMode                      **/
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 000 : EV_L_INDEX_USB_TEST_START */
    {   PID_OK_SEND,                       MODE_L_NO_CHG                   },  /* 001 : EV_L_INDEX_SPK_CHK_START */
    {   PID_USB_SPK_CHK_END,               MODE_L_SPK_CHK_END              },  /* 002 : EV_L_INDEX_SPK_CHK_END */
    {   PID_USB_TEST_END,                  MODE_L_USB_TEST_OFF             },  /* 003 : EV_L_INDEX_USB_TEST_END */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 004 : EV_L_INDEX_SPK_BIBIRI_START */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 005 : EV_L_INDEX_SET_BIBIRI_CHECK */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 006 : EV_L_INDEX_BIBIRI_CHECK_END */
},
{ /* MODE_L_SPK_CHK_END */
    /** ProcessId                          NextMode                      **/
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 000 : EV_L_INDEX_USB_TEST_START */
    {   PID_USB_SPK_CHK_START,             MODE_L_SPK_CHK_START            },  /* 001 : EV_L_INDEX_SPK_CHK_START */
    {   PID_OK_SEND,                       MODE_L_NO_CHG                   },  /* 002 : EV_L_INDEX_SPK_CHK_END */
    {   PID_USB_TEST_END,                  MODE_L_USB_TEST_OFF             },  /* 003 : EV_L_INDEX_USB_TEST_END */
    {   PID_USB_BIBIRI_START,              MODE_L_SPK_BIBIRI_START         },  /* 004 : EV_L_INDEX_SPK_BIBIRI_START */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 005 : EV_L_INDEX_SET_BIBIRI_CHECK */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 006 : EV_L_INDEX_BIBIRI_CHECK_END */
},
{ /* MODE_L_SPK_BIBIRI_START */
    /** ProcessId                          NextMode                      **/
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 000 : EV_L_INDEX_USB_TEST_START */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 001 : EV_L_INDEX_SPK_CHK_START */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 002 : EV_L_INDEX_SPK_CHK_END */
    {   PID_USB_TEST_END,                  MODE_L_USB_TEST_OFF             },  /* 003 : EV_L_INDEX_USB_TEST_END */
    {   PID_OK_SEND,                       MODE_L_NO_CHG                   },  /* 004 : EV_L_INDEX_SPK_BIBIRI_START */
    {   PID_SET_BIBIRI_CHK,                MODE_L_NO_CHG                   },  /* 005 : EV_L_INDEX_SET_BIBIRI_CHECK */
    {   PID_USB_BIBIRI_END,                MODE_L_SPK_BIBIRI_END           },  /* 006 : EV_L_INDEX_BIBIRI_CHECK_END */
},
{ /* MODE_L_SPK_BIBIRI_END */
    /** ProcessId                          NextMode                      **/
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 000 : EV_L_INDEX_USB_TEST_START */
    {   PID_USB_SPK_CHK_START,             MODE_L_SPK_CHK_START            },  /* 001 : EV_L_INDEX_SPK_CHK_START */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 002 : EV_L_INDEX_SPK_CHK_END */
    {   PID_USB_TEST_END,                  MODE_L_USB_TEST_OFF             },  /* 003 : EV_L_INDEX_USB_TEST_END */
    {   PID_USB_BIBIRI_START,              MODE_L_SPK_BIBIRI_START         },  /* 004 : EV_L_INDEX_SPK_BIBIRI_START */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 005 : EV_L_INDEX_SET_BIBIRI_CHECK */
    {   PID_OK_SEND,                       MODE_L_NO_CHG                   },  /* 006 : EV_L_INDEX_BIBIRI_CHECK_END */
}
};


static const LowIndexMatrix_ts nstBsmTestIndexMatrix_a[MODE_L_BSM_MAX][EV_L_INDEX_BSM_MAX] = {
{ /* MODE_L_BSM_TEST_OFF */
    /** ProcessId                          NextMode                      **/
    {   PID_BSM_START_TP_SET,              MODE_L_BSM_TEST_START           },  /* 000 : EV_L_INDEX_BSM_START_TP_SET */
    {   PID_OK_SEND,                       MODE_L_NO_CHG                   },  /* 001 : EV_L_INDEX_BSM_END */
},
{ /* MODE_L_BSM_TEST_START */
    /** ProcessId                          NextMode                      **/
    {   PID_OK_SEND,                       MODE_L_NO_CHG                   },  /* 000 : EV_L_INDEX_BSM_START_TP_SET */
    {   PID_BSM_END,                       MODE_L_BSM_TEST_OFF             },  /* 001 : EV_L_INDEX_BSM_END */
}
};

static const LowIndexMatrix_ts nstTunerTestIndexMatrix_a[MODE_L_TUNER_MAX][EV_L_INDEX_TUNER_MAX] = {
{ /* MODE_L_TUNER_TEST_OFF */
    /** ProcessId                          NextMode                      **/
    {   PID_TUNER_START,                   MODE_L_TUNER_TEST_START         },  /* 000 : EV_L_INDEX_TUNER_TEST_START */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 001 : EV_L_INDEX_SET_TUNER_TEST */
    {   PID_OK_SEND,                       MODE_L_NO_CHG                   },  /* 002 : EV_L_INDEX_TUNER_TEST_END */
},
{ /* MODE_L_TUNER_TEST_START */
    /** ProcessId                          NextMode                      **/
    {   PID_OK_SEND,                       MODE_L_NO_CHG                   },  /* 000 : EV_L_INDEX_TUNER_TEST_START */
    {   PID_SET_TUNER,                     MODE_L_NO_CHG                   },  /* 001 : EV_L_INDEX_SET_TUNER_TEST */
    {   PID_TUNER_END,                     MODE_L_TUNER_TEST_OFF           },  /* 002 : EV_L_INDEX_TUNER_TEST_END */
}
};

static const LowIndexMatrix_ts nstSpkBeepIndexMatrix_a[MODE_L_SPK_BEEP_MAX][EV_L_INDEX_SPK_BEEP_MAX] = {
{ /* MODE_L_SPK_BEEP_OFF */
    /** ProcessId                          NextMode                      **/
    {   PID_SPK_BEEP_START,                MODE_L_SPK_BEEP_START           },  /* 000 : EV_L_INDEX_SPK_BEEP_START */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 001 : EV_L_INDEX_SPK_BEEP_CH_UP */
    {   PID_OK_SEND,                       MODE_L_NO_CHG                   },  /* 002 : EV_L_INDEX_SPK_BEEP_END */
    {   PID_NO_ACTION,                     MODE_L_NO_CHG                   },  /* 003 : EV_L_INDEX_SPK_BEEP_BUTTON */
},
{ /* MODE_L_SPK_BEEP_START */
    /** ProcessId                          NextMode                      **/
    {   PID_OK_SEND,                       MODE_L_NO_CHG                   },  /* 000 : EV_L_INDEX_SPK_BEEP_START */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 001 : EV_L_INDEX_SPK_BEEP_CH_UP */
    {   PID_SPK_BEEP_END,                  MODE_L_SPK_BEEP_WAIT            },  /* 002 : EV_L_INDEX_SPK_BEEP_END */
    {   PID_NO_ACTION,                     MODE_L_NO_CHG                   },  /* 003 : EV_L_INDEX_SPK_BEEP_BUTTON */
},
{ /* MODE_L_SPK_BEEP_WAIT */
    /** ProcessId                          NextMode                      **/
    {   PID_OK_SEND,                       MODE_L_SPK_BEEP_START           },  /* 000 : EV_L_INDEX_SPK_BEEP_START */
    {   PID_SPK_BEEP_CH_UP,                MODE_L_NO_CHG                   },  /* 001 : EV_L_INDEX_SPK_BEEP_CH_UP */
    {   PID_NO_ACTION,                     MODE_L_NO_CHG                   },  /* 002 : EV_L_INDEX_SPK_BEEP_END */
    {   PID_SPK_BEEP_BUTTON,               MODE_L_NO_CHG                   },  /* 003 : EV_L_INDEX_SPK_BEEP_BUTTON */
}
};

static const LowIndexMatrix_ts nstMemIndexMatrix_a[MODE_L_MEM_CLEAR_MAX][EV_L_INDEX_MEM_MAX] = {
{ /* MODE_L_MEM_CLEAR_OFF */
    /** ProcessId                          NextMode                      **/
    {   PID_MEM_CLEAR_START,               MODE_L_MEM_CLEAR_START          },  /* 000 : EV_L_INDEX_MEM_CLEAR_START */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 001 : EV_L_INDEX_LET_MEM_CLEAR */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 002 : EV_L_INDEX_INITIAL_CLEAR */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 003 : EV_L_INDEX_INLINE_CLEAR */
    {   PID_NG_NON_CONDITIONS,             MODE_L_NO_CHG                   },  /* 004 : EV_L_INDEX_EER_MEM_CLEAR */
    {   PID_OK_SEND,                       MODE_L_NO_CHG                   },  /* 005 : EV_L_INDEX_MEM_CLEAR_END */
},
{ /* MODE_L_MEM_CLEAR_START */
    /** ProcessId                          NextMode                      **/
    {   PID_OK_SEND,                       MODE_L_NO_CHG                   },  /* 000 : EV_L_INDEX_MEM_CLEAR_START */
    {   PID_LET_MEM_CLEAR,                 MODE_L_NO_CHG                   },  /* 001 : EV_L_INDEX_LET_MEM_CLEAR */
    {   PID_INITIAL_CLEAR,                 MODE_L_NO_CHG                   },  /* 002 : EV_L_INDEX_INITIAL_CLEAR */
    {   PID_INLINE_CLEAR,                  MODE_L_NO_CHG                   },  /* 003 : EV_L_INDEX_INLINE_CLEAR */
    {   PID_ERR_MEM_CLEAR,                 MODE_L_NO_CHG                   },  /* 004 : EV_L_INDEX_EER_MEM_CLEAR */
    {   PID_MEM_CLEAR_END,                 MODE_L_MEM_CLEAR_OFF            },  /* 005 : EV_L_INDEX_MEM_CLEAR_END */
}
};

static void (* const LetAppProcFunc_a[PID_PROC_ID_MAX])(void) = {
    nvdLetAppLetStart,                /* 000 : PID_LETDIAG_START */
    nvdLetAppLetEnd,                  /* 001 : PID_LETDIAG_END */
    nvdLetAppAmStart,                 /* 002 : PID_AM_START */
    nvdLetAppAmEnd,                   /* 003 : PID_AM_END */
    nvdLetAppAmAnt,                   /* 004 : PID_AM_ANT */
    nvdLetAppAmAntEnd,                /* 005 : PID_AM_ANT_END */
    nvdLetAppFmStart,                 /* 006 : PID_FM_START */
    nvdLetAppFmEnd,                   /* 007 : PID_FM_END */
    nvdLetAppFmAnt,                   /* 008 : PID_FM_ANT */
    nvdLetAppFmAntEnd,                /* 009 : PID_FM_ANT_END */
    nvdLetAppUsbTestStart,            /* 010 : PID_USB_TEST_START */
    nvdLetAppUsbTestEnd,              /* 011 : PID_USB_TEST_END */
    nvdLetAppUsbSpkStart,             /* 012 : PID_USB_SPK_CHK_START */
    nvdLetAppUsbSpkEnd,               /* 013 : PID_USB_SPK_CHK_END */
    nvdLetAppBibiriStart,             /* 014 : PID_USB_BIBIRI_START */
    nvdLetAppBibiriSet,               /* 015 : PID_SET_BIBIRI_CHK */
    nvdLetAppBibiriEnd,               /* 016 : PID_USB_BIBIRI_END */
    nvdLetAppBsmInitial,              /* 017 : PID_BSM_INITIAL */
    nvdLetAppBsmTpSetStart,           /* 018 : PID_BSM_START_TP_SET */
    nvdLetAppBsmTestEnd,              /* 019 : PID_BSM_END */
    nvdLetAppTunerStart,              /* 020 : PID_TUNER_START */
    nvdLetAppTunerSet,                /* 021 : PID_SET_TUNER */
    nvdLetAppTunerEnd,                /* 022 : PID_TUNER_END */
    nvdLetAppBeepChkStart,            /* 023 : PID_SPK_BEEP_START */
    nvdLetAppBeepChkChUp,             /* 024 : PID_SPK_BEEP_CH_UP */
    nvdLetAppBeepChkEnd,              /* 025 : PID_SPK_BEEP_END */
    nvdLetAppBeepChkButton,           /* 026 : PID_SPK_BEEP_BUTTON */
    nvdLetAppMemClearStart,           /* 027 : PID_MEM_CLEAR_START */
    nvdLetAppLetMemClear,             /* 028 : PID_LET_MEM_CLEAR */
    nvdLetAppInitialClear,            /* 029 : PID_INITIAL_CLEAR */
    nvdLetAppMemClearEnd,             /* 030 : PID_MEM_CLEAR_END */
    nvdLetAppAmAntRest,               /* 031 : PID_AM_ANT_REST */
    nvdLetAppFmAntRest,               /* 032 : PID_FM_ANT_REST */
    nvdLetAppUsbSpkRest,              /* 033 : PID_SPK_CHECK_REST */
    nvdLetAppBsmRest,                 /* 034 : PID_BSM_CHECK_REST */
    nvdLetAppBibiriRest,              /* 035 : PID_BIBIRI_CHK_REST */
    nvdLetAppVolVspRest,              /* 036 : PID_AUTO_VOL_VSP_REST */
    nvdLetAppVinCompRest,             /* 037 : PID_VIN_COMP_REST */
    nvdLetAppVinNoRest,               /* 038 : PID_AUDIO_VIN_REST */
    nvdLetAppTotalTimeRest,           /* 039 : PID_TOTAL_TIME_REST */
    nvdLetAppModeTimeRest,            /* 040 : PID_MODE_TIME_REST */
    nvdLetAppAllLetRest,              /* 041 : PID_AUDIO_DIAG_REST */
    nvdLetAppDwgRest,                 /* 042 : PID_AUDIO_DWG_REST */
    nvdLetAppDateRest,                /* 043 : PID_AUDIO_DATE_REST */
    nvdLetAppSerialRest,              /* 044 : PID_AUDIO_SERIAL_REST */
    nvdLetAppTunerTestRest,           /* 045 : PID_TUNER_TEST_REST */
    nvdLetAppSpkBeepRest,             /* 046 : PID_SPK_BEEP_REST */
    nvdLetAppInlineRest,              /* 047 : PID_INLINE_DIAG_REST */
    nvdLetAppInlineClear,             /* 048 : PID_INLINE_CLEAR */
    nvdLetAppSndVolumeSet,            /* 049 : PID_SND_VOLUME_SET */
    nvdLetAppSndBassSet,              /* 050 : PID_SND_BASS_SET */
    nvdLetAppSndTrebleSet,            /* 051 : PID_SND_TREBLE_SET */
    nvdLetAppHotErrRest,              /* 052 : PID_HOT_ERR_REST */
    nvdLetAppResOkSend,               /* 053 : PID_OK_SEND */
    nvdLetAppResNgBusy,               /* 054 : PID_NG_BUSY */
    OS_NULL,                          /* 055 : PID_NO_ACTION */
    nvdLetAppNonCondition,            /* 056 : PID_NG_NON_CONDITIONS */
    nvdLetAppSpkButtonInfo,           /* 057 : PID_SPK_BUTTON_INFO */
    nvdLetAppERRMemClear,             /* 058 : PID_ERR_MEM_CLEAR */

    /* LET Response PID */
    nvdLetAppLetStartRes,             /* 000 : PID_RES_AUDIO_DIAG_START */
    nvdLetAppLetEndRes,               /* 001 : PID_RES_AUDIO_DIAG_END */
    nvdLetAppAmStartRes,              /* 002 : PID_RES_AM_ANT_START */
    nvdLetAppAmAnt1Res,               /* 003 : PID_RES_AM_ANT1 */
    nvdLetAppAmAnt2Res,               /* 004 : PID_RES_AM_ANT2 */
    nvdLetAppAmAnt3Res,               /* 005 : PID_RES_AM_ANT3 */
    nvdLetAppAmAnt1EndRes,            /* 006 : PID_RES_AM_ANT1_END */
    nvdLetAppAmAnt2EndRes,            /* 007 : PID_RES_AM_ANT2_END */
    nvdLetAppAmAnt3EndRes,            /* 008 : PID_RES_AM_ANT3_END */
    nvdLetAppAmAntEndRes,             /* 009 : PID_RES_AM_ANT_END */
    nvdLetAppFmStartRes,              /* 010 : PID_RES_FM_ANT_START */
    nvdLetAppFmAnt1Res,               /* 011 : PID_RES_FM_ANT1 */
    nvdLetAppFmAnt2Res,               /* 012 : PID_RES_FM_ANT2 */
    nvdLetAppFmAnt3Res,               /* 013 : PID_RES_FM_ANT3 */
    nvdLetAppFmAnt1EndRes,            /* 014 : PID_RES_FM_ANT1_END */
    nvdLetAppFmAnt2EndRes,            /* 015 : PID_RES_FM_ANT2_END */
    nvdLetAppFmAnt3EndRes,            /* 016 : PID_RES_FM_ANT3_END */
    nvdLetAppFmAntEndRes,             /* 017 : PID_RES_FM_ANT_END */
    nvdLetAppUsbTestStartRes,         /* 018 : PID_RES_USB_TEST_START */
    nvdLetAppUsbSpkStartRes,          /* 019 : PID_RES_SPK_CHK_START */
    nvdLetAppUsbSpkEndRes,            /* 020 : PID_RES_SPK_CHK_END */
    nvdLetAppUsbTestEndRes,           /* 021 : PID_RES_USB_TEST_END */
    nvdLetAppBibiriStartRes,          /* 022 : PID_RES_SPK_BIBIRI_START */
    nvdLetAppBibiriSetRes,            /* 023 : PID_RES_SET_BIBIRI_CHECK */
    nvdLetAppBibiriEndRes,            /* 024 : PID_RES_BIBIRI_CHECK_END */
    nvdLetAppBsmInitialRes,           /* 025 : PID_RES_BSM_INITIAL */
    nvdLetAppBsmTpSetStartRes,        /* 026 : PID_RES_BSM_START_TP_SET */
    nvdLetAppBsmTestEndRes,           /* 027 : PID_RES_BSM_END */
    nvdLetAppBsmRestRes,              /* 028 : PID_RES_BSM_REST */
    nvdLetAppTunerStartRes,           /* 029 : PID_RES_TUNER_TEST_START */
    nvdLetAppTunerSetRes,             /* 030 : PID_RES_SET_TUNER_TEST */
    nvdLetAppTunerEndRes,             /* 031 : PID_RES_TUNER_TEST_END */
    nvdLetAppBeepChkStartRes,         /* 032 : PID_RES_SPK_BEEP_START */
    nvdLetAppBeepChkChUpRes,          /* 033 : PID_RES_SPK_BEEP_CH_UP */
    nvdLetAppBeepChkEndRes,           /* 034 : PID_RES_SPK_BEEP_END */
    nvdLetAppBeepDispRes,             /* 035 : PID_RES_SPK_DISP */
    nvdLetAppMemClearStartRes,        /* 036 : PID_RES_MEMORY_CLEAR_START */
    nvdLetAppLetMemClearRes,          /* 037 : PID_RES_LET_MEMORY_CLEAR */
    nvdLetAppInitialClearRes,         /* 038 : PID_RES_INITIAL_CLEAR */
    nvdLetAppInlineClearRes,          /* 039 : PID_RES_INLINE_CLEAR */
    nvdLetAppMemClearEndRes,          /* 040 : PID_RES_MEMORY_CLEAR_END */
    nvdLetAppVolumeSetRes,            /* 041 : PID_RES_VOLUME_SET */
    nvdLetAppBassSetRes,              /* 042 : PID_RES_BASS_SET */
    nvdLetAppTrebleSetRes,            /* 043 : PID_RES_TREBLE_SET */
    nvdLetAppBusyRes,                  /* 044 : PID_RES_BUSY */
    nvdLetAppERRMemClearRes          /* 045 : PID_RES_EER_MEMORY_CLEAR */
};

/********************************************************************************************/
/*                           Static Variable Definition Section                             */
/********************************************************************************************/

/* not used. */

/********************************************************************************************/
/*                           Global variable definition section                             */
/********************************************************************************************/
LowIndexMatrix_ts* wstLetAmIndex_p;
LowIndexMatrix_ts* wstLetFmIndex_p;
LowIndexMatrix_ts* wstLetTunerIndex_p;
LowIndexMatrix_ts* wstLetUsbIndex_p;
LowIndexMatrix_ts* wstLetBeepIndex_p;
LowIndexMatrix_ts* wstLetMemIndex_p;
LowIndexMatrix_ts* wstLetBsmIndex_p;

Type_uByte wubLetAppComModeSts;
Type_uByte wubLetAppHighIndexMode;
Type_uByte wubLetAppAmIndexMode;
Type_uByte wubLetAppFmIndexMode;
Type_uByte wubLetAppTunerIndexMode;
Type_uByte wubLetAppUsbIndexMode;
Type_uByte wubLetAppBeepIndexMode;
Type_uByte wubLetAppMemIndexMode;
Type_uByte wubLetAppBsmIndexMode;

/********************************************************************************************/
/*                           Function Definition Section                                    */
/********************************************************************************************/

#if defined ( STACK_CHECK )
/********************************************************************************************/
/**	\function		nvdLetProcessForStackCheck
 *	\date			2016/09/28
 *	\author			
 *	\description	
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		aubEventCode
 					auwLastTimeOut
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			aswTimeOut - Next Stage Wait Time or Guard Time
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
static void nvdLetProcessForStackCheck(void)
{
	nvdLetAppLetStart();			
	nvdLetAppLetEnd();			
	nvdLetAppAmStart();			
	nvdLetAppAmEnd();                  
	nvdLetAppAmAnt();                  
	nvdLetAppAmAntEnd();             
	nvdLetAppFmStart();                 
	nvdLetAppFmEnd();                  
	nvdLetAppFmAnt();                   
	nvdLetAppFmAntEnd();              
	nvdLetAppUsbTestStart();          
	nvdLetAppUsbTestEnd();           
	nvdLetAppUsbSpkStart();          
	nvdLetAppUsbSpkEnd();            
	nvdLetAppBibiriStart();             
	nvdLetAppBibiriSet();               
	nvdLetAppBibiriEnd();               
	nvdLetAppBsmInitial();              
	nvdLetAppBsmTpSetStart();      
	nvdLetAppBsmTestEnd();          
	nvdLetAppTunerStart();             
	nvdLetAppTunerSet();               
	nvdLetAppTunerEnd();              
	nvdLetAppBeepChkStart();        
	nvdLetAppBeepChkChUp();       
	nvdLetAppBeepChkEnd();          
	nvdLetAppBeepChkButton();      
	nvdLetAppMemClearStart();      
	nvdLetAppLetMemClear();         
	nvdLetAppInitialClear();            
	nvdLetAppMemClearEnd();        
	nvdLetAppAmAntRest();            
	nvdLetAppFmAntRest();             
	nvdLetAppUsbSpkRest();           
	nvdLetAppBsmRest();               
	nvdLetAppBibiriRest();              
	nvdLetAppVolVspRest();            
	nvdLetAppVinCompRest();         
	nvdLetAppVinNoRest();             
	nvdLetAppTotalTimeRest();        
	nvdLetAppModeTimeRest();       
	nvdLetAppAllLetRest();              
	nvdLetAppDwgRest();               
	nvdLetAppDateRest();               
	nvdLetAppSerialRest();             
	nvdLetAppTunerTestRest();       
	nvdLetAppSpkBeepRest();         
	nvdLetAppInlineRest();              
	nvdLetAppInlineClear();            
	nvdLetAppSndVolumeSet();       
	nvdLetAppSndBassSet();           
	nvdLetAppSndTrebleSet();         
	nvdLetAppHotErrRest();             
	nvdLetAppResOkSend();           
	nvdLetAppResNgBusy();            
	nvdLetAppNonCondition();         
	nvdLetAppSpkButtonInfo();       
	nvdLetAppERRMemClear();
	nvdLetAppLetStartRes();           
	nvdLetAppLetEndRes();             
	nvdLetAppAmStartRes();           
	nvdLetAppAmAnt1Res();           
	nvdLetAppAmAnt2Res();           
	nvdLetAppAmAnt3Res();           
	nvdLetAppAmAnt1EndRes();      
	nvdLetAppAmAnt2EndRes();      
	nvdLetAppAmAnt3EndRes();      
	nvdLetAppAmAntEndRes();        
	nvdLetAppFmStartRes();           
	nvdLetAppFmAnt1Res();            
	nvdLetAppFmAnt2Res();            
	nvdLetAppFmAnt3Res();            
	nvdLetAppFmAnt1EndRes();       
	nvdLetAppFmAnt2EndRes();       
	nvdLetAppFmAnt3EndRes();       
	nvdLetAppFmAntEndRes();        
	nvdLetAppUsbTestStartRes();    
	nvdLetAppUsbSpkStartRes();     
	nvdLetAppUsbSpkEndRes();       
	nvdLetAppUsbTestEndRes();      
	nvdLetAppBibiriStartRes();		
	nvdLetAppBibiriSetRes();		
	nvdLetAppBibiriEndRes();		
	nvdLetAppBsmInitialRes();		
	nvdLetAppBsmTpSetStartRes(); 
	nvdLetAppBsmTestEndRes();     
	nvdLetAppBsmRestRes();          
	nvdLetAppTunerStartRes();       
	nvdLetAppTunerSetRes();          
	nvdLetAppTunerEndRes();         
	nvdLetAppBeepChkStartRes();   
	nvdLetAppBeepChkChUpRes();  
	nvdLetAppBeepChkEndRes();    
	nvdLetAppBeepDispRes();         
	nvdLetAppMemClearStartRes(); 
	nvdLetAppLetMemClearRes();    
	nvdLetAppInitialClearRes();       
	nvdLetAppInlineClearRes();       
	nvdLetAppMemClearEndRes();   
	nvdLetAppVolumeSetRes();       
	nvdLetAppBassSetRes();           
	nvdLetAppTrebleSetRes();         
	nvdLetAppBusyRes();
	nvdLetAppERRMemClearRes();
}
#endif
/*********************************************************************************************
*  function                 : wubLetApp_SendMail
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
void wubLetApp_SendMail(Type_uByte aubsrc,Type_uHWord auhEventCode, void* avdBuffer_p, Type_uWord auwSize)
{
	Type_sByte asbResult;
	
	asbResult = RES_NG;

	asbResult = wsbRPCSendMsg(aubsrc,ARPC_ID_LET,auhEventCode, avdBuffer_p, auwSize);
	if(asbResult != RES_OK){
		LETAPP("wubLetApp_SendMail NG");
	}
}
/*********************************************************************************************
*  function                 : wvdLetAppControl
*  date                     : 2016.05.01
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
void wvdLetAppControl(void)
{
    LetModeMatrix_ts* astLetModeIndex_p;

    astLetModeIndex_p = OS_NULL;
	
    if (wstLetAppStdMail_p->vdQueueBuffer_p != OS_NULL){

        if (wstLetAppStdMail_p->uhEventCode < LETAPP_EV_MAX){ /* Let Request Event Check */
			
            nvdLetAppCmdSave((Type_uByte *)(wstLetAppStdMail_p->vdQueueBuffer_p), (Type_uByte)(wstLetAppStdMail_p->uwQueueSize));
            /* Debug Message */

            if (wubLetAppComModeSts < LET_MODE_MAX){ /* Let Mode Status Translate Table Check */

                astLetModeIndex_p = (LetModeMatrix_ts*)(&(nstLetStateCtrlMatrix_a[wubLetAppComModeSts][(wstLetAppStdMail_p->uhEventCode)]));

                if (PID_HIGH_INDEX == astLetModeIndex_p->ubProcessId){ /* Let HighIndex Translate Table Check */

                    nvdLetAppHighProcContrl(astLetModeIndex_p->ubHighIndexEvent); /* HighIndex Event Execute */
                }
                else {
					
                    nvdLetAppProcContrl(astLetModeIndex_p->ubProcessId); /* Process Execute */
                }

                /* Communication State Update */
                if (astLetModeIndex_p->ubNextMode < LET_MODE_MAX){
					
                    wubLetAppComModeSts = astLetModeIndex_p->ubNextMode;
                }
                else {
                    /* No Action */
                }
            }
            else {
				
                wubLetAppComModeSts = LET_MODE_OFFED;
            }
        }
        else if (wstLetAppStdMail_p->uhEventCode < LETAPP_RES_MAX){ /* Let Response Event Check */
			
            astLetModeIndex_p = (LetModeMatrix_ts*)(&nstLetStateCtrlMatrix_a[wubLetAppComModeSts][(wstLetAppStdMail_p->uhEventCode)]);

            nvdLetAppProcContrl(astLetModeIndex_p->ubProcessId);

            /* Communication State Update */
            if (astLetModeIndex_p->ubNextMode < LET_MODE_MAX){
                wubLetAppComModeSts = astLetModeIndex_p->ubNextMode;
            }
            else {
                /* No Action */
            }
        }
        else {
            /* No Action */
        }
    }
    else {
        /* No Action */
    }
}
/*********************************************************************************************
*  function                 : nvdLetAppCmdSave
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
static void nvdLetAppCmdSave(Type_uByte* aubDataBuff_p, Type_uByte aubSize)
{	
    if (aubSize >= sizeof(wubLetAppCmdData)){
        wubLetAppCmdSize = sizeof(wubLetAppCmdData);
        wvdGen_MemCpy(&wubLetAppCmdData[LETAPP_NUM_0], aubDataBuff_p, sizeof(wubLetAppCmdData));
    }
    else {
        wubLetAppCmdSize = aubSize;
        wvdGen_MemCpy(&wubLetAppCmdData[LETAPP_NUM_0], aubDataBuff_p, aubSize);
    }
}
/*********************************************************************************************
*  function                 : nvdLetAppHighProcContrl
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
static void nvdLetAppHighProcContrl(Type_uByte aubHighEvent)
{
    HighIndexMatrix_ts* astHighIndex_p;

    astHighIndex_p = OS_NULL;

    if ((aubHighEvent < EV_H_INDEX_MAX) && (wubLetAppHighIndexMode < H_INDEX_MODE_MAX)){ /* HighIndex Event Check */
        /* Debug Message */

        astHighIndex_p = (HighIndexMatrix_ts*)(&(nstHighIndexMatrix_a[wubLetAppHighIndexMode][aubHighEvent]));
        if ((astHighIndex_p->ubNextHIndexMode) < H_INDEX_MODE_MAX){
            wubLetAppHighIndexMode = astHighIndex_p->ubNextHIndexMode;
        }
        else {
            /* No Action */
        }

        if ((astHighIndex_p->ubProcessId > PID_LET_INVALID) && (astHighIndex_p->ubProcessId < PID_LET_MAX)){
            nvdLetAppLowProcContrl(astHighIndex_p->ubProcessId, astHighIndex_p->ubLowIndexEvent); /* LowIndex Event Execute */
        }
        else {
            nvdLetAppProcContrl(astHighIndex_p->ubProcessId); /* Process Execute */
        }
    }
    else {
        /* No Action */
    }
}
/*********************************************************************************************
*  function                 : nvdLetAppLowProcContrl
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
static void nvdLetAppLowProcContrl(Type_uByte aubProcId, Type_uByte aubLowEvent)
{
    switch (aubProcId){
        case PID_LET_AM_TEST: /* AM Test */
            if ((wubLetAppAmIndexMode < MODE_L_AM_MAX) && (aubLowEvent < EV_L_INDEX_AM_MAX)){
                wstLetAmIndex_p = (LowIndexMatrix_ts*)(&(nstAmIndexMatrix_a[wubLetAppAmIndexMode][aubLowEvent]));
                aubProcId = wstLetAmIndex_p->ubProcessId;
            }
            else {
                /* No Action */
            }
            /* Debug Message */
            break;
        case PID_LET_FM_TEST: /* FM Test */
            if ((wubLetAppFmIndexMode < MODE_L_FM_MAX) && (aubLowEvent < EV_L_INDEX_FM_MAX)){
                wstLetFmIndex_p = (LowIndexMatrix_ts*)(&(nstFmIndexMatrix_a[wubLetAppFmIndexMode][aubLowEvent]));
                aubProcId = wstLetFmIndex_p->ubProcessId;
            }
            else {
                /* No Action */
            }
            /* Debug Message */
            break;
        case PID_LET_TUNER_TEST: /* Tuner Test */
            if ((wubLetAppTunerIndexMode < MODE_L_TUNER_MAX) && (aubLowEvent < EV_L_INDEX_TUNER_MAX)){
                wstLetTunerIndex_p = (LowIndexMatrix_ts*)(&(nstTunerTestIndexMatrix_a[wubLetAppTunerIndexMode][aubLowEvent]));
                aubProcId = wstLetTunerIndex_p->ubProcessId;
            }
            else {
                /* No Action */
            }
            /* Debug Message */
            break;
        case PID_LET_USB_SPK_TEST: /* USB Speaker Test */
            if ((wubLetAppUsbIndexMode < MODE_L_USB_MAX) && (aubLowEvent < EV_L_INDEX_USB_MAX)){
                wstLetUsbIndex_p = (LowIndexMatrix_ts*)(&(nstUsbSpkIndexMatrix_a[wubLetAppUsbIndexMode][aubLowEvent]));
                aubProcId = wstLetUsbIndex_p->ubProcessId;
            }
            else {
                /* No Action */
            }
            /* Debug Message */
            break;
        case PID_LET_SPK_BEEP_TEST: /* Beep Speaker Test */
            if ((wubLetAppBeepIndexMode < MODE_L_SPK_BEEP_MAX) && (aubLowEvent < EV_L_INDEX_SPK_BEEP_MAX)){
                wstLetBeepIndex_p = (LowIndexMatrix_ts*)(&(nstSpkBeepIndexMatrix_a[wubLetAppBeepIndexMode][aubLowEvent]));
                aubProcId = wstLetBeepIndex_p->ubProcessId;
            }
            else {
                /* No Action */
            }
            /* Debug Message */
            break;
        case PID_LET_MEM_CLEAR_TEST: /* Memory Clear Test */
            if ((wubLetAppMemIndexMode < MODE_L_MEM_CLEAR_MAX) && (aubLowEvent < EV_L_INDEX_MEM_MAX)){
                wstLetMemIndex_p = (LowIndexMatrix_ts*)(&(nstMemIndexMatrix_a[wubLetAppMemIndexMode][aubLowEvent]));
                aubProcId = wstLetMemIndex_p->ubProcessId;
            }
            else {
                /* No Action */
            }
            /* Debug Message */
            break;
        case PID_LET_BSM_TEST: /* BSM Test */
            if ((wubLetAppBsmIndexMode < MODE_L_BSM_MAX) && (aubLowEvent < EV_L_INDEX_BSM_MAX)){
                wstLetBsmIndex_p = (LowIndexMatrix_ts*)(&(nstBsmTestIndexMatrix_a[wubLetAppBsmIndexMode][aubLowEvent]));
                aubProcId = wstLetBsmIndex_p->ubProcessId;
            }
            else {
                /* No Action */
            }
            /* Debug Message */
            break;
        default:
            break;
    }
    nvdLetAppProcContrl(aubProcId);
}
/*********************************************************************************************
*  function                 : nvdLetAppProcContrl
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
static void nvdLetAppProcContrl(Type_uByte aubProcId)
{
    if (aubProcId < PID_PROC_ID_MAX){
        if (LetAppProcFunc_a[aubProcId] != OS_NULL){
#ifndef STACK_CHECK
            LetAppProcFunc_a[aubProcId](); /* Process Execute */
#else
	     nvdLetProcessForStackCheck();
#endif
        }
        else {
            /* No Action */
        }
    }
    else {
        /* No Action */
    }
    /* Debug Message */
}
/*********************************************************************************************
*  function                 : nvdLetIndexModeUpdate
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
static void nvdLetIndexModeUpdate(void)
{
    switch (wstLetAppStdMail_p->uhEventCode){
        case LETAPP_RES_AM_ANT_START:
        case LETAPP_RES_AM_ANT1:
        case LETAPP_RES_AM_ANT2:
        case LETAPP_RES_AM_ANT3:
        case LETAPP_RES_AM_ANT1_END:
        case LETAPP_RES_AM_ANT2_END:
        case LETAPP_RES_AM_ANT3_END:
        case LETAPP_RES_AM_ANT_END:
            if (wstLetAmIndex_p != OS_NULL){ /* LET AM Test Mode Change */
                if (wstLetAmIndex_p->ubNextLIndexMode < MODE_L_AM_MAX){
                    wubLetAppAmIndexMode = wstLetAmIndex_p->ubNextLIndexMode;
                }
                else {
                    /* No Action */
                }
            }
            else {
                wubLetAppAmIndexMode = MODE_L_AM_OFF;
            }
            /* Debug Message */
            break;
        case LETAPP_RES_FM_ANT_START:
        case LETAPP_RES_FM_ANT1:
        case LETAPP_RES_FM_ANT2:
        case LETAPP_RES_FM_ANT3:
        case LETAPP_RES_FM_ANT1_END:
        case LETAPP_RES_FM_ANT2_END:
        case LETAPP_RES_FM_ANT3_END:
        case LETAPP_RES_FM_ANT_END:
            if (wstLetFmIndex_p != OS_NULL){ /* LET FM Test Mode Change */
                if (wstLetFmIndex_p->ubNextLIndexMode < MODE_L_FM_MAX){
                    wubLetAppFmIndexMode = wstLetFmIndex_p->ubNextLIndexMode;
                }
                else {
                    /* No Action */
                }
            }
            else {
                wubLetAppFmIndexMode = MODE_L_FM_OFF;
            }
            /* Debug Message */
            break;
        case LETAPP_RES_USB_TEST_START:
        case LETAPP_RES_SPK_CHK_START:
        case LETAPP_RES_SPK_CHK_END:
        case LETAPP_RES_USB_TEST_END:
        case LETAPP_RES_SPK_BIBIRI_START:
        case LETAPP_RES_SET_BIBIRI_CHECK:
        case LETAPP_RES_BIBIRI_CHECK_END:
            if (wstLetUsbIndex_p != OS_NULL){ /* LET USB Speaker Test Mode Change */
                if (wstLetUsbIndex_p->ubNextLIndexMode < MODE_L_USB_MAX){
                    wubLetAppUsbIndexMode = wstLetUsbIndex_p->ubNextLIndexMode;
                }
                else {
                    /* No Action */
                }
            }
            else {
                wubLetAppUsbIndexMode = MODE_L_USB_TEST_OFF;
            }
            /* Debug Message */
            break;
        case LETAPP_RES_BSM_START_TP_SET:
        case LETAPP_RES_BSM_END:
        case LETAPP_RES_BSM_REST:
            if (wstLetBsmIndex_p != OS_NULL){ /* LET BSM Test Mode Change */
                if (wstLetBsmIndex_p->ubNextLIndexMode < MODE_L_BSM_MAX){
                    wubLetAppBsmIndexMode = wstLetBsmIndex_p->ubNextLIndexMode;
                }
                else {
                    /* No Action */
                }
            }
            else {
                wubLetAppBsmIndexMode = MODE_L_BSM_TEST_OFF;
            }
            /* Debug Message */
            break;
        case LETAPP_RES_TUNER_TEST_START:
        case LETAPP_RES_SET_TUNER_TEST:
        case LETAPP_RES_TUNER_TEST_END:
            if (wstLetTunerIndex_p != OS_NULL){ /* LET Tuner Test Mode Change */
                if (wstLetTunerIndex_p->ubNextLIndexMode < MODE_L_TUNER_MAX){
                    wubLetAppTunerIndexMode = wstLetTunerIndex_p->ubNextLIndexMode;
                }
                else {
                    /* No Action */
                }
            }
            else {
                wubLetAppTunerIndexMode = MODE_L_TUNER_TEST_OFF;
            }
            /* Debug Message */
            break;
        case LETAPP_RES_SPK_BEEP_START:
        case LETAPP_RES_SPK_BEEP_CH_UP:
        case LETAPP_RES_SPK_BEEP_END:
        case LETAPP_RES_SPK_DISP:
            if (wstLetBeepIndex_p != OS_NULL){ /* LET Beep Speaker Test Mode Change */
                if (wstLetBeepIndex_p->ubNextLIndexMode < MODE_L_SPK_BEEP_MAX){
                    wubLetAppBeepIndexMode = wstLetBeepIndex_p->ubNextLIndexMode;
                }
                else {
                    /* No Action */
                }
            }
            else {
                wubLetAppBeepIndexMode = MODE_L_SPK_BEEP_OFF;
            }
            /* Debug Message */
            break;
        case LETAPP_RES_MEMORY_CLEAR_START:
        case LETAPP_RES_LET_MEMORY_CLEAR:
        case LETAPP_RES_INITIAL_CLEAR:
        case LETAPP_RES_INLINE_CLEAR:
        case LETAPP_RES_ERR_MEMORY_CLEAR:
        case LETAPP_RES_MEMORY_CLEAR_END:
            if (wstLetMemIndex_p != OS_NULL){ /* LET Memory Test Mode Change */
                if (wstLetMemIndex_p->ubNextLIndexMode < MODE_L_MEM_CLEAR_MAX){
                    wubLetAppMemIndexMode = wstLetMemIndex_p->ubNextLIndexMode;
                }
                else {
                    /* No Action */
                }
            }
            else {
                wubLetAppMemIndexMode = MODE_L_MEM_CLEAR_OFF;
            }
            /* Debug Message */
            break;
        default:
            break;
    }
}
/*********************************************************************************************
*  function                 : nvdLetAppLetStart
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
static void nvdLetAppLetStart(void)
{
    (void)wubLetAppSubA_SendMail(LET_SUBA_EV_LET_START, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
}
/*********************************************************************************************
*  function                 : nvdLetAppLetEnd
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
static void nvdLetAppLetEnd(void)
{
    (void)wubLetAppSubA_SendMail(LET_SUBA_EV_LET_END, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
}
/*********************************************************************************************
*  function                 : nvdLetAppAmStart
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
static void nvdLetAppAmStart(void)
{
    (void)wubLetAppSubB_SendMail(LET_SUBB_EV_AM_START, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
}
/*********************************************************************************************
*  function                 : nvdLetAppAmEnd
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
static void nvdLetAppAmEnd(void)
{
    (void)wubLetAppSubB_SendMail(LET_SUBB_EV_AM_END, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
}
/*********************************************************************************************
*  function                 : nvdLetAppAmAnt
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
static void nvdLetAppAmAnt(void)
{
    (void)wubLetAppSubB_SendMail(LET_SUBB_EV_AM_ANT, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
}
/*********************************************************************************************
*  function                 : nvdLetAppAmAntEnd
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
static void nvdLetAppAmAntEnd(void)
{
    (void)wubLetAppSubB_SendMail(LET_SUBB_EV_AM_ANT_END, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
}
/*********************************************************************************************
*  function                 : nvdLetAppFmStart
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
static void nvdLetAppFmStart(void)
{
    (void)wubLetAppSubB_SendMail(LET_SUBB_EV_FM_START, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
}
/*********************************************************************************************
*  function                 : nvdLetAppFmEnd
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
static void nvdLetAppFmEnd(void)
{
    (void)wubLetAppSubB_SendMail(LET_SUBB_EV_FM_END, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
}
/*********************************************************************************************
*  function                 : nvdLetAppFmAnt
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
static void nvdLetAppFmAnt(void)
{
    (void)wubLetAppSubB_SendMail(LET_SUBB_EV_FM_ANT, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
}
/*********************************************************************************************
*  function                 : nvdLetAppFmAntEnd
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
static void nvdLetAppFmAntEnd(void)
{
    (void)wubLetAppSubB_SendMail(LET_SUBB_EV_FM_ANT_END, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
}
/*********************************************************************************************
*  function                 : nvdLetAppUsbTestStart
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
static void nvdLetAppUsbTestStart(void)
{
    (void)wubLetAppSubA_SendMail(LET_SUBA_EV_USB_TEST_START, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
}
/*********************************************************************************************
*  function                 : nvdLetAppUsbTestEnd
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
static void nvdLetAppUsbTestEnd(void)
{
    (void)wubLetAppSubA_SendMail(LET_SUBA_EV_USB_TEST_END, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
}
/*********************************************************************************************
*  function                 : nvdLetAppUsbSpkStart
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
static void nvdLetAppUsbSpkStart(void)
{
    (void)wubLetAppSubA_SendMail(LET_SUBA_EV_USB_SPK_START, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
}
/*********************************************************************************************
*  function                 : nvdLetAppUsbSpkEnd
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
static void nvdLetAppUsbSpkEnd(void)
{
    (void)wubLetAppSubA_SendMail(LET_SUBA_EV_USB_SPK_END, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
}
/*********************************************************************************************
*  function                 : nvdLetAppBibiriStart
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
static void nvdLetAppBibiriStart(void)
{
    (void)wubLetAppSubA_SendMail(LET_SUBA_EV_BIBIRI_START, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
}
/*********************************************************************************************
*  function                 : nvdLetAppBibiriSet
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
static void nvdLetAppBibiriSet(void)
{
    (void)wubLetAppSubA_SendMail(LET_SUBA_EV_BIBIRI_SET, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
}
/*********************************************************************************************
*  function                 : nvdLetAppBibiriEnd
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
static void nvdLetAppBibiriEnd(void)
{
    (void)wubLetAppSubA_SendMail(LET_SUBA_EV_BIBIRI_END, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
}
/*********************************************************************************************
*  function                 : nvdLetAppBsmInitial
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
static void nvdLetAppBsmInitial(void)
{	
    (void)wubLetAppSubA_SendMail(LET_SUBA_EV_BSM_INITIAL, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
}
/*********************************************************************************************
*  function                 : nvdLetAppBsmTpSetStart
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
static void nvdLetAppBsmTpSetStart(void)
{
    (void)wubLetAppSubA_SendMail(LET_SUBA_EV_BSM_START_TP_SET, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
}
/*********************************************************************************************
*  function                 : nvdLetAppBsmTestEnd
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
static void nvdLetAppBsmTestEnd(void)
{
    (void)wubLetAppSubA_SendMail(LET_SUBA_EV_BSM_END, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
}
/*********************************************************************************************
*  function                 : nvdLetAppTunerStart
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
static void nvdLetAppTunerStart(void)
{
    (void)wubLetAppSubB_SendMail(LET_SUBB_EV_TUNER_START, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
}
/*********************************************************************************************
*  function                 : nvdLetAppTunerSet
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
static void nvdLetAppTunerSet(void)
{
    (void)wubLetAppSubB_SendMail(LET_SUBB_EV_TUNER_SET, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
}
/*********************************************************************************************
*  function                 : nvdLetAppTunerEnd
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
static void nvdLetAppTunerEnd(void)
{
    (void)wubLetAppSubB_SendMail(LET_SUBB_EV_TUNER_END, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
}
/*********************************************************************************************
*  function                 : nvdLetAppBeepChkStart
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
static void nvdLetAppBeepChkStart(void)
{
    (void)wubLetAppSubA_SendMail(LET_SUBA_EV_SPK_BEEP_START, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
}
/*********************************************************************************************
*  function                 : nvdLetAppBeepChkChUp
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
static void nvdLetAppBeepChkChUp(void)
{
    (void)wubLetAppSubA_SendMail(LET_SUBA_EV_SPK_BEEP_CH_UP, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
}
/*********************************************************************************************
*  function                 : nvdLetAppBeepChkEnd
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
static void nvdLetAppBeepChkEnd(void)
{
    (void)wubLetAppSubA_SendMail(LET_SUBA_EV_SPK_BEEP_END, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
}
/*********************************************************************************************
*  function                 : nvdLetAppBeepChkButton
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
static void nvdLetAppBeepChkButton(void)
{
    (void)wubLetAppSubA_SendMail(LET_SUBA_EV_SPK_BEEP_BUTTON, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
}
/*********************************************************************************************
*  function                 : nvdLetAppMemClearStart
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
static void nvdLetAppMemClearStart(void)
{
    (void)wubLetAppSubA_SendMail(LET_SUBA_EV_MEM_CLEAR_START, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
}
/*********************************************************************************************
*  function                 : nvdLetAppLetMemClear
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
static void nvdLetAppLetMemClear(void)
{
    (void)wubLetAppSubA_SendMail(LET_SUBA_EV_LET_MEM_CLEAR, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
}
/*********************************************************************************************
*  function                 : nvdLetAppInitialClear
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
static void nvdLetAppInitialClear(void)
{
    (void)wubLetAppSubA_SendMail(LET_SUBA_EV_MEM_INIT_CLEAR, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
}
/*********************************************************************************************
*  function                 : nvdLetAppMemClearEnd
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
static void nvdLetAppMemClearEnd(void)
{
    (void)wubLetAppSubA_SendMail(LET_SUBA_EV_MEM_CLEAR_END, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
}
/*********************************************************************************************
*  function                 : nvdLetAppAmAntRest
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
static void nvdLetAppAmAntRest(void)
{
    Type_uByte aubEepResult;

    aubEepResult = RES_NG;

    wvdGen_MemSet(&wubLetAppTempBuff[LETAPP_NUM_0], LETAPP_NUM_0, sizeof(wubLetAppTempBuff));

    /* Commond Data Get */
    wvdGen_MemCpy(&wubLetAppTempBuff[LETAPP_NUM_0], &wubLetAppCmdData[LETAPP_NUM_0], LETAPP_NUM_2);

    //LXQADD aubEepResult = wubEEP_BlockRead(LET_EEP_ADD_AM_ANT1_REST, LETAPP_NUM_15, &wubLetAppTempBuff[LETAPP_NUM_2]);

    if (RES_OK == aubEepResult){
        (void)wubKlineDrvActiveRes(wubLetAppTempBuff[0], &wubLetAppTempBuff[LETAPP_NUM_1], LETAPP_NUM_16);
    }
    else {
        (void)wubKlineDrvNegativeRes(wubLetAppTempBuff[0], KLINEDRV_RES_REJECT);
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppFmAntRest
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
static void nvdLetAppFmAntRest(void)
{
    Type_uByte aubEepResult;

    aubEepResult = RES_NG;

    wvdGen_MemSet(&wubLetAppTempBuff[LETAPP_NUM_0], LETAPP_NUM_0, sizeof(wubLetAppTempBuff));

    /* Commond Data Get */
    wvdGen_MemCpy(&wubLetAppTempBuff[LETAPP_NUM_0], &wubLetAppCmdData[LETAPP_NUM_0], LETAPP_NUM_2);

    //LXQADD aubEepResult = wubEEP_BlockRead(LET_EEP_ADD_FM_ANT1_REST, LETAPP_NUM_15, &wubLetAppTempBuff[LETAPP_NUM_2]);

    if (RES_OK == aubEepResult){
        (void)wubKlineDrvActiveRes(wubLetAppTempBuff[0], &wubLetAppTempBuff[LETAPP_NUM_1], LETAPP_NUM_16);
    }
    else {
        (void)wubKlineDrvNegativeRes(wubLetAppTempBuff[0], KLINEDRV_RES_REJECT);
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppUsbSpkRest
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
static void nvdLetAppUsbSpkRest(void)
{
    Type_uByte aubEepResult;
    Type_uByte aubEepData[LETAPP_NUM_4];

    aubEepResult = RES_NG;
    wvdGen_MemSet(&aubEepData[LETAPP_NUM_0], LETAPP_NUM_0, sizeof(aubEepData));
    wvdGen_MemSet(&wubLetAppTempBuff[LETAPP_NUM_0], '-', sizeof(wubLetAppTempBuff));

    //LXQADD aubEepResult = wubEEP_BlockRead(LET_EEP_ADD_USB_SPK_REST, LETAPP_NUM_4, &aubEepData[LETAPP_NUM_0]);

    if (RES_OK == aubEepResult){

        nvdLetAppSpkRestChk(&aubEepData[LETAPP_NUM_0], LETAPP_NUM_4); /* USB Speaker Result Check */

        wvdGen_MemCpy(&wubLetAppTempBuff[LETAPP_NUM_30], &wubLetAppTempBuff[LETAPP_NUM_0], LETAPP_NUM_28);

        /* Commond Data Get */
        wvdGen_MemCpy(&wubLetAppTempBuff[LETAPP_NUM_0], &wubLetAppCmdData[LETAPP_NUM_0], LETAPP_NUM_2);

        wvdGen_MemCpy(&wubLetAppTempBuff[LETAPP_NUM_2], &wubLetAppTempBuff[LETAPP_NUM_30], LETAPP_NUM_28);

        (void)wubKlineDrvActiveRes(wubLetAppTempBuff[LETAPP_NUM_0], &wubLetAppTempBuff[LETAPP_NUM_1], LETAPP_NUM_29);
    }
    else {
        (void)wubKlineDrvNegativeRes(wubLetAppTempBuff[LETAPP_NUM_0], KLINEDRV_RES_REJECT);
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppSpkRestChk
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
static void nvdLetAppSpkRestChk(Type_uByte* aubEepData_p, Type_uByte aubSize)
{
    Type_uByte aubTempSize;
    Type_uByte aubMaskBit;

    aubTempSize = LETAPP_NUM_0;
    aubMaskBit = LETAPP_NUM_0;
    
/*  wvdGen_MemSet(&wubLetAppTempBuff[LETAPP_NUM_0], '-', sizeof(wubLetAppTempBuff)); */

    for (aubTempSize = LETAPP_NUM_0; aubTempSize < aubSize; aubTempSize++){
        for (aubMaskBit = LETAPP_NUM_0; aubMaskBit < LETAPP_NUM_8; aubMaskBit += LETAPP_NUM_2){
            switch((aubEepData_p[aubTempSize] >> aubMaskBit) & LETAPP_MASK_03){
                case LETAPP_RES_NO_RESULT:
                    wubLetAppTempBuff[(aubTempSize * LETAPP_NUM_8) + aubMaskBit] = 'X';
                    wubLetAppTempBuff[(aubTempSize * LETAPP_NUM_8) + aubMaskBit + LETAPP_NUM_1] = 'X';
                    break;
                case LETAPP_RES_OK:
                    wubLetAppTempBuff[(aubTempSize * LETAPP_NUM_8) + aubMaskBit] = 'O';
                    wubLetAppTempBuff[(aubTempSize * LETAPP_NUM_8) + aubMaskBit + LETAPP_NUM_1] = 'K';
                    break;
                case LETAPP_RES_NG:
                    wubLetAppTempBuff[(aubTempSize * LETAPP_NUM_8) + aubMaskBit] = 'N';
                    wubLetAppTempBuff[(aubTempSize * LETAPP_NUM_8) + aubMaskBit + LETAPP_NUM_1] = 'G';
                    break;
                case LETAPP_RES_NO_CORRES:
                    wubLetAppTempBuff[(aubTempSize * LETAPP_NUM_8) + aubMaskBit] = '-';
                    wubLetAppTempBuff[(aubTempSize * LETAPP_NUM_8) + aubMaskBit + LETAPP_NUM_1] = '-';
                    break;
                default:
                    break;
            }
        }
    } 
}
/*********************************************************************************************
*  function                 : nvdLetAppBsmRest
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
static void nvdLetAppBsmRest(void)
{	
    (void)wubLetAppSubA_SendMail(LET_SUBA_EV_BSM_REST, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
}
/*********************************************************************************************
*  function                 : nvdLetAppVolVspRest
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
static void nvdLetAppVolVspRest(void)
{
    Type_uByte aubEepResult;
    
    aubEepResult = RES_NG;

    /* Commond Data Get */
    wvdGen_MemCpy(&wubLetAppTempBuff[LETAPP_NUM_0], &wubLetAppCmdData[LETAPP_NUM_0], LETAPP_NUM_2);

    //LXQADD aubEepResult = wubEEP_BlockRead(LET_EEP_ADD_VOL_VSP, LETAPP_NUM_1, &wubLetAppTempBuff[LETAPP_NUM_2]);

    if (RES_OK == aubEepResult){
        (void)wubKlineDrvActiveRes(wubLetAppTempBuff[LETAPP_NUM_0], &wubLetAppTempBuff[LETAPP_NUM_1], LETAPP_NUM_2);
    }
    else {
        (void)wubKlineDrvNegativeRes(wubLetAppTempBuff[LETAPP_NUM_0], KLINEDRV_RES_REJECT);
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppVinCompRest
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
static void nvdLetAppVinCompRest(void)
{
    /* Commond Data Get */
    wvdGen_MemCpy(&wubLetAppTempBuff[LETAPP_NUM_0], &wubLetAppCmdData[LETAPP_NUM_0], LETAPP_NUM_2);

    wubLetAppTempBuff[LETAPP_NUM_2] = '-';
    wubLetAppTempBuff[LETAPP_NUM_3] = '-';

    (void)wubKlineDrvActiveRes(wubLetAppTempBuff[LETAPP_NUM_0], &wubLetAppTempBuff[LETAPP_NUM_1], LETAPP_NUM_3);

}
/*********************************************************************************************
*  function                 : nvdLetAppVinNoRest
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
static void nvdLetAppVinNoRest(void)
{
    /* Commond Data Get */
    wvdGen_MemCpy(&wubLetAppTempBuff[LETAPP_NUM_0], &wubLetAppCmdData[LETAPP_NUM_0], LETAPP_NUM_2);

    /* Antitheft Function Not Exit */
    wubLetAppTempBuff[LETAPP_NUM_2] = '-';
    wubLetAppTempBuff[LETAPP_NUM_3] = '-';
    wubLetAppTempBuff[LETAPP_NUM_4] = '-';
    wubLetAppTempBuff[LETAPP_NUM_5] = '-';
    wubLetAppTempBuff[LETAPP_NUM_6] = '-';
    wubLetAppTempBuff[LETAPP_NUM_7] = '-';
    wubLetAppTempBuff[LETAPP_NUM_8] = '-';
    wubLetAppTempBuff[LETAPP_NUM_9] = '-';

    (void)wubKlineDrvActiveRes(wubLetAppTempBuff[LETAPP_NUM_0], &wubLetAppTempBuff[LETAPP_NUM_1], LETAPP_NUM_9);

}
/*********************************************************************************************
*  function                 : nvdLetAppTotalTimeRest
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
static void nvdLetAppTotalTimeRest(void)
{
    Type_uByte aubEepResult;

    aubEepResult = RES_NG;

    /* Commond Data Get */
    wvdGen_MemCpy(&wubLetAppTempBuff[LETAPP_NUM_0], &wubLetAppCmdData[LETAPP_NUM_0], LETAPP_NUM_2);

    wubLetAppTempBuff[LETAPP_NUM_2] = 'T';
    wubLetAppTempBuff[LETAPP_NUM_3] = 'O';
    wubLetAppTempBuff[LETAPP_NUM_4] = 'T';
    wubLetAppTempBuff[LETAPP_NUM_5] = 'A';
    wubLetAppTempBuff[LETAPP_NUM_6] = 'L';
    wubLetAppTempBuff[LETAPP_NUM_7] = ' ';

    //LXQADD aubEepResult = wubEEP_BlockRead(LET_EEP_ADD_TOTAL_TIME, LETAPP_NUM_2, &wubLetAppTempBuff[LETAPP_NUM_8]);

    if (RES_OK == aubEepResult){
        (void)wubKlineDrvActiveRes(wubLetAppTempBuff[LETAPP_NUM_0], &wubLetAppTempBuff[LETAPP_NUM_1], LETAPP_NUM_9);
    }
    else {
        (void)wubKlineDrvNegativeRes(wubLetAppTempBuff[LETAPP_NUM_0], KLINEDRV_RES_REJECT);
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppModeTimeRest
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
static void nvdLetAppModeTimeRest(void)
{
    Type_uByte aubEepResult;

    aubEepResult = RES_NG;

    /* Commond Data Get */
    wvdGen_MemCpy(&wubLetAppTempBuff[LETAPP_NUM_0], &wubLetAppCmdData[LETAPP_NUM_0], LETAPP_NUM_2);

    /* Radio Mode Time */
    wubLetAppTempBuff[LETAPP_NUM_2] = 'R';
    wubLetAppTempBuff[LETAPP_NUM_3] = 'A';
    wubLetAppTempBuff[LETAPP_NUM_4] = 'D';
    wubLetAppTempBuff[LETAPP_NUM_5] = 'I';
    wubLetAppTempBuff[LETAPP_NUM_6] = 'O';
    wubLetAppTempBuff[LETAPP_NUM_7] = ' ';

    //LXQADD aubEepResult = wubEEP_BlockRead(LET_EEP_ADD_RADIO_TIME, LETAPP_NUM_2, &wubLetAppTempBuff[LETAPP_NUM_8]);

    /* CD/DVD Mode Time */
    wvdGen_MemSet(&wubLetAppTempBuff[LETAPP_NUM_10], '-', LETAPP_NUM_6);
    wvdGen_MemSet(&wubLetAppTempBuff[LETAPP_NUM_16], LETAPP_NUM_0, LETAPP_NUM_2);

    /* SC/HDD Mode Time */
    wvdGen_MemSet(&wubLetAppTempBuff[LETAPP_NUM_18], '-', LETAPP_NUM_6);
    wvdGen_MemSet(&wubLetAppTempBuff[LETAPP_NUM_24], LETAPP_NUM_0, LETAPP_NUM_2);

    /* Other Mode Time */
    wubLetAppTempBuff[LETAPP_NUM_26] = 'O';
    wubLetAppTempBuff[LETAPP_NUM_27] = 'T';
    wubLetAppTempBuff[LETAPP_NUM_28] = 'H';
    wubLetAppTempBuff[LETAPP_NUM_29] = 'E';
    wubLetAppTempBuff[LETAPP_NUM_30] = 'R';
    wubLetAppTempBuff[LETAPP_NUM_31] = ' ';

    //LXQADD aubEepResult += wubEEP_BlockRead(LET_EEP_ADD_OTHER_TIME, LETAPP_NUM_2, &wubLetAppTempBuff[LETAPP_NUM_32]);

    if (RES_OK == aubEepResult){
        (void)wubKlineDrvActiveRes(wubLetAppTempBuff[LETAPP_NUM_0], &wubLetAppTempBuff[LETAPP_NUM_1], LETAPP_NUM_33);
    }
    else {
        (void)wubKlineDrvNegativeRes(wubLetAppTempBuff[LETAPP_NUM_0], KLINEDRV_RES_REJECT);
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppAllLetRest
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
static void nvdLetAppAllLetRest(void)
{
    nvdLetAllRestFlame1(); /* Let Audio Result Flame1 Result */
    (void)OS_SleepTask(LETAPP_NUM_1);
    nvdLetAllRestFlame2(); /* Let Audio Result Flame2 Result */
    (void)OS_SleepTask(LETAPP_NUM_1);
    nvdLetAllRestFlame3(); /* Let Audio Result Flame3 Result */
    (void)OS_SleepTask(LETAPP_NUM_1);
    nvdLetAllRestFlame4(); /* Let Audio Result Flame4 Result */
}
/*********************************************************************************************
*  function                 : nvdLetAllRestFlame1
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
static void nvdLetAllRestFlame1(void)
{
    Type_uByte aubEepResult;

    aubEepResult = RES_NG;

    wvdGen_MemSet(&wubLetAppTempBuff[LETAPP_NUM_0], LETAPP_NUM_0, sizeof(wubLetAppTempBuff));

    /* Commond Data Get */
    wvdGen_MemCpy(&wubLetAppTempBuff[LETAPP_NUM_0], &wubLetAppCmdData[LETAPP_NUM_0], LETAPP_NUM_2);

    /* Flame Data Set */
    wubLetAppTempBuff[LETAPP_NUM_2] = LETAPP_NUM_20;

    //LXQADD aubEepResult = wubEEP_BlockRead(EEP_B_PART_NUMBER, LETAPP_NUM_14, &wubLetAppTempBuff[LETAPP_NUM_3]); /* Dwg */
    //LXQADD aubEepResult += wubEEP_BlockRead(EEP_B_MAKE_DATE, LETAPP_NUM_10, &wubLetAppTempBuff[LETAPP_NUM_17]); /* Date */
    //LXQADD aubEepResult += wubEEP_BlockRead(EEP_B_SERIAL_NUMBER, LETAPP_NUM_8, &wubLetAppTempBuff[LETAPP_NUM_27]); /* Serial No*/

    wubLetAppTempBuff[LETAPP_NUM_35] = (VERSION_NO1 & LETAPP_NUM_15) + LETAPP_ASCII_NUM_0;
    wubLetAppTempBuff[LETAPP_NUM_36] = ((VERSION_NO2 >> LETAPP_NUM_4) & LETAPP_NUM_15) + LETAPP_ASCII_NUM_0;
    wubLetAppTempBuff[LETAPP_NUM_37] = (VERSION_NO2 & LETAPP_NUM_15) + LETAPP_ASCII_NUM_0;
    wubLetAppTempBuff[LETAPP_NUM_38] = (VERSION_NO3 & LETAPP_NUM_15) + LETAPP_ASCII_NUM_0;
	
    wvdGen_MemSet(&wubLetAppTempBuff[LETAPP_NUM_40], '-', LETAPP_NUM_8); /* VIN NO */

    if (RES_OK == aubEepResult){
        (void)wubKlineDrvActiveRes(wubLetAppTempBuff[LETAPP_NUM_0], &wubLetAppTempBuff[LETAPP_NUM_1], LETAPP_NUM_46);
    }
    else {
        (void)wubKlineDrvNegativeRes(wubLetAppTempBuff[LETAPP_NUM_0], KLINEDRV_RES_REJECT);
    }

}
/*********************************************************************************************
*  function                 : nvdLetAllRestFlame2
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
static void nvdLetAllRestFlame2(void)
{
    Type_uByte aubEepResult;

    aubEepResult = RES_NG;

    wvdGen_MemSet(&wubLetAppTempBuff[LETAPP_NUM_0], LETAPP_NUM_0, sizeof(wubLetAppTempBuff));

    /* Commond Data Get */
    wvdGen_MemCpy(&wubLetAppTempBuff[LETAPP_NUM_0], &wubLetAppCmdData[LETAPP_NUM_0], LETAPP_NUM_2);

    /* Flame Data Set */
    wubLetAppTempBuff[LETAPP_NUM_2] = LETAPP_NUM_36;

    //LXQADD aubEepResult = wubEEP_BlockRead(LET_EEP_ADD_AM_ANT1_REST, LETAPP_NUM_15, &wubLetAppTempBuff[LETAPP_NUM_3]); /* AM */
    //LXQADD aubEepResult += wubEEP_BlockRead(LET_EEP_ADD_FM_ANT1_REST, LETAPP_NUM_15, &wubLetAppTempBuff[LETAPP_NUM_18]); /* FM */
    wvdGen_MemSet(&wubLetAppTempBuff[LETAPP_NUM_33], '-', LETAPP_NUM_19); /* CD & TMC */

    if (RES_OK == aubEepResult){
        (void)wubKlineDrvActiveRes(wubLetAppTempBuff[LETAPP_NUM_0], &wubLetAppTempBuff[LETAPP_NUM_1], LETAPP_NUM_52);
    }
    else {
        (void)wubKlineDrvNegativeRes(wubLetAppTempBuff[LETAPP_NUM_0], KLINEDRV_RES_REJECT);
    }

}
/*********************************************************************************************
*  function                 : nvdLetAllRestFlame3
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
static void nvdLetAllRestFlame3(void)
{
    Type_uByte aubEepResult;
    Type_uByte aubBibiriResult;

    aubEepResult = RES_NG;
    aubBibiriResult = LETAPP_RES_NO_RESULT;
    wvdGen_MemSet(&wubLetAppTempBuff[LETAPP_NUM_0], LETAPP_NUM_0, sizeof(wubLetAppTempBuff));

    /* Result Get */
    //LXQADD aubEepResult = wubEEP_BlockRead(LET_EEP_ADD_USB_SPK_REST, LETAPP_NUM_4, &wubLetAppTempBuff[LETAPP_NUM_34]);
    nvdLetAppSpkRestChk(&wubLetAppTempBuff[LETAPP_NUM_34], LETAPP_NUM_4);
    wvdGen_MemCpy(&wubLetAppTempBuff[LETAPP_NUM_34], &wubLetAppTempBuff[LETAPP_NUM_0], LETAPP_NUM_28); /* usb spk temp save */
    //LXQADD aubEepResult += wubEEP_BlockRead(LET_EEP_ADD_BIBIRI_REST, LETAPP_NUM_1, &aubBibiriResult);
    nvdLetAppSpkRestChk(&aubBibiriResult, LETAPP_NUM_1);
    wvdGen_MemCpy(&wubLetAppTempBuff[LETAPP_NUM_62], &wubLetAppTempBuff[LETAPP_NUM_0], LETAPP_NUM_2); /* bibiri */

    /* Commond Data Get */
    wvdGen_MemCpy(&wubLetAppTempBuff[LETAPP_NUM_0], &wubLetAppCmdData[LETAPP_NUM_0], LETAPP_NUM_2);

    /* Flame Data Set */
    wubLetAppTempBuff[LETAPP_NUM_2] = LETAPP_NUM_52;

    /* Result Make */
    wvdGen_MemCpy(&wubLetAppTempBuff[LETAPP_NUM_3], &wubLetAppTempBuff[LETAPP_NUM_34], LETAPP_NUM_30);
    wvdGen_MemSet(&wubLetAppTempBuff[LETAPP_NUM_33], '-', LETAPP_NUM_18); /* audio input 10 byte & anc 4 byte & ac 2 byte & amp 2 byte*/

    if (RES_OK == aubEepResult){
        (void)wubKlineDrvActiveRes(wubLetAppTempBuff[LETAPP_NUM_0], &wubLetAppTempBuff[LETAPP_NUM_1], LETAPP_NUM_50);
    }
    else {
        (void)wubKlineDrvNegativeRes(wubLetAppTempBuff[LETAPP_NUM_0], KLINEDRV_RES_REJECT);
    }

}
/*********************************************************************************************
*  function                 : nvdLetAllRestFlame4
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
static void nvdLetAllRestFlame4(void)
{
    Type_uByte aubEepResult;
    Type_uByte aubVspResult;

    aubEepResult = RES_NG;
    aubVspResult = LETAPP_RES_NO_RESULT;
    wvdGen_MemSet(&wubLetAppTempBuff[LETAPP_NUM_0], LETAPP_NUM_0, sizeof(wubLetAppTempBuff));

    /* Result Get */
    //LXQADD aubEepResult = wubEEP_BlockRead(LET_EEP_ADD_VOL_VSP, LETAPP_NUM_1, &aubVspResult);

	if(aubVspResult == LETAPP_RES_VSP_OK)
	{
		aubVspResult = LETAPP_RES_OK;
	}
	else
	{
		aubVspResult = LETAPP_RES_NG;
	}

	nvdLetAppSpkRestChk(&aubVspResult, LETAPP_NUM_1);
       wvdGen_MemCpy(&wubLetAppTempBuff[LETAPP_NUM_49], &wubLetAppTempBuff[LETAPP_NUM_0], LETAPP_NUM_2); /* vsp */

    /* Commond Data Get */
    wvdGen_MemCpy(&wubLetAppTempBuff[LETAPP_NUM_0], &wubLetAppCmdData[LETAPP_NUM_0], LETAPP_NUM_2);

    /* Flame Data Set */
    wubLetAppTempBuff[LETAPP_NUM_2] = LETAPP_NUM_68;

    wvdGen_MemSet(&wubLetAppTempBuff[LETAPP_NUM_3], '-', LETAPP_NUM_46); /* GANET 42 byte & LCD 4 byte */
    wvdGen_MemSet(&wubLetAppTempBuff[LETAPP_NUM_51], '-', LETAPP_NUM_2); /* vin compare 2 byte */

    if (RES_OK == aubEepResult){
        (void)wubKlineDrvActiveRes(wubLetAppTempBuff[LETAPP_NUM_0], &wubLetAppTempBuff[LETAPP_NUM_1], LETAPP_NUM_52);
    }
    else {
        (void)wubKlineDrvNegativeRes(wubLetAppTempBuff[LETAPP_NUM_0], KLINEDRV_RES_REJECT);
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppDwgRest
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
static void nvdLetAppDwgRest(void)
{
    Type_uByte aubEepResult;

    aubEepResult = RES_NG;

    wvdGen_MemSet(&wubLetAppTempBuff[LETAPP_NUM_0], LETAPP_NUM_0, sizeof(wubLetAppTempBuff));

    /* Commond Data Get */
    wvdGen_MemCpy(&wubLetAppTempBuff[LETAPP_NUM_0], &wubLetAppCmdData[LETAPP_NUM_0], LETAPP_NUM_2);

    //LXQADD aubEepResult = wubEEP_BlockRead(EEP_B_PART_NUMBER, LETAPP_NUM_14, &wubLetAppTempBuff[LETAPP_NUM_2]);

    if (RES_OK == aubEepResult){
        (void)wubKlineDrvActiveRes(wubLetAppTempBuff[LETAPP_NUM_0], &wubLetAppTempBuff[LETAPP_NUM_1], LETAPP_NUM_15);
    }
    else {
        (void)wubKlineDrvNegativeRes(wubLetAppTempBuff[LETAPP_NUM_0], KLINEDRV_RES_REJECT);
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppDateRest
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
static void nvdLetAppDateRest(void)
{
    Type_uByte aubEepResult;

    aubEepResult = RES_NG;

    wvdGen_MemSet(&wubLetAppTempBuff[LETAPP_NUM_0], LETAPP_NUM_0, sizeof(wubLetAppTempBuff));

    /* Commond Data Get */
    wvdGen_MemCpy(&wubLetAppTempBuff[LETAPP_NUM_0], &wubLetAppCmdData[LETAPP_NUM_0], LETAPP_NUM_2);

    //LXQADD aubEepResult = wubEEP_BlockRead(EEP_B_MAKE_DATE, LETAPP_NUM_10, &wubLetAppTempBuff[LETAPP_NUM_2]);

    if (RES_OK == aubEepResult){
        (void)wubKlineDrvActiveRes(wubLetAppTempBuff[LETAPP_NUM_0], &wubLetAppTempBuff[LETAPP_NUM_1], LETAPP_NUM_11);
    }
    else {
        (void)wubKlineDrvNegativeRes(wubLetAppTempBuff[LETAPP_NUM_0], KLINEDRV_RES_REJECT);
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppSerialRest
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
static void nvdLetAppSerialRest(void)
{
    Type_uByte aubEepResult;

    aubEepResult = RES_NG;

    wvdGen_MemSet(&wubLetAppTempBuff[LETAPP_NUM_0], LETAPP_NUM_0, sizeof(wubLetAppTempBuff));

    /* Commond Data Get */
    wvdGen_MemCpy(&wubLetAppTempBuff[LETAPP_NUM_0], &wubLetAppCmdData[LETAPP_NUM_0], LETAPP_NUM_2);

    //LXQADD aubEepResult = wubEEP_BlockRead(EEP_B_SERIAL_NUMBER, LETAPP_NUM_8, &wubLetAppTempBuff[LETAPP_NUM_2]);

    wubLetAppTempBuff[LETAPP_NUM_10] = (VERSION_NO1 & LETAPP_NUM_15) + LETAPP_ASCII_NUM_0;
    wubLetAppTempBuff[LETAPP_NUM_11] = ((VERSION_NO2 >> LETAPP_NUM_4) & LETAPP_NUM_15) + LETAPP_ASCII_NUM_0;
    wubLetAppTempBuff[LETAPP_NUM_12] = (VERSION_NO2 & LETAPP_NUM_15) + LETAPP_ASCII_NUM_0;
    wubLetAppTempBuff[LETAPP_NUM_13] = (VERSION_NO3 & LETAPP_NUM_15) + LETAPP_ASCII_NUM_0;

    if (RES_OK == aubEepResult){
        (void)wubKlineDrvActiveRes(wubLetAppTempBuff[LETAPP_NUM_0], &wubLetAppTempBuff[LETAPP_NUM_1], LETAPP_NUM_13);
    }
    else {
        (void)wubKlineDrvNegativeRes(wubLetAppTempBuff[LETAPP_NUM_0], KLINEDRV_RES_REJECT);
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppTunerTestRest
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
static void nvdLetAppTunerTestRest(void)
{
    Type_uByte aubEepResult;

    aubEepResult = RES_NG;

    wvdGen_MemSet(&wubLetAppTempBuff[LETAPP_NUM_0], LETAPP_NUM_0, sizeof(wubLetAppTempBuff));

    /* Commond Data Get */
    wvdGen_MemCpy(&wubLetAppTempBuff[LETAPP_NUM_0], &wubLetAppCmdData[LETAPP_NUM_0], LETAPP_NUM_2);
    wvdGen_MemSet(&wubLetAppTempBuff[LETAPP_NUM_7], '-', LETAPP_NUM_5);

    //LXQADD aubEepResult = wubEEP_BlockRead(LET_EEP_ADD_TUNER_REST, LETAPP_NUM_5, &wubLetAppTempBuff[LETAPP_NUM_2]);

    if (RES_OK == aubEepResult){
        (void)wubKlineDrvActiveRes(wubLetAppTempBuff[LETAPP_NUM_0], &wubLetAppTempBuff[LETAPP_NUM_1], LETAPP_NUM_11);
    }
    else {
        (void)wubKlineDrvNegativeRes(wubLetAppTempBuff[LETAPP_NUM_0], KLINEDRV_RES_REJECT);
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppSpkBeepRest
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
static void nvdLetAppSpkBeepRest(void)
{
    Type_uByte aubEepResult;
    Type_uByte aubEepData[LETAPP_NUM_2];

    aubEepResult = RES_NG;
    wvdGen_MemSet(&aubEepData[LETAPP_NUM_0], LETAPP_NUM_0, sizeof(aubEepData));
    wvdGen_MemSet(&wubLetAppTempBuff[LETAPP_NUM_0], LETAPP_NUM_0, sizeof(wubLetAppTempBuff));

    //LXQADD aubEepResult = wubEEP_BlockRead(LET_EEP_ADD_BEEP_SPK_REST, LETAPP_NUM_2, &aubEepData[LETAPP_NUM_0]);

    if (RES_OK == aubEepResult){

        nvdLetAppSpkRestChk(&aubEepData[LETAPP_NUM_0], LETAPP_NUM_2); /* Beep Speaker Result Check */

        wvdGen_MemCpy(&wubLetAppTempBuff[LETAPP_NUM_30], &wubLetAppTempBuff[LETAPP_NUM_0], LETAPP_NUM_12);

        /* Commond Data Get */
        wvdGen_MemCpy(&wubLetAppTempBuff[LETAPP_NUM_0], &wubLetAppCmdData[LETAPP_NUM_0], LETAPP_NUM_2);

        wvdGen_MemCpy(&wubLetAppTempBuff[LETAPP_NUM_2], &wubLetAppTempBuff[LETAPP_NUM_30], LETAPP_NUM_12);

        (void)wubKlineDrvActiveRes(wubLetAppTempBuff[LETAPP_NUM_0], &wubLetAppTempBuff[LETAPP_NUM_1], LETAPP_NUM_13);
    }
    else {
        (void)wubKlineDrvNegativeRes(wubLetAppTempBuff[LETAPP_NUM_0], KLINEDRV_RES_REJECT);
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppInlineRest
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
static void nvdLetAppInlineRest(void)
{
    Type_uByte aubEepResult;
    Type_uByte aubInlineData[LETAPP_NUM_8];
    Type_uByte aubInlineResData[LETAPP_NUM_64];

    aubEepResult = RES_NG;
    wvdGen_MemSet(&aubInlineData[LETAPP_NUM_0], LETAPP_NUM_0, sizeof(aubInlineData));
    wvdGen_MemSet(&aubInlineResData[LETAPP_NUM_0], LETAPP_NUM_0, sizeof(aubInlineResData));
    wvdGen_MemSet(&wubLetAppTempBuff[LETAPP_NUM_0], LETAPP_NUM_0, sizeof(wubLetAppTempBuff));

    /* Commond Data Get */
    wvdGen_MemCpy(&aubInlineResData[LETAPP_NUM_0], &wubLetAppCmdData[LETAPP_NUM_0], LETAPP_NUM_2);

    //LXQADD aubEepResult = wubEEP_BlockRead(LET_EEP_ADD_INLINE_REST, LETAPP_NUM_7, &aubInlineData[LETAPP_NUM_0]);

    if (RES_OK == aubEepResult){
        nvdLetInlineDiagRes(&aubInlineData[LETAPP_NUM_0]);
        wvdGen_MemCpy(&aubInlineResData[LETAPP_NUM_2], &wubLetAppTempBuff[LETAPP_NUM_0], LETAPP_NUM_61);
        (void)wubKlineDrvActiveRes(aubInlineResData[LETAPP_NUM_0], &aubInlineResData[LETAPP_NUM_1], LETAPP_NUM_62);
    }
    else {
        (void)wubKlineDrvNegativeRes(aubInlineResData[LETAPP_NUM_0], KLINEDRV_RES_REJECT);
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppInlineRest
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
static void nvdLetInlineDiagRes(const Type_uByte * aubEepData_p)
{
    Type_uByte aubTempData;

    aubTempData = LETAPP_NUM_0;

    for (aubTempData = LETAPP_INLINE_SYNTHESISE; aubTempData < LETAPP_INLINE_MAX; aubTempData++){

	  wubLetAppTempBuff[aubTempData] = (Type_uByte)LETAPP_INLINE_RES_NG;
	  }
	  wubLetAppTempBuff[LETAPP_INLINE_SYNTHESISE		] = (((Type_uByte)(~aubEepData_p[LETAPP_NUM_0])) >> LETAPP_NUM_0) & LETAPP_SET_BIT0;
	  wubLetAppTempBuff[LETAPP_INLINE_B_CAN			] = (((Type_uByte)(~aubEepData_p[LETAPP_NUM_0])) >> LETAPP_NUM_1) & LETAPP_SET_BIT0;
	  wubLetAppTempBuff[LETAPP_INLINE_REAR_CAMERA	] = (((Type_uByte)(~aubEepData_p[LETAPP_NUM_0])) >> LETAPP_NUM_2) & LETAPP_SET_BIT0;
	  wubLetAppTempBuff[LETAPP_INLINE_STRG_PICKUP	] = (((Type_uByte)(~aubEepData_p[LETAPP_NUM_2])) >> LETAPP_NUM_4) & LETAPP_SET_BIT0;
	  wubLetAppTempBuff[LETAPP_INLINE_STRG_HANGUP	] = (((Type_uByte)(~aubEepData_p[LETAPP_NUM_2])) >> LETAPP_NUM_2) & LETAPP_SET_BIT0;
	  wubLetAppTempBuff[LETAPP_INLINE_STRG_TALK		] = (((Type_uByte)(~aubEepData_p[LETAPP_NUM_2])) >> LETAPP_NUM_3) & LETAPP_SET_BIT0;
	  wubLetAppTempBuff[LETAPP_INLINE_MIC				] = (((Type_uByte)(~aubEepData_p[LETAPP_NUM_2])) >> LETAPP_NUM_5) & LETAPP_SET_BIT0;
	  wubLetAppTempBuff[LETAPP_INLINE_TCU				] = (((Type_uByte)(~aubEepData_p[LETAPP_NUM_2])) >> LETAPP_NUM_1) & LETAPP_SET_BIT0;
	  wubLetAppTempBuff[LETAPP_INLINE_RTM_EVCC		] = (((Type_uByte)(~aubEepData_p[LETAPP_NUM_3])) >> LETAPP_NUM_0) & LETAPP_SET_BIT0;
	  wubLetAppTempBuff[LETAPP_INLINE_4G_MAIN_ANT		] = (((Type_uByte)(~aubEepData_p[LETAPP_NUM_3])) >> LETAPP_NUM_1) & LETAPP_SET_BIT0;
	  wubLetAppTempBuff[LETAPP_INLINE_4G_SUB_ANT		] = (((Type_uByte)(~aubEepData_p[LETAPP_NUM_3])) >> LETAPP_NUM_2) & LETAPP_SET_BIT0;
	  wubLetAppTempBuff[LETAPP_INLINE_GPS_ANT		] = (((Type_uByte)(~aubEepData_p[LETAPP_NUM_3])) >> LETAPP_NUM_3) & LETAPP_SET_BIT0;
	  wubLetAppTempBuff[LETAPP_INLINE_RTM_EVCC_USB	] = (((Type_uByte)(~aubEepData_p[LETAPP_NUM_3])) >> LETAPP_NUM_4) & LETAPP_SET_BIT0;

}
/*********************************************************************************************
*  function                 : nvdLetAppInlineClear
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
static void nvdLetAppInlineClear(void)
{
    (void)wubLetAppSubA_SendMail(LET_SUBA_EV_MEM_INLINE_CLEAR, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
}
/*********************************************************************************************
*  function                 : nvdLetAppSndVolumeSet
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
static void nvdLetAppSndVolumeSet(void)
{
    (void)wubLetAppSubA_SendMail(LET_SUBA_EV_VOL_SET, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
}
/*********************************************************************************************
*  function                 : nvdLetAppSndBassSet
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
static void nvdLetAppSndBassSet(void)
{
    (void)wubLetAppSubA_SendMail(LET_SUBA_EV_BASS_SET, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
}
/*********************************************************************************************
*  function                 : nvdLetAppSndTrebleSet
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
static void nvdLetAppSndTrebleSet(void)
{
    (void)wubLetAppSubA_SendMail(LET_SUBA_EV_TREBLE_SET, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
}
/*********************************************************************************************
*  function                 : nvdLetAppHotErrRest
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
static void nvdLetAppHotErrRest(void)
{
    Type_uByte aubEepResult;

    aubEepResult = RES_NG;

    wvdGen_MemSet(&wubLetAppTempBuff[LETAPP_NUM_0], LETAPP_NUM_0, sizeof(wubLetAppTempBuff));

    /* Commond Data Get */
    wvdGen_MemCpy(&wubLetAppTempBuff[LETAPP_NUM_0], &wubLetAppCmdData[LETAPP_NUM_0], LETAPP_NUM_2);

    wubLetAppTempBuff[LETAPP_NUM_2] = 'H';
    wubLetAppTempBuff[LETAPP_NUM_3] = 'O';
    wubLetAppTempBuff[LETAPP_NUM_4] = 'T';
    wubLetAppTempBuff[LETAPP_NUM_5] = ' ';
    wubLetAppTempBuff[LETAPP_NUM_6] = ' ';
    wubLetAppTempBuff[LETAPP_NUM_7] = ' ';

    //LXQADD aubEepResult = wubEEP_BlockRead(LET_EEP_ADD_HOT_ERR, LETAPP_NUM_2, &wubLetAppTempBuff[LETAPP_NUM_8]);

    if (RES_OK == aubEepResult){
        (void)wubKlineDrvActiveRes(wubLetAppTempBuff[LETAPP_NUM_0], &wubLetAppTempBuff[LETAPP_NUM_1], LETAPP_NUM_9);
    }
    else {
        (void)wubKlineDrvNegativeRes(wubLetAppTempBuff[LETAPP_NUM_0], KLINEDRV_RES_REJECT);
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppResOkSend
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
static void nvdLetAppResOkSend(void)
{	
    Type_uByte aubDataBuff[LETAPP_NUM_1];
	Type_sByte asbResult;

	asbResult = RES_NG;
    wvdGen_MemSet(&aubDataBuff[LETAPP_NUM_0], LETAPP_NUM_0, sizeof(aubDataBuff));

    if (wubLetAppCmdSize > LETAPP_NUM_1){
        (void)wubKlineDrvActiveRes(wubLetAppCmdData[LETAPP_NUM_0], &wubLetAppCmdData[LETAPP_NUM_1], wubLetAppCmdSize - LETAPP_NUM_1);
    }
    else if (LETAPP_NUM_1 == wubLetAppCmdSize){
        if (LETAPP_NUM_130 == wubLetAppCmdData[LETAPP_NUM_0]){
            aubDataBuff[LETAPP_NUM_0] = wubLetAppCmdData[LETAPP_NUM_0] + LETAPP_NUM_64;
           // (void)wubKlineDrv_SendMail(KLINEDRV_EV_LET_RES, &aubDataBuff[LETAPP_NUM_0], LETAPP_NUM_1);
			asbResult = wsbRPCSendMsg(ARPC_ID_LET,ARPC_ID_KLINEDRV,KLINEDRV_EV_LET_RES, &aubDataBuff[LETAPP_NUM_0], LETAPP_NUM_1);
			if(asbResult != RES_OK){
				LETAPP("nvdLetAppResOkSend NG");
			}
		}
    }
    else {
        /* No Action */
    }
}
/*********************************************************************************************
*  function                 : nvdLetAppResNgBusys
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
static void nvdLetAppResNgBusy(void)
{
    (void)wubKlineDrvNegativeRes(wubLetAppCmdData[LETAPP_NUM_0], KLINEDRV_RES_BUSY);
}
/*********************************************************************************************
*  function                 : nvdLetAppNonCondition
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
static void nvdLetAppNonCondition(void)
{
    (void)wubKlineDrvNegativeRes(wubLetAppCmdData[LETAPP_NUM_0], KLINEDRV_RES_NOT_CONDITION);
}
/*********************************************************************************************
*  function                 : nvdLetAppSpkButtonInfo
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
static void nvdLetAppSpkButtonInfo(void)
{
    (void)wubLetAppSubA_SendMail(LET_SUBA_EV_SPK_BUTTON_RES, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
}
/*********************************************************************************************
*  function                 : nvdLetAppERRMemClear
*  date                     : 2016.11.22
*  autho                    : liuminghao
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
static void nvdLetAppERRMemClear(void)
{
    (void)wubLetAppSubA_SendMail(LET_SUBA_EV_ERR_MEM_CLEAR, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
}
/*********************************************************************************************
*  function                 : nvdLetAppLetStartRes
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
static void nvdLetAppLetStartRes(void)
{
    LetResponseRes_ts astLetResData;

    wvdGen_MemSet(&astLetResData, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    if (wstLetAppStdMail_p->uwQueueSize == sizeof(LetResponseRes_ts)){
        wvdGen_MemCpy(&astLetResData, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
        if (astLetResData.ubProcessRes == PROC_SUCCESS){
            (void)wubKlineDrvActiveRes(astLetResData.ubSeriveId, &(astLetResData.ubResData[LETAPP_NUM_0]), LETAPP_NUM_1);
        }
        else {
            (void)wubKlineDrvNegativeRes(astLetResData.ubSeriveId, astLetResData.ubResData[LETAPP_NUM_0]);
        }
    }
    else {
        /* No Action */
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppLetEndRes
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
static void nvdLetAppLetEndRes(void)
{
    LetResponseRes_ts astLetResData;

    wvdGen_MemSet(&astLetResData, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    if (wstLetAppStdMail_p->uwQueueSize == sizeof(LetResponseRes_ts)){
        wvdGen_MemCpy(&astLetResData, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
        if (astLetResData.ubProcessRes == PROC_SUCCESS){
            nvdLetIndexModeUpdate();
            (void)wubKlineDrvActiveRes(astLetResData.ubSeriveId, OS_NULL, LETAPP_NUM_0);
        }
        else {
            (void)wubKlineDrvNegativeRes(astLetResData.ubSeriveId, astLetResData.ubResData[LETAPP_NUM_0]);
        }
    }
    else {
        /* No Action */
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppAmStartRes
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
static void nvdLetAppAmStartRes(void)
{
    LetResponseRes_ts astLetResData;

    wvdGen_MemSet(&astLetResData, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    if (wstLetAppStdMail_p->uwQueueSize == sizeof(LetResponseRes_ts)){
        wvdGen_MemCpy(&astLetResData, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
        if (astLetResData.ubProcessRes == PROC_SUCCESS){
            nvdLetIndexModeUpdate();
            (void)wubKlineDrvActiveRes(astLetResData.ubSeriveId, &(astLetResData.ubResData[LETAPP_NUM_0]), LETAPP_NUM_2);
        }
        else {
            (void)wubKlineDrvNegativeRes(astLetResData.ubSeriveId, astLetResData.ubResData[LETAPP_NUM_0]);
        }
    }
    else {
        /* No Action */
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppAmAnt1Res
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
static void nvdLetAppAmAnt1Res(void)
{
    LetResponseRes_ts astLetResData;

    wvdGen_MemSet(&astLetResData, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    if (wstLetAppStdMail_p->uwQueueSize == sizeof(LetResponseRes_ts)){
        wvdGen_MemCpy(&astLetResData, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
        if (astLetResData.ubProcessRes == PROC_SUCCESS){
            nvdLetIndexModeUpdate();
            (void)wubKlineDrvActiveRes(astLetResData.ubSeriveId, &(astLetResData.ubResData[LETAPP_NUM_0]), LETAPP_NUM_2);
        }
        else {
            (void)wubKlineDrvNegativeRes(astLetResData.ubSeriveId, astLetResData.ubResData[LETAPP_NUM_0]);
        }
    }
    else {
        /* No Action */
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppAmAnt2Res
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
static void nvdLetAppAmAnt2Res(void)
{
    LetResponseRes_ts astLetResData;

    wvdGen_MemSet(&astLetResData, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    if (wstLetAppStdMail_p->uwQueueSize == sizeof(LetResponseRes_ts)){
        wvdGen_MemCpy(&astLetResData, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
        if (astLetResData.ubProcessRes == PROC_SUCCESS){
            nvdLetIndexModeUpdate();
            (void)wubKlineDrvActiveRes(astLetResData.ubSeriveId, &(astLetResData.ubResData[LETAPP_NUM_0]), LETAPP_NUM_2);
        }
        else {
            (void)wubKlineDrvNegativeRes(astLetResData.ubSeriveId, astLetResData.ubResData[LETAPP_NUM_0]);
        }
    }
    else {
        /* No Action */
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppAmAnt3Res
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
static void nvdLetAppAmAnt3Res(void)
{
    LetResponseRes_ts astLetResData;

    wvdGen_MemSet(&astLetResData, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    if (wstLetAppStdMail_p->uwQueueSize == sizeof(LetResponseRes_ts)){
        wvdGen_MemCpy(&astLetResData, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
        if (astLetResData.ubProcessRes == PROC_SUCCESS){
            nvdLetIndexModeUpdate();
            (void)wubKlineDrvActiveRes(astLetResData.ubSeriveId, &(astLetResData.ubResData[LETAPP_NUM_0]), LETAPP_NUM_2);
        }
        else {
            (void)wubKlineDrvNegativeRes(astLetResData.ubSeriveId, astLetResData.ubResData[LETAPP_NUM_0]);
        }
    }
    else {
        /* No Action */
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppAmAnt1EndRes
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
static void nvdLetAppAmAnt1EndRes(void)
{
    LetResponseRes_ts astLetResData;

    wvdGen_MemSet(&astLetResData, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    if (wstLetAppStdMail_p->uwQueueSize == sizeof(LetResponseRes_ts)){
        wvdGen_MemCpy(&astLetResData, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
        if (astLetResData.ubProcessRes == PROC_SUCCESS){
            nvdLetIndexModeUpdate();
            (void)wubKlineDrvActiveRes(astLetResData.ubSeriveId, &(astLetResData.ubResData[LETAPP_NUM_0]), LETAPP_NUM_2);
        }
        else {
            (void)wubKlineDrvNegativeRes(astLetResData.ubSeriveId, astLetResData.ubResData[LETAPP_NUM_0]);
        }
    }
    else {
        /* No Action */
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppAmAnt2EndRes
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
static void nvdLetAppAmAnt2EndRes(void)
{
    LetResponseRes_ts astLetResData;

    wvdGen_MemSet(&astLetResData, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    if (wstLetAppStdMail_p->uwQueueSize == sizeof(LetResponseRes_ts)){
        wvdGen_MemCpy(&astLetResData, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
        if (astLetResData.ubProcessRes == PROC_SUCCESS){
            nvdLetIndexModeUpdate();
            (void)wubKlineDrvActiveRes(astLetResData.ubSeriveId, &(astLetResData.ubResData[LETAPP_NUM_0]), LETAPP_NUM_2);
        }
        else {
            (void)wubKlineDrvNegativeRes(astLetResData.ubSeriveId, astLetResData.ubResData[LETAPP_NUM_0]);
        }
    }
    else {
        /* No Action */
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppAmAnt3EndRes
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
static void nvdLetAppAmAnt3EndRes(void)
{
    LetResponseRes_ts astLetResData;

    wvdGen_MemSet(&astLetResData, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    if (wstLetAppStdMail_p->uwQueueSize == sizeof(LetResponseRes_ts)){
        wvdGen_MemCpy(&astLetResData, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
        if (astLetResData.ubProcessRes == PROC_SUCCESS){
            nvdLetIndexModeUpdate();
            (void)wubKlineDrvActiveRes(astLetResData.ubSeriveId, &(astLetResData.ubResData[LETAPP_NUM_0]), LETAPP_NUM_2);
        }
        else {
            (void)wubKlineDrvNegativeRes(astLetResData.ubSeriveId, astLetResData.ubResData[LETAPP_NUM_0]);
        }
    }
    else {
        /* No Action */
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppAmAntEndRes
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
static void nvdLetAppAmAntEndRes(void)
{
    LetResponseRes_ts astLetResData;

    wvdGen_MemSet(&astLetResData, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    if (wstLetAppStdMail_p->uwQueueSize == sizeof(LetResponseRes_ts)){
        wvdGen_MemCpy(&astLetResData, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
        if (astLetResData.ubProcessRes == PROC_SUCCESS){
            nvdLetIndexModeUpdate();
            (void)wubKlineDrvActiveRes(astLetResData.ubSeriveId, &(astLetResData.ubResData[LETAPP_NUM_0]), LETAPP_NUM_2);
        }
        else {
            (void)wubKlineDrvNegativeRes(astLetResData.ubSeriveId, astLetResData.ubResData[LETAPP_NUM_0]);
        }
    }
    else {
        /* No Action */
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppFmStartRes
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
static void nvdLetAppFmStartRes(void)
{
    LetResponseRes_ts astLetResData;

    wvdGen_MemSet(&astLetResData, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    if (wstLetAppStdMail_p->uwQueueSize == sizeof(LetResponseRes_ts)){
        wvdGen_MemCpy(&astLetResData, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
        if (astLetResData.ubProcessRes == PROC_SUCCESS){
            nvdLetIndexModeUpdate();
            (void)wubKlineDrvActiveRes(astLetResData.ubSeriveId, &(astLetResData.ubResData[LETAPP_NUM_0]), LETAPP_NUM_2);
        }
        else {
            (void)wubKlineDrvNegativeRes(astLetResData.ubSeriveId, astLetResData.ubResData[LETAPP_NUM_0]);
        }
    }
    else {
        /* No Action */
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppFmAnt1Res
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
static void nvdLetAppFmAnt1Res(void)
{
    LetResponseRes_ts astLetResData;

    wvdGen_MemSet(&astLetResData, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    if (wstLetAppStdMail_p->uwQueueSize == sizeof(LetResponseRes_ts)){
        wvdGen_MemCpy(&astLetResData, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
        if (astLetResData.ubProcessRes == PROC_SUCCESS){
            nvdLetIndexModeUpdate();
            (void)wubKlineDrvActiveRes(astLetResData.ubSeriveId, &(astLetResData.ubResData[LETAPP_NUM_0]), LETAPP_NUM_2);
        }
        else {
            (void)wubKlineDrvNegativeRes(astLetResData.ubSeriveId, astLetResData.ubResData[LETAPP_NUM_0]);
        }
    }
    else {
        /* No Action */
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppFmAnt2Res
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
static void nvdLetAppFmAnt2Res(void)
{
    LetResponseRes_ts astLetResData;

    wvdGen_MemSet(&astLetResData, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    if (wstLetAppStdMail_p->uwQueueSize == sizeof(LetResponseRes_ts)){
        wvdGen_MemCpy(&astLetResData, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
        if (astLetResData.ubProcessRes == PROC_SUCCESS){
            nvdLetIndexModeUpdate();
            (void)wubKlineDrvActiveRes(astLetResData.ubSeriveId, &(astLetResData.ubResData[LETAPP_NUM_0]), LETAPP_NUM_2);
        }
        else {
            (void)wubKlineDrvNegativeRes(astLetResData.ubSeriveId, astLetResData.ubResData[LETAPP_NUM_0]);
        }
    }
    else {
        /* No Action */
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppFmAnt3Res
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
static void nvdLetAppFmAnt3Res(void)
{
    LetResponseRes_ts astLetResData;

    wvdGen_MemSet(&astLetResData, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    if (wstLetAppStdMail_p->uwQueueSize == sizeof(LetResponseRes_ts)){
        wvdGen_MemCpy(&astLetResData, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
        if (astLetResData.ubProcessRes == PROC_SUCCESS){
            nvdLetIndexModeUpdate();
            (void)wubKlineDrvActiveRes(astLetResData.ubSeriveId, &(astLetResData.ubResData[LETAPP_NUM_0]), LETAPP_NUM_2);
        }
        else {
            (void)wubKlineDrvNegativeRes(astLetResData.ubSeriveId, astLetResData.ubResData[LETAPP_NUM_0]);
        }
    }
    else {
        /* No Action */
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppFmAnt1EndRes
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
static void nvdLetAppFmAnt1EndRes(void)
{
    LetResponseRes_ts astLetResData;

    wvdGen_MemSet(&astLetResData, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    if (wstLetAppStdMail_p->uwQueueSize == sizeof(LetResponseRes_ts)){
        wvdGen_MemCpy(&astLetResData, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
        if (astLetResData.ubProcessRes == PROC_SUCCESS){
            nvdLetIndexModeUpdate();
            (void)wubKlineDrvActiveRes(astLetResData.ubSeriveId, &(astLetResData.ubResData[LETAPP_NUM_0]), LETAPP_NUM_2);
        }
        else {
            (void)wubKlineDrvNegativeRes(astLetResData.ubSeriveId, astLetResData.ubResData[LETAPP_NUM_0]);
        }
    }
    else {
        /* No Action */
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppFmAnt2EndRes
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
static void nvdLetAppFmAnt2EndRes(void)
{
    LetResponseRes_ts astLetResData;

    wvdGen_MemSet(&astLetResData, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    if (wstLetAppStdMail_p->uwQueueSize == sizeof(LetResponseRes_ts)){
        wvdGen_MemCpy(&astLetResData, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
        if (astLetResData.ubProcessRes == PROC_SUCCESS){
            nvdLetIndexModeUpdate();
            (void)wubKlineDrvActiveRes(astLetResData.ubSeriveId, &(astLetResData.ubResData[LETAPP_NUM_0]), LETAPP_NUM_2);
        }
        else {
            (void)wubKlineDrvNegativeRes(astLetResData.ubSeriveId, astLetResData.ubResData[LETAPP_NUM_0]);
        }
    }
    else {
        /* No Action */
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppFmAnt3EndRes
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
static void nvdLetAppFmAnt3EndRes(void)
{
    LetResponseRes_ts astLetResData;

    wvdGen_MemSet(&astLetResData, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    if (wstLetAppStdMail_p->uwQueueSize == sizeof(LetResponseRes_ts)){
        wvdGen_MemCpy(&astLetResData, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
        if (astLetResData.ubProcessRes == PROC_SUCCESS){
            nvdLetIndexModeUpdate();
            (void)wubKlineDrvActiveRes(astLetResData.ubSeriveId, &(astLetResData.ubResData[LETAPP_NUM_0]), LETAPP_NUM_2);
        }
        else {
            (void)wubKlineDrvNegativeRes(astLetResData.ubSeriveId, astLetResData.ubResData[LETAPP_NUM_0]);
        }
    }
    else {
        /* No Action */
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppFmAntEndRes
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
static void nvdLetAppFmAntEndRes(void)
{
    LetResponseRes_ts astLetResData;

    wvdGen_MemSet(&astLetResData, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    if (wstLetAppStdMail_p->uwQueueSize == sizeof(LetResponseRes_ts)){
        wvdGen_MemCpy(&astLetResData, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
        if (astLetResData.ubProcessRes == PROC_SUCCESS){
            nvdLetIndexModeUpdate();
            (void)wubKlineDrvActiveRes(astLetResData.ubSeriveId, &(astLetResData.ubResData[LETAPP_NUM_0]), LETAPP_NUM_2);
        }
        else {
            (void)wubKlineDrvNegativeRes(astLetResData.ubSeriveId, astLetResData.ubResData[LETAPP_NUM_0]);
        }
    }
    else {
        /* No Action */
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppUsbTestStartRes
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
static void nvdLetAppUsbTestStartRes(void)
{
    LetResponseRes_ts astLetResData;

    wvdGen_MemSet(&astLetResData, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    if (wstLetAppStdMail_p->uwQueueSize == sizeof(LetResponseRes_ts)){
        wvdGen_MemCpy(&astLetResData, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
        if (astLetResData.ubProcessRes == PROC_SUCCESS){
            nvdLetIndexModeUpdate();
            (void)wubKlineDrvActiveRes(astLetResData.ubSeriveId, &(astLetResData.ubResData[LETAPP_NUM_0]), LETAPP_NUM_2);
        }
        else {
            (void)wubKlineDrvNegativeRes(astLetResData.ubSeriveId, astLetResData.ubResData[LETAPP_NUM_0]);
        }
    }
    else {
        /* No Action */
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppUsbSpkStartRes
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
static void nvdLetAppUsbSpkStartRes(void)
{
    LetResponseRes_ts astLetResData;

    wvdGen_MemSet(&astLetResData, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    if (wstLetAppStdMail_p->uwQueueSize == sizeof(LetResponseRes_ts)){
        wvdGen_MemCpy(&astLetResData, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
        if (astLetResData.ubProcessRes == PROC_SUCCESS){
            nvdLetIndexModeUpdate();
            (void)wubKlineDrvActiveRes(astLetResData.ubSeriveId, &(astLetResData.ubResData[LETAPP_NUM_0]), LETAPP_NUM_2);
        }
        else {
            (void)wubKlineDrvNegativeRes(astLetResData.ubSeriveId, astLetResData.ubResData[LETAPP_NUM_0]);
        }
    }
    else {
        /* No Action */
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppUsbSpkEndRes
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
static void nvdLetAppUsbSpkEndRes(void)
{
    LetResponseRes_ts astLetResData;

    wvdGen_MemSet(&astLetResData, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    if (wstLetAppStdMail_p->uwQueueSize == sizeof(LetResponseRes_ts)){
        wvdGen_MemCpy(&astLetResData, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
        if (astLetResData.ubProcessRes == PROC_SUCCESS){
            nvdLetIndexModeUpdate();
            (void)wubKlineDrvActiveRes(astLetResData.ubSeriveId, &(astLetResData.ubResData[LETAPP_NUM_0]), LETAPP_NUM_2);
        }
        else {
            (void)wubKlineDrvNegativeRes(astLetResData.ubSeriveId, astLetResData.ubResData[LETAPP_NUM_0]);
        }
    }
    else {
        /* No Action */
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppUsbTestEndRes
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
static void nvdLetAppUsbTestEndRes(void)
{
    LetResponseRes_ts astLetResData;

    wvdGen_MemSet(&astLetResData, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    if (wstLetAppStdMail_p->uwQueueSize == sizeof(LetResponseRes_ts)){
        wvdGen_MemCpy(&astLetResData, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
        if (astLetResData.ubProcessRes == PROC_SUCCESS){
            nvdLetIndexModeUpdate();
            (void)wubKlineDrvActiveRes(astLetResData.ubSeriveId, &(astLetResData.ubResData[LETAPP_NUM_0]), LETAPP_NUM_2);
        }
        else {
            (void)wubKlineDrvNegativeRes(astLetResData.ubSeriveId, astLetResData.ubResData[LETAPP_NUM_0]);
        }
    }
    else {
        /* No Action */
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppBibiriStartRes
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
static void nvdLetAppBibiriStartRes(void)
{
    LetResponseRes_ts astLetResData;

    wvdGen_MemSet(&astLetResData, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    if (wstLetAppStdMail_p->uwQueueSize == sizeof(LetResponseRes_ts)){
        wvdGen_MemCpy(&astLetResData, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
        if (astLetResData.ubProcessRes == PROC_SUCCESS){
            nvdLetIndexModeUpdate();
            (void)wubKlineDrvActiveRes(astLetResData.ubSeriveId, &(astLetResData.ubResData[LETAPP_NUM_0]), LETAPP_NUM_2);
        }
        else {
            (void)wubKlineDrvNegativeRes(astLetResData.ubSeriveId, astLetResData.ubResData[LETAPP_NUM_0]);
        }
    }
    else {
        /* No Action */
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppBibiriSetRes
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
static void nvdLetAppBibiriSetRes(void)
{
    LetResponseRes_ts astLetResData;

    wvdGen_MemSet(&astLetResData, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    if (wstLetAppStdMail_p->uwQueueSize == sizeof(LetResponseRes_ts)){
        wvdGen_MemCpy(&astLetResData, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
        if (astLetResData.ubProcessRes == PROC_SUCCESS){
            nvdLetIndexModeUpdate();
            (void)wubKlineDrvActiveRes(astLetResData.ubSeriveId, &(astLetResData.ubResData[LETAPP_NUM_0]), LETAPP_NUM_2);
        }
        else {
            (void)wubKlineDrvNegativeRes(astLetResData.ubSeriveId, astLetResData.ubResData[LETAPP_NUM_0]);
        }
    }
    else {
        /* No Action */
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppBibiriEndRes
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
static void nvdLetAppBibiriEndRes(void)
{
    LetResponseRes_ts astLetResData;

    wvdGen_MemSet(&astLetResData, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    if (wstLetAppStdMail_p->uwQueueSize == sizeof(LetResponseRes_ts)){
        wvdGen_MemCpy(&astLetResData, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
        if (astLetResData.ubProcessRes == PROC_SUCCESS){
            nvdLetIndexModeUpdate();
            (void)wubKlineDrvActiveRes(astLetResData.ubSeriveId, &(astLetResData.ubResData[LETAPP_NUM_0]), LETAPP_NUM_2);
        }
        else {
            (void)wubKlineDrvNegativeRes(astLetResData.ubSeriveId, astLetResData.ubResData[LETAPP_NUM_0]);
        }
    }
    else {
        /* No Action */
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppBsmInitialRes
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
static void nvdLetAppBsmInitialRes(void)
{	
    LetResponseRes_ts astLetResData;

    wvdGen_MemSet(&astLetResData, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    if (wstLetAppStdMail_p->uwQueueSize == sizeof(LetResponseRes_ts)){
        wvdGen_MemCpy(&astLetResData, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
        if (astLetResData.ubProcessRes == PROC_SUCCESS){
            nvdLetIndexModeUpdate();
            (void)wubKlineDrvActiveRes(astLetResData.ubSeriveId, &(astLetResData.ubResData[LETAPP_NUM_0]), LETAPP_NUM_1);
        }
        else {
            (void)wubKlineDrvNegativeRes(astLetResData.ubSeriveId, astLetResData.ubResData[LETAPP_NUM_0]);
        }
    }
    else {
        /* No Action */
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppBsmTpSetStartRes
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
static void nvdLetAppBsmTpSetStartRes(void)
{	
    LetResponseRes_ts astLetResData;

    wvdGen_MemSet(&astLetResData, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    if (wstLetAppStdMail_p->uwQueueSize == sizeof(LetResponseRes_ts)){
        wvdGen_MemCpy(&astLetResData, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
        if (astLetResData.ubProcessRes == PROC_SUCCESS){
            nvdLetIndexModeUpdate();
            (void)wubKlineDrvActiveRes(astLetResData.ubSeriveId, &(astLetResData.ubResData[LETAPP_NUM_0]), LETAPP_NUM_2);
        }
        else {
            (void)wubKlineDrvNegativeRes(astLetResData.ubSeriveId, astLetResData.ubResData[LETAPP_NUM_0]);
        }
    }
    else {
        /* No Action */
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppBsmTestEndRes
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
static void nvdLetAppBsmTestEndRes(void)
{
    LetResponseRes_ts astLetResData;

    wvdGen_MemSet(&astLetResData, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    if (wstLetAppStdMail_p->uwQueueSize == sizeof(LetResponseRes_ts)){
        wvdGen_MemCpy(&astLetResData, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
        if (astLetResData.ubProcessRes == PROC_SUCCESS){
            nvdLetIndexModeUpdate();
            (void)wubKlineDrvActiveRes(astLetResData.ubSeriveId, &(astLetResData.ubResData[LETAPP_NUM_0]), LETAPP_NUM_2);
        }
        else {
            (void)wubKlineDrvNegativeRes(astLetResData.ubSeriveId, astLetResData.ubResData[LETAPP_NUM_0]);
        }
    }
    else {
        /* No Action */
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppBsmRestRes
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
static void nvdLetAppBsmRestRes(void)
{
    LetResponseRes_ts astLetResData;

    wvdGen_MemSet(&astLetResData, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    if (wstLetAppStdMail_p->uwQueueSize == sizeof(LetResponseRes_ts)){
        wvdGen_MemCpy(&astLetResData, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
        if (astLetResData.ubProcessRes == PROC_SUCCESS){
            nvdLetIndexModeUpdate();
            (void)wubKlineDrvActiveRes(astLetResData.ubSeriveId, &(astLetResData.ubResData[LETAPP_NUM_0]), LETAPP_NUM_52);
        }
        else {
            (void)wubKlineDrvNegativeRes(astLetResData.ubSeriveId, astLetResData.ubResData[LETAPP_NUM_0]);
        }
    }
    else {
        /* No Action */
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppTunerStartRes
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
static void nvdLetAppTunerStartRes(void)
{
    LetResponseRes_ts astLetResData;

    wvdGen_MemSet(&astLetResData, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    if (wstLetAppStdMail_p->uwQueueSize == sizeof(LetResponseRes_ts)){
        wvdGen_MemCpy(&astLetResData, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
        if (astLetResData.ubProcessRes == PROC_SUCCESS){
            nvdLetIndexModeUpdate();
            (void)wubKlineDrvActiveRes(astLetResData.ubSeriveId, &(astLetResData.ubResData[LETAPP_NUM_0]), LETAPP_NUM_2);
        }
        else {
            (void)wubKlineDrvNegativeRes(astLetResData.ubSeriveId, astLetResData.ubResData[LETAPP_NUM_0]);
        }
    }
    else {
        /* No Action */
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppTunerSetRes
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
static void nvdLetAppTunerSetRes(void)
{
    LetResponseRes_ts astLetResData;

    wvdGen_MemSet(&astLetResData, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    if (wstLetAppStdMail_p->uwQueueSize == sizeof(LetResponseRes_ts)){
        wvdGen_MemCpy(&astLetResData, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
        if (astLetResData.ubProcessRes == PROC_SUCCESS){
            nvdLetIndexModeUpdate();
            (void)wubKlineDrvActiveRes(astLetResData.ubSeriveId, &(astLetResData.ubResData[LETAPP_NUM_0]), LETAPP_NUM_2);
        }
        else {
            (void)wubKlineDrvNegativeRes(astLetResData.ubSeriveId, astLetResData.ubResData[LETAPP_NUM_0]);
        }
    }
    else {
        /* No Action */
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppTunerEndRes
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
static void nvdLetAppTunerEndRes(void)
{
    LetResponseRes_ts astLetResData;

    wvdGen_MemSet(&astLetResData, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    if (wstLetAppStdMail_p->uwQueueSize == sizeof(LetResponseRes_ts)){
        wvdGen_MemCpy(&astLetResData, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
        if (astLetResData.ubProcessRes == PROC_SUCCESS){
            nvdLetIndexModeUpdate();
            (void)wubKlineDrvActiveRes(astLetResData.ubSeriveId, &(astLetResData.ubResData[LETAPP_NUM_0]), LETAPP_NUM_2);
        }
        else {
            (void)wubKlineDrvNegativeRes(astLetResData.ubSeriveId, astLetResData.ubResData[LETAPP_NUM_0]);
        }
    }
    else {
        /* No Action */
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppBeepChkStartRes
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
static void nvdLetAppBeepChkStartRes(void)
{
    LetResponseRes_ts astLetResData;

    wvdGen_MemSet(&astLetResData, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    if (wstLetAppStdMail_p->uwQueueSize == sizeof(LetResponseRes_ts)){
        wvdGen_MemCpy(&astLetResData, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
        if (astLetResData.ubProcessRes == PROC_SUCCESS){
            nvdLetIndexModeUpdate();
            (void)wubKlineDrvActiveRes(astLetResData.ubSeriveId, &(astLetResData.ubResData[LETAPP_NUM_0]), LETAPP_NUM_2);
        }
        else {
            (void)wubKlineDrvNegativeRes(astLetResData.ubSeriveId, astLetResData.ubResData[LETAPP_NUM_0]);
        }
    }
    else {
        /* No Action */
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppBeepChkChUpRes
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
static void nvdLetAppBeepChkChUpRes(void)
{
    LetResponseRes_ts astLetResData;

    wvdGen_MemSet(&astLetResData, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    if (wstLetAppStdMail_p->uwQueueSize == sizeof(LetResponseRes_ts)){
        wvdGen_MemCpy(&astLetResData, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
        if (astLetResData.ubProcessRes == PROC_SUCCESS){
            nvdLetIndexModeUpdate();
            (void)wubKlineDrvActiveRes(astLetResData.ubSeriveId, &(astLetResData.ubResData[LETAPP_NUM_0]), LETAPP_NUM_2);
        }
        else {
            (void)wubKlineDrvNegativeRes(astLetResData.ubSeriveId, astLetResData.ubResData[LETAPP_NUM_0]);
        }
    }
    else {
        /* No Action */
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppBeepChkEndRes
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
static void nvdLetAppBeepChkEndRes(void)
{
    LetResponseRes_ts astLetResData;

    wvdGen_MemSet(&astLetResData, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    if (wstLetAppStdMail_p->uwQueueSize == sizeof(LetResponseRes_ts)){
        wvdGen_MemCpy(&astLetResData, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
        wubLetAppHighIndexMode = H_INDEX_MODE_WAIT;
        wubLetAppBeepIndexMode = MODE_L_SPK_BEEP_OFF;
        if (astLetResData.ubProcessRes == PROC_SUCCESS){
            nvdLetIndexModeUpdate();
            (void)wubKlineDrvActiveRes(astLetResData.ubSeriveId, &(astLetResData.ubResData[LETAPP_NUM_0]), LETAPP_NUM_2);
        }
        else {
            (void)wubKlineDrvNegativeRes(astLetResData.ubSeriveId, astLetResData.ubResData[LETAPP_NUM_0]);
        }
    }
    else {
        /* No Action */
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppBeepDispRes
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
static void nvdLetAppBeepDispRes(void)
{
     nvdLetIndexModeUpdate();
}
/*********************************************************************************************
*  function                 : nvdLetAppMemClearStartRes
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
static void nvdLetAppMemClearStartRes(void)
{
    LetResponseRes_ts astLetResData;

    wvdGen_MemSet(&astLetResData, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    if (wstLetAppStdMail_p->uwQueueSize == sizeof(LetResponseRes_ts)){
        wvdGen_MemCpy(&astLetResData, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
        if (astLetResData.ubProcessRes == PROC_SUCCESS){
            nvdLetIndexModeUpdate();
            (void)wubKlineDrvActiveRes(astLetResData.ubSeriveId, &(astLetResData.ubResData[LETAPP_NUM_0]), LETAPP_NUM_2);
        }
        else {
            (void)wubKlineDrvNegativeRes(astLetResData.ubSeriveId, astLetResData.ubResData[LETAPP_NUM_0]);
        }
    }
    else {
        /* No Action */
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppLetMemClearRes
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
static void nvdLetAppLetMemClearRes(void)
{
    LetResponseRes_ts astLetResData;

    wvdGen_MemSet(&astLetResData, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    if (wstLetAppStdMail_p->uwQueueSize == sizeof(LetResponseRes_ts)){
        wvdGen_MemCpy(&astLetResData, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
        if (astLetResData.ubProcessRes == PROC_SUCCESS){
            nvdLetIndexModeUpdate();
            (void)wubKlineDrvActiveRes(astLetResData.ubSeriveId, &(astLetResData.ubResData[LETAPP_NUM_0]), LETAPP_NUM_1);
        }
        else {
            (void)wubKlineDrvNegativeRes(astLetResData.ubSeriveId, astLetResData.ubResData[LETAPP_NUM_0]);
        }
    }
    else {
        /* No Action */
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppInitialClearRes
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
static void nvdLetAppInitialClearRes(void)
{
    LetResponseRes_ts astLetResData;

    wvdGen_MemSet(&astLetResData, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    if (wstLetAppStdMail_p->uwQueueSize == sizeof(LetResponseRes_ts)){
        wvdGen_MemCpy(&astLetResData, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
        if (astLetResData.ubProcessRes == PROC_SUCCESS){
            nvdLetIndexModeUpdate();
            (void)wubKlineDrvActiveRes(astLetResData.ubSeriveId, &(astLetResData.ubResData[LETAPP_NUM_0]), LETAPP_NUM_1);
        }
        else {
            (void)wubKlineDrvNegativeRes(astLetResData.ubSeriveId, astLetResData.ubResData[LETAPP_NUM_0]);
        }
    }
    else {
        /* No Action */
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppInlineClearRes
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
static void nvdLetAppInlineClearRes(void)
{
    LetResponseRes_ts astLetResData;

    wvdGen_MemSet(&astLetResData, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    if (wstLetAppStdMail_p->uwQueueSize == sizeof(LetResponseRes_ts)){
        wvdGen_MemCpy(&astLetResData, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
        if (astLetResData.ubProcessRes == PROC_SUCCESS){
            nvdLetIndexModeUpdate();
            (void)wubKlineDrvActiveRes(astLetResData.ubSeriveId, &(astLetResData.ubResData[LETAPP_NUM_0]), LETAPP_NUM_1);
        }
        else {
            (void)wubKlineDrvNegativeRes(astLetResData.ubSeriveId, astLetResData.ubResData[LETAPP_NUM_0]);
        }
    }
    else {
        /* No Action */
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppMemClearEndRes
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
static void nvdLetAppMemClearEndRes(void)
{
    LetResponseRes_ts astLetResData;

    wvdGen_MemSet(&astLetResData, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    if (wstLetAppStdMail_p->uwQueueSize == sizeof(LetResponseRes_ts)){
        wvdGen_MemCpy(&astLetResData, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
        if (astLetResData.ubProcessRes == PROC_SUCCESS){
            nvdLetIndexModeUpdate();
            (void)wubKlineDrvActiveRes(astLetResData.ubSeriveId, &(astLetResData.ubResData[LETAPP_NUM_0]), LETAPP_NUM_2);
        }
        else {
            (void)wubKlineDrvNegativeRes(astLetResData.ubSeriveId, astLetResData.ubResData[LETAPP_NUM_0]);
        }
    }
    else {
        /* No Action */
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppVolumeSetRes
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
static void nvdLetAppVolumeSetRes(void)
{
    LetResponseRes_ts astLetResData;

    wvdGen_MemSet(&astLetResData, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    if (wstLetAppStdMail_p->uwQueueSize == sizeof(LetResponseRes_ts)){
        wvdGen_MemCpy(&astLetResData, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
        if (astLetResData.ubProcessRes == PROC_SUCCESS){
            nvdLetIndexModeUpdate();
            (void)wubKlineDrvActiveRes(astLetResData.ubSeriveId, &(astLetResData.ubResData[LETAPP_NUM_0]), LETAPP_NUM_2);
        }
        else {
            (void)wubKlineDrvNegativeRes(astLetResData.ubSeriveId, astLetResData.ubResData[LETAPP_NUM_0]);
        }
    }
    else {
        /* No Action */
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppBassSetRes
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
static void nvdLetAppBassSetRes(void)
{
    LetResponseRes_ts astLetResData;

    wvdGen_MemSet(&astLetResData, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    if (wstLetAppStdMail_p->uwQueueSize == sizeof(LetResponseRes_ts)){
        wvdGen_MemCpy(&astLetResData, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
        if (astLetResData.ubProcessRes == PROC_SUCCESS){
            nvdLetIndexModeUpdate();
            (void)wubKlineDrvActiveRes(astLetResData.ubSeriveId, &(astLetResData.ubResData[LETAPP_NUM_0]), LETAPP_NUM_2);
        }
        else {
            (void)wubKlineDrvNegativeRes(astLetResData.ubSeriveId, astLetResData.ubResData[LETAPP_NUM_0]);
        }
    }
    else {
        /* No Action */
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppTrebleSetRes
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
static void nvdLetAppTrebleSetRes(void)
{
    LetResponseRes_ts astLetResData;

    wvdGen_MemSet(&astLetResData, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    if (wstLetAppStdMail_p->uwQueueSize == sizeof(LetResponseRes_ts)){
        wvdGen_MemCpy(&astLetResData, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
        if (astLetResData.ubProcessRes == PROC_SUCCESS){
            nvdLetIndexModeUpdate();
            (void)wubKlineDrvActiveRes(astLetResData.ubSeriveId, &(astLetResData.ubResData[LETAPP_NUM_0]), LETAPP_NUM_2);
        }
        else {
            (void)wubKlineDrvNegativeRes(astLetResData.ubSeriveId, astLetResData.ubResData[LETAPP_NUM_0]);
        }
    }
    else {
        /* No Action */
    }

}
/*********************************************************************************************
*  function                 : nvdLetAppBusyRes
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
static void nvdLetAppBusyRes(void)
{
    LetResponseRes_ts astLetResData;

    wvdGen_MemSet(&astLetResData, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    if (wstLetAppStdMail_p->uwQueueSize == sizeof(LetResponseRes_ts)){
        wvdGen_MemCpy(&astLetResData, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
        if (astLetResData.ubProcessRes == PROC_SUCCESS){
            nvdLetIndexModeUpdate();
            (void)wubKlineDrvActiveRes(astLetResData.ubSeriveId, OS_NULL, LETAPP_NUM_0);
        }
        else {
            (void)wubKlineDrvNegativeRes(astLetResData.ubSeriveId, astLetResData.ubResData[LETAPP_NUM_0]);
        }
    }
    else {
        /* No Action */
    }
}
/*********************************************************************************************
*  function                 : nvdLetAppBibiriRest
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
static void nvdLetAppBibiriRest(void)
{
    Type_uByte aubEepResult;
    Type_uByte aubBibiriResult;

    aubEepResult = RES_NG;
    aubBibiriResult = LETAPP_NUM_0;
    wvdGen_MemSet(&wubLetAppTempBuff[LETAPP_NUM_0], LETAPP_NUM_0, sizeof(wubLetAppTempBuff));

    //LXQADD aubEepResult = wubEEP_BlockRead(LET_EEP_ADD_BIBIRI_REST, LETAPP_NUM_1, &aubBibiriResult);

    if (RES_OK == aubEepResult){

        nvdLetAppSpkRestChk(&aubBibiriResult, LETAPP_NUM_1);

        wvdGen_MemCpy(&wubLetAppTempBuff[LETAPP_NUM_2], &wubLetAppTempBuff[LETAPP_NUM_0], LETAPP_NUM_2);

        /* Commond Data Get */
        wvdGen_MemCpy(&wubLetAppTempBuff[LETAPP_NUM_0], &wubLetAppCmdData[LETAPP_NUM_0], LETAPP_NUM_2);

        (void)wubKlineDrvActiveRes(wubLetAppTempBuff[LETAPP_NUM_0], &wubLetAppTempBuff[LETAPP_NUM_1], LETAPP_NUM_3);
    }
    else {
        (void)wubKlineDrvNegativeRes(wubLetAppTempBuff[LETAPP_NUM_0], KLINEDRV_RES_REJECT);
    }
}

/*********************************************************************************************
*  function                 : nvdLetAppERRMemClearRes
*  date                     : 2016.11.23
*  autho                    : liuminghao
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
static void nvdLetAppERRMemClearRes(void)
{
    LetResponseRes_ts astLetResData;

    wvdGen_MemSet(&astLetResData, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    if (wstLetAppStdMail_p->uwQueueSize == sizeof(LetResponseRes_ts)){
        wvdGen_MemCpy(&astLetResData, wstLetAppStdMail_p->vdQueueBuffer_p, wstLetAppStdMail_p->uwQueueSize);
        if (astLetResData.ubProcessRes == PROC_SUCCESS){
            nvdLetIndexModeUpdate();
            (void)wubKlineDrvActiveRes(astLetResData.ubSeriveId, &(astLetResData.ubResData[LETAPP_NUM_0]), LETAPP_NUM_1);
        }
        else {
            (void)wubKlineDrvNegativeRes(astLetResData.ubSeriveId, astLetResData.ubResData[LETAPP_NUM_0]);
        }
    }
    else {
        /* No Action */
    }

}

/* End of File */

