/****************************************************************************************
 *   FileName    : tcc_ipc_protocol.h
 *   Description :
 ****************************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips Inc.
 *   All rights reserved

This source code contains confidential information of Telechips.
Any unauthorized use without a written permission of Telechips including not limited
to re-distribution in source or binary form is strictly prohibited.
This source code is provided ??AS IS?? and nothing contained in this source code
shall constitute any express or implied warranty of any kind, including without limitation,
any warranty of merchantability, fitness for a particular purpose or non-infringement of any patent,
copyright or other third party intellectual property right.
No warranty is made, express or implied, regarding the information??s accuracy,
completeness, or performance.
In no event shall Telechips be liable for any claim, damages or other liability arising from,
out of or in connection with this source code or the use in the source code.
This source code is provided subject to the terms of a Mutual Non-Disclosure Agreement
between Telechips and Company.
*
****************************************************************************************/

#ifndef __TCC_IPC_PROTOCOL_H__
#define __TCC_IPC_PROTOCOL_H__

/***********************************************************************************
*	IPC CMD ID - Power Management
************************************************************************************/
#include "HAL_IPC.h"
#include "HAL_API.h"
#include "apn_basic_type.h"
#include "rpc_if.h"

#define TCC_IPC_CMD_PWR 			0x01

#define PWR_ACC_OFF 				0x01
#define PWR_BU_OFF 					0x02
#define PWR_EXT_SRC 				0x03
#define PWR_BOOT_REASON 			0x04


/***********************************************************************************
*	IPC CMD ID - Key
************************************************************************************/
/* define CMD1 */
#define TCC_IPC_CMD_KEY			0x02

/* define CMD2 */
#define TCC_IPC_CMD_KEY_EXT 		0x01
#define TCC_IPC_CMD_KEY_ROTARY 	0x02

/* define values - Ext key */
#define EXTKEY_1 				0x01
#define EXTKEY_2 				0x02
#define EXTKEY_3 				0x03
#define EXTKEY_4 				0x04
#define EXTKEY_5 				0x05
#define EXTKEY_6 				0x06
#define EXTKEY_ENTER 			0x11
#define EXTKEY_SCAN 			0x12
#define EXTKEY_SETUP 			0x13
#define EXTKEY_MENU 			0x14
#define EXTKEY_DISP 				0x15
#define EXTKEY_TRACKDOWN		0x16
#define EXTKEY_TRACKUP 			0x17
#define EXTKEY_FOLDERDOWN 	0x18
#define EXTKEY_FOLDERUP 		0x19
#define EXTKEY_POWER 			0x1A
#define EXTKEY_RADIO 			0x1B
#define EXTKEY_MEDIA 			0x1C
#define EXTKEY_PHONE 			0x1D
#define EXTKEY_VOLUMEUP 		0x1E
#define EXTKEY_VOLUMEDOWN 	0x1F
#define EXTKEY_MODE 			0x20
#define EXTKEY_SEEKUP 			0x21
#define EXTKEY_SEEKDOWN 		0x22
#define EXTKEY_MUTE 			0x23
#define EXTKEY_PHONEON 		0x24
#define EXTKEY_PHONEOFF 		0x25

/* define values - Rotary key */
#define ROTKEY_CTRLUP 			0x01
#define ROTKEY_CTRLDOWN 		0x02
#define ROTKEY_VOLUP 			0x03
#define ROTKEY_VOLDOWN 		0x04


/***********************************************************************************
*	IPC CMD ID - NVM management
************************************************************************************/
/* define CMD1 */
#define TCC_IPC_CMD_NVM_MANAGER				0x03

/* define CMD2 */
#define IPC_NVM_SAVE_DATA 			0x01
#define IPC_NVM_LOAD_DATA 			0x02
#define IPC_NVM_RESET_DATA 		0x03

/* define Ack */
#define IPC_NVM_ACK_FAIL 			0x00
#define IPC_NVM_ACK_SUCCESS 		0x01

