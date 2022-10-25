/****************************************************************************
 *   FileName    : PM_PinDefine.h
 *   Description : Power Management
 ****************************************************************************/
#ifndef  _PM_PINDEFINE_H_
#define  _PM_PINDEFINE_H_

#include <gpio.h>
#include "temp_compile_switch.h"
#include "compile_switch.h"

#define GPIO_HIGH 1
#define GPIO_LOW  0

#if (TEMP_COMPILE_SWITCH_05WORK_10WORK == TEMP_COMPILE_SWITCH_05WORK)
/*GPIO_B group*/
#define PM_LCD_RST				TCC_GPB(5)	//A53
#define PM_LCD_DISP				TCC_GPB(6)	//A53
#define PM_LCD_PON				TCC_GPB(7)	//A53
#define PM_SATURN_RESET			TCC_GPB(8)
#define PM_SAT_33V_CONT			TCC_GPB(10)
#define PM_M5V_CONT             TCC_GPB(12)
#define PM_LVDS_18V_CONT        TCC_GPB(13)
#define PM_LVDS_33V_CONT        TCC_GPB(14)
#define PM_FAN_CONT             TCC_GPB(15)
#define PM_FAN_SPEED_CONT		TCC_GPB(16)
#define PM_PMIC_SCL             TCC_GPB(21)
#define PM_PMIC_SDA		        TCC_GPB(22)
/*GPIO_C group*/
#define PM_TP_RST				TCC_GPC(7)	//A53
#define PM_BL_SYNC				TCC_GPC(15)
#define PM_B_DET_ALARM          TCC_GPC(23)
/*GPIO_G group*/
#define PM_DSP_RST              TCC_GPG(6)
#define PM_LED_PWM              TCC_GPG(10)
/*GPIO_E group*/
#define PM_TPLCD_33V_CONT		TCC_GPE(0)
#define PM_ADC_I2C_SCL			TCC_GPE(14)
#define PM_ADC_I2C_SDA			TCC_GPE(15)
#define PM_M6V_FCONT			TCC_GPE(18)
/*GPIO_H group*/
#define PM_SNOR_D0              TCC_GPH(0)
#define PM_SNOR_D1              TCC_GPH(1)
#define PM_SNOR_D2              TCC_GPH(2)
#define PM_SNOR_D3              TCC_GPH(3)
#define PM_H06_NC	        	TCC_GPH(6)
#define PM_H07_NC	        	TCC_GPH(7)
#define PM_SNOR_CLK             TCC_GPH(8)
#define PM_SNOR_CS              TCC_GPH(9)
#define PM_H10_NC          		TCC_GPH(10)
#define PM_BT_33V_CONT 	       	TCC_GPH(11)
/*GPIO_K group*/
#define PM_HI_DET  				TCC_GPK(0)
#define PM_B_DET_BUFF	        TCC_GPK(1)
#define PM_GPIO_K02    	        TCC_GPK(2)
#define PM_A_DET           		TCC_GPK(3)
#define PM_GPIO_K04        		TCC_GPK(4)
#define PM_M_CAN_STB        	TCC_GPK(5)
#define PM_K06_NC     			TCC_GPK(6)
#define PM_GPIO_K07	        	TCC_GPK(7)
#define PM_GPIO_K08         	TCC_GPK(8)
#define PM_EEP_CONT		        TCC_GPK(9)
#define PM_AMP_STBY		        TCC_GPK(10)
#define PM_K11_NC               TCC_GPK(11)
#define PM_A_MUTE              	TCC_GPK(12)
#define PM_BCAN_TX              TCC_GPK(13)
#define PM_BCAN_RX              TCC_GPK(14)
#define PM_M_CAN_TX             TCC_GPK(15)
#define PM_M_CAN_RX		        TCC_GPK(16)
#define PM_RCV_CONT             TCC_GPK(17)
#define PM_LWC_CONT             TCC_GPK(18)
/*GPIO_MA group*/
#define PM_K_LINE_TX            TCC_GPMA(0)
#define PM_K_LINE_RX            TCC_GPMA(1)
#define PM_GNSSA_CONT           TCC_GPMA(2)
#define PM_D3P_CONT            	TCC_GPMA(3)
#define PM_A53_OK_INPUT         TCC_GPMA(4)
#define PM_BL_EN             	TCC_GPMA(5)
#define PM_LINE_TOOL_TXD        TCC_GPMA(6)
#define PM_LINE_TOOL_RXD        TCC_GPMA(7)
#define PM_PANT_CONT          	TCC_GPMA(8)
#define PM_BT_18V_CONT        	TCC_GPMA(9)
#define PM_BL_PWM      			TCC_GPMA(10)
#define PM_M6V_CONT             TCC_GPMA(11)
#define PM_BL_FAULT           	TCC_GPMA(12)
#define PM_ILLM_ON           	TCC_GPMA(13)
#define PM_DAB_18_12_PCONT      TCC_GPMA(14)
#define PM_VSP             		TCC_GPMA(20)
#define PM_EEP_I2C_SCL         	TCC_GPMA(22)
#define PM_EEP_I2C_SDA	        TCC_GPMA(23)
#define PM_R5_DBG_UT_TX         TCC_GPMA(24)
#define PM_R5_DBG_UT_RX         TCC_GPMA(25)
#define PM_RCV_RESET	        TCC_GPMA(28)
#define PM_VIDEO_DEC_12V_CONT   TCC_GPMA(29)
/*GPIO_AD group*/
#define PM_AD_OSC_TEMP			ADC_CHANNEL0
#define PM_AD_ADC_AIN01			ADC_CHANNEL1
#define PM_AD_ADC_AIN02			ADC_CHANNEL2
#define PM_AD_ADC_AIN03			ADC_CHANNEL3
#define PM_AD_ADC_AIN04			ADC_CHANNEL4
#define PM_AD_HFT_REMOTE		ADC_CHANNEL5
#define PM_AD_GNSSA_CS			ADC_CHANNEL6
#define PM_AD_PMIC_TEMP			ADC_CHANNEL7
#define PM_AD_LCD_TEMP			ADC_CHANNEL8
#define PM_AD_ADC_AIN09			ADC_CHANNEL9
#define PM_AD_ADC_AIN10			ADC_CHANNEL10
#define PM_AD_ADC_AIN11			ADC_CHANNEL11

