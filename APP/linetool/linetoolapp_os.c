/********************************************************************************************/
/* File Name	: linetoolapp.h 															*/
/* Date 		: 2021.11.30																*/
/* Author		: Liujie																*/
/* Model		: EDA 																*/
/* Copyright (c)  2021 Alpine Electronics (China) Co. LTD.	All Rights Reserved 			*/
/*------------------------------------------------------------------------------------------*/
/* Revision History 																		*/
/* No		Date		Revised by		Function Name										*/
/* 0001     2021.11.30  Liujie          Create												*/
/*------------------------------------------------------------------------------------------*/
/********************************************************************************************/

/********************************************************************************************/
/*                             Include File Section                                         */
/********************************************************************************************/

/* System Header Files */
#include "os_framework.h"
// #include "Log_Ext.h"

/* Linetool Header Files */
#include "linetoolapp.h"
// #include "linetoolapp_os.h"      //0329 cxx for PCLintResult 766
#include "linetoolapp_common.h"
#include "linetoolapp_core.h"

/* Other Components */

/********************************************************************************************/
/*                           Macro Definition Section                                       */
/********************************************************************************************/
#define UART_LINETOOL_TIMEOUT   (50U)

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
/* Timeout values */
static Type_sWord  nswLineToolDrvFlagGuardTimer;  /* for waiting event flag */

/********************************************************************************************/
/*                           Static Function Declaration Section                            */
/********************************************************************************************/

/* not used. */

/********************************************************************************************/
/*                           Global variable definition section                             */
/********************************************************************************************/

/* For Main Task */
OS_TimeTick_t    wswLineToolAppTmout;       /* OS_ReceiveQueue timeout */

/********************************************************************************************/
/*                           Function Definition Section                                    */
/********************************************************************************************/
/*********************************************************************************************
*  function                 : TSK_LINETOOLAPP
*  date                     : 2016.05.01
*  autho                    : zhangjinzhu
*  description              : LineToolApp Task
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
void TSK_LINETOOLAPP(void)
{
    LINETOOLAPP_D("TSK_LINETOOLDRV start!");
    /* Auto variable Define */
    OS_ErrCode_t aswErrCode; /* <AUTO> ErrorCode */
    OS_StdQueue_ts astMainStdQueue;  /* <AUTO> MessageInformation */
    OS_StdQueue_ts *astMainStdQueue_p = &astMainStdQueue;
    Type_uByte aubInitRes = RES_NG; //0329 cxx for PCLintResult 534

	/* Initalize */
    aswErrCode = OS_ERR_OK;
    wswLineToolAppTmout = OS_TOUT_FOREVER;
    wvdGen_MemSet(&astMainStdQueue, LINETOOL_NUM_0, sizeof(OS_StdQueue_ts));

    aubInitRes = wubLineTool_Init();     //0329 cxx for PCLintResult 534
    if(RES_NG == aubInitRes){
        /* No Action */
    }
    /* Setup Main Process Engine */
    wvdLineToolApp_SetupProEng();

    while (LINETOOL_NUM_1) {
        /* Receive DiagApp Message Information */
        aswErrCode = OS_ReceiveQueue(MBX_ID_LINETOOL, wswLineToolAppTmout, &astMainStdQueue_p);

        /* Receive DiagApp Message OK */
        if (OS_ERR_OK == aswErrCode) {
            if (astMainStdQueue_p != OS_NULL) {
                /* Execute Process */
                wvdLineToolAppControl(astMainStdQueue_p);
                /* Queue Buffer Free Memory */
                if (OS_NULL != astMainStdQueue_p->vdQueueBuffer_p) {
                    /* Free Memory */
                    (void)OS_ReleaseFMemoryBlock( astMainStdQueue_p->vdQueueBuffer_p);
                    astMainStdQueue_p->vdQueueBuffer_p = OS_NULL;
                } else {
                    /* No Action */
                }
            } else {
                ; /* No Action */
            }
        } else if (OS_ERR_TMOUT == aswErrCode) {
            /* Receive DiagApp Message Timeout */
            astMainStdQueue.uhEventCode = LINETOOL_EV_TIMEOUT;
            /* Execute Process */
            wvdLineToolAppControl(astMainStdQueue_p);
        } else {
            /* No Action */
        }
    }
}

