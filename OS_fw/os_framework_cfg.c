/********************************************************************************************/
/* File Name	: os_framework_cfg.c														*/
/* Date		: 2020/12/16																*/
/* Author		: plj																*/
/* Model		: EDA																	*/
/* Description	: OS Framework - Source File for configuration.								*/
/* Copyright (c)  2020 Alpine Electronics (China) Co. LTD.  All Rights Reserved				*/
/*------------------------------------------------------------------------------------------*/
/* Revision History																			*/
/* No		Date		Revised by		Function Name										*/
/* 																							*/
/*------------------------------------------------------------------------------------------*/
/********************************************************************************************/

/********************************************************************************************/
/*							Include File Section											*/
/********************************************************************************************/

//#include "os_framework_cfg.h"
#include "os_framework.h"
#include "os_timer_cfg.h"
#include "osal.h"
#include "debug.h"
#include "app_cfg.h"
#include "Eep_core.h"

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

/* Task function extern */
extern void TSK_POWER(void);
extern void TSK_POWERDRV(void);
//extern void TSK_POWERHARDWDT(void);
extern void TSK_POWERSOFTWDT(void);
extern void TSK_Agent(void);
extern void TSK_EEP(void);


extern void TSK_KLINEDRV(void);
extern void TSK_KLINEDRV_INT(void);
extern void TSK_LETAPP(void);
extern void TSK_LETAPP_SUBA(void);
extern void TSK_LETAPP_SUBB(void);
extern void TSK_LINETOOLAPP(void);
extern void TSK_LINETOOLDRV(void);
extern void TSK_LED(void);
extern void TSK_KEY(void);
extern void TSK_KEY_ADC(void);
extern void TSK_KEY_I2C(void);
extern void TSK_KEY_GPIO(void);
extern void TSK_CYC_SNR(void);
extern void TSK_SNR(void);
extern void TSK_LOGAPP(void);
extern void TSK_RPC(void);
extern void TSK_MAIN_RECIVER(void);
extern void TSK_SUB_RECIVER(void);
extern void TSK_MAIN_PARSER(void);
extern void TSK_SUB_PARSER(void);

#if (SERVICE_CONSOLE_EN == 1u)
extern void TSK_APP(void);
extern void TAK_CONSOLE(void);
extern void ConsoleDemoTask(void);
#endif

/* CYC timer callback extern */


/********************************************************************************************/
/*							Global Variable Definition Section								*/
/********************************************************************************************/
/* Task ID */
static OS_TskId_t TSK_ID_POWER;
static OS_TskId_t TSK_ID_POWERDRV;
static OS_TskId_t TSK_ID_POWERSOFTWDT;
static OS_TskId_t TSK_ID_AGENT;
static OS_TskId_t TSK_ID_KLINEDRV;
static OS_TskId_t TSK_ID_KLINEDRV_INT;
static OS_TskId_t TSK_ID_LETAPP;
static OS_TskId_t TSK_ID_LETAPP_SUBA;
static OS_TskId_t TSK_ID_LETAPP_SUBB;
static OS_TskId_t TSK_ID_LINETOOLAPP;
static OS_TskId_t TSK_ID_LINETOOLDRV;
static OS_TskId_t TSK_ID_LED;
static OS_TskId_t TSK_ID_KEY;
static OS_TskId_t TSK_ID_KEY_ADC;
static OS_TskId_t TSK_ID_KEY_I2C;
static OS_TskId_t TSK_ID_KEY_GPIO;
static OS_TskId_t TSK_ID_CYC_SNR;
static OS_TskId_t TSK_ID_SNR;
static OS_TskId_t TSK_ID_LOGAPP;
static OS_TskId_t TSK_ID_EEP;
static OS_TskId_t TSK_ID_RPC;
static OS_TskId_t TSK_ID_MAIN_RECIVER;
static OS_TskId_t TSK_ID_SUB_RECIVER;
static OS_TskId_t TSK_ID_MAIN_PARSER;
static OS_TskId_t TSK_ID_SUB_PARSER;
#if (SERVICE_CONSOLE_EN == 1u)
static OS_TskId_t TSK_ID_APP;
static OS_TskId_t TSK_ID_CONSOLE;
static OS_TskId_t TSK_ID_CONSOLE_DEMOAPP;
#endif