#else
/* GPIO_A group:R5 */
#define PM_A1_NC				TCC_GPA(1)
#define PM_A2_NC				TCC_GPA(2)
#define PM_A11_NC				TCC_GPA(11)
#define PM_A21_NC				TCC_GPA(21)
#define PM_GNSS_CONT			TCC_GPA(22)
#define PM_A23_NC				TCC_GPA(23)
#define PM_A24_NC				TCC_GPA(24)
#define PM_A25_NC				TCC_GPA(25)

/* GPIO_A group:A7 */
#define PM_CIF1_CLKI			TCC_GPA(0)		//BT.656
#define PM_CIF1_PD0				TCC_GPA(3)		//BT.656
#define PM_CIF1_PD1				TCC_GPA(4)		//BT.656
#define PM_CIF1_PD2				TCC_GPA(5)		//BT.656
#define PM_CIF1_PD3				TCC_GPA(6)		//BT.656
#define PM_CIF1_PD4				TCC_GPA(7)		//BT.656
#define PM_CIF1_PD5				TCC_GPA(8)		//BT.656
#define PM_CIF1_PD6				TCC_GPA(9)		//BT.656
#define PM_CIF1_PD7				TCC_GPA(10)		//BT.656
#define PM_RCV_RESET			TCC_GPA(12)
#define PM_CAMERA_BIT0_CONT		TCC_GPA(13)
#define PM_CAMERA_BIT1_CONT		TCC_GPA(14)
#define PM_CAMERA_DET			TCC_GPA(15)
#define PM_LWC_SW_IN			TCC_GPA(16)
#define PM_GNSS_REVERSE			TCC_GPA(17)
#define PM_LWC_UART_TXD			TCC_GPA(18)		//UART
#define PM_LWC_UART_RXD			TCC_GPA(19)		//UART
#define PM_LWC_TURN				TCC_GPA(20)

