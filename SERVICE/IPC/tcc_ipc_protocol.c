/****************************************************************************************
 *   FileName    : tcc_ipc_protocol.c
 *   Description : 
 ****************************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips Inc.
 *   All rights reserved 
 
This source code contains confidential information of Telechips.
Any unauthorized use without a written permission of Telechips including not limited 
to re-distribution in source or binary form is strictly prohibited.
This source code is provided ��AS IS�� and nothing contained in this source code 
shall constitute any express or implied warranty of any kind, including without limitation, 
any warranty of merchantability, fitness for a particular purpose or non-infringement of any patent, 
copyright or other third party intellectual property right. 
No warranty is made, express or implied, regarding the information��s accuracy, 
completeness, or performance. 
In no event shall Telechips be liable for any claim, damages or other liability arising from, 
out of or in connection with this source code or the use in the source code. 
This source code is provided subject to the terms of a Mutual Non-Disclosure Agreement 
between Telechips and Company.
*
****************************************************************************************/
#include <app_cfg.h>
#include <types.h>
#include "HAL_API.h"
#include "HAL_IPC.h"
#include "tcc_ipc_protocol.h"
#include "debug.h"
#include "tcc_ipc_typedef.h"
#include "rpc_if.h"


ipc_ch_info_ts ipc_ch_info[IPC_SVC_CH_MAX] = {
#ifdef IPC_CA53_S
	{IPC_SVC_CH_RESERVED0, NULL},
#endif
#ifdef IPC_CA53_NS
	{IPC_SVC_CH_MAIN, NULL},
#endif
#ifdef IPC_CA7_S
	{IPC_SVC_CH_RESERVED1, NULL},
#endif
#ifdef IPC_CA7_NS
	{IPC_SVC_CH_SUB, NULL},
#endif
#ifdef IPC_CM4_NS
	{IPC_SVC_CH_M4, NULL},
#endif
};

static struct ipc_cb_func cb_func[IPC_SVC_CH_MAX][IPC_PROCESS_NUM];
static Type_uByte wubpacket_receive[IPC_SVC_CH_MAX][IPC_MAX_PACKET_SIZE];

static struct ipc_cb_func_ts cb_func_ts[IPC_PROCESS_NUM];

static Type_uHWord nuhipc_calc_crc16(const Type_uByte *aubbuffer_p, Type_uWord auwlength, Type_uHWord auhinit);

void wvdipc_register_cb_func(IPC_SVC_CH ch, Type_uByte aubid, ipc_call_back func, void *arg1, void *arg2)
{
	cb_func[ch][aubid].ipc_cb_func = func;
	cb_func[ch][aubid].arg1 = arg1;
	cb_func[ch][aubid].arg2 = arg2;
}

Type_sByte wsbIPCRegisterFunc(Type_uByte aubsrc, wvdRPCCallback func)
{
	Type_sByte asbret;
	if(cb_func_ts[aubsrc].vdipc_cb_func == NULL)
	{
		cb_func_ts[aubsrc].vdipc_cb_func = func;
		//cb_func_ts[aubsrc].ubTaskId = aubTaskId;
		asbret = RPC_OK;
	}
	else
	{
		asbret = RPC_NG;
	}
	return asbret;
}

Type_uByte wubRPCToMoudlue1(Type_uByte aubSrc, Type_uByte aubdest, Type_uHWord auhevent, void* data, Type_uHWord auhlength)
{
	Type_uByte aubret;
	
	aubret = RES_OK;
	aubdest = aubdest - R5_MOULDE_START;
	cb_func[IPC_SVC_CH_MAIN][aubdest].ipc_cb_func(auhevent, data, auhlength);

	return aubret;
}

Type_uByte wubRPCToMoudlue(Type_uByte aubSrc, Type_uByte aubdest, Type_uHWord auhevent, void* data, Type_uHWord auhlength)
{
	Type_uByte aubret;
	aubret = RES_OK;

	aubdest = aubdest - R5_MOULDE_START;
	cb_func_ts[aubdest].vdipc_cb_func(aubSrc, auhevent, data, auhlength);

	return aubret;
}

Type_uByte wvdGetSrcId(Type_uByte aubTaskId)
{
	Type_uByte aubIndex = INDEX_INIT;
	Type_uByte aubSrcId = SRCID_INIT;
	for(aubIndex = INDEX_INIT; aubIndex < IPC_PROCESS_NUM; aubIndex++)
	{
		if(cb_func_ts[aubIndex].ubTaskId == aubTaskId)
		{
			aubSrcId = aubIndex + R5_MOULDE_START;
			
		}
		else
		{
			aubSrcId = IPC_ID_NOEXSIT;
		}
		break;
	}
	return aubSrcId;
}

