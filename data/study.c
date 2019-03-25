/**********************************************************************************************************************************
									
目录
1.linux常用命令

2.基本概念

3.编程语言

4.平台开发

5.相关文档


***********************************************************************************************************************************/




/***********************************************************************************************************************************
*1.linux 常用命令
***********************************************************************************************************************************/
{
    如何将shell命令的输出信息自动输出到文件中保存
    sh batchjob.sh 2>&1 | tee mylog.log
    
    查看系统环境变量
    printenv
    
    gcc -E hello.c -o hello.i   预处理
    gcc -S hello.i -o hello.s   编译
    gcc -c hello.s -o hello.o   汇编
    gcc hello.o -o hello_elf    链接
    
    
    线程编译命令  -lpthread
    
    VirtualBoxVM --startvm <name>  启动虚拟机 
    
    
    
}


/***********************************************************************************************************************************/



































/*************************************************************************************************************************************
*4.基本概念
*************************************************************************************************************************************/
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

        遇到一个问题站在设计者的角度还是使用者的角度，转换思考的角度有些问题就可以想清楚点
    }




        内存管理：
        内核所管理的另外一个重要资源是内存。为了提高效率,如果由硬件管理虚拟内存,内存是按照所谓的
        内存页方式进行管理的(对于大部分体系结构来说都是 4KB)。Linux 包括了管理可用内存的方式,以及物
        理和虚拟映射所使用的硬件机制。
        不过内存管理要管理的可不止 4KB 缓冲区。Linux 提供了对 4KB 缓冲区的抽象,例如 slab 分配器。
        这种内存管理模式使用 4KB 缓冲区为基数,然后从中分配结构,并跟踪内存页使用情况,比如哪些内存页是
        满的,哪些页面没有完全使用,哪些页面为空。这样就允许该模式根据系统需要来动态调整内存使用。
        为了支持多个用户使用内存,有时会出现可用内存被消耗光的情况。由于这个原因,页面可以移出内存
        并放入磁盘中。这个过程称为交换,因为页面会被从内存交换到硬盘上。内存管理的源代码可以
        在 ./linux/mm 中找到。
        
        
        


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

}


/*************************************************************************************************************************************/
    


         

























/***********************************************************************************************************************************
*3.编程语言
***********************************************************************************************************************************/
/*python 课程*/
{
    python基础语法体系
    {
        1.编写自己的模块
            将自己的模块加入到脚本执行路径中  sys.path.expanduser("路径")  

        模块中增加测试代码：
            #!/usr/bin/env python
            # coding=utf-8

            #正常的功能
            def hello():
                print("hello,world!")
                
                
            #测试部分    
            def test():
                hello()    
                
            #判断是作为模块还是主程序
            if __name__ == '__main__':test()    

            #定义，导入一些公共接口
            __all__:   
        
        查找帮助文档：
            #导入库    
            import copy

            help(copy.copy)
            #或者
            print copy.copy.__doc__

            查找源码
            sys.path
            #或者
            print copy.__file__



    }

}


/*     C++       不要浮躁*/
{
    《Effective C++》和《More Effective C++》以及《Exceptional C++》
    《C++面向对象高效编程（C++ Effective Object-Oriented Software Construction）》
    《面向对象软件构造(Object-Oriented Software Construction)》《设计模式（Design Patterns）》《The Art of Computer Programming》； 
        
     构造函数的特点
        1.对象实例化时被自动调用
        2.构造函数与类同名，没有返回值
        3.可以有多个重载
        4.实例化对象时只用到一个构造函数
        5.用户没有定义构造函数时，编译器自动生成一个构造函数   
        
        //Teacher(string name,int age):m_strName("james"),m_iAge(10){} 构造函数初始化列表
        初始化列表先于构造函数执行，只能用于构造函数，可以同事初始化多个数据成员   
     
     拷贝构造函数                    
     当采用直接初始化或复制初始化实例化对象时个自动调用拷贝构造函数，拷贝构造函数的参数是确定的，不能重载	                    
         Student(const Student &stu); 
         
     析构函数  释放资源，在对象销毁时自动调用，没有返回值，没有参数也不能重载
         ～Student(); 
         
         2.先把C语言的基本东西熟悉一下，好多东西都忘了
    gdb的使用
        编译 gcc  -g  test.c -o  test
        执行 gdb test    
        start 开始执行
        p 打印变量
        next 单步执行
        step 跳转到函数内部
        bt 查看函数堆栈
        f 进入哪个堆栈
        x/3d  0x7fffffffdafc   从内存地址连续打印多个地址
        
        
    预处理，编译，汇编，链接  
        
        宏函数
        #define N(n)  n*10
        #define ADD(a,b) (a)+(b)
        
        typedef 给一个变量类型取别名
        typedef int U8;
        typedef   (*)(....)函数指针   
        typedef   (*)[]数组指针  
        
        #define 与typedef的区别
        在C/C++语言中，typedef常用来定义一个标识符及关键字的别名，它是语言编译过程的一部分，但它并不实际分配内存空间，
        #define为一宏定义语句，通常用它来定义常量(包括无参量与带参量)，以及用来实现那些“表面似和善、背后一长串”的宏，它本身并不在编
        译过程中进行，而是在这之前(预处理过程)就已经完成了，但也因此难以发现潜在的错误及其它代码维护问题，
        
        
        typedef是为一个数据类型起一个新的别名，如typedef int INT;那么要告诉我的是INT表示整型，typedef int* INTPTR;则告诉我们INTPTR是一个指向整型变量的指针类型，
        这点与#define是决然不同的，#define只是作简单的字符串替换，不表达 任何含义

        typedef   有作用域的限制


    结构体 struct 不同变量类型的集合
        typedef struct weapon{
                    char name[20];
                    int atk;
                    int prince;
            
                }WAEPON;
                
        WEAPON ak47;         
        结构体里的字符串赋值
        strcpy(ak47.name,"baoge");
        或
        WEAPON ak47={"baoge",1,100};
        
        (*weapon1).name,weapon1->name;
           
    共用体
        union 几个不同类型变量共用同一片内存
        
        
        与 &  ，清零，保存哪一位
        或 |  ，置位， 
        异或^ ,定位翻转，数值交换       
         
}


