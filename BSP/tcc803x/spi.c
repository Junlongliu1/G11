/****************************************************************************
 *   FileName    : spi.c
 *   Description : Telechips TCC803x SPI driver using GPSB
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
#include <types.h>
#include <osal.h>
#include <clock.h>
#include <bsp_int.h>
#include <bsp.h>
#include "reg_phys.h"
#include "tcc_ckc.h"
#include "debug.h"
#include "gpio.h"
#include "spi.h"
#include "device_share_config.h"

static struct tcc_gpsb tccgpsb[GPSB_CH_NUM] = {
	{
		0,                     /* GPSB channel */
		GPSB0_BASE,            /* GPSB base address */
		PERI_GPSB0,            /* Peri. clock number */
		IOBUS_GPSB0,           /* IOBUS number */
		BSP_TRUE,              /* GPSB mode (Master or Slave) */
		/* GPIO information  */
		-1,                    /* SDO */
		-1,                    /* SDI */
		-1,                    /* SCLK */
		-1,                    /* PORT */
		GPSB_DEF_INT_PRIORITY, /* Interrupt priority */
		BSP_FALSE,             /* support dma */
		{0, },
		{0, },
		(void *)0,             /* async pio tx buffer address */
		(void *)0,             /* async pio rx buffer address */
		0,                     /* async pio tx buffer size */
		0,                     /* async pio rx buffer size */
		0,
		{0, },
	},
	{
		1,                     /* GPSB channel */
		GPSB1_BASE,            /* GPSB base address */
		PERI_GPSB1,            /* Peri.clock nmumber */
		IOBUS_GPSB1,           /* IOBUS number */
		BSP_TRUE,              /* GPSB mode (Master or Slave) */
		/* GPIO information  */
		-1,                    /* SDO */
		-1,                    /* SDI */
		-1,                    /* SCLK */
		-1,                    /* PORT */
		GPSB_DEF_INT_PRIORITY, /* Interrupt priority */
		BSP_FALSE,
		{0, },
		{0, },
		(void *)0,             /* async pio tx buffer address */
		(void *)0,             /* async pio rx buffer address */
		0,                     /* async pio tx buffer size */
		0,                     /* async pio rx buffer size */
		0,
		{0, },
	},
	{
		2,
		GPSB2_BASE,            /* GPSB channel */
		PERI_GPSB2,            /* GPSB base address */
		IOBUS_GPSB2,           /* Peri.clock number */
		BSP_TRUE,              /* GPSB mode (Master or Slave) */
		/* GPIO information  */
		-1,                    /* SDO */
		-1,                    /* SDI */
		-1,                    /* SCLK */
		-1,                    /* PORT */
		GPSB_DEF_INT_PRIORITY, /* Interrupt priority */
		BSP_FALSE,
		{0, },
		{0, },
		(void *)0,             /* async pio tx buffer address */
		(void *)0,             /* async pio rx buffer address */
		0,                     /* async pio tx buffer size */
		0,                     /* async pio rx buffer size */
		0,
		{0, },
	},
	{
		3,
		GPSB3_BASE,            /* GPSB channel */
		PERI_GPSB3,              /* GPSB base address */
		IOBUS_GPSB3,           /* Peri.clock number */
		BSP_TRUE,              /* GPSB mode (Master or Slave) */
		/* GPIO information  */
		-1,                    /* SDO */
		-1,                    /* SDI */
		-1,                    /* SCLK */
		-1,                    /* PORT */
		GPSB_DEF_INT_PRIORITY, /* Interrupt priority */
		BSP_FALSE,
		{0, },
		{0, },
		(void *)0,             /* async pio tx buffer address */
		(void *)0,             /* async pio rx buffer address */
		0,                     /* async pio tx buffer size */
		0,                     /* async pio rx buffer size */
		0,
		{0, },
	},
};

#define GPSB_SCLK 0
#define GPSB_CS   1
#define GPSB_SDO  2
#define GPSB_SDI  3
#define DEDICATED_DMA_NUM 4
#define CH_SDM 3

#define tcc_gpsb_is_master(ch) ((uint32)((tccgpsb[ch].is_slave == BSP_FALSE) ? 1 : 0))

/* Enable GPSB operation */
#define tcc_gpsb_enable(ch) tcc_gpsb_bit_set(tccgpsb[(ch)].base + GPSB_MODE, BSP_BIT_03)

/* Disable GPSB operation */
#define tcc_gpsb_disable(ch) tcc_gpsb_bit_clr(tccgpsb[(ch)].base + GPSB_MODE, BSP_BIT_03)

/* Enable GPSB Interrupt */
#define tcc_gpsb_enable_interrupt(ch) tcc_gpsb_bit_set(tccgpsb[(ch)].base + GPSB_INTEN, BSP_BIT_00)

/* Disable GPSB Interrupt */
#define tcc_gpsb_disable_interrupt(ch) tcc_gpsb_bit_clr(tccgpsb[ch].base + GPSB_INTEN, BSP_BIT_00)

/* Get Tx FIFO count */
#define tcc_gpsb_get_tx_fifo_cnt(ch) ((uint8)((readl(tccgpsb[ch].base + GPSB_STAT) >> 24) & 0xF))

/* Get Rx FIFO count */
#define tcc_gpsb_get_rx_fifo_cnt(ch) ((uint8)((readl(tccgpsb[ch].base + GPSB_STAT) >> 16) & 0xF))

static void tcc_gpsb_start_dma(uint8 ch);
static void tcc_gpsb_stop_dma(uint8 ch);
static int32 tcc_gpsb_set_dma_addr(uint8 ch, uint32 tx, uint32 rx);//static int32 tcc_gpsb_set_dma_addr(uint8 ch, const uint32 tx, uint32 rx); //GHC Warning
static int32 tcc_gpsb_reset(uint8 ch);
static int32 tcc_gpsb_set_port(uint8 ch, int32 gpio_sdo, int32 gpio_sdi, int32 gpio_sclk);
static void tcc_gpsb_set_transfer_state(uint8 ch, uint32 state);
static uint32 tcc_gpsb_write_fifo(uint8 ch, const uint8 *tx_buf, uint32 tx_buf_size);
static uint32 tcc_gpsb_read_fifo(uint8 ch, uint8 *rx_buf, uint32 rx_buf_size);
static uint8 tcc_spi_master_get_bpw(uint8 ch);
static int32 tcc_spi_master_set_cs(int32 cs_gpio, uint8 enable, uint8 cs_high);
static int32 tcc_spi_master_pio_wait_rx_fifo_valid(uint32 ch);
static int32 tcc_spi_master_sync_pio_txrx_8(uint8 ch, const void *_tx, void *_rx, uint32 data_size);
static int32 tcc_spi_master_sync_pio_txrx_16(uint8 ch, const void *_tx, void *_rx, uint32 data_size);
static int32 tcc_spi_master_sync_pio_txrx_32(uint8 ch, const void *_tx, void *_rx, uint32 data_size);
static int32 tcc_spi_master_async_pio_txrx(uint8 ch, const void *tx, void *rx, uint32 data_size);
static int32 tcc_spi_master_async_dma_txrx(uint8 ch, const void *tx, void *rx, uint32 data_size);
static void tcc_gpsb_hwinit_for_spi_master(uint8 ch);
static void tcc_gpsb_isr(uint8 ch);
static void tcc_gpsb0_isr(void *arg);
static void tcc_gpsb1_isr(void *arg);
static void tcc_gpsb2_isr(void *arg);
static void tcc_gpsb3_isr(void *arg);
static void tcc_gpsb0_dma_isr(void *arg);
static void tcc_gpsb1_dma_isr(void *arg);
static void tcc_gpsb2_dma_isr(void *arg);
static void tcc_gpsb3_dma_isr(void *arg);

#ifdef SPI_DEBUG
static void tcc_gpsb_reg_dump(uint32 ch);
static void tcc_gpsb_reg_dump(uint32 ch)
{
	SPI_DBG("##### SPI REG DUMP CH %d (@0x%X)#####\n", ch, tccgpsb[ch].base);
	SPI_DBG("STAT    0x%08X\n", readl(tccgpsb[ch].base + GPSB_STAT));
	SPI_DBG("INTEN   0x%08X\n", readl(tccgpsb[ch].base + GPSB_INTEN));
	SPI_DBG("MODE    0x%08X\n", readl(tccgpsb[ch].base + GPSB_MODE));
	SPI_DBG("CTRL    0x%08X\n", readl(tccgpsb[ch].base + GPSB_CTRL));
	if(ch < DEDICATED_DMA_NUM) {
		SPI_DBG("TXBASE  0x%08X\n", readl(tccgpsb[ch].base + GPSB_TXBASE));
		SPI_DBG("RXBASE  0x%08X\n", readl(tccgpsb[ch].base + GPSB_RXBASE));
		SPI_DBG("PACKET  0x%08X\n", readl(tccgpsb[ch].base + GPSB_PACKET));
		SPI_DBG("DMACTR  0x%08X\n", readl(tccgpsb[ch].base + GPSB_DMACTR));
		SPI_DBG("DMASTAT 0x%08X\n", readl(tccgpsb[ch].base + GPSB_DMASTR));
		SPI_DBG("DMAICR  0x%08X\n", readl(tccgpsb[ch].base + GPSB_DMAICR));
	}
	SPI_DBG("PCFG    0x%08X\n", readl(GPSB_PORT_CFG(0)));
}
#endif

