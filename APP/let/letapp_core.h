/**********************************************************************************************
*  file name                : letapp_core.h
*  date                     : 2016.06.01
*  autho                    : zhangjinzhu
*  model                    : TBTA
*  description              : letapp relate file
*
*----------------------------------------------------------------------------------------------
*  revision History
*  No.       date           revised by        description
*  0001      2016.05.01     zhangjinzhu       Create
**********************************************************************************************/
#ifndef _LETAPP_CORE_H_
#define _LETAPP_CORE_H_

/**********************************************************************************************/
/*                           Include file section                                             */
/**********************************************************************************************/

/* not used. */

/**********************************************************************************************/
/*                           Macro definition section                                         */
/**********************************************************************************************/

#define	LETMAIN_MPF_MAXSIZE			(100U)

/**********************************************************************************************/
/*                           Enumeration Type Definition Section                              */
/**********************************************************************************************/

enum LET_PROC_ID{
/* 000 */ PID_LETDIAG_START = 0U,
/* 001 */ PID_LETDIAG_END,
/* 002 */ PID_AM_START,
/* 003 */ PID_AM_END,
/* 004 */ PID_AM_ANT,
/* 005 */ PID_AM_ANT_END,
/* 006 */ PID_FM_START,
/* 007 */ PID_FM_END,
/* 008 */ PID_FM_ANT,
/* 009 */ PID_FM_ANT_END,
/* 010 */ PID_USB_TEST_START,
/* 011 */ PID_USB_TEST_END,
/* 012 */ PID_USB_SPK_CHK_START,
/* 013 */ PID_USB_SPK_CHK_END,
/* 014 */ PID_USB_BIBIRI_START,
/* 015 */ PID_SET_BIBIRI_CHK,
/* 016 */ PID_USB_BIBIRI_END,
/* 017 */ PID_BSM_INITIAL,
/* 018 */ PID_BSM_START_TP_SET,
/* 019 */ PID_BSM_END,
/* 020 */ PID_TUNER_START,
/* 021 */ PID_SET_TUNER,
/* 022 */ PID_TUNER_END,
/* 023 */ PID_SPK_BEEP_START,
/* 024 */ PID_SPK_BEEP_CH_UP,
/* 025 */ PID_SPK_BEEP_END,
/* 026 */ PID_SPK_BEEP_BUTTON,
/* 027 */ PID_MEM_CLEAR_START,
/* 028 */ PID_LET_MEM_CLEAR,
/* 029 */ PID_INITIAL_CLEAR,
/* 030 */ PID_MEM_CLEAR_END,
/* 031 */ PID_AM_ANT_REST,
/* 032 */ PID_FM_ANT_REST,
/* 033 */ PID_SPK_CHECK_REST,
/* 034 */ PID_BSM_CHECK_REST,
/* 035 */ PID_BIBIRI_CHK_REST,
/* 036 */ PID_AUTO_VOL_VSP_REST,
/* 037 */ PID_VIN_COMP_REST,
/* 038 */ PID_AUDIO_VIN_REST,
/* 039 */ PID_TOTAL_TIME_REST,
/* 040 */ PID_MODE_TIME_REST,
/* 041 */ PID_AUDIO_DIAG_REST,
/* 042 */ PID_AUDIO_DWG_REST,
/* 043 */ PID_AUDIO_DATE_REST,
/* 044 */ PID_AUDIO_SERIAL_REST,
/* 045 */ PID_TUNER_TEST_REST,
/* 046 */ PID_SPK_BEEP_REST,
/* 047 */ PID_INLINE_DIAG_REST,
/* 048 */ PID_INLINE_CLEAR,
/* 049 */ PID_SND_VOLUME_SET,
/* 050 */ PID_SND_BASS_SET,
/* 051 */ PID_SND_TREBLE_SET,
/* 052 */ PID_HOT_ERR_REST,
/* 053 */ PID_OK_SEND,
/* 054 */ PID_NG_BUSY,
/* 055 */ PID_NO_ACTION,
/* 056 */ PID_NG_NON_CONDITIONS,
/* 057 */ PID_SPK_BUTTON_INFO,
/* 058 */ PID_ERR_MEM_CLEAR,
/* LET Response PID */
/* 000 */ PID_RES_AUDIO_DIAG_START,
/* 001 */ PID_RES_AUDIO_DIAG_END,
/* 002 */ PID_RES_AM_ANT_START,
/* 003 */ PID_RES_AM_ANT1,
/* 004 */ PID_RES_AM_ANT2,
/* 005 */ PID_RES_AM_ANT3,
/* 006 */ PID_RES_AM_ANT1_END,
/* 007 */ PID_RES_AM_ANT2_END,
/* 008 */ PID_RES_AM_ANT3_END,
/* 009 */ PID_RES_AM_ANT_END,
/* 010 */ PID_RES_FM_ANT_START,
/* 011 */ PID_RES_FM_ANT1,
/* 012 */ PID_RES_FM_ANT2,
/* 013 */ PID_RES_FM_ANT3,
/* 014 */ PID_RES_FM_ANT1_END,
/* 015 */ PID_RES_FM_ANT2_END,
/* 016 */ PID_RES_FM_ANT3_END,
/* 017 */ PID_RES_FM_ANT_END,
/* 018 */ PID_RES_USB_TEST_START,
/* 019 */ PID_RES_SPK_CHK_START,
/* 020 */ PID_RES_SPK_CHK_END,
/* 021 */ PID_RES_USB_TEST_END,
/* 022 */ PID_RES_SPK_BIBIRI_START,
/* 023 */ PID_RES_SET_BIBIRI_CHECK,
/* 024 */ PID_RES_BIBIRI_CHECK_END,
/* 025 */ PID_RES_BSM_INITIAL,
/* 026 */ PID_RES_BSM_START_TP_SET,
/* 027 */ PID_RES_BSM_END,
/* 028 */ PID_RES_BSM_REST,
/* 029 */ PID_RES_TUNER_TEST_START,
/* 030 */ PID_RES_SET_TUNER_TEST,
/* 031 */ PID_RES_TUNER_TEST_END,
/* 032 */ PID_RES_SPK_BEEP_START,
/* 033 */ PID_RES_SPK_BEEP_CH_UP,
/* 034 */ PID_RES_SPK_BEEP_END,
/* 035 */ PID_RES_SPK_DISP,
/* 036 */ PID_RES_MEMORY_CLEAR_START,
/* 037 */ PID_RES_LET_MEMORY_CLEAR,
/* 038 */ PID_RES_INITIAL_CLEAR,
/* 039 */ PID_RES_INLINE_CLEAR,
/* 040 */ PID_RES_MEMORY_CLEAR_END,
/* 041 */ PID_RES_VOLUME_SET,
/* 042 */ PID_RES_BASS_SET,
/* 043 */ PID_RES_TREBLE_SET,
/* 044 */ PID_RES_BUSY,
/* 045 */ PID_RES_EER_MEMORY_CLEAR,
/* *** */ PID_PROC_ID_MAX
};

