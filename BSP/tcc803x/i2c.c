/****************************************************************************
 *   FileName    : i2c.c
 *   Description : Telechips TCC803x I2C driver
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
#include <bsp.h>
#include <osal.h>
#include <clock.h>
#include <app_cfg.h>
#include <bsp_int.h>

#include "reg_phys.h"
#include "tcc_ckc.h"
#include "debug.h"
#include "gpio.h"
#include "device_share_config.h"

#include "i2c.h"

#include "temp_compile_switch.h"


#ifndef __GNU_C__
#define __func__ __FUNCTION__
#endif

#define I2C0_BASE        (TCC_I2C_BASE + 0x00000)
#define I2C1_BASE        (TCC_I2C_BASE + 0x10000)
#define I2C2_BASE        (TCC_I2C_BASE + 0x20000)
#define I2C_PCFG_BASE    (TCC_I2C_BASE + 0x30000)
#define I2C_PORT_SEL(x)  (I2C_PCFG_BASE + (0x4 * (x)))

#define I2C_PRES         (0x00)
#define I2C_CTRL         (0x04)
#define I2C_TXR          (0x08)
#define I2C_CMD          (0x0C)
#define I2C_RXR          (0x10)
#define I2C_SR           (0x14)
#define I2C_TIME         (0x18)

#define I2C_WR           0       /* Write Flag */
#define I2C_RD           1       /* Read Flag */

#define I2C_PCLK         6000000 /* I2C Default Peri Clock */

#define I2C_POLL_TIMEOUT (100000)
#define I2C_CH_NUM       3
#define I2C_PORT_NUM     29

/* I2C transfer state machine */
typedef enum {
	I2C_STATE_IDLE = 0,
	I2C_STATE_SEND,
	I2C_STATE_RECV,
	I2C_STATE_RECV_START,
	I2C_STATE_SEND_DONE,
	I2C_STATE_RECV_DONE,
	I2C_STATE_DISABLED,
	I2C_STATE_RUNNING
}tcc_i2c_state;

struct tcc_i2c_cmd_complete {
	tcc_i2c_handle_t cb;
	void *arg;
};

struct tcc_i2c_async {
	uint8 slave_addr;
	uint8 *cmd_buf;
	uint8 cmd_len;
	uint8 *out_buf;
	uint8 out_len;
	uint8 *in_buf;
	uint8 in_len;
	uint32 opt;
};

struct tcc_i2c {
	/* TCC i2c channel */
	uint32 clk;
	/* i2c base address */
	uint32 base;
	/* peri clock number */
	uint32 peri_name;
	/* iobus number */
	uint32 iobus_name;
	/* GPIO pins */
	int32 sda;
	int32 scl;
	int32 port;

	struct tcc_i2c_cmd_complete complete;
	struct tcc_i2c_async async;

	tcc_i2c_state state;
};

static struct tcc_i2c i2c[I2C_CH_NUM] = {
	{
		400,
		I2C0_BASE,
		PERI_I2C0,
		IOBUS_I2C0,
		-1,
		-1,
		-1,
		{0, },
		{0, },
		I2C_STATE_DISABLED
	},
	{
		400,
		I2C1_BASE,
		PERI_I2C1,
		IOBUS_I2C1,
		-1,
		-1,
		-1,
		{0, },
		{0, },
		I2C_STATE_DISABLED
	},
	{
		400,
		I2C2_BASE,
		PERI_I2C2,
		IOBUS_I2C2,
		-1,
		-1,
		-1,
		{0, },
		{0, },
		I2C_STATE_DISABLED
	},
};

enum
{
	I2C_SCL = 0,
	I2C_SDA,
	I2C_FUNC,
	I2C_DRISTR,
	I2C_MAX
};
	
#define tcc_i2c_enable_irq(ch) (writel(readl(i2c[ch].base + I2C_CTRL) | BSP_BIT_06, i2c[ch].base + I2C_CTRL))
#define tcc_i2c_disable_irq(ch) do{ \
	writel(readl(i2c[ch].base + I2C_CTRL) & ~BSP_BIT_06, i2c[ch].base + I2C_CTRL); \
	writel(readl(i2c[ch].base + I2C_CMD) | BSP_BIT_00, i2c[ch].base + I2C_CMD); \
}while(0)

#define tcc_i2c_complete_async_xfer(ch, event) do { \
	tcc_i2c_disable_irq(ch); \
	i2c[ch].state = I2C_STATE_IDLE; \
	if(i2c[ch].complete.cb != BSP_NULL) {i2c[ch].complete.cb(ch, (event), i2c[ch].complete.arg);}\
}while(0)

#define tcc_i2c_clear_intr(ch) (writel(readl(i2c[ch].base + I2C_CMD) | BSP_BIT_00, i2c[ch].base + I2C_CMD))

