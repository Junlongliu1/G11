/**********************************************************************************************
*  file name                : klinedrv_os.c
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
#include "klinedrv_os.h"
#include "klinedrv_core.h"
#include "klinedrv_func.h"
#include "rpc_if.h"
#include "uart.h"
/* Other Components */
//#include "srMN103LF26R.h"
//#include "dserial.h"
#include "HAL_API.h"
#include "HAL_devices.h"
#include "HAL_KLINEDRV/HAL_KlineDrv.h"

//#include "HAL_KlineDrv.h"
/********************************************************************************************/
/*                           Macro Definition Section                                       */
/********************************************************************************************/
#define KLINEDRV_INFINITE_LOOP    (1U)
#define KLINEDRV_TIMER_P2_MIN     (25 / 2 + 3)
#define KLINEDRV_TIMER_P2_MAX     (50 / 2 + 1)
#define KLINEDRV_TIMER_P3_MIN     (55 / 2 + 2)
#define KLINEDRV_TIMER_P3_MAX     (5000 / 2 + 1)
#define KLINEDRV_TIMER_300MS      (300 / 2 + 1)
#define KLINEDRV_TIMER_P4_MAX     (10U)

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
/*                           ROM Table Section                                              */
/********************************************************************************************/

/* not used. */

/********************************************************************************************/
/*                           Static Variable Definition Section                             */
/********************************************************************************************/

static void nvdKlineIdleWaitExe(void);
static void nvdKlineP2TimeOutExe(void);
static void nvdKlineP3TimeOutExe(void);


/********************************************************************************************/
/*                           Static Function Declaration Section                            */
/********************************************************************************************/

void wvdKlineDrv_RpcCallback(const Type_uByte aubsrc, const Type_uHWord auhevent, void* avdQueueBuffer_p, const Type_uHWord aubQueueSize);

/********************************************************************************************/
/*                           Global variable definition section                             */
/********************************************************************************************/

/* For Main Task */
OS_Timeout_t wswKlineDrvTmout;       /* OS_ReceiveMail timeout */
OS_Timeout_t wswKlineDrvGuardTimer;   /* OS_ReceiveMail timeout */
OS_StdQueue_ts *wstKlineDrvStdMail_p;  /* Mailbox Object Pointer */
Type_uByte wubIdleWaitTimerAct;
Type_uByte wubP2TimerAct;
Type_uByte wubP3TimerAct;
Type_uByte wubP2TimerCount;
Type_uByte wubIdleWaitTimerCount;
Type_uHWord wuhP3TimerCount;

extern HAL_HANDLE nvdKlineDrvHalHandle;


