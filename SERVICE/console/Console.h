/****************************************************************************
 *   FileName    : Console.h
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

#ifndef _CONSOLESERVICE_H_
#define _CONSOLESERVICE_H_

#define	MAX_HIST_NUM					8

#define	cCR		('\r')
#define	cLF		('\n')
#define	cBS		('\b')//8
#define	cESC	27
#define	UART_CR	13
#define	UART_HOTIZONTAL_TAB 9 //'\t'
#define	UART_SPACE 32 //' '
#define	UART_BACKSLASH 92 //'\'
#define	UART_DOUBLE_QUOTES 34 //'"'
#define	cHISU	('[')//#define	UART_OPENNING_BRACKET 91//"["
#define	cHISD	(']')//#define	UART_CLOSING_BRACKET 93//"]"
#define 	UART_UPPERCASE_A	65
#define 	UART_UPPERCASE_B 	66
#define 	UART_UPPERCASE_C	67
#define 	UART_UPPERCASE_D 	68
#define	UART_LOWERCASE_B	98
#define	UART_LOWERCASE_H	104

//#define	cHISU	((uint8)'[')
//#define	cHISD	((uint8)']')
#define	cCTR_U	21
#define	cCTR_R	18

typedef	void (CmdFuncType)(uint8 argc, void *argv[]);

typedef struct COMMANDLIST_tag {
	uint8 *pucCommand;
	CmdFuncType *func;
	uint8 *pucRemark;
	struct COMMANDLIST_tag *pNext;
} COMMANDLIST;

// Service Status
#define CONSOLE_NO_INIT 		0 // can not use console service yet
#define CONSOLE_RUNNING 		1 // can use console service


extern void InitializeConsole(void);
extern int8 ConsoleServiceState(void);
extern void ConsoleWriteBytes(const uint8 *pBytes, uint32 length);//MISRA-C:2004 Rule 16.7
extern int32 AddConsoleCommand(COMMANDLIST * pMyCommandList, uint8 *pucCommand, CmdFuncType *func, uint8 *pucRemark);
extern int32 RemoveConsoleCommand(const uint8 *pucCommand, CmdFuncType *func);
extern int8 ConsoleServiceState(void);
extern void TAK_CONSOLE(void *p_arg);


#endif	/* _CONSOLESERVICE_H_ */

