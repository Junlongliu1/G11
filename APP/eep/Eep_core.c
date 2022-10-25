
/********************************************************************************************
 ** <!-- Copyright (C)2018 Alpine Electronics, Inc. All rights reserved. -------------------

 *  file name                : Eep_core.c
 *	date                     : 2021.11.01
 *	author                   : zhangbo
 *	model                    : EDA
 *	description			     : Task(Main Proc) & Control Routine
 *
 *----------------------------------------------------------------------------------------------
 *  revision History
 *  No.       date           revised by        description
 *  0001      2021.11.01     zhangbo           New
 **********************************************************************************************/

/********************************************************************************************/
/*                      	Include File Section                           				    */
/********************************************************************************************/

/* System Header Files */
#include "apn_basic_type.h"
#include "os_framework.h"
#include "generic.h"

/* EEPROM Header Files */
#include "Eepromcontrol.h"
#include "HAL_Eep.h"
#include "Eep_core.h"
#include "gpio.h"
#include "i2c.h" 
#include "bsp.h" 

#include "debug.h"
/* Other Components */
#include "rpc_event.h"
#include "rpc_if.h"

#include "model.h" 
#include "HAL_devices.h"
#include "temp_compile_switch.h"
#include "logapp.h"

#if (SERVICE_TEST_EN == 1u)
#include "Robust_Test.h" 
#endif

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
/*							Global Variable Definition Section								*/
/********************************************************************************************/
HAL_HANDLE 		nvbEEP_I2cHalHandle = STD_NULL;
Type_uByte		wubEEPModuleState = EEP_STATE_STANDY;			/* EepromControl Module State */

//TODO:Confirm eepmap
static Type_uByte 		wubEepRamMap_log[EEP_LOG_SIZE] = 
{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF, 
0xFF,0xFF};

static Type_uByte		wubEepRamMap_CarPlayID[EEP_CARPLAY_SIZE] = 
{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF, 
0xFF,0xFF};

