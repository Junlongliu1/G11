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

#ifndef __HAL_IPC_H__
#define __HAL_IPC_H__

#include "tcc_ipc.h"

#define IPC_NO 0
#define IPC_YES 1
#define VALUE_0 0

enum
{
	IOCTL_IPC_WRITE = 0,
	IOCTL_IPC_READ = 1,
	IOCTL_IPC_SET_PARAM = 2,
	IOCTL_IPC_GET_PARAM = 3,
	IOCTL_IPC_PING_TEST = 4,
	IOCTL_IPC_FLUSH = 5,
	IOCTL_IPC_ISREADY = 6,
};

#endif /* __HAL_IPC_H__ */

