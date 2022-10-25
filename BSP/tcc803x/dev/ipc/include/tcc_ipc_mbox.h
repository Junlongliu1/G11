/****************************************************************************************
 *   FileName    : tcc_ipc_mbox.h
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

#ifndef __TCC_IPC_MBOX_H__
#define __TCC_IPC_MBOX_H__
//#include "tcc_ipc_typedef.h"
#include "apn_basic_type.h"

#define HwMBox0_CA53_S_BASE			(0x1BA00000) /* CA53 -> CR5 */
#define HwMBox0_CR5_S_BASE				(0x1B800000) /* CR5 -> CA53 */

#define HwMBox0_CA53_NS_BASE			(0x1BA10000) /* CA53 -> CR5 */
#define HwMBox0_CR5_NS_BASE			(0x1B810000) /* CR5 -> CA53 */

#define HwMBox1_CA7_S_BASE				(0x1BA20000) /* CA7 -> CR5 */
#define HwMBox1_CR5_S_BASE				(0x1B820000) /* CR5 -> CA7 */

#define HwMBox1_CA7_NS_BASE			(0x1BA30000) /* CA7 -> CR5 */
#define HwMBox1_CR5_NS_BASE			(0x1B830000) /* CR5 -> CA7 */

#define HwMBox2_CM4_NS_BASE			(0x1BA40000) /* CM4 -> CR5 */
#define HwMBox2_CR5_NS_BASE			(0x1B840000) /* CR5 -> CM4 */


/*******************************************************************************
*    MailBox CTR Register Field Define
********************************************************************************/
#define D_FLUSH_BIT		IPC_Hw7
#define FLUSH_BIT		IPC_Hw6
#define OEN_BIT			IPC_Hw5
#define IEN_BIT			IPC_Hw4

#define INT_LEVEL_0		0x0UL
#define INT_LEVEL_2		0x1UL
#define INT_LEVEL_4		0x2UL
#define INT_LEVEL_8		0x3UL


/*******************************************************************************
*    MailBox Register Define (Base Addr = 0x1BA10000[main], 0x1B810000[sub])
********************************************************************************/
typedef struct {
    Type_uWord DATA           :32;
} MBOX_DATA_IDX_TYPE;

typedef union {
	Type_uWord			nREG;
	MBOX_DATA_IDX_TYPE      bREG;
}	MBOX_CTL_xxx_TYPE;

typedef struct {
	Type_uWord MEMP		:1;
	Type_uWord	MFUL		:1;
	Type_uWord 				:2;
	Type_uWord MCOUNT        :4;
	Type_uWord 				:8;
	Type_uWord SEMP		:1;
	Type_uWord SFUL		:1;
	Type_uWord 				:2;
	Type_uWord SCOUNT		:4;
	Type_uWord 				:8;
}	MBOX_STR_IDX_TYPE;

typedef struct {
	Type_uWord LEVEL			:2;
	Type_uWord                :2;
	Type_uWord IEN            :1;
	Type_uWord OEN            :1;
	Type_uWord FLUSH          :1;
	Type_uWord D_FLUSH          :1;
	Type_uWord                :24;
}	MBOX_CTL_IDX_TYPE;

typedef union {
	Type_uWord           nREG;
	MBOX_STR_IDX_TYPE       bREG;
}	MBOX_STR_TYPE;

typedef struct {
	Type_uWord MCOUNT		:16;
	Type_uWord				:14;
	Type_uWord MFUL			:1;
	Type_uWord MEMP			:1;
}	MBOX_DT_STR_IDX_TYPE;

typedef struct {
	Type_uWord SCOUNT		:16;
	Type_uWord				:14;
	Type_uWord SFUL			:1;
	Type_uWord SEMP			:1;
}	MBOX_DR_STR_IDX_TYPE;

typedef union {
	Type_uWord           nREG;
	MBOX_DT_STR_IDX_TYPE       bREG;
}	MBOX_DT_STR_TYPE;

typedef union {
	Type_uWord           nREG;
	MBOX_DR_STR_IDX_TYPE       bREG;
}	MBOX_DR_STR_TYPE;

typedef union {
	Type_uWord           nREG;
	MBOX_CTL_IDX_TYPE       bREG;
}	MBOX_CTL_016_TYPE;

typedef struct {
	Type_uWord TXD            :32;
}	MBOX_TXD_IDX_TYPE;

typedef union {
	Type_uWord           nREG;
	MBOX_TXD_IDX_TYPE       bREG;
}	MBOX_CTL_TXD_TYPE;

typedef struct {
	Type_uWord RXD            :32;
}	MBOX_RXD_IDX_TYPE;

typedef union {
	Type_uWord           nREG;
	MBOX_RXD_IDX_TYPE       bREG;
}	MBOX_CTL_RXD_TYPE;

