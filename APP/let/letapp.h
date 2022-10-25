/**********************************************************************************************
*  file name                : letapp.h
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
#ifndef _LETAPP_H_
#define _LETAPP_H_

/**********************************************************************************************/
/*                           Include file section                                             */
/**********************************************************************************************/
#include <string.h>
#include "logapp.h"

/**********************************************************************************************/
/*                           Macro definition section                                         */
/**********************************************************************************************/
//#define LET_DEBUG_LOG

#define filename(x) (strrchr(x,'/')?strrchr(x,'/')+1:x)

#ifdef LET_DEBUG_LOG
#define LETAPP(fmt,...) printf("\n[LETAPP] "fmt",file = %s,line = %d\n",##__VA_ARGS__,filename(__FILE__),__LINE__)
#else
#define LETAPP(fmt,...) LOG_OUTPUT( LOG_APP_ID_LET,DEBUGLOG_LEVEL_ERROR,ATAT_LEVEL_OFF,"\n[LETAPP] "fmt"\n",##__VA_ARGS__)
#endif
/**********************************************************************************************/
/*                           Enumeration Type Definition Section                              */
/**********************************************************************************************/

/*** Main Event Requset ***/
enum LET_EVENT_DEF{
/* 000 */ LETAPP_EV_AUDIO_DIAG_START = 0x00U,
/* 001 */ LETAPP_EV_AUDIO_DIAG_END,
/* 002 */ LETAPP_EV_AM_ANT_START,
/* 003 */ LETAPP_EV_AM_ANT1,
/* 004 */ LETAPP_EV_AM_ANT2,
/* 005 */ LETAPP_EV_AM_ANT3,
/* 006 */ LETAPP_EV_AM_ANT1_END,
/* 007 */ LETAPP_EV_AM_ANT2_END,
/* 008 */ LETAPP_EV_AM_ANT3_END,
/* 009 */ LETAPP_EV_AM_ANT_END,
/* 010 */ LETAPP_EV_AM_ANT_REST,
/* 011 */ LETAPP_EV_FM_ANT_START,
/* 012 */ LETAPP_EV_FM_ANT1,
/* 013 */ LETAPP_EV_FM_ANT2,
/* 014 */ LETAPP_EV_FM_ANT3,
/* 015 */ LETAPP_EV_FM_ANT1_END,
/* 016 */ LETAPP_EV_FM_ANT2_END,
/* 017 */ LETAPP_EV_FM_ANT3_END,
/* 018 */ LETAPP_EV_FM_ANT_END,
/* 019 */ LETAPP_EV_FM_ANT_REST,
/* 020 */ LETAPP_EV_USB_TEST_START,
/* 021 */ LETAPP_EV_SPK_CHK_START,
/* 022 */ LETAPP_EV_SPK_CHK_END,
/* 023 */ LETAPP_EV_USB_TEST_END,
/* 024 */ LETAPP_EV_SPK_CHECK_REST,
/* 025 */ LETAPP_EV_SPK_BIBIRI_START,
/* 026 */ LETAPP_EV_SET_BIBIRI_CHECK,
/* 027 */ LETAPP_EV_BIBIRI_CHECK_END,
/* 028 */ LETAPP_EV_BIBIRI_CHK_REST,
/* 029 */ LETAPP_EV_BSM_INITIAL,
/* 030 */ LETAPP_EV_BSM_START_TP_SET,
/* 031 */ LETAPP_EV_BSM_END,
/* 032 */ LETAPP_EV_BSM_REST,
/* 033 */ LETAPP_EV_AUTO_VOL_VSP_REST,
/* 034 */ LETAPP_EV_VIN_COMP_REST,
/* 035 */ LETAPP_EV_AUDIO_VIN_REST,
/* 036 */ LETAPP_EV_TOTAL_TIME_REST,
/* 037 */ LETAPP_EV_MODE_TIME_REST,
/* 038 */ LETAPP_EV_AUDIO_DIAG_REST,
/* 039 */ LETAPP_EV_AUDIO_DWG_REST,
/* 040 */ LETAPP_EV_AUDIO_DATE_REST,
/* 041 */ LETAPP_EV_AUDIO_SERIAL_REST,
/* 042 */ LETAPP_EV_TUNER_TEST_START,
/* 043 */ LETAPP_EV_SET_TUNER_TEST,
/* 044 */ LETAPP_EV_TUNER_TEST_END,
/* 045 */ LETAPP_EV_TUNER_TEST_REST,
/* 046 */ LETAPP_EV_SPK_BEEP_START,
/* 047 */ LETAPP_EV_SPK_BEEP_CH_UP,
/* 048 */ LETAPP_EV_SPK_BEEP_END,
/* 049 */ LETAPP_EV_SPK_BEEP_REST,
/* 050 */ LETAPP_EV_MEMORY_CLEAR_START,
/* 051 */ LETAPP_EV_LET_MEMORY_CLEAR,
/* 052 */ LETAPP_EV_INITIAL_CLEAR,
/* 053 */ LETAPP_EV_MEMORY_CLEAR_END,
/* 054 */ LETAPP_EV_INLINE_DIAG_REST,
/* 055 */ LETAPP_EV_INLINE_DIAG_CLEAR,
/* 056 */ LETAPP_EV_VOL_SET,
/* 057 */ LETAPP_EV_BASS_SET,
/* 058 */ LETAPP_EV_TREBLE_SET,
/* 059 */ LETAPP_EV_HOT_ERR_REST,
/* 060 */ LETAPP_EV_BUTTON_INFO,
/* 061 */ LETAPP_EV_LET_ERR_MEMORY_CLEAR,
/* *** */ LETAPP_EV_MAX
};