/* Mailbox ID */
OS_QueueId_t MBX_ID_POWER;
OS_QueueId_t MBX_ID_POWERDRV;
OS_QueueId_t MBX_ID_POWERSOFTWDT;
OS_QueueId_t MBX_ID_AGENT;
OS_QueueId_t MBX_ID_KLINEDRV; /* LXQ ADD START */
OS_QueueId_t MBX_ID_LETAPP;
OS_QueueId_t MBX_ID_LETAPP_SUBA;
OS_QueueId_t MBX_ID_LETAPP_SUBB;
OS_QueueId_t MBX_ID_LINETOOL; /* LXQ ADD END */
OS_QueueId_t MBX_ID_LED;
OS_QueueId_t MBX_ID_KEY;
OS_QueueId_t MBX_ID_SNR;
OS_QueueId_t MBX_ID_LOGAPP;
OS_QueueId_t MBX_ID_EEP;
OS_QueueId_t MBX_ID_RPC;
OS_QueueId_t MBX_ID_MAIN_RECIVER;
OS_QueueId_t MBX_ID_SUB_RECIVER;



/********************************************************************************************/
/*							Static Variable Definition Section								*/
/********************************************************************************************/
/* Task stack buffer */
static OS_MemSize_t _TSK_ID_POWER_buf[512];
static OS_MemSize_t _TSK_ID_POWERDRV_buf[512];
static OS_MemSize_t _TSK_ID_AGENT_buf[512];
static OS_MemSize_t _TSK_ID_KLINEDRV_buf[512];
static OS_MemSize_t _TSK_ID_KLINEDRV_INT_buf[512];
static OS_MemSize_t _TSK_ID_LETAPP_buf[512];
static OS_MemSize_t _TSK_ID_LETAPPSUBA_buf[512];
static OS_MemSize_t _TSK_ID_LETAPPSUBB_buf[512];
static OS_MemSize_t _TSK_ID_LINETOOL_buf[512];
static OS_MemSize_t _TSK_ID_LINETOOLDRV_buf[512];
static OS_MemSize_t _TSK_ID_LED_buf[512];
static OS_MemSize_t _TSK_ID_KEY_buf[512];
static OS_MemSize_t _TSK_ID_KEY_ADC_buf[512];
static OS_MemSize_t _TSK_ID_KEY_I2C_buf[512];
static OS_MemSize_t _TSK_ID_KEY_GPIO_buf[512];
static OS_MemSize_t _TSK_ID_POWERSOFTWDT_buf[512];
static OS_MemSize_t _TSK_ID_CYC_SNR_buf[512];
static OS_MemSize_t _TSK_ID_SNR_buf[512];
static OS_MemSize_t _TSK_ID_LOGAPP_buf[512];
static OS_MemSize_t _TSK_ID_EEP_buf[512];
static OS_MemSize_t _TSK_ID_RPC_buf[512];
static OS_MemSize_t _TSK_ID_MAIN_RECIVER_buf[512];
static OS_MemSize_t _TSK_ID_SUB_RECIVER_buf[512];
static OS_MemSize_t _TSK_ID_MAIN_PARSER_buf[512];
static OS_MemSize_t _TSK_ID_SUB_PARSER_buf[512];
#if (SERVICE_CONSOLE_EN ==1)
static OS_MemSize_t _TSK_ID_APP_buf[512];
static OS_MemSize_t _TSK_ID_SUB_CONSOLE_buf[512];
static OS_MemSize_t _TSK_ID_SUB_CONSOLE_DEMO_buf[1024];
#endif

