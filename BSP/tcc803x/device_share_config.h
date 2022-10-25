/****************************************************************************
 *   FileName    : device_share_config.h
 *   Description : Configuration information of device sharing with MAIN AP
 ****************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips Inc.
 *   All rights reserved 
 
This source code contains confidential information of Telechips.
Any unauthorized use without a written permission of Telechips including not limited to re-
distribution in source or binary form is strictly prohibited.
This source code is provided "AS IS" and nothing contained in this source code shall 
constitute any express or implied warranty of any kind, including without limitation, any warranty 
of merchantability, fitness for a particular purpose or non-infringement of any patent, copyright 
or other third party intellectual property right. No warranty is made, express or implied, 
regarding the information's accuracy, completeness, or performance. 
In no event shall Telechips be liable for any claim, damages or other liability arising from, out of 
or in connection with this source code or the use in the source code. 
This source code is provided subject to the terms of a Mutual Non-Disclosure Agreement 
between Telechips and Company.
*
****************************************************************************/
#ifndef DEVICE_SHARE_CONFIG_H
#define DEVICE_SHARE_CONFIG_H

#define SHARE_CONFIGURE_MAJOR_VER	1
#define SHARE_CONFIGURE_MINOR_VER	0
#define SHARE_CONFIGURE_PATCH_VER	0

typedef struct
{
	// Configuration Version
	uint8 major_version;
	uint8 minor_version;
	uint8 patch_version;
	uint8 version_reserve;
}DEVICE_SHARE_VER;


typedef enum
{
	SHARE_DEVICE_ADC = 0,
	SHARE_DEVICE_PWM_OUT,
	SHARE_DEVICE_PWM_IN,	
	SHARE_DEVICE_I2C,
	SHARE_DEVICE_UART,
	SHARE_DEVICE_UDMA,
	SHARE_DEVICE_SPI,
	SHARE_DEVICE_TIMER,
	SHARE_DEVICE_CB_WDT,
	SHARE_DEVICE_PLL,
	SHARE_DEVICE_MAX
} SHARE_DEVICE;

// ADC
typedef struct {
	uint32  ADC00 : 1; //ADC0
	uint32  ADC01 : 1; //ADC1
	uint32        :30;
} ADC_CONF;

typedef union {
	uint32   nADCCONF;
	ADC_CONF bADCCONF;
} ADC_CONF_U;

// PWM
typedef struct {
	uint32  PWM00 : 1; //PWM0
	uint32  PWM01 : 1; //PWM1
	uint32  PWM02 : 1; //PWM2
	uint32  PWM03 : 1; //PWM3
	uint32        reserv0:12;
	uint32  ICTC00 : 1; //ICTC0
	uint32  ICTC01 : 1; //ICTC1
	uint32  ICTC02 : 1; //ICTC2
	uint32        reserv1:13;
} PWM_CONF;

typedef union {
	uint32   nPWMCONF;
	PWM_CONF bPWMCONF;
}PWM_CONF_U;

// I2C
typedef struct {
	uint32  I2C00 : 1; //I2C0
	uint32  I2C01 : 1; //I2C1
	uint32  I2C02 : 1; //I2C2
	uint32  I2C03 : 1; //I2C3
	uint32  I2C04 : 1; //I2C4
	uint32  I2C05 : 1; //I2C5
	uint32  I2C06 : 1; //I2C6
	uint32  I2C07 : 1; //I2C7
	uint32        :24;
} I2C_CONF;

typedef union {
	uint32   nI2CCONF;
	I2C_CONF bI2CCONF;
}I2C_CONF_U;

// UART
typedef struct {
	uint32  UART00 : 1; //UART0
	uint32  UART01 : 1; //UART1
	uint32  UART02 : 1; //UART2
	uint32  UART03 : 1; //UART3
	uint32  UART04 : 1; //UART4
	uint32  UART05 : 1; //UART5
	uint32  UART06 : 1; //UART6
	uint32  UART07 : 1; //UART7
	uint32	      :8;
	uint32  UDMA00 : 1; // UART DMA0
	uint32  UDMA01 : 1; // UART DMA1
	uint32  UDMA02 : 1; // UART DMA2
	uint32  UDMA03 : 1; // UART DMA3
	uint32        :12;
}UART_CONF;

typedef union {
	uint32   nUARTCONF;
	UART_CONF bUARTCONF;
}UART_CONF_U;

// SPI
typedef struct {
	uint32  SPI00 : 1; //SPI0
	uint32  SPI01 : 1; //SPI1
	uint32  SPI02 : 1; //SPI2
	uint32  SPI03 : 1; //SPI3
	uint32  SPI04 : 1; //SPI4
	uint32  SPI05 : 1; //SPI5
	uint32        :26;
}SPI_CONF;

typedef union {
	uint32   nSPICONF;
	SPI_CONF bSPICONF;
}SPI_CONF_U;

// TIMER
typedef struct {
	uint32 TIMER00 : 1; //TIMER0 16"
	uint32 TIMER01 : 1; //TIMER1 16"
	uint32 TIMER02 : 1; //TIMER2 16"
	uint32 TIMER03 : 1; //TIMER3 16"
	uint32 TIMER04 : 1; //TIMER4 20"
	uint32 TIMER05 : 1; //TIMER5 20"
	uint32         :10;
	uint32 CB_WDT0 : 1; //CB_WDT0
	uint32 CB_WDT1 : 1; //CB_WDT1
	uint32 CB_WDT2 : 1; //CB_WDT2
	uint32 CB_WDT3 : 1; //CB_WDT3
	uint32 CB_WDT4 : 1; //CB_WDT4
	uint32         :11;
}TIMER_CONF;

typedef union {
	uint32     nTIMERCONF;
	TIMER_CONF bTIMERCONF;
}TIMER_CONF_U;

// PLL
typedef struct {
	uint32 PLL00 : 1;
	uint32 PLL01 : 1;
	uint32 PLL02 : 1;
	uint32 PLL03 : 1; // for memory bus
	uint32 PLL04 : 1; // dithered pll
	uint32       :27;
}PLL_CONF;

typedef union {
	uint32   nPLLCONF;
	PLL_CONF bPLLCONF;
}PLL_CONF_U;

typedef struct {
	DEVICE_SHARE_VER stVERSION;  // 0x00
	ADC_CONF_U       uADCCFG;    // 0x04
	PWM_CONF_U       uPWMCFG;    // 0x08
	I2C_CONF_U       uI2CCFG;    // 0x0C
	UART_CONF_U      uUARTCFG;   // 0x10
	SPI_CONF_U       uSPICFG;    // 0x14
	TIMER_CONF_U     uTIMERCFG;  // 0x18
	PLL_CONF_U       uPLLCFG;    // 0x1C
} DEVICE_SHARE_CONFIG_T;

extern void WriteDeviceShareConfig( uint32 dest);

// check if device is dedicated for micom, if it dedicated, then return 1. else return 0.
// deviceType : type of SHARE_DEVICE
// channel : index of device channel starting from 0 
extern uint8 ConfirmDeviceChannel(uint32 deviceType, uint32 channel);
#endif

