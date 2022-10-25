/********************************************************************************************/
/** <!-- Copyright (C) Alpine Electronics, Inc. All rights reserved. ------------------------>
 *
 *	\file			klinedrv_os.h
 *	\date			2016/06/30
 *	\author			Akihito SUZUKI
 *	\model			MN103L
 *	\description
 *	\version
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/

#ifndef _KLINEDRV_OS_H_
#define _KLINEDRV_OS_H_

/********************************************************************************************/
/*							Include File Section											*/
/********************************************************************************************/
#include "os_framework.h"
#include "apn_basic_type.h"
/********************************************************************************************/
/*							Macro Definition Section										*/
/********************************************************************************************/

/********************************************************************************************/
/*							Type Definition Section											*/
/********************************************************************************************/

/********************************************************************************************/
/*							Enumeration Type Definition Section								*/
/********************************************************************************************/

/********************************************************************************************/
/*							Structure/Union Type Definition Section							*/
/********************************************************************************************/

/********************************************************************************************/
/*							Extern Declaration												*/
/********************************************************************************************/
extern OS_Timeout_t wswKlineDrvTmout;       /* OS_ReceiveMail timeout */
extern OS_StdQueue_ts *wstKlineDrvStdMail_p;  /* Mailbox Object Pointer */
extern Type_uByte wubIdleWaitTimerAct;
extern Type_uByte wubP2TimerAct;
extern Type_uByte wubP3TimerAct;
extern Type_uByte wubP2TimerCount;
extern Type_uByte wubIdleWaitTimerCount;
extern Type_uHWord wuhP3TimerCount;

#define KLINE_VALUE_0		(0U)
#define KLINE_VALUE_1		(1U)
#define KLINE_VALUE_2		(2U)
#define KLINE_VALUE_3		(3U)
#define KLINE_VALUE_4		(4U)

/********************************************************************************************/
/*							Global Function Prototype Declaration							*/
/********************************************************************************************/
extern void wvdKlineIdleWaitTimerSet(Type_uByte aubWaitTimer);
extern void wvdKlineP2TimerSet(Type_uByte aubTimerSet);
extern void wvdKlineP3TimerSet(Type_uByte aubTimerSet);


#endif /* KLINEDRV_OS_H */

/* End of File */