/* GPIO_B group:R5 */
#define PM_SATUTN_SPI_CLK		TCC_GPB(0)
#define PM_SATUTN_SPI_CS		TCC_GPB(1)
#define PM_SATUTN_SPI_TX		TCC_GPB(2)
#define PM_SATUTN_SPI_RX		TCC_GPB(3)
#define PM_SATURN_RESET			TCC_GPB(8)
#define PM_DAB_DRM_BOOTSEL		TCC_GPB(9)
#define PM_SAT_33V_CONT			TCC_GPB(10)
#define PM_DAB_DRM_EN			TCC_GPB(11)
#define PM_K_LINE_TX            TCC_GPB(12)		//UART
#define PM_K_LINE_RX            TCC_GPB(13)		//UART
#define PM_ILLM_ON           	TCC_GPB(14)
#define PM_TEL_MUTE           	TCC_GPB(15)		//INT
#define PM_VTR_PARK           	TCC_GPB(16)
#define PM_PMIC_SCL             TCC_GPB(21)
#define PM_PMIC_SDA		        TCC_GPB(22)
#define PM_B26_NC		        TCC_GPB(26)
#define PM_B27_NC		        TCC_GPB(27)
#define PM_B28_NC		        TCC_GPB(28)
/* GPIO_B group:A53 */
#define PM_LCD_FAIL_DET			TCC_GPB(4)
#define PM_LCD_RST				TCC_GPB(5)
#define PM_LCD_DISP				TCC_GPB(6)
#define PM_LCD_PON				TCC_GPB(7)
#define PM_TP_RST				TCC_GPB(17)
#define PM_TP_CHG				TCC_GPB(18)		//INT
#define PM_IPOD_SCL				TCC_GPB(19)		//IIC
#define PM_IPOD_SDA				TCC_GPB(20)		//IIC
#define PM_TP_I2C_SCL			TCC_GPB(23)		//IIC
#define PM_TP_I2C_SDA			TCC_GPB(24)		//IIC
#define PM_SYS_EN				TCC_GPB(25)		//INT

/* GPIO_C group:R5 */
#define PM_C08_NC			   	TCC_GPC(8)
#define PM_C09_NC			   	TCC_GPC(9)
#define PM_VIDEO_DEC_12V_CONT   TCC_GPC(13)
#define PM_VIDEO_DEC_33V_CONT   TCC_GPC(14)
#define PM_LCD_33V_CONT			TCC_GPC(15)
/* GPIO_C group:A53 */
#define PM_USB1_CONT		   	TCC_GPC(0)
#define PM_USB1_FAULT		   	TCC_GPC(1)
#define PM_USB1_SYNC			TCC_GPC(2)
#define PM_A53_OK_OUTPUT	   	TCC_GPC(3)
#define PM_BC12_MODE		   	TCC_GPC(4)
#define PM_USB1_ATTACH			TCC_GPC(5)
#define PM_USB2_FAULT		   	TCC_GPC(6)
#define PM_USB2_CONT		   	TCC_GPC(7)
#define PM_A53_DBG_UT_TX	   	TCC_GPC(10)
#define PM_A53_DBG_UT_RX		TCC_GPC(11)
#define PM_USB2_SYNC			TCC_GPC(12)
#define PM_BT_UART_TXD		   	TCC_GPC(16)
#define PM_BT_UART_RXD		   	TCC_GPC(17)
#define PM_BT_UART_RTS			TCC_GPC(18)
#define PM_BT_UART_CTS			TCC_GPC(19)
#define PM_BL_SYNC				TCC_GPC(20)
#define PM_BL_PWM				TCC_GPC(21)		//TIMER
#define PM_BL_BL_FAULT			TCC_GPC(22)
#define PM_BL_EN				TCC_GPC(23)
#define PM_WL_EN				TCC_GPC(24)
#define PM_BT_EN				TCC_GPC(25)
#define PM_BT_I2S_BCLK			TCC_GPC(26)		//IIS
#define PM_BT_I2S_LRCK			TCC_GPC(27)		//IIS
#define PM_BT_I2S_DAO			TCC_GPC(28)		//IIS
#define PM_BT_I2S_DAI			TCC_GPC(29)		//IIS
/* GPIO_C group:A7 */
#define PM_RCV_I2C_SCL			TCC_GPC(8)		//IIC
#define PM_RCV_I2C_SDA		   	TCC_GPC(9)		//IIC

