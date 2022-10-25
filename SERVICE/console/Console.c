/****************************************************************************
 *   FileName    : Console.c
 *   Description : Console Application
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
#include <types.h>
#include <osal.h>
//#include <uart.h>
#include "HAL_API.h"
#include "HAL_devices.h"
#include <Console.h>
#include <app_cfg.h>

#if (SERVICE_CONSOLE_EN == 1u)

//#define CONSOLE_HISTORY_SUPPORT

#define MAX_ARGUMENT_COUNT	10

#define COMMAND_BUFFER_SIZE		128
#define COMMAND_PROMPT_SIZE		2
#define CRLF 	"\r\n"
#ifdef CONSOLE_HISTORY_SUPPORT
typedef struct CS_HISTORY_MANAGER_T
{
	uint8 iHistoryNum;
	uint8 iHistoryTail;
	uint8 iHistoryPtr;
	int16 sCmdHistoryLength[MAX_HIST_NUM];
	int8 sCmdHistory[MAX_HIST_NUM][COMMAND_BUFFER_SIZE];
}CS_HISTORY_MANAGER;
#endif

typedef struct {
	int8 ServiceState;
	COMMANDLIST CommandListHead;
	#ifdef CONSOLE_HISTORY_SUPPORT
	CS_HISTORY_MANAGER *pHistoryMgr;
	#endif
	HAL_HANDLE halHandle;
} CONSOLESERVICE_CONTEXT;

static CONSOLESERVICE_CONTEXT gConsole;
static COMMANDLIST gDefaultHelpCommandList;
#ifdef CONSOLE_HISTORY_SUPPORT
static CS_HISTORY_MANAGER gHistoryMgr;
#endif
//static int8 ConsoleRead(uint32 UART);
//static void ConsoleWriteOneByte(uint32 UART, int8 c);
static void DisplayCommandList(uint8 argc, void * argv[]);
static uint8 CheckValidCharacter(uint8 ucKey);
static uint32 TrimHead(const uint8 *pucCommandBuffer, uint32 ulCommandLength);
static uint32 GetCommandToken(const uint8 *pucCommandBuffer, uint32 ulCommandLength);
static int32 ExecuteCommand(CONSOLESERVICE_CONTEXT * pContext, COMMANDLIST *pCommandListHead, uint8 *pucCommandBuffer,uint32 ulCommandLength);
//static void ConsoleTask(void *p_arg);
#ifdef CONSOLE_HISTORY_SUPPORT
static void history_save(CS_HISTORY_MANAGER *pHistoryManager, const int8 *str, int32 len);
static uint32 history_puts(const CS_HISTORY_MANAGER *pHistoryManager, int8 *commandBuffer, uint16 currentCommandLength);
#endif
static void InitStaticVariable(void);//MISRA-C:2004 Rule 8.7
static void DisplayStartMessage( int8 *commandBuffer, uint32 commandBufferLength);
static void DisplayPrompt( int8 *commandBuffer, uint32 commandBufferLength);

static void DisplayCommandList(uint8 argc,  void *argv[])//MISRA-C:2004 Rule 16.7
{
	COMMANDLIST *pCommand;//=pCommandListHead->pNext;
	if ( (argc == 1) && (argv[0] != 0))
	{
		const COMMANDLIST * pCommandListHead;
		pCommandListHead = (const COMMANDLIST *)argv[0];
		pCommand = pCommandListHead->pNext;
		ConsoleWriteBytes((uint8 *)"====================\r\n",22);
		ConsoleWriteBytes((uint8 *)"Command List\r\n",14);
		ConsoleWriteBytes((uint8 *)"====================\r\n",22);

		while (pCommand!=0) {
			uint32 ulLen;
			//console display : "command" "remark"\r\n
			ulLen = OSAL_StrLen((int8 *)pCommand->pucCommand); //compile warning
			if ( ulLen != 0)
			{
				ConsoleWriteBytes(pCommand->pucCommand,ulLen);
				ulLen = OSAL_StrLen((int8 *)pCommand->pucRemark); //compile warning
				if ( ulLen != 0)
				{
					ConsoleWriteBytes((uint8 *)"\t",1);
					ConsoleWriteBytes(pCommand->pucRemark,ulLen);
				}
				ConsoleWriteBytes((uint8 *)CRLF ,2);
			}
			pCommand=pCommand->pNext;
		}
		ConsoleWriteBytes((uint8 *)"====================\r\n",22);
	}
}

#ifdef CONSOLE_HISTORY_SUPPORT
static uint32 history_puts(const CS_HISTORY_MANAGER *pHistoryManager, int8 *commandBuffer, uint16 currentCommandLength)
{
	int32	i, iCmdLength =0;
	int8	*src	= pHistoryManager->sCmdHistory[pHistoryManager->iHistoryPtr];
	uint8	*dest;

	/* Remove current command in command buffer */
	for (i = 0; i < currentCommandLength; i ++)
	{
		ConsoleWriteBytes((uint8 *)"\b \b",3);//PUTC(cBS);
	}

	dest	= commandBuffer;
	i = pHistoryManager->sCmdHistoryLength[pHistoryManager->iHistoryPtr];

	while (i > 0)
	{
		iCmdLength ++;

		if (*src == 0)
		{
			*dest = UART_SPACE;
			ConsoleWriteBytes((uint8 *)dest,1);//PUTC(*dest = UART_SPACE); /*QAC : 12.13*/
			dest++;
		}
		else
		{
			*dest = *src;
			ConsoleWriteBytes((uint8 *)dest,1);//PUTC(*dest = *src); /*QAC : 12.13*/
			dest++;
		}

		src ++;
		i --;
	}

	//iCmdPosition	= iCmdLength;
	return iCmdLength;
}

