/****************************************************************************************
 *   FileName    : tcc_ipc_buffer.c
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
#include <types.h>
#include <osal.h>
#include "tcc_ipc_typedef.h"
#include "tcc_ipc.h"
#include "tcc_ipc_buffer.h"

void	wvdipc_buffer_init(IPC_RINGBUF_st *BufCtrl_p,Type_uByte* aubbuff_p,Type_uWord auwsize)

{
	BufCtrl_p->uwHead = BufCtrl_p->uwTail = 0;
	BufCtrl_p->uwMaxBufferSize = auwsize;
	BufCtrl_p->uwBuffer_p = aubbuff_p;
}

void	wvdipc_buffer_flush(IPC_RINGBUF_st *BufCtrl_p)

{
	BufCtrl_p->uwHead = BufCtrl_p->uwTail = 0;
}

Type_sWord wswipc_buffer_data_available(const IPC_RINGBUF_st *BufCtrl_p)

{
	Type_sWord aswiRet, aswiRead, aswiWrite;
	aswiRead = (int32)BufCtrl_p->uwHead;
	aswiWrite = (int32)BufCtrl_p->uwTail;

	if (aswiWrite >= aswiRead)
	{
		// The read pointer is before the write pointer in the bufer.
		aswiRet = aswiWrite -	aswiRead;
	}
	else
	{
		// The write pointer is before the read pointer in the buffer.
		aswiRet = (int32)BufCtrl_p->uwMaxBufferSize - (aswiRead - aswiWrite);
	}

	return aswiRet;
}

Type_sWord wswipc_buffer_free_space(const IPC_RINGBUF_st *BufCtrl_p)

{
	Type_sWord aswiRet, aswiRead, aswiWrite;
	aswiRead = (Type_sWord)BufCtrl_p->uwHead;
	aswiWrite = (Type_sWord)BufCtrl_p->uwTail;

	if (aswiWrite < aswiRead)
	{
		// The write pointer is before the read pointer in the buffer.
		aswiRet = aswiRead - aswiWrite - 1;
	}
	else
	{
		// The read pointer is before the write pointer in the bufer.
		aswiRet = (int32)BufCtrl_p->uwMaxBufferSize - (aswiWrite - aswiRead) - 1;
	}
	/*
	If the head and tail are the same(pBufCtrl->uwHead ==pBufCtrl->uwTail), the buffer is empty.
	Therefore, the head must be 1 less than the tail.
	So, free space is (uwMaxBufferSize -1)
	*/

	return aswiRet;
}

Type_sWord wswipc_push_buffer(IPC_RINGBUF_st *BufCtrl_p,const Type_uByte * aubbuffer_p, Type_uWord auwsize)

{
	Type_sWord aswret= IPC_BUFFER_ERROR;
	Type_uWord auwfreeSpace;

	auwfreeSpace = wswipc_buffer_free_space(BufCtrl_p);
	if(auwfreeSpace >= auwsize)
	{
		Type_uWord auwcontinuousSize;

		auwcontinuousSize = BufCtrl_p->uwMaxBufferSize - BufCtrl_p->uwTail;
		if(auwcontinuousSize > auwsize)
		{
			(void)OSAL_MemCopy((void *)&BufCtrl_p->uwBuffer_p[BufCtrl_p->uwTail], (const void *)aubbuffer_p, (uint32)auwsize);
			BufCtrl_p->uwTail +=auwsize;
			aswret =auwsize;
		}
		else
		{
			Type_uWord auwremainSize;

			(void)OSAL_MemCopy((void *)&BufCtrl_p->uwBuffer_p[BufCtrl_p->uwTail], (const void *)aubbuffer_p, (uint32)auwcontinuousSize);
			auwremainSize = auwsize - auwcontinuousSize;
			(void)OSAL_MemCopy((void *)&BufCtrl_p->uwBuffer_p[0], (const void *)&aubbuffer_p[auwcontinuousSize], (uint32)auwremainSize);
			BufCtrl_p->uwTail = auwremainSize;
			aswret =auwsize;
		}
	}
	else
	{
		aswret =IPC_BUFFER_FULL;
	}

	return aswret;
}