/* Now we support dma operation for channel 0 to 2 only */
/* Start dma operation */
static void tcc_gpsb_start_dma(uint8 ch)
{
	/* Setup GPSB DMA */
	if(ch < DEDICATED_DMA_NUM) {
		/* Set GPSB DMA Address mode - Multiple address mode */
		//tcc_gpsb_bit_clr(tccgpsb[ch].base + GPSB_DMACTR, (0x3 << 14) | (0x3 << 16));

		/* Enable DMA request for TX/RX FIFO */
		tcc_gpsb_bit_set(tccgpsb[ch].base + GPSB_INTEN, BSP_BIT_31 | BSP_BIT_30);

		/* Enable DMA Tx and Rx request */
		tcc_gpsb_bit_set(tccgpsb[ch].base + GPSB_DMACTR, BSP_BIT_31 | BSP_BIT_30);

		/* Enbale DMA done interrupt and Disable DMA packet interrupt */
		tcc_gpsb_bit_sclr(tccgpsb[ch].base + GPSB_DMAICR, BSP_BIT_17, BSP_BIT_16);

		/* Set DMA Rx interrupt */
		tcc_gpsb_bit_clr(tccgpsb[ch].base + GPSB_DMAICR, BSP_BIT_20);

		/* Enable GPSB DMA operation */
		tcc_gpsb_bit_set(tccgpsb[ch].base + GPSB_DMACTR, BSP_BIT_00);
	}
}

/* Stop dma operation and clear interrupt status */
static void tcc_gpsb_stop_dma(uint8 ch)
{
	if(ch < DEDICATED_DMA_NUM) {
		/* Disable DMA request for TX/RX FIFO */
		tcc_gpsb_bit_clr(tccgpsb[ch].base + GPSB_INTEN, BSP_BIT_31 | BSP_BIT_30);

		/* Disable DMA Tx and Rx request */
		tcc_gpsb_bit_clr(tccgpsb[ch].base + GPSB_DMACTR, BSP_BIT_31 | BSP_BIT_30);

		/* Clear DMA done and packet interrupt status */
		tcc_gpsb_bit_set(tccgpsb[ch].base + GPSB_DMAICR, BSP_BIT_29 | BSP_BIT_28);

		/* Disable GPSB DMA operation */
		tcc_gpsb_bit_clr(tccgpsb[ch].base + GPSB_DMACTR, BSP_BIT_00);
	} else {
		/* Nothing to do */
	}
}

static int32 tcc_gpsb_set_dma_addr(uint8 ch, uint32 tx, uint32 rx)
{
	static uint32 dma_tx_dummy = 0x0;
	static uint32 dma_rx_dummy = 0x0;
	uint32 _tx, _rx;
	if(ch >= DEDICATED_DMA_NUM) {
		SPI_DBG("%s ch %d is not support dma operation\n", __func__, ch);
		return OSAL_ERR_INVALID_PARAMETER;
	}

	/* Set TXBASE and RXBASE */
	if(tx != BSP_NULL) {
		/* Set multiple packet address mode */
		_tx = GPSB_GET_PHY_ADDR(tx);
		tcc_gpsb_bit_clr(tccgpsb[ch].base + GPSB_DMACTR, BSP_BIT_17 | BSP_BIT_16);
	}
	else {
		/* Set fixed address mode */
		_tx = GPSB_GET_PHY_ADDR(&dma_tx_dummy);
		tcc_gpsb_bit_cset(tccgpsb[ch].base + GPSB_DMACTR, BSP_BIT_17 | BSP_BIT_16, BSP_BIT_16);
	}
	writel((_tx & 0xFFFFFFFF), tccgpsb[ch].base + GPSB_TXBASE);

	if(rx != BSP_NULL) {
		/* Set multiple packet address mode */
		_rx = GPSB_GET_PHY_ADDR(rx);
		tcc_gpsb_bit_clr(tccgpsb[ch].base + GPSB_DMACTR, BSP_BIT_15 | BSP_BIT_14);
	}
	else {
		/* Set fixed address mode */
		_rx = GPSB_GET_PHY_ADDR(&dma_rx_dummy);
		tcc_gpsb_bit_cset(tccgpsb[ch].base + GPSB_DMACTR, BSP_BIT_15 | BSP_BIT_14, BSP_BIT_14);
	}
	writel((_rx & 0xFFFFFFFF), tccgpsb[ch].base + GPSB_RXBASE);

	/* Set Tx and Rx FIFO threshold for interrupt/DMA request */
	tcc_gpsb_bit_cset(tccgpsb[ch].base + GPSB_INTEN, (0x7 << 16), ((GPSB_CFGRTH & 0x7) << 16));
	tcc_gpsb_bit_cset(tccgpsb[ch].base + GPSB_INTEN, (0x7 << 20), ((GPSB_CFGWTH & 0x7) << 20));

	return OSAL_ERR_NONE;
}

/* Reset GPSB block */
static int32 tcc_gpsb_reset(uint8 ch)
{
	int32 ret;
	/* SW reset */
	ret = tcc_set_iobus_swreset(tccgpsb[ch].iobus_name, 1);
	if(ret != 0) {
		return ret;
	}

	ret = tcc_set_iobus_swreset(tccgpsb[ch].iobus_name, 0);
	if(ret != 0) {
		return ret;
	}

	return OSAL_ERR_NONE;
}

/*
 ************************************************************************************************************************
 * Description: Configure GPSB port and gpio settings (SDI, SDO, SCLK)
 *              SFRM will be configured before start transfer
 *
 * Arguments  : ch                is GPSB channel
 *
 * Returns    : int32
 ************************************************************************************************************************
 */
static int32 tcc_gpsb_set_port(uint8 ch, int32 gpio_sdo, int32 gpio_sdi, int32 gpio_sclk)
{
	int32 tccgpsbport[GPSB_PORT_NUM][4] = {
		{NULL, NULL, NULL, NULL},
		{NULL, NULL, NULL, NULL},
		{NULL, NULL, NULL, NULL},
		{NULL, NULL, NULL, NULL},
		{NULL, NULL, NULL, NULL},
		{TCC_GPA(0)  , TCC_GPA(1)  , TCC_GPA(2)  , TCC_GPA(3)},
		{TCC_GPA(6)  , TCC_GPA(7)  , TCC_GPA(8)  , TCC_GPA(9)},
		{TCC_GPA(12) , TCC_GPA(13) , TCC_GPA(14) , TCC_GPA(15)},
		{TCC_GPA(18) , TCC_GPA(19) , TCC_GPA(20) , TCC_GPA(21)},
		{TCC_GPA(24) , TCC_GPA(25) , TCC_GPA(26) , TCC_GPA(27)},
		{TCC_GPB(0)  , TCC_GPB(1)  , TCC_GPB(2)  , TCC_GPB(3)},
		{TCC_GPB(6)  , TCC_GPB(7)  , TCC_GPB(8)  , TCC_GPB(9)},
		{TCC_GPB(12) , TCC_GPB(13) , TCC_GPB(14) , TCC_GPB(15)},
		{TCC_GPB(19) , TCC_GPB(20) , TCC_GPB(21) , TCC_GPB(22)},
		{TCC_GPB(25) , TCC_GPB(26) , TCC_GPB(27) , TCC_GPB(28)},
		{TCC_GPC(0)  , TCC_GPC(1)  , TCC_GPC(2)  , TCC_GPC(3)},
		{TCC_GPC(4)  , TCC_GPC(5)  , TCC_GPC(6)  , TCC_GPC(7)},
		{TCC_GPC(12) , TCC_GPC(13) , TCC_GPC(14) , TCC_GPC(15)},
		{TCC_GPC(16) , TCC_GPC(17) , TCC_GPC(18) , TCC_GPC(19)},
		{TCC_GPC(20) , TCC_GPC(21) , TCC_GPC(22) , TCC_GPC(23)},
		{TCC_GPC(26) , TCC_GPC(27) , TCC_GPC(28) , TCC_GPC(29)},
		{TCC_GPG(0)  , TCC_GPG(1)  , TCC_GPG(2)  , TCC_GPG(3)},
		{TCC_GPG(7)  , TCC_GPG(8)  , TCC_GPG(9)  , TCC_GPG(10)},
		{TCC_GPE(5)  , TCC_GPE(6)  , TCC_GPE(7)  , TCC_GPE(8)},
		{TCC_GPE(11) , TCC_GPE(12) , TCC_GPE(13) , TCC_GPE(14)},
		{TCC_GPE(16) , TCC_GPE(17) , TCC_GPE(18) , TCC_GPE(19)},
		{TCC_GPH(4)  , TCC_GPH(5)  , TCC_GPH(6)  , TCC_GPH(7)},
		{TCC_GPMA(0) , TCC_GPMA(1) , TCC_GPMA(2) , TCC_GPMA(3)},
		{TCC_GPMA(6) , TCC_GPMA(7) , TCC_GPMA(8) , TCC_GPMA(9)},
		{TCC_GPMA(12), TCC_GPMA(13), TCC_GPMA(14), TCC_GPMA(15)},
		{TCC_GPMA(18), TCC_GPMA(19), TCC_GPMA(20), TCC_GPMA(21)},
		{TCC_GPMA(24), TCC_GPMA(25), TCC_GPMA(26), TCC_GPMA(27)},
		{TCC_GPH(0)  , TCC_GPH(1)  , TCC_GPH(2)  , TCC_GPH(3)}
	};
	int32 ret        = 0;
	int32 i;
	uint8 match_port;
	int32 port       = 0;

	/* Set GPIO configuration */
	if(((gpio_sdo < 0) || (gpio_sdi < 0)) || (gpio_sclk < 0)) {
		SPI_DBG("%s: gpio is invalid (sdo %d sdi %d sclk %d)\n",
				__func__, gpio_sdo, gpio_sdi, gpio_sclk);
		return OSAL_ERR_INVALID_PARAMETER;
	}

	match_port = 0;
	for (i = 0; i < GPSB_PORT_NUM; i++) {
		if (gpio_sclk == tccgpsbport[i][GPSB_SCLK]) {
			if (gpio_sdo == tccgpsbport[i][GPSB_SDO]) {
				if (gpio_sdi == tccgpsbport[i][GPSB_SDI]) {
					match_port = 1;
					port = i;
				}
			}
		}
	}
	if (match_port == 1) {
		tccgpsb[ch].gpsb_port = port;
		SPI_DBG("%s: spi port %d\n", __func__, tccgpsb[ch].gpsb_port);
	} else {
		SPI_DBG("%s: spi gpsb port is wrong\n", __func__);
		return OSAL_ERR_INVALID_PARAMETER;
	}

	for (i = 0; i < GPSB_CH_NUM; i++) {
		if ((i != ch) && (tccgpsb[i].gpsb_port == port)) {
			SPI_DBG("%s: %d port is already used by ch%d\n", __func__, port, i);
			return OSAL_ERR_INVALID_PARAMETER;
		}
	}

	ret = gpio_config(gpio_sdo, GPIO_FUNC(6) | GPIO_OUTPUT);
	if(ret != 0) {
		return OSAL_ERR_INVALID_PARAMETER;
	}

	ret = gpio_config(gpio_sdi, GPIO_FUNC(6) | GPIO_INPUT | GPIO_INPUTBUF_EN);

	if(ret != 0) {
		return OSAL_ERR_INVALID_PARAMETER;
	}

	ret = gpio_config(gpio_sclk, GPIO_FUNC(6) | GPIO_OUTPUT);
	if(ret != 0) {
		return OSAL_ERR_INVALID_PARAMETER;
	}

	/* Set port configuration */
	tcc_gpsb_bit_cset(GPSB_PORT_CFG(0), (0xFF << (ch * 8)), (tccgpsb[ch].gpsb_port << (ch * 8)));
	SPI_DBG("%s: PCFG 0x%08X\n", __func__, readl(GPSB_PORT_CFG(0)));

	return OSAL_ERR_NONE;
}

