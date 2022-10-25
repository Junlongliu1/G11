/****************************************************************************
 *   FileName    : uart.c
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

#include <types.h>
#include <osal.h> // QAC for OASL_MemSet
#include <bsp.h>
#include <bsp_int.h>
#include "reg_phys.h"
#include "tcc_ckc.h"
#include "uart.h"
#include "clock.h"
#include "gpio.h"
#include "mpu.h" //mem_get_dma_base

#include "temp_compile_switch.h"

#define TCC_GPNONE		0xFFFF
#define TCC_GPSD0(x)	TCC_GPNONE
#define TCC_GPSD1(x)	TCC_GPNONE
#define TCC_GPSD2(x)	TCC_GPNONE

// UART FIFO buffer size
#define TX_FIFO_SIZE	8
#define RX_FIFO_SIZE	12

// UART Base address
#define UART_GET_BASE(n)	(TCC_UART_BASE + (0x10000 * (n)))
#define UARTPORTCFG			(TCC_UART_BASE + 0x40010)

// UART Port CFG Base address
#define SNOR_UARTCFG_VAL_ADDR(n)	(TCC_SNOR_BASE + 0x1100 + (0x4 * (n)))

// UART Register Offset
#define OFFSET_DR		(0x00)		// Data register
#define OFFSET_RSR		(0x04)		// Receive Status register
#define OFFSET_ECR		(0x04)		// Error Clear register
#define OFFSET_FR		(0x18)		// Flag register
#define OFFSET_ILPR		(0x20)		// IrDA Low-power Counter register
#define OFFSET_IBRD		(0x24)		// Integer Baud rate register
#define OFFSET_FBRD		(0x28)		// Fractional Baud rate register
#define OFFSET_LCRH		(0x2c)		// Line Control register
#define OFFSET_CR		(0x30)		// Control register
#define OFFSET_IFLS		(0x34)		// Interrupt FIFO Level status register
#define OFFSET_IMSC		(0x38)		// Interrupt Mask Set/Clear register
#define OFFSET_RIS		(0x3c)		// Raw Interrupt Status register
#define OFFSET_MIS		(0x40)		// Masked Interrupt Status register
#define OFFSET_ICR		(0x44)		// Interrupt Clear register
#define OFFSET_DMACR	(0x48)		// DMA Control register

#define FR_TXFE			(1 << 7)	// Transmit FIFO empty
#define FR_RXFF			(1 << 6)	// Receive FIFO full
#define FR_TXFF			(1 << 5)	// Transmit FIFO full
#define FR_RXFE			(1 << 4)	// Receive FIFO empty
#define FR_BUSY			(1 << 3)	// UART busy

#define LCRH_SPS		(1 << 7)	// Stick parity select
#define LCRH_WLEN(x)	((x)<<5)	// Word length
#define LCRH_FEN		(1 << 4)	// Enable FIFOs
#define LCRH_STP2		(1 << 3)	// Two stop bits select
#define LCRH_EPS		(1 << 2)	// Even parity select
#define LCRH_PEN		(1 << 1)	// Parity enable
#define LCRH_BRK		(1 << 0)	// Send break

#define INT_OEIS		(1 << 10)	// Overrun error interrupt
#define INT_BEIS		(1 << 9)	// Break error interrupt
#define INT_PEIS		(1 << 8)	// Parity error interrupt
#define INT_FEIS		(1 << 7)	// Framing error interrupt
#define INT_RTIS		(1 << 6)	// Receive timeout interrupt
#define INT_TXIS		(1 << 5)	// Transmit interrupt
#define INT_RXIS		(1 << 4)	// Receive interrupt

enum {
	WORD_LEN_5 = 0,
	WORD_LEN_6,
	WORD_LEN_7,
	WORD_LEN_8
};

#define CR_CTSEN		(1 << 15)	// CTS hardware flow control enable
#define CR_RTSEN		(1 << 14)	// RTS hardware flow control enable
#define CR_OUT2			(1 << 13)	// Reserved
#define CR_OUT1			(1 << 12)	// Reserved
#define CR_RTS			(1 << 11)	// Request to send
#define CR_DTR			(1 << 10)	// Reserved
#define CR_RXE			(1 << 9)	// Receive enable
#define CR_TXE			(1 << 8)	// Transmit enable
#define CR_LBE			(1 << 7)	// Loopback enable
#define CR_IIRLP		(1 << 2)	// SIR low-power IrDA mode
#define CR_SIREN		(1 << 1)	// SIR enable
#define CR_UARTEN		(1 << 0)	// UART enable

#define DMACR_DMAONERR	(1 << 2)	// DMA on error
#define DMACR_TXDMAE	(1 << 1)	// Transmit DMA enable
#define DMACR_RXDMAE	(1 << 0)	// Receive DMA enable

#define UART_MODE_TX	0
#define UART_MODE_RX	1

#define RX_FLAG			(1 << 0)
#define TX_FLAG			(1 << 1)

#define PORT_CFG_MAX	36
#define PORT_TBL_SIZE	(PORT_CFG_MAX - 5)

#define uart_reg_write(p, a, v)	writel(v, uart[p].base + (a))
#define uart_reg_read(p, a)	readl(uart[p].base + (a))

struct uart_stat {
	bool is_probed;
	addr_t base;	// UART Controller BASE
	uint32 ch;		// UART Channel
	uint32 op_mode;	// Operation Mode
	uint32 crtscts;
	struct udma_data rx_dma;
	struct udma_data tx_dma;
	struct intr_data rx_intr;
	struct intr_data tx_intr;
	uartCallback func;
};

static struct uart_stat uart[UART_CH_MAX];

#if (TEMP_COMPILE_SWITCH_05WORK_10WORK == TEMP_COMPILE_SWITCH_05WORK)
static int32 uart_cfg[UART_CH_MAX] = {35, 31, 32, -1}; //Msg(4:1504) The object 'uart_cfg' is only referenced in the translation unit where it is defined. MISRA-C:2004 Rule 8.10
#else
static int32 uart_cfg[UART_CH_MAX] = {35, 12, 26, -1}; //Msg(4:1504) The object 'uart_cfg' is only referenced in the translation unit where it is defined. MISRA-C:2004 Rule 8.10
#endif

#if defined(UART0_MODE)
static uint8 uart_buff0[2][UART_BUFF_SIZE];
#endif
#if defined(UART1_MODE)
static uint8 uart_buff1[2][UART_BUFF_SIZE];
#endif
#if defined(UART2_MODE)
static uint8 uart_buff2[2][UART_BUFF_SIZE];
#endif
#if defined(UART3_MODE)
static uint8 uart_buff3[2][UART_BUFF_SIZE];
#endif

static uint8 *uart_buff[UART_CH_MAX][2] =
{
#if defined(UART0_MODE)
	{ uart_buff0[0], uart_buff0[1] },
#else
	{ NULL, NULL },
#endif
#if defined(UART1_MODE)
	{ uart_buff1[0], uart_buff1[1] },
#else
	{ NULL, NULL },
#endif
#if defined(UART2_MODE)
	{ uart_buff2[0], uart_buff2[1] },
#else
	{ NULL, NULL },
#endif
#if defined(UART3_MODE)
	{ uart_buff3[0], uart_buff3[1] },
#else
	{ NULL, NULL }
#endif
};

static uint32 uart_clk[UART_CH_MAX] =
{
#if defined(UART0_CFG)
	UART0_CLK,
#else
	0,
#endif
#if defined(UART1_CFG)
        UART1_CLK,
#else
        UART1_CLK,
#endif
#if defined(UART2_CFG)
        UART2_CLK,
#else
        UART2_CLK,	/* add for LineTool Uart */
