 
 
 1. Description

XXX客户从后台反馈，出现比较多的hang detect thread KE, 请尽快分析.

 

2. Analysis

Hang detect 是MTK  的独家设计，具体的设计和问题分析流程可以参考前面章节的说明.
从客户提交的KE db 来看， 我们看到大批量的thread 全部卡住，并且watchdog 的状态是：

    [179942.389198] 0)watchdog        D  
    [179942.389206] 0) ffffffc000085a84   
    [179942.389207] 0)    0  1374    320 0x00000008  
    [179942.389210] 0)Call trace:  
    [179942.389221] 0)[] __switch_to+0x74/0x8c  
    [179942.389234] 0)[] __schedule+0x314/0x794  
    [179942.389248] 0)[] schedule+0x24/0x68  
    [179942.389258] 0)[] __refrigerator+0x6c/0xf4  
    [179942.389269] 0)[] futex_wait_queue_me+0x150/0x158  
    [179942.389280] 0)[] futex_wait+0x120/0x20c  
    [179942.389291] 0)[] do_futex+0x184/0xa48  
    [179942.389301] 0)[] SyS_futex+0x88/0x19c  
    [179942.389313] 0)android.bg      D  
    [179942.389320] 0) ffffffc000085a84   
    [179942.389322] 0)    0  1447    320 0x00000008  
    [179942.389325] 0)Call trace:  
    [179942.389336] 0)[] __switch_to+0x74/0x8c  
    [179942.389350] 0)[] __schedule+0x314/0x794  
    [179942.389363] 0)[] schedule+0x24/0x68  
    [179942.389372] 0)[] __refrigerator+0x6c/0xf4  
    [179942.389385] 0)[] do_nanosleep+0x108/0x110  
    [179942.389396] 0)[] hrtimer_nanosleep+0x8c/0x108  
    [179942.389407] 0)[] SyS_nanosleep+0x90/0xa8  

__refrigerator(进程冻结技术)
此时你会发现很特别的__refrigerator 这个函数，怎么会停留在这里呢，貌似有点不占边， 其实此是当机器suspend 时，freezer user space task 后，强行将process
切换到__refrigerator 中等待，具体的流程说明可以参考
/kernel-3.10/Documentation/power/freezing-of-tasks.txt

如果你看到这样的情况，那么即说明当时系统的suspend 流程卡住了，并且不是卡在了driver 的early suspend 流程，而是pm_suspend 流程中了，因此你需要快速的找到
suspend 的kworker 卡在了哪里.

    [179942.393792] 0)Workqueue: autosleep try_to_suspend  
    [179942.393795] 0)Call trace:  
    [179942.393807] 0)[] __switch_to+0x74/0x8c  
    [179942.393821] 0)[] __schedule+0x314/0x794  
    [179942.393835] 0)[] schedule+0x24/0x68  
    [179942.393850] 0)[] synchronize_irq+0x88/0xc4  
    [179942.393865] 0)[] suspend_device_irqs+0xdc/0xe4  
    [179942.393878] 0)[] dpm_suspend_noirq+0x2c/0x288  
    [179942.393889] 0)[] dpm_suspend_end+0x34/0x84  
    [179942.393905] 0)[] suspend_devices_and_enter+0x150/0x4a4  
    [179942.393917] 0)[] enter_state+0x15c/0x190  
    [179942.393929] 0)[] pm_suspend+0x2c/0xa8  
    [179942.393939] 0)[] try_to_suspend+0x148/0x1a8  
    [179942.393955] 0)[] process_one_work+0x148/0x468  
    [179942.393968] 0)[] worker_thread+0x138/0x3c0  
    [179942.393979] 0)[] kthread+0xb0/0xbc  