static int32 wait_intr(uint8 ch);
static int32 ack_check(uint8 ch);
static int32 tcc_i2c_sync_xfer(const uint8 ch, const uint8 slave_addr,
		const uint8 cmd_len, const uint8 *cmd_buf,
		const uint8 out_len, const uint8 *out_buf,
		const uint8 in_len,  uint8 *in_buf,
		uint32 opt);
static int32 tcc_i2c_async_xfer(const uint8 ch, const uint8 slave_addr,
		const uint8 cmd_len, uint8 *cmd_buf,
		const uint8 out_len, uint8 *out_buf,
		const uint8 in_len,  uint8 *in_buf,
		uint32 opt);
static void tcc_i2c_isr(uint8 ch);
static void tcc_i2c0_isr(void *arg);
static void tcc_i2c1_isr(void *arg);
static void tcc_i2c2_isr(void *arg);

static int32 tcc_i2c_reset(uint32 ch);
static int32 tcc_i2c_set_port(uint32 ch);
static int32 tcc_i2c_clear_port(uint32 ch);
static int32 tcc_i2c_set_clock(uint32 ch, uint32 input_clk_kHz);
static int32 tcc_i2c_enable(uint32 ch);
static void tcc_i2c_reinit(uint8 ch);

/*
 * Wait until cmd operation is working correctly
 * Notice: If timeout occur, check SCL and SDA signal status
 * If SCL is low by slave devices during the byte transfer, CMD is not completed
 */
static int32 wait_intr(uint8 ch)
{
	int32 ret = BSP_ERR_NONE;
	int32 cnt = 0;

	while(((readl(i2c[ch].base + I2C_CMD) & 0xF0)) != 0) {
		if(cnt > I2C_POLL_TIMEOUT) {
			I2C_DBG("%s: ch %d non interrupt time out, cmd(0x%08x)\n",
			                __func__, ch, readl(i2c[ch].base + I2C_CMD));
			ret = BSP_ERR_TIMEOUT;
			break;
		}
		cnt++;
	}

	if(ret == BSP_ERR_NONE) {
		cnt = 0;
		while(((readl(i2c[ch].base + I2C_SR) & BSP_BIT_01)) != 0) {
			if(cnt > I2C_POLL_TIMEOUT) {
				I2C_DBG("%s: ch %d non interrupt time out, status(0x%08x)\n",
				                __func__, ch, readl(i2c[ch].base + I2C_SR));
				ret = BSP_ERR_TIMEOUT;
				break;
			}
			cnt++;
		}
	}

	tcc_i2c_clear_intr(ch);
	if(ret != BSP_ERR_NONE) {
		/* If command is not completed, try to reinit the core */
		tcc_i2c_reinit(ch);
	}

	return ret;
}

/* Check whether receive the acknowledgement after send the byte */
static int32 ack_check(uint8 ch)
{
	if((readl(i2c[ch].base + I2C_SR) & BSP_BIT_07) == 0) {
		return BSP_ERR_NONE;
	}
	return BSP_ERR_NO_ACK;
}

