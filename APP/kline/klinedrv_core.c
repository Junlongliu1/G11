/**********************************************************************************************
*  file name                : klinedrv_core.c
*  date                     : 2016.06.01
*  autho                    : zhangjinzhu
*  model                    : TBTA
*  description              : kline Task execute related file
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
/* kline Header Files */
#include "klinedrv.h"
#include "klinedrv_func.h"
#include "klinedrv_os.h"
#include "klinedrv_core.h"
/* Other Components */
//#include "dserial.h"
#include "uart.h"
#include "model.h"
#include "letapp.h"
#include "rpc_module.h"
#include "rpc_if.h"
//#include "HAL_KLINEDRV/HAL_KlineDrv.h"
#include "HAL_API.h"
/********************************************************************************************/
/*                           Macro Definition Section                                       */
/********************************************************************************************/


/********************************************************************************************/
/*                           Enumeration Type Definition Section                            */
/********************************************************************************************/

enum KLINEDRV_INTERNAL_EVENT{
/* 000 */ EV_KLINE_REQ_DATA_RCV,
/* 001 */ EV_KLINE_P2MIN_TIMEOUT,
/* 002 */ EV_KLINE_P2MAX_TIMEOUT,
/* 003 */ EV_KLINE_P3MIN_TIMEOUT,
/* 004 */ EV_KLINE_P3MAX_TIMEOUT,
/* *** */ EV_KLINE_MAX
};

enum KLINEDRV_PROCESS{
/* 000 */ PCODE_KLINE_REQ_DATA_RCV,
/* 001 */ PCODE_KLINE_P2MIN_TIMEOUT,
/* 002 */ PCODE_KLINE_P2MAX_TIMEOUT,
/* 003 */ PCODE_KLINE_P3MIN_TIMEOUT,
/* 004 */ PCODE_KLINE_P3MAX_TIMEOUT,
/* *** */ PCODE_KLINE_MAX,
/* *** */ PCODE_KLINE_INVALID
};

/********************************************************************************************/
/*                           User type definition section                                   */
/********************************************************************************************/

/* not used. */

/********************************************************************************************/
/*                           Structure/Union Type Definition Section                        */
/********************************************************************************************/

typedef struct {
    Type_uHWord uhExternEvent;           /* Extern Ecent */
    Type_uByte ubInternalEvent;          /* Internal Event */
} EventExchange_ts;

/********************************************************************************************/
/*                           Static Variable Definition Section                             */
/********************************************************************************************/

/* not used. */

/********************************************************************************************/
/*                           Static Function Declaration Section                            */
/********************************************************************************************/
static void nvdProcRequestDataRcv(void);
static void nvdProcP2MinTimeOut(void);
static void nvdProcP2MaxTimeOut(void);
static void nvdProcP3MinTimeOut(void);
static void nvdProcP3MaxTimeOut(void);
static void nvdKlineRcvMsgCheck(void);
static void nvdKlineResDataSend(void);
static void nvdKlineResponseDataCache(void);
static void nvdKlineModeSet(void);
static void nvdKlineResMake(void);
static void nvdKlineResFlameData(void);
static void nvdKlineResDataMake(void);

/********************************************************************************************/
/*                           ROM Table Section                                              */
/********************************************************************************************/

static const EventExchange_ts nstEventExchangeTable_a[] = {
/* Extern Event                      Internal Event */
{  KLINEDRV_EV_REQUEST_DATA_RCV,     EV_KLINE_REQ_DATA_RCV   },
{  KLINEDRV_EV_P2MIN_TIMEOUT,        EV_KLINE_P2MIN_TIMEOUT  },
{  KLINEDRV_EV_P2MAX_TIMEOUT,        EV_KLINE_P2MAX_TIMEOUT  },
{  KLINEDRV_EV_P3MIN_TIMEOUT,        EV_KLINE_P3MIN_TIMEOUT  },
{  KLINEDRV_EV_P3MAX_TIMEOUT,        EV_KLINE_P3MAX_TIMEOUT  },
{  KLINEDRV_EV_INVALID,              EV_KLINE_MAX            }
};