#endif
#if defined(UART3_CFG)
        UART3_CLK,
#else
        0,
#endif

};

//uint32* dma_tx_buf; //Msg(4:1514) The object 'dma_tx_buf' is only referenced by function 'uart_probe', in the translation unit where it is defined MISRA-C:2004 Rule 8.7
//uint32* dma_rx_buf; //Msg(4:1514) The object 'dma_tx_buf' is only referenced by function 'uart_probe', in the translation unit where it is defined MISRA-C:2004 Rule 8.7

static int32 uart_set_gpio(uint32 ch, const struct board_serial_data *info);
static int32 uart_set_port_config(uint32 ch, uint32 port, uint32 status);
static int32 uart_set_ch_config(uint32 ch, uint32 baud);
static int32 uart_rx(uint32 ch);
static int32 uart_rx_char(uint32 ch);
static int32 uart_tx_char(uint32 ch, uint8 c);
static int32 uart_tx(uint32 ch);
//static int32 wait_uart_intr(uint32 ch, uint32 input);
static void uart_isr(void *arg);
static void __uart_putc(uint32 ch, uint8 c);
static void uart_enable_interrupts(uint32 ch);
static int32 uart_dma_rx_trigger_dma(uint32 ch);
static void uart_dma_rx_irq(uint32 ch);
static int32 uart_set_baudrate(uint32 ch, uint32 baud);

static int32 uart_clear_gpio(uint32 ch, const struct board_serial_data *info);
static void uart_disable_interrupts(uint32 ch);

static int32 uart_set_gpio(uint32 ch, const struct board_serial_data *info)
{
	void __iomem *portcfg = (void __iomem *)UARTPORTCFG;
	uint32 portcfg_val;
	int32 ret;

	/* set portcfg */
	portcfg_val = readl(portcfg);
	portcfg_val = (portcfg_val & ~(0xFF<<(ch*8))) | (info->port_cfg<<(ch*8));
	writel(portcfg_val, portcfg);

	/* set debug port */
#if (TEMP_COMPILE_SWITCH_05WORK_10WORK == TEMP_COMPILE_SWITCH_05WORK)
	ret = gpio_config(info->port_tx, (info->port_fn)); // TX
	if (ret != 0) {
		return BSP_ERR_INIT;
	}
#else
	ret = gpio_config(info->port_tx, (info->port_fn) | GPIO_OUTPUT | GPIO_CD(2)); // TX
	if (ret != 0) {
		return BSP_ERR_INIT;
	}
#endif
	ret = gpio_config(info->port_rx, (info->port_fn|GPIO_INPUT|GPIO_INPUTBUF_EN)); // RX
	if (ret != 0) {
		return BSP_ERR_INIT;
	}

	if (uart[ch].crtscts != 0) {
		ret = gpio_config(info->port_rts, info->port_fn); // RTS
		if (ret != 0) {
			return BSP_ERR_INIT;
		}
		ret = gpio_config(info->port_cts, info->port_fn); // CTS
		if (ret != 0) {
			return BSP_ERR_INIT;
		}
	}

	return BSP_ERR_NONE;
}

static int32 uart_clear_gpio(uint32 ch, const struct board_serial_data *info) {

	void __iomem *portcfg = (void __iomem *)UARTPORTCFG;
	uint32 portcfg_val;
	int32 ret;
    uint32 reg_byte;

	if ( ch >= UART_CH_MAX)
	{
		return BSP_ERR_INVALID_PARAMETER;
	}

	/* clear uart gpio */
	ret = gpio_config(info->port_tx, (info->port_fn)); // TX
	if (ret != 0) {
		return BSP_ERR_INIT;
	}
	ret = gpio_config(info->port_rx, (info->port_fn|GPIO_INPUT|GPIO_INPUTBUF_EN)); // RX
	if (ret != 0) {
		return BSP_ERR_INIT;
	}

	if (uart[ch].crtscts != 0) {
		ret = gpio_config(info->port_rts, info->port_fn); // RTS
		if (ret != 0) {
			return BSP_ERR_INIT;
		}
		ret = gpio_config(info->port_cts, info->port_fn); // CTS
		if (ret != 0) {
			return BSP_ERR_INIT;
		}
	}

	/* set portcfg */
	reg_byte = ch & 0x03U;

	portcfg_val = readl(portcfg);
	portcfg_val |= (reg_byte << (reg_byte * 8));
	writel(portcfg_val, portcfg);
	return BSP_ERR_NONE;
}