static OS_TmrCfgTbl_ts os_fw_timer_table[OS_TIMER_ID_MAX];
/********************************************************************************************/
/*							ROM Table Section												*/
/********************************************************************************************/
const OS_TskCfgTbl_ts OS_cfg_tsk[] = {
	/* ID,				    Function,			  Name,			    	Priority,	Stack Depth,	 Stack Pointer*/
	{&TSK_ID_POWER, 		TSK_POWER, 			"TSK_POWER", 			14, 		512, 			_TSK_ID_POWER_buf},
	{&TSK_ID_POWERDRV, 		TSK_POWERDRV, 		"TSK_POWERDRV", 		14, 		512, 			_TSK_ID_POWERDRV_buf},
	{&TSK_ID_RPC, 			TSK_RPC,			"TSK_RPC",	  		  	10, 		512,			_TSK_ID_RPC_buf},	
	{&TSK_ID_CYC_SNR, 		TSK_CYC_SNR, 		"TSK_CYC_SNR", 		    11, 		512, 			_TSK_ID_CYC_SNR_buf},
	{&TSK_ID_MAIN_RECIVER, 	TSK_MAIN_RECIVER,	"TSK_MAIN_RECIVER",		10, 		512,			_TSK_ID_MAIN_RECIVER_buf},
	{&TSK_ID_SUB_RECIVER, 	TSK_SUB_RECIVER,	"TSK_SUB_RECIVER",		10, 		512,			_TSK_ID_SUB_RECIVER_buf},
	{&TSK_ID_MAIN_PARSER, 	TSK_MAIN_PARSER,	"TSK_MAIN_PARSER",		9, 			512,			_TSK_ID_MAIN_PARSER_buf},
	{&TSK_ID_SUB_PARSER, 	TSK_SUB_PARSER,		"TSK_SUB_PARSER",		9, 			512,			_TSK_ID_SUB_PARSER_buf},
	{&TSK_ID_EEP, 			TSK_EEP,			"TSK_EEP",	  		  	8, 		  	512,			_TSK_ID_EEP_buf},
	{&TSK_ID_AGENT,			TSK_Agent,		    "TSK_Agent",		 	7,		  	512,			_TSK_ID_AGENT_buf},
	{&TSK_ID_LED,			TSK_LED,			"TSK_LED",		  		6,		  	512,			_TSK_ID_LED_buf},
	{&TSK_ID_SNR,			TSK_SNR,			"TSK_SNR",		 		6,		  	512,			_TSK_ID_SNR_buf},
	{&TSK_ID_KEY,			TSK_KEY,			"TSK_KEY",				6,			512,			_TSK_ID_KEY_buf},
	{&TSK_ID_KEY_ADC,		TSK_KEY_ADC,		"TSK_KEY_ADC",			6, 			512,			_TSK_ID_KEY_ADC_buf},
	{&TSK_ID_KEY_I2C,		TSK_KEY_I2C,		"TSK_KEY_I2C",			6, 			512,			_TSK_ID_KEY_I2C_buf},
	{&TSK_ID_KEY_GPIO,		TSK_KEY_GPIO,		"TSK_KEY_GPIO",			6,			512,			_TSK_ID_KEY_GPIO_buf},
	{&TSK_ID_KLINEDRV,    	TSK_KLINEDRV,	    "TSK_KLINEDRV",		  	6, 		  	512,			_TSK_ID_KLINEDRV_buf},
	{&TSK_ID_KLINEDRV_INT, 	TSK_KLINEDRV_INT,	"TSK_KLINEDRV_INT",	  	6, 			512,			_TSK_ID_KLINEDRV_INT_buf},
	{&TSK_ID_LOGAPP,		TSK_LOGAPP,			"TSK_LOGAPP",		  	4,		  	512,			_TSK_ID_LOGAPP_buf},
	{&TSK_ID_LETAPP, 	    TSK_LETAPP,		    "TSK_LETAPP",		  	5, 		  	512,			_TSK_ID_LETAPP_buf},
	{&TSK_ID_LETAPP_SUBA, 	TSK_LETAPP_SUBA,    "TSK_LETAPP_SUBA",	  	5, 		  	512,			_TSK_ID_LETAPPSUBA_buf},
	{&TSK_ID_LETAPP_SUBB, 	TSK_LETAPP_SUBB,	"TSK_LETAPP_SUBB",	  	5, 		  	512,			_TSK_ID_LETAPPSUBB_buf},
	{&TSK_ID_LINETOOLAPP, 	TSK_LINETOOLAPP,	"TSK_LINETOOLAPP",	  	5, 		  	512,			_TSK_ID_LINETOOL_buf},
	{&TSK_ID_LINETOOLDRV,	TSK_LINETOOLDRV,	"TSK_LINETOOLDRV",	  	5,		  	512,			_TSK_ID_LINETOOLDRV_buf},
	{&TSK_ID_POWERSOFTWDT, 	TSK_POWERSOFTWDT, 	"TSK_POWERSOFTWDT", 	3, 			512, 			_TSK_ID_POWERSOFTWDT_buf},
#if (SERVICE_CONSOLE_EN == 1u)
	{&TSK_ID_APP,	 		TSK_APP,			"TSK_APP",				4, 		512,			_TSK_ID_APP_buf},
	{&TSK_ID_CONSOLE,		TAK_CONSOLE,		"TSK_ID_CONSOLE",		4,			512,			_TSK_ID_SUB_CONSOLE_buf},
	{&TSK_ID_CONSOLE_DEMOAPP,		ConsoleDemoTask,		"TSK_ID_CONSOLEDEMO",		10,			1024,			_TSK_ID_SUB_CONSOLE_DEMO_buf},
#endif
	{0,0,0,0,0,0},
};

