/********************************************************************************************/
/* File Name	: pm.c																		*/
/* Date			: 2021/11/12																*/
/* Author		: wangying																	*/
/* Model		: EDA																		*/
/* Description	: power watchdog															*/
/* Copyright (c)  2020 Alpine Electronics (China) Co. LTD.  All Rights Reserved				*/
/*------------------------------------------------------------------------------------------*/
/* Revision History																			*/
/* No		Date		Revised by		Function Name										*/
/* 																							*/
/*------------------------------------------------------------------------------------------*/
/********************************************************************************************/

/********************************************************************************************/
/*							Include File Section										*/
/********************************************************************************************/

#include "apn_basic_type.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "os_framework_type.h"
#include "os_framework.h"
#include "tcc_wdt.h"
#include "pmwdt.h"
#include "rpc_if.h"
#include "rpc_event.h"
#include "logapp.h"


/********************************************************************************************/
/*							Function Definition Section									*/
/********************************************************************************************/
static void nvdPmSoftWdtRpcRcv(const Type_uByte aubsrc, const Type_uHWord auhCmd, void *avdData_p, const Type_uHWord auhLength);
static void nvdPmSoftWdtHeartResCtrl(Type_uByte aubRes);
static void nvdPmSoftWdtSendMsg(Type_uByte aubReceiver, Type_uHWord auhEvent, void * auvData_p, Type_uHWord auhLength);
//static void nvdPmHardWdtSendMsg(Type_uByte aubReceiver, Type_uHWord auhEvent, void * auvData_p, Type_uHWord auhLength);
static void nvdPmSoftWdtInit(void);
static void nvdPmSoftWdtHeartSend(OS_StdQueue_ts *astMessage_p);
static void nvdPmSoftWdtA53HeartCtrl(OS_StdQueue_ts *astMessage_p);
static void nvdPmSoftWdtA7HeartCtrl(OS_StdQueue_ts *astMessage_p);
static void nvdPmSoftWdtA53RebootCtrl(OS_StdQueue_ts *astMessage_p);
static void nvdPmSoftWdtA7RebootCtrl(OS_StdQueue_ts *astMessage_p);


/********************************************************************************************/
/*							Macro Definition Section										*/
/********************************************************************************************/
#define PMWDT_A53_HEART_FLAG	(0x01U)
#define PMWDT_A7_HEART_FLAG		(0x02U)

#define PMWDT_HEART_ERROR_MAX	(0x03U)

#define PMWDT_DATA_ZERO			(0x00U)

#define PMWDT_ARPC_ID_ANY		(0xFFU)


/********************************************************************************************/
/*							ROM Table Section												*/
/********************************************************************************************/


/********************************************************************************************/
/*							Static Variable Definition Section								*/
/********************************************************************************************/
static Type_uByte nubPmWdtSocHeartRes = 0;
static Type_uByte nubPmWdtSocHeartCntA53 = 0;
static Type_uByte nubPmWdtSocHeartCntA7 = 0;