/* GPIO_G group:R5 */
#define PM_PANT_CONT          	TCC_GPG(10)
/* GPIO_G group:A53 */
#define PM_RTC_CLOCK            TCC_GPG(0)		//CLK_OUT
#define PM_DSP_I2S_BCK			TCC_GPG(1)		//IIS
#define PM_DSP_I2S_LRCK	        TCC_GPG(2)		//IIS
#define PM_DSP_I2S_DATA			TCC_GPG(3)		//IIS
#define PM_DSP_READY	        TCC_GPG(4)
#define PM_DSP_RST		        TCC_GPG(5)
#define PM_D3P_CONT            	TCC_GPG(6)
#define PM_CARPLAY_SUB_BCK      TCC_GPG(7)
#define PM_CARPLAY_SUB_LRCK     TCC_GPG(8)
#define PM_CARPLAY_SUB_DATA     TCC_GPG(9)

/* GPIO_E group:R5 */
#define PM_FAN_CONT             TCC_GPE(0)
#define PM_FAN_SPEED_CONT		TCC_GPE(4)
#define PM_AMP_STBY		        TCC_GPE(9)
#define PM_A_MUTE              	TCC_GPE(10)
#define PM_LINE_TOOL_TXD        TCC_GPE(11)		//UART
#define PM_LINE_TOOL_RXD        TCC_GPE(12)		//UART
#define PM_ADC_I2C_SCL			TCC_GPE(14)		//IIC
#define PM_ADC_I2C_SDA			TCC_GPE(15)		//IIC
#define PM_VSP             		TCC_GPE(18)		//TIMMER
/* GPIO_E group:A53 */
#define PM_ESIRI_DS_BCK         TCC_GPE(1)		//IIS
#define PM_ESIRI_DS_LRCK		TCC_GPE(2)		//IIS
#define PM_ESIRI_DS_DATA        TCC_GPE(3)		//IIS
#define PM_DSP_SPI_CLK        	TCC_GPE(5)		//SPI
#define PM_DSP_SPI_CS			TCC_GPE(6)		//SPI
#define PM_DSP_SPI_TX	        TCC_GPE(7)		//SPI
#define PM_DSP_SPI_RX	        TCC_GPE(8)		//SPI
#define PM_PIC_DIAG            	TCC_GPE(13)
#define PM_HFT_US_BCK	        TCC_GPE(16)		//IIS
#define PM_HFT_US_LRCK			TCC_GPE(17)		//IIS
#define PM_HFT_US_DATA			TCC_GPE(19)		//IIS

/* GPIO_H group:R5 */
#define PM_SNOR_D0              TCC_GPH(0)		//SPI
#define PM_SNOR_D1              TCC_GPH(1)		//SPI
#define PM_SNOR_D2              TCC_GPH(2)		//SPI
#define PM_SNOR_D3              TCC_GPH(3)		//SPI
#define PM_H06_NC	        	TCC_GPH(6)
#define PM_H07_NC	        	TCC_GPH(7)
#define PM_SNOR_CLK             TCC_GPH(8)		//SPI
#define PM_SNOR_CS              TCC_GPH(9)		//SPI
#define PM_H10_NC          		TCC_GPH(10)
#define PM_H11_NC 	       		TCC_GPH(11)
/* GPIO_H group:A7 */
#define PM_A7_DEBUG_TXD         TCC_GPH(4)		//UART
#define PM_A7_DEBUG_RXD         TCC_GPH(5)		//UART

/* GPIO_K group:R5 */
#define PM_HI_DET  				TCC_GPK(0)		//INT
#define PM_B_DET_BUFF	        TCC_GPK(1)		//INT
#define PM_A_DET           		TCC_GPK(2)		//INT
#define PM_GPIO_K03        		TCC_GPK(3)
#define PM_GPIO_K04        		TCC_GPK(4)
#define PM_GPIO_K05        		TCC_GPK(5)
#define PM_GPIO_K06        		TCC_GPK(6)
#define PM_GPIO_K07        		TCC_GPK(7)
#define PM_GPIO_K08        		TCC_GPK(8)
#define PM_GPIO_K09        		TCC_GPK(9)
#define PM_GPIO_K10        		TCC_GPK(10)
#define PM_GPIO_K11        		TCC_GPK(11)
#define PM_GPIO_K12        		TCC_GPK(12)
#define PM_GPIO_K13        		TCC_GPK(13)
#define PM_GPIO_K14        		TCC_GPK(14)
#define PM_GPIO_K15        		TCC_GPK(15)
#define PM_GPIO_K16        		TCC_GPK(16)
#define PM_GPIO_K17        		TCC_GPK(17)
#define PM_GPIO_K18        		TCC_GPK(18)