/*
 ************************************************************************************************************************
 * Description: Set GPSB transfer state
 *
 * Arguments  : ch                is GPSB channel
 *
 *              state             is GPSB transfer state:
 *
 *                                 GPSB_XFER_STATE_DISABLED           disable the gpsb transfer
 *                                 GPSB_XFER_STATE_IDLE               is able to transfer data
 *                                 GPSB_XFER_STATE_RUNNING            is transferring data
 *
 * Returns    :
 ************************************************************************************************************************
 */
static void tcc_gpsb_set_transfer_state(uint8 ch, uint32 state)
{
	OSAL_SR_ALLOC();

	switch(state) {
		case GPSB_XFER_STATE_DISABLED:
		case GPSB_XFER_STATE_IDLE:
		case GPSB_XFER_STATE_RUNNING:
			SPI_DBG("%s: ch %d xfer state is set to 0x%x\n", __func__, ch, state);
			OSAL_CRITICAL_ENTER();
			tccgpsb[ch].state = state;
			OSAL_CRITICAL_EXIT();
			break;
		default:
			break;
	}
}

static uint32 tcc_gpsb_write_fifo(uint8 ch, const uint8 *tx_buf, uint32 tx_buf_size)
{
	uint8 i, word_size;
	const uint8 *_tx;
	uint32 cnt, write_data, ret;


	word_size = (uint8)((readl(tccgpsb[ch].base + GPSB_MODE) >> 8) & 0x1F) + 1;
	word_size = word_size / 8;

	/* Get empty fifo count */
	cnt = (readl(tccgpsb[ch].base + GPSB_STAT) >> 24) & 0xF;

	if(word_size > 2) {
		/*
		 * With BPW=32 and higher speed, ROR error occurs.
		 * In order to prevent ROR error, we limit FIFO size (8->4).
		 * We confirmed operation at speed up to 25 MHz.
		 * We recommend DMA transfer at high speed.
		 */
		cnt = 4 - cnt;
	} else {
		cnt = 8 - cnt;
	}

	_tx = tx_buf;
	ret = 0;
	for(i = 0; i < cnt; i++)
	{
		write_data = 0;
		if(_tx != NULL) {
			if(word_size <= 1) {
				write_data = _tx[0];
			} else if(word_size <= 2) {
				write_data = (((uint32)_tx[1] << 8)
						| (uint32)_tx[0]);
			} else {
				write_data = (((uint32)_tx[3] << 24)
						| ((uint32)_tx[2] << 16)
						| ((uint32)_tx[1] << 8)
						| ((uint32)_tx[0] << 0));
			}
			_tx +=word_size;
		}

		writel(write_data, tccgpsb[ch].base + GPSB_PORT);
		ret += word_size;

		if(ret >= tx_buf_size) {
			break;
		}
	}

	return ret;
}

static uint32 tcc_gpsb_read_fifo(uint8 ch, uint8 *rx_buf, uint32 rx_buf_size)
{
	uint8 i, word_size;
	uint8 *_rx;
	uint32 cnt, read_data, ret;

	/* Get empty fifo count */
	cnt = (readl(tccgpsb[ch].base + GPSB_STAT) >> 16) & 0xF;

	word_size = (uint8)((readl(tccgpsb[ch].base + GPSB_MODE) >> 8) & 0x1F) + 1;
	word_size = word_size / 8;

	_rx = rx_buf;
	ret = 0;
	/* i = 0; */ //CS : useless assignment
	for(i = 0; i < cnt; i++)
	{
		read_data = readl(tccgpsb[ch].base + GPSB_PORT);

		if(_rx != NULL) {
			if(word_size <= 1) {
				_rx[0] = (uint8)(read_data & 0xFF);
			} else if(word_size <= 2) {
				_rx[0] = (uint8)(read_data & 0xFF);
				_rx[1] = (uint8)((read_data >> 8) & 0xFF);
			} else {
				_rx[0] = (uint8)(read_data & 0xFF);
				_rx[1] = (uint8)((read_data >> 8) & 0xFF);
				_rx[2] = (uint8)((read_data >> 16) & 0xFF);
				_rx[3] = (uint8)((read_data >> 24) & 0xFF);
			}
			_rx += word_size;
		}

		ret += word_size;

		if(ret >= rx_buf_size) {
			break;
		}
	}

	return ret;
}

/*
 ************************************************************************************************************************
 * Description: Set SPI master clock speed in Hz
 *
 * Arguments  : ch                is GPSB channel
 *
 *              speed_in_hz       is SPI CLK speed in Hz
 *
 * Returns    : int32
 ************************************************************************************************************************
 */
int32 tcc_spi_master_set_speed(uint8 ch, uint32 speed_in_hz)
{
	uint32 pclk;
	uint8 divldv;
	int32 ret;

	divldv = ((readl(tccgpsb[ch].base + GPSB_MODE) >> 24) & 0xFF);
	pclk = speed_in_hz * (divldv + 1) * 2;
	SPI_DBG("%s: speed: %d / divldv: %d / pclk: %d\n", __func__, speed_in_hz, divldv, pclk);

	ret = tcc_set_peri(tccgpsb[ch].peri_name, ENABLE, pclk);
	if(ret != 0) {
		SPI_DBG("%s: ch %d failed to set peri. clock\n",
				__func__, ch);
		return OSAL_ERR_INVALID_PARAMETER;
	}

	return OSAL_ERR_NONE;
}

/*
 ************************************************************************************************************************
 * Description: Set Bits per word
 *
 * Arguments  : ch        is GPSB channel
 *
 *              bpw       is bits per word to set
 *
 * Returns    : int32
 ************************************************************************************************************************
 */
int32 tcc_spi_master_set_bpw(uint8 ch, uint8 bpw)
{
	int32 ret;

	if(((bpw != 8) && (bpw != 16)) && (bpw != 32)) {
		SPI_DBG("%s: ch %d bpw is not supported\n", __func__, bpw);
		return OSAL_ERR_INVALID_PARAMETER;
	}

	tcc_gpsb_bit_cset(tccgpsb[ch].base + GPSB_MODE, 0x1F << 8, (uint32)(((bpw -1) & 0x1F)) << 8);

	/*
	 * According to bits per word, Tx/Rx half-word and byte swap should be set
	 */
	if(bpw == 8) {
		tcc_gpsb_bit_clr(tccgpsb[ch].base + GPSB_INTEN,
				(BSP_BIT_24 | BSP_BIT_25 | BSP_BIT_26 | BSP_BIT_27));
	} else if(bpw == 16) {
		tcc_gpsb_bit_cset(tccgpsb[ch].base + GPSB_INTEN,
				(BSP_BIT_24 | BSP_BIT_25 | BSP_BIT_26 | BSP_BIT_27),
				(BSP_BIT_24 | BSP_BIT_26));
	} else {
		tcc_gpsb_bit_set(tccgpsb[ch].base + GPSB_INTEN,
				(BSP_BIT_24 | BSP_BIT_25 | BSP_BIT_26 | BSP_BIT_27));
	}

	ret = OSAL_ERR_NONE;

	return ret;
}

/*
 * If you don't have a specific reason, please use tcc_spi_master_set_bpw().
 * In some cases, need to send/receive the data without half-word and byte swapping.
 */
int32 tcc_spi_master_set_bpw_noswap(uint8 ch, uint8 bpw)
{
	int32 ret;

	if(((bpw != 8) && (bpw != 16)) && (bpw != 32)) {
		SPI_DBG("%s ch %d bpw is not supported\n", __func__, bpw);
		return OSAL_ERR_INVALID_PARAMETER;
	}

	tcc_gpsb_bit_cset(tccgpsb[ch].base + GPSB_MODE, 0x1F << 8, (uint32)(((bpw -1) & 0x1F)) << 8);

	tcc_gpsb_bit_clr(tccgpsb[ch].base + GPSB_INTEN,
			(BSP_BIT_24 | BSP_BIT_25 | BSP_BIT_26 | BSP_BIT_27));

	ret = OSAL_ERR_NONE;

	return ret;
}

/*
 ************************************************************************************************************************
 * Description: Get Bits per word from GPSB register
 *
 * Arguments  : ch        is GPSB channel
 *
 * Returns    : uint8
 ************************************************************************************************************************
 */
