/********************************************************************************************/
/* File Name	: Os_timer_cfg.c																		   */
/* Date			: 2021/11/04																*/
/* Author		: xulei																*/
/* Model		: EDA																	*/
/* Description	: OS Framework - Source File for configuration.								*/
/* Copyright (c)  2020 Alpine Electronics (China) Co. LTD.  All Rights Reserved				*/
/*------------------------------------------------------------------------------------------*/
/* Revision History																			*/
/* No		Date			Revised by		Function Name										*/
/* 0001		2021/11/04		xulei			New																						*/
/*------------------------------------------------------------------------------------------*/
/********************************************************************************************/

/********************************************************************************************/
/*							Include File Section											*/
/********************************************************************************************/

#include "os_framework.h"
#include "os_framework_type.h"
#include "os_timer_cfg.h"
#include"debug.h"

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
/*							Static Prototype Declaration Section							*/
/********************************************************************************************/


/* CYC timer callback extern */
extern void wvdOsWdtCycCtrl(void);
extern void wvdPmDrvCycCtrl(void);
extern void wvdPmSoftWdtCycCtrl(void);
extern void wvdPmSocInitGTimout(void);
extern void wvdPmStandbyOutGTimout(void);
extern void wvdPmStandbyInGTimout(void);
extern void wvdPmPowerKeyOnGTimout(void);
extern void wvdPmPowerKeyOffGTimout(void);
extern void CYC_KLINEDRV(void);
extern void wvdPmDrvDetOnStage1(void);
extern void wvdPmDrvDetOnStage2(void);
extern void wvdPmDrvDetOnStage3(void);
extern void wvdPmDrvDetOnStage4(void);
extern void wvdPmDrvDetOnStage5(void);
extern void wvdPmDrvDetOffStage1(void);
extern void wvdPmDrvDetOffStage2(void);
extern void wvdPmDrvDetOffStage3(void);
extern void wvdPmDrvDetOffStage4(void);

#if (SERVICE_TEST_EN == 1u)
extern void CYC_TEST(void);
#endif


/********************************************************************************************/
/*							Global Variable Definition Section								*/
/********************************************************************************************/

/********************************************************************************************/
/*							Static Variable Definition Section								*/
/********************************************************************************************/
/* OS timer callback extern */
static const OS_TmrCfgTbl_ts OS_TmrCfgTable[OS_TIMER_ID_MAX] = {
	/* ID,						Function,					Name,						Period(ms),		AutoStart        AutoReload*/
	{OS_TIMER_ID_PMDRVCYC, 					wvdPmDrvCycCtrl, 			"CYC_PMDRV", 				2, 				FALSE,		    TRUE},
	{OS_TIMER_ID_PMWDTCYCHEART, 			wvdPmSoftWdtCycCtrl, 		"CYC_PMWDTHEART", 			3000, 			FALSE,		    TRUE},
	{OS_TIMER_ID_OSWDTCYCHEART, 			wvdOsWdtCycCtrl, 			"CYC_OSWDTHEART", 			18000, 			TRUE,		    FALSE},
	{OS_TIMER_ID_PMSOCSTARTGT, 				wvdPmSocInitGTimout,		"PM_SOCINIT_GTIMER", 		30000, 			FALSE,		    FALSE},//TBD:need to set 30s
	{OS_TIMER_ID_PMSTANDBYOUTGT, 			wvdPmStandbyOutGTimout,		"PM_STADBYOUT_GTIMER", 		10000, 			FALSE,		    FALSE},
	{OS_TIMER_ID_PMSTANDBYINGT, 			wvdPmStandbyInGTimout,		"PM_STADBYIN_GTIMER", 		10000, 			FALSE,		    FALSE},
	{OS_TIMER_ID_PMPOWERKEYONGT, 			wvdPmPowerKeyOnGTimout,		"PM_POWERKEYON_GTIMER",		10000, 			FALSE,		    FALSE},
	{OS_TIMER_ID_PMPOWERKEYOFFGT, 			wvdPmPowerKeyOffGTimout,	"PM_POWERKEYOFF_GTIMER",	10000, 			FALSE,		    FALSE},
	{OS_TIMER_ID_KLINEDRV, 					CYC_KLINEDRV,				"CYC_KLINEDRV",				2,				TRUE,			TRUE},
#if (TEMP_COMPILE_SWITCH_05WORK_10WORK != TEMP_COMPILE_SWITCH_05WORK)
	{OS_TIMER_ID_PMDRVPINSETUP1, 			wvdPmDrvDetOnStage1,		"PMDRV_PINTIMEUP_1",		16,				FALSE,			FALSE},
#endif
#if (SERVICE_TEST_EN == 1u)
	{OS_TIMER_ID_TEST,						CYC_TEST,					"CYC_TEST",					10000,			FALSE,			FALSE},
#endif
#if (TEMP_COMPILE_SWITCH_05WORK_10WORK != TEMP_COMPILE_SWITCH_05WORK)
	{OS_TIMER_ID_PMDRVPINSETUP2, 			wvdPmDrvDetOnStage2,		"PMDRV_PINTIMEUP_2",		21,				FALSE,			FALSE},
	{OS_TIMER_ID_PMDRVPINSETUP3, 			wvdPmDrvDetOnStage3,		"PMDRV_PINTIMEUP_3",		31,				FALSE,			FALSE},
	{OS_TIMER_ID_PMDRVPINSETUP4, 			wvdPmDrvDetOnStage4,		"PMDRV_PINTIMEUP_4",		41,				FALSE,			FALSE},
	{OS_TIMER_ID_PMDRVPINSETUP5, 			wvdPmDrvDetOnStage5,		"PMDRV_PINTIMEUP_5",		71,				FALSE,			FALSE},
	{OS_TIMER_ID_PMDRVPINSETDN1, 			wvdPmDrvDetOffStage1,		"PMDRV_PINTIMEDN_1",		85,				FALSE,			FALSE},
	{OS_TIMER_ID_PMDRVPINSETDN3, 			wvdPmDrvDetOffStage3,		"PMDRV_PINTIMEDN_3",		11,				FALSE,			FALSE},
	{OS_TIMER_ID_PMDRVPINSETDN4, 			wvdPmDrvDetOffStage4,		"PMDRV_PINTIMEDN_4",		11,				FALSE,			FALSE},
#endif
};
	
/********************************************************************************************/
/*							Function Definition Section										*/
/********************************************************************************************/
/********************************************************************************************/
/**	\function		OS_Timer_Initialize
 *	\date			2021/11/03
 *	\author			xulei
 *	\description		interface Initialize
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		none
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			none
 *					
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/11/03			xulei			New
 ********************************************************************************************/

OS_ErrCode_t OS_Timer_Initialize(void)
{
	OS_u8Bit_t  i;
	OS_ErrCode_t aswErrCode= OS_ERR_OK;
		
	for(i = 0;i < OS_TIMER_ID_MAX;i++){
		
		aswErrCode = OS_fw_cfg_Init_Timer(i, &OS_TmrCfgTable[i]);
		
		if(OS_ERR_OK !=aswErrCode)
		{
			return aswErrCode;
		}
	}
	return aswErrCode;
}
/* End of File */
