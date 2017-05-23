

 ipi机制 


大家知道，在做内核调试器的时候，为了不影响当前环境，当中断产生的时候必须将非当前cpu外的其他cpu
的运行中断下来。那么内核调试器是怎么做到的呢？实际上这是APIC的ipi（处理器间中断）。下面我根据Linux和Windows的相应源码进行讲解。
当进入内核调试模式时，内核由KdEnterDebugger进入KeFreezeExcution。在KeFreezeExcution中首先检测是否KiFreezeExecutionLock
这个锁是busy状态，如果是则表示其他的处理器试图投递IPI给当前处理器，于是调用KiIpiServiceRoutine，这便是处理器间中断的
服务例程。那么什么是处理器间中断（IPI）呢?这要从8259A讲起。
传统的i386采用的8259A中断控制器有个缺点，如果在smp中采用这样的设计，那就只能静态的把所有外部中断源划分成若干组，
分别把每组都链接到8259A，使其与CPU有一对一的连接，然而这样就达不到动态分配中断请求的目的，也使得硬件设计变的很不简洁。
因此intel设计了更为通用的中断控制器--APIC，高级可编程中断控制器。另一方面，koala到处理器间中断请求的需要，每个cpu都
需要有个本地APIC，因为一个cpu常常要有目标的向系统中的其他CPU发出中断请求,所以从pentium开始intel就在cpu芯片
内部集成了一个本地APIC。但是SMP结构中还需要一个外部的、全局的APIC。
不管是外部还是内部，APIC都支持从0x20-0xff共240个不同的中断向量，其中0-0x1f用于cpu本身的异常。这些中断分为15个
优先级，可以按中断向量号处于16算得。"除了外部APIC可以把来自外部设备的中断请求提交给系统的各个cpu外，每个cpu也可以通过其内部的apic
向其他cpu发出中断请求，这便是处理器间中断。"当一个cpu向其他cpu发送中断信号时，就在自己的本地ICR寄存器（
interrupt command ragister）中存放其中断向量，和目标cpu拥有的本地apic标识出发中断。IPI信号由apic总线传递再到目标APIC。
例如，下面是我的某个cpu的idt表：
lkd> !idt

Dumping IDT:

37: 806e4864 hal!PicSpuriousService37
3d: 806e5e2c hal!HalpApcInterrupt
41: 806e5c88 hal!HalpDispatchInterrupt
50: 806e493c hal!HalpApicRebootService
63: 89b3948c serial!SerialCIsrSw (KINTERRUPT 89b39450)
          USBPORT!USBPORT_InterruptService (KINTERRUPT 898d3308)
          USBPORT!USBPORT_InterruptService (KINTERRUPT 89c50360)
73: 89d6f7e4 atapi!IdePortInterrupt (KINTERRUPT 89d6f7a8)
          atapi!IdePortInterrupt (KINTERRUPT 89daf008)
          atapi!IdePortInterrupt (KINTERRUPT 89e20bb0)
          atapi!IdePortInterrupt (KINTERRUPT 89e01bb0)
          atapi!IdePortInterrupt (KINTERRUPT 89da0bb0)
          USBPORT!USBPORT_InterruptService (KINTERRUPT 89c4f208)
83: 89c03bec VIDEOPRT!pVideoPortInterrupt (KINTERRUPT 89c03bb0)
          *** ERROR: Module load completed but symbols could not be loaded for \SystemRoot\system32\DRIVERS\HECI.sys
HECI+0x486 (KINTERRUPT 89b39bb0)
          USBPORT!USBPORT_InterruptService (KINTERRUPT 898cebb0)
          *** ERROR: Module load completed but symbols could not be loaded for \SystemRoot\system32\DRIVERS\HDAudBus.sys
HDAudBus+0x2C12 (KINTERRUPT 898c1bb0)
92: 89c445b4 serial!SerialCIsrSw (KINTERRUPT 89c44578)
94: 89c57044 USBPORT!USBPORT_InterruptService (KINTERRUPT 89c57008)
          USBPORT!USBPORT_InterruptService (KINTERRUPT 897933b8)
