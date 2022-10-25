/*******************************************************************************************/
/**<!-------Copyright (C) 2021 Alpine Electronics, Inc. All rights reserved. --------------->
 * 
 * \file					Eep_if.c
 * \date					2021/11/01
 * \author					zhangbo
 * \model					EDA
 * \description				Agent Control Routine
 * \version					0.55
 *
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/11/01		zhangbo		New
 ********************************************************************************************/


/********************************************************************************************/
/*                      	Include File Section                           				    */
/********************************************************************************************/
#include  "model.h"
#include "apn_basic_type.h"
#include "os_framework.h"
#include "generic.h"
#include "rpc_if.h"
#include "HAL_devices.h"
#include "HAL_Eep.h"
#include "Eepromcontrol.h"
#include "Eep_core.h"
#include "rpc_event.h"
#include "logapp.h" 
#include <app_cfg.h>

#if (SERVICE_TEST_EN == 1u)
#include "Robust_Test.h" 
#endif


/********************************************************************************************/
/*							Macro Definition Section										*/
/********************************************************************************************/

/********************************************************************************************/
/*							Type Definition Section											*/
/********************************************************************************************/
const Variation_Table_ts Variation_G11[] = {
 /*ubVariation_Index                    ubCarType_Variation_0x10                ubCarType_Variation_0x20                ubCarType_Variation_0x30                ubCarType_Variation_0x40                ubCarType_Variation_0x50                ubCarType_Variation_0x60              **/
{EN_VARI_FUNC_MIN,				        USELESS_NUM,			                USELESS_NUM,				            USELESS_NUM,			                USELESS_NUM,		                    USELESS_NUM,	                        USELESS_NUM                             },      
{EN_VARI_FUNC_DESTINATION,				EN_VARI_RES_FUNC_DEST_ID,			    EN_VARI_RES_FUNC_DEST_KN,				EN_VARI_RES_FUNC_DEST_ID,			    EN_VARI_RES_FUNC_DEST_KN,		        EN_VARI_RES_FUNC_DEST_ID,	            EN_VARI_RES_FUNC_DEST_KN                },      
{EN_VARI_FUNC_HANDLE,	 				EN_VARI_RES_FUNC_HANDLE_RH,			    EN_VARI_RES_FUNC_HANDLE_RH,	 			EN_VARI_RES_FUNC_HANDLE_RH,	 		    EN_VARI_RES_FUNC_HANDLE_RH,	 	        EN_VARI_RES_FUNC_HANDLE_RH,	            EN_VARI_RES_FUNC_HANDLE_RH              },
{EN_VARI_FUNC_DISP_SIZE,				EN_VARI_RES_FUNC_DISP_INCH_8,		    EN_VARI_RES_FUNC_DISP_INCH_8,			EN_VARI_RES_FUNC_DISP_INCH_10,		    EN_VARI_RES_FUNC_DISP_INCH_10,	        EN_VARI_RES_FUNC_DISP_INCH_8,	        EN_VARI_RES_FUNC_DISP_INCH_8            },         
{EN_VARI_FUNC_DEFAULT_LANGUAGE,			EN_VARI_RES_FUNC_DFT_LAGE_UKENG,	    EN_VARI_RES_FUNC_DFT_LAGE_UKENG,		EN_VARI_RES_FUNC_DFT_LAGE_UKENG,	    EN_VARI_RES_FUNC_DFT_LAGE_UKENG,	    EN_VARI_RES_FUNC_DFT_LAGE_UKENG,	    EN_VARI_RES_FUNC_DFT_LAGE_UKENG         },
{EN_VARI_FUNC_RADIO_NOR,				EN_VARI_RES_FUNC_EXIST,				    EN_VARI_RES_FUNC_EXIST,					EN_VARI_RES_FUNC_EXIST,				    EN_VARI_RES_FUNC_EXIST,	                EN_VARI_RES_FUNC_EXIST,	                EN_VARI_RES_FUNC_EXIST                  },
{EN_VARI_FUNC_RADIO_DBS,				EN_VARI_RES_FUNC_NOT_EXIST,			    EN_VARI_RES_FUNC_NOT_EXIST,				EN_VARI_RES_FUNC_NOT_EXIST,			    EN_VARI_RES_FUNC_NOT_EXIST,	            EN_VARI_RES_FUNC_NOT_EXIST,	            EN_VARI_RES_FUNC_NOT_EXIST              },
{EN_VARI_FUNC_RADIO_RDS,				EN_VARI_RES_FUNC_EXIST,				    EN_VARI_RES_FUNC_EXIST,					EN_VARI_RES_FUNC_EXIST,				    EN_VARI_RES_FUNC_EXIST,	                EN_VARI_RES_FUNC_EXIST,	                EN_VARI_RES_FUNC_EXIST                  },
{EN_VARI_FUNC_RADIO_FREQ,				EN_VARI_RES_FUNC_RADIO_FREQ_GEN,	    EN_VARI_RES_FUNC_RADIO_FREQ_GEN,		EN_VARI_RES_FUNC_RADIO_FREQ_GEN,	    EN_VARI_RES_FUNC_RADIO_FREQ_GEN,	    EN_VARI_RES_FUNC_RADIO_FREQ_GEN,	    EN_VARI_RES_FUNC_RADIO_FREQ_GEN         },
{EN_VARI_FUNC_BT_AUDIO,					EN_VARI_RES_FUNC_EXIST,				    EN_VARI_RES_FUNC_EXIST,					EN_VARI_RES_FUNC_EXIST,				    EN_VARI_RES_FUNC_EXIST,	                EN_VARI_RES_FUNC_EXIST,	                EN_VARI_RES_FUNC_EXIST                  },
{EN_VARI_FUNC_BT_HFT,					EN_VARI_RES_FUNC_EXIST,				    EN_VARI_RES_FUNC_EXIST,					EN_VARI_RES_FUNC_EXIST,				    EN_VARI_RES_FUNC_EXIST,	                EN_VARI_RES_FUNC_EXIST,	                EN_VARI_RES_FUNC_EXIST                  },
{EN_VARI_FUNC_BT_RCAPP,					EN_VARI_RES_FUNC_EXIST,				    EN_VARI_RES_FUNC_EXIST,					EN_VARI_RES_FUNC_EXIST,				    EN_VARI_RES_UNDEFINE,	                EN_VARI_RES_UNDEFINE,	                EN_VARI_RES_UNDEFINE                    },
{EN_VARI_FUNC_CARPALY_WIRED,			EN_VARI_RES_FUNC_EXIST,				    EN_VARI_RES_FUNC_EXIST,					EN_VARI_RES_FUNC_EXIST,				    EN_VARI_RES_FUNC_EXIST,	                EN_VARI_RES_FUNC_EXIST,	                EN_VARI_RES_FUNC_EXIST                  },
{EN_VARI_FUNC_CARPALY_WIRELESS,			EN_VARI_RES_FUNC_EXIST,				    EN_VARI_RES_FUNC_EXIST,					EN_VARI_RES_FUNC_EXIST,				    EN_VARI_RES_FUNC_EXIST,	                EN_VARI_RES_FUNC_EXIST,	                EN_VARI_RES_FUNC_EXIST                  },
{EN_VARI_FUNC_AAP_WIRED,				EN_VARI_RES_FUNC_EXIST,				    EN_VARI_RES_FUNC_EXIST,					EN_VARI_RES_FUNC_EXIST,				    EN_VARI_RES_FUNC_EXIST,	                EN_VARI_RES_FUNC_EXIST,	                EN_VARI_RES_FUNC_EXIST                  },
{EN_VARI_FUNC_AAP_WIRELESS,				EN_VARI_RES_FUNC_EXIST,				    EN_VARI_RES_FUNC_EXIST,					EN_VARI_RES_FUNC_EXIST,				    EN_VARI_RES_FUNC_EXIST,	                EN_VARI_RES_FUNC_EXIST,	                EN_VARI_RES_FUNC_EXIST                  },
{EN_VARI_FUNC_WLINK_WIRED,				EN_VARI_RES_FUNC_EXIST,				    EN_VARI_RES_FUNC_EXIST,					EN_VARI_RES_FUNC_EXIST,				    EN_VARI_RES_FUNC_EXIST,	                EN_VARI_RES_FUNC_EXIST,	                EN_VARI_RES_FUNC_EXIST                  },
{EN_VARI_FUNC_WIFI_CH_DUAL,				EN_VARI_RES_FUNC_NOT_EXIST,			    EN_VARI_RES_FUNC_NOT_EXIST,				EN_VARI_RES_FUNC_NOT_EXIST,			    EN_VARI_RES_FUNC_NOT_EXIST,	            EN_VARI_RES_FUNC_NOT_EXIST,	            EN_VARI_RES_FUNC_NOT_EXIST              },
{EN_VARI_FUNC_WIFI_CH_2,				EN_VARI_RES_FUNC_NOT_EXIST,			    EN_VARI_RES_FUNC_NOT_EXIST,				EN_VARI_RES_FUNC_NOT_EXIST,			    EN_VARI_RES_FUNC_NOT_EXIST,	            EN_VARI_RES_FUNC_NOT_EXIST,	            EN_VARI_RES_FUNC_NOT_EXIST              },
{EN_VARI_FUNC_WIFI_CH_5,				EN_VARI_RES_FUNC_EXIST,				    EN_VARI_RES_FUNC_EXIST,					EN_VARI_RES_FUNC_EXIST,				    EN_VARI_RES_FUNC_EXIST,	                EN_VARI_RES_FUNC_EXIST,	                EN_VARI_RES_FUNC_EXIST                  },
{EN_VARI_FUNC_WIFI_SETTING,				EN_VARI_RES_FUNC_WIFI_DFT_ON,		    EN_VARI_RES_FUNC_WIFI_DFT_OFF,			EN_VARI_RES_FUNC_WIFI_DFT_ON,		    EN_VARI_RES_FUNC_WIFI_DFT_OFF,	        EN_VARI_RES_FUNC_WIFI_DFT_ON,	        EN_VARI_RES_FUNC_WIFI_DFT_OFF           },
{EN_VARI_FUNC_RWC_NORMAL,				EN_VARI_RES_FUNC_NOT_EXIST,			    EN_VARI_RES_FUNC_NOT_EXIST,				EN_VARI_RES_FUNC_NOT_EXIST,			    EN_VARI_RES_FUNC_NOT_EXIST,	            EN_VARI_RES_FUNC_NOT_EXIST,	            EN_VARI_RES_FUNC_NOT_EXIST              },
{EN_VARI_FUNC_RWC_WIDE,					EN_VARI_RES_FUNC_EXIST,				    EN_VARI_RES_FUNC_EXIST,					EN_VARI_RES_FUNC_EXIST,				    EN_VARI_RES_FUNC_EXIST,	                EN_VARI_RES_FUNC_EXIST,	                EN_VARI_RES_FUNC_EXIST                  },
{EN_VARI_FUNC_LWC,						EN_VARI_RES_FUNC_EXIST,				    EN_VARI_RES_FUNC_EXIST,					EN_VARI_RES_FUNC_EXIST,				    EN_VARI_RES_FUNC_EXIST,	                EN_VARI_RES_FUNC_EXIST,	                EN_VARI_RES_FUNC_EXIST                  },
{EN_VARI_FUNC_COM_BCAN,					EN_VARI_RES_FUNC_NOT_EXIST,			    EN_VARI_RES_FUNC_NOT_EXIST,				EN_VARI_RES_FUNC_NOT_EXIST,			    EN_VARI_RES_FUNC_NOT_EXIST,	            EN_VARI_RES_FUNC_NOT_EXIST,	            EN_VARI_RES_FUNC_NOT_EXIST              },
{EN_VARI_FUNC_COM_KLINE,				EN_VARI_RES_FUNC_EXIST,				    EN_VARI_RES_FUNC_EXIST,					EN_VARI_RES_FUNC_EXIST,				    EN_VARI_RES_FUNC_EXIST,	                EN_VARI_RES_FUNC_EXIST,	                EN_VARI_RES_FUNC_EXIST                  },
{EN_VARI_FUNC_MULT_LAGE_UKENG,			EN_VARI_RES_FUNC_EXIST,				    EN_VARI_RES_FUNC_EXIST,					EN_VARI_RES_FUNC_EXIST,				    EN_VARI_RES_FUNC_EXIST,	                EN_VARI_RES_FUNC_EXIST,	                EN_VARI_RES_FUNC_EXIST                  },
{EN_VARI_FUNC_MULT_LAGE_FRENCH,			EN_VARI_RES_FUNC_NOT_EXIST,			    EN_VARI_RES_FUNC_EXIST,					EN_VARI_RES_FUNC_NOT_EXIST,			    EN_VARI_RES_FUNC_EXIST,	                EN_VARI_RES_FUNC_NOT_EXIST,	            EN_VARI_RES_FUNC_EXIST                  },
{EN_VARI_FUNC_TIME_ZONE,				EN_VARI_RES_FUNC_TIME_P_0530,		    EN_VARI_RES_FUNC_TIME_P_0200,			EN_VARI_RES_FUNC_TIME_P_0530,		    EN_VARI_RES_FUNC_TIME_P_0200,	        EN_VARI_RES_FUNC_TIME_P_0530,	        EN_VARI_RES_FUNC_TIME_P_0200            },
{EN_VARI_FUNC_MAX,				        USELESS_NUM,			                USELESS_NUM,				            USELESS_NUM,			                USELESS_NUM,		                    USELESS_NUM,	                        USELESS_NUM                             },      
};