static void history_save(CS_HISTORY_MANAGER *pHistoryManager, const int8 *str, int32 len)
{
	int8	*dest;
	int32	Target;

	if (pHistoryManager->iHistoryNum < MAX_HIST_NUM)
	{
		//Target	= pCmdManager->iHistoryTail = pCmdManager->iHistoryNum ++;
		Target	= pHistoryManager->iHistoryNum; /*QAC : 12.13*/
		pHistoryManager->iHistoryNum++;
		pHistoryManager->iHistoryPtr	= pHistoryManager->iHistoryNum;
	}
	else
	{
		Target	= pHistoryManager->iHistoryTail; /*QAC : 12.13*/
		pHistoryManager->iHistoryTail++;
		if (pHistoryManager->iHistoryTail >= MAX_HIST_NUM)
		{
			pHistoryManager->iHistoryTail	= 0;
		}

		pHistoryManager->iHistoryPtr	= pHistoryManager->iHistoryTail;
	}

	dest	= pHistoryManager->sCmdHistory[Target];
	OSAL_MemCopy(dest, str, len);
	pHistoryManager->sCmdHistoryLength[Target] = len;
}
#endif
static uint8 CheckValidCharacter(uint8 ucKey)
{
	uint8 ucRetVal=1;

	if (ucKey<=' ') {
		ucRetVal=0;
	}

	return ucRetVal;
}

static uint32 TrimHead(const uint8 *pucCommandBuffer, uint32 ulCommandLength)
{
	uint32 i;

	for (i=0; i<ulCommandLength; i++) {
		if (CheckValidCharacter(*pucCommandBuffer)!=0) {
			break;
		}
	}

	return i;
}

static uint32 GetCommandToken(const uint8 *pucCommandBuffer, uint32 ulCommandLength)
{
	uint32 ulTokenLength; // CS : Useless Assignment

	for (ulTokenLength=0; ulTokenLength<ulCommandLength; ulTokenLength++) {
		if (CheckValidCharacter(*(pucCommandBuffer+ulTokenLength))!=0) {
		} else {
			break;
		}
	}

	return ulTokenLength;
}