static uint8 tcc_spi_master_get_bpw(uint8 ch)
{
	uint8 bpw;

	bpw = (uint8)((readl(tccgpsb[ch].base + GPSB_MODE) >> 8) & 0x1F);

	return (bpw + 1);
}


/*
 ************************************************************************************************************************
 * Description: Set SPI master mode to GPSB block(Clocking, Data direction and Loopback mode)
 *
 * Arguments  : ch         is GPSB channel
 *
 *              mode       is modes for cloking, loopback and data-direction
 *
 *                                 SPI_CPOL           is set for CPOL
 *                                 SPI_CPHA           is set for CPHA
 *                                 SPI_CS_HIGH        is that cs is activated when cs is HIGH (GPSB control this)
 *                                 SPI_LSB_FIRST      is data shift direction control
 *                                 SPI_LOOP           is loop-back test mode
 *
 * Returns    : void
 ************************************************************************************************************************
 */
int32 tcc_spi_master_set_mode(uint8 ch, uint32 mode)
{
	int32 ret;

	if((mode & SPI_CPOL) != 0) {
		tcc_gpsb_bit_set(tccgpsb[ch].base + GPSB_MODE, BSP_BIT_16);
	}
	else {
		tcc_gpsb_bit_clr(tccgpsb[ch].base + GPSB_MODE, BSP_BIT_16);
	}

	if((mode & SPI_CPHA) != 0) {
		tcc_gpsb_bit_set(tccgpsb[ch].base + GPSB_MODE, BSP_BIT_18 | BSP_BIT_17);
	}
	else {
		tcc_gpsb_bit_clr(tccgpsb[ch].base + GPSB_MODE, BSP_BIT_18 | BSP_BIT_17);
	}

	if((mode & SPI_CS_HIGH) != 0) {
		tcc_gpsb_bit_set(tccgpsb[ch].base + GPSB_MODE, BSP_BIT_20 | BSP_BIT_19);
	}
	else {
		tcc_gpsb_bit_clr(tccgpsb[ch].base + GPSB_MODE, BSP_BIT_20 | BSP_BIT_19);
	}

	if((mode & SPI_LSB_FIRST) != 0) {
		tcc_gpsb_bit_set(tccgpsb[ch].base + GPSB_MODE, BSP_BIT_07);
	}
	else {
		tcc_gpsb_bit_clr(tccgpsb[ch].base + GPSB_MODE, BSP_BIT_07);
	}

	if((mode & SPI_LOOP) != 0) {
		tcc_gpsb_bit_set(tccgpsb[ch].base + GPSB_MODE, BSP_BIT_06);
	}
	else {
		tcc_gpsb_bit_clr(tccgpsb[ch].base + GPSB_MODE, BSP_BIT_06);
	}

	SPI_DBG("%s: ch %d GPSB MODE 0x%x\n", __func__, ch, readl(tccgpsb[ch].base + GPSB_MODE));

	ret = OSAL_ERR_NONE;
	return ret;
}

/*
 ************************************************************************************************************************
 * Description: Initialize gpio for GPSB FRM(CS), Configure GPIO to GFB function and GPSB PCFG FRM field
 *
 * Arguments  : ch             is gpsb channel
 *
 *              cs_gpio        is GPIO index for cs
 *
 * Returns    : int32
 ************************************************************************************************************************
 */
int32 tcc_spi_master_alloc_cs(uint8 ch, int32 cs_gpio)
{
	int32 ret, gfb_cs;

	/* Configure CS_GPIO as gpio function */
	if(cs_gpio > -1) {
		gfb_cs = gpio_to_num(cs_gpio);
		if(gfb_cs < 0) {
			return OSAL_ERR_INVALID_PARAMETER;
		}

		/* Configure CS_GPIO as GFB function and GPSB controls CS */
		ret = gpio_config(cs_gpio, GPIO_FUNC(6) | GPIO_OUTPUT);
		if(ret != 0) {
			return OSAL_ERR_INVALID_PARAMETER;
		}
	} else {
		return OSAL_ERR_INVALID_PARAMETER;
	}

	return OSAL_ERR_NONE;
}

/*
 ************************************************************************************************************************
 * Description: De-initialize GPSB FRM(CS), Clear GPSB PCFG FRM field
 *
 * Arguments  : ch             is gpsb channel
 *
 * Returns    : int32
 ************************************************************************************************************************
 */
int32 tcc_spi_master_dealloc_cs(uint8 ch)
{
	int32 ret;

	ret = OSAL_ERR_NONE;

	return ret;
}

/*
 ************************************************************************************************************************
 * Description: Control cs_gpio
 *
 * Arguments  : cs_gpio        is GPIO index for cs
 *
 *              enable         is cs_state
 *
 *              cs_high        is for activating cs when gpio is high
 *
 * Returns    : int32
 ************************************************************************************************************************
 */
static int32 tcc_spi_master_set_cs(int32 cs_gpio, uint8 enable, uint8 cs_high)
{
	int32 ret;

	if(cs_gpio < 0) {
		return OSAL_ERR_INVALID_PARAMETER;
	}

	if(cs_high != BSP_FALSE) {
		enable = !enable;
	}

	// CS: Unreachable Computation
	(void)gpio_set(cs_gpio, !enable);

	/*  //CS : redundant condition
	if(ret != OSAL_ERR_NONE) {
		return OSAL_ERR_INVALID_HANDLE;
	}
	*/
	ret = OSAL_ERR_NONE;

	return ret;
}

/*
 ************************************************************************************************************************
 * Description: Initialize gpio using CS
 *
 * Arguments  : ch             is gpsb channel
 *
 *              cs_gpio        is GPIO index for cs
 *
 *              cs_high        is for activating cs when gpio is high
 *
 * Returns    : int32
 ************************************************************************************************************************
 */
int32 tcc_spi_master_init_cs(uint8 ch, int32 cs_gpio, uint8 cs_high)
{
	int32 ret;

	/* Configure CS_GPIO as gpio function */
	if(cs_gpio > -1) {
		ret = gpio_config(cs_gpio, GPIO_FUNC(0) | GPIO_OUTPUT);
		if(ret != 0) {
			return OSAL_ERR_INVALID_PARAMETER;
		}
		/* Initial state is deactivated state */
		ret = tcc_spi_master_set_cs(cs_gpio, BSP_FALSE, cs_high);
		if(ret != OSAL_ERR_NONE) {
			SPI_DBG("%s: ch %d deactivating cs_gpios(%d) is failed\n",
					__func__, ch, cs_gpio);
			return ret;
		}
	} else {
		return OSAL_ERR_INVALID_PARAMETER;
	}

	return OSAL_ERR_NONE;
}

/*
 ************************************************************************************************************************
 * Description: Activate chip select gpio
 *
 * Arguments  : ch             is gpsb channel
 *
 *              cs_gpio        is GPIO index for cs
 *
 *              cs_high        is for activating cs when gpio is high
 *
 * Returns    : int32
 ************************************************************************************************************************
 */
int32 tcc_spi_master_activate_cs(uint8 ch, int32 cs_gpio, uint8 cs_high)
{
	return tcc_spi_master_set_cs(cs_gpio, BSP_TRUE, cs_high);
}

/*
 ************************************************************************************************************************
 * Description: De-activate chip select gpio
 *
 * Arguments  : ch             is gpsb channel
 *
 *              cs_gpio        is GPIO index for cs
 *
 *              cs_high        is for activating cs when gpio is high
 *
 * Returns    : int32
 ************************************************************************************************************************
 */
int32 tcc_spi_master_deactivate_cs(uint8 ch, int32 cs_gpio, uint8 cs_high)
{
	return tcc_spi_master_set_cs(cs_gpio, BSP_FALSE, cs_high);
}

/*
 ************************************************************************************************************************
 * Description: Wait until RX FIFO is valid (support polling only because support to use before OS initialization)
 *
 * Arguments  : ch             is GPSB channel
 *
 * Returns    : int32
 ************************************************************************************************************************
 */
static int32 tcc_spi_master_pio_wait_rx_fifo_valid(uint32 ch)
{
	int32 cnt = 0;

	while(((readl(tccgpsb[ch].base + GPSB_STAT) & BSP_BIT_00) == 0)) {
		cnt++;
		if(cnt > GPSB_POLL_TIMEOUT) {
			tcc_gpsb_disable(ch);
			SPI_DBG("%s ch %d pio polling mode timeout err\n", __func__, ch);
			return OSAL_ERR_TIMEOUT;
		}
	}

	return OSAL_ERR_NONE;
}

/*
 ************************************************************************************************************************
 * Description: Synchronous transfer (bits per word = 8)
 *
 * Arguments  : ch               is GPSB channel
 *
 *              tx               is buffer for transmitting data
 *
 *              rx               is buffer for receiving data
 *
 *              data_size        is buffer size
 *
 * Returns    : int32
 ************************************************************************************************************************
 */
