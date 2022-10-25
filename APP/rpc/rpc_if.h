#ifndef __RPC_IF_H__
#define __RPC_IF_H__
#include "apn_basic_type.h"
//#include "tcc_ipc_protocol.h"
#include "os_framework.h"
#include "debug.h"
#include "rpc_module.h"

#define RPC_INIT 0x00
#define R5_MIN 0x10
#define R5_MOULDE_START (R5_MIN+1)
#define QUEUE_MAX_SIZE 2048

#define RPC_NG -1
#define RPC_OK 0
#define RPC_BUSY 	-2
#define RPC_ARGUMENT 	-3
#define RPC_NOEXSIT 	-4

#define SRCID_INIT 0

#define DECODE8(val)    ((val & 0x000000ff) >> 0)
#define DECODE16(val)   ((val & 0x0000ff00) >> 8)
#define DECODE24(val)   ((val & 0x00ff0000) >> 16)
#define DECODE32(val)   ((val & 0xff000000) >> 24)

#define ENCODE8(byte)   (byte)
#define ENCODE16(byte)  (byte << 8)
#define ENCODE24(byte)  (byte << 16)
#define ENCODE32(byte)  (byte << 24)

#define LIBARPC_DEBUG_LEVEL (0U)

#if (DEBUG_ENABLE)
#define RPC_D(fmt, ...)	printf("(D)(%8d)[RPC][%s:%d]"fmt"\r\n",OSAL_GetOSTimeTick(), __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define RPC_E(fmt, ...)	printf("(E)(%8d)[RPC][%s:%d]"fmt"\r\n",OSAL_GetOSTimeTick(), __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define RPC_D(fmt, args...)
#define RPC_E(fmt, args...)
#endif

#define DEBUG_LIBARPC_PRINTF(format, arg...) \
	if ((uint32_t)wswg_arpc_ctrl_debug > LIBARPC_DEBUG_LEVEL) \
	{ \
        printf("####zjqarpc####[%s:%d] : " format "", __FUNCTION__, __LINE__, ##arg); \
	}

typedef struct{
    Type_uByte            buf[QUEUE_MAX_SIZE];
    Type_uHWord           w_ptr;
} quene_buf;

extern Type_sWord wswg_arpc_ctrl_debug;
typedef void (*wvdRPCCallback)(const Type_uByte aubsrc, const Type_uHWord auhevent, void* data, const Type_uHWord auhlength);
typedef void (*ipc_call_back)(Type_uHWord auhcmd, Type_uByte *data, Type_uHWord auhlength);
extern Type_sByte wsbRPCInit(const Type_uByte aubsrcId, wvdRPCCallback func);
extern Type_sByte wsbRPCSendMsg(Type_uByte aubsrc, Type_uByte aubdest, Type_uHWord auhevent, void* data, Type_uHWord auhlength);
extern Type_uByte wubRPC_Init(void);
extern Type_uByte wubRPC_Standbyout(void);
extern Type_uByte wubRPC_Standbyin(void);
extern Type_sWord nswRPC_SendQueue(OS_QueueId_t auwMbxId, const Type_uHWord auhEventCode, const OS_ObjPri_t aubPriority);
extern Type_sByte wswRPCInit(const Type_uByte srcId, ipc_call_back func);
extern Type_sByte wubRPCSendMsg(Type_uByte sender, Type_uByte receiver, Type_uHWord event, void* data, Type_uHWord length);
#endif