static int32 uart_set_port_config(uint32 ch, uint32 port, uint32 status)
{
	int32 ret;
	uint32 i;

	static const struct board_serial_data board_serial[PORT_TBL_SIZE + 1] = {
#if 0
		/* Coretex-R5 is not allowed to use the GPIO_SD0-2 ports */
		{ 0,  TCC_GPSD0(11), TCC_GPSD0(12), TCC_GPSD0(13), TCC_GPSD0(14), GPIO_FUNC(7) },
		{ 1,  TCC_GPSD1(0),  TCC_GPSD1(1),  TCC_GPSD1(2),  TCC_GPSD1(3),  GPIO_FUNC(7) },
		{ 2,  TCC_GPSD1(6),  TCC_GPSD1(7),  TCC_GPSD1(8),  TCC_GPSD1(9),  GPIO_FUNC(7) },
		{ 3,  TCC_GPSD2(0),  TCC_GPSD2(1),  TCC_GPSD2(2),  TCC_GPSD2(3),  GPIO_FUNC(7) },
		{ 4,  TCC_GPSD2(6),  TCC_GPSD2(7),  TCC_GPNONE,    TCC_GPNONE,    GPIO_FUNC(7) },
#endif
		{ 5,  TCC_GPA(0),    TCC_GPA(1),    TCC_GPA(2),    TCC_GPA(3),    GPIO_FUNC(7) },
		{ 6,  TCC_GPA(6),    TCC_GPA(7),    TCC_GPNONE,    TCC_GPNONE,    GPIO_FUNC(7) },
		{ 7,  TCC_GPA(12),   TCC_GPA(13),   TCC_GPNONE,    TCC_GPNONE,    GPIO_FUNC(7) },
		{ 8,  TCC_GPA(18),   TCC_GPA(19),   TCC_GPNONE,    TCC_GPNONE,    GPIO_FUNC(7) },
		{ 9,  TCC_GPA(24),   TCC_GPA(25),   TCC_GPA(26),   TCC_GPA(27),   GPIO_FUNC(7) },
		{ 10, TCC_GPB(0),    TCC_GPB(1),    TCC_GPB(2),    TCC_GPB(3),    GPIO_FUNC(7) },
		{ 11, TCC_GPB(6),    TCC_GPB(7),    TCC_GPNONE,    TCC_GPNONE,    GPIO_FUNC(7) },
		{ 12, TCC_GPB(12),   TCC_GPB(13),   TCC_GPNONE,    TCC_GPNONE,    GPIO_FUNC(7) },
		{ 13, TCC_GPB(19),   TCC_GPB(20),   TCC_GPB(21),   TCC_GPB(22),   GPIO_FUNC(7) },
		{ 14, TCC_GPB(25),   TCC_GPB(26),   TCC_GPB(27),   TCC_GPB(28),   GPIO_FUNC(7) },
		{ 15, TCC_GPC(0),    TCC_GPC(1),    TCC_GPC(2),    TCC_GPC(3),    GPIO_FUNC(7) },
		{ 16, TCC_GPC(4),    TCC_GPC(5),    TCC_GPC(6),    TCC_GPC(7),    GPIO_FUNC(7) },
		{ 17, TCC_GPC(8),    TCC_GPC(9),    TCC_GPC(10),   TCC_GPC(11),   GPIO_FUNC(7) },
		{ 18, TCC_GPC(10),   TCC_GPC(11),   TCC_GPC(12),   TCC_GPC(13),   GPIO_FUNC(8) },
		{ 19, TCC_GPC(12),   TCC_GPC(13),   TCC_GPC(14),   TCC_GPC(15),   GPIO_FUNC(7) },
		{ 20, TCC_GPC(16),   TCC_GPC(17),   TCC_GPC(18),   TCC_GPC(19),   GPIO_FUNC(7) },
		{ 21, TCC_GPC(20),   TCC_GPC(21),   TCC_GPC(22),   TCC_GPC(23),   GPIO_FUNC(7) },
		{ 22, TCC_GPC(26),   TCC_GPC(27),   TCC_GPC(28),   TCC_GPC(29),   GPIO_FUNC(7) },
		{ 23, TCC_GPG(0),    TCC_GPG(1),    TCC_GPG(2),    TCC_GPG(3),    GPIO_FUNC(7) },
		{ 24, TCC_GPG(7),    TCC_GPG(8),    TCC_GPG(9),    TCC_GPG(10),   GPIO_FUNC(7) },
		{ 25, TCC_GPE(5),    TCC_GPG(6),    TCC_GPG(7),    TCC_GPG(8),    GPIO_FUNC(7) },
		{ 26, TCC_GPE(11),   TCC_GPE(12),   TCC_GPG(13),   TCC_GPG(14),   GPIO_FUNC(7) },
		{ 27, TCC_GPE(16),   TCC_GPE(17),   TCC_GPG(18),   TCC_GPG(19),   GPIO_FUNC(7) },
		{ 28, TCC_GPH(4),    TCC_GPH(5),    TCC_GPNONE,    TCC_GPNONE,    GPIO_FUNC(7) },
		{ 29, TCC_GPH(6),    TCC_GPH(7),    TCC_GPNONE,    TCC_GPNONE,    GPIO_FUNC(7) },
		{ 30, TCC_GPH(0),    TCC_GPH(1),    TCC_GPH(2),    TCC_GPH(3),    GPIO_FUNC(7) },
		{ 31, TCC_GPMA(0),   TCC_GPMA(1),   TCC_GPMA(2),   TCC_GPMA(3),   GPIO_FUNC(7) },
		{ 32, TCC_GPMA(6),   TCC_GPMA(7),   TCC_GPMA(8),   TCC_GPMA(9),   GPIO_FUNC(7) },
		{ 33, TCC_GPMA(12),  TCC_GPMA(13),  TCC_GPMA(14),  TCC_GPMA(15),  GPIO_FUNC(7) },
		{ 34, TCC_GPMA(18),  TCC_GPMA(19),  TCC_GPMA(20),  TCC_GPMA(21),  GPIO_FUNC(7) },
		{ 35, TCC_GPMA(24),  TCC_GPMA(25),  TCC_GPMA(26),  TCC_GPMA(27),  GPIO_FUNC(7) },
		{ 36, TCC_GPK(9),    TCC_GPK(10),   TCC_GPK(11),   TCC_GPK(12),   GPIO_FUNC(2) }
	};

	for(i = 0; i <= PORT_TBL_SIZE; i++) {
		if(board_serial[i].port_cfg == port) {
			if (true == status) {
				/* set gpio config */
				ret = uart_set_gpio(ch, &board_serial[i]);
			} else {
				/* clear gpio config*/
				ret = uart_clear_gpio(ch, &board_serial[i]);
			}
			if (ret != 0) {
				return BSP_ERR_INIT;
			}
			break;
		}
	}

	return BSP_ERR_NONE;
}

static int32 uart_set_ch_config(uint32 ch, uint32 baud)
{
	uint32 cr_data;
	uint32 lcr_data;
	int32 ret = 0;

	/* Enable the UART controller peri clock */
	(void)tcc_iobus_enable(IOBUS_UART0 + ch);
	ret = tcc_set_peri(PERI_UART0 + ch, ENABLE, uart_clk[ch]);
	if (ret != 0) {
		return BSP_ERR_INIT;
	}

	(void)uart_set_baudrate(ch, baud);//Msg(4:3200) 'uart_set_baudrate' returns a value which is not being used. MISRA-C:2004 Rule 16.10

	// control register setting
	cr_data = CR_UARTEN;
	cr_data |= CR_TXE;
	cr_data |= CR_RXE;
	if (uart[ch].crtscts != 0)
	{ //brace
		cr_data |= (CR_RTSEN | CR_CTSEN);
	}
	uart_reg_write(ch, OFFSET_CR, cr_data);

	// line control setting
	lcr_data = LCRH_WLEN(WORD_LEN_8);
	if ((uart[ch].op_mode == UART_DMA_MODE) || (uart[ch].op_mode == UART_INTR_MODE))
	{
		lcr_data |= LCRH_FEN;
	}

	uart_reg_write(ch, OFFSET_LCRH, lcr_data);

	return BSP_ERR_NONE;
}

static int32 uart_set_baudrate(uint32 ch, uint32 baud) // (uint32 => int32)return type mismatched
{
	uint32 u32_div; //Msg(4:5115) The names of standard library macros, objects and functions shall not be reused. MISRA-C:2004 Rule 20.2
	uint32 mod;
	uint32 brd_i;
	uint32 brd_f;
	uint32 pclk;

	/* QAC : Rule 21.18 Apparent: Dereference of an invalid pointer value  */
	if ( ch >= UART_CH_MAX)
	{
		return BSP_ERR_INVALID_PARAMETER;
	}
		
	// Read the peri clock
	pclk = tcc_get_peri(PERI_UART0 + ch);
	if (pclk == 0)
	{
		return BSP_ERR_INIT;
	}

	// calculate integer baud rate divisor
	u32_div = 16 * baud;
	brd_i = pclk / u32_div;
	uart_reg_write(ch, OFFSET_IBRD, brd_i);

	// calculate faction baud rate divisor
	// NOTICE : fraction maybe need sampling
	mod = pclk % (16 * baud);
	u32_div = ((1 << 3) * 16 * mod) / (16 * baud);
	brd_f = u32_div / 2;
	uart_reg_write(ch, OFFSET_FBRD, brd_f);

	return BSP_ERR_NONE;
}

