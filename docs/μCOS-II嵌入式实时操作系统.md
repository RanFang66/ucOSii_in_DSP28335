# μC/OS-II嵌入式实时操作系统

## μC/OS简介

μC/OS是一款应用非常广泛的嵌入式实时系统，它最大的特点是抢占式（preemptive）的内核，确保高优先级的任务能够优先得到执行。除了任务调度，μC/OS系统还提供信号量（Semaphore），消息队列（Message Queue），内存管理（Memory Management）等多种可配置的服务。同时官方还提供了多个平台下的移植例程，参照历程用户可以方便的将μC/OS系统移植到自己的平台上。

μC/OS目前已经发展到了μC/OS-iii了，μC/OS-iii相较于ii在任务调度上引入了时间片的概念，允许多个任务具有相同的优先级，通过时间片轮转来调度就绪的相同优先级的任务。此外μC/OS-iii在还在其他功能上做了扩展，但本文主要介绍μC/OS-ii，因此下文不加说明的μC/OS指的就是μC/OS-II。

## μC/OS嵌入式实时操作系统的一些基本概念

- **实时系统（Real-Time System）:**

   系统中计算结果的正确性不仅取决于计算逻辑的正确性，还取决于产生结果的时间。如果完成时间不符合要求，系统就可能会出现严重问题。

  notice：实时系统的要求并不是处理速度快，而是**在预期的时间内完成任务**。例如个人PC系统的运算速度已经非常快，但是并不是实时系统。

- **原语（Primitive）:**

  由若干条指令组成的程序段，用来实现某个特定功能，**在执行过程中不可被中断**，也有称为临界代码区（Critical Section of Code）的。

- **任务（Task）：**

  内核进行任务调度的单位，任务在被调度器调度后就会占用CPU资源，每个任务有自己的一套CPU寄存器和栈空间的上下文。在μC/OS中任务都是被写成一个任务函数，内部运行一个死循环。

- **多任务并发（Multitasking）:** 

  多任务并发其实是靠操作系统对多个任务进行调度和切换实现的，下图是在μC/OS-ii内核的调度下，任务的状态切换图。

  ![image-20210225120325222](μCOS-II嵌入式实时操作系统.assets/image-20210225120325222.png)

- **内核（Kernel）**

  内核是操作系统的核心，它提供操作系统的最基本功能，主要包括任务调度，内存管理，文件管理，设备管理等功能。对于μC/OS这样的嵌入式实时微内核来讲，最主要就是提供任务调度功能，文件管理，设备管理等功能没有集成。

- **抢占式内核（Preemptive Kernel）**

  大多数的实时操作系统都采用抢占式的内核，及存在高优先级的任务已经进入就绪之后，调度器会打断当前正在执行的低优先级的任务，将CPU控制权交给就绪的高优先级任务。与之相反的是非抢占式的内核，对于任何任务都需要任务自身执行操作去主动放弃CPU的占用。

  非抢占式内核逻辑简单，能够安全的调用不可重入函数，但是实时性差。抢占式内核则保证了实时性，但是由于低优先级的任务会被打断，因此再非抢占式内核中使用不可重入内核时要非常小心，一般会采取一些措施保证数据安全。

- **可重入函数/不可重入函数（Reentrancy）**

  可重入函数指的是函数在被执行的过程中可以在任意时刻被中断，过一段时间后再返回执行时不会对函数执行结果有影响。对于可重入函数来说，要么只使用本地变量，要么采取了措施对全局变量的操作进行保护。不满足可重入特性的函数就是不可重入函数，在抢占式内核中使用不可重入函数可能会导致意料之外的错误。

- **临界资源（Critical Resource）与临界区（Critical Section）**

  在某段时间内只允许一个任务使用的资源称为临界资源，任务中访问临界资源的那段程序称为临界区。为了防止多个任务同时访问临界资源，需要采取措施进行保护。μC/OS提供了信号量机制和开关调度器/中断的方式来保护临界资源。

## μC/OS 内核结构