/********************************************************************************************/
/*							Enumeration Type Definition Section								*/
/********************************************************************************************/

/********************************************************************************************/
/*							Structure/Union Type Definition Section							*/
/********************************************************************************************/

/********************************************************************************************/
/*							Global Variable Definition Section								*/
/********************************************************************************************/

/********************************************************************************************/
/*							Static Variable Definition Section								*/
/********************************************************************************************/

/********************************************************************************************/
/*							Static Prototype Declaration Section							*/
/********************************************************************************************/

/********************************************************************************************/
/*							ROM Table Section												*/
/********************************************************************************************/

/********************************************************************************************/
/*							Prototype Definition Section									*/
/********************************************************************************************/

/********************************************************************************************/
/*							Function Definition Section										*/
/********************************************************************************************/

/*********************************************************************************************
*  function                 : wvdEEP_Init
*  date                     : 2021.11.01
*  autho                    : zhangbo
*  description              : Function Call when Init
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
Type_uByte wubEEP_Init(void)
 {
	wubEEPModuleState = EEP_STATE_STANDY;		/* Module State Unload */

	return RES_OK;
 }
 
/*********************************************************************************************
*  function                 : wvdEEP_EnforceStandby
*  date                     : 2021.11.01
*  autho                    : zhangbo
*  description              : Function Call when EnforceStandby
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
Type_uByte wubEEP_EnforceStandby(void)
{
#if (EEP_1_1_1 == 1u)
	PF_TEST_START;
#endif

	if(NULL != nvbEEP_I2cHalHandle){
		HAL_Close(nvbEEP_I2cHalHandle);
	}
	nvdEEP_SW_Reset();
	
#if (EEP_1_1_1 == 1u)
	PF_TEST_STOP;
#endif
	return RES_OK;
}

/*********************************************************************************************
*  function                 : wubEEP_StandbyOut
*  date                     : 2021.11.01
*  autho                    : zhangbo
*  description              : Function Call when EEP_Load
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
Type_uByte wubEEP_StandbyOut(void)
{
	Type_uByte	aubRtnCode;
	aubRtnCode 	= RES_NG;

	aubRtnCode = wubEEP_SendMail(MBX_ID_POWER,EV_INF_STANDBYOUT_NOTIFY,STD_NULL,STD_NULL);
	if (aubRtnCode == RES_OK){
		aubRtnCode = RES_START;
	}
	else{
		aubRtnCode = RES_NG;
	}
	return (aubRtnCode);	
}
/*********************************************************************************************
*  function                 : wubEEP_StandbyIn
*  date                     : 2021.11.01
*  autho                    : zhangbo
*  description              : Function Call when EEP_Unload
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
Type_uByte wubEEP_StandbyIn(void)
{
	Type_uByte	aubRtnCode;
	aubRtnCode = RES_NG;                    /* Result NG Set */
	aubRtnCode = wubEEP_SendMail(MBX_ID_POWER,EV_INF_STANDBYIN_NOTIFY,STD_NULL,STD_NULL);
	if (aubRtnCode == RES_OK){
		aubRtnCode = RES_START;
	}
	else{
		;
	}	
	return (aubRtnCode);
}

