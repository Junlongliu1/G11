/****************************************************************************
 *   FileName    : uart.h
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

#ifndef _PLATFORM_TCC_UART_H_
#define _PLATFORM_TCC_UART_H_

#include "udma.h"

#define UART_CH0	0
#define UART_CH1	1
#define UART_CH2	2
#define UART_CH3	3
#define UART_CH_MAX	4

//
// UART Transfer Operation Mode selection
//
#define UART_NON_INTR_MODE	0
#define UART_INTR_MODE		1
#define UART_DMA_MODE		2

#define UDMA_ADDRESS_OFFSET             0
#define UDMA_ADDRESS_UNIT               (UDMA_BUFF_SIZE + UDMA_BUFF_MARGIN)
#define UDMA_ADDRESS_UNIT_CH_RX(ch)     (UDMA_ADDRESS_OFFSET+(UDMA_ADDRESS_UNIT*2*(ch)))
#define UDMA_ADDRESS_UNIT_CH_TX(ch)     ((UDMA_ADDRESS_OFFSET+(UDMA_ADDRESS_UNIT*2*(ch)))+UDMA_ADDRESS_UNIT)

#define UART_BUFF_SIZE		0x100	// 256

// channel number : 0, 1, 2, 3
// baudrate : 9600, 14400, 19200, 38400, 57600, 115200, 230400, 460800, 921600 etc.
// flow control. Enable : 1, Disable : 0
// mode : UART_NON_INTR_MODE, UART_INTR_MODE, UART_DMA_MODE

#define UART0_CFG			35
#define UART0_BR			115200
#define UART0_RTSCTS_USE		0
#define UART0_MODE			UART_NON_INTR_MODE
#define UART0_CLK			48000000	// 48MHz

#define UART1_CLK			48000000	/* add for KLieDrv Uart*/

#define UART2_CLK			48000000	/* add for LineTool Uart*/


#if DEBUG_ENABLE
/* Debug */
#define DEBUG_UART                      UART_CH0
//#define DEBUG_CFG			UART0_CFG // if not defined, debug port number will be a number stored in SNOR flash.
#define DEBUG_BR			UART0_BR
#define DEBUG_RTSCTS_USE		UART0_RTSCTS_USE
#define DEBUG_UT_MODE			UART0_MODE
#else
#define DEBUG_UART			NULL
#endif

#define UT_CR_CTSEN		(1 << 15)	// CTS hardware flow control enable
#define UT_CR_RTSEN		(1 << 14)	// RTS hardware flow control enable
#define UT_CR_OUT2		(1 << 13)	// Reserved
#define UT_CR_OUT1		(1 << 12)	// Reserved
#define UT_CR_RTS		(1 << 11)	// Request to send
#define UT_CR_DTR		(1 << 10)	// Reserved
#define UT_CR_RXE		(1 << 9)	// Receive enable
#define UT_CR_TXE		(1 << 8)	// Transmit enable
#define UT_CR_LBE		(1 << 7)	// Loopback enable
#define UT_CR_IIRLP		(1 << 2)	// SIR low-power IrDA mode
#define UT_CR_SIREN		(1 << 1)	// SIR enable
#define UT_CR_UARTEN	(1 << 0)	// UART enable

struct board_serial_data {
	uint32 port_cfg;	// Config port ID
	uint32 port_tx;		// UT_TXD GPIO
	uint32 port_rx;		// UT_RXD GPIO
	uint32 port_rts;	// UT_RTS GPIO
	uint32 port_cts;	// UT_CTS GPIO
	uint32 port_fn;		// UART function select
};

struct intr_data
{
	uint32 ch;
	uint8 *xmit_buf;
	int32 head;
	int32 tail;
	int32 size;
};
typedef  void (*uartCallback)(uint8 data, uint8 error);

//extern int32 uart_cfg[UART_CH_MAX]; //Msg(4:1504) The object 'uart_cfg' is only referenced in the translation unit where it is defined. MISRA-C:2004 Rule 8.10

int32 uart_init(void);
int32 uart_probe(uint32 ch, uint32 baud, uint32 mode, uint32 crtscts);
int32 uart_Callback(uint32 ch,uartCallback func);
//int32 uart_set_baudrate(uint32 ch, uint32 baud);//Msg(4:1505) The function 'uart_set_baudrate' is only referenced in the translation unit where it is defined. MISRA-C:2004 Rule 8.10
void uart_set_control(uint32 ch, uint32 val, uint32 flag);
int32 uart_putc(uint32 ch, uint8 c);
int32 uart_getc(uint32 ch, int32 wait, int8 * err);
void console_putc(uint32 ch, uint8 c);
int32 console_getc(uint32 ch, int32 wait);
int32 uart_write(uint32 ch, const uint8 *buf, uint32 size);
int32 uart_read(uint32 ch, uint8 *buf, uint32 size);
void uart_flush_tx(uint32 ch);
void uart_flush_rx(uint32 ch);
void uart_intr_tx_probe(uint32 ch);
void uart_intr_rx_probe(uint32 ch);
int32 uart_intr_write(uint32 ch, const uint8 *buf, uint32 size);
int32 uart_intr_read(uint32 ch, uint8 *buf, uint32 size);
void uart_dma_tx_probe(uint32 ch, uint32 *nc_addr);
void uart_dma_rx_probe(uint32 ch, uint32 *nc_addr);
int32 uart_dma_tx_enable(uint32 ch, uint32 size, const struct udma_data *dmacon);
int32 uart_dma_rx_enable(uint32 ch, uint32 size, const struct udma_data *dmacon);
int32 uart_dma_write(uint32 ch, const uint8 *buf, uint32 size);
int32 uart_dma_read(uint32 ch, uint8 *buf, uint32 size);
int32 uart_close(uint32 ch);
#endif /* _PLATFORM_TCC_UART_H_ */