/* Synchronous transfer mode */
static int32 tcc_i2c_sync_xfer(const uint8 ch, const uint8 slave_addr,
		const uint8 cmd_len, const uint8 *cmd_buf,
		const uint8 out_len, const uint8 *out_buf,
		const uint8 in_len,  uint8 *in_buf,
		uint32 opt)
{
	int32 ret;
	int32 i;
	int32 err = BSP_ERR_NONE;//Msg(4:3353) The variable 'err' is possibly unset at this point. MISRA-C:2004 Rule 9.1

	ret = BSP_ERR_NONE;

	/* Write*/
	if((out_len > 0) || (cmd_len > 0)) {
		/* Send START condition and slave address with write flag*/
		writel((slave_addr | I2C_WR), i2c[ch].base + I2C_TXR);
		writel(BSP_BIT_07 | BSP_BIT_04 , i2c[ch].base + I2C_CMD);

		ret = wait_intr(ch);
		if(ret != BSP_ERR_NONE) {
			return ret;
		}

		err = ack_check(ch);
		if(err != BSP_ERR_NONE) {
			goto stop;
		}

		if(cmd_len > 0) {
			for(i = 0; i < cmd_len; i++) {
				/* Send data */
				writel(cmd_buf[i], i2c[ch].base + I2C_TXR);
				writel(BSP_BIT_04 , i2c[ch].base + I2C_CMD);

				ret = wait_intr(ch);
				if(ret != BSP_ERR_NONE) {
					return ret;
				}

				err = ack_check(ch);
				if(err != BSP_ERR_NONE) {
					goto stop;
				}
			}
		}

		if(out_len > 0) {
			for(i = 0; i < out_len; i++) {
				/* Send data */
				writel(out_buf[i], i2c[ch].base + I2C_TXR);
				writel(BSP_BIT_04 , i2c[ch].base + I2C_CMD);

				ret = wait_intr(ch);
				if(ret != BSP_ERR_NONE) {
					return ret;
				}

				err = ack_check(ch);
				if(err != BSP_ERR_NONE) {
					goto stop;
				}
			}
		}

		/* if read data is none, send STOP condition */
		if(in_len <= 0) {
			if((opt & WR_WITHOUT_STOP) != 0 ) //Msg(4:3344) Value of control expression is not derived from an explicit logical operation. MISRA-C:2004 Rule 13.2
			{
				return BSP_ERR_NONE;
			}
		}
	}

	/* Read */
	if(in_len > 0) {
		/* Send START condition and slave address with read flag*/
		writel((slave_addr | I2C_RD), i2c[ch].base + I2C_TXR);
		if(((opt & RD_WITHOUT_RPEAT_START) != 0)) {
			writel((BSP_BIT_07), i2c[ch].base + I2C_CMD);
		} else {
			writel((BSP_BIT_07 | BSP_BIT_04), i2c[ch].base + I2C_CMD);
		}

		ret = wait_intr(ch);
		if(ret != BSP_ERR_NONE) {
			return ret;
		}

		err = ack_check(ch);
		if(err != BSP_ERR_NONE) {
			goto stop;
		}

		for(i = 0; i < in_len; i++) {
			if (i == (in_len - 1)) {
				/* Last byte to read */
				if((opt & RD_WITHOUT_ACK) != 0) {
					writel(BSP_BIT_05, i2c[ch].base + I2C_CMD);
				} else {
					writel((BSP_BIT_05 | BSP_BIT_03), i2c[ch].base + I2C_CMD);
				}
			}
			else {
				writel(BSP_BIT_05 , i2c[ch].base + I2C_CMD);
			}

			ret = wait_intr(ch);
			if(ret != BSP_ERR_NONE) {
				return ret;
			}

			/* Store read data */
			in_buf[i] = readl(i2c[ch].base + I2C_RXR);
		}

		if((opt & RD_WITHOUT_STOP) != 0) //Msg(4:3344) Value of control expression is not derived from an explicit logical operation. MISRA-C:2004 Rule 13.2
		{
			return BSP_ERR_NONE;
		}
	}
stop:   /* Send STOP condition */
	writel(BSP_BIT_06 , i2c[ch].base + I2C_CMD);
	ret = wait_intr(ch);
	if(ret != BSP_ERR_NONE){
		return ret;
	}
	if(err != BSP_ERR_NONE){
		I2C_DBG("%s: i2c ch %d received NACK from 0x%x\n", __func__, ch, (slave_addr >> 1));
		return err;
	}
	return BSP_ERR_NONE;
}

/* Asynchronous transfer mode */
static int32 tcc_i2c_async_xfer(const uint8 ch, const uint8 slave_addr,
		const uint8 cmd_len, uint8 *cmd_buf,
		const uint8 out_len, uint8 *out_buf,
		const uint8 in_len,  uint8 *in_buf,
		uint32 opt)
{
	OSAL_SR_ALLOC();

	i2c[ch].async.slave_addr = slave_addr;
	i2c[ch].async.cmd_len = cmd_len;
	i2c[ch].async.cmd_buf = cmd_buf;
	i2c[ch].async.out_len = out_len;
	i2c[ch].async.out_buf = out_buf;
	i2c[ch].async.in_buf = in_buf;
	i2c[ch].async.in_len = in_len;
	i2c[ch].async.opt = opt;

	tcc_i2c_enable_irq(ch);

	if((i2c[ch].async.cmd_len > 0) || (i2c[ch].async.out_len > 0)) {
		OSAL_CRITICAL_ENTER();
		i2c[ch].state = I2C_STATE_SEND;
		OSAL_CRITICAL_EXIT();
		I2C_DBG("%s ch %d I2C_STATE_SEND\n", __func__, ch);
		writel((slave_addr | I2C_WR), i2c[ch].base + I2C_TXR);
		writel(BSP_BIT_07 | BSP_BIT_04 , i2c[ch].base + I2C_CMD);
	} else if(i2c[ch].async.in_len > 0) {
		OSAL_CRITICAL_ENTER();
		i2c[ch].state = I2C_STATE_RECV_START;
		OSAL_CRITICAL_EXIT();
		I2C_DBG("%s ch %d I2C_STATE_RECV_START\n", __func__, ch);
		writel((slave_addr | I2C_RD), i2c[ch].base + I2C_TXR);
		writel((BSP_BIT_07 | BSP_BIT_04), i2c[ch].base + I2C_CMD);
	} else {
		tcc_i2c_disable_irq(ch);
		OSAL_CRITICAL_ENTER();
		i2c[ch].state = I2C_STATE_IDLE;
		OSAL_CRITICAL_EXIT();
		return BSP_ERR_INVALID_PARAMETER;
	}

	return BSP_ERR_NONE;
}

