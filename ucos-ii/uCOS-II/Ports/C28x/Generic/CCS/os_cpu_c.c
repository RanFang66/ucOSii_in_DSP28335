/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*
*
*                         (c) Copyright 2009-2015; Micrium, Inc.; Weston, FL
*                    All rights reserved.  Protected by international copyright laws.
*
*                                             TI C28x Port
*
* File      : OS_CPU_C.C
; Version   : V2.92.11
* By        : JFT
*
* LICENSING TERMS:
* ---------------
*           uC/OS-II is provided in source form for FREE short-term evaluation, for educational use or 
*           for peaceful research.  If you plan or intend to use uC/OS-II in a commercial application/
*           product then, you need to contact Micrium to properly license uC/OS-II for its use in your 
*           application/product.   We provide ALL the source code for your convenience and to help you
*           experience uC/OS-II.  The fact that the source is provided does NOT mean that you can use 
*           it commercially without paying a licensing fee.
*
*           Knowledge of the source code may NOT be used to develop a similar product.
*
*           Please help us continue to provide the embedded community with the finest software available.
*           Your honesty is greatly appreciated.
*
*           You can contact us at www.micrium.com, or by phone at +1 (954) 217-2036.
*
* For       : TI C28x
* Mode      : C28 Object mode
* Toolchain : TI C/C++ Compiler
*********************************************************************************************************
*/

#define   OS_CPU_GLOBALS


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include  <ucos_ii.h>
#include  <lib_def.h>


/*
*********************************************************************************************************
*                                          LOCAL VARIABLES
*********************************************************************************************************
*/

#if OS_TMR_EN > 0u
static  INT16U  OSTmrCtr;
#endif


/*
*********************************************************************************************************
*                                       OS INITIALIZATION HOOK
*                                            (BEGINNING)
*
* Description: This function is called by OSInit() at the beginning of OSInit().
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts should be disabled during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0u
void  OSInitHookBegin (void)
{
                                                                /* Explicitly initialize all global variables.          */
    OSCtxSwCtr = 0u;

#if (OS_EVENT_EN) && (OS_MAX_EVENTS > 0u)
    OSEventFreeList = DEF_NULL;
#endif

#if (OS_FLAG_EN > 0u) && (OS_MAX_FLAGS > 0u)
    OSFlagFreeList = DEF_NULL;
#endif

#if OS_TASK_STAT_EN > 0u
    OSCPUUsage   = 0;
    OSIdleCtrMax = 0;
    OSIdleCtrRun = 0;
    OSStatRdy    = 0;
#endif

    OSIntNesting = 0;

    OSLockNesting = 0;

    OSPrioCur     = 0;
    OSPrioHighRdy = 0;

    OSRdyGrp = 0;

    OSRunning = 0;

    OSTaskCtr = 0;

    OSIdleCtr = 0;

#ifdef OS_SAFETY_CRITICAL_IEC61508
    OSSafetyCriticalStartFlag = 0;
#endif

    OSTCBCur      = DEF_NULL;
    OSTCBFreeList = DEF_NULL;
    OSTCBHighRdy  = DEF_NULL;
    OSTCBList     = DEF_NULL;

#if OS_TICK_STEP_EN > 0u
    OSTickStepState = 0;
#endif

#if (OS_MEM_EN > 0u) && (OS_MAX_MEM_PART > 0u)
    OSMemFreeList = DEF_NULL;
#endif

#if (OS_Q_EN > 0u) && (OS_MAX_QS > 0u)
    OSQFreeList = DEF_NULL;
#endif

#if OS_TASK_REG_TBL_SIZE > 0u
    OSTaskRegNextAvailID = 0;
#endif

#if OS_TIME_GET_SET_EN > 0u
    OSTime = 0;
#endif

#if OS_TMR_EN > 0u
    OSTmrFree = 0;
    OSTmrUsed = 0;
    OSTmrTime = 0;

    OSTmrSem       = DEF_NULL;
    OSTmrSemSignal = DEF_NULL;

    OSTmrFreeList = DEF_NULL;
#endif
}
#endif


