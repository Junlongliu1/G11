#include "rpc_os.h"
#include "rpc_if.h"
#include "os_framework_type.h"
#include "IPC_Parser.h"
#include "tcc_ipc_os.h"
#include "tcc_ipc.h"
#include "tcc_ipc_ctl.h"
#include "HAL_IPC_CA53.h"
#include "HAL_IPC_CA7.h"
#include "tcc_ipc_protocol.h"
//#include "debug.h"

Type_uByte wubRPCStandbyFlag;
Type_sWord wswg_arpc_ctrl_debug = 0;

void rpc_callback(Type_uByte src, Type_uHWord event, void* data, Type_uHWord len)
{
    //printf("[ARPC]: arpc callback.");
	if(src == (ARPC_ID_A53_MAX -1))
	{
		printf("zjqrpccallback############event=%x\n", event);
		wswg_arpc_ctrl_debug = (Type_sWord)event;
	}

}



void TSK_RPC(void)
{
	OS_ErrCode_t aswQueueRet;
	OS_StdQueue_ts astMessage;
	OS_StdQueue_ts* astMessage_p;

	wvdGen_MemSet(&astMessage, VALUE_INIT, sizeof(astMessage)); 
	astMessage_p = &astMessage;

	(void)wsbRPCInit(ARPC_ID_R5_RPC, rpc_callback);
	while(1)
	{
		aswQueueRet = OS_ReceiveQueue(MBX_ID_RPC, OS_TOUT_FOREVER, &astMessage_p);
		switch(aswQueueRet)
		{	
			case OS_ERR_OK:
					if(astMessage_p != NULL)
					{
						switch(astMessage_p->uhEventCode)
						{
							case RPC_STANDBYOUT:
								wvdIPCOpen(); 
							break;
							case RPC_STANDBYIN:
								wvdRPCClear();
							break;
							default:
							break;
						}
						
					}
				break;
			case OS_ERR_TMOUT:
				break;
			default:
				break;
				
		}
		
	}

}

void wvdRPCClear(void)
{
	wvdRingBufferInit(IPC_CH_CA53_NS);
	wvdRingBufferInit(IPC_CH_CA7_NS);
	wvdMboxDataInit(IPC_CH_CA53_NS);
	wvdMboxDataInit(IPC_CH_CA7_NS);
	wvdIPCHandlerInit(IPC_CH_CA53_NS);
	wvdIPCHandlerInit(IPC_CH_CA7_NS);
	wvdPCCA53_MainInit();
	wvdPCCA7_MainInit();
	wvdParseBfferInit();
}