static Type_uByte		wubEepRamMap_PartID[EEP_PART_SIZE] = 
{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

static Type_uByte		wubEepRamMap_Serial[EEP_SERIAL_SIZE] = 
{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

static Type_uByte		wubEepRamMap_Date[EEP_DATE_SIZE] = 
{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

static Type_uByte		wubEepRamMap_Osc[EEP_OSC_SIZE] = 
{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

static Type_uByte		wubEepRamMap_Modle[EEP_MODEL_SIZE] = 
{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

static Type_uByte		wubEepRamMap_Transit[EEP_TRANSIT_SIZE] = 
{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};


static Type_uByte		wubEepRamMap_Traceability[EEP_TRACEABILITY_SIZE] = 
{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF};


static Type_uByte		wubEepRamMap_Self[EEP_SELF_SIZE] = 
{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

static Type_uByte		wubEepRamMap_Hac[EEP_HAC_SIZE] = 
{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};


static Type_uByte	wubEepAllClearMap_a[EEP_SIZE_32]=
{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF};

static Type_uByte 		wubEepRamMap_misc[EEP_MISC_SIZE] = 
{0xFF,0xFF,0xFF};

static Type_uByte 	wubEepDefaultMap_misc[EEP_MISC_SIZE] = 
{0x05,0x00,0x06};

/********************************************************************************************/
/*							ROM Table Section												*/
/********************************************************************************************/
//TODO: to be change
static const EEP_AllDataBlockTbl_ts nstEEPAllDataBlock_Tbl[] = 
{
	/*ModleId						ubData_P				default	ubData_P			DataBlock Start Adr         Length				 DataType*/		
	{MODULE_ID_MISC,			wubEepRamMap_misc,			wubEepDefaultMap_misc,		EEP_ADDR_MISC,			EEP_MISC_SIZE,			EEP_R+EEP_W+EEP_C},
	{MODULE_ID_LOG,				wubEepRamMap_log,			NULL,						EEP_ADDR_LOG,			EEP_LOG_SIZE,			EEP_S+EEP_R+EEP_W		},

	{MODULE_ID_CARPLAY,			wubEepRamMap_CarPlayID,		NULL,						EEP_ADDR_CARPLAY,		EEP_CARPLAY_SIZE,		EEP_S+EEP_R		},
	{MODULE_ID_PART,			wubEepRamMap_PartID,		NULL,						EEP_ADDR_PART,			EEP_PART_SIZE,			EEP_S+EEP_R		},
	{MODULE_ID_SERIAL,			wubEepRamMap_Serial,		NULL,						EEP_ADDR_SERIAL,		EEP_SERIAL_SIZE,		EEP_S+EEP_R		},
	{MODULE_ID_DATE,			wubEepRamMap_Date,			NULL,						EEP_ADDR_DATE,			EEP_DATE_SIZE,			EEP_S+EEP_R		},
	{MODULE_ID_OSC,				wubEepRamMap_Osc,			NULL,						EEP_ADDR_OSC,			EEP_OSC_SIZE,			EEP_S+EEP_R		},
	{MODULE_ID_MODEL,			wubEepRamMap_Modle,			NULL,						EEP_ADDR_MODEL,			EEP_MODEL_SIZE,			EEP_S+EEP_R+EEP_W		},

	{MODULE_ID_TRANSIT,			wubEepRamMap_Transit,			NULL,					EEP_ADDR_TRANSIT,			EEP_TRANSIT_SIZE,			EEP_S+EEP_R		},
	{MODULE_ID_TRACEABILITY,	wubEepRamMap_Traceability,		NULL,					EEP_ADDR_TRACEABILITY,		EEP_TRACEABILITY_SIZE,		EEP_S+EEP_R		},
	{MODULE_ID_SELF,			wubEepRamMap_Self,				NULL,					EEP_ADDR_SELF,				EEP_SELF_SIZE,				EEP_S+EEP_R		},
	/* 0210 issO */
	{MODULE_ID_HAC,				wubEepRamMap_Hac,				NULL,					EEP_ADDR_HAC,				EEP_HAC_SIZE,				EEP_S+EEP_R 	}
};

/********************************************************************************************/
/*							Static Variable Definition Section								*/
/********************************************************************************************/
/********************************************************************************************/
/*							Static Prototype Declaration Section							*/
/********************************************************************************************/
static void nubEEPReadAllData(void);
static void nubEEPStandyOut(OS_StdQueue_ts* pstMsg);
static void nubEEPStandyIn(OS_StdQueue_ts* pstMsg);
static Type_uByte nubEEPBufferDataCheck(Type_uHWord auhStarAdr, Type_uHWord auhDataSize,Type_uByte *aubBuff_P );
static Type_uByte nubEEPGetBlockIndex(Type_uHWord auhStaAdr, Type_uHWord auhRdSize,Type_uByte *aubBlockIndex,Type_uHWord *auhOffset);
static Type_uByte nubEepDrvCRCCalc( Type_uByte *aubStartAddr, Type_uHWord auhDataSize );
static void nvdEEPMain( OS_StdQueue_ts* pstMsg );
static Type_uByte nubEEPReadLog(Type_uHWord auhStaAdr, Type_uByte aubRdSize, Type_uByte* aubBuff_p);
static void nvdEEPLogClear(void);
static Type_uByte eeprom_init(void);
static Type_uByte eeprom_Stop(void);
static void ubEEPLogOut(void);

/********************************************************************************************/
/*							ROM Table Section												*/
/********************************************************************************************/
static const EEPProc_st EEPProc_T[] = { 
       /*Event*/		   				/*Proc*/
	{EV_INF_STANDBYOUT_NOTIFY, 	       	    	nubEEPStandyOut },
	{EV_INF_STANDBYIN_NOTIFY, 	        		nubEEPStandyIn },
 };
/********************************************************************************************/
/*							Prototype Definition Section									*/
/********************************************************************************************/

/********************************************************************************************/
/*							Function Definition Section										*/
/********************************************************************************************/
/*********************************************************************************************
*  function                 : TSK_EEP
*  date                     : 2021.11.01
*  autho                    : zhangbo
*  description              : Function Call when TSK_EEP
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*  
*********************************************************************************************/
void TSK_EEP(void)
{
	OS_StdQueue_ts		astRcvMail;	/* Mail Receive Area */
	OS_StdQueue_ts * 	astRcvMail_p;  /* Mail Receive Area Point */
	OS_ErrCode_t 		aswOSRtnCode;/* OS Return Code */
	wvdGen_MemSet(&astRcvMail, EEP_CLEAR, sizeof(astRcvMail)); /* Initial Data */ 
	astRcvMail_p = &astRcvMail;
	aswOSRtnCode = EEP_CLEAR;
	
	while(TRUE){
		aswOSRtnCode = OS_ReceiveQueue(MBX_ID_EEP, OS_TOUT_FOREVER, &astRcvMail_p);	
		switch (aswOSRtnCode){		
			case OS_ERR_OK: 
				/* Receive Mail */
				nvdEEPMain(astRcvMail_p);
				/* Release Memory Pool */
				if(STD_NULL !=  astRcvMail_p->vdQueueBuffer_p){
					(void)OS_ReleaseFMemoryBlock(astRcvMail_p->vdQueueBuffer_p);
				}
				break;
			default:
				EEP_DBG("\n EEP OS_Err %d",aswOSRtnCode);
				LOG_OUTPUT( LOG_APP_ID_EEP, DEBUGLOG_LEVEL_ERROR,ATAT_LEVEL_OFF,"\n EEP TASK Queue is ErrCode %d",aswOSRtnCode);
				break;
			}
		}
}
/*********************************************************************************************
*  function                 : nvdEEPMain
*  date                     : 2021.11.01
*  autho                    : zhangbo
*  description              : Function Call when EEPMain
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*  
*********************************************************************************************/
static void nvdEEPMain( OS_StdQueue_ts* pstMsg )
{
	Type_uByte auhIndex;
	for(auhIndex = EEP_NUM_0;auhIndex < sizeof(EEPProc_T)/sizeof(EEPProc_st);auhIndex++){
		if(pstMsg ->uhEventCode == EEPProc_T[auhIndex].uhEvent){	
			if(EEPProc_T[auhIndex].Proc != NULL){
				EEPProc_T[auhIndex].Proc(pstMsg);
			}
			break;
		}else{

		}
	}
}

/*********************************************************************************************
*  function                 : nubEEPBufferDateCheck
*  date                     : 2021.11.01
*  autho                    : wupengfei
*  description              : Function Call when EEPBufferDateCheck
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*  
*********************************************************************************************/
static Type_uByte nubEEPBufferDataCheck(Type_uHWord auhStarAdr, Type_uHWord auhDataSize,Type_uByte *aubBuff_P )
{
	
	Type_uByte		aubCheckSum;
	Type_uByte		aubCheckSumData;
	Type_uByte		aubReturn;
	Type_uHWord 	auhDataLength;
		
	aubCheckSum 	= EEP_CLEAR;
	aubReturn   	= RES_NG;

	auhDataLength = auhDataSize - EEP_CHECKSUM_SIZE;
	aubCheckSum = nubEepDrvCRCCalc(aubBuff_P,auhDataLength);
	aubCheckSumData = *(aubBuff_P + auhDataLength);
	
	if (aubCheckSum == aubCheckSumData)
	{
		aubReturn = RES_OK;
	}else{
		EEP_DBG("\n comp  CheckSum = %d, EEP CheckSum = %d \n",aubCheckSum, aubCheckSumData);
		LOG_OUTPUT( LOG_APP_ID_EEP, DEBUGLOG_LEVEL_ERROR,ATAT_LEVEL_OFF,"\n Pre CheckSum = %d, EEP CheckSum = %d \n",aubCheckSum, aubCheckSumData);
	}
	
	return	aubReturn;
}
/*********************************************************************************************
*  function                 : nubEEPLogOut
*  date                     : 2022.6.13
*  autho                    : xulei
*  description              : EEPROM lOG OUt
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*  
*********************************************************************************************/
void nubEEPLogOut(void)
{
	Type_uByte	aubLogData;
	Type_uByte	aubNum;

	if(wubEepRamMap_log[TPYE_COUNT] != 0xff){
		for( aubNum = 1; aubNum < EEP_BLOCK_LENGTH; aubNum++ ){
			aubLogData = wubEepRamMap_log[aubNum];
			if( aubLogData != 0xff){
				switch(aubNum){
					case TYPE_HW:
						LOG_OUTPUT( LOG_APP_ID_OS, DEBUGLOG_LEVEL_FALAL,ATAT_LEVEL_OFF,"Err COUNT:%d,HW_Int:%d",wubEepRamMap_log[TPYE_COUNT],aubLogData);
						break;
					case TYPE_TASK:
						LOG_OUTPUT( LOG_APP_ID_OS, DEBUGLOG_LEVEL_FALAL,ATAT_LEVEL_OFF,"Err COUNT:%d,TASK_Create:%d",wubEepRamMap_log[TPYE_COUNT],aubLogData);
						break;
					case TYPE_SEAM:
						LOG_OUTPUT( LOG_APP_ID_OS, DEBUGLOG_LEVEL_FALAL,ATAT_LEVEL_OFF,"Err COUNT:%d,SEAM_Create:%d",wubEepRamMap_log[TPYE_COUNT],aubLogData);
						break;
					case TYPE_EVENT:
						LOG_OUTPUT( LOG_APP_ID_OS, DEBUGLOG_LEVEL_FALAL,ATAT_LEVEL_OFF,"Err COUNT:%d,EVENT_Create:%d",wubEepRamMap_log[TPYE_COUNT],aubLogData);
						break;
					case TYPE_QUEUE:
						LOG_OUTPUT( LOG_APP_ID_OS, DEBUGLOG_LEVEL_FALAL,ATAT_LEVEL_OFF,"Err COUNT:%d,QUEUE_Create:%d",wubEepRamMap_log[TPYE_COUNT],aubLogData);
						break;
					case TYPE_TIMER:
						LOG_OUTPUT( LOG_APP_ID_OS, DEBUGLOG_LEVEL_FALAL,ATAT_LEVEL_OFF,"Err COUNT:%d,TIMER_Create:%d",wubEepRamMap_log[TPYE_COUNT],aubLogData);
						break;
					case TYPE_PMMNG:
						switch(aubLogData){
							case PM_EEP_ERROR_MNG_HALOPEN:
								LOG_OUTPUT( LOG_APP_ID_OS, DEBUGLOG_LEVEL_FALAL,ATAT_LEVEL_OFF,"Err COUNT:%d,PM_EEP_ERROR_MNG_HALOPEN",wubEepRamMap_log[TPYE_COUNT]);
								break;
							case PM_EEP_ERROR_MNG_HALCTRL_DOWNWAIT:
								LOG_OUTPUT( LOG_APP_ID_OS, DEBUGLOG_LEVEL_FALAL,ATAT_LEVEL_OFF,"Err COUNT:%d,PM_EEP_ERROR_MNG_HALCTRL_DOWNWAIT",wubEepRamMap_log[TPYE_COUNT]);
								break;
							case PM_EEP_ERROR_MNG_HALCTRL_SETINT:
								LOG_OUTPUT( LOG_APP_ID_OS, DEBUGLOG_LEVEL_FALAL,ATAT_LEVEL_OFF,"Err COUNT:%d,PM_EEP_ERROR_MNG_HALCTRL_SETINT",wubEepRamMap_log[TPYE_COUNT]);
								break;
							case PM_EEP_ERROR_MNG_HALCTRL_SETCALLBACK:
								LOG_OUTPUT( LOG_APP_ID_OS, DEBUGLOG_LEVEL_FALAL,ATAT_LEVEL_OFF,"Err COUNT:%d,PM_EEP_ERROR_MNG_HALCTRL_SETCALLBACK",wubEepRamMap_log[TPYE_COUNT]);
								break;
							case PM_EEP_ERROR_MNG_HALCTRL_POWERDOWN:
								LOG_OUTPUT( LOG_APP_ID_OS, DEBUGLOG_LEVEL_FALAL,ATAT_LEVEL_OFF,"Err COUNT:%d,PM_EEP_ERROR_MNG_HALCTRL_POWERDOWN",wubEepRamMap_log[TPYE_COUNT]);
								break;
							case PM_EEP_ERROR_MNG_SOCINIT_TIMOUT:
								LOG_OUTPUT( LOG_APP_ID_OS, DEBUGLOG_LEVEL_FALAL,ATAT_LEVEL_OFF,"Err COUNT:%d,PM_EEP_ERROR_MNG_SOCINIT_TIMOUT",wubEepRamMap_log[TPYE_COUNT]);
								break;
							case PM_EEP_ERROR_MNG_SOCSTANDBYOUT_TIMOUT:
								LOG_OUTPUT( LOG_APP_ID_OS, DEBUGLOG_LEVEL_FALAL,ATAT_LEVEL_OFF,"Err COUNT:%d,PM_EEP_ERROR_MNG_SOCSTANDBYOUT_TIMOUT",wubEepRamMap_log[TPYE_COUNT]);
								break;
							default:
								break;
						}
						break;
					case TYPE_PMDRV:
						switch(aubLogData){
							case PM_EEP_ERROR_DRV_HALHANDLE:
								LOG_OUTPUT( LOG_APP_ID_OS, DEBUGLOG_LEVEL_FALAL,ATAT_LEVEL_OFF,"Err COUNT:%d,PM_EEP_ERROR_DRV_HALHANDLE",wubEepRamMap_log[TPYE_COUNT]);
								break;
							case PM_EEP_ERROR_DRV_HALGETBAT:
								LOG_OUTPUT( LOG_APP_ID_OS, DEBUGLOG_LEVEL_FALAL,ATAT_LEVEL_OFF,"Err COUNT:%d,PM_EEP_ERROR_DRV_HALGETBAT",wubEepRamMap_log[TPYE_COUNT]);
								break;
							case PM_EEP_ERROR_DRV_BATTIMER:
								LOG_OUTPUT( LOG_APP_ID_OS, DEBUGLOG_LEVEL_FALAL,ATAT_LEVEL_OFF,"Err COUNT:%d,PM_EEP_ERROR_DRV_BATTIMER",wubEepRamMap_log[TPYE_COUNT]);
								break;
							default:
								break;
						}
						break;
					default:
						break;
				}
			}
		}
		nvdEEPLogClear();
	}
}

/*********************************************************************************************
*  function                 : nubEEPReadAllData
*  date                     : 2021.11.01
*  autho                    : zhangbo
*  description              : Function Call when EEPAllDataCheck
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*  
*********************************************************************************************/
void nubEEPReadAllData(void)
{
	Type_uHWord			auhPoz;
	Type_uHWord			auhStarAdr;
	Type_uHWord			auhDataSize;
	Type_uByte			*aubData_P;
	Type_uByte			*aubDefaultData_P;
	Type_sByte			asbReturn;
	Type_uByte 			aubRtn;	
	
	auhPoz 		= EEP_CLEAR;
	auhStarAdr 	= EEP_CLEAR;
	auhDataSize = EEP_CLEAR;
	asbReturn 	= HAL_OK;
	aubRtn		= RES_NG;
	
	for(auhPoz = EEP_CLEAR; auhPoz <sizeof(nstEEPAllDataBlock_Tbl)/sizeof(EEP_AllDataBlockTbl_ts) ; auhPoz++)
	{
		auhStarAdr 	= nstEEPAllDataBlock_Tbl[auhPoz].uhBlockStrA;		
		auhDataSize = nstEEPAllDataBlock_Tbl[auhPoz].uhBlockSize;
		aubData_P = nstEEPAllDataBlock_Tbl[auhPoz].ubData_P;
		aubDefaultData_P = nstEEPAllDataBlock_Tbl[auhPoz].ubDefaultData_P;

		asbReturn = HAL_Ioctl(nvbEEP_I2cHalHandle,HAL_IO_EEP_READ,&auhStarAdr,&auhDataSize,aubData_P,NULL);
		
		if( HAL_OK == asbReturn ){
			if( nstEEPAllDataBlock_Tbl[auhPoz].aubModleId == MODULE_ID_LOG){
				nubEEPLogOut();
			}
			
			if ((nstEEPAllDataBlock_Tbl[auhPoz].ubDataType & EEP_S) == EEP_S){
				/* chack sum no chack*/
			}else{
				aubRtn = nubEEPBufferDataCheck(auhStarAdr,auhDataSize,aubData_P);
				if (aubRtn != RES_OK){
					(void)wubEEPBlockWrite(auhStarAdr,auhDataSize,aubDefaultData_P);	
					/*Check Sum NG,write default value*/
				}else{
					EEP_DBG("\n CHECK SUM OK");
				}
			}
		}else{
			LOG_OUTPUT( LOG_APP_ID_EEP, DEBUGLOG_LEVEL_ERROR,ATAT_LEVEL_OFF,"\n RES_HARDWARE_NG HAL_Err = %d",asbReturn);
			EEP_DBG("\n RES_HARDWARE_NG HAL_Err = %d",asbReturn);
		}
	}
}

/*********************************************************************************************
*  function                 : nubEEPStandyOut
*  date                     : 2021.11.01
*  autho                    : zhangbo
*  description              : Function Call when EEPStandyOut
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*  
*********************************************************************************************/
static void nubEEPStandyOut(OS_StdQueue_ts* pstMsg)
{
	 if (pstMsg != NULL)
	{
		nvbEEP_I2cHalHandle = HAL_Open(HAL_IIC_EEPROM, HAL_EEP_OPTION_NORMAL);
		
		if( nvbEEP_I2cHalHandle == NULL ){
			LOG_OUTPUT( LOG_APP_ID_EEP, DEBUGLOG_LEVEL_ERROR,ATAT_LEVEL_OFF,"\n  HAL_Open Err");
		}else{
			wubEEPModuleState = EEP_STATE_BUSY;		/* Module State Unload */

			#if (EEP_1_1_3 == 1u)
			PF_TEST_START;
			#endif
			nubEEPReadAllData();
			#if (EEP_1_1_3 == 1u)
			PF_TEST_STOP;
			#endif

			wubEEPModuleState = EEP_STATE_IDLE; 
			//TODO:Power callback
			wsbRPCSendMsg(ARPC_ID_EEPROM, ARPC_ID_R5_POWERMNG, EV_INF_STANDBYOUT_FINISH,STD_NULL, STD_NULL);
		}
		
	}
	else{
		; 
	}
}

/*********************************************************************************************
*  function                 : nubEEPStandyIn
*  date                     : 2021.11.01
*  autho                    : wupengfei
*  description              : Function Call when EEPIdleStandyIn
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*  
*********************************************************************************************/
static void nubEEPStandyIn(OS_StdQueue_ts* pstMsg)
{		
	
	if (pstMsg != NULL)
	{
		if( nvbEEP_I2cHalHandle == NULL ){
			EEP_DBG("\n  HAL_Open Not OPEN ");
			LOG_OUTPUT( LOG_APP_ID_EEP, DEBUGLOG_LEVEL_ERROR,ATAT_LEVEL_OFF,"\n  HAL_Open Not OPEN");
		}else{
			(void)HAL_Close(nvbEEP_I2cHalHandle);

		}
		nvdEEP_SW_Reset();
		wsbRPCSendMsg(ARPC_ID_EEPROM, ARPC_ID_R5_POWERMNG, EV_INF_STANDBYIN_FINISH,STD_NULL, STD_NULL);
	}
	else
	{
		; /* No action */
	}
}

/*********************************************************************************************
*  function                 : nubEepDrvCRCCalc
*  date                     : 2018.06.01
*  autho                    : wupengfei
*  description              : Function Call when EepDrvCRCCalc
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*  
*********************************************************************************************/
static Type_uByte nubEepDrvCRCCalc( Type_uByte *aubStartAddr, Type_uHWord auhDataSize )
{
	Type_uHWord	LoopCount;
	Type_uByte CalcData;

	CalcData =	0x00;
	if (STD_NULL != aubStartAddr){
		for (LoopCount = 0; LoopCount < auhDataSize; LoopCount++){
			CalcData ^=	*(aubStartAddr + LoopCount);
		}
	}
	else{
		/* No action */
		;
	}
	if (CalcData==0xFE){
		;
	}
	else{
		CalcData = CalcData + 1;
	}
	return	CalcData;	
}
/*********************************************************************************************
*  function                 : wubLineToolReadEEP
*  date                     : 2021.11.01
*  autho                    : zhangbo
*  description              : 
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*  
*********************************************************************************************/
static Type_uByte nubEEPReadLog(Type_uHWord auhStaAdr, Type_uByte aubRdSize, Type_uByte* aubBuff_p)
{
	Type_uByte		aubRtnCode;
	Type_sByte		asbReturn;
	
	aubRtnCode 		= RES_OK;
	asbReturn		= BSP_ERR_NONE;
		
	if((aubBuff_p != STD_NULL)&&(aubRdSize != 0))
	{
		asbReturn = eeprom_read(auhStaAdr,aubRdSize,aubBuff_p);
		if(BSP_ERR_NONE != asbReturn){
			EEP_DBG("\n %s:%dl eeprom_read asbReturn: %d", __FUNCTION__, __LINE__,asbReturn);
			aubRtnCode = RES_HARDWARE_NG;
		}
	}
	else
	{
		aubRtnCode = RES_NG;
	}

	return (aubRtnCode);	
}

/*********************************************************************************************
*  function                 : wubDirectReadRAM
*  date                     : 2021.11.01
*  autho                    : zhangbo
*  description              : Function Call when DirectReadRAM
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*  
*********************************************************************************************/
Type_uByte wubDirectReadRAM(Type_uHWord auhStaAdr, Type_uHWord auhRdSize, Type_uByte* aubBuff_p)
{
	Type_uByte		aubRtnCode;
	Type_uHWord		auhSet;
	Type_uByte		aubBlockIndex;
	Type_uHWord		auhOffset;
	aubRtnCode 		= RES_NG;
	auhSet 			= EEP_CLEAR;
	aubBlockIndex 	= EEP_CLEAR;
	auhOffset		= EEP_CLEAR;
	auhSet = (auhStaAdr + auhRdSize);
	if(aubBuff_p != STD_NULL)
	{
		if ((auhSet > EEP_SIZE_MAX) ||(EEP_NUM_0 == auhRdSize))
	   	{	
			aubRtnCode = RES_NG;
	   	}
		else
		{
		    auhSet = EEP_NUM_0;
			aubRtnCode = nubEEPGetBlockIndex(auhStaAdr, auhRdSize, &aubBlockIndex,&auhOffset);
			if((RES_OK == aubRtnCode) && (aubBlockIndex <= (sizeof(nstEEPAllDataBlock_Tbl)/sizeof(EEP_AllDataBlockTbl_ts)))
			&& (auhOffset <= nstEEPAllDataBlock_Tbl[aubBlockIndex].uhBlockSize))
			{
				wvdGen_MemCpy(aubBuff_p, &nstEEPAllDataBlock_Tbl[aubBlockIndex].ubData_P[auhOffset],auhRdSize);
			}
	    }
	}
	else
	{
		/* No Action */
		;
	}
   return (aubRtnCode);	
}

/*********************************************************************************************
*  function                 : wubLineToolReadEEP
*  date                     : 2021.11.01
*  autho                    : zhangbo
*  description              : 
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*  
*********************************************************************************************/
Type_uByte wubLineToolReadEEP(Type_uHWord auhStaAdr, Type_uHWord auhRdSize, Type_uByte* aubBuff_p)
{
	Type_uByte		aubRtnCode;
	Type_sByte		asbReturn;
	
	aubRtnCode 		= RES_OK;
	asbReturn		= HAL_OK;
		
	if((aubBuff_p != STD_NULL)&&(auhRdSize != 0))
	{
		asbReturn = HAL_Ioctl(nvbEEP_I2cHalHandle,HAL_IO_EEP_READ,&auhStaAdr,&auhRdSize,aubBuff_p,NULL);
		if(HAL_OK != asbReturn){
			aubRtnCode = RES_HARDWARE_NG;
		}
	}
	else
	{
		aubRtnCode = RES_NG;
	}

	return (aubRtnCode);	
}

/*********************************************************************************************
*  function                 : wubLineToolReadRAM
*  date                     : 2021.11.01
*  autho                    : zhangbo
*  description              : 
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*  
*********************************************************************************************/
Type_uByte wubLineToolWriteEEP(Type_uHWord auhStaAdr, Type_uHWord auhRdSize, const Type_uByte* aubBuff_p)
{
	Type_uByte		aubRtnCode;
	Type_sByte		asbReturn;
	aubRtnCode 		= RES_OK;
	asbReturn		= HAL_OK;

	if((aubBuff_p != STD_NULL)&&(auhRdSize != 0))
   	{
   		asbReturn = HAL_Ioctl(nvbEEP_I2cHalHandle,HAL_IO_EEP_WRITE,&auhStaAdr,&auhRdSize,NULL,(void *)aubBuff_p);
		if(HAL_OK != asbReturn){
			aubRtnCode = RES_HARDWARE_NG;
		}
   	}
	else
	{
		aubRtnCode = RES_NG;
	}

   	return (aubRtnCode);	
}


/*********************************************************************************************
*  function                 : nubEEPGetBlockIndex
*  date                     : 2021.11.01
*  autho                    : zhangbo
*  description              : Function Call when BlockReadCheck
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*  
*********************************************************************************************/
Type_uByte nubEEPGetBlockIndex(Type_uHWord auhStaAdr, Type_uHWord auhRdSize,Type_uByte *aubBlockIndex,Type_uHWord *auhOffset)
{
	Type_uByte			aubTbNum;
	Type_uByte			aubTbSize;
	Type_uHWord 		auhBlkFirstAdd;
	Type_uHWord 		auhBlkEndAdd;
	Type_uByte			aubReturn;
	
	aubTbNum			= EEP_NUM_0;
	auhBlkFirstAdd		= EEP_NUM_0;
	auhBlkEndAdd		= EEP_NUM_0;
	aubReturn			= RES_NG;
	aubTbSize			= sizeof(nstEEPAllDataBlock_Tbl)/sizeof(EEP_AllDataBlockTbl_ts);
	
	for(aubTbNum = EEP_NUM_0;aubTbNum < aubTbSize;aubTbNum++)
	{
		auhBlkFirstAdd = nstEEPAllDataBlock_Tbl[aubTbNum].uhBlockStrA;
		auhBlkEndAdd =auhBlkFirstAdd + nstEEPAllDataBlock_Tbl[aubTbNum].uhBlockSize;
	
		if ((auhBlkFirstAdd <= auhStaAdr) && ((auhStaAdr + auhRdSize) <= auhBlkEndAdd))
		{
			*aubBlockIndex = aubTbNum;
			*auhOffset = auhStaAdr - auhBlkFirstAdd;
			aubReturn = RES_OK;
			break;
		}
	}
	
	return (aubReturn);
}

/*********************************************************************************************
*  function                 : wubBlockReadCheck
*  date                     : 2021.11.01
*  autho                    : zhangbo
*  description              : Function Call when BlockReadCheck
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*  
*********************************************************************************************/
Type_uByte wubBlockReadCheck(Type_uHWord auhStaAdr, Type_uHWord auhRdSize)
{
	Type_uByte			aubTbNum;
	Type_uByte			aubTbSize;
	Type_uHWord 		auhBlkFirstAdd;
	Type_uHWord 		auhBlkEndAdd;
	Type_uByte			aubReturn;
	
	aubTbNum			= EEP_NUM_0;
	auhBlkFirstAdd		= EEP_NUM_0;
	auhBlkEndAdd		= EEP_NUM_0;
	aubReturn			= RES_NG;
	aubTbSize			= sizeof(nstEEPAllDataBlock_Tbl)/sizeof(EEP_AllDataBlockTbl_ts);
	
	for(aubTbNum = EEP_NUM_0;aubTbNum < aubTbSize;aubTbNum++)
	{
		auhBlkFirstAdd = nstEEPAllDataBlock_Tbl[aubTbNum].uhBlockStrA;
		auhBlkEndAdd = auhBlkFirstAdd + nstEEPAllDataBlock_Tbl[aubTbNum].uhBlockSize;
	
		if ((auhBlkFirstAdd <= auhStaAdr) && ((auhStaAdr + auhRdSize) <= auhBlkEndAdd))
		{
			aubReturn = RES_OK;
			break;
		}
	}
	
	return (aubReturn);
}

/*********************************************************************************************
*  function                 : wubBlockWriteCheck
*  date                     : 2021.11.01
*  autho                    : zhangbo
*  description              : Function Call when BlockWriteCheck
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*  
*********************************************************************************************/
Type_uByte wubBlockWriteCheck(Type_uHWord auhStaAdr, Type_uHWord auhRdSize,Type_uByte aubWriteTpye)
{
	Type_uByte			aubTbNum;
	Type_uByte			aubTbSize;
	Type_uHWord 		auhBlkFirstAdd;
	Type_uHWord 		auhBlkEndAdd;
	Type_uByte			aubReturn;

	aubTbNum			= EEP_NUM_0;
	auhBlkFirstAdd		= EEP_NUM_0;
	auhBlkEndAdd		= EEP_NUM_0;
	aubReturn			= RES_NG;
	aubTbSize			= sizeof(nstEEPAllDataBlock_Tbl)/sizeof(EEP_AllDataBlockTbl_ts);

	for(aubTbNum = EEP_NUM_0;aubTbNum < aubTbSize;aubTbNum++)
	{
		auhBlkFirstAdd = nstEEPAllDataBlock_Tbl[aubTbNum].uhBlockStrA;
		auhBlkEndAdd =auhBlkFirstAdd+ nstEEPAllDataBlock_Tbl[aubTbNum].uhBlockSize;

		if ( (auhBlkFirstAdd <= auhStaAdr) && ((auhStaAdr + auhRdSize)<= auhBlkEndAdd ) 
			&&(( EEP_W == (nstEEPAllDataBlock_Tbl[aubTbNum].ubDataType & EEP_W)) || ( aubWriteTpye == EEP_WRITABLE_ON )))
		{
			aubReturn = RES_OK;
			break;
		}
		else
		{
			;
		}
	}

	return (aubReturn);
}

/*********************************************************************************************
*  function                 : wubEEPBlockWrite
*  date                     : 2021.11.01
*  autho                    : zhangbo
*  description              : Function Call when EEPBlockWrite
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*  
*********************************************************************************************/
Type_uByte wubEEPBlockWrite(Type_uHWord auhStaBlk, Type_uHWord auhWtSize,const Type_uByte *aubBuff_p)
{
	Type_uByte		aubRtnCode;
	Type_sByte		asbReturn;
	
	Type_uByte		aubCheckSum;
	Type_uByte		aubBlockSize;
	Type_uByte		aubBlockIndex;
	Type_uHWord		auhOffset;
	
	Type_uHWord		auhStrA;
	Type_uHWord		auhSize;
	OS_VoidPtr_t 	avdMpf_p;

	aubRtnCode 		= RES_NG;
	asbReturn 		= HAL_OK;
	aubCheckSum 	= EEP_NUM_0;
	aubBlockIndex	= EEP_NUM_0;
	aubBlockSize	= EEP_NUM_0;
	avdMpf_p 		= STD_NULL;
	
	aubRtnCode = nubEEPGetBlockIndex(auhStaBlk,auhWtSize,&aubBlockIndex,&auhOffset);
	if((RES_OK == aubRtnCode) && (aubBlockIndex <= (sizeof(nstEEPAllDataBlock_Tbl)/sizeof(EEP_AllDataBlockTbl_ts)))
		&& (auhOffset <= nstEEPAllDataBlock_Tbl[aubBlockIndex].uhBlockSize))
	{
		if(aubBuff_p != STD_NULL)
		{
			if( (nstEEPAllDataBlock_Tbl[aubBlockIndex].ubDataType  & EEP_S) == EEP_S ){
				wvdGen_MemCpy(&nstEEPAllDataBlock_Tbl[aubBlockIndex].ubData_P[auhOffset],aubBuff_p, auhWtSize);

				asbReturn = HAL_Ioctl(nvbEEP_I2cHalHandle,HAL_IO_EEP_WRITE,&auhStaBlk,&auhWtSize,NULL,(void*)aubBuff_p);

				if( HAL_OK != asbReturn){
					aubRtnCode = RES_HARDWARE_NG;
					EEP_DBG("\n  HAL_Err = %d",asbReturn);
				}

			}else{
				/* chacksum length prep */
				aubBlockSize = nstEEPAllDataBlock_Tbl[aubBlockIndex].uhBlockSize - 1;
				wvdGen_MemCpy(&nstEEPAllDataBlock_Tbl[aubBlockIndex].ubData_P[auhOffset],aubBuff_p, auhWtSize);
				aubCheckSum = nubEepDrvCRCCalc(nstEEPAllDataBlock_Tbl[aubBlockIndex].ubData_P,aubBlockSize);		
				nstEEPAllDataBlock_Tbl[aubBlockIndex].ubData_P[aubBlockSize] = aubCheckSum;

				auhStrA	= nstEEPAllDataBlock_Tbl[aubBlockIndex].uhBlockStrA;
				auhSize	= nstEEPAllDataBlock_Tbl[aubBlockIndex].uhBlockSize;
				
				asbReturn = OS_GetFMemoryBlock(&avdMpf_p, auhSize);
				
				if( RES_OK == asbReturn ){
					wvdGen_MemCpy(avdMpf_p,nstEEPAllDataBlock_Tbl[aubBlockIndex].ubData_P, auhSize);
					asbReturn = HAL_Ioctl(nvbEEP_I2cHalHandle,HAL_IO_EEP_WRITE,&auhStrA,&auhSize,NULL,(void*)avdMpf_p);
					if( HAL_OK != asbReturn){
						EEP_DBG("\n  HAL_Err = %d",asbReturn);
						aubRtnCode = RES_HARDWARE_NG;
					}
				}else{
					aubRtnCode = RES_NG;
					EEP_DBG("\n  NO_SPACE ");
				}
			}
		}else{  
			aubRtnCode = RES_NG;
			EEP_DBG("\n aubBuff_p = STD_NULL");
		}
	}else
	{
		aubRtnCode = RES_NG;
		EEP_DBG("\n Write range error");
	}

	/* Get Memory Block Check */
	if (avdMpf_p != STD_NULL) {
		OS_ReleaseFMemoryBlock(avdMpf_p);
	} else {
		/* No Action */
		;
	}
		
	return(aubRtnCode);
}

/*********************************************************************************************
*  function                 : wubEEPLINETOOLAPPAllClear
*  date                     : 2021.11.01
*  autho                    : zhangbo
*  description              : Function Call when EEPDataAllClear
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*  
*********************************************************************************************/
Type_uByte wubEEPLINETOOLAPPAllClear(void)
{  
	Type_uHWord 	        auhSize;
	Type_uHWord    			auhStartAdd;
	Type_uByte 				aubRtn;	
	Type_sByte				asbReturn;
	
	aubRtn 		= RES_OK;
	asbReturn 	= HAL_OK;
	auhSize		= EEP_SIZE_32;
	auhStartAdd = EEP_ADDR_CLEAR;
	
	wvdGen_MemSet(wubEepRamMap_misc, 0xFF, sizeof(wubEepRamMap_misc));
	wvdGen_MemSet(wubEepRamMap_log, 0xFF, sizeof(wubEepRamMap_log));
	wvdGen_MemSet(wubEepRamMap_CarPlayID, 0xFF, sizeof(wubEepRamMap_CarPlayID));
	wvdGen_MemSet(wubEepRamMap_PartID, 0xFF, sizeof(wubEepRamMap_PartID));
	wvdGen_MemSet(wubEepRamMap_Serial, 0xFF, sizeof(wubEepRamMap_Serial));
	wvdGen_MemSet(wubEepRamMap_Date, 0xFF, sizeof(wubEepRamMap_Date));
	wvdGen_MemSet(wubEepRamMap_Osc, 0xFF, sizeof(wubEepRamMap_Osc));
	wvdGen_MemSet(wubEepRamMap_Modle, 0xFF, sizeof(wubEepRamMap_Modle));
	wvdGen_MemSet(wubEepRamMap_Transit, 0xFF, sizeof(wubEepRamMap_Transit));
	wvdGen_MemSet(wubEepRamMap_Traceability, 0xFF, sizeof(wubEepRamMap_Traceability));
	wvdGen_MemSet(wubEepRamMap_Self, 0xFF, sizeof(wubEepRamMap_Self));
	wvdGen_MemSet(wubEepRamMap_Hac, 0xFF, sizeof(wubEepRamMap_Hac));

	do{
		asbReturn = HAL_Ioctl(nvbEEP_I2cHalHandle,HAL_IO_EEP_WRITE,&auhStartAdd,&auhSize,NULL,&wubEepAllClearMap_a);
		if( HAL_OK!= asbReturn){
			EEP_DBG("\n  HAL_Err = %d",asbReturn);
			LOG_OUTPUT( LOG_APP_ID_EEP, DEBUGLOG_LEVEL_ERROR,ATAT_LEVEL_OFF,"\n  APPAllClear HAL_Err = %d",asbReturn);
			aubRtn = RES_HARDWARE_NG;
			break;
		}else{
			auhStartAdd = auhStartAdd + EEP_SIZE_32;
		}
	}while(auhStartAdd < EEP_ADDR_END);

	return aubRtn;
}

/*********************************************************************************************
*  function                 : wubEEPDataDefaultSet
*  date                     : 2021.11.01
*  autho                    : zhangbo
*  description              : Function Call when EEPDataDefaultSet
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*  
*********************************************************************************************/
Type_uByte wubEEPDataDefaultSet(void)
{
	Type_uByte    aubPoz;       /* Block Number */
	Type_uHWord	  auhStarAdr;
	Type_uHWord	  auhDataSize;
	Type_uByte		aubRtn;	
	Type_sByte		asbReturn;
	Type_uByte	  aubTbSize;
	Type_uByte   *aubDefaultBuff_P;
	
	aubPoz 			= EEP_CLEAR;
	auhStarAdr 		= EEP_CLEAR;
	auhDataSize 	= EEP_CLEAR;
	asbReturn		= HAL_OK;
	aubRtn 			= RES_OK;
	aubDefaultBuff_P= STD_NULL;
	aubTbSize 		= sizeof(nstEEPAllDataBlock_Tbl)/sizeof(EEP_AllDataBlockTbl_ts);

	for(aubPoz = EEP_CLEAR; aubPoz < aubTbSize; aubPoz++)
	{
		if((nstEEPAllDataBlock_Tbl[aubPoz].ubDataType & EEP_C) == EEP_C)
		{
			auhStarAdr 	= nstEEPAllDataBlock_Tbl[aubPoz].uhBlockStrA;		/* Block Start Adress */
			auhDataSize = nstEEPAllDataBlock_Tbl[aubPoz].uhBlockSize;		/* Block Data Length */
			aubDefaultBuff_P = nstEEPAllDataBlock_Tbl[aubPoz].ubDefaultData_P;
			asbReturn = HAL_Ioctl(nvbEEP_I2cHalHandle,HAL_IO_EEP_WRITE,&auhStarAdr,&auhDataSize,NULL,aubDefaultBuff_P);
			if( HAL_OK !=asbReturn ){
				EEP_DBG("\n DefaultSet HAL_Err = %d",asbReturn);
				LOG_OUTPUT( LOG_APP_ID_EEP, DEBUGLOG_LEVEL_ERROR,ATAT_LEVEL_OFF,"\n  DefaultSet HAL_Err = %d",asbReturn);
				aubRtn = RES_HARDWARE_NG;
			}else{
				;
			}
		}
		else
		{
			;
		}
	}

	return aubRtn;
}
/*********************************************************************************************
*  function                 : wubEEPLogClear
*  date                     : 2021.11.01
*  autho                    : zhangbo
*  description              : Function Call when EEPDataAllClear
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*  
*********************************************************************************************/
static void nvdEEPLogClear(void)
{  
	Type_uHWord 	        auhSize;
	Type_uHWord    			auhStartAdd;
	Type_sByte				asbReturn;

	asbReturn = HAL_OK;

	auhSize		= EEP_SIZE_32;
	auhStartAdd	= EEP_ADDR_LOG;
	
	wvdGen_MemSet(wubEepRamMap_log, 0xFF, sizeof(wubEepRamMap_log));
	asbReturn = HAL_Ioctl(nvbEEP_I2cHalHandle,HAL_IO_EEP_WRITE,&auhStartAdd,&auhSize,NULL,&wubEepAllClearMap_a);
	if( HAL_OK !=asbReturn ){
		EEP_DBG("\n  nvdEEPLogClear HAL_Err = %d",asbReturn);
	}
	
}

/********************************************************************************************/
/**	\function		wvdEEPLOGWrite
 *	\date			2021/02/21
 *	\author			xulei
 *	\description	EEP Hal Open
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 * 
  *********************************************************************************************
 *	\par	performance:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 * 12.9ms Syn interface
 ********************************************************************************************/
void wvdEEPLOGWrite(Type_uByte	tpye,Type_uByte	data)
{
	Type_uByte	aubCount;
	Type_uByte 	aubLogBuff[EEP_BLOCK_LENGTH];
	
	aubCount = EEP_VALUE_0;
	wvdGen_MemSet(aubLogBuff, EEP_VALUE_0, sizeof(aubLogBuff));

	EEP_ENABLE;
	eeprom_init();

	if ( RES_OK != nubEEPReadLog(EEP_ADDR_COUNT, EEP_BLOCK_LENGTH, aubLogBuff)){
		EEP_DBG("\n  nubEEPReadLog Err");
		return;
	}
	aubCount = aubLogBuff[TPYE_COUNT] + EEP_VALUE_1;
	aubLogBuff[TPYE_COUNT] = aubCount ;
	
	switch( tpye ){
		case TYPE_HW:
			aubLogBuff[TYPE_HW] = data;
			break;
		case TYPE_TASK:
			aubLogBuff[TYPE_TASK] = data;
			break;
		case TYPE_SEAM:
			aubLogBuff[TYPE_SEAM] = data;
			break;
		case TYPE_EVENT:
			aubLogBuff[TYPE_EVENT] = data;
			break;
		case TYPE_QUEUE:
			aubLogBuff[TYPE_QUEUE] = data;
			break;
		case TYPE_TIMER:
			aubLogBuff[TYPE_TIMER] = data;
			break;
		case TYPE_PMMNG:
			aubLogBuff[TYPE_PMMNG] = data;
			break;
		case TYPE_PMDRV:
			aubLogBuff[TYPE_PMDRV] = data;
			break;
		default:
			break;
	}

	eeprom_write(EEP_ADDR_COUNT,EEP_BLOCK_LENGTH, aubLogBuff);
	
	EEP_DISABLE;

	eeprom_Stop();

}
/********************************************************************************************/
/**	\function		eeprom_init
 *	\date			2022/04/29
 *	\author			xulei
 *	\description	EEP Hal Open
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *Call the underlying interface directly
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 * 
  *********************************************************************************************
 *	\par	performance:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 * 
 ********************************************************************************************/

Type_uByte eeprom_init(void)
{
	Type_sWord 	alRet;
	Type_uByte	aubRtnCode;
	
	alRet = BSP_ERR_NONE;
	aubRtnCode = RES_OK;

	alRet = tcc_i2c_open(EEP_I2C_CHANNEL,
		PM_EEP_I2C_SCL,
		PM_EEP_I2C_SDA,
		EEP_I2C_SCL_kHZ,
		0,0);
	if( alRet == BSP_ERR_NONE){
		aubRtnCode = RES_OK;
	}else{
		aubRtnCode= RES_NG;
	}
	return aubRtnCode;
}
/********************************************************************************************/
/**	\function		eeprom_Stop
 *	\date			2022/04/29
 *	\author			xulei
 *	\description	EEP Hal Open
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		exinf - parameter(no use)
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 * Call the underlying interface directly
  *********************************************************************************************
 *	\par	performance:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 * 
 ********************************************************************************************/

Type_uByte eeprom_Stop(void)
{
	Type_sWord 	alRet;
	Type_uByte	aubRtnCode;
	
	alRet = BSP_ERR_NONE;
	aubRtnCode = RES_OK;

	alRet = tcc_i2c_close(EEP_I2C_CHANNEL);

	if( alRet == BSP_ERR_NONE){
		aubRtnCode = RES_OK;
	}else{
		aubRtnCode= RES_NG;
	}
	return aubRtnCode;


}


