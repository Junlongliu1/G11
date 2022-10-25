/****************************************************************************
 *   FileName    : div64.c
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
//#include <stdint.h>
#include <div64.h>

u32 __attribute__((weak)) __div64_32(u64 *dividend, u32 divisor)
{
	u64 rem = *dividend;
	u64 b = divisor;
	u64 res, d = 1;
	u32 high = rem >> 32;

	/* Reduce the thing a bit first */
	res = 0;
	if (high >= divisor) {
		high /= divisor;
		res = (u64) high << 32;
		rem -= (u64) (high*divisor) << 32;
	}

	while ((((u64)(b > 0)) && (b < rem))!=0) {
		b = b+b;
		d = d+d;
	}

	do {
		if (rem >= b) {
			rem -= b;
			res += d;
		}
		b >>= 1;
		d >>= 1;
	} while (d!=0);

	*dividend = res;
	return rem;
}