void uart_set_control(uint32 ch, uint32 val, uint32 flag)
{
	uint32 cr_reg;
	
	/* QAC : Rule 21.18 Apparent: Dereference of an invalid pointer value  */
	if ( ch < UART_CH_MAX)
	{

		cr_reg = uart_reg_read(ch, OFFSET_CR);

		if (flag == 0)
		{
			cr_reg &= ~(val);
		}
		else
		{
			cr_reg |= (val);
		}

		uart_reg_write(ch, OFFSET_CR, cr_reg);
	}
}

int32 uart_init(void)
{
	int32 res = BSP_ERR_NONE;
	uint32 i;
	#if DEBUG_ENABLE
	uint16 cfg_val;
	#endif
	for (i = 0; i < UART_CH_MAX; i++)
	{
		uart[i].is_probed = 0;
		uart[i].base = UART_GET_BASE(i);
		uart[i].ch = i;
		uart[i].op_mode = UART_NON_INTR_MODE;
		uart[i].crtscts = 0;
		/* Interupt mode init */
		uart[i].tx_intr.ch = 0xFF;
		uart[i].tx_intr.xmit_buf = NULL;
		uart[i].tx_intr.head = -1;
		uart[i].tx_intr.tail = -1;
		uart[i].tx_intr.size = 0;
		uart[i].rx_intr.ch = 0xFF;
		uart[i].rx_intr.xmit_buf = NULL;
		uart[i].rx_intr.head = -1;
		uart[i].rx_intr.tail = -1;
		uart[i].rx_intr.size = 0;
		/* DMA mode init */
		uart[i].tx_dma.con = 0xFF;
		uart[i].tx_dma.ch = 0xFF;
		uart[i].tx_dma.src_addr = NULL;
		uart[i].tx_dma.dest_addr = NULL;
		uart[i].tx_dma.buff_size = UDMA_BUFF_SIZE;
		uart[i].tx_dma.transfer_size = 0;
		uart[i].tx_dma.is_run = 0;
		uart[i].rx_dma.con = 0xFF;
		uart[i].rx_dma.ch = 0xFF;
		uart[i].rx_dma.src_addr = NULL;
		uart[i].rx_dma.dest_addr = NULL;
		uart[i].rx_dma.buff_size = UDMA_BUFF_SIZE;
		uart[i].rx_dma.transfer_size = 0;
		uart[i].rx_dma.is_run = 0;
	}

#if DEBUG_ENABLE
#if defined(DEBUG_CFG)
	uart_cfg[DEBUG_UART] = DEBUG_CFG; // if DEBUG_CFG is defined, use DEBUG_PORT as debug port number
#else
	/* for read the debug UART configuration from snor flash */
	cfg_val = (readl(SNOR_UARTCFG_VAL_ADDR(DEBUG_UART)) & 0xFF);
	if (cfg_val <= PORT_CFG_MAX) {
		uart_cfg[DEBUG_UART] = (int32)cfg_val;// QAC : Definite: Dereference of an invalid pointer value.
	}
#endif
	res = uart_probe(DEBUG_UART, DEBUG_BR, DEBUG_UT_MODE, DEBUG_RTSCTS_USE);
#else
#if defined(UART0_CFG)
	uart_cfg[UART_CH0] = UART0_CFG;
	res = uart_probe(UART_CH0, UART0_BR, UART0_MODE, UART0_RTSCTS_USE);
#endif
#endif
#if defined(UART1_CFG)
	uart_cfg[UART_CH1] = UART1_CFG;
	res = uart_probe(UART_CH1, UART1_BR, UART1_MODE, UART1_RTSCTS_USE);
#endif
#if defined(UART2_CFG)
	uart_cfg[UART_CH2] = UART2_CFG;
	res = uart_probe(UART_CH2, UART2_BR, UART2_MODE, UART2_RTSCTS_USE);
#endif
#if defined(UART3_CFG)
	uart_cfg[UART_CH3] = UART3_CFG;
	res = uart_probe(UART_CH3, UART3_BR, UART3_MODE, UART3_RTSCTS_USE);
#endif

	return res;
}
int32 uart_Callback(uint32 ch,uartCallback func)
{
	int32 ret = BSP_ERR_NONE;

	uart[ch].func = func;

	return ret;
}

int32 uart_probe(uint32 ch, uint32 baud, uint32 mode, uint32 crtscts)
{
	int32 ret;
	static uint32* dma_tx_buf; //Msg(4:1514) The object 'dma_tx_buf' is only referenced by function 'uart_probe', in the translation unit where it is defined MISRA-C:2004 Rule 8.7
	static uint32* dma_rx_buf; //Msg(4:1514) The object 'dma_tx_buf' is only referenced by function 'uart_probe', in the translation unit where it is defined MISRA-C:2004 Rule 8.7
	
	/* QAC : Rule 21.18 Apparent: Dereference of an invalid pointer value  */
	if ( ch >= UART_CH_MAX)
	{
		return BSP_ERR_INVALID_PARAMETER;
	}

	if (uart[ch].is_probed != 0) {
		return BSP_ERR_INVALID_HANDLE;
	}

	uart[ch].op_mode = mode;
	uart[ch].crtscts = crtscts;

	// Set port config
	ret = uart_set_port_config(ch, uart_cfg[ch], true);
	if (ret != BSP_ERR_NONE) {
		return ret;
	}
	ret = uart_set_ch_config(ch, baud);
	if (ret != BSP_ERR_NONE) {
		return ret;
	}

	// Configure for interrupt mode
	if (uart[ch].op_mode == UART_INTR_MODE) 
	{
		uart_intr_tx_probe(ch);
		uart_intr_rx_probe(ch);

		uart_enable_interrupts(ch);
	}
	else if (uart[ch].op_mode == UART_DMA_MODE) 
	{
		/* If want to use the rx/tx dma mode, call the probe func at APP side */
		// Ex)
		// mem_get_dma_base() returns 0xC003E000
		if(ch == UART_CH0){
			dma_rx_buf = (uint32 *)(mem_get_dma_base() + UDMA_ADDRESS_UNIT_CH_RX(0));//warning. assignment pointer form integer without a case
			dma_tx_buf = (uint32 *)(mem_get_dma_base() + UDMA_ADDRESS_UNIT_CH_TX(0));
		} else if(ch == UART_CH1){
                        dma_rx_buf = (uint32 *)(mem_get_dma_base() + UDMA_ADDRESS_UNIT_CH_RX(1));
                        dma_tx_buf = (uint32 *)(mem_get_dma_base() + UDMA_ADDRESS_UNIT_CH_TX(1));
		} else if(ch == UART_CH2){
                        dma_rx_buf = (uint32 *)(mem_get_dma_base() + UDMA_ADDRESS_UNIT_CH_RX(2));
                        dma_tx_buf = (uint32 *)(mem_get_dma_base() + UDMA_ADDRESS_UNIT_CH_TX(2));
		}
		else//else if(ch == UART_CH3) //QAC : Always true
		{
                        dma_rx_buf = (uint32 *)(mem_get_dma_base() + UDMA_ADDRESS_UNIT_CH_RX(3));
                        dma_tx_buf = (uint32 *)(mem_get_dma_base() + UDMA_ADDRESS_UNIT_CH_TX(3));
		}

		OSAL_MemSet((void *)dma_rx_buf, 0, UDMA_BUFF_SIZE);
		OSAL_MemSet((void *)dma_tx_buf, 0, UDMA_BUFF_SIZE);
		uart_dma_tx_probe(ch, dma_tx_buf);
		uart_dma_rx_probe(ch, dma_rx_buf);
		uart_enable_interrupts(ch);

	}
	else //QAC : else if, else
	{
		;//No Statement
	}

	uart[ch].is_probed = 1;

	return BSP_ERR_NONE;
}
int32 uart_close(uint32 ch) {
	int32 ret;

	ret = BSP_ERR_NONE;
	if ( ch >= UART_CH_MAX)
	{
		return BSP_ERR_INVALID_PARAMETER;
	}

	uart[ch].is_probed = 0;

	/* close interrupts */
	uart_disable_interrupts(ch);

	/* clear clock */
	tcc_iobus_disable(IOBUS_UART0 + ch);
	(void)tcc_set_peri(PERI_UART0 + ch, DISABLE, 0);

	// Set port config
	ret = uart_set_port_config(ch, uart_cfg[ch], false);
	if (ret != BSP_ERR_NONE) {
		return ret;
	}
	return ret;
}