static const Type_uByte nubKLineDrvMtrxTbl[][KLINE_MODE_MAX] = {
/*-----------------------------------------STOP---------------------------------------+--------------------- Wakeup detect --------------+---------------------------- Fast Initiarize ----------------------------------------+--------------------------------------- Diag Communication Mode-----------------------------------------------*/
/*------------------------------------------------------------------------------------+--------------------------------------------------+-------------------------------------------------------------------------------------+---------------------------------------------------------------------------------------------------------------
  ROW index                         KLINE_MODE_IDLE_WAIT       KLINE_MODE_IDLE         KLINE_MODE_INIT_LOW     KLINE_MODE_INIT_HI         KLINE_MODE_INIT_REQ         KLINE_MODE_INIT_RES_WAIT      KLINE_MODE_INIT_RES         KLINE_MODE_REQ_WAIT         KLINE_MODE_REQ_RCV          KLINE_MODE_RES_WAIT          KLINE_MODE_SEND_RES
--------------------------------------------------------------------------------------+--------------------------------------------------+------------------------------------------------------------------------------------ +---------------------------------------------------------------------------------------------------------------*/
/* EV_KLINE_REQ_DATA_RCV    */  {   PCODE_KLINE_INVALID,       PCODE_KLINE_INVALID,    PCODE_KLINE_INVALID,    PCODE_KLINE_INVALID,       PCODE_KLINE_REQ_DATA_RCV,   PCODE_KLINE_INVALID,          PCODE_KLINE_INVALID,        PCODE_KLINE_INVALID,        PCODE_KLINE_REQ_DATA_RCV,   PCODE_KLINE_INVALID,         PCODE_KLINE_INVALID        },
/* EV_KLINE_P2MIN_TIMEOUT   */  {   PCODE_KLINE_INVALID,       PCODE_KLINE_INVALID,    PCODE_KLINE_INVALID,    PCODE_KLINE_INVALID,       PCODE_KLINE_INVALID,        PCODE_KLINE_P2MIN_TIMEOUT,    PCODE_KLINE_INVALID,        PCODE_KLINE_INVALID,        PCODE_KLINE_INVALID,        PCODE_KLINE_P2MIN_TIMEOUT,   PCODE_KLINE_INVALID        },
/* EV_KLINE_P2MAX_TIMEOUT   */  {   PCODE_KLINE_INVALID,       PCODE_KLINE_INVALID,    PCODE_KLINE_INVALID,    PCODE_KLINE_INVALID,       PCODE_KLINE_INVALID,        PCODE_KLINE_P2MAX_TIMEOUT,    PCODE_KLINE_INVALID,        PCODE_KLINE_INVALID,        PCODE_KLINE_INVALID,        PCODE_KLINE_P2MAX_TIMEOUT,   PCODE_KLINE_INVALID        },
/* EV_KLINE_P3MIN_TIMEOUT   */  {   PCODE_KLINE_INVALID,       PCODE_KLINE_INVALID,    PCODE_KLINE_INVALID,    PCODE_KLINE_INVALID,       PCODE_KLINE_INVALID,        PCODE_KLINE_INVALID,          PCODE_KLINE_INVALID,        PCODE_KLINE_P3MIN_TIMEOUT,  PCODE_KLINE_INVALID,        PCODE_KLINE_INVALID,         PCODE_KLINE_INVALID        },
/* EV_KLINE_P3MAX_TIMEOUT   */  {   PCODE_KLINE_INVALID,       PCODE_KLINE_INVALID,    PCODE_KLINE_INVALID,    PCODE_KLINE_P3MAX_TIMEOUT, PCODE_KLINE_P3MAX_TIMEOUT,  PCODE_KLINE_P3MAX_TIMEOUT,    PCODE_KLINE_P3MAX_TIMEOUT,  PCODE_KLINE_P3MAX_TIMEOUT,  PCODE_KLINE_P3MAX_TIMEOUT,  PCODE_KLINE_P3MAX_TIMEOUT,   PCODE_KLINE_P3MAX_TIMEOUT  },
};
static void (*const nvdKLineDrvProcTbl[PCODE_KLINE_MAX])(void) = {
    nvdProcRequestDataRcv,      /* 0x00 : PCODE_KLINE_REQ_DATA_RCV */
    nvdProcP2MinTimeOut,        /* 0x01 : PCODE_KLINE_P2MIN_TIMEOUT */
    nvdProcP2MaxTimeOut,        /* 0x02 : PCODE_KLINE_P2MAX_TIMEOUT */
    nvdProcP3MinTimeOut,        /* 0x03 : PCODE_KLINE_P3MIN_TIMEOUT */
    nvdProcP3MaxTimeOut,        /* 0x04 : PCODE_KLINE_P3MAX_TIMEOUT */
};