/* Write and read data */
int32 tcc_i2c_xfer(const uint8 ch, const uint8 slave_addr,
		const uint8 out_len, uint8 *out_buf,
		const uint8 in_len,  uint8 *in_buf,
		uint32 opt, uint8 async)
{
	int32 ret;
	OSAL_SR_ALLOC();

	OSAL_CRITICAL_ENTER();
	if(i2c[ch].state != I2C_STATE_IDLE) {
		OSAL_CRITICAL_EXIT();
		return BSP_ERR_INVALID_HANDLE;
	}
	i2c[ch].state = I2C_STATE_RUNNING;
	OSAL_CRITICAL_EXIT();

	if(async != 0) {
		ret = tcc_i2c_async_xfer(ch, slave_addr, 0, BSP_NULL, out_len, out_buf, in_len, in_buf, opt);
	} else {
		ret = tcc_i2c_sync_xfer(ch, slave_addr, 0, BSP_NULL, out_len, out_buf, in_len, in_buf, opt);
		OSAL_CRITICAL_ENTER();
		i2c[ch].state = I2C_STATE_IDLE;
		OSAL_CRITICAL_EXIT();
	}

	return ret;
}

/* Write cmd and data, read data */
int32 tcc_i2c_xfer_w_cmd(const uint8 ch, const uint8 slave_addr,
		const uint8 cmd_len, uint8 *cmd_buf,
		const uint8 out_len, uint8 *out_buf,
		const uint8 in_len,  uint8 *in_buf,
		uint32 opt, uint8 async)
{
	int32 ret;
	OSAL_SR_ALLOC();

	OSAL_CRITICAL_ENTER();
	if(i2c[ch].state != I2C_STATE_IDLE) {
		OSAL_CRITICAL_EXIT();
		return BSP_ERR_INVALID_HANDLE;
	}
	i2c[ch].state = I2C_STATE_RUNNING;
	OSAL_CRITICAL_EXIT();

	if(async != 0) {
		ret = tcc_i2c_async_xfer(ch, slave_addr, cmd_len, cmd_buf, out_len, out_buf, in_len, in_buf, opt);
	} else {
		ret = tcc_i2c_sync_xfer(ch, slave_addr, cmd_len, cmd_buf, out_len, out_buf, in_len, in_buf, opt);
		if(ack_check(ch)==0)
		{
			I2C_DBG("Receive ACK ! addr : %x \n", (slave_addr >> 1));
		}
		OSAL_CRITICAL_ENTER();
		i2c[ch].state = I2C_STATE_IDLE;
		OSAL_CRITICAL_EXIT();
	}

	return ret;
}
/* I2C interrupt handler */
static void tcc_i2c_isr(uint8 ch)
{
	uint32 status;
	uint8 is_intr, is_ack;

	status = readl(i2c[ch].base + I2C_SR);
	is_intr = status & BSP_BIT_00;
	if(is_intr != 0) {

		is_ack = (ack_check(ch) == BSP_ERR_NONE) ? 1 : 0;

		tcc_i2c_clear_intr(ch);

		if(i2c[ch].state == I2C_STATE_SEND) {
			if(is_ack != 0) {
				if(i2c[ch].async.cmd_len != 0) {
					/* Send cmd first, if exist */
					writel(*i2c[ch].async.cmd_buf, i2c[ch].base + I2C_TXR);
					i2c[ch].async.cmd_len--;
					i2c[ch].async.cmd_buf++;

					writel(BSP_BIT_04 , i2c[ch].base + I2C_CMD);
				} else {
					if(i2c[ch].async.out_len != 0) {
						/* Send data next to the cmd */
						writel(*i2c[ch].async.out_buf, i2c[ch].base + I2C_TXR);
						i2c[ch].async.out_len--;
						i2c[ch].async.out_buf++;

						writel(BSP_BIT_04 , i2c[ch].base + I2C_CMD);
					} else {
						/* After send cmd and out data */
						if(i2c[ch].async.in_len != 0) {
							/* Start to recevie data */
							i2c[ch].state = I2C_STATE_RECV_START;
							I2C_DBG("%s ch %d I2C_STATE_RECV_START\n", __func__, ch);
							writel((i2c[ch].async.slave_addr | I2C_RD), i2c[ch].base + I2C_TXR);
							if(((i2c[ch].async.opt & RD_WITHOUT_RPEAT_START) != 0)) {
								writel((BSP_BIT_07), i2c[ch].base + I2C_CMD);
							} else {
								writel((BSP_BIT_07 | BSP_BIT_04), i2c[ch].base + I2C_CMD);
							}
						} else {
							/* Finish the transfer */
							i2c[ch].state = I2C_STATE_SEND_DONE;
							if((i2c[ch].async.opt & WR_WITHOUT_STOP) == 0) {
								writel(BSP_BIT_06 , i2c[ch].base + I2C_CMD);
							} else {
								tcc_i2c_complete_async_xfer(ch, I2C_EVENT_XFER_COMPLETE);
							}
						}
					}
				}
			} else {
				writel(BSP_BIT_06 , i2c[ch].base + I2C_CMD);
				I2C_DBG("%s: i2c ch %d received NACK from 0x%x\n", __func__, ch, (i2c[ch].async.slave_addr >> 1));
				tcc_i2c_complete_async_xfer(ch, I2C_EVENT_NACK);
			}
		} else if(i2c[ch].state == I2C_STATE_SEND_DONE) {
			tcc_i2c_complete_async_xfer(ch, I2C_EVENT_XFER_COMPLETE);
		} else if(i2c[ch].state == I2C_STATE_RECV_START) {
			if(is_ack != 0) {
				if(i2c[ch].async.in_len != 0) {
					i2c[ch].state = I2C_STATE_RECV;
					I2C_DBG("%s ch %d I2C_STATE_RECV\n", __func__, ch);

					if(i2c[ch].async.in_len == 1) {
						/* Read last byte */
						if((i2c[ch].async.opt & RD_WITHOUT_ACK) != 0) {
							writel(BSP_BIT_05, i2c[ch].base + I2C_CMD);
						} else {
							writel((BSP_BIT_05 | BSP_BIT_03), i2c[ch].base + I2C_CMD);
						}
					} else {
						writel(BSP_BIT_05, i2c[ch].base + I2C_CMD);
					}
				} else {
					i2c[ch].state = I2C_STATE_RECV_DONE;
					I2C_DBG("%s ch %d I2C_STATE_RECV_DONE\n", __func__, ch);
					if((i2c[ch].async.opt & RD_WITHOUT_STOP) == 0) {
						writel(BSP_BIT_06 , i2c[ch].base + I2C_CMD);
					}
				}
			} else {
				writel(BSP_BIT_06 , i2c[ch].base + I2C_CMD);
				I2C_DBG("%s: i2c ch %d received NACK from 0x%x\n", __func__, ch, (i2c[ch].async.slave_addr >> 1));
				tcc_i2c_complete_async_xfer(ch, I2C_EVENT_NACK);
			}
		} else if(i2c[ch].state == I2C_STATE_RECV) {
			*i2c[ch].async.in_buf = readl(i2c[ch].base + I2C_RXR);
			i2c[ch].async.in_len--;
			i2c[ch].async.in_buf++;

			if(i2c[ch].async.in_len != 0) {
				if(i2c[ch].async.in_len == 1) {
					if((i2c[ch].async.opt & RD_WITHOUT_ACK) != 0) {
						/* Read last byte */
						writel(BSP_BIT_05, i2c[ch].base + I2C_CMD);
					} else {
						writel((BSP_BIT_05 | BSP_BIT_03), i2c[ch].base + I2C_CMD);
					}
				} else {
					writel(BSP_BIT_05, i2c[ch].base + I2C_CMD);
				}
			} else {
				i2c[ch].state = I2C_STATE_RECV_DONE;
				I2C_DBG("%s ch %d I2C_STATE_RECV_DONE\n", __func__, ch);
				if((i2c[ch].async.opt & RD_WITHOUT_STOP) == 0) {
					writel(BSP_BIT_06 , i2c[ch].base + I2C_CMD);
				}
			}
		} else if(i2c[ch].state == I2C_STATE_RECV_DONE) {
			tcc_i2c_complete_async_xfer(ch, I2C_EVENT_XFER_COMPLETE);
		} else {
			/* Nothing to do */
		}
	}
}