static Type_uHWord nuhipc_calc_crc16(const Type_uByte *aubbuffer_p, Type_uWord auwlength, Type_uHWord auhinit)
{
	Type_uHWord auhcrcCode = auhinit;
	Type_uWord auwindex;
	static const Type_uHWord _crc16Table[CRCTBL_SIZE] = {
		0x0000U, 0x1021U, 0x2042U, 0x3063U, 0x4084U, 0x50a5U, 0x60c6U, 0x70e7U,
		0x8108U, 0x9129U, 0xa14aU, 0xb16bU, 0xc18cU, 0xd1adU, 0xe1ceU, 0xf1efU,
		0x1231U, 0x0210U, 0x3273U, 0x2252U, 0x52b5U, 0x4294U, 0x72f7U, 0x62d6U,
		0x9339U, 0x8318U, 0xb37bU, 0xa35aU, 0xd3bdU, 0xc39cU, 0xf3ffU, 0xe3deU,
		0x2462U, 0x3443U, 0x0420U, 0x1401U, 0x64e6U, 0x74c7U, 0x44a4U, 0x5485U,
		0xa56aU, 0xb54bU, 0x8528U, 0x9509U, 0xe5eeU, 0xf5cfU, 0xc5acU, 0xd58dU,
		0x3653U, 0x2672U, 0x1611U, 0x0630U, 0x76d7U, 0x66f6U, 0x5695U, 0x46b4U,
		0xb75bU, 0xa77aU, 0x9719U, 0x8738U, 0xf7dfU, 0xe7feU, 0xd79dU, 0xc7bcU,
		0x48c4U, 0x58e5U, 0x6886U, 0x78a7U, 0x0840U, 0x1861U, 0x2802U, 0x3823U,
		0xc9ccU, 0xd9edU, 0xe98eU, 0xf9afU, 0x8948U, 0x9969U, 0xa90aU, 0xb92bU,
		0x5af5U, 0x4ad4U, 0x7ab7U, 0x6a96U, 0x1a71U, 0x0a50U, 0x3a33U, 0x2a12U,
		0xdbfdU, 0xcbdcU, 0xfbbfU, 0xeb9eU, 0x9b79U, 0x8b58U, 0xbb3bU, 0xab1aU,
		0x6ca6U, 0x7c87U, 0x4ce4U, 0x5cc5U, 0x2c22U, 0x3c03U, 0x0c60U, 0x1c41U,
		0xedaeU, 0xfd8fU, 0xcdecU, 0xddcdU, 0xad2aU, 0xbd0bU, 0x8d68U, 0x9d49U,
		0x7e97U, 0x6eb6U, 0x5ed5U, 0x4ef4U, 0x3e13U, 0x2e32U, 0x1e51U, 0x0e70U,
		0xff9fU, 0xefbeU, 0xdfddU, 0xcffcU, 0xbf1bU, 0xaf3aU, 0x9f59U, 0x8f78U,
		0x9188U, 0x81a9U, 0xb1caU, 0xa1ebU, 0xd10cU, 0xc12dU, 0xf14eU, 0xe16fU,
		0x1080U, 0x00a1U, 0x30c2U, 0x20e3U, 0x5004U, 0x4025U, 0x7046U, 0x6067U,
		0x83b9U, 0x9398U, 0xa3fbU, 0xb3daU, 0xc33dU, 0xd31cU, 0xe37fU, 0xf35eU,
		0x02b1U, 0x1290U, 0x22f3U, 0x32d2U, 0x4235U, 0x5214U, 0x6277U, 0x7256U,
		0xb5eaU, 0xa5cbU, 0x95a8U, 0x8589U, 0xf56eU, 0xe54fU, 0xd52cU, 0xc50dU,
		0x34e2U, 0x24c3U, 0x14a0U, 0x0481U, 0x7466U, 0x6447U, 0x5424U, 0x4405U,
		0xa7dbU, 0xb7faU, 0x8799U, 0x97b8U, 0xe75fU, 0xf77eU, 0xc71dU, 0xd73cU,
		0x26d3U, 0x36f2U, 0x0691U, 0x16b0U, 0x6657U, 0x7676U, 0x4615U, 0x5634U,
		0xd94cU, 0xc96dU, 0xf90eU, 0xe92fU, 0x99c8U, 0x89e9U, 0xb98aU, 0xa9abU,
		0x5844U, 0x4865U, 0x7806U, 0x6827U, 0x18c0U, 0x08e1U, 0x3882U, 0x28a3U,
		0xcb7dU, 0xdb5cU, 0xeb3fU, 0xfb1eU, 0x8bf9U, 0x9bd8U, 0xabbbU, 0xbb9aU,
		0x4a75U, 0x5a54U, 0x6a37U, 0x7a16U, 0x0af1U, 0x1ad0U, 0x2ab3U, 0x3a92U,
		0xfd2eU, 0xed0fU, 0xdd6cU, 0xcd4dU, 0xbdaaU, 0xad8bU, 0x9de8U, 0x8dc9U,
		0x7c26U, 0x6c07U, 0x5c64U, 0x4c45U, 0x3ca2U, 0x2c83U, 0x1ce0U, 0x0cc1U,
		0xef1fU, 0xff3eU, 0xcf5dU, 0xdf7cU, 0xaf9bU, 0xbfbaU, 0x8fd9U, 0x9ff8U,
		0x6e17U, 0x7e36U, 0x4e55U, 0x5e74U, 0x2e93U, 0x3eb2U, 0x0ed1U, 0x1ef0U
	};
	
	if (aubbuffer_p != NULL) {
		Type_uWord auwtemp;
		for (auwindex = INDEX_INIT; auwindex < auwlength; auwindex++) {
			auwtemp = ((auhcrcCode >> 8) ^ aubbuffer_p[auwindex]) & 0xff;
			//auwtemp = DECODE8((DECODE16(auhcrcCode) ^ aubbuffer_p[auwindex]));
			auhcrcCode = _crc16Table[auwtemp] ^ (ENCODE16(auhcrcCode));
		}
	}

	return auhcrcCode;
}

