/********************************************************************************************/
/* File Name         : logapp_if.c                                                          */
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
#include <stdio.h>
#include <stdarg.h>
#include <string.h> 
#include "os_framework.h"
#include "apn_basic_type.h"
#include "generic.h"

/* Log Header Files */
#include "logapp.h"
#include "logapp_common.h"
#include "logapp_core.h"

/* Other modules header files */
//#include "pm_ext.h"
#include  "rpc_if.h"
#include  "rpc_event.h"


/********************************************************************************************/
/*                           Macro Definition Section                                       */
/********************************************************************************************/

/********************************************************************************************/
/*                           Enumeration Type Definition Section                            */
/********************************************************************************************/

/********************************************************************************************/
/*                           User type definition section                                   */
/********************************************************************************************/

/********************************************************************************************/
/*                           Structure/Union Type Definition Section                        */
/********************************************************************************************/
typedef struct {
	Type_uHWord uhRpcEvent;		/* rpc event */
	Type_uHWord uhLogEvent; 	/* log event */
}LogResEvent_ts;				/* RES EVENT STRUCT*/

/********************************************************************************************/
/*                           ROM Table Section                                              */
/********************************************************************************************/
static const LogResEvent_ts nstLogRpcEventChg[] = {
{ EVT_LOG_A53TOR5_ATAT_ON,  		EVT_ATATLOG_LOGGING_ON 	},
{ EVT_LOG_A53TOR5_ATAT_OFF, 		EVT_ATATLOG_LOGGING_OFF },
{ EVT_LOG_A53TOR5_POWER_ON,			EVT_A53_POWER_ON		},
{ EVT_LOG_A53TOR5_POWER_OFF,		EVT_A53_POWER_OFF		},
{ EVT_LOG_MAX,                      EVT_LOG_INF_MAX 		}
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
LogOutputInfo_ts wstLogInfo;
/********************************************************************************************/
/*							Function Definition Section										*/
/********************************************************************************************/

/********************************************************************************************/
/* Function Name	: wubLogApp_Init									        	        */
/* Date				: 2021/10/27    										                */
/* Author			: tianni 													            */
/* Description		: init												                    */
/* Argument Code	: NONE  														        */
/* Return Code		: NONE  								    				            */
/*------------------------------------------------------------------------------------------*/
/* Revision History                                                                         */
/* No.        Date        Revised by        Explanation                                     */
/********************************************************************************************/
Type_uByte wubLogApp_Init(void)
{
#ifdef LOGMGR_DEBUG
	LOG_OUTPUT(LOG_APP_ID_LOGAPP, DEBUGLOG_LEVEL_DEBUG, ATAT_LEVEL_OFF, "LOG[wubLogApp_Init]id = %d, atatlevel = %d\n",LOG_APP_ID_LOGAPP, ATAT_LEVEL_OFF);
#endif
	Type_uByte aubRes = RES_OK;
	//Global Variable Initialize
	wubATATLogState = LOGGINGAPP_ATAT_OFF;
	wubStartUpState = LOGGINGAPP_START_UP_OFF;
	wubMaxCount = LOGAPP_NUM_0;
	wvdGen_MemSet(wstLogSyncInfo, LOGAPP_NUM_0, sizeof(LogOutputInfo_ts)*LOGGINGAPP_INFO_NUM_MAX);	/* change by tn in 20220402 for error num & 20220406 for PClink delete '&'*/
	wvdGen_MemSet(&wstLogInfo, LOGAPP_NUM_0, sizeof(LogOutputInfo_ts));
	return aubRes;
}

/********************************************************************************************/
/* Function Name	: wubR5Log_StandbyOut													*/
/* Date				: 2021/10/27															*/
/* Author			: tianni     															*/
/* Description		: (API)Standby-Out		                 								*/
/* Argument Code	: NONE																	*/
/* Return Code		: RES_OK			- Success											*/
/*					: RES_NG			- Failed											*/
/*					: RES_BUSY			- Semaphore Timeout									*/
/*------------------------------------------------------------------------------------------*/
/* Revision History																			*/
/* No		Date		Revised by		Explanation											*/
/********************************************************************************************/
Type_uByte wubR5Log_StandbyOut(void)
{
	// delete by tn in 20220505 for LOG_OUTPUT time-consuming operation.
	// LOG_OUTPUT(LOG_APP_ID_LOGAPP, DEBUGLOG_LEVEL_DEBUG, ATAT_LEVEL_OFF, "LOG[wubR5Log_StandbyOut]\n");
	Type_uByte aubRes = RES_OK;
	/* do nothing. */
	return aubRes;
}

/********************************************************************************************/
/* Function Name	: wubR5Log_StandbyIn													*/
/* Date				: 2021/10/27															*/
/* Author			: tianni     															*/
/* Description		: (API)Standby-In									                 	*/
/* Argument Code	: NONE																	*/
/* Return Code		: RES_OK			- Success											*/
/*					: RES_NG			- Failed											*/
/*					: RES_BUSY			- Semaphore Timeout									*/
/*------------------------------------------------------------------------------------------*/
/* Revision History																			*/
/* No		Date		Revised by		Explanation											*/
/********************************************************************************************/
Type_uByte wubR5Log_StandbyIn(void)
{
	// delete by tn in 20220505 for LOG_OUTPUT time-consuming operation.
	// LOG_OUTPUT(LOG_APP_ID_LOGAPP, DEBUGLOG_LEVEL_DEBUG, ATAT_LEVEL_OFF, "LOG[wubR5Log_StandbyIn]\n");
	Type_uByte aubRes = RES_OK;
	wubStartUpState = LOGGINGAPP_START_UP_OFF;
	return aubRes;
}

/********************************************************************************************/
/* Function Name	: wubR5Log_EnforceStandby									     		*/
/* Date				: 2021/10/27															*/
/* Author			: tianni    															*/
/* Description		: (API) Enforce Standby-In			                             		*/
/* Argument Code	: NONE																	*/
/* Return Code		: NONE																	*/
/*------------------------------------------------------------------------------------------*/
/* Revision History																			*/
/* No		Date		Revised by		Explanation											*/
/*==========================================================================================*/
/********************************************************************************************/
Type_uByte wubR5Log_EnforceStandby(void)
{
	Type_uByte aubRes = RES_OK;
	wubStartUpState = LOGGINGAPP_START_UP_OFF;
	return aubRes;
}

/********************************************************************************************/
/* Function Name	: wvdLogAppCallBack  													*/
/* Date				: 2021/10/27															*/
/* Author			: tianni 																*/
/* Description		: to OS_SendQueue()											          	*/
/* Argument Code	: aubSrcTsk			[IN]	Send Mail Box ID							*/
/* 					  aswEventCode		[IN]	Send Event Code								*/
/* 					  avdBuffer_p		[IN]	Send Mail Buffer							*/
/* 					  auwBufferSize		[IN]	Send Mail Buffer Size						*/
/* Return Code		: RES_OK			Success												*/
/*					: RES_NG			Failed												*/
/*------------------------------------------------------------------------------------------*/
/* Revision History																			*/
/* No		Date		Revised by		Explanation											*/
/********************************************************************************************/
void wvdLogAppCallBack(const Type_uByte aubsrcid, const Type_uHWord aubevent, const void *data, const Type_uHWord auhlength)
{
#ifdef LOGMGR_DEBUG
	LOG_OUTPUT(LOG_APP_ID_LOGAPP, DEBUGLOG_LEVEL_DEBUG, ATAT_LEVEL_OFF, "[wvdLogAppCallBack](event=0x%04x)\n",aubevent);
#endif
	Type_uByte aubResult;	//add by tn in 20220329 for PClint No.1395 Ignoring return.
	Type_uHWord auhLogEvent;
	Type_uByte aubChkNum;

	aubResult = RES_NG;
	auhLogEvent = EVT_LOG_MIN;
	aubChkNum = LOGAPP_NUM_0;

	while (aubChkNum < (sizeof(nstLogRpcEventChg) / sizeof(LogResEvent_ts)))
	{
		
		if(nstLogRpcEventChg[aubChkNum].uhRpcEvent == aubevent)
		{
			auhLogEvent = nstLogRpcEventChg[aubChkNum].uhLogEvent;
			aubResult = wubLogApp_SendMail(aubsrcid, auhLogEvent, data, auhlength);
			if (RES_NG == aubResult)	//add by tn in 20220329 for PClint No.1395 Ignoring return.
			{
				wubLogApp_SendMail(aubsrcid, auhLogEvent, data, auhlength);
			}
			else
			{
				/* do nothing */
			}
			
			break;
		}
		else
		{
			aubChkNum++;
		}
	}

}

/********************************************************************************************/
/* Function Name	: LOG_OUTPUT									        	            */
/* Date				: 2021/10/27    										                */
/* Author			: tianni 													            */
/* Description		: output log											                */
/* Argument Code	: aubLogLevel		[IN]	 Send Log Level  				            */
/*                    aubATATLevel      [IN]     Send ATAT Level                            */
/*                    auhLen  	    	[IN]	 Send Log length                            */
/*                    achLog  	    	[IN]	 Send Log Text                              */
/* Return Code		: -										    				            */
/*------------------------------------------------------------------------------------------*/
/* Revision History                                                                         */
/* No.        Date        Revised by        Explanation                                     */
/********************************************************************************************/
#ifdef LOGMGR_WINDOWS
Type_uByte LOG_OUTPUT(Type_uByte aubAppID, Type_uByte aubLogLevel, Type_uByte aubATATLevel, Type_Char* achLog)
#else
Type_uByte LOG_OUTPUT(Type_uByte aubAppID, Type_uByte aubLogLevel, Type_uByte aubATATLevel, Type_Char *achFormat,...)
#endif
{
	Type_uByte ret = RES_NG;
	Type_sByte asbResult = RES_NG;	//add by tn in 20220329 for PClint No.1419 Ignoring return.
	if((DEBUGLOG_LEVEL_OFF != aubLogLevel && DEBUGLOG_LEVEL_SET >= aubLogLevel) || 
	(ATAT_LEVEL_ON == aubATATLevel && LOGGINGAPP_ATAT_ON == wubATATLogState))	/* add by tn in 202203023 for #64706. */
	{
		Type_uByte auhLen = 0;
#ifndef LOGMGR_WINDOWS
		va_list astArgs;
		Type_Char achLog[LOGGINGAPP_LOG_SIZE_MAX]={};
		va_start(astArgs,achFormat);
		vsprintf(achLog, achFormat, astArgs);

		va_end(astArgs);
#endif
		auhLen = strlen(achLog)+LOGAPP_NUM_1;	// add by tn in 202203018 for #63960.
	
#ifdef LOGMGR_DEBUG
		/*printf("pLOG[LOG_OUTPUT]appid = %d, loglevel = %d, atatlevel = %d, loglen = %d, logtext = %s\n",
					aubAppID,aubLogLevel,aubATATLevel,auhLen,achLog);*/
#endif
		if (achLog != NULL)
		{
#ifdef LOGMGR_DEBUG
			// printf("pLOG[LOG_OUTPUT]A53 STATE = %d\n", wubStartUpState);
#endif
			if (LOGGINGAPP_START_UP_OFF == wubStartUpState)	
			{
				/* A53 start off */
				if (wubMaxCount < LOGGINGAPP_INFO_NUM_MAX)
				{
					wvdGen_MemSet(&wstLogSyncInfo[wubMaxCount], 0x00, sizeof(LogOutputInfo_ts));
					wstLogSyncInfo[wubMaxCount].ubAppID = aubAppID;
					wstLogSyncInfo[wubMaxCount].ubLogLevel = aubLogLevel;
					wstLogSyncInfo[wubMaxCount].ubATATLevel = aubATATLevel;
					wstLogSyncInfo[wubMaxCount].ubLogLen = auhLen;
					if (auhLen < LOGGINGAPP_LOG_SIZE_MAX)
					{
						wvdGen_MemCpy(wstLogSyncInfo[wubMaxCount].chLogText, achLog, auhLen);
						ret = RES_OK;
					}
					else 
					{
						wvdGen_MemCpy(wstLogSyncInfo[wubMaxCount].chLogText, achLog, LOGGINGAPP_LOG_SIZE_MAX);
						ret = RES_NG;
					}
#ifdef LOGMGR_DEBUG
					/* printf("pLOG[LOG_OUTPUT]<100 Count=%d id=%d, loglevel=%d, atlevel=%d, len=%d, text=%s\n", wubMaxCount, 
					wstLogSyncInfo[wubMaxCount].ubAppID, wstLogSyncInfo[wubMaxCount].ubLogLevel, 
					wstLogSyncInfo[wubMaxCount].ubATATLevel, wstLogSyncInfo[wubMaxCount].ubLogLen, 
					wstLogSyncInfo[wubMaxCount].chLogText);*/
#endif
					wubMaxCount++;
				}
				else
				{
					wubMaxCount = (LOGGINGAPP_INFO_NUM_MAX_X2 <= wubMaxCount ? LOGGINGAPP_INFO_NUM_MAX : wubMaxCount);
					
					wvdGen_MemSet(&wstLogSyncInfo[wubMaxCount-LOGGINGAPP_INFO_NUM_MAX], 0x00, sizeof(LogOutputInfo_ts));
					wstLogSyncInfo[wubMaxCount-LOGGINGAPP_INFO_NUM_MAX].ubAppID = aubAppID;
					wstLogSyncInfo[wubMaxCount-LOGGINGAPP_INFO_NUM_MAX].ubLogLevel = aubLogLevel;
					wstLogSyncInfo[wubMaxCount-LOGGINGAPP_INFO_NUM_MAX].ubATATLevel = aubATATLevel;
					wstLogSyncInfo[wubMaxCount-LOGGINGAPP_INFO_NUM_MAX].ubLogLen = auhLen;
					if (auhLen < LOGGINGAPP_LOG_SIZE_MAX)
					{
						wvdGen_MemCpy(wstLogSyncInfo[wubMaxCount-LOGGINGAPP_INFO_NUM_MAX].chLogText, achLog, auhLen);
						ret = RES_OK;
					}	
					else 
					{
						wvdGen_MemCpy(wstLogSyncInfo[wubMaxCount-LOGGINGAPP_INFO_NUM_MAX].chLogText, achLog, LOGGINGAPP_LOG_SIZE_MAX);
						ret = RES_NG;
					}
#ifdef LOGMGR_DEBUG
					/*printf("pLOG[LOG_OUTPUT]>100 Count=%d id=%d, loglevel=%d, atlevel=%d, len=%d, text=%s\n", wubMaxCount, 
					wstLogSyncInfo[wubMaxCount-LOGGINGAPP_INFO_NUM_MAX].ubAppID, wstLogSyncInfo[wubMaxCount-LOGGINGAPP_INFO_NUM_MAX].ubLogLevel, 
					wstLogSyncInfo[wubMaxCount-LOGGINGAPP_INFO_NUM_MAX].ubATATLevel, wstLogSyncInfo[wubMaxCount-LOGGINGAPP_INFO_NUM_MAX].ubLogLen, 
					wstLogSyncInfo[wubMaxCount-LOGGINGAPP_INFO_NUM_MAX].chLogText);*/
#endif
					wubMaxCount++;
				}
			}
			else
			{
				/* A53启动，正常log输出  */
				wvdGen_MemSet(&wstLogInfo, 0x00, sizeof(LogOutputInfo_ts));
				Type_uHWord auhInfoLen = 0;
				wstLogInfo.ubAppID = aubAppID;
				wstLogInfo.ubLogLevel = aubLogLevel;
				wstLogInfo.ubATATLevel = aubATATLevel;
				wstLogInfo.ubLogLen = auhLen < LOGGINGAPP_LOG_SIZE_MAX ? auhLen : LOGGINGAPP_LOG_SIZE_MAX;
				wvdGen_MemCpy(wstLogInfo.chLogText, achLog, wstLogInfo.ubLogLen);
				auhInfoLen = wstLogInfo.ubLogLen + LOGGINGAPP_INFO_LEN;
				
				if (DEBUGLOG_LEVEL_SET >= aubLogLevel && DEBUGLOG_LEVEL_OFF != aubLogLevel)	/* add by tn in 202203023 for #64706. */
				{
					/* change by tn in 20220414 for #64609 conversion event to myself when outputlog. */
					asbResult = wubLogApp_SendMail(ARPC_ID_R5_LOGAPP, EVT_LOG_R5_CHANGE_LOG_INFO, &wstLogInfo, auhInfoLen);
					if (RES_NG == asbResult)	//add by tn in 20220329 for PClint No.1419 Ignoring return.
					{
						wubLogApp_SendMail(ARPC_ID_R5_LOGAPP, EVT_LOG_R5_CHANGE_LOG_INFO, &wstLogInfo, auhInfoLen);
					}
					else
					{
						/* do nothing */
					}
				}
				else 
				{
					/* do nothing. */
				}
				
				if (ATAT_LEVEL_ON == aubATATLevel && LOGGINGAPP_ATAT_ON == wubATATLogState)
				{
					printf("[ATAT_CN_01]%s\n", wstLogInfo.chLogText);
				}
				else 
				{
					/* do nothing. */
				}
				if (auhLen < LOGGINGAPP_LOG_SIZE_MAX)
				{
					ret = RES_OK;
				}
				else 
				{
					ret = RES_NG;
				}
			}
		}
		else 
		{
			/* do nothing. */
		}
	}
	return ret;
}


/* End of File */

