/********************************************************************************************/
/* File Name         : logapp_core.c                                                        */
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
#include "apn_basic_type.h"
// #include "string.h"

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
/*                           Static Function Declaration Section                            */
/********************************************************************************************/

/********************************************************************************************/
/*							Global Variable Definition Section								*/
/********************************************************************************************/
Type_uByte wubATATLogState; 	/* ATAT log state */
Type_uByte wubStartUpState; /* power start up state */
Type_uByte wubMaxCount;	/* 存储log总数 */
LogOutputInfo_ts wstLogSyncInfo[LOGGINGAPP_INFO_NUM_MAX];	/* log数据 */

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
/* Function Name	: wubLogApp_SendMail									        	    */
/* Date				: 2021/10/27    										                */
/* Author			: tianni 													            */
/* Description		: send mail												                */
/* Argument Code	: auhEventCode      [IN]     Send Event Code                            */
/*                    avdBuffer_p       [IN]     Send Mail Buffer                           */
/*                    auwSize  	    	[IN]	 Send Mail Buffer Size                      */
/* Return Code		: RES_OK			Success												*/
/*					: RES_NG			Failed												*/
/*------------------------------------------------------------------------------------------*/
/* Revision History                                                                         */
/* No.        Date        Revised by        Explanation                                     */
/********************************************************************************************/
Type_uByte wubLogApp_SendMail(const Type_uByte aubsrcid, const Type_uHWord auhEventCode, const void* avdBuffer_p, const Type_uWord auwSize)
{
	OS_ErrCode_t aswErrCode;        /* <AUTO> Error Code */
	OS_VoidPtr_t avdMailBuff_p;     /* <AUTO> Memory Address */
	OS_StdQueue_ts astStdMail;       /* <AUTO> Mail */
	Type_uByte aubReturn;           /* <AUTO> Return Code */
	Type_uWord auwSetBufferSize;

	/* Initalize */
	aswErrCode = OS_ERR_NG;
	avdMailBuff_p = OS_NULL;
	aubReturn = RES_NG;
	wvdGen_MemSet(&astStdMail, LOGAPP_NUM_0, sizeof(OS_StdQueue_ts));
	auwSetBufferSize = auwSize;

	if (LOGAPP_MPF_MAXSIZE < auwSetBufferSize) 
	{
		auwSetBufferSize = LOGAPP_MPF_MAXSIZE;
	}
	else 
	{
		/* No Action */
	}

	if ((STD_NULL != avdBuffer_p) && (LOGAPP_NUM_0 < auwSetBufferSize)) 
	{
		(void)OS_GetFMemoryBlock(&avdMailBuff_p, auwSetBufferSize);

		if (OS_NULL != avdMailBuff_p) 
		{ 	
			/* Message With MailBuffer */
			/* Copy sending data */
			wvdGen_MemCpy(avdMailBuff_p, avdBuffer_p, auwSetBufferSize);

			astStdMail.swSrcTaskId = aubsrcid;
			astStdMail.uhEventCode = auhEventCode;
			astStdMail.vdQueueBuffer_p = avdMailBuff_p;
			astStdMail.uwQueueSize = auwSetBufferSize;

			aswErrCode = OS_SendQueue(MBX_ID_LOGAPP, OS_MAIL_PRI_NORMAL, &astStdMail);
			if (OS_ERR_OK == aswErrCode) 
			{
				aubReturn = RES_OK;
			}
			else 
			{
				/* Send Mail Error */
				if (OS_NULL != avdMailBuff_p) 
				{
					(void)OS_ReleaseFMemoryBlock(avdMailBuff_p);
					avdMailBuff_p = OS_NULL;
				}
				else
				{
					/* Debug Message */
				}
			}
		}
		else 
		{
			/* Memory Alloc Error */
		}
	}
	else 
	{ /* Message Without MailBuffer */
		astStdMail.swSrcTaskId = aubsrcid;
		astStdMail.uhEventCode = auhEventCode;
		astStdMail.vdQueueBuffer_p = OS_NULL;
		astStdMail.uwQueueSize = LOGAPP_NUM_0;

		aswErrCode = OS_SendQueue(MBX_ID_LOGAPP, OS_MAIL_PRI_NORMAL, &astStdMail);

		if (OS_ERR_OK == aswErrCode) 
		{
			aubReturn = RES_OK;
		}
		else 
		{
			/* Debug Message */
		}
	}
	return aubReturn;
}

