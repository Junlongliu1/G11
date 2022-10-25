/********************************************************************************************/
/* File Name	: pm_gpio.c																	*/
/* Date			: 2020/12/16																*/
/* Author		: plj																		*/
/* Model		: EDA																		*/
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
#include "os_framework.h"
#include "pm_pin_define.h"
#include <pwm.h>
#include "temp_compile_switch.h"


/********************************************************************************************/
/*							Macro Definition Section										*/
/********************************************************************************************/
#define PMGPIO_DEBUG
#ifdef PMGPIO_DEBUG
#define PMGPIO_DBG(fmt, args...)   printf(fmt, ## args)
#else
#define PMGPIO_DBG(fmt, args...)   do {} while(0)
#endif

#define PM_GPIO_DRIVER_STRENGTH_X1			GPIO_CD(0)
#define PM_GPIO_DRIVER_STRENGTH_X4			GPIO_CD(1)
#define PM_GPIO_DRIVER_STRENGTH_X2			GPIO_CD(2)
#define PM_GPIO_DRIVER_STRENGTH_X6			GPIO_CD(3)
#define PM_GPIO_DRIVER_STRENGTH_DEFAULT		0

#define PM_GPIO_LEVEL_DEFAULT				0xFF


/*********************************************************************************************
 * Variables
 ********************************************************************************************/
 
/********************************************************************************************/
/*							Structure/Union Type Definition Section 						*/
/********************************************************************************************/
typedef struct
{	
	Type_uWord	uwName;
	Type_uWord	uwFunc;
	Type_uWord	uwDirection;
	Type_uWord	uwDriverStrength;
	Type_uByte	ubInitLevel;
	Type_uByte	ubStbyLevel;
	Type_uByte	ubPull;
}PinDefine_st;