static void tcc_i2c0_isr(void *arg) {
	I2C_DBG("%s: Enter CH0 ISR\n", __func__);
	tcc_i2c_isr(0);
}

static void tcc_i2c1_isr(void *arg) {
	I2C_DBG("%s: Enter CH1 ISR\n", __func__);
	tcc_i2c_isr(1);
}

static void tcc_i2c2_isr(void *arg) {
	I2C_DBG("%s: Enter CH2 ISR\n", __func__);
	tcc_i2c_isr(2);
}

/* Reset i2c core */
static int32 tcc_i2c_reset(uint32 ch)
{
	int32 ret;

	/* SW reset */
	ret = tcc_set_iobus_swreset(i2c[ch].iobus_name, 1);
	if(ret != BSP_ERR_NONE) {
		return BSP_ERR_INVALID_PARAMETER;
	}
	ret = tcc_set_iobus_swreset(i2c[ch].iobus_name, 0);
	if(ret != BSP_ERR_NONE) {
		return BSP_ERR_INVALID_PARAMETER;
	}

	return BSP_ERR_NONE;
}

/* Set i2c port and gpio */
static int32 tcc_i2c_set_port(uint32 ch)
{
	int32 tcci2cport[I2C_PORT_NUM][I2C_MAX] = {
		{NULL, NULL, NULL},
		{NULL, NULL, NULL},
		{NULL, NULL, NULL},
		{TCC_GPA(4),   TCC_GPA(5),   8,	GPIO_CD(2)},
		{TCC_GPA(10),  TCC_GPA(11),  8,	GPIO_CD(2)},
		{TCC_GPA(16),  TCC_GPA(17),  8,	GPIO_CD(2)},
		{TCC_GPA(22),  TCC_GPA(23),  8,	GPIO_CD(2)},
		{TCC_GPA(28),  TCC_GPA(29),  8,	GPIO_CD(2)},
		{TCC_GPB(4),   TCC_GPB(5),   8,	GPIO_CD(2)},
		{TCC_GPB(10),  TCC_GPB(11),  8,	GPIO_CD(2)},
		{TCC_GPB(17),  TCC_GPB(18),  8,	GPIO_CD(2)},
		{TCC_GPB(19),  TCC_GPB(20),  8,	GPIO_CD(2)},
		{TCC_GPB(21),  TCC_GPB(22),  8,	GPIO_CD(2)},
		{TCC_GPB(23),  TCC_GPB(24),  8,	GPIO_CD(2)},
		{TCC_GPC(14),  TCC_GPC(15),  8,	GPIO_CD(2)},
		{TCC_GPC(20),  TCC_GPC(21),  8,	GPIO_CD(2)},
		{TCC_GPC(24),  TCC_GPC(25),  8,	GPIO_CD(2)},
		{TCC_GPC(28),  TCC_GPC(29),  8,	GPIO_CD(2)},
		{TCC_GPG(4),   TCC_GPG(5),   8,	GPIO_CD(2)},
		{TCC_GPE(3),   TCC_GPE(4),   8,	GPIO_CD(2)},
		{TCC_GPE(9),   TCC_GPE(10),  8,	GPIO_CD(2)},
		{TCC_GPE(14),  TCC_GPE(15),  8,	GPIO_CD(2)},
		{TCC_GPH(4),   TCC_GPH(5),   9,	GPIO_CD(2)},
		{TCC_GPH(6),   TCC_GPH(7),   9,	GPIO_CD(2)},
		{TCC_GPMA(4),  TCC_GPMA(5),  8,	GPIO_CD(2)},
		{TCC_GPMA(10), TCC_GPMA(11), 8,	GPIO_CD(2)},
		{TCC_GPMA(16), TCC_GPMA(17), 8,	GPIO_CD(2)},
		{TCC_GPMA(22), TCC_GPMA(23), 8,	GPIO_CD(2)},
		{TCC_GPMA(28), TCC_GPMA(29), 8,	GPIO_CD(2)}
	};

	uint32 pcfg_val, port_val;
	int32 sda, scl;
	int32 ret;
	int32 match_port;
	int32 port = 0;
	uint32 i;
	/* QAC : 21.18 : Dereference of an invalid pointer value of ch */
	if(ch >= I2C_CH_NUM) 
	{
		return BSP_ERR_INVALID_PARAMETER;
	}

	sda = i2c[ch].sda;
	scl = i2c[ch].scl;

	if(((sda < 0) || (scl < 0))) {
		I2C_DBG("%s: ch %d gpio set error sda %d scl %d\n", __func__, ch , sda, scl);
		return BSP_ERR_INVALID_PARAMETER;
	}

	match_port = 0;
	for (i = 0; i < I2C_PORT_NUM; i++) {
		if (scl == tcci2cport[i][I2C_SCL]) {
			if (sda == tcci2cport[i][I2C_SDA]) {
				match_port = 1;
				port = i;
			}
		}
	}
	if (match_port == 1) {
		i2c[ch].port = port;
		I2C_DBG("%s: i2c port %d\n", __func__, i2c[ch].port);
	}
	else {
		I2C_DBG("%s: i2c port is wrong\n", __func__);
		return BSP_ERR_INVALID_PARAMETER;
	}

	for (i = 0; i < I2C_CH_NUM; i++) {
		if ((i != ch) && (i2c[i].port == port)) {
			I2C_DBG("%s: %d port is already used by ch%d\n", __func__, port, i);
			return BSP_ERR_INVALID_PARAMETER;
		}
	}

	/* Set i2c pcfg */
	//if(ch < I2C_CH_NUM) { //QAC always true
	pcfg_val = readl(I2C_PORT_SEL(0));
	pcfg_val &= ~((uint32)0xFF << (ch * 8));
	port_val = (i2c[ch].port << (ch * 8));
	writel(pcfg_val | port_val, I2C_PORT_SEL(0));
	I2C_DBG("%s: I2C_PORT_SEL: 0x%08X\n", __func__, readl(I2C_PORT_SEL(0)));
	//} 
	//else 
	//{
	//	return OSAL_ERR_INIT;
	//}

	/* Set gpio */
#if (TEMP_COMPILE_SWITCH_05WORK_10WORK == TEMP_COMPILE_SWITCH_05WORK)
	ret = gpio_config(sda, GPIO_FUNC(tcci2cport[port][I2C_FUNC]) | GPIO_INPUT | GPIO_INPUTBUF_EN);
	if(ret != 0) {
		return BSP_ERR_INVALID_PARAMETER;
	}
	//ret = gpio_config(scl, GPIO_FUNC(tcci2cport[port][I2C_FUNC]) | GPIO_INPUT | GPIO_INPUTBUF_EN);
	ret = gpio_config(scl, GPIO_FUNC(tcci2cport[port][I2C_FUNC]) | GPIO_OUTPUT);
	if(ret != 0) {
		return BSP_ERR_INVALID_PARAMETER;
	}
#else
	ret = gpio_config(sda, GPIO_FUNC(tcci2cport[port][I2C_FUNC]) | GPIO_INPUT | GPIO_INPUTBUF_EN | tcci2cport[port][I2C_DRISTR]);
	if(ret != 0) {
		return BSP_ERR_INVALID_PARAMETER;
	}
	ret = gpio_config(scl, GPIO_FUNC(tcci2cport[port][I2C_FUNC]) | GPIO_OUTPUT | tcci2cport[port][I2C_DRISTR]);
	if(ret != 0) {
		return BSP_ERR_INVALID_PARAMETER;
	}
#endif

	return BSP_ERR_NONE;
}

