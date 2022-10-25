/****************************************************************************
 *   FileName    : HAL_IPC_CA53.h
 *   Description : 
 ****************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips Inc.
 *   All rights reserved 
 
This source code contains confidential information of Telechips.
Any unauthorized use without a written permission of Telechips including not limited to re-
distribution in source or binary form is strictly prohibited.
This source code is provided "AS IS" and nothing contained in this source code shall 
constitute any express or implied warranty of any kind, including without limitation, any warranty 
of merchantability, fitness for a particular purpose or non-infringement of any patent, copyright 
or other third party intellectual property right. No warranty is made, express or implied, 
regarding the information's accuracy, completeness, or performance. 
In no event shall Telechips be liable for any claim, damages or other liability arising from, out of 
or in connection with this source code or the use in the source code. 
This source code is provided subject to the terms of a Mutual Non-Disclosure Agreement 
between Telechips and Company.
*
****************************************************************************/
/******************************************************************************
*  yyyy/mm/dd     ver            descriptions                Author
*	---------	--------   ---------------       -----------------
*    2018/06/11     0.1            created                      bskim
*******************************************************************************/

#ifndef __HAL_IPC_CA7_H__
#define __HAL_IPC_CA7_H__
#include "apn_basic_type.h"
//#include "tcc_ipc_typedef.h"

typedef struct
{
	Type_uWord uwHALIPCCA7Status;
	
	Type_uWord uwHALIPCCA7IsOpen;
	Type_uWord uwHALIPCCA7CallCount;
} Hal_IPCCA7Type;

extern int32 IPCCA7_Open(uint32 OPTIONS);
extern int32 IPCCA7_Read(uint8 *buff, uint32 size);
extern int32 IPCCA7_Write(const uint8 *buff, uint32 size);
extern int32 IPCCA7_Close(void);
extern int32 IPCCA7_Ioctl(uint32 OPT, void* PAR1, void* PAR2, void* PAR3, void* PAR4); //QAC
void wvdPCCA7_MainInit(void);

#endif /* __HAL_IPC_CA7_H__ */

