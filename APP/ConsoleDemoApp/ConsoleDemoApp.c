/****************************************************************************
 *   FileName    : ConsoleDemoApp.c
 *   Description : Demo Application for console function
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


#define  APP_MODULE
#include <types.h>
#include <osal.h>
#include <app_cfg.h>
#if (SERVICE_CONSOLE_EN ==1)
#include "debug.h"
#include <bsp.h>
#include <bsp_int.h>
#include <Console.h>
#include <tcc_wdt.h> //QAC
#include <HAL_API.h>
#include <HAL_IPC.h>
#include <os_framework.h>
#include "ConsoleDemoApp.h"
#include "tcc_ipc_protocol.h"
#include <reg_phys.h>
#if defined(TCC803x)
#include "fwug.h"
#endif
#include "string.h"
#include "task.h"
#include "generic.h"
#include "gpio.h"
#include "pm.h"

#include "os_framework_cfg.h"
#include "Eepromcontrol.h"

#define CONSOLE_DEMO_NO_INIT 	0
#define CONSOLE_DEMO_INIT 		1
#define CONSOLE_DEMO_PREPAIRED 	2
#define CONSOLE_DEMO_RUN 		3
#define CONSOLE_DEMO_STOP 		4
#define CONSOLE_DEMO_FINISH 	5

#define TEST_STR_LEN 20 //Compile Warning

#define CMD_NAME "cmd"/*console - >cmd*/

static int8 ConsoleDemoState = CONSOLE_DEMO_NO_INIT;
//static COMMANDLIST gConsoleDemoCommandList; //QAC -File scope static
//Static APIs
static void ConsoleDemoCommandList(uint8 argc, void * argv[]);
static void Test_Timestat(void);
static void Test_Tasklist(void);
static void Test_heap(void);
static void Test_queue(void);
static void Test_gpio(void);
#ifdef PM_ST_EN
static void ConsolePowerSt(void * argv[]);
#endif

//
#if (SERVICE_TEST_EN == 1u)
void CYC_TEST(void)
{
	OS_SetEventFlag(FLG_ID_TEST, TEST_FLGPTN_WDTIRQ);
}
#endif

