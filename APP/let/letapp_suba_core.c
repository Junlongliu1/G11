/**********************************************************************************************
*  file name                : letapp_suba_core.c
*  date                     : 2016.06.01
*  autho                    : zhangjinzhu
*  model                    : TBTA
*  description              : LetApp SubA related file
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
#include "letapp_suba_core.h"
#include "letapp_core.h"
/* Other Components */
//#include "dserial.h"
#include "klinedrv.h"
//#include "eepromcontrol.h"
//#include "rpcapp_event.h"
//#include "bcan.h"
//#include "soundstm_if_fnc.h"
//#include "sndapp_common.h"
//#include "soundapp.h"
#include "model.h"
#include "rpc_event.h"
#include "rpc_data.h"
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
#define WTVALUE_FEVR            (-1)           /* Timer Value(time forever)    */

/* LET Speaker Connect Info */
#define LET_SPK_CONNECT_INFO       (0x2DU)        /* FR-L/FR-R/RR-R/RR-L */
#define LET_SPK_CONNECT_FR_L       (0x01U)
#define LET_SPK_CONNECT_FR_R       (0x04U)
#define LET_SPK_CONNECT_RR_R       (0x08U)
#define LET_SPK_CONNECT_RR_L       (0x20U)

/* LET Speaker Target */
#define LET_SPK_TARGET_FR_L_R      (0x80U)
#define LET_SPK_TARGET_FRTW_L_R    (0x40U)
#define LET_SPK_TARGET_FR_CNT      (0x20U)
#define LET_SPK_TARGET_RR_L_R      (0x10U)
#define LET_SPK_TARGET_RR2_L_R     (0x08U)
#define LET_SPK_TARGET_RRTW_L_R    (0x04U)
#define LET_SPK_TARGET_RR_CNT      (0x02U)
#define LET_SPK_TARGET_SW          (0x01U)

/* LET Speaker Result Default */
#define LET_SPK_DEF_EEP_1      (0x0CU)
#define LET_SPK_DEF_EEP_2      (0xF3U)
#define LET_SPK_DEF_EEP_3      (0xFFU)
#define LET_SPK_DEF_EEP_4      (0xFFU)

/* LET SND Speaker Default */
#define SND_LET_SPEAKER_FL		(0x00U)
#define SND_LET_SPEAKER_FR		(0x01U)
#define SND_LET_SPEAKER_RL		(0x02U)
#define SND_LET_SPEAKER_RR		(0x03U)
#define SND_LET_SPEAKER_TW_FL	(0x04U)
#define SND_LET_SPEAKER_TW_FR	(0x05U)
#define SND_LET_SPEAKER_TW_RL	(0x06U)
#define SND_LET_SPEAKER_TW_RR	(0x07U)
#define SND_LET_SPEAKER_ALL     (0x08U)
#define SND_LET_MAX_DATA		SND_LET_SPEAKER_ALL

#define	BEEP_SINGEN_500HZ		(0x00U)
#define	BEEP_SINGEN_8000HZ		(0x01U)

/********************************************************************************************/
/*                           Enumeration Type Definition Section                            */
/********************************************************************************************/

/* LetAppSubA Process ID Define */
enum LETAPP_SUBA_PROC_ID{
/* 000 */ PID_SUBA_LET_START = 0U,
/* 001 */ PID_SUBA_LET_END,
/* 002 */ PID_SUBA_USB_TEST_START,
/* 003 */ PID_SUBA_USB_TEST_END,
/* 004 */ PID_SUBA_USB_SPK_START,
/* 005 */ PID_SUBA_USB_SPK_END,
/* 006 */ PID_SUBA_BIBIRI_START,
/* 007 */ PID_SUBA_BIBIRI_SET,
/* 008 */ PID_SUBA_BIBIRI_END,
/* 009 */ PID_SUBA_BSM_INITIAL,
/* 010 */ PID_SUBA_BSM_START_TP_SET,
/* 011 */ PID_SUBA_BSM_END,
/* 012 */ PID_SUBA_BSM_REST,
/* 013 */ PID_SUBA_SPK_BEEP_START,
/* 014 */ PID_SUBA_SPK_BEEP_CH_UP,
/* 015 */ PID_SUBA_SPK_BEEP_END,
/* 016 */ PID_SUBA_SPK_BEEP_BUTTON,
/* 017 */ PID_SUBA_MEM_CLEAR_START,
/* 018 */ PID_SUBA_LET_MEM_CLEAR,
/* 019 */ PID_SUBA_MEM_INIT_CLEAR,
/* 020 */ PID_SUBA_MEM_INLINE_CLEAR,
/* 021 */ PID_SUBA_MEM_CLEAR_END,
/* 022 */ PID_SUBA_SND_SET_BACK,
/* 023 */ PID_SUBA_SND_VOLUME_SET,
/* 024 */ PID_SUBA_SND_BASS_SET,
/* 025 */ PID_SUBA_SND_TREBLE_SET,
/* 026 */ PID_SUBA_ERR_MEM_CLEAR,
/* *** */ PID_SUBA_PROC_MAX
};

/* Sub Process ID Define */
enum LETAPP_SUBA_SUBPROC_ID{
/* 000 */ SPID_SUBA_LET_START = 0U,
/* 001 */ SPID_SUBA_LET_START_CB,
/* 002 */ SPID_SUBA_LET_START_DEF,
/* 003 */ SPID_SUBA_LET_START_OK,
/* 004 */ SPID_SUBA_LET_START_NG,
/* 005 */ SPID_SUBA_LET_VSP_GET,
/* 006 */ SPID_SUBA_LET_VSP_GET_CB,
/* 007 */ SPID_SUBA_LET_VSP_EEP,
/* 008 */ SPID_SUBA_LET_END,
/* 009 */ SPID_SUBA_LET_END_CB,
/* 010 */ SPID_SUBA_LET_END_DEF,
/* 011 */ SPID_SUBA_LET_END_OK,
/* 012 */ SPID_SUBA_LET_END_NG,
/* 013 */ SPID_SUBA_USB_SRC_ON,
/* 014 */ SPID_SUBA_USB_SRC_ON_CB,
/* 015 */ SPID_SUBA_USB_START_DEF,
/* 016 */ SPID_SUBA_USB_START_OK,
/* 017 */ SPID_SUBA_USB_START_NG,
/* 018 */ SPID_SUBA_USB_SRC_OFF,
/* 019 */ SPID_SUBA_USB_SRC_OFF_CB,
/* 020 */ SPID_SUBA_USB_END_DEF,
/* 021 */ SPID_SUBA_USB_END_OK,
/* 022 */ SPID_SUBA_USB_END_NG,
/* 023 */ SPID_SUBA_SPK_INFO_JUDGE,
/* 024 */ SPID_SUBA_SPK_START_DEF,
/* 025 */ SPID_SUBA_SPK_START_OK,
/* 026 */ SPID_SUBA_SPK_START_NG,
/* 027 */ SPID_SUBA_SPK_END_JUDGE,
/* 028 */ SPID_SUBA_SPK_SND_SET,
/* 029 */ SPID_SUBA_SPK_SND_SET_CB,
/* 030 */ SPID_SUBA_USB_PLAY,
/* 031 */ SPID_SUBA_USB_PLAY_CB,
/* 032 */ SPID_SUBA_SPK_DISP,
/* 033 */ SPID_SUBA_SPK_RESULT_WAIT,
/* 034 */ SPID_SUBA_SPK_RESULT_DISP,
/* 035 */ SPID_SUBA_LET_BLANK_DISP,
/* 036 */ SPID_SUBA_USB_PLAY_STOP,
/* 037 */ SPID_SUBA_USB_PLAY_STOP_CB,
/* 038 */ SPID_SUBA_SPK_CHG_NEXT,
/* 039 */ SPID_SUBA_SPK_CHG_BACK,
/* 040 */ SPID_SUBA_SPK_RESULT_EEP,
/* 041 */ SPID_SUBA_SPK_END_OK,
/* 042 */ SPID_SUBA_SPK_NOCHK_EEP,
/* 043 */ SPID_SUBA_SPK_NOCHK_DISP,
/* 044 */ SPID_SUBA_SPK_END_NG,
/* 045 */ SPID_SUBA_BIBIRI_PLAY,
/* 046 */ SPID_SUBA_BIBIRI_PLAY_CB,
/* 047 */ SPID_SUBA_BIBIRI_SND_SET,
/* 048 */ SPID_SUBA_BIBIRI_SND_SET_CB,
/* 049 */ SPID_SUBA_BIBIRI_DEF,
/* 050 */ SPID_SUBA_BIBIRI_START_OK,
/* 051 */ SPID_SUBA_BIBIRI_START_NG,
/* 052 */ SPID_SUBA_USB_SONG_SET,
/* 053 */ SPID_SUBA_BIBIRI_SET_OK,
/* 054 */ SPID_SUBA_BIBIRI_SET_NG,
/* 055 */ SPID_SUBA_BIBIRI_DISP,
/* 056 */ SPID_SUBA_BIBIRI_RESULT_WAIT,
/* 057 */ SPID_SUBA_BIBIRI_RESULT_DISP,
/* 058 */ SPID_SUBA_BIBIRI_RESULT_EEP,
/* 059 */ SPID_SUBA_BIBIRI_END_OK,
/* 060 */ SPID_SUBA_BIBIRI_NOCHK_EEP,
/* 061 */ SPID_SUBA_BIBIRI_NOCHK_DISP,
/* 062 */ SPID_SUBA_BIBIRI_END_NG,
/* 063 */ SPID_SUBA_BSM_INITIAL,
/* 064 */ SPID_SUBA_BSM_INITIAL_CB,
/* 065 */ SPID_SUBA_BSM_INITIAL_OK,
/* 066 */ SPID_SUBA_BSM_INITIAL_NG,
/* 067 */ SPID_SUBA_BSM_TP_SET,
/* 068 */ SPID_SUBA_BSM_TP_SET_CB,
/* 069 */ SPID_SUBA_BSM_TP_SET_OK,
/* 070 */ SPID_SUBA_BSM_TP_SET_NG,
/* 071 */ SPID_SUBA_BSM_END,
/* 072 */ SPID_SUBA_BSM_END_CB,
/* 073 */ SPID_SUBA_BSM_END_OK,
/* 074 */ SPID_SUBA_BSM_END_NG,
/* 075 */ SPID_SUBA_BSM_REST,
/* 076 */ SPID_SUBA_BSM_REST_CB,
/* 077 */ SPID_SUBA_BSM_REST_OK,
/* 078 */ SPID_SUBA_BSM_REST_NG,
/* 079 */ SPID_SUBA_BEEP_START_CHK,
/* 080 */ SPID_SUBA_BEEP_SRC_ON,
/* 081 */ SPID_SUBA_BEEP_SRC_ON_CB,
/* 082 */ SPID_SUBA_BEEP_DEF,
/* 083 */ SPID_SUBA_BEEP_START_OK,
/* 084 */ SPID_SUBA_BEEP_START_NG,
/* 085 */ SPID_SUBA_BEEP_END_JUDGE,
/* 086 */ SPID_SUBA_BEEP_CH_UP_OK,
/* 087 */ SPID_SUBA_BEEP_CH_UP_NG,
/* 088 */ SPID_SUBA_BEEP_SND_SET,
/* 089 */ SPID_SUBA_BEEP_SND_SET_CB,
/* 090 */ SPID_SUBA_BEEP_DISP,
/* 091 */ SPID_SUBA_BEEP_DISP_OK,
/* 092 */ SPID_SUBA_BEEP_BUTTON_CHK,
/* 093 */ SPID_SUBA_BEEP_RESULT_DISP,
/* 094 */ SPID_SUBA_BEEP_CHG_NEXT,
/* 095 */ SPID_SUBA_BEEP_CHG_BACK,
/* 096 */ SPID_SUBA_BEEP_RESULT_EEP,
/* 097 */ SPID_SUBA_BEEP_SRC_OFF,
/* 098 */ SPID_SUBA_BEEP_SRC_OFF_CB,
/* 099 */ SPID_SUBA_BEEP_END_OK,
/* 100 */ SPID_SUBA_BEEP_NOCHK_EEP,
/* 101 */ SPID_SUBA_BEEP_NOCHK_DISP,
/* 102 */ SPID_SUBA_BEEP_END_NG,
/* 103 */ SPID_SUBA_MEM_CLR_START,
/* 104 */ SPID_SUBA_MEM_CLR_START_OK,
/* 105 */ SPID_SUBA_MEM_CLR_START_NG,
/* 106 */ SPID_SUBA_MEM_LET_CLR,
/* 107 */ SPID_SUBA_MEM_LET_CLR_OK,
/* 108 */ SPID_SUBA_MEM_LET_CLR_NG,
/* 109 */ SPID_SUBA_MEM_EEROR_CLR,
/* 110 */ SPID_SUBA_MEM_INIT_OK,
/* 111 */ SPID_SUBA_MEM_INIT_NG,
/* 112 */ SPID_SUBA_MEM_INLINE_CLR,
/* 113 */ SPID_SUBA_MEM_INLINE_CLR_OK,
/* 114 */ SPID_SUBA_MEM_INLINE_CLR_NG,
/* 115 */ SPID_SUBA_MEM_CLR_END,
/* 116 */ SPID_SUBA_MEM_CLR_END_OK,
/* 117 */ SPID_SUBA_MEM_CLR_END_NG,
/* 118 */ SPID_SUBA_SND_BASS_DEF,
/* 119 */ SPID_SUBA_SND_BASS_DEF_CB,
/* 120 */ SPID_SUBA_SND_TREBLE_DEF,
/* 121 */ SPID_SUBA_SND_TREBLE_DEF_CB,
/* 122 */ SPID_SUBA_SND_BACK_REST,
/* 123 */ SPID_SUBA_SND_VOL_SET,
/* 124 */ SPID_SUBA_SND_VOL_SET_CB,
/* 125 */ SPID_SUBA_SND_VOL_SET_OK,
/* 126 */ SPID_SUBA_SND_VOL_SET_NG,
/* 127 */ SPID_SUBA_SND_BASS_SET,
/* 128 */ SPID_SUBA_SND_BASS_SET_CB,
/* 129 */ SPID_SUBA_SND_BASS_SET_OK,
/* 130 */ SPID_SUBA_SND_BASS_SET_NG,
/* 131 */ SPID_SUBA_SND_TREBLE_SET,
/* 132 */ SPID_SUBA_SND_TREBLE_SET_CB,
/* 133 */ SPID_SUBA_SND_TREBLE_SET_OK,
/* 134 */ SPID_SUBA_SND_TREBLE_SET_NG,
/* 135 */ SPID_SUBA_LET_SPEAKER_DISP,
/* 136 */ SPID_SUBA_LET_MUTE_ON,
/* 137 */ SPID_SUBA_LET_MUTE_OFF,
/* 138 */ SPID_SUBA_LET_BACK_BUTTON_CHECK,
/* 139 */ SPID_SUBA_ERR_MEM_CLR,
/* 140 */ SPID_SUBA_ERR_MEM_CLR_OK,
/* 141 */ SPID_SUBA_ERR_MEM_CLR_NG,
/* 142 */ SPID_SUBA_WAIT_INFO,
/* *** */ SPID_SUBA_PROC_MAX
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
}LetSubA_EventChg_ts;

typedef struct{
    Type_uByte ubSpkType;
    Type_uByte ubSpkConInfo;
    Type_uByte ubSpkIndex;
    Type_uHWord uhTrackNo;
    Type_uByte ubSpkNoRest;
    Type_uByte ubSpkOkRest;
    Type_uByte ubSpkNgRest;
}LetSubASpkChkInfo_ts;

typedef struct{
    Type_uByte ubBeepType;
    Type_uByte ubBeepConInfo;
    Type_uByte ubBeepIndex;
    Type_uByte ubBeepFreq;
    Type_uByte ubBeepNoRest;
    Type_uByte ubBeepOkRest;
    Type_uByte ubBeepNgRest;
}LetSubABeepChkInfo_ts;

