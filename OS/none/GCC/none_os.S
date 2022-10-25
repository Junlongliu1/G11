/*********************************************************************************************************
*                                          PUBLIC FUNCTIONS
*********************************************************************************************************/
                                                                @ External references.

                                                                @ Functions declared in this file.
                                                                @ Functions related to exception handling.
    .global  OS_CPU_ARM_ExceptUndefInstrHndlr
    .global  OS_CPU_ARM_ExceptSwiHndlr
    .global  OS_CPU_ARM_ExceptPrefetchAbortHndlr
    .global  OS_CPU_ARM_ExceptDataAbortHndlr
    .global  OS_CPU_ARM_ExceptIrqHndlr
    .global  OS_CPU_ARM_ExceptFiqHndlr

    .global  OS_CPU_ARM_DRegCntGet
    .global  CPU_WaitForEvent
    .global  __DSB


/*********************************************************************************************************
*                                               EQUATES
*********************************************************************************************************/

#define OS_CPU_ARM_CONTROL_INT_DIS        0xC0                     // Disable both FIQ and IRQ.
#define OS_CPU_ARM_CONTROL_FIQ_DIS        0x40                     // Disable FIQ.
#define OS_CPU_ARM_CONTROL_IRQ_DIS        0x80                     // Disable IRQ.
#define OS_CPU_ARM_CONTROL_THUMB          0x20                     // Set THUMB mode.
#define OS_CPU_ARM_CONTROL_ARM            0x00                     // Set ARM mode.

#define OS_CPU_ARM_MODE_MASK              0x1F
#define OS_CPU_ARM_MODE_USR               0x10
#define OS_CPU_ARM_MODE_FIQ               0x11
#define OS_CPU_ARM_MODE_IRQ               0x12
#define OS_CPU_ARM_MODE_SVC               0x13
#define OS_CPU_ARM_MODE_ABT               0x17
#define OS_CPU_ARM_MODE_UND               0x1B
#define OS_CPU_ARM_MODE_SYS               0x1F

#define OS_CPU_ARM_EXCEPT_RESET           0x00
#define OS_CPU_ARM_EXCEPT_UNDEF_INSTR     0x01
#define OS_CPU_ARM_EXCEPT_SWI             0x02
#define OS_CPU_ARM_EXCEPT_PREFETCH_ABORT  0x03
#define OS_CPU_ARM_EXCEPT_DATA_ABORT      0x04
#define OS_CPU_ARM_EXCEPT_ADDR_ABORT      0x05
#define OS_CPU_ARM_EXCEPT_IRQ             0x06
#define OS_CPU_ARM_EXCEPT_FIQ             0x07

#define OS_CPU_ARM_FPEXC_EN               0x40000000


/*********************************************************************************************************
*                                     CODE GENERATION DIRECTIVES
*********************************************************************************************************/

//    RSEG CODE:CODE:NOROOT(2)
//    AAPCS INTERWORK
//    PRESERVE8
//    REQUIRE8
//    CODE32

/*********************************************************************************************************
*                               UNDEFINED INSTRUCTION EXCEPTION HANDLER
*
* Register Usage:  R0     Exception Type
*                  R1
*                  R2     Return PC
*********************************************************************************************************/

OS_CPU_ARM_ExceptUndefInstrHndlr:
                                                                @ LR offset to return from this exception:  0.
    STMFD   SP!, {R0-R3}                                        @ Push working registers.
    MOV     R2, LR                                              @ Save link register.
    MOV     R0, #OS_CPU_ARM_EXCEPT_UNDEF_INSTR                  @ Set exception ID to OS_CPU_ARM_EXCEPT_UNDEF_INSTR.
    B            OS_CPU_ARM_ExceptHndlr                         @ Branch to global exception handler.


/*********************************************************************************************************
*                                SOFTWARE INTERRUPT EXCEPTION HANDLER
*
* Register Usage:  R0     Exception Type
*                  R1
*                  R2     Return PC
*********************************************************************************************************/

OS_CPU_ARM_ExceptSwiHndlr:
                                                                @ LR offset to return from this exception:  0.
    STMFD   SP!, {R0-R3}                                        @ Push working registers.
    MOV     R2, LR                                              @ Save link register.
    MOV     R0, #OS_CPU_ARM_EXCEPT_SWI                          @ Set exception ID to OS_CPU_ARM_EXCEPT_SWI.
    B            OS_CPU_ARM_ExceptHndlr                         @ Branch to global exception handler.


