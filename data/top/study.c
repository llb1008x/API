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
       


}





/*4412开发板*/
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


    烧写镜像文件不能用fastboot，需要通过串口进入uboot模式
    需要注意的是,fastboot 命令只能用来和开发板的 Uboot 模式进行交互,前面烧写镜像时提到的那些
命令也都是运行在开发板的 UBOOT 模式。


    更换make编译器
    但是报错了，

    1.Makefile:1039: recipe for target 'check-regression' failed
    make[2]: *** [check-regression] Error 1
    make[2]: Leaving directory '/home/llb/project/WORK/linux_tar/make-3.81'
    Makefile:880: recipe for target 'check-am' failed
    make[1]: *** [check-am] Error 2
    make[1]: Leaving directory '/home/llb/project/WORK/linux_tar/make-3.81'
    Makefile:603: recipe for target 'check-recursive' failed
    make: *** [check-recursive] Error 1

    ./configure

    make

    make install


    2.Error:
    frameworks/base/include/utils/KeyedVector.h:193:31: note: declarations in dependent base ‘android::KeyedVector<android::String8, android::sp<AaptDir> >’ are not found by unqualified lookup
    frameworks/base/include/utils/KeyedVector.h:193:31: note: use ‘this->indexOfKey’ instead
    make: *** [out/host/linux-x86/obj/EXECUTABLES/aapt_intermediates/AaptAssets.o] Error 1

    Fix:
    vi frameworks/base/tools/aapt/Android.mk

    Add '-fpermissive' to line 31:
    LOCAL_CFLAGS += -Wno-format-y2k -fpermissive


    3.external/srec/tools/thirdparty/OpenFst/fst/lib/determinize.h:297:12: error: ‘SetArcs’ was not declared in this scope, and no declarations were found by argument-dependent lookup at the point of instantiation [-fpermissive]
    解决办法：
    cd external/srec
    wget "https://github.com/CyanogenMod/android_external_srec/commit/4d7ae7b79eda47e489669fbbe1f91ec501d42fb2.diff"
    patch -p1 < 4d7ae7b79eda47e489669fbbe1f91ec501d42fb2.diff
    rm -f 4d7ae7b79eda47e489669fbbe1f91ec501d42fb2.diff
    cd ../..


    4.<command-line>:0:0: error: "_FORTIFY_SOURCE" redefined [-Werror

    解决方法：

    修改源码目录下/build/core/combo/HOST_linux-x86.mk文件：

    将以下语句

    HOST_GLOBAL_CFLAGS += -D_FORTIFY_SOURCE=0

    修改为

    HOST_GLOBAL_CFLAGS += -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=0




    5.external/oprofile/libpp/format_output.h:94:22: error: reference ‘counts’ cannot be declared ‘mutable’ [-fpermissive]
   mutable counts_t & counts;

   解决办法：
    external/oprofile/libpp/format_output.h

    把：
    mutable counts_t & counts;
    改为：
    counts_t & counts;


    6.external/gtest/src/../include/gtest/internal/gtest-param-util.h:122:11: error: ‘ptrdiff_t’ does not name a type
In file included from external/gtest/src/gtest-all.cc:38:0:

    解决方法：
    $vi external/gtest/src/../include/gtest/internal/gtest-param-util.h
    #include <cstddef>


    7.host SharedLib: libext2_profile_host (out/host/linux-x86/obj/lib/libext2_profile.so)
host Executable: ext2simg (out/host/linux-x86/obj/EXECUTABLES/ext2simg_intermediates/ext2simg)
host Executable: ext4fixup (out/host/linux-x86/obj/EXECUTABLES/ext4fixup_intermediates/ext4fixup)
out/host/linux-x86/obj/SHARED_LIBRARIES/libext2_e2p_host_intermediates/feature.o：在函数‘ext2fs_mark_block_bitmap’中：
/home/llb/project/PRO/exynos4412/android_4.0/iTop4412_ICS_git/external/e2fsprogs/lib/ext2fs/bitops.h:307：对‘ext2fs_mark_generic_bitmap’未定义的引用
out/host/linux-x86/obj/SHARED_LIBRARIES/libext2_e2p_host_intermediates/feature.o：在函数‘ext2fs_unmark_block_bitmap’中：
/home/llb/project/PRO/exynos4412/android_4.0/iTop4412_ICS_git/external/e2fsprogs/lib/ext2fs/bitops.h:307：对‘ext2fs_unmark_generic_bitmap’未定义的引用
out/host/linux-x86/obj/SHARED_LIBRARIES/libext2_e2p_host_intermediates/feature.o：在函数‘ext2fs_test_block_bitmap’中：
/home/llb/project/PRO/exynos4412/android_4.0/iTop4412_ICS_git/external/e2fsprogs/lib/ext2fs/bitops.h:307：对‘ext2fs_test_generic_bitmap’未定义的引用
out/host/linux-x86/obj/SHARED_LIBRARIES/libext2_e2p_host_intermediates/feature.o：在函数‘ext2fs_mark_inode_bitmap’中：
/home/llb/project/PRO/exynos4412/android_4.0/iTop4412_ICS_git/external/e2fsprogs/lib/ext2fs/bitops.h:333：对‘ext2fs_mark_generic_bitmap’未定义的引用
out/host/linux-x86/obj/SHARED_LIBRARIES/libext2_e2p_host_intermediates/feature.o：在函数‘ext2fs_unmark_inode_bitmap’中：
/home/llb/project/PRO/exynos4412/android_4.0/iTop4412_ICS_git/external/e2fsprogs/lib/ext2fs/bitops.h:340：对‘ext2fs_unmark_generic_bitmap’未定义的引用
out/host/linux-x86/obj/SHARED_LIBRARIES/libext2_e2p_host_intermediates/feature.o：在函数‘ext2fs_test_inode_bitmap’中：
/home/llb/project/PRO/exynos4412/android_4.0/iTop4412_ICS_git/external/e2fsprogs/lib/ext2fs/bitops.h:342：对‘ext2fs_test_generic_bitmap’未定义的引用
out/host/linux-x86/obj/SHARED_LIBRARIES/libext2_e2p_host_intermediates/feature.o：在函数‘ext2fs_fast_mark_block_bitmap’中：
/home/llb/project/PRO/exynos4412/android_4.0/iTop4412_ICS_git/external/e2fsprogs/lib/ext2fs/bitops.h:342：对‘ext2fs_mark_generic_bitmap’未定义的引用
out/host/linux-x86/obj/SHARED_LIBRARIES/libext2_e2p_host_intermediates/feature.o：在函数‘ext2fs_fast_unmark_block_bitmap’中：
/home/llb/project/PRO/exynos4412/android_4.0/iTop4412_ICS_git/external/e2fsprogs/lib/ext2fs/bitops.h:342：对‘ext2fs_unmark_generic_bitmap’未定义的引用
out/host/linux-x86/obj/SHARED_LIBRARIES/libext2_e2p_host_intermediates/feature.o：在函数‘ext2fs_fast_test_block_bitmap’中：
/home/llb/project/PRO/exynos4412/android_4.0/iTop4412_ICS_git/external/e2fsprogs/lib/ext2fs/bitops.h:342：对‘ext2fs_test_generic_bitmap’未定义的引用
out/host/linux-x86/obj/SHARED_LIBRARIES/libext2_e2p_host_intermediates/feature.o：在函数‘ext2fs_fast_mark_inode_bitmap’中：
/home/llb/project/PRO/exynos4412/android_4.0/iTop4412_ICS_git/external/e2fsprogs/lib/ext2fs/bitops.h:373：对‘ext2fs_mark_generic_bitmap’未定义的引用
out/host/linux-x86/obj/SHARED_LIBRARIES/libext2_e2p_host_intermediates/feature.o：在函数‘ext2fs_fast_unmark_inode_bitmap’中：
/home/llb/project/PRO/exynos4412/android_4.0/iTop4412_ICS_git/external/e2fsprogs/lib/ext2fs/bitops.h:379：对‘ext2fs_unmark_generic_bitmap’未定义的引用
out/host/linux-x86/obj/SHARED_LIBRARIES/libext2_e2p_host_intermediates/feature.o：在函数‘ext2fs_fast_test_inode_bitmap’中：
/home/llb/project/PRO/exynos4412/android_4.0/iTop4412_ICS_git/external/e2fsprogs/lib/ext2fs/bitops.h:385：对‘ext2fs_test_generic_bitmap’未定义的引用
out/host/linux-x86/obj/SHARED_LIBRARIES/libext2_e2p_host_intermediates/feature.o：在函数‘ext2fs_get_block_bitmap_start’中：
/home/llb/project/PRO/exynos4412/android_4.0/iTop4412_ICS_git/external/e2fsprogs/lib/ext2fs/bitops.h:387：对‘ext2fs_get_generic_bitmap_start’未定义的引用
out/host/linux-x86/obj/SHARED_LIBRARIES/libext2_e2p_host_intermediates/feature.o：在函数‘ext2fs_get_inode_bitmap_start’中：
/home/llb/project/PRO/exynos4412/android_4.0/iTop4412_ICS_git/external/e2fsprogs/lib/ext2fs/bitops.h:396：对‘ext2fs_get_generic_bitmap_start’未定义的引用
out/host/linux-x86/obj/SHARED_LIBRARIES/libext2_e2p_host_intermediates/feature.o：在函数‘ext2fs_get_block_bitmap_end’中：
/home/llb/project/PRO/exynos4412/android_4.0/iTop4412_ICS_git/external/e2fsprogs/lib/ext2fs/bitops.h:397：对‘ext2fs_get_generic_bitmap_end’未定义的引用
out/host/linux-x86/obj/SHARED_LIBRARIES/libext2_e2p_host_intermediates/feature.o：在函数‘ext2fs_get_inode_bitmap_end’中：
/home/llb/project/PRO/exynos4412/android_4.0/iTop4412_ICS_git/external/e2fsprogs/lib/ext2fs/bitops.h:406：对‘ext2fs_get_generic_bitmap_end’未定义的引用
out/host/linux-x86/obj/SHARED_LIBRARIES/libext2_e2p_host_intermediates/feature.o：在函数‘ext2fs_fast_test_block_bitmap_range’中：
/home/llb/project/PRO/exynos4412/android_4.0/iTop4412_ICS_git/external/e2fsprogs/lib/ext2fs/bitops.h:412：对‘ext2fs_test_block_bitmap_range’未定义的引用
out/host/linux-x86/obj/SHARED_LIBRARIES/libext2_e2p_host_intermediates/feature.o：在函数‘ext2fs_fast_mark_block_bitmap_range’中：
/home/llb/project/PRO/exynos4412/android_4.0/iTop4412_ICS_git/external/e2fsprogs/lib/ext2fs/bitops.h:418：对‘ext2fs_mark_block_bitmap_range’未定义的引用
out/host/linux-x86/obj/SHARED_LIBRARIES/libext2_e2p_host_intermediates/feature.o：在函数‘ext2fs_fast_unmark_block_bitmap_range’中：
/home/llb/project/PRO/exynos4412/android_4.0/iTop4412_ICS_git/external/e2fsprogs/lib/ext2fs/bitops.h:424：对‘ext2fs_unmark_block_bitmap_range’未定义的引用

    diff --git a/include/ext2fs/ext2fs.h b/include/ext2fs/ext2fs.h
    index 17481a0..78f517a 100644
    --- a/include/ext2fs/ext2fs.h
    +++ b/include/ext2fs/ext2fs.h
    @@ -221,12 +221,6 @@ struct struct_ext2_filsys {
        struct ext2_inode_cache		*icache;
    };
    
    -#if EXT2_FLAT_INCLUDES
    -#include "e2_bitops.h"
    -#else
    -#include "ext2fs/bitops.h"
    -#endif
    -


    8.host Executable: test-librsloader (out/host/linux-x86/obj/EXECUTABLES/test-librsloader_intermediates/test-librsloader)
out/host/linux-x86/obj/STATIC_LIBRARIES/libLLVMSupport_intermediates/libLLVMSupport.a(Signals.o)：在函数‘PrintStackTrace(void*)’中：
/home/llb/project/PRO/exynos4412/android_4.0/iTop4412_ICS_git/external/llvm/lib/Support/Unix/Signals.inc:219：对‘dladdr’未定义的引用
/home/llb/project/PRO/exynos4412/android_4.0/iTop4412_ICS_git/external/llvm/lib/Support/Unix/Signals.inc:231：对‘dladdr’未定义的引用
out/host/linux-x86/obj/STATIC_LIBRARIES/libLLVMSupport_intermediates/libLLVMSupport.a(Threading.o)：在函数‘llvm::llvm_execute_on_thread(void (*)(void*), void*, unsigned int)’中：
/home/llb/project/PRO/exynos4412/android_4.0/iTop4412_ICS_git/external/llvm/lib/Support/Threading.cpp:96：对‘pthread_create’未定义的引用
/home/llb/project/PRO/exynos4412/android_4.0/iTop4412_ICS_git/external/llvm/lib/Support/Threading.cpp:100：对‘pthread_join’未定义的引用
/home/llb/project/PRO/exynos4412/android_4.0/iTop4412_ICS_git/external/llvm/lib/Support/Threading.cpp:91：对‘pthread_attr_setstacksize’未定义的引用
out/host/linux-x86/obj/STATIC_LIBRARIES/libLLVMSupport_intermediates/libLLVMSupport.a(Mutex.o)：在函数‘llvm::sys::MutexImpl::MutexImpl(bool)’中：
/home/llb/project/PRO/exynos4412/android_4.0/iTop4412_ICS_git/external/llvm/lib/Support/Mutex.cpp:69：对‘pthread_mutexattr_init’未定义的引用
/home/llb/project/PRO/exynos4412/android_4.0/iTop4412_ICS_git/external/llvm/lib/Support/Mutex.cpp:75：对‘pthread_mutexattr_settype’未定义的引用
/home/llb/project/PRO/exynos4412/android_4.0/iTop4412_ICS_git/external/llvm/lib/Support/Mutex.cpp:80：对‘pthread_mutexattr_setpshared’未定义的引用
/home/llb/project/PRO/exynos4412/android_4.0/iTop4412_ICS_git/external/llvm/lib/Support/Mutex.cpp:89：对‘pthread_mutexattr_destroy’未定义的引用
out/host/linux-x86/obj/STATIC_LIBRARIES/libLLVMSupport_intermediates/libLLVMSupport.a(Mutex.o)：在函数‘llvm::sys::MutexImpl::tryacquire()’中：
/home/llb/project/PRO/exynos4412/android_4.0/iTop4412_ICS_git/external/llvm/lib/Support/Mutex.cpp:143：对‘pthread_mutex_trylock’未定义的引用

    $vi external/llvm/llvm-host-build.mk +
    LOCAL_LDLIBS := -lpthread -ldl


    9.external/llvm/include/llvm/ADT/PointerUnion.h:56:10: error: enumeral mismatch in conditional expression: ‘llvm::PointerLikeTypeTraits<llvm::PointerUnion<clang::Stmt*, const clang::Type*> >::<anonymous enum>’ vs ‘llvm::PointerLikeTypeTraits<clang::ObjCInterfaceDecl*>::<anonymous enum>’ [-Werror]
cc1plus: all warnings being treated as errors
make: *** [out/host/linux-x86/obj/EXECUTABLES/llvm-rs-cc_intermediates/slang_rs.o] Error 1

    Fix:
    vi frameworks/compile/slang/Android.mk

    Remove '-Werror' from line 22:
    local_cflags_for_slang := -Wno-sign-promo -Wall -Wno-unused-parameter 


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


    }


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