static const OS_SemCfgTbl_ts OS_cfg_sem[] = {
	/* ID,		MaxCnt,	InitialCnt */
	{SEM_ID_MAIN_RECIVER_MBOX, 1, 1},
	{SEM_ID_MAIN_RECIVER_READ, 1, 1},
	{SEM_ID_MAIN_RECIVER_WRITE, 1, 1},
	{SEM_ID_MAIN_RECIVER_RBUF, 1, 1},
	{SEM_ID_MAIN_RECIVER_WBUF, 1, 1},
	{SEM_ID_MAIN_RECIVER_COMMON, 1, 1},
	{SEM_ID_SUB_RECIVER_MBOX, 1, 1},
	{SEM_ID_SUB_RECIVER_READ, 1, 1},
	{SEM_ID_SUB_RECIVER_WRITE, 1, 1},
	{SEM_ID_SUB_RECIVER_RBUF, 1, 1},
	{SEM_ID_SUB_RECIVER_WBUF, 1, 1},
	{SEM_ID_SUB_RECIVER_COMMON, 1, 1},
};

static const OS_FlgCfgTbl_ts OS_cfg_evtflg[] = {
	/* ID */
	{FLG_ID_LINETOOLDRV},
	{FLG_ID_MAIN_RECIVER_ACK},
	{FLG_ID_SUB_RECIVER_ACK},
	{FLG_ID_KLINEDRV},
	#if (SERVICE_CONSOLE_EN == 1u)
	{FLG_ID_TEST},
	#endif
};

static const OS_QueueCfgTbl_ts OS_cfg_queue[] = {
	/* ID,					Name,						Length*/
	{&MBX_ID_POWER,			"MBX_ID_POWER", 			18},
	{&MBX_ID_POWERDRV,		"MBX_ID_POWERDRV", 			10},
	{&MBX_ID_AGENT, 		"MBX_AGENT",				12},
	{&MBX_ID_KLINEDRV, 		"MBX_KLINEDRV",				2},
	{&MBX_ID_LETAPP, 		"MBX_LETAPP",				2},
	{&MBX_ID_LETAPP_SUBA, 	"MBX_LETAPPSUBA",			2},
	{&MBX_ID_LETAPP_SUBB, 	"MBX_LETAPPSUBB",			2},
	{&MBX_ID_LINETOOL, 		"MBX_LINETOOL",				5},
	{&MBX_ID_LED,			"MBX_LEDAPP",				10},
	{&MBX_ID_KEY,			"MBX_KEYAPP",				10},
	{&MBX_ID_POWERSOFTWDT,	"MBX_ID_POWERSOFTWDT", 		10},
	{&MBX_ID_SNR,			"MBX_SNRAPP",				20},
	{&MBX_ID_LOGAPP,		"MBX_LOGAPP",				128},
	{&MBX_ID_EEP, 			"MBX_ID_EEP",				5},
	{&MBX_ID_RPC, 			"MBX_RPC",					2},
	{&MBX_ID_MAIN_RECIVER, 	"MBX_ID_MAIN_RECIVER",		16},
	{&MBX_ID_SUB_RECIVER, 	"MBX_ID_SUB_RECIVER",		4},
	{0,0,0},
};


/********************************************************************************************/
/*							Function Definition Section										*/
/********************************************************************************************/
/********************************************************************************************/
/**	\function		OS_Register
 *	\date			2021/11/03
 *	\author			xulei
 *	\description		Create all the resources staticly used for the whole project.
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		void
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			void
 *					
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001		-			-					New
 *					0002	2021/11/11			xulei			
 ********************************************************************************************/

