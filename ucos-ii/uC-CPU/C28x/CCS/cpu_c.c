/*
*********************************************************************************************************
*                                                uC/CPU
*                                    CPU CONFIGURATION & PORT LAYER
*
*                          (c) Copyright 2004-2015; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*
*               uC/CPU is provided in source form to registered licensees ONLY.  It is
*               illegal to distribute this source code to any third party unless you receive
*               written permission by an authorized Micrium representative.  Knowledge of
*               the source code may NOT be used to develop a similar product.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*
*               You can find our product's user manual, API reference, release notes and
*               more information at https://doc.micrium.com.
*               You can contact us at www.micrium.com.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                             CPU PORT FILE
*
*                                                TI C28x
*                                           TI C/C++ Compiler
*
*
* Filename      : cpu_c.c
* Version       : V1.30.02.00
* Programmer(s) : JFT
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <cpu.h>
#include  <cpu_core.h>


#ifdef __cplusplus
extern  "C" {
#endif


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           CPU_IntSrcDis()
*
* Description : Disable an interrupt source.
*
* Argument(s) : int_id      Interrupt source in interrupt enable register.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) The RESET interrupt cannot be disabled.
*********************************************************************************************************
*/

void  CPU_IntSrcDis (CPU_DATA  bit)
{
    CPU_SR_ALLOC();


    if (bit <= CPU_INT_RTOSINT) {
        CPU_CRITICAL_ENTER();
        IER = IER & ~(1u << (bit-1));
        CPU_CRITICAL_EXIT();
    }
}


/*
*********************************************************************************************************
*                                            CPU_IntSrcEn()
*
* Description : Enable an interrupt source.
*
* Argument(s) : int_id      Interrupt source in interrupt enable register.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  CPU_IntSrcEn (CPU_DATA  bit)
{
    CPU_SR_ALLOC();


    if (bit <= CPU_INT_RTOSINT) {
        CPU_CRITICAL_ENTER();
        IER = IER | (1u << (bit-1));
        CPU_CRITICAL_EXIT();
    }
}


/*
*********************************************************************************************************
*                                          CPU_IntSrcPendClr()
*
* Description : Clear a pending interrupt.
*
* Argument(s) : bit     Bit of interrupt source in interrupt enable register (see 'CPU_IntSrcDis()').
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  CPU_IntSrcPendClr (CPU_DATA  bit)
{
                                                                /* The 'AND IFR' instruction cannot be used with ...    */
                                                                /* ... anything else than a 16bit constant.             */
    switch (bit) {
        case CPU_INT_RTOSINT:
             asm("        AND IFR, #0x7FFF");
             break;


        case CPU_INT_DLOGINT:
             asm("        AND IFR, #0xBFFF");
             break;


        case CPU_INT_INT14:
             asm("        AND IFR, #0xDFFF");
             break;


        case CPU_INT_INT13:
             asm("        AND IFR, #0xEFFF");
             break;


        case CPU_INT_INT12:
             asm("        AND IFR, #0xF7FF");
             break;


        case CPU_INT_INT11:
             asm("        AND IFR, #0xFBFF");
             break;


        case CPU_INT_INT10:
             asm("        AND IFR, #0xFDFF");
             break;


        case CPU_INT_INT9:
             asm("        AND IFR, #0xFEFF");
             break;


        case CPU_INT_INT8:
             asm("        AND IFR, #0xFF7F");
             break;


        case CPU_INT_INT7:
             asm("        AND IFR, #0xFFBF");
             break;


        case CPU_INT_INT6:
             asm("        AND IFR, #0xFFDF");
             break;


        case CPU_INT_INT5:
             asm("        AND IFR, #0xFFEF");
             break;


        case CPU_INT_INT4:
             asm("        AND IFR, #0xFFF7");
             break;


        case CPU_INT_INT3:
             asm("        AND IFR, #0xFFFB");
             break;


        case CPU_INT_INT2:
             asm("        AND IFR, #0xFFFD");
             break;


        case CPU_INT_INT1:
             asm("        AND IFR, #0xFFFE");
             break;


        default:                                                /* 'default' case intentionally empty.                  */
             break;
    }
}


#ifdef __cplusplus
}
#endif
