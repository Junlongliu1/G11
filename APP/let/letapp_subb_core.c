/**********************************************************************************************
*  file name                : letapp_subb_core.c
*  date                     : 2016.06.01
*  autho                    : zhangjinzhu
*  model                    : TBTA
*  description              : LetApp SubB related file
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
#include "ProEng.h"
#include "generic.h"

/* LetApp Header Files */
#include "letapp.h"
#include "letapp_os.h"
#include "letapp_common.h"
#include "letapp_subb_core.h"

/* Other Components */
//#include "dserial.h"
#include "klinedrv.h"
//#include "eepromcontrol.h"
//#include "radioctrl_def.h"
#include "model.h"
#include "rpc_module.h"
#include "rpc_if.h"
/********************************************************************************************/
/*                           Macro Definition Section                                       */
/********************************************************************************************/

/* For Process Engine Define start */
#define WTID_0                  (0U)            /* Timer ID(wait time 0)        */
#define WTID_1                  (1U)            /* Timer ID(wait time 1)        */
#define WTID_5                  (2U)            /* Timer ID(wait time 5)        */
#define WTID_50                 (3U)            /* Timer ID(wait time 50)       */
#define WTID_100                (4U)            /* Timer ID(wait time 100)      */
#define WTID_500                (5U)            /* Timer ID(wait time 500)      */
#define WTID_1000               (6U)            /* Timer ID(wait time 1000)     */
#define WTID_5000               (7U)            /* Timer ID(wait time 5000)     */
#define WTID_10000              (8U)            /* Timer ID(wait time 10000)    */
#define WTID_15000              (9U)            /* Timer ID(wait time 15000)    */
#define WTID_30000              (10U)           /* Timer ID(wait time 30000)    */
#define WTID_60000              (11U)           /* Timer ID(wait time 60000)    */
#define WTID_FEVR               (12U)           /* Timer ID(wait time forever)  */
#define WTID_MAX                (WTID_FEVR + 1) /* Timer ID Max Define          */

/* Timer Value Define */
#define WTVALUE_0               (0)            /* Timer Value(0msec)           */
#define WTVALUE_1               (1)            /* Timer Value(1msec)           */
#define WTVALUE_5               (5)            /* Timer Value(5msec)           */
#define WTVALUE_50              (50)           /* Timer Value(50msec)          */
#define WTVALUE_100             (100)          /* Timer Value(100msec)         */
#define WTVALUE_500             (500)          /* Timer Value(500msec)         */
#define WTVALUE_1000            (1000)         /* Timer Value(1000msec)        */
#define WTVALUE_5000            (5000)         /* Timer Value(5000msec)        */
#define WTVALUE_10000           (10000)        /* Timer Value(10000msec)       */
#define WTVALUE_15000           (15000)        /* Timer Value(15000msec)       */
#define WTVALUE_30000           (30000)        /* Timer Value(30000msec)       */
#define WTVALUE_60000           (60000)        /* Timer Value(60000msec)       */
#define WTVALUE_FEVR            (-1)            /* Timer Value(time forever)    */

/********************************************************************************************/
/*                           Enumeration Type Definition Section                            */
/********************************************************************************************/

/* LetAppSubB Process ID Define */
enum LETAPP_SUBB_PROC_ID{
/* 000 */ PID_SUBB_AM_START = 0U,
/* 001 */ PID_SUBB_AM_END,
/* 002 */ PID_SUBB_AM_ANT,
/* 003 */ PID_SUBB_AM_ANT_END,
/* 004 */ PID_SUBB_FM_START,
/* 005 */ PID_SUBB_FM_END,
/* 006 */ PID_SUBB_FM_ANT,
/* 007 */ PID_SUBB_FM_ANT_END,
/* 008 */ PID_SUBB_TUNER_START,
/* 009 */ PID_SUBB_TUNER_SET,
/* 010 */ PID_SUBB_TUNER_END,
/* *** */ PID_SUBB_PROC_MAX
};

/* Sub Process ID Define */
enum LETAPP_SUBB_SUBPROC_ID{
/* 000 */ SPID_SUBB_AM_MODE_ON = 0U,
/* 001 */ SPID_SUBB_AM_MODE_ON_CB,
/* 002 */ SPID_SUBB_AM_START_DEF,
/* 003 */ SPID_SUBB_AM_START_OK,
/* 004 */ SPID_SUBB_AM_START_NG,
/* 005 */ SPID_SUBB_AM_MODE_OFF,
/* 006 */ SPID_SUBB_AM_MODE_OFF_CB,
/* 007 */ SPID_SUBB_AM_END_DEF,
/* 008 */ SPID_SUBB_AM_END_OK,
/* 009 */ SPID_SUBB_AM_END_NG,
/* 010 */ SPID_SUBB_AM_ANT_CHK,
/* 011 */ SPID_SUBB_AM_ANT_SET,
/* 012 */ SPID_SUBB_AM_ANT_SET_CB,
/* 013 */ SPID_SUBB_AM_ANT_OK,
/* 014 */ SPID_SUBB_AM_ANT_NG,
/* 015 */ SPID_SUBB_AM_ANT_SM_CHK,
/* 016 */ SPID_SUBB_AM_ANT_SM_CHK_CB,
/* 017 */ SPID_SUBB_AM_ANT_SM_EEP,
/* 018 */ SPID_SUBB_AM_ANT_END_OK,
/* 019 */ SPID_SUBB_AM_ANT_END_NG,
/* 020 */ SPID_SUBB_FM_MODE_ON,
/* 021 */ SPID_SUBB_FM_MODE_ON_CB,
/* 022 */ SPID_SUBB_FM_START_DEF,
/* 023 */ SPID_SUBB_FM_START_OK,
/* 024 */ SPID_SUBB_FM_START_NG,
/* 025 */ SPID_SUBB_FM_MODE_OFF,
/* 026 */ SPID_SUBB_FM_MODE_OFF_CB,
/* 027 */ SPID_SUBB_FM_END_DEF,
/* 028 */ SPID_SUBB_FM_END_OK,
/* 029 */ SPID_SUBB_FM_END_NG,
/* 030 */ SPID_SUBB_FM_ANT_CHK,
/* 031 */ SPID_SUBB_FM_ANT_SET,
/* 032 */ SPID_SUBB_FM_ANT_SET_CB,
/* 033 */ SPID_SUBB_FM_ANT_OK,
/* 034 */ SPID_SUBB_FM_ANT_NG,
/* 035 */ SPID_SUBB_FM_ANT_SM_CHK,
/* 036 */ SPID_SUBB_FM_ANT_SM_CHK_CB,
/* 037 */ SPID_SUBB_FM_ANT_SM_EEP,
/* 038 */ SPID_SUBB_FM_ANT_END_OK,
/* 039 */ SPID_SUBB_FM_ANT_END_NG,
/* 040 */ SPID_SUBB_TUNER_START,
/* 041 */ SPID_SUBB_TUNER_START_OK,
/* 042 */ SPID_SUBB_TUNER_START_NG,
/* 043 */ SPID_SUBB_TUNER_FREQ_CHK,
/* 044 */ SPID_SUBB_TUNER_SET_OK,
/* 045 */ SPID_SUBB_TUNER_SET_NG,
/* 046 */ SPID_SUBB_TUNER_END,
/* 047 */ SPID_SUBB_TUNER_END_OK,
/* 048 */ SPID_SUBB_TUNER_END_NG,
/* *** */ SPID_SUBB_PROC_MAX
};

/********************************************************************************************/
/*                           User type definition section                                   */
/********************************************************************************************/

/* not used. */

/********************************************************************************************/
/*                           Structure/Union Type Definition Section                        */
/********************************************************************************************/

typedef struct{
    Type_uByte ubInputEvent;
    Type_uByte ubProcessID;
}LetSubB_EventChg_ts;

/********************************************************************************************/
/*                           ROM Table Section                                              */
/********************************************************************************************/
/* DiagApp PE Wait Timer */
static const Type_sWord nswLetSubBPETimer_Tbl[] = {
    WTVALUE_0,
    WTVALUE_1,
    WTVALUE_5,
    WTVALUE_50,
    WTVALUE_100,
    WTVALUE_500,
    WTVALUE_1000,
    WTVALUE_5000,
    WTVALUE_10000,
    WTVALUE_15000,
    WTVALUE_30000,
    WTVALUE_60000,
    WTVALUE_FEVR
};
/* Let Event Exchange Table */
static LetSubB_EventChg_ts const nstLetSubBEvChg_a[] = {
    /*** Input Event ***                *** Process ID ***/
    {  LET_SUBB_EV_AM_START,            PID_SUBB_AM_START            },
    {  LET_SUBB_EV_AM_END,              PID_SUBB_AM_END              },
    {  LET_SUBB_EV_AM_ANT,              PID_SUBB_AM_ANT              },
    {  LET_SUBB_EV_AM_ANT_END,          PID_SUBB_AM_ANT_END          },
    {  LET_SUBB_EV_FM_START,            PID_SUBB_FM_START            },
    {  LET_SUBB_EV_FM_END,              PID_SUBB_FM_END              },
    {  LET_SUBB_EV_FM_ANT,              PID_SUBB_FM_ANT              },
    {  LET_SUBB_EV_FM_ANT_END,          PID_SUBB_FM_ANT_END          },
    {  LET_SUBB_EV_TUNER_START,         PID_SUBB_TUNER_START         },
    {  LET_SUBB_EV_TUNER_SET,           PID_SUBB_TUNER_SET           },
    {  LET_SUBB_EV_TUNER_END,           PID_SUBB_TUNER_END           },
    {  LET_SUBB_EV_MAX,                 PID_SUBB_PROC_MAX            },
};
/********************************************************************************************/
/*                           Static Variable Definition Section                             */
/********************************************************************************************/

