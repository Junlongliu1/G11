/*******************************************************************************************/
/**<!-------Copyright (C) 2021 Alpine Electronics, Inc. All rights reserved. --------------->
 * 
 * \file					Agent_Core.c
 * \date					2021/11/01
 * \author					zhangbo
 * \model					EDA
 * \description				Agent Control Routine
 * \version					0.55
 *
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/11/01		zhangbo		New
 ********************************************************************************************/


/********************************************************************************************/
/*							Include File Section											*/
/********************************************************************************************/
/* System header files */
#include "os_framework.h"

/* Agent module header files */
#include "agent_core.h"
#include "Eepromcontrol.h"

/* Other modules header files */
#include <HAL_API.h>
#include "backup_ram.h"
#include "rpc_event.h"
#include "rpc_if.h"
#include "logapp.h"

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
Type_uByte wubAgentStandbyFlag;

/********************************************************************************************/
/*							Static Variable Definition Section								*/
/********************************************************************************************/

/********************************************************************************************/
/*							Prototype Declaration Section									*/
/********************************************************************************************/

/********************************************************************************************/
/*							Table Include Section											*/
/********************************************************************************************/
//TODO:del ram, add rom 
static const AgentDataSave_st nstAgentDataSave_T[] = 
{ 
	/* PropertyID					uhStartPos					ubWriteType				DataSize	*/
	{EVT_CARPLAYMAC_REQ,			EEP_ADDR_CARPLAY,				AGENT_DATA_ROM,		EEP_CARPLAY_SIZE},
	{EVT_DANUMBER_REQ,				EEP_ADDR_PART,					AGENT_DATA_ROM,		EEP_PART_SIZE},
	{EVT_SERIALNUM_REQ,			EEP_ADDR_SERIAL,				AGENT_DATA_ROM,		EEP_SERIAL_SIZE},
	{EVT_MANUFACTURE_DATA_REQ,				EEP_ADDR_DATE,					AGENT_DATA_ROM,		EEP_DATE_SIZE},
	{EVT_CRYSTAL_OSCILLATOR_REQ,				EEP_ADDR_OSC,					AGENT_DATA_ROM,		EEP_OSC_SIZE},
	{EVT_MOUDLE_DIFFERENCE_REQ,			EEP_ADDR_MODEL,					AGENT_DATA_ROM,		EEP_MODEL_SIZE},
	{EVT_RPOJECT_REQ,			EEP_ADDR_TRANSIT,				AGENT_DATA_ROM,		EEP_TRANSIT_SIZE},
	{EVT_TRACEABILITY_REQ,		EEP_ADDR_TRACEABILITY,			AGENT_DATA_ROM,		EEP_TRACEABILITY_SIZE},
	{EVT_SELF_AGING_REQ,				EEP_ADDR_SELF,					AGENT_DATA_ROM,		EEP_SELF_SIZE},
	{EVT_HAC_AO_PN_REQ,				EEP_ADDR_HAC,					AGENT_DATA_ROM,		EEP_HAC_SIZE},
	/* usb*/
	{EVT_USB_MUSIC_REQ,				BACKUP_ADDR_USB,				AGENT_DATA_RAM,		BACKUP_RAM_USB_SIZE}
};
/********************************************************************************************/
/*							Function Definition Section										*/
/********************************************************************************************/
static void nvdAgentDataProc(const OS_StdQueue_ts* stMail_p);
static void nvdAgentDataWrite(const Type_uByte* ubData_p);
static void nvdAgentDataRead(const Type_uByte* ubData_p);
static void wvdAgent_Callback (const Type_uByte aubsrcId, const Type_uWord auhEventCode, void* avdMailBuffer_p, const Type_uWord aubMailSize);

static Type_uByte 	nubDataBuff_a[AGENT_DATA_SIZE_MAX];

