 /*******************************************************************************
  
*   Copyright (c) Telechips Inc.
 
 
*   TCC Version 1.0
  
This source code contains confidential information of Telechips.
 
Any unauthorized use without a written permission of Telechips including not
limited to re-distribution in source or binary form is strictly prohibited.
 
This source code is provided "AS IS" and nothing contained in this source code
shall constitute any express or implied warranty of any kind, including without
limitation, any warranty of merchantability, fitness for a particular purpose
or non-infringement of any patent, copyright or other third party intellectual
property right.
No warranty is made, express or implied, regarding the information's accuracy,
completeness, or performance.
 
In no event shall Telechips be liable for any claim, damages or other
liability arising from, out of or in connection with this source code or
the use in the source code.
  
This source code is provided subject to the terms of a Mutual Non-Disclosure
Agreement between Telechips and Company.
*
*******************************************************************************/
/****************************************************************************
 *   FileName    : cache.S
 *   Description : 
 ****************************************************************************/

#define CACHE_LINE 64

//;==============
//; CPSR I,F bit
//;==============
//;int32 SET_IF(void);
//;The return value is current CPSR.
//;void WR_IF(int32 cpsrValue);
   .global WR_IF
WR_IF:
   // ;This function works only if the processor is in previliged mode.
   msr  cpsr_cxsf,r0             
   mov pc,lr



//;====================================
//; MMU Cache/TLB/etc on/off functions
//;====================================
R1_I   =    (1<<12)
R1_C   =    (1<<2)
R1_A   =    (1<<1)
R1_M   =    (1)
R1_iA  =    (1<<31)
R1_nF  =    (1<<30)

//;void MMU_EnableICache(void)
   .global MMU_EnableICache
MMU_EnableICache:        
   mrc  p15,0,r0,c1,c0,0
   orr  r0,r0,#R1_I
   mcr  p15,0,r0,c1,c0,0
   mov pc,lr

//;void MMU_DisableICache(void)
   .global MMU_DisableICache
MMU_DisableICache:       
   mrc  p15,0,r0,c1,c0,0
   bic  r0,r0,#R1_I
   mcr  p15,0,r0,c1,c0,0
   mov pc,lr

//;void MMU_EnableDCache(void)
   .global MMU_EnableDCache
MMU_EnableDCache:        
   mrc  p15,0,r0,c1,c0,0
   orr  r0,r0,#R1_C
   mcr  p15,0,r0,c1,c0,0
   mov pc,lr

//;void MMU_DisableDCache(void)
   .global MMU_DisableDCache
MMU_DisableDCache:       
   mrc  p15,0,r0,c1,c0,0
   bic  r0,r0,#R1_C
   mcr  p15,0,r0,c1,c0,0
   mov pc,lr

//;void MMU_EnableMMU(void)
   .global MMU_EnableMMU
MMU_EnableMMU:
   mrc  p15,0,r0,c1,c0,0
   orr  r0,r0,#R1_M
   mcr  p15,0,r0,c1,c0,0
   mov pc,lr

//;void MMU_DisableMMU(void)
   .global MMU_DisableMMU
MMU_DisableMMU:
   mrc  p15,0,r0,c1,c0,0
   bic  r0,r0,#R1_M
   mcr  p15,0,r0,c1,c0,0
   mov pc,lr

//;=========================
//; ICache/DCache functions
//;=========================
//;void MMU_InvalidateIDCache(void)
   .global MMU_InvalidateIDCache
MMU_InvalidateIDCache:
   mcr  p15,0,r0,c7,c7,0
   mov pc,lr

//;void MMU_InvalidateICache(void)
   .global MMU_InvalidateICache
MMU_InvalidateICache:
   mcr  p15,0,r0,c7,c5,0
   mov pc,lr

//;void MMU_InvalidateDCache(void)
   .global MMU_InvalidateDCache
MMU_InvalidateDCache:
   mcr  p15,0,r0,c7,c6,0
   mov pc,lr

//;void MMU_CleanDCacheIndex(U32 index)
   .global MMU_CleanDCacheIndex
MMU_CleanDCacheIndex:
   //;r0=index 
   mcr  p15,0,r0,c7,c10,2
   mov pc,lr

//;void MMU_CleanInvalidateDCacheIndex(U32 index) 
   .global MMU_CleanInvalidateDCacheIndex
MMU_CleanInvalidateDCacheIndex:  
   //;r0=index
   mcr  p15,0,r0,c7,c14,2
   mov pc,lr

   .global MMU_CleanCacheRange
MMU_CleanCacheRange:
   stmfd  sp!, {r2, lr}
   add  r2, r0, r1		// Calculate the end address
   bic  r0,#(CACHE_LINE-1)	// Align start with cache line
0:
   mcr  p15,0,r0,c7,c10,1	// clean cache to PoC by MVA
   add  r0,r0,#CACHE_LINE
   cmp  r0,r2
   blo  0b 
   mov  r0, #0
   dsb
   ldmfd  sp!, {r2, pc}

   .global MMU_CleanInvalidateCacheRange
MMU_CleanInvalidateCacheRange:
   stmfd  sp!, {r2, lr}
   dsb
   add  r2,r0,r1		// Calculate the end address
   bic  r0,#(CACHE_LINE-1)	// Align start with cache line
0:
   mcr  p15,0,r0,c7,c14,1	// clean & invalidate cache to PoC by MVA
   add  r0,r0,#CACHE_LINE
   cmp  r0,r2
   blo  0b
   mov  r0,#0
   dsb
   ldmfd  sp!, {r2, pc}


//;===============
//; TLB functions
//;===============
//;voic MMU_InvalidateTLB(void)
   .global MMU_InvalidateTLB
MMU_InvalidateTLB:
   stmfd sp!, {r0, lr}
   dsb
   isb
   mov  r0, #0
   mcr  p15,0,r0,c8,c7,0
   dsb	sy
   isb	sy
   ldmfd sp!, {r0, pc}

   .global MMU_SetTTBase
MMU_SetTTBase:
   mcr  p15,0,r0,c2,c0,0
   mov  pc,lr

   .global MMU_SetTTBCR
MMU_SetTTBCR:
   mcr  p15,0,r0,c2,c0,2
   mov  pc,lr

   .global MMU_SetDomain
MMU_SetDomain:
   mcr  p15,0,r0,c3,c0,0
   mov  pc,lr

   .END