从流程上看可以看到suspend 的流程已经进入了device 操作的尾声了.

    176static int suspend_enter(suspend_state_t state, boolbool *wakeup)  
    177{  
    178 int error;  
    179  
    180 if (need_suspend_ops(state) && suspend_ops->prepare) {  
    181     error = suspend_ops->prepare();  
    182     if (error)  
    183         goto Platform_finish;  
    184 }  
    185  
    186 error = dpm_suspend_end(PMSG_SUSPEND);  
    187 if (error) {  
    188     printk(KERN_ERR "PM: Some devices failed to power down\n");  
    189     goto Platform_finish;  
    190 }  
    191  
    192 if (need_suspend_ops(state) && suspend_ops->prepare_late) {  
    193     error = suspend_ops->prepare_late();  
    194     if (error)  
    195         goto Platform_wake;  

在这个过程中，有执行synchronize_irq， 这个是等目前的IRQ 做完为后续的disable irq: arch_suspend_disable_irqs 做准备.  当时的第一直觉是，
这个irq handler 执行太久，按理早就WDT 重启了，不至于等到hang detect 来检测到了。 于是看了一下synchronize_irq 的代码.


    45void synchronize_irq(unsigned int irq)  
    46{  
    47  struct irq_desc *desc = irq_to_desc(irq);  
    48  bool inprogress;  
    49  
    50  if (!desc)  
    51      return;  
    52  
    53  do {  
    54      unsigned long flags;  
    55  
    56      /* 
    57       * Wait until we're out of the critical section.  This might 
    58       * give the wrong answer due to the lack of memory barriers. 
    59       */  
    60      while (irqd_irq_inprogress(&desc->irq_data))  
    61          cpu_relax();  
    62  
    63      /* Ok, that indicated we're done: double-check carefully. */  
    64      raw_spin_lock_irqsave(&desc->lock, flags);  
    65      inprogress = irqd_irq_inprogress(&desc->irq_data);  
    66      raw_spin_unlock_irqrestore(&desc->lock, flags);  
    67  
    68      /* Oops, that failed? */  
    69  } while (inprogress);  
    70  
    71  /* 
    72   * We made sure that no hardirq handler is running. Now verify 
    73   * that no threaded handlers are active. 
    74   */  
    75  wait_event(desc->wait_for_threads, !atomic_read(&desc->threads_active));  
    76}  


可以看到最后的wait_event最后在wait_for_threads， 对应唤醒的代码是：


    831/* 
    832 * Interrupt handler thread 
    833 */  
    834static int irq_thread(voidvoid *data)  
    835{  
    836 struct callback_head on_exit_work;  
    837 static const struct sched_param param = {  
    838     .sched_priority = MAX_USER_RT_PRIO/2,  
    839 };  
    840 struct irqaction *action = data;  
    841 struct irq_desc *desc = irq_to_desc(action->irq);  
    842 irqreturn_t (*handler_fn)(struct irq_desc *desc,  
    843         struct irqaction *action);  
    844  
    845 if (force_irqthreads && test_bit(IRQTF_FORCED_THREAD,  
    846                 &action->thread_flags))  
    847     handler_fn = irq_forced_thread_fn;  
    848 else  
    849     handler_fn = irq_thread_fn;  
    850  
    851 sched_setscheduler(current, SCHED_FIFO, ¶m);  
    852  
    853 init_task_work(&on_exit_work, irq_thread_dtor);  
    854 task_work_add(current, &on_exit_work, false);  
    855  
    856 irq_thread_check_affinity(desc, action);  
    857  
    858 while (!irq_wait_for_interrupt(action)) {  
    859     irqreturn_t action_ret;  
    860  
    861     irq_thread_check_affinity(desc, action);  
    862  
    863     action_ret = handler_fn(desc, action);  
    864     if (action_ret == IRQ_HANDLED)  
    865         atomic_inc(&desc->threads_handled);  
    866  
    867     wake_threads_waitq(desc);  
    868 }  
    869  
    870 /* 
    871  * This is the regular exit path. __free_irq() is stopping the 
    872  * thread via kthread_stop() after calling 
    873  * synchronize_irq(). So neither IRQTF_RUNTHREAD nor the 
    874  * oneshot mask bit can be set. We cannot verify that as we 
    875  * cannot touch the oneshot mask at this point anymore as 
    876  * __setup_irq() might have given out currents thread_mask 
    877  * again. 
    878  */  
    879 task_work_cancel(current, irq_thread_dtor);  
    880 return 0;  
    881}  


即我们注册IRQ 有两种方式，requests_irq, request_thread_irq,  request_irq 只能注册irq handler for line, 而request_threaded_irq, 
可同时注册irq handler 以及 irq thread function,  irq thread function 即为这个irq 开一个thread, irq_thread, 然后执行你的irq thread function, 
对应这个过程不会disable irq, 不在irq 原子流程中了.  但synchronized_irq 即会等irq handler 做完也会等irq thread function 做完。

于是此问题就变成了肯定有人注册了irq thread function, 并且没有执行完毕，因为这个thread 肯定从irq_thread 中执行，于是追查，可以看到：

    [179942.389537] 0)irq/291-spi0.0  D  
    [179942.389544] 0) ffffffc000085a84   
    [179942.389546] 0)    0  1090      2 0x00000000  
    [179942.389549] 0)Call trace:  
    [179942.389561] 0)[] __switch_to+0x74/0x8c  
    [179942.389575] 0)[] __schedule+0x314/0x794  
    [179942.389588] 0)[] schedule+0x24/0x68  
    [179942.389601] 0)[] schedule_timeout+0x128/0x20c  
    [179942.389614] 0)[] __down_timeout+0x68/0xc8  
    [179942.389625] 0)[] down_timeout+0x5c/0x84  
    [179942.389640] 0)[] connection_read_data+0x38/0xac  
    [179942.389654] 0)[] mc_wait_notification+0xd0/0x148  
    [179942.389670] 0)[] gf516m_send_cmd_secdrv+0x68/0x1e8  
    [179942.389681] 0)[] gf516m_irq+0x50/0x638  
    [179942.389692] 0)[] irq_thread+0x110/0x164  
    [179942.389703] 0)[] kthread+0xb0/0xbc  


