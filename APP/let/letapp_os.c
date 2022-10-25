/**********************************************************************************************
*  file name                : letapp_os.c
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
#include "letapp_suba_core.h"
#include "letapp_subb_core.h"
/* Other Components */
#include "model.h"
/* rpc header files */
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

/* For Main Task */
OS_StdQueue_ts   *wstLetAppStdMail_p;   /* Mailbox Object Pointer */
OS_StdQueue_ts   *wstLetAppSubAMail_p;  /* Mailbox SObject Pointer */
OS_StdQueue_ts   *wstLetAppSubBMail_p;  /* Mailbox Object Pointer */

OS_Timeout_t wswLetAppSubATmout;
OS_Timeout_t wswLetAppSubBTmout;

Type_uWord wswLetAppSubAPassTime;
Type_uWord wswLetAppSubBPassTime;
Type_uWord wuwLetAppSubANowTime;
Type_uWord wuwLetAppSubBNowTime;
Type_uWord wuwLetSubAStartTime;
Type_uWord wuwLetSubBStartTime;
/********************************************************************************************/
/*                           Function Definition Section                                    */
/********************************************************************************************/
/*********************************************************************************************
*  function                 : TSK_LETAPP
*  date                     : 2016.05.01
*  autho                    : zhangjinzhu
*  description              : LetApp Task
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*  
*********************************************************************************************/
void TSK_LETAPP(void)
{
    /* Auto variable Define */
    OS_StdQueue_ts astMainStdMail;  /* <AUTO> MessageInformation */
    OS_ErrCode_t aswErrCode; /* <AUTO> ErrorCode */
	/* Initalize */
    aswErrCode = OS_ERR_OK;
    wvdGen_MemSet(&astMainStdMail, LETAPP_NUM_0, sizeof(OS_StdQueue_ts));
    wstLetAppStdMail_p = &astMainStdMail;
		
	/* regiest rpc  */
	(void)wsbRPCInit(ARPC_ID_LET, (wvdRPCCallback)wvdLetApp_RpcCallback);

    while (LETAPP_NUM_1){
        /* Receive DiagApp Message Information */
        aswErrCode = OS_ReceiveQueue(MBX_ID_LETAPP, OS_TOUT_FOREVER, &wstLetAppStdMail_p);
        /* Receive DiagApp Message OK */
        if (OS_ERR_OK == aswErrCode){
            if (wstLetAppStdMail_p != OS_NULL){
				/* Execute Process */
                wvdLetAppControl();
                /* Mail Buffer Free Memory */
                if (OS_NULL != wstLetAppStdMail_p->vdQueueBuffer_p){
                    /* Free Memory */
                    (void)OS_ReleaseFMemoryBlock(wstLetAppStdMail_p->vdQueueBuffer_p);
                    wstLetAppStdMail_p->vdQueueBuffer_p = OS_NULL;
                }
                else {
                    /* No Action */
                }
            }
            else {
				LETAPP("OS_ERR_NG");
                /* No Action */
            }
        }
        else {
            /* No Action */
        }
    }
}
/*********************************************************************************************
*  function                 : TSK_LETAPP_SUBA
*  date                     : 2016.05.01
*  autho                    : zhangjinzhu
*  description              : LetApp Task
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*  
*********************************************************************************************/
void TSK_LETAPP_SUBA(void)
{
    /* Auto variable Define */
    OS_StdQueue_ts astSubAStdMail;  /* <AUTO> MessageInformation */
    OS_ErrCode_t aswErrCode; /* <AUTO> ErrorCode */

	/* Initalize */
    aswErrCode = OS_ERR_OK;
    wvdGen_MemSet(&astSubAStdMail, LETAPP_NUM_0, sizeof(OS_StdQueue_ts));

    wswLetAppSubATmout = OS_TOUT_FOREVER;
    wstLetAppSubAMail_p = &astSubAStdMail;
    /* Setup Main Process Engine */
    wvdLetAppSubA_SetupProEng();
	/* regiest rpc  */
	/* check rep init result, if NG retry 3  */
	(void)wsbRPCInit(ARPC_ID_LET_SUBA, (wvdRPCCallback)wvdLetAppSubA_RpcCallback);

    while (LETAPP_NUM_1){
        /* Receive DiagApp Message Information */
        aswErrCode = OS_ReceiveQueue(MBX_ID_LETAPP_SUBA, wswLetAppSubATmout, &wstLetAppSubAMail_p);

        /* Receive DiagApp Message OK */
        if (OS_ERR_OK == aswErrCode){
            if (wstLetAppSubAMail_p != OS_NULL){
                /* Execute Process */
                wvdLetAppSubAControl();
                /* Mail Buffer Free Memory */
                if (OS_NULL != wstLetAppSubAMail_p->vdQueueBuffer_p){
                    /* Free Memory */
                    (void)OS_ReleaseFMemoryBlock(wstLetAppSubAMail_p->vdQueueBuffer_p);
                    wstLetAppSubAMail_p->vdQueueBuffer_p = OS_NULL;
                }
                else {
                    /* No Action */
                }
            }
            else {
                /* No Action */
            }
        }
		else if ((OS_ErrCode_t)OS_ERR_TMOUT == aswErrCode){
            /* Receive DiagApp Message Timeout */
            astSubAStdMail.uhEventCode = LET_SUBA_EV_TIMEOUT;
            /* Execute Process */
            wvdLetAppSubAControl();
        }
        else {
            /* No Action */
        }
    }
}
/*********************************************************************************************
*  function                 : TSK_LETAPP_SUBB
*  date                     : 2016.05.01
*  autho                    : zhangjinzhu
*  description              : LetApp Task
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*  
*********************************************************************************************/
void TSK_LETAPP_SUBB(void)
{
    /* Auto variable Define */
    OS_StdQueue_ts astSubBStdMail;  /* <AUTO> MessageInformation */
    OS_ErrCode_t aswErrCode; /* <AUTO> ErrorCode */

	/* Initalize */
    aswErrCode = OS_ERR_OK;
    wvdGen_MemSet(&astSubBStdMail, LETAPP_NUM_0, sizeof(OS_StdQueue_ts));

    wswLetAppSubBTmout = OS_TOUT_FOREVER;
    wstLetAppSubBMail_p = &astSubBStdMail;
    /* Setup Main Process Engine */
    wvdLetAppSubB_SetupProEng();
	/* regiest rpc  */
	/* check rep init result, if NG retry 3  */
	(void)wsbRPCInit(ARPC_ID_LET_SUBB, (wvdRPCCallback)wvdLetAppSubB_RpcCallback);

    while (LETAPP_NUM_1){
        /* Receive DiagApp Message Information */
        aswErrCode = OS_ReceiveQueue(MBX_ID_LETAPP_SUBB, wswLetAppSubBTmout, &wstLetAppSubBMail_p);

        /* Receive DiagApp Message OK */
        if (OS_ERR_OK == aswErrCode){
            if (wstLetAppSubBMail_p != OS_NULL){
                /* Execute Process */
                wvdLetAppSubBControl();
                /* Mail Buffer Free Memory */
                if (OS_NULL != wstLetAppSubBMail_p->vdQueueBuffer_p){
                    /* Free Memory */
                    (void)OS_ReleaseFMemoryBlock(wstLetAppSubBMail_p->vdQueueBuffer_p);
                    wstLetAppSubBMail_p->vdQueueBuffer_p = OS_NULL;
                }
                else {
                    /* No Action */
                }
            }
            else {
                ; /* No Action */
            }
        }
		else if ((OS_ErrCode_t)OS_ERR_TMOUT == aswErrCode){
            /* Receive DiagApp Message Timeout */
            astSubBStdMail.uhEventCode = LET_SUBB_EV_TIMEOUT;
            /* Execute Process */
            wvdLetAppSubBControl();
        }
        else {
            /* No Action */
        }
    }
}
/*********************************************************************************************
*  function                 : TSK_LETAPP
*  date                     : 2016.05.01
*  autho                    : zhangjinzhu
*  description              : LetApp Task
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*  
*********************************************************************************************/
Type_sWord wswLetApplGetProcTime(Type_uWord auwStartTime, Type_uWord auwEndTime)
{
    Type_sWord aswTime;
    Type_uWord auwProcTime;

    aswTime = LETAPP_NUM_0;
    auwProcTime = LETAPP_NUM_0;

    if (auwEndTime > auwStartTime){
        auwProcTime = (auwEndTime - auwStartTime);
    }
    else {
        auwProcTime = (LETAPP_TICK_TIME_MAX - auwStartTime) + auwEndTime;
        auwProcTime++;
    }
    if(auwProcTime > LETAPP_PROC_TIME_MAX){
        auwProcTime = LETAPP_PROC_TIME_MAX;
    }
    aswTime = (Type_sWord)auwProcTime;

    return(aswTime);
}
/* End of File */