static int32 uart_rx(uint32 ch)
{
	uint32 status;
	int32 max_count = UART_BUFF_SIZE;
	uint32 data;
	uint8 *buf = NULL_PTR;
	
	/* QAC : Rule 21.18 Apparent: Dereference of an invalid pointer value  */
	if ( ch >= UART_CH_MAX)
	{
		return BSP_ERR_INVALID_PARAMETER;
	}

	buf = uart[ch].rx_intr.xmit_buf;

	while(max_count > 0) {
		status = uart_reg_read(ch, OFFSET_FR);
		if((status & FR_RXFE) != 0) {
			break;
		}

		data = uart_reg_read(ch, OFFSET_DR);

		buf[uart[ch].rx_intr.head] = data & 0xFF;
		uart[ch].rx_intr.head++;
		if(uart[ch].rx_intr.head >= uart[ch].rx_intr.size) {
			uart[ch].rx_intr.head = 0;
		}
		max_count--;
	}

	return BSP_ERR_NONE;
}

static int32 uart_rx_char(uint32 ch)
{
	uint8 data;
	uint8 error;

	if ( ch >= UART_CH_MAX)
	{
		return BSP_ERR_INVALID_PARAMETER;
	}

	if ((uart_reg_read(ch, OFFSET_FR) & FR_RXFE) != 0) {
		return BSP_ERR_NO_DATA;
	}
	data = uart_reg_read(ch, OFFSET_DR);

	if (NULL_PTR != uart[ch].func) {
		error = uart_reg_read(ch, OFFSET_RSR);
		if (0 == error) {
			uart[ch].func(data, BSP_ERR_NONE);
		} else {
			uart[ch].func(0, BSP_ERR_NO_DATA);
			uart_reg_write(ch, OFFSET_ECR, 0);
		}
	}

	return BSP_ERR_NONE;
}

static int32 uart_tx_char(uint32 ch, uint8 c)
{
	/* QAC : Rule 21.18 Apparent: Dereference of an invalid pointer value  */
	if ( ch >= UART_CH_MAX)
	{
		return BSP_ERR_INVALID_PARAMETER;
	}

	if ((uart_reg_read(ch, OFFSET_FR) & FR_TXFF) != 0) {
		return -1;
	}

	uart_reg_write(ch, OFFSET_DR, c);

	return 0;
}

static int32 uart_tx(uint32 ch)
{
	uint8 *buf = NULL_PTR;
	
	/* QAC : Rule 21.18 Apparent: Dereference of an invalid pointer value  */
	if ( ch >= UART_CH_MAX)
	{
		return BSP_ERR_INVALID_PARAMETER;
	}

	buf = uart[ch].tx_intr.xmit_buf;
	// xmit buffer is empty
	if (uart[ch].tx_intr.head == uart[ch].tx_intr.tail) {
		uart_reg_write(ch, OFFSET_ICR, INT_TXIS);
		return 0;
	}

	do {
		if (uart_tx_char(ch, buf[uart[ch].tx_intr.tail]) != 0) {
			break;
		}
		uart[ch].tx_intr.tail++;
		if (uart[ch].tx_intr.tail >= uart[ch].tx_intr.size) {
			uart[ch].tx_intr.tail = 0;
		}

	} while (uart[ch].tx_intr.head != uart[ch].tx_intr.tail);

	return BSP_ERR_NONE;
}

static void uart_isr(void *arg)
{
	struct uart_stat *up = arg;
	uint32 status;
	uint32 imsc;
	int32 max_count = UART_BUFF_SIZE;

	imsc = uart_reg_read(up->ch, OFFSET_IMSC);
	status = uart_reg_read(up->ch, OFFSET_MIS) & imsc;
	if (status != 0) {
		do {
			uart_reg_write(up->ch, OFFSET_ICR,
					status & ~(INT_RXIS | INT_RTIS | INT_TXIS));
			if((status & (INT_RTIS | INT_RXIS)) != 0) {
				if (uart[up->ch].op_mode == UART_DMA_MODE) {
					uart_dma_rx_irq(up->ch);
				}
				else {
					// (void)uart_rx(up->ch);//Msg(4:3200) 'uart_rx' returns a value which is not being used. MISRA-C:2004 Rule 16.10
					(void)uart_rx_char(up->ch);
				}

				uart_reg_write(up->ch, OFFSET_ICR, INT_RXIS | INT_RTIS);
			}

			if((status & INT_TXIS) != 0) {
				(void)uart_tx(up->ch);//Msg(4:3200) 'uart_rx' returns a value which is not being used. MISRA-C:2004 Rule 16.10
				//uart_reg_write(up->ch, OFFSET_ICR, INT_TXIS);
			}
			status = uart_reg_read(up->ch, OFFSET_MIS) & imsc;
			
			if (max_count > 0) {
				max_count--;
			}else{
				break;
			}
			
		} while(status != 0);
	}
}

/* GHC Warning : declared but never referenced
static int32 wait_uart_intr(uint32 ch, uint32 input)
{
	uint32 imsc;
	
	// QAC : Rule 21.18 Apparent: Dereference of an invalid pointer value  
	if ( ch >= UART_CH_MAX)
	{
		return BSP_ERR_INVALID_PARAMETER;
	}

	imsc = uart_reg_read(ch, OFFSET_IMSC);
	if(input == RX_FLAG) {
		imsc |= (INT_RXIS | INT_RTIS);
		uart_reg_write(ch, OFFSET_IMSC, imsc);
	}
	if(input == TX_FLAG) {
		imsc |= INT_TXIS;
		uart_reg_write(ch, OFFSET_IMSC, imsc);
	}

	return BSP_ERR_NONE;
}
*/
	
static void __uart_putc(uint32 ch, uint8 c)
{
	while ((uart_reg_read(ch, OFFSET_FR) & FR_TXFF) != 0) {
		if ((uart_reg_read(ch, OFFSET_FR) & FR_TXFF) == 0) {
			break;
		}
	}

	uart_reg_write(ch, OFFSET_DR, c);
}

