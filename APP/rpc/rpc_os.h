#ifndef __RPC_OS_H__
#define __RPC_OS_H__
#include "apn_basic_type.h"


#define RPC_CLEAR_UB (0x00U)
#define RPC_STANDBYOUT (0x0001U)
#define RPC_STANDBYIN (0x0002U)

#define VALUE_INIT 0

extern Type_uByte wubRPCStandbyFlag;

void wvdRPCClear(void);




#endif