Type_sWord wswipc_push_buffer_overwrite(IPC_RINGBUF_st *BufCtrl_p,const Type_uByte * aubbuffer_p, Type_uWord auwsize)

{
	Type_sWord aswret= IPC_BUFFER_ERROR;
	Type_uWord auwcontinuousSize;
	if(BufCtrl_p->uwMaxBufferSize >= auwsize)
	{
		auwcontinuousSize = BufCtrl_p->uwMaxBufferSize - BufCtrl_p->uwTail;
		if(auwcontinuousSize > auwsize)
		{
			(void)OSAL_MemCopy((void *)&BufCtrl_p->uwBuffer_p[BufCtrl_p->uwTail], (const void *)aubbuffer_p, (Type_uWord)auwsize);
			BufCtrl_p->uwTail +=auwsize;
			BufCtrl_p->uwHead = (BufCtrl_p->uwTail+1) % BufCtrl_p->uwMaxBufferSize;
			aswret =auwsize;
		}
		else
		{
			Type_uWord auwremainSize;

			(void)OSAL_MemCopy((void *)&BufCtrl_p->uwBuffer_p[BufCtrl_p->uwTail], (const void *)aubbuffer_p, (Type_uWord)auwcontinuousSize);
			auwremainSize = auwsize - auwcontinuousSize;
			(void)OSAL_MemCopy((void *)&BufCtrl_p->uwBuffer_p[0], (const void *)&aubbuffer_p[auwcontinuousSize], (Type_uWord)auwremainSize);
			BufCtrl_p->uwTail = auwremainSize;
			BufCtrl_p->uwHead = (BufCtrl_p->uwTail+1) % BufCtrl_p->uwMaxBufferSize;
			aswret = auwsize;
		}
	}
	return aswret;
}

Type_sWord wswipc_pop_buffer(IPC_RINGBUF_st *BufCtrl_p,Type_uByte * aubbuffer_p, Type_uWord auwsize)

{
	Type_sWord aswret = IPC_BUFFER_ERROR;

	if (BufCtrl_p->uwTail == BufCtrl_p->uwHead)
	{
		aswret = IPC_BUFFER_EMPTY;
	}
	else
	{
		Type_uWord auwdataSize;
		auwdataSize = wswipc_buffer_data_available(BufCtrl_p);
		if(auwdataSize >= auwsize)
		{
			Type_uWord auwcontinuousSize;

			auwcontinuousSize = BufCtrl_p->uwMaxBufferSize - BufCtrl_p->uwHead;
			if(auwcontinuousSize > auwsize)
			{
				(void)OSAL_MemCopy((void *)aubbuffer_p, (const void *)&BufCtrl_p->uwBuffer_p[BufCtrl_p->uwHead],auwsize );
				BufCtrl_p->uwHead += auwsize;
				aswret = IPC_BUFFER_OK;
			}
			else
			{
				Type_uWord auwremainSize;
				(void)OSAL_MemCopy((void *)aubbuffer_p, (const void *)&BufCtrl_p->uwBuffer_p[BufCtrl_p->uwHead],auwcontinuousSize );
				auwremainSize = auwsize - auwcontinuousSize;
				(void)OSAL_MemCopy((void *)&aubbuffer_p[auwcontinuousSize], (const void *)&BufCtrl_p->uwBuffer_p[0],auwremainSize );
				BufCtrl_p->uwHead= auwremainSize;
				aswret = IPC_BUFFER_OK;
			}
		}
		#if 0 //codesonar
		else
		{
			aswret = IPC_BUFFER_ERROR;
		}
		#endif
	}
	return aswret;
}


