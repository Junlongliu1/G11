/********************************************************************************************/
/* File Name	: linetoolapp_common.c														*/
/* Date 		: 2021.11.12																*/
/* Author		: liujie																*/
/* Model		: EDA 																*/
/* Copyright (c)  2021 Alpine Electronics (China) Co. LTD.	All Rights Reserved 			*/
/*------------------------------------------------------------------------------------------*/
/* Revision History 																		*/
/* No		Date		Revised by		Function Name										*/
/* 0001 	2021.11.12 	liujie 	        Create												*/
/*------------------------------------------------------------------------------------------*/
/********************************************************************************************/

/********************************************************************************************/
/*                             Include File Section                                         */
/********************************************************************************************/

/* System Header Files */
#include "os_framework.h"
// #include "log_Ext.h"

/* Linetool Header Files */
#include "linetoolapp.h"
#include "linetoolapp_common.h"

/* Other Components */
#include "rpc_if.h"
#include "logapp.h"	//add by cxx on 20220421 for LOG
/********************************************************************************************/
/*                           Macro Definition Section                                       */
/********************************************************************************************/

#define LINETOOL_RPC_SEND_RETRY                 (3)

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

/* not used. */

/********************************************************************************************/
/*                           Static Function Declaration Section                            */
/********************************************************************************************/

/* not used. */

/********************************************************************************************/
/*                           Global variable definition section                             */
/********************************************************************************************/
LineToolVar_ts wstLineToolDrvVar;

/********************************************************************************************/
/*                           Function Definition Section                                    */
/********************************************************************************************/

/*********************************************************************************************
*  function                 : wubLineToolAppSendQueue
*  date                     : 2021.11.12
*  autho                    : liujie
*  description              : Send Queue To LineToolApp
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*
*********************************************************************************************/
Type_uByte wubLineToolAppSendQueue(const Type_uHWord auhEventCode, const void *avdBuffer_p, const Type_uWord auwSize)
{
    OS_ErrCode_t    aswErrCode;     /* <AUTO> Error Code */
    OS_ObjId_t      aswTaskId;          /* <AUTO> Task ID */
    OS_VoidPtr_t    avdQueueBuff_p;   /* <AUTO> Memory Address */
    OS_StdQueue_ts   astStdQueue;     /* <AUTO> Queue */
    Type_uByte      aubReturn;          /* <AUTO> Return Code */

    /* Initalize */
    aswErrCode          = OS_ERR_NG;
    aswTaskId           = OS_NULL;
    avdQueueBuff_p       = OS_NULL;
    aubReturn           = RES_NG;
    wvdGen_MemSet(&astStdQueue, LINETOOL_NUM_0, sizeof(OS_StdQueue_ts));

    if ((STD_NULL != avdBuffer_p) && (LINETOOL_NUM_0 < auwSize)) {

        (void)OS_GetFMemoryBlock( &avdQueueBuff_p, auwSize);

        if (OS_NULL != avdQueueBuff_p){
            wvdGen_MemCpy(avdQueueBuff_p, avdBuffer_p, auwSize);
            astStdQueue.swSrcTaskId   = aswTaskId;
            astStdQueue.uhEventCode   = auhEventCode;
            astStdQueue.vdQueueBuffer_p = avdQueueBuff_p;
            astStdQueue.uwQueueSize    = auwSize;

            aswErrCode = OS_SendQueue(MBX_ID_LINETOOL, OS_MAIL_PRI_NORMAL, &astStdQueue);

            if (OS_ERR_OK == aswErrCode) {
                aubReturn = RES_OK;
            } else {
                /* Send Queue Error */

                if (OS_NULL != avdQueueBuff_p) {
                    (void)OS_ReleaseFMemoryBlock( avdQueueBuff_p);
                    avdQueueBuff_p = OS_NULL;
                } else {
                    /* No Action */
                }
                /* Debug Message */
                LINETOOLAPP_D("wubLineToolAppSendQueue-ERROR");
            }
        } else {
            /* Memory Alloc Error */
            LINETOOLAPP_D("wubLineToolAppSendQueue-ERROR");
        }
    } else { /* Message Without QueueBuffer */
            astStdQueue.swSrcTaskId   = aswTaskId;
            astStdQueue.uhEventCode   = auhEventCode;
            astStdQueue.vdQueueBuffer_p = OS_NULL;
            astStdQueue.uwQueueSize    = LINETOOL_NUM_0;

            aswErrCode = OS_SendQueue(MBX_ID_LINETOOL, OS_MAIL_PRI_NORMAL, &astStdQueue);

            if (OS_ERR_OK == aswErrCode){
                aubReturn = RES_OK;
            } else {
                LINETOOLAPP_D("wubLineToolAppSendQueue-ERROR");
            }
    }
    return aubReturn;
}

Type_uByte wubLineToolApp_RPCSend(const Type_uByte aubDest, const Type_uHWord auhEvent, void* avdData_p, const Type_uHWord auhLength) {
    LOG_OUTPUT(LOG_APP_ID_LINE, DEBUGLOG_LEVEL_INFO, ATAT_LEVEL_OFF, "\n Rpc snd aubdest = %x auhEvent = %x auhlength = %d", aubDest, auhEvent, auhLength);    //add by cxx on 20220421 for LOG
    // printf("Rpc snd aubdest = %x auhEvent = %x auhlength = %d", aubDest, auhEvent, auhLength);       //delete by cxx on 20220421 for LOG
    Type_uByte aubCount;
    Type_uByte aubReturn;
    Type_sByte asbResult;

    aubCount  = LINETOOL_NUM_0;
    aubReturn = RES_NG;
    asbResult = RPC_NG;
#ifndef LINETOOLAPP_DEBUG    //add by cxx on 20220421 for LOG
    // TODO: for test   
    printf(" data = [ ");
    Type_uByte * test = avdData_p;
    for (int i = 0; i < auhLength; i++) {
        printf("%02x ", test[i]);
    }
    printf("] \n");
    // TODO: for test end
#endif
    for (aubCount = LINETOOL_NUM_0; aubCount < LINETOOL_RPC_SEND_RETRY; aubCount++) {
        asbResult = wsbRPCSendMsg(ARPC_ID_LINETOOLAPP,aubDest, auhEvent, avdData_p, auhLength);
        if (RPC_OK == asbResult) {
            aubReturn = RES_OK;
            break;
        } else if (RPC_BUSY == asbResult) {
            OS_SleepTask(1);
            /* No Action continue retry */
        } else {
            aubReturn = RES_NG;
            break;
        }
    }
    return aubReturn;
}

/* End of File */