/* GPIO_MA group:R5 */
#define PM_RCV_CONT             TCC_GPMA(0)
#define PM_LWC_CONT             TCC_GPMA(1)
#define PM_M5V_CONT             TCC_GPMA(2)
#define PM_TP_33V_CONT			TCC_GPMA(3)
#define PM_A53_OK_INPUT         TCC_GPMA(4)
#if (COMPILE_SWITCH_DISP_SIZ == COMPILE_SWITCH_8INCH)
#define PM_POWER_KEY	        TCC_GPMA(5)
#define PM_VOL_UP_KEY	        TCC_GPMA(6)
#define PM_VOL_DOWN_KEY	        TCC_GPMA(7)
#define PM_DAY_NIGHT_KEY	    TCC_GPMA(8)
#define PM_BACK_KEY	    	    TCC_GPMA(9)
#define PM_HOME_KEY	    	    TCC_GPMA(10)
#else
#define PM_DAY_NIGHT_KEY        TCC_GPMA(5)
#define PM_BACK_KEY		        TCC_GPMA(6)
#define PM_HOME_KEY		        TCC_GPMA(7)
#define PM_VOL_UP_KEY    	    TCC_GPMA(8)
#define PM_POWER_KEY		    TCC_GPMA(9)
#define PM_VOL_DOWN_KEY    	    TCC_GPMA(10)
#endif
#define PM_LED_PWM	    	    TCC_GPMA(11)	//TIMER
#define PM_MA14_NC	        	TCC_GPMA(14)
#define PM_MA15_NC	        	TCC_GPMA(15)
#define PM_MA18_NC	        	TCC_GPMA(18)
#define PM_MA19_NC	        	TCC_GPMA(19)
#define PM_MA20_NC	        	TCC_GPMA(20)
#define PM_MA21_NC	        	TCC_GPMA(21)
#define PM_EEP_I2C_SCL         	TCC_GPMA(22)	//IIC
#define PM_EEP_I2C_SDA	        TCC_GPMA(23)	//IIC
#define PM_R5_DBG_UT_TX         TCC_GPMA(24)	//UART
#define PM_R5_DBG_UT_RX         TCC_GPMA(25)	//UART
#define PM_MA26_NC	        	TCC_GPMA(26)
#define PM_MA27_NC	        	TCC_GPMA(27)
#define PM_MA28_NC	        	TCC_GPMA(28)
#define PM_MA29_NC	        	TCC_GPMA(29)
/* GPIO_MA group:A53 */
#define PM_BT_33V_18V_CONT	    TCC_GPMA(12)
#define PM_BT_33V_CONT        	TCC_GPMA(13)

/* GPIO_AD group:R5 */
#define PM_AD_OSC_TEMP			ADC_CHANNEL0
#define PM_AD_ADC_AIN01			ADC_CHANNEL1
#define PM_AD_ADC_AIN02			ADC_CHANNEL2
#define PM_AD_ADC_AIN03			ADC_CHANNEL3
#define PM_AD_ADC_AIN04			ADC_CHANNEL4
#define PM_AD_HFT_REMOTE		ADC_CHANNEL5
#define PM_AD_AD7_NC			ADC_CHANNEL7
#define PM_AD_LCD_TEMP			ADC_CHANNEL8
#define PM_AD_ADC_AIN09			ADC_CHANNEL9
#define PM_AD_ADC_AIN10			ADC_CHANNEL10
#define PM_AD_ADC_AIN11			ADC_CHANNEL11

#endif


#if 0  //for TCC8031 poc start
/*GPIO_A group*/
#define PM_A01_NC              TCC_GPA(1)
#define PM_A02_NC              TCC_GPA(2)
#define PM_A11_NC              TCC_GPA(11)
#define PM_A12_NC              TCC_GPA(12)
#define PM_A13_NC              TCC_GPA(13)
#define PM_A14_NC              TCC_GPA(14)
#define PM_A15_NC              TCC_GPA(15)
#define PM_A22_NC              TCC_GPA(22)
#define PM_A25_NC              TCC_GPA(25)
#define PM_A28_NC              TCC_GPA(28)

/*GPIO_B group*/
#define PM_B12_NC              TCC_GPB(12)
#define PM_B15_NC              TCC_GPB(15)
#define PM_B16_NC              TCC_GPB(16)
#define PM_PMIC_SCL              TCC_GPB(21)
#define PM_PMIC_SDA              TCC_GPB(22)