/********************************************************************************************/
/*                           ROM Table Section                                              */
/********************************************************************************************/
/* DiagApp PE Wait Timer */
static const Type_sWord nswLetSubAPETimer_Tbl[] = {
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
static LetSubA_EventChg_ts const nstLetSubAEvChg_a[] = {
    /*** Input Event ***                *** Process ID ***/
    {  LET_SUBA_EV_LET_START,             PID_SUBA_LET_START           },
    {  LET_SUBA_EV_LET_END,               PID_SUBA_LET_END             },
    {  LET_SUBA_EV_USB_TEST_START,        PID_SUBA_USB_TEST_START      },
    {  LET_SUBA_EV_USB_TEST_END,          PID_SUBA_USB_TEST_END        },
    {  LET_SUBA_EV_USB_SPK_START,         PID_SUBA_USB_SPK_START       },
    {  LET_SUBA_EV_USB_SPK_END,           PID_SUBA_USB_SPK_END         },
    {  LET_SUBA_EV_BIBIRI_START,          PID_SUBA_BIBIRI_START        },
    {  LET_SUBA_EV_BIBIRI_SET,            PID_SUBA_BIBIRI_SET          },
    {  LET_SUBA_EV_BIBIRI_END,            PID_SUBA_BIBIRI_END          },
    {  LET_SUBA_EV_BSM_INITIAL,           PID_SUBA_BSM_INITIAL         },
    {  LET_SUBA_EV_BSM_START_TP_SET,      PID_SUBA_BSM_START_TP_SET    },
    {  LET_SUBA_EV_BSM_END,               PID_SUBA_BSM_END             },
    {  LET_SUBA_EV_BSM_REST,              PID_SUBA_BSM_REST            },
    {  LET_SUBA_EV_SPK_BEEP_START,        PID_SUBA_SPK_BEEP_START      },
    {  LET_SUBA_EV_SPK_BEEP_CH_UP,        PID_SUBA_SPK_BEEP_CH_UP      },
    {  LET_SUBA_EV_SPK_BEEP_END,          PID_SUBA_SPK_BEEP_END        },
    {  LET_SUBA_EV_SPK_BEEP_BUTTON,       PID_SUBA_SPK_BEEP_BUTTON     },
    {  LET_SUBA_EV_MEM_CLEAR_START,       PID_SUBA_MEM_CLEAR_START     },
    {  LET_SUBA_EV_LET_MEM_CLEAR,         PID_SUBA_LET_MEM_CLEAR       },
    {  LET_SUBA_EV_MEM_INIT_CLEAR,        PID_SUBA_MEM_INIT_CLEAR      },
    {  LET_SUBA_EV_MEM_INLINE_CLEAR,      PID_SUBA_MEM_INLINE_CLEAR    },
    {  LET_SUBA_EV_MEM_CLEAR_END,         PID_SUBA_MEM_CLEAR_END       },
    {  LET_SUBA_EV_VOL_SET,               PID_SUBA_SND_VOLUME_SET      },
    {  LET_SUBA_EV_BASS_SET,              PID_SUBA_SND_BASS_SET        },
    {  LET_SUBA_EV_TREBLE_SET,            PID_SUBA_SND_TREBLE_SET      },
    {  LET_SUBA_EV_ERR_MEM_CLEAR,         PID_SUBA_ERR_MEM_CLEAR       },
    {  LET_SUBA_EV_MAX,                   PID_SUBA_PROC_MAX            },
};
/* Let Speaker */
static LetSubASpkChkInfo_ts const nstLetSubASpkInfo_a[] = {
/*  SpkType                   SpkInfo                   SpkIndex           TrackNo              SpkNoRest               SpkOkRest                  SpkNgRest             */
{   SND_LET_SPEAKER_FL,       LET_SPK_CONNECT_FR_L,     LETAPP_NUM_0,      LETAPP_NUM_1,        LET_SPK_DISP_FR_L,      LET_SPK_DISP_FR_L_OK,      LET_SPK_DISP_FR_L_NG   },
{   SND_LET_SPEAKER_FR,       LET_SPK_CONNECT_FR_R,     LETAPP_NUM_1,      LETAPP_NUM_50,       LET_SPK_DISP_FR_R,      LET_SPK_DISP_FR_R_OK,      LET_SPK_DISP_FR_R_NG   },
{   SND_LET_SPEAKER_RR,       LET_SPK_CONNECT_RR_R,     LETAPP_NUM_2,      LETAPP_NUM_2,        LET_SPK_DISP_RR_R,      LET_SPK_DISP_RR_R_OK,      LET_SPK_DISP_RR_R_NG   },
{   SND_LET_SPEAKER_RL,       LET_SPK_CONNECT_RR_L,     LETAPP_NUM_3,      LETAPP_NUM_20,       LET_SPK_DISP_RR_L,      LET_SPK_DISP_RR_L_OK,      LET_SPK_DISP_RR_L_NG   },
};
static LetSubABeepChkInfo_ts const nstLetSubABeepInfo_a[] = {
/*  BeepType                  BeepInfo                  BeepIndex          BeepFreq             BeepNoRest              BeepOkRest                 BeepNgRest             */
{   SND_LET_SPEAKER_FL,       LET_SPK_CONNECT_FR_L,     LETAPP_NUM_0,      BEEP_SINGEN_500HZ,   LET_SPK_DISP_FR_L,      LET_SPK_DISP_FR_L_OK,      LET_SPK_DISP_FR_L_NG   },
{   SND_LET_SPEAKER_FR,       LET_SPK_CONNECT_FR_R,     LETAPP_NUM_1,      BEEP_SINGEN_500HZ,   LET_SPK_DISP_FR_R,      LET_SPK_DISP_FR_R_OK,      LET_SPK_DISP_FR_R_NG   },
{   SND_LET_SPEAKER_RR,       LET_SPK_CONNECT_RR_R,     LETAPP_NUM_2,      BEEP_SINGEN_500HZ,   LET_SPK_DISP_RR_R,      LET_SPK_DISP_RR_R_OK,      LET_SPK_DISP_RR_R_NG   },
{   SND_LET_SPEAKER_RL,       LET_SPK_CONNECT_RR_L,     LETAPP_NUM_3,      BEEP_SINGEN_500HZ,   LET_SPK_DISP_RR_L,      LET_SPK_DISP_RR_L_OK,      LET_SPK_DISP_RR_L_NG   },
};
/********************************************************************************************/
/*                           Static Variable Definition Section                             */
/********************************************************************************************/

static ProcCTB_ts nstLetAppSubACTB;    /* Process Control Table */
static ProcBuf_ts nstLetAppSubAProcBuf_a[LETAPP_NUM_3];    /* Process Buffer */

Type_uByte nubBackState;    /* Whether a back notification was received */
/********************************************************************************************/
/*                           Static Function Declaration Section                            */
/********************************************************************************************/

static void nvdLetAppSubACmdSave(Type_uByte* aubDataBuff_p, Type_uByte aubSize);
static Type_uByte nubLetSubApEvPidChg(void);
static Type_uByte nubLetSubALetStart(void);
static Type_uByte nubLetSubALetStartCB(void);
static Type_uByte nubLetSubALetStartDef(void);
static Type_uByte nubLetSubALetStartOk(void);
static Type_uByte nubLetSubALetStartNg(void);
static Type_uByte nubLetSubAVspGet(void);
static Type_uByte nubLetSubAVspGetCB(void);
static Type_uByte nubLetSubAVspEep(void);
static Type_uByte nubLetSubALetEnd(void);
static Type_uByte nubLetSubALetEndCB(void);
static Type_uByte nubLetSubALetEndDef(void);
static Type_uByte nubLetSubALetEndOk(void);
static Type_uByte nubLetSubALetEndNg(void);
static Type_uByte nubLetSubAUsbSrcOn(void);
static Type_uByte nubLetSubAUsbSrcOnCB(void);
static Type_uByte nubLetSubAUsbStartDef(void);
static Type_uByte nubLetSubAUsbStartOk(void);
static Type_uByte nubLetSubAUsbStartNg(void);
static Type_uByte nubLetSubAUsbSrcOff(void);
static Type_uByte nubLetSubAUsbSrcOffCB(void);
static Type_uByte nubLetSubAUsbEndDef(void);
static Type_uByte nubLetSubAUsbEndOk(void);
static Type_uByte nubLetSubAUsbEndNg(void);
static Type_uByte nubLetSubASpkInfoJudge(void);
static Type_uByte nubLetSubASpkStartDef(void);
static Type_uByte nubLetSubASpkStartOk(void);
static Type_uByte nubLetSubASpkStartNg(void);
static Type_uByte nubLetSubASpkEndJudge(void);
static Type_uByte nubLetSubASpkSndSet(void);
static Type_uByte nubLetSubASpkSndSetCB(void);
static Type_uByte nubLetSubAUsbPlay(void);
static Type_uByte nubLetSubAUsbPlayCB(void);
static Type_uByte nubLetSubASpkDisp(void);
static Type_uByte nubLetSubASpkResultWait(void);
static Type_uByte nubLetSubASpkResultDisp(void);
static Type_uByte nubLetSubALetBlankDisp(void);
static Type_uByte nubLetSubAUsbPlayStop(void);
static Type_uByte nubLetSubAUsbPlayStopCB(void);
static Type_uByte nubLetSubASpkChgNext(void);
static Type_uByte nubLetSubASpkChgBack(void);
static Type_uByte nubLetSubASpkResultEep(void);
static Type_uByte nubLetSubASpkEndOk(void);
static Type_uByte nubLetSubASpkNoChkEep(void);
static Type_uByte nubLetSubASpkNoChkDisp(void);
static Type_uByte nubLetSubASpkEndNg(void);
static Type_uByte nubLetSubABibiriPlay(void);
static Type_uByte nubLetSubABibiriPlayCB(void);
static Type_uByte nubLetSubABibiriSndSet(void);
static Type_uByte nubLetSubABibiriSndSetCB(void);
static Type_uByte nubLetSubABibiriDef(void);
static Type_uByte nubLetSubABibiriStartOk(void);
static Type_uByte nubLetSubABibiriStartNg(void);
static Type_uByte nubLetSubAUsbSongSet(void);
static Type_uByte nubLetSubABibiriSetOk(void);
static Type_uByte nubLetSubABibiriSetNg(void);
static Type_uByte nubLetSubABibiriDisp(void);
static Type_uByte nubLetSubABibiriResultWait(void);
static Type_uByte nubLetSubABibiriResultDisp(void);
static Type_uByte nubLetSubABibiriResultEep(void);
static Type_uByte nubLetSubABibiriEndOk(void);
static Type_uByte nubLetSubABibiriNoChkEep(void);
static Type_uByte nubLetSubABibiriNoChkDisp(void);
static Type_uByte nubLetSubABibiriEndNg(void);
static Type_uByte nubLetSubABsmInitial(void);
static Type_uByte nubLetSubABsmInitialCB(void);
static Type_uByte nubLetSubABsmInitialOk(void);
static Type_uByte nubLetSubABsmInitialNg(void);
static Type_uByte nubLetSubABsmTpSet(void);
static Type_uByte nubLetSubABsmTpSetCB(void);
static Type_uByte nubLetSubABsmTpSetOk(void);
static Type_uByte nubLetSubABsmTpSetNg(void);
static Type_uByte nubLetSubABsmEnd(void);
static Type_uByte nubLetSubABsmEndCB(void);
static Type_uByte nubLetSubABsmEndOk(void);
static Type_uByte nubLetSubABsmEndNg(void);
static Type_uByte nubLetSubABsmRest(void);
static Type_uByte nubLetSubABsmRestCB(void);
static Type_uByte nubLetSubABsmRestOk(void);
static Type_uByte nubLetSubABsmRestNg(void);
static Type_uByte nubLetSubABeepStartChk(void);
static Type_uByte nubLetSubABeepSrcOn(void);
static Type_uByte nubLetSubABeepSrcOnCB(void);
static Type_uByte nubLetSubABeepDef(void);
static Type_uByte nubLetSubABeepStartOk(void);
static Type_uByte nubLetSubABeepStartNg(void);
static Type_uByte nubLetSubABeepEndJudge(void);
static Type_uByte nubLetSubABeepChUpOk(void);
static Type_uByte nubLetSubABeepChUpNg(void);
static Type_uByte nubLetSubABeepSndSet(void);
static Type_uByte nubLetSubABeepSndSetCB(void);
static Type_uByte nubLetSubABeepDisp(void);
static Type_uByte nubLetSubABeepDispOk(void);
static Type_uByte nubLetSubABeepButtonChk(void);
static Type_uByte nubLetSubABeepResultDisp(void);
static Type_uByte nubLetSubABeepChgNext(void);
static Type_uByte nubLetSubABeepChgBack(void);
static Type_uByte nubLetSubABeepResultEep(void);
static Type_uByte nubLetSubABeepSrcOff(void);
static Type_uByte nubLetSubABeepSrcOffCB(void);
static Type_uByte nubLetSubABeepEndOk(void);
static Type_uByte nubLetSubABeepNoChkEep(void);
static Type_uByte nubLetSubABeepNoChkDisp(void);
static Type_uByte nubLetSubABeepEndNg(void);
static Type_uByte nubLetSubAMemClrStart(void);
static Type_uByte nubLetSubAMemClrStartOk(void);
static Type_uByte nubLetSubAMemClrStartNg(void);
static Type_uByte nubLetSubAMemLetClr(void);
static Type_uByte nubLetSubAMemLetClrOk(void);
static Type_uByte nubLetSubAMemLetClrNg(void);
static Type_uByte nubLetSubAMemErrorClr(void);
static Type_uByte nubLetSubAMemInitOk(void);
static Type_uByte nubLetSubAMemInitNg(void);
static Type_uByte nubLetSubAMemInlineClr(void);
static Type_uByte nubLetSubAMemInlineClrOk(void);
static Type_uByte nubLetSubAMemInlineClrNg(void);
static Type_uByte nubLetSubAMemClrEnd(void);
static Type_uByte nubLetSubAMemClrEndOk(void);
static Type_uByte nubLetSubAMemClrEndNg(void);
static Type_uByte nubLetSubASndBassDef(void);
static Type_uByte nubLetSubASndBassDefCB(void);
static Type_uByte nubLetSubASndTrebleDef(void);
static Type_uByte nubLetSubASndTrebleDefCB(void);
static Type_uByte nubLetSubASndBackRest(void);
static Type_uByte nubLetSubASndVolSet(void);
static Type_uByte nubLetSubASndVolSetCB(void);
static Type_uByte nubLetSubASndVolSetOk(void);
static Type_uByte nubLetSubASndVolSetNg(void);
static Type_uByte nubLetSubASndBassSet(void);
static Type_uByte nubLetSubASndBassSetCB(void);
static Type_uByte nubLetSubASndBassSetOk(void);
static Type_uByte nubLetSubASndBassSetNg(void);
static Type_uByte nubLetSubASndTrebleSet(void);
static Type_uByte nubLetSubASndTrebleSetCB(void);
static Type_uByte nubLetSubASndTrebleSetOk(void);
static Type_uByte nubLetSubASndTrebleSetNg(void);
static Type_uByte nubLetSubALetSpeakerDisp(void);
static Type_uByte nubLetSubALetMuteOn(void);
static Type_uByte nubLetSubALetMuteOff(void);
static Type_uByte nubLetSubALetBackButtonCheck(void);
static Type_uByte nubLetSubAERRMemClr(void);
static Type_uByte nubLetSubAERRMemClrOk(void);
static Type_uByte nubLetSubAERRMemClrNg(void);
static Type_uByte nubLetSubAWaitInfo(void);

static Type_uByte nubLetSubASpkChkInfo(Type_uByte aubTargetSpk);
static void nvdLetSubASpkRestSave(Type_uByte aubSpkInfo, Type_uByte aubSpkRest);

/********************************************************************************************/
/*                           Global variable definition section                             */
/********************************************************************************************/

Type_uByte wubLetSpkIndex; /* Let USB Beep Speaker Check Order */
Type_uByte wubLetSpkBtnInfo; /* Let USB Beep Speaker Response Button Info */
Type_uHWord wuhLetSpkConnectInfo; /* Let USB Beep Speaker Connect Info */
Type_uHWord wuhLetBibiriSong; /* Let USB Beep Speaker Check Order */
Type_uByte wubLetSndBass; /* Let Current Bass */
Type_uByte wubLetSndTreble; /* Let Current Treble */

/********************************************************************************************/
/*                           Function Definition Section                                    */
/********************************************************************************************/
/*************************** LetAppSubA Process Table ***************************************/

const ElementOfProcTbl_ts nstLetSubALetStart_Tbl[] = { /* PID_SUBA_LET_START */
/* STAGE00 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_LET_START,            STAGE1,      WTID_5000,   C_STAGE4,    WTID_0),
/* STAGE01 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_LET_START_CB,         C_STAGE2,    WTID_0,      C_STAGE4,    WTID_0),
/* STAGE02 */ //PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_LET_MUTE_ON,         C_STAGE3,    WTID_0,      C_STAGE5,    WTID_0),
/* STAGE03 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_LET_START_DEF,        C_STAGE3,    WTID_0,      C_STAGE4,    WTID_0),
/* STAGE04 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_LET_START_OK,         PE_PEC0,      WTID_FEVR,    PE_PEC1,     WTID_FEVR),
/* STAGE05 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_LET_START_NG,         PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
/* STAGE06 */ //PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_LET_VSP_GET,          STAGE7,      WTID_1000,   PE_PEC1,     WTID_FEVR),
/* STAGE07 */ //PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_LET_VSP_GET_CB,       C_STAGE8,    WTID_0,      PE_PEC1,     WTID_FEVR),
/* STAGE08 */ //PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_LET_VSP_EEP,          PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
};

const ElementOfProcTbl_ts nstLetSubALetEnd_Tbl[] = { /* PID_SUBA_LET_END */
/* STAGE00 */ //PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_LET_MUTE_OFF,         C_STAGE1,    WTID_0,      C_STAGE5,    WTID_0),
/* STAGE01 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_LET_END,              STAGE1,      WTID_5000,   C_STAGE4,    WTID_0),
/* STAGE02 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_LET_END_CB,           C_STAGE2,    WTID_0,      C_STAGE4,    WTID_0),
/* STAGE03 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_LET_END_DEF,          C_STAGE3,    WTID_0,      C_STAGE4,    WTID_0),
/* STAGE04 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_LET_END_OK,           PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
/* STAGE05 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_LET_END_NG,           PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
};

const ElementOfProcTbl_ts nstLetSubAUsbTestStart_Tbl[] = { /* PID_SUBA_USB_TEST_START */
/* STAGE00 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_USB_SRC_ON,           STAGE1,      WTID_5000,   C_STAGE4,    WTID_0),
/* STAGE01 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_USB_SRC_ON_CB,        C_STAGE2,    WTID_0,      C_STAGE4,    WTID_0),
/* STAGE02 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_USB_START_DEF,        C_STAGE3,    WTID_0,      C_STAGE4,    WTID_0),
/* STAGE03 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_USB_START_OK,         PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
/* STAGE04 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_USB_START_NG,         PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
};

const ElementOfProcTbl_ts nstLetSubAUsbTestEnd_Tbl[] = { /* PID_SUBA_USB_TEST_END */
/* STAGE00 */ PE_MAKE_PROC_TABLE(PE_HAVE_CHILD,      PID_SUBA_SND_SET_BACK,          C_STAGE1,    WTID_0,      C_STAGE1,    WTID_0),
/* STAGE01 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_USB_SRC_OFF,          STAGE2,      WTID_5000,   C_STAGE5,    WTID_0),
/* STAGE02 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_USB_SRC_OFF_CB,       C_STAGE3,    WTID_0,      C_STAGE5,    WTID_0),
/* STAGE03 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_USB_END_DEF,          C_STAGE4,    WTID_0,      C_STAGE5,    WTID_0),
/* STAGE04 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_USB_END_OK,           PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
/* STAGE05 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_USB_END_NG,           PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
};

const ElementOfProcTbl_ts nstLetSubAUsbSpkStart_Tbl[] = { /* PID_SUBA_USB_SPK_START */
/* STAGE00 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_SPK_INFO_JUDGE,       C_STAGE1,    WTID_0,      C_STAGE3,    WTID_0),
/* STAGE01 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_SPK_START_DEF,        C_STAGE2,    WTID_0,      C_STAGE3,    WTID_0),
/* STAGE02 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_SPK_START_OK,         PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
/* STAGE03 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_SPK_START_NG,         PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
};

const ElementOfProcTbl_ts nstLetSubAUsbSpkEnd_Tbl[] = { /* PID_SUBA_USB_SPK_END */
/* STAGE00 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_LET_MUTE_OFF,         STAGE1,      WTID_0,      C_STAGE1,    WTID_0),
/* STAGE01 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_LET_BLANK_DISP,       STAGE2,      WTID_0,      C_STAGE2,    WTID_0),
/* STAGE02 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_USB_PLAY_STOP,        STAGE3,      WTID_10000,  C_STAGE19,   WTID_0),
/* STAGE03 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_USB_PLAY_STOP_CB,     C_STAGE4,    WTID_0,      C_STAGE19,   WTID_0),
/* STAGE04 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_SPK_END_JUDGE,        C_STAGE5,    WTID_0,      C_STAGE16,   WTID_0),
/* STAGE05 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_SPK_SND_SET,          STAGE6,      WTID_5000,   C_STAGE19,   WTID_0),
/* STAGE06 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_SPK_SND_SET_CB,       C_STAGE7,    WTID_0,      C_STAGE19,   WTID_0),
/* STAGE07 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_USB_PLAY,             STAGE8,      WTID_10000,  C_STAGE19,   WTID_0),
/* STAGE08 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_USB_PLAY_CB,          C_STAGE9,    WTID_0,      C_STAGE19,   WTID_0),
/* STAGE09 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_LET_BACK_BUTTON_CHECK,        C_STAGE10,    WTID_0,      C_STAGE15,   WTID_0),
/* STAGE10 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_SPK_DISP,             STAGE11,     WTID_FEVR,   C_STAGE19,   WTID_0),
/* STAGE11 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_WAIT_INFO,            C_STAGE12,   WTID_0,      C_STAGE19,   WTID_0),
/* STAGE12 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_SPK_RESULT_WAIT,      C_STAGE13,   WTID_0,      C_STAGE15,   WTID_0),
/* STAGE13 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_SPK_RESULT_DISP,      STAGE14,     WTID_500,    STAGE14,     WTID_500),
/* STAGE14 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_SPK_CHG_NEXT,         C_STAGE1,    WTID_0,      C_STAGE1,    WTID_0),
/* STAGE15 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_SPK_CHG_BACK,         C_STAGE1,    WTID_0,      C_STAGE2,    WTID_0),
/* STAGE16 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_LET_SPEAKER_DISP,     C_STAGE17,   WTID_0,      C_STAGE17,   WTID_0),
/* STAGE17 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_SPK_RESULT_EEP,       STAGE18,     WTID_0,      STAGE18,     WTID_0),
/* STAGE18 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_SPK_END_OK,           PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
/* STAGE19 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_SPK_NOCHK_EEP,        STAGE20,     WTID_0,      C_STAGE20,   WTID_0),
/* STAGE20 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_SPK_NOCHK_DISP,       STAGE21,     WTID_500,    C_STAGE21,   WTID_0),
/* STAGE21 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_LET_SPEAKER_DISP,     C_STAGE22,   WTID_0,      C_STAGE22,   WTID_0),
/* STAGE22 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_SPK_END_NG,           PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
};

const ElementOfProcTbl_ts nstLetSubABibiriStart_Tbl[] = { /* PID_SUBA_BIBIRI_START */
/* STAGE00 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BIBIRI_SND_SET,       STAGE1,      WTID_5000,   C_STAGE4,    WTID_0),
/* STAGE01 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BIBIRI_SND_SET_CB,    C_STAGE2,    WTID_0,      C_STAGE4,    WTID_0),
/* STAGE02 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BIBIRI_DEF,           C_STAGE3,    WTID_0,      C_STAGE4,    WTID_0),
/* STAGE03 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BIBIRI_START_OK,      PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
/* STAGE04 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BIBIRI_START_NG,      PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
};

const ElementOfProcTbl_ts nstLetSubABibiriSet_Tbl[] = { /* PID_SUBA_BIBIRI_SET */
/* STAGE00 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_USB_SONG_SET,         C_STAGE1,    WTID_0,      C_STAGE2,    WTID_0),
/* STAGE01 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BIBIRI_SET_OK,        PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
/* STAGE02 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BIBIRI_SET_NG,        PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
};

const ElementOfProcTbl_ts nstLetSubABibiriEnd_Tbl[] = { /* PID_SUBA_BIBIRI_END */
/* STAGE00 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_LET_MUTE_OFF,         STAGE1,      WTID_0,      C_STAGE1,    WTID_0),
/* STAGE01 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_LET_BLANK_DISP,       C_STAGE2,    WTID_0,      C_STAGE2,    WTID_0),
/* STAGE02 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BIBIRI_PLAY,          STAGE3,      WTID_5000,   C_STAGE16,   WTID_0),
/* STAGE03 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BIBIRI_PLAY_CB,       C_STAGE4,    WTID_0,      C_STAGE16,   WTID_0),
/* STAGE04 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BIBIRI_DISP,          STAGE5,      WTID_FEVR,   STAGE5,      WTID_0),
/* STAGE05 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_WAIT_INFO,            C_STAGE6,    WTID_0,      C_STAGE16,   WTID_0),
/* STAGE06 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BIBIRI_RESULT_WAIT,   STAGE9,      WTID_0,      C_STAGE7,    WTID_0),
/* STAGE07 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_USB_PLAY_STOP,        STAGE8,      WTID_5000,   C_STAGE16,   WTID_0),
/* STAGE08 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_USB_PLAY_STOP_CB,     C_STAGE2,    WTID_0,      C_STAGE16,   WTID_0),
/* STAGE09 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BIBIRI_RESULT_DISP,   STAGE10,     WTID_500,    C_STAGE10,   WTID_0),
/* STAGE10 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_LET_SPEAKER_DISP,     C_STAGE11,   WTID_0,      C_STAGE11,   WTID_0),
/* STAGE11 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_LET_MUTE_ON,         C_STAGE12,      WTID_0,      C_STAGE12,    WTID_0),
/* STAGE12 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_USB_PLAY_STOP,        STAGE13,     WTID_5000,   C_STAGE16,   WTID_0),
/* STAGE13 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_USB_PLAY_STOP_CB,     C_STAGE14,   WTID_0,      C_STAGE16,   WTID_0),
/* STAGE14 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BIBIRI_RESULT_EEP,    C_STAGE15,   WTID_0,      C_STAGE16,   WTID_0),
/* STAGE15 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BIBIRI_END_OK,        PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
/* STAGE16 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BIBIRI_NOCHK_EEP,     C_STAGE17,   WTID_0,      C_STAGE19,   WTID_0),
/* STAGE17 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BIBIRI_NOCHK_DISP,    STAGE18,     WTID_500,    STAGE18,     WTID_500),
/* STAGE18 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_LET_SPEAKER_DISP,     C_STAGE19,   WTID_0,      C_STAGE19,   WTID_0),
/* STAGE19 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BIBIRI_END_NG,        PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
};

const ElementOfProcTbl_ts nstLetSubABsmInitial_Tbl[] = { /* PID_SUBA_BSM_INITIAL */
/* STAGE00 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BSM_INITIAL,          STAGE1,      WTID_5000,   C_STAGE3,    WTID_0),
/* STAGE01 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BSM_INITIAL_CB,       C_STAGE2,    WTID_0,      C_STAGE3,    WTID_0),
/* STAGE02 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BSM_INITIAL_OK,       PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
/* STAGE03 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BSM_INITIAL_NG,       PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
};

const ElementOfProcTbl_ts nstLetSubABsmTpSet_Tbl[] = { /* PID_SUBA_BSM_START_TP_SET */
/* STAGE00 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BSM_TP_SET,           STAGE1,      WTID_5000,   C_STAGE3,    WTID_0),
/* STAGE01 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BSM_TP_SET_CB,        C_STAGE2,    WTID_0,      C_STAGE3,    WTID_0),
/* STAGE02 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BSM_TP_SET_OK,        PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
/* STAGE03 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BSM_TP_SET_NG,        PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
};

const ElementOfProcTbl_ts nstLetSubABsmEnd_Tbl[] = { /* PID_SUBA_BSM_END */
/* STAGE00 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BSM_END,              STAGE1,      WTID_FEVR,   C_STAGE3,    WTID_0),
/* STAGE01 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BSM_END_CB,           C_STAGE2,    WTID_0,      C_STAGE3,    WTID_0),
/* STAGE02 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BSM_END_OK,           PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
/* STAGE03 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BSM_END_NG,           PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
};

const ElementOfProcTbl_ts nstLetSubABsmRest_Tbl[] = { /* PID_SUBA_BSM_REST */
/* STAGE00 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BSM_REST,             STAGE1,      WTID_5000,   C_STAGE3,    WTID_0),
/* STAGE01 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BSM_REST_CB,          C_STAGE2,    WTID_0,      C_STAGE3,    WTID_0),
/* STAGE02 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BSM_REST_OK,          PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
/* STAGE03 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BSM_REST_NG,          PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
};

const ElementOfProcTbl_ts nstLetSubASpkBeepStart_Tbl[] = { /* PID_SUBA_SPK_BEEP_START */
/* STAGE00 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BEEP_START_CHK,       C_STAGE1,    WTID_0,      C_STAGE6,    WTID_0),
/* STAGE01 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_LET_BLANK_DISP,       STAGE2,      WTID_0,      C_STAGE6,    WTID_0),
/* STAGE02 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BEEP_SRC_ON,          STAGE3,      WTID_5000,   C_STAGE6,    WTID_0),
/* STAGE03 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BEEP_SRC_ON_CB,       C_STAGE4,    WTID_0,      C_STAGE6,    WTID_0),
/* STAGE04 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BEEP_DEF,             C_STAGE5,    WTID_0,      C_STAGE6,    WTID_0),
/* STAGE05 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BEEP_START_OK,        PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
/* STAGE06 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BEEP_START_NG,        PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
};

const ElementOfProcTbl_ts nstLetSubASpkBeepChUp_Tbl[] = { /* PID_SUBA_SPK_BEEP_CH_UP */
/* STAGE00 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BEEP_CHG_NEXT,        C_STAGE1,    WTID_0,      C_STAGE6,    WTID_0),
/* STAGE01 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BEEP_END_JUDGE,       C_STAGE2,    WTID_0,      C_STAGE6,    WTID_0),
/* STAGE02 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BEEP_SND_SET,         STAGE3,      WTID_5000,   C_STAGE6,    WTID_0),
/* STAGE03 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BEEP_SND_SET_CB,      C_STAGE4,    WTID_0,      C_STAGE6,    WTID_0),
/* STAGE04 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BEEP_DISP,            STAGE5,      WTID_0,      STAGE6,      WTID_0),
/* STAGE05 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BEEP_CH_UP_OK,        PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
/* STAGE06 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BEEP_CH_UP_NG,        PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
};

const ElementOfProcTbl_ts nstLetSubASpkBeepEnd_Tbl[] = { /* PID_SUBA_SPK_BEEP_END */
/* STAGE00 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_LET_MUTE_OFF,       STAGE1,      WTID_0,      C_STAGE1,    WTID_0),
/* STAGE01 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BEEP_END_JUDGE,       C_STAGE2,    WTID_0,      C_STAGE6,     WTID_0),
/* STAGE02 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BEEP_SND_SET,         STAGE3,      WTID_5000,   C_STAGE11,    WTID_0),
/* STAGE03 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BEEP_SND_SET_CB,      C_STAGE4,    WTID_0,      C_STAGE11,    WTID_0),
/* STAGE04 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BEEP_DISP,            STAGE5,      WTID_0,      C_STAGE11,    WTID_0),
/* STAGE05 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BEEP_DISP_OK,         PE_PEC0,     WTID_FEVR,   C_STAGE11,    WTID_0),
/* STAGE06 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_LET_SPEAKER_DISP,     C_STAGE7,    WTID_0,      C_STAGE7,     WTID_0),
/* STAGE07 */ PE_MAKE_PROC_TABLE(PE_HAVE_CHILD,      PID_SUBA_SND_SET_BACK,          C_STAGE8,    WTID_0,      C_STAGE11,    WTID_0),
/* STAGE08 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BEEP_SRC_OFF,         STAGE9,      WTID_5000,   C_STAGE11,    WTID_0),
/* STAGE09 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BEEP_SRC_OFF_CB,      C_STAGE10,    WTID_0,      C_STAGE11,    WTID_0),
/* STAGE10 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BEEP_END_OK,          PE_PEC0,     WTID_FEVR,   PE_PEC1,      WTID_FEVR),
/* STAGE11 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BEEP_NOCHK_EEP,       STAGE12,     WTID_0,      C_STAGE12,    WTID_0),
/* STAGE12 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BEEP_NOCHK_DISP,      STAGE13,     WTID_500,    C_STAGE13,    WTID_0),
/* STAGE13 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_LET_BLANK_DISP,       STAGE14,     WTID_0,      C_STAGE14,    WTID_0),
/* STAGE14 */ PE_MAKE_PROC_TABLE(PE_HAVE_CHILD,      PID_SUBA_SND_SET_BACK,          C_STAGE15,   WTID_0,      C_STAGE15,    WTID_0),
/* STAGE15 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BEEP_SRC_OFF,         STAGE16,     WTID_5000,   C_STAGE17,    WTID_0),
/* STAGE16 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BEEP_SRC_OFF_CB,      C_STAGE17,   WTID_0,      C_STAGE17,    WTID_0),
/* STAGE17 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_LET_SPEAKER_DISP,     C_STAGE18,   WTID_0,      C_STAGE18,    WTID_0),
/* STAGE18 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BEEP_END_NG,          PE_PEC0,     WTID_FEVR,   PE_PEC1,      WTID_FEVR),
};

const ElementOfProcTbl_ts nstLetSubASpkBeepButton_Tbl[] = { /* PID_SUBA_SPK_BEEP_BUTTON */
/* STAGE00 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BEEP_BUTTON_CHK,      C_STAGE1,    WTID_0,      C_STAGE3,    WTID_0),
/* STAGE01 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BEEP_RESULT_DISP,     STAGE2,      WTID_500,    PE_PEC1,     WTID_FEVR),
/* STAGE02 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BEEP_CHG_NEXT,        C_STAGE4,    WTID_0,      C_STAGE4,    WTID_0),
/* STAGE03 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BEEP_CHG_BACK,        C_STAGE4,    WTID_0,      PE_PEC1,     WTID_FEVR),
/* STAGE04 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_LET_BLANK_DISP,       STAGE5,      WTID_0,      C_STAGE5,    WTID_0),
/* STAGE05 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_BEEP_RESULT_EEP,      STAGE6,      WTID_0,      C_STAGE6,    WTID_0),
/* STAGE06 */ PE_MAKE_PROC_TABLE(PE_HAVE_CHILD,      PID_SUBA_SPK_BEEP_END,          PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
};

const ElementOfProcTbl_ts nstLetSubAMemClearStart_Tbl[] = { /* PID_SUBA_MEM_CLEAR_START */
/* STAGE00 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_MEM_CLR_START,        C_STAGE1,    WTID_0,      C_STAGE2,    WTID_0),
/* STAGE01 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_MEM_CLR_START_OK,     PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
/* STAGE02 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_MEM_CLR_START_NG,     PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
};

const ElementOfProcTbl_ts nstLetSubALetMemClear_Tbl[] = { /* PID_SUBA_LET_MEM_CLEAR */
/* STAGE00 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_MEM_LET_CLR,          C_STAGE1,    WTID_0,      C_STAGE2,    WTID_0),
/* STAGE01 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_MEM_LET_CLR_OK,       PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
/* STAGE02 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_MEM_LET_CLR_NG,       PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
};

const ElementOfProcTbl_ts nstLetSubAInitClear_Tbl[] = { /* PID_SUBA_MEM_INIT_CLEAR */
/* STAGE00 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_MEM_LET_CLR,          C_STAGE1,    WTID_0,      C_STAGE1,    WTID_0),
/* STAGE01 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_MEM_EEROR_CLR,        C_STAGE2,    WTID_0,      C_STAGE2,    WTID_0),
/* STAGE02 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_MEM_INIT_OK,          PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
/* STAGE03 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_MEM_INIT_NG,          PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
};

const ElementOfProcTbl_ts nstLetSubAInlineClear_Tbl[] = { /* PID_SUBA_MEM_INLINE_CLEAR */
/* STAGE00 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_MEM_INLINE_CLR,       C_STAGE1,    WTID_0,      C_STAGE2,    WTID_0),
/* STAGE01 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_MEM_INLINE_CLR_OK,    PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
/* STAGE02 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_MEM_INLINE_CLR_NG,    PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
};

const ElementOfProcTbl_ts nstLetSubAMemClearEnd_Tbl[] = { /* PID_SUBA_MEM_CLEAR_END */
/* STAGE00 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_MEM_CLR_END,          C_STAGE1,    WTID_0,      C_STAGE2,    WTID_0),
/* STAGE01 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_MEM_CLR_END_OK,       PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
/* STAGE02 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_MEM_CLR_END_NG,       PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
};

const ElementOfProcTbl_ts nstLetSubASndSetBack_Tbl[] = { /* PID_SUBA_SND_SET_BACK */
/* STAGE00 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_SND_BASS_DEF,         STAGE1,      WTID_5000,   C_STAGE2,    WTID_0),
/* STAGE01 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_SND_BASS_DEF_CB,      C_STAGE2,    WTID_0,      C_STAGE2,    WTID_0),
/* STAGE02 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_SND_TREBLE_DEF,       STAGE3,      WTID_5000,   C_STAGE4,    WTID_0),
/* STAGE03 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_SND_TREBLE_DEF_CB,    C_STAGE4,    WTID_0,      C_STAGE4,    WTID_0),
/* STAGE04 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_SND_BACK_REST,        PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
};

const ElementOfProcTbl_ts nstLetSubASndVolSet_Tbl[] = { /* PID_SUBA_SND_VOLUME_SET */
/* STAGE00 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_SND_VOL_SET,          STAGE1,      WTID_5000,   C_STAGE3,    WTID_0),
/* STAGE01 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_SND_VOL_SET_CB,       C_STAGE2,    WTID_0,      C_STAGE3,    WTID_0),
/* STAGE02 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_SND_VOL_SET_OK,       PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
/* STAGE03 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_SND_VOL_SET_NG,       PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
};

const ElementOfProcTbl_ts nstLetSubASndBassSet_Tbl[] = { /* PID_SUBA_SND_BASS_SET */
/* STAGE00 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_SND_BASS_SET,         STAGE1,      WTID_5000,   C_STAGE3,    WTID_0),
/* STAGE01 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_SND_BASS_SET_CB,      C_STAGE2,    WTID_0,      C_STAGE3,    WTID_0),
/* STAGE02 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_SND_BASS_SET_OK,      PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
/* STAGE03 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_SND_BASS_SET_NG,      PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
};

const ElementOfProcTbl_ts nstLetSubASndTrebleSet_Tbl[] = { /* PID_SUBA_SND_TREBLE_SET */
/* STAGE00 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_SND_TREBLE_SET,       STAGE1,      WTID_5000,   C_STAGE3,    WTID_0),
/* STAGE01 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_SND_TREBLE_SET_CB,    C_STAGE2,    WTID_0,      C_STAGE3,    WTID_0),
/* STAGE02 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_SND_TREBLE_SET_OK,    PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
/* STAGE03 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_SND_TREBLE_SET_NG,    PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
};

const ElementOfProcTbl_ts nstLetSubAERRMemClear_Tbl[] = { /* PID_SUBA_ERR_MEM_CLEAR */
/* STAGE00 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_ERR_MEM_CLR,          C_STAGE1,    WTID_0,      C_STAGE2,    WTID_0),
/* STAGE01 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_ERR_MEM_CLR_OK,       PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
/* STAGE02 */ PE_MAKE_PROC_TABLE(PE_NO_CHILD,        SPID_SUBA_ERR_MEM_CLR_NG,       PE_PEC0,     WTID_FEVR,   PE_PEC1,     WTID_FEVR),
};

/*************************** LetAppSubA Main Process ****************************************/
const ElementOfProcTbl_ts (*const nstLetAppSubAProc_Tbl[PID_SUBA_PROC_MAX]) = {
/* 000 */ nstLetSubALetStart_Tbl,            /*  PID_SUBA_LET_START */
/* 001 */ nstLetSubALetEnd_Tbl,              /*  PID_SUBA_LET_END */
/* 002 */ nstLetSubAUsbTestStart_Tbl,        /*  PID_SUBA_USB_TEST_START */
/* 003 */ nstLetSubAUsbTestEnd_Tbl,          /*  PID_SUBA_USB_TEST_END */
/* 004 */ nstLetSubAUsbSpkStart_Tbl,         /*  PID_SUBA_USB_SPK_START */
/* 005 */ nstLetSubAUsbSpkEnd_Tbl,           /*  PID_SUBA_USB_SPK_END */
/* 006 */ nstLetSubABibiriStart_Tbl,         /*  PID_SUBA_BIBIRI_START */
/* 007 */ nstLetSubABibiriSet_Tbl,           /*  PID_SUBA_BIBIRI_SET */
/* 008 */ nstLetSubABibiriEnd_Tbl,           /*  PID_SUBA_BIBIRI_END */
/* 009 */ nstLetSubABsmInitial_Tbl,          /*  PID_SUBA_BSM_INITIAL */
/* 010 */ nstLetSubABsmTpSet_Tbl,            /*  PID_SUBA_BSM_START_TP_SET */
/* 011 */ nstLetSubABsmEnd_Tbl,              /*  PID_SUBA_BSM_END */
/* 012 */ nstLetSubABsmRest_Tbl,             /*  PID_SUBA_BSM_REST */
/* 013 */ nstLetSubASpkBeepStart_Tbl,        /*  PID_SUBA_SPK_BEEP_START */
/* 014 */ nstLetSubASpkBeepChUp_Tbl,         /*  PID_SUBA_SPK_BEEP_CH_UP */
/* 015 */ nstLetSubASpkBeepEnd_Tbl,          /*  PID_SUBA_SPK_BEEP_END */
/* 016 */ nstLetSubASpkBeepButton_Tbl,       /*  PID_SUBA_SPK_BEEP_BUTTON */
/* 017 */ nstLetSubAMemClearStart_Tbl,       /*  PID_SUBA_MEM_CLEAR_START */
/* 018 */ nstLetSubALetMemClear_Tbl,         /*  PID_SUBA_LET_MEM_CLEAR */
/* 019 */ nstLetSubAInitClear_Tbl,           /*  PID_SUBA_MEM_INIT_CLEAR */
/* 020 */ nstLetSubAInlineClear_Tbl,         /*  PID_SUBA_MEM_INLINE_CLEAR */
/* 021 */ nstLetSubAMemClearEnd_Tbl,         /*  PID_SUBA_MEM_CLEAR_END */
/* 022 */ nstLetSubASndSetBack_Tbl,          /*  PID_SUBA_SND_SET_BACK */
/* 023 */ nstLetSubASndVolSet_Tbl,           /*  PID_SUBA_SND_VOLUME_SET */
/* 024 */ nstLetSubASndBassSet_Tbl,          /*  PID_SUBA_SND_BASS_SET */
/* 025 */ nstLetSubASndTrebleSet_Tbl,        /*  PID_SUBA_SND_TREBLE_SET */
/* 026 */ nstLetSubAERRMemClear_Tbl          /*  PID_SUBA_ERR_MEM_CLEAR */
};

/*************************** LetAppSubA SubProcess ******************************************/
Type_uByte (*const nstLetAppSubASubProc_Tbl[SPID_SUBA_PROC_MAX])(void) = {
/* 000 */ nubLetSubALetStart,                /* SPID_SUBA_LET_START */
/* 001 */ nubLetSubALetStartCB,              /* SPID_SUBA_LET_START_CB */
/* 002 */ nubLetSubALetStartDef,             /* SPID_SUBA_LET_START_DEF */
/* 003 */ nubLetSubALetStartOk,              /* SPID_SUBA_LET_START_OK */
/* 004 */ nubLetSubALetStartNg,              /* SPID_SUBA_LET_START_NG */
/* 005 */ nubLetSubAVspGet,                  /* SPID_SUBA_LET_VSP_GET */
/* 006 */ nubLetSubAVspGetCB,                /* SPID_SUBA_LET_VSP_GET_CB */
/* 007 */ nubLetSubAVspEep,                  /* SPID_SUBA_LET_VSP_EEP */
/* 008 */ nubLetSubALetEnd,                  /* SPID_SUBA_LET_END */
/* 009 */ nubLetSubALetEndCB,                /* SPID_SUBA_LET_END_CB */
/* 010 */ nubLetSubALetEndDef,               /* SPID_SUBA_LET_END_DEF */
/* 011 */ nubLetSubALetEndOk,                /* SPID_SUBA_LET_END_OK */
/* 012 */ nubLetSubALetEndNg,                /* SPID_SUBA_LET_END_NG */
/* 013 */ nubLetSubAUsbSrcOn,                /* SPID_SUBA_USB_SRC_ON */
/* 014 */ nubLetSubAUsbSrcOnCB,              /* SPID_SUBA_USB_SRC_ON_CB */
/* 015 */ nubLetSubAUsbStartDef,             /* SPID_SUBA_USB_START_DEF */
/* 016 */ nubLetSubAUsbStartOk,              /* SPID_SUBA_USB_START_OK */
/* 017 */ nubLetSubAUsbStartNg,              /* SPID_SUBA_USB_START_NG */
/* 018 */ nubLetSubAUsbSrcOff,               /* SPID_SUBA_USB_SRC_OFF */
/* 019 */ nubLetSubAUsbSrcOffCB,             /* SPID_SUBA_USB_SRC_OFF_CB */
/* 020 */ nubLetSubAUsbEndDef,               /* SPID_SUBA_USB_END_DEF */
/* 021 */ nubLetSubAUsbEndOk,                /* SPID_SUBA_USB_END_OK */
/* 022 */ nubLetSubAUsbEndNg,                /* SPID_SUBA_USB_END_NG */
/* 023 */ nubLetSubASpkInfoJudge,            /* SPID_SUBA_SPK_INFO_JUDGE */
/* 024 */ nubLetSubASpkStartDef,             /* SPID_SUBA_SPK_START_DEF */
/* 025 */ nubLetSubASpkStartOk,              /* SPID_SUBA_SPK_START_OK */
/* 026 */ nubLetSubASpkStartNg,              /* SPID_SUBA_SPK_START_NG */
/* 027 */ nubLetSubASpkEndJudge,             /* SPID_SUBA_SPK_END_JUDGE */
/* 028 */ nubLetSubASpkSndSet,               /* SPID_SUBA_SPK_SND_SET */
/* 029 */ nubLetSubASpkSndSetCB,             /* SPID_SUBA_SPK_SND_SET_CB */
/* 030 */ nubLetSubAUsbPlay,                 /* SPID_SUBA_USB_PLAY */
/* 031 */ nubLetSubAUsbPlayCB,               /* SPID_SUBA_USB_PLAY_CB */
/* 032 */ nubLetSubASpkDisp,                 /* SPID_SUBA_SPK_DISP */
/* 033 */ nubLetSubASpkResultWait,           /* SPID_SUBA_SPK_RESULT_WAIT */
/* 034 */ nubLetSubASpkResultDisp,           /* SPID_SUBA_SPK_RESULT_DISP */
/* 035 */ nubLetSubALetBlankDisp,            /* SPID_SUBA_LET_BLANK_DISP */
/* 036 */ nubLetSubAUsbPlayStop,             /* SPID_SUBA_USB_PLAY_STOP */
/* 037 */ nubLetSubAUsbPlayStopCB,           /* SPID_SUBA_USB_PLAY_STOP_CB */
/* 038 */ nubLetSubASpkChgNext,              /* SPID_SUBA_SPK_CHG_NEXT */
/* 039 */ nubLetSubASpkChgBack,              /* SPID_SUBA_SPK_CHG_BACK */
/* 040 */ nubLetSubASpkResultEep,            /* SPID_SUBA_SPK_RESULT_EEP */
/* 041 */ nubLetSubASpkEndOk,                /* SPID_SUBA_SPK_END_OK */
/* 042 */ nubLetSubASpkNoChkEep,             /* SPID_SUBA_SPK_NOCHK_EEP */
/* 043 */ nubLetSubASpkNoChkDisp,            /* SPID_SUBA_SPK_NOCHK_DISP */
/* 044 */ nubLetSubASpkEndNg,                /* SPID_SUBA_SPK_END_NG */
/* 045 */ nubLetSubABibiriPlay,              /* SPID_SUBA_BIBIRI_PLAY */
/* 046 */ nubLetSubABibiriPlayCB,            /* SPID_SUBA_BIBIRI_PLAY_CB */
/* 047 */ nubLetSubABibiriSndSet,            /* SPID_SUBA_BIBIRI_SND_SET */
/* 048 */ nubLetSubABibiriSndSetCB,          /* SPID_SUBA_BIBIRI_SND_SET_CB */
/* 049 */ nubLetSubABibiriDef,               /* SPID_SUBA_BIBIRI_DEF */
/* 050 */ nubLetSubABibiriStartOk,           /* SPID_SUBA_BIBIRI_START_OK */
/* 051 */ nubLetSubABibiriStartNg,           /* SPID_SUBA_BIBIRI_START_NG */
/* 052 */ nubLetSubAUsbSongSet,              /* SPID_SUBA_USB_SONG_SET */
/* 053 */ nubLetSubABibiriSetOk,             /* SPID_SUBA_BIBIRI_SET_OK */
/* 054 */ nubLetSubABibiriSetNg,             /* SPID_SUBA_BIBIRI_SET_NG */
/* 055 */ nubLetSubABibiriDisp,              /* SPID_SUBA_BIBIRI_DISP */
/* 056 */ nubLetSubABibiriResultWait,        /* SPID_SUBA_BIBIRI_RESULT_WAIT */
/* 057 */ nubLetSubABibiriResultDisp,        /* SPID_SUBA_BIBIRI_RESULT_DISP */
/* 058 */ nubLetSubABibiriResultEep,         /* SPID_SUBA_BIBIRI_RESULT_EEP */
/* 059 */ nubLetSubABibiriEndOk,             /* SPID_SUBA_BIBIRI_END_OK */
/* 060 */ nubLetSubABibiriNoChkEep,          /* SPID_SUBA_BIBIRI_NOCHK_EEP */
/* 061 */ nubLetSubABibiriNoChkDisp,         /* SPID_SUBA_BIBIRI_NOCHK_DISP */
/* 062 */ nubLetSubABibiriEndNg,             /* SPID_SUBA_BIBIRI_END_NG */
/* 063 */ nubLetSubABsmInitial,              /* SPID_SUBA_BSM_INITIAL */
/* 064 */ nubLetSubABsmInitialCB,            /* SPID_SUBA_BSM_INITIAL_CB */
/* 065 */ nubLetSubABsmInitialOk,            /* SPID_SUBA_BSM_INITIAL_OK */
/* 066 */ nubLetSubABsmInitialNg,            /* SPID_SUBA_BSM_INITIAL_NG */
/* 067 */ nubLetSubABsmTpSet,                /* SPID_SUBA_BSM_TP_SET */
/* 068 */ nubLetSubABsmTpSetCB,              /* SPID_SUBA_BSM_TP_SET_CB */
/* 069 */ nubLetSubABsmTpSetOk,              /* SPID_SUBA_BSM_TP_SET_OK */
/* 070 */ nubLetSubABsmTpSetNg,              /* SPID_SUBA_BSM_TP_SET_NG */
/* 071 */ nubLetSubABsmEnd,                  /* SPID_SUBA_BSM_END */
/* 072 */ nubLetSubABsmEndCB,                /* SPID_SUBA_BSM_END_CB */
/* 073 */ nubLetSubABsmEndOk,                /* SPID_SUBA_BSM_END_OK */
/* 074 */ nubLetSubABsmEndNg,                /* SPID_SUBA_BSM_END_NG */
/* 075 */ nubLetSubABsmRest,                 /* SPID_SUBA_BSM_REST */
/* 076 */ nubLetSubABsmRestCB,               /* SPID_SUBA_BSM_REST_CB */
/* 077 */ nubLetSubABsmRestOk,               /* SPID_SUBA_BSM_REST_OK */
/* 078 */ nubLetSubABsmRestNg,               /* SPID_SUBA_BSM_REST_NG */
/* 079 */ nubLetSubABeepStartChk,            /* SPID_SUBA_BEEP_START_CHK */
/* 080 */ nubLetSubABeepSrcOn,               /* SPID_SUBA_BEEP_SRC_ON */
/* 081 */ nubLetSubABeepSrcOnCB,             /* SPID_SUBA_BEEP_SRC_ON_CB */
/* 082 */ nubLetSubABeepDef,                 /* SPID_SUBA_BEEP_DEF */
/* 083 */ nubLetSubABeepStartOk,             /* SPID_SUBA_BEEP_START_OK */
/* 084 */ nubLetSubABeepStartNg,             /* SPID_SUBA_BEEP_START_NG */
/* 085 */ nubLetSubABeepEndJudge,            /* SPID_SUBA_BEEP_END_JUDGE */
/* 086 */ nubLetSubABeepChUpOk,              /* SPID_SUBA_BEEP_CH_UP_OK */
/* 087 */ nubLetSubABeepChUpNg,              /* SPID_SUBA_BEEP_CH_UP_NG */
/* 088 */ nubLetSubABeepSndSet,              /* SPID_SUBA_BEEP_SND_SET */
/* 089 */ nubLetSubABeepSndSetCB,            /* SPID_SUBA_BEEP_SND_SET_CB */
/* 090 */ nubLetSubABeepDisp,                /* SPID_SUBA_BEEP_DISP */
/* 091 */ nubLetSubABeepDispOk,              /* SPID_SUBA_BEEP_DISP_OK */
/* 092 */ nubLetSubABeepButtonChk,           /* SPID_SUBA_BEEP_BUTTON_CHK */
/* 093 */ nubLetSubABeepResultDisp,          /* SPID_SUBA_BEEP_RESULT_DISP */
/* 094 */ nubLetSubABeepChgNext,             /* SPID_SUBA_BEEP_CHG_NEXT */
/* 095 */ nubLetSubABeepChgBack,             /* SPID_SUBA_BEEP_CHG_BACK */
/* 096 */ nubLetSubABeepResultEep,           /* SPID_SUBA_BEEP_RESULT_EEP */
/* 097 */ nubLetSubABeepSrcOff,              /* SPID_SUBA_BEEP_SRC_OFF */
/* 098 */ nubLetSubABeepSrcOffCB,            /* SPID_SUBA_BEEP_SRC_OFF_CB */
/* 099 */ nubLetSubABeepEndOk,               /* SPID_SUBA_BEEP_END_OK */
/* 100 */ nubLetSubABeepNoChkEep,            /* SPID_SUBA_BEEP_NOCHK_EEP */
/* 101 */ nubLetSubABeepNoChkDisp,           /* SPID_SUBA_BEEP_NOCHK_DISP */
/* 102 */ nubLetSubABeepEndNg,               /* SPID_SUBA_BEEP_END_NG */
/* 103 */ nubLetSubAMemClrStart,             /* SPID_SUBA_MEM_CLR_START */
/* 104 */ nubLetSubAMemClrStartOk,           /* SPID_SUBA_MEM_CLR_START_OK */
/* 105 */ nubLetSubAMemClrStartNg,           /* SPID_SUBA_MEM_CLR_START_NG */
/* 106 */ nubLetSubAMemLetClr,               /* SPID_SUBA_MEM_LET_CLR */
/* 107 */ nubLetSubAMemLetClrOk,             /* SPID_SUBA_MEM_LET_CLR_OK */
/* 108 */ nubLetSubAMemLetClrNg,             /* SPID_SUBA_MEM_LET_CLR_NG */
/* 109 */ nubLetSubAMemErrorClr,             /* SPID_SUBA_MEM_EEROR_CLR */
/* 110 */ nubLetSubAMemInitOk,               /* SPID_SUBA_MEM_INIT_OK */
/* 111 */ nubLetSubAMemInitNg,               /* SPID_SUBA_MEM_INIT_NG */
/* 112 */ nubLetSubAMemInlineClr,            /* SPID_SUBA_MEM_INLINE_CLR */
/* 113 */ nubLetSubAMemInlineClrOk,          /* SPID_SUBA_MEM_INLINE_CLR_OK */
/* 114 */ nubLetSubAMemInlineClrNg,          /* SPID_SUBA_MEM_INLINE_CLR_NG */
/* 115 */ nubLetSubAMemClrEnd,               /* SPID_SUBA_MEM_CLR_END */
/* 116 */ nubLetSubAMemClrEndOk,             /* SPID_SUBA_MEM_CLR_END_OK */
/* 117 */ nubLetSubAMemClrEndNg,             /* SPID_SUBA_MEM_CLR_END_NG */
/* 118 */ nubLetSubASndBassDef,              /* SPID_SUBA_SND_BASS_DEF */
/* 119 */ nubLetSubASndBassDefCB,            /* SPID_SUBA_SND_BASS_DEF_CB */
/* 120 */ nubLetSubASndTrebleDef,            /* SPID_SUBA_SND_TREBLE_DEF */
/* 121 */ nubLetSubASndTrebleDefCB,          /* SPID_SUBA_SND_TREBLE_DEF_CB */
/* 122 */ nubLetSubASndBackRest,             /* SPID_SUBA_SND_BACK_REST */
/* 123 */ nubLetSubASndVolSet,               /* SPID_SUBA_SND_VOL_SET */
/* 124 */ nubLetSubASndVolSetCB,             /* SPID_SUBA_SND_VOL_SET_CB */
/* 125 */ nubLetSubASndVolSetOk,             /* SPID_SUBA_SND_VOL_SET_OK */
/* 126 */ nubLetSubASndVolSetNg,             /* SPID_SUBA_SND_VOL_SET_NG */
/* 127 */ nubLetSubASndBassSet,              /* SPID_SUBA_SND_BASS_SET */
/* 128 */ nubLetSubASndBassSetCB,            /* SPID_SUBA_SND_BASS_SET_CB */
/* 129 */ nubLetSubASndBassSetOk,            /* SPID_SUBA_SND_BASS_SET_OK */
/* 130 */ nubLetSubASndBassSetNg,            /* SPID_SUBA_SND_BASS_SET_NG */
/* 131 */ nubLetSubASndTrebleSet,            /* SPID_SUBA_SND_TREBLE_SET */
/* 132 */ nubLetSubASndTrebleSetCB,          /* SPID_SUBA_SND_TREBLE_SET_CB */
/* 133 */ nubLetSubASndTrebleSetOk,          /* SPID_SUBA_SND_TREBLE_SET_OK */
/* 134 */ nubLetSubASndTrebleSetNg,          /* SPID_SUBA_SND_TREBLE_SET_NG */
/* 135 */ nubLetSubALetSpeakerDisp,          /* SPID_SUBA_LET_SPEAKER_DISP */
/* 136 */ nubLetSubALetMuteOn,               /* SPID_SUBA_LET_MUTE_ON */
/* 137 */ nubLetSubALetMuteOff,              /* SPID_SUBA_LET_MUTE_OFF */
/* 138 */ nubLetSubALetBackButtonCheck,      /* SPID_SUBA_LET_BACK_BUTTON_CHECK */
/* 139 */ nubLetSubAERRMemClr,               /* SPID_SUBA_ERR_MEM_CLR */
/* 140 */ nubLetSubAERRMemClrOk,             /* SPID_SUBA_ERR_MEM_CLR_OK */
/* 141 */ nubLetSubAERRMemClrNg,             /* SPID_SUBA_ERR_MEM_CLR_NG */
/* 142 */ nubLetSubAWaitInfo                 /* SPID_SUBA_WAIT_INFO */
};

#if defined ( STACK_CHECK )
/********************************************************************************************/
/**	\function		nvdLetSubAProcessForStackCheck
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
static void nvdLetSubAProcessForStackCheck(void)
{
/* 000 */(void)nubLetSubALetStart();             
/* 001 */(void)nubLetSubALetStartCB();         
/* 002 */(void)nubLetSubALetStartDef();        
/* 003 */(void)nubLetSubALetStartOk();         
/* 004 */(void)nubLetSubALetStartNg();         
/* 005 */(void)nubLetSubAVspGet();              
/* 006 */(void)nubLetSubAVspGetCB();          
/* 007 */(void)nubLetSubAVspEep();              
/* 008 */(void)nubLetSubALetEnd();               
/* 009 */(void)nubLetSubALetEndCB();          
/* 010 */(void)nubLetSubALetEndDef();          
/* 011 */(void)nubLetSubALetEndOk();           
/* 012 */(void)nubLetSubALetEndNg();           
/* 013 */(void)nubLetSubAUsbSrcOn();          
/* 014 */(void)nubLetSubAUsbSrcOnCB();      
/* 015 */(void)nubLetSubAUsbStartDef();       
/* 016 */(void)nubLetSubAUsbStartOk();        
/* 017 */(void)nubLetSubAUsbStartNg();        
/* 018 */(void)nubLetSubAUsbSrcOff();          
/* 019 */(void)nubLetSubAUsbSrcOffCB();      
/* 020 */(void)nubLetSubAUsbEndDef();         
/* 021 */(void)nubLetSubAUsbEndOk();          
/* 022 */(void)nubLetSubAUsbEndNg();          
/* 023 */(void)nubLetSubASpkInfoJudge();     
/* 024 */(void)nubLetSubASpkStartDef();       
/* 025 */(void)nubLetSubASpkStartOk();        
/* 026 */(void)nubLetSubASpkStartNg();        
/* 027 */(void)nubLetSubASpkEndJudge();     
/* 028 */(void)nubLetSubASpkSndSet();        
/* 029 */(void)nubLetSubASpkSndSetCB();    
/* 030 */(void)nubLetSubAUsbPlay();             
/* 031 */(void)nubLetSubAUsbPlayCB();         
/* 032 */(void)nubLetSubASpkDisp();            
/* 033 */(void)nubLetSubASpkResultWait();    
/* 034 */(void)nubLetSubASpkResultDisp();    
/* 035 */(void)nubLetSubALetBlankDisp();      
/* 036 */(void)nubLetSubAUsbPlayStop();      
/* 037 */(void)nubLetSubAUsbPlayStopCB();  
/* 038 */(void)nubLetSubASpkChgNext();      
/* 039 */(void)nubLetSubASpkChgBack();      
/* 040 */(void)nubLetSubASpkResultEep();     
/* 041 */(void)nubLetSubASpkEndOk();         
/* 042 */(void)nubLetSubASpkNoChkEep();    
/* 043 */(void)nubLetSubASpkNoChkDisp();   
/* 044 */(void)nubLetSubASpkEndNg();          
/* 045 */(void)nubLetSubABibiriPlay();           
/* 046 */(void)nubLetSubABibiriPlayCB();       
/* 047 */(void)nubLetSubABibiriSndSet();       
/* 048 */(void)nubLetSubABibiriSndSetCB();  
/* 049 */(void)nubLetSubABibiriDef();            
/* 050 */(void)nubLetSubABibiriStartOk();      
/* 051 */(void)nubLetSubABibiriStartNg();      
/* 052 */(void)nubLetSubAUsbSongSet();       
/* 053 */(void)nubLetSubABibiriSetOk();        
/* 054 */(void)nubLetSubABibiriSetNg();        
/* 055 */(void)nubLetSubABibiriDisp();           
/* 056 */(void)nubLetSubABibiriResultWait();  
/* 057 */(void)nubLetSubABibiriResultDisp();  
/* 058 */(void)nubLetSubABibiriResultEep();   
/* 059 */(void)nubLetSubABibiriEndOk();        
/* 060 */(void)nubLetSubABibiriNoChkEep();  
/* 061 */(void)nubLetSubABibiriNoChkDisp(); 
/* 062 */(void)nubLetSubABibiriEndNg();        
/* 063 */(void)nubLetSubABsmInitial();          
/* 064 */(void)nubLetSubABsmInitialCB();      
/* 065 */(void)nubLetSubABsmInitialOk();      
/* 066 */(void)nubLetSubABsmInitialNg();      
/* 067 */(void)nubLetSubABsmTpSet();          
/* 068 */(void)nubLetSubABsmTpSetCB();     
/* 069 */(void)nubLetSubABsmTpSetOk();      
/* 070 */(void)nubLetSubABsmTpSetNg();      
/* 071 */(void)nubLetSubABsmEnd();             
/* 072 */(void)nubLetSubABsmEndCB();        
/* 073 */(void)nubLetSubABsmEndOk();         
/* 074 */(void)nubLetSubABsmEndNg();         
/* 075 */(void)nubLetSubABsmRest();            
/* 076 */(void)nubLetSubABsmRestCB();       
/* 077 */(void)nubLetSubABsmRestOk();        
/* 078 */(void)nubLetSubABsmRestNg();        
/* 079 */(void)nubLetSubABeepStartChk();    
/* 080 */(void)nubLetSubABeepSrcOn();        
/* 081 */(void)nubLetSubABeepSrcOnCB();    
/* 082 */(void)nubLetSubABeepDef();            
/* 083 */(void)nubLetSubABeepStartOk();      
/* 084 */(void)nubLetSubABeepStartNg();      
/* 085 */(void)nubLetSubABeepEndJudge();   
/* 086 */(void)nubLetSubABeepChUpOk();     
/* 087 */(void)nubLetSubABeepChUpNg();     
/* 088 */(void)nubLetSubABeepSndSet();       
/* 089 */(void)nubLetSubABeepSndSetCB();  
/* 090 */(void)nubLetSubABeepDisp();           
/* 091 */(void)nubLetSubABeepDispOk();       
/* 092 */(void)nubLetSubABeepButtonChk();  
/* 093 */(void)nubLetSubABeepResultDisp();  
/* 094 */(void)nubLetSubABeepChgNext();     
/* 095 */(void)nubLetSubABeepChgBack();    
/* 096 */(void)nubLetSubABeepResultEep();   
/* 097 */(void)nubLetSubABeepSrcOff();        
/* 098 */(void)nubLetSubABeepSrcOffCB();    
/* 099 */(void)nubLetSubABeepEndOk();        
/* 100 */(void)nubLetSubABeepNoChkEep();  
/* 101 */(void)nubLetSubABeepNoChkDisp(); 
/* 102 */(void)nubLetSubABeepEndNg();        
/* 103 */(void)nubLetSubAMemClrStart();      
/* 104 */(void)nubLetSubAMemClrStartOk();  
/* 105 */(void)nubLetSubAMemClrStartNg();  
/* 106 */(void)nubLetSubAMemLetClr();         
/* 107 */(void)nubLetSubAMemLetClrOk();     
/* 108 */(void)nubLetSubAMemLetClrNg();     
/* 109 */(void)nubLetSubAMemErrorClr();      
/* 110 */(void)nubLetSubAMemInitOk();         
/* 111 */(void)nubLetSubAMemInitNg();         
/* 112 */(void)nubLetSubAMemInlineClr();     
/* 113 */(void)nubLetSubAMemInlineClrOk(); 
/* 114 */(void)nubLetSubAMemInlineClrNg(); 
/* 115 */(void)nubLetSubAMemClrEnd();        
/* 116 */(void)nubLetSubAMemClrEndOk();    
/* 117 */(void)nubLetSubAMemClrEndNg();    
/* 118 */(void)nubLetSubASndBassDef();       
/* 119 */(void)nubLetSubASndBassDefCB();   
/* 120 */(void)nubLetSubASndTrebleDef();     
/* 121 */(void)nubLetSubASndTrebleDefCB();
/* 122 */(void)nubLetSubASndBackRest();
/* 123 */(void)nubLetSubASndVolSet();         
/* 124 */(void)nubLetSubASndVolSetCB();     
/* 125 */(void)nubLetSubASndVolSetOk();     
/* 126 */(void)nubLetSubASndVolSetNg();      
/* 127 */(void)nubLetSubASndBassSet();       
/* 128 */(void)nubLetSubASndBassSetCB();   
/* 129 */(void)nubLetSubASndBassSetOk();   
/* 130 */(void)nubLetSubASndBassSetNg();   
/* 131 */(void)nubLetSubASndTrebleSet();     
/* 132 */(void)nubLetSubASndTrebleSetCB(); 
/* 133 */(void)nubLetSubASndTrebleSetOk(); 
/* 134 */(void)nubLetSubASndTrebleSetNg();
/* 135 */(void)nubLetSubALetSpeakerDisp();
/* 136 */(void)nubLetSubALetMuteOn();
/* 137 */(void)nubLetSubALetMuteOff();
/* 138 */(void)nubLetSubALetBackButtonCheck();
/* 139 */(void)nubLetSubAERRMemClr();
/* 140 */(void)nubLetSubAERRMemClrOk();
/* 141 */(void)nubLetSubAERRMemClrNg();
/* 142 */(void)nubLetSubAWaitInfo();
}
#endif
/*********************************************************************************************
*  function                 : wvdLetAppSubA_SetupProEng
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
void wvdLetAppSubA_SetupProEng(void)
{
    (void)wubPESetUp(&nstLetAppSubACTB, &nstLetAppSubAProcBuf_a[LETAPP_NUM_0], nstLetAppSubAProc_Tbl, nstLetAppSubASubProc_Tbl);
}
/*********************************************************************************************
*  function                 : wubLetAppSubA_SendMail
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
void wubLetAppSubA_SendMail(Type_uHWord auhEventCode,void* avdBuffer_p, Type_uWord auwSize)
{
	Type_sByte asbResult;

	asbResult = RES_NG;
	
	asbResult = wsbRPCSendMsg(ARPC_ID_LET,ARPC_ID_LET_SUBA,auhEventCode, avdBuffer_p, auwSize);
	if(asbResult != RES_OK){
		LETAPP("wubLetAppSubA_SendMail NG");
	}

}
/*********************************************************************************************
*  function                 : wvdLetAppSubAControl
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
void wvdLetAppSubAControl(void)
{
    /* Auto variable Define */
    Type_uHWord auhMainProcPID;     /* <AUTO> Procedure PID */
    Type_uByte aubProcIsExecute;    /* <AUTO> Procedure Status */
    Type_uByte aubProcResult;       /* <AUTO> Procedure Result */
    Type_uByte aubLetSpkBtnInfo;

    /* Initalize */
    aubProcIsExecute    = STD_FALSE;
    aubProcResult       = PERC_NO_EXECUTE_PROC;
    auhMainProcPID      = PID_SUBA_PROC_MAX;
    aubLetSpkBtnInfo    = LET_SPK_BTN_INVALID;
    
    /* Setting Request for Stand-by Process */
    wvdPESetReq(&nstLetAppSubACTB);

    if ((LET_SUBA_EV_INVALID < wstLetAppSubAMail_p->uhEventCode) && (LET_SUBA_EV_MAX > wstLetAppSubAMail_p->uhEventCode)){

        /* Process Execute status check */
        aubProcIsExecute = wubPEIsExecuteing(&nstLetAppSubACTB);

        if ((STD_TRUE == aubProcIsExecute) && (wstLetAppSubAMail_p->uhEventCode != LET_SUBA_EV_LET_START) && (wstLetAppSubAMail_p->uhEventCode != LET_SUBA_EV_LET_END)){
            /* No Action */
        }
        else { /* No Process Execute */
            if ((wstLetAppSubAMail_p->vdQueueBuffer_p != OS_NULL) && (wstLetAppSubAMail_p->uhEventCode != LET_SUBA_EV_SPK_BEEP_BUTTON)){
                nvdLetAppSubACmdSave(wstLetAppSubAMail_p->vdQueueBuffer_p, wstLetAppSubAMail_p->uwQueueSize);
            }
            else {
                /* No Action */
            }
            auhMainProcPID = (Type_uHWord)nubLetSubApEvPidChg(); /* Get PE Execute PID */

            if(PID_SUBA_PROC_MAX > auhMainProcPID){ /* Check PE PID */

                wvdPEResetProcess(&nstLetAppSubACTB); /* PE Reset */

                wvdPESetProcess(auhMainProcPID, &nstLetAppSubACTB); /* PE Set */

                aubProcResult = wubPEExecute(&nstLetAppSubACTB); /* PE Execute */
#if defined ( STACK_CHECK )
		  nvdLetSubAProcessForStackCheck();
#endif
                /* Getting wait time */
                if ((PERC_NO_EXECUTE_PROC != aubProcResult) && (WTID_MAX > aubProcResult)){

                    wswLetAppSubATmout = nswLetSubAPETimer_Tbl[aubProcResult]; /* PE Wait Timer Get */

                    wuwLetSubAStartTime = OSAL_GetOSTimeTick(); /* Timer Caculate Start */
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
    else if ((LET_SUBA_EV_INVALID_RES < wstLetAppSubAMail_p->uhEventCode) && (LET_SUBA_EV_MAX_RES > wstLetAppSubAMail_p->uhEventCode)){

        if (LET_SUBA_EV_SPK_BUTTON_RES == wstLetAppSubAMail_p->uhEventCode){
    
            if ((wstLetAppSubAMail_p->vdQueueBuffer_p) != OS_NULL){

                aubLetSpkBtnInfo = *((Type_uByte*)(wstLetAppSubAMail_p->vdQueueBuffer_p)); /* Let Button Info Get */
        
                if (LET_SPK_BTN_BACK == aubLetSpkBtnInfo){
                    nubBackState = LETAPP_NUM_1;
                }
                else{
                    /*No Action*/
                }
    			
            }
            else{
                /*No Action*/
            }
        }
        else{
            /*No Action*/
        }

        aubProcIsExecute = wubPEIsExecuteing(&nstLetAppSubACTB); /* Process Execute status check */

        if (STD_TRUE == aubProcIsExecute){

            aubProcResult = wubPEExecute(&nstLetAppSubACTB);
            /* Getting wait time */
            if ((PERC_NO_EXECUTE_PROC != aubProcResult) && (WTID_MAX > aubProcResult)){

                wswLetAppSubATmout = nswLetSubAPETimer_Tbl[aubProcResult]; /* PE Wait Timer Get */

                wuwLetSubAStartTime = OSAL_GetOSTimeTick(); /* Timer Caculate Start */
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
    if (wswLetAppSubATmout != OS_TOUT_FOREVER){
        /* Get Current Time */
        wuwLetAppSubANowTime = OSAL_GetOSTimeTick();
        /* Get Passed Time */
        wswLetAppSubAPassTime = wswLetApplGetProcTime(wuwLetSubAStartTime, wuwLetAppSubANowTime);
        if (wswLetAppSubAPassTime < wswLetAppSubATmout){
            /* Timeout time caculate */
            wswLetAppSubATmout = (wswLetAppSubATmout - wswLetAppSubAPassTime);
        }
        else{
            /* Timeout time 0 */
            wswLetAppSubATmout = LETAPP_NUM_0;
        }
    }
    else{
        /* No Action */
    }

    /* Timeout time caculate Start */
    wuwLetSubAStartTime = OSAL_GetOSTimeTick();

}
/*********************************************************************************************
*  function                 : nvdLetAppSubACmdSave
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
static void nvdLetAppSubACmdSave(Type_uByte* aubDataBuff_p, Type_uByte aubSize)
{
    if (aubSize >= sizeof(wubLetAppSubACmdData)){
        wubLetAppSubACmdSize = sizeof(wubLetAppSubACmdData);
        wvdGen_MemCpy(&wubLetAppSubACmdData[LETAPP_NUM_0], aubDataBuff_p, sizeof(wubLetAppSubACmdData));
    }
    else {
        wubLetAppSubACmdSize = aubSize;
        wvdGen_MemCpy(&wubLetAppSubACmdData[LETAPP_NUM_0], aubDataBuff_p, aubSize);
    }
}
/*********************************************************************************************
*  function                 : nubLetSubApEvPidChg
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
static Type_uByte nubLetSubApEvPidChg(void)
{
    /* Auto variable Define */
    Type_uByte aubProcPID;
    Type_uByte aubTempNum;

    /* Initalize */
    aubProcPID = PID_SUBA_PROC_MAX;
    aubTempNum = LETAPP_NUM_0;

    /* Check Event Code */
    if ((OS_NULL != wstLetAppSubAMail_p) && (OS_NULL != wstLetAppSubAMail_p->vdQueueBuffer_p)){
        while((aubTempNum < PID_SUBA_PROC_MAX) && (nstLetSubAEvChg_a[aubTempNum].ubInputEvent != wstLetAppSubAMail_p->uhEventCode)){
            aubTempNum++;
        }
        aubProcPID = nstLetSubAEvChg_a[aubTempNum].ubProcessID;
    }
    else {
        /* No Action */
    }

    return aubProcPID;
}
/*********************************************************************************************
*  function                 : nubLetSubALetStart
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
static Type_uByte nubLetSubALetStart(void)
{
    Type_uByte aubReturn;
    Type_sByte asbResult;

    aubReturn = PE_SEC1;
    asbResult = RES_NG;

    asbResult = wsbRPCSendMsg(ARPC_ID_LET_SUBA,ARPC_ID_HMI,EV_INF_LET_AUDIO_DIAG_START_REQ, OS_NULL, LETAPP_NUM_0);

    if (RES_OK == asbResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubALetStartCB
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
static Type_uByte nubLetSubALetStartCB(void)
{
    Type_uByte aubReturn;
    aubReturn = PE_SEC1;

    if (LET_SUBA_EV_LET_START_RES == wstLetAppSubAMail_p->uhEventCode){
        aubReturn = PE_SEC0;
    }
    else if (LET_SUBA_EV_TIMEOUT == wstLetAppSubAMail_p->uhEventCode){
        aubReturn = PE_SEC1;
    }
    else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubALetStartDef
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
static Type_uByte nubLetSubALetStartDef(void)
{
    Type_uByte aubReturn;

    aubReturn = PE_SEC0;

    wvdLetAppComInit();
    wubLetAppComModeSts = LET_MODE_WAIT;

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubALetStartOk
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
static Type_uByte nubLetSubALetStartOk(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_SUCCESS;
    astLetRes.ubResSize = LETAPP_NUM_0;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];
    astLetRes.ubResData[LETAPP_NUM_0] = wubLetAppSubACmdData[LETAPP_NUM_1];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_AUDIO_DIAG_START, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubALetStartNg
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
static Type_uByte nubLetSubALetStartNg(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_FAILURE;
    astLetRes.ubResData[LETAPP_NUM_0] = KLINEDRV_RES_REJECT;
    astLetRes.ubResSize = LETAPP_NUM_1;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_AUDIO_DIAG_START, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubAVspGet
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
static Type_uByte nubLetSubAVspGet(void)
{
    Type_uByte aubReturn;
    Type_uByte aubResult;

    aubResult = RES_NG;
    aubReturn = PE_SEC1;

    //LXQADD aubResult = BCAN_Ioctl(IOCTL_BCAN_LET_GET_VSP, OS_NULL);

    if (RES_OK == aubResult){
      aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubAVspGetCB
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
static Type_uByte nubLetSubAVspGetCB(void)
{
    Type_uByte aubReturn;
    aubReturn = PE_SEC1;

    if (LET_SUBA_EV_VSP_RES == wstLetAppSubAMail_p->uhEventCode){
        aubReturn = PE_SEC0;
    }
    else if (LET_SUBA_EV_TIMEOUT == wstLetAppSubAMail_p->uhEventCode){
        aubReturn = PE_SEC1;
    }
    else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubAVspEep
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
static Type_uByte nubLetSubAVspEep(void)
{
    Type_uByte aubReturn;
    Type_uByte aubVspValue;
    Type_uByte aubLetVspRes;

    aubReturn = PE_SEC1;
    aubVspValue = LETAPP_NUM_0;
    aubLetVspRes = LETAPP_NOMAL_NG;
	//for error will delete
	if(aubLetVspRes){;}
    if (wstLetAppSubAMail_p->vdQueueBuffer_p != OS_NULL){
        aubVspValue = *((Type_uByte*)wstLetAppSubAMail_p->vdQueueBuffer_p);
        if (aubVspValue >= LETAPP_NUM_3){ /* VSP Value Check */
       		aubLetVspRes = LETAPP_NOMAL_OK;
			
       //LXQADD (void)wubEEP_BlockWrite(LET_EEP_ADD_VOL_VSP, LETAPP_NUM_1, &aubLetVspRes, EEP_FORCE_WRITE);
            aubReturn = PE_SEC0;
        }
        else {
       		aubLetVspRes = LETAPP_NOMAL_NG;
            //LXQADD (void)wubEEP_BlockWrite(LET_EEP_ADD_VOL_VSP, LETAPP_NUM_1, &aubLetVspRes, EEP_FORCE_WRITE);
            aubReturn = PE_SEC0;
        }
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubALetEnd
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
static Type_uByte nubLetSubALetEnd(void)
{
    Type_uByte aubReturn;
    Type_sByte asbResult;

    asbResult = RES_NG;
    aubReturn = PE_SEC1;

    asbResult = wsbRPCSendMsg(ARPC_ID_LET_SUBA,ARPC_ID_HMI,EV_INF_LET_AUDIO_DIAG_END_REQ, OS_NULL, LETAPP_NUM_0);

    if (RES_OK == asbResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubALetEndCB
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
static Type_uByte nubLetSubALetEndCB(void)
{
    Type_uByte aubReturn;
    aubReturn = PE_SEC1;

    if (LET_SUBA_EV_LET_END_RES == wstLetAppSubAMail_p->uhEventCode){
        aubReturn = PE_SEC0;
    }
    else if (LET_SUBA_EV_TIMEOUT == wstLetAppSubAMail_p->uhEventCode){
        aubReturn = PE_SEC1;
    }
    else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubALetEndDef
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
static Type_uByte nubLetSubALetEndDef(void)
{
    /* Let Mode Status Default */
    wvdLetAppComInit();
    wubLetAppComModeSts = LET_MODE_WAIT;

    return PE_SEC0;
}
/*********************************************************************************************
*  function                 : nubLetSubALetEndOk
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
static Type_uByte nubLetSubALetEndOk(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_SUCCESS;
    astLetRes.ubResSize = LETAPP_NUM_0;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_AUDIO_DIAG_END, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubALetEndNg
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
static Type_uByte nubLetSubALetEndNg(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_FAILURE;
    astLetRes.ubResData[LETAPP_NUM_0] = KLINEDRV_RES_REJECT;
    astLetRes.ubResSize = LETAPP_NUM_1;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_AUDIO_DIAG_END, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubAUsbSrcOn
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
static Type_uByte nubLetSubAUsbSrcOn(void)
{
    Type_uByte aubReturn;
    Type_sByte asbResult;

    asbResult = RES_NG;
    aubReturn = PE_SEC1;

    asbResult = wsbRPCSendMsg(ARPC_ID_LET_SUBA,ARPC_ID_HMI,EV_LET_USB_SPK_SRC_ON, OS_NULL, LETAPP_NUM_0);

    if (RES_OK == asbResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubAUsbSrcOnCB
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
static Type_uByte nubLetSubAUsbSrcOnCB(void)
{
    Type_uByte aubReturn;

    aubReturn = PE_SEC1;

    if (LET_SUBA_EV_USB_SRC_ON_RES == wstLetAppSubAMail_p->uhEventCode){
        aubReturn = PE_SEC0;
    }
    else if (LET_SUBA_EV_TIMEOUT == wstLetAppSubAMail_p->uhEventCode){
        aubReturn = PE_SEC1;
    }
    else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubAUsbStartDef
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
static Type_uByte nubLetSubAUsbStartDef(void)
{
    //LXQADD wubEEP_BlockRead(LET_EEP_ADD_SND_BASS, LETAPP_NUM_1, &wubLetSndBass);
    //LXQADD wubEEP_BlockRead(LET_EEP_ADD_SND_TREBLE, LETAPP_NUM_1, &wubLetSndTreble);

    return PE_SEC0;
}
/*********************************************************************************************
*  function                 : nubLetSubAUsbStartOk
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
static Type_uByte nubLetSubAUsbStartOk(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_SUCCESS;
    astLetRes.ubResSize = LETAPP_NUM_0;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];
    astLetRes.ubResData[LETAPP_NUM_0] = wubLetAppSubACmdData[LETAPP_NUM_1];
    astLetRes.ubResData[LETAPP_NUM_1] = wubLetAppSubACmdData[LETAPP_NUM_2];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_USB_TEST_START, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubAUsbStartNg
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
static Type_uByte nubLetSubAUsbStartNg(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_FAILURE;
    astLetRes.ubResData[LETAPP_NUM_0] = KLINEDRV_RES_REJECT;
    astLetRes.ubResSize = LETAPP_NUM_1;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_USB_TEST_START, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubAUsbSrcOff
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
static Type_uByte nubLetSubAUsbSrcOff(void)
{
    Type_uByte aubReturn;
    Type_sByte asbResult;

    asbResult = RES_NG;
    aubReturn = PE_SEC1;

    asbResult = wsbRPCSendMsg(ARPC_ID_LET_SUBA,ARPC_ID_HMI,EV_LET_USB_SPK_SRC_OFF, OS_NULL, LETAPP_NUM_0);

    if (RES_OK == asbResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubAUsbSrcOffCB
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
static Type_uByte nubLetSubAUsbSrcOffCB(void)
{
    Type_uByte aubReturn;

    aubReturn = PE_SEC1;

    if (LET_SUBA_EV_USB_SRC_OFF_RES == wstLetAppSubAMail_p->uhEventCode){
        aubReturn = PE_SEC0;
    }
    else if (LET_SUBA_EV_TIMEOUT == wstLetAppSubAMail_p->uhEventCode){
        aubReturn = PE_SEC1;
    }
    else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubAUsbEndDef
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
static Type_uByte nubLetSubAUsbEndDef(void)
{

    /* Debug Message */

    return PE_SEC0;
}
/*********************************************************************************************
*  function                 : nubLetSubAUsbEndOk
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
static Type_uByte nubLetSubAUsbEndOk(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_SUCCESS;
    astLetRes.ubResSize = LETAPP_NUM_0;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];
    astLetRes.ubResData[LETAPP_NUM_0] = wubLetAppSubACmdData[LETAPP_NUM_1];
    astLetRes.ubResData[LETAPP_NUM_1] = wubLetAppSubACmdData[LETAPP_NUM_2];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_USB_TEST_END, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubAUsbEndNg
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
static Type_uByte nubLetSubAUsbEndNg(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_FAILURE;
    astLetRes.ubResData[LETAPP_NUM_0] = KLINEDRV_RES_REJECT;
    astLetRes.ubResSize = LETAPP_NUM_1;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_USB_TEST_END, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubASpkInfoJudge
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
static Type_uByte nubLetSubASpkInfoJudge(void)
{
    Type_uByte aubReturn;
    Type_uByte aubSpkChkRes;
    Type_uByte *aubLetData_p;

    aubReturn = PE_SEC1;
    aubSpkChkRes = RES_NG;
    aubLetData_p = OS_NULL;

    wuhLetSpkConnectInfo = LETAPP_NUM_0;
    aubLetData_p = wstLetAppSubAMail_p->vdQueueBuffer_p;

    if (aubLetData_p != OS_NULL){
        if ((wstLetAppSubAMail_p->uwQueueSize == LETAPP_NUM_4) && (aubLetData_p[LETAPP_NUM_3] != LETAPP_NUM_0)){
            
            aubSpkChkRes = nubLetSubASpkChkInfo(aubLetData_p[LETAPP_NUM_3]);
            if (RES_OK == aubSpkChkRes){
                aubReturn = PE_SEC0;
            }
            else {
                /* No Action */
            }
        }
        else {
            wuhLetSpkConnectInfo = LET_SPK_CONNECT_INFO;
            aubReturn = PE_SEC0;
        }
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubASpkChkInfo
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
static Type_uByte nubLetSubASpkChkInfo(Type_uByte aubTargetSpk)
{
    Type_uByte aubReturn;

    aubReturn = RES_NG;
    /* Let Target Speaker FR_L_R Check */
    if ((aubTargetSpk & LET_SPK_TARGET_FR_L_R) == LET_SPK_TARGET_FR_L_R){
        wuhLetSpkConnectInfo = LET_SPK_CONNECT_FR_L | LET_SPK_CONNECT_FR_R;
        aubReturn = RES_OK;
    }
    else{
        /* No Action */
    }
    /* Let Target Speaker RR_L_R Check */
    if ((aubTargetSpk & LET_SPK_TARGET_RR_L_R) == LET_SPK_TARGET_RR_L_R){
        wuhLetSpkConnectInfo = LET_SPK_CONNECT_RR_L | LET_SPK_CONNECT_RR_R;
        aubReturn = RES_OK;
    }
    else{
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubASpkStartDef
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
static Type_uByte nubLetSubASpkStartDef(void)
{
    Type_uByte aubEepResult;
    Type_uByte aubReturn;
    
    aubEepResult = RES_NG;
    aubReturn = PE_SEC1;

    nubBackState = LETAPP_NUM_0;

    /* USB Speaker Eeprom Default */
    wubLetSubATempBuff[LETAPP_NUM_0] = LET_SPK_DEF_EEP_1;
    wubLetSubATempBuff[LETAPP_NUM_1] = LET_SPK_DEF_EEP_2;
    wubLetSubATempBuff[LETAPP_NUM_2] = LET_SPK_DEF_EEP_3;
    wubLetSubATempBuff[LETAPP_NUM_3] = LET_SPK_DEF_EEP_4;

    //LXQADD aubEepResult = wubEEP_BlockWrite(LET_EEP_ADD_USB_SPK_REST, LETAPP_NUM_4, &wubLetSubATempBuff[LETAPP_NUM_0], EEP_FORCE_WRITE);

    if (RES_OK == aubEepResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    wubLetSpkIndex = LETAPP_NUM_0; /* Speaker Check Index Init */

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubASpkStartOk
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
static Type_uByte nubLetSubASpkStartOk(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_SUCCESS;
    astLetRes.ubResSize = LETAPP_NUM_0;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];
    astLetRes.ubResData[LETAPP_NUM_0] = wubLetAppSubACmdData[LETAPP_NUM_1];
    astLetRes.ubResData[LETAPP_NUM_1] = wubLetAppSubACmdData[LETAPP_NUM_2];
    
    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_SPK_CHK_START, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubASpkStartNg
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
static Type_uByte nubLetSubASpkStartNg(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_FAILURE;
    astLetRes.ubResData[LETAPP_NUM_0] = KLINEDRV_RES_REJECT;
    astLetRes.ubResSize = LETAPP_NUM_1;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_SPK_CHK_START, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubASpkEndJudge
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
static Type_uByte nubLetSubASpkEndJudge(void)
{
    Type_uByte aubReturn;
    
    aubReturn = PE_SEC1;

    while ((wubLetSpkIndex < LETAPP_NUM_4) && ((wuhLetSpkConnectInfo & nstLetSubASpkInfo_a[wubLetSpkIndex].ubSpkConInfo) == LETAPP_NUM_0)){
        wubLetSpkIndex++;
    }

    if (wubLetSpkIndex < LETAPP_NUM_4){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubASpkSndSet
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
static Type_uByte nubLetSubASpkSndSet(void)
{
    Type_uByte aubReturn;
    Type_uByte aubResult;
    Type_uByte aubSpkType;

    aubReturn = PE_SEC1;
    aubResult = RES_NG;
    aubSpkType = SND_LET_SPEAKER_ALL;
	//for error will delete
	if(aubSpkType){;}
    aubSpkType = nstLetSubASpkInfo_a[wubLetSpkIndex].ubSpkType;

    //LXQADD aubResult = wubSoundStm_EventNotice(TSK_ID_LETAPP_SUBA, SOUND_STM_EVENT_REQ_LET_SP_SELECT, &aubSpkType, LETAPP_NUM_1);

    if (RES_OK == aubResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubASpkSndSetCB
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
static Type_uByte nubLetSubASpkSndSetCB(void)
{
    Type_uByte aubReturn;

    aubReturn = PE_SEC1;

    if (LET_SUBA_EV_SPK_SND_SET_RES == wstLetAppSubAMail_p->uhEventCode){
        aubReturn = PE_SEC0;
    }
    else if (LET_SUBA_EV_TIMEOUT == wstLetAppSubAMail_p->uhEventCode){
        aubReturn = PE_SEC1;
    }
    else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubAUsbPlay
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
static Type_uByte nubLetSubAUsbPlay(void)
{
    Type_uByte aubReturn;
    Type_sByte asbResult;
    Type_uHWord auhTrackNo;

    asbResult = RES_NG;
    aubReturn = PE_SEC1;
    auhTrackNo = LETAPP_NUM_0;

    auhTrackNo = nstLetSubASpkInfo_a[wubLetSpkIndex].uhTrackNo;

    asbResult = wsbRPCSendMsg(ARPC_ID_LET_SUBA,ARPC_ID_USBSTM,EV_LET_USB_PLAY, &auhTrackNo, sizeof(Type_uHWord));

    if (RES_OK == asbResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubAUsbPlayCB
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
static Type_uByte nubLetSubAUsbPlayCB(void)
{
    Type_uByte aubReturn;
    aubReturn = PE_SEC1;

    if (LET_SUBA_EV_USB_PLAY_RES == wstLetAppSubAMail_p->uhEventCode){
        aubReturn = PE_SEC0;
    }
    else if (LET_SUBA_EV_TIMEOUT == wstLetAppSubAMail_p->uhEventCode){
        aubReturn = PE_SEC1;
    }
    else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubASpkDisp
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
static Type_uByte nubLetSubASpkDisp(void)
{
    Type_uByte aubReturn;
    Type_sByte asbResult;
    Type_uByte aubDispInfo;

    asbResult = RES_NG;
    aubReturn = PE_SEC1;
    aubDispInfo = nstLetSubASpkInfo_a[wubLetSpkIndex].ubSpkNoRest;

    asbResult = wsbRPCSendMsg(ARPC_ID_LET_SUBA,ARPC_ID_HMI,EV_LET_SPK_DISPLAY, &aubDispInfo, LETAPP_NUM_1);

    if (RES_OK == asbResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubASpkResultWait
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
static Type_uByte nubLetSubASpkResultWait(void)
{
    Type_uByte aubReturn;

    aubReturn = PE_SEC1;

    //LXQADD wubLetSpkBtnInfo = LET_SPK_BTN_INVALID;
    
    if (LET_SUBA_EV_SPK_BUTTON_RES == wstLetAppSubAMail_p->uhEventCode){
        if ((wstLetAppSubAMail_p->vdQueueBuffer_p) != OS_NULL){

            wubLetSpkBtnInfo = *((Type_uByte*)(wstLetAppSubAMail_p->vdQueueBuffer_p));

            if ((LET_SPK_BTN_OK == wubLetSpkBtnInfo) || (LET_SPK_BTN_NG == wubLetSpkBtnInfo)){
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
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubASpkResultDisp
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
static Type_uByte nubLetSubASpkResultDisp(void)
{
    Type_uByte aubReturn;
    Type_sByte asbResult;
    Type_uByte aubDispInfo;

    asbResult = RES_NG;
    aubReturn = PE_SEC1;
    aubDispInfo = LET_SPK_DISP_INVALID;

    if (LET_SPK_BTN_OK == wubLetSpkBtnInfo){ /* Speaker Check Result Ok */
        nvdLetSubASpkRestSave(nstLetSubASpkInfo_a[wubLetSpkIndex].ubSpkConInfo, LETAPP_RES_OK);
        aubDispInfo = nstLetSubASpkInfo_a[wubLetSpkIndex].ubSpkOkRest;
        asbResult = wsbRPCSendMsg(ARPC_ID_LET_SUBA,ARPC_ID_HMI,EV_LET_SPK_DISPLAY, &aubDispInfo, LETAPP_NUM_1);
    }
    else if (LET_SPK_BTN_NG == wubLetSpkBtnInfo){ /* Speaker Check Result Ng */
        nvdLetSubASpkRestSave(nstLetSubASpkInfo_a[wubLetSpkIndex].ubSpkConInfo, LETAPP_RES_NG);
        aubDispInfo = nstLetSubASpkInfo_a[wubLetSpkIndex].ubSpkNgRest;
        asbResult = wsbRPCSendMsg(ARPC_ID_LET_SUBA,ARPC_ID_HMI,EV_LET_SPK_DISPLAY, &aubDispInfo, LETAPP_NUM_1);
    }
    else {
        /* No Action */
    }
   
    if (RES_OK == asbResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nvdLetSubASpkRestSave
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
static void nvdLetSubASpkRestSave(Type_uByte aubSpkInfo, Type_uByte aubSpkRest)
{
    switch (aubSpkInfo){
        case LET_SPK_CONNECT_FR_L:
            wubLetSubATempBuff[LETAPP_NUM_0] = (wubLetSubATempBuff[LETAPP_NUM_0] & LETAPP_MASK_FC) | aubSpkRest;
            break;
        case LET_SPK_CONNECT_FR_R:
            wubLetSubATempBuff[LETAPP_NUM_0] = (wubLetSubATempBuff[LETAPP_NUM_0] & LETAPP_MASK_CF) | (aubSpkRest << LETAPP_NUM_4);
            break;
        case LET_SPK_CONNECT_RR_R:
            wubLetSubATempBuff[LETAPP_NUM_0] = (wubLetSubATempBuff[LETAPP_NUM_0] & LETAPP_MASK_3F) | (aubSpkRest << LETAPP_NUM_6);
            break;
        case LET_SPK_CONNECT_RR_L:
            wubLetSubATempBuff[LETAPP_NUM_1] = (wubLetSubATempBuff[LETAPP_NUM_1] & LETAPP_MASK_F3) | (aubSpkRest << LETAPP_NUM_2);
            break;
        default :
            break;
    }

}
/*********************************************************************************************
*  function                 : nubLetSubALetBlankDisp
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
static Type_uByte nubLetSubALetBlankDisp(void)
{
    Type_uByte aubReturn;
    Type_sByte asbResult;
    Type_uByte aubDispInfo;

    asbResult = RES_NG;
    aubReturn = PE_SEC1;
    aubDispInfo = LET_SPK_DISP_BLANK;

    asbResult = wsbRPCSendMsg(ARPC_ID_LET_SUBA,ARPC_ID_HMI,EV_LET_SPK_DISPLAY, &aubDispInfo, LETAPP_NUM_1);

    if (RES_OK == asbResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubAUsbPlayStop
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
static Type_uByte nubLetSubAUsbPlayStop(void)
{
    Type_uByte aubReturn;
    Type_sByte asbResult;
    
    asbResult = RES_NG;
    aubReturn = PE_SEC1;

    asbResult = wsbRPCSendMsg(ARPC_ID_LET_SUBA,ARPC_ID_USBSTM,EV_LET_USB_PLAY_STOP, OS_NULL, LETAPP_NUM_0);

    if (RES_OK == asbResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubAUsbPlayStopCB
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
static Type_uByte nubLetSubAUsbPlayStopCB(void)
{
    Type_uByte aubReturn;
    aubReturn = PE_SEC1;

    if (LET_SUBA_EV_USB_PLAYSTOP_RES == wstLetAppSubAMail_p->uhEventCode){
        aubReturn = PE_SEC0;
    }
    else if (LET_SUBA_EV_TIMEOUT == wstLetAppSubAMail_p->uhEventCode){
        aubReturn = PE_SEC1;
    }
    else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubASpkChgNext
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
static Type_uByte nubLetSubASpkChgNext(void)
{
    Type_uByte aubReturn;

    aubReturn = PE_SEC1;

    wubLetSpkIndex++;

    if (wubLetSpkIndex < LETAPP_NUM_4){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubASpkChgBack
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
static Type_uByte nubLetSubASpkChgBack(void)
{
    Type_uByte aubReturn;
    Type_uByte aubSpkIndex;

    aubReturn = PE_SEC1;
    aubSpkIndex = wubLetSpkIndex;

    nubBackState = LETAPP_NUM_0;

    if (aubSpkIndex > LETAPP_NUM_0){
        aubSpkIndex--;
        while ((aubSpkIndex > LETAPP_NUM_0) && (LETAPP_NUM_0 == (wuhLetSpkConnectInfo & nstLetSubASpkInfo_a[aubSpkIndex].ubSpkConInfo))){
            aubSpkIndex--;
        }

        if ((wuhLetSpkConnectInfo & nstLetSubASpkInfo_a[aubSpkIndex].ubSpkConInfo) != LETAPP_NUM_0){
            wubLetSpkIndex = aubSpkIndex;
            aubReturn = PE_SEC0;
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
*  function                 : nubLetSubASpkResultEep
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
static Type_uByte nubLetSubASpkResultEep(void)
{
    Type_uByte aubEepResult;
    Type_uByte aubReturn;

    aubEepResult = RES_NG;
    aubReturn = PE_SEC1;

    //LXQADD aubEepResult = wubEEP_BlockWrite(LET_EEP_ADD_USB_SPK_REST, LETAPP_NUM_4, &wubLetSubATempBuff[LETAPP_NUM_0], EEP_FORCE_WRITE);

    if (RES_OK == aubEepResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubASpkEndOk
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
static Type_uByte nubLetSubASpkEndOk(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_SUCCESS;
    astLetRes.ubResSize = LETAPP_NUM_0;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];
    astLetRes.ubResData[LETAPP_NUM_0] = wubLetAppSubACmdData[LETAPP_NUM_1];
    astLetRes.ubResData[LETAPP_NUM_1] = wubLetAppSubACmdData[LETAPP_NUM_2];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_SPK_CHK_END, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubASpkNoChkEep
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
static Type_uByte nubLetSubASpkNoChkEep(void)
{
    Type_uByte aubEepResult;
    Type_uByte aubReturn;

    aubEepResult = RES_NG;
    aubReturn = PE_SEC1;

    //LXQADD aubEepResult = wubEEP_BlockWrite(LET_EEP_ADD_USB_SPK_REST, LETAPP_NUM_4, &wubLetSubATempBuff[LETAPP_NUM_0], EEP_FORCE_WRITE);

    if (RES_OK == aubEepResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubASpkNoChkDisp
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
static Type_uByte nubLetSubASpkNoChkDisp(void)
{
    Type_uByte aubReturn;
    Type_sByte asbResult;
    Type_uByte aubDispInfo;

    aubReturn = PE_SEC1;
    asbResult = RES_NG;
    aubDispInfo = LET_SPK_DISP_NO_CHK;

    asbResult = wsbRPCSendMsg(ARPC_ID_LET_SUBA,ARPC_ID_HMI,EV_LET_SPK_DISPLAY, &aubDispInfo, LETAPP_NUM_1);

    if (RES_OK == asbResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubASpkEndNg
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
static Type_uByte nubLetSubASpkEndNg(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_FAILURE;
    astLetRes.ubResData[LETAPP_NUM_0] = KLINEDRV_RES_REJECT;
    astLetRes.ubResSize = LETAPP_NUM_1;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_SPK_CHK_END, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABibiriPlay
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
static Type_uByte nubLetSubABibiriPlay(void)
{
    Type_uByte aubReturn;
    Type_sByte asbResult;
    Type_uHWord auhTrackNo;

    asbResult = RES_NG;
    aubReturn = PE_SEC1;
    auhTrackNo = LETAPP_NUM_0;

    auhTrackNo = wuhLetBibiriSong;

    asbResult = wsbRPCSendMsg(ARPC_ID_LET_SUBA,ARPC_ID_USBSTM,EV_LET_USB_PLAY, &auhTrackNo, sizeof(Type_uHWord));

    if (RES_OK == asbResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABibiriPlayCB
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
static Type_uByte nubLetSubABibiriPlayCB(void)
{
    Type_uByte aubReturn;

    aubReturn = PE_SEC1;

    if (LET_SUBA_EV_USB_PLAY_RES == wstLetAppSubAMail_p->uhEventCode){
        aubReturn = PE_SEC0;
    }
    else if (LET_SUBA_EV_TIMEOUT == wstLetAppSubAMail_p->uhEventCode){
        aubReturn = PE_SEC1;
    }
    else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABibiriSndSet
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
static Type_uByte nubLetSubABibiriSndSet(void)
{
    //LXQADD Type_uByte aubSpkType;
    Type_uByte aubReturn;
    Type_uByte aubResult;

    aubReturn = PE_SEC1;
    aubResult = RES_NG;
    //LXQADD aubSpkType = SND_LET_SPEAKER_ALL;

    //LXQADD aubResult = wubSoundStm_EventNotice(TSK_ID_LETAPP_SUBA, SOUND_STM_EVENT_REQ_LET_SP_SELECT, &aubSpkType, LETAPP_NUM_1);

    if (RES_OK == aubResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABibiriSndSetCB
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
static Type_uByte nubLetSubABibiriSndSetCB(void)
{
    Type_uByte aubReturn;

    aubReturn = PE_SEC1;

    if (LET_SUBA_EV_SPK_SND_SET_RES == wstLetAppSubAMail_p->uhEventCode){
        aubReturn = PE_SEC0;
    }
    else if (LET_SUBA_EV_TIMEOUT == wstLetAppSubAMail_p->uhEventCode){
        aubReturn = PE_SEC1;
    }
    else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABibiriDef
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
static Type_uByte nubLetSubABibiriDef(void)
{
    wubLetSubATempBuff[LETAPP_NUM_4] = LETAPP_RES_NO_RESULT; /* Bibiri Result Initial */
    wuhLetBibiriSong = LETAPP_NUM_3;

    return PE_SEC0;
}
/*********************************************************************************************
*  function                 : nubLetSubABibiriStartOk
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
static Type_uByte nubLetSubABibiriStartOk(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_SUCCESS;
    astLetRes.ubResSize = LETAPP_NUM_0;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];
    astLetRes.ubResData[LETAPP_NUM_0] = wubLetAppSubACmdData[LETAPP_NUM_1];
    astLetRes.ubResData[LETAPP_NUM_1] = wubLetAppSubACmdData[LETAPP_NUM_2];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_SPK_BIBIRI_START, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABibiriStartNg
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
static Type_uByte nubLetSubABibiriStartNg(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_FAILURE;
    astLetRes.ubResData[LETAPP_NUM_0] = KLINEDRV_RES_REJECT;
    astLetRes.ubResSize = LETAPP_NUM_1;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_SPK_BIBIRI_START, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubAUsbSongSet
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
static Type_uByte nubLetSubAUsbSongSet(void)
{
    Type_uByte aubReturn;
    Type_uByte *aubLetData_p;

    aubReturn = PE_SEC1;
    aubLetData_p = OS_NULL;

    aubLetData_p = wstLetAppSubAMail_p->vdQueueBuffer_p;

    if ((aubLetData_p != OS_NULL) && (wstLetAppSubAMail_p->uwQueueSize == LETAPP_NUM_4)){
        wuhLetBibiriSong = aubLetData_p[LETAPP_NUM_3];
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABibiriSetOk
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
static Type_uByte nubLetSubABibiriSetOk(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_SUCCESS;
    astLetRes.ubResSize = LETAPP_NUM_0;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];
    astLetRes.ubResData[LETAPP_NUM_0] = wubLetAppSubACmdData[LETAPP_NUM_1];
    astLetRes.ubResData[LETAPP_NUM_1] = wubLetAppSubACmdData[LETAPP_NUM_2];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_SET_BIBIRI_CHECK, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABibiriSetNg
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
static Type_uByte nubLetSubABibiriSetNg(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_FAILURE;
    astLetRes.ubResData[LETAPP_NUM_0] = KLINEDRV_RES_REJECT;
    astLetRes.ubResSize = LETAPP_NUM_1;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_SET_BIBIRI_CHECK, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABibiriDisp
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
static Type_uByte nubLetSubABibiriDisp(void)
{
    Type_uByte aubReturn;
    Type_sByte asbResult;
    Type_uByte aubDispInfo;

    aubReturn = PE_SEC1;
    asbResult = RES_NG;
    aubDispInfo = LET_SPK_DISP_BIBIRI;

    asbResult = wsbRPCSendMsg(ARPC_ID_LET_SUBA,ARPC_ID_HMI,EV_LET_SPK_DISPLAY, &aubDispInfo, LETAPP_NUM_1);

    if (RES_OK == asbResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABibiriResultWait
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
static Type_uByte nubLetSubABibiriResultWait(void)
{
    Type_uByte aubReturn;

    aubReturn = PE_SEC1;

    //LXQADD wubLetSpkBtnInfo = LET_SPK_BTN_INVALID;
    
    if (LET_SUBA_EV_SPK_BUTTON_RES == wstLetAppSubAMail_p->uhEventCode){
        if ((wstLetAppSubAMail_p->vdQueueBuffer_p) != OS_NULL){

            wvdGen_MemCpy(&wubLetSpkBtnInfo, wstLetAppSubAMail_p->vdQueueBuffer_p, LETAPP_NUM_1);

            if ((LET_SPK_BTN_OK == wubLetSpkBtnInfo) || (LET_SPK_BTN_NG == wubLetSpkBtnInfo)){
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
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABibiriResultDisp
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
static Type_uByte nubLetSubABibiriResultDisp(void)
{
    Type_uByte aubReturn;
    Type_sByte asbResult;
    Type_uByte aubDispInfo;

    asbResult = RES_NG;
    aubReturn = PE_SEC1;
    aubDispInfo = LET_SPK_DISP_INVALID;

    if (LET_SPK_BTN_OK == wubLetSpkBtnInfo){
        wubLetSubATempBuff[LETAPP_NUM_4] = LETAPP_RES_OK;
        aubDispInfo = LET_SPK_DISP_BIBIRI_OK;
		asbResult = wsbRPCSendMsg(ARPC_ID_LET_SUBA,ARPC_ID_HMI,EV_LET_SPK_DISPLAY, &aubDispInfo, LETAPP_NUM_1);
    }
    else if (LET_SPK_BTN_NG == wubLetSpkBtnInfo){
        wubLetSubATempBuff[LETAPP_NUM_4] = LETAPP_RES_NG;
        aubDispInfo = LET_SPK_DISP_BIBIRI_NG;
		asbResult = wsbRPCSendMsg(ARPC_ID_LET_SUBA,ARPC_ID_HMI,EV_LET_SPK_DISPLAY, &aubDispInfo, LETAPP_NUM_1);
    }
    else {
        /* No Action */
    }
   
    if (RES_OK == asbResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABibiriResultEep
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
static Type_uByte nubLetSubABibiriResultEep(void)
{
    Type_uByte aubEepResult;
    Type_uByte aubReturn;

    aubEepResult = RES_NG;
    aubReturn = PE_SEC1;

    //LXQADD aubEepResult = wubEEP_BlockWrite(LET_EEP_ADD_BIBIRI_REST, LETAPP_NUM_1, &wubLetSubATempBuff[LETAPP_NUM_4], EEP_FORCE_WRITE);

    if (RES_OK == aubEepResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABibiriEndOk
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
static Type_uByte nubLetSubABibiriEndOk(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_SUCCESS;
    astLetRes.ubResSize = LETAPP_NUM_2;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];
    astLetRes.ubResData[LETAPP_NUM_0] = wubLetAppSubACmdData[LETAPP_NUM_1];
    astLetRes.ubResData[LETAPP_NUM_1] = wubLetAppSubACmdData[LETAPP_NUM_2];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_BIBIRI_CHECK_END, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABibiriNoChkEep
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
static Type_uByte nubLetSubABibiriNoChkEep(void)
{
    Type_uByte aubEepResult;
    Type_uByte aubReturn;

    aubEepResult = RES_NG;
    aubReturn = PE_SEC1;

    //LXQADD aubEepResult = wubEEP_BlockWrite(LET_EEP_ADD_BIBIRI_REST, LETAPP_NUM_1, &wubLetSubATempBuff[LETAPP_NUM_4], EEP_FORCE_WRITE);

    if (RES_OK == aubEepResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABibiriNoChkDisp
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
static Type_uByte nubLetSubABibiriNoChkDisp(void)
{
    Type_uByte aubReturn;
    Type_sByte asbResult;
    Type_uByte aubDispInfo;

    asbResult = RES_NG;
    aubReturn = PE_SEC1;
    aubDispInfo = LET_SPK_DISP_BIBIRI_NO_CHK;

    asbResult = wsbRPCSendMsg(ARPC_ID_LET_SUBA,ARPC_ID_HMI,EV_LET_SPK_DISPLAY, &aubDispInfo, LETAPP_NUM_1);

    if (RES_OK == asbResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABibiriEndNg
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
static Type_uByte nubLetSubABibiriEndNg(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_FAILURE;
    astLetRes.ubResData[LETAPP_NUM_0] = KLINEDRV_RES_REJECT;
    astLetRes.ubResSize = LETAPP_NUM_1;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_BIBIRI_CHECK_END, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABsmInitial
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
static Type_uByte nubLetSubABsmInitial(void)
{
    Type_uByte aubReturn;
    Type_sByte asbResult;

    asbResult = RES_NG;
    aubReturn = PE_SEC1;

    asbResult = wsbRPCSendMsg(ARPC_ID_LET_SUBA,ARPC_ID_LANEWATCH,EV_REQ_LET_BSM_INITIAL, OS_NULL, LETAPP_NUM_0);

    if (RES_OK == asbResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABsmInitialCB
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
static Type_uByte nubLetSubABsmInitialCB(void)
{
    Type_uByte aubReturn;

    aubReturn = PE_SEC1;

    if (LET_SUBA_EV_BSM_INIT_RES == wstLetAppSubAMail_p->uhEventCode){
        aubReturn = PE_SEC0;
    }
    else if (LET_SUBA_EV_TIMEOUT == wstLetAppSubAMail_p->uhEventCode){
        aubReturn = PE_SEC1;
    }
    else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABsmInitialOk
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
static Type_uByte nubLetSubABsmInitialOk(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_SUCCESS;
    astLetRes.ubResSize = LETAPP_NUM_2;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];
    astLetRes.ubResData[LETAPP_NUM_0] = wubLetAppSubACmdData[LETAPP_NUM_1];
    astLetRes.ubResData[LETAPP_NUM_1] = wubLetAppSubACmdData[LETAPP_NUM_2];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_BSM_INITIAL, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABsmInitialNg
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
static Type_uByte nubLetSubABsmInitialNg(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_FAILURE;
    astLetRes.ubResData[LETAPP_NUM_0] = KLINEDRV_RES_REJECT;
    astLetRes.ubResSize = LETAPP_NUM_1;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_BSM_INITIAL, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABsmTpSet
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
static Type_uByte nubLetSubABsmTpSet(void)
{
    Type_uByte aubReturn;
    Type_sByte asbResult;
    Type_uByte *aubBsmTp_p;

    asbResult = RES_NG;
    aubReturn = PE_SEC1;
    aubBsmTp_p = OS_NULL;

    if ((wstLetAppSubAMail_p->vdQueueBuffer_p!= OS_NULL) && (LETAPP_NUM_9 == (wstLetAppSubAMail_p->uwQueueSize))){

    	aubBsmTp_p = (Type_uByte*)(wstLetAppSubAMail_p->vdQueueBuffer_p);

        asbResult = wsbRPCSendMsg(ARPC_ID_LET_SUBA,ARPC_ID_LANEWATCH,EV_REQ_LET_BSM_START_TP_SET, &aubBsmTp_p[LETAPP_NUM_3], LETAPP_NUM_6);

        if (RES_OK == asbResult){
            aubReturn = PE_SEC0;
        }
        else {
            /* No Action */
        }
   }
   else {

   }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABsmTpSetCB
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
static Type_uByte nubLetSubABsmTpSetCB(void)
{
    Type_uByte aubReturn;
    aubReturn = PE_SEC1;

    if (LET_SUBA_EV_BSM_START_RES == wstLetAppSubAMail_p->uhEventCode){
        aubReturn = PE_SEC0;
    }
    else if (LET_SUBA_EV_TIMEOUT == wstLetAppSubAMail_p->uhEventCode){
        aubReturn = PE_SEC1;
    }
    else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABsmTpSetOk
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
static Type_uByte nubLetSubABsmTpSetOk(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_SUCCESS;
    astLetRes.ubResSize = LETAPP_NUM_2;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];
    astLetRes.ubResData[LETAPP_NUM_0] = wubLetAppSubACmdData[LETAPP_NUM_1];
    astLetRes.ubResData[LETAPP_NUM_1] = wubLetAppSubACmdData[LETAPP_NUM_2];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_BSM_START_TP_SET, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABsmTpSetNg
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
static Type_uByte nubLetSubABsmTpSetNg(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_FAILURE;
    astLetRes.ubResData[LETAPP_NUM_0] = KLINEDRV_RES_REJECT;
    astLetRes.ubResSize = LETAPP_NUM_1;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_BSM_START_TP_SET, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABsmEnd
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
static Type_uByte nubLetSubABsmEnd(void)
{
    Type_uByte aubReturn;
    Type_sByte asbResult;

    asbResult = RES_NG;
    aubReturn = PE_SEC1;

    asbResult = wsbRPCSendMsg(ARPC_ID_LET_SUBA,ARPC_ID_LANEWATCH,EV_REQ_LET_BSM_END, OS_NULL, LETAPP_NUM_0);

    if (RES_OK == asbResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABsmEndCB
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
static Type_uByte nubLetSubABsmEndCB(void)
{
    Type_uByte aubReturn;
    aubReturn = PE_SEC1;

    if (LET_SUBA_EV_BSM_END_RES == wstLetAppSubAMail_p->uhEventCode){
        aubReturn = PE_SEC0;
    }
    else if (LET_SUBA_EV_TIMEOUT == wstLetAppSubAMail_p->uhEventCode){
        aubReturn = PE_SEC1;
    }
    else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABsmEndOk
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
static Type_uByte nubLetSubABsmEndOk(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_SUCCESS;
    astLetRes.ubResSize = LETAPP_NUM_2;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];
    astLetRes.ubResData[LETAPP_NUM_0] = wubLetAppSubACmdData[LETAPP_NUM_1];
    astLetRes.ubResData[LETAPP_NUM_1] = wubLetAppSubACmdData[LETAPP_NUM_2];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_BSM_END, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABsmEndNg
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
static Type_uByte nubLetSubABsmEndNg(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_FAILURE;
    astLetRes.ubResData[LETAPP_NUM_0] = KLINEDRV_RES_REJECT;
    astLetRes.ubResSize = LETAPP_NUM_1;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_BSM_END, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABsmRest
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
static Type_uByte nubLetSubABsmRest(void)
{
    Type_uByte aubReturn;
    Type_sByte asbResult;

    asbResult = RES_NG;
    aubReturn = PE_SEC1;

    asbResult = wsbRPCSendMsg(ARPC_ID_LET_SUBA,ARPC_ID_LANEWATCH,EV_REQ_LET_BSM_RESULT, OS_NULL, LETAPP_NUM_0);

    if (RES_OK == asbResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABsmRestCB
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
static Type_uByte nubLetSubABsmRestCB(void)
{
    Type_uByte aubReturn;

    aubReturn = PE_SEC1;
    wvdGen_MemSet(&wubLetSubATempBuff[LETAPP_NUM_0], LETAPP_NUM_0, sizeof(wubLetSubATempBuff));

    if (LET_SUBA_EV_BSM_RESULT_RES == wstLetAppSubAMail_p->uhEventCode){
        if (((wstLetAppSubAMail_p->vdQueueBuffer_p) != OS_NULL) && (LETAPP_NUM_51 == (wstLetAppSubAMail_p->uwQueueSize))){
            wvdGen_MemCpy(&wubLetSubATempBuff[LETAPP_NUM_0], wstLetAppSubAMail_p->vdQueueBuffer_p, LETAPP_NUM_51);
            aubReturn = PE_SEC0;
        }
        else {
            /* No Action */
        }
    }
    else if (LET_SUBA_EV_TIMEOUT == wstLetAppSubAMail_p->uhEventCode){
        aubReturn = PE_SEC1;
    }
    else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABsmRestOk
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
static Type_uByte nubLetSubABsmRestOk(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_SUCCESS;
    astLetRes.ubResSize = LETAPP_NUM_52;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];
    astLetRes.ubResData[LETAPP_NUM_0] = wubLetAppSubACmdData[LETAPP_NUM_1];
    wvdGen_MemCpy(&(astLetRes.ubResData[LETAPP_NUM_1]), &wubLetSubATempBuff[LETAPP_NUM_0], LETAPP_NUM_51);

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_BSM_REST, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABsmRestNg
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
static Type_uByte nubLetSubABsmRestNg(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_FAILURE;
    astLetRes.ubResData[LETAPP_NUM_0] = KLINEDRV_RES_REJECT;
    astLetRes.ubResSize = LETAPP_NUM_1;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_BSM_REST, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABeepStartChk
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
static Type_uByte nubLetSubABeepStartChk(void)
{
    Type_uByte aubReturn;
    Type_uByte aubSpkChkRes;
    Type_uByte *aubLetData_p;

    aubReturn = PE_SEC1;
    aubSpkChkRes = RES_NG;
    aubLetData_p = OS_NULL;

    wuhLetSpkConnectInfo = LETAPP_NUM_0;
    aubLetData_p = wstLetAppSubAMail_p->vdQueueBuffer_p;

    if (aubLetData_p != OS_NULL){
        if ((wstLetAppSubAMail_p->uwQueueSize == LETAPP_NUM_4) && (aubLetData_p[LETAPP_NUM_3] != LETAPP_NUM_0)){
            
            aubSpkChkRes = nubLetSubASpkChkInfo(aubLetData_p[LETAPP_NUM_3]);
            if (RES_OK == aubSpkChkRes){
                aubReturn = PE_SEC0;
            }
            else {
                /* No Action */
            }
        }
        else {
            wuhLetSpkConnectInfo = LET_SPK_CONNECT_INFO;
            aubReturn = PE_SEC0;
        }
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABeepSrcOn
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
static Type_uByte nubLetSubABeepSrcOn(void)
{
    Type_uByte aubReturn;
    Type_sByte asbResult;
    
    asbResult = RES_NG;
    aubReturn = PE_SEC1;

   // asbResult = wsbRPCSendMsg(ARPC_ID_LET_SUBA,ARPC_ID_LANEWATCH,EV_LET_BEEP_SPK_SRC_ON, OS_NULL, LETAPP_NUM_0);

    if (RES_OK == asbResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABeepSrcOnCB
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
static Type_uByte nubLetSubABeepSrcOnCB(void)
{
    Type_uByte aubReturn;
    aubReturn = PE_SEC1;

    if (LET_SUBA_EV_BEEP_SRC_ON_RES == wstLetAppSubAMail_p->uhEventCode){
        aubReturn = PE_SEC0;
    }
    else if (LET_SUBA_EV_TIMEOUT == wstLetAppSubAMail_p->uhEventCode){
        aubReturn = PE_SEC1;
    }
    else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABeepDef
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
static Type_uByte nubLetSubABeepDef(void)
{
    Type_uByte aubEepResult;
    Type_uByte aubReturn;
    
    aubEepResult = RES_NG;
    aubReturn = PE_SEC1;

    //LXQADD wubEEP_BlockRead(LET_EEP_ADD_SND_BASS, LETAPP_NUM_1, &wubLetSndBass);
    //LXQADD wubEEP_BlockRead(LET_EEP_ADD_SND_TREBLE, LETAPP_NUM_1, &wubLetSndTreble);

    /* Beep Speaker Eeprom Default */
    wubLetSubATempBuff[LETAPP_NUM_0] = LET_SPK_DEF_EEP_1;
    wubLetSubATempBuff[LETAPP_NUM_1] = LET_SPK_DEF_EEP_2;
    wubLetSubATempBuff[LETAPP_NUM_2] = LET_SPK_DEF_EEP_3;
    wubLetSubATempBuff[LETAPP_NUM_3] = LET_SPK_DEF_EEP_4;

    //LXQADD aubEepResult = wubEEP_BlockWrite(LET_EEP_ADD_BEEP_SPK_REST, LETAPP_NUM_4, &wubLetSubATempBuff[LETAPP_NUM_0], EEP_FORCE_WRITE);

    if (RES_OK == aubEepResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    wubLetSpkIndex = LETAPP_NUM_0; /* Speaker Check Index Init */

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABeepStartOk
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
static Type_uByte nubLetSubABeepStartOk(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_SUCCESS;
    astLetRes.ubResSize = LETAPP_NUM_2;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];
    astLetRes.ubResData[LETAPP_NUM_0] = wubLetAppSubACmdData[LETAPP_NUM_1];
    astLetRes.ubResData[LETAPP_NUM_1] = wubLetAppSubACmdData[LETAPP_NUM_2];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_SPK_BEEP_START, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABeepStartNg
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
static Type_uByte nubLetSubABeepStartNg(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_FAILURE;
    astLetRes.ubResData[LETAPP_NUM_0] = KLINEDRV_RES_REJECT;
    astLetRes.ubResSize = LETAPP_NUM_1;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_SPK_BEEP_START, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABeepEndJudge
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
static Type_uByte nubLetSubABeepEndJudge(void)
{
    Type_uByte aubReturn;
    
    aubReturn = PE_SEC1;

    while ((wubLetSpkIndex < LETAPP_NUM_4) && ((wuhLetSpkConnectInfo & nstLetSubABeepInfo_a[wubLetSpkIndex].ubBeepConInfo) == LETAPP_NUM_0)){
        wubLetSpkIndex++;
    }

    if (wubLetSpkIndex < LETAPP_NUM_4){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABeepChUpOk
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
static Type_uByte nubLetSubABeepChUpOk(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_SUCCESS;
    astLetRes.ubResSize = LETAPP_NUM_2;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];
    astLetRes.ubResData[LETAPP_NUM_0] = wubLetAppSubACmdData[LETAPP_NUM_1];
    astLetRes.ubResData[LETAPP_NUM_1] = wubLetAppSubACmdData[LETAPP_NUM_2];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_SPK_BEEP_CH_UP, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABeepChUpNg
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
static Type_uByte nubLetSubABeepChUpNg(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_FAILURE;
    astLetRes.ubResData[LETAPP_NUM_0] = KLINEDRV_RES_REJECT;
    astLetRes.ubResSize = LETAPP_NUM_1;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_SPK_BEEP_CH_UP, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABeepSndSet
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
static Type_uByte nubLetSubABeepSndSet(void)
{
    //LXQADDType_uByte aubDataBuff[LETAPP_NUM_2];
    Type_uByte aubReturn;
    Type_uByte aubResult;

    aubReturn = PE_SEC1;
    aubResult = RES_NG;
   //LXQADD wvdGen_MemSet(&aubDataBuff[LETAPP_NUM_0], LETAPP_NUM_0, sizeof(aubDataBuff));

   //LXQADD aubDataBuff[LETAPP_NUM_0] = nstLetSubABeepInfo_a[wubLetSpkIndex].ubBeepType;
   //LXQADD aubDataBuff[LETAPP_NUM_1] = nstLetSubABeepInfo_a[wubLetSpkIndex].ubBeepFreq;

    //LXQADD aubResult = wubSoundStm_EventNotice(TSK_ID_LETAPP_SUBA, SOUND_STM_EVENT_REQ_LET_SP_CHECK, &aubDataBuff[LETAPP_NUM_0], sizeof(aubDataBuff));

    if (RES_OK == aubResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABeepSndSetCB
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
static Type_uByte nubLetSubABeepSndSetCB(void)
{
    Type_uByte aubReturn;

    aubReturn = PE_SEC1;

    if (LET_SUBA_EV_BEEP_SND_SET_RES == wstLetAppSubAMail_p->uhEventCode){
        aubReturn = PE_SEC0;
    }
    else if (LET_SUBA_EV_TIMEOUT == wstLetAppSubAMail_p->uhEventCode){
        aubReturn = PE_SEC1;
    }
    else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABeepDisp
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
static Type_uByte nubLetSubABeepDisp(void)
{
    Type_uByte aubReturn;
    Type_sByte asbResult;
    Type_uByte aubDispInfo;

    asbResult = RES_NG;
    aubReturn = PE_SEC1;
    aubDispInfo = nstLetSubABeepInfo_a[wubLetSpkIndex].ubBeepNoRest;

    asbResult = wsbRPCSendMsg(ARPC_ID_LET_SUBA,ARPC_ID_HMI,EV_LET_SPK_DISPLAY, &aubDispInfo, LETAPP_NUM_1);

    if (RES_OK == asbResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABeepDispOk
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
static Type_uByte nubLetSubABeepDispOk(void)
{
    Type_uByte aubReturn;
    Type_sByte aubResult;
    Type_uByte aubDispRest;

    aubReturn = PE_SEC1;
    aubResult = RES_NG;
    aubDispRest = PROC_SUCCESS;

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_SPK_DISP, &aubDispRest, sizeof(aubDispRest));

    if (RES_OK == aubResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABeepButtonChk
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
static Type_uByte nubLetSubABeepButtonChk(void)
{
    Type_uByte aubReturn;

    aubReturn = PE_SEC1;

   //LXQADD wubLetSpkBtnInfo = LET_SPK_BTN_INVALID;
    
    if ((wstLetAppSubAMail_p->vdQueueBuffer_p) != OS_NULL){

        wubLetSpkBtnInfo = *((Type_uByte*)(wstLetAppSubAMail_p->vdQueueBuffer_p)); /* Let Button Info Get */

        if ((LET_SPK_BTN_OK == wubLetSpkBtnInfo) || (LET_SPK_BTN_NG == wubLetSpkBtnInfo)){
            aubReturn = PE_SEC0;
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
*  function                 : nubLetSubABeepResultDisp
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
static Type_uByte nubLetSubABeepResultDisp(void)
{
    Type_uByte aubReturn;
    Type_sByte asbResult;
    Type_uByte aubDispInfo;

    asbResult = RES_NG;
    aubReturn = PE_SEC1;
    aubDispInfo = LET_SPK_DISP_INVALID;

    if (LET_SPK_BTN_OK == wubLetSpkBtnInfo){ /* Let Button Ok Check */
        nvdLetSubASpkRestSave(nstLetSubABeepInfo_a[wubLetSpkIndex].ubBeepConInfo, LETAPP_RES_OK);
        aubDispInfo = nstLetSubABeepInfo_a[wubLetSpkIndex].ubBeepOkRest;
        asbResult = wsbRPCSendMsg(ARPC_ID_LET_SUBA,ARPC_ID_HMI,EV_LET_SPK_DISPLAY, &aubDispInfo, LETAPP_NUM_1);

	}
    else if (LET_SPK_BTN_NG == wubLetSpkBtnInfo){ /* Let Button Ng Check */
        nvdLetSubASpkRestSave(nstLetSubABeepInfo_a[wubLetSpkIndex].ubBeepConInfo, LETAPP_RES_NG);
        aubDispInfo = nstLetSubABeepInfo_a[wubLetSpkIndex].ubBeepNgRest;
        asbResult = wsbRPCSendMsg(ARPC_ID_LET_SUBA,ARPC_ID_HMI,EV_LET_SPK_DISPLAY, &aubDispInfo, LETAPP_NUM_1);

	}
    else {
        /* No Action */
    }
    
    if (RES_OK == asbResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABeepChgNext
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
static Type_uByte nubLetSubABeepChgNext(void)
{
    Type_uByte aubReturn;

    aubReturn = PE_SEC1;

    wubLetSpkIndex++; /* Let Beep Check Next */

    if (wubLetSpkIndex < LETAPP_NUM_4){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABeepChgBack
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
static Type_uByte nubLetSubABeepChgBack(void)
{
    Type_uByte aubReturn;
    Type_uByte aubSpkIndex;

    aubReturn = PE_SEC1;
    aubSpkIndex = wubLetSpkIndex;

    if (aubSpkIndex > LETAPP_NUM_0){
        aubSpkIndex--; /* Let Beep Check Back */
        /* Let Beep Speaker Check */
        while ((aubSpkIndex > LETAPP_NUM_0) && ((wuhLetSpkConnectInfo & nstLetSubABeepInfo_a[aubSpkIndex].ubBeepConInfo) == LETAPP_NUM_0)){
            aubSpkIndex--;
        }

        if ((wuhLetSpkConnectInfo & nstLetSubABeepInfo_a[aubSpkIndex].ubBeepConInfo) != LETAPP_NUM_0){
            wubLetSpkIndex = aubSpkIndex;
            aubReturn = PE_SEC0;
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
*  function                 : nubLetSubABeepResultEep
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
static Type_uByte nubLetSubABeepResultEep(void)
{
    Type_uByte aubEepResult;
    Type_uByte aubReturn;

    aubEepResult = RES_NG;
    aubReturn = PE_SEC1;

    //LXQADD aubEepResult = wubEEP_BlockWrite(LET_EEP_ADD_BEEP_SPK_REST, LETAPP_NUM_2, &wubLetSubATempBuff[LETAPP_NUM_0], EEP_FORCE_WRITE);

    if (RES_OK == aubEepResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABeepSrcOff
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
static Type_uByte nubLetSubABeepSrcOff(void)
{
    Type_uByte aubReturn;
    Type_sByte asbResult;
    
    asbResult = RES_NG;
    aubReturn = PE_SEC1;

    asbResult = wsbRPCSendMsg(ARPC_ID_LET_SUBA,ARPC_ID_HMI,EV_LET_BEEP_SPK_SRC_OFF, OS_NULL, LETAPP_NUM_0);

    if (RES_OK == asbResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABeepSrcOffCB
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
static Type_uByte nubLetSubABeepSrcOffCB(void)
{
    Type_uByte aubReturn;
    aubReturn = PE_SEC1;

    if (LET_SUBA_EV_BEEP_SRC_OFF_RES == wstLetAppSubAMail_p->uhEventCode){
        aubReturn = PE_SEC0;
    }
    else if (LET_SUBA_EV_TIMEOUT == wstLetAppSubAMail_p->uhEventCode){
        aubReturn = PE_SEC1;
    }
    else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABeepEndOk
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
static Type_uByte nubLetSubABeepEndOk(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_SUCCESS;
    astLetRes.ubResSize = LETAPP_NUM_2;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];
    astLetRes.ubResData[LETAPP_NUM_0] = wubLetAppSubACmdData[LETAPP_NUM_1];
    astLetRes.ubResData[LETAPP_NUM_1] = wubLetAppSubACmdData[LETAPP_NUM_2];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_SPK_BEEP_END, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABeepNoChkEep
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
static Type_uByte nubLetSubABeepNoChkEep(void)
{
    Type_uByte aubEepResult;
    Type_uByte aubReturn;

    aubEepResult = RES_NG;
    aubReturn = PE_SEC1;

    //LXQADD aubEepResult = wubEEP_BlockWrite(LET_EEP_ADD_USB_SPK_REST, LETAPP_NUM_2, &wubLetSubATempBuff[LETAPP_NUM_0], EEP_FORCE_WRITE);

    if (RES_OK == aubEepResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABeepNoChkDisp
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
static Type_uByte nubLetSubABeepNoChkDisp(void)
{
    Type_uByte aubReturn;
    Type_sByte asbResult;
    Type_uByte aubDispInfo;

    asbResult = RES_NG;
    aubReturn = PE_SEC1;
    aubDispInfo = LET_SPK_DISP_NO_CHK;

    asbResult = wsbRPCSendMsg(ARPC_ID_LET_SUBA,ARPC_ID_HMI,EV_LET_SPK_DISPLAY, &aubDispInfo, LETAPP_NUM_1);

    if (RES_OK == asbResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubABeepEndNg
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
static Type_uByte nubLetSubABeepEndNg(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_FAILURE;
    astLetRes.ubResData[LETAPP_NUM_0] = KLINEDRV_RES_REJECT;
    astLetRes.ubResSize = LETAPP_NUM_1;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_SPK_BEEP_END, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubAMemClrStart
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
static Type_uByte nubLetSubAMemClrStart(void)
{
    Type_uByte aubReturn;
    Type_uByte aubFmAmIsChk;

    aubReturn = PE_SEC1;
    aubFmAmIsChk = LETAPP_NOMAL_NG;

    aubFmAmIsChk = wubLetAppFmAmIsChk();

    if (LETAPP_NOMAL_OK == aubFmAmIsChk){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubAMemClrStartOk
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
static Type_uByte nubLetSubAMemClrStartOk(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_SUCCESS;
    astLetRes.ubResSize = LETAPP_NUM_2;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];
    astLetRes.ubResData[LETAPP_NUM_0] = wubLetAppSubACmdData[LETAPP_NUM_1];
    astLetRes.ubResData[LETAPP_NUM_1] = wubLetAppSubACmdData[LETAPP_NUM_2];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_MEMORY_CLEAR_START, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubAMemClrStartNg
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
static Type_uByte nubLetSubAMemClrStartNg(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_FAILURE;
    astLetRes.ubResData[LETAPP_NUM_0] = KLINEDRV_RES_REJECT;
    astLetRes.ubResSize = LETAPP_NUM_1;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_MEMORY_CLEAR_START, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubAMemLetClr
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
static Type_uByte nubLetSubAMemLetClr(void)
{
    Type_uByte aubReturn;
    Type_uByte aubResult;

    aubReturn = PE_SEC1;
    aubResult = RES_NG;

    wvdGen_MemSet(&wubLetSubATempBuff[LETAPP_NUM_0], LETAPP_NUM_0, sizeof(wubLetSubATempBuff));
    //LXQADD aubResult = wubEEP_BlockWrite(LET_EEP_ADD_VOL_VSP, LETAPP_NUM_64, &wubLetSubATempBuff[LETAPP_NUM_0], EEP_FORCE_WRITE);

    if (RES_OK == aubResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubAMemLetClrOk
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
static Type_uByte nubLetSubAMemLetClrOk(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_SUCCESS;
    astLetRes.ubResSize = LETAPP_NUM_1;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];
    astLetRes.ubResData[LETAPP_NUM_0] = wubLetAppSubACmdData[LETAPP_NUM_1];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_LET_MEMORY_CLEAR, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubAMemLetClrNg
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
static Type_uByte nubLetSubAMemLetClrNg(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_FAILURE;
    astLetRes.ubResData[LETAPP_NUM_0] = KLINEDRV_RES_REJECT;
    astLetRes.ubResSize = LETAPP_NUM_1;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_LET_MEMORY_CLEAR, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubAMemErrorClr
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
static Type_uByte nubLetSubAMemErrorClr(void)
{
    Type_uByte aubReturn;
    Type_uByte aubResult;

    aubResult = RES_NG;
    aubReturn = PE_SEC1;
    
    /* LET Memory Clear */
    wvdGen_MemSet(&wubLetSubATempBuff[LETAPP_NUM_0], LETAPP_NUM_0, sizeof(wubLetSubATempBuff));
    //LXQADD aubResult = wubEEP_BlockWrite(LET_EEP_ADD_VOL_VSP, LETAPP_NUM_64, &wubLetSubATempBuff[LETAPP_NUM_0], EEP_FORCE_WRITE);

    /* Error History Clear */
    wvdGen_MemSet(&wubLetAppTempBuff[LETAPP_NUM_0], LETAPP_NUM_0, sizeof(wubLetAppTempBuff));

    //LXQADD aubResult += wubEEP_BlockWrite(LET_EEP_ADD_HOT_ERR, LETAPP_NUM_2, &wubLetAppTempBuff[LETAPP_NUM_0], EEP_FORCE_WRITE);
    //LXQADD aubResult += wubEEP_BlockWrite(LET_EEP_ADD_TOTAL_TIME, LETAPP_NUM_2, &wubLetAppTempBuff[LETAPP_NUM_0], EEP_FORCE_WRITE);
    //LXQADD aubResult += wubEEP_BlockWrite(LET_EEP_ADD_RADIO_TIME, LETAPP_NUM_4, &wubLetAppTempBuff[LETAPP_NUM_0], EEP_FORCE_WRITE);

    if (RES_OK == aubResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubAMemInitOk
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
static Type_uByte nubLetSubAMemInitOk(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_SUCCESS;
    astLetRes.ubResSize = LETAPP_NUM_1;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];
    astLetRes.ubResData[LETAPP_NUM_0] = wubLetAppSubACmdData[LETAPP_NUM_1];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_INITIAL_CLEAR, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubAMemInitNg
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
static Type_uByte nubLetSubAMemInitNg(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_FAILURE;
    astLetRes.ubResData[LETAPP_NUM_0] = KLINEDRV_RES_REJECT;
    astLetRes.ubResSize = LETAPP_NUM_1;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_INITIAL_CLEAR, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubAMemInlineClr
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
static Type_uByte nubLetSubAMemInlineClr(void)
{
    Type_uByte aubReturn;
    Type_sByte asbResult;

    asbResult = RES_NG;
    aubReturn = PE_SEC1;

    asbResult = wsbRPCSendMsg(ARPC_ID_LET_SUBA,ARPC_ID_DIAGAPP,EV_LET_INLINE_AGENT_CLR, OS_NULL, LETAPP_NUM_0);

    if (RES_OK == asbResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubAMemInlineClrOk
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
static Type_uByte nubLetSubAMemInlineClrOk(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_SUCCESS;
    astLetRes.ubResSize = LETAPP_NUM_1;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];
    astLetRes.ubResData[LETAPP_NUM_0] = wubLetAppSubACmdData[LETAPP_NUM_1];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_INLINE_CLEAR, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubAMemInlineClrNg
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
static Type_uByte nubLetSubAMemInlineClrNg(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_FAILURE;
    astLetRes.ubResData[LETAPP_NUM_0] = KLINEDRV_RES_REJECT;
    astLetRes.ubResSize = LETAPP_NUM_1;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_INLINE_CLEAR, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubAMemClrEnd
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
static Type_uByte nubLetSubAMemClrEnd(void)
{

    /* Debug Message */

    return PE_SEC0;
}
/*********************************************************************************************
*  function                 : nubLetSubAMemClrEndOk
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
static Type_uByte nubLetSubAMemClrEndOk(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_SUCCESS;
    astLetRes.ubResSize = LETAPP_NUM_2;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];
    astLetRes.ubResData[LETAPP_NUM_0] = wubLetAppSubACmdData[LETAPP_NUM_1];
    astLetRes.ubResData[LETAPP_NUM_1] = wubLetAppSubACmdData[LETAPP_NUM_2];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_MEMORY_CLEAR_END, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubAMemClrEndNg
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
static Type_uByte nubLetSubAMemClrEndNg(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_FAILURE;
    astLetRes.ubResData[LETAPP_NUM_0] = KLINEDRV_RES_REJECT;
    astLetRes.ubResSize = LETAPP_NUM_1;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_MEMORY_CLEAR_END, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubASndBassDef
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
static Type_uByte nubLetSubASndBassDef(void)
{
    Type_uByte aubResult;
    Type_uByte aubReturn;

    aubResult = RES_NG;
    aubReturn = PE_SEC1;
    
    //LXQADD aubResult = wubSoundStm_EventNotice(TSK_ID_LETAPP_SUBA, SOUND_STM_EVENT_REQ_BASS_VOL_SETTING, &wubLetSndBass, LETAPP_NUM_1);

    if (RES_OK == aubResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubASndBassDefCB
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
static Type_uByte nubLetSubASndBassDefCB(void)
{
    Type_uByte aubReturn;

    aubReturn = PE_SEC1;

    if (LET_SUBA_EV_SND_BASS_SET_RES == wstLetAppSubAMail_p->uhEventCode){
        aubReturn = PE_SEC0;
    }
    else if (LET_SUBA_EV_TIMEOUT == wstLetAppSubAMail_p->uhEventCode){
        aubReturn = PE_SEC1;
    }
    else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubASndTrebleDef
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
static Type_uByte nubLetSubASndTrebleDef(void)
{
    Type_uByte aubResult;
    Type_uByte aubReturn;

    aubResult = RES_NG;
    aubReturn = PE_SEC1;
    
    //LXQADD aubResult = wubSoundStm_EventNotice(TSK_ID_LETAPP_SUBA, SOUND_STM_EVENT_REQ_TREBLE_VOL_SETTING, &wubLetSndTreble, LETAPP_NUM_1);

    if (RES_OK == aubResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubASndTrebleDefCB
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
static Type_uByte nubLetSubASndTrebleDefCB(void)
{
    Type_uByte aubReturn;

    aubReturn = PE_SEC1;

    if (LET_SUBA_EV_SND_TREBLE_SET_RES == wstLetAppSubAMail_p->uhEventCode){
        aubReturn = PE_SEC0;
    }
    else if (LET_SUBA_EV_TIMEOUT == wstLetAppSubAMail_p->uhEventCode){
        aubReturn = PE_SEC1;
    }
    else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubASndBackRest
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
static Type_uByte nubLetSubASndBackRest(void)
{
    /* Debug Message */

    return PE_SEC0;
}
/*********************************************************************************************
*  function                 : nubLetSubASndVolSet
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
static Type_uByte nubLetSubASndVolSet(void)
{
   Type_uByte aubResult;
   Type_uByte aubReturn;
   //LXQADD Type_uByte *aubLetData_p;
   //LXQADD Type_uByte aubVolData[LETAPP_NUM_2];

   aubResult = RES_NG;
   aubReturn = PE_SEC1;
   //LXQADD aubLetData_p = OS_NULL;
   //LXQADD  wvdGen_MemSet(&aubVolData[LETAPP_NUM_0], LETAPP_NUM_0, sizeof(aubVolData));

   //LXQADD aubLetData_p = wstLetAppSubAMail_p->vdQueueBuffer_p;
   //LXQADD aubVolData[LETAPP_NUM_0] = SND_SOUND_DIRECT;
   //LXQADD aubVolData[LETAPP_NUM_1] = aubLetData_p[LETAPP_NUM_3];

   //LXQADD aubResult = wubSoundStm_EventNotice(TSK_ID_LETAPP_SUBA, SOUND_STM_EVENT_REQ_SND_VOL_SETTING, &aubVolData[LETAPP_NUM_0], sizeof(aubVolData));

    if (RES_OK == aubResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubASndVolSet
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
static Type_uByte nubLetSubASndVolSetCB(void)
{
    Type_uByte aubReturn;

    aubReturn = PE_SEC1;

    if (LET_SUBA_EV_SND_VOL_SET_RES == wstLetAppSubAMail_p->uhEventCode){
        aubReturn = PE_SEC0;
    }
    else if (LET_SUBA_EV_TIMEOUT == wstLetAppSubAMail_p->uhEventCode){
        aubReturn = PE_SEC1;
    }
    else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubASndVolSet
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
static Type_uByte nubLetSubASndVolSetOk(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_SUCCESS;
    astLetRes.ubResSize = LETAPP_NUM_2;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];
    astLetRes.ubResData[LETAPP_NUM_0] = wubLetAppSubACmdData[LETAPP_NUM_1];
    astLetRes.ubResData[LETAPP_NUM_1] = wubLetAppSubACmdData[LETAPP_NUM_2];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_VOLUME_SET, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubASndVolSet
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
static Type_uByte nubLetSubASndVolSetNg(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_FAILURE;
    astLetRes.ubResData[LETAPP_NUM_0] = KLINEDRV_RES_REJECT;
    astLetRes.ubResSize = LETAPP_NUM_1;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_VOLUME_SET, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubASndVolSet
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
static Type_uByte nubLetSubASndBassSet(void)
{
    Type_uByte aubResult;
    Type_uByte aubReturn;
    //LXQADDType_uByte aubBassData[LETAPP_NUM_2];
    //LXQADDType_uByte *aubLetData_p;

    aubResult = RES_NG;
    aubReturn = PE_SEC1;
    //LXQADDaubLetData_p = OS_NULL;
    //LXQADDwvdGen_MemSet(&aubBassData[LETAPP_NUM_0], LETAPP_NUM_0, sizeof(aubBassData));

    //LXQADDaubLetData_p = wstLetAppSubAMail_p->vdQueueBuffer_p;
    //LXQADDaubBassData[LETAPP_NUM_0] = SND_SOUND_DIRECT;
    //LXQADDaubBassData[LETAPP_NUM_1] = (Type_uByte)(((Type_sByte)(aubLetData_p[LETAPP_NUM_3])) + LETAPP_NUM_6);

    //LXQADDaubResult = wubSoundStm_EventNotice(TSK_ID_LETAPP_SUBA, SOUND_STM_EVENT_REQ_BASS_VOL_SETTING, &aubBassData[LETAPP_NUM_0], sizeof(aubBassData));

    if (RES_OK == aubResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubASndVolSet
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
static Type_uByte nubLetSubASndBassSetCB(void)
{
    Type_uByte aubReturn;

    aubReturn = PE_SEC1;

    if (LET_SUBA_EV_SND_BASS_SET_RES == wstLetAppSubAMail_p->uhEventCode){
        aubReturn = PE_SEC0;
    }
    else if (LET_SUBA_EV_TIMEOUT == wstLetAppSubAMail_p->uhEventCode){
        aubReturn = PE_SEC1;
    }
    else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubASndVolSet
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
static Type_uByte nubLetSubASndBassSetOk(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
   wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

   astLetRes.ubProcessRes = PROC_SUCCESS;
   astLetRes.ubResSize = LETAPP_NUM_2;
   astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];
   astLetRes.ubResData[LETAPP_NUM_0] = wubLetAppSubACmdData[LETAPP_NUM_1];
   astLetRes.ubResData[LETAPP_NUM_1] = wubLetAppSubACmdData[LETAPP_NUM_2];

   (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_BASS_SET, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubASndVolSet
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
static Type_uByte nubLetSubASndBassSetNg(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_FAILURE;
    astLetRes.ubResData[LETAPP_NUM_0] = KLINEDRV_RES_REJECT;
    astLetRes.ubResSize = LETAPP_NUM_1;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_BASS_SET, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubASndVolSet
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
static Type_uByte nubLetSubASndTrebleSet(void)
{
    Type_uByte aubResult;
    Type_uByte aubReturn;
    //LXQADD Type_uByte aubTrebleData[LETAPP_NUM_2];
    //LXQADD Type_uByte *aubLetData_p;

    aubResult = RES_NG;
    aubReturn = PE_SEC1;
    //LXQADD aubLetData_p = OS_NULL;
    //LXQADD wvdGen_MemSet(&aubTrebleData[LETAPP_NUM_0], LETAPP_NUM_0, sizeof(aubTrebleData));

    //LXQADD aubLetData_p = wstLetAppSubAMail_p->vdQueueBuffer_p;
    //LXQADD aubTrebleData[LETAPP_NUM_0] = SND_SOUND_DIRECT;
    //LXQADD aubTrebleData[LETAPP_NUM_1] = (Type_uByte)(((Type_sByte)(aubLetData_p[LETAPP_NUM_3])) + LETAPP_NUM_6);

    //LXQADD aubResult = wubSoundStm_EventNotice(TSK_ID_LETAPP_SUBA, SOUND_STM_EVENT_REQ_TREBLE_VOL_SETTING, &aubTrebleData[LETAPP_NUM_0], sizeof(aubTrebleData));

    if (RES_OK == aubResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubASndVolSet
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
static Type_uByte nubLetSubASndTrebleSetCB(void)
{
    Type_uByte aubReturn;

    aubReturn = PE_SEC1;

    if (LET_SUBA_EV_SND_TREBLE_SET_RES == wstLetAppSubAMail_p->uhEventCode){
        aubReturn = PE_SEC0;
    }
    else if (LET_SUBA_EV_TIMEOUT == wstLetAppSubAMail_p->uhEventCode){
        aubReturn = PE_SEC1;
    }
    else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubASndVolSet
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
static Type_uByte nubLetSubASndTrebleSetOk(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_SUCCESS;
    astLetRes.ubResSize = LETAPP_NUM_2;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];
    astLetRes.ubResData[LETAPP_NUM_0] = wubLetAppSubACmdData[LETAPP_NUM_1];
    astLetRes.ubResData[LETAPP_NUM_1] = wubLetAppSubACmdData[LETAPP_NUM_2];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_TREBLE_SET, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubASndVolSet
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
static Type_uByte nubLetSubASndTrebleSetNg(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_FAILURE;
    astLetRes.ubResData[LETAPP_NUM_0] = KLINEDRV_RES_REJECT;
    astLetRes.ubResSize = LETAPP_NUM_1;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_TREBLE_SET, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubALetSpeakerDisp
*  date                     : 2016.10.30
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
static Type_uByte nubLetSubALetSpeakerDisp(void)
{
    Type_uByte aubReturn;
    Type_sByte asbResult;
    Type_uByte aubDispInfo;

    asbResult = RES_NG;
    aubReturn = PE_SEC1;
    aubDispInfo = LET_SPK_DISP_DIAG;

    asbResult = wsbRPCSendMsg(ARPC_ID_LET_SUBA,ARPC_ID_HMI,EV_LET_SPK_DISPLAY, &aubDispInfo, LETAPP_NUM_1);

    if (RES_OK == asbResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubALetMuteOn
*  date                     : 2016.11.14
*  autho                    : liuminghao
*  description              : DSP Mute On
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return           
*                   
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*  
*********************************************************************************************/
static Type_uByte nubLetSubALetMuteOn(void)
{
    Type_uByte aubReturn;
    Type_uByte aubResult;

    aubResult = RES_NG;
    aubReturn = PE_SEC1;

    //LXQADD aubResult = apiSoundApp_DspMuteOn();

    if (RES_OK == aubResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubALetMuteOff
*  date                     : 2016.11.14
*  autho                    : liuminghao
*  description              : DSP Mute Off
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return           
*                   
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*  
*********************************************************************************************/
static Type_uByte nubLetSubALetMuteOff(void)
{
    Type_uByte aubReturn;
    Type_uByte aubResult;

    aubResult = RES_NG;
    aubReturn = PE_SEC1;

    //LXQADD aubResult = apiSoundApp_DspMuteOff();

    if (RES_OK == aubResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}
/*********************************************************************************************
*  function                 : nubLetSubALetBackButtonCheck
*  date                     : 2016.11.17
*  autho                    : liuminghao
*  description              : Whether a back notification was received
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return           
*                   
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*  
*********************************************************************************************/
static Type_uByte nubLetSubALetBackButtonCheck(void)
{
    Type_uByte aubReturn;

    aubReturn = PE_SEC1;

    if (LETAPP_NUM_1 == nubBackState){

        if (LETAPP_NUM_0 < wubLetSpkIndex){
	     wubLetSpkIndex--;	
        }
        else{
            aubReturn = PE_SEC0;
        }
    }
    else {
        aubReturn = PE_SEC0;
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLetSubAERRMemClr
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
static Type_uByte nubLetSubAERRMemClr(void)
{
    Type_uByte aubReturn;
    Type_uByte aubEepResult;

    aubEepResult = RES_NG;
    aubReturn = PE_SEC1;

    wvdGen_MemSet(&wubLetSubATempBuff[LETAPP_NUM_0], LETAPP_NUM_0, sizeof(wubLetSubATempBuff));

    //LXQADD aubEepResult = wubEEP_BlockWrite(LET_EEP_ADD_HOT_ERR, LETAPP_NUM_2, &wubLetSubATempBuff[LETAPP_NUM_0], EEP_FORCE_WRITE);


    if (RES_OK == aubEepResult){
        aubReturn = PE_SEC0;
    }
    else {
        /* No Action */
    }

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLetSubAERRMemClrOk
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
static Type_uByte nubLetSubAERRMemClrOk(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_SUCCESS;
    astLetRes.ubResSize = LETAPP_NUM_1;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];
    astLetRes.ubResData[LETAPP_NUM_0] = wubLetAppSubACmdData[LETAPP_NUM_1];

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_ERR_MEMORY_CLEAR, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLetSubAERRMemClrNg
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
static Type_uByte nubLetSubAERRMemClrNg(void)
{
    Type_uByte aubReturn;
    LetResponseRes_ts astLetRes;

    aubReturn = PE_SEC0;
    wvdGen_MemSet(&astLetRes, LETAPP_NUM_0, sizeof(LetResponseRes_ts));

    astLetRes.ubProcessRes = PROC_FAILURE;
    astLetRes.ubResSize = LETAPP_NUM_1;
    astLetRes.ubSeriveId = wubLetAppSubACmdData[LETAPP_NUM_0];
    astLetRes.ubResData[LETAPP_NUM_0] = KLINEDRV_RES_REJECT;

    (void)wubLetApp_SendMail(ARPC_ID_LET_SUBA,LETAPP_RES_ERR_MEMORY_CLEAR, &astLetRes, sizeof(LetResponseRes_ts));

    return aubReturn;
}

/*********************************************************************************************
*  function                 : nubLetSubAWaitInfo
*  date                     : 2016.12.21
*  autho                    : liuminghao
*  description              : Button Info or USB Disconnect
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return           
*                   
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*  
*********************************************************************************************/
static Type_uByte nubLetSubAWaitInfo(void)
{
    Type_uByte aubReturn;

    aubReturn = PE_SEC1;
    
    if (LET_SUBA_EV_SPK_BUTTON_RES == wstLetAppSubAMail_p->uhEventCode){
        aubReturn = PE_SEC0;
    }
    else if (LET_SUBA_EV_USB_DISCONNECT == wstLetAppSubAMail_p->uhEventCode){
        aubReturn = PE_SEC1;
    }
    else {
        aubReturn = PE_SEC_CONTINUE;
    }

    return aubReturn;
}

/* End of File */