![image-20210226161511042](μCOS-II嵌入式实时操作系统.assets/image-20210226161511042.png)

μC/OS的结构如上图所示，主要包括

1. 内核功能代码文件，这部分文件与芯片和平台无关，其中每个文件都实现一个具体的内核功能。
2. 处理器接口代码文件，这部分代码与具体的CPU配置有关，将μC/OS移植到不同的处理器上时需要重新编写这部分代码。
3. 内核功能配置代码，主要是一个包含很多宏定义的头文件，用户可以通过修改宏定义来使能或者关闭内核的不同功能，对内核进行裁剪。

## μC/OS的任务调度

### μC/OS中的任务(Task)

#### μC/OS中任务的形式

μC/OS中的任务是通过一个包含死循环的函数来实现的，它包含一个空指针类型的参数，用来在运行过程中向任务传递参数，返回值类型只能定义为void型。

```c
void YourTask(void *pdata)
{
    for ( ; ; ) {
        /* USER CODE */
        
        /* Call one of uC/OS services: */
        OSFlagPend();
        OSMboxPend();
        OSMutexPend();
        OSQPend();
        OSSemPend();
        OSTaskDel(OS_PRIO_SELF);
        OSTimeDly();
        OSTimeDlyHMSM();
        
        /* USER CODE */
    }
}
```

除了死循环，任务还可以在完成工作之后删除自己，因此任务的定义还可以有如下形式：

```c
void YourTask(void *pdata)
{
    /* USER CODE */
    OSTaskDel(OS_PRIO_SELF);
}
```

μC/OS中最多可以管理256个任务，可以管理的任务数量由宏定义 `OS_LOWEST_PRIO` 定义，默认为64个。每个任务都需要有一个与其他任务不同的优先级，但系统中最低优先级的任务已经被分配给了IDLE任务，次低优先级已经被内核分配给了统计任务。此外μC/OS还建议保留最高优先级0，1用于今后扩展。

#### 任务控制块

每个任务在内核中被分配一个任务控制块（TCB，Task Control Block ），其实就是操作系统中的进程控制块（PCB）的概念。TCB是一个结构体，会记录当前任务执行的所有重要属性信息，如任务优先级，任务栈指针等。内核对任务的调度，其实就是对于TCB的操作。μC/OS中对于TCB的定义如下，其中通过宏定义（在OS_CFG.h中）的配置，可以选择性的包含一些任务属性。

