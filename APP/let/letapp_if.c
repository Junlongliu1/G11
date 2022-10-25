/**********************************************************************************************
*  file name                : linetoolapp_if.c
*  date                     : 2016.06.01
*  autho                    : zhangjinzhu
*  model                    : TBTA
*  description              : linetool interface related file
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

/* Linetool Header Files */
#include "letapp.h"
#include "letapp_common.h"

/* Other Components */
//#include "dserial.h"
#include "rpc_module.h"
#include "rpc_event.h"
#include "rpc_if.h"

/********************************************************************************************/
/*                           Macro Definition Section                                       */
/********************************************************************************************/

/* not used. */

/********************************************************************************************/
/*                           Enumeration Type Definition Section                            */
/********************************************************************************************/

enum LET_TASK_NO{
    LET_TASK_INVALID,
    LET_TASK_MAIN,
    LET_TASK_SUBA,
    LET_TASK_SUBB
};


/********************************************************************************************/
/*                           User type definition section                                   */
/********************************************************************************************/

/* not used. */

/********************************************************************************************/
/*                           Structure/Union Type Definition Section                        */
/********************************************************************************************/

typedef struct {
    Type_uHWord uhRpcEvent;
    Type_uHWord uhLetEvent;
    Type_uByte ubTaskNo;
}LetResEvent_ts;

/********************************************************************************************/
/*                           ROM Table Section                                              */
/********************************************************************************************/

static const LetResEvent_ts nstLetRpcEvent_a[] = {
    {  EV_INF_LET_AUDIO_DIAG_START_RES,    LET_SUBA_EV_LET_START_RES,      LET_TASK_SUBA     },
    {  EV_INF_LET_AUDIO_DIAG_END_RES,      LET_SUBA_EV_LET_END_RES,        LET_TASK_SUBA     },
    {  EV_LET_USB_SPK_SRC_ON_RES,      LET_SUBA_EV_USB_SRC_ON_RES,     LET_TASK_SUBA     },
    {  EV_LET_USB_SPK_SRC_OFF_RES,     LET_SUBA_EV_USB_SRC_OFF_RES,    LET_TASK_SUBA     },
    {  EV_LET_BEEP_SPK_SRC_ON_RES,     LET_SUBA_EV_BEEP_SRC_ON_RES,    LET_TASK_SUBA     },
    {  EV_LET_BEEP_SPK_SRC_OFF_RES,    LET_SUBA_EV_BEEP_SRC_OFF_RES,   LET_TASK_SUBA     },
    {  EV_LET_SPK_DISPLAY_RES,         LET_SUBA_EV_SPK_DISP_RES,       LET_TASK_SUBA     },
    {  EV_LET_USB_PLAY_RES,            LET_SUBA_EV_USB_PLAY_RES,       LET_TASK_SUBA     },
    {  EV_LET_USB_PLAY_STOP_RES,       LET_SUBA_EV_USB_PLAYSTOP_RES,   LET_TASK_SUBA     },
    {  EV_RES_LET_BSM_INITIAL,         LET_SUBA_EV_BSM_INIT_RES,       LET_TASK_SUBA     },
    {  EV_RES_LET_BSM_START_TP_SET,    LET_SUBA_EV_BSM_START_RES,      LET_TASK_SUBA     },
    {  EV_RES_LET_BSM_END,             LET_SUBA_EV_BSM_END_RES,        LET_TASK_SUBA     },
    {  EV_RES_LET_BSM_RESULT,          LET_SUBA_EV_BSM_RESULT_RES,     LET_TASK_SUBA     },
    {  EV_LET_BUTTON_INF,              LETAPP_EV_BUTTON_INFO,          LET_TASK_MAIN     },
    {  EV_LET_USB_DISCONNECT,          LET_SUBA_EV_USB_DISCONNECT,     LET_TASK_SUBA     },
    {  EV_LET_MAX,                     LETAPP_EV_INVALID,              LET_TASK_INVALID  }
};

/********************************************************************************************/
/*                           Static Variable Definition Section                             */
/********************************************************************************************/

/* not used. */

/********************************************************************************************/
/*                           Static Function Declaration Section                            */
/********************************************************************************************/

/* not used. */

/********************************************************************************************/
/*                           Global variable definition section                             */
/********************************************************************************************/


