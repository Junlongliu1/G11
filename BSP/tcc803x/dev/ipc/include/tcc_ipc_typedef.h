/****************************************************************************************
 *   FileName    : tcc_ipc_typedef.h
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
#ifndef __TCC_IPC_TYPE_DEF_H__
#define __TCC_IPC_TYPE_DEF_H__

#include "types.h"

#ifndef NULL
#define NULL	(0)
#endif

#define IPC_Hw37		(1LL << 37)
#define IPC_Hw36		(1LL << 36)
#define IPC_Hw35		(1LL << 35)
#define IPC_Hw34		(1LL << 34)
#define IPC_Hw33		(1LL << 33)
#define IPC_Hw32		(1LL << 32)
#define IPC_Hw31		0x80000000
#define IPC_Hw30		0x40000000
#define IPC_Hw29		0x20000000
#define IPC_Hw28		0x10000000
#define IPC_Hw27		0x08000000
#define IPC_Hw26		0x04000000
#define IPC_Hw25		0x02000000
#define IPC_Hw24		0x01000000
#define IPC_Hw23		0x00800000
#define IPC_Hw22		0x00400000
#define IPC_Hw21		0x00200000
#define IPC_Hw20		0x00100000
#define IPC_Hw19		0x00080000
#define IPC_Hw18		0x00040000
#define IPC_Hw17		0x00020000
#define IPC_Hw16		0x00010000
#define IPC_Hw15		0x00008000
#define IPC_Hw14		0x00004000
#define IPC_Hw13		0x00002000
#define IPC_Hw12		0x00001000
#define IPC_Hw11		0x00000800
#define IPC_Hw10		0x00000400
#define IPC_Hw9		    0x00000200
#define IPC_Hw8		    0x00000100
#define IPC_Hw7		    0x00000080
#define IPC_Hw6		    0x00000040
#define IPC_Hw5		    0x00000020
#define IPC_Hw4		    0x00000010
#define IPC_Hw3		    0x00000008
#define IPC_Hw2		    0x00000004
#define IPC_Hw1		    0x00000002
#define IPC_Hw0		    0x00000001
#define IPC_HwZERO	    0x00000000

#ifndef	BITSET
#define BITSET(X, MASK)				((X) |= (uint32)(MASK))
#endif
#ifndef	BITSCLR
#define BITSCLR(X, SMASK, CMASK)	((X) = ((((uint32)(X)) | ((uint32)(SMASK))) & ~((uint32)(CMASK))) )
#endif
#ifndef	BITCSET
#define BITCSET(X, CMASK, SMASK)	((X) = ((((uint32)(X)) & ~((uint32)(CMASK))) | ((uint32)(SMASK))) )
#endif
#ifndef	BITCLR
#define	BITCLR(X, MASK)				( (X) &= ~((uint32)(MASK)) )
#endif

typedef		int8			IPC_CHAR;                      /*  8-bit character : char						*/
typedef		uint8		IPC_UCHAR;                   /*  8-bit unsigned character : unsigned char		*/
typedef  	uint8		IPC_BOOLEAN;			/*  8-bit boolean or logical : unsigned  char		*/
typedef  	uint8		IPC_UINT8;				/*  8-bit unsigned integer : unsigned  char			*/
typedef		int8			IPC_INT8;                   	/*  8-bit   signed integer : signed  char			*/
typedef		uint16		IPC_UINT16;                   	/* 16-bit unsigned integer : unsigned  short		*/
typedef		int16		IPC_INT16;                   	/* 16-bit   signed integer : signed  short			*/
typedef		uint32		IPC_UINT32;			/* 32-bit unsigned integer : unsigned  int			*/
typedef		int32		IPC_INT32;				/* 32-bit   signed integer : signed  int				*/
typedef 		uint64  		IPC_INT64U;                   /* 64-bit unsigned integer                              */
typedef    	int64  		IPC_INT64S;                   /* 64-bit   signed integer      				*/


typedef enum {
	CTL_CMD = 0x0000,
	WRITE_CMD,
	MAX_CMD_TYPE,
}IpcCmdType;

typedef enum {
    TCC_SYS_TYPE = 0x0001,
    CST_SYS_TYPE,
    MAX_SYS_TYPE,
} IpcSysType;

typedef enum {
    TCC_SYS_TST00 = 0x0000,
    TCC_SYS_LVDS_CMD,
    MAX_TCC_SYS_TYPE,
} IpcTccSysType;

typedef enum {
    CST_SYS_TST00 = 0x0000,
    CST_SYS_TST01,
    MAX_CST_SYS_TYPE,
} IpcCstSysType;
    
typedef enum {
	/* control command */
	IPC_OPEN = 0x0001,
	IPC_CLOSE,
	IPC_SEND_PING,
	IPC_WRITE,
	IPC_ACK,
	MAX_CMD_ID,
}IpcCmdID;

typedef enum {
	CMD_A53_REBOOT = 0x000,
	CMD_A7S_REBOOT,
	CMD_A7S_MCP,
	MAX_REBOOT_TYPE,
}RebootCmdType;

#endif /* __TCC_IPC_TYPE_DEF_H__ */