/*********************************************************************************************
*  function                 : nvdEEP_SW_Reset
*  date                     : 2022.01.26
*  author                   : 
*  description              : 
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  \param[in]		none
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  \return			none
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*  
*********************************************************************************************/
void nvdEEP_SW_Reset(void)
{
	Type_uByte	aubCount;

	gpio_config(PM_EEP_I2C_SCL, GPIO_FUNC(0)|GPIO_OUTPUT);
	gpio_config(PM_EEP_I2C_SDA, GPIO_FUNC(0)|GPIO_OUTPUT);
	
	EEP_SCL_L;
	EEP_SDA_H;
	OS_WaitTime(2);   /* 2usec Wait */

	for( aubCount = 0;aubCount<=EEP_RESET_COUNT;aubCount++ ){
		EEP_SCL_H;
		OS_WaitTime(2);   /* 2usec Wait */
		EEP_SCL_L;
		OS_WaitTime(2);   /* 2usec Wait */
	}

	nvdEEP_SW_StartCondition();
	nvdEEP_SW_StartCondition();
}
/*********************************************************************************************
*  function                 : nvdEEP_SW_StartCondition
*  date                     : 2022.01.26
*  author                   : 
*  description              : 
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  \param[in]		none
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  \return			none
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*  
*********************************************************************************************/

