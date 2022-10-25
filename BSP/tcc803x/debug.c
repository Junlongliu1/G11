/****************************************************************************
 *   FileName    : debug.c
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
#include "uart.h"
#include <debug.h>

#if DEBUG_ENABLE
#define	is_digit(c)     (((c) >= '0') && ((c) <= '9'))
#define	to_upper(c)	((c) & ~0x20)
#define	s_put_ch(c) do { *dest++ = (c);  } while (0)

typedef	void (fPUTC)(int8 c);

//static int32 _dgetc(int8 *c);
static int32 prt8(uint32 num, int8 *str);
static int32 prt10(uint32 num, int8 *str);
static int32 prt16(uint32 num, int8 *str);
static int32 PRT16(uint32 num, int8 *str);

/*------- external -----------*/
int32 _dputc(int8 c) /* QAC - MISRA-C:2004 Rule 8.8 - This function is used in ASM */
{
	(void) console_putc(DEBUG_UART, c);
	
	return 0;
}

/* GHC Warning : declared but never referenced
static int32 _dgetc(int8 *c)
{
	int32 n;
	n = console_getc(DEBUG_UART, 0);
	if (n < 0) {
		return -1;
	} else {
		*c = (int8)(n&0xFF);
		return 0;
	}
}
*/

/*
 * print character
 */
static int32 prt8(uint32 num, int8 *str)
{
	int32	len;
	int8	temp[20];
	int8	*p	= temp;

	*p		= 0;
	len		= 0;
	do {
		++p;
		*p      = (int8)((num & 7) + '0');
		num		= num >> 3;
		len		++;
	} while (num!=0);

	while(1)
	{
		*str    = *p;
		str++;
		if((*p) == 0) {
			break;
		}
		p--;
	}

	return	len;
}

/*
 * print decimal
 */
static int32 prt10(uint32 num, int8 *str)
{
	int32	len;
	int8	temp[20];
	int8	*p	= temp;

	*p		= 0;
	len		= 0;
	do {
		++p;
		*p      = (int8)((num % 10) + '0');
		num		/= 10;
		len		++;
	} while (num != 0);

	while(1)
	{
		*str    = *p;
		str++;
		if((*p) == 0) {
			break;
		}
		p--;
	}

	return	len;
}

/*
 * print hexa-decimal, lower case
 */
static int32 prt16(uint32 num, int8 *str)
{
	int32		len;
	int8	temp[20];
	int8	*p	= temp;

	*p		= 0;
	len		= 0;
	do {
		++p;
		*p      = "0123456789abcdef"[num & 0x0f];
		num		= num >> 4;
		len		++;
	} while (num !=0);

	while(1)
	{
		*str    = *p;
		str++;
		if((*p) == 0) {
			break;
		}
		p--;
	}

	return	len;
}

/*
 * print hexa-decimal, upper case
 */
static int32 PRT16(uint32 num, int8 *str)
{
	int32		len;
	int8	temp[20];
	int8	*p	= temp;

	*p		= 0;
	len		= 0;
	do {
		++p;
		*p      = "0123456789ABCDEF"[num & 0x0f];
		num		= num >> 4;
		len		++;
	} while (num != 0);

	while(1)
	{
		*str    = *p;
		str++;
		if((*p) == 0) {
			break;
		}
		p--;
	}

	return	len;
}

/*
 *
 */
