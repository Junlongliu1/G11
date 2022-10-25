/**********************************************************************************************
*  file name                : letapp_common.h
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
#ifndef _LETAPP_COMMON_H_
#define _LETAPP_COMMON_H_

/**********************************************************************************************/
/*                           Include file section                                             */
/**********************************************************************************************/

/* not used. */

/**********************************************************************************************/
/*                           Macro definition section                                         */
/**********************************************************************************************/
/* Debug Used */
#define LETAPP_DEBUG       (0)

/* Let Used Number */
#define LETAPP_NOMAL_OK    (0x01U)
#define LETAPP_NOMAL_NG    (0x00U)

#define LETAPP_SET_BIT0    (0x01U)
#define LETAPP_SET_BIT1    (0x02U)
#define LETAPP_SET_BIT2    (0x04U)
#define LETAPP_SET_BIT3    (0x08U)
#define LETAPP_SET_BIT4    (0x10U)
#define LETAPP_SET_BIT5    (0x20U)
#define LETAPP_SET_BIT6    (0x40U)
#define LETAPP_SET_BIT7    (0x80U)

#define LETAPP_MASK_03     (0x03U)
#define LETAPP_MASK_3F     (0x3FU)
#define LETAPP_MASK_CF     (0xCFU)
#define LETAPP_MASK_F3     (0xF3U)
#define LETAPP_MASK_FC     (0xFCU)

/* ASCII Number */
#define LETAPP_ASCII_NUM_0    (0x30U)
#define LETAPP_ASCII_DOT      (0x2EU)

/* Eeprom Address */
#define LET_EEP_ADD_DWG_NO           (0x029EU)
#define LET_EEP_ADD_SERIAL_NO        (0x02AEU)
#define LET_EEP_ADD_FACTORY_NO       (0x02B7U)
#define LET_EEP_ADD_INLINE_REST      (0x0C9AU)
#define LET_EEP_ADD_SND_BASS         (0x0CDBU)
#define LET_EEP_ADD_SND_TREBLE       (0x0CDDU)
#define LET_EEP_ADD_RADIO_TIME       (0x0C73U)
#define LET_EEP_ADD_OTHER_TIME       (0x0C75U)
#define LET_EEP_ADD_TOTAL_TIME       (0x0D7EU)
#define LET_EEP_ADD_HOT_ERR          (0x0D80U)
#define LET_EEP_ADD_VOL_VSP          (0x0D82U)
#define LET_EEP_ADD_AM_ANT1_REST     (0x0D87U)
#define LET_EEP_ADD_AM_ANT2_REST     (0x0D8CU)
#define LET_EEP_ADD_AM_ANT3_REST     (0x0D91U)
#define LET_EEP_ADD_FM_ANT1_REST     (0x0D96U)
#define LET_EEP_ADD_FM_ANT2_REST     (0x0D9BU)
#define LET_EEP_ADD_FM_ANT3_REST     (0x0DA0U)
#define LET_EEP_ADD_TUNER_REST       (0x0DA6U)
#define LET_EEP_ADD_USB_SPK_REST     (0x0DB0U)
#define LET_EEP_ADD_BIBIRI_REST      (0x0DB5U)
#define LET_EEP_ADD_BEEP_SPK_REST    (0x0DB6U)


#define LET_VALUE_0		(0U)
#define LET_VALUE_2		(2U)
#define LET_VALUE_3		(3U)
#define LET_VALUE_5		(5U)
#define LET_VALUE_6		(6U)
/**********************************************************************************************/
/*                           Enumeration Type Definition Section                              */
/**********************************************************************************************/

/* LetApp Use Constant Value */
enum LETAPP_COMMON_NUM{
    LETAPP_NUM_0 = 0U,
    LETAPP_NUM_1,
    LETAPP_NUM_2,
    LETAPP_NUM_3,
    LETAPP_NUM_4,
    LETAPP_NUM_5,
    LETAPP_NUM_6,
    LETAPP_NUM_7,
    LETAPP_NUM_8,
    LETAPP_NUM_9,
    LETAPP_NUM_10,
    LETAPP_NUM_11,
    LETAPP_NUM_12,   
    LETAPP_NUM_13,
    LETAPP_NUM_14,   
    LETAPP_NUM_15,
    LETAPP_NUM_16,
    LETAPP_NUM_17,
    LETAPP_NUM_18,
    LETAPP_NUM_19,
    LETAPP_NUM_20,
    LETAPP_NUM_21,
    LETAPP_NUM_22,   
    LETAPP_NUM_23,
    LETAPP_NUM_24,   
    LETAPP_NUM_25,
    LETAPP_NUM_26,
    LETAPP_NUM_27,
    LETAPP_NUM_28,
    LETAPP_NUM_29,
    LETAPP_NUM_30,
    LETAPP_NUM_31,
    LETAPP_NUM_32,   
    LETAPP_NUM_33,
    LETAPP_NUM_34,   
    LETAPP_NUM_35,
    LETAPP_NUM_36,
    LETAPP_NUM_37,
    LETAPP_NUM_38,
    LETAPP_NUM_39,
    LETAPP_NUM_40,
    LETAPP_NUM_41,
    LETAPP_NUM_42,   
    LETAPP_NUM_43,
    LETAPP_NUM_44,   
    LETAPP_NUM_45,
    LETAPP_NUM_46,
    LETAPP_NUM_47,
    LETAPP_NUM_48,   
    LETAPP_NUM_49,
    LETAPP_NUM_50,   
    LETAPP_NUM_51,
    LETAPP_NUM_52,   
    LETAPP_NUM_53,
    LETAPP_NUM_54,   
    LETAPP_NUM_55,
    LETAPP_NUM_56,
    LETAPP_NUM_57,
    LETAPP_NUM_58,   
    LETAPP_NUM_59,
    LETAPP_NUM_60,   
    LETAPP_NUM_61,
    LETAPP_NUM_62,
    LETAPP_NUM_63,
    LETAPP_NUM_64,
    LETAPP_NUM_65,
    LETAPP_NUM_66,
    LETAPP_NUM_67,
    LETAPP_NUM_68,
    LETAPP_NUM_100 = (100U),
    LETAPP_NUM_111 = (111U),
    LETAPP_NUM_130 = (130U),
    LETAPP_NUM_255 = (255U),
    LETAPP_NUM_1000 = (1000U),
    LETAPP_NUM_2000 = (2000U),
    LETAPP_NUM_FFFF = (0xFFFFU)
};

