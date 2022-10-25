/**********************************************************************************************
*  file name                : klinedrv_func.c
*  date                     : 2016.06.01
*  autho                    : zhangjinzhu
*  model                    : TBTA
*  description              : kline execute related file
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
#include "os_framework_cfg.h"

#include "apn_basic_type.h"
#include "generic.h"

/* kline Header Files */
#include "klinedrv.h"
#include "klinedrv_func.h"
#include "klinedrv_os.h"
/* Other Components */
//#include "srMN103LF26R.h"
#include "letapp.h"
#include "uart.h"
//#include "dserial.h"
#include "rpc_module.h"
#include "rpc_if.h"

/********************************************************************************************/
/*                           Macro Definition Section                                       */
/********************************************************************************************/
/* Wakeup Timer */
#define KLINE_TIM_WUP_LOW_MIN     (24U)        /* wakeup timer Tinil Min, -1 is deviation */
#define KLINE_TIM_WUP_LOW_MAX     (26 + 1)     /* wakeup timer Tinil Max, +1 is deviation */
#define KLINE_TIM_WUP_PITCH_MIN   (49 - 1)     /* wakeup timer TWup Min, -1 is deviation */
#define KLINE_TIM_WUP_PITCH_MAX   (51 + 1)     /*  wakeup timer TWup Max, +1 is deviation (+2 bytes read Delay) */
/* Kline negative service id */
#define KLINE_SID_POSITIVE_MARK   (0x40U)
#define KLINE_SID_COM_START       (0x81U)
#define KLINE_DB1_COM_START       (0xE9U)
#define KLINE_DB2_COM_START       (0x8FU)
#define KLINE_SID_TEST_PRESENT    (0x3EU)

/********************************************************************************************/
/*                           Enumeration Type Definition Section                            */
/********************************************************************************************/
enum KLINEDRV_CMD_CHK_FORMAT{
    CMD_CHK_DATA_LEN = 0U,
    CMD_CHK_DB_ID_LEN,
    CMD_CHK_DB_ID1,
    CMD_CHK_DB_ID2,
    CMD_CHK_EXE_KIND,
    CMD_CHK_DATA_CHK,
    CMD_CHK_INFO_NOTICE,
    CMD_CHK_MAX
};

enum KLINEDRV_EXT_KIND{
    EXE_KIND_INFO_LET = 0U,
    EXE_KIND_START_COM_RES,
    EXE_KIND_PRESENT_RES,
    EXE_KIND_RCV_WAIT,
    EXE_KIND_NON
};

enum KLINEDRV_CHK_DATA{
    CHK_DATA_NON = 0U,
    CHK_DATA_VALID
};

/********************************************************************************************/
/*                           User type definition section                                   */
/********************************************************************************************/

typedef struct{
    Type_uByte ubChkData[CMD_CHK_MAX];
}KLineDrvCmd_ts;

typedef struct {
	Type_uByte ubSid;
	Type_uByte ubTblSize;
	KLineDrvCmd_ts *stTbl_p;
}KlineDrvSidChk_ts;

typedef struct {
	Type_uByte ubSid;
	Type_uByte ubTblSize;
	Type_uByte *stTbl_p;
}KlineDrvNoUseSidChk_ts;

/********************************************************************************************/
/*                           Structure/Union Type Definition Section                        */
/********************************************************************************************/

/* not used. */

/********************************************************************************************/
/*                           ROM Table Section                                              */
/********************************************************************************************/

/* Sid=0x81  SID_StartCommunication */
static const KLineDrvCmd_ts nstKLineDrvCmdStartCom_a[] = {
    /*      DataLength, DidNum, Did1,   Did2,   ExeKind ,                   DataCheReq,         InfoNotice  */
    {   {   0x01,       0x00,   0x00,   0x00,   EXE_KIND_START_COM_RES  ,   CHK_DATA_NON    ,   LETAPP_EV_INVALID               }   },  /* CMD_START_COM            */
};

/* Sid=0x82  SID_StopCommunication */
static const KLineDrvCmd_ts nstKLineDrvCmdStopCom_a[] = {
    /*      DataLength, DidNum, Did1,   Did2,   ExeKind ,                   DataCheReq,         InfoNotice  */
    {   {   0x01,       0x00,   0x00,   0x00,   EXE_KIND_INFO_LET       ,   CHK_DATA_NON    ,   LETAPP_EV_AUDIO_DIAG_END        }   },  /* CMD_AUDIO_DIAG_END       */
};

/* Sid=0x3E  SID_TesterPresent */
static const KLineDrvCmd_ts nstKLineDrvCmdTesterPresent_a[] = {
    /*      DataLength, DidNum, Did1,   Did2,   ExeKind ,                   DataCheReq,         InfoNotice  */
 /* {   {   0x02,       0x01,   0x01,   0x00,   EXE_KIND_PRESENT_RES    ,   CHK_DATA_NON    ,   LETAPP_EV_INVALID               }   }, */ /* CMD_COM_CONTINUE1        */
 /* {   {   0x02,       0x01,   0x02,   0x00,   EXE_KIND_RCV_WAIT       ,   CHK_DATA_NON    ,   LETAPP_EV_INVALID               }   }, */ /* CMD_COM_CONTINUE2        */
    {   {   0x01,       0x00,   0x00,   0x00,   EXE_KIND_PRESENT_RES    ,   CHK_DATA_NON    ,   LETAPP_EV_INVALID               }   },  /* CMD_COM_CONTINUE0        */
};

/* Sid=0x10  SID_StartDiagnostic */
static const KLineDrvCmd_ts nstKLineDrvCmdStartDiag_a[] = {
    /*      DataLength, DidNum, Did1,   Did2,   ExeKind ,                   DataCheReq,         InfoNotice  */
    {   {   0x02,       0x01,   0x81,   0x00,   EXE_KIND_INFO_LET       ,   CHK_DATA_NON    ,   LETAPP_EV_AUDIO_DIAG_START      }   },  /* CMD_AUDIO_DIAG_START     */
};

/* Sid=0x11  SID_EcuReset */
static const KLineDrvCmd_ts nstKLineDrvCmdEcuReset_a[] = {
    /*      DataLength, DidNum, Did1,   Did2,   ExeKind ,                   DataCheReq,         InfoNotice  */
     {   {   0x01,       0x00,   0x00,   0x00,   EXE_KIND_INFO_LET       ,   CHK_DATA_NON    ,   LETAPP_EV_AUDIO_DIAG_END        }   },  /* CMD_AUDIO_DIAG_END      */ /* Cannot delete this not support command, otherwise Make error,so change DataLength to 0x00*/
};