static void ConsoleDemoCommandList(uint8 argc, void * argv[]) //QAC const for argv[]
{
	if (OSAL_StrCmp(argv[0],CMD_NAME)==0)
	{
		if ( (argc == 2) && (argv[1] != 0))
		{
			if(OSAL_StrCmp(argv[1],"start")==0)
			{
				ConsoleWriteBytes((const uint8 *)"Console Demo Start\r\n",20);
				ConsoleDemoState = CONSOLE_DEMO_RUN;
			}
			else if (OSAL_StrCmp(argv[1],"stop")==0)
			{
				ConsoleWriteBytes((const uint8 *)"Console Demo Stop\r\n",19);
				ConsoleDemoState = CONSOLE_DEMO_STOP;
			}
			else if (OSAL_StrCmp(argv[1],"end")==0)
			{
				ConsoleWriteBytes((const uint8 *)"Console Demo End\r\n",18);
				ConsoleDemoState = CONSOLE_DEMO_FINISH;
			}			
			else if(OSAL_StrCmp(argv[1],"reset")==0) // system reset test
			{
				ConsoleWriteBytes((const uint8 *)"Do Reset after 1 Sec !!!\r\n",26);
				OSAL_TaskDelay(1000);
				(void)tcc_wdt_init();
				tcc_wdt_reset();
			}
			else if(OSAL_StrCmp(argv[1],"halt")==0) // system halt test
			{
				ConsoleWriteBytes((const uint8 *)"System Halt 1 second after !!!\r\n",32);
				OSAL_TaskDelay(1000);
				System_Halt();
			}
			else if(OSAL_StrCmp(argv[1], "fwug_test")==0)
			{
				FWUG_CopyAndJump();
			}
			
			#if (SERVICE_TEST_EN == 1u)
			else if(OSAL_StrCmp(argv[1], "stack")==0)
			{
				Test_Tasklist();
			}
			
			else if(OSAL_StrCmp(argv[1], "cpu")==0)
			{
				Test_Timestat();
			}
			else if(OSAL_StrCmp(argv[1], "queue")==0)
			{
				Test_queue();
			}

			else if(OSAL_StrCmp(argv[1], "heap")==0)
			{
				Test_heap();
			}

			else if(OSAL_StrCmp(argv[1], "gpio")==0)
			{
				Test_gpio();
			}
			#endif
			else //QAC if - else if - else
			{
				; // no statement
			}
		}
		#if (SERVICE_TEST_EN == 1u)
		else if ( (argc == 3) && (argv[1] != 0))
		{
			if( OSAL_StrCmp(argv[1], "addr")==0){
				uint32	data;
				uint32	addr;
				
				addr = (uint32)(CSL_StringToHex(argv[2]));
				if ( (addr >= 0x1b000000) && (addr <= 0x1bb00000 )){
					data = readl(addr);
					printf(" reg_addr data = 0x%x",data);
				}else{
					ConsoleWriteBytes((const uint8 *)"ADDR NG!\r\n",11);
				}
			}
			else if( OSAL_StrCmp(argv[1], "rt")==0){

				if(OSAL_StrCmp(argv[2], "stop")==0)
				{
					OS_StopTimer(OS_TIMER_ID_TEST);
					
				}else{
					Type_uWord	auwGTimer = 0;
				
					auwGTimer = CSL_StringToDec(argv[2]);

					if ( auwGTimer >  0 ){
						OS_TimerChangePeriod(OS_TIMER_ID_TEST, auwGTimer);
					}else{
						ConsoleWriteBytes((const uint8 *)"FreeRTOS Timer is 0!\r\n",18);
					}
				}
			}
			else if(OSAL_StrCmp(argv[1], "ipc_ping")==0)
			{
				Type_sWord halRet;
				
				if(OSAL_StrCmp(argv[2], "ca53")==0)
				{
					#ifdef IPC_CA53_NS
					halRet = HAL_Ioctl(ipc_ch_info[IPC_SVC_CH_MAIN].halHandle, IOCTL_IPC_PING_TEST, 0, 0, 0, 0);
					if(halRet == HAL_OK) {
						ConsoleWriteBytes((const uint8 *)"IPC ping test OK!\r\n",19);
					} else {
						ConsoleWriteBytes((const uint8 *)"IPC ping test FAIL!\r\n",21);
					}
					#else
					ConsoleWriteBytes((const uint8 *)"IPC CA53 is not supported!\r\n",28);
					#endif
				}
				if(OSAL_StrCmp(argv[2], "ca7")==0)
				{
					#ifdef IPC_CA7_NS
					halRet = HAL_Ioctl(ipc_ch_info[IPC_SVC_CH_SUB].halHandle, IOCTL_IPC_PING_TEST, 0, 0, 0, 0);
					if(halRet == HAL_OK) {
						ConsoleWriteBytes((const uint8 *)"IPC ping test OK!\r\n",19);
					} else {
						ConsoleWriteBytes((const uint8 *)"IPC ping test FAIL!\r\n",21);
					}
					#else
					ConsoleWriteBytes((const uint8 *)"IPC CA7 is not supported!\r\n",27);
					#endif
				}
				
			}
			else if(OSAL_StrCmp(argv[1], "er")==0)
			{
				Type_uByte	ReadData[EEPROM_LEN];
				Type_uHWord	ReadAddr;

				ReadAddr = (Type_uHWord)(CSL_StringToHex(argv[2]));

				if( wubEEP_LineToolReadEEP(ReadAddr,EEPROM_LEN,&ReadData[0]) == RES_OK){
					printf(" EEPROM Read Data 0x%x,0x%x",ReadData[0],ReadData[1]);
				}else{
					printf(" EEPROM Read NG ");
				}

			}
#ifdef PM_ST_EN
			else if(OSAL_StrCmp(argv[1], "st_power")==0)
			{
				ConsolePowerSt(argv);
			}
#endif

			else //QAC if - else if - else
			{
				; // no statement
			}
		}
		else if ( (argc == 4) && (argv[1] != 0)){
			if(OSAL_StrCmp(argv[1], "ew")==0)
			{
				Type_uHWord	WriteAddr;
				Type_uHWord	WriteData;
				Type_uByte	Writedata[EEPROM_LEN];
				
				WriteAddr = (uint16)(CSL_StringToHex(argv[2]));
				WriteData = (uint16)(CSL_StringToHex(argv[3]));
				
				Writedata[0]= (WriteData >> 8) & 0xFF;
				Writedata[1] = (WriteData ) & 0xFF;
					
				if( wubEEP_LineToolWriteEEP(WriteAddr, EEPROM_LEN, &Writedata[0]) == RES_OK){
					printf(" EEPROM Write OK");
				}else{
					printf(" EEPROM Write NG");
				}

			}
			else
			{
				; // no statement
			}
		}
		#endif
		else //QAC if - else if - else
		{
			; // no statement
		}

	}
}

