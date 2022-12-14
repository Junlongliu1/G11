/**********************************************************************************************
*  file name                : letapp_subb_core.h
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
#ifndef _LETAPP_SUBB_CORE_H_
#define _LETAPP_SUBB_CORE_H_

/**********************************************************************************************/
/*                           Include file section                                             */
/**********************************************************************************************/

#define LET_SUBB_AM_FREQ_MIN    (531U)
#define LET_SUBB_AM_FREQ_MAX    (1602U)
#define LET_SUBB_FM_FREQ_MIN    (875U)
#define LET_SUBB_FM_FREQ_MAX    (1080U)

/**********************************************************************************************/
/*                           Macro definition section                                         */
/**********************************************************************************************/

#define	LETSUBB_MPF_MAXSIZE			(100U)

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

/* not used. */

/**********************************************************************************************/
/*                           Global Function Prototype Declaration                            */
/**********************************************************************************************/

extern void wvdLetAppSubB_SetupProEng(void);
extern void wvdLetAppSubBControl(void);

#endif
/* end of file */

