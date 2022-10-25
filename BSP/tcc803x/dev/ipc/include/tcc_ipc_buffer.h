/****************************************************************************************
 *   FileName    : tcc_ipc_buffer.h
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

#ifndef __TCC_IPC_BUFFER_H__
#define __TCC_IPC_BUFFER_H__
#include "apn_basic_type.h"
//#include "tcc_ipc_typedef.h"

#define	IPC_BUFFER_ERROR		(-1)
#define	IPC_BUFFER_FULL		(-2)
#define	IPC_BUFFER_EMPTY		(-3)
#define	IPC_BUFFER_OK		0

typedef struct _IPC_BUFFER
{
	Type_uWord 	uwHead;		//read position
	Type_uWord	uwTail;		//write position
	Type_uWord	uwMaxBufferSize;
	Type_uByte* 	uwBuffer_p;
}IPC_RINGBUF_st;

void	wvdipc_buffer_init(IPC_RINGBUF_st *BufCtrl_p,Type_uByte* aubbuff_p,Type_uWord auwsize);
void	wvdipc_buffer_flush(IPC_RINGBUF_st *BufCtrl_p);
Type_sWord wswipc_buffer_data_available(const IPC_RINGBUF_st *BufCtrl_p);
Type_sWord wswipc_buffer_free_space(const IPC_RINGBUF_st *BufCtrl_p);
Type_sWord wswipc_push_buffer(IPC_RINGBUF_st *BufCtrl_p,const Type_uByte * aubbuffer_p, Type_uWord auwsize);
Type_sWord wswipc_push_buffer_overwrite(IPC_RINGBUF_st *BufCtrl_p,const Type_uByte * aubbuffer_p, Type_uWord auwsize);
Type_sWord wswipc_pop_buffer(IPC_RINGBUF_st *BufCtrl_p,Type_uByte * aubbuffer_p, Type_uWord auwsize);

#endif /* __TCC_IPC_BUFFER_H__ */

