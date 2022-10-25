/****************************************************************************
 *   FileName    : HAL_Timer.h
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
*    2011/03/23     0.1            created                      Jim
*******************************************************************************/

#ifndef __HAL_CONSOLE_H__
#define __HAL_CONSOLE_H__

typedef struct
{
	uint32 gConsoleStatus;
	uint32 gUartChannel;
	uint32 gConsoleOpenOptions;
} Hal_ConsoleType;

enum
{
	HAL_IO_CONSOLE_SET_BAUDRATE = 1,
};


extern int32 Console_Open(uint32 OPTIONS);
extern int32 Console_Read(uint8 *buff, uint32 size);
extern int32 Console_Write(const uint8 *buff, uint32 size);
extern int32 Console_Close(void);
extern int32 Console_Ioctl(uint32 OPT, void* PAR1, void* PAR2, void* PAR3, void* PAR4);
#endif /* __HAL_CONSOLE_H__ */