void ConsoleDemoTask(void *p_arg)
{
	int8 serviceState;
	//int32 ret32s;//warning. set but not used
	uint16 delay =1;
	static COMMANDLIST gConsoleDemoCommandList;//QAC -File scope static
	
	while (BSP_TRUE) {
		switch(ConsoleDemoState)
		{
			case CONSOLE_DEMO_NO_INIT:
				//wait for running console service.
				serviceState = ConsoleServiceState();
				if ( serviceState == 1)
				{ //QAC
					ConsoleDemoState = CONSOLE_DEMO_INIT;
				}
				break;
			case CONSOLE_DEMO_INIT:
				OSAL_MemSet(&gConsoleDemoCommandList,0x00,sizeof(COMMANDLIST));
				(void)AddConsoleCommand(&gConsoleDemoCommandList, (uint8 *)CMD_NAME, (CmdFuncType *)ConsoleDemoCommandList,(uint8 *)"Console Demo Command");
				ConsoleDemoState = CONSOLE_DEMO_PREPAIRED;
				break;
			default:
				break;
		}
		OSAL_TaskDelay(delay);
	}
}

static void Test_Tasklist(void)
{
	OS_VoidPtr_t	avdMemAddr_p;
	OS_ErrCode_t	aswGetMpfResult;

	avdMemAddr_p = NULL;

	aswGetMpfResult = OS_GetFMemoryBlock(&avdMemAddr_p, 2560);

	if (aswGetMpfResult == OS_ERR_OK){
		wvdGen_MemSet(avdMemAddr_p, 0, 2560);

	}else{
		if ( NULL != avdMemAddr_p ) {
			(void)OS_ReleaseFMemoryBlock(avdMemAddr_p);
		}
		return;
	}
	
	ConsoleWriteBytes((const uint8 *)TASK_LIST_TITLE,sizeof(TASK_LIST_TITLE));

	vTaskList(avdMemAddr_p);

	ConsoleWriteBytes((const uint8 *)avdMemAddr_p,strlen(avdMemAddr_p));
	
	(void)OS_ReleaseFMemoryBlock(avdMemAddr_p);
}

static void Test_Timestat(void)
{

	OS_VoidPtr_t	avdMemAddr_p;
	OS_ErrCode_t	aswGetMpfResult;

	avdMemAddr_p = NULL;

	aswGetMpfResult = OS_GetFMemoryBlock(&avdMemAddr_p, 2560);

	if (aswGetMpfResult == OS_ERR_OK){
		wvdGen_MemSet(avdMemAddr_p, 0, 2560);

	}else{
		if ( NULL != avdMemAddr_p ) {
			(void)OS_ReleaseFMemoryBlock(avdMemAddr_p);
		}
		ConsoleWriteBytes((const uint8 *)"\r\nmalloc NG\r\n",13UL);
		return;
	}

	ConsoleWriteBytes((const uint8 *)TASK_STA_TITLE,sizeof(TASK_STA_TITLE));

	vTaskGetRunTimeStats(avdMemAddr_p);

	ConsoleWriteBytes((const uint8 *)avdMemAddr_p,strlen(avdMemAddr_p));
	
	(void)OS_ReleaseFMemoryBlock(avdMemAddr_p);
}

static void Test_heap(void)
{

	OS_MemSize_t auwMinimumHeapSize = xPortGetMinimumEverFreeHeapSize();

	OS_MemSize_t auwHeapSize = xPortGetFreeHeapSize();

	printf("\n  minimal space : %d,free space : %d",auwMinimumHeapSize,auwHeapSize);

}

static void Test_queue(void)
{
	Type_uWord auwqueue;
	OS_ErrCode_t aswErrCode = OS_ERR_OK;

	for(int i=0;i < 17;i++){ /* 17 is OS_cfg_queue size (os_framework_cfg.c)*/
		aswErrCode = OS_MessagesWaitingQueue(i,&auwqueue);
		if( aswErrCode == OS_ERR_OK){
			printf("\n Queue ID= %d,The number of queues waiting  = %d",i,auwqueue);
		}else{
			printf("\n Test_Queue ID Err");
		}
	}
	
}

static void Test_gpio(void)
{
	Type_uByte aubIndex;
	Type_uByte aubSize;
	Type_uWord Prot;
	aubSize = GetGpioTableSize();
	for( aubIndex= 0; aubIndex < aubSize ; aubIndex++){
		Prot = GetGpioTablePort(aubIndex);
		gpio_scan(Prot,GPIO_SCAN_ALL);

	}
}

#ifdef PM_ST_EN
static void ConsolePowerSt(void * argv[])
{
	int32 data;

	data = atoi(argv[2]);

	wvdPmStNoSet(data);

#if 0	
	if(OSAL_StrCmp(argv[2],"1-1-4")==0)
	{
		const int8 *pStr = "WaitTimer Test:between reset to BATT EN\r\n";
		ConsoleWriteBytes((const uint8 *)pStr, OSAL_StrLen(pStr));
	}
	else
	{
		const int8 *pStr = "Invalid Case\r\n";
		ConsoleWriteBytes((const uint8 *)pStr, OSAL_StrLen(pStr));
	}
#endif
}
#endif

#endif