/* Sid=0x31  SID_StartRoutine */
static const KLineDrvCmd_ts nstKLineDrvCmdStartRoutine_a[] = {
    /*      DataLength, DidNum, Did1,   Did2,   ExeKind ,                   DataCheReq,         InfoNotice  */
    /*{   {   0x03,       0x02,   0x01,   0x01,   EXE_KIND_INFO_LET       ,   CHK_DATA_NON    ,   LETAPP_EV_AM_ANT_START          }   },*/  /* CMD_AM_ANT_START         */
    /*{   {   0x0B,       0x02,   0x01,   0x02,   EXE_KIND_INFO_LET       ,   CHK_DATA_VALID  ,   LETAPP_EV_AM_ANT1               }   },*/  /* CMD_AM_ANT1              */
    /*{   {   0x0B,       0x02,   0x01,   0x03,   EXE_KIND_INFO_LET       ,   CHK_DATA_VALID  ,   LETAPP_EV_AM_ANT2               }   },*/  /* CMD_AM_ANT2              */
    /*{   {   0x0B,       0x02,   0x01,   0x04,   EXE_KIND_INFO_LET       ,   CHK_DATA_VALID  ,   LETAPP_EV_AM_ANT3               }   },*/  /* CMD_AM_ANT3              */
    /*{   {   0x03,       0x02,   0x02,   0x01,   EXE_KIND_INFO_LET       ,   CHK_DATA_NON    ,   LETAPP_EV_FM_ANT_START          }   },*/  /* CMD_FM_ANT_START         */
    /*{   {   0x0B,       0x02,   0x02,   0x02,   EXE_KIND_INFO_LET       ,   CHK_DATA_VALID  ,   LETAPP_EV_FM_ANT1               }   },*/  /* CMD_FM_ANT1              */
    /*{   {   0x0B,       0x02,   0x02,   0x03,   EXE_KIND_INFO_LET       ,   CHK_DATA_VALID  ,   LETAPP_EV_FM_ANT2               }   },*/  /* CMD_FM_ANT2              */
    /*{   {   0x0B,       0x02,   0x02,   0x04,   EXE_KIND_INFO_LET       ,   CHK_DATA_VALID  ,   LETAPP_EV_FM_ANT3               }   },*/  /* CMD_FM_ANT3              */
    /*{   {   0x03,       0x02,   0x10,   0x01,   EXE_KIND_INFO_LET       ,   CHK_DATA_NON    ,   LETAPP_EV_USB_TEST_START        }   },*/  /* CMD_CD_TEST_START        */
    /*{   {   0x04,       0x02,   0x11,   0x01,   EXE_KIND_INFO_LET       ,   CHK_DATA_VALID  ,   LETAPP_EV_VOL_SET               }   },*/  /* SET_VOLUME_LEVEL         */
    /*{   {   0x04,       0x02,   0x11,   0x02,   EXE_KIND_INFO_LET       ,   CHK_DATA_VALID  ,   LETAPP_EV_BASS_SET              }   },*/  /* SET_BASS_LEVEL           */
    /*{   {   0x04,       0x02,   0x11,   0x03,   EXE_KIND_INFO_LET       ,   CHK_DATA_VALID  ,   LETAPP_EV_TREBLE_SET            }   },*/  /* SET_TREBLE_LEVEL         */
    /*{   {   0x04,       0x02,   0x11,   0x04,   EXE_KIND_INFO_LET       ,   CHK_DATA_VALID  ,   LETAPP_EV_SET_BIBIRI_CHECK      }   },*/  /* SET_BIBIRI_CHECK         */
    /*{   {   0x03,       0x02,   0x12,   0x01,   EXE_KIND_INFO_LET       ,   CHK_DATA_NON    ,   LETAPP_EV_SPK_CHK_START         }   },*/  /* CMD_SPK_CHECK_START      */
    /*{   {   0x04,       0x02,   0x12,   0x01,   EXE_KIND_INFO_LET       ,   CHK_DATA_VALID  ,   LETAPP_EV_SPK_CHK_START         }   },*/  /* CMD_SPK_CHECK_START      */
    /*{   {   0x03,       0x02,   0x13,   0x01,   EXE_KIND_INFO_LET       ,   CHK_DATA_NON    ,   LETAPP_EV_SPK_BIBIRI_START      }   },*/  /* CMD_SPK_BIBIRI_START     */
    {   {   0x09,	    0x02,   0x40,   0x01,   EXE_KIND_INFO_LET	    ,	CHK_DATA_VALID	,   LETAPP_EV_BSM_START_TP_SET	 	}   },	/* CMD_BSM_STAR_TP_SET		*/
    /*{   {   0x03,       0x02,   0x80,   0x01,   EXE_KIND_INFO_LET       ,   CHK_DATA_NON    ,   LETAPP_EV_TUNER_TEST_START      }   },*/  /* CMD_TUNER_TEST_START     */
    /*{   {   0x0B,       0x02,   0x80,   0x02,   EXE_KIND_INFO_LET       ,   CHK_DATA_VALID  ,   LETAPP_EV_SET_TUNER_TEST        }   },*/  /* SET_TUNER_TEST           */
    /*{   {   0x03,       0x02,   0x81,   0x01,   EXE_KIND_INFO_LET       ,   CHK_DATA_NON    ,   LETAPP_EV_SPK_BEEP_START        }   },*/  /* CMD_SPK_BEEP_START       */
    /*{   {   0x04,       0x02,   0x81,   0x01,   EXE_KIND_INFO_LET       ,   CHK_DATA_VALID  ,   LETAPP_EV_SPK_BEEP_START        }   },*/  /* CMD_SPK_BEEP_START       */
    /*{   {   0x03,       0x02,   0x81,   0x02,   EXE_KIND_INFO_LET       ,   CHK_DATA_NON    ,   LETAPP_EV_SPK_BEEP_CH_UP        }   },*/  /* CMD_SPK_BEEP_CH_UP       */
    /*{   {   0x03,       0x02,   0xA0,   0x01,   EXE_KIND_INFO_LET       ,   CHK_DATA_NON    ,   LETAPP_EV_MEMORY_CLEAR_START    }   },*/  /* CMD_MEMORY_CLEAR_START   */
};