OS_ErrCode_t OS_Register(void)
{
	OS_u8Bit_t i;
	OS_ErrCode_t aswErrCode = OS_ERR_OK;

	/* Create all the resources staticly used for the whole project. */
	for (i = 0;( i < sizeof(OS_cfg_tsk)/sizeof(OS_TskCfgTbl_ts) - 1)&&( aswErrCode == OS_ERR_OK); i++)
	{
		aswErrCode  = OS_CreateTask(
								(void*)OS_cfg_tsk[i].uwTskId_p,
									OS_cfg_tsk[i].vdTskFunc_p,
									OS_cfg_tsk[i].vdTskName_p,
									OS_cfg_tsk[i].uwTskPriority,
									OS_cfg_tsk[i].uwTskStackDepth,
									OS_cfg_tsk[i].uwTskStack_p);
		
		OS_DBG("\n %s:OS_CreateTask id : %d, name : %s, ,aswErrCode:%d", __FUNCTION__, *OS_cfg_tsk[i].uwTskId_p, OS_cfg_tsk[i].vdTskName_p,aswErrCode);
		if( OS_ERR_OK != aswErrCode){
			wvdEEPLOGWrite( TYPE_TASK, i );
		}
	}

	for (i = 0;( i < SEM_ID_MAX )&&( aswErrCode == OS_ERR_OK); i++)
	{
		aswErrCode = OS_CreateSemaphore(OS_cfg_sem[i].ubSemId ,
									OS_cfg_sem[i].uwSemMaxCnt,
									OS_cfg_sem[i].uwSemInitialCnt);
		OS_DBG("\nOS_CreateSemaphore id : %d  aswErrCode:%d", OS_cfg_sem[i].ubSemId,aswErrCode);

		if( OS_ERR_OK != aswErrCode){
			wvdEEPLOGWrite( TYPE_SEAM, i );
		}
	}

	for (i = 0;( i < FLG_ID_MAX )&&( aswErrCode == OS_ERR_OK); i++)
	{
		aswErrCode = OS_CreateEventFlag(OS_cfg_evtflg[i].ubFlgId);
		OS_DBG("\nOS_CreateEventFlag id : %d, aswErrCode:%d", OS_cfg_evtflg[i].ubFlgId,aswErrCode);

		if( OS_ERR_OK != aswErrCode){
			wvdEEPLOGWrite( TYPE_EVENT, i );
		}
	}

	for (i = 0;( i < sizeof(OS_cfg_queue)/sizeof(OS_QueueCfgTbl_ts) - 1)&&( aswErrCode == OS_ERR_OK); i++)
	{
		aswErrCode = OS_CreateQueue((void*)(OS_cfg_queue[i].uwQueueId_p),
									OS_cfg_queue[i].uwQueueLength,
									OS_cfg_queue[i].vdQueueName_p);
		OS_DBG("\nOS_CreateQueue id : %d, name : %s ,aswErrCode:%d", *OS_cfg_queue[i].uwQueueId_p,OS_cfg_queue[i].vdQueueName_p,aswErrCode);

		if( OS_ERR_OK != aswErrCode){
			wvdEEPLOGWrite( TYPE_QUEUE, i );
		}
	}

	/* os timer Initialize*/
	aswErrCode = OS_Timer_Initialize();

	for (i = 0; (i < OS_TIMER_ID_MAX)&&( aswErrCode == OS_ERR_OK); i++)
	{
		aswErrCode = OS_CreateTimer(os_fw_timer_table[i].ubTmrId,
									os_fw_timer_table[i].vdTmrFunc_p,
									os_fw_timer_table[i].vdTmrName_p,
									os_fw_timer_table[i].uwTmrPeriod,
									os_fw_timer_table[i].ubAutoReload);
		OS_DBG("\nOS_CreateTimer id : %d, name : %s, aswErrCode :%d \n", os_fw_timer_table[i].ubTmrId, os_fw_timer_table[i].vdTmrName_p,aswErrCode);

		if( OS_ERR_OK != aswErrCode){
			wvdEEPLOGWrite( TYPE_TIMER, i );
		}else{
			if (TRUE == os_fw_timer_table[i].ubAutoStart) {
				(void)OS_StartTimer(os_fw_timer_table[i].ubTmrId);
			}
		}
	} 	
	return aswErrCode;
}
/********************************************************************************************/
/*							Function Definition Section										*/
/********************************************************************************************/
/********************************************************************************************/
/**	\function		OS_fw_cfg_Init_Timer
 *	\date			2021/11/03
 *	\author			xulei
 *	\description		interface Initialize
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		timerID
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			OS_ErrCode_t
 *					
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/11/03			xulei			New
 ********************************************************************************************/
OS_ErrCode_t OS_fw_cfg_Init_Timer(OS_u8Bit_t timerID,const OS_TmrCfgTbl_ts *ptmrtable)
{
	OS_ErrCode_t aswErrCode = OS_ERR_OK;
	if(( timerID< OS_TIMER_ID_MAX)&&( ptmrtable !=0))
	{
		os_fw_timer_table[timerID].ubAutoReload = ptmrtable->ubAutoReload;
		os_fw_timer_table[timerID].ubAutoStart = ptmrtable->ubAutoStart;
		os_fw_timer_table[timerID].ubTmrId = ptmrtable->ubTmrId;
		os_fw_timer_table[timerID].uwTmrPeriod = ptmrtable->uwTmrPeriod;
		os_fw_timer_table[timerID].vdTmrFunc_p = ptmrtable->vdTmrFunc_p;
		os_fw_timer_table[timerID].vdTmrName_p = ptmrtable->vdTmrName_p;
	}else{
		aswErrCode = OS_ERR_PAR;
	}
	return aswErrCode;
}

/* End of File */
