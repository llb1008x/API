/***************************************************************************************
									学习计划

	指针，数组，字符串，函数，链表，文件操作，
	系统调用，linux环境编程，网络编程，linux驱动
	数据结构与算法
	linux内核	
	底层硬件还是上层软件又或者是+编程语言

	有时间整理一下写的函数库，是否有改进的地方，想法是否有问题




	这些都是培训上的课程，有必要调整一下，该学什么，怎么学，如何有效的学习，制定好计划
	坚持，专注的执行。
****************************************************************************************/

/*对一些概念的理解*/
{

	变量：
	变量的本质就是一个代号，不同类型的变量用来标记多大数量的内存为什么，为这段内存贴个标签，能进行什么操作，

	内存分配地址空间的时候，编译器会做一定的优化，为了提高效率，同一类型变量内存分布的一致性

	函数指针：
	指针指向的地址是代码段的函数,一个函数一般是有若干代码组成

	计算机内存分布
	{
		高位	  操作系统内核->

				栈->保存当前程序的执行情况，函数调用，变量的值这些临时的，变化的东西
					
					当然栈还有个特点就是先进后出，函数的压栈，弹栈，程序入口
					所以最先分配的函数地址要大点

					用户可以分配的内存->

				堆->

				数据段-> 

		低位     代码段->
	}


	内存分布
		栈区：存放函数的参数值，返回值，局部变量的能等
		
		堆区：用于动态分配内存
		
		未初始化数据：全局未初始化，静态未初始化数据
		
		初始化数据：全局初始化，静态初始化数据
		
		文字常量区：字符串常量
		
		代码区：可执行文件的二进制代码
		
		

	字符数组，字符串指针，堆


}





/*学习linux这些问题一定要搞懂*/
{
进程调度与内存管理

进程调度
{
	1.Linux进程和线程如何创建、退出？进程退出的时候，自己没有释放的资源（如内存没有free）会怎样？

	2.什么是写时拷贝？

	3.Linux的线程如何实现，与进程的本质区别是什么？

	4.Linux能否满足硬实时的需求？

	5.进程如何睡眠等资源，此后又如何被唤醒？

	6.进程的调度延时是多少？

	7.调度器追求的吞吐率和响应延迟之间是什么关系？CPU消耗型和I/O消耗型进程的诉求？

	8.Linux怎么区分进程优先级？实时的调度策略和普通调度策略有什么区别？

	9.nice值的作s用是什么？nice值低有什么优势？

	10.Linux可以被改造成硬实时吗？有什么方案？

	11.多核、多线程的情况下，Linux如何实现进程的负载均衡？

	12.这么多线程，究竟哪个线程在哪个CPU核上跑？有没有办法把某个线程固定到某个CPU跑？

	13.多核下如何实现中断、软中断的负载均衡？

	14.如何利用cgroup对进行进程分组，并调控各个group的CPU资源？

	15.CPU利用率和CPU负载之间的关系？CPU负载高一定用户体验差吗？

}


内存管理
{
	1.Linux系统的内存用掉了多少，还剩余多少？下面这个free命令每一个数字是什么意思？

	2.为什么要有DMA、NORMAL、HIGHMEM zone？每个zone的大小是由谁决定的？

	3.系统的内存是如何被内核和应用瓜分掉的？

	4.底层的内存管理算法buddy是怎么工作的？它和内核里面的slab分配器是什么关系？

	5.频繁的内存申请和释放是否会导致内存的碎片化？它的后果是什么？

	6.Linux内存耗尽后，系统会发生怎样的情况？

	7.应用程序的内存是什么时候拿到的？malloc()成功后，是否真的拿到了内存？应用程序的malloc()与free()与内核的关系究竟是什么？

	8.什么是lazy分配机制？应用的内存为什么会延后以最懒惰的方式拿到？

	9.我写的应用究竟耗费了多少内存？进程的vss/rss/pss/uss分别是什么概念？虚拟的，真实的，共享的，独占的，究竟哪个是哪个？

	10.内存为什么要做文件系统的缓存？如何做？缓存何时放弃？

	11.Free命令里面显示的buffers和cached分别是什么？二者有何区别？

	12.交换分区、虚拟内存究竟是什么鬼？它们针对的是什么性质的内存？什么是匿名页？

	13.进程耗费的内存、文件系统的缓存何时回收？回收的算法是不是类似LRU？

	14.怎样追踪和判决发生了内存泄漏？内存泄漏后如何查找泄漏源？

	15.内存大小这样影响系统的性能？CPU、内存、I/O三角如何互动？它们如何综合决定系统的一些关键性能？


}


遇到一个问题站在设计者的角度还是使用者的角度，转换思考的角度有些问题就可以想清楚点


}