/* Sid=0x32  SID_StopRoutine */
static const KLineDrvCmd_ts nstKLineDrvCmdStopRoutine_a[] = {
    /*      DataLength, DidNum, Did1,   Did2,   ExeKind ,                   DataCheReq,         InfoNotice  */
    /*{   {   0x03,       0x02,   0x01,   0x01,   EXE_KIND_INFO_LET       ,   CHK_DATA_NON    ,   LETAPP_EV_AM_ANT_END            }   },*/  /* CMD_AM_ANT_END           */
    /*{   {   0x03,       0x02,   0x01,   0x02,   EXE_KIND_INFO_LET       ,   CHK_DATA_NON    ,   LETAPP_EV_AM_ANT1_END           }   },*/  /* CMD_AM_ANT1_END          */
    /*{   {   0x03,       0x02,   0x01,   0x03,   EXE_KIND_INFO_LET       ,   CHK_DATA_NON    ,   LETAPP_EV_AM_ANT2_END           }   },*/  /* CMD_AM_ANT2_END          */
    /*{   {   0x03,       0x02,   0x01,   0x04,   EXE_KIND_INFO_LET       ,   CHK_DATA_NON    ,   LETAPP_EV_AM_ANT3_END           }   },*/  /* CMD_AM_ANT3_END          */
    /*{   {   0x03,       0x02,   0x02,   0x01,   EXE_KIND_INFO_LET       ,   CHK_DATA_NON    ,   LETAPP_EV_FM_ANT_END            }   },*/  /* CMD_FM_ANT_END           */
    /*{   {   0x03,       0x02,   0x02,   0x02,   EXE_KIND_INFO_LET       ,   CHK_DATA_NON    ,   LETAPP_EV_FM_ANT1_END           }   },*/  /* CMD_FM_ANT1_END          */
    /*{   {   0x03,       0x02,   0x02,   0x03,   EXE_KIND_INFO_LET       ,   CHK_DATA_NON    ,   LETAPP_EV_FM_ANT2_END           }   },*/  /* CMD_FM_ANT2_END          */
    /*{   {   0x03,       0x02,   0x02,   0x04,   EXE_KIND_INFO_LET       ,   CHK_DATA_NON    ,   LETAPP_EV_FM_ANT3_END           }   },*/  /* CMD_FM_ANT3_END          */
    /*{   {   0x03,       0x02,   0x10,   0x01,   EXE_KIND_INFO_LET       ,   CHK_DATA_NON    ,   LETAPP_EV_USB_TEST_END          }   },*/  /* CMD_CD_TEST_END          */
    /*{   {   0x03,       0x02,   0x12,   0x01,   EXE_KIND_INFO_LET       ,   CHK_DATA_NON    ,   LETAPP_EV_SPK_CHK_END           }   },*/  /* CMD_SPK_CHECK_END        */
    /*{   {   0x03,       0x02,   0x13,   0x01,   EXE_KIND_INFO_LET       ,   CHK_DATA_NON    ,   LETAPP_EV_BIBIRI_CHECK_END      }   },*/  /* CMD_SPK_BIBIRI_END       */
    {   {   0x03,	    0x02,   0x40,   0x01,   EXE_KIND_INFO_LET	    ,	CHK_DATA_NON	,   LETAPP_EV_BSM_END				}   },	/* CMD_BSM_END				*/
    /*{   {   0x03,       0x02,   0x80,   0x01,   EXE_KIND_INFO_LET       ,   CHK_DATA_NON    ,   LETAPP_EV_TUNER_TEST_END        }   },*/  /* CMD_TUNER_TEST_END       */
    /*{   {   0x03,       0x02,   0x81,   0x01,   EXE_KIND_INFO_LET       ,   CHK_DATA_NON    ,   LETAPP_EV_SPK_BEEP_END          }   },*/  /* CMD_SPK_BEEP_END         */
    /*{   {   0x03,       0x02,   0xA0,   0x01,   EXE_KIND_INFO_LET       ,   CHK_DATA_NON    ,   LETAPP_EV_MEMORY_CLEAR_END      }   },*/  /* CMD_MEMORY_CLEAR_END     */
};

/* Sid=0x21  SID_ReadData */
static const KLineDrvCmd_ts nstKLineDrvCmdReadData_a[] = {
    /*      DataLength, DidNum, Did1,   Did2,   ExeKind ,                   DataCheReq,         InfoNotice  */
    {   {   0x02,       0x01,   0xF0,   0x00,   EXE_KIND_INFO_LET       ,   CHK_DATA_NON    ,   LETAPP_EV_AUDIO_DWG_REST        }   },  /* CMD_AUDIO_DWG_REST       */
    {   {   0x02,       0x01,   0xF1,   0x00,   EXE_KIND_INFO_LET       ,   CHK_DATA_NON    ,   LETAPP_EV_AUDIO_DATE_REST       }   },  /* CMD_AUDIO_DATE_REST      */
    {   {   0x02,       0x01,   0xF2,   0x00,   EXE_KIND_INFO_LET       ,   CHK_DATA_NON    ,   LETAPP_EV_AUDIO_SERIAL_REST     }   },  /* CMD_AUDIO_SERIAL_REST    */
 	{   {   0x02,       0x01,   0xF3,   0x00,   EXE_KIND_INFO_LET       ,   CHK_DATA_NON    ,   LETAPP_EV_AUDIO_VIN_REST        }   },	/* CMD_AUDIO_VIN_REST       */ 
    /*{   {   0x02,       0x01,   0x10,   0x00,   EXE_KIND_INFO_LET       ,   CHK_DATA_NON    ,   LETAPP_EV_AM_ANT_REST           }   },*/  /* CMD_AM_ANT_REST          */
    /*{   {   0x02,       0x01,   0x11,   0x00,   EXE_KIND_INFO_LET       ,   CHK_DATA_NON    ,   LETAPP_EV_FM_ANT_REST           }   },*/  /* CMD_FM_ANT_REST          */
    /*{   {   0x02,       0x01,   0x21,   0x00,   EXE_KIND_INFO_LET       ,   CHK_DATA_NON    ,   LETAPP_EV_SPK_CHECK_REST        }   },*/  /* CMD_SPK_CHECK_REST       */
    /*{   {   0x02,       0x01,   0x22,   0x00,   EXE_KIND_INFO_LET       ,   CHK_DATA_NON    ,   LETAPP_EV_BIBIRI_CHK_REST       }   },*/  /* CMD_BIBIRI_CHECK_REST    */
    /*{   {   0x02,       0x01,   0x42,   0x00,   EXE_KIND_INFO_LET       ,   CHK_DATA_NON    ,   LETAPP_EV_AUTO_VOL_VSP_REST     }   },*/  /* CMD_AUTO_VOL_VSP_REST    */
  	/*{   {   0x02,       0x01,   0x43,   0x00,   EXE_KIND_INFO_LET       ,   CHK_DATA_NON    ,   LETAPP_EV_VIN_COMP_REST         }   },*//* CMD_AUDIO_VIN_COMP_REST  */
    /*{   {   0x02,       0x01,   0x50,   0x00,   EXE_KIND_INFO_LET       ,   CHK_DATA_NON    ,   LETAPP_EV_TOTAL_TIME_REST       }   },*/  /* CMD_TOTAL_TIME_REST      */
    /*{   {   0x02,       0x01,   0x51,   0x00,   EXE_KIND_INFO_LET       ,   CHK_DATA_NON    ,   LETAPP_EV_MODE_TIME_REST        }   },*/  /* CMD_MODE_TIME_REST       */
    /*{   {   0x02,       0x01,   0x57,   0x00,   EXE_KIND_INFO_LET       ,   CHK_DATA_NON    ,   LETAPP_EV_HOT_ERR_REST          }   },*/  /* CMD_HOT_ERROR_REST       */
    /*{   {   0x02,       0x01,   0x60,   0x00,   EXE_KIND_INFO_LET       ,   CHK_DATA_NON    ,   LETAPP_EV_TUNER_TEST_REST       }   },*/  /* CMD_TUNER_TEST_REST      */
    /*{   {   0x02,       0x01,   0x61,   0x00,   EXE_KIND_INFO_LET       ,   CHK_DATA_NON    ,   LETAPP_EV_SPK_BEEP_REST         }   },*/  /* CMD_SPK_BEEP_REST        */
    {   {   0x02,       0x01,   0x6E,   0x00,   EXE_KIND_INFO_LET       ,   CHK_DATA_NON    ,   LETAPP_EV_INLINE_DIAG_REST      }   },  /* CMD_IN_LINE_DIAG_REST    */
    /*{   {   0x02,       0x01,   0x6F,   0x00,   EXE_KIND_INFO_LET       ,   CHK_DATA_NON    ,   LETAPP_EV_AUDIO_DIAG_REST       }   },*/  /* CMD_AUDIO_DIAG_REST      */
    {	{   0x02,	    0x01,   0x70,   0x00,   EXE_KIND_INFO_LET	    ,	CHK_DATA_NON	,   LETAPP_EV_BSM_REST			    }   },	/* CMD_BSM_REST 	        */
};

