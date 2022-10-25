/****************************************************************************
 *   FileName    : i2c.h
 *   Description : Telechips TCC803x I2C driver header
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

#ifndef _TCC_I2C_H_
#define _TCC_I2C_H_

/* I2C Debug message control */
//#define I2C_DEBUG
#ifdef I2C_DEBUG
#define I2C_DBG(fmt, args...)	printf(fmt, ## args)
#else
#define I2C_DBG(fmt, args...)	do {} while(0)
#endif

/* I2C transfer options */
#define WR_WITHOUT_STOP         0x0001 /* do not generate STOP condition after write last byte */
#define RD_WITHOUT_ACK          0x0010 /* do not send ACK when read last byte */
#define RD_WITHOUT_STOP         0x0020 /* do not generate STOP condition after read last byte */
#define RD_WITHOUT_RPEAT_START  0x0040 /* do not generate START condition between write and read sequence */

/* I2C event */
#define I2C_EVENT_XFER_COMPLETE 0x0000
#define I2C_EVENT_XFER_ABORT    0x0001
#define I2C_EVENT_NACK          0x0002

typedef void (*tcc_i2c_handle_t)(uint8 ch, int32 event, void *arg);

extern int32 tcc_i2c_xfer(const uint8 ch, const uint8 slave_addr,
	const uint8 out_len, uint8 *out_buf,
	const uint8 in_len,  uint8 *in_buf,
	uint32 opt, uint8 async);
extern int32 tcc_i2c_xfer_w_cmd(const uint8 ch, const uint8 slave_addr,
	const uint8 cmd_len, uint8 *cmd_buf,
	const uint8 out_len, uint8 *out_buf,
	const uint8 in_len,  uint8 *in_buf,
	uint32 opt, uint8 async);
extern int32 tcc_i2c_open(uint8 ch, int32 gpio_scl, int32 gpio_sda, uint32 speed_in_kHz,
	tcc_i2c_handle_t cb, void *arg);
extern int32 tcc_i2c_close(uint8 ch);
extern int32 init_i2c(void);
extern int32 deinit_i2c(void);

#endif /* _TCC_I2C_H_ */