static const PmWdtProcCtrl_ts nstPmSoftWdtProcCtrl_Tbl[] = 
{
	{ARPC_ID_A53_POWER,		EV_INF_R5_TO_A53_HEARTBEAT_RES,		nvdPmSoftWdtA53HeartCtrl},
	{ARPC_ID_A7_POWER,		EV_INF_R5_TO_A7_HEARTBEAT_RES,		nvdPmSoftWdtA7HeartCtrl},
	{ARPC_ID_A53_POWER,		EV_INF_A53_TO_R5_REBOOT_REQ,		nvdPmSoftWdtA53RebootCtrl},
	{ARPC_ID_A7_POWER,		EV_INF_A7_TO_R5_REBOOT_REQ,			nvdPmSoftWdtA7RebootCtrl},
	{ARPC_ID_R5_SOFTWDT,	EV_INF_PMWDT_CYC_TIMOUT,			nvdPmSoftWdtHeartSend},
};
/********************************************************************************************/
/********************************************************************************************/
/**	\function		TSK_POWERHARDWDT
 *	\date			2021/11/12
 *	\author			wangying
 *	\description	main task
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
void wvdOsWdtCycCtrl(void)
{
	tcc_wdt_ping();
}

/********************************************************************************************/
/**	\function		wvdPmHardWdtSend
 *	\date			2021/11/12
 *	\author			wangying
 *	\description	main task
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
void wvdPmHardWdtSend(void)
{
	OS_ErrCode_t aswRst = OS_ERR_OK;

	aswRst = OS_TimerChangePeriod(OS_TIMER_ID_OSWDTCYCHEART, 18000);
	if(aswRst != OS_ERR_OK){
		PM_DBG("########## Timer Start Failed:%d ##########\n", aswRst);
		LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]Timer Start Failed:%d",__func__, aswRst);
	}
		
}

#if 0
/********************************************************************************************/
/**	\function		nvdPmHardWdtSendMsg
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	Send Rpc Message
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
static void nvdPmHardWdtSendMsg(Type_uByte aubReceiver, Type_uHWord auhEvent, void * auvData_p, Type_uHWord auhLength)
{
	Type_sByte asbRet = RPC_NG;

	asbRet = wsbRPCSendMsg(ARPC_ID_R5_HARDWDT, aubReceiver, auhEvent, auvData_p, auhLength);
	if(RPC_OK != asbRet){
		PMWDT_DBG("\n########## PMWDT Send Rpc Failed ##########\n");
		LOG_OUTPUT(LOG_APP_ID_PMWT, DEBUGLOG_LEVEL_FALAL,ATAT_LEVEL_OFF,"[%s]Failed to send rpc:%d",__func__, asbRet);
	}
}
#endif

/********************************************************************************************/
/**	\function		wvdPmSoftWdtRcv
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	RPC Reive CallBack
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
static void nvdPmSoftWdtRpcRcv(const Type_uByte aubsrc, const Type_uHWord auhCmd, void *avdData_p, const Type_uHWord auhLength)
{
	OS_ErrCode_t auwExeRslt = OS_ERR_TMOUT;
	OS_ErrCode_t auwGetRslt = OS_ERR_NOMEM;		
	OS_VoidPtr_t avdMsgData_p = NULL;
	OS_StdQueue_ts astMsg;

	wvdGen_MemSet(&astMsg, PMWDT_DATA_ZERO, sizeof(OS_StdQueue_ts));
	astMsg.swSrcTaskId = aubsrc;
	astMsg.uhEventCode = auhCmd;
	
	if((avdData_p != NULL) && (auhLength > PMWDT_DATA_ZERO)){
		auwGetRslt = OS_GetFMemoryBlock(&avdMsgData_p, auhLength);
		if(auwGetRslt == OS_ERR_OK)
		{
			wvdGen_MemSet(avdMsgData_p, PMWDT_DATA_ZERO, auhLength);
			wvdGen_MemCpy(avdMsgData_p , avdData_p, auhLength); 	
			astMsg.vdQueueBuffer_p = avdMsgData_p ;						
			astMsg.uwQueueSize = auhLength;	
		}
		else{
			PMWDT_DBG("\n######## PMWDT Failed to Get Memory #########");
			LOG_OUTPUT(LOG_APP_ID_PMWT, DEBUGLOG_LEVEL_FALAL,ATAT_LEVEL_OFF,"[%s]Failed to get memory",__func__);
		}

	}
		
	auwExeRslt = OS_SendQueue(MBX_ID_POWERSOFTWDT, OS_MAIL_PRI_NORMAL, &astMsg); 
	if(auwExeRslt != OS_ERR_OK){
		PMWDT_DBG("\n########## PMWDT Faild to Send Queue ##########\n");
		LOG_OUTPUT(LOG_APP_ID_PMWT, DEBUGLOG_LEVEL_FALAL,ATAT_LEVEL_OFF,"[%s]Failed to send queue",__func__);
		if(NULL != astMsg.vdQueueBuffer_p){
			auwExeRslt = OS_ReleaseFMemoryBlock(astMsg.vdQueueBuffer_p);
			if(auwExeRslt != OS_ERR_OK){
				PM_DBG("\n########## PM Faild to realease memory:%d ##########\n", auwExeRslt);
				LOG_OUTPUT(LOG_APP_ID_PMWT, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]Faild to realease memory:%d",__func__, auwExeRslt);
			}
		}

	}
	
}

/********************************************************************************************/
/**	\function		TSK_POWERSOFTWDT
 *	\date			2021/11/12
 *	\author			wangying
 *	\description	main task
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
void TSK_POWERSOFTWDT(void)
{
	OS_ErrCode_t 	auwRst;
	OS_StdQueue_ts 	astMsg; 
	OS_StdQueue_ts * astMsg_p = &astMsg;
	Type_sByte 		asbRet;
	Type_uByte 		aubIndex = PMWDT_DATA_ZERO;
	Type_uByte 		aubFlag = FALSE;

	nvdPmSoftWdtInit();
	asbRet = wsbRPCInit(ARPC_ID_R5_SOFTWDT, (wvdRPCCallback)nvdPmSoftWdtRpcRcv);
	if(asbRet != RPC_OK){
		PMWDT_DBG("\n######## PMWDT RPC Init Failed:%d#######\n", asbRet);
		LOG_OUTPUT(LOG_APP_ID_PM, DEBUGLOG_LEVEL_FALAL,ATAT_LEVEL_OFF,"[%s]RPC Init Failed:%d",__func__, asbRet);
	}
	
	for( ; ; )
	{		
		auwRst = OS_ReceiveQueue(MBX_ID_POWERSOFTWDT, OS_TOUT_FOREVER, &astMsg_p);
		
		if (auwRst == OS_ERR_OK){
			for(aubIndex = PMWDT_DATA_ZERO; aubIndex < (sizeof(nstPmSoftWdtProcCtrl_Tbl)/sizeof(PmWdtProcCtrl_ts)); aubIndex++){
				if(((nstPmSoftWdtProcCtrl_Tbl[aubIndex].ubModuleID == PMWDT_ARPC_ID_ANY) || (nstPmSoftWdtProcCtrl_Tbl[aubIndex].ubModuleID == astMsg.swSrcTaskId))
					&& (nstPmSoftWdtProcCtrl_Tbl[aubIndex].uhEvent == astMsg.uhEventCode)){
						if(nstPmSoftWdtProcCtrl_Tbl[aubIndex].proc_fp != NULL){
							nstPmSoftWdtProcCtrl_Tbl[aubIndex].proc_fp(astMsg_p);
						}
						aubFlag = TRUE;
						break;
					}
			}

			if(aubFlag == FALSE){
				PMWDT_DBG("\n############# PMWDT Invalid message receive:%x-%x ########\n", astMsg.swSrcTaskId, astMsg.uhEventCode);
				LOG_OUTPUT(LOG_APP_ID_PMWT, DEBUGLOG_LEVEL_ERROR,ATAT_LEVEL_OFF,"[%s]Invalid event:%x src:%x",__func__, astMsg.uhEventCode, astMsg.swSrcTaskId);
			}
		
			if((astMsg.uwQueueSize > PMWDT_DATA_ZERO) && (NULL != astMsg.vdQueueBuffer_p)){
				auwRst = OS_ReleaseFMemoryBlock(astMsg.vdQueueBuffer_p);
				if (auwRst != OS_ERR_OK){
					PMWDT_DBG("\n########## PMWDT Failed to release memory:%d ##########\n", auwRst);
					LOG_OUTPUT(LOG_APP_ID_PMWT, DEBUGLOG_LEVEL_FALAL,ATAT_LEVEL_OFF,"[%s]Failed to release memory:%d",__func__, auwRst);
				}
			}
		}
	}
 
}


/********************************************************************************************/
/**	\function		wubPmSoftWdtStandbyOut
 *	\date			2021/11/12
 *	\author			wangying
 *	\description	StandbyOut
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
Type_uByte wubPmSoftWdtStandbyOut(void)
{
	OS_ErrCode_t aswRet = OS_ERR_OK;

	nubPmWdtSocHeartRes = PMWDT_DATA_ZERO;
	nubPmWdtSocHeartCntA53 = PMWDT_DATA_ZERO;
	nubPmWdtSocHeartCntA7 = PMWDT_DATA_ZERO;

	/* Start Heart CYC Timer */
	aswRet = OS_StartTimer(OS_TIMER_ID_PMWDTCYCHEART);
	if(aswRet != OS_ERR_OK){
		PMWDT_DBG("\n########## Timer Start Failed:%d ##########\n", aswRet);
		LOG_OUTPUT(LOG_APP_ID_PMWT, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]Timer Start Failed:%d",__func__, aswRet);
	}

	return RES_OK;
}

