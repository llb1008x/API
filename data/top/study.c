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



/*python 课程*/
{
    科学计算可视化
    {
      VTK: Visualization Toolkit  
      #创建一个三维对象
      s=tvtk.CubeSource(traits) 

      #显示数据源
        {
            #显示一个三维对象
            from tvtk.api import tvtk

            #创建一个长方体数据源
            s=tvtk.CubeSource(x_length=1.0,y_length=2.0,z_length=3.0)
            #使用polydatamapper将数据转换为图形数据
            m=tvtk.PolyDataMapper(input_connection=s.output_port)
            #创建一个Actor
            a=tvtk.Actor(mapper=m)
            #创建一个renderer,将actor添加进去
            r=tvtk.Renderer(background=(0,0,0))
            r.add_actor(a)
            #创建一个renderWindow(窗口),将renderer添加进去
            w=tvtk.RenderWindow(size=(3000,3000))
            w.add_renderer(r)
            #创建一个renderWindowInteractor,窗口交互式工具
            i=tvtk.RenderWindowInteractor(render_window=w)
            #开启交互
            i.initialize()
            i.start()        

            多个对象协调完成工作的过程，称为管线

            数据 -> 数据预处理 -> 数据映射 -> 绘制 -> 显示

            ivtk就是可以动态改变三维对象的可视化窗口工具
        }
    }


}







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

        遇到一个问题站在设计者的角度还是使用者的角度，转换思考的角度有些问题就可以想清楚点
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


}