/********************************************************************************************/
/*                           Function Definition Section                                    */
/********************************************************************************************/
/*********************************************************************************************
*  function                 : TSK_KLINEDRV
*  date                     : 2016.07.01
*  autho                    : zhangjinzhu
*  description              : klinedrv Task
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*  
*********************************************************************************************/
void TSK_KLINEDRV(void)
{	
    /* Auto variable Define */
    OS_StdQueue_ts astMainStdMail;  /* <AUTO> MessageInformation */
    OS_ErrCode_t aswErrCode; /* <AUTO> ErrorCode */

	/* Initalize */
    aswErrCode = OS_ERR_OK;
    wvdGen_MemSet(&astMainStdMail, KLINEDRV_NUM_0, sizeof(OS_StdQueue_ts));

    wswKlineDrvTmout = OS_TOUT_FOREVER;
    wstKlineDrvStdMail_p = &astMainStdMail;
	/* regiest rpc  */
	/* check rep init result, if NG retry 3  */
	
	(void)wsbRPCInit(ARPC_ID_KLINEDRV, (wvdRPCCallback)wvdKlineDrv_RpcCallback);

    while (KLINEDRV_INFINITE_LOOP){
        /* Receive DiagApp Message Information */
        aswErrCode = OS_ReceiveQueue(MBX_ID_KLINEDRV, wswKlineDrvTmout, &wstKlineDrvStdMail_p);

        /* Receive DiagApp Message OK */
        if (OS_ERR_OK == aswErrCode){
            if (wstKlineDrvStdMail_p != OS_NULL){				
                /* Execute Process */
                wvdKlineDrvControl();
                /* Mail Buffer Free Memory */
                if (OS_NULL != wstKlineDrvStdMail_p->vdQueueBuffer_p){
					
                    /* Free Memory */
                    (void)OS_ReleaseFMemoryBlock(wstKlineDrvStdMail_p->vdQueueBuffer_p);
                    wstKlineDrvStdMail_p->vdQueueBuffer_p = OS_NULL;
                }
                else {
                    /* No Action */
                }
            }
            else {
				
                ; /* No Action */
            }
        }
		else if (OS_ERR_TMOUT == aswErrCode){
			
            wswKlineDrvTmout = OS_TOUT_FOREVER;
        }
        else {
            /* No Action */
        }
    }
}
/*********************************************************************************************
*  function                 : TSK_KLINEDRV_INT
*  date                     : 2016.07.01
*  autho                    : zhangjinzhu
*  description              : klinedrv Task int
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*  
*********************************************************************************************/
void TSK_KLINEDRV_INT(void)
{
    OS_ErrCode_t aswFlagResult;
    OS_FlgPtn_t auwFlagPattern;

    aswFlagResult = OS_ERR_OK;
    auwFlagPattern = KLINEDRV_NUM_0;
    wswKlineDrvGuardTimer = OS_TOUT_FOREVER;

    while (KLINEDRV_INFINITE_LOOP){
        aswFlagResult = OS_WaitEventFlag(FLG_ID_KLINEDRV, KLINEDRV_FLGPTN, OS_FALSE, &auwFlagPattern, wswKlineDrvGuardTimer);
        if (OS_ERR_OK == aswFlagResult) {
		       if (KLINEDRV_FLGPTN_RCVDATA == (auwFlagPattern & KLINEDRV_FLGPTN)){
                wswKlineDrvGuardTimer = KLINEDRV_TIMER_P4_MAX;
                (void)OS_ClearEventFlag(FLG_ID_KLINEDRV, ~KLINEDRV_FLGPTN_RCVDATA);
            }
            else {
	            wswKlineDrvGuardTimer = OS_TOUT_FOREVER;
				if((KLINEDRV_ADD_HU == wubKlineDataRcvBuf[KLINEDRV_NUM_1])&&(KLINEDRV_ADD_LET == wubKlineDataRcvBuf[KLINEDRV_NUM_2])){
					(void)wubKlineDrv_SendMail(KLINEDRV_EV_REQUEST_DATA_RCV, &wubKlineDataRcvBuf[KLINEDRV_NUM_0], wubKlineDataRcvLen);
				}
				else{
					/* no Action */
				}			
	            wubKlineDataRcvLen = KLINEDRV_NUM_0;
       		}
            
        }
        else{		
            /* No Action */
        }
    }

}
/*********************************************************************************************
*  function                 : CYC_KLINEDRV
*  date                     : 2016.07.01
*  autho                    : zhangjinzhu
*  description              : klinedrv Cyc Task
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*  
*********************************************************************************************/
void CYC_KLINEDRV(void)
{
    switch (wubKlineMode){
        case KLINE_MODE_IDLE_WAIT:
            nvdKlineIdleWaitExe();
            break;
        case KLINE_MODE_IDLE:
        case KLINE_MODE_INIT_LOW:
        case KLINE_MODE_INIT_HI:
            wvdKlineWakeupDetection();
            break;
        default:
            nvdKlineP2TimeOutExe();
            nvdKlineP3TimeOutExe();
            break;
    }
}
/*********************************************************************************************
*  function                 : wubKlineDrv_Reset
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
Type_uByte wubKlineDrv_Reset(void)
{
    wvdKlineDrvInitial();
	return RES_OK;
}
/*********************************************************************************************
*  function                 : wubKlineDrv_ForceStandby
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
Type_uByte wubKlineDrv_ForceStandby(void)
{
    wvdKlineDrvInitial();
	if (OS_NULL != nvdKlineDrvHalHandle) {
        HAL_Close(nvdKlineDrvHalHandle);
    }
	return RES_OK;	
}
/*********************************************************************************************
*  function                 : wubKlineDrv_StandbyIn
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
Type_uByte wubKlineDrv_StandbyIn(void)
{
    wvdKlineDrvInitial();

    return RES_OK;
}
/*********************************************************************************************
*  function                 : wubKlineDrv_Close
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
void wubKlineDrv_Close(void)
{   
    if (OS_NULL != nvdKlineDrvHalHandle) {
        HAL_Close(nvdKlineDrvHalHandle);
    }	
}
/*********************************************************************************************
*  function                 : wubKlineDrv_StandbyOut
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
Type_uByte wubKlineDrv_StandbyOut(void)
{
    wvdKlineDrvInitial();
    wvdKlineIdleWaitTimerSet(KLINE_IDLE_WAIT_300MS);
	nvdKlineDrvHalHandle = HAL_Open(HAL_KLINEDRV,HAL_KLINEDRV_UART);
	HAL_SetCallbackFunction(nvdKlineDrvHalHandle,(pfCallback)wvdKlineDrvInt);
	
    return RES_OK;
}
/*********************************************************************************************
*  function                 : wvdKlineIdleWaitTimerSet
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
void wvdKlineIdleWaitTimerSet(Type_uByte aubWaitTimer)
{   
    wubIdleWaitTimerAct = aubWaitTimer;
    wubIdleWaitTimerCount = KLINEDRV_NUM_0;
}
/*********************************************************************************************
*  function                 : wvdKlineP2TimerSet
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
void wvdKlineP2TimerSet(Type_uByte aubTimerSet)
{   
    wubP2TimerAct = aubTimerSet;
    wubP2TimerCount = KLINEDRV_NUM_0;
}
/*********************************************************************************************
*  function                 : wvdKlineP3TimerSet
*  date                     : 2016.07.01
*  autho                    : zhangjinzhu
*  description              : klinedrv Cyc Task
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*  
*********************************************************************************************/
void wvdKlineP3TimerSet(Type_uByte aubTimerSet)
{
    wubP3TimerAct = aubTimerSet;
    wuhP3TimerCount = KLINEDRV_NUM_0;
}
/*********************************************************************************************
*  function                 : nvdKlineIdleWaitExe
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
static void nvdKlineIdleWaitExe(void)
{   
    if ((KLINE_RX_STATE == GPIO_HIGH) && (KLINE_IDLE_WAIT_300MS == wubIdleWaitTimerAct)){
		wubIdleWaitTimerCount++;
        if (wubIdleWaitTimerCount >= KLINEDRV_TIMER_300MS){
            wubKlineMode = KLINE_MODE_IDLE;
            wubIdleWaitTimerAct = KLINE_TIMER_WAIT_FALSE;
        }
        else {
            /* do nothing */
        }
    }
    else if ((KLINE_RX_STATE == GPIO_HIGH) && (KLINE_IDLE_WAIT_P3MIN == wubIdleWaitTimerAct)){
        wubIdleWaitTimerCount++;
        if (wubIdleWaitTimerCount >= KLINEDRV_TIMER_P3_MIN){
            wubKlineMode = KLINE_MODE_IDLE;
            wubIdleWaitTimerAct = KLINE_TIMER_WAIT_FALSE;
        }
        else {
            /* do nothing */
        }
    }
    else {
        wubIdleWaitTimerCount = KLINEDRV_NUM_0;
    }
}
/*********************************************************************************************
*  function                 : nvdKlineP2TimeOutExe
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
static void nvdKlineP2TimeOutExe(void)
{   
    if (KLINE_TIMER_WAIT_TURE == wubP2TimerAct){
        wubP2TimerCount++;
        if (KLINEDRV_TIMER_P2_MIN == wubP2TimerCount){
            (void)wubKlineDrv_SendMail(KLINEDRV_EV_P2MIN_TIMEOUT, OS_NULL, KLINEDRV_NUM_0);
        }
        else if (KLINEDRV_TIMER_P2_MAX == wubP2TimerCount){
			
            (void)wubKlineDrv_SendMail(KLINEDRV_EV_P2MAX_TIMEOUT, OS_NULL, KLINEDRV_NUM_0);
        }
        else {
            /* do nothing */
        }
    }
    else {
        /* do nothing */
    }
}
/*********************************************************************************************
*  function                 : nvdKlineP3TimeOutExe
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
static void nvdKlineP3TimeOutExe(void)
{   
    if (KLINE_TIMER_WAIT_TURE == wubP3TimerAct){
        wuhP3TimerCount++;
        if (KLINEDRV_TIMER_P3_MIN == wuhP3TimerCount){
            (void)wubKlineDrv_SendMail(KLINEDRV_EV_P3MIN_TIMEOUT, OS_NULL, KLINEDRV_NUM_0);
        }
        else if (KLINEDRV_TIMER_P3_MAX == wuhP3TimerCount){
			
            (void)wubKlineDrv_SendMail(KLINEDRV_EV_P3MAX_TIMEOUT, OS_NULL, KLINEDRV_NUM_0);
        }
        else {
            /* do nothing */
        }
    }
    else {
        /* do nothing */
    }
}

