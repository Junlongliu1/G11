/****************************************************************************
 *   FileName    : HAL_Console.c
 *   Description : HAL Console device using UART
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
*    2016/10/31     0.1            created                      hskim
*******************************************************************************/

#include <types.h>
#include <HAL_API.h>
#include <HAL_devices.h>
#include "HAL_Console.h"
#include "uart.h"
#include <bsp_int.h>

#define	HAL_TIMER_ST_OPENED		1
#define	HAL_TIMER_ST_NO_OPEN		0
#ifdef USE_HAL_CONSOLE
static Hal_ConsoleType HalConsole_Main;
int32 Console_Open(uint32 OPTIONS)
{
	int32 lRet;
	lRet = HAL_OK;
	//HalConsole_Main.gConsoleStatus = HAL_TIMER_ST_OPENED; //CS : Useless Assignment 

	if (OPTIONS  != NULL )
	{
		lRet = HAL_ERR_NOT_SUPPORTED;
	}
	else
	{
		HalConsole_Main.gConsoleOpenOptions  = OPTIONS;
		HalConsole_Main.gUartChannel  = DEBUG_UART;
		HalConsole_Main.gConsoleStatus = HAL_TIMER_ST_OPENED;
	}

	return lRet;
}

int32 Console_Read(uint8 *buff, uint32 size)
{
	int32 lRet;

	lRet = HAL_OK;

	if (HalConsole_Main.gConsoleStatus != HAL_TIMER_ST_OPENED )
	{
		lRet = HAL_ERR_NOT_OPENED;
	}
	else
	{
		if ( buff !=0)
		{
			if ( size == 1)
			{
				int32 c;
				c = console_getc(HalConsole_Main.gUartChannel, 0);
				if ( c > 0) {
					*buff = (uint8)(c&0xFF);
				}
				else {
					lRet = HAL_ERR_NO_ACK;
				}
			}
			else
			{
				lRet = HAL_ERR_NOT_SUPPORTED;
			}
		}
		else
		{
			lRet = HAL_ERR_BAD_PARAMETER;
		}
	}

	return lRet;
}

int32 Console_Write(const uint8 *buff, uint32 size)
{
	int32 lRet;

	lRet = HAL_OK;

	if (HalConsole_Main.gConsoleStatus != HAL_TIMER_ST_OPENED )
	{
		lRet = HAL_ERR_NOT_OPENED;
	}
	else
	{
		/* Codesonar : Buffer Overrun
		uint32 remained;
		remained = size;
		
		while((*buff != 0) && (remained!=0))
		{
			remained--;
			(void) console_putc(HalConsole_Main.gUartChannel, (int8)*buff);
			buff++;
		}
		*/
		uint32 i;
		uint8 local_putc; //QACMsg(4:5115) The names of standard library macros, objects and functions shall not be reused. MISRA-C:2004 Rule 20.2
		for(i = 0; i < size; i++)
		{
			if ( ((const uint8 *)buff + i) != 0)
			{
				local_putc = *((const uint8 *)buff + i); 
				(void) console_putc(HalConsole_Main.gUartChannel, local_putc);
			}
		}
	}

	return lRet;
}

int32 Console_Close(void)
{
	int32 lRet;

	lRet = HAL_OK;

	if (HalConsole_Main.gConsoleStatus != HAL_TIMER_ST_OPENED )
	{
		lRet = HAL_ERR_NOT_OPENED;
	}
	else
	{
		//if (lRet == HAL_OK) //CS : Redundant Condition
		//{
			HalConsole_Main.gConsoleStatus = HAL_TIMER_ST_NO_OPEN;
			//close driver
		//}
	}

	return lRet;
}

int32 Console_Ioctl(uint32 OPT,  void* PAR1, void* PAR2, void* PAR3, void* PAR4)
{
	int32 lRet;

	lRet = HAL_OK;

	if (HalConsole_Main.gConsoleStatus != HAL_TIMER_ST_OPENED )
	{
		lRet = HAL_ERR_NOT_OPENED;
	}
	else
	{
		switch (OPT)
		{
			case HAL_IO_CONSOLE_SET_BAUDRATE:
				break;
			default:
				break;
		}
	}

	return lRet;
}
#endif