a4: 89bedbec USBPORT!USBPORT_InterruptService (KINTERRUPT 89bedbb0)
b1: 89e2867c ACPI!ACPIInterruptServiceRoutine (KINTERRUPT 89e28640)
b4: 899021c4 NDIS!ndisMIsr (KINTERRUPT 89902188)
c1: 806e4ac0 hal!HalpBroadcastCallService
d1: 806e3e54 hal!HalpClockInterrupt
e1: 806e5048 hal!HalpIpiHandler
e3: 806e4dac hal!HalpLocalApicErrorService
fd: 806e55a8 hal!HalpProfileInterrupt
fe: 806e5748 hal!HalpPerfInterrupt
可以看到0xe1号中断便是ipi的处理例程HalpIpiHandler，在HalpIpiHandler里又会调用KiIpiServiceRoutine。
那么这些中断又是在哪注册的呢？实际上当系统初始化时，Phase1Initialization -> HalInitSystem 时，会
        HalpSetInternalVector(APIC_PROFILE_VECTOR, HalpProfileInterrupt);
        HalpSetInternalVector(APIC_PERF_VECTOR, HalpPerfInterrupt);
        HalpSetInternalVector(DPC_VECTOR, HalpDispatchInterrupt);
        HalpSetInternalVector(APC_VECTOR, HalpApcInterrupt);
        HalpSetInternalVector(APIC_IPI_VECTOR, HalpIpiHandler);

APIC_IPI_VECTOR便是0xE1(Windows下)。

下面看发送ipi的过程，Windows下通过KiIpiSend -> HalRequestIpi。
下面是HalRequestIpi的代码：

;++
;
; VOID
; HalRequestIpi(
;       IN ULONG Mask
;       );
;
;Routine Description:
;
;    Requests an interprocessor interrupt
;
;Arguments:
;
;    Mask - Supplies a mask of the processors to be interrupted
;
;Return Value:
;
;    None.
;
;--
APIC_IPI equ (DELIVER_FIXED OR LOGICAL_DESTINATION OR ICR_USE_DEST_FIELD OR APIC_IPI_VECTOR)

cPublicProc _HalRequestIpi ,1
cPublicFpo 1, 0

        mov     eax, [esp+4]            ; (eax) = Processor bitmask
        shl     eax, DESTINATION_SHIFT

;
; With an APIC we will IPI everyone needed at the same time.
; This assumes that:
;   (mask passed in) == (APIC logical destination mask) Since we've programmed
; the APIC Local Units to use the Processor ID as the Logical ID's this IS true
;

        pushfd                          ; save interrupt mode
        cli                             ; disable interrupt

        STALL_WHILE_APIC_BUSY
;
; Set the destination address, (eax) = Processor bitmask
;

        mov     dword ptr APIC[LU_INT_CMD_HIGH], eax
        APICFIX edx

;
; Now issue the command by writing to the Memory Mapped Register
;

        mov     dword ptr APIC[LU_INT_CMD_LOW], APIC_IPI

;
;   Wait for the delivery to happen
;   KenR why is there a wait here????
;
        STALL_WHILE_APIC_BUSY

        popfd
        stdRET    _HalRequestIpi

stdENDP _HalRequestIpi

        page ,132
        subttl "PC+MP IPI Interrupt Handler"


Linux下则通过send_IPI_mask发送。

下面不妨再看看send_IPI_mask的流程：

static inline void send_IPI_mask(int mask, int vector)
{
unsigned long cfg;
unsigned long flags;

__save_flags(flags);
__cli();

  
/*
* Wait for idle.
*/
apic_wait_icr_idle();
  
/*
* prepare target chip field
*/
cfg = __prepare_ICR2(mask);
apic_write_around(APIC_ICR2, cfg);
  
/*
* program the ICR
*/
cfg = __prepare_ICR(0, vector);
   
/*
* Send the IPI. The write to APIC_ICR fires this off.
*/
apic_write_around(APIC_ICR, cfg);

__restore_flags(flags);
}

static inline int __prepare_ICR (unsigned int shortcut, int vector)
{
return APIC_DM_FIXED | shortcut | vector | INT_DEST_ADDR_MODE;
}

static inline int __prepare_ICR2 (unsigned int mask)
{
return SET_APIC_DEST_FIELD(mask);
}
讲解下上面代码，CPU内部还有一些中断控制器，APIC_ICR和APIC_ICR2是其中的两个。APIC_ICR2主要用来说明要发送的中断请求的目标。这种目标可以是具体的cpu，也可以是除发送自身外的
所有cpu，还可以是包括自身在内、甚至自身。在Linux下的IPI服务例程是在init_IRQ()中注册的。比如CALL_FUNCTION_VECTOR就可以请求目标cpu执行一个指定的函数。要想系统某个CPU发
出中断需要通过apic_wait_icr_idle确认或等待APIC_ICR处于空闲状态，然后通过__prepare_ICR和__prepare_ICR2准备要写入的数值。