/*
*********************************************************************************************************
*                                       OS INITIALIZATION HOOK
*                                               (END)
*
* Description: This function is called by OSInit() at the end of OSInit().
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts should be disabled during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0u
void  OSInitHookEnd (void)
{

}
#endif


/*
*********************************************************************************************************
*                                          TASK CREATION HOOK
*
* Description: This function is called when a task is created.
*
* Arguments  : ptcb   is a pointer to the task control block of the task being created.
*
* Note(s)    : 1) Interrupts are disabled during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0u
void  OSTaskCreateHook (OS_TCB *ptcb)
{
#if OS_APP_HOOKS_EN > 0u
    App_TaskCreateHook(ptcb);
#else
    (void)&ptcb;                                                /* Prevent compiler warning                             */
#endif
}
#endif


/*
*********************************************************************************************************
*                                           TASK DELETION HOOK
*
* Description: This function is called when a task is deleted.
*
* Arguments  : ptcb   is a pointer to the task control block of the task being deleted.
*
* Note(s)    : 1) Interrupts are disabled during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0u
void  OSTaskDelHook (OS_TCB *ptcb)
{
#if OS_APP_HOOKS_EN > 0u
    App_TaskDelHook(ptcb);
#else
    (void)&ptcb;                                                /* Prevent compiler warning                             */
#endif
}
#endif


/*
*********************************************************************************************************
*                                             IDLE TASK HOOK
*
* Description: This function is called by the idle task.  This hook has been added to allow you to do
*              such things as STOP the CPU to conserve power.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts are enabled during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0u
void  OSTaskIdleHook (void)
{
#if OS_APP_HOOKS_EN > 0u
    App_TaskIdleHook();
#endif
}
#endif


/*
*********************************************************************************************************
*                                            TASK RETURN HOOK
*
* Description: This function is called if a task accidentally returns.  In other words, a task should
*              either be an infinite loop or delete itself when done.
*
* Arguments  : ptcb      is a pointer to the task control block of the task that is returning.
*
* Note(s)    : none
*********************************************************************************************************
*/

#if OS_CPU_HOOKS_EN > 0u
void  OSTaskReturnHook (OS_TCB  *ptcb)
{
#if OS_APP_HOOKS_EN > 0u
    App_TaskReturnHook(ptcb);
#else
    (void)ptcb;
#endif
}
#endif


/*
*********************************************************************************************************
*                                           STATISTIC TASK HOOK
*
* Description: This function is called every second by uC/OS-II's statistics task.  This allows your
*              application to add functionality to the statistics task.
*
* Arguments  : none
*********************************************************************************************************
*/

#if OS_CPU_HOOKS_EN > 0u
void  OSTaskStatHook (void)
{
#if OS_APP_HOOKS_EN > 0u
    App_TaskStatHook();
#endif
}
#endif


/*
*********************************************************************************************************
*                                        INITIALIZE A TASK'S STACK
*
* Description: This function is called by either OSTaskCreate() or OSTaskCreateExt() to initialize the
*              stack frame of the task being created.  This function is highly processor specific.
*
* Arguments  : task          is a pointer to the task code
*
*              p_arg         is a pointer to a user supplied data area that will be passed to the task
*                            when the task first executes.
*
*              ptos          is a pointer to the top of stack.  It is assumed that 'ptos' points to
*                            a 'free' entry on the task stack.  If OS_STK_GROWTH is set to 1 then
*                            'ptos' will contain the HIGHEST valid address of the stack.  Similarly, if
*                            OS_STK_GROWTH is set to 0, the 'ptos' will contains the LOWEST valid address
*                            of the stack.
*
*              opt           specifies options that can be used to alter the behavior of OSTaskStkInit().
*                            (see uCOS_II.H for OS_TASK_OPT_xxx).
*
* Returns    : Always returns the location of the new top-of-stack once the processor registers have
*              been placed on the stack in the proper order.
*
* Note(s)    : 1) Interrupts are enabled when task starts executing.
*
*              2) See inline comments for stack frame format.
**********************************************************************************************************
*/