static int32 ExecuteCommand(CONSOLESERVICE_CONTEXT * pContext, COMMANDLIST *pCommandListHead, uint8 *pucCommandBuffer,uint32 ulCommandLength)
{
	int32 retval=0;
	uint32 ulCursorPos=0;
	uint32 ulTokenLength=1;
	uint8 argc;// CS : Useless Assignment
	void * argv[MAX_ARGUMENT_COUNT];
	COMMANDLIST *pCommand=pCommandListHead->pNext;

	if ( ulCommandLength > 0)
	{
		#ifdef CONSOLE_HISTORY_SUPPORT
		history_save(pContext->pHistoryMgr, pucCommandBuffer, ulCommandLength);
		#endif
		OSAL_MemSet(argv,0x00,MAX_ARGUMENT_COUNT*sizeof(void *));
		for (argc=0; argc<MAX_ARGUMENT_COUNT; argc++) {
			if ((ulCommandLength<=ulCursorPos) || (ulTokenLength==0)) {
				break;
			}

			ulCursorPos+=TrimHead(pucCommandBuffer+ulCursorPos,ulCommandLength-ulCursorPos);
			ulTokenLength=GetCommandToken(pucCommandBuffer+ulCursorPos,ulCommandLength-ulCursorPos);

			if (ulTokenLength!=0) {
				argv[argc]=pucCommandBuffer+ulCursorPos;
				pucCommandBuffer[ulCursorPos+ulTokenLength]=0;
				ulCursorPos+=ulTokenLength+1;
			}
		}

		while (pCommand!=0) {
			if (OSAL_StrCmp(argv[0],(const int8 *)pCommand->pucCommand)==0) {
				CmdFuncType *pFunc =pCommand->func;
				if (pFunc==&DisplayCommandList) {
					argc = 1;
					argv[0] = (void *)pCommandListHead;
					DisplayCommandList(argc,argv);
				} else {
					(pCommand->func)(argc,argv);
				}
				OSAL_TaskDelay(10);
				break;
			}

			pCommand=pCommand->pNext;
		}
	}
	return retval;
}

int32 AddConsoleCommand(COMMANDLIST * pMyCommandList, uint8 *pucCommand, CmdFuncType * func, uint8 *pucRemark)
{

	int32 retval=0;
	COMMANDLIST *pCommandListHead;

	OSAL_SR_ALLOC();
	pCommandListHead = &gConsole.CommandListHead;

	if (pucCommand && func) {
		COMMANDLIST *pCommand=pCommandListHead;
		COMMANDLIST *pNewCommand;
		OSAL_CRITICAL_ENTER();
		while (pCommand->pNext!=0) {
			pCommand=pCommand->pNext;
		}

		pNewCommand=pMyCommandList;

		if (pNewCommand!=0) {
			pNewCommand->pucCommand=pucCommand;
			pNewCommand->func=func;
			pNewCommand->pucRemark=pucRemark;
			pNewCommand->pNext=0;
			pCommand->pNext=pNewCommand;
		}
		OSAL_CRITICAL_EXIT();
	} 
	else 
	{
		retval=1;
	}
	
	return retval;

}

int32 RemoveConsoleCommand(const uint8 *pucCommand, CmdFuncType *func)
{
	int32 retval=1;
	COMMANDLIST *pCommandListHead;

	OSAL_SR_ALLOC();
	pCommandListHead = &gConsole.CommandListHead;

	if ((pucCommand!=0) && (func!=0)) {
		COMMANDLIST *pCommand=pCommandListHead->pNext;
		COMMANDLIST *pParentCommand=pCommandListHead;
		OSAL_CRITICAL_ENTER();
		while (pCommand!=0) {
			if ((OSAL_StrCmp((const int8 *)pCommand->pucCommand,(const int8 *)pucCommand)==0) && (pCommand->func==func)) {//compile warning
				pParentCommand->pNext=pCommand->pNext;
				retval=0;
				break;
			}

			pParentCommand=pCommand;
			pCommand=pCommand->pNext;
		}
		OSAL_CRITICAL_EXIT();
	} 
	else 
	{
		// invalid parameter
	}

	return retval;
}

static void DisplayStartMessage( int8 *commandBuffer, uint32 commandBufferLength) 
{
	if (commandBufferLength>3) {
		ConsoleWriteBytes((uint8 *)CRLF,2);
		(void)OSAL_StrCopy((int8 *)commandBuffer, (const int8 *)"Console start...>>>>"); //QAC
		ConsoleWriteBytes((uint8 *)commandBuffer,20);
	}
}