static int32 tcc_i2c_clear_port(uint32 ch)
{
	uint32 pcfg_val;
	int32 sda, scl;
	int32 ret;
	
	/* QAC : 21.18 : Dereference of an invalid pointer value of ch */
	if(ch >= I2C_CH_NUM) 
	{
		return BSP_ERR_INVALID_PARAMETER;
	}

	sda = i2c[ch].sda;
	scl = i2c[ch].scl;

	if(((sda < 0) || (scl < 0))) {
		return BSP_ERR_INVALID_PARAMETER;
	}

	/* Reset gpio */
	ret = gpio_config(sda, GPIO_FUNC(0));
	if(ret != 0) {
		return BSP_ERR_INVALID_PARAMETER;
	}
	ret = gpio_config(scl, GPIO_FUNC(0));
	if(ret != 0) {
		return BSP_ERR_INVALID_PARAMETER;
	}

	/* Reset i2c pcfg */
	//if(ch < I2C_CH_NUM) { //QAC always true
		pcfg_val = readl(I2C_PORT_SEL(0));
		pcfg_val |= ((uint32)0xFF << (ch * 8));
		writel(pcfg_val, I2C_PORT_SEL(0));
	//}
	//else {
	//	return OSAL_ERR_INIT;
	//}

	return BSP_ERR_NONE;
}

