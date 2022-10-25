/**********************************************************************************************
*  file name                : letapp_os.h
*  date                     : 2016.06.01
*  autho                    : zhangjinzhu
*  model                    : TBTA
*  description              : letapp relate file
*
*----------------------------------------------------------------------------------------------
*  revision History
*  No.       date           revised by        description
*  0001      2016.05.01     zhangjinzhu       Create
**********************************************************************************************/
#ifndef _LETAPP_OS_H_
#define _LETAPP_OS_H_

/**********************************************************************************************/
/*                           Include file section                                             */
/**********************************************************************************************/

/* not used. */

/**********************************************************************************************/
/*                           Macro definition section                                         */
/**********************************************************************************************/

#define LETAPP_TICK_TIME_MAX    (0xFFFFFFFFU)
#define	LETAPP_PROC_TIME_MAX    (0x7FFFFFFFU)

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

/* not used. */

/**********************************************************************************************/
/*                           Global variable definition section                               */
/**********************************************************************************************/

extern OS_StdQueue_ts *wstLetAppStdMail_p;   /* Mailbox Object Pointer */
extern OS_StdQueue_ts *wstLetAppSubAMail_p;  /* Mailbox SObject Pointer */
extern OS_StdQueue_ts *wstLetAppSubBMail_p;  /* Mailbox Object Pointer */
extern OS_Timeout_t wswLetAppSubATmout;
extern OS_Timeout_t wswLetAppSubBTmout;
extern Type_uWord wswLetAppSubAPassTime;
extern Type_uWord wswLetAppSubBPassTime;
extern Type_uWord wuwLetAppSubANowTime;
extern Type_uWord wuwLetAppSubBNowTime;
extern Type_uWord wuwLetSubAStartTime;
extern Type_uWord wuwLetSubBStartTime;
/**********************************************************************************************/
/*                           Global Function Prototype Declaration                            */
/**********************************************************************************************/
extern Type_sWord wswLetApplGetProcTime(Type_uWord auwStartTime, Type_uWord auwEndTime);


#endif
/* end of file */