void console_putc(uint32 ch, uint8 c)
{
	/* QAC : Rule 21.18 Apparent: Dereference of an invalid pointer value  */
	if ( ch < UART_CH_MAX)
	{

		if (c == '\n') {
			__uart_putc(ch, '\r');
		}

		__uart_putc(ch,c);
	}
}

int32 console_getc(uint32 ch, int32 wait)
{
	uint32 data;
	/* QAC : Rule 21.18 Apparent: Dereference of an invalid pointer value  */
	if ( ch >= UART_CH_MAX)
	{
		return BSP_ERR_INVALID_PARAMETER;
	}

	if(wait == 1) {
		while ((uart_reg_read(ch, OFFSET_FR) & FR_RXFE) != 0) {
			if((uart_reg_read(ch, OFFSET_FR) & FR_RXFE) == 0) {
				break;
			}
		}
	}
	else {
		if((uart_reg_read(ch, OFFSET_FR) & FR_RXFE) != 0) {
			return -1;
		}
	}

	data = uart_reg_read(ch, OFFSET_DR);

	return (int32)data;
}

int32 uart_putc(uint32 ch, uint8 c)
{
	/* QAC : Rule 21.18 Apparent: Dereference of an invalid pointer value  */
	if ( ch >= UART_CH_MAX)
	{
		return BSP_ERR_INVALID_PARAMETER;
	}

	while ((uart_reg_read(ch, OFFSET_FR) & FR_TXFF) != 0) {
		if ((uart_reg_read(ch, OFFSET_FR) & FR_TXFF) == 0) {
			break;
		}
	}

	uart_reg_write(ch, OFFSET_DR, c);

	return BSP_ERR_NONE;
}

int32 uart_getc(uint32 ch, int32 wait, int8 * err)
{
	uint32 data;
	/* QAC : Rule 21.18 Apparent: Dereference of an invalid pointer value  */
	if ( ch >= UART_CH_MAX)
	{
		return BSP_ERR_INVALID_PARAMETER;
	}

	if(wait == 1) {
		while ((uart_reg_read(ch, OFFSET_FR) & FR_RXFE) != 0) {
			if ((uart_reg_read(ch, OFFSET_FR) & FR_RXFE) == 0) {
				break;
			}
		}
	}
	else {
		if((uart_reg_read(ch, OFFSET_FR) & FR_RXFE) != 0) {
			*err = (int8)(-1);
			return -1;
		}
	}

	data = uart_reg_read(ch, OFFSET_DR);

	return (int32) data;
}

int32 uart_write(uint32 ch, const uint8 *buf, uint32 size)
{
	uint32 i;
	/* QAC : Rule 21.18 Apparent: Dereference of an invalid pointer value  */
	if ( ch >= UART_CH_MAX)
	{
		return BSP_ERR_INVALID_PARAMETER;
	}

	if (uart[ch].op_mode == UART_DMA_MODE)
	{
		return uart_dma_write(ch, buf, size);
	}
	else if (uart[ch].op_mode == UART_INTR_MODE)
	{
		return uart_intr_write(ch, buf, size);
	}
	else
	{
		for (i = 0; i < size; i++) {
			(void)uart_putc(ch, buf[i]);//Msg(4:3200) 'uart_rx' returns a value which is not being used. MISRA-C:2004 Rule 16.10
		}
	}

	return 0;
}

int32 uart_read(uint32 ch, uint8 *buf, uint32 size)
{
	uint32 i;

	/* QAC : Rule 21.18 Apparent: Dereference of an invalid pointer value  */
	if ( ch >= UART_CH_MAX)
	{
		return BSP_ERR_INVALID_PARAMETER;
	}

	if (uart[ch].op_mode == UART_DMA_MODE)
	{
		return uart_dma_read(ch, buf, size);
	}
	else if (uart[ch].op_mode == UART_INTR_MODE)
	{
		return uart_intr_read(ch, buf, size);
	}
	else
	{
		int8 getc_err;
		int32 getc_data =0;
		for (i = 0; i < size; i++) {
			getc_data = uart_getc(ch, 0, (int8 *)&getc_err); /*Code sonar : Null Pointer dereference */
			if ( getc_data >= 0)
			{
				buf[i] = (uint8)(getc_data&0xFF);
			}
			else
			{
				break;
			}
		}
	}
	return i;//QAC : 17.4 non-void return type
}

void uart_flush_tx(uint32 ch)
{
	/* QAC : Rule 21.18 Apparent: Dereference of an invalid pointer value  */
	if ( ch < UART_CH_MAX)
	{
		while((uart_reg_read(ch, OFFSET_FR) & FR_TXFE) == 0) {
			if ((uart_reg_read(ch, OFFSET_FR) & FR_TXFE) != 0) {
				break;
			}
		}
	}
}

void uart_flush_rx(uint32 ch)
{
	/* QAC : Rule 21.18 Apparent: Dereference of an invalid pointer value  */
	if ( ch < UART_CH_MAX)
	{
		while((uart_reg_read(ch, OFFSET_FR) & FR_RXFE) == 0) {
			volatile int8 c = (int8)(uart_reg_read(ch, OFFSET_DR));
			(void)c;
		}
	}
}

static void uart_enable_interrupts(uint32 ch)
{
	uint32 i;
	uint32 im;
	
	/* QAC : Rule 21.18 Apparent: Dereference of an invalid pointer value  */
	if ( ch < UART_CH_MAX)
	{
		switch (ch)
		{
			case UART_CH1:
				(void)BSP_IntVectSet(INT_UART0 + ch, 0x7, INT_TYPE_LEVEL_LOW, uart_isr, &(uart[ch]));
				break;
			case UART_CH2:
				(void)BSP_IntVectSet(INT_UART0 + ch, 0x7, INT_TYPE_LEVEL_LOW, uart_isr, &(uart[ch]));
				break;
			default:
				(void)BSP_IntVectSet(INT_UART0 + ch, PRIORITY_NO_MEAN, INT_TYPE_LEVEL_LOW, uart_isr, &(uart[ch]));
				break;
		}
		
		BSP_IntSrcEn(INT_UART0 + ch);

		uart_reg_write(ch, OFFSET_ICR, INT_RXIS | INT_TXIS | INT_RTIS);

		for (i = 0; i < RX_FIFO_SIZE * 2; ++i)
		{
			if ((uart_reg_read(ch, OFFSET_FR) & FR_RXFF) != 0) {
				break;
			}
			uart_reg_read(ch, OFFSET_DR);
		}

		im = INT_RTIS;
		if (uart[ch].rx_dma.is_run == 0)
		{
			im |= INT_RXIS;
		}

		uart_reg_write(ch, OFFSET_IMSC, im);
	}
}
static void uart_disable_interrupts(uint32 ch) {

	if ( ch < UART_CH_MAX)
	{
		(void)BSP_IntVectSet(INT_UART0 + ch, PRIORITY_NO_MEAN, INT_TYPE_LEVEL_LOW, NULL_PTR, NULL_PTR);
		BSP_IntSrcDis(INT_UART0 + ch);
		uart_reg_write(ch, OFFSET_ICR, 0x7FF);
	}


}