enum LET_TEST_PROC_ID{
    PID_LET_INVALID = PID_PROC_ID_MAX,
    PID_LET_AM_TEST,
    PID_LET_FM_TEST,
    PID_LET_TUNER_TEST,
    PID_LET_USB_SPK_TEST,
    PID_LET_SPK_BEEP_TEST,
    PID_LET_MEM_CLEAR_TEST,
    PID_LET_BSM_TEST,
    PID_LET_MAX,
    PID_HIGH_INDEX
};

enum LET_COM_MODE{
    LET_MODE_OFFED = 0U,
    LET_MODE_ONED,
    LET_MODE_WAIT,
    LET_MODE_MAX,
    LET_MODE_NO_CHG
};

enum LET_HIGH_INDEX_EV{
/* 000 */ EV_H_INDEX_AM_ANT_START = 0U,
/* 001 */ EV_H_INDEX_AM_ANT1,
/* 002 */ EV_H_INDEX_AM_ANT2,
/* 003 */ EV_H_INDEX_AM_ANT3,
/* 004 */ EV_H_INDEX_AM_ANT1_END,
/* 005 */ EV_H_INDEX_AM_ANT2_END,
/* 006 */ EV_H_INDEX_AM_ANT3_END,
/* 007 */ EV_H_INDEX_AM_ANT_END,
/* 008 */ EV_H_INDEX_FM_ANT_START,
/* 009 */ EV_H_INDEX_FM_ANT1,
/* 010 */ EV_H_INDEX_FM_ANT2,
/* 011 */ EV_H_INDEX_FM_ANT3,
/* 012 */ EV_H_INDEX_FM_ANT1_END,
/* 013 */ EV_H_INDEX_FM_ANT2_END,
/* 014 */ EV_H_INDEX_FM_ANT3_END,
/* 015 */ EV_H_INDEX_FM_ANT_END,
/* 016 */ EV_H_INDEX_USB_TEST_START,
/* 017 */ EV_H_INDEX_SPK_CHK_START,
/* 018 */ EV_H_INDEX_SPK_CHK_END,
/* 019 */ EV_H_INDEX_USB_TEST_END,
/* 020 */ EV_H_INDEX_SPK_BIBIRI_START,
/* 021 */ EV_H_INDEX_SET_BIBIRI_CHECK,
/* 022 */ EV_H_INDEX_BIBIRI_CHECK_END,
/* 023 */ EV_H_INDEX_BSM_START_TP_SET,
/* 024 */ EV_H_INDEX_BSM_END,
/* 025 */ EV_H_INDEX_TUNER_TEST_START,
/* 026 */ EV_H_INDEX_SET_TUNER_TEST,
/* 027 */ EV_H_INDEX_TUNER_TEST_END,
/* 028 */ EV_H_INDEX_SPK_BEEP_START,
/* 029 */ EV_H_INDEX_SPK_BEEP_CH_UP,
/* 030 */ EV_H_INDEX_SPK_BEEP_END,
/* 031 */ EV_H_INDEX_MEM_CLEAR_START,
/* 032 */ EV_H_INDEX_LET_MEM_CLEAR,
/* 033 */ EV_H_INDEX_INITIAL_CLEAR,
/* 034 */ EV_H_INDEX_INLINE_CLEAR,
/* 035 */ EV_H_INDEX_MEM_CLEAR_END,
/* 036 */ EV_H_INDEX_SPK_BUTTON,
/* 037 */ EV_H_INDEX_EER_MEMORY_CLEAR,
/* *** */ EV_H_INDEX_MAX,
/* *** */ EV_H_INDEX_NO_CHG
};

