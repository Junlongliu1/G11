;****************************************************************************
;   FileName    : cache.s
;   Description : 
;****************************************************************************
;
;   TCC Version 1.0
;   Copyright (c) Telechips Inc.
;   All rights reserved 
; 
;This source code contains confidential information of Telechips.
;Any unauthorized use without a written permission of Telechips including not limited to re-
;distribution in source or binary form is strictly prohibited.
;This source code is provided "AS IS" and nothing contained in this source code shall 
;constitute any express or implied warranty of any kind, including without limitation, any warranty 
;of merchantability, fitness for a particular purpose or non-infringement of any patent, copyright 
;or other third party intellectual property right. No warranty is made, express or implied, 
;regarding the information's accuracy, completeness, or performance. 
;In no event shall Telechips be liable for any claim, damages or other liability arising from, out of 
;or in connection with this source code or the use in the source code. 
;This source code is provided subject to the terms of a Mutual Non-Disclosure Agreement 
;between Telechips and Company.
;
;****************************************************************************


;********************************************************************************************************
;                                           PUBLIC FUNCTIONS
;********************************************************************************************************

	GLOBAL	MMU_EnableICache
	GLOBAL	MMU_DisableICache
	GLOBAL	MMU_EnableDCache
	GLOBAL	MMU_DisableDCache
	GLOBAL	MMU_EnableMMU
	GLOBAL	MMU_DisableMMU
	GLOBAL	MMU_InvalidateIDCache
	GLOBAL	MMU_InvalidateICache
	GLOBAL	MMU_InvalidateDCache
	GLOBAL	MMU_CleanDCacheIndex
	GLOBAL	MMU_CleanInvalidateDCacheIndex
	GLOBAL	MMU_CleanCacheRange
	GLOBAL	MMU_CleanInvalidateCacheRange
	GLOBAL	MMU_InvalidateTLB
	GLOBAL	MMU_SetTTBase
	GLOBAL	MMU_SetTTBCR
	GLOBAL	MMU_SetDomain


;********************************************************************************************************
;                                           MACROS AND DEFINIITIONS
;********************************************************************************************************

CACHE_LINE	EQU	64
R1_I		EQU	(1<<12)
R1_C		EQU	(1<<2)
R1_A		EQU	(1<<1)
R1_M		EQU	(1)
R1_iA		EQU	(1<<31)
R1_nF		EQU	(1<<30)


;********************************************************************************************************
;                                      CODE GENERATION DIRECTIVES
;********************************************************************************************************

;	RSEG CODE:CODE:NOROOT(2)
	CODE32


;********************************************************************************************************
;
;********************************************************************************************************

MMU_EnableICache
	mrc	p15,0,r0,c1,c0,0
	orr	r0,r0,#R1_I
	mcr	p15,0,r0,c1,c0,0
	mov	pc,lr


MMU_DisableICache
	mrc	p15,0,r0,c1,c0,0
	bic	r0,r0,#R1_I
	mcr	p15,0,r0,c1,c0,0
	mov	pc,lr


MMU_EnableDCache
	mrc	p15,0,r0,c1,c0,0
	orr	r0,r0,#R1_C
	mcr	p15,0,r0,c1,c0,0
	mov	pc,lr


MMU_DisableDCache
	mrc	p15,0,r0,c1,c0,0
	bic	r0,r0,#R1_C
	mcr	p15,0,r0,c1,c0,0
	mov	pc,lr


MMU_EnableMMU
	mrc	p15,0,r0,c1,c0,0
	orr	r0,r0,#R1_M
	mcr	p15,0,r0,c1,c0,0
	mov	pc,lr


MMU_DisableMMU
	mrc	p15,0,r0,c1,c0,0
	bic	r0,r0,#R1_M
	mcr	p15,0,r0,c1,c0,0
	mov	pc,lr


MMU_InvalidateIDCache
	mcr	p15,0,r0,c7,c7,0
	mov	pc,lr


MMU_InvalidateICache
	mcr	p15,0,r0,c7,c5,0
	mov	pc,lr


MMU_InvalidateDCache
	mcr	p15,0,r0,c7,c6,0
	mov	pc,lr


MMU_CleanDCacheIndex
							;r0=index
	mcr	p15,0,r0,c7,c10,2
	mov	pc,lr


MMU_CleanInvalidateDCacheIndex
							;r0=index
	mcr	p15,0,r0,c7,c14,2
	mov	pc,lr


MMU_CleanCacheRange
	stmfd	sp!, {r2, lr}
	add	r2, r0, r1				; Calculate the end address
	bic	r0,#(CACHE_LINE-1)			; Align start with cache line
0:
	mcr	p15,0,r0,c7,c10,1			; clean cache to PoC by MVA
	add	r0,r0,#CACHE_LINE
	cmp	r0,r2
	blo	0b
	mov	r0, #0
	dsb
	ldmfd	sp!, {r2, pc}


MMU_CleanInvalidateCacheRange
	stmfd  sp!, {r2, lr}
	dsb
	add	r2,r0,r1				; Calculate the end address
	bic	r0,#(CACHE_LINE-1)			; Align start with cache line
0:
	mcr	p15,0,r0,c7,c14,1			; clean & invalidate cache to PoC by MVA
	add	r0,r0,#CACHE_LINE
	cmp	r0,r2
	blo	0b
	mov	r0,#0
	dsb
	ldmfd	sp!, {r2, pc}


MMU_InvalidateTLB
	stmfd	sp!, {r0, lr}
	dsb
	isb
	mov	r0, #0
	mcr	p15,0,r0,c8,c7,0
	dsb	sy
	isb	sy
	ldmfd	sp!, {r0, pc}


MMU_SetTTBase
	mcr	p15,0,r0,c2,c0,0
	mov	pc,lr


MMU_SetTTBCR
	mcr	p15,0,r0,c2,c0,2
	mov	pc,lr


MMU_SetDomain
	mcr	p15,0,r0,c3,c0,0
	mov	pc,lr

   END