Type_sWord wswipc_packet_parser1(IPC_SVC_CH ch)
{
	Type_sWord aswret = IPC_SUCCESS;
	Type_uHWord auhcrc1, auhcrc2, auhcmd2, auhtotalLength, auhdataLength;
	Type_uByte aubdest;
	auhdataLength = (Type_uHWord)((((Type_uHWord)wubpacket_receive[ch][7]) << 8) | ((Type_uHWord)wubpacket_receive[ch][8]));
	if(auhdataLength == LENGTH_INIT) {
		auhdataLength++; //dummy data length	
	}
	
	auhtotalLength = (Type_uHWord)(IPC_PACKET_PREPARE_SIZE + auhdataLength);
	if (auhtotalLength < PACKET_MAXSIZE) {
		auhcrc1 = nuhipc_calc_crc16(&wubpacket_receive[ch][0], auhtotalLength, CRC_INIT);
		auhcrc2 = (Type_uHWord)((Type_uHWord)((Type_uHWord)wubpacket_receive[ch][auhtotalLength] << 8) | (Type_uHWord)(wubpacket_receive[ch][auhtotalLength+1]));

		if (auhcrc1 == auhcrc2)
		{
			if ((wubpacket_receive[ch][0] == IPC_SYNC) && (wubpacket_receive[ch][1] == IPC_START1) && (wubpacket_receive[ch][2] == IPC_START2)) {
				aubdest = wubpacket_receive[ch][3] - R5_MOULDE_START;
				auhcmd2 = (Type_uHWord)((Type_uHWord)((Type_uHWord)wubpacket_receive[ch][5] << 8) | (Type_uHWord)wubpacket_receive[ch][6]);

				if(cb_func[ch][aubdest].ipc_cb_func != NULL) // null pointer checking
				{
					cb_func[ch][aubdest].ipc_cb_func(auhcmd2, &wubpacket_receive[ch][9], auhdataLength);
				}
				else
				{
					//printf("Callback Func NULL:[%s]: <%s:%d>\n", __func__, __FILE__, __LINE__);
				}	
				
			}
			else
			{
				//printf("Package Head Error:[%s]: <%s:%d>\n", __func__, __FILE__, __LINE__);
			}
			
		}
		else
		{
			//printf("Checksum Error:[%s]: <%s:%d>\n", __func__, __FILE__, __LINE__);
		}
		
	} 
	else 
	{
		aswret = IPC_ERR_READ;
	}

	return aswret;
}

