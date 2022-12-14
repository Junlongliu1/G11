/**********************************************************************************************
*  file name                : letapp_suba_core.h
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
#ifndef _LETAPP_SUBA_CORE_H_
#define _LETAPP_SUBA_CORE_H_

/**********************************************************************************************/
/*                           Include file section                                             */
/**********************************************************************************************/

/* not used. */

/**********************************************************************************************/
/*                           Macro definition section                                         */
/**********************************************************************************************/

#define	LETSUBA_MPF_MAXSIZE			(100U)

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

extern Type_uByte wubLetSpkIndex;
extern Type_uByte wubLetSpkBtnInfo;
extern Type_uHWord wuhLetSpkConnectInfo;
extern Type_uHWord wuhLetBibiriSong;
extern Type_uByte wubLetSndBass;
extern Type_uByte wubLetSndTreble;
extern Type_uByte nubBackState;

/**********************************************************************************************/
/*                           Global Function Prototype Declaration                            */
/**********************************************************************************************/

extern void wvdLetAppSubA_SetupProEng(void);
extern void wvdLetAppSubAControl(void);


#endif
/* end of file */