void nvdEEP_SW_StartCondition(void)
{
	EEP_SCL_H;
	OS_WaitTime(1);   /* 2usec Wait */
	EEP_SDA_L;
	OS_WaitTime(1);   /* 2usec Wait */
	EEP_SCL_L;
	OS_WaitTime(1);   /* 2usec Wait */
	EEP_SDA_H;
	OS_WaitTime(1);   /* 2usec Wait */
}
/*********************************************************************************************
*  function                 : wubEEP_BlockRead
*  date                     : 2021.11.01
*  autho                    : zhangbo
*  description              : Function Call when EEP_BlockRead
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
Type_uByte wubEEP_BlockRead(Type_uHWord auhStaAdr,Type_uHWord auhRdSize,Type_uByte* aubBuff_p)
{
	Type_uByte			aubRtnCode;
	aubRtnCode 			= RES_NG;

	
	if (wubEEPModuleState == EEP_STATE_IDLE)	/* State IDLE */
	{	
		wubEEPModuleState = EEP_STATE_BUSY;

		aubRtnCode = wubBlockReadCheck(auhStaAdr, auhRdSize);		
			
		if (RES_OK == aubRtnCode)
		{				
			aubRtnCode = wubDirectReadRAM(auhStaAdr, auhRdSize, aubBuff_p);
		}
		else
		{
			EEP_DBG("\n Read range error");
			LOG_OUTPUT( LOG_APP_ID_EEP, DEBUGLOG_LEVEL_WARN,ATAT_LEVEL_OFF,"\n Read range error");
			
		}
		wubEEPModuleState = EEP_STATE_IDLE;
	}
	else
	{	
		aubRtnCode = RES_BUSY;
	}
	
	return (aubRtnCode);
}

