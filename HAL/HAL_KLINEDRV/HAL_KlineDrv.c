/********************************************************************************************/
/* File Name	: HAL_KlineDrv.c														    */
/* Date 		: 2021.12.28																*/
/* Author		: WangRong																    */
/* Model		: EDA 																        */
/* Copyright (c)  2021 Alpine Electronics (China) Co. LTD.	All Rights Reserved 			*/
/*------------------------------------------------------------------------------------------*/
/* Revision History 																		*/
/* No		Date		Revised by		Function Name										*/
/* 0001 	2021.12.28 	WangRong 	        Create												*/
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
#include "HAL_KlineDrv.h"

#include "bsp.h"
#include "uart.h"

/********************************************************************************************/
/*                           Macro Definition Section                                       */
/********************************************************************************************/

#define KLINEDRV_UART                   UART_CH1
#define KLINEDRV_CFG			        (31U)
#define KLINEDRV_BR			            (10400U)
#define KLINEDRV_RTSCTS_USE		        (0U)
#define KLINEDRV_MODE			        UART_INTR_MODE
#define KLINEDRV_CLK			        (48000000U)	// 48MHz

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
} Hal_KlineDrv_Type_ts;

/********************************************************************************************/
/*                           ROM Table Section                                              */
/********************************************************************************************/

/* not used. */

/********************************************************************************************/
/*                           Static Variable Definition Section                             */
/********************************************************************************************/

static Hal_KlineDrv_Type_ts nstKlineDrvType;

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
extern void wvdKlineDrvInt(Type_uByte aubData, Type_uByte aubState);

/*********************************************************************************************
*  function                 : KlineDrv_Open
*  date                     : 2021.12.28
*  autho                    : WangRong
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
*  1         2021.12.28     WangRong            creat
*********************************************************************************************/
int32 KlineDrv_Open(uint32 OPTIONS) {

    int32 alRet;
    int32 alResult;
    alRet = HAL_OK;
    alResult = BSP_ERR_NONE;

    if (HAL_KLINEDRV_UART != OPTIONS) {
        alRet = HAL_ERR_NOT_SUPPORTED;
    } else {
		/* init uart device */
        alResult = uart_probe(KLINEDRV_UART, KLINEDRV_BR, KLINEDRV_MODE, KLINEDRV_RTSCTS_USE);
        if (alResult != BSP_ERR_NONE) {
            alRet = HAL_ERR_PHY;
        } else {
			nstKlineDrvType.uwOptions = OPTIONS;
			nstKlineDrvType.uwStatus = HAL_DEVICE_ST_OPENED;
			nstKlineDrvType.uwChannel = KLINEDRV_UART;
			alRet = HAL_OK;
        }
    }
    return alRet;
}

/*********************************************************************************************
*  function                 : KlineDrv_Close
*  date                     : 2021.12.28
*  autho                    : WangRong
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
*  1         2021.12.28     WangRong            creat
*********************************************************************************************/
int32 KlineDrv_Close(void) {
     int32 alRet;

    alRet = HAL_OK;
	if (HAL_DEVICE_ST_OPENED != nstKlineDrvType.uwStatus) {
		alRet = HAL_ERR_NOT_OPENED;
    } else {
		/* close uart device */
        (void)uart_close(nstKlineDrvType.uwChannel);

		nstKlineDrvType.uwOptions = 0;
		nstKlineDrvType.uwStatus = HAL_DEVICE_ST_NO_OPEN;
		nstKlineDrvType.uwChannel = KLINEDRV_UART;
		alRet = HAL_OK;
    }
    return alRet;
}

/*********************************************************************************************
*  function                 : KlineDrv_Write
*  date                     : 2021.12.28
*  autho                    : WangRong
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
*  1         2021.12.28     WangRong            creat
*********************************************************************************************/
int32 KlineDrv_Write(const uint8 *buff, uint32 size) {

	int32 alRet;
	uint8 local_putc;

	alRet = HAL_OK;
	local_putc = 0;
	
	if (HAL_DEVICE_ST_OPENED != nstKlineDrvType.uwStatus) {
		alRet = HAL_ERR_NOT_OPENED;
	} else {
		if (NULL_PTR != buff) {
			for(uint32 i = 0; i < size; i++)
			{
				if ( ((const uint8 *)buff + i) != 0)
				{
					local_putc = *((const uint8 *)buff + i);
					/* send data to uart */
					(void) uart_putc(nstKlineDrvType.uwChannel, local_putc);
				}
			}
		} else {
			alRet = HAL_ERR_BAD_PARAMETER;
		}
	}

    return alRet;
}

int32 KlineDrv_CallBACK(uartCallback func){

    int32 alRet;
    int32 alResult;

    alRet = HAL_OK;
    alResult = BSP_ERR_NONE;

	if (HAL_DEVICE_ST_OPENED != nstKlineDrvType.uwStatus) {
		alRet = HAL_ERR_NOT_OPENED;
    } else {
		/* init uart device */
        alResult = uart_Callback(KLINEDRV_UART,func);
		
		if (alResult != BSP_ERR_NONE) {
            alRet = HAL_ERR_PHY;
		}
    }
	 return alRet;
}

/* End of File */
