/********************************************************************************************/
/** <!-- Copyright (C) Alpine Electronics, Inc. All rights reserved. ------------------------>
 *
 *	\file			apn_basic_type.h
 *	\date			2008/11/21
 *	\author			Daniel Morgan (ALPINE)
 *	\model			Common
 *	\description	Alpine basic type definitions and macros.
 *	\version		1.0
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/

#ifndef APN_BASIC_TYPE_H
#define APN_BASIC_TYPE_H

/********************************************************************************************/
/*							Include File Section											*/
/********************************************************************************************/

/********************************************************************************************/
/*							Macro Definition Section										*/
/********************************************************************************************/
/*==========================================================================================*/
/*	Common Macros																			*/
/*==========================================================================================*/
#define STD_NULL					0					/*!< Null Value						*/

#define STD_TRUE					1					/*!< Boolean True					*/
#define STD_FALSE					0					/*!< Boolean False					*/

/*
 *	General	common value -------------------------------------------------
 */

#ifndef	NULL						/* Avoid a multi definition of NULL */
#define	NULL			0
#endif

#ifndef	TRUE						/* Avoid a multi definition of TRUE */
#define	TRUE			1
#endif

#ifndef	FALSE						/* Avoid a multi definition of FALSE */
#define	FALSE			0
#endif

/********************************************************************************************/
/*							Type Definition Section											*/
/********************************************************************************************/
/*==========================================================================================*/
/*	Common Typedefs																			*/
/*==========================================================================================*/
typedef unsigned char				Type_uByte;			/*!< Unsigned 8-Bit Integer			*/
typedef unsigned short int			Type_uHWord;		/*!< Unsigned 16-Bit Integer		*/
typedef unsigned long int			Type_uWord;			/*!< Unsigned 32-Bit Integer		*/
/*typedef unsigned long long int	Type_uDWord;*/		/*!< Unsigned 64-Bit Integer		*/

typedef signed char					Type_sByte;			/*!< Signed 8-Bit Integer 			*/
typedef signed short int			Type_sHWord;		/*!< Signed 16-Bit Integer			*/
typedef signed long int				Type_sWord;			/*!< Signed 32-Bit Integer			*/
/*typedef signed long long int		Type_sDWord;*/		/*!< Signed 64-Bit Integer			*/

typedef	float						Type_Float;			/*!< 32-Bit Floating Point Number	*/
typedef	double						Type_Double;		/*!< 64-Bit Floating Point Number 	*/

typedef char						Type_Char;			/*! Character 						*/

/********************************************************************************************/
/*							Enumeration Type Definition Section								*/
/********************************************************************************************/
/*==========================================================================================*/
/*	Common Enumeration Types																*/
/*==========================================================================================*/
/*!	API Call Response */
enum ApiCallResponse {

	RES_OK =						0,					/*!< OK								*/
	RES_NG							,					/*!< NG								*/
	RES_BUSY						,					/*!< Busy							*/
	RES_TIMEOUT						,					/*!< Timeout						*/
	RES_SYSTEMERR					,					/*!< System Err						*/
	RES_START						,					/*!< Async func start				*/
	RES_LOADED						,					/*!< Already Load.					*/
	RES_LOADING						,					/*!< Loading.						*/
	RES_UNLOADED					,					/*!< Already Unload.				*/
	RES_UNLOADING					,					/*!< Unloading						*/
	RES_TERMINATE					,					/*!< Terminater						*/
	RES_HARDWARE_NG
};

/*! Macro used for the starting number of user defined API Call Responses */
#define RES_USER_DEFINED			(RES_BUSY + 1)

/*!	Process Result */
enum ProcessResult {

	PROC_SUCCESS =					0,					/*!< Success						*/
	PROC_FAILURE					,					/*!< Failure						*/
	PROC_NON											/*!< None							*/

};
#define PROC_HARD_ERROR RES_HARDWARE_NG

/*! Macro used for the starting number of user defined Process Results */
#define PROC_USER_DEFINED			(PROC_NON + 1)

/********************************************************************************************/
/*							Structure/Union Type Definition Section							*/
/********************************************************************************************/

/********************************************************************************************/
/*							Extern Declaration												*/
/********************************************************************************************/

/********************************************************************************************/
/*							Global Function Prototype Declaration							*/
/********************************************************************************************/

#endif /* APN_BASIC_TYPE_H */

/* End of File */