其实质便是朝APIC_BASE寄存器中写入中断号、cpu标识等。和windows下的流程也差不多。需要说明的是IOAPIC通过读写相应寄存器来进行编程，但这些寄存器又不是通
用寄存器，而是通过映射成内存的物理地址进行访问。
下面是SoBeIt的话：
和PIC一样，控制本地APIC和IO APIC的方法是通过读写该单元中
的相关寄存器。不过和PIC不一样的是，Intel把本地APIC和IO APIC的寄存器都映射到了物
理地址空间，本地APIC默认映射到物理地址0xffe00000，IO APIC默认映射到物理地址
0xfec00000。windows HAL再进一步把本地APIC映射到虚拟地址0xfffe0000，把IO APIC映射
到虚拟地址0xffd06000，也就是说对该地址的读写实际就是对寄存器的读写，本地APIC里几
个重要的寄存有EOI寄存器，任务优先级寄存器(TPR)，处理器优先级寄存器(PPR)，中断命
令寄存器(ICR，64位)，中断请求寄存器(IRR，256位，对应每个向量一位)，中断在服务寄
存器(ISR，256位)等。IO APIC里几个重要的寄存器有版本寄存器，I/O寄存器选择寄存器、
I/O窗口寄存器(用要访问的I/O APIC寄存器的索引设置地址I/O寄存器选择寄存器，此时访
问I/O窗口寄存器就是访问被选定的寄存器)还有很重要的是一个IO重定向表，每一个表项是
一个64位寄存器，包括向量和目标模式、传输模式等相关位，每一个表项连接一条IRQ线，
表项的数目随处理器的版本而不一样，在Pentium 4上为24个表项。表项的数目保存在IO
APIC版本寄存器的[16:23]位。APIC系统支持255个中断向量，但Intel保留了0-15向量，可
用的向量是16-255。并引进一个概念叫做任务优先级=中断向量/16，因为保留了16个向量，
所以可用的优先级是2-15。当用一个指定的优先级设置本地APIC中的任务优先级寄存器TPR
后，所有优先级低于TPR中优先级的中断都被屏蔽，是不是很象IRQL的机制？事实上，APIC
HAL里的IRQL机制也就是靠着这个任务优先级寄存器得以实现。同一个任务优先级包括了16
个中断向量，可以进一步细粒度地区分中断的优先级。

有时候cpu还需要发送给除本身之外的cpu中断请求。在Linux下这是send_IPI_allbutself做的，下面看代码：
send_IPI_allbutself->__send_IPI_shortcut

static inline void __send_IPI_shortcut(unsigned int shortcut, int vector)
{
/*
* Subtle. In the case of the 'never do double writes' workaround
* we have to lock out interrupts to be safe. As we don't care
* of the value read we use an atomic rmw access to avoid costly
* cli/sti. Otherwise we use an even cheaper single atomic write
* to the APIC.
*/
unsigned int cfg;

/*
* Wait for idle.
*/
apic_wait_icr_idle();

/*
* No need to touch the target chip field
*/
cfg = __prepare_ICR(shortcut, vector);

/*
* Send the IPI. The write to APIC_ICR fires this off.
*/
apic_write_around(APIC_ICR, cfg);
}

参考文献：
http://www.nsfocus.NET/index.PHP?act=magazine&do=view&mid=2534 从IRQ到IRQL(APIC版)
http://blog.chinaunix.Net/u2/87729/showart_2251707.html
http://blog.chinaunix.net/u2/76349/showart_2194431.html Kernel硬件中断的初始化流程
http://wenku.baidu.com/view/e0a4806925c52cc58bd6bedc.html A32上Linux内核中断机制分析
http://hi.baidu.com/sysnap/blog/item/e58ab887d73ffc22c65cc396.html IRQ和中断号的关系
http://www.ibm.com/developerworks/cn/linux/kernel/l-ia64irq/index.html IA64 Linux 外部中断处理机制
http://www.linuxforum.net/forum/gshowflat.php?Cat=&Board=linuxK&Number=707247&o=all&fpart= kgdb的源代码分析
http://www.unixresources.net/linux/clf/linuxK/archive/00/00/59/03/590356.html 中断向量，IRQ，外部设备的关系分析
http://www.qqgb.com/Program/Clanguage/CJQ/Program_53626_5.html Linux 进程调度机制

nt4\ReactOS\Linux源码等也是必不可少的。

以上是我的一些粗略理解，有错误之处大家可以指出来。