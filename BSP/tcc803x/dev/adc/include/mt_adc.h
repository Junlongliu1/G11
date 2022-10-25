#ifndef MTADC_H
#define MTADC_H
/****************************************************************************
 *   FileName    : mt_adc.h
 *   Description : Multi-input ADC
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

#include "types.h"

// ADC Register
#define SMP_CMD_STS_REG		0x00
#define IRQ_DREQ_STS_REG	0x01
#define CTRL_CFG_REG		0x02
#define SDOUT0_REG			0x20
#define SDOUT1_REG			0x21
#define SDOUT2_REG			0x22
#define SDOUT3_REG			0x23
#define SDOUT4_REG			0x24
#define SDOUT5_REG			0x25
#define SDOUT6_REG			0x26
#define SDOUT7_REG			0x27
#define SDOUT8_REG			0x28
#define SDOUT9_REG			0x29
#define SDOUT10_REG			0x2A
#define SDOUT11_REG			0x2B

#define ADC_IRQ_CLR			(1<<0)
#define ADC_IRQ_EN			(1<<8)
#define ADC_DMA_EN			(1<<9)

enum {
	ADC_CHANNEL0 = 0,
	ADC_CHANNEL1,
	ADC_CHANNEL2,
	ADC_CHANNEL3,
	ADC_CHANNEL4,
	ADC_CHANNEL5,
	ADC_CHANNEL6,
	ADC_CHANNEL7,
	ADC_CHANNEL8,
	ADC_CHANNEL9,
	ADC_CHANNEL10,
	ADC_CHANNEL11,
	ADC_CHANNEL_MAX
};

enum {
	ADC_MODE_NORMAL = 0,
	ADC_MODE_IRQ,
	ADC_MODE_DMA
};

typedef struct {
	uint32 	uADC_CMD;						// 0x00
	uint32 	uADC_STS;						// 0x04
	uint32 	uADC_CFG;						// 0x08
	uint32  reserved_0c[(0x040-0x00c)/4];	// 0x0c ~ 0x3f
	uint32	uADC_DATA0;						// 0x40
	uint32	uADC_DATA1;						// 0x44
	uint32	uADC_DATA2;						// 0x48
	uint32	uADC_DATA3;						// 0x4c
	uint32	uADC_DATA4;						// 0x50
	uint32	uADC_DATA5;						// 0x54
	uint32	uADC_DATA6;						// 0x58
	uint32	uADC_DATA7;						// 0x5c
	uint32	reserved1;						// 0x60
	uint32	reserved2;						// 0x64
	uint32	uADC_DATA8;						// 0x68
	uint32	uADC_DATA9;						// 0x6c
} MC_ADC;

void init_adc(int32 Type);
uint32 tcc_adc_read(int32 Channel);
uint32* mt_adc_auto_scan(void);
void tcc_adc_watching_evt(void);
#endif /* MTADC_H */
