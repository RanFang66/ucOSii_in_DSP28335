;
;********************************************************************************************************
;                                                uC/OS-II
;                                          The Real-Time Kernel
;
;
;                         (c) Copyright 2009-2015; Micrium, Inc.; Weston, FL
;                    All rights reserved.  Protected by international copyright laws.
;
;                                             TI C28x Port
;
; File      : OS_CPU_A.ASM
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
;                                             INCLUDE FILES
;********************************************************************************************************

    .include "os_cpu_i.asm"


;********************************************************************************************************
;                                       EXTERNAL GLOBAL VARIABLES
;********************************************************************************************************
                                                                ; External references
    .ref   _OSRunning
    .ref   _OSPrioCur
    .ref   _OSPrioHighRdy
    .ref   _OSTCBCur
    .ref   _OSTCBHighRdy
    .ref   _OSIntExit
    .ref   _OSTaskSwHook


;********************************************************************************************************
;                                          PUBLIC FUNCTIONS
;********************************************************************************************************
                                                                ; Functions declared in this file
    .def   _OS_CPU_GetST0
    .def   _OS_CPU_GetST1
    .def   _OS_CPU_SR_Save
    .def   _OS_CPU_SR_Restore
    .def   _OSStartHighRdy
    .def   _OS_CPU_RTOSINT_Handler


;********************************************************************************************************
;                                               EQUATES
;********************************************************************************************************


;********************************************************************************************************
;                                     CODE GENERATION DIRECTIVES
;********************************************************************************************************
                                                                ; Set text section and reset local labels.
    .text
    .newblock


;********************************************************************************************************
;                                            GET ST0 and ST1
;
; Description : Wrapper function to get ST0 and ST1 registers from a C function.
;
; Prototypes  : INT16U  OS_CPU_GetST0(void);
;               INT16U  OS_CPU_GetST1(void);
;********************************************************************************************************

    .asmfunc
_OS_CPU_GetST0:
    PUSH    ST0
    POP     AL
    LRETR
    .endasmfunc

    .asmfunc
_OS_CPU_GetST1:
    PUSH    ST1
    POP     AL
    LRETR
    .endasmfunc


;********************************************************************************************************
;                                   CRITICAL SECTION METHOD 3 FUNCTIONS
;
; Description: Disable/Enable interrupts by preserving the state of interrupts.  Generally speaking you
;              would store the state of the interrupt disable flag in the local variable 'cpu_sr' and then
;              disable interrupts.  'cpu_sr' is allocated in all of uC/OS-II's functions that need to
;              disable interrupts.  You would restore the interrupt disable state by copying back 'cpu_sr'
;              into the CPU's status register.
;
; Prototypes :     OS_CPU_SR  OS_CPU_SR_Save(void);
;                  void       OS_CPU_SR_Restore(OS_CPU_SR cpu_sr);
;
;
; Note(s)    : 1) These functions are used in general like this:
;
;                 void Task (void *p_arg)
;                 {
;                 #if OS_CRITICAL_METHOD == 3          /* Allocate storage for CPU status register */
;                     OS_CPU_SR  cpu_sr;
;                 #endif
;
;                          :
;                          :
;                     OS_ENTER_CRITICAL();             /* cpu_sr = OS_CPU_SaveSR();                */
;                          :
;                          :
;                     OS_EXIT_CRITICAL();              /* OS_CPU_RestoreSR(cpu_sr);                */
;                          :
;                          :
;                 }
;********************************************************************************************************

    .asmfunc
_OS_CPU_SR_Save:
    PUSH    ST1
    DINT
    POP    @AL
    AND     AL, #1
    LRETR
    .endasmfunc

    .asmfunc
_OS_CPU_SR_Restore:
    PUSH    ST1
    POP     AR0
    AND     AR0, #0xFFFE
    OR      AL, AR0
    PUSH    AL
    POP     ST1
    LRETR
    .endasmfunc


;********************************************************************************************************
;                                         START MULTITASKING
;                                      void OSStartHighRdy(void)
; Note(s) : 1) This function triggers a PendSV exception (essentially, causes a context switch) to cause
;              the first task to start.
;
;           2) OSStartHighRdy() MUST:
;              a) Call OSTaskSwHook().
;              b) Set OSRunning to TRUE;
;              c) Restore context for OSTCBCurPtr.
;              d) IRET into highest ready task.
;********************************************************************************************************
    .asmfunc
