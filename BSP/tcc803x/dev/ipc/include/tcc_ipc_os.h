/****************************************************************************************
 *   FileName    : tcc_ipc_os.h
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

#ifndef __TCC_IPC_OS_H__
#define __TCC_IPC_OS_H__

#include <bsp_int.h>
#include "tcc_ipc.h"
#include "tcc_ipc_typedef.h"
#include "apn_basic_type.h"

#define TICK_PER_MS		1

#define READ_BUF_CHECK_FLAGS 		0x0000000Ful
#define READ_BUF_OVERFLOW_BIT		0x00000001ul
#define READ_BUF_CLOSE_BIT			0x00000002ul
#define READ_BUF_NO_INIT_BIT		0x00000004ul
#define READ_BUF_TIME_OUT_BIT		0x00000008ul

IPC_INT64S ipc_get_msec(void);
void wvdipc_mdelay(Type_uWord auwdly);
void wvdipc_udelay(Type_uWord auwdly);
void wvdipc_common_lock(IPC_CH ch);
void wvdipc_common_unlock(IPC_CH ch);
void wvdipc_mbox_lock(IPC_CH ch);
void wvdipc_mbox_unlock(IPC_CH ch);
void wvdipc_write_lock(IPC_CH ch);
void wvdipc_write_unlock(IPC_CH ch);
void wvdipc_read_lock(IPC_CH ch);
void wvdipc_read_unlock(IPC_CH ch);
void wvdipc_rbuffer_lock(IPC_CH ch);
void wvdipc_rbuffer_unlock(IPC_CH ch);
void wvdipc_wbuffer_lock(IPC_CH ch);
void wvdipc_wbuffer_unlock(IPC_CH ch);
Type_sWord wswipc_cmd_wait_event_timeout(IPC_CH ch, IpcCmdType cmdType, Type_uWord auwtimeOut);
void wvdipc_cmd_wake_preset(IPC_CH ch, IpcCmdType cmdType, Type_uWord auwseqID);
void wvdipc_cmd_wake_up(IPC_CH ch, IpcCmdType cmdType, Type_uWord auwseqID);
Type_sWord wswipc_request_irq(Type_uWord auwirq, BSP_INT_FNCT_PTR func);
Type_sWord wswipc_free_irq(Type_uWord auwirq);
void wvdipc_os_resouce_init(IPC_CH ch);
void wvdipc_os_resouce_release(IPC_CH ch);
#endif /* __TCC_IPC_OS_H__ */

