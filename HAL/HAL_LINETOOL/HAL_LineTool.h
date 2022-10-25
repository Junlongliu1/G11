/********************************************************************************************/
/* File Name	: HAL_LineTool.c														    */
/* Date 		: 2021.12.14																*/
/* Author		: Liujie																    */
/* Model		: EDA 																        */
/* Copyright (c)  2021 Alpine Electronics (China) Co. LTD.	All Rights Reserved 			*/
/*------------------------------------------------------------------------------------------*/
/* Revision History 																		*/
/* No		Date		Revised by		Function Name										*/
/* 0001 	2021.12.14 	Liujie 	        Create												*/
/*------------------------------------------------------------------------------------------*/
/********************************************************************************************/
#ifndef __HAL_LINETOOL_H__
#define __HAL_LINETOOL_H__
/********************************************************************************************/
/*							Include File Section											*/
/********************************************************************************************/
#include "uart.h"
/********************************************************************************************/
/*							Macro Definition Section										*/
/********************************************************************************************/

/********************************************************************************************/
/*							Type Definition Section											*/
/********************************************************************************************/

/********************************************************************************************/
/*							Enumeration Type Definition Section								*/
/********************************************************************************************/
enum {
	HAL_LINETOOL_UART = 1
};

/********************************************************************************************/
/*							Extern Declaration												*/
/********************************************************************************************/
extern int32 LineTool_Open(uint32 OPTIONS);
extern int32 LineTool_Close(void);
extern int32 LineTool_Write(const uint8 *buff, uint32 size);
extern int32 LineTool_CallBACK(uartCallback func);

#endif /* __HAL_LINETOOL_H__ */