/****************************************************************************
 *   FileName    : spi.h
 *   Description : Telechips TCC803x SPI driver header
 ****************************************************************************
 *
 *   TCC Version 1.01
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

#ifndef _TCC_SPI_H_
#define _TCC_SPI_H_

#include <bsp.h>

#ifndef ENABLE
#define ENABLE 1
#endif
#ifndef DISABLE
#define DISABLE 0
#endif

/* SPI Debug message control */
//#define SPI_DEBUG
#ifdef SPI_DEBUG
#define SPI_DBG(fmt, args...) printf("[ GPSB ] " fmt, ## args)
#else
#define SPI_DBG(fmt, args...) do {} while(0)
#endif

#ifndef __GNU_C__
#define __func__ __FUNCTION__
#endif

/* GPSB register address */
#define GPSB0_BASE       (TCC_GPSB_BASE + 0x00000)
#define GPSB1_BASE       (TCC_GPSB_BASE + 0x10000)
#define GPSB2_BASE       (TCC_GPSB_BASE + 0x20000)
#define GPSB_PCFG_BASE   (TCC_GPSB_BASE + 0x30000)
#define GPSB_PORT_CFG(x) (GPSB_PCFG_BASE + (0x4 * (x)))
#define GPSB3_BASE       (TCC_GPSB_BASE + 0x40000)

/* GPSB register */
#define GPSB_PORT        0x00 /* Data port */
#define GPSB_STAT        0x04 /* Status register */
#define GPSB_INTEN       0x08 /* Interrupt enable */
#define GPSB_MODE        0x0C /* Mode register */
#define GPSB_CTRL        0x10 /* Control register */
#define GPSB_EVTCTRL     0x14 /* Counter and Ext. Event Control */
#define GPSB_CCV         0x18 /* Counter current value */
#define GPSB_TXBASE      0x20 /* TX base address register */
#define GPSB_RXBASE      0x24 /* RX base address register */
#define GPSB_PACKET      0x28 /* Packet register */
#define GPSB_DMACTR      0x2C /* DMA control register */
#define GPSB_DMASTR      0x30 /* DMA status register */
#define GPSB_DMAICR      0x34 /* DMA interrupt control register */

#define GPSB_CH_NUM      4
#define GPSB_PORT_NUM    33

enum {
	GPSB_CH_0 = 0,
	GPSB_CH_1,
	GPSB_CH_2,
	GPSB_CH_3,
};

/* GPSB Tx/Rx FIFO Threshold for irq/dma req. */
#define GPSB_CFGWTH     0
#define GPSB_CFGRTH     0

/* Default clock divider load value */
#define GPSB_DEF_DIVLDV 0

/* SPI master transfer mode */
#define GPSB_XFER_PIO   0x0001 /* PIO mode */
#define GPSB_XFER_DMA   0x0002 /* DMA mode*/

/* SPI clocking mode */
#define SPI_CPHA        0x01
#define SPI_CPOL        0x02

#define SPI_MODE_0      (0|0)
#define SPI_MODE_1      (0|SPI_CPHA)
#define SPI_MODE_2      (SPI_CPOL|0)
#define SPI_MODE_3      (SPI_CPOL|SPI_CPHA)

#define SPI_CS_HIGH     0x04
#define SPI_LSB_FIRST   0x08
#define SPI_LOOP        0x10

/* Event for compelete */
#define GPSB_EVENT_COMPLETE               0x0000 /* Transfer complete */
#define GPSB_EVENT_ERR_ROR                0x0100 /* Read FIFO over-run error */
#define GPSB_EVENT_ERR_WUR                0x0200 /* Write FIFO under-run error */
#define GPSB_EVENT_ERR_RUR                0x0400 /* Read FIFO under-run error */
#define GPSB_EVENT_ERR_WOR                0x080  /* Write FIFO over-run error */

/* State of GPSB transfer */
#define GPSB_XFER_MODE_WITHOUT_INTERRUPT  0x01 /* w/o interrupt mode */
#define GPSB_XFER_MODE_WITH_INTERRUPT     0x02 /* w/ interrupt mode */
#define GPSB_XFER_MODE_DMA                0x10 /* DMA mode */
#define GPSB_XFER_MODE_PIO                0x20 /* Non-DMA mode */
#define GPSB_XFER_MODE_WITHOUT_CTF        0x40 /* w/o GPSB CTF mode */

#define GPSB_XFER_MODE_WITH_INTR_WITH_DMA (GPSB_XFER_MODE_WITH_INTERRUPT | GPSB_XFER_MODE_DMA)
#define GPSB_XFER_MODE_WITH_INTR_WITH_PIO (GPSB_XFER_MODE_WITH_INTERRUPT | GPSB_XFER_MODE_PIO)