/*************************************************************************************************************************************/























/*************************************************************************************************************************************
*2.平台开发
*************************************************************************************************************************************/
/*bingup msm8916_64  20171231*/
{
    start :
        怎么为DragonBoard 410c定制Android系统
        http://blog.csdn.net/cuichuankai/article/details/49866501
        {
            下载源码：https://www.codeaurora.org/
            下载命令：
                repo init -u git://codeaurora.org/platform/manifest -b release -m LA.BR.1.2.4-01810-8x16.0.xml
                repo sync -c --no-tags -j4 
                
                repo init -u git://codeaurora.org/platform/manifest.git -b release -m LA.BR.1.2.9-04310-8x16.0.xml --repo-url=git://codeaurora.org/tools/repo.git --repo-branch=caf-stable
               
                msm8916_64 December 20, 2017 	 LA.BR.1.2.9-04310-8x16.0 	 msm8916_64 	LA.BR.1.2.9-04310-8x16.0.xml	 07.01.02    
                
            制作sd卡镜像：
                先查看sd卡是哪个：
                sudo fdisk -l
                制作命令：
                sudo dd if=db410_sd_install_android.img of=/dev/XXX bs=2M
                sync
                
                
            fastboot方式烧写：
              adb reboot-bootloader
              
              sudo  fastboot devices
              sudo  fastboot flash partition gpt_both0.bin
              sudo  fastboot flash hyp hyp.mbn
              sudo  fastboot flash modem NON-HLOS.bin
              sudo  fastboot flash rpm rpm.mbn
              sudo  fastboot flash sbl1 sbl1.mbn
              sudo  fastboot flash sec sec.dat
              sudo  fastboot flash tz tz.mbn
              sudo  fastboot flash aboot emmc_appsboot.mbn
              sudo  fastboot erase boot
              sudo  fastboot erase rootfs
              sudo  fastboot erase devinfo   
              
              
            编译源码：
                source build/envsetup.sh 
                lunch msm8916_64-userdebug 

                make -j10 | tee log.txt
                
                1.                
                ninja: error: 'out/target/product/msm8916_64/obj_arm/SHARED_LIBRARIES/libqct_resampler_intermediates/export_includes', needed by 'out/target/product/msm8916_64/obj_arm/SHARED_LIBRARIES/libaudioresampler_intermediates/import_includes', missing and no known rule to make it
                build/core/ninja.mk:148: recipe for target 'ninja_wrapper' failed
                make: *** [ninja_wrapper] Error 1

                解决：配置好java环境和jack_vm_args。

                # set java 8
                export JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64
                export JRE_HOME=$JAVA_HOME/jre
                export PATH=$JAVA_HOME/bin:$JRE_HOME/bin:$PATH
                export CLASSPATH=$CLASSPATH:.:$JAVA_HOME/lib:$JAVA_HOME/jre/lib


                # set for android N build
                export ANDROID_JACK_VM_ARGS="-Dfile.encoding=UTF-8 -XX:+TieredCompilation -Xmx4g"
                echo "ANDROID_JACK_VM_ARGS=$ANDROID_JACK_VM_ARGS"



                ninja: error: 'out/target/product/msm8916_64/obj/SHARED_LIBRARIES/libsrsprocessing_intermediates/export_includes', needed by 'out/target/product/msm8916_64/obj/SHARED_LIBRARIES/libaudioflinger_intermediates/import_includes', missing and no known rule to make it



                No private recovery resources for TARGET_DEVICE msm8916_64


                prebuilts/gcc/linux-x86/aarch64/aarch64-linux-android-4.9/aarch64-linux-android/bin/ld.gold: error: version script assignment of LIBC to symbol __propClientDispatch failed: symbol not defined
                prebuilts/gcc/linux-x86/aarch64/aarch64-linux-android-4.9/aarch64-linux-android/bin/ld.gold: error: version script assignment of LIBC to symbol __propClientDispatchWrite failed: symbol not defined
                clang++: error: linker command failed with exit code 1 (use -v to see invocation)


                编译过了，但是屏幕一直无法正常使用
                
        }    
       
        下载sdm660的代码
        repo init -u git://codeaurora.org/platform/manifest.git -b release  -m LA.UM.6.2.r1-07700-sdm660.0.xml  --repo-url=git://codeaurora.org/tools/repo.git --repo-branch=caf-stable


}


