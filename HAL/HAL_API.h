/****************************************************************************
 *   FileName    : HAL_API.h
 *   Description : This file is HAL interface codes.
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
*    2011/03/23     0.1            created                      Jim
*******************************************************************************/

#ifndef __HAL_API_H__
#define __HAL_API_H__

#define HAL_ONE_ms 1000
#define HAL_ONE_us 1


/* HAL device list ID */
typedef uint32 HAL_ID;

/* HAL OBJ status */
typedef enum
{
	HAL_OBJST_INITIALIZED = 0,
	HAL_OBJST_DISABLE,
	HAL_OBJST_ENABLE,
	HAL_OBJST_SHARED
} HAL_OBJ_ST;

typedef void (*pfCallback)(uint8 data, uint8 error);

/* HAL Object */
typedef	int32 (InitFuncType)(void);
typedef	int32 (OpenFuncType)(uint32 OPTIONS);
typedef	int32 (WriteFuncType)(const uint8 *buff, uint32 size);//MISRA-C_11_01
typedef	int32 (ReadFuncType)(uint8 *buff, uint32 size);
typedef	int32 (CloseFuncType)(void);
typedef	int32 (IoctlFuncType)(uint32 OPT, void* PAR1, void* PAR2, void* PAR3, void* PAR4);

typedef	int32 (CallFuncType)(pfCallback fun);

typedef	int32 (DeinitFuncType)(void);

typedef struct HAL_DEV_FUNCS_{
	OpenFuncType * fpOpen; 	//Mandatory
	ReadFuncType * fpRead;		//Optionnal
	WriteFuncType * fpWrite;		//Optionnal
	CloseFuncType* fpClose;		//Optionnal
	IoctlFuncType * fpIoctl;		//Optionnal

	CallFuncType * fpCall;
} HAL_DEV_FUNCS;

typedef struct _HAL_OBJ
{
	HAL_ID mHAL_ID;
	HAL_OBJ_ST mHAL_ST;
	uint32 mSemaID;
	HAL_DEV_FUNCS * mIOFuncs;
} HAL_OBJ,*pHAL_OBJ;


#define HAL_ID_OF_LISTHEADER	1

/* HAL API return value */
#define	HAL_OK 				 			(0L)
#define	HAL_ERR_NOT_OPENED  			(-1L) 
#define	HAL_ERR_PHY 					(-2L) 	/* Physcal device driver error from MCAL*/
#define	HAL_ERR_OPENED_ALREADY		(-3L)
#define	HAL_ERR_OVER_SHARED			(-4L) 
#define	HAL_ERR_RESOURCE_CRASH		(-5L) 	/* Duplicate HW resource error from MCAL*/
#define	HAL_ERR_NOT_SUPPORTED		(-6L)	/* Some option or API is not support */
#define	HAL_ERR_DEVICE_BUSY			(-7L) 
#define	HAL_ERR_OS_API				(-8L	) 	/* Error for OS API function */
#define	HAL_ERR_HAL_DEV				(-9L	)	/* HAL devices error */
#define	HAL_ERR_NO_ACK 				(-10L) 	/* No ACK or NAK */
#define	HAL_ERR_TIME_OUT				(-11L)
#define	HAL_ERR_BAD_PARAMETER		(-12L)	/* Invalid parameter */
#define	HAL_ERR_DEVICE_NOT_FOUND	(-13L)	/* Device not found */
#define	HAL_ERR_INITIALIZED_ALREADY	(-14L)	/* Device is initialized already */
#define	HAL_ERR_NOT_INITIALIZED		(-15L)	/* Device is not initialized */

/* HAL Open options */
#define	HAL_OP_READ	0x00000001UL
#define	HAL_OP_WRITE	(0x00000001UL << 1UL)
#define	HAL_OP_RXIRQ	(0x00000001 << 2)
#define	HAL_OP_TXIRQ	(0x00000001 << 3)
#define	HAL_OP_RXDMA	(0x00000001 << 4)
#define	HAL_OP_TXDMA	(0x00000001 << 5)
#define	HAL_OP_DATAFULL_EVT	(0x00000001 << 6)
#define	HAL_OP_SLAVE_WORK	(0x00000001 << 7)

typedef 	void*	HAL_HANDLE;

/* HAL */
extern HAL_HANDLE HAL_Open(HAL_ID handleID, uint32 option);
extern int32 HAL_Write(HAL_HANDLE HALHandle, const uint8 *pBuff, uint32 size);
extern int32 HAL_Read(HAL_HANDLE HALHandle, uint8 *pBuff, uint32 size);
extern int32 HAL_Close(HAL_HANDLE HALHandle);
extern int32 HAL_Ioctl(HAL_HANDLE HALHandle, uint32 ulCommand, void* param1, void* param2, void* param3, void* param4);
extern int32 HAL_SetCallbackFunction(HAL_HANDLE HALHandle, pfCallback halCBFunc);
extern void HAL_InitHALObject(uint32 deviceID, const HAL_DEV_FUNCS * pIOFuncs);
#endif /* End of HAL API */
