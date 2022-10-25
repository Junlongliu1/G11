/****************************************************************************
 *   FileName    : types.c
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

#ifndef __BSP_TYPES_H__
#define __BSP_TYPES_H__

//#include <stddef.h>
//#include <stdint.h>

#ifndef __cplusplus
#define false 0
#define true 1
typedef int bool;
#endif

#ifndef NULL
#define NULL	(0)
#endif

#ifndef NULL_PTR
#define NULL_PTR ((void *) 0)
#endif

#if 0
typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef uintptr_t addr_t;
typedef uintptr_t vaddr_t;
typedef uintptr_t paddr_t;
#else
typedef char int8;
typedef short int16;
typedef int int32;
typedef long long int64;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

typedef unsigned int addr_t;
typedef unsigned int vaddr_t;
typedef unsigned int paddr_t;

#endif

#ifndef size_t
typedef uint32 size_t;
#endif

//typedef uint32 uint;

#define ARRAY_SIZE(x) (sizeof(x)/sizeof((x)[0]))

#endif /* __BSP_TYPES_H__ */