static int32 tcc_spi_master_sync_pio_txrx_8(uint8 ch, const void *_tx, void *_rx, uint32 data_size)
{
	uint32 data_num, i, dummy;
	const uint8 *tx = _tx;
	uint8 *rx = _rx;
	int32 err;

	if(ch >= GPSB_CH_NUM) {
		SPI_DBG("%s %d channel is wrong\n", __func__, ch);
		return OSAL_ERR_INVALID_PARAMETER;
	}

	tcc_gpsb_set_transfer_state(ch, GPSB_XFER_STATE_RUNNING);

	tcc_gpsb_enable(ch);

	data_num = data_size;
	for(i = 0; i < data_num; i++) {
		/* Write data to TXFIFO */
		if(tx != BSP_NULL) {
			writel((tx[i] & 0xFF), tccgpsb[ch].base + GPSB_PORT);
		}
		else {
			writel(0, tccgpsb[ch].base + GPSB_PORT);
		}

		/* Wait until Read FIFO is valid */
		err = tcc_spi_master_pio_wait_rx_fifo_valid(ch);
		if(err != OSAL_ERR_NONE) {
			tcc_gpsb_disable(ch);
			tcc_gpsb_set_transfer_state(ch, GPSB_XFER_STATE_IDLE);
			SPI_DBG("%s ch %d wait rx fifo valid err %d\n", __func__, ch, err);
			return err;
		}

		/* Read data from RX FIFO */
		dummy = readl(tccgpsb[ch].base + GPSB_PORT);
		if(rx != BSP_NULL) {
			rx[i] = (uint8)(dummy & 0xFF);
		} else {
			/* Nothing to do */
		}
	}

	tcc_gpsb_disable(ch);
	tcc_gpsb_set_transfer_state(ch, GPSB_XFER_STATE_IDLE);

	return OSAL_ERR_NONE;
}

/*
 ************************************************************************************************************************
 * Description: Synchronous transfer (bits per word = 16)
 *
 * Arguments  : ch               is GPSB channel
 *
 *              tx               is buffer for transmitting data
 *
 *              rx               is buffer for receiving data
 *
 *              data_size        is buffer size
 *
 * Returns    : int32
 ************************************************************************************************************************
 */
static int32 tcc_spi_master_sync_pio_txrx_16(uint8 ch, const void *_tx, void *_rx, uint32 data_size)
{
	uint32 data_num, i, dummy;
	const uint16 *tx = _tx;
	uint16 *rx = _rx;
	int32 err;

	if(ch >= GPSB_CH_NUM) {
		SPI_DBG("%s %d channel is wrong\n", __func__, ch);
		return OSAL_ERR_INVALID_PARAMETER;
	}

	if((data_size % 2) != 0) {
		SPI_DBG("%s ch %d data size(%d) is not alligned\n", __func__, ch, data_size);
		return OSAL_ERR_INVALID_PARAMETER;
	}

	tcc_gpsb_set_transfer_state(ch, GPSB_XFER_STATE_RUNNING);

	tcc_gpsb_enable(ch);

	data_num = data_size / 2;
	for(i = 0; i < data_num; i++) {
		/* Write data to TXFIFO */
		if(tx != BSP_NULL) {
			writel((tx[i] & 0xFFFF), tccgpsb[ch].base + GPSB_PORT);
		}
		else {
			writel(0, tccgpsb[ch].base + GPSB_PORT);
		}

		/* Wait until Read FIFO is valid */
		err = tcc_spi_master_pio_wait_rx_fifo_valid(ch);
		if(err != OSAL_ERR_NONE) {
			tcc_gpsb_disable(ch);
			tcc_gpsb_set_transfer_state(ch, GPSB_XFER_STATE_IDLE);
			SPI_DBG("%s ch %d wait rx fifo valid err %d\n", __func__, ch, err);
			return err;
		}

		/* Read data from RX FIFO */
		dummy = readl(tccgpsb[ch].base + GPSB_PORT);
		if(rx != BSP_NULL) {
			rx[i] = (uint16)(dummy & 0xFFFF);
		} else {
			/* Nothing to do */
		}
	}

	tcc_gpsb_disable(ch);
	tcc_gpsb_set_transfer_state(ch, GPSB_XFER_STATE_IDLE);

	return OSAL_ERR_NONE;
}

/*
 ************************************************************************************************************************
 * Description: Synchronous transfer (bits per word = 32)
 *
 * Arguments  : ch               is GPSB channel
 *
 *              tx               is buffer for transmitting data
 *
 *              rx               is buffer for receiving data
 *
 *              data_size        is buffer size
 *
 * Returns    : int32
 ************************************************************************************************************************
 */
static int32 tcc_spi_master_sync_pio_txrx_32(uint8 ch, const void *_tx, void *_rx, uint32 data_size)
{
	uint32 data_num, i, dummy;
	const uint32 *tx = _tx;
	uint32 *rx = _rx;
	int32 err;

	if(ch >= GPSB_CH_NUM) {
		SPI_DBG("%s %d channel is wrong\n", __func__, ch);
		return OSAL_ERR_INVALID_PARAMETER;
	}

	if((data_size % 4) != 0) {
		SPI_DBG("%s ch %d data size(%d) is not alligned\n", __func__, ch, data_size);
		return OSAL_ERR_INVALID_PARAMETER;
	}

	tcc_gpsb_set_transfer_state(ch, GPSB_XFER_STATE_RUNNING);

	tcc_gpsb_enable(ch);

	data_num = data_size / 4;
	for(i = 0; i < data_num; i++) {
		/* Write data to TXFIFO */
		if(tx != BSP_NULL) {
			writel((tx[i] & 0xFFFFFFFF), tccgpsb[ch].base + GPSB_PORT);
		}
		else {
			writel(0, tccgpsb[ch].base + GPSB_PORT);
		}

		/* Wait until Read FIFO is valid */
		err = tcc_spi_master_pio_wait_rx_fifo_valid(ch);
		if(err != OSAL_ERR_NONE) {
			tcc_gpsb_disable(ch);
			tcc_gpsb_set_transfer_state(ch, GPSB_XFER_STATE_IDLE);
			SPI_DBG("%s ch %d wait rx fifo valid err %d\n", __func__, ch, err);
			return err;
		}

		/* Read data from RX FIFO */
		dummy = readl(tccgpsb[ch].base + GPSB_PORT);
		if(rx != BSP_NULL) {
			rx[i] = (uint32)(dummy & 0xFFFFFFFF);
		} else {
			/* Nothing to do */
		}
	}

	tcc_gpsb_disable(ch);
	tcc_gpsb_set_transfer_state(ch, GPSB_XFER_STATE_IDLE);

	return OSAL_ERR_NONE;
}

/*
 ************************************************************************************************************************
 * Description: Asynchronous transfer without GPSB dedicated DMA
 *
 * Arguments  : ch               is GPSB channel
 *
 *              tx               is buffer for transmitting data
 *
 *              rx               is buffer for receiving data
 *
 *              data_size        is buffer size
 *
 * Returns    : int32
 ************************************************************************************************************************
 */
static int32 tcc_spi_master_async_pio_txrx(uint8 ch, const void *tx, void *rx, uint32 data_size)
{
	uint32 ret;

	SPI_DBG("%s: ch %d\n", __func__, ch);

	tccgpsb[ch].async_tx_buf = tx;
	tccgpsb[ch].async_rx_buf = rx;
	tccgpsb[ch].async_tx_data_size = data_size;
	tccgpsb[ch].async_rx_data_size = data_size;

	/* Clear FIFO counter */
	tcc_gpsb_bit_set(tccgpsb[ch].base + GPSB_MODE, (BSP_BIT_15 | BSP_BIT_14));
	tcc_gpsb_bit_clr(tccgpsb[ch].base + GPSB_MODE, (BSP_BIT_15 | BSP_BIT_14));

	/* Write data to FIFO */
	ret = tcc_gpsb_write_fifo(ch, tccgpsb[ch].async_tx_buf, tccgpsb[ch].async_tx_data_size);
	if(tccgpsb[ch].async_tx_buf != NULL) {
		tccgpsb[ch].async_tx_buf = (void *)((uint32)tccgpsb[ch].async_tx_buf + ret);
	}
	tccgpsb[ch].async_tx_data_size -= ret;

	/* Enable operation */
	tcc_gpsb_set_transfer_state(ch, GPSB_XFER_STATE_RUNNING);
	tcc_gpsb_enable_interrupt(ch);
	tcc_gpsb_enable(ch);

	return OSAL_ERR_NONE;
}

/*
 ************************************************************************************************************************
 * Description: Asynchronous transfer with GPSB dedicated DMA
 *
 * Arguments  : ch               is GPSB channel
 *
 *              tx               is buffer for transmitting data
 *
 *              rx               is buffer for receiving data
 *
 *              data_size        is buffer size
 *
 * Returns    : int32
 ************************************************************************************************************************
 */


/* Tx/Rx DMA buffer */
static int32 tcc_spi_master_async_dma_txrx(uint8 ch, const void *tx, void *rx, uint32 data_size)
{
	int32 err;
	uint32 copy_length;
	uint8 bpw;
	/* Check wrong GPSB channel number */
	if (ch >= GPSB_CH_NUM) {
		SPI_DBG("%d channel is wrong\n", ch);
		return OSAL_ERR_INVALID_PARAMETER;
	}

	if(tccgpsb[ch].support_dma != BSP_TRUE) {
		SPI_DBG("%s ch %d does not support dma transfer\n",
				__func__, ch);
		return OSAL_ERR_NOT_SUPPORT;
	}

	/*
	 * For channels which is able to use a dedicated dma,
	 * set byte endian mode according to bits per word.
	 */
	bpw = tcc_spi_master_get_bpw(ch);
	//if(ch < DEDICATED_DMA_NUM) { //QAC : always true
		if(bpw != 32) {
			tcc_gpsb_bit_set(tccgpsb[ch].base + GPSB_DMACTR, BSP_BIT_28);
		}
		else {
			/* When bits per word is equal to 32, disable half-word and byte */
			tcc_gpsb_bit_clr(tccgpsb[ch].base + GPSB_INTEN,
					(BSP_BIT_24 | BSP_BIT_25 | BSP_BIT_26 | BSP_BIT_27));
			tcc_gpsb_bit_clr(tccgpsb[ch].base + GPSB_DMACTR, BSP_BIT_28);
		}
	//}

	/* Clear FIFO counter */
	tcc_gpsb_bit_set(tccgpsb[ch].base + GPSB_MODE, (BSP_BIT_15 | BSP_BIT_14));
	tcc_gpsb_bit_clr(tccgpsb[ch].base + GPSB_MODE, (BSP_BIT_15 | BSP_BIT_14));

	/* Clear packet counter (CH 0-2) */
	tcc_gpsb_bit_set(tccgpsb[ch].base + GPSB_DMACTR, BSP_BIT_02);
	tcc_gpsb_bit_clr(tccgpsb[ch].base + GPSB_DMACTR, BSP_BIT_02);

	if(data_size > tccgpsb[ch].tx_dma.size) {
		copy_length = tccgpsb[ch].tx_dma.size;
	} else {
		copy_length = data_size;
	}

	if(tx != BSP_NULL) {
		OSAL_MemCopy(tccgpsb[ch].tx_dma.vaddr, tx, copy_length);
	}

	/* Set DMA TXBASE and RXBASE */
	err = tcc_gpsb_set_dma_addr(ch, (uint32)tccgpsb[ch].tx_dma.paddr, (uint32)tccgpsb[ch].rx_dma.paddr);
	if(err != OSAL_ERR_NONE) {
		SPI_DBG("%s %d dma set address err %d\n", __func__, ch, err);
		return err;
	}

	/* Set Packet size and count */
	writel(((copy_length & 0x1FFF) << 0), tccgpsb[ch].base + GPSB_PACKET);

	tccgpsb[ch].async_tx_buf = tx;
	tccgpsb[ch].async_rx_buf = rx;
	tccgpsb[ch].async_tx_data_size = data_size;
	tccgpsb[ch].async_rx_data_size = 0;

	/* Enable dma operation */
	tcc_gpsb_set_transfer_state(ch, GPSB_XFER_STATE_RUNNING);
	tcc_gpsb_enable(ch);
	tcc_gpsb_start_dma(ch);

	return OSAL_ERR_NONE;
}