/*4412开发板*/
{
    烧写镜像文件
    {
           烧写镜像文件不能用fastboot，需要通过串口进入uboot模式
        需要注意的是,fastboot 命令只能用来和开发板的 Uboot 模式进行交互,前面烧写镜像时提到的那些
        命令也都是运行在开发板的 UBOOT 模式。 

           烧写步骤：
        1.四个镜像文件
        system.img ，ramdisk-uboot.img，u-boot-iTOP-4412.bin，u-boot-iTOP-4412.bin

        2.打开超级终端,然后上电启动开发板,按“回车”,进入 Uboot 模式,不明白 Uboot 模式可以
        参考前面“Uboot 模式和文件系统模式”。如下图所示,进入 Uboot 模式

        3.创建 eMMC 分区并格式化。如果原来已经做过此步骤,则可以跳过,不必每次烧写前都分区和格
        式化。在超级终端中,输入下面分区和格式化命令。
        —fdisk -c 0
        —fatformat mmc 0:1
        —ext3format mmc 0:2
        —ext3format mmc 0:3
        —ext3format mmc 0:4

        (5)在 Windows 命令行中,输入下面的命令:
        fastboot flash bootloader u-boot-iTOP-4412.bin
        特别提醒,不建议用户烧写“u-boot-iTOP-4412.bin”这个文件,可跳过此步骤,因为出厂前已经烧
        写过这个镜像文件了。
        fastboot flash boot boot.img
        fastboot flash kernel zImage
        fastboot flash ramdisk ramdisk-uboot.img
        fastboot flash system system.img
        fastboot -w
    }


    搭建最小服务器
    {
            下面是设置 PC 机和开发板以太网的例子:
        1)设置 PC 主机 IPV4 地址为 192.168.1.2(IP 地址可以根据实际情况选择)
        2)连接开发板和主机的串口,网口,启动开发板,系统启动后,设置 Android 的 IP 地址和主机在同一
        个网段,例如,在串口中输入:
        ifconfig eth0 192.168.1.130 (IP 地址可以根据实际情况自行确定)
        测试以太网连接,在串口中输入:
        ping 192.168.1.2(开发板 ping PC)
        或者在 PC 的 cmd 窗口中输入:
        ping 192.168.1.130(PC ping 开发板)
        然后可以看到 ping 命令执行的结果。
        开发板连接路由器例子:
        ifconfig eth0 192.168.1.130 netmask 255.255.255.0 up
        route add default gw 192.168.1.1 dev eth0
        setprop net.dns1 192.168.1.1


            搭建一个tftp服务器：
        {
            TFTP(Trivial File Transfer Protocol,简单文件传输协议),是一个基于 UDP 协议实现的用于在客户
            机和服务器之间进行简单文件传输的协议,适合于开销不大、不复杂的应用场合。TFTP 协议专门为小文件传
            输而设计,只能从服务器上获取文件,或者向服务器写入文件,不能列出目录,也不能进行认证。
            根据上面关于 TFTP 的介绍,实现 TFTP 我们需要搭建一个 TFTP 的服务器,iTOP-4412 开发板当做客
            户端。
            使用的虚拟机 Ubuntu 来当做服务器,下面我们先讲解一下服务器端的配置。
            根据上面关于 TFTP 的介绍,实现 TFTP 最终需要搭建一个 TFTP 的服务器,iTOP-4412 开发板当做客
            户端。
            
            搭建服务器
                在的虚拟机 Ubuntu 上打开终端,首先输入命令:sudo apt-get install xinetd,安装 xinetd。安装完
                xinetd,接下来输入命令安装 tftp 和 tftpd:sudo apt-get install tftp tftpd。
                然后建立 TFTP 的配置文件,使用命令:vi /etc/xinetd.d/tftp 建立文件,写入下面的代码:
                
                service tftp
                {
                    socket_type = dgram
                    protocol    = udp
                    wait        = yes
                    user        = root
                    server      = /usr/sbin/in.tftpd
                    server_args = -s /home/llb/project/PRO/exynos4412/tftpboot
                    disable     = no
                    per_source  = 11
                    cps         = 100 2
                    flags       = IPv4
                }

                其中 server_args 设置的/var/tftpboot 目录是 tftp 服务器的目录
                
                先互ping一下看是否联通
                如果返回上面的信息就表示开发板和 TFTP 服务器是连通的,现在我们获取 TFTP 上的文件,在开发板
                的串口输入:tftp -g -l test.txt -r test.txt 192.168.31.18
                
                
                这个地方好像一直是只可以从服务器上下载，不能往上推文件
                tftp -g -r file ip//从TFTP下载文件
                tftp -p -l file ip//向TFTP上传文件
        
        }

    }


    内核启动流程
    {
            编译之后生成的System.map文件 
        System.map是内核的内核符号表，在这里可以找到函数地址，变量地址，包括一些链接过程中的地址定义等等， 
        build/out/linux/System.map（这里列出一些关键部分）

            通过反汇编命令对vmlinux进行反汇编，可以解析出详细的汇编代码，包括了一些地址 
        指令如下：
        arm-linux-gnueabi-objdump -D  vmlinux  >  vmlinux_objdump.txt

            通过arm-linux-gnueabi-readelf -s vmlinux查看各个段的布局
    }
 



    编译源码
    {
           安装make 3.81
        如果之前没有安装过make, 可以先在configure之后运行 sh build.sh(README文件中有说明),再运行make install.
        解压之后，进入make-3.81目录， 
        1)
        ./configure  --prefix=$HOME/jdeng/local
        2)
        make 
        3) optional (也可不选)
        make check
        4)
        make install
        make clean是去掉安装.
        其中configure加安装目录是因为要装一个本地的make, 避免跟系统make冲突了，而且在服务器上也没有管理员的权限。不然默认是安装在/usr/local下。
        --program-prefix=PREFIX
        --program-suffix=SUFFIX 可以给程序加前缀或后缀
        configure --help 可看帮助 
        
        
        可能是ubuntu版本不一样，有差异，所以ubuntu16编译的时候有那么多的问题，但是一套源码怎么有那么多的问题
        编译是个问题


        
        ./build_android.sh: line 71: mkimage: command not found

        ==============================================================================================================

        解决方案是：

        

        1. 将 iTop4412_uboot_scp_20141224.tar 

            iTop4412_Kernel_3.0_20150403.tar

        iTop4412_ICS_20150413.tar

        解压出来的三个文件夹，放在同一个目录下


        2. 将 uboot/tools/目录里面的 mkimage.c 和 mkimage.h 复制到 /usr/bin/ 目录下

        3. 执行apt-get install uboot-mkimage 命令


        最后，使用讯为iTOP4412开发板，终于顺利生成如下四个文件：

        1. system.img                       210040 KB

        2. ramdisk-uboot.img                901    KB

        3. u-boot-iTOP-4412.bin             515    KB

        4. zImage                           3907   KB

        并且通过fastboot模式，将四个文件成功烧写下载到开发板，现在开发板的Android系统，正常运行。

        #modify by lilubao for compile 20180113
        export PATH=$PATH:/usr/local/arm/arm-2009q3/bin


    }



    搭建nfs服务器
    {
        NFS 是 Network FileSystem 的缩写,是由 SUN 公司研制的 UNIX 表示层协议(pressentation layer
        protocol),NFS 是基于 UDP/IP 协议的应用。它的最大功能就是可以通过网络让不同的机器,不通的操作
        系统彼此共享文件,可以通过 NFS 挂载远程主机的目录,访问该目录就像访问本地目录一样,所以也可以简
        单的将它看做一个文件服务器。通过 NFS 服务,可以实现在线调试文件系统或应用程序,而不用像传统的方
        式生成文件系统镜像,然后烧写到 iTOP-4412 的 eMMC 里,最后再启动开发板。通过 NFS 服务可以提高
        我们的调试效率。
        
        sudo apt-get install nfs-kernel-server
        
        安装完成,接下来需要配置/etc/exports,使用 vi 命令打开/etc/exports,在/etc/export 文件的最后
一行添加:
        /home/llb/project/PRO/exynos4412/nfs *(rw,sync,no_root_squash)
        
        接下来重启 portmap 服务,在控制台输入“/etc/init.d/rpcbind restart”命令,然后重启 nfs 服务,
输入“/etc/init.d/nfs-kernel-server restart”命令。


        完成前面的搭建工作之后,就可以在虚拟机 Ubuntu 本机上验证一下,例如把/home/llb/project/PRO/exynos4412/nfs
挂载到/mnt 目录下,需要输入“sudo mount -t nfs localhost:/home/llb/project/PRO/exynos4412/nfs /mnt”命令。

        实现 nfs 文件系统需要修改 Linux 最小文件系统的 etc/init.d/ifconfig-eth0 文件,使用 vi 命令修改第
八行,修改为“if grep -q nfs /proc/mounts ; then”,修改完成后保存并退出

        root=/dev/nfs rw nfsroot=192.168.31.18:/home/llb/project/PRO/exynos4412/nfs
ip=192.168.31.230:192.168.31.18:192.168.31.1:255.255.255.0:iTOP:eth0:off rootfstype=ext4
init=/linuxrc console=ttySAC2,115200"

        Boot options 配置界面,然后再 Default kernel command 里面输入:
        root=/dev/nfs rw nfsroot=192.168.1.103:/home/topeet/linux/app/system
        ip=192.168.1.230:192.168.1.103:192.168.1.1:255.255.255.0:iTOP:eth0:off rootfstype=ext4
        init=/linuxrc console=ttySAC2,115200"。
        下面讲解一下上面输入命令的命令里面的几个参数:
        root=/dev/nfs rw nfsroot=192.168.1.103:/home/minilinux/system 表示挂载的 nfs 服务器 ip 是
        192.168.1.103,挂载的目录是/home/minilinux/system,(注意:/home/minilinux/是前面我们搭建 nfs
        服务器设置的)
        ip=192.168.1.230:192.168.1.103:192.168.1.1:255.255.255.0:iTOP:eth0:off 这里面,第一个
        ip192.168.1.230 是我们开发板的 ip 地址,第二个 ip192.168.1.103 是 nfs 服务器的 ip,第三个 ip192.168.1.1
        是开发板的网关,255.255.255.0 是子网掩码,iTOP 是开发主机的名字(一般无关紧要,可以随便填写),
        eth0 是网卡设备的名称。
        至此,Linux 内核的配置就完成了,退出 make menuconfig 配置,会弹出保存窗口,使用键盘的左右
        方向键,选中上图的”Yes“,然后输入回车,退出保存配置,如下图:
        然后输入 make 命令开始编译内核,这样 Linux 内核支持 nfs 制作完成了。
        将新生成的内核烧写到开发板,重启开发板,就可以使用 NFS 文件系统了。
    
    }
    
            
}



