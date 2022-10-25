/****************************************************************************
 *   FileName    : cache.h
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

#ifndef __BSP_ARM_CACHE_H__
#define __BSP_ARM_CACHE_H__

void MMU_EnableICache(void);
void MMU_DisableICache(void);
void MMU_EnableDCache(void);
void MMU_DisableDCache(void);
void MMU_EnableMMU(void);
void MMU_DisableMMU(void);

void MMU_InvalidateIDCache(void);
void MMU_InvalidateICache(void);
void MMU_InvalidateDCache(void);
void MMU_CleanDCacheIndex(uint32 idx);
void MMU_CleanInvalidateDCacheIndex(uint32 idx);

void MMU_InvalidateTLB(void);
void MMU_SetTTBase(uint32 addr);
void MMU_SetTTBCR(uint32 val);
void MMU_SetDomain(uint32 val);

void MMU_CleanCacheRange(uint32, uint32);
void MMU_CleanInvalidateCacheRange(uint32 a, uint32 b);

void ARM_DSB(void);

/*__BSP_ARM_CACHE_H__*/
#endif