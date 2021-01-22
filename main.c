/*
 * main.c
 *
 *  Created on: 2021��1��22��
 *      Author: fangran
 */

#include  <app_cfg.h>
#include  <ucos_ii.h>
#include  <cpu_core.h>
#include  <lib_def.h>
#include  <os_cpu.h>

#include "DSP2833x_Device.h"     // DSP2833x ͷ�ļ�
#include "DSP2833x_Examples.h"   // DSP2833x �������ͷ�ļ�

#define LED1        GpioDataRegs.GPATOGGLE.bit.GPIO16
CPU_STK_SIZE  App_TaskStartStk[APP_CFG_TASK_STK_SIZE];
CPU_STK_SIZE  App_TaskPendStk[APP_CFG_TASK_STK_SIZE];
CPU_STK_SIZE  App_TaskPostStk[APP_CFG_TASK_STK_SIZE];

static  OS_EVENT    *AppTaskObjSem;

static  void  App_TaskStart(void  *p_arg);
static  void  App_TaskPing (void  *p_arg);
static  void  App_TaskPong (void  *p_arg);

interrupt void cpu_timer0_isr(void);  // �ж�����
void BSP_Tick_Init(void);

void BSP_Tick_Init(void)
{
    EALLOW;
    PieVectTable.TINT0 = &cpu_timer0_isr; // ��ʱ���жϵ�ַ
    // PieVectTable.RTOSINT = &RTOSINT_ISR;    // test
    PieVectTable.RTOSINT = &OS_CPU_RTOSINT_Handler;  // RTOS
    // MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
    // InitFlash();  //��ʼ��FLASH
    EDIS;
    InitCpuTimers();
    ConfigCpuTimer(&CpuTimer0, 60, 1000);  // test 500000
    CpuTimer0Regs.TCR.all = 0x4001; // ���� TSS bit = 0
    IER |= M_INT1;   // CPU-Timer 0����CPU INT1��ʹ��:
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1; // Enable TINT0 in the PIE: Group 1 interrupt 7
    EINT;   // ʹ��INTM�ж�
    ERTM;   // ʹ��DBGM�ж�
}
void main(void)
{
   InitSysCtrl();
   // InitGpio();  // ����δ�õ�  // �ڶ�������ʼ��GPIO:
   DINT;
   InitPieCtrl();
   IER = 0x0000;
   IFR = 0x0000;
   InitPieVectTable();

   EALLOW;
   GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 0;  // LED0
   GpioCtrlRegs.GPADIR.bit.GPIO16 = 1;
   EDIS;
   EINT;   // ʹ��INTM�ж�
   ERTM;   // ʹ��DBGM�ж�

   OSInit();
   OSTaskCreateExt(App_TaskStart,
                   (void    *)0,
                   (CPU_STK *)&App_TaskStartStk[0],
                   (INT8U    )APP_CFG_TASK_START_PRIO,
                   (INT16U   )APP_CFG_TASK_START_PRIO,
                   (CPU_STK *)&App_TaskStartStk[APP_CFG_TASK_STK_SIZE - 1u],
                   (INT32U   )APP_CFG_TASK_STK_SIZE,
                   (void    *)0,
                   (INT16U   )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
   OSStart();
   while (DEF_TRUE) {
       ;
   }
}

static  void  App_TaskStart (void *p_arg)
{
    volatile CPU_INT08U  os_err;  // test
    (void)&p_arg;
    BSP_Tick_Init();
    AppTaskObjSem = OSSemCreate(0);

    OSTaskCreateExt(App_TaskPing,
                    (void    *)0,
                    (CPU_STK *)&App_TaskPendStk[0],
                    (INT8U    )APP_CFG_TASK_PEND_PRIO,
                    (INT16U   )APP_CFG_TASK_PEND_PRIO,
                    (CPU_STK *)&App_TaskPendStk[APP_CFG_TASK_STK_SIZE - 1u],
                    (INT32U   )APP_CFG_TASK_STK_SIZE,
                    (void    *)0,
                    (INT16U   )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
    OSTaskCreateExt(App_TaskPong,
                    (void    *)0,
                    (CPU_STK *)&App_TaskPostStk[0],
                    (INT8U    )APP_CFG_TASK_POST_PRIO,
                    (INT16U   )APP_CFG_TASK_POST_PRIO,
                    (CPU_STK *)&App_TaskPostStk[APP_CFG_TASK_STK_SIZE - 1u],
                    (INT32U   )APP_CFG_TASK_STK_SIZE,
                    (void    *)0,
                    (INT16U   )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
    while (DEF_TRUE) {
        os_err = OSSemPost(AppTaskObjSem);
        OSTimeDlyHMSM(0, 0, 0, 1);
    }
}
static  void  App_TaskPing (void *p_arg)
{
    CPU_INT08U  os_err;
    (void)&p_arg;
    while (DEF_TRUE)
    {
        OSSemPend( AppTaskObjSem,
                   0,
                  &os_err);
    }
}
static  void  App_TaskPong (void *p_arg)
{
    (void)&p_arg;
    while (DEF_TRUE)
    {
        OSTimeDlyHMSM(0, 0, 0, 300);   // test   before is 300
        LED1=1;
        CpuTimer0.InterruptCount++;    // test  ���������
    }
}
interrupt void cpu_timer0_isr(void)
{
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
   // LED0=1;     // test
   // CpuTimer0.InterruptCount++;  //test
   OSIntEnter();
   OSTimeTick();
   OSIntExit();
}