enum LET_HIGH_INDEX_MODE{
    H_INDEX_MODE_WAIT = 0U,
    H_INDEX_MODE_FMAM,
    H_INDEX_MODE_USB_SPK,
    H_INDEX_MODE_SPK_BEEP,
    H_INDEX_MODE_BSM,
    H_INDEX_MODE_MEM_CLEAR,
    H_INDEX_MODE_MAX,
    H_INDEX_MODE_NO_CHG
};


enum LET_LOW_INDEX_NO_CHG{
    EV_L_INDEX_NO_CHG = 0xffU,
    MODE_L_NO_CHG = 0xffU
};

/*********** LET AM TEST DEFINE START ************/
enum LET_LOW_INDEX_EV_AM{
    EV_L_INDEX_AM_ANT_START = 0U,
    EV_L_INDEX_AM_ANT1,
    EV_L_INDEX_AM_ANT2,
    EV_L_INDEX_AM_ANT3,
    EV_L_INDEX_AM_ANT1_END,
    EV_L_INDEX_AM_ANT2_END,
    EV_L_INDEX_AM_ANT3_END,
    EV_L_INDEX_AM_ANT_END,
    EV_L_INDEX_AM_MAX
};

enum LET_LOW_INDEX_MODE_AM{
    MODE_L_AM_OFF = 0U,
    MODE_L_AM_START,
    MODE_L_AM_ANT1_CHK,
    MODE_L_AM_ANT2_CHK,
    MODE_L_AM_ANT3_CHK,
    MODE_L_AM_MAX
};
/*********** LET AM TEST DEFINE END **************/

/*********** LET FM TEST DEFINE START ************/
enum LET_LOW_INDEX_EV_FM{
    EV_L_INDEX_FM_ANT_START = 0U,
    EV_L_INDEX_FM_ANT1,
    EV_L_INDEX_FM_ANT2,
    EV_L_INDEX_FM_ANT3,
    EV_L_INDEX_FM_ANT1_END,
    EV_L_INDEX_FM_ANT2_END,
    EV_L_INDEX_FM_ANT3_END,
    EV_L_INDEX_FM_ANT_END,
    EV_L_INDEX_FM_MAX
};

enum LET_LOW_INDEX_MODE_FM{
    MODE_L_FM_OFF = 0U,
    MODE_L_FM_START,
    MODE_L_FM_ANT1_CHK,
    MODE_L_FM_ANT2_CHK,
    MODE_L_FM_ANT3_CHK,
    MODE_L_FM_MAX
};
/*********** LET FM TEST DEFINE END **************/

/*********** LET USB TEST DEFINE START ***********/
enum LET_LOW_INDEX_EV_USB{
    EV_L_INDEX_USB_TEST_START = 0U,
    EV_L_INDEX_SPK_CHK_START,
    EV_L_INDEX_SPK_CHK_END,
    EV_L_INDEX_USB_TEST_END,
    EV_L_INDEX_SPK_BIBIRI_START,
    EV_L_INDEX_SET_BIBIRI_CHECK,
    EV_L_INDEX_BIBIRI_CHECK_END,
    EV_L_INDEX_USB_MAX
};

enum LET_LOW_INDEX_MODE_USB{
    MODE_L_USB_TEST_OFF = 0U,
    MODE_L_USB_TEST_START,
    MODE_L_SPK_CHK_START,
    MODE_L_SPK_CHK_END,
    MODE_L_SPK_BIBIRI_START,
    MODE_L_SPK_BIBIRI_END,
    MODE_L_USB_MAX
};
/*********** LET USB TEST DEFINE END *************/