void uart_intr_tx_probe(uint32 ch)
{
	/* QAC : Rule 21.18 Apparent: Dereference of an invalid pointer value  */
	if ( ch < UART_CH_MAX)
	{
		uart[ch].tx_intr.xmit_buf = uart_buff[ch][UART_MODE_TX];
		uart[ch].tx_intr.size = UART_BUFF_SIZE;
		uart[ch].tx_intr.head = 0;
		uart[ch].tx_intr.tail = 0;
	}
}

void uart_intr_rx_probe(uint32 ch)
{
	/* QAC : Rule 21.18 Apparent: Dereference of an invalid pointer value  */
	if ( ch < UART_CH_MAX)
	{
		uart[ch].rx_intr.xmit_buf = uart_buff[ch][UART_MODE_RX];
		uart[ch].rx_intr.size = UART_BUFF_SIZE;
		uart[ch].rx_intr.head = 0;
		uart[ch].rx_intr.tail = 0;
	}
}

int32 uart_intr_write(uint32 ch, const uint8 *buf, uint32 size)
{
	uint32 i;
	uint32 imsc;
	/* QAC : Rule 21.18 Apparent: Dereference of an invalid pointer value  */
	if ( ch >= UART_CH_MAX)
	{
		return BSP_ERR_INVALID_PARAMETER;
	}

	// copy user buffer to tx buffer
	for(i = 0; i < size; i++) {
		uart[ch].tx_intr.xmit_buf[uart[ch].tx_intr.head] = buf[i];
		uart[ch].tx_intr.head++;
		if(uart[ch].tx_intr.head >= uart[ch].tx_intr.size) {
			uart[ch].tx_intr.head = 0;
		}
	}

	(void)uart_tx(ch);

	imsc = uart_reg_read(ch, OFFSET_IMSC);
	imsc |= INT_TXIS;
	uart_reg_write(ch, OFFSET_IMSC, imsc);

	return BSP_ERR_NONE;
}

int32 uart_intr_read(uint32 ch, uint8 *buf, uint32 size)
{
	uint32 cnt = 0;
	uint32 i;
	/* QAC : Rule 21.18 Apparent: Dereference of an invalid pointer value  */
	if ( ch >= UART_CH_MAX)
	{
		return BSP_ERR_INVALID_PARAMETER;
	}

	if(uart[ch].rx_intr.head > uart[ch].rx_intr.tail) {
		cnt = uart[ch].rx_intr.head - uart[ch].rx_intr.tail;
	}
	else if (uart[ch].rx_intr.head < uart[ch].rx_intr.tail) {
		cnt = uart[ch].rx_intr.size - uart[ch].rx_intr.tail;
	}
	else {
		return 0;
	}

	if (cnt > size) {
		cnt = size;
	}

	// copy rx buffer to user buffer
	for(i = 0; i < cnt; i++) {
		buf[i] = uart[ch].rx_intr.xmit_buf[uart[ch].rx_intr.tail];
		uart[ch].rx_intr.tail++;

		if(uart[ch].rx_intr.tail >= uart[ch].rx_intr.size) {
			uart[ch].rx_intr.tail = 0;
		}
	}

	return cnt;
}

void uart_dma_tx_probe(uint32 ch, uint32 *nc_addr)
{
	/* QAC : Rule 21.18 Apparent: Dereference of an invalid pointer value  */
	if ( ch < UART_CH_MAX)
	{
		uart[ch].tx_dma.con = ch;
		uart[ch].tx_dma.ch = UDMA_PERI_TX;
		uart[ch].tx_dma.src_addr = (int8 *)(nc_addr);
		uart[ch].tx_dma.dest_addr = (int8 *)(uart[ch].base);
		uart[ch].tx_dma.buff_size = UDMA_BUFF_SIZE;
		uart[ch].tx_dma.transfer_size = 0;

		(void)udma_init(&uart[ch].tx_dma, PRIORITY_NO_MEAN);

		// Enable Transmit DMA
		uart_reg_write(ch, OFFSET_DMACR, (uart_reg_read(ch, OFFSET_DMACR) | DMACR_DMAONERR | DMACR_TXDMAE));

		udma_set_flowcontrol(&uart[ch].tx_dma, UDMA_FLOW_TYPE_M2P);
		udma_set_inc(&uart[ch].tx_dma, UDMA_NO_INC, UDMA_INC);
		udma_set_burstsize(&uart[ch].tx_dma, UDMA_BURST_SIZE_1, UDMA_BURST_SIZE_1);

		udma_set_peri(&uart[ch].tx_dma, UDMA_PERI_TX, 0);

		(void)uart_dma_tx_enable(ch, uart[ch].tx_dma.buff_size, (const struct udma_data *)&uart[ch].tx_dma);
	}
}

void uart_dma_rx_probe(uint32 ch, uint32 *nc_addr)
{
	/* QAC : Rule 21.18 Apparent: Dereference of an invalid pointer value  */
	if ( ch < UART_CH_MAX)
	{

		uart[ch].rx_dma.con = ch;
		uart[ch].rx_dma.ch = UDMA_PERI_RX;
		uart[ch].rx_dma.src_addr = (int8 *)(uart[ch].base);
		uart[ch].rx_dma.dest_addr = (int8 *)(nc_addr);
		uart[ch].rx_dma.buff_size = UDMA_BUFF_SIZE;
		uart[ch].rx_dma.transfer_size = 0;

	(void)udma_init(&uart[ch].rx_dma, PRIORITY_NO_MEAN);//QAC return is not being used

	// Enable Receive DMA
	uart_reg_write(ch, OFFSET_DMACR, (uart_reg_read(ch, OFFSET_DMACR) | DMACR_DMAONERR | DMACR_RXDMAE));

	udma_set_flowcontrol(&uart[ch].rx_dma, UDMA_FLOW_TYPE_P2M);
	udma_set_inc(&uart[ch].rx_dma, UDMA_INC, UDMA_NO_INC);
	udma_set_burstsize(&uart[ch].rx_dma, UDMA_BURST_SIZE_1, UDMA_BURST_SIZE_1);

	udma_set_peri(&uart[ch].rx_dma, 0, UDMA_PERI_RX);

	(void)uart_dma_rx_enable(ch, uart[ch].rx_dma.buff_size, (const struct udma_data *)&uart[ch].rx_dma);//QAC return is not being used
	}
}

int32 uart_dma_tx_enable(uint32 ch, uint32 size, const struct udma_data *dmacon)
{
	/* QAC : Rule 21.18 Apparent: Dereference of an invalid pointer value  */
	if ( ch >= UART_CH_MAX)
	{
		return BSP_ERR_INVALID_PARAMETER;
	}

	uart[ch].tx_dma.src_addr = (int8 *)(dmacon->src_addr);
	uart[ch].tx_dma.dest_addr = (int8 *)(dmacon->dest_addr);
	udma_set_src_addr(&uart[ch].tx_dma, (uint32)(uart[ch].tx_dma.src_addr));
	udma_set_dest_addr(&uart[ch].tx_dma, (uint32)(uart[ch].tx_dma.dest_addr));
	udma_tcint_enable(&uart[ch].tx_dma);
	udma_set_transferwidth(&uart[ch].tx_dma, UDMA_TRANSFER_SIZE_BYTE, UDMA_TRANSFER_SIZE_BYTE);
	udma_set_transfersize(&uart[ch].tx_dma, size);


	return 0;
}