/********************************************************************************************/
/* Function Name	: wvdLogRecieveEvent									        	    */
/* Date				: 2021/10/27    										                */
/* Author			: tianni 													            */
/* Description		: recieve event		        									        */
/* Argument Code	: NONE														            */
/* Return Code		: NONE  								    				            */
/*------------------------------------------------------------------------------------------*/
/* Revision History                                                                         */
/* No.        Date        Revised by        Explanation                                     */
/********************************************************************************************/
void wvdLogRecieveEvent(void)
{
	Type_sByte asbResult = RES_NG;	//add by tn in 20220329 for PClint No.1378&1380&1382 Ignoring return.
	/* 当前log数 */
	Type_uByte aubNowCount = 0;
	/* 当前replace log数 */
	Type_uByte aubReplace = 0;
	Type_uHWord auhLen = 0;
	LogOutputInfo_ts astR5Info;	//add by tn in 20220414 for #64609 conversion event to myself when outputlog.
	wvdGen_MemSet(&astR5Info, 0x00, sizeof(LogOutputInfo_ts));
	if (wstLogAppStdMail_p->uhEventCode != OS_NULL) 
	{
		if (wstLogAppStdMail_p->uhEventCode < EVT_LOG_INF_MAX) 
		{ /* Log Request Event Check */

			switch (wstLogAppStdMail_p->uhEventCode)
			{
				case EVT_ATATLOG_LOGGING_ON:
					wubATATLogState = LOGGINGAPP_ATAT_ON;
#ifdef LOGMGR_DEBUG
					LOG_OUTPUT(LOG_APP_ID_LOGAPP, DEBUGLOG_LEVEL_DEBUG, ATAT_LEVEL_OFF, "[wvdLogRecieveEvent]EVT_ATATLOG_LOGGING_ON\n");
#endif
					break;
				case EVT_ATATLOG_LOGGING_OFF:
					wubATATLogState = LOGGINGAPP_ATAT_OFF;
#ifdef LOGMGR_DEBUG
					LOG_OUTPUT(LOG_APP_ID_LOGAPP, DEBUGLOG_LEVEL_DEBUG, ATAT_LEVEL_OFF, "[wvdLogRecieveEvent]EVT_ATATLOG_LOGGING_OFF\n");
#endif
					break;
				case EVT_A53_POWER_OFF:
					wubStartUpState = LOGGINGAPP_START_UP_OFF;
#ifdef LOGMGR_DEBUG
					LOG_OUTPUT(LOG_APP_ID_LOGAPP, DEBUGLOG_LEVEL_DEBUG, ATAT_LEVEL_OFF, "[wvdLogRecieveEvent]EVT_A53_POWER_OFF\n");
#endif
					break;	
				case EVT_A53_POWER_ON:
					wubStartUpState = LOGGINGAPP_START_UP_ON;
					if(wubMaxCount < LOGGINGAPP_INFO_NUM_MAX)
					{
						for(aubNowCount = 0; aubNowCount < wubMaxCount; aubNowCount++)
						{
							if(wubStartUpState == LOGGINGAPP_START_UP_ON)
							{
								/* add by tn in 202203023 for #64706. */
								if (DEBUGLOG_LEVEL_SET >= wstLogSyncInfo[aubNowCount].ubLogLevel && DEBUGLOG_LEVEL_OFF != wstLogSyncInfo[aubNowCount].ubLogLevel)
								{
									auhLen = wstLogSyncInfo[aubNowCount].ubLogLen + LOGGINGAPP_INFO_LEN;
									asbResult = wsbRPCSendMsg(ARPC_ID_R5_LOGAPP, ARPC_ID_A53_LOGAPP, EVT_LOG_R5_OUTPUT_LOG_INFO, &wstLogSyncInfo[aubNowCount], auhLen);
									if (RES_OK != asbResult)	//add by tn in 20220329 for PClint No.1378 Ignoring return.
									{
										wsbRPCSendMsg(ARPC_ID_R5_LOGAPP, ARPC_ID_A53_LOGAPP, EVT_LOG_R5_OUTPUT_LOG_INFO, &wstLogSyncInfo[aubNowCount], auhLen);
									}
									else
									{
										/* do nothing */	// RPC_OK
									}	
#ifdef LOGMGR_DEBUG
									// printf("pLOG[wvdLogRecieveEvent]< 100 power on wsbRPCSendMsg Info=%s, len=%d\n", wstLogSyncInfo[aubNowCount].chLogText, auhLen);
#endif
								}
							}
							else
							{
								break;
							}
						}
					}
					else
					{
						for(aubNowCount = (wubMaxCount - LOGGINGAPP_INFO_NUM_MAX); aubNowCount < LOGGINGAPP_INFO_NUM_MAX; aubNowCount++)
						{
							if(wubStartUpState == LOGGINGAPP_START_UP_ON)
							{
								/* add by tn in 202203023 for #64706. */
								if (DEBUGLOG_LEVEL_SET >= wstLogSyncInfo[aubNowCount].ubLogLevel && DEBUGLOG_LEVEL_OFF != wstLogSyncInfo[aubNowCount].ubLogLevel)
								{
									auhLen = wstLogSyncInfo[aubNowCount].ubLogLen + LOGGINGAPP_INFO_LEN;
									asbResult = wsbRPCSendMsg(ARPC_ID_R5_LOGAPP, ARPC_ID_A53_LOGAPP, EVT_LOG_R5_OUTPUT_LOG_INFO, &wstLogSyncInfo[aubNowCount], auhLen);
									if (RES_OK != asbResult)	//add by tn in 20220329 for PClint No.1380 Ignoring return.
									{
										wsbRPCSendMsg(ARPC_ID_R5_LOGAPP, ARPC_ID_A53_LOGAPP, EVT_LOG_R5_OUTPUT_LOG_INFO, &wstLogSyncInfo[aubNowCount], auhLen);
									}
									else
									{
										/* do nothing */	// RPC_OK
									}
#ifdef LOGMGR_DEBUG
									// printf("pLOG[wvdLogRecieveEvent]< wubMaxCount wsbRPCSendMsg Info=%s, len=%d\n", wstLogSyncInfo[aubNowCount].chLogText, auhLen);
#endif
								}
							}
							else
							{
								break;
							}
						}
						for(aubReplace = 0; aubReplace < (wubMaxCount - LOGGINGAPP_INFO_NUM_MAX); aubReplace++)
						{
							if(wubStartUpState == LOGGINGAPP_START_UP_ON)
							{
								/* add by tn in 202203023 for #64706. */
								if ((DEBUGLOG_LEVEL_SET >= wstLogSyncInfo[aubReplace].ubLogLevel && DEBUGLOG_LEVEL_OFF != wstLogSyncInfo[aubReplace].ubLogLevel))
								{
									auhLen = wstLogSyncInfo[aubReplace].ubLogLen + LOGGINGAPP_INFO_LEN;
									asbResult = wsbRPCSendMsg(ARPC_ID_R5_LOGAPP, ARPC_ID_A53_LOGAPP, EVT_LOG_R5_OUTPUT_LOG_INFO, &wstLogSyncInfo[aubReplace], auhLen);
									if (RES_OK != asbResult)	//add by tn in 20220329 for PClint No.1382 Ignoring return.
									{
										wsbRPCSendMsg(ARPC_ID_R5_LOGAPP, ARPC_ID_A53_LOGAPP, EVT_LOG_R5_OUTPUT_LOG_INFO, &wstLogSyncInfo[aubReplace], auhLen);
									}
									else
									{
										/* do nothing */	// RPC_OK
									}
#ifdef LOGMGR_DEBUG
									// printf("pLOG[wvdLogRecieveEvent]cover wsbRPCSendMsg Info=%s, len=%d\n", wstLogSyncInfo[aubReplace].chLogText, auhLen);
#endif
								}
							}
							else
							{
								break;
							}
						}
					}
#ifdef LOGMGR_DEBUG
					LOG_OUTPUT(LOG_APP_ID_LOGAPP, DEBUGLOG_LEVEL_DEBUG, ATAT_LEVEL_OFF, "[wvdLogRecieveEvent]EVT_A53_POWER_ON end:wubMaxCount = %d\n", wubMaxCount);
#endif
					break;
				case EVT_LOG_R5_CHANGE_LOG_INFO:	/* add by tn in 20220414 for #64609 conversion event to myself when outputlog. */
					if (wstLogAppStdMail_p->vdQueueBuffer_p != STD_NULL) 
					{
						wvdGen_MemCpy(&astR5Info, wstLogAppStdMail_p->vdQueueBuffer_p, wstLogAppStdMail_p->uwQueueSize);
#ifdef LOGMGR_DEBUG
						/*printf("[EVT_LOG_R5_CHANGE_LOG_INFO](id=%d, log=%d, atat=%d, text=%s, len=%d, alllen=%d)\n",
						astR5Info.ubAppID, astR5Info.ubLogLevel,astR5Info.ubATATLevel,astR5Info.chLogText,astR5Info.ubLogLen,wstLogAppStdMail_p->uwQueueSize);*/
#endif
						asbResult = wsbRPCSendMsg(ARPC_ID_R5_LOGAPP, ARPC_ID_A53_LOGAPP, EVT_LOG_R5_OUTPUT_LOG_INFO, &astR5Info, wstLogAppStdMail_p->uwQueueSize);
						if (RES_OK != asbResult)	//add by tn in 20220329 for PClint No.1419 Ignoring return.
						{
							wsbRPCSendMsg(ARPC_ID_R5_LOGAPP, ARPC_ID_A53_LOGAPP, EVT_LOG_R5_OUTPUT_LOG_INFO, &astR5Info, wstLogAppStdMail_p->uwQueueSize);
						}
						else
						{
							/* do nothing */	// RPC_OK
						}
					}
					break;
				default:
					break;
			}
		}
		else
		{
			/* do nothing */
		}
	}
	else
	{
		/* do nothing */
	}
}