typedef struct _MAILBOX {
	MBOX_CTL_TXD_TYPE    uMBOX_TX0;	// 0x0000
	MBOX_CTL_TXD_TYPE    uMBOX_TX1;	// 0x0004
	MBOX_CTL_TXD_TYPE    uMBOX_TX2; 	// 0x0008
	MBOX_CTL_TXD_TYPE    uMBOX_TX3; 	// 0x000C
	MBOX_CTL_TXD_TYPE    uMBOX_TX4;	// 0x0010
	MBOX_CTL_TXD_TYPE    uMBOX_TX5;	// 0x0014
	MBOX_CTL_TXD_TYPE    uMBOX_TX6;	// 0x0018
	MBOX_CTL_TXD_TYPE    uMBOX_TX7;	// 0x001C
	MBOX_CTL_RXD_TYPE    uMBOX_RX0;	// 0x0020
	MBOX_CTL_RXD_TYPE    uMBOX_RX1;	// 0x0024
	MBOX_CTL_RXD_TYPE    uMBOX_RX2;	// 0x0028
	MBOX_CTL_RXD_TYPE    uMBOX_RX3;	// 0x002C
	MBOX_CTL_RXD_TYPE    uMBOX_RX4;	// 0x0030
	MBOX_CTL_RXD_TYPE    uMBOX_RX5;	// 0x0034
	MBOX_CTL_RXD_TYPE    uMBOX_RX6;	// 0x0038
	MBOX_CTL_RXD_TYPE    uMBOX_RX7;	// 0x003C
	MBOX_CTL_016_TYPE    uMBOX_CTL_016;	// 0x0040
	MBOX_STR_TYPE		 uMBOX_STR;		// 0x0044
	MBOX_CTL_xxx_TYPE	uMBOX_DUMMY_018;	// 0x0048
	MBOX_CTL_xxx_TYPE	uMBOX_DUMMY_019;	// 0x004C
	MBOX_DT_STR_TYPE	uMBOX_TXFIFO_STR;	// 0x0050
	MBOX_DR_STR_TYPE	uMBOX_RXFIFO_STR;	// 0x0054
	MBOX_CTL_xxx_TYPE	uMBOX_DUMMY_022;	// 0x0058
	MBOX_CTL_xxx_TYPE	uMBOX_DUMMY_023;	// 0x005C
	MBOX_CTL_TXD_TYPE	uMBOX_TXFIFO;		// 0x0060
	MBOX_CTL_xxx_TYPE	uMBOX_DUMMY_025;	// 0x0064
	MBOX_CTL_xxx_TYPE	uMBOX_DUMMY_026;	// 0x0068
	MBOX_CTL_xxx_TYPE	uMBOX_DUMMY_027;	// 0x006C
	MBOX_CTL_RXD_TYPE	uMBOX_RXFIFO;		// 0x0070
} MAILBOX, *PMAILBOX;


#define MAX_MBOX_DATA_IDX	2
#define MAX_MBOX_CMD_FIFO_CNT	8
#define MAX_MBOX_DATA_FIFO_CNT	128

typedef struct {
	Type_uWord reg[MAX_MBOX_CMD_FIFO_CNT];
	Type_uWord sel_dataIdx;
}MboxMsgReg;

typedef struct {
	Type_uWord uwcur_dataIdx;
    	Type_uWord uwdata_size;
	Type_uWord data[MAX_MBOX_DATA_IDX][MAX_MBOX_DATA_FIFO_CNT];
}MboxDataReg_ts;

typedef void (*ipc_mbox_receive)(IPC_CH ch, const MboxMsgReg * );

Type_sWord wswipc_mailbox_init(IPC_CH ch, Type_uWord auwmyMBoxBase, Type_uWord auwotherMBoxBase,Type_uWord auwirqNum, Type_uByte aubintLevel, ipc_mbox_receive handler);
void wvdipc_mailbox_deinit(IPC_CH ch);
Type_sWord wswipc_mailbox_send(IPC_CH ch, const MboxMsgReg * Msg_p, Type_uWord auwTimeOut);
Type_sWord wswipc_mailbox_send_open(IPC_CH ch, const MboxMsgReg * Msg_p, Type_uWord auwTimeOut); //for only open
Type_sWord wswipc_mailbox_send_close(IPC_CH ch, const MboxMsgReg * Msg_p, Type_uWord auwTimeOut);
Type_sWord wswipc_mailbox_send_data(IPC_CH ch, const MboxMsgReg * Msg_p, const Type_uByte *aubbuff_p, const Type_uWord auwsize, Type_uWord auwTimeOut); //QAC
void wvdipc_mailbox_get_data(IPC_CH ch, Type_uByte * aubbuff_p, Type_uWord auwsize);
Type_uWord wuwipc_mailbox_get_data_size(IPC_CH ch);
void wvdMboxDataInit(IPC_CH ch);

extern MboxDataReg_ts mboxData[IPC_CH_MAX];//Msg(4:3408) 'mboxData' has external linkage but is being defined without any previous declaration. MISRA-C:2004 Rule 8.8

#endif /* __TCC_IPC_MBOX_H__ */
