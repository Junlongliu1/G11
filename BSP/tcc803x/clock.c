/****************************************************************************
 *   FileName    : clock.c
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

#include "clock.h"


static struct tcc_ckc_ops *clock_ops = NULL;

int32 tcc_set_pll(int32 id, bool en, uint32 rate, plldiv_t divide_val)
{
	int32 ret = -CKC_NO_OPS_DATA;

	if(clock_ops != NULL) {
		if (clock_ops->ckc_pll_set_rate!=0)
		{
			ret = clock_ops->ckc_pll_set_rate(id, rate);
		}
	}

	/* set clkdivc after pll set */
	if (divide_val!=0) {
		if(clock_ops != NULL) {
			if (clock_ops->ckc_plldiv_set!=0)
			{
				(void)clock_ops->ckc_plldiv_set(id, (divide_val>1)?(divide_val-1):0);
			}
		}
	}

	return ret;
}

uint32 tcc_get_pll(int32 id)
{
	if(clock_ops != NULL) {
		if (clock_ops->ckc_pll_get_rate!=0)
		{
			return clock_ops->ckc_pll_get_rate(id);
		}
	}
	return 0;
}

int32 tcc_set_clkctrl(int32 id, bool en, uint32 rate)
{
	int32 ret = -CKC_NO_OPS_DATA;

	if(clock_ops == NULL) {
		return ret;
	}

	if (en!=0) {
		if (clock_ops->ckc_swreset!=0)
		{
			(void)clock_ops->ckc_swreset(id, 0);
		}

		if (clock_ops->ckc_clkctrl_set_rate!=0) {
			ret = clock_ops->ckc_clkctrl_set_rate(id, rate);
			if (ret!=0)
			{
				return ret;
			}
		}
	}
	else {
		if (clock_ops->ckc_clkctrl_set_rate!=0) {
			ret = clock_ops->ckc_clkctrl_set_rate(id, rate);
			if (ret!=0)
			{
				return ret;
			}
		}

		if (clock_ops->ckc_swreset!=0)
		{
			(void)clock_ops->ckc_swreset(id, 1);
		}
	}
	return ret;
}

uint32 tcc_get_clkctrl(int32 id)
{
	if(clock_ops != NULL) {
		if (clock_ops->ckc_clkctrl_get_rate!=0)
		{
			return clock_ops->ckc_clkctrl_get_rate(id);
		}
	}

	return 0;
}

int32 tcc_set_peri(int32 id, bool en, uint32 rate)
{
	int32 ret = -CKC_NO_OPS_DATA;

	if(clock_ops != NULL) {
		if (clock_ops->ckc_peri_set_rate!=0) {
			if (en!=0) {
				if ((clock_ops->ckc_peri_enable!=0))
				{
					if (rate == 0) {
						return ret;
					}
					ret = clock_ops->ckc_peri_set_rate(id, rate);
					if (ret!=0) {
						return ret;
					}

					ret = clock_ops->ckc_peri_enable(id);
				}
			}
			else {
				if (clock_ops->ckc_peri_disable!=0)
				{
					ret = clock_ops->ckc_peri_disable(id);
				}
			}
		}
	}

	return ret;
}

uint32 tcc_get_peri(int32 id)
{
	if(clock_ops != NULL) {
		if (clock_ops->ckc_peri_get_rate!=0)
		{
			return clock_ops->ckc_peri_get_rate(id);
		}
	}

	return 0;
}

int32 tcc_set_iobus_pwdn(int32 id, bool pwdn)
{
	if(clock_ops != NULL) {
		if (clock_ops->ckc_iobus_pwdn!=0)
		{
			return clock_ops->ckc_iobus_pwdn(id, pwdn);
		}
	}

	return -CKC_NO_OPS_DATA;
}

int32 tcc_set_iobus_swreset(int32 id, bool reset)
{
	if(clock_ops != NULL) {
		if (clock_ops->ckc_iobus_swreset!=0)
		{
			return clock_ops->ckc_iobus_swreset(id, reset);
		}
	}

	return -CKC_NO_OPS_DATA;
}

int32 tcc_iobus_enable(int32 id)
{
	if(clock_ops != NULL) {
		if (clock_ops->ckc_iobus_pwdn!=0)
		{
			(void)clock_ops->ckc_iobus_pwdn(id, false);
		}
		if (clock_ops->ckc_iobus_swreset!=0)
		{
			(void)clock_ops->ckc_iobus_swreset(id, false);
		}
	}

        return 0;
}

void tcc_iobus_disable(int32 id)
{
	if(clock_ops != NULL) {
		if (clock_ops->ckc_iobus_swreset!=0)
		{
			(void)clock_ops->ckc_iobus_swreset(id, true);
		}
		if (clock_ops->ckc_iobus_pwdn!=0)
		{
			(void)clock_ops->ckc_iobus_pwdn(id, true);
		}
	}
}

/*
 * Register clock ops
 */
void tcc_ckc_set_ops(struct tcc_ckc_ops *ops)
{
	clock_ops = ops;
}