/*********************************************************************************************
*  function                 : wubEEP_BlockWrite
*  date                     : 2021.11.01
*  autho                    : zhangbo
*  description              : Function Call when EEP_BlockWrite
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
Type_uByte wubEEP_BlockWrite(Type_uHWord auhStaBlk, Type_uHWord auhWtSize,const Type_uByte *aubBuff_p)
{
	Type_uByte		aubRtnCode;

	aubRtnCode = RES_NG;    /* Result NG Set */	

	if (wubEEPModuleState == EEP_STATE_IDLE)
	{
		wubEEPModuleState = EEP_STATE_BUSY;

		aubRtnCode = wubBlockWriteCheck(auhStaBlk, auhWtSize,EEP_WRITABLE_OFF);

		if (RES_OK == aubRtnCode)
		{
			aubRtnCode = wubEEPBlockWrite(auhStaBlk, auhWtSize, aubBuff_p);
		}
		else
		{
			EEP_DBG("\n Write range error");
			LOG_OUTPUT( LOG_APP_ID_EEP, DEBUGLOG_LEVEL_ERROR,ATAT_LEVEL_OFF,"\n Read range error");
		}
		wubEEPModuleState = EEP_STATE_IDLE;	
	}
	else
	{
		aubRtnCode = RES_BUSY;/* Module Status Busy */
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
Type_uByte wubEEP_LineToolReadEEP(Type_uHWord auhStaAdr, Type_uHWord auhRdSize, Type_uByte* aubBuff_p)
{
	Type_uByte		aubRtnCode;
	aubRtnCode 		= RES_NG;
	
	if (wubEEPModuleState == EEP_STATE_IDLE)
	{
		wubEEPModuleState = EEP_STATE_BUSY;

		aubRtnCode = wubBlockReadCheck(auhStaAdr, auhRdSize);

		if(RES_OK == aubRtnCode)
		{
			aubRtnCode = wubLineToolReadEEP(auhStaAdr,auhRdSize,aubBuff_p);
		}
		else
		{
			EEP_DBG("\n Read range error");
			LOG_OUTPUT( LOG_APP_ID_EEP, DEBUGLOG_LEVEL_ERROR,ATAT_LEVEL_OFF,"\n LineToolRead range error");
		}
		wubEEPModuleState = EEP_STATE_IDLE;
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
Type_uByte wubEEP_LineToolWriteEEP(Type_uHWord auhStaAdr, Type_uHWord auhRdSize, const Type_uByte* aubBuff_p)
{
	Type_uByte		aubRtnCode;
	aubRtnCode		= RES_NG;
	if (wubEEPModuleState == EEP_STATE_IDLE)
	{
		wubEEPModuleState = EEP_STATE_BUSY;

		aubRtnCode = wubBlockWriteCheck(auhStaAdr, auhRdSize,EEP_WRITABLE_ON);

		if(RES_OK == aubRtnCode)
		{
			aubRtnCode = wubLineToolWriteEEP(auhStaAdr,auhRdSize,aubBuff_p);
		}
		else
		{
			LOG_OUTPUT( LOG_APP_ID_EEP, DEBUGLOG_LEVEL_ERROR,ATAT_LEVEL_ON,"\n LineToolWrite range error");
		}
		wubEEPModuleState = EEP_STATE_IDLE;
	}

	return (aubRtnCode);
}

/*********************************************************************************************
*  function                 : wubEEP_SendMail
*  date                     : 2021.11.01
*  autho                    : zhangbo
*  description              : Function Call when EEP_SendMail
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
Type_uByte wubEEP_SendMail(const Type_uByte aubsrcId, const Type_uWord auhEventCode, void* avdMailBuffer_p, const Type_uWord aubMailSize)
{
	Type_uByte		aubReturnCode	= RES_NG;
	OS_ErrCode_t	aswErrCode		= OS_ERR_OK;
	OS_StdQueue_ts	astStdMail;
	OS_VoidPtr_t	avdMpf_p		= STD_NULL;
	
	/* Variable Initialize */
	wvdGen_MemSet(&astStdMail, EEP_NUM_0, sizeof(astStdMail));
	
	astStdMail.swSrcTaskId = aubsrcId;
	astStdMail.uhEventCode = auhEventCode;
	if((avdMailBuffer_p != STD_NULL) && (aubMailSize != EEP_NUM_0)) 
	{
		aswErrCode = OS_GetFMemoryBlock(&avdMpf_p, aubMailSize);

		/* Get Memory Block Result Check */
		if (OS_ERR_OK == aswErrCode) 
		{
			wvdGen_MemCpy(avdMpf_p, avdMailBuffer_p, aubMailSize);
			astStdMail.vdQueueBuffer_p = avdMpf_p;
			astStdMail.uwQueueSize = aubMailSize;
			aubReturnCode = RES_OK;
		} 
		else 
		{
			EEP_DBG("\n NO SPACE ");
		}
	} 
	else 
	{
		astStdMail.vdQueueBuffer_p = STD_NULL;
		astStdMail.uwQueueSize = EEP_NUM_0;
		aubReturnCode = RES_OK;
	}

	if (aubReturnCode == RES_OK) 
	{
		aswErrCode = OS_SendQueue(MBX_ID_EEP, OS_MAIL_PRI_NORMAL, &astStdMail);
		/* Send Mail Result Check */
		if (OS_ERR_OK == aswErrCode) 
		{
			/* No Action */
			;
		} 
		else 
		{
			/* Get Memory Block Check */
			if (avdMpf_p != STD_NULL) 
			{
				(void)OS_ReleaseFMemoryBlock(avdMpf_p);
			} 
			else 
			{
				/* No Action */
				;
			}
			aubReturnCode = RES_NG;
		}
	}
	return aubReturnCode;
}

/*********************************************************************************************
*  function                 : wubEEP_LineToolAllClear
*  date                     : 2021.11.01
*  autho                    : zhangbo
*  description              : Function Call when EEPAllClear
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
Type_uByte wubEEP_LineToolAllClear(void)
{
	Type_uByte    		aubRtn;               /* Return value */
	aubRtn = RES_NG;            /* Result NG Set */
	if (wubEEPModuleState == EEP_STATE_IDLE)
	{
		wubEEPModuleState = EEP_STATE_BUSY;
		aubRtn =wubEEPLINETOOLAPPAllClear();
		wubEEPModuleState = EEP_STATE_IDLE;
	}
	else
	{		/* Module Status Err */
		aubRtn = RES_BUSY;
	}
	return aubRtn;
}

/*********************************************************************************************
*  function                 : wubEEPDefaultSet
*  date                     : 2021.11.01
*  autho                    : zhangbo
*  description              : Function Call when EEPDefaultSet
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
Type_uByte wubEEP_LineToolDefaultSet(void)
{
	Type_uByte    	aubRtn;               /* Return value */
	aubRtn 	= RES_NG;            /* Result NG Set */
	if (wubEEPModuleState == EEP_STATE_IDLE)
	{
		wubEEPModuleState = EEP_STATE_BUSY;
		aubRtn =wubEEPDataDefaultSet();
		wubEEPModuleState = EEP_STATE_IDLE;
	}
	else
	{		/* Module Status Err */
		aubRtn = RES_BUSY;
	}
	return aubRtn;
}