/* LET Response Event CallBack */
enum LETAPP_RESPONSE_DEF{
/* 000 */ LETAPP_RES_AUDIO_DIAG_START = LETAPP_EV_MAX,
/* 001 */ LETAPP_RES_AUDIO_DIAG_END,
/* 002 */ LETAPP_RES_AM_ANT_START,
/* 003 */ LETAPP_RES_AM_ANT1,
/* 004 */ LETAPP_RES_AM_ANT2,
/* 005 */ LETAPP_RES_AM_ANT3,
/* 006 */ LETAPP_RES_AM_ANT1_END,
/* 007 */ LETAPP_RES_AM_ANT2_END,
/* 008 */ LETAPP_RES_AM_ANT3_END,
/* 009 */ LETAPP_RES_AM_ANT_END,
/* 010 */ LETAPP_RES_FM_ANT_START,
/* 011 */ LETAPP_RES_FM_ANT1,
/* 012 */ LETAPP_RES_FM_ANT2,
/* 013 */ LETAPP_RES_FM_ANT3,
/* 014 */ LETAPP_RES_FM_ANT1_END,
/* 015 */ LETAPP_RES_FM_ANT2_END,
/* 016 */ LETAPP_RES_FM_ANT3_END,
/* 017 */ LETAPP_RES_FM_ANT_END,
/* 018 */ LETAPP_RES_USB_TEST_START,
/* 019 */ LETAPP_RES_SPK_CHK_START,
/* 020 */ LETAPP_RES_SPK_CHK_END,
/* 021 */ LETAPP_RES_USB_TEST_END,
/* 022 */ LETAPP_RES_SPK_BIBIRI_START,
/* 023 */ LETAPP_RES_SET_BIBIRI_CHECK,
/* 024 */ LETAPP_RES_BIBIRI_CHECK_END,
/* 025 */ LETAPP_RES_BSM_INITIAL,
/* 026 */ LETAPP_RES_BSM_START_TP_SET,
/* 027 */ LETAPP_RES_BSM_END,
/* 028 */ LETAPP_RES_BSM_REST,
/* 029 */ LETAPP_RES_TUNER_TEST_START,
/* 030 */ LETAPP_RES_SET_TUNER_TEST,
/* 031 */ LETAPP_RES_TUNER_TEST_END,
/* 032 */ LETAPP_RES_SPK_BEEP_START,
/* 033 */ LETAPP_RES_SPK_BEEP_CH_UP,
/* 034 */ LETAPP_RES_SPK_BEEP_END,
/* 035 */ LETAPP_RES_SPK_DISP,
/* 036 */ LETAPP_RES_MEMORY_CLEAR_START,
/* 037 */ LETAPP_RES_LET_MEMORY_CLEAR,
/* 038 */ LETAPP_RES_INITIAL_CLEAR,
/* 039 */ LETAPP_RES_INLINE_CLEAR,
/* 040 */ LETAPP_RES_MEMORY_CLEAR_END,
/* 041 */ LETAPP_RES_VOLUME_SET,
/* 042 */ LETAPP_RES_BASS_SET,
/* 043 */ LETAPP_RES_TREBLE_SET,
/* 044 */ LETAPP_RES_BUSY,
/* 045 */ LETAPP_RES_ERR_MEMORY_CLEAR,
/* *** */ LETAPP_RES_MAX,
/* *** */ LETAPP_EV_INVALID
};