/********************************************************************************************/
/*                           Function Definition Section                                    */
/********************************************************************************************/
/*********************************************************************************************
*  function                 : wubLetApp_Reset
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
Type_uByte wubLetApp_Reset(void)
{
    /* Let Initial */
    wvdLetAppInitial();
	return RES_OK;
}
/*********************************************************************************************
*  function                 : wubLetApp_ForceStandby
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
Type_uByte wubLetApp_ForceStandby(void)
{
    /* Let Mode Status Init */
    wvdLetAppComInit();
	return RES_OK;
}
/*********************************************************************************************
*  function                 : wubLetApp_StandbyIn
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
Type_uByte wubLetApp_StandbyIn(void)
{
    /* Let Mode Status Init */
    wvdLetAppComInit();
    /* Let Working Time Save Eeprom */
    wvdLetAppSysWorkTime(LETAPP_NOMAL_NG);
    return RES_OK;
}
/*********************************************************************************************
*  function                 : wubLetApp_StandbyOut
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
Type_uByte wubLetApp_StandbyOut(void)
{
    /* Let Mode Status Init */
    wvdLetAppComInit();
    /* Let Working Time Caculate Start */
    wvdLetAppSysWorkTime(LETAPP_NOMAL_OK);
    return RES_OK;
}

/*********************************************************************************************
*  function                 : wubLetApp_RpcSendMail
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
void wubLetApp_RpcSendMail(Type_uByte aubModleId, Type_uHWord auhEventCode, void* avdBuffer_p, Type_uWord auwSize)
{
    Type_uHWord auhLetEvent;
    Type_uHWord auhChkNum;
	Type_sByte 	asbResult;

	asbResult = RES_NG;
    auhLetEvent = EV_LET_INVALID;
    auhChkNum = LETAPP_NUM_0;
	
    while ((auhChkNum < (sizeof(nstLetRpcEvent_a) / sizeof(LetResEvent_ts))) && (nstLetRpcEvent_a[auhChkNum].uhRpcEvent != auhEventCode)){
        auhChkNum++;
    }

    if (auhChkNum < (sizeof(nstLetRpcEvent_a) / sizeof(LetResEvent_ts))){
        
        auhLetEvent = nstLetRpcEvent_a[auhChkNum].uhLetEvent;

        if (LET_TASK_MAIN == nstLetRpcEvent_a[auhChkNum].ubTaskNo){
			asbResult = wsbRPCSendMsg(aubModleId,ARPC_ID_LET,auhLetEvent, avdBuffer_p, auwSize);
        }
        else if (LET_TASK_SUBA == nstLetRpcEvent_a[auhChkNum].ubTaskNo){
			asbResult = wsbRPCSendMsg(aubModleId,ARPC_ID_LET_SUBA,auhLetEvent, avdBuffer_p, auwSize);
        }
        else if (LET_TASK_SUBB == nstLetRpcEvent_a[auhChkNum].ubTaskNo){
		    asbResult = wsbRPCSendMsg(aubModleId,LET_TASK_SUBB,auhLetEvent, avdBuffer_p, auwSize);
        }
        else {
			//asbResult = wsbRPCSendMsg(aubModleId,ARPC_ID_LET,auhLetEvent, avdBuffer_p, auwSize);
            /* No Action */
        }
    }
    else {
        /* No Action */
    }
	if(asbResult != RES_OK){
			LETAPP("wubLetApp_RpcSendMail NG");
	}

    /* Debug Message */

}