于是这个问题就变得很明显了，gf516m 这个指纹识别的irq, 通过request_thread_irq 注册，然后和TEE 通讯mc_wait_notification, 结果卡住了，
从而导致hang detect 重启. 

找汇顶的工程师协助处理，将流程改成irq — work queue 的方式执行，以规避掉目前的情况。

3. Root Cause
suspend 时因为gf516m driver IRQ 卡住，导致hang detect 触发重启.

4. Solution
vendor 修改driver 将流程改成irq-- work queue 执行的方式.

. Description

待机过程中会重复重启。

2. Analysis

用GAT解开db，查看SYS_KERNEL_LOG：
    [ 3005.572831].(4)[107:hang_detect][Hang_Detect] we should triger HWT ...  
    [ 3005.573649].(4)[107:hang_detect]BUG: failure at kernel-3.10/drivers/misc/mediatek/aee/aed/monitor_hang.c:437/hang_detect_thread()!  
    [ 3005.575584].(4)[107:hang_detect]Unable to handle kernel paging request at virtual address 0000dead  
    [ 3005.576695].(4)[107:hang_detect]pgd = ffffffc00007d000  
    [ 3005.577332].(4)[107:hang_detect][0000dead] *pgd=00000000ed807003, *pmd=00000000ed808003, *pte=0000000000000000  
    [ 3005.578356]-(4)[107:hang_detect]Internal error: Oops: 96000046 [#1] PREEMPT SMP  
    [ 3006.693872]-(4)[107:hang_detect]Modules linked in:  
    [ 3006.694237]-(4)[107:hang_detect]CPU: 4 PID: 107 Comm: hang_detect Tainted: P W O 3.10.72+ #1  
    [ 3006.695374]-(4)[107:hang_detect]task: ffffffc0ac4df000 ti: ffffffc0ac65c000 task.ti: ffffffc0ac65c000  
    [ 3006.696527]-(4)[107:hang_detect]PC is at hang_detect_thread+0x2b8/0x2c0  
    [ 3006.697345]-(4)[107:hang_detect]LR is at hang_detect_thread+0x2ac/0x2c0  

看到上面的log，那么就可以判断是hang detect的问题了。

首先先找下count关键字，看看是否有如下log：


    [Hang_Detect] hang_detect thread counts down -4529:20.  


结果是没有搜索到，SYS_KERNEL_LOG比较小，log被冲走了。因此我们从watchdog线程看起，搜索watchdog关键字：
    [ 3005.343913].(4)[107:hang_detect]watchdog D  
    [ 3005.344252].(4)[107:hang_detect] ffffffc0000864ec [ 3005.344849].(4)[107:hang_detect] 0 1533 410 0x00000008  
    [ 3005.345560].(4)[107:hang_detect]Call trace:  
    [ 3005.346089].(4)[107:hang_detect][] __switch_to+0x74/0x8c  
    [ 3005.346947].(4)[107:hang_detect][] __schedule+0x318/0x7b4  
    [ 3005.347814].(4)[107:hang_detect][] schedule+0x24/0x68  
    [ 3005.348638].(4)[107:hang_detect][] __refrigerator+0x7c/0x144  
    [ 3005.349540].(4)[107:hang_detect][] futex_wait_queue_me+0x150/0x158  
    [ 3005.350515].(4)[107:hang_detect][] futex_wait+0x13c/0x23c  
    [ 3005.351381].(4)[107:hang_detect][] do_futex+0x184/0xa48  
    [ 3005.352226].(4)[107:hang_detect][] SyS_futex+0x88/0x19c  


所以此时要看kworker进程调用栈，搜索kworker关键字：
    [ 3005.513786].(4)[107:hang_detect]kworker/u16:6 D  
    [ 3005.514121].(4)[107:hang_detect] ffffffc0000864ec [ 3005.514718].(4)[107:hang_detect] 0 6910 2 0x00000000  
    [ 3005.515437].(4)[107:hang_detect]Workqueue: autosleep try_to_suspend  
    [ 3005.515447].(4)[107:hang_detect]Call trace:  
    [ 3005.515965].(4)[107:hang_detect][] __switch_to+0x74/0x8c  
    [ 3005.516823].(4)[107:hang_detect][] __schedule+0x318/0x7b4  
    [ 3005.517690].(4)[107:hang_detect][] schedule+0x24/0x68  
    [ 3005.518515].(4)[107:hang_detect][] schedule_timeout+0x168/0x20c  
    [ 3005.519448].(4)[107:hang_detect][] wait_for_common+0xa0/0x148  
    [ 3005.520370].(4)[107:hang_detect][] wait_for_completion+0x10/0x1c  
    [ 3005.521313].(4)[107:hang_detect][] flush_kthread_work+0xe4/0x124  
    [ 3005.522255].(4)[107:hang_detect][] mc_fastcall+0x5c/0x70  
    [ 3005.523111].(4)[107:hang_detect][] mc_switch_core+0x60/0x8c  
    [ 3005.524001].(4)[107:hang_detect][] mc_cpu_offfline.part.1+0x54/0x68  
    [ 3005.524977].(4)[107:hang_detect][] mobicore_cpu_callback+0x98/0xa0  
    [ 3005.525943].(4)[107:hang_detect][] notifier_call_chain+0x68/0x11c  
    [ 3005.526897].(4)[107:hang_detect][] __raw_notifier_call_chain+0x8/0x14  
    [ 3005.527896].(4)[107:hang_detect][] _cpu_down+0x98/0x2a4  
    [ 3005.528743].(4)[107:hang_detect][] disable_nonboot_cpus+0xa8/0x168  
    [ 3005.529706].(4)[107:hang_detect][] suspend_devices_and_enter+0x1bc/0x4e8  
    [ 3005.530745].(4)[107:hang_detect][] enter_state+0x1e4/0x224  
    [ 3005.531620].(4)[107:hang_detect][] pm_suspend+0x2c/0xa8  
    [ 3005.532467].(4)[107:hang_detect][] try_to_suspend+0x1d8/0x240  
    [ 3005.533384].(4)[107:hang_detect][] process_one_work+0x134/0x470  
    [ 3005.534313].(4)[107:hang_detect][] worker_thread+0x138/0x3c0  
    [ 3005.535215].(4)[107:hang_detect][] kthread+0xb0/0xbc  


从流程上看可以看到suspend流程已经进入disable_nonboot_cpus()。但最终卡在mc_fastcall()，这时需要我们看下这个函数在干嘛。


    bool mc_fastcall(voidvoid *data)  
    {  
        struct fastcall_work fc_work = {  
            KTHREAD_WORK_INIT(fc_work.work, fastcall_work_func),  
            .data = data,  
        };  
      
        if (!queue_kthread_work(&fastcall_worker, &fc_work.work))  
            return false;  
        flush_kthread_work(&fc_work.work);  
        return true;  
    }  

从代码来看是等待fastcall_worker这个进程做完fc_work。我们看下fastcall_worker是哪个进程：

    int mc_fastcall_init(struct mc_context *context)  
    {  
        int ret = 0;  
        ctx = context;  
      
        fastcall_thread = kthread_create(kthread_worker_fn, &fastcall_worker, "mc_fastcall");  
        ......  
    }  


看到fastcall_worker的进程名是mc_fastcall，看下这个进程的调用栈：

    [ 3004.761752].(4)[107:hang_detect]mc_fastcall D  
    [ 3004.762080].(4)[107:hang_detect] ffffffc0000864ec [ 3004.762677].(4)[107:hang_detect] 0 218 2 0x00000000  
    [ 3004.763389].(4)[107:hang_detect]Call trace:  
    [ 3004.763918].(4)[107:hang_detect][] __switch_to+0x74/0x8c  
    [ 3004.764775].(4)[107:hang_detect][] __schedule+0x318/0x7b4  
    [ 3004.765642].(4)[107:hang_detect][] schedule+0x24/0x68  
    [ 3004.766467].(4)[107:hang_detect][] __refrigerator+0x7c/0x144  
    [ 3004.767368].(4)[107:hang_detect][] kthread_worker_fn+0xfc/0x19c  
    [ 3004.768300].(4)[107:hang_detect][] kthread+0xb0/0xbc  


咦，这不死锁了吗？suspend一定会让绝大部分进程进入__refrigerator，冻住这个进程。可是看起来后面的流程需要这个冻住的进程做事，这个死锁就导致了卡死重启了。

如何解开这个死锁？经过讨论后，mc_fastcall进程不能进入__refrigerator状态。因此需要调整代码，将mc_fastcall进程调整为non-freezeable进程。

3. Root Cause

mobicore驱动在suspend阶段存在死锁。

4. Solution

修正mobicore驱动。

*************************************************************************************************************
1. Description

XXX客户反馈，在拨打电话时，机器小概率卡死，等待几分钟后机器自动重启.

2. Analysis

Kernel Panic (内核错误)
从客户提供的LOG 来看，LOG 中有明显的Kernel Panic 的AEE DB, 解压DB， 在exp_main 里面可以看到：

    Exception Class: Kernel (KE)  
    PC is at [] hang_detect_thread+0x278/0x280  
      
    Current Executing Process:  
    [hang_detect, 107][kthreadd, 2]  
      
    Backtrace:  
    [] __do_kernel_fault.part.5+0x70/0x84  
    [] do_page_fault+0x218/0x364   
    [] do_translation_fault+0x40/0x4c      
    [] do_mem_abort+0x38/0x9c      
    [] el1_da+0x1c/0x88    
    [] kthread+0xb0/0xbc   
    [] ret_from_fork+0xc/0x30      
    [] 0xffffffffffffffff  


这个是hang detect 主动call bug, 即hang detect 检测到Android 上层已经卡住，主动重启手机.

先查看system server tick hang detect 的情况.
    [ 7892.901454] (0)[107:hang_detect][Hang_Detect] init found pid:1.  
    [ 7892.901530] (0)[107:hang_detect][Hang_Detect] mmcqd/0 found pid:172.  
    [ 7892.901565] (0)[107:hang_detect][Hang_Detect] surfaceflinger found pid:296.  
    [ 7892.901576] (0)[107:hang_detect][Hang_Detect] debuggerd found pid:312.  
    [ 7892.901583] (0)[107:hang_detect][Hang_Detect] debuggerd64 found pid:313.  
    [ 7892.901610] (0)[107:hang_detect][Hang_Detect] system_server found pid:771.  
    [ 7892.901618] (0)[107:hang_detect][Hang_Detect] ndroid.systemui found pid:1129.  
    [ 7892.901658] (0)[107:hang_detect][Hang_Detect] hang_detect thread counts down 0:24.  
    [ 7892.901664] (0)[107:hang_detect][Hang_Detect] dump system_ui all thread bt  


可以很明显的看到system server tick count=24, 即system server 已经发生了watchdog reboot (SWT), 但看到system server 的pid 还是771， 
说明system server 没有能够重启成功. 就需要进一步的分析为何system server 没有重启成功.

然后，我们马上查看system server 的kernel backtrace.

    [ 7893.231460] (0)[107:hang_detect]system_server   x  
    [ 7893.231466] (0)[107:hang_detect] ffffffc000085b94 [ 7893.231474] (0)[107:hang_detect]    0   771    345 0x00000009  
    [ 7893.231480] (0)[107:hang_detect]Call trace:  
    [ 7893.231490] (0)[107:hang_detect][] __switch_to+0x74/0x8c  
    [ 7893.231499] (0)[107:hang_detect][] __schedule+0x314/0x784  
    [ 7893.231509] (0)[107:hang_detect][] schedule+0x24/0x68  
    [ 7893.231520] (0)[107:hang_detect][] do_exit+0x52c/0x8e0  
    [ 7893.231530] (0)[107:hang_detect][] do_group_exit+0x40/0xb0  
    [ 7893.231541] (0)[107:hang_detect][] get_signal_to_deliver+0x2a0/0x55c  
    [ 7893.231552] (0)[107:hang_detect][] do_signal+0x238/0x564  
    [ 7893.231561] (0)[107:hang_detect][] do_notify_resume+0x58/0x64  
      
    [ 7893.231567] (0)[107:hang_detect]Binder_C        D  
    [ 7893.231573] (0)[107:hang_detect] ffffffc000085b94 [ 7893.231582] (0)[107:hang_detect]    0  1868    345 0x00000001  
    [ 7893.231587] (0)[107:hang_detect]Call trace:  
    [ 7893.231596] (0)[107:hang_detect][] __switch_to+0x74/0x8c  
    [ 7893.231605] (0)[107:hang_detect][] __schedule+0x314/0x784  
    [ 7893.231614] (0)[107:hang_detect][] schedule+0x24/0x68  
    [ 7893.231624] (0)[107:hang_detect][] schedule_preempt_disabled+0x10/0x24  
    [ 7893.231633] (0)[107:hang_detect][] __mutex_lock_slowpath+0x134/0x23c  
    [ 7893.231642] (0)[107:hang_detect][] mutex_lock+0x40/0x60  
    [ 7893.231652] (0)[107:hang_detect][] inv_flush_batch_store+0x40/0x6c  
    [ 7893.231662] (0)[107:hang_detect][] dev_attr_store+0x14/0x28  
    [ 7893.231674] (0)[107:hang_detect][] sysfs_write_file+0xd8/0x154  
    [ 7893.231686] (0)[107:hang_detect][] vfs_write+0xa4/0x194  
    [ 7893.231696] (0)[107:hang_detect][] SyS_write+0x40/0x8c  


这个时候你就会发现，system server 的主进程的状态是 X， 这个就意味着system server 的主进程其实已经dead, 然后在等其他的线程退出，上面的backtrace
不难发现是在等Binder_C 这个线程退出，而这个线程在等mutex lock， 一直无法退出.

3. Root Cause

从backtrace 追查，这个并非MTK 所撰写的代码，然后找对应的客户工程师确认，他们的sensor driver 有问题，可能导致无限等待的情况，已经找相关的工程师修正。



***************************************************************************************************************************
1. Description

手机在灭屏一段时间后（较长时间，半小时以上）再按powerkey不能点亮屏幕，只能长按powerkey 重新开机。

2. Analysis

eng版本或mtklogger打开的情况下，发生hang detect后会卡在那边而不会重启，表现为卡死，目的是保留现场，用于调试。

拿到uart log后，搜索counts关键字，看看是卡在哪一阶段：

    [ 6396.581177] .(0)[170:hang_detect][Hang_Detect] init found pid:1.  
    [ 6396.582144] .(0)[170:hang_detect][Hang_Detect] mmcqd/0 found pid:183.  
    [ 6396.583057] .(0)[170:hang_detect][Hang_Detect] surfaceflinger found pid:300.  
    [ 6396.584009] .(0)[170:hang_detect][Hang_Detect] debuggerd found pid:1627.  
    [ 6396.584877] .(0)[170:hang_detect][Hang_Detect] debuggerd64 found pid:1628.  
    [ 6396.585811] .(0)[170:hang_detect][Hang_Detect] system_server found pid:2285.  
    [ 6396.586727] .(0)[170:hang_detect][Hang_Detect] ndroid.systemui found pid:3523.  
    [ 6396.587735] .(0)[170:hang_detect][Hang_Detect] hang_detect thread counts down 0:20.  
    [ 6396.588722] .(0)[170:hang_detect][Hang_Detect] dump system_ui all thread bt  

可以看到是20，表示watchdog处于打印调用栈，打印调用栈需要通过debuggerd才行，这时应该看下deubggerd的调用栈，看看是否卡住了。搜索debuggerd关键字，
发现没有看到debuggerd调用栈，debuggerd应该是没有卡住的，那我们回头看下watchdog线程调用栈。

    [ 6397.822449] .(2)[170:hang_detect]watchdog D  
    [ 6397.822455] .(2)[170:hang_detect] ffffffc000088390   
    [ 6397.822457] .(2)[170:hang_detect] 0 4062 1717 0x00000000  
    [ 6397.822461] .(2)[170:hang_detect]Call trace:  
    [ 6397.822469] .(2)[170:hang_detect][] __switch_to+0x74/0x8c  
    [ 6397.822476] .(2)[170:hang_detect][] __schedule+0x434/0x8ac  
    [ 6397.822484] .(2)[170:hang_detect][] schedule+0x24/0x74  
    [ 6397.822491] .(2)[170:hang_detect][] io_schedule+0x64/0xa0  
    [ 6397.822498] .(2)[170:hang_detect][] bit_wait_io+0x30/0x54  
    [ 6397.822506] .(2)[170:hang_detect][] __wait_on_bit+0x9c/0xd4  
    [ 6397.822514] .(2)[170:hang_detect][] wait_on_page_bit_killable+0x78/0x8c  
    [ 6397.822521] .(2)[170:hang_detect][] __lock_page_or_retry+0xa8/0xd0  
    [ 6397.822528] .(2)[170:hang_detect][] filemap_fault+0x754/0x814  
    [ 6397.822535] .(2)[170:hang_detect][] __do_fault+0x34/0xa0  
    [ 6397.822542] .(2)[170:hang_detect][] do_read_fault.isra.94+0x1d0/0x2a4  
    [ 6397.822552] .(2)[170:hang_detect][] handle_mm_fault+0x3dc/0xac4  
    [ 6397.822561] .(2)[170:hang_detect][] do_page_fault+0x278/0x390  
    [ 6397.822568] .(2)[170:hang_detect][] do_mem_abort+0x38/0x9c  


  看到发生缺页异常，而最终卡在io，应该是emmc卡住了。需要检查下mmcqd进程的调用栈：

    [ 6397.824104] .(2)[170:hang_detect][Hang_Detect] dump mmcqd/0 all thread bt  
    [ 6397.824113] .(2)[170:hang_detect]mmcqd/0 D  
    [ 6397.824119] .(2)[170:hang_detect] ffffffc000088390   
    [ 6397.824121] .(2)[170:hang_detect] 0 183 2 0x00000000  
    [ 6397.824125] .(2)[170:hang_detect]Call trace:  
    [ 6397.824133] .(2)[170:hang_detect][] __switch_to+0x74/0x8c  
    [ 6397.824141] .(2)[170:hang_detect][] __schedule+0x434/0x8ac  
    [ 6397.824148] .(2)[170:hang_detect][] schedule+0x24/0x74  
    [ 6397.824157] .(2)[170:hang_detect][] __mmc_claim_host+0xb8/0x210  
    [ 6397.824165] .(2)[170:hang_detect][] mmc_get_card+0x10/0x1c  
    [ 6397.824172] .(2)[170:hang_detect][] mmc_blk_issue_rq+0x210/0x44c  
    [ 6397.824179] .(2)[170:hang_detect][] mmc_queue_thread+0xa8/0x16c  
    [ 6397.824186] .(2)[170:hang_detect][] kthread+0xe0/0xf8  


看到这个调用栈不是mmcqd正常的调用栈，应该是卡在emmc了，我们搜到__mmc_claim_host还看到其他进程也卡在这里了：


    [ 6397.821781] .(2)[170:hang_detect]rpmb_Dci D  
    [ 6397.821788] .(2)[170:hang_detect] ffffffc000088390   
    [ 6397.821789] .(2)[170:hang_detect] 0 420 2 0x00000000  
    [ 6397.821793] .(2)[170:hang_detect]Call trace:  
    [ 6397.821801] .(2)[170:hang_detect][] __switch_to+0x74/0x8c  
    [ 6397.821809] .(2)[170:hang_detect][] __schedule+0x434/0x8ac  
    [ 6397.821817] .(2)[170:hang_detect][] schedule+0x24/0x74  
    [ 6397.821825] .(2)[170:hang_detect][] __mmc_claim_host+0xb8/0x210  
    [ 6397.821834] .(2)[170:hang_detect][] emmc_rpmb_req_handle+0x60/0xd8  
    [ 6397.821841] .(2)[170:hang_detect][] emmc_rpmb_listenDci+0x14c/0x1a0  
    [ 6397.821848] .(2)[170:hang_detect][] kthread+0xe0/0xf8  


搜索emmc异常相关的log，发现emmc在suspend 进入sleep时有发生fail，然后再resume时也fail掉了：

host向emmc发送CMD5进入sleep mode发生timeout error:

    [ 4268.440079] .(0)[2339:system_server][msdc][msdc_command_resp_polling]: msdc0 CMD MSDC_INT_CMDTMO Arg  
    [ 4268.440081] .(0)[2339:system_server][msdc][msdc_command_resp_polling]: msdc0 XXX CMD ARG is R1B, TMO not reset hw  
    [ 4268.440087] .(0)[2339:system_server][msdc]Sleep_Awake CMD timeout, MSDC_PS 81ff0002  
    [ 4268.440090] .(0)[2339:system_server][msdc]eMMC sleep CMD5 TMO will reinit  


host向emmc发送CMD5 awake emmc发生timeout error:


    [ 4268.471086] .(0)[2339:system_server]PM: Some devices failed to suspend, or early wake event detected  
    [ 4268.471178] .(0)[2339:system_server][msdc][msdc_command_resp_polling]: msdc0 CMD MSDC_INT_CMDTMO Arg  
    [ 4268.471180] .(0)[2339:system_server][msdc][msdc_command_resp_polling]: msdc0 XXX CMD ARG is R1B, TMO not reset hw  
    [ 4268.471189] .(0)[2339:system_server][msdc]msdc0 -> XXX CMD Error Resp  PID  
    [ 4268.471201] .(0)[2339:system_server]mmc0: error -110 during resume (card was removed?)  


剩下通常需要理清是否单体或某个型号emmc复现，需要抓uart log看emmc的行为（mobile log无法看到，因为异常时无法写到emmc）。

3. Root Cause

多半是emmc问题，需要厂商一起排查。

4. Solution

多半是emmc问题，需要厂商一起排查。