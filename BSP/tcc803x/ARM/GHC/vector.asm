;********************************************************************************************************
;
; vector.asm
; Telechips TCC803x SoCs Cortex-R5 Single Processor
;
; History
; -------------
; Created by : SangWon, Lee  2016/8/19
;
;********************************************************************************************************


#define REMAP_MICOM_BASE	0x1B936014
#define COLD_RESET_BASE	0x1B93617C
;********************************************************************************************************
;                                      CODE GENERATION DIRECTIVES
;********************************************************************************************************

;	RSEG CODE:CODE:NOROOT(2)
	CODE32

	GLOBAL	_vector
	GLOBAL	arm_reserved
	EXTERN	reset

_vector
	ldr pc, =reset_handler
	ldr pc, =ARM_ExceptUndefInstrHndlr
	ldr pc, =ARM_ExceptSwiHndlr
	ldr pc, =ARM_ExceptPrefetchAbortHndlr
	ldr pc, =ARM_ExceptDataAbortHndlr
	ldr pc, =arm_reserved
	ldr pc, =ARM_ExceptIrqHndlr
	ldr pc, =ARM_ExceptFiqHndlr

	
reset_handler
	@ldr	r0, =0x1B93617C
	@mov	r1, #0x1
	@str	r1, [r0]

	ldr pc, = reset

arm_reserved
	wfi
	b arm_reserved

	END
