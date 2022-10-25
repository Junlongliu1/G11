/********************************************************************************************/
/** <!-- Copyright (C) Alpine Electronics, Inc. All rights reserved. ------------------------>
 *
 *	\file			klinedrv_func.h
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
#include "pm_pin_define.h"

#ifndef _KLINEDRV_FUNC_H_
#define _KLINEDRV_FUNC_H_

/********************************************************************************************/
/*							Include File Section											*/
/********************************************************************************************/

/********************************************************************************************/
/*							Macro Definition Section										*/
/********************************************************************************************/
#define KLINEDRV_DEBUG             (0x00U)

#define KLINE_RX_STATE      gpio_get(PM_K_LINE_RX)

#define KLINEDRV_FLGPTN                (0x03U)
#define KLINEDRV_FLGPTN_ERR            (0x01U)
#define KLINEDRV_FLGPTN_RCVDATA        (0x02U)

#define	KLINEDRV_BYTE_FORMAT       (0U)            /* Format Byte */
#define	KLINEDRV_BYTE_TARGET       (1U)            /* Target Byte */
#define	KLINEDRV_BYTE_SOURCE       (2U)            /* Source Byte */
#define	KLINEDRV_BYTE_SID          (3U)            /* ServiceID Byte */
#define	KLINEDRV_BYTE_DID          (4U)            /* Data ID Byte */

#define	KLINEDRV_LEN_ADD_DATA      (4U)             /* Header + Checksum	*/
#define	KLINEDRV_LEN_DATA          (63U)            /* Service id + Data */
#define	KLINEDRV_LEN_MESSAGE       (67U)            /* Message length */

#define	KLINEDRV_DATA_FMT          (0x80U)          /* Format Data */
#define	KLINEDRV_ADD_LET           (0xF6U)          /* Format Data */
#define	KLINEDRV_ADD_HU            (0x81U)          /* Format Data */

#define	KLINEDRV_FMT_MASK          (0xC0U)          /* Format mask */
#define	KLINEDRV_FMT_LEN_MASK      (0x3FU)          /* Format mask length */

#define	KLINEDRV_CLEAR             (0x00U)
#define	KLINEDRV_TURE              (0x01U)
#define	KLINEDRV_FLASE             (0x00U)
#define	KLINEDRV_ALLRES_TRUE       (0x02U)

#define	KLINE_MPF_MAXSIZE			(80U)

/********************************************************************************************/
/*							Type Definition Section											*/
/********************************************************************************************/

/********************************************************************************************/
/*							Enumeration Type Definition Section								*/
/********************************************************************************************/
enum KLINEDRVNUM{
    KLINEDRV_NUM_0 = 0U,
    KLINEDRV_NUM_1,
    KLINEDRV_NUM_2,
    KLINEDRV_NUM_3,
    KLINEDRV_NUM_4,
    KLINEDRV_NUM_5,
    KLINEDRV_NUM_6,
    KLINEDRV_NUM_7,
    KLINEDRV_NUM_8,
    KLINEDRV_NUM_9,
    KLINEDRV_NUM_10,
    KLINEDRV_NUM_11,
    KLINEDRV_NUM_12,   
    KLINEDRV_NUM_13,
    KLINEDRV_NUM_14,   
    KLINEDRV_NUM_15,
    KLINEDRV_NUM_33 = (33U),
    KLINEDRV_NUM_62 = (62U),
    KLINEDRV_NUM_63,
    KLINEDRV_NUM_64,
    KLINEDRV_NUM_111 = (111U),
    KLINEDRV_NUM_255 = (255U)
};
enum KLINEDRV_MODE{
/* 000 */ KLINE_MODE_IDLE_WAIT,
/* 001 */ KLINE_MODE_IDLE,
/* 002 */ KLINE_MODE_INIT_LOW,
/* 003 */ KLINE_MODE_INIT_HI,
/* 004 */ KLINE_MODE_INIT_REQ,
/* 005 */ KLINE_MODE_INIT_RES_WAIT,
/* 006 */ KLINE_MODE_INIT_RES,
/* 007 */ KLINE_MODE_REQ_WAIT,
/* 008 */ KLINE_MODE_REQ_RCV,
/* 009 */ KLINE_MODE_RES_WAIT,
/* 010 */ KLINE_MODE_SEND_RES,
/* *** */ KLINE_MODE_MAX
};

enum KLINE_IDLE_WAIT_TIMER{
/* 000 */ KLINE_TIMER_WAIT_FALSE,
/* 001 */ KLINE_TIMER_WAIT_TURE,
/* 002 */ KLINE_IDLE_WAIT_300MS,
/* 003 */ KLINE_IDLE_WAIT_P3MIN
};

/********************************************************************************************/
/*							Structure/Union Type Definition Section							*/
/********************************************************************************************/

/********************************************************************************************/
/*							Extern Declaration												*/
/********************************************************************************************/
extern Type_uByte wubKlineMode;
extern Type_uByte wubKlineDataRcvBuf[KLINEDRV_LEN_MESSAGE];
extern Type_uByte wubKlineDataRcvLen;
extern Type_uByte wubKlineDataBuf[KLINEDRV_LEN_MESSAGE];
extern Type_uByte wubKlineDataLen;
extern Type_uByte wubKlineDataResBuf[KLINEDRV_LEN_MESSAGE];
extern Type_uByte wubKlineDataResLen;
extern Type_uByte wubKlineResSts;
extern Type_uByte wubKlineAllResChk;
extern Type_uByte wubKlineFlameDataBuf[KLINEDRV_LEN_MESSAGE * KLINEDRV_NUM_4];
/********************************************************************************************/
/*							Global Function Prototype Declaration							*/
/********************************************************************************************/
extern void wvdKlineDrvInitial(void);
extern void wvdKlineDrvComInitial(void);
extern void wvdKlineWakeupDetection(void);
extern void wvdKlineDrvCmdChk(void);
extern void wvdKlineDrvNackResMake(Type_uByte aubReqSerId, Type_uByte aubResCode);
extern Type_uByte wubKlineDrvMakeChkSum(Type_uByte* aubChkData, Type_uByte aubChkSize);

#endif /* KLINEDRV_FUNC_H */

/* End of File */