/* Sid=0x3B  SID_WriteData */
static const KLineDrvCmd_ts nstKLineDrvCmdWriteData_a[] = {
    /*      DataLength, DidNum, Did1,   Did2,   ExeKind ,                   DataCheReq,         InfoNotice  */
    /*{   {   0x02,       0x01,   0xA0,   0x00,   EXE_KIND_INFO_LET       ,   CHK_DATA_NON    ,   LETAPP_EV_LET_ERR_MEMORY_CLEAR  }   },*/  /* CMD_ERROR_MEMORY_CLEAR     */
    /*{   {   0x02,       0x01,   0xA1,   0x00,   EXE_KIND_INFO_LET       ,   CHK_DATA_NON    ,   LETAPP_EV_LET_MEMORY_CLEAR      }   },*/  /* CMD_LET_MEMORY_CLEAR     */
    /*{   {   0x02,       0x01,   0xA2,   0x00,   EXE_KIND_INFO_LET       ,   CHK_DATA_NON    ,   LETAPP_EV_INITIAL_CLEAR         }   },*/  /* CMD_INITIAL_MEMORY_CLEAR */
    {   {   0x02,       0x01,   0xA3,   0x00,   EXE_KIND_INFO_LET       ,   CHK_DATA_NON    ,   LETAPP_EV_INLINE_DIAG_CLEAR     }   },  /* CMD_IN_LINE_DIAG_CLEAR   */
    {   {   0x02,       0x01,   0xA5,   0x00,   EXE_KIND_INFO_LET       ,   CHK_DATA_NON    ,   LETAPP_EV_BSM_INITIAL           }   },  /* CMD_BSM_INITIALIZE   */
};

static const KlineDrvSidChk_ts nstSidChkTbl_a[] = {
    /* SID         Table Size                                    Table point */
    {  0x81U,      sizeof(nstKLineDrvCmdStartCom_a) / sizeof(KLineDrvCmd_ts),             (KLineDrvCmd_ts*)(&nstKLineDrvCmdStartCom_a[KLINEDRV_NUM_0])          },
    {  0x82U,      sizeof(nstKLineDrvCmdStopCom_a) / sizeof(KLineDrvCmd_ts),              (KLineDrvCmd_ts*)(&nstKLineDrvCmdStopCom_a[KLINEDRV_NUM_0])           },
    {  0x3EU,      sizeof(nstKLineDrvCmdTesterPresent_a) / sizeof(KLineDrvCmd_ts),        (KLineDrvCmd_ts*)(&nstKLineDrvCmdTesterPresent_a[KLINEDRV_NUM_0])     },
    {  0x10U,      sizeof(nstKLineDrvCmdStartDiag_a) / sizeof(KLineDrvCmd_ts),            (KLineDrvCmd_ts*)(&nstKLineDrvCmdStartDiag_a[KLINEDRV_NUM_0])         },
    {  0x11U,      sizeof(nstKLineDrvCmdEcuReset_a) / sizeof(KLineDrvCmd_ts),             (KLineDrvCmd_ts*)(&nstKLineDrvCmdEcuReset_a[KLINEDRV_NUM_0])          },
    {  0x31U,      sizeof(nstKLineDrvCmdStartRoutine_a) / sizeof(KLineDrvCmd_ts),         (KLineDrvCmd_ts*)(&nstKLineDrvCmdStartRoutine_a[KLINEDRV_NUM_0])      },
    {  0x32U,      sizeof(nstKLineDrvCmdStopRoutine_a) / sizeof(KLineDrvCmd_ts),          (KLineDrvCmd_ts*)(&nstKLineDrvCmdStopRoutine_a[KLINEDRV_NUM_0])       },
    {  0x21U,      sizeof(nstKLineDrvCmdReadData_a) / sizeof(KLineDrvCmd_ts),             (KLineDrvCmd_ts*)(&nstKLineDrvCmdReadData_a[KLINEDRV_NUM_0])          },
    {  0x3BU,      sizeof(nstKLineDrvCmdWriteData_a) / sizeof(KLineDrvCmd_ts),            (KLineDrvCmd_ts*)(&nstKLineDrvCmdWriteData_a[KLINEDRV_NUM_0])         }
};

/********************************************************************************************/
/*                           No Use Table Section                                              */
/********************************************************************************************/

/* Sid No Use  */
static const Type_uByte nstSidNoUseTbl_a[] = {
    /* SID */
    0x83U,      
   	0x11U,      
	0x27U,      
	0x1AU,      
	0x23U,      
	0x2CU,      
	0x3DU,      
	0x14U,      
	0x17U,      
	0x18U,      
	0x30U,      
	0x34U,      
	0x35U,      
	0x36U, 
	0x37U
};



/* Sid=0x31 Did No Use  */
static const Type_uByte nstStartRoutineDidNoUseTbl_a[] = {
    /* DID */
	0x11U, 
	0x80U, 
	0x81U, 
	0x82U, 
	0x83U, 
	0x84U, 
	0x86U
};
	
/* Sid=0x32 Did No Use  */
static const Type_uByte nstStopRoutineDidNoUseTbl_a[] = {
    /* DID */
	0x80U, 
	0x81U, 
	0x82U, 
	0x83U, 
	0x84U, 
	0x85U, 
	0x86U
};
	