/********************************************************************************************/
/**	\function		wvdLetApp_RpcCallback
 *	\date			2021/11/30
 *	\author			WangRong
 *	\description	Receive rpc message(callback function of wsbRPCInit)
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
void wvdLetApp_RpcCallback(const Type_uByte aubsrc, const Type_uHWord auhevent, void* avdQueueBuffer_p, const Type_uHWord aubQueueSize)
{
	Type_uByte		aubReturnCode;
	OS_ErrCode_t	aswErrCode;
	OS_StdQueue_ts	astStdQueue;
	OS_VoidPtr_t	avdMpf_p;

	/* Variable Initialize */
	aubReturnCode	= RES_OK;
	aswErrCode		= OS_ERR_OK;
	avdMpf_p		= STD_NULL;
	wvdGen_MemSet(&astStdQueue, LET_VALUE_0, sizeof(astStdQueue));

	astStdQueue.swSrcTaskId = aubsrc;
	astStdQueue.uhEventCode = auhevent;
	if ((avdQueueBuffer_p != STD_NULL) && (aubQueueSize != LET_VALUE_0)) {
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
		astStdQueue.uwQueueSize = LET_VALUE_0;
	}

	/* send msg to letapp task */
	if (aubReturnCode == RES_OK) {
		
		aswErrCode = OS_SendQueue(MBX_ID_LETAPP, OS_MAIL_PRI_NORMAL, &astStdQueue);
		
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
/********************************************************************************************/
/**	\function		wvdLetAppSubA_RpcCallback
 *	\date			2021/11/30
 *	\author			WangRong
 *	\description	Receive rpc message(callback function of wsbRPCInit)
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
void wvdLetAppSubA_RpcCallback(const Type_uByte aubsrc, const Type_uHWord auhevent, void* avdQueueBuffer_p, const Type_uHWord aubQueueSize)
{
	Type_uByte		aubReturnCode;
	OS_ErrCode_t	aswErrCode;
	OS_StdQueue_ts	astStdQueue;
	OS_VoidPtr_t	avdMpf_p;

	/* Variable Initialize */
	aubReturnCode	= RES_OK;
	aswErrCode		= OS_ERR_OK;
	avdMpf_p		= STD_NULL;
	wvdGen_MemSet(&astStdQueue, LET_VALUE_0, sizeof(astStdQueue));

	astStdQueue.swSrcTaskId = aubsrc;
	astStdQueue.uhEventCode = auhevent;
	
	switch (aubsrc){
		case ARPC_ID_HMI:		
			if(auhevent == EV_INF_LET_AUDIO_DIAG_START_RES){
				astStdQueue.uhEventCode = LET_SUBA_EV_LET_START_RES;				
			}     
			else if(auhevent == EV_INF_LET_AUDIO_DIAG_END_RES){
				astStdQueue.uhEventCode = LET_SUBA_EV_LET_END_RES;	
			}
		break;
		case ARPC_ID_LANEWATCH:		
			if(auhevent == EV_RES_LET_BSM_INITIAL){
				astStdQueue.uhEventCode = LET_SUBA_EV_BSM_INIT_RES;
			}else if(auhevent == EV_RES_LET_BSM_START_TP_SET){
				astStdQueue.uhEventCode = LET_SUBA_EV_BSM_START_RES;
			}else if(auhevent == EV_RES_LET_BSM_END){
				astStdQueue.uhEventCode = LET_SUBA_EV_BSM_END_RES;
			}else if(auhevent == EV_RES_LET_BSM_RESULT){
				astStdQueue.uhEventCode = LET_SUBA_EV_BSM_RESULT_RES;
			}
		break;
		default:
		break;
	}	
	
	if ((avdQueueBuffer_p != STD_NULL) && (aubQueueSize != LET_VALUE_0)) {
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
		astStdQueue.uwQueueSize = LET_VALUE_0;
	}

	/* send msg to letapp task */
	if (aubReturnCode == RES_OK) {
		aswErrCode = OS_SendQueue(MBX_ID_LETAPP_SUBA, OS_MAIL_PRI_NORMAL, &astStdQueue);
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
/********************************************************************************************/
/**	\function		wvdLetAppSubB_RpcCallback
 *	\date			2021/11/30
 *	\author			WangRong
 *	\description	Receive rpc message(callback function of wsbRPCInit)
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
void wvdLetAppSubB_RpcCallback(const Type_uByte aubsrc, const Type_uHWord auhevent, void* avdQueueBuffer_p, const Type_uHWord aubQueueSize)
{
	Type_uByte		aubReturnCode;
	OS_ErrCode_t	aswErrCode;
	OS_StdQueue_ts	astStdQueue;
	OS_VoidPtr_t	avdMpf_p;

	/* Variable Initialize */
	aubReturnCode	= RES_OK;
	aswErrCode		= OS_ERR_OK;
	avdMpf_p		= STD_NULL;
	wvdGen_MemSet(&astStdQueue, LET_VALUE_0, sizeof(astStdQueue));
	
	astStdQueue.swSrcTaskId = aubsrc;
	astStdQueue.uhEventCode = auhevent;
	if ((avdQueueBuffer_p != STD_NULL) && (aubQueueSize != LET_VALUE_0)) {
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
		astStdQueue.uwQueueSize = LET_VALUE_0;
	}

	/* send msg to letapp task */
	if (aubReturnCode == RES_OK) {
		aswErrCode = OS_SendQueue(MBX_ID_LETAPP_SUBB, OS_MAIL_PRI_NORMAL, &astStdQueue);
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
