/********************************************************************************/
/* File Name         : logapp.h                                                 */
/* Date              : 2021/10/27                                               */
/* Author            : tianni                                                   */
/* Model             : EDA                                                      */
/* Description       : log Task execute related file                            */
/* File Version      : -                                                        */
/*------------------------------------------------------------------------------*/
/* Revision History                                                             */
/* No.        Date        Revised by        Function Name                       */
/* #0000      21/10/27    tianni            New                                 */
/********************************************************************************/
#ifndef _LOGAPP_H_
#define _LOGAPP_H_

/**********************************************************************************************/
/*                           Include file section                                             */
/**********************************************************************************************/

/**********************************************************************************************/
/*                           Macro definition section                                         */
/**********************************************************************************************/
#define ATAT_LEVEL_OFF	(0x00U)
#define ATAT_LEVEL_ON	(0x01U)
#define	LOG_STR_SIZE	(255U)

#define LOGMGR_DEBUG
//#define LOGMGR_WINDOWS 
#define DEBUGLOG_LEVEL_SET DEBUGLOG_LEVEL_INFO	/* add by tn in 202203023 for #64706. */
/**********************************************************************************************/
/*                           Enumeration Type Definition Section                              */
/**********************************************************************************************/
enum
{
	DEBUGLOG_LEVEL_OFF
,	DEBUGLOG_LEVEL_FALAL
,	DEBUGLOG_LEVEL_ERROR
,	DEBUGLOG_LEVEL_WARN
,	DEBUGLOG_LEVEL_INFO
,	DEBUGLOG_LEVEL_DEBUG
};

/* APP ID */
enum
{
	LOG_APP_ID_R5_NONE,
	LOG_APP_ID_PM,
	LOG_APP_ID_PMDR,
	LOG_APP_ID_PMWT,
	LOG_APP_ID_LINE,
	LOG_APP_ID_LET,
	LOG_APP_ID_LCD,
	LOG_APP_ID_AGE,
	LOG_APP_ID_KEY,
	LOG_APP_ID_ADCK,
	LOG_APP_ID_I2CK,
	LOG_APP_ID_GPK,
	LOG_APP_ID_RPC,
	LOG_APP_ID_SNR,
	LOG_APP_ID_LED,
	LOG_APP_ID_OS,
	LOG_APP_ID_EEP,
	LOG_APP_ID_LOGAPP
};


/* Log Task CallBack */
enum LOGAPP_INF_DEF{
/* 000 */ EVT_LOG_INF_MIN = 0X00U,
/* 001 */ EVT_ATATLOG_LOGGING_ON,
/* 002 */ EVT_ATATLOG_LOGGING_OFF,
/* 003 */ EVT_A53_POWER_ON,
/* 004 */ EVT_A53_POWER_OFF,
/* 005 */ EVT_LOG_R5_CHANGE_LOG_INFO,	//add by tn in 20220414 for #64609 conversion event to myself when outputlog.

/* *** */ EVT_LOG_INF_MAX
};

/**********************************************************************************************/
/*                           User type definition section                                     */
/**********************************************************************************************/

/* not used. */

/**********************************************************************************************/
/*                           Structure/Union type definition section                          */
/**********************************************************************************************/

/**********************************************************************************************/
/*                           Global variable definition section                               */
/**********************************************************************************************/

/**********************************************************************************************/
/*                           Global Function Prototype Declaration                            */
/**********************************************************************************************/
extern void TSK_LOGAPP(void);
extern Type_uByte wubLogApp_Init(void);
extern Type_uByte wubR5Log_StandbyOut(void);
extern Type_uByte wubR5Log_StandbyIn(void);
extern Type_uByte wubR5Log_EnforceStandby(void);
extern void wvdLogAppCallBack(const Type_uByte aubsrcid, const Type_uHWord aubevent, const void *data, const Type_uHWord aublength);
extern Type_uByte wubLogApp_SendMail(const Type_uByte aubsrcid, const Type_uHWord auhEventCode, const void *avdBuffer_p, const Type_uWord auwSize);

#ifdef LOGMGR_WINDOWS
extern Type_uByte LOG_OUTPUT(Type_uByte aubAppID, Type_uByte aubLogLevel, Type_uByte aubATATLevel, Type_Char* achLog);
#else
extern Type_uByte LOG_OUTPUT(Type_uByte aubAppID, Type_uByte aubLogLevel, Type_uByte aubATATLevel, Type_Char *achFormat,...);
#endif

#endif
/* end of file */