/*********************************************************************************************
*  function                 : wubVariation_CarType_Get
*  date                     : 2022.02.18
*  autho                    : zhangbo
*  description              : Function CarType Get
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
/* Variation_CarType */

Type_uByte wubVariation_CarType_Get(void)
{
	Type_uByte aubVariationCarTypeResult = 0xff;
	(void)wubDirectReadRAM(EEP_ADDR_MODEL,1,&aubVariationCarTypeResult);
	if( aubVariationCarTypeResult == 0xff){
		LOG_OUTPUT( LOG_APP_ID_EEP, DEBUGLOG_LEVEL_ERROR,ATAT_LEVEL_OFF,"\n CarType Get Err");
		aubVariationCarTypeResult = 0x10;
	}
	return aubVariationCarTypeResult;
}
/*********************************************************************************************
*  function                 : wubVariation_Function_Get
*  date                     : 2022.02.18
*  autho                    : zhangbo
*  description              : Function CarType Get
*
*  <!-------------- Argument Code ---------------------------------------------------------->
*  param[in]aubFuntionIndex
*
*  <!-------------- Return Code ------------------------------------------------------------>
*  return
*
*********************************************************************************************
*  revision History
*  No.       date           revised by        description
*  
*********************************************************************************************/

Type_uByte wubVariation_Function_Get(Type_uByte aubFuntionIndex)
{
	Type_uByte aubFuntionResult  = EN_VARI_RES_ERROR;
	Type_uByte aubVariationCarType = EN_VARI_RES_ERROR;

	aubVariationCarType = wubVariation_CarType_Get();
	if((aubFuntionIndex > EN_VARI_FUNC_MIN)&&(aubFuntionIndex <= EN_VARI_FUNC_MAX))
	{
		switch (aubVariationCarType)
		{
			case CarType_Variation_0x10:
					aubFuntionResult = Variation_G11[aubFuntionIndex].ubCarType_Variation_0x10;
					break;
			case CarType_Variation_0x20:
					aubFuntionResult = Variation_G11[aubFuntionIndex].ubCarType_Variation_0x20;
					break;
			case CarType_Variation_0x30:
					aubFuntionResult = Variation_G11[aubFuntionIndex].ubCarType_Variation_0x30;
					break;
			case CarType_Variation_0x40:
					aubFuntionResult = Variation_G11[aubFuntionIndex].ubCarType_Variation_0x40;
					break;
			case CarType_Variation_0x50:
					aubFuntionResult = Variation_G11[aubFuntionIndex].ubCarType_Variation_0x50;
					break;
			case CarType_Variation_0x60:
					aubFuntionResult = Variation_G11[aubFuntionIndex].ubCarType_Variation_0x60;
					break;
			default:
				break;
		}
	}
	return aubFuntionResult;
}

/********************************************************************************************/
/**	\function		wvEepHalOpen
 *	\date			2022/03/16
 *	\author			xulei
 *	\description	Eep Hal Open
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
 ********************************************************************************************/
void wvEepHalOpen(void)
{
	nvbEEP_I2cHalHandle = HAL_Open(HAL_IIC_EEPROM, HAL_EEP_OPTION_NORMAL);

}
/********************************************************************************************/
/**	\function		wvdEepHalOpen
 *	\date			2022/03/16
 *	\author			xulei
 *	\description	Eep Hal Open
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
 ********************************************************************************************/
HAL_HANDLE wvdEepHalHandleGet(void)
{
	return nvbEEP_I2cHalHandle;
}