Type_sWord wswipc_packet_parser(IPC_SVC_CH ch)
{
	Type_sWord aswret = IPC_SUCCESS;
	Type_uHWord auhcrc1, auhcrc2, auhcmd2, auhtotalLength, auhdataLength;
	Type_uByte aubdest;
	auhdataLength = (Type_uHWord)((((Type_uHWord)wubpacket_receive[ch][7]) << 8) | ((Type_uHWord)wubpacket_receive[ch][8]));
	if(auhdataLength == LENGTH_INIT) {
		auhdataLength++; //dummy data length	
	}
	
	auhtotalLength = (Type_uHWord)(IPC_PACKET_PREPARE_SIZE + auhdataLength);
	if (auhtotalLength < PACKET_MAXSIZE) {
		auhcrc1 = nuhipc_calc_crc16(&wubpacket_receive[ch][0], auhtotalLength, CRC_INIT);
		auhcrc2 = (Type_uHWord)((Type_uHWord)((Type_uHWord)wubpacket_receive[ch][auhtotalLength] << 8) | (Type_uHWord)(wubpacket_receive[ch][auhtotalLength+1]));

		if (auhcrc1 == auhcrc2)
		{
			if ((wubpacket_receive[ch][0] == IPC_SYNC) && (wubpacket_receive[ch][1] == IPC_START1) && (wubpacket_receive[ch][2] == IPC_START2)) {
				aubdest = wubpacket_receive[ch][3] - R5_MOULDE_START;
				auhcmd2 = (Type_uHWord)((Type_uHWord)((Type_uHWord)wubpacket_receive[ch][5] << 8) | (Type_uHWord)wubpacket_receive[ch][6]);
				DEBUG_LIBARPC_PRINTF("aubsrc:%x aubdest:%x auhevent:%x auhlength:%d\n",wubpacket_receive[ch][4], wubpacket_receive[ch][3], auhcmd2, auhdataLength);
				if(cb_func_ts[aubdest].vdipc_cb_func != NULL) // null pointer checking
				{
					cb_func_ts[aubdest].vdipc_cb_func(wubpacket_receive[ch][4], auhcmd2, &wubpacket_receive[ch][9], auhdataLength);
				}
				else
				{
					printf("Callback Func NULL:[%s]: <%s:%d>\n", __func__, __FILE__, __LINE__);
				}	
				
			}
			else
			{
				printf("Package Head Error:[%s]: <%s:%d>\n", __func__, __FILE__, __LINE__);
			}
			
		}
		else
		{
			printf("Checksum Error:[%s]: <%s:%d>\n", __func__, __FILE__, __LINE__);
		}
		
	} 
	else 
	{
		aswret = IPC_ERR_READ;
	}

	return aswret;
}

Type_sWord wswipc_send_packet(IPC_SVC_CH ch, Type_uHWord auhcmd1, Type_uHWord auhcmd2, const Type_uByte* data, Type_uHWord auhlength)
{
	Type_sWord aswhalRet, aswret;
	Type_uWord nuwcmd[CMDSIZE];
	Type_uByte aubindex;

	aswret = IPC_ERR_COMMON;
	aswhalRet = IPC_ERR_COMMON;

	aubindex = INDEX_INIT;
	if (auhlength >= (IPC_MAX_PACKET_SIZE - IPC_PACKET_PREFIX_SIZE)) {
		aswret = IPC_ERR_ARGUMENT;
	} 
	else 
	{
		// packaging the ipc command
		nuwcmd[0] = IPC_APP_ID_IPC; /* IPC */
		nuwcmd[1] = VALUE_0; /* sequence ID */
		nuwcmd[2] = (WRITE_CMD<<16)|(IPC_WRITE);
		nuwcmd[3] = VALUE_0;
		nuwcmd[4] = ((Type_uWord)ENCODE24(auhcmd1)) | ((Type_uWord)ENCODE8(auhcmd2));
		nuwcmd[5] = VALUE_0;
		nuwcmd[6] = VALUE_0;
		nuwcmd[7] = VALUE_0;

		// send the ipc packet
		for(aubindex = INDEX_INIT; (aubindex < RETRY_COUNT)&&(aswhalRet != HAL_OK); aubindex++)
		{
			aswhalRet = HAL_Ioctl(ipc_ch_info[ch].halHandle, IOCTL_IPC_WRITE, (void *)&nuwcmd[0], (void *)data, (void *)&auhlength, VALUE_0);
		}
		if(aswhalRet == HAL_OK) 
		{
			aswret = IPC_SUCCESS;
		}
		else if(aswhalRet == HAL_ERR_OS_API)
		{
			aswret = IPC_ERR_BUSY;
		}
		else
		{
			aswret = IPC_ERR_WRITE;
		}
	}

	return aswret;
}