/********************************************************************************************/
/* Function Name	: TSK_AGENT																*/
/* Date				: 																		*/
/* Author			: 																		*/
/* Description		: 																		*/
/* Argument Code	:																		*/
/* Return Code		:																		*/
/*------------------------------------------------------------------------------------------*/
/* Revision History																			*/
/* No		Date		Revised by		Explanation											*/
/*======================================================================*/
/* 																							*/
/********************************************************************************************/
void TSK_Agent(void)
{
	OS_StdQueue_ts astStdQueue_st;
	OS_StdQueue_ts	* astQueue_p;
	OS_ErrCode_t aswQueueRet;

	OSAL_MemSet(&astStdQueue_st, AGENT_VALUE_0, sizeof(OS_StdQueue_ts));
	
	astQueue_p = &astStdQueue_st;

	(void)wsbRPCInit(ARPC_ID_R5_AGENT,(wvdRPCCallback)wvdAgent_Callback);
	
	while(1){
		aswQueueRet = OS_ReceiveQueue(MBX_ID_AGENT, OS_TOUT_FOREVER, &astQueue_p);
		switch (aswQueueRet){
			case OS_ERR_OK: 
				if(AGENT_STANDBYOUT == wubAgentStandbyFlag){
					nvdAgentDataProc(astQueue_p);
				}else{
					LOG_OUTPUT( LOG_APP_ID_AGE, DEBUGLOG_LEVEL_ERROR,ATAT_LEVEL_OFF," NO AGENT_STANDBYOUT ");
				}
				if(astQueue_p->vdQueueBuffer_p != NULL){
					/* Memory Release */
					(void)OS_ReleaseFMemoryBlock(astQueue_p->vdQueueBuffer_p);
				}
				break;
			case OS_ERR_TMOUT:
				break;
			case OS_ERR_NG:
				break;
			default:
				break;
		}
	}
}

/********************************************************************************************/
/* Function Name	: nvdAgentDataProc														*/
/* Date				: 																		*/
/* Author			: 																		*/
/* Description		: 																		*/
/* Argument Code	:																		*/
/* Return Code		:																		*/
/*------------------------------------------------------------------------------------------*/
/* Revision History																			*/
/* No		Date		Revised by		Explanation											*/
/*==========================================================================================*/
/* 																							*/
/********************************************************************************************/
static void nvdAgentDataProc(const OS_StdQueue_ts* stMail_p)
{
	Type_uHWord	auhReqCode;
	Type_uByte		*aubMLBuf_p;
	Type_uByte aubRet = RES_OK;

	auhReqCode = stMail_p->uhEventCode;
	aubMLBuf_p = stMail_p->vdQueueBuffer_p;
	AGENT_D("\n Write or Read TYPE %x",auhReqCode);

	switch(auhReqCode)
	{	
		case EVENT_CODE_REQ_DATA_WRITE:
			nvdAgentDataWrite(aubMLBuf_p);
			break;
		case EVENT_CODE_REQ_DATA_READ:
			nvdAgentDataRead(aubMLBuf_p);
			break;
		case DIAGAPP_EV_ALL_CLEAR_NOTIFY:
			aubRet = wubBackup_AllClear();
			wsbRPCSendMsg(ARPC_ID_R5_AGENT, ARPC_ID_DIAGAPP, DIAGAPP_EV_ALL_CLEAR_CB, &aubRet, sizeof(aubRet));
			AGENT_D("\n DIAGAPP_EV_ALL_CLEAR_NOTIFY %d",aubRet);
			break;
		case DIAGAPP_EV_FACTORY_RESET_NOTIFY:
			aubRet = wubBackup_AllClear();
			wsbRPCSendMsg(ARPC_ID_R5_AGENT, ARPC_ID_DIAGAPP, DIAGAPP_EV_FACTORY_RESET_CB, &aubRet, sizeof(aubRet));
			AGENT_D("\n DIAGAPP_EV_FACTORY_RESET_NOTIFY %d",aubRet);
			break;
		case DIAGAPP_EV_UPDATE_CLEAR_NOTIFY:
			aubRet = wubBackup_AllClear();
			wsbRPCSendMsg(ARPC_ID_R5_AGENT, ARPC_ID_DIAGAPP, DIAGAPP_EV_UPDATE_CLEAR_CB, &aubRet, sizeof(aubRet));
			AGENT_D("\n DIAGAPP_EV_UPDATE_CLEAR_NOTIFY %d",aubRet);
			break;
		default:
			//AGENT_E("Unknown PROPID=0x%04X", auhReqCode);
			break;
	}

}