static void DisplayPrompt( int8 *commandBuffer, uint32 commandBufferLength) 
{
	if (commandBufferLength>3) {
		ConsoleWriteBytes((uint8 *)CRLF,2);
		commandBuffer[0]='\r';
		commandBuffer[1]='>';
		commandBuffer[2]=0;
		ConsoleWriteBytes((uint8 *)commandBuffer,3);
	}
}

#if 0
static int8 ConsoleRead(uint32 UART)
{
	int32 c;
	int8 retC = 0;
	c = console_getc(UART, 0);
	if ( c > 0)
		retC = (int8)(c&0xFF);
	else
		retC = 0;
	return retC;
}

static void ConsoleWriteOneByte(uint32 UART, int8 c)
{
	int32 ret;
	ret =console_putc(UART, c);
	(void)ret;
}
#endif

void ConsoleWriteBytes(const uint8 *pBytes, uint32 length)//MISRA-C:2004 Rule 16.7
{
	CONSOLESERVICE_CONTEXT * pCSContext;

	pCSContext = &gConsole;
	if ( pCSContext->halHandle != 0)
	{
		(void)HAL_Write(pCSContext->halHandle, pBytes,length);// CS : Unused Value 
	}
}

void TAK_CONSOLE(void *p_arg)
{
	uint8 CommandBuffer[COMMAND_BUFFER_SIZE+COMMAND_PROMPT_SIZE+2];
	uint32 ulCommandLength=0;
	uint32 bufferPosition = 0;//compiler warning
	COMMANDLIST *pCommandListHead;
	CONSOLESERVICE_CONTEXT * pCSContext;
	uint8 c=0;
	int8 command=0, alreadyHandled = 0;
	int32 ulRetVal;
	
	pCSContext = &gConsole;
	pCSContext->ServiceState = CONSOLE_NO_INIT;
	OSAL_MemSet(pCSContext,0x00,sizeof(CONSOLESERVICE_CONTEXT));

	pCommandListHead = &gConsole.CommandListHead;
	#ifdef CONSOLE_HISTORY_SUPPORT
	pCSContext->pHistoryMgr = &gHistoryMgr;
	#endif
	ulRetVal = AddConsoleCommand(&gDefaultHelpCommandList, (uint8 *)"help",DisplayCommandList,(uint8 *)"Display command list");
	#ifdef USE_HAL_CONSOLE
	pCSContext->halHandle = HAL_Open(HAL_CONSOLE, 0);
	#endif
	if (pCSContext->halHandle != 0)
	{
		OSAL_MemSet(CommandBuffer,0x00,sizeof(CommandBuffer));
		#if (VEHICLE_SIGNAL_DEMO_APP ==0)
		DisplayStartMessage((int8 *)&CommandBuffer[0], sizeof(CommandBuffer));
		DisplayPrompt((int8 *)&CommandBuffer[0], sizeof(CommandBuffer));
		#endif
		pCSContext->ServiceState = CONSOLE_RUNNING;
		for (;;) {
			c=0;
			command=0;
			alreadyHandled = 0;
			//OSAL_TaskDelay(1);

			//c= ConsoleRead(DEBUG_UART);
			ulRetVal = HAL_Read(pCSContext->halHandle, &c, 1);
			if ( ulRetVal == HAL_OK)
			{
				switch (c) 
				{
					#if (VEHICLE_SIGNAL_DEMO_APP ==1)
					case 0:
						break;
					#endif
					case 0x7f:	// DEL. For Putty
					case cBS:
						if (ulCommandLength>0) {
							ulCommandLength--;
							ConsoleWriteBytes((uint8 *)"\b \b",3);
							alreadyHandled = 1;
						}
						break;
					case cCR:
					case cLF:
						command=1;
						break;
					#ifdef CONSOLE_HISTORY_SUPPORT
					case cHISU:
						{
							CS_HISTORY_MANAGER *pHistoryMgr;
							pHistoryMgr = pCSContext->pHistoryMgr;
							if (pHistoryMgr->iHistoryPtr == 0)
							{
								if (pHistoryMgr->iHistoryNum != 0)
								{
									pHistoryMgr->iHistoryPtr	= pHistoryMgr->iHistoryNum - 1;
								}
							}
							else
							{
								pHistoryMgr->iHistoryPtr	--;
							}
							CommandBuffer[0]='\r';
							CommandBuffer[1]='>';
							ulCommandLength = history_puts(pHistoryMgr, CommandBuffer+COMMAND_PROMPT_SIZE, ulCommandLength);
							bufferPosition = ulCommandLength;
							alreadyHandled = 1;
						}
						break;
					case cHISD:
						{
							CS_HISTORY_MANAGER *pHistoryMgr;
							pHistoryMgr = pCSContext->pHistoryMgr;
							pHistoryMgr->iHistoryPtr	++;

							if (pHistoryMgr->iHistoryPtr >= pHistoryMgr->iHistoryNum)
							{
								pHistoryMgr->iHistoryPtr	= 0;
							}
							CommandBuffer[0]='\r';
							CommandBuffer[1]='>';
							ulCommandLength = history_puts(pHistoryMgr, CommandBuffer+COMMAND_PROMPT_SIZE, ulCommandLength);
							bufferPosition = ulCommandLength;
							alreadyHandled = 1;
						}
						break;
					#endif
					case cCTR_U:
						{
							uint32 i;//compile warning
							/* Remove current command in command buffer */
							for (i = 0UL; i < ulCommandLength; i ++)
							{
								ConsoleWriteBytes((uint8 *)"\b \b",3UL);
							}
							ulCommandLength	= 0;
							//iCmdPosition	= 0;
						}
						break;
					#ifdef CONSOLE_HISTORY_SUPPORT
					case cCTR_R:
						{
							CS_HISTORY_MANAGER *pHistoryMgr;
							pHistoryMgr = pCSContext->pHistoryMgr;
							history_puts(pHistoryMgr, CommandBuffer, ulCommandLength);
						}
						break;
					#endif	
					case cESC:
						{
							//c= ConsoleRead(DEBUG_UART); // '['
							ulRetVal = HAL_Read(pCSContext->halHandle, &c, 1);
							//c= ConsoleRead(DEBUG_UART);
							ulRetVal = HAL_Read(pCSContext->halHandle, &c, 1);
							switch (c)
							{
								case UART_UPPERCASE_A: // ^ (up)
								break;
								case UART_UPPERCASE_B: // V (down)
								break;
								case UART_UPPERCASE_C: // > (right)
									if (bufferPosition < ulCommandLength)
									{
										ConsoleWriteBytes((uint8 *)&CommandBuffer[bufferPosition+COMMAND_PROMPT_SIZE],1);
										bufferPosition++;
									}
								break;
								case UART_UPPERCASE_D: // < (left)
									if (bufferPosition!= 0)
									{
										uint8 temp_c;
										temp_c= cBS;
										ConsoleWriteBytes((uint8 *)&temp_c,1);
										bufferPosition--;
									}
								break;
								default:
								break;
							}
							alreadyHandled = 1;
						}
						break;
					default:
						if (ulCommandLength<(COMMAND_BUFFER_SIZE-1)) {
							#if (VEHICLE_SIGNAL_DEMO_APP ==0)
							CommandBuffer[ulCommandLength+COMMAND_PROMPT_SIZE]=c;
							#else
							CommandBuffer[ulCommandLength]=c;
							#endif
							ulCommandLength++;
							bufferPosition++;
						}
						break;
				}

				if ( alreadyHandled ==0)
				{
					ConsoleWriteBytes((uint8 *)&c,1);
					if (command!=0) {
					ConsoleWriteBytes((uint8 *)CRLF,2);
						ulRetVal=ExecuteCommand(pCSContext, pCommandListHead,CommandBuffer+COMMAND_PROMPT_SIZE, ulCommandLength);

						ulCommandLength=0;
						bufferPosition = 0;

						DisplayPrompt((int8 *)CommandBuffer,sizeof(CommandBuffer));

						OSAL_TaskDelay(1);
					}
				}
			}
			else
			{
				OSAL_TaskDelay(1);
			}
		}
	}
	return;
}

int8 ConsoleServiceState(void)
{
	return gConsole.ServiceState;
}

static void InitStaticVariable(void)
{
	OSAL_MemSet(&gDefaultHelpCommandList, 0x00, sizeof(COMMANDLIST));
}

void InitializeConsole(void)
{
	InitStaticVariable();
}
#endif

