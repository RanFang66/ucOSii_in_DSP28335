;
;********************************************************************************************************
;                                                uC/OS-II
;                                          The Real-Time Kernel
;
;
;                         (c) Copyright 2009-2015; Micrium, Inc.; Weston, FL
;                    All rights reserved.  Protected by international copyright laws.
;
;                                       ASSEMBLY LANGUAGE MACROS
;                                             TI C28x Port
;
; File      : OS_CPU_I.ASM
; Version   : V2.92.11
; By        : JFT
;
; LICENSING TERMS:
; ---------------
;           uC/OS-II is provided in source form for FREE short-term evaluation, for educational use or
;           for peaceful research.  If you plan or intend to use uC/OS-II in a commercial application/
;           product then, you need to contact Micrium to properly license uC/OS-II for its use in your
;           application/product.   We provide ALL the source code for your convenience and to help you
;           experience uC/OS-II.  The fact that the source is provided does NOT mean that you can use
;           it commercially without paying a licensing fee.
;
;           Knowledge of the source code may NOT be used to develop a similar product.
;
;           Please help us continue to provide the embedded community with the finest software available.
;           Your honesty is greatly appreciated.
;
;           You can contact us at www.micrium.com, or by phone at +1 (954) 217-2036.
;
; For       : TI C28x
; Mode      : C28 Object mode
; Toolchain : TI C/C++ Compiler
;********************************************************************************************************
;

;********************************************************************************************************
;                                             OS_CTX_SAVE
;
; Description : This MACRO saves the CPU registers (i.e. CPU context) onto the current task's stack using
;               the same order as they were saved in OSTaskStkInit().
;********************************************************************************************************

OS_CTX_SAVE  .macro
                                                                ; Save remaining registers.
    PUSH    AR1H:AR0H                                           ;   AR1H:AR0H
    PUSH    XAR2                                                ;   XAR2
    PUSH    XAR3                                                ;   XAR3
    PUSH    XAR4                                                ;   XAR4
    PUSH    XAR5                                                ;   XAR5
    PUSH    XAR6                                                ;   XAR6
    PUSH    XAR7                                                ;   XAR7
    PUSH    XT                                                  ;   XT
    PUSH    RPC                                                 ;   RPC

 .if .TMS320C2800_FPU32 = 1                                     ; Save FPU registers, if enabled.
    MOVZ    AR1   , @SP                                         ;   Can't use SP for indirect addressing.
    MOV32  *XAR1++, R0H                                         ;   R0H
    MOV32  *XAR1++, R1H                                         ;   R1H
    MOV32  *XAR1++, R2H                                         ;   R2H
    MOV32  *XAR1++, R3H                                         ;   R3H
    MOV32  *XAR1++, R4H                                         ;   R4H
    MOV32  *XAR1++, R5H                                         ;   R5H
    MOV32  *XAR1++, R6H                                         ;   R6H
    MOV32  *XAR1++, R7H                                         ;   R7H
    MOV32  *XAR1++, STF                                         ;   STF
    MOV     AL    , AR1                                         ;   Can't use indirect addressing with RB.
    MOV     @SP   , AL                                          ;   Restore SP.
    PUSH    RB                                                  ;   RB
 .endif
    .endm


;********************************************************************************************************
;                                           OS_CTX_RESTORE
;
; Description : This MACRO restores the CPU registers (i.e. context) from the new task's stack in the
;               reverse order of OS_CTX_SAVE (see above)
;********************************************************************************************************

OS_CTX_RESTORE  .macro
 .if .TMS320C2800_FPU32 = 1                                     ; Restore FPU registers, if enabled.
    POP     RB                                                  ;   RB
    MOVZ    AR1, @SP                                            ;   Can't use SP for indirect addressing.
    MOV32   STF, *--XAR1                                        ;   STF
    MOV32   R7H, *--XAR1                                        ;   R7H
    MOV32   R6H, *--XAR1                                        ;   R6H
    MOV32   R5H, *--XAR1                                        ;   R5H
    MOV32   R4H, *--XAR1                                        ;   R4H
    MOV32   R3H, *--XAR1                                        ;   R3H
    MOV32   R2H, *--XAR1                                        ;   R2H
    MOV32   R1H, *--XAR1                                        ;   R1H
    MOV32   R0H, *--XAR1                                        ;   R0H
    MOV     AL    , AR1                                         ;   Restore SP.
    MOV     @SP   , AL
 .endif
                                                                ; Restore CPU registers part 1.
    POP     RPC                                                 ;   RPC
    POP     XT                                                  ;   XT
    POP     XAR7                                                ;   XAR7
    POP     XAR6                                                ;   XAR6
    POP     XAR5                                                ;   XAR5
    POP     XAR4                                                ;   XAR4
    POP     XAR3                                                ;   XAR3
    POP     XAR2                                                ;   XAR2

                                                                ; Overwrite saved IER in stack frame.
    MOVZ    AR0 , @SP                                           ;   Get SP.
    SUBB    XAR0, #6                                            ;   Point to old IER.
    MOVL    ACC , *XAR0                                         ;   Get old DBGSTAT:IER.
    AND     ACC, #0xFFFF << 16
    MOV     AL, IER                                             ;   Get current IER.
    MOVL   *XAR0, ACC                                           ;   Overwrite IER.

                                                                ; Restore registers part 2.
    POP     AR1H:AR0H                                           ;   AR1H:AR0H
    .endm

