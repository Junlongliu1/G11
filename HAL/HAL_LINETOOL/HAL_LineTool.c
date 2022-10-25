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

/********************************************************************************************/
/*                             Include File Section                                         */
/********************************************************************************************/
#include <types.h>
#include "debug.h"
#include "apn_basic_type.h"

#include "HAL_API.h"
#include "HAL_devices.h"
#include "HAL_LineTool.h"

#include "bsp.h"
#include "uart.h"

/********************************************************************************************/
/*                           Macro Definition Section                                       */
/********************************************************************************************/

#define LINETOOL_UART                   UART_CH2
#define LINETOOL_CFG			        (35U)
#define LINETOOL_BR			            (9600U)
#define LINETOOL_RTSCTS_USE		        (0U)
#define LINETOOL_MODE			        UART_INTR_MODE
#define LINETOOL_CLK			        (48000000U)	// 48MHz

/********************************************************************************************/
/*                           Enumeration Type Definition Section                            */
/********************************************************************************************/

/* not used. */

/********************************************************************************************/
/*                           User type definition section                                   */
/********************************************************************************************/

/* not used. */

/********************************************************************************************/
/*                           Structure/Union Type Definition Section                        */
/********************************************************************************************/

typedef struct {
	Type_uWord uwStatus;
	Type_uWord uwOptions;
    Type_uWord uwChannel;
} Hal_LineTool_Type_ts;

/********************************************************************************************/
/*                           ROM Table Section                                              */
/********************************************************************************************/

/* not used. */

/********************************************************************************************/
/*                           Static Variable Definition Section                             */
/********************************************************************************************/

static Hal_LineTool_Type_ts nstLineToolType;

/********************************************************************************************/
/*                           Static Function Declaration Section                            */
/********************************************************************************************/

/* not used. */

/********************************************************************************************/
/*                           Global variable definition section                             */
/********************************************************************************************/

/* not used. */

/********************************************************************************************/
/*                           Function Definition Section                                    */
/********************************************************************************************/
extern void wvdLineToolUartInt(Type_uByte aubData, Type_uByte aubState);

/*********************************************************************************************
*  function                 : LineTool_Open
*  date                     : 2021.12.14
*  autho                    : liujie
*  description              : Open Uart
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*  1         2021.12.14     liujie            creat
*********************************************************************************************/
int32 LineTool_Open(uint32 OPTIONS) {

    int32 alRet;
    int32 alResult;

    alRet = HAL_OK;
    alResult = BSP_ERR_NONE;

    if (HAL_LINETOOL_UART != OPTIONS) {
        alRet = HAL_ERR_NOT_SUPPORTED;
    } else {
		/* init uart device */
        alResult = uart_probe(LINETOOL_UART, LINETOOL_BR, LINETOOL_MODE, LINETOOL_RTSCTS_USE);
        if (alResult != BSP_ERR_NONE) {
            alRet = HAL_ERR_PHY;
        } else {
			nstLineToolType.uwOptions = OPTIONS;
			nstLineToolType.uwStatus = HAL_DEVICE_ST_OPENED;
			nstLineToolType.uwChannel = LINETOOL_UART;
			alRet = HAL_OK;
        }
    }
    return alRet;
}

/*********************************************************************************************
*  function                 : LineTool_Close
*  date                     : 2021.12.14
*  autho                    : liujie
*  description              : Close Uart
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*  1         2021.12.14     liujie            creat
*********************************************************************************************/
int32 LineTool_Close(void) {
    int32 alRet;

    alRet = HAL_OK;

	if (HAL_DEVICE_ST_OPENED != nstLineToolType.uwStatus) {
		alRet = HAL_ERR_NOT_OPENED;
    } else {
		/* close uart device */
        (void)uart_close(nstLineToolType.uwChannel);

		nstLineToolType.uwOptions = 0;
		nstLineToolType.uwStatus = HAL_DEVICE_ST_NO_OPEN;
		nstLineToolType.uwChannel = LINETOOL_UART;
		alRet = HAL_OK;
    }
    return alRet;
}

/*********************************************************************************************
*  function                 : LineTool_Write
*  date                     : 2021.12.14
*  autho                    : liujie
*  description              : Write to Uart
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*  1         2021.12.14     liujie            creat
*********************************************************************************************/
int32 LineTool_Write(const uint8 *buff, uint32 size) {

	int32 alRet;
	uint8 local_putc;

	alRet = HAL_OK;
	local_putc = 0;

	if (HAL_DEVICE_ST_OPENED != nstLineToolType.uwStatus) {
		alRet = HAL_ERR_NOT_OPENED;
	} else {
		if (NULL_PTR != buff) {
			for(uint32 i = 0; i < size; i++)
			{
				if ( ((const uint8 *)buff + i) != 0)
				{
					local_putc = *((const uint8 *)buff + i);
					/* send data to uart */
					//(void) console_putc(nstLineToolType.uwChannel, local_putc);
					uart_putc(nstLineToolType.uwChannel, local_putc);
				}
			}
		} else {
			alRet = HAL_ERR_BAD_PARAMETER;
		}
	}

    return alRet;
}

int32 LineTool_CallBACK(uartCallback func){

    int32 alRet;
    int32 alResult;

    alRet = HAL_OK;
    alResult = BSP_ERR_NONE;

	if (HAL_DEVICE_ST_OPENED != nstLineToolType.uwStatus) {
		alRet = HAL_ERR_NOT_OPENED;
    } else {
		/* init uart device */
        alResult = uart_Callback(LINETOOL_UART,func);
		
		if (alResult != BSP_ERR_NONE) {
            alRet = HAL_ERR_PHY;
		}
    }
	 return alRet;
}

/* End of File */
