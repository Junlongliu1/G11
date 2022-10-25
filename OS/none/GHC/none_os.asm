/*********************************************************************************************************
*                                          PUBLIC FUNCTIONS
*********************************************************************************************************/
                                                                @ External references.

                                                                @ Functions declared in this file.
                                                                @ Functions related to exception handling.
    GLOBAL  OS_CPU_ARM_ExceptUndefInstrHndlr
    GLOBAL  OS_CPU_ARM_ExceptSwiHndlr
    GLOBAL  OS_CPU_ARM_ExceptPrefetchAbortHndlr
    GLOBAL  OS_CPU_ARM_ExceptDataAbortHndlr
    GLOBAL  OS_CPU_ARM_ExceptIrqHndlr
    GLOBAL  OS_CPU_ARM_ExceptFiqHndlr

    GLOBAL  OS_CPU_ARM_DRegCntGet
    GLOBAL  CPU_WaitForEvent
    GLOBAL  __DSB


;********************************************************************************************************
;                                               EQUATES
;********************************************************************************************************

OS_CPU_ARM_CONTROL_INT_DIS        EQU  0xC0                     ; Disable both FIQ and IRQ.
OS_CPU_ARM_CONTROL_FIQ_DIS        EQU  0x40                     ; Disable FIQ.
OS_CPU_ARM_CONTROL_IRQ_DIS        EQU  0x80                     ; Disable IRQ.
OS_CPU_ARM_CONTROL_THUMB          EQU  0x20                     ; Set THUMB mode.
OS_CPU_ARM_CONTROL_ARM            EQU  0x00                     ; Set ARM mode.

OS_CPU_ARM_MODE_MASK              EQU  0x1F
OS_CPU_ARM_MODE_USR               EQU  0x10
OS_CPU_ARM_MODE_FIQ               EQU  0x11
OS_CPU_ARM_MODE_IRQ               EQU  0x12
OS_CPU_ARM_MODE_SVC               EQU  0x13
OS_CPU_ARM_MODE_ABT               EQU  0x17
OS_CPU_ARM_MODE_UND               EQU  0x1B
OS_CPU_ARM_MODE_SYS               EQU  0x1F

OS_CPU_ARM_EXCEPT_RESET           EQU  0x00
OS_CPU_ARM_EXCEPT_UNDEF_INSTR     EQU  0x01
OS_CPU_ARM_EXCEPT_SWI             EQU  0x02
OS_CPU_ARM_EXCEPT_PREFETCH_ABORT  EQU  0x03
OS_CPU_ARM_EXCEPT_DATA_ABORT      EQU  0x04
OS_CPU_ARM_EXCEPT_ADDR_ABORT      EQU  0x05
OS_CPU_ARM_EXCEPT_IRQ             EQU  0x06
OS_CPU_ARM_EXCEPT_FIQ             EQU  0x07

OS_CPU_ARM_FPEXC_EN               EQU  0x40000000


;********************************************************************************************************
;                                     CODE GENERATION DIRECTIVES
;********************************************************************************************************

    ;RSEG CODE:CODE:NOROOT(2)
    ;AAPCS INTERWORK
    ;PRESERVE8
    ;REQUIRE8
    CODE32


;********************************************************************************************************
;                               UNDEFINED INSTRUCTION EXCEPTION HANDLER
;
; Register Usage:  R0     Exception Type
;                  R1
;                  R2     Return PC
;********************************************************************************************************

OS_CPU_ARM_ExceptUndefInstrHndlr
                                                                ; LR offset to return from this exception:  0.
    STMFD   SP!, {R0-R3}                                        ; Push working registers.
    MOV     R2, LR                                              ; Save link register.
    MOV     R0, #OS_CPU_ARM_EXCEPT_UNDEF_INSTR                  ; Set exception ID to OS_CPU_ARM_EXCEPT_UNDEF_INSTR.
    B            OS_CPU_ARM_ExceptHndlr                         ; Branch to global exception handler.


;********************************************************************************************************
;                                SOFTWARE INTERRUPT EXCEPTION HANDLER
;
; Register Usage:  R0     Exception Type
;                  R1
;                  R2     Return PC
;********************************************************************************************************

OS_CPU_ARM_ExceptSwiHndlr
                                                                ; LR offset to return from this exception:  0.
    STMFD   SP!, {R0-R3}                                        ; Push working registers.
    MOV     R2, LR                                              ; Save link register.
    MOV     R0, #OS_CPU_ARM_EXCEPT_SWI                          ; Set exception ID to OS_CPU_ARM_EXCEPT_SWI.
    B            OS_CPU_ARM_ExceptHndlr                         ; Branch to global exception handler.