enum LETAPP_SPK_RES{
    LETAPP_RES_NO_RESULT = (0U),
    LETAPP_RES_OK,
    LETAPP_RES_NG,
    LETAPP_RES_NO_CORRES
};
enum LETAPP_VSP_RES{
    LETAPP_RES_VSP_NG = (0U),
    LETAPP_RES_VSP_OK    
};
enum LETAPP_INLINE_RES{
    LETAPP_INLINE_SYNTHESISE = (00U),				/* 1 */
    LETAPP_INLINE_B_CAN,							/* 2 */
    LETAPP_INLINE_REAR_CAMERA = (05U),			/* 6 */
    LETAPP_INLINE_STRG_PICKUP = (20U),				/* 21 */
    LETAPP_INLINE_STRG_HANGUP,					/* 22 */
    LETAPP_INLINE_STRG_TALK,						/* 23 */
    LETAPP_INLINE_MIC = (24U),						/* 25 */
    LETAPP_INLINE_METER_DISPLAY = (30U),			/* 31 */
    LETAPP_INLINE_TCU = (32U),						/* 33 */
    LETAPP_INLINE_RESERVE1,						/* 34 */
    LETAPP_INLINE_RTM_EVCC,						/* 35 */
    LETAPP_INLINE_4G_MAIN_ANT,						/* 36 */
    LETAPP_INLINE_4G_SUB_ANT,						/* 37 */
    LETAPP_INLINE_GPS_ANT,							/* 38 */
    LETAPP_INLINE_RTM_EVCC_USB,					/* 39 */
    LETAPP_INLINE_RESERVE2,						/* 40-44 */
    LETAPP_INLINE_PARK = (45U),						/* 46 */
    LETAPP_INLINE_VSP,								/* 47 */
    LETAPP_INLINE_ILL,								/* 48 */
    LETAPP_INLINE_BACK_LT,							/* 49 */
    LETAPP_INLINE_USB1 = (51U),						/* 52 */
    LETAPP_INLINE_USB2,							/* 53 */
    LETAPP_INLINE_USB3,							/* 54 */
    LETAPP_INLINE_RESERVE3 = (56),					/* 57-61 */
    LETAPP_INLINE_MAX = (61U),						/* Max */
    LETAPP_INLINE_RES_OK = (00U),
    LETAPP_INLINE_RES_NG
};

/**********************************************************************************************/
/*                           User type definition section                                     */
/**********************************************************************************************/

/* not used. */

/**********************************************************************************************/
/*                           Structure/Union type definition section                          */
/**********************************************************************************************/

typedef struct {
    Type_uByte ubResData[LETAPP_NUM_64];
    Type_uByte ubResSize;
    Type_uByte ubSeriveId;
    Type_uByte ubProcessRes;
}LetResponseRes_ts;

/**********************************************************************************************/
/*                           Global variable definition section                               */
/**********************************************************************************************/
extern Type_uByte wubLetAppCmdSize;
extern Type_uByte wubLetAppSubACmdSize;
extern Type_uByte wubLetAppSubBCmdSize;
extern Type_uByte wubLetAppCmdData[LETAPP_NUM_3];
extern Type_uByte wubLetAppSubACmdData[LETAPP_NUM_3];
extern Type_uByte wubLetAppSubBCmdData[LETAPP_NUM_3];
extern Type_uByte wubLetAppTempBuff[LETAPP_NUM_64];
extern Type_uByte wubLetSubATempBuff[LETAPP_NUM_64];
extern Type_uByte wubLetSubBTempBuff[LETAPP_NUM_64];

/**********************************************************************************************/
/*                           Global Function Prototype Declaration                            */
/**********************************************************************************************/
extern void wvdLetAppInitial(void);
extern void wvdLetAppComInit(void);
extern Type_uByte wubLetAppFmAmIsChk(void);
extern void wvdLetAppSysWorkTime(Type_uByte aubWorkSts);


#endif
/* end of file */

