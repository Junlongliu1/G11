/********************************************************************************************/
/* File Name         : logapp_core.h                                                        */
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

#ifndef _LOGAPP_CORE_H_
#define _LOGAPP_CORE_H_

/********************************************************************************************/
/*							Include File Section											*/
/********************************************************************************************/
/********************************************************************************************/
/*							Macro Definition Section										*/
/********************************************************************************************/
#define	LOGGINGAPP_LOG_SIZE_MAX	(64U)
#define	LOGGINGAPP_INFO_NUM_MAX	(100U)
#define	LOGGINGAPP_INFO_NUM_MAX_X2	(200U)

#define	LOGGINGAPP_INFO_LEN	(5U)	// add by tn in 202203016 for #63960.

#define LOGGINGAPP_R5_OFF	(0x00U)
#define	LOGGINGAPP_R5_ON	(0x01U)

#define LOGGINGAPP_ATAT_OFF	(0x00U)
#define	LOGGINGAPP_ATAT_ON	(0x01U)

#define LOGGINGAPP_START_UP_OFF	(0x00U)
#define	LOGGINGAPP_START_UP_ON	(0x01U)
/********************************************************************************************/
/*							Type Definition Section											*/
/********************************************************************************************/

/********************************************************************************************/
/*							Enumeration Type Definition Section								*/
/********************************************************************************************/

/********************************************************************************************/
/*							Structure/Union Type Definition Section							*/
/********************************************************************************************/
typedef struct{
	Type_uByte ubAppID;
	Type_uByte ubLogLevel;
    Type_uByte ubATATLevel;
    Type_uHWord ubLogLen;
    Type_Char chLogText[LOGGINGAPP_LOG_SIZE_MAX];
}LogOutputInfo_ts;

/********************************************************************************************/
/*							Extern Declaration												*/
/********************************************************************************************/
extern Type_uByte wubATATLogState; 	/* ATAT log state */
extern Type_uByte wubStartUpState; /* power start up state */
extern Type_uByte wubMaxCount;	/* 存储log总数 */
extern LogOutputInfo_ts wstLogSyncInfo[LOGGINGAPP_INFO_NUM_MAX];	/* log数据 */

/********************************************************************************************/
/*							Global Function Prototype Declaration							*/
/********************************************************************************************/
extern void wvdLogRecieveEvent(void);

#endif
