/********************************************************************************************/
/* File Name         : logapp_common.h                                                      */
/* Date              : 2021/10/27                                                           */
/* Author            : tianni                                                               */
/* Model             : EDA                                                                  */
/* Description       : log Task execute related file                                        */
/* File Version      : -                                                                    */
/*------------------------------------------------------------------------------------------*/
/* Revision History                                                                         */
/* No.        Date        Revised by        Function Name                                   */
/* #0000      21/10/27    tianni            New                                             */
/********************************************************************************************/

#ifndef _LOGAPP_COMMON_H_
#define _LOGAPP_COMMON_H_

/**********************************************************************************************/
/*                           Include file section                                             */
/**********************************************************************************************/

/* not used. */

/**********************************************************************************************/
/*                           Macro definition section                                         */
/**********************************************************************************************/

#define LOGAPP_MPF_MAXSIZE             (70U)	//change by tn in 20220414 for #64609 conversion event to myself when outputlog.

/**********************************************************************************************/
/*                           Enumeration Type Definition Section                              */
/**********************************************************************************************/

/* LogApp Use Constant Value */
enum logAppCommonNum{
    LOGAPP_NUM_0 = 0U,
	LOGAPP_NUM_1,
	LOGAPP_NUM_2,
	LOGAPP_NUM_3,
	LOGAPP_NUM_4,
	LOGAPP_NUM_5,
	LOGAPP_NUM_6,
	LOGAPP_NUM_7,
	LOGAPP_NUM_8,
	LOGAPP_NUM_9,
	LOGAPP_NUM_10,
	LOGAPP_NUM_11,
	LOGAPP_NUM_12,
	LOGAPP_NUM_13,
	LOGAPP_NUM_14,
	LOGAPP_NUM_15,
	LOGAPP_NUM_16,
	LOGAPP_NUM_100 = (100U),
	LOGAPP_NUM_119 = (119U),
	LOGAPP_NUM_205 = (205U),
	LOGAPP_NUM_255 = (255U),
	LOGAPP_NUM_1000 = (1000U)
};

/**********************************************************************************************/
/*                           User type definition section                                     */
/**********************************************************************************************/

/* not used. */

/**********************************************************************************************/
/*                           Structure/Union type definition section                          */
/**********************************************************************************************/

/* not used. */

/**********************************************************************************************/
/*                           Global variable definition section                               */
/**********************************************************************************************/

/* not used. */

/**********************************************************************************************/
/*                           Global Function Prototype Declaration                            */
/**********************************************************************************************/

#endif
/* end of file */