/* Sid=0x21 Did No Use  */
static const Type_uByte nstReadDataDidNoUseTbl_a[] = {
     /* DID */
	0xF0U, 
	0xF1U, 
	0xF2U, 
	0xF3U, 
	0x1AU, 
	0x40U, 
	0x42U, 
	0x43U, 
	0x60U,
	0x61U,
	0x62U,
	0x63U,
	0x64U,
	0x65U,
	0x66U,
	0x6DU, 
	0x6EU
};

/* Sid=0x3B Did No Use  */
static const Type_uByte nstWriteDataDidNoUseTbl_a[] = {
    /* DID */
	0xA3U, 
	0xA4U
};

/* Sid Use but Did No Use  */
static const KlineDrvNoUseSidChk_ts nstSidUse_DidNoUseTbl_a[] = {
	/* 	SID         Table Size                              Table point */
	{	0x31U,		sizeof(nstStartRoutineDidNoUseTbl_a),	(Type_uByte*)(&nstStartRoutineDidNoUseTbl_a[KLINEDRV_NUM_0])      },    
	{	0x32U,		sizeof(nstStopRoutineDidNoUseTbl_a),	(Type_uByte*)(&nstStopRoutineDidNoUseTbl_a[KLINEDRV_NUM_0])       },   
	{	0x21U,		sizeof(nstReadDataDidNoUseTbl_a),		(Type_uByte*)(&nstReadDataDidNoUseTbl_a[KLINEDRV_NUM_0])          }, 
	{	0x3BU,		sizeof(nstWriteDataDidNoUseTbl_a),		(Type_uByte*)(&nstWriteDataDidNoUseTbl_a[KLINEDRV_NUM_0])         }
};

/********************************************************************************************/
/*                           Static Variable Definition Section                             */
/********************************************************************************************/

static Type_uByte nubWakeupTimer;

/********************************************************************************************/
/*                           Static Function Declaration Section                            */
/********************************************************************************************/

static void nvdKlineDrvCmdReq(Type_uByte* aubChkData_p);
static void nvdKlineDrvComStartRes(void);
static void nvdKlineDrvPresentRes(void);
static void nvdKlineAllDiagRestSet(Type_uByte aubEventCode);