OS_STK  *OSTaskStkInit (void   (*task)(void  *p_arg),
                        void    *p_arg,
                        OS_STK  *ptos,
                        INT16U   opt)
{
    OS_STK  *p_stk;
    INT32U  *p_stk32;

                                                                /* Prevent compiler warnings.                           */
    (void)&opt;

                                                                /* Load and pre-align stack pointer.                    */
    p_stk = ptos;
    p_stk++;
    p_stk32 = (INT32U *)p_stk;
                                                                /* Save registers as if auto-saved.                     */
                                                                /* Follow stacking method in "9) Perform automatic ...  */
                                                                /* ..context save" of section 3.4 in "SPRU430E"         */
    *p_stk32++ = (0x11110000) | OS_CPU_GetST0();                /*   T:ST0                                              */
    *p_stk32++ = 0x33332222;                                    /*   AH:AL                                              */
    *p_stk32++ = 0x55554444;                                    /*   PH:PL                                              */
    *p_stk32++ = 0x77776666;                                    /*   AR1:AR0                                            */
    *p_stk32++ = (0x00000000) | OS_CPU_GetST1();                /*   DP:ST1                                             */
    *p_stk32++ = 0x00000000;                                    /*   DBGSTAT:IER                                        */
    *p_stk32++ = (INT32U)task;                                  /*   Save Return Address [PC+1].                        */

                                                                /* Save remaining registers.                            */
    *p_stk32++ = 0x77776666;                                    /*   AR1H:AR0H                                          */
    *p_stk32++ = 0x99999999;                                    /*   XAR2                                               */
    *p_stk32++ = 0xAAAAAAAA;                                    /*   XAR3                                               */
    *p_stk32++ = (INT32U)p_arg;                                 /*   XAR4: void * parameter.                            */
    *p_stk32++ = 0xCCCCCCCC;                                    /*   XAR5                                               */
    *p_stk32++ = 0xDDDDDDDD;                                    /*   XAR6                                               */
    *p_stk32++ = 0xEEEEEEEE;                                    /*   XAR7                                               */
    *p_stk32++ = 0xFFFFFFFF;                                    /*   XT                                                 */
    *p_stk32++ = (INT32U)&OS_TaskReturn;                        /*   RPC                                                */

#if __TMS320C28XX_FPU32__ == 1                                  /* Save FPU registers, if enabled.                      */
    *p_stk32++ = 0x00000000;                                    /*   R0H                                                */
    *p_stk32++ = 0x11111111;                                    /*   R1H                                                */
    *p_stk32++ = 0x22222222;                                    /*   R2H                                                */
    *p_stk32++ = 0x33333333;                                    /*   R3H                                                */
    *p_stk32++ = 0x44444444;                                    /*   R4H                                                */
    *p_stk32++ = 0x55555555;                                    /*   R5H                                                */
    *p_stk32++ = 0x66666666;                                    /*   R6H                                                */
    *p_stk32++ = 0x77777777;                                    /*   R7H                                                */
    *p_stk32++ = 0x00000000;                                    /*   STF                                                */
    *p_stk32++ = 0x00000000;                                    /*   RB                                                 */
#endif
    														    /* Return pointer to next free location.                */
    return ((OS_STK *)p_stk32);
}


/*
*********************************************************************************************************
*                                           TASK SWITCH HOOK
*
* Description: This function is called when a task switch is performed.  This allows you to perform other
*              operations during a context switch.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts are disabled during this call.
*
*              2) It is assumed that the global pointer 'OSTCBHighRdy' points to the TCB of the task that
*                 will be 'switched in' (i.e. the highest priority task) and, 'OSTCBCur' points to the
*                 task being switched out (i.e. the preempted task).
*********************************************************************************************************
*/
#if (OS_CPU_HOOKS_EN > 0u) && (OS_TASK_SW_HOOK_EN > 0u)
void  OSTaskSwHook (void)
{
#if OS_APP_HOOKS_EN > 0u
    App_TaskSwHook();
#endif
}
#endif


/*
*********************************************************************************************************
*                                           OS_TCBInit() HOOK
*
* Description: This function is called by OS_TCBInit() after setting up most of the TCB.
*
* Arguments  : ptcb    is a pointer to the TCB of the task being created.
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0u
void  OSTCBInitHook (OS_TCB *ptcb)
{
#if OS_APP_HOOKS_EN > 0u
    App_TCBInitHook(ptcb);
#else
    (void)&ptcb;                                                 /* Prevent compiler warning                             */
#endif
}
#endif


/*
*********************************************************************************************************
*                                               TICK HOOK
*
* Description: This function is called every tick.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/
#if (OS_CPU_HOOKS_EN > 0u) && (OS_TIME_TICK_HOOK_EN > 0u)
void  OSTimeTickHook (void)
{
#if OS_APP_HOOKS_EN > 0u
    App_TimeTickHook();
#endif

#if OS_TMR_EN > 0u
    OSTmrCtr++;
    if (OSTmrCtr >= (OS_TICKS_PER_SEC / OS_TMR_CFG_TICKS_PER_SEC)) {
        OSTmrCtr = 0;
        OSTmrSignal();
    }
#endif
}
#endif