#define GPSB_XFER_STATE_DISABLED          0x0000 /* GPSB is dieabled */
#define GPSB_XFER_STATE_IDLE              0x0001 /* GPSB is in idle state */
#define GPSB_XFER_STATE_RUNNING           0x0002 /* GPSB is in blocking transfer */
#define GPSB_XFER_STATE_MASK              (GPSB_XFER_STATE_DISABLED | GPSB_XFER_STATE_IDLE | GPSB_XFER_STATE_RUNNING)

#define GPSB_DEF_INT_PRIORITY             PRIORITY_NO_MEAN

/* Get Physical address */
#define GPSB_GET_PHY_ADDR(v_addr)         ((uint32)(v_addr))
/* Get Non-cacheable memory address */
//#define GPSB_DMA_BUF_ADDR(x)              ((uint32)((x) + 0x100000))
#define GPSB_DMA_BUF_ADDR(x)              ((uint32)(x))

#define GPSB_POLL_TIMEOUT                 1000000

#define tcc_gpsb_bit_clr(addr, mask) do{\
	uint32 val;\
	val = readl((addr));\
	BSP_BIT_CLR_32(val, (mask));\
	writel(val, (addr));\
}while(0)

#define tcc_gpsb_bit_set(addr, mask) do{\
	uint32 val;\
	val = readl((addr));\
	BSP_BIT_CLR_32(val, (mask));\
	BSP_BIT_SET_32(val, (mask));\
	writel(val, (addr));\
}while(0)

#define tcc_gpsb_bit_cset(addr, cmask, smask) do{\
	uint32 val;\
	val = readl((addr));\
	BSP_BIT_CLR_32(val, (cmask));\
	BSP_BIT_SET_32(val, (smask));\
	writel(val, (addr));\
}while(0)

#define tcc_gpsb_bit_sclr(addr, smask, cmask) do{\
	uint32 val;\
	val = readl((addr));\
	BSP_BIT_SET_32(val, (smask));\
	BSP_BIT_CLR_32(val, (cmask));\
	writel(val, (addr));\
}while(0)

typedef void (*tcc_spi_handler_t)(uint8 ch, int32 event, void *arg);

struct tcc_spi_transfer_complete {
	tcc_spi_handler_t cb;
	void *arg;
};

struct tcc_gpsb_dma_buf {
	void *vaddr;
	void *paddr;
	uint32 size;
};

struct tcc_gpsb {
	/* GPSB channel */
	uint8 gpsb_channel;
	/* GPSB base address */
	uint32 base;
	/* Peri. clock number */
	uint32 peri_name;
	/* IOBUS number */
	uint32 iobus_name;

	/* GPSB mode (Master or Slave) (reserved) */
	uint8 is_slave;

	/* GPIO information  */
	int32 gpsb_sdo;
	int32 gpsb_sdi;
	int32 gpsb_sclk;
	int32 gpsb_port;

	/* Interrupt priority */
	uint32 interrupt_priority;

	uint8 support_dma;
	struct tcc_gpsb_dma_buf tx_dma;
	struct tcc_gpsb_dma_buf rx_dma;

	const void *async_tx_buf;
	void *async_rx_buf;
	uint32 async_tx_data_size;
	uint32 async_rx_data_size;

	/* State of GPSB */
	uint32 state;

	/* Notify the completion of non-blocking transfer */
	struct tcc_spi_transfer_complete complete;
};

extern int32 tcc_spi_master_set_speed(uint8 ch, uint32 speed_in_hz);
extern int32 tcc_spi_master_set_bpw(uint8 ch, uint8 bpw);
extern int32 tcc_spi_master_set_bpw_noswap(uint8 ch, uint8 bpw);
extern int32 tcc_spi_master_set_mode(uint8 ch, uint32 mode);
extern int32 tcc_spi_master_alloc_cs(uint8 ch, int32 cs_gpio);
extern int32 tcc_spi_master_dealloc_cs(uint8 ch);
extern int32 tcc_spi_master_init_cs(uint8 ch, int32 cs_gpio, uint8 cs_high);
extern int32 tcc_spi_master_activate_cs(uint8 ch, int32 cs_gpio, uint8 cs_high);
extern int32 tcc_spi_master_deactivate_cs(uint8 ch, int32 cs_gpio, uint8 cs_high);
extern int32 tcc_spi_master_xfer(uint8 ch, const void *tx, void *rx, uint32 data_size, uint8 xfer_mode);
extern void tcc_spi_master_abort_xfer(uint8 ch);
extern int32 tcc_spi_master_open(uint8 ch, int32 gpio_sdo, int32 gpio_sdi, int32 gpio_sclk,
		void *dma_addr_tx, void *dma_addr_rx, uint32 dma_buf_size, tcc_spi_handler_t cb, void *arg);
extern int32 tcc_spi_master_close(uint8 ch);
extern int32 init_spi(void);
extern int32 deinit_spi(void);
#endif