/*
 ************************************************************************************************************************
 * Description: Transfer data according to the transfer mode
 *
 * Arguments  : ch               is GPSB channel
 *
 *              tx               is buffer for transmitting data
 *
 *              rx               is buffer for receiving data
 *
 *              data_size        is buffer size
 *
 *              xfer_mode        is transfer mode
 *
 *                                 GPSB_XFER_MODE_WITHOUT_INTERRUPT : synchronous transfer
 *                                 GPSB_XFER_MODE_WITH_INTERRUPT : asynchronous transfer (use with below options)
 *                                 GPSB_XFER_MODE_DMA : use GPSB dedicatede DMA (support only channel 0 - 2)
 *                                 GPSB_XFER_MODE_PIO : do not use DMA (support all channel)
 *
 * Returns    : int32
 ************************************************************************************************************************
 */
int32 tcc_spi_master_xfer(uint8 ch, const void *tx, void *rx, uint32 data_size, uint8 xfer_mode)
{
	int32 ret = OSAL_ERR_NONE;
	uint8 bpw;
	uint32 word_size;
	
	if (ch >= GPSB_CH_NUM) { //QAC : Rule 21.18
		SPI_DBG("%d channel is wrong\n", ch);
		return OSAL_ERR_INVALID_PARAMETER;
	}

	if(tccgpsb[ch].state != GPSB_XFER_STATE_IDLE) {
		SPI_DBG("%s: ch %d is not in idle state\n",
				__func__, ch);
		return OSAL_ERR_INVALID_HANDLE;
	}

	/* Check data length and bpw */
	if(data_size == 0) {
		SPI_DBG("%s: ch %d data size is zero\n", __func__, ch);
		return OSAL_ERR_INVALID_PARAMETER;
	}

	bpw = tcc_spi_master_get_bpw(ch);
	if(bpw <= 8) {
		word_size = 1;
	} else if(bpw <= 16) {
		word_size = 2;
	} else {
		word_size = 4;
	}

	if((data_size % word_size) != 0) {
		SPI_DBG("%s: ch %d data size(%d) is not aligned by word size(%d)\n",
				__func__, ch, data_size, word_size);
		return OSAL_ERR_INVALID_PARAMETER;
	}

	/*
	 * Set GPSB CTF(Continuous Transfer mode) according to the flag.
	 * In case of transfer without CTF mode, you should configure
	 * GPSB PCFG FRM settings by using tcc_spi_master_alloc_cs() before
	 * transfer.
	 * In case of transfer with CTF mode, you can control CS as GPIO
	 * or assign CS to GPSB FRM.
	 */
	if((xfer_mode & GPSB_XFER_MODE_WITHOUT_CTF) != 0) {
		tcc_gpsb_bit_clr(tccgpsb[ch].base + GPSB_MODE, BSP_BIT_04);
	} else {
		tcc_gpsb_bit_set(tccgpsb[ch].base + GPSB_MODE, BSP_BIT_04);
	}

	/* Start transfer according to xfer mode */
	ret = -1; // CS: Useless assignment
	if((xfer_mode & GPSB_XFER_MODE_WITH_INTERRUPT) != 0) {
		/* w/ interrupt mode (support non-bloning mode only) */
		if((xfer_mode & GPSB_XFER_MODE_DMA) != 0) {
			/* DMA mode */
			//if(ch >= DEDICATED_DMA_NUM) { //QAC : always false
				ret = OSAL_ERR_NOT_SUPPORT;
			//} else {
				ret = tcc_spi_master_async_dma_txrx(ch, tx, rx, data_size);
			//}
		} else {
			/* Non-DMA mode */
			ret = tcc_spi_master_async_pio_txrx(ch, tx, rx, data_size);
		}
	} else if((xfer_mode & GPSB_XFER_MODE_WITHOUT_INTERRUPT) != 0) {
		/* w/o interrupt mode (support blocking mode only) */

		/* Clear FIFO counter */
		tcc_gpsb_bit_set(tccgpsb[ch].base + GPSB_MODE, (BSP_BIT_15 | BSP_BIT_14));
		tcc_gpsb_bit_clr(tccgpsb[ch].base + GPSB_MODE, (BSP_BIT_15 | BSP_BIT_14));

		if(bpw <= 8) {
			ret = tcc_spi_master_sync_pio_txrx_8(ch, tx, rx, data_size);
		} else if(bpw <= 16) {
			ret = tcc_spi_master_sync_pio_txrx_16(ch, tx, rx, data_size);
		} else {
			ret = tcc_spi_master_sync_pio_txrx_32(ch, tx, rx, data_size);
		}
	} else {
		SPI_DBG("%s: ch %d unsupported transfer mode 0x%x\n",
				__func__, ch, xfer_mode);
		return OSAL_ERR_NOT_SUPPORT;
	}

	return ret;
}

/*
 ************************************************************************************************************************
 * Description: Abort the transfer
 *
 * Arguments  : ch               is GPSB channel
 *
 * Returns    : int32
 ************************************************************************************************************************
 */
void tcc_spi_master_abort_xfer(uint8 ch)
{
	/* Stop operate due to error */
	tcc_gpsb_stop_dma(ch);
	tcc_gpsb_disable_interrupt(ch);
	tcc_gpsb_disable(ch);
	tcc_gpsb_set_transfer_state(ch, GPSB_XFER_STATE_IDLE);

	/* Clear FIFO counter */
	tcc_gpsb_bit_set(tccgpsb[ch].base + GPSB_MODE, (BSP_BIT_15 | BSP_BIT_14));
	tcc_gpsb_bit_clr(tccgpsb[ch].base + GPSB_MODE, (BSP_BIT_15 | BSP_BIT_14));

	tccgpsb[ch].async_tx_buf = BSP_NULL;
	tccgpsb[ch].async_rx_buf = BSP_NULL;
	tccgpsb[ch].async_tx_data_size = 0;
	tccgpsb[ch].async_rx_data_size = 0;
}

/*
 ************************************************************************************************************************
 * Description: Initiailize GPSB registers for SPI master
 *
 * Arguments  : ch             is GPSB channel
 *
 * Returns    :
 ************************************************************************************************************************
 */
static void tcc_gpsb_hwinit_for_spi_master(uint8 ch)
{
	int32 ret; // CS: Coercion Alters Value
	/* Enable iobus */
	ret = tcc_set_iobus_pwdn(tccgpsb[ch].iobus_name, 0);
	if(ret != 0) {
		SPI_DBG("%s: ch %d failed to enable iobus pwdn\n", __func__, ch);
	}
	else
	{
		/* Disable operation */
		tcc_gpsb_disable(ch);

		/* Set GPSB DIVLDV */
		tcc_gpsb_bit_cset(tccgpsb[ch].base + GPSB_MODE, (0xFF << 24), ((GPSB_DEF_DIVLDV & 0xFF) << 24));

		/* Set GPSB MODE register */
		tcc_gpsb_bit_cset(tccgpsb[ch].base + GPSB_MODE,
				(BSP_BIT_02 | BSP_BIT_01 | BSP_BIT_00),  /* Set SPI master */
				(BSP_BIT_04)                             /* Set CTF mode */
				);

		/* Set interrup for handle error status (for ROR, WUR, RUR, WOR) */
		tcc_gpsb_bit_cset(tccgpsb[ch].base + GPSB_INTEN,
				(0x3FF),
				(BSP_BIT_05 | BSP_BIT_06 | BSP_BIT_07 | BSP_BIT_08)
				);

		/* Set GPSB FIFO Theshold for generating interrupt request */
		tcc_gpsb_bit_cset(tccgpsb[ch].base + GPSB_INTEN, (0x7 << 20), (GPSB_CFGWTH << 20));
		tcc_gpsb_bit_cset(tccgpsb[ch].base + GPSB_INTEN, (0x7 << 16), (GPSB_CFGRTH << 16));
	}

}

/*
 ************************************************************************************************************************
 * Description: This function is called to initialize the GPSB block as SPI master mode
 *
 * Arguments  : master                  is structure for SPI master
 *
 *              gpio_sdo                is GPIO index for SDO
 *
 *              gpio_sdi                is GPIO index for SDI
 *
 *              gpio_sclk               is GPIO index for SCLK
 *
 *              cb                      is callback funtion for notify transfer complete
 *
 *              arg                     is argument for callback
 *
 * Returns    : int32
 ************************************************************************************************************************
 */