tensorflow
{
    1.电脑相关软件版本

        1.ubuntu18.04 + CUDA10 + CUDNN7 deb安装  这篇文档有效 
        https://blog.csdn.net/yjt1325/article/details/84641062
        
        安装cuda10  ubuntu18 支持cuda10
            `sudo dpkg -i cuda-repo-ubuntu1804-10-0-local-10.0.130-410.48_1.0-1_amd64.deb`
            `sudo apt-key add /var/cuda-repo-<version>/7fa2af80.pub`
            `sudo apt-get update`
            `sudo apt-get install cuda`
            
            
        2.nvidia显卡安装，三种方法 
        https://linuxconfig.org/how-to-install-the-nvidia-drivers-on-ubuntu-18-04-bionic-beaver-linux#h7-automatic-install-using-ppa-repository-to-install-nvidia-beta-drivers        
        注意事项
            1. 当进入蓝色背景的界面perform mok management 后，选择 enroll mok , 
            2. 进入enroll mok 界面，选择 continue , 
            3. 进入enroll the key 界面，选择 yes ,  
            4. 接下来输入你在安装驱动时输入的密码， 
            5. 之后会跳到蓝色背景的界面perform mok management 选择第一个 reboot
            
        检测显卡，推荐显卡驱动
        ubuntu-drivers devices   
        检验显卡是否安装成功
        nvidia-sim
        
        但是还有问题  版本不匹配  软件版本更新导致 ，关闭更新
        Failed to initialize NVML: Driver/library version mismatch
        https://my.oschina.net/wangsifangyuan/blog/1606093
            
        1.python  2.7,3.6 anaconda 安装了3.7.1     命令   ls -l /usr/bin/python*
        
        查看显卡版本          lspci | grep -i nvidia
        nvcc  -V              cuda  10
        
        设置环境
        conda create -n tensorflow pip python=3.6 #python版本根据要安装的tensorflow选择
        启动tensorflows
        source activate tensorflow
        打开anaconda
        anaconda-navigator
        
        
        tensorflow  1.13
        
        Ubuntu18.04安装配置及使用aria2
        https://www.jianshu.com/p/2f7e087f452b
        
        后台开启aria
        sudo aria2c --conf-path=/etc/aria2/aria2.conf -D
        
 }   
    