/********************************************************************************************/
/*                           Global variable definition section                             */
/********************************************************************************************/
HAL_HANDLE nvdKlineDrvHalHandle = STD_NULL;

/********************************************************************************************/
/*                           Function Definition Section                                    */
/********************************************************************************************/

#if defined ( STACK_CHECK )
/********************************************************************************************/
/**	\function		nvdKlineProcessForStackCheck
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
static void nvdKlineProcessForStackCheck(void)
{
	nvdProcRequestDataRcv();
	nvdProcP2MinTimeOut();
	nvdProcP2MaxTimeOut();
	nvdProcP3MinTimeOut();
	nvdProcP3MaxTimeOut();  
}
#endif
/*********************************************************************************************
*  function                 : wvdKlineDrvControl
*  date                     : 2016.07.01
*  autho                    : zhangjinzhu
*  description              : klinedrv Control
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*  
*********************************************************************************************/
void wvdKlineDrvControl(void)
{
    /* Auto variable Define */
    const EventExchange_ts *astEvChg_p;
    Type_uByte aubInternalEv;
    Type_uByte aubPointCnt;
    Type_uByte aubProcID;

    /* Initalize */
    astEvChg_p = OS_NULL;
    aubPointCnt = KLINEDRV_NUM_0;
    aubProcID = PCODE_KLINE_INVALID;
    aubInternalEv = EV_KLINE_MAX;

    astEvChg_p = nstEventExchangeTable_a;
    
    if (OS_NULL != wstKlineDrvStdMail_p){
		
    	while((astEvChg_p[aubPointCnt].uhExternEvent != wstKlineDrvStdMail_p->uhEventCode) && (astEvChg_p[aubPointCnt].uhExternEvent != KLINEDRV_EV_INVALID)){
			aubPointCnt++;
    	}
        aubInternalEv = astEvChg_p[aubPointCnt].ubInternalEvent;

        if ((aubInternalEv < EV_KLINE_MAX) && (wubKlineMode < KLINE_MODE_MAX)){
			
            aubProcID = nubKLineDrvMtrxTbl[aubInternalEv][wubKlineMode];
        }
        else {
            /* No Action */
        }
		

        if (aubProcID < PCODE_KLINE_MAX){
			
            nvdKlineModeSet();
#ifndef STACK_CHECK
            nvdKLineDrvProcTbl[aubProcID]();
#else
	     nvdKlineProcessForStackCheck();
#endif
        }
        else {
            /* No Action */
        }

        nvdKlineResMake();
    }
    else {
		
		KLINEDRV("wstKlineDrvStdMail_p == OS_NULL");
        /* No Action */
    }
}
/*********************************************************************************************
*  function                 : nvdKlineModeSet
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
static void nvdKlineModeSet(void)
{
    if (KLINEDRV_ALLRES_TRUE == wubKlineResSts){
		
        wubKlineMode = KLINE_MODE_RES_WAIT;
    }
    else {
		
        wubKlineMode++;

        if (wubKlineMode > KLINE_MODE_SEND_RES){
            wubKlineMode = KLINE_MODE_REQ_WAIT;
        }
        else {
            /* No Action */
        }
    }
}
/*********************************************************************************************
*  function                 : nvdKlineResMake
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
static void nvdKlineResMake(void)
{

    if (KLINEDRV_EV_LET_RES == wstKlineDrvStdMail_p->uhEventCode){

        if (wubKlineAllResChk == KLINEDRV_TURE){

            nvdKlineResFlameData();
        }
        else {
            nvdKlineResponseDataCache();
        }
    }
    else {
        /* No Action */
    }
}
/*********************************************************************************************
*  function                 : nvdKlineResponseDataCache
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
static void nvdKlineResponseDataCache(void)
{
    if (OS_NULL != wstKlineDrvStdMail_p->vdQueueBuffer_p){
        if (wstKlineDrvStdMail_p->uwQueueSize <= KLINEDRV_LEN_MESSAGE - KLINEDRV_BYTE_SID){
            wubKlineDataResBuf[KLINEDRV_BYTE_FORMAT] = KLINEDRV_DATA_FMT + wstKlineDrvStdMail_p->uwQueueSize;
            wubKlineDataResBuf[KLINEDRV_BYTE_TARGET] = KLINEDRV_ADD_LET;
            wubKlineDataResBuf[KLINEDRV_BYTE_SOURCE] = KLINEDRV_ADD_HU;
            wvdGen_MemCpy(&wubKlineDataResBuf[KLINEDRV_BYTE_SID], wstKlineDrvStdMail_p->vdQueueBuffer_p, wstKlineDrvStdMail_p->uwQueueSize);
            wubKlineDataResLen = wstKlineDrvStdMail_p->uwQueueSize + KLINEDRV_BYTE_SID;
            wubKlineDataResBuf[(wstKlineDrvStdMail_p->uwQueueSize + KLINEDRV_BYTE_SID)] = wubKlineDrvMakeChkSum(&wubKlineDataResBuf[KLINEDRV_BYTE_FORMAT], wubKlineDataResLen);
            wubKlineDataResLen++;
            wubKlineResSts = KLINEDRV_TURE;
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
*  function                 : nvdKlineResFlameData
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
static void nvdKlineResFlameData(void)
{
    Type_uByte aubFlameNum;
    Type_uByte *aubFlameData_p;

    aubFlameNum = KLINEDRV_NUM_0;
    aubFlameData_p = OS_NULL;

    if (OS_NULL != wstKlineDrvStdMail_p->vdQueueBuffer_p){

        aubFlameData_p = wstKlineDrvStdMail_p->vdQueueBuffer_p;
        aubFlameNum = ((aubFlameData_p[KLINEDRV_NUM_2] >> KLINEDRV_NUM_4) & KLINEDRV_NUM_15) - KLINEDRV_NUM_1;

        if ((wstKlineDrvStdMail_p->uwQueueSize <= (KLINEDRV_LEN_MESSAGE - KLINEDRV_BYTE_SID)) && (aubFlameNum < KLINEDRV_NUM_4)){

            wubKlineFlameDataBuf[(aubFlameNum * KLINEDRV_LEN_MESSAGE) + KLINEDRV_BYTE_FORMAT] = KLINEDRV_DATA_FMT + wstKlineDrvStdMail_p->uwQueueSize;
            wubKlineFlameDataBuf[(aubFlameNum * KLINEDRV_LEN_MESSAGE) + KLINEDRV_BYTE_TARGET] = KLINEDRV_ADD_LET;
            wubKlineFlameDataBuf[(aubFlameNum * KLINEDRV_LEN_MESSAGE) + KLINEDRV_BYTE_SOURCE] = KLINEDRV_ADD_HU;

            wvdGen_MemCpy(&wubKlineFlameDataBuf[(aubFlameNum * KLINEDRV_LEN_MESSAGE) + KLINEDRV_BYTE_SID], wstKlineDrvStdMail_p->vdQueueBuffer_p, wstKlineDrvStdMail_p->uwQueueSize);

            wubKlineDataResLen = wstKlineDrvStdMail_p->uwQueueSize + KLINEDRV_BYTE_SID;

            wubKlineFlameDataBuf[(aubFlameNum * KLINEDRV_LEN_MESSAGE) + wubKlineDataResLen] = wubKlineDrvMakeChkSum(&wubKlineFlameDataBuf[(aubFlameNum * KLINEDRV_LEN_MESSAGE)], wubKlineDataResLen);

            wubKlineDataResLen++;

            wubKlineResSts = KLINEDRV_ALLRES_TRUE;
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
*  function                 : nvdProcRequestDataRcv
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
static void nvdProcRequestDataRcv(void)
{
    if (OS_NULL != wstKlineDrvStdMail_p->vdQueueBuffer_p){
        
        if (wstKlineDrvStdMail_p->uwQueueSize <= KLINEDRV_LEN_MESSAGE){
            wubKlineDataLen = wstKlineDrvStdMail_p->uwQueueSize;
            wvdGen_MemCpy(&wubKlineDataBuf[KLINEDRV_NUM_0], wstKlineDrvStdMail_p->vdQueueBuffer_p, wstKlineDrvStdMail_p->uwQueueSize);
            nvdKlineRcvMsgCheck();
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
*  function                 : nvdKlineRcvMsgCheck
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
static void nvdKlineRcvMsgCheck(void)
{
    Type_uByte aubChkSum;
    aubChkSum = KLINEDRV_NUM_0;
    if (((wubKlineDataBuf[KLINEDRV_BYTE_FORMAT] & KLINEDRV_FMT_MASK) == KLINEDRV_DATA_FMT) && 
        (wubKlineDataBuf[KLINEDRV_BYTE_TARGET] == KLINEDRV_ADD_HU) && 
        (wubKlineDataBuf[KLINEDRV_BYTE_SOURCE] == KLINEDRV_ADD_LET)){
        aubChkSum = wubKlineDrvMakeChkSum(&wubKlineDataBuf[KLINEDRV_NUM_0], wubKlineDataLen - KLINEDRV_NUM_1);
        if (aubChkSum == wubKlineDataBuf[wubKlineDataLen - KLINEDRV_NUM_1]){
			
            wvdKlineP3TimerSet(KLINE_TIMER_WAIT_FALSE);
            wvdKlineP2TimerSet(KLINE_TIMER_WAIT_TURE);
			
            wvdKlineDrvCmdChk();
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
*  function                 : nvdKlineResDataMake
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
static void nvdKlineResDataMake(void)
{
    if (KLINEDRV_TURE == wubKlineResSts){
        /* No Action */
    }
    else if (KLINEDRV_ALLRES_TRUE == wubKlineResSts){
        wvdGen_MemCpy(&wubKlineDataResBuf[KLINEDRV_BYTE_FORMAT], &wubKlineFlameDataBuf[KLINEDRV_BYTE_FORMAT], KLINEDRV_LEN_MESSAGE);
        wubKlineDataResLen = (wubKlineDataResBuf[KLINEDRV_BYTE_FORMAT] & KLINEDRV_FMT_LEN_MASK) + KLINEDRV_NUM_4;
        wvdGen_MemCpy(&wubKlineFlameDataBuf[KLINEDRV_BYTE_FORMAT], &wubKlineFlameDataBuf[KLINEDRV_LEN_MESSAGE], KLINEDRV_LEN_MESSAGE * KLINEDRV_NUM_3);
    }
    else {
        wvdKlineDrvNackResMake(wubKlineDataBuf[KLINEDRV_BYTE_SID], KLINEDRV_RES_BUSY);
    }
}
/*********************************************************************************************
*  function                 : nvdKlineResDataSend
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
static void nvdKlineResDataSend(void)
{
    if ((KLINEDRV_TURE == wubKlineResSts) && (KLINEDRV_LEN_MESSAGE >= wubKlineDataResLen)){
        wvdKlineP2TimerSet(KLINE_TIMER_WAIT_FALSE);
		HAL_Write(nvdKlineDrvHalHandle,&wubKlineDataResBuf[KLINEDRV_BYTE_FORMAT],wubKlineDataResLen);			
		wubKlineResSts = KLINEDRV_FLASE;
        wvdKlineP3TimerSet(KLINE_TIMER_WAIT_TURE);
    }
    else if ((KLINEDRV_ALLRES_TRUE == wubKlineResSts) && (KLINEDRV_LEN_MESSAGE >= wubKlineDataResLen)){
        wvdKlineP2TimerSet(KLINE_TIMER_WAIT_FALSE);
		HAL_Write(nvdKlineDrvHalHandle,&wubKlineDataResBuf[KLINEDRV_BYTE_FORMAT],wubKlineDataResLen);
		if (((wubKlineDataResBuf[KLINEDRV_NUM_5] >> KLINEDRV_NUM_4) & KLINEDRV_NUM_15) == KLINEDRV_NUM_4){ /* Flame4 Judge */
            wubKlineResSts = KLINEDRV_FLASE;
            wubKlineMode = KLINE_MODE_SEND_RES;
            wubKlineAllResChk = KLINEDRV_FLASE;
            wvdGen_MemSet(&wubKlineFlameDataBuf[KLINEDRV_NUM_0], KLINEDRV_NUM_0, sizeof(wubKlineFlameDataBuf));
            wvdKlineP3TimerSet(KLINE_TIMER_WAIT_TURE);
        }
        else {
            wvdKlineP2TimerSet(KLINE_TIMER_WAIT_TURE);
        }
    }
    else {
        /* No Action */
    }
}
/*********************************************************************************************
*  function                 : nvdProcP2MinTimeOut
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
static void nvdProcP2MinTimeOut(void)
{
    nvdKlineResDataMake();
    
    nvdKlineResDataSend();

    nvdKlineModeSet();
}
/*********************************************************************************************
*  function                 : nvdPrsocP2MaxTimeOut
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
static void nvdProcP2MaxTimeOut(void)
{
    wvdKlineDrvComInitial();
}
/*********************************************************************************************
*  function                 : nvdProcP3MinTimeOut
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
static void nvdProcP3MinTimeOut(void)
{
    wubKlineMode = KLINE_MODE_REQ_RCV;
}
/*********************************************************************************************
*  function                 : nvdProcP3MaxTimeOut
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
static void nvdProcP3MaxTimeOut(void)
{
    Type_uByte aubBuffer[KLINEDRV_NUM_4];
    Type_sByte asbResult;

	asbResult = RES_NG;
    aubBuffer[KLINEDRV_NUM_0] = KLINEDRV_DATA_FMT | KLINEDRV_NUM_1;
    aubBuffer[KLINEDRV_NUM_1] = KLINEDRV_ADD_HU;
    aubBuffer[KLINEDRV_NUM_2] = KLINEDRV_ADD_LET;
    aubBuffer[KLINEDRV_NUM_3] = KLINEDRV_DATA_FMT | KLINEDRV_NUM_2;

    wvdKlineDrvInitial();
    wvdKlineIdleWaitTimerSet(KLINE_IDLE_WAIT_300MS);

    asbResult = wsbRPCSendMsg(ARPC_ID_KLINEDRV,ARPC_ID_LET,LETAPP_EV_AUDIO_DIAG_END, &aubBuffer, sizeof(aubBuffer));
	if(asbResult != RES_OK){
		KLINEDRV("nvdProcP3MaxTimeOut NG");
	}
}

/* End of File */