/********************************************************************************************/
/**	\function		wvdKlineDrv_RpcCallback
 *	\date			2021/12/01
 *	\author			WangRong
 *	\description	Receive rpc message(callback function of wswRPCInit)
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		auhEventCode
 *					aubQueueBoxID
 *					aubQueuePriority
 *					avdQueueBuffer_p
 *					aubQueueSize
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			RES_OK
 *					RES_NG
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/10/25		Guanjiafu		New
 ********************************************************************************************/
void wvdKlineDrv_RpcCallback(const Type_uByte aubsrc, const Type_uHWord auhevent, void* avdQueueBuffer_p, const Type_uHWord aubQueueSize)
{
	Type_uByte		aubReturnCode;
	OS_ErrCode_t	aswErrCode;
	OS_StdQueue_ts	astStdQueue;
	OS_VoidPtr_t	avdMpf_p;

	/* Variable Initialize */
	aubReturnCode	= RES_OK;
	aswErrCode		= OS_ERR_OK;
	avdMpf_p		= STD_NULL;
	OSAL_MemSet(&astStdQueue, KLINE_VALUE_0, sizeof(astStdQueue));

	astStdQueue.swSrcTaskId = aubsrc;
	astStdQueue.uhEventCode = auhevent;
	if ((avdQueueBuffer_p != STD_NULL) && (aubQueueSize != KLINE_VALUE_0)) {
		aswErrCode = OS_GetFMemoryBlock(&avdMpf_p, aubQueueSize);
		/* Get Memory Block Result Check */
		if (OS_ERR_OK == aswErrCode) {
			wvdGen_MemCpy(avdMpf_p, avdQueueBuffer_p, aubQueueSize);
			astStdQueue.vdQueueBuffer_p = avdMpf_p;
			astStdQueue.uwQueueSize = aubQueueSize;
		} else {
			/* Can`t get memory, Ignore the message */
			aubReturnCode = RES_NG;
		}
	} else {
		astStdQueue.vdQueueBuffer_p = STD_NULL;
		astStdQueue.uwQueueSize = KLINE_VALUE_0;
	}

	/* send msg to SENSOR task */
	if (aubReturnCode == RES_OK) {
		aswErrCode = OS_SendQueue(MBX_ID_KLINEDRV, OS_MAIL_PRI_NORMAL, &astStdQueue);
		/* Send msg Result Check */
		if (OS_ERR_OK == aswErrCode) {
			/* No Action */
			aubReturnCode = RES_OK;
		} else {
			/* Get Memory Block Check */
			if (avdMpf_p != STD_NULL) {
				(void)OS_ReleaseFMemoryBlock(avdMpf_p);
			} else {
				/* No Action */
				;
			}
			aubReturnCode = RES_NG;
		}
	}
	return;
}

/* End of File */