/*********************************************************************************************************
*                                  PREFETCH ABORT EXCEPTION HANDLER
*
* Register Usage:  R0     Exception Type
*                  R1
*                  R2     Return PC
*********************************************************************************************************/

OS_CPU_ARM_ExceptPrefetchAbortHndlr:
    SUB     LR, LR, #4                                          @ LR offset to return from this exception: -4.
    STMFD   SP!, {R0-R3}                                        @ Push working registers.
    MOV     R2, LR                                              @ Save link register.
    MOV     R0, #OS_CPU_ARM_EXCEPT_PREFETCH_ABORT               @ Set exception ID to OS_CPU_ARM_EXCEPT_PREFETCH_ABORT.
    B            OS_CPU_ARM_ExceptHndlr                         @ Branch to global exception handler.


/*********************************************************************************************************
*                                    DATA ABORT EXCEPTION HANDLER
*
* Register Usage:  R0     Exception Type
*                  R1
*                  R2     Return PC
*********************************************************************************************************/

OS_CPU_ARM_ExceptDataAbortHndlr:
    SUB     LR, LR, #8                                          @ LR offset to return from this exception: -8.
    STMFD   SP!, {R0-R3}                                        @ Push working registers.
    MOV     R2, LR                                              @ Save link register.
    MOV     R0, #OS_CPU_ARM_EXCEPT_DATA_ABORT                   @ Set exception ID to OS_CPU_ARM_EXCEPT_DATA_ABORT.
    B            OS_CPU_ARM_ExceptHndlr                         @ Branch to global exception handler.


/*********************************************************************************************************
*                                   ADDRESS ABORT EXCEPTION HANDLER
*
* Register Usage:  R0     Exception Type
*                  R1
*                  R2     Return PC
*********************************************************************************************************/

OS_CPU_ARM_ExceptAddrAbortHndlr:
    SUB     LR, LR, #8                                          @ LR offset to return from this exception: -8.
    STMFD   SP!, {R0-R3}                                        @ Push working registers.
    MOV     R2, LR                                              @ Save link register.
    MOV     R0, #OS_CPU_ARM_EXCEPT_ADDR_ABORT                   @ Set exception ID to OS_CPU_ARM_EXCEPT_ADDR_ABORT.
    B            OS_CPU_ARM_ExceptHndlr                         @ Branch to global exception handler.


/*********************************************************************************************************
*                                 INTERRUPT REQUEST EXCEPTION HANDLER
*
* Register Usage:  R0     Exception Type
*                  R1
*                  R2     Return PC
*********************************************************************************************************/

OS_CPU_ARM_ExceptIrqHndlr:
    SUB     LR, LR, #4                                          @ LR offset to return from this exception: -4.
    STMFD   SP!, {R0-R3}                                        @ Push working registers.
    MOV     R2, LR                                              @ Save link register.
    MOV     R0, #OS_CPU_ARM_EXCEPT_IRQ                          @ Set exception ID to OS_CPU_ARM_EXCEPT_IRQ.
    B            OS_CPU_ARM_ExceptHndlr                         @ Branch to global exception handler.


/*********************************************************************************************************
*                              FAST INTERRUPT REQUEST EXCEPTION HANDLER
*
* Register Usage:  R0     Exception Type
*                  R1
*                  R2     Return PC
*********************************************************************************************************/

OS_CPU_ARM_ExceptFiqHndlr:
    SUB     LR, LR, #4                                          @ LR offset to return from this exception: -4.
    STMFD   SP!, {R0-R3}                                        @ Push working registers.
    MOV     R2, LR                                              @ Save link register.
    MOV     R0, #OS_CPU_ARM_EXCEPT_FIQ                          @ Set exception ID to OS_CPU_ARM_EXCEPT_FIQ.
    B            OS_CPU_ARM_ExceptHndlr                         @ Branch to global exception handler.


/*********************************************************************************************************
*                                      GLOBAL EXCEPTION HANDLER
*
*********************************************************************************************************/

OS_CPU_ARM_ExceptHndlr:

     BX      LR


/*********************************************************************************************************
*                              VFP/NEON REGISTER COUNT
*
* Register Usage:  R0     Double Register Count
*********************************************************************************************************/

OS_CPU_ARM_DRegCntGet:
    MOV     R0, #0
    BX      LR


CPU_WaitForEvent:
        DSB
        WFE                                     @ Wait for exception
        BX      LR


__DSB:
        DSB
        ISB
        BX      LR

    .end