/*********** LET BSM TEST DEFINE START ***********/
enum LET_LOW_INDEX_EV_BSM{
    EV_L_INDEX_BSM_START_TP_SET = 0U,
    EV_L_INDEX_BSM_END,
    EV_L_INDEX_BSM_MAX
};

enum LET_LOW_INDEX_MODE_BSM{
    MODE_L_BSM_TEST_OFF = 0U,
    MODE_L_BSM_TEST_START,
    MODE_L_BSM_MAX
};
/*********** LET BSM TEST DEFINE END *************/

/*********** LET TUNER TEST DEFINE START *********/
enum LET_LOW_INDEX_EV_TUNER{
    EV_L_INDEX_TUNER_TEST_START = 0U,
    EV_L_INDEX_SET_TUNER_TEST,
    EV_L_INDEX_TUNER_TEST_END,
    EV_L_INDEX_TUNER_MAX
};

enum LET_LOW_INDEX_MODE_TUNER{
    MODE_L_TUNER_TEST_OFF = 0U,
    MODE_L_TUNER_TEST_START,
    MODE_L_TUNER_MAX
};
/*********** LET TUNER TEST DEFINE END **********/

/*********** LET BEEP TEST DEFINE START *********/
enum LET_LOW_INDEX_EV_BEEP{
    EV_L_INDEX_SPK_BEEP_START = 0U,
    EV_L_INDEX_SPK_BEEP_CH_UP,
    EV_L_INDEX_SPK_BEEP_END,
    EV_L_INDEX_SPK_BEEP_BUTTON,
    EV_L_INDEX_SPK_BEEP_MAX
};
enum LET_LOW_INDEX_MODE_BEEP{
    MODE_L_SPK_BEEP_OFF = 0U,
    MODE_L_SPK_BEEP_START,
    MODE_L_SPK_BEEP_WAIT,
    MODE_L_SPK_BEEP_MAX
};
/*********** LET BEEP TEST DEFINE END **********/

/*********** LET MEMORY TEST DEFINE START *********/
enum LET_LOW_INDEX_EV_MEM{
    EV_L_INDEX_MEM_CLEAR_START = 0U,
    EV_L_INDEX_LET_MEM_CLEAR,
    EV_L_INDEX_INITIAL_CLEAR,
    EV_L_INDEX_INLINE_CLEAR,
    EV_L_INDEX_EER_MEM_CLEAR,
    EV_L_INDEX_MEM_CLEAR_END,
    EV_L_INDEX_MEM_MAX
};
enum LET_LOW_INDEX_MODE_MEM{
    MODE_L_MEM_CLEAR_OFF = 0U,
    MODE_L_MEM_CLEAR_START,
    MODE_L_MEM_CLEAR_MAX
};
/*********** LET MEMORY TEST DEFINE END **********/


/**********************************************************************************************/
/*                           User type definition section                                     */
/**********************************************************************************************/

/* not used. */

/**********************************************************************************************/
/*                           Structure/Union type definition section                          */
/**********************************************************************************************/

typedef struct{
    Type_uByte ubProcessId;
    Type_uByte ubNextMode;
    Type_uByte ubHighIndexEvent;
}LetModeMatrix_ts;

typedef struct{
    Type_uByte ubProcessId;
    Type_uByte ubNextHIndexMode;
    Type_uByte ubLowIndexEvent;
}HighIndexMatrix_ts;

typedef struct {
    Type_uByte ubProcessId;
    Type_uByte ubNextLIndexMode;
}LowIndexMatrix_ts;

/**********************************************************************************************/
/*                           Global variable definition section                               */
/**********************************************************************************************/

extern LowIndexMatrix_ts* wstLetAmIndex_p;
extern LowIndexMatrix_ts* wstLetFmIndex_p;
extern LowIndexMatrix_ts* wstLetTunerIndex_p;
extern LowIndexMatrix_ts* wstLetUsbIndex_p;
extern LowIndexMatrix_ts* wstLetBeepIndex_p;
extern LowIndexMatrix_ts* wstLetMemIndex_p;
extern LowIndexMatrix_ts* wstLetBsmIndex_p;

extern Type_uByte wubLetAppComModeSts;
extern Type_uByte wubLetAppHighIndexMode;
extern Type_uByte wubLetAppAmIndexMode;
extern Type_uByte wubLetAppFmIndexMode;
extern Type_uByte wubLetAppTunerIndexMode;
extern Type_uByte wubLetAppUsbIndexMode;
extern Type_uByte wubLetAppBeepIndexMode;
extern Type_uByte wubLetAppMemIndexMode;
extern Type_uByte wubLetAppBsmIndexMode;

/**********************************************************************************************/
/*                           Global Function Prototype Declaration                            */
/**********************************************************************************************/
extern void wvdLetAppControl(void);


#endif
/* end of file */