/***********************************************************************************
*	IPC CMD ID - Vehicle Signal Demo App
************************************************************************************/
// define CMD1
#define DEMO_SIGNAL_CMD1 		0x5
// define CMD2
#define DEMO_POWER_CMD2 		0x10
#define DEMO_ACCELATOR_CMD2 	0x11
#define DEMO_ILLUMINATION_CMD2 	0x12
#define DEMO_RESERVE_DET_CMD2	0x13
#define DEMO_REVERSE_DET_NORMAL_DATA0	0x0
#define DEMO_REVERSE_DET_REVERSE_DATA0	0x1
#define DEMO_CLIMATE_CMD2 		0x14
#define DEMO_CLIMATE_AUTO_DATA0 0x1 // auto
#define DEMO_CLIMATE_TEMPERATURE_DATA0 0x2 // temperature
#define DEMO_CLIMATE_ONOFF_DATA0 0x3 // On/Off
#define DEMO_CLIMATE_AC_DATA0 0x4 // A/C
#define DEMO_CLIMATE_WIND_MODE_DATA0 0x5 // wind path
#define DEMO_CLIMATE_WIND_MODE_UP_DATA1 0x1 //Upward
#define DEMO_CLIMATE_WIND_MODE_DOWN_DATA1 0x2 //down ward
#define DEMO_CLIMATE_WIND_MODE_ALL_DATA1 0x3 //all
#define DEMO_CLIMATE_AIR_INOUT_DATA0 0x6 // Inner or Outer
#define DEMO_CLIMATE_AIR_INOUT_IN_DATA1 0x1 // Inner
#define DEMO_CLIMATE_AIR_INOUT_OUT_DATA1 0x2 // Outer
#define DEMO_CLIMATE_AIR_INOUT_AUTO_DATA1 0x3 // Auto in/out
#define DEMO_CLIMATE_FRONT_HEATLINE_DATA0 0x7 // Front Heat LIne
#define DEMO_CLIMATE_REAR_HEATLINE_DATA0 0x8 // Rear Heat Line
#define DEMO_TIMESTAMP_CMD2 	0x15
#define DEMO_TIMESTAMP_MCU_BOOT_END_DATA0 	0x1
#define DEMO_TIMESTAMP_XXX1_END_DATA0 	0x2
#define DEMO_TIMESTAMP_XXX2_END_DATA0 	0x3
#define DEMO_TIMESTAMP_MAINAP_BOOT_END_DATA0 	0x4
#define DEMO_MAIN_APP_LAUNCHED_CMD2 	0x16
#define DEMO_RPM_CMD2 	0x17

/***********************************************************************************
*	IPC CMD ID - System Monitoring
************************************************************************************/
/* define CMD1 */
#define TCC_IPC_CMD_SYS_MONITORING			0x04

/* define CMD2 */
#define IPC_SYS_MON_RECOG_REQ	0x01 //(MICOM => Peer Core)
#define IPC_SYS_MON_RECOG_RES	0x01 //(Peer Core => MICOM)

#define IPC_SYS_MONITORING_OK			0x02 //(MICOM => Peer Core)
#define CPU_RESPONSE_OK					0x02 //(Peer Core => MICOM)

/***********************************************************************************
*	IPC Protocol
************************************************************************************/

#define IPC_SYNC 					0xFF
#define IPC_START1 					0x55
#define IPC_START2 					0xAA
#define IPC_RESERVED1				0x00
#define IPC_RESERVED2				0x00
#define IPC_RESERVED3				0x00

//
//	SYNC: 1byte ,  START1: 1byte ,  START2: 1byte ,  CMD1: 2byte ,  CMD2: 2byte ,  LENGTH: 2byte ,  DATA: variable ,  CRC: 2byte
//