_OSStartHighRdy:
                                                                ; Call OSTaskSwHook()
    LCR     #_OSTaskSwHook
                                                                ; Set OSRunning to TRUE.
    MOVL    XAR0, #_OSRunning
    MOV     *AR0, #1
                                                                ; Restore context.
    MOVL    XAR0, #_OSTCBCur                                    ; Get the process's SP.
    MOVL    XAR1, *XAR0
    MOV     AL  , *AR1
    MOV    @SP  ,  AL
                                                                ; Restore registers.
    OS_CTX_RESTORE
                                                                ; IRET into task.
    IRET
    .endasmfunc
                                                                ; Catch start high failure.
OSStartHang:
    SB      OSStartHang, UNC


;********************************************************************************************************
;                                       HANDLE RTOSINT INTERRUPT
;                                   void OS_CPU_RTOSINT_Handler(void)
;
; Note(s) : 1) The RTOSINT interrupt is used to perform a context switch. The C28x core saves the
;              ST0, T, ACC, P, AR0, AR1, ST1, DP, IER, DBGSTAT (shadow) registers and the Return
;              Address.
;              The remaining registers AR1H, AR0H, XAR2..XAR7, XT and RPC are saved by the handler.
;              Using RTOSINT means that the switching mechanism is the same for a task or an ISR.

;           2) The context switching RTOSINT handler pseudo-code is:
;              a) Save remaining registers on the process stack;
;              b) Save the process SP in its TCB, OSTCBCurPtr->OSTCBStkPtr = SP;
;              c) Call OSTaskSwHook();
;              d) Get current high priority, OSPrioCur = OSPrioHighRdy;
;              e) Get current ready thread TCB, OSTCBCur = OSTCBHighRdy;
;              f) Get new process SP from TCB, SP = OSTCBHighRdy->OSTCBStkPtr;
;              g) Restore AR1H, AR0H, XAR2..XAR7, XT and RPC registers from the new process stack;
;              h) Overwrite the previously saved (at context switch out) IER register with current IER.
;              i) Perform exception return which will restore remaining context.
;
;           3) On entry into RTOSINT handler:
;              a) The following have been saved on the process stack (by processor):
;                 ST0, T, ACC, P, AR0, AR1, ST1, DP, IER, DBGSTAT (shadow) registers and the Return
;                 Address.
;              b) OSTCBCurPtr      points to the OS_TCB of the task to suspend
;                 OSTCBHighRdyPtr  points to the OS_TCB of the task to resume
;
;           4) This function MUST be placed in entry 16 (for RTOSINT) of the C28x interrupt table.
;********************************************************************************************************

    .asmfunc
_OS_CPU_RTOSINT_Handler:
                                                                ; Save registers.
    OS_CTX_SAVE
                                                                ; Save SP to current process.
    MOVL    XAR0, #_OSTCBCur                                    ; Get the process's SP.
    MOVL    XAR1, *XAR0
    MOV     AL  , @SP
    MOV     *AR1,  AL

    ASP                                                         ; Align the stack pointer.
                                                                ; Call OSTaskSwHook.
    LCR     #_OSTaskSwHook
    NASP                                                        ; Restore alignement of the stack pointer.
    
                                                                ; OSPrioCur = OSPrioHighRdy
    MOVL    XAR0, #_OSPrioHighRdy
    MOVL    XAR1, #_OSPrioCur
    MOV     ACC, *XAR0
    MOV     *XAR1, ACC
                                                                ; OSTCBCur = OSTCBHighRdy
    MOVL    XAR0, #_OSTCBHighRdy
    MOVL    XAR1, #_OSTCBCur
    MOVL    ACC, *XAR0
    MOVL    *XAR1, ACC
                                                                ; Get SP from new process.
    MOVL    XAR0, *XAR1
    MOV     AL  , *AR0
    MOV     @SP , AL

    OS_CTX_RESTORE
                                                                ; Return from interrupt to restore remaining registers.
    IRET
    .endasmfunc


;********************************************************************************************************
;                                     OS-II ASSEMBLY PORT FILE END
;********************************************************************************************************

    .end