/********************************************************************************************/
/** \function       TSK_LINETOOLDRV
 *  \date           2021/01/11
 *  \author         KFED
 *  \description    LineTool Driver task
 *
 *  <!------------- Argument Code ----------------------------------------------------------->
 *  \param[in]
 *
 *  <!------------- Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *  \par    Revision History:
 *  <!----- No.     Date        Revised by      Details ------------------------------------->
 *
 ********************************************************************************************/
void TSK_LINETOOLDRV(void)
{
    OS_ErrCode_t aswFlagResult;
    OS_FlgPtn_t auwFlagPattern;

    aswFlagResult = OS_ERR_OK;
    auwFlagPattern = LINETOOL_NUM_0;

    nswLineToolDrvFlagGuardTimer = OS_TOUT_FOREVER;

    while (LINETOOL_NUM_1) {
        aswFlagResult = OS_WaitEventFlag(FLG_ID_LINETOOLDRV, LINETOOLDRV_FLGPTN, OS_FALSE, &auwFlagPattern, nswLineToolDrvFlagGuardTimer);

        if (OS_ERR_OK == aswFlagResult) {

            if ((auwFlagPattern & LINETOOLDRV_FLGPTN_RECDATA) == LINETOOLDRV_FLGPTN_RECDATA) {

                /* Reading 1Byte data from UARTn succeeded. */
                (void)OS_ClearEventFlag(FLG_ID_LINETOOLDRV, ~LINETOOLDRV_FLGPTN_RECDATA);
                auwFlagPattern = LINETOOL_NUM_0;
                nswLineToolDrvFlagGuardTimer = UART_LINETOOL_TIMEOUT;

            } else if ((auwFlagPattern & LINETOOLDRV_FLGPTN_ERR) == LINETOOLDRV_FLGPTN_ERR) {

                nswLineToolDrvFlagGuardTimer = OS_TOUT_FOREVER;
                wstLineToolDrvVar.ubSaveData_p = wstLineToolDrvVar.ubRevData_a;
                wstLineToolDrvVar.ubDataRecLength = LINETOOL_NUM_0;
                (void)OS_ClearEventFlag(FLG_ID_LINETOOLDRV, ~LINETOOLDRV_FLGPTN_ERR);
                auwFlagPattern = LINETOOL_NUM_0;

            } else {

                nswLineToolDrvFlagGuardTimer = OS_TOUT_FOREVER;
                {// TODO: add for test
#if (LINETOOLAPP_DEBUG)
                    printf("Uart rcv = [ ");
                    for (int i = 0; i < wstLineToolDrvVar.ubDataRecLength; i++) {
                        printf("%02x ", wstLineToolDrvVar.ubRevData_a[i]);
                    }
                    printf("]\n");
#endif
                }
                /* send uart data to TSK_LINETOOLAPP */
                if (wstLineToolDrvVar.ubDataRecLength <= LINETOOL_CMDLENGTH) {
                    (void)wubLineToolAppSendQueue(LINETOOL_EV_UART_DATA, wstLineToolDrvVar.ubRevData_a, wstLineToolDrvVar.ubDataRecLength);
                } else {
                    (void)wubLineToolAppSendQueue(LINETOOL_EV_UART_DATA, wstLineToolDrvVar.ubRevData_a, LINETOOL_CMDLENGTH);
                }
                wstLineToolDrvVar.ubSaveData_p = wstLineToolDrvVar.ubRevData_a;
                wstLineToolDrvVar.ubDataRecLength = LINETOOL_NUM_0;

            }

        } else if (OS_ERR_TMOUT == aswFlagResult) {
        	nswLineToolDrvFlagGuardTimer = OS_TOUT_FOREVER;
            /* send uart data to TSK_LINETOOLAPP */
            if (wstLineToolDrvVar.ubDataRecLength <= LINETOOL_CMDLENGTH) {
                (void)wubLineToolAppSendQueue(LINETOOL_EV_UART_DATA, wstLineToolDrvVar.ubRevData_a, wstLineToolDrvVar.ubDataRecLength);
            } else {
                (void)wubLineToolAppSendQueue(LINETOOL_EV_UART_DATA, wstLineToolDrvVar.ubRevData_a, LINETOOL_CMDLENGTH);
            }
            wstLineToolDrvVar.ubSaveData_p = wstLineToolDrvVar.ubRevData_a;
            wstLineToolDrvVar.ubDataRecLength = LINETOOL_NUM_0;

        } else {
            /* No Action*/
        }
    }

}

/* End of File */