Type_uByte *wubpackage(Type_uHWord auhcmd1, Type_uHWord auhcmd2, const Type_uByte* data, Type_uHWord auhlength)
{
	Type_uHWord auhcrc;
	Type_sWord aswcnt;
	Type_uWord auwpacket_size;
	static Type_uByte nubpacket_send[IPC_MAX_PACKET_SIZE];

	aswcnt = INDEX_INIT;
	auwpacket_size = VALUE_0;
	auhcrc = VALUE_0;

	// packaging the ipc packet
	nubpacket_send[0] = IPC_SYNC;
	nubpacket_send[1] = IPC_START1;
	nubpacket_send[2] = IPC_START2;
	nubpacket_send[3] = (Type_uByte)((auhcmd1 >> 8) & 0xFF);
	nubpacket_send[4] = (Type_uByte)(auhcmd1 & 0xFF);
	nubpacket_send[5] = (Type_uByte)((auhcmd2 >> 8) & 0xFF);
	nubpacket_send[6] = (Type_uByte)(auhcmd2 & 0xFF);
	nubpacket_send[7] = (Type_uByte)((auhlength >> 8) & 0xFF);
	nubpacket_send[8] = (Type_uByte)(auhlength & 0xFF);

	if ((auhlength == LENGTH_INIT) || (data == NULL)) 
	{
		nubpacket_send[IPC_PACKET_PREPARE_SIZE] = VALUE_0; //dummy data
		aswcnt = CRC_OUTSIDE; //CS:Useless Assignment - Remove initialized value //dummy data length
		auwpacket_size = DUMMY_DATA_PACKET;
	} 
	else 
	{
		for (aswcnt = INDEX_INIT; aswcnt < auhlength; aswcnt++) 
		{
			nubpacket_send[IPC_PACKET_PREPARE_SIZE+aswcnt] = data[aswcnt];
		}
		auwpacket_size = IPC_PACKET_PREFIX_SIZE + auhlength;
	}

		auhcrc = nuhipc_calc_crc16(&nubpacket_send[0], (auwpacket_size - CRC_COUNT), CRC_INIT);
		nubpacket_send[IPC_PACKET_PREPARE_SIZE+aswcnt] =  (Type_uByte)((auhcrc >> 8) & 0xFF);
		nubpacket_send[IPC_PACKET_PREPARE_SIZE+aswcnt+1] = (Type_uByte)(auhcrc & 0xFF);

	return nubpacket_send;
}

Type_sWord wswipc_receive_packet(IPC_SVC_CH ch)
{
	Type_sWord aswret = IPC_ERR_COMMON;
	Type_uHWord auhpacket_size = IPC_PACKET_PREPARE_SIZE;

	aswret = HAL_Read(ipc_ch_info[ch].halHandle, &wubpacket_receive[ch][0], auhpacket_size);
	//printf("aswret:%d [%s]: <%s:%d>\n",aswret, __func__, __FILE__, __LINE__);
	if (aswret > DUMMY_DATA_SIZE) {
		auhpacket_size = (Type_uHWord)((Type_uHWord)((Type_uHWord)wubpacket_receive[ch][7] << 8) | (Type_uHWord)(wubpacket_receive[ch][8]));
		if (auhpacket_size > (IPC_MAX_PACKET_SIZE - IPC_PACKET_PREPARE_SIZE - CRC_COUNT/* crc byte */))		
		{
			aswret = IPC_ERR_READ; //IPC read size error		
		}
		else
		{
			if (auhpacket_size < PACKET_MINSIZE) {
				auhpacket_size = PACKET_MINSIZE + CRC_COUNT; // 1 is data length , 2 is crc byte
			} else {
				auhpacket_size += CRC_COUNT; // 2 is crc byte
			}
			
			(void) HAL_Read(ipc_ch_info[ch].halHandle, &wubpacket_receive[ch][9], auhpacket_size);
			aswret = auhpacket_size + IPC_PACKET_PREPARE_SIZE;
			//printf("aswret:%d [%s]: <%s:%d>\n",aswret, __func__, __FILE__, __LINE__);
		}
	}

	return aswret;
}

void wvdParseBfferInit(void)
{
	(void)OSAL_MemSet(&wubpacket_receive[IPC_SVC_CH_MAIN][VALUE_0], VALUE_0, IPC_MAX_PACKET_SIZE);
	(void)OSAL_MemSet(&wubpacket_receive[IPC_SVC_CH_SUB][VALUE_0], VALUE_0, IPC_MAX_PACKET_SIZE);
}