;********************************************************************************************************
;                                  PREFETCH ABORT EXCEPTION HANDLER
;
; Register Usage:  R0     Exception Type
;                  R1
;                  R2     Return PC
;********************************************************************************************************

OS_CPU_ARM_ExceptPrefetchAbortHndlr
    SUB     LR, LR, #4                                          ; LR offset to return from this exception: -4.
    STMFD   SP!, {R0-R3}                                        ; Push working registers.
    MOV     R2, LR                                              ; Save link register.
    MOV     R0, #OS_CPU_ARM_EXCEPT_PREFETCH_ABORT               ; Set exception ID to OS_CPU_ARM_EXCEPT_PREFETCH_ABORT.
    B            OS_CPU_ARM_ExceptHndlr                         ; Branch to global exception handler.


;********************************************************************************************************
;                                    DATA ABORT EXCEPTION HANDLER
;
; Register Usage:  R0     Exception Type
;                  R1
;                  R2     Return PC
;********************************************************************************************************

OS_CPU_ARM_ExceptDataAbortHndlr
    SUB     LR, LR, #8                                          ; LR offset to return from this exception: -8.
    STMFD   SP!, {R0-R3}                                        ; Push working registers.
    MOV     R2, LR                                              ; Save link register.
    MOV     R0, #OS_CPU_ARM_EXCEPT_DATA_ABORT                   ; Set exception ID to OS_CPU_ARM_EXCEPT_DATA_ABORT.
    B            OS_CPU_ARM_ExceptHndlr                         ; Branch to global exception handler.


;********************************************************************************************************
;                                   ADDRESS ABORT EXCEPTION HANDLER
;
; Register Usage:  R0     Exception Type
;                  R1
;                  R2     Return PC
;********************************************************************************************************

OS_CPU_ARM_ExceptAddrAbortHndlr
    SUB     LR, LR, #8                                          ; LR offset to return from this exception: -8.
    STMFD   SP!, {R0-R3}                                        ; Push working registers.
    MOV     R2, LR                                              ; Save link register.
    MOV     R0, #OS_CPU_ARM_EXCEPT_ADDR_ABORT                   ; Set exception ID to OS_CPU_ARM_EXCEPT_ADDR_ABORT.
    B            OS_CPU_ARM_ExceptHndlr                         ; Branch to global exception handler.


;********************************************************************************************************
;                                 INTERRUPT REQUEST EXCEPTION HANDLER
;
; Register Usage:  R0     Exception Type
;                  R1
;                  R2     Return PC
;********************************************************************************************************

OS_CPU_ARM_ExceptIrqHndlr
    SUB     LR, LR, #4                                          ; LR offset to return from this exception: -4.
    STMFD   SP!, {R0-R3}                                        ; Push working registers.
    MOV     R2, LR                                              ; Save link register.
    MOV     R0, #OS_CPU_ARM_EXCEPT_IRQ                          ; Set exception ID to OS_CPU_ARM_EXCEPT_IRQ.
    B            OS_CPU_ARM_ExceptHndlr                         ; Branch to global exception handler.


;********************************************************************************************************
;                              FAST INTERRUPT REQUEST EXCEPTION HANDLER
;
; Register Usage:  R0     Exception Type
;                  R1
;                  R2     Return PC
;********************************************************************************************************

OS_CPU_ARM_ExceptFiqHndlr
    SUB     LR, LR, #4                                          ; LR offset to return from this exception: -4.
    STMFD   SP!, {R0-R3}                                        ; Push working registers.
    MOV     R2, LR                                              ; Save link register.
    MOV     R0, #OS_CPU_ARM_EXCEPT_FIQ                          ; Set exception ID to OS_CPU_ARM_EXCEPT_FIQ.
    B            OS_CPU_ARM_ExceptHndlr                         ; Branch to global exception handler.


;********************************************************************************************************
;                                      GLOBAL EXCEPTION HANDLER
;
;********************************************************************************************************

OS_CPU_ARM_ExceptHndlr

     BX      LR


;********************************************************************************************************
;                              VFP/NEON REGISTER COUNT
;
; Register Usage:  R0     Double Register Count
;********************************************************************************************************

OS_CPU_ARM_DRegCntGet
    MOV     R0, #0
    BX      LR

CPU_WaitForEvent
        DSB
        WFE                                     ; Wait for exception
        BX      LR


__DSB
        DSB
        ISB
        BX      LR

    END