/********************************************************************************************/
/* Function Name	: nvdAgentDataSave														*/
/* Date			: 																			*/
/* Author		: 																			*/
/* Description		: 																		*/
/* Argument Code	: 																		*/
/* Return Code	: 																			*/
/*------------------------------------------------------------------------------------------*/
/* Revision History																			*/
/* No		Date		Revised by		Explanation											*/
/*==========================================================================================*/
/* 																							*/
/********************************************************************************************/
static void nvdAgentDataWrite(const Type_uByte* ubData_p)
{
	Type_uByte	aubPropIndex;
	Type_uHWord	auhDataAddr;
	Type_uHWord	auhDataLength;
	
	Type_uByte  aubRet;	
	Type_uByte 	aubRetry;
	Type_uByte 	aubBusyRetry;
	Type_sByte 	asbReturn;
	
	AgentDataWriteReq_st * astWriteReq_p;
	AgentDataWriteRes_st astWriteRes;

	aubRet = RES_NG;
	asbReturn = RPC_OK;
	aubPropIndex = AGENT_CLEAR_UB;
	aubRetry = AGENT_CLEAR_UB;
	aubBusyRetry = AGENT_CLEAR_UB;
	auhDataAddr = AGENT_CLEAR_UB;
	auhDataLength = AGENT_CLEAR_UB;
	wvdGen_MemSet(&nubDataBuff_a[AGENT_VALUE_0], AGENT_CLEAR_UB , sizeof(nubDataBuff_a));

	if(STD_NULL != ubData_p){
		astWriteReq_p = (AgentDataWriteReq_st*)ubData_p;
		auhDataAddr = astWriteReq_p->uhStartPos;
		auhDataLength = astWriteReq_p->uhChangeSize;
		
		if(AGENT_DATA_SIZE_MAX >= auhDataLength){
			wvdGen_MemCpy(&nubDataBuff_a[AGENT_VALUE_0], &(astWriteReq_p->ubData_a[AGENT_VALUE_0]), auhDataLength);
			//   Data Write
			for(aubPropIndex = AGENT_VALUE_0; aubPropIndex < sizeof(nstAgentDataSave_T)/sizeof(AgentDataSave_st); aubPropIndex++){
				
				if (astWriteReq_p->uhReqMoudelEvt == nstAgentDataSave_T[aubPropIndex].uhReqMoudelEvt){
					do{
						if (AGENT_DATA_ROM == (nstAgentDataSave_T[aubPropIndex].ubWriteType)){
							// EEP Write
							aubRet = wubEEP_BlockWrite((nstAgentDataSave_T[aubPropIndex].uhStartPos + auhDataAddr),auhDataLength,&nubDataBuff_a[AGENT_INDEX_0]);
						}
						else{
							//BackupRam Write Not supported
							aubRet = wubBackup_WriteData((nstAgentDataSave_T[aubPropIndex].uhStartPos + auhDataAddr),auhDataLength,&nubDataBuff_a[AGENT_INDEX_0]);
						}
						AGENT_D("\n Write Type %x",nstAgentDataSave_T[aubPropIndex].ubWriteType);
						if( aubRet != RES_OK ){
							aubBusyRetry++;
							OS_SleepTask(1);
						}
					}while((aubBusyRetry < AGENT_RETRY_BUSY_CONT)&&(aubRet != RES_OK));
						
					break;
				}
			}
		}else{
			AGENT_D("\n  DataWrite Size over %d",auhDataLength);
		}
		astWriteRes.ubPropertyID	= astWriteReq_p->ubPropertyID;
		astWriteRes.uhReqMoudelEvt	= astWriteReq_p->uhReqMoudelEvt;
		astWriteRes.ubResult		= aubRet;
		astWriteRes.uhStartPos		= astWriteReq_p->uhStartPos;
		astWriteRes.uhSize			= astWriteReq_p->uhChangeSize;
		
		wvdGen_MemCpy(&astWriteRes.ubData_a[AGENT_VALUE_0],&astWriteReq_p[AGENT_VALUE_0], astWriteReq_p->uhChangeSize);
		
		do{
			asbReturn = wsbRPCSendMsg(ARPC_ID_R5_AGENT, ARPC_ID_A53_AGENT, EVENT_CODE_RES_DATA_WRITE, &astWriteRes,(sizeof(AgentDataWriteRes_st) - AGENT_DATA_SIZE_MAX + astWriteRes.uhSize));
			aubRetry++;
		}while((aubRetry < AGENT_RETRY_CONT) && (asbReturn != RES_OK));
		AGENT_D("\n @@@@@ WriteData Ret=%d",aubRet);
	}else{
		AGENT_D("\n ubData_p is NULL");
	}

}
/********************************************************************************************/
/* Function Name	: nvdAgentDataRead														*/
/* Date			: 																			*/
/* Author		: 																			*/
/* Description		: 																		*/
/* Argument Code	: 																		*/
/* Return Code	: 																			*/
/*------------------------------------------------------------------------------------------*/
/* Revision History																			*/
/* No		Date		Revised by		Explanation											*/
/*==========================================================================================*/
/* 																							*/
/********************************************************************************************/
static void nvdAgentDataRead(const Type_uByte* ubData_p)
{
	Type_uByte	aubRet;
	Type_uByte 	aubRetry;
	Type_uByte 	aubBusyRetry;
	Type_sByte	asbReturn;
	Type_uByte	aubPropIndex;
	Type_uHWord  auhStartPos;
	Type_uHWord	 auhDataLength;

	AgentDataReadReq_st * astReadReq_p;
	AgentDataReadRes_st astReadRes;
	
	aubRet = RES_NG;
	asbReturn = RPC_OK;
	aubRetry = AGENT_CLEAR_UB;
	aubBusyRetry = AGENT_CLEAR_UB;
	aubPropIndex = AGENT_CLEAR_UB;
	auhStartPos = AGENT_CLEAR_UB;
	auhDataLength = AGENT_CLEAR_UB;

	if(STD_NULL != ubData_p){
		astReadReq_p = (AgentDataReadReq_st*)ubData_p;
		wvdGen_MemSet(nubDataBuff_a, AGENT_CLEAR_UB, sizeof(nubDataBuff_a));
		AGENT_D("\n  astReadReq_p->uhReqMoudelEvt %x",astReadReq_p->uhReqMoudelEvt);

		for(aubPropIndex = AGENT_VALUE_0; aubPropIndex < sizeof(nstAgentDataSave_T)/sizeof(AgentDataSave_st); aubPropIndex++){
			if (astReadReq_p->uhReqMoudelEvt == nstAgentDataSave_T[aubPropIndex].uhReqMoudelEvt){
				auhStartPos = nstAgentDataSave_T[aubPropIndex].uhStartPos;
				auhDataLength = nstAgentDataSave_T[aubPropIndex].uhDataSize;
				do{
					if (AGENT_DATA_ROM == (nstAgentDataSave_T[aubPropIndex].ubWriteType)){
						//Read EEP
						aubRet = wubEEP_BlockRead(auhStartPos,auhDataLength,&nubDataBuff_a[AGENT_VALUE_0]);
					}
					else{
						//Read BackupRam Not supported
						aubRet = wubBackup_ReadData(auhStartPos,auhDataLength,&(nubDataBuff_a[AGENT_VALUE_0]));
					}
					AGENT_D("\n Read Type %x",nstAgentDataSave_T[aubPropIndex].ubWriteType);
					
					if( aubRet != RES_OK ){
						aubBusyRetry++;
						OS_SleepTask(1);
					}
				}while((aubBusyRetry < AGENT_RETRY_BUSY_CONT)&&(aubRet != RES_OK));

				astReadRes.ubPropertyID = astReadReq_p->ubPropertyID;
				astReadRes.uhReqMoudelEvt = astReadReq_p->uhReqMoudelEvt;
				astReadRes.ubResult	= aubRet;
				astReadRes.uhStartPos = astReadReq_p->uhStartPos;
				astReadRes.uhDataSize = astReadReq_p->uhDataSize;
				wvdGen_MemCpy(&astReadRes.ubData_a[AGENT_VALUE_0],&nubDataBuff_a[AGENT_VALUE_0], astReadReq_p->uhDataSize);
				
				do{
					asbReturn = wsbRPCSendMsg(ARPC_ID_R5_AGENT, ARPC_ID_A53_AGENT, EVENT_CODE_RES_DATA_READ, &astReadRes,(sizeof(AgentDataReadRes_st) - AGENT_DATA_SIZE_MAX + astReadRes.uhDataSize ));
					aubRetry++;
				}while((aubRetry < AGENT_RETRY_CONT)&&(asbReturn != RPC_OK));
				AGENT_D("\n @@@@@ ReadData Ret=%d",aubRet);
				break;
			}
		}
	}else{
		AGENT_D("\n ubData_p is NULL");
	}
}