/********************************************************************************************/
/**	\function		wubPmSoftWdtStandbyIn
 *	\date			2021/11/12
 *	\author			wangying
 *	\description	StandbyIn
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
Type_uByte wubPmSoftWdtStandbyIn(void)
{
	OS_ErrCode_t aswRet = OS_ERR_OK;
	
	/* Stop Heart CYC Timer */
	aswRet = OS_StopTimer(OS_TIMER_ID_PMWDTCYCHEART);
	if(aswRet != OS_ERR_OK){
		PMWDT_DBG("\n########## Timer Stop Failed:%d ##########\n", aswRet);
		LOG_OUTPUT(LOG_APP_ID_PMWT, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]Timer Stop Failed:%d",__func__, aswRet);
	}

	return RES_OK;
}

/********************************************************************************************/
/**	\function		wubPmSoftWdtForceStandby
 *	\date			2021/11/12
 *	\author			wangying
 *	\description	Force Standby
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
Type_uByte wubPmSoftWdtForceStandby(void)
{
	OS_ErrCode_t aswRet = OS_ERR_OK;
	
	/* Stop Heart CYC Timer */
	aswRet = OS_StopTimer(OS_TIMER_ID_PMWDTCYCHEART);
	if(aswRet != OS_ERR_OK){
		PMWDT_DBG("\n########## Timer Stop Failed:%d ##########\n", aswRet);
		//LOG_OUTPUT(LOG_APP_ID_PMWT, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]Timer Stop Failed:%d",__func__, aswRet);
	}

	return RES_OK;
}