int32 tcc_spi_master_open(uint8 ch, int32 gpio_sdo, int32 gpio_sdi, int32 gpio_sclk,
		void *dma_addr_tx, void *dma_addr_rx, uint32 dma_buf_size, tcc_spi_handler_t cb, void *arg)
{
	int32 err;

	OSAL_SR_ALLOC();

	/* Check wrong GPSB channel number */
	if(ch >= GPSB_CH_NUM) {
		SPI_DBG("%s: %d channel is wrong\n", __func__, ch);
		return OSAL_ERR_INVALID_PARAMETER;
	}

	OSAL_CRITICAL_ENTER();
	if(tccgpsb[ch].state != GPSB_XFER_STATE_DISABLED) {
		OSAL_CRITICAL_EXIT();
		SPI_DBG("%s: %d channel is already opened\n", __func__, ch);
		return OSAL_ERR_INIT;
	}
	tccgpsb[ch].state = GPSB_XFER_STATE_IDLE;
	OSAL_CRITICAL_EXIT();

	/* Initialize the gpsb structure */
	tccgpsb[ch].gpsb_sdo     = gpio_sdo;
	tccgpsb[ch].gpsb_sdi     = gpio_sdi;
	tccgpsb[ch].gpsb_sclk    = gpio_sclk;
	tccgpsb[ch].complete.cb  = cb;
	tccgpsb[ch].complete.arg = arg;

	tccgpsb[ch].support_dma  = BSP_FALSE;

	tccgpsb[ch].tx_dma.paddr = BSP_NULL;
	tccgpsb[ch].tx_dma.vaddr = BSP_NULL;
	tccgpsb[ch].tx_dma.size  = 0;

	tccgpsb[ch].rx_dma.paddr = BSP_NULL;
	tccgpsb[ch].rx_dma.vaddr = BSP_NULL;
	tccgpsb[ch].rx_dma.size  = 0;

	if((dma_buf_size != 0) && ((dma_addr_tx != BSP_NULL) || (dma_addr_rx != BSP_NULL))) {
		tccgpsb[ch].support_dma = BSP_TRUE;

		if(dma_addr_tx != BSP_NULL) {
			tccgpsb[ch].tx_dma.paddr = (void *)GPSB_GET_PHY_ADDR(dma_addr_tx);
			tccgpsb[ch].tx_dma.vaddr = (void *)GPSB_DMA_BUF_ADDR(dma_addr_tx);
			tccgpsb[ch].tx_dma.size = dma_buf_size;
			SPI_DBG("%s: tx_dma.paddr: 0x%08X / vaddr: 0x%08X / size: %d\n", __func__, tccgpsb[ch].tx_dma.paddr, tccgpsb[ch].tx_dma.vaddr, tccgpsb[ch].tx_dma.size);
		}

		if(dma_addr_rx != BSP_NULL) {
			tccgpsb[ch].rx_dma.paddr = (void *)GPSB_GET_PHY_ADDR(dma_addr_rx);
			tccgpsb[ch].rx_dma.vaddr = (void *)GPSB_DMA_BUF_ADDR(dma_addr_rx);
			tccgpsb[ch].rx_dma.size = dma_buf_size;
			SPI_DBG("%s: rx_dma.paddr: 0x%08X / vaddr: 0x%08X\n", __func__, tccgpsb[ch].rx_dma.paddr, tccgpsb[ch].rx_dma.vaddr, tccgpsb[ch].rx_dma.size);
		}
	}

	/* Reset GPSB block */
	err = tcc_gpsb_reset(ch);
	if(err != OSAL_ERR_NONE) {
		SPI_DBG("%s: ch %d failed to reset gpsb err %d\n",
				__func__, ch, err);
		return err;
	}

	/* Set port and gpio configuration */
	if (ch != CH_SDM) {
		err = tcc_gpsb_set_port(ch, gpio_sdo, gpio_sdi, gpio_sclk);
		if(err != OSAL_ERR_NONE) {
			SPI_DBG("%s: ch %d gpsb set port failed err %d\n",
					__func__, ch, err);
			return err;
		}
	}

	/* Stop dma operation */
	tcc_gpsb_stop_dma(ch);

	/* Initialize GPSB for spi master*/
	tcc_gpsb_hwinit_for_spi_master(ch);

	/* Set gpsb init and mode state */
	tccgpsb[ch].is_slave = BSP_FALSE;

	tcc_gpsb_set_transfer_state(ch, GPSB_XFER_STATE_IDLE);

	return OSAL_ERR_NONE;
}

/*
 ************************************************************************************************************************
 * Description: This function is called to disabled GPSB block
 *
 * Arguments  : ch             is GPSB channel
 *
 * Returns    : int32
 ************************************************************************************************************************
 */
int32 tcc_spi_master_close(uint8 ch)
{
	int32 ret; /* CS: Coercion Alters Value */

	OSAL_SR_ALLOC();

	/* Check wrong GPSB channel number */
	if(ch >= GPSB_CH_NUM) {
		SPI_DBG("%s %d channel is wrong\n", __func__, ch);
		return OSAL_ERR_INVALID_PARAMETER;
	}

	OSAL_CRITICAL_ENTER();
	if(tccgpsb[ch].state == GPSB_XFER_STATE_DISABLED) {
		OSAL_CRITICAL_EXIT();
		SPI_DBG("%s %d channel is already closed\n", __func__, ch);
		return OSAL_ERR_NONE;
	}
	tccgpsb[ch].state = GPSB_XFER_STATE_DISABLED;
	OSAL_CRITICAL_EXIT();

	/* Stop the operation */
	tcc_gpsb_stop_dma(ch);
	tcc_gpsb_disable_interrupt(ch);
	tcc_gpsb_disable(ch);

	/* Disable iobus */
	ret = tcc_set_iobus_pwdn(tccgpsb[ch].iobus_name, 1);
	if(ret != 0) {
		SPI_DBG("%s ch %d failed to disable iobus pwdn\n", __func__, ch);
		return OSAL_ERR_INVALID_PARAMETER;
	}

	tcc_gpsb_set_transfer_state(ch, GPSB_XFER_STATE_DISABLED);

	return OSAL_ERR_NONE;
}

/*
 ************************************************************************************************************************
 * Description: GPSB interrupt service routine
 *              Because GPSB has shared interrupt, it handles interrupt in one service routine
 *
 * Arguments  : arg             is the interrupt handler argument
 *
 * Returns    : int32
 ************************************************************************************************************************
 */