static ProcCTB_ts nstLetAppSubBCTB;    /* Process Control Table */
static ProcBuf_ts nstLetAppSubBProcBuf_a[LETAPP_NUM_3];    /* Process Buffer */

/********************************************************************************************/
/*                           Static Function Declaration Section                            */
/********************************************************************************************/
static void nvdLetAppSubBCmdSave(Type_uByte* aubDataBuff_p, Type_uByte aubSize);
static Type_uByte nubLetSubBpEvPidChg(void);
static Type_uByte nubLetSubBAmModeOn(void);
static Type_uByte nubLetSubBAmModeOnCB(void);
static Type_uByte nubLetSubBAmStartDef(void);
static Type_uByte nubLetSubBAmStartOk(void);
static Type_uByte nubLetSubBAmStartNg(void);
static Type_uByte nubLetSubBAmModeOff(void);
static Type_uByte nubLetSubBAmModeOffCB(void);
static Type_uByte nubLetSubBAmEndDef(void);
static Type_uByte nubLetSubBAmEndOk(void);
static Type_uByte nubLetSubBAmEndNg(void);
static Type_uByte nubLetSubBAmAntChk(void);
static Type_uByte nubLetSubBAmAntSet(void);
static Type_uByte nubLetSubBAmAntSetCB(void);
static Type_uByte nubLetSubBAmAntOk(void);
static Type_uByte nubLetSubBAmAntNg(void);
static Type_uByte nubLetSubBAmAntSmChk(void);
static Type_uByte nubLetSubBAmAntSmChkCB(void);
static Type_uByte nubLetSubBAmSmEep(void);
static Type_uByte nubLetSubBAmAntEndOk(void);
static Type_uByte nubLetSubBAmAntEndNg(void);
static Type_uByte nubLetSubBFmModeOn(void);
static Type_uByte nubLetSubBFmModeOnCB(void);
static Type_uByte nubLetSubBFmStartDef(void);
static Type_uByte nubLetSubBFmStartOk(void);
static Type_uByte nubLetSubBFmStartNg(void);
static Type_uByte nubLetSubBFmModeOff(void);
static Type_uByte nubLetSubBFmModeOffCB(void);
static Type_uByte nubLetSubBFmEndDef(void);
static Type_uByte nubLetSubBFmEndOk(void);
static Type_uByte nubLetSubBFmEndNg(void);
static Type_uByte nubLetSubBFmAntChk(void);
static Type_uByte nubLetSubBFmAntSet(void);
static Type_uByte nubLetSubBFmAntSetCB(void);
static Type_uByte nubLetSubBFmAntOk(void);
static Type_uByte nubLetSubBFmAntNg(void);
static Type_uByte nubLetSubBFmAntSmChk(void);
static Type_uByte nubLetSubBFmAntSmChkCB(void);
static Type_uByte nubLetSubBFmSmEep(void);
static Type_uByte nubLetSubBFmAntEndOk(void);
static Type_uByte nubLetSubBFmAntEndNg(void);
static Type_uByte nubLetSubBTunerStart(void);
static Type_uByte nubLetSubBTunerStartOk(void);
static Type_uByte nubLetSubBTunerStartNg(void);
static Type_uByte nubLetSubBTunerFreqChk(void);
static Type_uByte nubLetSubBTunerSetOk(void);
static Type_uByte nubLetSubBTunerSetNg(void);
static Type_uByte nubLetSubBTunerEnd(void);
static Type_uByte nubLetSubBTunerEndOk(void);
static Type_uByte nubLetSubBTunerEndNg(void);

/********************************************************************************************/
/*                           Global variable definition section                             */
/********************************************************************************************/

/* not used. */

/********************************************************************************************/
/*                           Function Definition Section                                    */
/********************************************************************************************/
/*************************** LetAppSubB Process Table ***************************************/
const ElementOfProcTbl_ts nstLetSubBAmStart_Tbl[] = { /* PID_SUBB_AM_START */
/* STAGE00 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_AM_MODE_ON,           STAGE1,      WTID_5000,   C_STAGE4,    WTID_0),
/* STAGE01 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_AM_MODE_ON_CB,        C_STAGE2,    WTID_0,      C_STAGE4,    WTID_0),
/* STAGE02 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_AM_START_DEF,         C_STAGE3,    WTID_0,      C_STAGE4,    WTID_0),
/* STAGE03 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_AM_START_OK,          PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
/* STAGE04 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_AM_START_NG,          PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
};

const ElementOfProcTbl_ts nstLetSubBAmEnd_Tbl[] = { /* PID_SUBB_AM_END */
/* STAGE00 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_AM_MODE_OFF,          STAGE1,      WTID_5000,   C_STAGE4,    WTID_0),
/* STAGE01 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_AM_MODE_OFF_CB,       C_STAGE2,    WTID_0,      C_STAGE4,    WTID_0),
/* STAGE02 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_AM_END_DEF,           C_STAGE3,    WTID_0,      C_STAGE4,    WTID_0),
/* STAGE03 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_AM_END_OK,            PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
/* STAGE04 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_AM_END_NG,            PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
};

const ElementOfProcTbl_ts nstLetSubBAmAnt_Tbl[] = { /* PID_SUBB_AM_ANT */
/* STAGE00 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_AM_ANT_CHK,           C_STAGE1,    WTID_0,      C_STAGE4,    WTID_0),
/* STAGE01 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_AM_ANT_SET,           STAGE2,      WTID_5000,   C_STAGE4,    WTID_0),
/* STAGE02 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_AM_ANT_SET_CB,        C_STAGE3,    WTID_0,      C_STAGE4,    WTID_0),
/* STAGE03 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_AM_ANT_OK,            PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
/* STAGE04 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_AM_ANT_NG,            PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
};

const ElementOfProcTbl_ts nstLetSubBAmAntEnd_Tbl[] = { /* PID_SUBB_AM_ANT_END */
/* STAGE00 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_AM_ANT_SM_CHK,        STAGE1,      WTID_5000,   C_STAGE4,    WTID_0),
/* STAGE01 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_AM_ANT_SM_CHK_CB,     C_STAGE2,    WTID_0,      C_STAGE4,    WTID_0),
/* STAGE02 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_AM_ANT_SM_EEP,        C_STAGE3,    WTID_0,      C_STAGE4,    WTID_0),
/* STAGE03 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_AM_ANT_END_OK,        PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
/* STAGE04 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_AM_ANT_END_NG,        PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
};

const ElementOfProcTbl_ts nstLetSubBFmStart_Tbl[] = { /* PID_SUBB_FM_START */
/* STAGE00 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_FM_MODE_ON,           STAGE1,      WTID_5000,   C_STAGE4,    WTID_0),
/* STAGE01 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_FM_MODE_ON_CB,        C_STAGE2,    WTID_0,      C_STAGE4,    WTID_0),
/* STAGE02 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_FM_START_DEF,         C_STAGE3,    WTID_0,      C_STAGE4,    WTID_0),
/* STAGE03 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_FM_START_OK,          PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
/* STAGE04 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_FM_START_NG,          PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
};

const ElementOfProcTbl_ts nstLetSubBFmEnd_Tbl[] = { /* PID_SUBB_FM_END */
/* STAGE00 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_FM_MODE_OFF,          STAGE1,      WTID_5000,   C_STAGE4,    WTID_0),
/* STAGE01 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_FM_MODE_OFF_CB,       C_STAGE2,    WTID_0,      C_STAGE4,    WTID_0),
/* STAGE02 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_FM_END_DEF,           C_STAGE3,    WTID_0,      C_STAGE4,    WTID_0),
/* STAGE03 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_FM_END_OK,            PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
/* STAGE04 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_FM_END_NG,            PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
};

const ElementOfProcTbl_ts nstLetSubBFmAnt_Tbl[] = { /* PID_SUBB_FM_ANT */
/* STAGE00 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_FM_ANT_CHK,           C_STAGE1,    WTID_0,      C_STAGE4,    WTID_0),
/* STAGE01 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_FM_ANT_SET,           STAGE2,      WTID_5000,   C_STAGE4,    WTID_0),
/* STAGE02 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_FM_ANT_SET_CB,        C_STAGE3,    WTID_0,      C_STAGE4,    WTID_0),
/* STAGE03 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_FM_ANT_OK,            PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
/* STAGE04 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_FM_ANT_NG,            PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
};

const ElementOfProcTbl_ts nstLetSubBFmAntEnd_Tbl[] = { /* PID_SUBB_FM_ANT_END */
/* STAGE00 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_FM_ANT_SM_CHK,        STAGE1,      WTID_5000,   C_STAGE4,    WTID_0),
/* STAGE01 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_FM_ANT_SM_CHK_CB,     C_STAGE2,    WTID_0,      C_STAGE4,    WTID_0),
/* STAGE02 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_FM_ANT_SM_EEP,        C_STAGE3,    WTID_0,      C_STAGE4,    WTID_0),
/* STAGE03 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_FM_ANT_END_OK,        PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
/* STAGE04 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_FM_ANT_END_NG,        PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
};

const ElementOfProcTbl_ts nstLetSubBTunerStart_Tbl[] = { /* PID_SUBB_TUNER_START */
/* STAGE00 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_TUNER_START,          C_STAGE1,    WTID_0,      C_STAGE2,    WTID_0),
/* STAGE01 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_TUNER_START_OK,       PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
/* STAGE02 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_TUNER_START_NG,       PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
};

const ElementOfProcTbl_ts nstLetSubBTuenrSet_Tbl[] = { /* PID_SUBB_TUNER_SET */
/* STAGE00 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,       SPID_SUBB_TUNER_FREQ_CHK,       C_STAGE1,    WTID_0,      C_STAGE11,   WTID_0),
/* STAGE01*/ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_FM_ANT_CHK,           C_STAGE2,    WTID_0,      C_STAGE22,    WTID_0), 
/* STAGE02*/ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_FM_MODE_ON,           STAGE3,      WTID_5000,   C_STAGE22,   WTID_0),
/* STAGE03*/ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_FM_MODE_ON_CB,        C_STAGE4,    WTID_0,      C_STAGE22,   WTID_0),
/* STAGE04*/ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_FM_ANT_SET,           STAGE5,      WTID_5000,   C_STAGE22,   WTID_0),
/* STAGE05*/ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_FM_ANT_SET_CB,        C_STAGE6,    WTID_0,      C_STAGE22,   WTID_0),
/* STAGE06*/ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_FM_ANT_SM_CHK,        STAGE7,      WTID_5000,   C_STAGE22,   WTID_0),
/* STAGE07*/ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_FM_ANT_SM_CHK_CB,     C_STAGE8,    WTID_0,      C_STAGE22,   WTID_0),
/* STAGE08*/ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_FM_ANT_SM_EEP,        STAGE9,      WTID_0,      C_STAGE22,   WTID_0),
/* STAGE09*/ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_FM_MODE_OFF,          STAGE10,      WTID_5000,   C_STAGE22,   WTID_0),
/* STAGE10*/ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_FM_MODE_OFF_CB,       C_STAGE21,   WTID_0,      C_STAGE22,   WTID_0),
/* STAGE11*/ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_AM_ANT_CHK,           C_STAGE12,    WTID_0,      C_STAGE22,    WTID_0), 
/* STAGE12*/ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_AM_MODE_ON,           STAGE13,     WTID_5000,   C_STAGE22,   WTID_0),
/* STAGE13*/ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_AM_MODE_ON_CB,        C_STAGE14,   WTID_0,      C_STAGE22,   WTID_0),
/* STAGE14*/ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_AM_ANT_SET,           STAGE15,     WTID_5000,   C_STAGE22,   WTID_0),
/* STAGE15*/ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_AM_ANT_SET_CB,        C_STAGE16,   WTID_0,      C_STAGE22,   WTID_0),
/* STAGE16*/ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_AM_ANT_SM_CHK,        STAGE17,     WTID_5000,   C_STAGE22,   WTID_0),
/* STAGE17*/ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_AM_ANT_SM_CHK_CB,     C_STAGE18,   WTID_0,      C_STAGE22,   WTID_0),
/* STAGE18*/ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_AM_ANT_SM_EEP,        STAGE19,     WTID_0,      C_STAGE22,   WTID_0),
/* STAGE19*/ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_AM_MODE_OFF,          STAGE20,     WTID_5000,   C_STAGE22,   WTID_0),
/* STAGE20*/ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_AM_MODE_OFF_CB,       C_STAGE21,   WTID_0,      C_STAGE22,   WTID_0),
/* STAGE21*/ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_TUNER_SET_OK,         PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
/* STAGE22*/ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_TUNER_SET_NG,         PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
};