/********************************************************************************************/
/*                           Global variable definition section                             */
/********************************************************************************************/
Type_uByte wubKlineMode;
Type_uByte wubKlineDataRcvBuf[KLINEDRV_LEN_MESSAGE];
Type_uByte wubKlineDataRcvLen;
Type_uByte wubKlineDataBuf[KLINEDRV_LEN_MESSAGE];
Type_uByte wubKlineDataLen;
Type_uByte wubKlineDataResBuf[KLINEDRV_LEN_MESSAGE];
Type_uByte wubKlineDataResLen;
Type_uByte wubKlineResSts;
Type_uByte wubKlineAllResChk;
Type_uByte wubKlineFlameDataBuf[KLINEDRV_LEN_MESSAGE * KLINEDRV_NUM_4];
/********************************************************************************************/
/*                           Function Definition Section                                    */
/********************************************************************************************/
/*********************************************************************************************
*  function                 : wvdKlineDrvInitial
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
void wvdKlineDrvInitial(void)
{
    wubIdleWaitTimerAct = KLINE_TIMER_WAIT_FALSE;
    wubP2TimerAct = KLINE_TIMER_WAIT_FALSE;
    wubP3TimerAct = KLINE_TIMER_WAIT_FALSE;
    wubIdleWaitTimerCount = KLINEDRV_NUM_0;
    wubP2TimerCount = KLINEDRV_NUM_0;
    wuhP3TimerCount = KLINEDRV_NUM_0;
    wubKlineMode = KLINE_MODE_IDLE_WAIT;
    nubWakeupTimer = KLINEDRV_NUM_0;
    wubKlineDataLen = KLINEDRV_NUM_0;
    wubKlineDataRcvLen = KLINEDRV_NUM_0;
    wubKlineDataResLen = KLINEDRV_NUM_0;
    wubKlineResSts = KLINEDRV_FLASE;
    wubKlineAllResChk = KLINEDRV_FLASE;
    wvdGen_MemSet(&wubKlineDataRcvBuf[KLINEDRV_NUM_0], KLINEDRV_NUM_0, sizeof(wubKlineDataRcvBuf));
    wvdGen_MemSet(&wubKlineDataBuf[KLINEDRV_NUM_0], KLINEDRV_NUM_0, sizeof(wubKlineDataBuf));
    wvdGen_MemSet(&wubKlineDataResBuf[KLINEDRV_NUM_0], KLINEDRV_NUM_0, sizeof(wubKlineDataResBuf));
    wvdGen_MemSet(&wubKlineFlameDataBuf[KLINEDRV_NUM_0], KLINEDRV_NUM_0, sizeof(wubKlineFlameDataBuf));
}
/*********************************************************************************************
*  function                 : wvdKlineDrvComInitial
*  date                     : 2016.07.01
*  autho                    : zhangjinzhu
*  description              : initial Receive
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*  
*********************************************************************************************/
void wvdKlineDrvComInitial(void)
{
    wvdKlineDrvInitial();
    wvdKlineIdleWaitTimerSet(KLINE_IDLE_WAIT_P3MIN);
}
/*********************************************************************************************
*  function                 : wvdKlineWakeupDetection
*  date                     : 2016.07.01
*  autho                    : zhangjinzhu
*  description              : Wakeup Detect
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*  
*********************************************************************************************/
void wvdKlineWakeupDetection(void)
{
    switch(wubKlineMode)
    {
        /* Kline Idle State */
        case KLINE_MODE_IDLE:
            if (KLINE_RX_STATE == GPIO_LOW){
                wubKlineMode = KLINE_MODE_INIT_LOW;				
                nubWakeupTimer = KLINEDRV_NUM_1;
            }
            else {
                nubWakeupTimer = KLINEDRV_NUM_0;
            }
            break;

        /* Kline WakeUp Low State */
        case KLINE_MODE_INIT_LOW:
            nubWakeupTimer += KLINEDRV_NUM_2;
            if (KLINE_RX_STATE == GPIO_HIGH){
                /* WakeUp Low MAX Check */
                if (nubWakeupTimer >= KLINE_TIM_WUP_LOW_MIN){
                    wubKlineMode = KLINE_MODE_INIT_HI;
					
                }
                else {
                    wubKlineMode = KLINE_MODE_IDLE;
					
                }
            }
            else {
                if(nubWakeupTimer > KLINE_TIM_WUP_LOW_MAX){
                    wubKlineMode = KLINE_MODE_IDLE_WAIT;
					
                    wvdKlineIdleWaitTimerSet(KLINE_IDLE_WAIT_P3MIN);
                }
                else {
                    /* No Action */
                }
            }
            break;

        /* Wake Up High State */
        case KLINE_MODE_INIT_HI:
            nubWakeupTimer += KLINEDRV_NUM_2;
            /* Wakeup High Time MAX */
            if (KLINE_RX_STATE == GPIO_HIGH){

                if (nubWakeupTimer > KLINE_TIM_WUP_PITCH_MAX ){
                    wubKlineMode = KLINE_MODE_IDLE;
					
                }
                else if ( nubWakeupTimer >= KLINE_TIM_WUP_PITCH_MIN ){
                    wubKlineMode = KLINE_MODE_INIT_REQ;
					
                }
                else {
                    /* No Action */
                }
            }
            else {
                wubKlineMode = KLINE_MODE_IDLE_WAIT;
				
                wvdKlineIdleWaitTimerSet(KLINE_IDLE_WAIT_P3MIN);
            }
            break;

        default:
            /* No Action */
            break;
    }
}
/*********************************************************************************************
*  function                 : wvdKlineDrvInt
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
void wvdKlineDrvInt(Type_uByte aubRcvData, Type_uByte aubState)
{
    if (KLINEDRV_CLEAR == aubState){
        wubKlineDataRcvBuf[wubKlineDataRcvLen] = aubRcvData;
		
		//KLINEDRV("aubRcvData = %x",aubRcvData);
        wubKlineDataRcvLen++;
        if (wubKlineDataRcvLen <= KLINEDRV_LEN_MESSAGE){
            (void)OS_SetEventFlag(FLG_ID_KLINEDRV, KLINEDRV_FLGPTN_RCVDATA);
			
			//KLINEDRV("set event flag 1");
        }
        else {
            wvdGen_MemSet(&wubKlineDataRcvBuf[KLINEDRV_NUM_0], KLINEDRV_NUM_0, sizeof(wubKlineDataRcvBuf));
            wubKlineDataRcvLen = KLINEDRV_NUM_0;
			
			//KLINEDRV("set event flag 2");
        }
    }
    else {
		
		//KLINEDRV("set event flag 3");
        wvdGen_MemSet(&wubKlineDataRcvBuf[KLINEDRV_NUM_0], KLINEDRV_NUM_0, sizeof(wubKlineDataRcvBuf));
        wubKlineDataRcvLen = KLINEDRV_NUM_0;
    }
}
/*********************************************************************************************
*  function                 : wvdKlineDrvCmdChk
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
void wvdKlineDrvCmdChk(void)
{
    Type_uByte* aubDid_p;
    Type_sWord aswCmpRes;
    Type_uByte aubCmdNum;
    Type_uByte aubSidChkTab;
	Type_uByte aubDidChkTab;
    Type_uByte aubDidSize;
	Type_uByte aubSid;
	
    aubDid_p = OS_NULL;
    aubDidSize = KLINEDRV_NUM_0;
    aubCmdNum = KLINEDRV_NUM_0;
    aubSidChkTab = KLINEDRV_NUM_0;
	aubDidChkTab = KLINEDRV_NUM_0;
    aswCmpRes = GEN_CMP_INVALID;
	aubSid = wubKlineDataBuf[KLINEDRV_BYTE_SID];
	aubDid_p = &wubKlineDataBuf[KLINEDRV_BYTE_DID];
	
	/*HU have no this SID function,resoponse 0x7F SID 0x23*/
	for(aubSidChkTab = KLINEDRV_NUM_0;aubSidChkTab < sizeof(nstSidNoUseTbl_a);aubSidChkTab++)
	{	
		if(nstSidNoUseTbl_a[aubSidChkTab] == aubSid)
		{
			wvdKlineDrvNackResMake(wubKlineDataBuf[KLINEDRV_BYTE_SID], KLINEDRV_RES_NOT_COMPLETE);
			return;
		}
		else
		{
			/* No Action */
		}
	}
	aubSidChkTab = KLINEDRV_NUM_0;
	
	/*HU Use this SID function,but Did No Use resoponse 0x7F SID 0x23*/
	for(aubSidChkTab = KLINEDRV_NUM_0;aubSidChkTab < (sizeof(nstSidUse_DidNoUseTbl_a) / sizeof(KlineDrvNoUseSidChk_ts));aubSidChkTab++)
	{
		if(nstSidUse_DidNoUseTbl_a[aubSidChkTab].ubSid == aubSid)
		{	
			for(aubDidChkTab = KLINEDRV_NUM_0;aubDidChkTab < (nstSidUse_DidNoUseTbl_a[aubSidChkTab].ubTblSize);aubDidChkTab++)
			{	
				if((nstSidUse_DidNoUseTbl_a[aubSidChkTab].stTbl_p)[aubDidChkTab] == *aubDid_p)
				{
					wvdKlineDrvNackResMake(wubKlineDataBuf[KLINEDRV_BYTE_SID], KLINEDRV_RES_NOT_COMPLETE);
					return;
				}
				else
				{
					/* No Action */
				}
			}
			aubDidChkTab = KLINEDRV_NUM_0;
		}
		else
		{
			/* No Action */
		}

    }
	aubSidChkTab = KLINEDRV_NUM_0;
		
    while ((aubSidChkTab < sizeof(nstSidChkTbl_a) / sizeof(KlineDrvSidChk_ts)) && (nstSidChkTbl_a[aubSidChkTab].ubSid != wubKlineDataBuf[KLINEDRV_BYTE_SID])){

        aubSidChkTab++;
    }
	
    if(aubSidChkTab < sizeof(nstSidChkTbl_a) / sizeof(KlineDrvSidChk_ts)){

        while ((aubCmdNum < nstSidChkTbl_a[aubSidChkTab].ubTblSize) && (GEN_CMP_INVALID == aswCmpRes)){
            if (nstSidChkTbl_a[aubSidChkTab].stTbl_p[aubCmdNum].ubChkData[CMD_CHK_DATA_LEN] == (wubKlineDataBuf[KLINEDRV_BYTE_FORMAT] & KLINEDRV_FMT_LEN_MASK)){
                aubDidSize = (nstSidChkTbl_a[aubSidChkTab].stTbl_p)[aubCmdNum].ubChkData[CMD_CHK_DB_ID_LEN];
                if (KLINEDRV_NUM_0 == aubDidSize) {
					
                    nvdKlineDrvCmdReq(&((nstSidChkTbl_a[aubSidChkTab].stTbl_p)[aubCmdNum].ubChkData[CMD_CHK_DATA_LEN]));
                    aswCmpRes = GEN_CMP_EQUAL;
                }
                else {
					
                    aubDid_p = &((nstSidChkTbl_a[aubSidChkTab].stTbl_p)[aubCmdNum].ubChkData[CMD_CHK_DB_ID1]);
                    aswCmpRes = wswGen_StrCmp(&wubKlineDataBuf[KLINEDRV_BYTE_DID], aubDid_p, aubDidSize);
                    if (GEN_CMP_EQUAL == aswCmpRes){
						
                        nvdKlineDrvCmdReq(&((nstSidChkTbl_a[aubSidChkTab].stTbl_p)[aubCmdNum].ubChkData[CMD_CHK_DATA_LEN]));
                    }
                    else {
						
                        aswCmpRes = GEN_CMP_INVALID;
                    }
                }
            }
            else {
                aswCmpRes = GEN_CMP_INVALID;
            }
            aubCmdNum++;
        }

        if (GEN_CMP_EQUAL == aswCmpRes){
            /* No Action */
        }
        else {
			
            wvdKlineDrvNackResMake(wubKlineDataBuf[KLINEDRV_BYTE_SID], KLINEDRV_RES_NOT_SUPPORT_PAR);/*Length is wrong,resoponse 0x7F SID 0x12*/
        }
    }
    else{
		
        wvdKlineDrvNackResMake(wubKlineDataBuf[KLINEDRV_BYTE_SID], KLINEDRV_RES_NOT_SUPPORT);/*Spec have no this SID function,resoponse 0x7F SID 0x11*/
    }
}
/*********************************************************************************************
*  function                 : nvdKlineDrvCmdReq
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
static void nvdKlineDrvCmdReq(Type_uByte* aubChkData_p)
{
	Type_sByte asbResult;

	asbResult = RES_NG;
	
    switch (aubChkData_p[CMD_CHK_EXE_KIND]){
        case EXE_KIND_INFO_LET:
			
            if (KLINEDRV_FLASE == wubKlineResSts){
                nvdKlineAllDiagRestSet(aubChkData_p[CMD_CHK_INFO_NOTICE]);				
				asbResult = wsbRPCSendMsg(ARPC_ID_KLINEDRV,ARPC_ID_LET,aubChkData_p[CMD_CHK_INFO_NOTICE], &wubKlineDataBuf[KLINEDRV_BYTE_SID], wubKlineDataLen - KLINEDRV_NUM_4);
				if(asbResult != RES_OK){
					KLINEDRV("nvdKlineDrvCmdReq NG");
				}
            }
            else {
                /* No Action */
            }
            break;
        case EXE_KIND_START_COM_RES:
			
            nvdKlineDrvComStartRes();
            break;
        case EXE_KIND_PRESENT_RES:
			
            nvdKlineDrvPresentRes();
            break;
        case EXE_KIND_RCV_WAIT:
			
            wvdKlineDrvComInitial();
            break;
        default:
            break;
    }
}
/*********************************************************************************************
*  function                 : nvdKlineDrvComStartRes
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
static void nvdKlineDrvComStartRes(void)
{
    Type_uByte aubKlineData[KLINEDRV_NUM_7];

    wvdGen_MemSet(&aubKlineData[KLINEDRV_NUM_0], KLINEDRV_NUM_0, sizeof(aubKlineData));

    aubKlineData[KLINEDRV_BYTE_FORMAT] = KLINEDRV_DATA_FMT | KLINEDRV_NUM_3;
    aubKlineData[KLINEDRV_BYTE_TARGET] = KLINEDRV_ADD_LET;
    aubKlineData[KLINEDRV_BYTE_SOURCE] = KLINEDRV_ADD_HU;
    aubKlineData[KLINEDRV_BYTE_SID] = KLINE_SID_COM_START + KLINE_SID_POSITIVE_MARK;
    aubKlineData[KLINEDRV_BYTE_DID] = KLINE_DB1_COM_START;
    aubKlineData[KLINEDRV_BYTE_DID + KLINEDRV_NUM_1] = KLINE_DB2_COM_START;

    aubKlineData[KLINEDRV_NUM_6] = wubKlineDrvMakeChkSum(&aubKlineData[KLINEDRV_NUM_0], KLINEDRV_NUM_6);

    wubKlineResSts = KLINEDRV_TURE;
    wubKlineDataResLen = sizeof(aubKlineData);
    wvdGen_MemCpy(&wubKlineDataResBuf[KLINEDRV_NUM_0], &aubKlineData[KLINEDRV_NUM_0], sizeof(aubKlineData));
}
/*********************************************************************************************
*  function                 : nvdKlineDrvPresentRes
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
static void nvdKlineDrvPresentRes(void)
{
    Type_uByte aubKlineData[KLINEDRV_NUM_5];

    wvdGen_MemSet(&aubKlineData[KLINEDRV_NUM_0], KLINEDRV_NUM_0, sizeof(aubKlineData));

    aubKlineData[KLINEDRV_BYTE_FORMAT] = KLINEDRV_DATA_FMT | KLINEDRV_NUM_1;
    aubKlineData[KLINEDRV_BYTE_TARGET] = KLINEDRV_ADD_LET;
    aubKlineData[KLINEDRV_BYTE_SOURCE] = KLINEDRV_ADD_HU;
    aubKlineData[KLINEDRV_BYTE_SID] = KLINE_SID_TEST_PRESENT + KLINE_SID_POSITIVE_MARK;

    aubKlineData[KLINEDRV_NUM_4] = wubKlineDrvMakeChkSum(&aubKlineData[KLINEDRV_NUM_0], KLINEDRV_NUM_4);

    wubKlineResSts = KLINEDRV_TURE;
    wubKlineDataResLen = sizeof(aubKlineData);
    wvdGen_MemCpy(&wubKlineDataResBuf[KLINEDRV_NUM_0], &aubKlineData[KLINEDRV_NUM_0], sizeof(aubKlineData));
}
/*********************************************************************************************
*  function                 : wubKlineDrv_SendMail
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
Type_uByte wubKlineDrv_SendMail(const Type_uHWord auhEventCode, const void *avdBuffer_p, const Type_uWord auwSize)
{
    OS_ErrCode_t aswErrCode;        /* <AUTO> Error Code */
    OS_VoidPtr_t avdMailBuff_p;    /* <AUTO> Memory Address */
    OS_StdQueue_ts astStdMail;       /* <AUTO> Mail */
    OS_TskId_t aswTaskId;           /* <AUTO> Task ID */
    Type_uByte aubReturn;           /* <AUTO> Return Code */
    Type_uWord auwSetBufferSize;

    /* Initalize */
    aswErrCode = OS_ERR_NG;
    aswTaskId = ARPC_ID_KLINEDRV;
    avdMailBuff_p = OS_NULL;
    aubReturn = RES_NG;
    wvdGen_MemSet(&astStdMail, KLINEDRV_NUM_0, sizeof(OS_StdQueue_ts));
    auwSetBufferSize = auwSize;

    //(void)OS_GetTaskId(&aswTaskId);

    if (KLINE_MPF_MAXSIZE < auwSetBufferSize){
        auwSetBufferSize = KLINE_MPF_MAXSIZE;
    }
    else{
        /* No Action */
    }

    if ((STD_NULL != avdBuffer_p) && (KLINEDRV_NUM_0 < auwSetBufferSize)){
		
        (void)OS_GetFMemoryBlock(&avdMailBuff_p, auwSetBufferSize);

        if (OS_NULL != avdMailBuff_p){
    
            /* Copy sending data */
            wvdGen_MemCpy(avdMailBuff_p, avdBuffer_p, auwSetBufferSize);
    		
            astStdMail.swSrcTaskId   = aswTaskId;
            astStdMail.uhEventCode   = auhEventCode;
            astStdMail.vdQueueBuffer_p = avdMailBuff_p;
            astStdMail.uwQueueSize    = auwSetBufferSize;
    
            aswErrCode = OS_SendQueue(MBX_ID_KLINEDRV, OS_MAIL_PRI_NORMAL, &astStdMail);
            if (OS_ERR_OK == aswErrCode){
                aubReturn = RES_OK;
            }
            else {
                /* Send Mail Error */
                if (OS_NULL != avdMailBuff_p){
                    (void)OS_ReleaseFMemoryBlock(avdMailBuff_p);
                    avdMailBuff_p = OS_NULL;
                }
            }
        }
        else {
            /* Memory Alloc Error */
        }
    }
    
    else {
		
        astStdMail.swSrcTaskId   = aswTaskId;
        astStdMail.uhEventCode   = auhEventCode;
        astStdMail.vdQueueBuffer_p = OS_NULL;
        astStdMail.uwQueueSize    = KLINEDRV_NUM_0;
    
        aswErrCode = OS_SendQueue(MBX_ID_KLINEDRV, OS_MAIL_PRI_NORMAL, &astStdMail);
    
        if (OS_ERR_OK == aswErrCode){
            aubReturn = RES_OK;
        }
        else {
            /* Send Mail Error */
        }
    }
    return aubReturn;
}
/*********************************************************************************************
*  function                 : wubKlineDrvNegativeRes
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
void wubKlineDrvNegativeRes(Type_uByte aubReqSerId, Type_uByte aubResCode)
{
    Type_uByte aubResBuff[KLINEDRV_NUM_3];
	Type_sByte asbResult;

	asbResult = RES_NG;
    wvdGen_MemSet(&aubResBuff[KLINEDRV_NUM_0], KLINEDRV_NUM_0, sizeof(aubResBuff));

    aubResBuff[KLINEDRV_NUM_0] = KLINE_NG_SERVICE_ID;
    aubResBuff[KLINEDRV_NUM_1] = aubReqSerId;
    aubResBuff[KLINEDRV_NUM_2] = aubResCode;
	asbResult = wsbRPCSendMsg(ARPC_ID_LET,ARPC_ID_KLINEDRV,KLINEDRV_EV_LET_RES,&aubResBuff[KLINEDRV_NUM_0],sizeof(aubResBuff));	
	if(asbResult != RES_OK){
		KLINEDRV("wubKlineDrvNegativeRes NG");
	}

}
/*********************************************************************************************
*  function                 : wubKlineDrvActiveRes
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
void wubKlineDrvActiveRes(Type_uByte aubReqSerId, Type_uByte* aubResData_p, Type_uByte aubResDataSize)
{
    Type_uByte aubDataBuff[KLINEDRV_LEN_DATA];
	Type_sByte asbResult;

	asbResult = RES_NG;
    wvdGen_MemSet(&aubDataBuff[KLINEDRV_NUM_0], KLINEDRV_NUM_0, sizeof(aubDataBuff));

    aubReqSerId = aubReqSerId + KLINE_SID_POSITIVE_MARK;
    aubDataBuff[KLINEDRV_NUM_0] = aubReqSerId;

    if (aubResDataSize < KLINEDRV_LEN_DATA){
        wvdGen_MemCpy(&aubDataBuff[KLINEDRV_NUM_1], aubResData_p, aubResDataSize);
		asbResult = wsbRPCSendMsg(ARPC_ID_LET,ARPC_ID_KLINEDRV,KLINEDRV_EV_LET_RES, &aubDataBuff[KLINEDRV_NUM_0],aubResDataSize + KLINEDRV_NUM_1);
		if(asbResult != RES_OK){
			KLINEDRV("wubKlineDrvActiveRes NG");
		}

	}
    else {
        /* No Action */
    }

}
/*********************************************************************************************
*  function                 : wubKlineDrvMakeChkSum
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
Type_uByte wubKlineDrvMakeChkSum(Type_uByte* aubChkData, Type_uByte aubChkSize)
{
    Type_uByte aubChkSum;

    aubChkSum = KLINEDRV_NUM_0;
    while(aubChkSize--){
		aubChkSum+=aubChkData[aubChkSize];
    }
    return(aubChkSum);
}
/*********************************************************************************************
*  function                 : wvdKlineDrvNackResMake
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
void wvdKlineDrvNackResMake(Type_uByte aubReqSerId, Type_uByte aubResCode)
{
    wubKlineDataResBuf[KLINEDRV_BYTE_FORMAT] = KLINEDRV_DATA_FMT + KLINEDRV_NUM_3;
    wubKlineDataResBuf[KLINEDRV_BYTE_TARGET] = KLINEDRV_ADD_LET;
    wubKlineDataResBuf[KLINEDRV_BYTE_SOURCE] = KLINEDRV_ADD_HU;
    wubKlineDataResBuf[KLINEDRV_BYTE_SID] = KLINE_NG_SERVICE_ID;
    wubKlineDataResBuf[KLINEDRV_BYTE_DID] = aubReqSerId;
    wubKlineDataResBuf[KLINEDRV_BYTE_DID + KLINEDRV_NUM_1] = aubResCode;
    wubKlineDataResBuf[KLINEDRV_NUM_6] = wubKlineDrvMakeChkSum(&wubKlineDataResBuf[KLINEDRV_NUM_0], KLINEDRV_NUM_6);
    wubKlineDataResLen = KLINEDRV_NUM_7;

    wubKlineResSts = KLINEDRV_TURE;
}
/*********************************************************************************************
*  function                 : nvdKlineAllDiagRestSet
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
static void nvdKlineAllDiagRestSet(Type_uByte aubEventCode)
{
    if ((LETAPP_EV_AUDIO_DIAG_REST == aubEventCode) && (wubKlineAllResChk == KLINEDRV_FLASE)){
		
        wubKlineAllResChk = KLINEDRV_TURE;
    }
    else {
		
        /* No Action */
    }
}
/* End of File */