int32 uart_dma_rx_enable(uint32 ch, uint32 size, const struct udma_data *dmacon)
{
	/* QAC : Rule 21.18 Apparent: Dereference of an invalid pointer value  */
	if ( ch >= UART_CH_MAX)
	{
		return BSP_ERR_INVALID_PARAMETER;
	}

	uart[ch].rx_dma.src_addr = (int8 *)(dmacon->src_addr);
	uart[ch].rx_dma.dest_addr = (int8 *)(dmacon->dest_addr);
	udma_set_src_addr(&uart[ch].rx_dma, (uint32)(uart[ch].rx_dma.src_addr));
	udma_set_dest_addr(&uart[ch].rx_dma, (uint32)(uart[ch].rx_dma.dest_addr));
	udma_tcint_enable(&uart[ch].rx_dma);
	udma_set_transferwidth(&uart[ch].rx_dma, UDMA_TRANSFER_SIZE_BYTE, UDMA_TRANSFER_SIZE_BYTE);
	udma_set_transfersize(&uart[ch].rx_dma, size);

	// Run DMA
	udma_ch_enable(&uart[ch].rx_dma);

	return 0;
}

int32 uart_dma_write(uint32 ch, const uint8 *buf, uint32 size)
{
	uint32 i = 0;

	/* QAC : Rule 21.18 Apparent: Dereference of an invalid pointer value  */
	if ( ch >= UART_CH_MAX)
	{
		return BSP_ERR_INVALID_PARAMETER;
	}
	
	if (size > UDMA_BUFF_SIZE)
	{
		return -1;
	}

	for (i = 0; i < size; i++)
	{
		uart[ch].tx_dma.src_addr[i] = (int8)buf[i];//compile warning
	}

	udma_set_src_addr(&uart[ch].tx_dma, (uint32)(uart[ch].tx_dma.src_addr));
	udma_set_dest_addr(&uart[ch].tx_dma, (uint32)(uart[ch].tx_dma.dest_addr));
	udma_tcint_enable(&uart[ch].tx_dma);
	udma_set_transferwidth(&uart[ch].tx_dma, UDMA_TRANSFER_SIZE_BYTE, UDMA_TRANSFER_SIZE_BYTE);
	udma_set_transfersize(&uart[ch].tx_dma, size);

	// Run DMA
	udma_ch_enable(&uart[ch].tx_dma);

	return BSP_ERR_NONE;
}

int32 uart_dma_read(uint32 ch, uint8 *buf, uint32 size)
{
	uint32 cnt = 0, post_cnt = 0;
	uint32 i, num_brige=0, prev_buf=0;
	uint32 c_size = 0;
	uint32 u_size = size;

	/* QAC : Rule 21.18 Apparent: Dereference of an invalid pointer value  */
	if ( ch >= UART_CH_MAX)
	{
		return BSP_ERR_INVALID_PARAMETER;
	}

	c_size = udma_get_transfersize(&uart[ch].rx_dma);

	if (uart[ch].rx_dma.is_run != 0)
	{
		return 0;
	}
	

	if(uart[ch].rx_dma.transfer_size != c_size)
	{

		if(uart[ch].rx_dma.transfer_size > c_size)
		{

			cnt = uart[ch].rx_dma.transfer_size - c_size;
			prev_buf=uart[ch].rx_dma.buff_size-uart[ch].rx_dma.transfer_size;

			if(u_size >= cnt) 
			{
				for (i = 0; i < cnt ; i++)
				{
					buf[i] = (uint8)(uart[ch].rx_dma.dest_addr[prev_buf+i]);
				}
				uart[ch].rx_dma.transfer_size = c_size;
			} 
			else //else if(u_size < cnt) //QAC : Always true
			{
				for (i = 0; i < u_size ; i++)
				{
					buf[i] = (uint8)(uart[ch].rx_dma.dest_addr[prev_buf+i]);
				}
				uart[ch].rx_dma.transfer_size -=  u_size;
				cnt = u_size;
			}

			return cnt;

		}
		else if(c_size > uart[ch].rx_dma.transfer_size)
		{

			prev_buf = uart[ch].rx_dma.buff_size - uart[ch].rx_dma.transfer_size;
			cnt = uart[ch].rx_dma.buff_size - c_size;
			post_cnt = uart[ch].rx_dma.transfer_size;

			if(u_size > post_cnt)
			{

				u_size = u_size - post_cnt;
				for( i= 0 ; i < uart[ch].rx_dma.transfer_size ; i++)
				{
					buf[i] = (uint8)(uart[ch].rx_dma.dest_addr[prev_buf + i]);
					num_brige=i+1;
				}
				if(u_size >= cnt) 
				{
					for( i = 0 ; i < cnt; i++){
						buf[num_brige+i] = (uint8)(uart[ch].rx_dma.dest_addr[i]);
					}
					uart[ch].rx_dma.transfer_size = c_size;
					cnt += post_cnt;
				} 
				else //else if(u_size < cnt) : QAC : Always true
				{
					for( i = 0 ; i < u_size; i++)
					{
						buf[num_brige+i] = (uint8)(uart[ch].rx_dma.dest_addr[i]);
					}
					uart[ch].rx_dma.transfer_size =  uart[ch].rx_dma.buff_size - u_size;
					cnt = u_size + post_cnt;
				}
			} 
			else 
			{
				for( i= 0 ; i < u_size ; i++)
				{
					buf[i] = (uint8)(uart[ch].rx_dma.dest_addr[prev_buf + i]);
					num_brige=i+1;
				}
				uart[ch].rx_dma.transfer_size -= u_size;
				cnt = u_size;
			}
			return cnt;
		}
		else
		{
			;// empty statement
		}
	}

	return cnt;
}

static int32 uart_dma_rx_trigger_dma(uint32 ch)
{
	uint32 dmacr;
	uint32 im;
	/* QAC : Rule 21.18 Apparent: Dereference of an invalid pointer value  */
	if ( ch >= UART_CH_MAX)
	{
		return BSP_ERR_INVALID_PARAMETER;
	}

	dmacr = uart_reg_read(ch, OFFSET_DMACR);
	dmacr |= DMACR_RXDMAE;
	uart_reg_write(ch, OFFSET_DMACR, dmacr);

	uart[ch].rx_dma.is_run = 1;

	im = uart_reg_read(ch, OFFSET_IMSC);
	im |= ~INT_RXIS;
	uart_reg_write(ch, OFFSET_IMSC, im);

	return 0;
}

static void uart_dma_rx_irq(uint32 ch)//GHC Warning : external/internal linkage conflict
{
	uint32 dmacr;
	/* QAC : Rule 21.18 Apparent: Dereference of an invalid pointer value  */
	if ( ch < UART_CH_MAX)
	{

		dmacr = uart_reg_read(ch, OFFSET_DMACR);
		dmacr &= ~DMACR_RXDMAE;
		uart_reg_write(ch, OFFSET_DMACR, dmacr);

		uart[ch].rx_dma.is_run = 0;

		uart_reg_write(ch, OFFSET_ICR, INT_OEIS | INT_BEIS | INT_PEIS | INT_FEIS);

		(void)uart_dma_rx_trigger_dma(ch);

		uart[ch].rx_dma.is_run = 0;
	}
}