int32 d_printfi(fPUTC *fputc, const int8 *fmt, const int32 *args) /* QAC - MISRA-C:2004 Rule 8.8 - This function is used in ASM */
{
	int32           i, f;
	int8 c;
	int32           length, leftjust = 0, leading;
	int32	fmax, fmin;
	int8	sign, fill;
	int8	*str, string[64];

	for(;;) {
		/* Echo character until '%' or end of fmt string */
		while ((*fmt) != '%' ) {
			c = *fmt;
			fmt++;
			if (!c)
			{
				return (-1);
			}
			(*fputc)(c);
		}
		fmt++;
		/* Echo " ... %%..." as '%'  */
		if (*fmt == '%') {
			(*fputc)(*fmt);
			fmt++;
			continue;
		}
		/* Check for "%-...",== Left-justified output */
		if ((*fmt == '-') != 0)
		{
			leftjust = (*fmt == '-');
			fmt ++;
		}
		else
		{
			leftjust = (*fmt == '-');
		}
		/* Allow for zero-filled numeric outputs ("%0...") */
		if (*fmt =='0')
		{
			fill = *fmt;
			fmt++;
		}
		else
		{
			fill = ' ';
		}
		/* Allow for minimum field width specifier for %d %u x,c,o,s */
		/* Also  allow %* for variable width (%0* as well)   */
		fmin = 0;
		if (*fmt == '*') {			
			fmin = *args;
			args++;
			++fmt;
		} else
		{
			while (is_digit(*fmt) != 0)
			{
				fmin = (((fmin * 10) + (*fmt)) - '0') ;
				fmt++;
			}
		}
		/* Allow for maximum string width for %s */
		fmax = 0;
		if (*fmt == '.') {
			++fmt;
			if (*fmt == '*' ) {				
				fmax = *args;
				args++;
				++fmt;
			} 
			else
			{
				while (is_digit(*fmt) != 0)
				{
					fmax = (((fmax * 10) + (*fmt)) - '0');
					fmt++;
				}
			}
		}		
		if ((*fmt) == 0) {
			f = *fmt;
			fmt++;
			(*fputc)('%');
			return (-1);
		}
		else
		{
			f = *fmt;
			fmt++;
		}
		sign = 0;
		switch (f) {
		case 'c' :
			string[0]	= (int8) *args;
			string[1]	= '\0';
			str			= string;
			length		= 1;
			fmax		= 0;
			fill		= ' ';
			break;
		case 's' :
			str			= (int8 *) *args;
			for (length = 0; ; length ++)
			{				
				if(*str == 0)
				{
					break;
				}
				str++;
			}
			str++;
			str			= (int8 *) *args;
			fill		= ' ';
			break;
		case 'd' :
			str			= string;
			i			= *(const int32 *)args;
			if (i < 0)
			{
				sign	= '-';
				i		= -i;
			}
			length		= prt10(i, str);
			fmax		= 0;
			break;
		case 'o' :
			str			= string;
			length		= prt8(*(const int32 *)args, str);
			fmax		= 0;
			break;
		case 'x' :
			str			= string;
			length		= prt16(*(const int32 *)args, str);
			fmax		= 0;
			break;
		case 'X' :
			str			= string;
			length		= PRT16(*(const int32 *)args, str);
			fmax		= 0;
			break;
		default:
			/* format is not supported */
			str			= (int8 *) *args;
			length		= 1;
			break;
		}
	//	if (fmin > uDBG_txbuf_len)
	//		fmin = 0;
	//	if (fmax > uDBG_txbuf_len)
	//		fmax = 0;

		leading = 0;
		if ((fmax != 0) || (fmin!=0)) {
			if (fmax != 0)
			{
				if (length > fmax)
				{
					length	= fmax;
				}
			}
			if (fmin!=0)
			{
				leading		= fmin - length;
			}
			#if 1
			{
				if (sign == '-')
				{
					leading --;
				}
			}
			#endif
		}
		{
			if ((sign == '-') && (fill == '0'))
			{
				(*fputc)(sign);
			}
		}
		if (!leftjust)
		{
			for (i = 0; i < leading; i ++)
			{
				(*fputc)(fill);
			}
		}
		{
			if ((sign == '-') && (fill == ' '))
			{
				(*fputc)(sign);
			}
		}
		for (i = 0; i < length; i ++)
		{			
			(*fputc)(*str);
			str++;
		}
		if (leftjust != 0)
		{
			for (i = 0; i < leading; i ++)
			{
				(*fputc)(fill);
			}
		}
		args ++;
	}
}

#if defined(__GNUC__)
__attribute__ ((naked)) void IO_DBG_SerialPrintf_(int8 *format, ...) /* QAC - MISRA-C:2004 Rule 16.1 - Printf() needs variable parameters */
{
	__asm volatile (
			"stmdb	sp!, {r1-r3}	\n"
			"mov	r1, r0			\n"		/* r1 = format string */
			"mov	r2, sp			\n"		/* r2 = arguments */
			"ldr	r0, =_dputc		\n"		/* r0 = function pointer which will put character */
			"str	lr, [sp, #-4]!	\n"		/* save r14 */
			"bl		d_printfi		\n"
			"ldr	lr, [sp], #4	\n"		/* restore r14 */
			"add	sp, sp, #12		\n"		/* restore stack */
			"mov	pc, lr			\n"
			:::"r0", "r1", "r2", "r3" \
	);
}
#else //Green Hills case
void IO_DBG_SerialPrintf_(int8 *format, ...) /* QAC - MISRA-C:2004 Rule 16.1 - Printf() needs variable parameters */
{
	#pragma asm
	mov	r1, r0 
	mov	r2, sp
	add	r2, r2, #8
	ldr	r0, =_dputc
	str	lr, [sp, #-4]!
	bl	d_printfi
	ldr	lr, [sp], #4
	add	sp, sp, #20
	mov	pc, lr
	#pragma endasm
}
#endif
#else
#if defined(__GNUC__)
__attribute__ ((naked)) void IO_DBG_SerialPrintf_(int8 *format, ...)
{

}
#else
void IO_DBG_SerialPrintf_(int8 *format, ...) // GHC Warning : "naked" ignored
{

}
#endif
#endif