/*GPIO_C group*/
#define PM_BL_SYNC			TCC_GPC(15)
#define PM_C23_NC              TCC_GPC(23)

/*GPIO_G group*/
#define PM_G00_NC              TCC_GPG(0)
#define PM_G03_NC              TCC_GPG(3)
#define PM_G06_NC              TCC_GPG(6)
#define PM_G10_NC              TCC_GPG(10)

/*GPIO_E group*/
#define PM_PAMP_EN			TCC_GPE(7)
#define PM_E08_NC			TCC_GPE(8)
#define PM_MAIN_I2C_SCL	TCC_GPE(9)
#define PM_MAIN_I2C_SDA	TCC_GPE(10)
#define PM_PAMP_DIAG		TCC_GPE(11)
#define PM_PAMP_MUTE		TCC_GPE(12)
#define PM_E18_NC			TCC_GPE(18)

/*GPIO_H group*/
#define PM_SNOR_D0              TCC_GPH(0)
#define PM_SNOR_D1              TCC_GPH(1)
#define PM_SNOR_D2              TCC_GPH(2)
#define PM_SNOR_D3              TCC_GPH(3)
#define PM_H06_NC	        	TCC_GPH(6)
#define PM_H07_NC	        	TCC_GPH(7)
#define PM_SNOR_CLK             TCC_GPH(8)
#define PM_SNOR_CS               TCC_GPH(9)
#define PM_H10_NC          		TCC_GPH(10)
#define PM_H11_NC 	       	TCC_GPH(11)

/*GPIO_K group*/
#define PM_K00_NC  			TCC_GPK(0)
#define PM_B_DET_BUFF	        TCC_GPK(1)
#define PM_K02_NC     	        TCC_GPK(2)
#define PM_A_DET            		TCC_GPK(3)
#define PM_PARK        		TCC_GPK(4)
#define PM_K05_NC          		TCC_GPK(5)
#define PM_K06_NC     		TCC_GPK(6)
#define PM_K07_NC  	        	TCC_GPK(7)
#define PM_BCAN_STBY         	TCC_GPK(8)
#define PM_LINE_TOOL_TXD        TCC_GPK(9)
#define PM_LINE_TOOL_RXD        TCC_GPK(10)
#define PM_K11_NC               TCC_GPK(11)
#define PM_FAN5V_CONT              	TCC_GPK(12)
#define PM_BCAN_TX              TCC_GPK(13)
#define PM_BCAN_RX              TCC_GPK(14)
#define PM_3R7V_PGOOD             TCC_GPK(15)
#define PM_A8R0V_PGOOD           TCC_GPK(16)
#define PM_A8R0V_CONT              TCC_GPK(17)
#define PM_ILL              		TCC_GPK(18)

/*GPIO_MA group*/
#define PM_MA00_NC            	TCC_GPMA(0)
#define PM_MA01_NC              	TCC_GPMA(1)
#define PM_GNSSA_CONT            TCC_GPMA(2)
#define PM_DG33V_CONT            TCC_GPMA(3)
#define PM_A53_OK_INPUT         TCC_GPMA(4)
#define PM_BL_EN             	TCC_GPMA(5)
#define PM_PANT_CONT          TCC_GPMA(6)
#define PM_MA07_NC              TCC_GPMA(7)
#define PM_MA08_NC          	TCC_GPMA(8)
#define PM_D12_18_SYNC        TCC_GPMA(9)
#define PM_BL_DIM      		TCC_GPMA(10)
#define PM_MA11_NC               TCC_GPMA(11)
#define PM_BL_FAULT           	TCC_GPMA(12)
#define PM_ILLM_ON           	TCC_GPMA(13)
#define PM_DAB_18_12_PCONT            TCC_GPMA(14)
#define PM_MA15_NC             	TCC_GPMA(15)
#define PM_VSP             			TCC_GPMA(20)
#define PM_R5_DBG_UT_TX            TCC_GPMA(24)
#define PM_R5_DBG_UT_RX            TCC_GPMA(25)
#endif  //for TCC8031  Poc end
/*
*********************************************************************************************************
*                                                 EXTERNS
*********************************************************************************************************
*/

#endif /* _PM_PINDEFINE_H_ */
