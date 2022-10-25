/********************************************************************************************/
/* File Name	: linetoolapp_common.h														*/
/* Date 		: 2021.11.12																*/
/* Author		: Liujie																*/
/* Model		: EDA 																*/
/* Copyright (c)  2021 Alpine Electronics (China) Co. LTD.	All Rights Reserved 			*/
/*------------------------------------------------------------------------------------------*/
/* Revision History 																		*/
/* No		Date		Revised by		Function Name										*/
/* 0001 	2021.11.12 	liujie 	        Create																							*/
/*------------------------------------------------------------------------------------------*/
/********************************************************************************************/

#ifndef _LINETOOLAPP_COMMON_H_
#define _LINETOOLAPP_COMMON_H_

/**********************************************************************************************/
/*                           Include file section                                             */
/**********************************************************************************************/

/* not used. */

/**********************************************************************************************/
/*                           Macro definition section                                         */
/**********************************************************************************************/
// TODO:delete
enum LOG_LEVEL_TYPE {
	LOG_LEVEL_NONE = 0,
	LOG_LEVEL_FATAL,	/* LOGFAT */
	LOG_LEVEL_ERROR,	/* LOGERR */
	LOG_LEVEL_WARN,		/* LOGWRN */
	LOG_LEVEL_INFO,		/* LOGINF */
	LOG_LEVEL_DEBUG,	/* LOGDBG */
};
#define LINETOOL_UNUSED(x) (void)(x)

#define LINETOOLAPP_DEBUG                   (0U)    //changed by cxx on 20220421 for LOG

#if (LINETOOLAPP_DEBUG)
#define LINETOOLAPP_D(fmt, ...)	printf("(D)[LINETOOL][%s:%d]"fmt"\r\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define LINETOOLAPP_E(fmt, ...)	printf("(E)[LINETOOL][%s:%d]"fmt"\r\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define LINETOOLAPP_D(fmt, args...)
#define LINETOOLAPP_E(fmt, args...)
#endif

// #define LINETOOL_CMDLENGTH         (13U)
#define LINETOOL_CMDLENGTH         (70U)	// changed by cxx on 20220505 for #69281	

/* Event flag patterns */
#define LINETOOLDRV_FLGPTN                (0x00000FFFUL)
#define LINETOOLDRV_FLGPTN_ERR            (0x00000001UL)
#define LINETOOLDRV_FLGPTN_RECDATA        (0x00000002UL)
#define LINETOOLDRV_FLGPTN_SENDALLOK      (0x00000004UL)
#define LINETOOLDRV_FLGPTN_TOUT           (0x00000008UL)

/**********************************************************************************************/
/*                           Enumeration Type Definition Section                              */
/**********************************************************************************************/


/* LineToolApp Use Constant Value */
enum LineToolAppCommonNum{
    LINETOOL_NUM_0 = 0U,
    LINETOOL_NUM_1,
    LINETOOL_NUM_2,
    LINETOOL_NUM_3,
    LINETOOL_NUM_4,
    LINETOOL_NUM_5,
    LINETOOL_NUM_6,
    LINETOOL_NUM_7,
    LINETOOL_NUM_8,
    LINETOOL_NUM_9,
    LINETOOL_NUM_10,
    LINETOOL_NUM_11,
    LINETOOL_NUM_12,
    LINETOOL_NUM_13,
    LINETOOL_NUM_14,
    LINETOOL_NUM_15,
    LINETOOL_NUM_16,
    LINETOOL_NUM_17,
    LINETOOL_NUM_18,
    LINETOOL_NUM_19,
    LINETOOL_NUM_20,
    LINETOOL_NUM_21,
    LINETOOL_NUM_22,
    LINETOOL_NUM_23,
    LINETOOL_NUM_24,
    LINETOOL_NUM_25,
    LINETOOL_NUM_26,
    LINETOOL_NUM_27,
    LINETOOL_NUM_28,
    LINETOOL_NUM_29,
    LINETOOL_NUM_30,
    LINETOOL_NUM_31,
    LINETOOL_NUM_32,
    LINETOOL_NUM_33,
    LINETOOL_NUM_34,
    LINETOOL_NUM_35,
    LINETOOL_NUM_36,
    LINETOOL_NUM_50 = (50U),
    LINETOOL_NUM_51,
    LINETOOL_NUM_62 = (62U),	// add by cxx on 20220505 for #69281	
    LINETOOL_NUM_63,
    LINETOOL_NUM_100 = (100U),
    LINETOOL_NUM_119 = (119U),
    LINETOOL_NUM_205 = (205U),
    LINETOOL_NUM_255 = (255U),
    LINETOOL_NUM_1000 = (1000U)
};

/* LineToolApp Use ASCII Value */
enum LineToolAppASCIINum{
    LINETOOL_ASCII_NUM_POINT = (0x2EU),
    LINETOOL_ASCII_NUM_0 = (0x30U),
    LINETOOL_ASCII_NUM_1,
    LINETOOL_ASCII_NUM_2,
    LINETOOL_ASCII_NUM_3,
    LINETOOL_ASCII_NUM_4,
    LINETOOL_ASCII_NUM_5,
    LINETOOL_ASCII_NUM_6,
    LINETOOL_ASCII_NUM_7,
    LINETOOL_ASCII_NUM_8,
    LINETOOL_ASCII_NUM_9,
    LINETOOL_ASCII_NUM_A = (0x41U),
    LINETOOL_ASCII_NUM_B,
    LINETOOL_ASCII_NUM_C,
    LINETOOL_ASCII_NUM_D,
    LINETOOL_ASCII_NUM_E,
    LINETOOL_ASCII_NUM_F,
    LINETOOL_ASCII_NUM_G,
    LINETOOL_ASCII_NUM_H,
    LINETOOL_ASCII_NUM_I,
    LINETOOL_ASCII_NUM_J,
    LINETOOL_ASCII_NUM_K,
    LINETOOL_ASCII_NUM_L,
    LINETOOL_ASCII_NUM_M,
    LINETOOL_ASCII_NUM_N,
    LINETOOL_ASCII_NUM_O,
    LINETOOL_ASCII_NUM_P,
    LINETOOL_ASCII_NUM_Q,
    LINETOOL_ASCII_NUM_R
};

/* LineToolApp Power State */
enum LineToolAppPowerState {
    LINETOOL_STATE_MIN =0x00,
    LINETOOL_STATE_ACC_OFF,
    LINETOOL_STATE_POWER_ON,
    LINETOOL_STATE_POWER_OFF,
    LINETOOL_STATE_MAX
};

/**********************************************************************************************/
/*                           User type definition section                                     */
/**********************************************************************************************/

typedef struct{
    Type_uByte          ubDataRecLength;
    Type_uByte          ubRevData_a[LINETOOL_CMDLENGTH];
    Type_uByte          *ubSaveData_p;
} LineToolVar_ts;

/**********************************************************************************************/
/*                           Structure/Union type definition section                          */
/**********************************************************************************************/

/* not used. */

/**********************************************************************************************/
/*                           Global variable definition section                               */
/**********************************************************************************************/
extern LineToolVar_ts wstLineToolDrvVar;
extern Type_uByte wubLineToolApp_RPCSend(const Type_uByte aubDest, const Type_uHWord auhEvent, void* avdData_p, const Type_uHWord auhLength) ;

/**********************************************************************************************/
/*                           Global Function Prototype Declaration                            */
/**********************************************************************************************/

#endif
/* end of file */