const ElementOfProcTbl_ts nstLetSubBTunerEnd_Tbl[] = { /* PID_SUBB_TUNER_END */
/* STAGE00 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_TUNER_END,            C_STAGE1,    WTID_0,      C_STAGE2,    WTID_0),
/* STAGE01 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_TUNER_END_OK,         PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
/* STAGE02 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBB_TUNER_END_NG,         PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
};

/*************************** LetAppSubB Main Process ****************************************/
const ElementOfProcTbl_ts (*const nstLetAppSubBProc_Tbl[PID_SUBB_PROC_MAX]) = {
/* 000 */ nstLetSubBAmStart_Tbl,           /* PID_SUBB_AM_START */
/* 001 */ nstLetSubBAmEnd_Tbl,             /* PID_SUBB_AM_END */
/* 002 */ nstLetSubBAmAnt_Tbl,             /* PID_SUBB_AM_ANT */
/* 003 */ nstLetSubBAmAntEnd_Tbl,          /* PID_SUBB_AM_ANT_END */
/* 004 */ nstLetSubBFmStart_Tbl,           /* PID_SUBB_FM_START */
/* 005 */ nstLetSubBFmEnd_Tbl,             /* PID_SUBB_FM_END */
/* 006 */ nstLetSubBFmAnt_Tbl,             /* PID_SUBB_FM_ANT */
/* 007 */ nstLetSubBFmAntEnd_Tbl,          /* PID_SUBB_FM_ANT_END */
/* 008 */ nstLetSubBTunerStart_Tbl,        /* PID_SUBB_TUNER_START */
/* 009 */ nstLetSubBTuenrSet_Tbl,          /* PID_SUBB_TUNER_SET */
/* 010 */ nstLetSubBTunerEnd_Tbl           /* PID_SUBB_TUNER_END */
};

/*************************** LetAppSubB SubProcess ******************************************/
Type_uByte (*const nstLetAppSubBSubProc_Tbl[SPID_SUBB_PROC_MAX])(void) = {
/* 000 */ nubLetSubBAmModeOn,              /* SPID_SUBB_AM_MODE_ON */
/* 001 */ nubLetSubBAmModeOnCB,            /* SPID_SUBB_AM_MODE_ON_CB */
/* 002 */ nubLetSubBAmStartDef,            /* SPID_SUBB_AM_START_DEF */
/* 003 */ nubLetSubBAmStartOk,             /* SPID_SUBB_AM_START_OK */
/* 004 */ nubLetSubBAmStartNg,             /* SPID_SUBB_AM_START_NG */
/* 005 */ nubLetSubBAmModeOff,             /* SPID_SUBB_AM_MODE_OFF */
/* 006 */ nubLetSubBAmModeOffCB,           /* SPID_SUBB_AM_MODE_OFF_CB */
/* 007 */ nubLetSubBAmEndDef,              /* SPID_SUBB_AM_END_DEF */
/* 008 */ nubLetSubBAmEndOk,               /* SPID_SUBB_AM_END_OK */
/* 009 */ nubLetSubBAmEndNg,               /* SPID_SUBB_AM_END_NG */
/* 010 */ nubLetSubBAmAntChk,              /* SPID_SUBB_AM_ANT_CHK */
/* 011 */ nubLetSubBAmAntSet,              /* SPID_SUBB_AM_ANT_SET */
/* 012 */ nubLetSubBAmAntSetCB,            /* SPID_SUBB_AM_ANT_SET_CB */
/* 013 */ nubLetSubBAmAntOk,               /* SPID_SUBB_AM_ANT_OK */
/* 014 */ nubLetSubBAmAntNg,               /* SPID_SUBB_AM_ANT_NG */
/* 015 */ nubLetSubBAmAntSmChk,            /* SPID_SUBB_AM_ANT_SM_CHK */
/* 016 */ nubLetSubBAmAntSmChkCB,          /* SPID_SUBB_AM_ANT_SM_CHK_CB */
/* 017 */ nubLetSubBAmSmEep,               /* SPID_SUBB_AM_ANT_SM_EEP */
/* 018 */ nubLetSubBAmAntEndOk,            /* SPID_SUBB_AM_ANT_END_OK */
/* 019 */ nubLetSubBAmAntEndNg,            /* SPID_SUBB_AM_ANT_END_NG */
/* 020 */ nubLetSubBFmModeOn,              /* SPID_SUBB_FM_MODE_ON */
/* 021 */ nubLetSubBFmModeOnCB,            /* SPID_SUBB_FM_MODE_ON_CB */
/* 022 */ nubLetSubBFmStartDef,            /* SPID_SUBB_FM_START_DEF */
/* 023 */ nubLetSubBFmStartOk,             /* SPID_SUBB_FM_START_OK */
/* 024 */ nubLetSubBFmStartNg,             /* SPID_SUBB_FM_START_NG */
/* 025 */ nubLetSubBFmModeOff,             /* SPID_SUBB_FM_MODE_OFF */
/* 026 */ nubLetSubBFmModeOffCB,           /* SPID_SUBB_FM_MODE_OFF_CB */
/* 027 */ nubLetSubBFmEndDef,              /* SPID_SUBB_FM_END_DEF */
/* 028 */ nubLetSubBFmEndOk,               /* SPID_SUBB_FM_END_OK */
/* 029 */ nubLetSubBFmEndNg,               /* SPID_SUBB_FM_END_NG */
/* 030 */ nubLetSubBFmAntChk,              /* SPID_SUBB_FM_ANT_CHK */
/* 031 */ nubLetSubBFmAntSet,              /* SPID_SUBB_FM_ANT_SET */
/* 032 */ nubLetSubBFmAntSetCB,            /* SPID_SUBB_FM_ANT_SET_CB */
/* 033 */ nubLetSubBFmAntOk,               /* SPID_SUBB_FM_ANT_OK */
/* 034 */ nubLetSubBFmAntNg,               /* SPID_SUBB_FM_ANT_NG */
/* 035 */ nubLetSubBFmAntSmChk,            /* SPID_SUBB_FM_ANT_SM_CHK */
/* 036 */ nubLetSubBFmAntSmChkCB,          /* SPID_SUBB_FM_ANT_SM_CHK_CB */
/* 037 */ nubLetSubBFmSmEep,               /* SPID_SUBB_FM_ANT_SM_EEP */
/* 038 */ nubLetSubBFmAntEndOk,            /* SPID_SUBB_FM_ANT_END_OK */
/* 039 */ nubLetSubBFmAntEndNg,            /* SPID_SUBB_FM_ANT_END_NG */
/* 040 */ nubLetSubBTunerStart,            /* SPID_SUBB_TUNER_START */
/* 041 */ nubLetSubBTunerStartOk,          /* SPID_SUBB_TUNER_START_OK */
/* 042 */ nubLetSubBTunerStartNg,          /* SPID_SUBB_TUNER_START_NG */
/* 043 */ nubLetSubBTunerFreqChk,          /* SPID_SUBB_TUNER_FREQ_CHK */
/* 044 */ nubLetSubBTunerSetOk,            /* SPID_SUBB_TUNER_SET_OK */
/* 045 */ nubLetSubBTunerSetNg,            /* SPID_SUBB_TUNER_SET_NG */
/* 046 */ nubLetSubBTunerEnd,              /* SPID_SUBB_TUNER_END */
/* 047 */ nubLetSubBTunerEndOk,            /* SPID_SUBB_TUNER_END_OK */
/* 048 */ nubLetSubBTunerEndNg,            /* SPID_SUBB_TUNER_END_NG */
};

#if defined ( STACK_CHECK )
/********************************************************************************************/
/**	\function		nvdLetSubBProcessForStackCheck
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
static void nvdLetSubBProcessForStackCheck(void)
{
/* 000 */(void)nubLetSubBAmModeOn();         
/* 001 */(void)nubLetSubBAmModeOnCB();     
/* 002 */(void)nubLetSubBAmStartDef();        
/* 003 */(void)nubLetSubBAmStartOk();         
/* 004 */(void)nubLetSubBAmStartNg();          
/* 005 */(void)nubLetSubBAmModeOff();         
/* 006 */(void)nubLetSubBAmModeOffCB();    
/* 007 */(void)nubLetSubBAmEndDef();          
/* 008 */(void)nubLetSubBAmEndOk();           
/* 009 */(void)nubLetSubBAmEndNg();           
/* 010 */(void)nubLetSubBAmAntChk();          
/* 011 */(void)nubLetSubBAmAntSet();           
/* 012 */(void)nubLetSubBAmAntSetCB();       
/* 013 */(void)nubLetSubBAmAntOk();            
/* 014 */(void)nubLetSubBAmAntNg();            
/* 015 */(void)nubLetSubBAmAntSmChk();     
/* 016 */(void)nubLetSubBAmAntSmChkCB(); 
/* 017 */(void)nubLetSubBAmSmEep();          
/* 018 */(void)nubLetSubBAmAntEndOk();      
/* 019 */(void)nubLetSubBAmAntEndNg();       
/* 020 */(void)nubLetSubBFmModeOn();         
/* 021 */(void)nubLetSubBFmModeOnCB();     
/* 022 */(void)nubLetSubBFmStartDef();         
/* 023 */(void)nubLetSubBFmStartOk();          
/* 024 */(void)nubLetSubBFmStartNg();          
/* 025 */(void)nubLetSubBFmModeOff();         
/* 026 */(void)nubLetSubBFmModeOffCB();     
/* 027 */(void)nubLetSubBFmEndDef();           
/* 028 */(void)nubLetSubBFmEndOk();            
/* 029 */(void)nubLetSubBFmEndNg();            
/* 030 */(void)nubLetSubBFmAntChk();          
/* 031 */(void)nubLetSubBFmAntSet();           
/* 032 */(void)nubLetSubBFmAntSetCB();       
/* 033 */(void)nubLetSubBFmAntOk();            
/* 034 */(void)nubLetSubBFmAntNg();            
/* 035 */(void)nubLetSubBFmAntSmChk();      
/* 036 */(void)nubLetSubBFmAntSmChkCB(); 
/* 037 */(void)nubLetSubBFmSmEep();           
/* 038 */(void)nubLetSubBFmAntEndOk();       
/* 039 */(void)nubLetSubBFmAntEndNg();       
/* 040 */(void)nubLetSubBTunerStart();          
/* 041 */(void)nubLetSubBTunerStartOk();      
/* 042 */(void)nubLetSubBTunerStartNg();      
/* 043 */(void)nubLetSubBTunerFreqChk();     
/* 044 */(void)nubLetSubBTunerSetOk();        
/* 045 */(void)nubLetSubBTunerSetNg();        
/* 046 */(void)nubLetSubBTunerEnd();            
/* 047 */(void)nubLetSubBTunerEndOk();        
/* 048 */(void)nubLetSubBTunerEndNg();  		    
}
#endif
/*********************************************************************************************
*  function                 : wvdLetAppSubB_SetupProEng
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
void wvdLetAppSubB_SetupProEng(void)
{
    (void)wubPESetUp(&nstLetAppSubBCTB, &nstLetAppSubBProcBuf_a[LETAPP_NUM_0], nstLetAppSubBProc_Tbl, nstLetAppSubBSubProc_Tbl);
}
/*********************************************************************************************
*  function                 : wubLetAppSubB_SendMail
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
void wubLetAppSubB_SendMail(Type_uHWord auhEventCode,void* avdBuffer_p, Type_uWord auwSize)
{
    Type_sByte asbResult;

	asbResult = RES_NG;
	
	asbResult = wsbRPCSendMsg(ARPC_ID_LET,ARPC_ID_LET_SUBB,auhEventCode,avdBuffer_p,auwSize);
	if(asbResult != RES_OK){
		LETAPP("wubLetAppSubB_SendMail NG");
	}
}
/*********************************************************************************************
*  function                 : wvdLetAppSubBControl
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
void wvdLetAppSubBControl(void)
{
    /* Auto variable Define */
    Type_uHWord auhMainProcPID;     /* <AUTO> Procedure PID */
    Type_uByte aubProcIsExecute;    /* <AUTO> Procedure Status */
    Type_uByte aubProcResult;       /* <AUTO> Procedure Result */

    /* Initalize */
    aubProcIsExecute    = STD_FALSE;
    aubProcResult       = PERC_NO_EXECUTE_PROC;
    auhMainProcPID      = PID_SUBB_PROC_MAX;
    
    /* Setting Request for Stand-by Process */
    wvdPESetReq(&nstLetAppSubBCTB);

    if ((LET_SUBB_EV_INVALID < wstLetAppSubBMail_p->uhEventCode) && (LET_SUBB_EV_MAX > wstLetAppSubBMail_p->uhEventCode)){

        /* Process Execute status check */
        aubProcIsExecute = wubPEIsExecuteing(&nstLetAppSubBCTB);

        if (STD_TRUE == aubProcIsExecute){
            /* No Action */
        }
        else { /* Process Execute */

            if (wstLetAppSubBMail_p->vdQueueBuffer_p != OS_NULL){
                nvdLetAppSubBCmdSave(wstLetAppSubBMail_p->vdQueueBuffer_p, wstLetAppSubBMail_p->uwQueueSize);
            }
            else {
                /* No Action */
            }
            auhMainProcPID = (Type_uHWord)nubLetSubBpEvPidChg(); /* PE PID Get */

            if(PID_SUBB_PROC_MAX > auhMainProcPID){
                /* PE Reset */
                wvdPEResetProcess(&nstLetAppSubBCTB);
                /* PE Set */
                wvdPESetProcess(auhMainProcPID, &nstLetAppSubBCTB);
                /* PE Execute */
                aubProcResult = wubPEExecute(&nstLetAppSubBCTB);
#if defined ( STACK_CHECK )
		  nvdLetSubBProcessForStackCheck();
#endif
                /* Getting wait time */
                if ((PERC_NO_EXECUTE_PROC != aubProcResult) && (WTID_MAX > aubProcResult)){

                    wswLetAppSubBTmout = nswLetSubBPETimer_Tbl[aubProcResult]; /* PE Wait Timer Get */

                    wuwLetSubBStartTime = OSAL_GetOSTimeTick(); /* Timer Caculate Start */
                }
                else {
                    /* No Action */
                }
            }
            else {
                /* No Action */
            }
        }
    }
    else if ((LET_SUBB_EV_INVALID_RES < wstLetAppSubBMail_p->uhEventCode) && (LET_SUBB_EV_MAX_RES > wstLetAppSubBMail_p->uhEventCode)){
        /* Process Execute status check */
        aubProcIsExecute = wubPEIsExecuteing(&nstLetAppSubBCTB);

        if (STD_TRUE == aubProcIsExecute){

            aubProcResult = wubPEExecute(&nstLetAppSubBCTB);
            /* Getting wait time */
            if ((PERC_NO_EXECUTE_PROC != aubProcResult) && (WTID_MAX > aubProcResult)){

                wswLetAppSubBTmout = nswLetSubBPETimer_Tbl[aubProcResult]; /* PE Wait Timer Get */

                wuwLetSubBStartTime = OSAL_GetOSTimeTick(); /* Timer Caculate Start */
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

    /* Timeout time Caculate */
    if (wswLetAppSubBTmout != OS_TOUT_FOREVER){
        /* Get Current Time */
        wuwLetAppSubBNowTime = OSAL_GetOSTimeTick();
        /* Get Passed Time */
        wswLetAppSubBPassTime = wswLetApplGetProcTime(wuwLetSubBStartTime, wuwLetAppSubBNowTime);
        if (wswLetAppSubBPassTime < wswLetAppSubBTmout){
            /* Timeout time caculate */
            wswLetAppSubBTmout = (wswLetAppSubBTmout - wswLetAppSubBPassTime);
        }
        else{
            /* Timeout time 0 */
            wswLetAppSubBTmout = LETAPP_NUM_0;
        }
    }
    else{
        /* No Action */
    }

    /* Timeout time caculate Start */
    wuwLetSubBStartTime = OSAL_GetOSTimeTick();
}
/*********************************************************************************************
*  function                 : nvdLetAppSubBCmdSave
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
static void nvdLetAppSubBCmdSave(Type_uByte* aubDataBuff_p, Type_uByte aubSize)
{
    if (aubSize >= sizeof(wubLetAppSubBCmdData)){
        wubLetAppSubBCmdSize = sizeof(wubLetAppSubBCmdData);
        wvdGen_MemCpy(&wubLetAppSubBCmdData[LETAPP_NUM_0], aubDataBuff_p, sizeof(wubLetAppSubBCmdData));
    }
    else {
        wubLetAppSubBCmdSize = aubSize;
        wvdGen_MemCpy(&wubLetAppSubBCmdData[LETAPP_NUM_0], aubDataBuff_p, aubSize);
    }
}
/*********************************************************************************************
*  function                 : nubLetSubBpEvPidChg
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
static Type_uByte nubLetSubBpEvPidChg(void)
{
    /* Auto variable Define */
    Type_uByte aubProcPID;
    Type_uByte aubTempNum;

    /* Initalize */
    aubProcPID = PID_SUBB_PROC_MAX;
    aubTempNum = LETAPP_NUM_0;

    /* Check Event Code */
    if ((OS_NULL != wstLetAppSubBMail_p) && (OS_NULL != wstLetAppSubBMail_p->vdQueueBuffer_p)){
        while((aubTempNum < PID_SUBB_PROC_MAX) && (nstLetSubBEvChg_a[aubTempNum].ubInputEvent != wstLetAppSubBMail_p->uhEventCode)){
            aubTempNum++;
        }
        aubProcPID = nstLetSubBEvChg_a[aubTempNum].ubProcessID;
    }
    else {
        /* No Action */
    }

    return aubProcPID;
}
/*********************************************************************************************
*  function                 : nubLetSubBAmModeOn
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
static Type_uByte nubLetSubBAmModeOn(void)
{
    Type_uByte aubReturn;
    Type_uByte aubResult;
    //LXQADD Type_uByte aubBand;

    aubResult = RES_NG;
    aubReturn = PE_SEC1;
    //LXQADD aubBand = TUCRL_MODE_BAND_MW1;

    //LXQADD aubResult = wubRadioCtrl_Interface(EVT_TUNERCTRL_REQ_LET_DIAG_MODE_ON, &aubBand, LETAPP_NUM_1);

    if (RES_OK == aubResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Actioon */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubBAmModeOnCB
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
static Type_uByte nubLetSubBAmModeOnCB(void)
{
    Type_uByte aubReturn;

    aubReturn = PE_SEC1;

    if (LET_SUBB_EV_AM_MODE_ON_RES == wstLetAppSubBMail_p->uhEventCode){
        aubReturn = PE_SEC0;
    }
    else if (LET_SUBB_EV_TIMEOUT == wstLetAppSubBMail_p->uhEventCode){
        aubReturn = PE_SEC1;
    }
    else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubBAmStartDef
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
static Type_uByte nubLetSubBAmStartDef(void)
{

    /* Debug Message */

    return PE_SEC0;
}
/*********************************************************************************************
*  function                 : nubLetSubBAmStartOk
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
static Type_uByte nubLetSubBAmStartOk(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_SUCCESS;
    astLetRes.ubResSize = LETAPP_NUM_0;
    astLetRes.ubSeriveId = wubLetAppSubBCmdData[LETAPP_NUM_0];
    astLetRes.ubResData[LETAPP_NUM_0] = wubLetAppSubBCmdData[LETAPP_NUM_1];
    astLetRes.ubResData[LETAPP_NUM_1] = wubLetAppSubBCmdData[LETAPP_NUM_2];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBB,LETAPP_RES_AM_ANT_START, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubBAmStartNg
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
static Type_uByte nubLetSubBAmStartNg(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_FAILURE;
    astLetRes.ubResData[LETAPP_NUM_0] = KLINEDRV_RES_REJECT;
    astLetRes.ubResSize = LETAPP_NUM_1;
    astLetRes.ubSeriveId = wubLetAppSubBCmdData[LETAPP_NUM_0];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBB,LETAPP_RES_AM_ANT_START, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubBAmModeOff
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
static Type_uByte nubLetSubBAmModeOff(void)
{
    Type_uByte aubReturn;
    Type_uByte aubResult;
    //LXQADD Type_uByte aubBand;

    aubResult = RES_NG;
    aubReturn = PE_SEC1;
    //LXQADD aubBand = TUCRL_MODE_BAND_MW1;

    //LXQADD aubResult = wubRadioCtrl_Interface(EVT_TUNERCTRL_REQ_LET_DIAG_MODE_OFF, &aubBand, LETAPP_NUM_1);

    if (RES_OK == aubResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubBAmModeOffCB
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
static Type_uByte nubLetSubBAmModeOffCB(void)
{
    Type_uByte aubReturn;
    aubReturn = PE_SEC1;

    if (LET_SUBB_EV_AM_MODE_OFF_RES == wstLetAppSubBMail_p->uhEventCode){
        aubReturn = PE_SEC0;
    }
    else if (LET_SUBB_EV_TIMEOUT == wstLetAppSubBMail_p->uhEventCode){
        aubReturn = PE_SEC1;
    }
    else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubBAmEndDef
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
static Type_uByte nubLetSubBAmEndDef(void)
{

    /* Debug Message */

    return PE_SEC0;
}
/*********************************************************************************************
*  function                 : nubLetSubBAmEndOk
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
static Type_uByte nubLetSubBAmEndOk(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_SUCCESS;
    astLetRes.ubResSize = LETAPP_NUM_0;
    astLetRes.ubSeriveId = wubLetAppSubBCmdData[LETAPP_NUM_0];
    astLetRes.ubResData[LETAPP_NUM_0] = wubLetAppSubBCmdData[LETAPP_NUM_1];
    astLetRes.ubResData[LETAPP_NUM_1] = wubLetAppSubBCmdData[LETAPP_NUM_2];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBB,LETAPP_RES_AM_ANT_END, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubBAmEndNg
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
static Type_uByte nubLetSubBAmEndNg(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_FAILURE;
    astLetRes.ubResData[LETAPP_NUM_0] = KLINEDRV_RES_REJECT;
    astLetRes.ubResSize = LETAPP_NUM_1;
    astLetRes.ubSeriveId = wubLetAppSubBCmdData[LETAPP_NUM_0];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBB,LETAPP_RES_AM_ANT_END, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubBAmAntChk
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
static Type_uByte nubLetSubBAmAntChk(void)
{
    Type_uHWord auhFreqData;
    Type_uByte aubStepData;
    Type_uByte aubReturn;

    auhFreqData = LETAPP_NUM_0;
    aubStepData = LETAPP_NUM_0;
    aubReturn = PE_SEC1;

    if ((wstLetAppSubBMail_p->vdQueueBuffer_p != OS_NULL) && (LETAPP_NUM_64 >=wstLetAppSubBMail_p->uwQueueSize)){
        wvdGen_MemCpy(&wubLetSubBTempBuff[LETAPP_NUM_0], wstLetAppSubBMail_p->vdQueueBuffer_p, wstLetAppSubBMail_p->uwQueueSize);
        if (LETAPP_ASCII_DOT == wubLetSubBTempBuff[LETAPP_NUM_7]){
            /* AM Ant Translate To Freq Step */
            auhFreqData = ((wubLetSubBTempBuff[LETAPP_NUM_3] - LETAPP_ASCII_NUM_0) * LETAPP_NUM_1000);
            auhFreqData += ((wubLetSubBTempBuff[LETAPP_NUM_4] - LETAPP_ASCII_NUM_0) * LETAPP_NUM_100);
            auhFreqData += ((wubLetSubBTempBuff[LETAPP_NUM_5] - LETAPP_ASCII_NUM_0) * LETAPP_NUM_10);
            auhFreqData += (wubLetSubBTempBuff[LETAPP_NUM_6] - LETAPP_ASCII_NUM_0);

            if ((auhFreqData >= LET_SUBB_AM_FREQ_MIN) && (auhFreqData <= LET_SUBB_AM_FREQ_MAX)){
                aubStepData = (auhFreqData - LET_SUBB_AM_FREQ_MIN) % LETAPP_NUM_9;
                if (LETAPP_NUM_0 == aubStepData){ /* AM Freq Step is OK */
                    wubLetSubBTempBuff[LETAPP_NUM_40] = (auhFreqData - LET_SUBB_AM_FREQ_MIN) / LETAPP_NUM_9;
                    aubReturn = PE_SEC0;
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
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubBAmAntSet
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
static Type_uByte nubLetSubBAmAntSet(void)
{
    Type_uByte aubReturn;
    Type_uByte aubResult;

    aubResult = RES_NG;
    aubReturn = PE_SEC1;

    //LXQADD aubResult = wubRadioCtrl_Interface(EVT_TUNERCTRL_REQ_TUNE_DIRECT, &wubLetSubBTempBuff[LETAPP_NUM_40], LETAPP_NUM_1);

    if (RES_OK == aubResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubBAmAntSetCB
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
static Type_uByte nubLetSubBAmAntSetCB(void)
{
    Type_uByte aubReturn;
    aubReturn = PE_SEC1;

    if (LET_SUBB_EV_FREQ_SET_RES == wstLetAppSubBMail_p->uhEventCode){
        aubReturn = PE_SEC0;
    }
    else if (LET_SUBB_EV_TIMEOUT == wstLetAppSubBMail_p->uhEventCode){
        aubReturn = PE_SEC1;
    }
    else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubBAmAntOk
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
static Type_uByte nubLetSubBAmAntOk(void)
{
    Type_uByte aubReturn;
    Type_uHWord auhEventCode;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    auhEventCode = LETAPP_EV_INVALID;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_SUCCESS;
    astLetRes.ubResSize = LETAPP_NUM_0;
    astLetRes.ubSeriveId = wubLetAppSubBCmdData[LETAPP_NUM_0];
    astLetRes.ubResData[LETAPP_NUM_0] = wubLetAppSubBCmdData[LETAPP_NUM_1];
    astLetRes.ubResData[LETAPP_NUM_1] = wubLetAppSubBCmdData[LETAPP_NUM_2];

    /* Response Check */
    switch (wubLetSubBTempBuff[LETAPP_NUM_2]){
        case LETAPP_NUM_2:
            auhEventCode = LETAPP_RES_AM_ANT1;
            break;
        case LETAPP_NUM_3:
            auhEventCode = LETAPP_RES_AM_ANT2;
            break;
        case LETAPP_NUM_4:
            auhEventCode = LETAPP_RES_AM_ANT3;
            break;
        default:
            /* No Action */
            break;
    }

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBB,auhEventCode, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubBAmAntNg
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
static Type_uByte nubLetSubBAmAntNg(void)
{
    Type_uByte aubReturn;
    Type_uHWord auhEventCode;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    auhEventCode = LETAPP_EV_INVALID;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    /* Response Check */
    switch (wubLetSubBTempBuff[LETAPP_NUM_2]){
        case LETAPP_NUM_2:
            auhEventCode = LETAPP_RES_AM_ANT1;
            break;
        case LETAPP_NUM_3:
            auhEventCode = LETAPP_RES_AM_ANT2;
            break;
        case LETAPP_NUM_4:
            auhEventCode = LETAPP_RES_AM_ANT3;
            break;
        default:
            /* No Action */
            break;
    }

    astLetRes.ubProcessRes = PROC_FAILURE;
    astLetRes.ubResData[LETAPP_NUM_0] = KLINEDRV_RES_REJECT;
    astLetRes.ubResSize = LETAPP_NUM_1;
    astLetRes.ubSeriveId = wubLetAppSubBCmdData[LETAPP_NUM_0];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBB,auhEventCode, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubBAmAntSmChk
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
static Type_uByte nubLetSubBAmAntSmChk(void)
{
    Type_uByte aubReturn;
    Type_uByte aubResult;

    aubResult = RES_NG;
    aubReturn = PE_SEC1;

    //LXQADD aubResult = wubRadioCtrl_Interface(EVT_TUNERCTRL_REQ_SMDATA_GET, OS_NULL, LETAPP_NUM_0);

    if (RES_OK == aubResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubBAmAntSmChkCB
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
static Type_uByte nubLetSubBAmAntSmChkCB(void)
{
    Type_uByte aubReturn;
    aubReturn = PE_SEC1;

    if (LET_SUBB_EV_SM_CHK_RES == wstLetAppSubBMail_p->uhEventCode){
        aubReturn = PE_SEC0;
    }
    else if (LET_SUBB_EV_TIMEOUT == wstLetAppSubBMail_p->uhEventCode){
        aubReturn = PE_SEC1;
    }
    else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubBAmSmEep
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
static Type_uByte nubLetSubBAmSmEep(void)
{
    Type_uByte aubSmData;
    Type_uHWord auhEepAdd;

    aubSmData = LETAPP_NUM_0;
    auhEepAdd = LET_EEP_ADD_AM_ANT1_REST;

    aubSmData = *(Type_uByte*)(wstLetAppSubBMail_p->vdQueueBuffer_p);

    if (aubSmData >= LETAPP_NUM_16){ /* If Signal Strong littler then 16, Signal is 0 */
        aubSmData -= LETAPP_NUM_16;
    }
    else {
        aubSmData = LETAPP_NUM_0;
    }

    /* Signal Strong Translate To ASCII */
    wubLetSubBTempBuff[LETAPP_NUM_40] = (aubSmData / LETAPP_NUM_2) / LETAPP_NUM_100 + LETAPP_ASCII_NUM_0;
    wubLetSubBTempBuff[LETAPP_NUM_41] = ((aubSmData / LETAPP_NUM_2) % LETAPP_NUM_100) / LETAPP_NUM_10 + LETAPP_ASCII_NUM_0;
    wubLetSubBTempBuff[LETAPP_NUM_42] = ((aubSmData / LETAPP_NUM_2) % LETAPP_NUM_10) + LETAPP_ASCII_NUM_0;
    wubLetSubBTempBuff[LETAPP_NUM_43] = LETAPP_ASCII_DOT;
    wubLetSubBTempBuff[LETAPP_NUM_44] = ((aubSmData % LETAPP_NUM_2) * LETAPP_NUM_5) + LETAPP_ASCII_NUM_0;

    /* Response Check */
    switch (wubLetSubBTempBuff[LETAPP_NUM_2]){
        case LETAPP_NUM_2:
           auhEepAdd = LET_EEP_ADD_AM_ANT1_REST;
            break;
        case LETAPP_NUM_3:
           auhEepAdd = LET_EEP_ADD_AM_ANT2_REST;
            break;
        case LETAPP_NUM_4:
           auhEepAdd = LET_EEP_ADD_AM_ANT3_REST;
            break;
        default:
            /* No Action */
            break;
    }
    //LXQADD (void)wubEEP_BlockWrite(auhEepAdd, LETAPP_NUM_5, &wubLetSubBTempBuff[LETAPP_NUM_40], EEP_FORCE_WRITE);
    //LXQADD (void)wubEEP_BlockWrite(LET_EEP_ADD_TUNER_REST, LETAPP_NUM_5, &wubLetSubBTempBuff[LETAPP_NUM_40], EEP_FORCE_WRITE);
	if(auhEepAdd){;}
    return PE_SEC0;
}
/*********************************************************************************************
*  function                 : nubLetSubBAmAntEndOk
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
static Type_uByte nubLetSubBAmAntEndOk(void)
{
    Type_uByte aubReturn;
    Type_uHWord auhEventCode;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    auhEventCode = LETAPP_EV_INVALID;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_SUCCESS;
    astLetRes.ubResSize = LETAPP_NUM_0;
    astLetRes.ubSeriveId = wubLetAppSubBCmdData[LETAPP_NUM_0];
    astLetRes.ubResData[LETAPP_NUM_0] = wubLetAppSubBCmdData[LETAPP_NUM_1];
    astLetRes.ubResData[LETAPP_NUM_1] = wubLetAppSubBCmdData[LETAPP_NUM_2];

    /* Response Check */
    switch (wubLetSubBTempBuff[LETAPP_NUM_2]){
        case LETAPP_NUM_2:
            auhEventCode = LETAPP_RES_AM_ANT1_END;
            break;
        case LETAPP_NUM_3:
            auhEventCode = LETAPP_RES_AM_ANT2_END;
            break;
        case LETAPP_NUM_4:
            auhEventCode = LETAPP_RES_AM_ANT3_END;
            break;
        default:
            /* No Action */
            break;
    }

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBB,auhEventCode, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubBAmAntEndNg
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
static Type_uByte nubLetSubBAmAntEndNg(void)
{
    Type_uByte aubReturn;
    Type_uHWord auhEventCode;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    auhEventCode = LETAPP_EV_INVALID;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    /* Response Check */
    switch (wubLetSubBTempBuff[LETAPP_NUM_2]){
        case LETAPP_NUM_2:
            auhEventCode = LETAPP_RES_AM_ANT1_END;
            break;
        case LETAPP_NUM_3:
            auhEventCode = LETAPP_RES_AM_ANT2_END;
            break;
        case LETAPP_NUM_4:
            auhEventCode = LETAPP_RES_AM_ANT3_END;
            break;
        default:
            /* No Action */
            break;
    }

    astLetRes.ubProcessRes = PROC_FAILURE;
    astLetRes.ubResData[LETAPP_NUM_0] = KLINEDRV_RES_REJECT;
    astLetRes.ubResSize = LETAPP_NUM_1;
    astLetRes.ubSeriveId = wubLetAppSubBCmdData[LETAPP_NUM_0];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBB,auhEventCode, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubBFmModeOn
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
static Type_uByte nubLetSubBFmModeOn(void)
{
    Type_uByte aubReturn;
    Type_uByte aubResult;
    //LXQADD Type_uByte aubBand;

    aubResult = RES_NG;
    aubReturn = PE_SEC1;
    //LXQADD aubBand = TUCRL_MODE_BAND_FM1;

    //LXQADD aubResult = wubRadioCtrl_Interface(EVT_TUNERCTRL_REQ_LET_DIAG_MODE_ON, &aubBand, LETAPP_NUM_1);

    if (RES_OK == aubResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubBFmModeOnCB
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
static Type_uByte nubLetSubBFmModeOnCB(void)
{
    Type_uByte aubReturn;

    aubReturn = PE_SEC1;

    if (LET_SUBB_EV_FM_MODE_ON_RES == wstLetAppSubBMail_p->uhEventCode){
        aubReturn = PE_SEC0;
    }
    else if (LET_SUBB_EV_TIMEOUT == wstLetAppSubBMail_p->uhEventCode){
        aubReturn = PE_SEC1;
    }
    else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubBFmStartDef
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
static Type_uByte nubLetSubBFmStartDef(void)
{

    /* Debug Message */

    return PE_SEC0;
}
/*********************************************************************************************
*  function                 : nubLetSubBFmStartOk
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
static Type_uByte nubLetSubBFmStartOk(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_SUCCESS;
    astLetRes.ubResSize = LETAPP_NUM_0;
    astLetRes.ubSeriveId = wubLetAppSubBCmdData[LETAPP_NUM_0];
    astLetRes.ubResData[LETAPP_NUM_0] = wubLetAppSubBCmdData[LETAPP_NUM_1];
    astLetRes.ubResData[LETAPP_NUM_1] = wubLetAppSubBCmdData[LETAPP_NUM_2];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBB,LETAPP_RES_FM_ANT_START, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubBFmStartNg
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
static Type_uByte nubLetSubBFmStartNg(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_FAILURE;
    astLetRes.ubResData[LETAPP_NUM_0] = KLINEDRV_RES_REJECT;
    astLetRes.ubResSize = LETAPP_NUM_1;
    astLetRes.ubSeriveId = wubLetAppSubBCmdData[LETAPP_NUM_0];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBB,LETAPP_RES_FM_ANT_START, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubBFmModeOff
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
static Type_uByte nubLetSubBFmModeOff(void)
{
    Type_uByte aubReturn;
    Type_uByte aubResult;
    //LXQADD Type_uByte aubBand;

    aubResult = RES_NG;
    aubReturn = PE_SEC1;
    //LXQADD aubBand = TUCRL_MODE_BAND_FM1;

    //LXQADD aubResult = wubRadioCtrl_Interface(EVT_TUNERCTRL_REQ_LET_DIAG_MODE_OFF, &aubBand, LETAPP_NUM_1);

    if (RES_OK == aubResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubBFmModeOffCB
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
static Type_uByte nubLetSubBFmModeOffCB(void)
{
    Type_uByte aubReturn;
    aubReturn = PE_SEC1;

    if (LET_SUBB_EV_FM_MODE_OFF_RES == wstLetAppSubBMail_p->uhEventCode){
        aubReturn = PE_SEC0;
    }
    else if (LET_SUBB_EV_TIMEOUT == wstLetAppSubBMail_p->uhEventCode){
        aubReturn = PE_SEC1;
    }
    else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubBFmEndDef
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
static Type_uByte nubLetSubBFmEndDef(void)
{

    /* Debug Message */

    return PE_SEC0;
}
/*********************************************************************************************
*  function                 : nubLetSubBFmEndOk
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
static Type_uByte nubLetSubBFmEndOk(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_SUCCESS;
    astLetRes.ubResSize = LETAPP_NUM_0;
    astLetRes.ubSeriveId = wubLetAppSubBCmdData[LETAPP_NUM_0];
    astLetRes.ubResData[LETAPP_NUM_0] = wubLetAppSubBCmdData[LETAPP_NUM_1];
    astLetRes.ubResData[LETAPP_NUM_1] = wubLetAppSubBCmdData[LETAPP_NUM_2];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBB,LETAPP_RES_FM_ANT_END, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubBFmEndNg
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
static Type_uByte nubLetSubBFmEndNg(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_FAILURE;
    astLetRes.ubResData[LETAPP_NUM_0] = KLINEDRV_RES_REJECT;
    astLetRes.ubResSize = LETAPP_NUM_1;
    astLetRes.ubSeriveId = wubLetAppSubBCmdData[LETAPP_NUM_0];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBB,LETAPP_RES_FM_ANT_END, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubBFmAntChk
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
static Type_uByte nubLetSubBFmAntChk(void)
{
    Type_uHWord auhFreqData;
    Type_uByte aubStepData;
    Type_uByte aubReturn;

    auhFreqData = LETAPP_NUM_0;
    aubStepData = LETAPP_NUM_0;
    aubReturn = PE_SEC1;

    if ((wstLetAppSubBMail_p->vdQueueBuffer_p != OS_NULL) && (LETAPP_NUM_64 >=wstLetAppSubBMail_p->uwQueueSize)){
        wvdGen_MemCpy(&wubLetSubBTempBuff[LETAPP_NUM_0], wstLetAppSubBMail_p->vdQueueBuffer_p, wstLetAppSubBMail_p->uwQueueSize);
        /* FM Ant Translate To Freq Step */
        if ((LETAPP_ASCII_NUM_0 == wubLetSubBTempBuff[LETAPP_NUM_3]) && (LETAPP_ASCII_DOT == wubLetSubBTempBuff[LETAPP_NUM_7])){
            auhFreqData += ((wubLetSubBTempBuff[LETAPP_NUM_4] - LETAPP_ASCII_NUM_0) * LETAPP_NUM_1000);
            auhFreqData += ((wubLetSubBTempBuff[LETAPP_NUM_5] - LETAPP_ASCII_NUM_0) * LETAPP_NUM_100);
            auhFreqData += ((wubLetSubBTempBuff[LETAPP_NUM_6] - LETAPP_ASCII_NUM_0) * LETAPP_NUM_10);
            auhFreqData += (wubLetSubBTempBuff[LETAPP_NUM_8] - LETAPP_ASCII_NUM_0);
            /* FM Freq Step Check */
            if ((auhFreqData >= LET_SUBB_FM_FREQ_MIN) && (auhFreqData <= LET_SUBB_FM_FREQ_MAX)){
                aubStepData =auhFreqData - LET_SUBB_FM_FREQ_MIN;
                wubLetSubBTempBuff[LETAPP_NUM_40] = aubStepData;
                aubReturn = PE_SEC0;
            }
            else {
                /* No Action */
            }
        }
        else{
            /* No Action */
        }
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubBFmAntSet
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
static Type_uByte nubLetSubBFmAntSet(void)
{
    Type_uByte aubReturn;
    Type_uByte aubResult;

    aubResult = RES_NG;
    aubReturn = PE_SEC1;

    //LXQADD aubResult = wubRadioCtrl_Interface(EVT_TUNERCTRL_REQ_TUNE_DIRECT, &wubLetSubBTempBuff[LETAPP_NUM_40], LETAPP_NUM_1);

    if (RES_OK == aubResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubBFmAntSetCB
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
static Type_uByte nubLetSubBFmAntSetCB(void)
{
    Type_uByte aubReturn;
    aubReturn = PE_SEC1;

    if (LET_SUBB_EV_FREQ_SET_RES == wstLetAppSubBMail_p->uhEventCode){
        aubReturn = PE_SEC0;
    }
    else if (LET_SUBB_EV_TIMEOUT == wstLetAppSubBMail_p->uhEventCode){
        aubReturn = PE_SEC1;
    }
    else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubBFmAntOk
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
static Type_uByte nubLetSubBFmAntOk(void)
{
    Type_uByte aubReturn;
    Type_uHWord auhEventCode;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    auhEventCode = LETAPP_EV_INVALID;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_SUCCESS;
    astLetRes.ubResSize = LETAPP_NUM_0;
    astLetRes.ubSeriveId = wubLetAppSubBCmdData[LETAPP_NUM_0];
    astLetRes.ubResData[LETAPP_NUM_0] = wubLetAppSubBCmdData[LETAPP_NUM_1];
    astLetRes.ubResData[LETAPP_NUM_1] = wubLetAppSubBCmdData[LETAPP_NUM_2];

    /* Response Check */
    switch (wubLetSubBTempBuff[LETAPP_NUM_2]){
        case LETAPP_NUM_2:
            auhEventCode = LETAPP_RES_FM_ANT1;
            break;
        case LETAPP_NUM_3:
            auhEventCode = LETAPP_RES_FM_ANT2;
            break;
        case LETAPP_NUM_4:
            auhEventCode = LETAPP_RES_FM_ANT3;
            break;
        default:
            /* No Action */
            break;
    }

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBB,auhEventCode, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubBFmAntNg
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
static Type_uByte nubLetSubBFmAntNg(void)
{
    Type_uByte aubReturn;
    Type_uHWord auhEventCode;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    auhEventCode = LETAPP_EV_INVALID;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    /* Response Check */
    switch (wubLetSubBTempBuff[LETAPP_NUM_2]){
        case LETAPP_NUM_2:
            auhEventCode = LETAPP_RES_FM_ANT1;
            break;
        case LETAPP_NUM_3:
            auhEventCode = LETAPP_RES_FM_ANT2;
            break;
        case LETAPP_NUM_4:
            auhEventCode = LETAPP_RES_FM_ANT3;
            break;
        default:
            /* No Action */
            break;
    }

    astLetRes.ubProcessRes = PROC_FAILURE;
    astLetRes.ubResData[LETAPP_NUM_0] = KLINEDRV_RES_REJECT;
    astLetRes.ubResSize = LETAPP_NUM_1;
    astLetRes.ubSeriveId = wubLetAppSubBCmdData[LETAPP_NUM_0];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBB,auhEventCode, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubBFmAntSmChk
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
static Type_uByte nubLetSubBFmAntSmChk(void)
{
    Type_uByte aubReturn;
    Type_uByte aubResult;

    aubResult = RES_NG;
    aubReturn = PE_SEC1;

    //LXQADD aubResult = wubRadioCtrl_Interface(EVT_TUNERCTRL_REQ_SMDATA_GET, OS_NULL, LETAPP_NUM_0);

    if (RES_OK == aubResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubBFmAntSmChkCB
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
static Type_uByte nubLetSubBFmAntSmChkCB(void)
{
    Type_uByte aubReturn;
    aubReturn = PE_SEC1;

    if (LET_SUBB_EV_SM_CHK_RES == wstLetAppSubBMail_p->uhEventCode){
        aubReturn = PE_SEC0;
    }
    else if (LET_SUBB_EV_TIMEOUT == wstLetAppSubBMail_p->uhEventCode){
        aubReturn = PE_SEC1;
    }
    else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubBFmSmEep
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
static Type_uByte nubLetSubBFmSmEep(void)
{
    Type_uByte aubSmData;
    Type_uHWord auhEepAdd;

    aubSmData = LETAPP_NUM_0;
    auhEepAdd = LET_EEP_ADD_FM_ANT1_REST;

    aubSmData = *(Type_uByte*)(wstLetAppSubBMail_p->vdQueueBuffer_p);

    if (aubSmData >= LETAPP_NUM_16){ /* If Signal Strong littler then 16, Signal is 0 */
        aubSmData -= LETAPP_NUM_16;
    }
    else {
        aubSmData = LETAPP_NUM_0;
    }

    /* Signal Strong Translate To ASCII */
    wubLetSubBTempBuff[LETAPP_NUM_40] = (aubSmData / LETAPP_NUM_2) / LETAPP_NUM_100 + LETAPP_ASCII_NUM_0;
    wubLetSubBTempBuff[LETAPP_NUM_41] = ((aubSmData / LETAPP_NUM_2) % LETAPP_NUM_100) / LETAPP_NUM_10 + LETAPP_ASCII_NUM_0;
    wubLetSubBTempBuff[LETAPP_NUM_42] = ((aubSmData / LETAPP_NUM_2) % LETAPP_NUM_10) + LETAPP_ASCII_NUM_0;
    wubLetSubBTempBuff[LETAPP_NUM_43] = LETAPP_ASCII_DOT;
    wubLetSubBTempBuff[LETAPP_NUM_44] = ((aubSmData % LETAPP_NUM_2) * LETAPP_NUM_5) + LETAPP_ASCII_NUM_0;

    /* Response Check */
    switch (wubLetSubBTempBuff[LETAPP_NUM_2]){
        case LETAPP_NUM_2:
           auhEepAdd = LET_EEP_ADD_FM_ANT1_REST;
            break;
        case LETAPP_NUM_3:
           auhEepAdd = LET_EEP_ADD_FM_ANT2_REST;
            break;
        case LETAPP_NUM_4:
           auhEepAdd = LET_EEP_ADD_FM_ANT3_REST;
            break;
        default:
            /* No Action */
            break;
    }
//for error will delete
if(auhEepAdd){;}
    //LXQADD (void)wubEEP_BlockWrite(auhEepAdd, LETAPP_NUM_5, &wubLetSubBTempBuff[LETAPP_NUM_40], EEP_FORCE_WRITE);
    //LXQADD (void)wubEEP_BlockWrite(LET_EEP_ADD_TUNER_REST, LETAPP_NUM_5, &wubLetSubBTempBuff[LETAPP_NUM_40], EEP_FORCE_WRITE);

    return PE_SEC0;
}
/*********************************************************************************************
*  function                 : nubLetSubBFmAntEndOk
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
static Type_uByte nubLetSubBFmAntEndOk(void)
{
    Type_uByte aubReturn;
    Type_uHWord auhEventCode;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    auhEventCode = LETAPP_EV_INVALID;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_SUCCESS;
    astLetRes.ubResSize = LETAPP_NUM_0;
    astLetRes.ubSeriveId = wubLetAppSubBCmdData[LETAPP_NUM_0];
    astLetRes.ubResData[LETAPP_NUM_0] = wubLetAppSubBCmdData[LETAPP_NUM_1];
    astLetRes.ubResData[LETAPP_NUM_1] = wubLetAppSubBCmdData[LETAPP_NUM_2];

    /* Response Check */
    switch (wubLetSubBTempBuff[LETAPP_NUM_2]){
        case LETAPP_NUM_2:
            auhEventCode = LETAPP_RES_FM_ANT1_END;
            break;
        case LETAPP_NUM_3:
            auhEventCode = LETAPP_RES_FM_ANT2_END;
            break;
        case LETAPP_NUM_4:
            auhEventCode = LETAPP_RES_FM_ANT3_END;
            break;
        default:
            /* No Action */
            break;
    }

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBB,auhEventCode, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubBFmAntEndNg
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
static Type_uByte nubLetSubBFmAntEndNg(void)
{
    Type_uByte aubReturn;
    Type_uHWord auhEventCode;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    auhEventCode = LETAPP_EV_INVALID;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    /* Response Check */
    switch (wubLetSubBTempBuff[LETAPP_NUM_2]){
        case LETAPP_NUM_2:
            auhEventCode = LETAPP_RES_FM_ANT1_END;
            break;
        case LETAPP_NUM_3:
            auhEventCode = LETAPP_RES_FM_ANT2_END;
            break;
        case LETAPP_NUM_4:
            auhEventCode = LETAPP_RES_FM_ANT3_END;
            break;
        default:
            /* No Action */
            break;
    }

    astLetRes.ubProcessRes = PROC_FAILURE;
    astLetRes.ubResData[LETAPP_NUM_0] = KLINEDRV_RES_REJECT;
    astLetRes.ubResSize = LETAPP_NUM_1;
    astLetRes.ubSeriveId = wubLetAppSubBCmdData[LETAPP_NUM_0];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBB,auhEventCode, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubBTunerStart
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
static Type_uByte nubLetSubBTunerStart(void)
{

    /* Debug Message */

    return PE_SEC0;
}
/*********************************************************************************************
*  function                 : nubLetSubBTunerStartOk
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
static Type_uByte nubLetSubBTunerStartOk(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_SUCCESS;
    astLetRes.ubResSize = LETAPP_NUM_0;
    astLetRes.ubSeriveId = wubLetAppSubBCmdData[LETAPP_NUM_0];
    astLetRes.ubResData[LETAPP_NUM_0] = wubLetAppSubBCmdData[LETAPP_NUM_1];
    astLetRes.ubResData[LETAPP_NUM_1] = wubLetAppSubBCmdData[LETAPP_NUM_2];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBB,LETAPP_RES_TUNER_TEST_START, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubBTunerStartNg
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
static Type_uByte nubLetSubBTunerStartNg(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_FAILURE;
    astLetRes.ubResData[LETAPP_NUM_0] = KLINEDRV_RES_REJECT;
    astLetRes.ubResSize = LETAPP_NUM_1;
    astLetRes.ubSeriveId = wubLetAppSubBCmdData[LETAPP_NUM_0];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBB,LETAPP_RES_TUNER_TEST_START, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubBTunerFreqChk
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
static Type_uByte nubLetSubBTunerFreqChk(void)
{
    Type_uByte aubChkData;
    Type_uByte aubReturn;

    aubChkData = LETAPP_NUM_0;
    aubReturn = PE_SEC1;

    if ((wstLetAppSubBMail_p->vdQueueBuffer_p != OS_NULL) && (LETAPP_NUM_64 >=wstLetAppSubBMail_p->uwQueueSize)){
        wvdGen_MemCpy(&wubLetSubBTempBuff[LETAPP_NUM_0], wstLetAppSubBMail_p->vdQueueBuffer_p, wstLetAppSubBMail_p->uwQueueSize);
        if (LETAPP_ASCII_DOT == wubLetSubBTempBuff[LETAPP_NUM_7]){ /* FM Or Am Check */
            aubChkData = ((wubLetSubBTempBuff[LETAPP_NUM_3] - LETAPP_ASCII_NUM_0) * LETAPP_NUM_10);
            aubChkData += (wubLetSubBTempBuff[LETAPP_NUM_4] - LETAPP_ASCII_NUM_0);

            if (aubChkData <= LETAPP_NUM_1){
                aubReturn = PE_SEC0;
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

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubBTunerSetOk
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
static Type_uByte nubLetSubBTunerSetOk(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_SUCCESS;
    astLetRes.ubResSize = LETAPP_NUM_0;
    astLetRes.ubSeriveId = wubLetAppSubBCmdData[LETAPP_NUM_0];
    astLetRes.ubResData[LETAPP_NUM_0] = wubLetAppSubBCmdData[LETAPP_NUM_1];
    astLetRes.ubResData[LETAPP_NUM_1] = wubLetAppSubBCmdData[LETAPP_NUM_2];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBB,LETAPP_RES_SET_TUNER_TEST, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubBTunerSetNg
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
static Type_uByte nubLetSubBTunerSetNg(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_FAILURE;
    astLetRes.ubResData[LETAPP_NUM_0] = KLINEDRV_RES_REJECT;
    astLetRes.ubResSize = LETAPP_NUM_1;
    astLetRes.ubSeriveId = wubLetAppSubBCmdData[LETAPP_NUM_0];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBB,LETAPP_RES_SET_TUNER_TEST, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubBTunerEnd
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
static Type_uByte nubLetSubBTunerEnd(void)
{

    /* Debug Message */


    return PE_SEC0;
}
/*********************************************************************************************
*  function                 : nubLetSubBTunerEndOk
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
static Type_uByte nubLetSubBTunerEndOk(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_SUCCESS;
    astLetRes.ubResSize = LETAPP_NUM_0;
    astLetRes.ubSeriveId = wubLetAppSubBCmdData[LETAPP_NUM_0];
    astLetRes.ubResData[LETAPP_NUM_0] = wubLetAppSubBCmdData[LETAPP_NUM_1];
    astLetRes.ubResData[LETAPP_NUM_1] = wubLetAppSubBCmdData[LETAPP_NUM_2];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBB,LETAPP_RES_TUNER_TEST_END, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubBTunerEndNg
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
static Type_uByte nubLetSubBTunerEndNg(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_FAILURE;
    astLetRes.ubResData[LETAPP_NUM_0] = KLINEDRV_RES_REJECT;
    astLetRes.ubResSize = LETAPP_NUM_1;
    astLetRes.ubSeriveId = wubLetAppSubBCmdData[LETAPP_NUM_0];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBB,LETAPP_RES_TUNER_TEST_END, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}

/* End of File */
