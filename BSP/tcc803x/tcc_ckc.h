/****************************************************************************
 *   FileName    : tcc_ckc.h
 *   Description :
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
#ifndef _PLATFORM_TCC_CKC_H_
#define _PLATFORM_TCC_CKC_H_

#include "types.h"

#define XIN_CLK_RATE	((24*1000)*1000)	// 24MHz
#define XTIN_CLK_RATE	32768	// 32.768kHz

#define CKC_DISABLE	0
#define CKC_ENABLE	1
#define CKC_NOCHANGE	2

/* PLL channel index */
enum {
	PLL_MICOM_0 = 0,
	PLL_MICOM_1,
};

/* MICOM pll source channel index */
enum {
	MPLL_0 = 0,
	MPLL_1,
	MPLL_DIV_0,
	MPLL_DIV_1,
	MPLL_XIN
};

enum {
	PCLKCTRL_SEL_PLL0=0,
	PCLKCTRL_SEL_PLL1,
	PCLKCTRL_SEL_XIN=5,
	PCLKCTRL_SEL_PLL0DIV=10,
	PCLKCTRL_SEL_PLL1DIV=11,
	PCLKCTRL_SEL_XINDIV=23
};

/* CLKCTRL channel index */
enum {
	FBUS_CPU = 0,
};

/* Peripheral Clocks */
enum {/* Peri. Name */
	// MICOM Peri
	PERI_SFMC = 0,
	PERI_CAN0,
	PERI_CAN1,
	PERI_CAN2,
	PERI_GPSB0,
	PERI_GPSB1,
	PERI_GPSB2,
	PERI_UART0,
	PERI_UART1,
	PERI_UART2,
	PERI_UART3,
	PERI_I2C0,
	PERI_I2C1,
	PERI_I2C2,
	PERI_PWM0,
	PERI_PWM1,
	PERI_PWM2,
	PERI_ICTC0,
	PERI_ICTC1,
	PERI_ICTC2,
	PERI_ADC,
	PERI_TIMER0,
	PERI_TIMER1,
	PERI_TIMER2,
	PERI_TIMER3,
	PERI_TIMER4,
	PERI_TIMER5,
	PERI_WDT,
	PERI_GPSB3,
	PERI_MAX,
};


/* I/O Bus pwdn/swreset */
enum {
	IOBUS_NSAID_FILTER_BUS = 0, // 0
	IOBUS_NSAID_FILTER_CORE, // 1
	IOBUS_NSAID_CFG_BUS, // 2
	IOBUS_SGID_FILTER_BUS, // 3
	IOBUS_SGID_FILTER_CORE, // 4
	IOBUS_SGID_CFG_BUS, // 5
	IOBUS_SFMC_BUS, // 6
	IOBUS_SFMC_MID_FILTER_BUS, // 7
	IOBUS_SFMC_MID_CFG_BUS, // 8
	IOBUS_SFMC_BRIDGE, // 9
	IOBUS_GPIO_BUS = 17, // 17
	IOBUS_SOC400, // 18
	IOBUS_JTAG, // 19
	IOBUS_DMA_GRP, // 20
	IOBUS_CAN_GRP, // 21
	IOBUS_CAN0, // 22
	IOBUS_CAN1, // 23
	IOBUS_CAN2, // 24
	IOBUS_CAN_BUS, // 25
	IOBUS_GPSB_GRP, // 26
	IOBUS_GPSB0, // 27
	IOBUS_GPSB1, // 28
	IOBUS_GPSB2, // 29
	IOBUS_GPSB_BUS, // 30
	IOBUS_GPSB3, // 31
	IOBUS_UART_GRP = 32, // 0 32
	IOBUS_UDMA0, // 1 33
	IOBUS_UDMA1, // 2 34
	IOBUS_UDMA2, // 3 35
	IOBUS_UDMA3, // 4 36
	IOBUS_UART0, // 5 37
	IOBUS_UART1, // 6 38
	IOBUS_UART2, // 7 39
	IOBUS_UART3, // 8 40
	IOBUS_UART_BUS, // 9 41
	IOBUS_I2C_GRP, // 10 42
	IOBUS_I2C0, // 11 43
	IOBUS_I2C1, // 12 44
	IOBUS_I2C2, // 13 45
	IOBUS_PWM_GRP, // 14 46
	IOBUS_PWM0, // 15 47
	IOBUS_PWM1, // 16 48
	IOBUS_PWM2, // 17 49
	IOBUS_PWM_BUS, // 18 50
	IOBUS_ICTC_GRP, // 19 51
	IOBUS_ICTC0, // 20 52
	IOBUS_ICTC1, // 21 53
	IOBUS_ICTC2, // 22 54
	IOBUS_ADC, // 23 55
	IOBUS_MAILBOX, // 24 56

	IOBUS_TIMER_GRP = 64, // 0 64
	IOBUS_TIMER0, // 1 65
	IOBUS_TIMER1 = 67, // 3 67
	IOBUS_TIMER2, // 4 68
	IOBUS_TIMER3, // 5 69
	IOBUS_TIMER4, // 6 70
	IOBUS_TIMER5, // 7 71
	IOBUS_WDT, // 8 72
	IOBUS_MAX = 73
};

typedef struct {
	uint32	fpll;
	uint32	en;
	uint32	p;
	uint32	m;
	uint32	s;
	uint32	src;
} tPMS;

typedef struct {
	uint32 fpll;
	uint32 pms;
} tPMSValue;

typedef struct {
	uint32	freq;
	uint32	en;
	uint32	config;
	uint32	sel;
} tCLKCTRL;

typedef struct {
	uint32	periname;
	uint32	freq;
	uint32	md;
	uint32	en;
	uint32	sel;
	uint32	div_val;
} tPCLKCTRL;

typedef struct {
	uint32 fpll;
	uint32 p;
	uint32 m;
	uint32 s;
} tFixedPMS;

extern void tcc_ckc_init(void);

#endif /* _PLATFORM_TCC_CKC_H_ */
