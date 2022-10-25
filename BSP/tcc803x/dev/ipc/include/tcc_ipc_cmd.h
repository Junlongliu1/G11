/****************************************************************************************
 *   FileName    : tcc_ipc_cmd.h
 *   Description : 
 ****************************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips Inc.
 *   All rights reserved 
 
This source code contains confidential information of Telechips.
Any unauthorized use without a written permission of Telechips including not limited 
to re-distribution in source or binary form is strictly prohibited.
This source code is provided ¡°AS IS¡± and nothing contained in this source code 
shall constitute any express or implied warranty of any kind, including without limitation, 
any warranty of merchantability, fitness for a particular purpose or non-infringement of any patent, 
copyright or other third party intellectual property right. 
No warranty is made, express or implied, regarding the information¡¯s accuracy, 
completeness, or performance. 
In no event shall Telechips be liable for any claim, damages or other liability arising from, 
out of or in connection with this source code or the use in the source code. 
This source code is provided subject to the terms of a Mutual Non-Disclosure Agreement 
between Telechips and Company.
*
****************************************************************************************/

#ifndef __TCC_IPC_CMD_H__
#define __TCC_IPC_CMD_H__
#include "apn_basic_type.h"
//#include "tcc_ipc_typedef.h"

#define CMD_TYPE_MASK 	0xFFFF0000U
#define CMD_SYS_MASK    CMD_TYPE_MASK	
#define CMD_ID_MASK 	0xFFFFU
#define CMD_USER_MASK 	0x0000FFFFU

Type_sWord wswipc_send_open(IPC_CH ch);
Type_sWord wswipc_send_close(IPC_CH ch);
Type_sWord wswipc_send_write(IPC_CH ch, const Type_uWord *auwcmd_p, const Type_uByte *aubbuff_p, const Type_uWord auwsize, Type_uWord auwisAck); //QAC
Type_sWord wswipc_send_ping(IPC_CH ch);
Type_sWord wswipc_send_ack(IPC_CH ch, Type_uWord auwseqID, IpcCmdType cmdType, Type_uWord auwsourcCmd);
//Type_sWord wswipc_send_sys_ctrl(IPC_CH ch, Type_uWord *auwcmd_p, Type_uByte *aubbuff_p, Type_uWord auwsize, Type_uWord auwisAck);

#endif /* __TCC_IPC_CMD_H__ */