/* @ input_clk : in kHz */
/* Set i2c clock (prescale) */
static int32 tcc_i2c_set_clock(uint32 ch, uint32 input_clk_kHz)
{
	uint32 prescale;

	/* Set prescale */
	prescale = (input_clk_kHz / ((i2c[ch].clk) * 5)) - 1;
	writel(prescale, i2c[ch].base + I2C_PRES);

	return BSP_ERR_NONE;
}

/* Enable i2c */
static int32 tcc_i2c_enable(uint32 ch)
{
	uint32 input_clk_kHz;
	int32 ret;

	/* enable i2c peri bus */
	ret = tcc_set_peri(i2c[ch].peri_name, ENABLE, I2C_PCLK);
	if(ret != 0) {
		return BSP_ERR_INVALID_HANDLE;
	}
	/* enable iobus */
	ret = tcc_set_iobus_pwdn(i2c[ch].iobus_name, 0);
	if(ret != 0) {
		return BSP_ERR_INVALID_HANDLE;
	}
	/* get i2c bus clock */
	input_clk_kHz = tcc_get_peri(i2c[ch].peri_name);		/* get I2C bus clock */
	I2C_DBG("%s: I2C bus clock %d\n", __func__, input_clk_kHz);

	/* set i2c clock */
	(void)tcc_i2c_set_clock(ch, (input_clk_kHz/1000)); //Codesonar Unused Value (err)
	//if(err != OSAL_ERR_NONE) { //QAC : always false
	//	return err;
	//}

	/* Enable I2C core. Set 8bit mode*/
	writel(BSP_BIT_07 | BSP_BIT_NONE, i2c[ch].base + I2C_CTRL);

	/* Clear pending interrupt */
	tcc_i2c_clear_intr(ch);

	return BSP_ERR_NONE;
}

/* Reinit i2c core */
static void tcc_i2c_reinit(uint8 ch)
{
	int32 ret;

	OSAL_SR_ALLOC();

	if(ch >= I2C_CH_NUM) {
		I2C_DBG("%s %d channel is wrong\n", __func__, ch);
		return;
	}

	OSAL_CRITICAL_ENTER();
	i2c[ch].state = I2C_STATE_DISABLED;
	OSAL_MemSet(&i2c[ch].async, 0, sizeof(struct tcc_i2c_async));
	OSAL_CRITICAL_EXIT();

	tcc_i2c_disable_irq(ch);

	tcc_i2c_clear_intr(ch);

	ret = tcc_i2c_reset(ch);
	if(ret != BSP_ERR_NONE) {
		I2C_DBG("%s ch %d failed to reset core\n", __func__, ch);
	}

	ret = tcc_i2c_set_port(ch);
	if(ret != BSP_ERR_NONE) {
		I2C_DBG("%s ch %d failed to set port\n", __func__, ch);
	}

	ret = tcc_i2c_enable(ch);
	if(ret != BSP_ERR_NONE) {
		I2C_DBG("%s ch %d failed to enable core\n", __func__, ch);
	}

	OSAL_CRITICAL_ENTER();
	i2c[ch].state = I2C_STATE_IDLE;
	OSAL_CRITICAL_EXIT();
}

