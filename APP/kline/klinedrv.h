/********************************************************************************************/
/** <!-- Copyright (C) Alpine Electronics, Inc. All rights reserved. ------------------------>
 *
 *  \file           klinedrv.h
 *  \date           2016/06/30
 *  \author         Akihito SUZUKI
 *  \model          MN103L
 *  \description
 *  \version
 *
 *********************************************************************************************
 *  \par    Revision History:
 *  <!----- No.     Date        Revised by      Details ------------------------------------->
 *
 ********************************************************************************************/

#ifndef _KLINEDRV_H_
#define _KLINEDRV_H_

/********************************************************************************************/
/*                          Include File Section                                            */
/********************************************************************************************/
#include <string.h>
#include "logapp.h"
/********************************************************************************************/
/*                          Macro Definition Section                                        */
/********************************************************************************************/
#define KLINE_NG_SERVICE_ID                (0x7FU)
#define KLINEDRV_RES_REJECT                (0x10U)
#define KLINEDRV_RES_NOT_SUPPORT           (0x11U)
#define KLINEDRV_RES_NOT_SUPPORT_PAR       (0x12U)
#define KLINEDRV_RES_BUSY                  (0x21U)
#define KLINEDRV_RES_NOT_CONDITION         (0x22U)
#define KLINEDRV_RES_NOT_COMPLETE          (0x23U)
#define KLINEDRV_RES_OUT_OF_RANGE          (0x31U)

#define filename(x) (strrchr(x,'/')?strrchr(x,'/')+1:x)

//#define KLINE_DEBUG_LOG

#ifdef KLINE_DEBUG_LOG
#define KLINEDRV(fmt,...) printf("\n[KLINEDRV] "fmt",file = %s,line = %d\n",##__VA_ARGS__,filename(__FILE__),__LINE__)
#else						 
#define KLINEDRV(fmt,...) LOG_OUTPUT( LOG_APP_ID_LET,DEBUGLOG_LEVEL_ERROR,ATAT_LEVEL_OFF,"\n[KLINEDRV] "fmt"\n",##__VA_ARGS__)
#endif
/********************************************************************************************/
/*                          Type Definition Section                                         */
/********************************************************************************************/

/********************************************************************************************/
/*                          Enumeration Type Definition Section                             */
/********************************************************************************************/
enum KLINEDRV_EVENT_DEF{
/* 000 */ KLINEDRV_EV_INVALID = 0x00U,
/* 001 */ KLINEDRV_EV_REQUEST_DATA_RCV,
/* 002 */ KLINEDRV_EV_P2MIN_TIMEOUT,
/* 003 */ KLINEDRV_EV_P2MAX_TIMEOUT,
/* 004 */ KLINEDRV_EV_P3MIN_TIMEOUT,
/* 005 */ KLINEDRV_EV_P3MAX_TIMEOUT,
/* 006 */ KLINEDRV_EV_LET_RES,
/* *** */ KLINEDRV_EV_MAX, /* <255 */
/* *** */ KLINEDRV_OS_TIMEOUT
};

/********************************************************************************************/
/*                          Structure/Union Type Definition Section                         */
/********************************************************************************************/

/********************************************************************************************/
/*                          Extern Declaration                                              */
/********************************************************************************************/

/********************************************************************************************/
/*                          Global Function Prototype Declaration                           */
/********************************************************************************************/
extern void TSK_KLINEDRV(void);
extern void TSK_KLINEDRV_INT(void);
extern void CYC_KLINEDRV(void);
extern void wvdKlineDrvInt(Type_uByte aubRcvData, Type_uByte aubRcvErr);
extern Type_uByte wubKlineDrv_Reset(void);
extern Type_uByte wubKlineDrv_ForceStandby(void);
extern Type_uByte wubKlineDrv_StandbyIn(void);
extern void 	  wubKlineDrv_Close(void);
extern Type_uByte wubKlineDrv_StandbyOut(void);
extern Type_uByte wubKlineDrv_SendMail(const Type_uHWord auhEventCode, const void *avdBuffer_p, const Type_uWord auwSize);
extern void wubKlineDrvNegativeRes(Type_uByte aubReqSerId, Type_uByte aubResCode);
extern void wubKlineDrvActiveRes(Type_uByte aubReqSerId, Type_uByte* aubResData_p, Type_uByte aubResDataSize);

#endif /* KLINEDRV_H */

/* End of File */
