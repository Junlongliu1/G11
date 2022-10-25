/****************************************************************************
 *   FileName    : clock.h
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

#ifndef __CLOCK_H__
#define __CLOCK_H__

#include "types.h"
#include "tcc_ckc.h"

#ifndef ENABLE
#define ENABLE 1
#endif
#ifndef DISABLE
#define DISABLE 0
#endif

#define CKC_MHZ(x)	(uint32)((x) * 1000000)
#define CKC_GET_RATE(pri_t)	tcc_get_peri((uint32)(pri_t))
#define CKC_SET_EN(id,rate)	tcc_set_peri((uint32)(id), ENABLE, (rate))
#define CKC_SET_DIS(id,rate)	 tcc_set_peri((uint32)(id), DISABLE, (rate))

enum {
	CKC_OK = 0,
	CKC_FAILED =1,
	CKC_INVALIED_SRC =2,
	CKC_NO_OPS_DATA = 10,
};

typedef enum {
	PLLDIV_0 = 0,	/* Disable */
	PLLDIV_2 = 2,	/* 1/2 */
	PLLDIV_3 =3,    /* 1/3 */
	PLLDIV_4 =4,    /* 1/4 */
} plldiv_t;

struct tcc_ckc_ops {
	/* software reset */
	int32 (*ckc_swreset)(int32 id, bool reset);

	/* pll */
	int32 (*ckc_pll_set_rate)(int32 id, uint32 rate);
	uint32 (*ckc_pll_get_rate)(int32 id);
	int32 (*ckc_plldiv_set)(int32 id, uint32 plldiv);

	/* clkctrl */
	int32 (*ckc_clkctrl_set_rate)(int32 id, uint32 rate);
	uint32 (*ckc_clkctrl_get_rate)(int32 id);

	/* peripheral */
	int32 (*ckc_peri_enable)(int32 id);
	int32 (*ckc_peri_disable)(int32 id);
	int32 (*ckc_peri_set_rate)(int32 id, uint32 rate);
	uint32 (*ckc_peri_get_rate)(int32 id);
	int32 (*ckc_is_peri_enabled)(int32 id);

	/* io bus */
	int32 (*ckc_iobus_pwdn)(int32 id, bool pwdn);
	int32 (*ckc_is_iobus_pwdn)(int32 id);
	int32 (*ckc_iobus_swreset)(int32 id, bool reset);
};

extern void tcc_ckc_set_ops(struct tcc_ckc_ops *ops);

extern int32 tcc_set_pll(int32 id, bool en, uint32 rate, plldiv_t divide_val);
extern uint32 tcc_get_pll(int32 id);
extern int32 tcc_set_clkctrl(int32 id, bool en, uint32 rate);
extern uint32 tcc_get_clkctrl(int32 id);
extern int32 tcc_set_peri(int32 id, bool en, uint32 rate);
extern uint32 tcc_get_peri(int32 id);
extern int32 tcc_set_iobus_pwdn(int32 id, bool pwdn);
extern int32 tcc_set_iobus_swreset(int32 id, bool reset);
extern int32 tcc_iobus_enable(int32 id);
extern void tcc_iobus_disable(int32 id);

#endif /* __CLOCK_H__ */