int32 tcc_i2c_open(uint8 ch, int32 gpio_scl, int32 gpio_sda, uint32 speed_in_kHz,
		tcc_i2c_handle_t cb, void *arg)
{
	int32 ret;

	OSAL_SR_ALLOC();

	if(ch >= I2C_CH_NUM) {
		I2C_DBG("%s: %d channel is wrong\n", __func__, ch);
		return BSP_ERR_INVALID_PARAMETER;
	}

	i2c[ch].scl          = gpio_scl;
	i2c[ch].sda          = gpio_sda;
	i2c[ch].clk          = speed_in_kHz;
	i2c[ch].complete.cb  = cb;
	i2c[ch].complete.arg = arg;
	OSAL_MemSet(&i2c[ch].async, 0, sizeof(struct tcc_i2c_async));

	ret = tcc_i2c_reset(ch);
	if(ret != BSP_ERR_NONE) {
		I2C_DBG("%s: ch %d failed to reset core\n", __func__, ch);
		return ret;
	}

	ret = tcc_i2c_set_port(ch);
	if(ret != BSP_ERR_NONE) {
		I2C_DBG("%s: ch %d failed to set port\n", __func__, ch);
		return ret;
	}

	ret = tcc_i2c_enable(ch);
	if(ret != BSP_ERR_NONE) {
		I2C_DBG("%s: ch %d failed to enable core\n", __func__, ch);
		return ret;
	}

	OSAL_CRITICAL_ENTER();
	i2c[ch].state = I2C_STATE_IDLE;
	OSAL_CRITICAL_EXIT();

	return BSP_ERR_NONE;
}

int32 tcc_i2c_close(uint8 ch)
{
	int32 ret;

	OSAL_SR_ALLOC();

	if(ch >=I2C_CH_NUM) {
		I2C_DBG("%s %d channel is wrong\n", __func__, ch);
		return BSP_ERR_INVALID_PARAMETER;
	}

	OSAL_CRITICAL_ENTER();
	i2c[ch].state = I2C_STATE_DISABLED;
	OSAL_CRITICAL_EXIT();

	tcc_i2c_disable_irq(ch);

	tcc_i2c_clear_intr(ch);

	writel(0x0, i2c[ch].base + I2C_CTRL);

	ret = tcc_set_peri(i2c[ch].peri_name, DISABLE, 0);
	if(ret != 0) {
		return BSP_ERR_INVALID_HANDLE;
	}

	ret = tcc_set_iobus_pwdn(i2c[ch].iobus_name, 1);
	if(ret != 0) {
		return BSP_ERR_INVALID_HANDLE;
	}

	ret = tcc_i2c_clear_port(ch);
	if(ret != BSP_ERR_NONE) {
		I2C_DBG("%s ch %d failed to reset port\n", __func__, ch);
		return ret;
	}

	ret = tcc_i2c_reset(ch);
	if(ret != BSP_ERR_NONE) {
		I2C_DBG("%s ch %d failed to reset core\n", __func__, ch);
		return ret;
	}

	return BSP_ERR_NONE;
}

int32 init_i2c(void)
{
	int32 ret;

	/* Enter i2c irq handler */
	ret = BSP_IntVectSet(INT_I2C, PRIORITY_NO_MEAN, INT_TYPE_LEVEL_HIGH, tcc_i2c0_isr, BSP_NULL);
	if(ret != BSP_OK) {
		I2C_DBG("%s failed to init vector %d\n", __func__, INT_I2C);
		return ret;
	}
	BSP_IntSrcEn(INT_I2C);

	ret = BSP_IntVectSet(INT_I2C1, PRIORITY_NO_MEAN, INT_TYPE_LEVEL_HIGH, tcc_i2c1_isr, BSP_NULL);
	if(ret != BSP_OK) {
		I2C_DBG("%s failed to init vector %d\n", __func__, INT_I2C1);
		return ret;
	}
	BSP_IntSrcEn(INT_I2C1);

	ret = BSP_IntVectSet(INT_I2C2, PRIORITY_NO_MEAN, INT_TYPE_LEVEL_HIGH, tcc_i2c2_isr, BSP_NULL);
	if(ret != BSP_OK) {
		I2C_DBG("%s failed to init vector %d\n", __func__, INT_I2C2);
		return ret;
	}
	BSP_IntSrcEn(INT_I2C2);

	return BSP_ERR_NONE;
}

int32 deinit_i2c(void)
{
	BSP_IntSrcDis(INT_I2C);
	BSP_IntSrcDis(INT_I2C1);
	BSP_IntSrcDis(INT_I2C2);

	return BSP_ERR_NONE;
}