/* LET SubA Task Event For LET */
enum LET_SUBA_EVENT_DEF{
/* 000 */ LET_SUBA_EV_INVALID = 0U,
/* 001 */ LET_SUBA_EV_LET_START,
/* 002 */ LET_SUBA_EV_LET_END,
/* 003 */ LET_SUBA_EV_USB_TEST_START,
/* 004 */ LET_SUBA_EV_USB_TEST_END,
/* 005 */ LET_SUBA_EV_USB_SPK_START,
/* 006 */ LET_SUBA_EV_USB_SPK_END,
/* 007 */ LET_SUBA_EV_BIBIRI_START,
/* 008 */ LET_SUBA_EV_BIBIRI_SET,
/* 009 */ LET_SUBA_EV_BIBIRI_END,
/* 010 */ LET_SUBA_EV_BSM_INITIAL,
/* 011 */ LET_SUBA_EV_BSM_START_TP_SET,
/* 012 */ LET_SUBA_EV_BSM_END,
/* 013 */ LET_SUBA_EV_BSM_REST,
/* 014 */ LET_SUBA_EV_SPK_BEEP_START,
/* 015 */ LET_SUBA_EV_SPK_BEEP_CH_UP,
/* 016 */ LET_SUBA_EV_SPK_BEEP_END,
/* 017 */ LET_SUBA_EV_SPK_BEEP_BUTTON,
/* 018 */ LET_SUBA_EV_MEM_CLEAR_START,
/* 019 */ LET_SUBA_EV_LET_MEM_CLEAR,
/* 020 */ LET_SUBA_EV_MEM_INIT_CLEAR,
/* 021 */ LET_SUBA_EV_MEM_INLINE_CLEAR,
/* 022 */ LET_SUBA_EV_MEM_CLEAR_END,
/* 023 */ LET_SUBA_EV_VOL_SET,
/* 024 */ LET_SUBA_EV_BASS_SET,
/* 025 */ LET_SUBA_EV_TREBLE_SET,
/* 026 */ LET_SUBA_EV_ERR_MEM_CLEAR,
/* *** */ LET_SUBA_EV_MAX
};

/* LET SubA Task CallBack */
enum LET_SUBA_INF_DEF{
/* 000 */ LET_SUBA_EV_INVALID_RES = LET_SUBA_EV_MAX,
/* 001 */ LET_SUBA_EV_LET_START_RES,
/* 002 */ LET_SUBA_EV_LET_END_RES,
/* 003 */ LET_SUBA_EV_VIN_NO_RES,
/* 004 */ LET_SUBA_EV_VSP_RES,
/* 005 */ LET_SUBA_EV_USB_SRC_ON_RES,
/* 006 */ LET_SUBA_EV_USB_SRC_OFF_RES,
/* 007 */ LET_SUBA_EV_USB_PLAY_RES,
/* 008 */ LET_SUBA_EV_USB_PLAYSTOP_RES,
/* 009 */ LET_SUBA_EV_SPK_SND_SET_RES,
/* 010 */ LET_SUBA_EV_BEEP_SND_SET_RES,
/* 011 */ LET_SUBA_EV_BSM_INIT_RES,
/* 012 */ LET_SUBA_EV_BSM_START_RES,
/* 013 */ LET_SUBA_EV_BSM_END_RES,
/* 014 */ LET_SUBA_EV_BSM_RESULT_RES,
/* 015 */ LET_SUBA_EV_BEEP_SRC_ON_RES,
/* 016 */ LET_SUBA_EV_BEEP_SRC_OFF_RES,
/* 017 */ LET_SUBA_EV_SPK_DISP_RES,
/* 018 */ LET_SUBA_EV_SND_VOL_SET_RES,
/* 019 */ LET_SUBA_EV_SND_BASS_SET_RES,
/* 020 */ LET_SUBA_EV_SND_TREBLE_SET_RES,
/* 021 */ LET_SUBA_EV_SPK_BUTTON_RES,
/* 022 */ LET_SUBA_EV_USB_DISCONNECT,
/* *** */ LET_SUBA_EV_TIMEOUT,
/* *** */ LET_SUBA_EV_MAX_RES
};