static void tcc_gpsb_isr(uint8 ch)
{
	uint32 dmaicr = 0;
	uint32 status = 0;
	int32 event   = 0;
	uint32 ret    = 0;

	if(tcc_gpsb_is_master(ch) != 0) {
		/* Check error status */
		status = readl(tccgpsb[ch].base + GPSB_STAT);
		if((status & (0xF << 5)) != 0) {
			SPI_DBG("%s: ch %d error status occur status 0x%08x\n",
					__func__, ch, status);
			if((status & BSP_BIT_05) != 0) {
				event |= GPSB_EVENT_ERR_ROR;
				SPI_DBG("%s: ch %d read fifo over-run error\n",
						__func__, ch);
			}
			if((status & BSP_BIT_06) != 0) {
				event |= GPSB_EVENT_ERR_WUR;
				SPI_DBG("%s: ch %d write fifo under-run error\n",
						__func__, ch);
			}
			if((status & BSP_BIT_07) != 0) {
				event |= GPSB_EVENT_ERR_RUR;
				SPI_DBG("%s: ch %d read fifo under-run error\n",
						__func__, ch);
			}
			if((status & BSP_BIT_08) != 0) {
				event |= GPSB_EVENT_ERR_WOR;
				SPI_DBG("%s: ch %d write fifo over-run error\n",
						__func__, ch);
			}

			/* Stop operate due to error */
			tcc_gpsb_disable(ch);
			tcc_gpsb_stop_dma(ch);
			tcc_gpsb_disable_interrupt(ch);
			tccgpsb[ch].state = GPSB_XFER_STATE_IDLE;

			/* Clear error status */
			tcc_gpsb_bit_set(tccgpsb[ch].base + GPSB_STAT,
					(BSP_BIT_05 | BSP_BIT_06 | BSP_BIT_07 | BSP_BIT_08));

			/* Notify error status */
			if(tccgpsb[ch].complete.cb != BSP_NULL) {
				tccgpsb[ch].complete.cb(ch, event, tccgpsb[ch].complete.arg);
			}

			return;
		}

		if((status & BSP_BIT_00) != 0) {
			SPI_DBG("%s: ch %d async pio transfer\n", __func__, ch);

			ret = tcc_gpsb_read_fifo(ch, tccgpsb[ch].async_rx_buf, tccgpsb[ch].async_rx_data_size);
			if(tccgpsb[ch].async_rx_buf != NULL) {
				tccgpsb[ch].async_rx_buf = (void *)((uint32)tccgpsb[ch].async_rx_buf + ret);
			}
			tccgpsb[ch].async_rx_data_size -= ret;

			if(tccgpsb[ch].async_tx_data_size > 0) {
				ret = tcc_gpsb_write_fifo(ch, tccgpsb[ch].async_tx_buf, tccgpsb[ch].async_tx_data_size);
				if(tccgpsb[ch].async_tx_buf != NULL) {
					tccgpsb[ch].async_tx_buf = (void *)((uint32)tccgpsb[ch].async_tx_buf + ret);
				}
				tccgpsb[ch].async_tx_data_size -= ret;
			}

			if(tccgpsb[ch].async_rx_data_size <= 0) {
				SPI_DBG("%s: ch %d complete async pio transfer\n", __func__, ch);

				tccgpsb[ch].async_tx_buf = BSP_NULL;
				tccgpsb[ch].async_rx_buf = BSP_NULL;
				tccgpsb[ch].async_tx_data_size = 0;

				tcc_gpsb_disable_interrupt(ch);
				tcc_gpsb_disable(ch);
				tccgpsb[ch].state = GPSB_XFER_STATE_IDLE;

				/* Notify the completion of transfer */
				event = GPSB_EVENT_COMPLETE;
				if(tccgpsb[ch].complete.cb != BSP_NULL) {
					tccgpsb[ch].complete.cb(ch, event, tccgpsb[ch].complete.arg);
				}
			}
		}

		dmaicr = readl(tccgpsb[ch].base + GPSB_DMAICR);
		if((dmaicr & BSP_BIT_29) != 0) {
			uint32 copy_length, data_size;

			SPI_DBG("%s: ch %d dma done interrupt\n", __func__, ch);

			writel(dmaicr, tccgpsb[ch].base + GPSB_DMAICR);

			data_size = tccgpsb[ch].async_tx_data_size;
			if(data_size > tccgpsb[ch].tx_dma.size) {
				copy_length = tccgpsb[ch].tx_dma.size;
			} else {
				copy_length = data_size;
			}

			if(tccgpsb[ch].async_rx_buf != BSP_NULL) {
				OSAL_MemCopy(tccgpsb[ch].async_rx_buf, tccgpsb[ch].rx_dma.vaddr, copy_length);
			}

			tccgpsb[ch].async_tx_data_size -= copy_length;
			tccgpsb[ch].async_tx_buf = (void *)((uint32)tccgpsb[ch].async_tx_buf + copy_length);
			tccgpsb[ch].async_rx_buf = (void *)((uint32)tccgpsb[ch].async_rx_buf + copy_length);

			SPI_DBG("%s: ch %d xfer in this time %d remainder %d\n",
					__func__, ch, copy_length, tccgpsb[ch].async_tx_data_size);

			if(tccgpsb[ch].async_tx_data_size != 0) {
				data_size = tccgpsb[ch].async_tx_data_size;
				if(data_size > tccgpsb[ch].tx_dma.size) {
					copy_length = tccgpsb[ch].tx_dma.size;
				} else {
					copy_length = data_size;
				}

				if(tccgpsb[ch].async_tx_buf  != BSP_NULL) {
					OSAL_MemCopy(tccgpsb[ch].tx_dma.vaddr, tccgpsb[ch].async_tx_buf, copy_length);
				}

				/* Set Packet size and count */
				writel(((copy_length & 0x1FFF) << 0), tccgpsb[ch].base + GPSB_PACKET);

				/* Re-start DMA for transferring remainder */
				tcc_gpsb_start_dma(ch);

			} else {
				tccgpsb[ch].async_tx_buf = BSP_NULL;
				tccgpsb[ch].async_rx_buf = BSP_NULL;
				tccgpsb[ch].async_tx_data_size = 0;
				tccgpsb[ch].async_rx_data_size = 0;

				/* DMA done interrupt */
				tcc_gpsb_stop_dma(ch);
				tcc_gpsb_disable(ch);
				tccgpsb[ch].state = GPSB_XFER_STATE_IDLE;

				/* Notify the completion of transfer */
				event = GPSB_EVENT_COMPLETE;
				if(tccgpsb[ch].complete.cb != BSP_NULL) {
					tccgpsb[ch].complete.cb(ch, event, tccgpsb[ch].complete.arg);
				}
			}
		}
	} else {
		//SPI_DBG("%s ch %d not defined mode\n", __func__, ch);
	}
}

static void tcc_gpsb0_isr(void *arg) {
	SPI_DBG("%s: enter ISR\n", __func__);
	tcc_gpsb_isr(0);
}

static void tcc_gpsb1_isr(void *arg) {
	SPI_DBG("%s: enter ISR\n", __func__);
	tcc_gpsb_isr(1);
}

static void tcc_gpsb2_isr(void *arg) {
	SPI_DBG("%s: enter ISR\n", __func__);
	tcc_gpsb_isr(2);
}

static void tcc_gpsb3_isr(void *arg) {
	SPI_DBG("%s: enter ISR\n", __func__);
	tcc_gpsb_isr(3);
}

static void tcc_gpsb0_dma_isr(void *arg) {
	SPI_DBG("%s: enter ISR\n", __func__);
	tcc_gpsb_isr(0);
}

static void tcc_gpsb1_dma_isr(void *arg) {
	SPI_DBG("%s: enter ISR\n", __func__);
	tcc_gpsb_isr(1);
}

static void tcc_gpsb2_dma_isr(void *arg) {
	SPI_DBG("%s: enter ISR\n", __func__);
	tcc_gpsb_isr(2);
}

static void tcc_gpsb3_dma_isr(void *arg) {
	SPI_DBG("%s: enter ISR\n", __func__);
	tcc_gpsb_isr(3);
}

/*
 ************************************************************************************************************************
 * Description: Register irq handler and enable interrupt
 *
 * Returns    : int32
 ************************************************************************************************************************
 */
int32 init_spi(void)
{
	int32 err = 0;

	/* Register gpsb irq handler */
	err = BSP_IntVectSet(INT_GPSB, GPSB_DEF_INT_PRIORITY, INT_TYPE_LEVEL_HIGH, tcc_gpsb0_isr, BSP_NULL);
	if(err != BSP_OK) {
		SPI_DBG("%s failed to set interrupt vector %d\n", __func__, INT_GPSB);
		return OSAL_ERR_INVALID_PARAMETER;
	}
	BSP_IntSrcEn(INT_GPSB);

	err = BSP_IntVectSet(INT_GPSB1, GPSB_DEF_INT_PRIORITY, INT_TYPE_LEVEL_HIGH, tcc_gpsb1_isr, BSP_NULL);
	if(err != BSP_OK) {
		SPI_DBG("%s: failed to set interrupt vector %d\n", __func__, INT_GPSB1);
		return OSAL_ERR_INVALID_PARAMETER;
	} else {}
	BSP_IntSrcEn(INT_GPSB1);

	err = BSP_IntVectSet(INT_GPSB2, GPSB_DEF_INT_PRIORITY, INT_TYPE_LEVEL_HIGH, tcc_gpsb2_isr, BSP_NULL);
	if(err != BSP_OK) {
		SPI_DBG("%s: failed to set interrupt vector %d\n", __func__, INT_GPSB2);
		return OSAL_ERR_INVALID_PARAMETER;
	} else {}
	BSP_IntSrcEn(INT_GPSB2);

	err = BSP_IntVectSet(INT_GPSB3, GPSB_DEF_INT_PRIORITY, INT_TYPE_LEVEL_HIGH, tcc_gpsb3_isr, BSP_NULL);
	if(err != BSP_OK) {
		SPI_DBG("%s: failed to set interrupt vector %d\n", __func__, INT_GPSB2);
		return OSAL_ERR_INVALID_PARAMETER;
	} else {}
	BSP_IntSrcEn(INT_GPSB3);

	err = BSP_IntVectSet(INT_GPSB0_DMA, GPSB_DEF_INT_PRIORITY, INT_TYPE_LEVEL_HIGH, tcc_gpsb0_dma_isr, BSP_NULL);
	if(err != BSP_OK) {
		SPI_DBG("%s: failed to set interrupt vector %d\n", __func__, INT_GPSB0_DMA);
		return OSAL_ERR_INVALID_PARAMETER;
	} else {}
	BSP_IntSrcEn(INT_GPSB0_DMA);

	err = BSP_IntVectSet(INT_GPSB1_DMA, GPSB_DEF_INT_PRIORITY, INT_TYPE_LEVEL_HIGH, tcc_gpsb1_dma_isr, BSP_NULL);
	if(err != BSP_OK) {
		SPI_DBG("%s: failed to set interrupt vector %d\n", __func__, INT_GPSB1_DMA);
		return OSAL_ERR_INVALID_PARAMETER;
	} else {}
	BSP_IntSrcEn(INT_GPSB1_DMA);

	err = BSP_IntVectSet(INT_GPSB2_DMA, GPSB_DEF_INT_PRIORITY, INT_TYPE_LEVEL_HIGH, tcc_gpsb2_dma_isr, BSP_NULL);
	if(err != BSP_OK) {
		SPI_DBG("%s: failed to set interrupt vector %d\n", __func__, INT_GPSB2_DMA);
		return OSAL_ERR_INVALID_PARAMETER;
	} else {}
	BSP_IntSrcEn(INT_GPSB2_DMA);

	err = BSP_IntVectSet(INT_GPSB3_DMA, GPSB_DEF_INT_PRIORITY, INT_TYPE_LEVEL_HIGH, tcc_gpsb3_dma_isr, BSP_NULL);
	if(err != BSP_OK) {
		SPI_DBG("%s: failed to set interrupt vector %d\n", __func__, INT_GPSB0_DMA);
		return OSAL_ERR_INVALID_PARAMETER;
	} else {}
	BSP_IntSrcEn(INT_GPSB3_DMA);

	return OSAL_ERR_NONE;
}

/*
 ************************************************************************************************************************
 * Description: Disable interrupt
 *
 * Returns    : void
 ************************************************************************************************************************
 */
int32 deinit_spi(void)
{
	BSP_IntSrcDis(INT_GPSB);
	BSP_IntSrcDis(INT_GPSB1);
	BSP_IntSrcDis(INT_GPSB2);
	BSP_IntSrcDis(INT_GPSB3);
	BSP_IntSrcDis(INT_GPSB0_DMA);
	BSP_IntSrcDis(INT_GPSB1_DMA);
	BSP_IntSrcDis(INT_GPSB2_DMA);
	BSP_IntSrcDis(INT_GPSB3_DMA);

	return OSAL_ERR_NONE;
}

