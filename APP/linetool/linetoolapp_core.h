/********************************************************************************************/
/* File Name	: linetoolapp.h 															*/
/* Date 		: 2021.11.30																*/
/* Author		: Liujie																*/
/* Model		: EDA 																*/
/* Copyright (c)  2021 Alpine Electronics (China) Co. LTD.	All Rights Reserved 			*/
/*------------------------------------------------------------------------------------------*/
/* Revision History 																		*/
/* No		Date		Revised by		Function Name										*/
/* 0001     2021.11.30  Liujie          Create												*/
/*------------------------------------------------------------------------------------------*/
/********************************************************************************************/

#ifndef _LINETOOLAPP_CORE_H_
#define _LINETOOLAPP_CORE_H_

/**********************************************************************************************/
/*                           Include file section                                             */
/**********************************************************************************************/

/* not used. */

/**********************************************************************************************/
/*                           Macro definition section                                         */
/**********************************************************************************************/

/* not used. */

/**********************************************************************************************/
/*                           Enumeration Type Definition Section                              */
/**********************************************************************************************/

/* not used. */

/**********************************************************************************************/
/*                           User type definition section                                     */
/**********************************************************************************************/

/* not used. */

/**********************************************************************************************/
/*                           Structure/Union type definition section                          */
/**********************************************************************************************/

typedef struct {
	Type_uHWord uhXposition;
	Type_uHWord uhYposition;
}Linetool_Touch_Data_ts;

typedef struct {
	Type_uByte ubhighPosition;
	Type_uByte ublowPosition;
}Linetool_Flicker_ts;

/**********************************************************************************************/
/*                           Global variable definition section                               */
/**********************************************************************************************/

/* not used. */

/**********************************************************************************************/
/*                           Global Function Prototype Declaration                            */
/**********************************************************************************************/
extern void wvdLineToolApp_SetupProEng(void);
extern void wvdLineToolAppControl(OS_StdQueue_ts *astStdQueue_p);
extern void wvdLineToolAppInit(void);
extern void wvdLineToolDrvInit(void);
extern void wvdLineToolDrvUnInit(void);
extern void  wvdREM_Callback (const Type_uByte aubInfoEvent, const void* avdOption_p);
#endif
/* end of file */