```c
typedef struct os_tcb {
    OS_STK          *OSTCBStkPtr;           /* Pointer to current top of stack                         */

#if OS_TASK_CREATE_EXT_EN > 0u
    void            *OSTCBExtPtr;           /* Pointer to user definable data for TCB extension        */
    OS_STK          *OSTCBStkBottom;        /* Pointer to bottom of stack                              */
    INT32U           OSTCBStkSize;          /* Size of task stack (in number of stack elements)        */
    INT16U           OSTCBOpt;              /* Task options as passed by OSTaskCreateExt()             */
    INT16U           OSTCBId;               /* Task ID (0..65535)                                      */
#endif

    struct os_tcb   *OSTCBNext;             /* Pointer to next TCB in the TCB list                 */
    struct os_tcb   *OSTCBPrev;             /* Pointer to previous TCB in the TCB list                 */

#if OS_TASK_CREATE_EXT_EN > 0u
#if defined(OS_TLS_TBL_SIZE) && (OS_TLS_TBL_SIZE > 0u)
    OS_TLS           OSTCBTLSTbl[OS_TLS_TBL_SIZE];
#endif
#endif

#if (OS_EVENT_EN)
    OS_EVENT        *OSTCBEventPtr;         /* Pointer to event control block                 */
#endif

#if (OS_EVENT_EN) && (OS_EVENT_MULTI_EN > 0u)
    OS_EVENT       **OSTCBEventMultiPtr;    /* Pointer to multiple event control blocks                */
#endif

#if ((OS_Q_EN > 0u) && (OS_MAX_QS > 0u)) || (OS_MBOX_EN > 0u)
    void            *OSTCBMsg;              /* Message received from OSMboxPost() or OSQPost()         */
#endif

#if (OS_FLAG_EN > 0u) && (OS_MAX_FLAGS > 0u)
#if OS_TASK_DEL_EN > 0u
    OS_FLAG_NODE    *OSTCBFlagNode;         /* Pointer to event flag node                              */
#endif
    OS_FLAGS         OSTCBFlagsRdy;         /* Event flags that made task ready to run                 */
#endif

    INT32U           OSTCBDly;              /* Nbr ticks to delay task or, timeout waiting for event   */
    INT8U            OSTCBStat;             /* Task status                                        */
    INT8U            OSTCBStatPend;         /* Task PEND status                                        */
    INT8U            OSTCBPrio;             /* Task priority (0 == highest)                            */

    INT8U            OSTCBX;                /* Bit position in group  corresponding to task priority   */
    INT8U            OSTCBY;                /* Index into ready table corresponding to task priority   */
    OS_PRIO          OSTCBBitX;             /* Bit mask to access bit position in ready table          */
    OS_PRIO          OSTCBBitY;             /* Bit mask to access bit position in ready group          */

#if OS_TASK_DEL_EN > 0u
    INT8U            OSTCBDelReq;           /* Indicates whether a task needs to delete itself         */
#endif

#if OS_TASK_PROFILE_EN > 0u
    INT32U           OSTCBCtxSwCtr;         /* Number of time the task was switched in                 */
    INT32U           OSTCBCyclesTot;        /* Total number of clock cycles the task has been running  */
    INT32U           OSTCBCyclesStart;      /* Snapshot of cycle counter at start of task resumption   */
    OS_STK          *OSTCBStkBase;          /* Pointer to the beginning of the task stack              */
    INT32U           OSTCBStkUsed;          /* Number of bytes used from the stack                     */
#endif

#if OS_TASK_NAME_EN > 0u
    INT8U           *OSTCBTaskName;
#endif

#if OS_TASK_REG_TBL_SIZE > 0u
    INT32U           OSTCBRegTbl[OS_TASK_REG_TBL_SIZE];
#endif
} OS_TCB;
```

### μC/OS中的任务就绪表（Ready List）

#### 任务就绪表的组织形式

为了进行任务调度，μC/OS内核中的任务调度器（scheduler）会维护一张就绪表，这张表中会记录当前所有任务是否就绪。为了提高效率，就绪表采取了分组的表示方式，由两个变量组成，一个是整形OSRdyGrp，它的每一位如果为1则代表对应的一组任务有已经就绪的，接着就会去查找OSRdyTbl数组中代表那一组任务的值（整形），通过检查这个值的每一位就能知道这一组任务中有哪些任务是就绪的。就绪表的定义如下，为了节约空间，提高效率，会自动根据用户定义的最大任务数量来变化OS_PRIO代表的数据类型，如果定义的任务优先级小于64，则用8位整型，否则使用16位整型表示。

```c
OS_EXT  OS_PRIO           OSRdyGrp;                        /* Ready list group                         */
OS_EXT  OS_PRIO           OSRdyTbl[OS_RDY_TBL_SIZE];       /* Table of tasks which are ready to run    */
```

下图很好的表示就绪表维护所有任务就绪状态信息的方式：

![image-20210228181717273](μCOS-II嵌入式实时操作系统.assets/image-20210228181717273.png)

#### 快速定位最高优先级任务

为了快速找到当前所有就绪任务中，优先级最高的任务，不需要对OSRdyTbl进行从前到后的扫描，而是用到了另一张表OSUnMapTbl[]。当通过OSRdyGrp去作为索引查询OSUnMapTbl时，就可以得到当前所有组中优先级最高的那一组所在的bit。同样，在确定了优先级最高的那一组任务之后，再用那一组的byte（OSRdyTbl[y]）去索引OSUnMapTbl又可以得到这一组任务中优先级最高的那个任务所在的bit。这样就能快速通过移位运算和查表的方式得到当前就绪表中优先级最高的任务。整个获取优先级最高任务的过程由以下代码完成：

