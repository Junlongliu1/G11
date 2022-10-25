#include "rpc_if.h"
#include "rpc_os.h"
#include <app_cfg.h>
#include "apn_basic_type.h"
#include "tcc_ipc_protocol.h"

#include "HAL_IPC.h"
#include "HAL_devices.h"
#include "HAL_IPC_CA53.h"
//#include "debug.h"


#include "tcc_ipc_typedef.h"

#include "os_framework_type.h"
#include "os_framework_cfg.h"

Type_sByte wubRPCSendMsg(Type_uByte sender, Type_uByte receiver, Type_uHWord event, void* data, Type_uHWord length)
{
	Type_sWord aswret = IPC_ERR_COMMON;
	Type_uHWord auhcmd1;

	if (length >= (IPC_MAX_PACKET_SIZE - IPC_PACKET_PREFIX_SIZE)) 
	{
		aswret = IPC_ERR_ARGUMENT;
	}
	else 
	{
		auhcmd1 = ((Type_uHWord)ENCODE16(receiver)) |(Type_uHWord)(ENCODE8(sender));
		if(receiver > ARPC_ID_A7_MIN && receiver < ARPC_ID_A7_MAX)
		{
			aswret = wswipc_send_packet(IPC_SVC_CH_SUB, auhcmd1, event, data, length);
		}
		else if(receiver > ARPC_ID_A53_MIN && receiver < ARPC_ID_A53_MAX)
		{
			aswret = wswipc_send_packet(IPC_SVC_CH_MAIN, auhcmd1, event, data, length);
		}
		else if(receiver > ARPC_ID_R5_MIN && receiver < ARPC_ID_R5_MAX)
		{
			aswret = wubRPCToMoudlue1(sender, receiver, event, data, length);
		}
		else
		{
			aswret = IPC_ERR_COMMON;
		}

	}

	return aswret;
}
Type_sByte wswRPCInit(const Type_uByte srcId, ipc_call_back func)
{
    Type_sByte ret = RPC_NOEXSIT;
	Type_uByte SrcId = SRCID_INIT;

    SrcId = srcId - R5_MOULDE_START;
    wvdipc_register_cb_func(IPC_SVC_CH_SUB, SrcId, (ipc_call_back)func, NULL, NULL);
    wvdipc_register_cb_func(IPC_SVC_CH_MAIN, SrcId, (ipc_call_back)func, NULL, NULL);
   	return ret;
}

Type_sByte wsbRPCSendMsg(Type_uByte aubsrc, Type_uByte aubdest, Type_uHWord auhevent, void* data, Type_uHWord auhlength)
{
	Type_sByte aswRet = RPC_NG;
	Type_uHWord auhCmd;



	if (auhlength >= (IPC_MAX_PACKET_SIZE - IPC_PACKET_PREFIX_SIZE)) 
	{
		aswRet = RPC_ARGUMENT;
	}
	else 
	{

		DEBUG_LIBARPC_PRINTF("aubsrc:%x aubdest:%x auhevent:%x auhlength:%d\n",aubsrc, aubdest, auhevent, auhlength);
		auhCmd = ((Type_uHWord)ENCODE16(aubdest)) | (Type_uHWord)(ENCODE8(aubsrc));
		if(aubdest > ARPC_ID_A7_MIN && aubdest < ARPC_ID_A7_MAX)  
		{
			aswRet = wswipc_send_packet(IPC_SVC_CH_SUB, auhCmd, auhevent, data, auhlength);
		}
		else if(aubdest > ARPC_ID_A53_MIN && aubdest < ARPC_ID_A53_MAX) 
		{
			aswRet = wswipc_send_packet(IPC_SVC_CH_MAIN, auhCmd, auhevent, data, auhlength);
		}
		else if(aubdest > ARPC_ID_R5_MIN && aubdest < ARPC_ID_R5_MAX) 
		{
			aswRet = wubRPCToMoudlue(aubsrc, aubdest, auhevent, data, auhlength);
		}
		else
		{
			aswRet = RPC_NG;
			printf("Dest ID Not Range[%s]: <%s:%d>\n", __func__, __FILE__, __LINE__);
		}
		if(aswRet == IPC_SUCCESS)
		{
			aswRet = RPC_OK;
		}
		else if(aswRet == IPC_ERR_BUSY)
		{
			aswRet = RPC_BUSY;
		}
		else
		{
			aswRet = RPC_NG;
		}
			

		
	}

	return aswRet;
}

Type_sByte wsbRPCInit(const Type_uByte aubsrcId, wvdRPCCallback func)
{
    Type_sByte asbret = RPC_NG;
	Type_uByte aubsrc;
	//OS_TskId_t auwCurrentTskId;
	
    aubsrc = aubsrcId - R5_MOULDE_START;
	//OS_GetTaskId(&auwCurrentTskId);
	asbret = wsbIPCRegisterFunc(aubsrc, (wvdRPCCallback)func);
   	return asbret;
}

Type_uByte wubRPC_Init(void)
{
	Type_uByte aubReturn;

	wubRPCStandbyFlag = RPC_STANDBYIN;
	aubReturn = RES_OK;

	return aubReturn;
}

Type_uByte wubRPC_Standbyout(void)
{
	Type_uByte aubReturn;
	aubReturn = RES_NG;
	wubRPCStandbyFlag = RPC_STANDBYOUT;
	aubReturn = nswRPC_SendQueue(MBX_ID_RPC, RPC_STANDBYOUT, OS_MAIL_PRI_NORMAL);
	return aubReturn;

}

Type_uByte wubRPC_Standbyin(void)
{
	Type_uByte aubReturn;
	
	aubReturn = RES_NG;
	wubRPCStandbyFlag = RPC_STANDBYIN;
	aubReturn = nswRPC_SendQueue(MBX_ID_RPC, RPC_STANDBYIN, OS_MAIL_PRI_NORMAL);
	return aubReturn;
	
}

Type_sWord nswRPC_SendQueue(OS_QueueId_t auwMbxId,
						   const Type_uHWord auhEventCode,
						   const OS_ObjPri_t aubPriority)
{
	Type_sWord			aubReturnCode;					/* Return Code						*/
	OS_StdQueue_ts		*astMemAddr_p;					/* Mail Buffer						*/
	OS_StdQueue_ts		astMemAddr;					/* Mail Buffer						*/
	OS_VoidPtr_t		avdMemAddr_p;					/* void Pointer Address				*/
	OS_ErrCode_t		aswOSErrorCode;					/* OS Error code					*/

	avdMemAddr_p = &astMemAddr;
	wvdGen_MemSet(&astMemAddr, RPC_CLEAR_UB , sizeof(astMemAddr));

	astMemAddr_p				 = (OS_StdQueue_ts *)avdMemAddr_p;
	astMemAddr_p->swSrcTaskId	 = ARPC_ID_R5_RPC;
	astMemAddr_p->uhEventCode	 = auhEventCode;
	astMemAddr_p->vdQueueBuffer_p = STD_NULL;
	astMemAddr_p->uwQueueSize	 = STD_NULL;

	/* Send Mail	*/
	aswOSErrorCode	= OS_SendQueue(auwMbxId, aubPriority, astMemAddr_p);
	if (OS_ERR_OK == aswOSErrorCode){
	aubReturnCode = RES_OK;
	}
	else{
	/* Send Mail Failed */
	aubReturnCode = RES_NG;
	}

	
	return aubReturnCode;
}