/********************************************************************************************/
/*							ROM Table Section												*/
/********************************************************************************************/
#if (TEMP_COMPILE_SWITCH_05WORK_10WORK == TEMP_COMPILE_SWITCH_05WORK)
static const PinDefine_st nstPinInit_tbl[] = 
{
/*uwName*/ 				/*uwFunc*/			/*uwDirection*/					/*uwDirection*/						/*ubInitLevel*/	/*ubStbyLevel*/
/*GPIO_B group*/
{PM_SATURN_RESET, 		GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_SAT_33V_CONT, 		GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_M5V_CONT, 			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_LVDS_18V_CONT, 		GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_LVDS_33V_CONT, 		GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_FAN_CONT, 			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_FAN_SPEED_CONT, 	GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_PMIC_SCL, 			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_PMIC_SDA, 			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
/*GPIO_C group*/
{PM_BL_SYNC, 			GPIO_FUNC(10),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_B_DET_ALARM, 		GPIO_FUNC(0),		GPIO_INPUT|GPIO_INPUTBUF_EN,	PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
/*GPIO_G group*/
{PM_DSP_RST, 			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_LED_PWM, 			GPIO_FUNC(10),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
/*GPIO_E group*/
{PM_TPLCD_33V_CONT, 	GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_ADC_I2C_SCL, 		GPIO_FUNC(8),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_ADC_I2C_SDA, 		GPIO_FUNC(8),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_M6V_FCONT, 			GPIO_FUNC(10),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
/*GPIO_H group*/
{PM_SNOR_D0, 			GPIO_FUNC(1),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_SNOR_D1, 			GPIO_FUNC(1),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_SNOR_D2, 			GPIO_FUNC(1),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_SNOR_D3, 			GPIO_FUNC(1),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_H06_NC, 			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_H07_NC, 			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_SNOR_CLK, 			GPIO_FUNC(1),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_SNOR_CS, 			GPIO_FUNC(1),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_H10_NC, 			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_BT_33V_CONT, 		GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
/*GPIO_K group*/
{PM_HI_DET, 			GPIO_FUNC(0),		GPIO_INPUT|GPIO_INPUTBUF_EN,	PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_B_DET_BUFF, 		GPIO_FUNC(0),		GPIO_INPUT|GPIO_INPUTBUF_EN,	PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_GPIO_K02, 			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_A_DET, 				GPIO_FUNC(0),		GPIO_INPUT|GPIO_INPUTBUF_EN,	PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_GPIO_K04, 			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_M_CAN_STB, 			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_K06_NC, 			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_GPIO_K07, 			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_GPIO_K08, 			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_EEP_CONT, 			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_AMP_STBY, 			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_K11_NC, 			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_A_MUTE, 			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_BCAN_TX, 			GPIO_FUNC(2),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_BCAN_RX, 			GPIO_FUNC(2),		GPIO_INPUT|GPIO_INPUTBUF_EN,	PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_M_CAN_TX, 			GPIO_FUNC(2),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_M_CAN_RX,			GPIO_FUNC(2),		GPIO_INPUT|GPIO_INPUTBUF_EN,	PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_RCV_CONT,			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_LWC_CONT,			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
/*GPIO_MA group*/
{PM_K_LINE_TX, 			GPIO_FUNC(7),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_K_LINE_RX, 			GPIO_FUNC(7),		GPIO_INPUT|GPIO_INPUTBUF_EN,	PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_GNSSA_CONT, 		GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_D3P_CONT, 			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_A53_OK_INPUT, 		GPIO_FUNC(0),		GPIO_INPUT|GPIO_INPUTBUF_EN,	PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_BL_EN, 				GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_LINE_TOOL_TXD, 		GPIO_FUNC(7),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_LINE_TOOL_RXD, 		GPIO_FUNC(7),		GPIO_INPUT|GPIO_INPUTBUF_EN,	PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_PANT_CONT, 			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_BT_18V_CONT, 		GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_BL_PWM, 			GPIO_FUNC(10),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_M6V_CONT, 			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_BL_FAULT, 			GPIO_FUNC(0),		GPIO_INPUT|GPIO_INPUTBUF_EN,	PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_ILLM_ON, 			GPIO_FUNC(0),		GPIO_INPUT|GPIO_INPUTBUF_EN,	PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_DAB_18_12_PCONT, 	GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_VSP, 				GPIO_FUNC(10),		GPIO_INPUT|GPIO_INPUTBUF_EN,	PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_R5_DBG_UT_TX,		GPIO_FUNC(7),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_R5_DBG_UT_RX,		GPIO_FUNC(7),		GPIO_INPUT|GPIO_INPUTBUF_EN,	PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_RCV_RESET,			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_VIDEO_DEC_12V_CONT,	GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
//A53
{PM_LCD_RST, 			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_TP_RST, 			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_LCD_DISP,			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
{PM_LCD_PON, 			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW},
};
#else
static const PinDefine_st nstPinInit_tbl[] = 
{
/*uwName*/ 				/*uwFunc*/			/*uwDirection*/					/*uwDirection*/						/*ubInitLevel*/				/*ubStbyLevel*/				/*ubPull*/
/*GPIO_A group*/
{PM_A1_NC,				GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,			GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_A2_NC,				GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_A11_NC,				GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_GNSS_CONT,			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_A23_NC,				GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_A24_NC, 			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_A25_NC, 			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
/*GPIO_B group*/
{PM_SATUTN_SPI_CLK,		GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_SATUTN_SPI_CS,		GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_SATUTN_SPI_TX, 		GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_SATUTN_SPI_RX, 		GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_SATURN_RESET,		GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_DAB_DRM_BOOTSEL, 	GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_SAT_33V_CONT, 		GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_DAB_DRM_EN, 		GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_K_LINE_TX, 			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X2,			PM_GPIO_LEVEL_DEFAULT,		PM_GPIO_LEVEL_DEFAULT,		GPIO_NOPULL},
{PM_K_LINE_RX, 			GPIO_FUNC(0),		GPIO_INPUT|GPIO_INPUTBUF_EN,	PM_GPIO_DRIVER_STRENGTH_DEFAULT,	PM_GPIO_LEVEL_DEFAULT,		PM_GPIO_LEVEL_DEFAULT,		GPIO_NOPULL},
{PM_ILLM_ON,			GPIO_FUNC(0),		GPIO_INPUT|GPIO_INPUTBUF_EN,	PM_GPIO_DRIVER_STRENGTH_DEFAULT,	PM_GPIO_LEVEL_DEFAULT,		PM_GPIO_LEVEL_DEFAULT,		GPIO_NOPULL},
{PM_TEL_MUTE, 			GPIO_FUNC(0),		GPIO_INPUT|GPIO_INPUTBUF_EN,	PM_GPIO_DRIVER_STRENGTH_DEFAULT,	PM_GPIO_LEVEL_DEFAULT,		PM_GPIO_LEVEL_DEFAULT,		GPIO_NOPULL},
{PM_VTR_PARK,			GPIO_FUNC(0),		GPIO_INPUT|GPIO_INPUTBUF_EN,	PM_GPIO_DRIVER_STRENGTH_DEFAULT,	PM_GPIO_LEVEL_DEFAULT,		PM_GPIO_LEVEL_DEFAULT,		GPIO_NOPULL},
{PM_PMIC_SCL,			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_PMIC_SDA,			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_B26_NC,				GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_B27_NC,				GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_B28_NC, 			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
/*GPIO_C group*/
{PM_C08_NC, 			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_C09_NC, 			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_VIDEO_DEC_12V_CONT, GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_VIDEO_DEC_33V_CONT,	GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_LCD_33V_CONT,		GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
/*GPIO_G group*/
{PM_PANT_CONT, 			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
/*GPIO_E group*/
{PM_FAN_CONT,			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_FAN_SPEED_CONT,		GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_AMP_STBY,			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_A_MUTE,				GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
//{PM_LINE_TOOL_TXD}
//{PM_LINE_TOOL_RXD}
//{PM_ADC_I2C_SCL}
//{PM_ADC_I2C_SDA}
//{VSP}
/*GPIO_H group*/
//{PM_SNOR_D0, 			GPIO_FUNC(1),		GPIO_INPUT|GPIO_INPUTBUF_EN,	PM_GPIO_DRIVER_STRENGTH_X2,			PM_GPIO_LEVEL_DEFAULT,		PM_GPIO_LEVEL_DEFAULT},
//{PM_SNOR_D1, 			GPIO_FUNC(1),		GPIO_INPUT|GPIO_INPUTBUF_EN,	PM_GPIO_DRIVER_STRENGTH_X2,			PM_GPIO_LEVEL_DEFAULT,		PM_GPIO_LEVEL_DEFAULT},
//{PM_SNOR_D2, 			GPIO_FUNC(1),		GPIO_INPUT|GPIO_INPUTBUF_EN,	PM_GPIO_DRIVER_STRENGTH_X2,			PM_GPIO_LEVEL_DEFAULT,		PM_GPIO_LEVEL_DEFAULT},
//{PM_SNOR_D3, 			GPIO_FUNC(1),		GPIO_INPUT|GPIO_INPUTBUF_EN,	PM_GPIO_DRIVER_STRENGTH_X2,			PM_GPIO_LEVEL_DEFAULT,		PM_GPIO_LEVEL_DEFAULT},
{PM_H06_NC,				GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_H07_NC, 			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_SNOR_CLK, 			GPIO_FUNC(1),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X4,			PM_GPIO_LEVEL_DEFAULT,		PM_GPIO_LEVEL_DEFAULT,		GPIO_NOPULL},
//{PM_SNOR_CS, 			GPIO_FUNC(1),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X4,			PM_GPIO_LEVEL_DEFAULT,		PM_GPIO_LEVEL_DEFAULT,		GPIO_NOPULL},
{PM_H10_NC, 			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_H11_NC, 			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
/*GPIO_K group*/
{PM_HI_DET, 			GPIO_FUNC(0),		GPIO_INPUT|GPIO_INPUTBUF_EN,	PM_GPIO_DRIVER_STRENGTH_DEFAULT,	PM_GPIO_LEVEL_DEFAULT,		PM_GPIO_LEVEL_DEFAULT,		GPIO_NOPULL},
{PM_B_DET_BUFF, 		GPIO_FUNC(0),		GPIO_INPUT|GPIO_INPUTBUF_EN,	PM_GPIO_DRIVER_STRENGTH_DEFAULT,	PM_GPIO_LEVEL_DEFAULT,		PM_GPIO_LEVEL_DEFAULT,		GPIO_NOPULL},
{PM_A_DET,				GPIO_FUNC(0),		GPIO_INPUT|GPIO_INPUTBUF_EN,	PM_GPIO_DRIVER_STRENGTH_DEFAULT,	PM_GPIO_LEVEL_DEFAULT,		PM_GPIO_LEVEL_DEFAULT,		GPIO_NOPULL},
{PM_GPIO_K03, 			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_GPIO_K04, 			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_GPIO_K05, 			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_GPIO_K06, 			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_GPIO_K07,			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_GPIO_K08,			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_GPIO_K09,			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_GPIO_K10,			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_GPIO_K11,			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_GPIO_K12,			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_GPIO_K13,			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_GPIO_K14,			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_GPIO_K15,			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_GPIO_K16,			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_GPIO_K17,			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_GPIO_K18,			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
/*GPIO_MA group*/
{PM_RCV_CONT,			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X2,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_LWC_CONT,			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X2,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_M5V_CONT,			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X2,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_TP_33V_CONT,		GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X2,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_A53_OK_INPUT,		GPIO_FUNC(0),		GPIO_INPUT|GPIO_INPUTBUF_EN,	PM_GPIO_DRIVER_STRENGTH_DEFAULT,	PM_GPIO_LEVEL_DEFAULT,		PM_GPIO_LEVEL_DEFAULT,		GPIO_NOPULL},
{PM_POWER_KEY,			GPIO_FUNC(0),		GPIO_INPUT|GPIO_INPUTBUF_EN,	PM_GPIO_DRIVER_STRENGTH_DEFAULT,	PM_GPIO_LEVEL_DEFAULT,		PM_GPIO_LEVEL_DEFAULT,		GPIO_NOPULL},
{PM_VOL_UP_KEY,			GPIO_FUNC(0),		GPIO_INPUT|GPIO_INPUTBUF_EN,	PM_GPIO_DRIVER_STRENGTH_DEFAULT,	PM_GPIO_LEVEL_DEFAULT,		PM_GPIO_LEVEL_DEFAULT,		GPIO_NOPULL},
{PM_VOL_DOWN_KEY,		GPIO_FUNC(0),		GPIO_INPUT|GPIO_INPUTBUF_EN,	PM_GPIO_DRIVER_STRENGTH_DEFAULT,	PM_GPIO_LEVEL_DEFAULT,		PM_GPIO_LEVEL_DEFAULT,		GPIO_NOPULL},
{PM_BACK_KEY,			GPIO_FUNC(0),		GPIO_INPUT|GPIO_INPUTBUF_EN,	PM_GPIO_DRIVER_STRENGTH_DEFAULT,	PM_GPIO_LEVEL_DEFAULT,		PM_GPIO_LEVEL_DEFAULT,		GPIO_NOPULL},
{PM_DAY_NIGHT_KEY, 		GPIO_FUNC(0),		GPIO_INPUT|GPIO_INPUTBUF_EN,	PM_GPIO_DRIVER_STRENGTH_DEFAULT,	PM_GPIO_LEVEL_DEFAULT,		PM_GPIO_LEVEL_DEFAULT,		GPIO_NOPULL},
{PM_HOME_KEY,			GPIO_FUNC(0),		GPIO_INPUT|GPIO_INPUTBUF_EN,	PM_GPIO_DRIVER_STRENGTH_DEFAULT,	PM_GPIO_LEVEL_DEFAULT,		PM_GPIO_LEVEL_DEFAULT,		GPIO_NOPULL},
{PM_LED_PWM,			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X2,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_MA14_NC,			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_MA15_NC,			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_MA18_NC,			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_MA19_NC,			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_MA20_NC,			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_MA21_NC,			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
//{PM_EEP_I2C_SCL}
//{PM_EEP_I2C_SDA}
//{PM_R5_DBG_UT_TX}
//{PM_R5_DBG_UT_RX}
{PM_MA26_NC,			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_MA27_NC,			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_MA28_NC,			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
{PM_MA29_NC,			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X1,	GPIO_LOW,					GPIO_LOW,					GPIO_NOPULL},
};

static const PinDefine_st nstPinInit_tbl_A53[] = 
{
/*uwName*/ 				/*uwFunc*/			/*uwDirection*/					/*uwDirection*/						/*ubInitLevel*/	/*ubStbyLevel*/				/*ubPull*/
/*GPIO_B group*/
{PM_LCD_RST,			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW,					GPIO_NOPULL},
{PM_LCD_DISP,			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW,					GPIO_NOPULL},
{PM_LCD_PON,			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW,					GPIO_NOPULL},
{PM_TP_RST,				GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW,					GPIO_NOPULL},
/*GPIO_C group*/
{PM_USB1_CONT,			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW,					GPIO_NOPULL},
{PM_USB1_SYNC,			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW,					GPIO_NOPULL},
{PM_USB1_ATTACH,		GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW,					GPIO_NOPULL},
{PM_A53_OK_OUTPUT,		GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW,					GPIO_NOPULL},
{PM_BC12_MODE, 			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW,					GPIO_NOPULL},
{PM_USB1_ATTACH,		GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_HIGH,		GPIO_LOW,					GPIO_NOPULL},
{PM_USB2_CONT,			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW,					GPIO_NOPULL},
{PM_USB2_SYNC,			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW,					GPIO_NOPULL},
{PM_BL_SYNC,			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW,					GPIO_NOPULL},
{PM_BL_PWM,				GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW,					GPIO_NOPULL},
{PM_BL_EN,				GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW,					GPIO_NOPULL},
{PM_WL_EN,				GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW,					GPIO_NOPULL},
{PM_BT_EN,				GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW,					GPIO_NOPULL},
/*GPIO_G group*/
{PM_RTC_CLOCK,			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW,					GPIO_NOPULL},
{PM_DSP_RST,			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW,					GPIO_NOPULL},
{PM_D3P_CONT, 			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW,					GPIO_NOPULL},
/*GPIO_MA group*/
{PM_BT_33V_18V_CONT,	GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW,					GPIO_NOPULL},
{PM_BT_33V_CONT,		GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW,					GPIO_NOPULL},
};

static const PinDefine_st nstPinInit_tbl_A7[] = 
{
/*uwName*/ 				/*uwFunc*/			/*uwDirection*/					/*uwDirection*/						/*ubInitLevel*/	/*ubStbyLevel*/
/*GPIO_A group*/
{PM_RCV_RESET,			GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW,					GPIO_NOPULL},
{PM_CAMERA_BIT0_CONT,	GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW,					GPIO_NOPULL},
{PM_CAMERA_BIT1_CONT,	GPIO_FUNC(0),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	GPIO_LOW,		GPIO_LOW,					GPIO_NOPULL},
};

#ifdef PM_ST_EN
static const PinDefine_st nstPinInitFunc_tbl_R5[] =
{
/*uwName*/ 				/*uwFunc*/			/*uwDirection*/					/*uwDirection*/						/*ubInitLevel*/				/*ubStbyLevel*/
/*GPIO_E group*/
{PM_LINE_TOOL_TXD,		GPIO_FUNC(7),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X2,			PM_GPIO_LEVEL_DEFAULT,		PM_GPIO_LEVEL_DEFAULT,				GPIO_NOPULL},
{PM_LINE_TOOL_RXD,		GPIO_FUNC(7),		GPIO_INPUT|GPIO_INPUTBUF_EN,	PM_GPIO_DRIVER_STRENGTH_DEFAULT,	PM_GPIO_LEVEL_DEFAULT,		PM_GPIO_LEVEL_DEFAULT,				GPIO_NOPULL},
{PM_ADC_I2C_SCL,		GPIO_FUNC(8),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_DEFAULT,	PM_GPIO_LEVEL_DEFAULT,		PM_GPIO_LEVEL_DEFAULT,				GPIO_NOPULL},
{PM_ADC_I2C_SDA,		GPIO_FUNC(8),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X2,			PM_GPIO_LEVEL_DEFAULT,		PM_GPIO_LEVEL_DEFAULT,				GPIO_NOPULL},
{PM_VSP,				GPIO_FUNC(10),		GPIO_INPUT|GPIO_INPUTBUF_EN,	PM_GPIO_DRIVER_STRENGTH_X2,			PM_GPIO_LEVEL_DEFAULT,		PM_GPIO_LEVEL_DEFAULT,				GPIO_NOPULL},
/*GPIO_H group*/
/*GPIO_H group*/
{PM_SNOR_D0,			GPIO_FUNC(1),		GPIO_INPUT|GPIO_INPUTBUF_EN,	PM_GPIO_DRIVER_STRENGTH_X2, 		PM_GPIO_LEVEL_DEFAULT,		PM_GPIO_LEVEL_DEFAULT,				GPIO_NOPULL},
{PM_SNOR_D1,			GPIO_FUNC(1),		GPIO_INPUT|GPIO_INPUTBUF_EN,	PM_GPIO_DRIVER_STRENGTH_X2, 		PM_GPIO_LEVEL_DEFAULT,		PM_GPIO_LEVEL_DEFAULT,				GPIO_NOPULL},
{PM_SNOR_D2,			GPIO_FUNC(1),		GPIO_INPUT|GPIO_INPUTBUF_EN,	PM_GPIO_DRIVER_STRENGTH_X2, 		PM_GPIO_LEVEL_DEFAULT,		PM_GPIO_LEVEL_DEFAULT,				GPIO_NOPULL},
{PM_SNOR_D3,			GPIO_FUNC(1),		GPIO_INPUT|GPIO_INPUTBUF_EN,	PM_GPIO_DRIVER_STRENGTH_X2, 		PM_GPIO_LEVEL_DEFAULT,		PM_GPIO_LEVEL_DEFAULT,				GPIO_NOPULL},
{PM_SNOR_CS,			GPIO_FUNC(1),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X2, 		PM_GPIO_LEVEL_DEFAULT,		PM_GPIO_LEVEL_DEFAULT,				GPIO_NOPULL},
/*GPIO_MA group*/
{PM_EEP_I2C_SCL,		GPIO_FUNC(8),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X2,			PM_GPIO_LEVEL_DEFAULT,		PM_GPIO_LEVEL_DEFAULT,				GPIO_NOPULL},
{PM_EEP_I2C_SDA,		GPIO_FUNC(8),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X2,			PM_GPIO_LEVEL_DEFAULT,		PM_GPIO_LEVEL_DEFAULT,				GPIO_NOPULL},
{PM_R5_DBG_UT_TX,		GPIO_FUNC(7),		GPIO_OUTPUT,					PM_GPIO_DRIVER_STRENGTH_X2, 		PM_GPIO_LEVEL_DEFAULT,		PM_GPIO_LEVEL_DEFAULT,				GPIO_NOPULL},
{PM_R5_DBG_UT_RX,		GPIO_FUNC(7),		GPIO_INPUT|GPIO_INPUTBUF_EN,	PM_GPIO_DRIVER_STRENGTH_DEFAULT,	PM_GPIO_LEVEL_DEFAULT,		PM_GPIO_LEVEL_DEFAULT,				GPIO_NOPULL},
};
#endif


#endif

#if 0  //for TCC8031 poc start
static const PinDefine_st nstPinInit_tbl[] = 
{
/*uwName*/ 			/*uwFunc*/			/*uwDirection*/		/*ubInitLevel*/	/*ubStbyLevel*/
/*GPIO_A group*/
{PM_A01_NC,			GPIO_FUNC(0),		GPIO_OUTPUT, 		GPIO_LOW,		GPIO_LOW},
{PM_A02_NC, 			GPIO_FUNC(0),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_A11_NC, 			GPIO_FUNC(0),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_A12_NC, 			GPIO_FUNC(0),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_A13_NC, 			GPIO_FUNC(0),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_A14_NC, 			GPIO_FUNC(0),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_A15_NC, 			GPIO_FUNC(0),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_A22_NC, 			GPIO_FUNC(0),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_A25_NC, 			GPIO_FUNC(0),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_A28_NC, 			GPIO_FUNC(0),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
/*GPIO_B group*/
{PM_B12_NC, 			GPIO_FUNC(0),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_B15_NC, 			GPIO_FUNC(0),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_B16_NC, 			GPIO_FUNC(0),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
//{PM_PMIC_SCL, 		GPIO_FUNC(8),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
//{PM_PMIC_SDA, 		GPIO_FUNC(8),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
/*GPIO_C group*/
{PM_BL_SYNC, 		GPIO_FUNC(10),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_C23_NC, 			GPIO_FUNC(0),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
/*GPIO_G group*/
{PM_G00_NC, 			GPIO_FUNC(0),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_G03_NC, 			GPIO_FUNC(0),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_G06_NC, 			GPIO_FUNC(0),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_G10_NC, 			GPIO_FUNC(0),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
/*GPIO_E group*/
{PM_PAMP_EN, 		GPIO_FUNC(0),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_E08_NC, 			GPIO_FUNC(0),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
//{PM_MAIN_I2C_SCL, 	GPIO_FUNC(8),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
//{PM_MAIN_I2C_SDA, 	GPIO_FUNC(8),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_PAMP_DIAG, 		GPIO_FUNC(0),		GPIO_INPUT|GPIO_INPUTBUF_EN,			GPIO_LOW,		GPIO_LOW},
{PM_PAMP_MUTE, 		GPIO_FUNC(0),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_E18_NC, 			GPIO_FUNC(0),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
/*GPIO_H group*/
{PM_SNOR_D0, 		GPIO_FUNC(1),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_SNOR_D1, 		GPIO_FUNC(1),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_SNOR_D2, 		GPIO_FUNC(1),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_SNOR_D3, 		GPIO_FUNC(1),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_H06_NC, 			GPIO_FUNC(0),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_H07_NC, 			GPIO_FUNC(0),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_SNOR_CLK, 		GPIO_FUNC(1),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_SNOR_CS, 		GPIO_FUNC(1),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_H10_NC, 			GPIO_FUNC(0),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_H11_NC, 			GPIO_FUNC(0),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
/*GPIO_K group*/
{PM_K00_NC, 			GPIO_FUNC(0),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_B_DET_BUFF, 		GPIO_FUNC(0),		GPIO_INPUT|GPIO_INPUTBUF_EN,			GPIO_LOW,		GPIO_LOW},
{PM_K02_NC, 			GPIO_FUNC(0),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_A_DET, 			GPIO_FUNC(0),		GPIO_INPUT|GPIO_INPUTBUF_EN,			GPIO_LOW,		GPIO_LOW},
{PM_PARK, 			GPIO_FUNC(0),		GPIO_INPUT|GPIO_INPUTBUF_EN,			GPIO_LOW,		GPIO_LOW},
{PM_K05_NC, 			GPIO_FUNC(0),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_K06_NC, 			GPIO_FUNC(0),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_K07_NC, 			GPIO_FUNC(0),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_BCAN_STBY, 		GPIO_FUNC(0),		GPIO_OUTPUT,		GPIO_HIGH,		GPIO_LOW},
{PM_LINE_TOOL_TXD, 	GPIO_FUNC(2),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_LINE_TOOL_RXD, 	GPIO_FUNC(2),		GPIO_INPUT|GPIO_INPUTBUF_EN,			GPIO_LOW,		GPIO_LOW},
{PM_K11_NC, 			GPIO_FUNC(0),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_FAN5V_CONT, 		GPIO_FUNC(0),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_BCAN_TX, 		GPIO_FUNC(2),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_BCAN_RX, 		GPIO_FUNC(2),		GPIO_INPUT|GPIO_INPUTBUF_EN,			GPIO_LOW,		GPIO_LOW},
{PM_3R7V_PGOOD, 	GPIO_FUNC(0),		GPIO_INPUT|GPIO_INPUTBUF_EN,			GPIO_LOW,		GPIO_LOW},
{PM_A8R0V_PGOOD, 	GPIO_FUNC(0),		GPIO_INPUT|GPIO_INPUTBUF_EN,			GPIO_LOW,		GPIO_LOW},
{PM_A8R0V_CONT, 		GPIO_FUNC(0),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_ILL, 				GPIO_FUNC(0),		GPIO_INPUT|GPIO_INPUTBUF_EN,			GPIO_LOW,		GPIO_LOW},
/*GPIO_MA group*/
{PM_MA00_NC, 		GPIO_FUNC(0),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_MA01_NC, 		GPIO_FUNC(0),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_GNSSA_CONT, 	GPIO_FUNC(0),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_DG33V_CONT, 	GPIO_FUNC(0),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_A53_OK_INPUT, 	GPIO_FUNC(10),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_BL_EN, 			GPIO_FUNC(0),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_PANT_CONT, 		GPIO_FUNC(0),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_MA07_NC, 		GPIO_FUNC(0),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_MA08_NC, 		GPIO_FUNC(0),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_D12_18_SYNC, 	GPIO_FUNC(0),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_BL_DIM, 			GPIO_FUNC(10),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_MA11_NC, 		GPIO_FUNC(0),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_BL_FAULT, 		GPIO_FUNC(0),		GPIO_INPUT|GPIO_INPUTBUF_EN,			GPIO_LOW,		GPIO_LOW},
{PM_ILLM_ON, 		GPIO_FUNC(0),		GPIO_INPUT|GPIO_INPUTBUF_EN,			GPIO_LOW,		GPIO_LOW},
{PM_DAB_18_12_PCONT, GPIO_FUNC(0),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_MA15_NC, 		GPIO_FUNC(0),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
{PM_VSP,				GPIO_FUNC(0),		GPIO_INPUT|GPIO_INPUTBUF_EN,		GPIO_LOW,		GPIO_LOW},
//{PM_R5_DBG_UT_TX,	GPIO_FUNC(7),		GPIO_OUTPUT,		GPIO_LOW,		GPIO_LOW},
//{PM_R5_DBG_UT_RX,	GPIO_FUNC(7),		GPIO_INPU|GPIO_INPUTBUF_ENT,		GPIO_LOW,		GPIO_LOW},
};
#endif  //for TCC8031  Poc end

/********************************************************************************************/
/*							Function Definition Section										*/
/********************************************************************************************/

void wvdPmGpioConfig(void)
{
	Type_uByte aubIndex;
	Type_sWord aswRet;
	
	for( aubIndex = 0 ; aubIndex < sizeof(nstPinInit_tbl)/sizeof(PinDefine_st) ; aubIndex++ )
	{
		aswRet = gpio_config(nstPinInit_tbl[aubIndex].uwName,nstPinInit_tbl[aubIndex].uwFunc |nstPinInit_tbl[aubIndex].uwDriverStrength |nstPinInit_tbl[aubIndex].uwDirection |nstPinInit_tbl[aubIndex].ubPull);		
		if(aswRet != 0){
			PMGPIO_DBG("######## PMGPIO Failed to config gpio:%d-%d", nstPinInit_tbl[aubIndex].uwName, aswRet);
		}
		if((GPIO_OUTPUT == nstPinInit_tbl[aubIndex].uwDirection) && (PM_GPIO_LEVEL_DEFAULT != nstPinInit_tbl[aubIndex].ubInitLevel))
		{
			aswRet = gpio_set(nstPinInit_tbl[aubIndex].uwName,nstPinInit_tbl[aubIndex].ubInitLevel);	
			if(aswRet != 0){
				PMGPIO_DBG("######## PMGPIO Failed to init gpio:%d-%d", nstPinInit_tbl[aubIndex].uwName, nstPinInit_tbl[aubIndex].ubInitLevel);
			}
		}
	}
	
}



void wvdPmGpioClear(void)
{
	Type_uByte aubIndex;
	Type_sWord aswRet;

	for( aubIndex = 0 ; aubIndex < sizeof(nstPinInit_tbl)/sizeof(PinDefine_st) ; aubIndex++ )
	{
		aswRet = gpio_config(nstPinInit_tbl[aubIndex].uwName,nstPinInit_tbl[aubIndex].uwFunc |nstPinInit_tbl[aubIndex].uwDriverStrength |nstPinInit_tbl[aubIndex].uwDirection);		
		if(aswRet != 0){
			PMGPIO_DBG("######## PMGPIO Failed to config gpio:%d-%d", nstPinInit_tbl[aubIndex].uwName, aswRet);
		}

		if((GPIO_OUTPUT == nstPinInit_tbl[aubIndex].uwDirection) && (PM_GPIO_LEVEL_DEFAULT != nstPinInit_tbl[aubIndex].ubStbyLevel))
		{
			aswRet = gpio_set(nstPinInit_tbl[aubIndex].uwName,nstPinInit_tbl[aubIndex].ubStbyLevel);	
			if(aswRet != 0){
				PMGPIO_DBG("######## PMGPIO Failed to clear gpio:%d-%d", nstPinInit_tbl[aubIndex].uwName, nstPinInit_tbl[aubIndex].ubStbyLevel);
			}
		}
	}
#if (TEMP_COMPILE_SWITCH_05WORK_10WORK != TEMP_COMPILE_SWITCH_05WORK)
	for( aubIndex = 0 ; aubIndex < sizeof(nstPinInit_tbl_A53)/sizeof(PinDefine_st) ; aubIndex++ )
	{
		aswRet = gpio_config(nstPinInit_tbl_A53[aubIndex].uwName,nstPinInit_tbl_A53[aubIndex].uwFunc |nstPinInit_tbl_A53[aubIndex].uwDriverStrength |nstPinInit_tbl_A53[aubIndex].uwDirection);		
		if(aswRet != 0){
			PMGPIO_DBG("######## PMGPIO Failed to config A53 gpio:%d-%d", nstPinInit_tbl[aubIndex].uwName, aswRet);
		}

		if((GPIO_OUTPUT == nstPinInit_tbl_A53[aubIndex].uwDirection) && (PM_GPIO_LEVEL_DEFAULT != nstPinInit_tbl_A53[aubIndex].ubStbyLevel))
		{
			aswRet = gpio_set(nstPinInit_tbl_A53[aubIndex].uwName,nstPinInit_tbl_A53[aubIndex].ubStbyLevel);	
			if(aswRet != 0){
				PMGPIO_DBG("######## PMGPIO Failed to clear A53 gpio:%d-%d", nstPinInit_tbl[aubIndex].uwName, nstPinInit_tbl[aubIndex].ubStbyLevel);
			}
		}
	}

	for( aubIndex = 0 ; aubIndex < sizeof(nstPinInit_tbl_A7)/sizeof(PinDefine_st) ; aubIndex++ )
	{
		aswRet = gpio_config(nstPinInit_tbl_A7[aubIndex].uwName,nstPinInit_tbl_A7[aubIndex].uwFunc |nstPinInit_tbl_A7[aubIndex].uwDriverStrength |nstPinInit_tbl_A7[aubIndex].uwDirection);		
		if(aswRet != 0){
			PMGPIO_DBG("######## PMGPIO Failed to config A7 gpio:%d-%d", nstPinInit_tbl[aubIndex].uwName, aswRet);
		}

		if((GPIO_OUTPUT == nstPinInit_tbl_A7[aubIndex].uwDirection) && (PM_GPIO_LEVEL_DEFAULT != nstPinInit_tbl_A7[aubIndex].ubStbyLevel))
		{
			aswRet = gpio_set(nstPinInit_tbl_A7[aubIndex].uwName,nstPinInit_tbl_A7[aubIndex].ubStbyLevel);	
			if(aswRet != 0){
				PMGPIO_DBG("######## PMGPIO Failed to clear A7 gpio:%d-%d", nstPinInit_tbl[aubIndex].uwName, nstPinInit_tbl[aubIndex].ubStbyLevel);
			}
		}
	}
#endif
}

/* End of File */

