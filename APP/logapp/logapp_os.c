/*
 * @Author: your name
 * @Date: 2021-11-19 09:48:12
 * @LastEditTime: 2021-11-19 16:51:49
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /TCC803x_IVI_MICOM_SDK_V1.9.0/src/APP/logapp/logapp_os.c
 */
/********************************************************************************************/
/* File Name         : logapp_os.c                                                          */
/* Date              : 2021/10/27                                                           */
/* Author            : tianni                                                               */
/* Model             : EDA                                                                  */
/* Description       : log Task execute related file                                        */
/* File Version      : -                                                                    */
/*------------------------------------------------------------------------------------------*/
/* Revision History                                                                         */
/* No.        Date        Revised by        Function Name                                   */
/* #0000      21/10/27    tianni            New                                             */
/********************************************************************************************/

/********************************************************************************************/
/*							Include File Section											*/
/********************************************************************************************/
/* System header files */
#include "os_framework.h"
#include "generic.h"
/* Log Header Files */
#include "logapp.h"
#include "logapp_os.h"
#include "logapp_common.h"
#include "logapp_core.h"

/* Other modules header files */
#include  "rpc_if.h"
#include  "rpc_event.h"


/********************************************************************************************/
/*							Debug Switch Definition Section									*/
/********************************************************************************************/

/********************************************************************************************/
/*							Macro Definition Section										*/
/********************************************************************************************/

/********************************************************************************************/
/*							User Type Definition Section									*/
/********************************************************************************************/

/********************************************************************************************/
/*							Structure Type Definition Section								*/
/********************************************************************************************/

/********************************************************************************************/
/*							Global Variable Definition Section								*/
/********************************************************************************************/
/* For Main Task */
OS_StdQueue_ts   *wstLogAppStdMail_p;   /* Mailbox Object Pointer */
/********************************************************************************************/
/*							Static Variable Definition Section								*/
/********************************************************************************************/

/********************************************************************************************/
/*							Prototype Declaration Section									*/
/********************************************************************************************/

/********************************************************************************************/
/*							Table Include Section											*/
/********************************************************************************************/

/********************************************************************************************/
/*							Function Definition Section										*/
/********************************************************************************************/

/********************************************************************************************/
/* Function Name	: TSK_lOGAPP									        	            */
/* Date				: 2021/10/27    										                */
/* Author			: tianni 													            */
/* Description		: LogApp Task												            */
/* Argument Code	: NONE 														            */
/* Return Code		: NONE									    				            */
/*------------------------------------------------------------------------------------------*/
/* Revision History                                                                         */
/* No.        Date        Revised by        Function Name                                   */
/********************************************************************************************/
void TSK_LOGAPP(void)
{
	//Type_uByte aubResult = RES_NG;	//add by tn in 20220329 for PClint No.1425 Ignoring return.       
	Type_sByte asbRpcRet = RES_NG;		//add by tn in 20220329 for PClint No.1426 Ignoring return.
	OS_StdQueue_ts astMail;
	OS_ErrCode_t auwMailRet;
	auwMailRet = OS_ERR_OK;
	/* clean log message buff */
	wvdGen_MemSet(&astMail, LOGAPP_NUM_0, sizeof(OS_StdQueue_ts));
	wstLogAppStdMail_p = &astMail;
	/* Initalize */
	
	/* delete by jc in 20220921 for power to LogAppInit start */
	//aubResult = wubLogApp_Init();
	//if (RES_OK != aubResult)	   //add by tn in 20220329 for PClint No.1425 Ignoring return.
	//{
		/* do nothing */
	//}
	/* delete by jc in 20220921 for power to LogAppInit end */
	
	asbRpcRet = wsbRPCInit(ARPC_ID_R5_LOGAPP,(wvdRPCCallback)wvdLogAppCallBack);
	if (RES_OK != asbRpcRet)	//add by tn in 20220329 for PClint No.1426 Ignoring return.
	{
		wsbRPCInit(ARPC_ID_R5_LOGAPP,(wvdRPCCallback)wvdLogAppCallBack);
	}
	else
	{
		/* do nothing */
	}
	
	while(1)
	{
		auwMailRet = OS_ReceiveQueue(MBX_ID_LOGAPP, OS_TOUT_FOREVER, &wstLogAppStdMail_p);
		switch (auwMailRet)
		{
			case OS_ERR_OK: 
				if(wstLogAppStdMail_p != OS_NULL)
				{
					wvdLogRecieveEvent();
				}
				else 
				{
					/* do nothing */
				}
				if (wstLogAppStdMail_p->vdQueueBuffer_p != NULL)
				{
					/* Memory Release */
					(void)OS_ReleaseFMemoryBlock(wstLogAppStdMail_p->vdQueueBuffer_p);
					wstLogAppStdMail_p->vdQueueBuffer_p = OS_NULL;
				}
				else 
				{
					/* do nothing */
				}
				break;
			case OS_ERR_TMOUT:
				break;
			case OS_ERR_NG:
				break;
			default:
				break;
		}
		OS_SleepTask(3);	/* add by tn in 20220414 for #64609 reset */
	}
}