/* LET SubB Task Event For LET */
enum LET_SUBB_EVENT_DEF{
/* 000 */ LET_SUBB_EV_INVALID = 0U,
/* 001 */ LET_SUBB_EV_AM_START,
/* 002 */ LET_SUBB_EV_AM_END,
/* 003 */ LET_SUBB_EV_AM_ANT,
/* 004 */ LET_SUBB_EV_AM_ANT_END,
/* 005 */ LET_SUBB_EV_FM_START,
/* 006 */ LET_SUBB_EV_FM_END,
/* 007 */ LET_SUBB_EV_FM_ANT,
/* 008 */ LET_SUBB_EV_FM_ANT_END,
/* 009 */ LET_SUBB_EV_TUNER_START,
/* 010 */ LET_SUBB_EV_TUNER_SET,
/* 011 */ LET_SUBB_EV_TUNER_END,
/* *** */ LET_SUBB_EV_MAX
};

/* LET SubB Task CallBack */
enum LET_SUBB_EVENT_RES_DEF{
/* 000 */ LET_SUBB_EV_INVALID_RES = LET_SUBB_EV_MAX,
/* 001 */ LET_SUBB_EV_AM_MODE_ON_RES,
/* 002 */ LET_SUBB_EV_AM_MODE_OFF_RES,
/* 003 */ LET_SUBB_EV_FREQ_SET_RES,
/* 004 */ LET_SUBB_EV_FM_MODE_ON_RES,
/* 005 */ LET_SUBB_EV_FM_MODE_OFF_RES,
/* 006 */ LET_SUBB_EV_SM_CHK_RES,
/* 007 */ LET_SUBB_EV_TIMEOUT,
/* *** */ LET_SUBB_EV_MAX_RES
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

/* not used. */

/**********************************************************************************************/
/*                           Global Function Prototype Declaration                            */
/**********************************************************************************************/
extern void TSK_LETAPP(void);
extern void TSK_LETAPP_SUBA(void);
extern void TSK_LETAPP_SUBB(void);
extern Type_uByte wubLetApp_Reset(void);
extern Type_uByte wubLetApp_ForceStandby(void);
extern Type_uByte wubLetApp_StandbyIn(void);
extern Type_uByte wubLetApp_StandbyOut(void);
extern void wubLetApp_SendMail(Type_uByte aubsrc,Type_uHWord auhEventCode, void* avdBuffer_p, Type_uWord auwSize);
extern void wubLetAppSubA_SendMail(Type_uHWord auhEventCode, void* avdBuffer_p, Type_uWord auwSize);
extern void wubLetAppSubB_SendMail(Type_uHWord auhEventCode, void* avdBuffer_p, Type_uWord auwSize);
extern void wubLetApp_RpcSendMail(Type_uByte aubModleId, Type_uHWord auhEventCode, void* avdBuffer_p, Type_uWord auwSize);
extern void wvdLetApp_RpcCallback(const Type_uByte aubsrc, const Type_uHWord auhevent, void* avdQueueBuffer_p, const Type_uHWord aubQueueSize);
extern void wvdLetAppSubA_RpcCallback(const Type_uByte aubsrc, const Type_uHWord auhevent, void* avdQueueBuffer_p, const Type_uHWord aubQueueSize);
extern void wvdLetAppSubB_RpcCallback(const Type_uByte aubsrc, const Type_uHWord auhevent, void* avdQueueBuffer_p, const Type_uHWord aubQueueSize);


#endif
/* end of file */

