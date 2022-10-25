/****************************************************************************
 *   FileName    : HAL_Timer.c
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
*    2016/10/31     0.1            created                      hskim
*******************************************************************************/
#include <types.h>
#include "HAL_API.h"
#include "HAL_Timer.h"
#include "tcc_timer.h"
#include "bsp_int.h"

#define	HAL_TIMER_ST_OPENED		1
#define	HAL_TIMER_ST_NO_OPEN		0

static Hal_TimerType HalTimer_Main;

int32 Timer_Open(uint32 OPTIONS)
{
	int32 lRet;
	lRet = HAL_OK;
	//HalTimer_Main.gTimerStatus = HAL_TIMER_ST_OPENED; // CS : Useless Assignment 

	if (OPTIONS  != NULL )
	{
		lRet = HAL_ERR_NOT_SUPPORTED;
	}
	else
	{
		HalTimer_Main.gTimerOpenOptions  = OPTIONS;
		HalTimer_Main.gTimerStatus = HAL_TIMER_ST_OPENED;
		// open driver
	}

	return lRet;
}

int32 Timer_Close(void)
{
	int32 lRet;

	lRet = HAL_OK;

	if (HalTimer_Main.gTimerStatus != HAL_TIMER_ST_OPENED )
	{
		lRet = HAL_ERR_NOT_OPENED;
	}
	else
	{
		//if (lRet == HAL_OK) // CS : Redundant Condition 
		//{
			HalTimer_Main.gTimerStatus = HAL_TIMER_ST_NO_OPEN;
			//close driver
		//}
	}

	return lRet;
}

int32 Timer_Ioctl(uint32 OPT,  void* PAR1, void* PAR2, void* PAR3, void* PAR4)
{
	int32 lRet;
	lRet = HAL_OK;

	if (HalTimer_Main.gTimerStatus != HAL_TIMER_ST_OPENED )
	{
		lRet = HAL_ERR_NOT_OPENED;
	}
	else
	{
		switch (OPT)
		{
			case HAL_IO_TIMER_STARTTIMER:
				{/*PAR1 : channel, PAR2 : interval, PAR3: notification callback, PAR4 : parameter of callback*/
					int32 ch = -1, usec = -1;
					//timer_handler_t cb; // warning. unused varialbe
					if ((PAR1 != 0) && (PAR2 != 0))
					{
						ch = *((int32 *)PAR1);
						usec = *((int32 *)PAR2);
						if ( PAR3 != 0)
						{
							(void)tcc_timer_enable(ch, usec, (timer_handler_t)PAR3, PAR4);//QAC
							#ifdef TCC803x
							BSP_IntSrcEn(INT_TIMER_0 + ch);
							#endif
						}
						else
						{
							(void)tcc_timer_enable(ch, usec, 0, 0); //QAC
						}
					}
					else
					{
						lRet = HAL_ERR_BAD_PARAMETER;
					}
				}
				break;
			case HAL_IO_TIMER_STOPTIMER:
				{
					int32 ch = -1;
					if (PAR1 != 0)
					{
						ch = *((const int32 *)PAR1);
						if ( ch > 0)
						{
							tcc_timer_disable(ch);
						}
					}
				}
				break;
			default :
				lRet = HAL_ERR_NOT_SUPPORTED;
				break;
		}
	}

	return lRet;
}