/***********************************************************************************************************************************/



































/************************************************************************************************************************************
*5.相关文档
************************************************************************************************************************************/
{
    1.linux内核
    {
        
	        list_for_each_entry 
	        http://blog.csdn.net/xubin341719/article/details/7098611


	        linux内核中的GPIO系统之（1）：软件框架 
	        http://blog.csdn.net/loongembedded/article/details/49466637


	        linux 内核分析之list_head
	        http://www.cnblogs.com/riky/archive/2006/12/28/606242.html


	        Linux 内存的布局 
	        http://blog.csdn.net/maotianwang/article/details/11370849


	        linux  thermal体系框架
	        http://kernel.meizu.com/linux-thermal-framework-intro.html


	        Linux动态频率调节系统CPUFreq 
	        http://blog.chinaunix.net/uid-20321537-id-4049149.html


	        linux下的tty设备
	        http://blog.csdn.net/goodluckwhh/article/details/13368279


	        深度剖析安卓Framebuffer设备驱动 
            https://yq.aliyun.com/articles/10411
	    
	    
	        android 关机/重启 Android关机/重启流程解析
	        http://blog.csdn.net/lxl584685501/article/details/45747967
                	
	    
	        http://blog.csdn.net/honour2sword/article/details/40213417
	        ARM中断向量表与响应流程 
    }









    2.电源相关
    {
            android 4.4 电池电量管理底层分析(C\C++层) 
	        http://blog.csdn.net/daweibalang717/article/details/41446993


	        android 4.4 电池电量显示分析（低电量提醒与电池图标）Java 层 
	        http://blog.csdn.net/daweibalang717/article/details/40615453


	        MT6589充电模块浅析
	        http://blog.csdn.net/wn360459196/article/details/11031073


	        如何让电源为电池充电提供最大功率
	        http://www.21ic.com/app/power/201409/601764.htm


	        Linux的电源管理架构 
	        http://blog.csdn.net/u012719256/article/details/52692395


	        高通8X16电池BMS算法（一） 
	        http://blog.csdn.net/yanleizhouqing/article/details/51592243


	        Linux电源管理相关的一些列文章（涉及面很广）
	        http://www.wowotech.net/sort/pm_subsystem


	        看完变高手 高通Quick Charge 2.0从入门到精通
	        http://www.cnbeta.com/articles/387057.htm

	    
	        高通平台充电(modem部分)
	        http://www.docin.com/p-969643101.html

	        power supply框架
	        http://blog.csdn.net/rikeyone/article/details/52881615

	        sdp cdp dcp及快速充电技术介绍 
	        http://blog.csdn.net/u012719256/article/details/72627022
	    
	        典型AC／DC电源模块
	        http://www.docin.com/p-50062023.html

            无线充电IC
	        http://www.elecfans.com/dianyuan/404455.html
	    
	    
	        高通batterydata电池曲线数据学习
	        https://www.2cto.com/kf/201601/488126.html


	        千万别乱用单位：详解电池的mAh和Wh的区别
	        http://libattery.ofweek.com/2015-06/ART-36001-11002-28964256.html
	    
	    
	        高通Quick Charge高速充电原理分析
	        https://www.cnblogs.com/lxjshuju/p/7252468.html

    }


	

    3.USB
    {
        相关的协议：
	    {
		    BC1.1标准 
		    http://blog.csdn.net/zhaoguowei2010/article/details/8970319


		    USB协 USB协通讯议--深入理解 通讯议--深入理解 
		    http://blog.csdn.net/myarrow/article/details/8484113
	    
	    }

	    基础内容：
	    {
		    USB电池充电基础：应急指南
		    https://www.maximintegrated.com/cn/app-notes/index.mvp/id/4803


		    USB OTG插入检测识别
		    http://blog.csdn.net/uranus_wm/article/details/9838847


		    USB眼图：
		    http://blog.csdn.net/vvcumt/article/details/38852347


		    一个USB驱动
		    http://wenku.baidu.com/link?url=KiEUVHKjGGMkWxGSiJ9eND2QIstcPxj_tmDILEjAT-tMrpTTZQm-nUbZgP_qABC93NSjhPGGDU4-LecGC6xqKRD04Tf_2dUlIQxbemB18hu


		    USB 枚举过程详解 
		    http://blog.csdn.net/zoosenpin/article/details/29795849


		    USB驱动调试
		    http://blog.csdn.net/liuqz2009/article/details/7886461


		    USB 枚举过程详解 
		    http://blog.csdn.net/zoosenpin/article/details/29795849


		    Android框架理解之USB 
		    http://blog.csdn.net/u011279649/article/details/15497991


		    android的USB MTP && USB CDC/USBnet(ECM, NCM, ACM) && USB gardget 
		    http://blog.csdn.net/zhandoushi1982/article/details/7563702
		    
		    
		    type-c:

			    USB&nbsp;Type-C
			    http://blog.csdn.net/fouweng/article/details/53435528

			    USB TYPE-C
			    http://blog.sina.com.cn/s/blog_63f7b4540102wch4.html

			    USB Type C的基本原理
			    http://blog.csdn.net/zoosenpin/article/details/49963031
			    
			    Introduction to Richtek USB Type-C Power Delivery Solutions
			    http://www.richtek.com/en/Design%20Support/Technical%20Document/AN056
			    
		    Linux USB 驱动开发实例（一） —— USB摄像头驱动实现源码分析 
	        http://blog.csdn.net/zqixiao_09/article/details/50984412

	    
	        Linux USB 驱动开发（一）—— USB设备基础概念 
	        http://blog.csdn.net/zqixiao_09/article/details/50984074	
	        
	        android adb 源码框架分析（1 系统）
	        http://blog.csdn.net/luansxx/article/details/25203269
	        
	        android adb源码分析(1) 
	        http://blog.csdn.net/xgbing/article/details/52058390
	        
	        USB2.0协议分析
	        https://blog.csdn.net/weijory/article/details/78272796
	    
	    }
    }









    4.other
    {
            手机主叫的信道流程与Modem Log简单分析
	        http://blog.csdn.net/zx249388847/article/details/52637563


	        GSM modem,GSM modem的组成/原理/分类/作
	        http://www.elecfans.com/baike/bandaoti/bandaotiqijian/20100323203212.html


	        ubuntu完美搭建git服务器：
	        http://blog.csdn.net/tommy_wxie/article/details/38779667


	        高通骁龙410C开发板基础知识
	        http://blog.csdn.net/sungod612/article/details/52698754


	        ubuntu下配置git和github初体验
	        http://blog.chinaunix.net/uid-24782829-id-3183604.html


	        配置vim
	        git clone https://github.com/chxuan/vimplus.git


	        linux下安装google protobuf（详细） 
	        http://blog.csdn.net/xiexievv/article/details/47396725


	        Google Protobuf的安装、配置、以及简单demo编译
	        http://blog.csdn.net/allenlinrui/article/details/10098809


	        Google Protobuf开发指南
	        https://my.oschina.net/macwe/blog/157862


	        高通平台开发一系列文章
	        http://blog.csdn.net/LoongEmbedded/article/category/5611401/1


	        linux公社资料分享
	        http://linux.linuxidc.com/
	        用户名与密码都是www.linuxidc.com


	        使用fastboot命令刷机流程详解
	        http://blog.chinaunix.net/uid-31390529-id-5753369.html


	        **SSH协议介绍 
	        http://blog.csdn.net/macrossdzh/article/details/5691924
	    
	    
	        git rebase命令
	        http://gitbook.liuhui998.com/4_2.html
	    
	    
	        cin、cin.get()、getline()（讲解的很详细）
	        http://blog.csdn.net/xuexiacm/article/details/8101859


	        eMMC分区详解
	        http://blog.sina.com.cn/s/blog_71fdf1f00102v5c2.html


	        Android关机流程解析 
	        http://blog.csdn.net/mirkerson/article/details/24322181
	    
	    
	        深入解析Android关机
	        http://mobile.51cto.com/aprogram-418981.htm


	        Linux中gdb 查看core堆栈信息 
	        http://blog.csdn.net/suxinpingtao51/article/details/12072559


	        Android从启动到程序运行整个过程的整理
	        http://www.cnblogs.com/zyanrong/p/5661114.html


	        linux内核中打印栈回溯信息 - dump_stack()函数分析 
	        http://blog.csdn.net/jasonchen_gbd/article/details/45585133


	        内核符号表的生成和查找过程 
	        http://blog.csdn.net/jasonchen_gbd/article/details/44025681


	        slab内存管理介绍
	        http://blog.chinaunix.net/uid-9185047-id-445227.html


	        常见手机定位方式浅谈
	        http://www.cnblogs.com/syfwhu/p/5084115.html


	        cdmaLocation与gsmlocation的区别 lbs定位 
	        http://blog.csdn.net/chengyingzhilian/article/details/7310977


	        深入MTK平台bootloader启动之【 Pre-loader -> Lk】分析笔记 
	        http://blog.csdn.net/forever_2015/article/details/53000643


	        AGPS定位基本原理浅析
	        http://www.cnblogs.com/magicboy110/archive/2010/12/12/1903927.html


	        显示LOGD打印信息的方法 
	        http://blog.csdn.net/batoom/article/details/6259468


	        android6.0按键处理浅析 
	        http://blog.csdn.net/liyanfei123456/article/details/53196693
	        kpd_aee_handler


	        HALL开关介绍 
	        http://blog.csdn.net/wlwl0071986/article/details/9154541


	        linux驱动学习2（kpd驱动初步分析） 
	        http://blog.csdn.net/jxnu_xiaobing/article/details/16805623


	        线性谐振传动器（LRA）如何工作
	        http://www.elecfans.com/dianzichangshi/20161114448503.html


	        Hang Detect 问题分析案例 
	        http://blog.csdn.net/ldinvicible/article/details/51042881


	        深入理解：Android 编译系统 
	        http://blog.csdn.net/huangyabin001/article/details/36383031

	    
	        linux 内核与用户空间通信之netlink使用方法 
	        http://blog.csdn.net/haomcu/article/details/7371835


	        ARM交叉编译工具链
	        http://www.veryarm.com/cross-tools

	    
	        Android Radio Interface Layer
	        http://www.52rd.com/Blog/Detail_RD.Blog_Lexucs_66161.html


	        Android基础系列-----------Android系统启动过程简析 
	        http://blog.csdn.net/zhoupenglei/article/details/46351701?ref=myread


	        Android OS MTK6573 SD卡驱动浅析 
	        http://blog.csdn.net/dwyane_zhang/article/details/7586570


	        ipi机制 
	        http://blog.csdn.net/cybertan/article/details/7206209


	        从0开始学习 GitHub 系列之「08.如何发现优秀的开源项目」 
	        http://blog.csdn.net/googdev/article/details/52060211


	        Android中解析XML
	        http://blog.csdn.net/liuhe688/article/details/6415593/


	        shell具体执行过程及自主实现shell解释器 
	        http://blog.csdn.net/woaimeinuo/article/details/51118882


	        fastboot命令汇总 
	        http://blog.csdn.net/tdstds/article/details/25179541


	        Android Bootloader分析
	        http://www.tuicool.com/articles/M7v6nuN


	        TCP 协议简介
	        http://www.admin10000.com/document/13621.html


	        Linux内核之旅——（一）中断
	        http://lib.csdn.net/article/operatingsystem/32054


	        【Android休眠】之Android休眠机制 
	        http://blog.csdn.net/u013686019/article/details/53645646


	        Android休眠唤醒机制简介（一）
	        http://blog.sina.com.cn/s/blog_a6559d920101aug9.html


	        Android 的 init.rc 文件简介 
	        http://blog.csdn.net/yimiyangguang1314/article/details/6268177


	        Android的init过程详解（一） 
	        http://blog.csdn.net/nokiaguy/article/details/8800962


	        android6.0源码目录简单分析 
	        http://blog.csdn.net/zhang_danf/article/details/52788336


	        高通msm8994启动流程简介
	        http://blog.csdn.net/finewind/article/details/46469645


	        android socket编程 
	        http://blog.csdn.net/xl19862005/article/details/8517921


	        Android热插拔事件处理详解 
	        https://yq.aliyun.com/articles/63629
	    

	        Linux USB 驱动开发实例（一） —— USB摄像头驱动实现源码分析 
	        http://blog.csdn.net/zqixiao_09/article/details/50984412


	        github android open source project :https://github.com/aosp-mirror

	        Linux SD卡驱动开发(一) —— SD 相关基础概念 
	        http://blog.csdn.net/zqixiao_09/article/details/51039378



	        Android O 前期预研之一：Android Treble 计划
	        http://blog.csdn.net/ljp1205/article/details/77684550	


	        android CTS 介绍
	        https://www.cnblogs.com/verawang/p/5421961.html

            Linux三剑客之awk命令
            https://www.cnblogs.com/ginvip/p/6352157.html
            
            一篇文章了解相见恨晚的 Android Binder 进程间通讯机制
            http://blog.csdn.net/freekiteyu/article/details/70082302
            
            C语言终极面试宝典
	        http://blog.csdn.net/ffmxnjm/article/details/51519976

	        4412 adc
	        {
		        基于ARM4412的ADC原理及实现
		        https://blog.csdn.net/jianwen_hi/article/details/53290461

		        Exynos4412 ADC 设备驱动开发
		        https://blog.csdn.net/zqixiao_09/article/details/50916781
	        } 
	    
	        kernel  启动流程分析
	        https://blog.csdn.net/ooonebook/article/details/52690132
    }




    人工智能
    {

	        TensorFlow 如何入门？
	        https://www.zhihu.com/question/49909565
	    
	        tensorflow中文社区
	        http://www.tensorfly.cn/index.html
    }


}



/********************************************************************************************************************************/
    


