文件的操作
{
Windows平台下
    以“文本”方式打开文件
    当读取文件的时候,系统会将所有的"\r\n"转换成"\n"
    当写入文件的时候,系统会将"\n"转换成"\r\n"写入
    以"二进制"方式打开文件,则读\写都不会进行这样的
    转换
    
在Unix/Linux平台下
    “文本”与“二进制”模式没有区别。
    \r\n作为两个字符原样输入输出


}




usb stack
{
      
    msm_otg (msm_otg_driver)
       
    msm_hsusb (usb_driver)
    
    28nm PHY
    – Ci13xxx_udc.c
    – Ci13xxx_msm.c
    android_usb (android_platform_driver)
   
    28nm PHY – msm_otg.c
    android.c
    msm_hsusb_host (ehci_msm_driver)
    ehci-msm.c
    ehci-msm2.c   
    
    
    Similarly, platform devices defined for the USB function drivers have platform drivers
    implemented in driver-specific files, e.g.:
    usb_mass_storage(fsg_platform_driver):[f_mass_storage.c]
    rndis(rndis_platform_driver)[f_rndis.c]
    usb_diag(usb_diag_driver)[f_diag.c]   
    
    
    
    usb core probe
    {
        msm_otg_probe() [msm_otg.c]
        Enables USB_HS1 peripheral bus clock (hs_pclk or cc_usb_hs1_hclk)
        Enables 3.3 and 1.8 VREGs
        Maps memory mapped (physical address) HS-USB OTG core registers to kernel address
        space
        Dynamically initializes the OTG state machine work queue
        Creates a new worker thread, k_otg, to queue the OTG work queue, etc.
         Registers the IRQ handler, i.e., msm_otg_irq
         Registers the callback function for PMIC ID and VBUS notifications
        ehci_msm_probe() [ehci-msm.c]; platform driver registration occurs in the EHCI host
        controller driver (ehc-hcd.c) init function

        Creates and initializes usb_hcd, used by the USB core
        Initializes the ehci-msm host controller-specific driver structure or instance:
        msm_hc_driver (type hc_driver) instance defined in ehci-msm.c
        Registration with core is deferred
        Registers the host driver with the OTG by calling otg_set_host()
        Calls otg_set_peripheral()

        usb_create_hcd
        The HCD probe is called before DCD. Consequently, by the time the DCD registers with
        OTG, HCD is already registered. As both DCD and HCD are registered, OTG kicks the
        state machine. The OTG state machine initial state is OTG_STATE_UNDEFINED. In
        this state, the USB hardware is reset and based on the ID/VBUS status, HCD/DCD is
        activated.
        ci13xxx_msm_probe() [ci13xxx_msm.c]
        Maps the USB registers from physical to kernel address space
        Calls udc_probe() [ci13xxx_udc.c]
        
        
        Initializes structure variables
        Assigns callbacks for the USB gadget driver usb_gadget_ops
        Initializes driver hardware bank register values
        
       
        Allocates and initializes a struct ci13xxx device assigned to _udc
        Saves an offsetted base address for different register I/O APIs, i.e., hw_aread(),
        hw_cread()
        Calls device_register() to register it to the system
        Calls otg_set_peripheral()
        Creates debugfs files, if enabled
        
        
        android_probe() [android.c]
        Registers a composite driver, android_usb
        Populates entries for the Android device of type android_dev, using platform data of the
        android_usb_device platform device
    
        composite_setup() is called during the USB device enumeration to handle standard USB setup
        requests.
    
    }
    



}




