#define IPC_MAX_PACKET_SIZE 		0x400 	// 1024
#define IPC_PACKET_PREPARE_SIZE 	0x09
#define IPC_PACKET_PREFIX_SIZE 		0x0B 	// 11

#define IPC_PROCESS_NUM			(ARPC_ID_R5_MAX - ARPC_ID_R5_MIN) 	 
#define IPC_ID_NOEXSIT			(ARPC_ID_R5_MAX+1) 

//#define REGIST_OK		0 	// 10
//#define REGIST_EXSIT	-1 

#define INDEX_INIT 0
#define CRCTBL_SIZE 256
#define LENGTH_INIT 0
#define PACKET_MAXSIZE IPC_MAX_PACKET_SIZE-1
#define CRC_INIT 0
#define CMDSIZE 8
#define RETRY_COUNT 3
#define CRC_OUTSIDE 1
#define DUMMY_DATA_PACKET IPC_PACKET_PREFIX_SIZE+1
#define CRC_COUNT 2
#define PACKET_MINSIZE 1
#define DUMMY_DATA_SIZE 0


typedef enum {
#ifdef IPC_CA53_S
	IPC_SVC_CH_RESERVED0,
#endif
#ifdef IPC_CA53_NS
	IPC_SVC_CH_MAIN,
#endif
#ifdef IPC_CA7_S
	IPC_SVC_CH_RESERVED1,
#endif
#ifdef IPC_CA7_NS
	IPC_SVC_CH_SUB,
#endif
#ifdef IPC_CM4_NS
	IPC_SVC_CH_M4,
#endif
	IPC_SVC_CH_MAX,
} IPC_SVC_CH;

typedef struct _ipc_ch_info {
	IPC_SVC_CH ch;
	HAL_HANDLE halHandle;
} ipc_ch_info_ts;

extern ipc_ch_info_ts ipc_ch_info[IPC_SVC_CH_MAX];


struct ipc_cb_func {
	ipc_call_back ipc_cb_func;
	void *arg1; 
	void *arg2;
};

struct ipc_cb_func_ts {
	wvdRPCCallback vdipc_cb_func;
	Type_uByte ubTaskId;
};

#if 0
typedef struct _tcc_ipc_protocol {
	Type_uByte 	sync;
	Type_uByte 	start1;
	Type_uByte 	start2;
	Type_uHWord 	cmd1;
	Type_uHWord 	cmd2;
	Type_uHWord 	length;
	Type_uByte 	data[IPC_MAX_DATA_SIZE];
	Type_uHWord 	crc;
} __attribute__((packed)) tcc_ipc_protocol;
#endif

extern Type_uByte *wubpackage(Type_uHWord auhcmd1, Type_uHWord auhcmd2, const Type_uByte* data, Type_uHWord auhlength);
extern Type_sByte wsbIPCRegisterFunc(Type_uByte aubsrc, wvdRPCCallback func);
extern Type_uByte wvdGetSrcId(Type_uByte aubTaskId);
extern Type_uByte wubRPCToMoudlue(Type_uByte aubSrc, Type_uByte aubdest, Type_uHWord auhevent, void* data, Type_uHWord auhlength);
extern void wvdipc_register_cb_func(IPC_SVC_CH ch, Type_uByte aubid, ipc_call_back func, void *arg1, void *arg2);
extern Type_sWord wswipc_packet_parser(IPC_SVC_CH ch);
extern Type_sWord wswipc_send_packet(IPC_SVC_CH ch, Type_uHWord auhcmd1, Type_uHWord auhcmd2, const Type_uByte* data, Type_uHWord auhlength);
extern Type_sWord wswipc_receive_packet(IPC_SVC_CH ch);
extern Type_uByte wubRPCToMoudlue1(Type_uByte aubSrc, Type_uByte aubdest, Type_uHWord auhevent, void* data, Type_uHWord auhlength);
extern Type_sWord wswipc_packet_parser1(IPC_SVC_CH ch);
void wvdParseBfferInit(void);

#endif