/********************************************************************************************/
/**	\function		wubPmSoftWdtForceStandby
 *	\date			2021/11/12
 *	\author			wangying
 *	\description	CycTimeout
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
void wvdPmSoftWdtCycCtrl(void)
{
	nvdPmSoftWdtSendMsg(ARPC_ID_R5_SOFTWDT, EV_INF_PMWDT_CYC_TIMOUT, NULL, PMWDT_DATA_ZERO);
}

/********************************************************************************************/
/**	\function		nvdPmSoftWdtA53HeartCtrl
 *	\date			2021/11/12
 *	\author			wangying
 *	\description	CycTimeout
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
static void nvdPmSoftWdtA53HeartCtrl(OS_StdQueue_ts *astMessage_p)
{
	nvdPmSoftWdtHeartResCtrl(PMWDT_A53_HEART_FLAG);
}

/********************************************************************************************/
/**	\function		nvdPmSoftWdtA7HeartCtrl
 *	\date			2021/11/12
 *	\author			wangying
 *	\description	CycTimeout
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
static void nvdPmSoftWdtA7HeartCtrl(OS_StdQueue_ts *astMessage_p)
{
	nvdPmSoftWdtHeartResCtrl(PMWDT_A7_HEART_FLAG);
}

/********************************************************************************************/
/**	\function		nvdPmSoftWdtA53RebootCtrl
 *	\date			2021/11/12
 *	\author			wangying
 *	\description	CycTimeout
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
static void nvdPmSoftWdtA53RebootCtrl(OS_StdQueue_ts *astMessage_p)
{
	PMWDT_DBG("\n########## PMWDT A53 Reboot Received:%x ##########\n", astMessage_p->uhEventCode);
	LOG_OUTPUT(LOG_APP_ID_PMWT, DEBUGLOG_LEVEL_FALAL,ATAT_LEVEL_OFF,"[%s]A53 Reboot Received:%x",__func__, astMessage_p->uhEventCode);
	nvdPmSoftWdtSendMsg(ARPC_ID_R5_POWERMNG, EV_INF_PM_A53HEAT_REBOOT_REQ, NULL, PMWDT_DATA_ZERO);

}

/********************************************************************************************/
/**	\function		nvdPmSoftWdtA7RebootCtrl
 *	\date			2021/11/12
 *	\author			wangying
 *	\description	CycTimeout
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
static void nvdPmSoftWdtA7RebootCtrl(OS_StdQueue_ts *astMessage_p)
{
	PMWDT_DBG("\n########## PMWDT A7 Reboot Received:%x ##########\n", astMessage_p->uhEventCode);
	LOG_OUTPUT(LOG_APP_ID_PMWT, DEBUGLOG_LEVEL_FALAL,ATAT_LEVEL_OFF,"[%s]A7 Reboot Received:%x",__func__, astMessage_p->uhEventCode);
	nvdPmSoftWdtSendMsg(ARPC_ID_R5_POWERMNG, EV_INF_PM_A7HEART_REBOOT_REQ, NULL, PMWDT_DATA_ZERO);
}

/********************************************************************************************/
/**	\function		nvdPmSoftWdtHeartSend
 *	\date			2021/11/12
 *	\author			wangying
 *	\description	CycTimeout
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
static void nvdPmSoftWdtHeartSend(OS_StdQueue_ts *astMessage_p)
{
	OS_ErrCode_t aswRet = OS_ERR_OK;

	if((nubPmWdtSocHeartRes & PMWDT_A53_HEART_FLAG) !=  PMWDT_DATA_ZERO){
		nubPmWdtSocHeartCntA53++;
	}
	else{
		nubPmWdtSocHeartCntA53 = PMWDT_DATA_ZERO;
	}

	if((nubPmWdtSocHeartRes & PMWDT_A7_HEART_FLAG) !=  PMWDT_DATA_ZERO){
		nubPmWdtSocHeartCntA7++;
	}
	else{
		nubPmWdtSocHeartCntA7 = PMWDT_DATA_ZERO;
	}

	if((nubPmWdtSocHeartCntA53 < PMWDT_HEART_ERROR_MAX) && (nubPmWdtSocHeartCntA7 < PMWDT_HEART_ERROR_MAX)){
		nvdPmSoftWdtSendMsg(ARPC_ID_A53_POWER, EV_INF_R5_TO_A53_HEARTBEAT_CMD, NULL, PMWDT_DATA_ZERO);
		nubPmWdtSocHeartRes |= PMWDT_A53_HEART_FLAG;
		
		nvdPmSoftWdtSendMsg(ARPC_ID_A7_POWER, EV_INF_R5_TO_A7_HEARTBEAT_CMD, NULL, PMWDT_DATA_ZERO);
		nubPmWdtSocHeartRes |= PMWDT_A7_HEART_FLAG;
	}
	else{
		PMWDT_DBG("\n########WatchDog Heart Send Reset Msg A53=%d, A7 = %d ######\n", nubPmWdtSocHeartCntA53, nubPmWdtSocHeartCntA7);
		aswRet = OS_StopTimer(OS_TIMER_ID_PMWDTCYCHEART);
		if(aswRet != OS_ERR_OK){
			PMWDT_DBG("\n########## Timer Stop Failed:%d ##########\n", aswRet);
			LOG_OUTPUT(LOG_APP_ID_PMWT, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]Timer Stop Failed:%d",__func__, aswRet);
		}

		if(nubPmWdtSocHeartCntA53 >= PMWDT_HEART_ERROR_MAX){
			nvdPmSoftWdtSendMsg(ARPC_ID_R5_POWERMNG, EV_INF_PM_A53HEART_TIMOUT_REBOOT, NULL, PMWDT_DATA_ZERO);
			LOG_OUTPUT(LOG_APP_ID_PMWT, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]A53 Heart Timout",__func__);
		}
		else{
			nvdPmSoftWdtSendMsg(ARPC_ID_R5_POWERMNG, EV_INF_PM_A7HEART_TIMOUT_REBOOT, NULL, PMWDT_DATA_ZERO);
			LOG_OUTPUT(LOG_APP_ID_PMWT, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]A7 Heart Timout",__func__);
		}
	}

}

/********************************************************************************************/
/**	\function		nvdPmSoftWdtInit
 *	\date			2021/11/12
 *	\author			wangying
 *	\description	CycTimeout
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
static void nvdPmSoftWdtInit(void)
{
	nubPmWdtSocHeartRes = PMWDT_DATA_ZERO;
	nubPmWdtSocHeartCntA53 = PMWDT_DATA_ZERO;
	nubPmWdtSocHeartCntA7 = PMWDT_DATA_ZERO;
}

/********************************************************************************************/
/**	\function		nvdPmSoftWdtHeartResCtrl
 *	\date			2021/11/12
 *	\author			wangying
 *	\description	heart response ctrl
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
static void nvdPmSoftWdtHeartResCtrl(Type_uByte aubRes)
{
	nubPmWdtSocHeartRes &= (~aubRes);
}

/********************************************************************************************/
/**	\function		nvdPmDrvSendMsg
 *	\date			2021/11/11
 *	\author			wangying
 *	\description	Send Message
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
static void nvdPmSoftWdtSendMsg(Type_uByte aubReceiver, Type_uHWord auhEvent, void * auvData_p, Type_uHWord auhLength)
{
	Type_sByte asbRet = RPC_NG;

	asbRet = wsbRPCSendMsg(ARPC_ID_R5_SOFTWDT, aubReceiver, auhEvent, auvData_p, auhLength);
	if(RPC_OK != asbRet){
		PMWDT_DBG("\n########## PMWDT Send Rpc Failed %d ##########\n",asbRet);
		PMWDT_DBG("\n########## nvdPmSoftWdtSendMsg Rece %x  Ev %x##########\n",aubReceiver,auhEvent);
		LOG_OUTPUT(LOG_APP_ID_PMWT, DEBUGLOG_LEVEL_FALAL, ATAT_LEVEL_OFF,"[%s]Failed to send rpc:%d",__func__, asbRet);
	}

}