```c
y             = OSUnMapTbl[OSRdyGrp];
OSPrioHighRdy = (INT8U)((y << 3u) + OSUnMapTbl[OSRdyTbl[y]]);
```

### μC/OS中的任务调度（Scheduling）

#### 任务级(task-level) 任务调度

μC/OS中的任务调度是通过内核中的调度器实现的。内核中存在两种级别的调度，一种时任务级别的调度，通过调用OS_Sched()函数实现，另一种是中断级别的调度，通过调用OSIntExit()实现。

```c
void  OS_Sched (void)
{
#if OS_CRITICAL_METHOD == 3u                           /* Allocate storage for CPU status register     */
    OS_CPU_SR  cpu_sr = 0u;
#endif



    OS_ENTER_CRITICAL();
    if (OSIntNesting == 0u) {                          /* Schedule only if all ISRs done and ...       */
        if (OSLockNesting == 0u) {                     /* ... scheduler is not locked                  */
            OS_SchedNew();
            OSTCBHighRdy = OSTCBPrioTbl[OSPrioHighRdy];
            if (OSPrioHighRdy != OSPrioCur) {          /* No Ctx Sw if current task is highest rdy     */
#if OS_TASK_PROFILE_EN > 0u
                OSTCBHighRdy->OSTCBCtxSwCtr++;         /* Inc. # of context switches to this task      */
#endif
                OSCtxSwCtr++;                          /* Increment context switch counter             */

#if OS_TASK_CREATE_EXT_EN > 0u
#if defined(OS_TLS_TBL_SIZE) && (OS_TLS_TBL_SIZE > 0u)
                OS_TLS_TaskSw();
#endif
#endif

                OS_TASK_SW();                          /* Perform a context switch                     */
            }
        }
    }
    OS_EXIT_CRITICAL();
}
```

上面列出了μC/OS中任务调度函数的源码，从源码中可以看到：

1. 当调度器OS_Sched()是在中断中被调用（将会由中断级调度OSIntExt()决定）或者是在调度器被锁时，函数将会直接退出。
2. 当不满足1中的条件时，调度器通过调用OS_SchedNew()，也就是上一节中所讲到的方法，来找到当前就绪任务中最高优先级的那个任务。而当前已就绪的最高优先级的任务的优先级在程序中用OSPrioHighRdy表示。用得到的优先级去索引任务控制块链表即可得到当前最高优先级的就绪任务的任务控制块，用OSTCBHighRdy表示。
3. 接着任务调度器就会比较目前最高优先级的任务和当前正在运行的任务是否一致，如果一致就直接退出，不进行任务切换，而如果不一致，则会调用OS_TASK_SW()进行任务切换，这个过程中还会对任务切换次数进行计数统计。OS_TASK_SW()需要用户在移植内核的时候根据不同的处理器去实现，它的功能是软件模拟一次中断，从而达到上下文切换的目的。
4. 任务调度函数属于临界区代码，所以在执行之前会关闭所有中断，防止在判断最高优先级任务时，就绪表被其他中断修改。

#### 任务级的上下文切换(Context Switch)

从前面知道，当调度器发现当前最高优先级的就绪任务与当前正在运行的任务不同时，就会调用OS_TASK_SW()函数进行一次上下文切换。所谓任务的上下文指的就是任务在当前运行状态下的所有CPU寄存器信息，上下文切换时所需要做的就是保存下当前被抢占的任务的上下文，同时将马上要运行的任务的上下文加载到CPU中。

OS_TASK_SW()在μC/OS中通常是通过宏定义调用来实现的，很多CPU都会提供软中断或者陷进(TRAP)这类处理器指令，可以模拟硬件中断的产生。接着用户就需要根据不同的处理器在软中断处理函数中去实现上下文切换，即保存当前的CPU寄存器值，栈指针等，然后将之前保存的要执行的高优先级任务的CPU寄存器值加载到CPU寄存器中，完成上下文切换。这个过程一般是通过用户编写一个汇编函数OSCtxSw()来实现。