/********************************************************************************************/
/* Function Name	: wvdAgent_Callback														*/
/* Date				: 																		*/
/* Author			: 																		*/
/* Description		: (API) 																*/
/* Argument Code	: 																		*/
/* Return Code		: NONE																	*/
/*------------------------------------------------------------------------------------------*/
/* Revision History																			*/
/* No		Date		Revised by		Explanation											*/
/*==========================================================================================*/
/********************************************************************************************/
void wvdAgent_Callback(const Type_uByte aubsrcId, const Type_uWord auhEventCode, void* avdMailBuffer_p, const Type_uWord auhMailSize)
{
	Type_uByte		aubReturnCode	= RES_NG;
	OS_ErrCode_t	aswErrCode		= OS_ERR_OK;
	OS_StdQueue_ts	astStdMail;
	OS_VoidPtr_t	avdMpf_p		= STD_NULL;
	
	/* Variable Initialize */
	wvdGen_MemSet(&astStdMail, AGENT_INDEX_0, sizeof(astStdMail));
	
	astStdMail.swSrcTaskId = aubsrcId;
	astStdMail.uhEventCode = auhEventCode;
	if((avdMailBuffer_p != STD_NULL) && (auhMailSize != AGENT_INDEX_0)) 
	{
		aswErrCode = OS_GetFMemoryBlock(&avdMpf_p, auhMailSize);
		/* Get Memory Block Result Check */
		if (OS_ERR_OK == aswErrCode) 
		{
			wvdGen_MemCpy(avdMpf_p, avdMailBuffer_p, auhMailSize);
			astStdMail.vdQueueBuffer_p = avdMpf_p;
			astStdMail.uwQueueSize = auhMailSize;
			aubReturnCode = RES_OK;
		} 
		else 
		{
			LOG_OUTPUT( LOG_APP_ID_AGE, DEBUGLOG_LEVEL_ERROR,ATAT_LEVEL_OFF,"\n Agent NO SPACE Err");
		}
	} 
	else 
	{
		astStdMail.vdQueueBuffer_p = STD_NULL;
		astStdMail.uwQueueSize = AGENT_INDEX_0;
		aubReturnCode = RES_OK;
	}

	if (aubReturnCode == RES_OK) 
	{
		aswErrCode = OS_SendQueue(MBX_ID_AGENT, OS_MAIL_PRI_NORMAL, &astStdMail);
		/* Send Mail Result Check */
		if (OS_ERR_OK == aswErrCode) 
		{
			/* No Action */
			;
		} 
		else 
		{
			/* Get Memory Block Check */
			if (avdMpf_p != STD_NULL) 
			{
				(void)OS_ReleaseFMemoryBlock(avdMpf_p);
			} 
			else 
			{
				/* No Action */
				;
			}
			aubReturnCode = RES_NG;
			LOG_OUTPUT( LOG_APP_ID_AGE, DEBUGLOG_LEVEL_ERROR,ATAT_LEVEL_OFF,"\n Agent OS_SendQueue Err");
		}
	}
	return;
}

