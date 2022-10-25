/****************************************************************************
 *   FileName    : gpio.h
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

#ifndef __BSP_GPIO__H__
#define __BSP_GPIO__H__

#include "types.h"
/*
 * gpio cfg structures
 *   [31:14]: reserved
 *   [13]   : slew rate
 *   [12]   : input type
 *   [11:10]: input buffer
 *   [9]    : direction
 *   [8:6]  : driver strength (0~3)
 *   [5:4]  : pull up/down
 *   [3:0]  : function selection (0~15)
 */

#define GPIO_SLEW_RATE_SHIFT	13
#define GPIO_SLEW_RATE_SLOW	(1<<GPIO_SLEW_RATE_SHIFT)
#define GPIO_SLEW_RATE_FAST	(0<<GPIO_SLEW_RATE_SHIFT)

#define GPIO_INPUT_TYPE_SHIFT	12
#define GPIO_INPUT_SCHMITT	(1<<GPIO_INPUT_TYPE_SHIFT)
#define GPIO_INPUT_CMOS		(0<<GPIO_INPUT_TYPE_SHIFT)

#define GPIO_INPUTBUF_SHIFT	10
#define GPIO_INPUTBUF_MASK	0x3
#define GPIO_INPUTBUF_EN	((2|1)<<GPIO_INPUTBUF_SHIFT)
#define GPIO_INPUTBUF_DIS	((2|0)<<GPIO_INPUTBUF_SHIFT)

#define GPIO_OUTPUT_SHIFT	9
#define GPIO_OUTPUT		(1<<GPIO_OUTPUT_SHIFT)
#define GPIO_INPUT		(0<<GPIO_OUTPUT_SHIFT)

#define GPIO_CD_SHIFT		6
#define GPIO_CD_MASK		0x7
#define GPIO_CD(x)		((((x)&GPIO_CD_MASK)|0x4)<<GPIO_CD_SHIFT)

#define GPIO_PULL_SHIFT		4
#define GPIO_PULL_MASK		0x3
#define GPIO_NOPULL		(0<<GPIO_PULL_SHIFT)
#define GPIO_PULLUP		(1<<GPIO_PULL_SHIFT)
#define GPIO_PULLDN		(2<<GPIO_PULL_SHIFT)

#define GPIO_FUNC_MASK		0xF
#define GPIO_FUNC(x)		((x)&GPIO_FUNC_MASK)


/*
 * gpio port & pin structures
 *   [31:10]: reserved
 *   [9:5] : port (A,B,C,...)
 *   [4:0] : pin number (0~31)
 */

#define GPIO_PIN_MASK		0x1F


#define GPIO_PORT_SHIFT		5
#define GPIO_PORT_MASK		(0x1F<<GPIO_PORT_SHIFT)

#define GPIO_PORT_TYPE		8 //A，B，C,E,G,H,K,MA

//(n<<GPIO_PORT_SHIFT)  n = ofset/0x40
#define GPIO_PORT_A		(0<<GPIO_PORT_SHIFT)		// offset: 0x000
#define GPIO_PORT_B		(1<<GPIO_PORT_SHIFT)		// offset: 0x040
#define GPIO_PORT_C		(2<<GPIO_PORT_SHIFT)		// offset: 0x080
//#define GPIO_PORT_D		(3<<GPIO_PORT_SHIFT)		// offset: 0x0c0
#define GPIO_PORT_E		(4<<GPIO_PORT_SHIFT)		// offset: 0x100
//#define GPIO_PORT_F		(5<<GPIO_PORT_SHIFT)		// offset: 0x140
#define GPIO_PORT_G		(6<<GPIO_PORT_SHIFT)		// offset: 0x180
//#define GPIO_PORT_SD		(9<<GPIO_PORT_SHIFT)		// offset: 0x240
#define GPIO_PORT_H		(25<<GPIO_PORT_SHIFT)		// offset: 0x640
#define GPIO_PORT_K		(26<<GPIO_PORT_SHIFT)		// offset: 0x680
#define GPIO_PORT_MA		(27<<GPIO_PORT_SHIFT)		// offset: 0x6c0 

#define TCC_GPA(x)		(GPIO_PORT_A | ((x)&0x1F))
#define TCC_GPB(x)		(GPIO_PORT_B | ((x)&0x1F))
#define TCC_GPC(x)		(GPIO_PORT_C | ((x)&0x1F))
//#define TCC_GPD(x)		(GPIO_PORT_D | ((x)&0x1F))
#define TCC_GPE(x)		(GPIO_PORT_E | ((x)&0x1F))
//#define TCC_GPF(x)		(GPIO_PORT_F | ((x)&0x1F))
#define TCC_GPG(x)		(GPIO_PORT_G | ((x)&0x1F))
#define TCC_GPH(x)		(GPIO_PORT_H | ((x)&0x1F))
#define TCC_GPK(x)		(GPIO_PORT_K | ((x)&0x1F))
//#define TCC_GPSD(x)		(GPIO_PORT_SD | ((x)&0x1F))
#define TCC_GPMA(x)		(GPIO_PORT_MA | ((x)&0x1F))


#define GPIO_LIST_NUM 6
#define GPIO_MAX_STRING 20
#define GPIO_SCAN_DATA 0
#define GPIO_SCAN_OUTPUT_ENABLE 1
#define GPIO_SCAN_INPUT_ENABLE 2
#define GPIO_SCAN_PULL_UP_DOWN 3
#define GPIO_SCAN_DRIVE_STRENGTH 4
#define GPIO_SCAN_SCHMITT 5
#define GPIO_SCAN_ALL 6


extern int32 gpio_config(uint32 port, uint32 config);
extern int32 gpio_get(uint32 port);
extern int32 gpio_set(uint32 port, uint32 data);
extern int32 gpio_to_gfb(uint32 port);
extern int32 gpio_to_num(uint32 port);//QAC
extern void gpio_scan(uint32 port, uint32 flag);
extern uint32 gpio_get_strength(uint32 port);

extern uint8 GetGpioTableSize(void);
extern uint32 GetGpioTablePort(uint8 id);

//QAC extern int32 gpio_to_F_IN_Src(uint32 port);
#endif /* __BSP_GPIO__H__ */
