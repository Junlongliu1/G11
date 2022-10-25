/****************************************************************************************
 *   FileName    : tcc_ipc.h
 *   Description :
 ****************************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips Inc.
 *   All rights reserved

This source code contains confidential information of Telechips.
Any unauthorized use without a written permission of Telechips including not limited
to re-distribution in source or binary form is strictly prohibited.
This source code is provided AS IS and nothing contained in this source code
shall constitute any express or implied warranty of any kind, including without limitation,
any warranty of merchantability, fitness for a particular purpose or non-infringement of any patent,
copyright or other third party intellectual property right.
No warranty is made, express or implied, regarding the information accuracy,
completeness, or performance.
In no event shall Telechips be liable for any claim, damages or other liability arising from,
out of or in connection with this source code or the use in the source code.
This source code is provided subject to the terms of a Mutual Non-Disclosure Agreement
between Telechips and Company.
*
****************************************************************************************/
#include <types.h>


#ifndef __TCC_IPC_H__
#define __TCC_IPC_H__

//#define IPC_CA53_S
#define IPC_CA53_NS
//#define IPC_CA7_S
#define IPC_CA7_NS // for around view process
#define IPC_CM4_NS

typedef enum {
#ifdef IPC_CA53_S
	IPC_CH_CA53_S,
#endif
#ifdef IPC_CA53_NS
	IPC_CH_CA53_NS,
#endif
#ifdef IPC_CA7_S
	IPC_CH_CA7_S,
#endif
#ifdef IPC_CA7_NS
	IPC_CH_CA7_NS,
#endif
#ifdef IPC_CM4_NS
	IPC_CH_CM4_NS,
#endif
	IPC_CH_MAX,
} IPC_CH;

typedef enum {
	IPC_APP_ID_UBOOT = 0,
	IPC_APP_ID_IPC = 1,
	IPC_APP_ID_ARMTF = 2,
	IPC_APP_ID_A7SR = 3,
	IPC_APP_ID_FWUG = 4,
	IPC_APP_ID_A53MR = 5,
	IPC_APP_ID_MAX = 6  //MAX
} IPC_APP_ID;

#define IPC_BUF_CHK_ERROR		(-1)		/* function error */
#define IPC_BUF_CHK_CLOSE		(0)		/* buffer close. Maybe MICOM or linux have closed ipc. */
#define IPC_BUF_CHK_OVERFLOW	(1)		/*detecting read buffer overflow  */
#define IPC_BUF_CHK_NOT_INIT	(2)		/* IPC (Buffer) Not ready  */
#define IPC_BUF_CHK_TIMEOUT	(3)

#define IPC_SUCCESS					(0)		/* Success */
#define IPC_ERR_COMMON				(-1)		/* common error*/
#define IPC_ERR_BUSY				(-2)		/* IPC is busy. You got the return, After a while you should try.*/
#define IPC_ERR_NOTREADY			(-3)		/* IPC is not ready. Other processor is not ready yet.*/
#define IPC_ERR_TIMEOUT			(-4)		/* Other process is not responding. */
#define IPC_ERR_WRITE				(-5)
#define IPC_ERR_READ				(-6)
#define IPC_ERR_BUFFER				(-7)
#define IPC_ERR_ARGUMENT			(-8)		/* Invalid argument */
#define IPC_ERR_RECEIVER_NOT_SET	(-9)		/* Receiver processor not set */
#define IPC_ERR_RECEIVER_DOWN		(-10)	/* Mbox is not set */

#define INIT_VALUE 0x00000000
#define VALUE_0 0
#define VALUE_1 1

typedef enum {
	IPC_PING_SUCCESS = 0,				/* Ping success */
	IPC_PING_ERR_INIT,					/* My ipc initialize failed */
	IPC_PING_ERR_NOT_READY,			/* Other IPC not open */
	IPC_PING_ERR_SENDER_MBOX,		/* My(sender) mbox is not set or error */
	IPC_PING_ERR_RECEIVER_MBOX,		/* Receiver mbox is not set or error*/
	IPC_PING_ERR_SEND,				/* Can not send data. Maybe receiver mbox interrupt is busy*/
	IPC_PING_ERR_RESPOND,				/* Receiver does not send respond data. */
	MAX_IPC_PING_ERR,
}ipc_ping_error;

typedef struct _IPC_OPTION
{
	//uint32 isBlock;
	uint32 isAck;
}IPC_OPTION;

//IPC block or non block option
//#define IPC_O_NONBLOCK 0x00
//#define IPC_O_BLOCK	0x01

//IPC ack or non ack option
#define IPC_O_NONACK 0x00
#define IPC_O_ACK	0x01

//IPC reserved0 option
#define IPC_O_RESERVED0 0x00

//IPC reserved1 option
#define IPC_O_RESERVED1 0x00

#define IPC_MODE_0	(IPC_O_RESERVED1<<24) | (IPC_O_RESERVED0<<16) | (IPC_O_ACK<<8) //| (IPC_O_NONBLOCK)
//#define IPC_MODE_1	(IPC_O_RESERVED1<<24) | (IPC_O_RESERVED0<<16) | (IPC_O_ACK<<8) | (IPC_O_BLOCK)

#define IPC_O_BLOCK_MASK 	0x000000FFU
#define IPC_O_ACK_MASK 		0x0000FF00U

typedef struct _ipc_ping_info{
	ipc_ping_error pingResult;
	uint32 sendByte;
	uint32 responseTime;
	uint32 compareResult;			/*Data compare results. */
}__attribute__((packed))ipc_ping_info;



//#define TCC_IPC_SDK_V180_LVDS

#endif /* __TCC_IPC_H__ */
