
linux设备驱动

一.设备驱动程序简介 
首先，我们知道驱动是内核的一部分，那么驱动在内核中到底扮演了什么角色呢？

设备驱动程序在内核中的角色：他们是一个个独立的“黑盒子”，使某个特定的硬件响应一个定义良好的内部编程接口，这些接口完全隐藏了设备的工作细节。
（说白了，驱动程序除了对外提供特定的接口外，任何实现细节对应用程序都是不可见的。）用户的操作通过一组标准化的调用执行，而这些调用独立于特定的驱动程序。
驱动程序的任务是把这些标准化调用映射到实际硬件的设备特有操作上。

在编写驱动程序时，程序员应该特别注意下面这个概念：编写访问硬件的内核代码时，不要给用户强加任何特定策略。这句话我的理解是，驱动只负责实现最基本的硬件功能，
而谁使用此硬件，怎么使用，一般都不做考虑。

不带策略的驱动程序包括一些典型的特征：同时支持同步和异步操作、驱动程序能够被多次打开、充分利用硬件特性，以及不具备用来“简化任务”的或提供与策略相关的软件层等。

驱动程序就是应用程序与实际硬件之间的一个软件层，相同的硬件，不同的驱动程序可能提供不同的功能。实际的驱动程序设计要在许多要考虑的因素之间做出平衡。总的来说，
驱动程序设计主要还是综合考虑下面三个方面的因素：提供给用户尽量多的选项、编写驱动程序要占用的时间以及尽量保持程序简单而不至于错误丛生。

了解了驱动在内核中扮演的角色，我们还可以简单了解下内核中包含的其他模块。一般把内核功能分成如下几部分：进程管理、内存管理、文件系统、设备控制、网络功能。
操作系统原理上面把操作系统的原理分为：处理机管理、进程管理、文件管理、存储管理、设备管理、网络与通信管理、用户接口，其中处理机管理归根到底其实就是进程管理。
因为处理器的分配和执行都是以进程为基本单位的。而存储管理就是说的内存管理。

进一步来看，电脑上有网卡，显卡，声卡等，还可以外接U盘，打印机等等外设，那么这么多的设备有没有分类呢？很明显，根据设备的接口，我们可以知道分为usb设备，串口设备，
pci设备，spi设备，i2c设备等等，那么在Linux内核中又有样的划分呢？下面所述就是linux中对所有设备的一个分类，并描述了相互之间的简单区别。

linux中设备和模块的分类：

字符设备：字符设备是能够像字节流(类似文件)一样被访问的设备，有字符设备驱动程序来实现这种特性。字符设备驱动程序通常至少要实现open、close、read、write系统调用。
字符设备可以通过文件系统节点来访问，这些设备文件和普通文件之间的唯一差别在于对普通文件的访问可以前后移动访问位置，而大多数字符设备是一个只能顺序访问的数据通道。
一个字符设备是一种字节流设备，对设备的存取只能按顺序按字节的存取而不能随机访问，字符设备没有请求缓冲区，所有的访问请求都是按顺序执行的。但事实上现在一些高级字符
设备也可以从指定位置一次读取一块数据。

块设备：块设备也是通过设备节点来访问。块设备上能够容纳文件系统。在大多数unix系统中，进行I/O操作时块设备每次只能传输一个或多个完整的块，而每块包含512字节
（或更2的更高次幂字节的数据）。linux可以让应用程序向字符设备一样读写块设备，允许一次传递任意多字节的数据。因而，块设备和字符设备的区别仅仅在于内核内部管理数据的方式，
也就是内核及驱动程序之间的软件接口，而这些不同对用户来讲是透明的。在内核中，和字符驱动程序相比，块驱动程序具有完全不同的接口。存储设备一 般属于块设备，
块设备有请求缓冲区，并且支持随机访问而不必按照顺序去存取数据，比如你可以 先存取后面的数据，然后在存取前面的数据，这对字符设备来说是不可能的。Linux下的磁盘设备都
是块设备，尽管在Linux下有块设备节点，但应用程序一般是通过文件系统及其高速缓存来访问块设备的,而不是直接通过设备节点来读写块设备上的数据。

网络设备：网络设备不同于字符设备和块设备，它是面向报文的而不是面向流的，它不支持随机访问，也没有请求缓冲区。由于不是面向流的设备，因此将网络接口映射到文件系统中的
节点比较困难。内核和网络设备驱动程序间的通讯，完全不同于内核和字符以及块驱动程序之间的通讯，内核调用一套和数据包传输相关的函数而不是read，write。网络接口没有像
字符设备和块设备一样的设备号，只有一个唯一的名字，如eth0、eth1等，而这个名字也不需要与设备文件节点对应。

由上大致总结下字符设备与块设备的区别：1、字符设备是面向流的，最小访问单位是字节；而块设备是面向块的，最小访问单位是512字节或2的更高次幂。2、字符设备只能顺序按字
节访问，而块设备可随机访问。3、块设备上可容纳文件系统，访问形式上，字符设备通过设备节点访问，而块设备虽然也可通过设备节点访问，但一般是通过文件系统来访问数据的。

而网络设备没有设备节点是因为，网络设备是面向报文的，很难实现相关read、write等文件读写函数。所以驱动的实现也与字符设备和块设备不同。





二.构造和运行模块 


上一篇介绍了linux驱动的概念，以及linux下设备驱动的基本分类情况及其各个分类的依据和差异，这一篇我们来描述如何写一个类似hello world的简单测试驱动程序。
而这个驱动的唯一功能就是输出hello world。

在编写具体的实例之前，我们先来了解下linux内核下调试程序的一个重要函数printk以及几个重要概念。

printk类似c语言的printf，是内核中输出打印信息的函数。以后驱动调试中的重要性不言而喻，下面先做一个简单介绍。

printk的级别

日志级别一共有8个级别，printk的日志级别定义如下（在include/linux/kernel.h中）：　　
#define KERN_EMERG 0/*紧急事件消息，系统崩溃之前提示，表示系统不可用*/　　
#define KERN_ALERT 1/*报告消息，表示必须立即采取措施*/　　
#define KERN_CRIT 2/*临界条件，通常涉及严重的硬件或软件操作失败*/　　
#define KERN_ERR 3/*错误条件，驱动程序常用KERN_ERR来报告硬件的错误*/　　
#define KERN_WARNING 4/*警告条件，对可能出现问题的情况进行警告*/　　
#define KERN_NOTICE 5/*正常但又重要的条件，用于提醒*/　　
#define KERN_INFO 6/*提示信息，如驱动程序启动时，打印硬件信息*/　　
#define KERN_DEBUG 7/*调试级别的消息*/

没有指定日志级别的printk语句默认采用的级别是：DEFAULT_ MESSAGE_LOGLEVEL（这个默认级别一般为<4>,即与KERN_WARNING在一个级别上），其定义在kernel/printk.c
中可以找到。在驱动调试过程中打开所有日志信息可使用echo 7 > /proc/sys/kernel/printk，相对应关闭日志使用echo 0 > /proc/sys/kernel/printk。

下面再来介绍几个重要的概念，这些概念可以先做一个了解，后续的文章中还会提到。

内核空间和用户空间

linux系统分为两个级别。内核运行在最高级别，可以进行所有的操作。而应用程序运行在最低级别，处理器控制着对硬件的直接访问以及对内存的非授权访问。内核空间和用户空间
不仅有不同的优先级等级，而且有不同的内存映射，有各自的地址空间。详见内存管理。

应用程序只能通过系统调用或中断从用户空间切换到内核空间，其中系统调用是软中断（0x80号中断）。执行系统调用的系统代码运行在进程上下文中，它代表调用进程执行操作，
因此能够访问进程地址空间的所有数据。而处理硬件中断的内核代码和进程是异步的，与任何一个特定进程无关。

内核中的并发

内核编程区别于常见应用程序编程的地方在于对并发的处理。大部分应用程序除多线程外，通常是顺序执行的，不需要关心由于其他事情的发生而改变它的运行环境。内核代码不是这样，
同一时刻，可能有多个进程使用访问同一个模块。

内核编程要考虑并发问题的原因：1.linux是通常正在运行多个并发进程，并且可能有多个进程同时使用我们的驱动程序。2.大多数设备能够中断处理器，而中断处理程序异步进行，
而且可能在驱动程序正试图处理其它任务时被调用。3.一些类似内核定时器的代码在异步运行。4.运行在对称多处理器上（SMP），不止一个cpu在运行驱动程序。5.内核代码是可抢占的。

当前进程

内核代码可通过访问全局项current来获得当前进程。current指针指向当前正在运行的进程。在open、read、等系统调用的执行过程中，当前进程指的是调用这些系统调用的进程。
内核代码可以通过current指针获得与当前进程相关的信息。

内核中带“__”的函数：内核API函数具有这种名称的，通常都是一些接口的底层函数，应该谨慎使用。实质上，这里的双下划线就是要告诉程序员：谨慎调用，否则后果自负。
以__init为例，__init表明该函数仅在初始化期间使用。在模块被装载之后，模块装载器就会将初始化函数扔掉，这样可以将函数占用的内存释放出来，已做它用。注意，不要在结束初
始化之后仍要使用的函数（或者数据结构）上使用__init、__initdata标记。这里摘抄网上的一段总结，如下。

__init， __initdata等属性标志，是要把这种属性的代码放入目标文件的.init.text节，数据放入.init.data节──这一过程是通过编译内核时为相关目标平台提供了xxx.lds
链接脚本来指导ld完成的。
   对编译成module的代码和数据来说，当模块加载时，__init属性的函数就被执行;
   对静态编入内核的代码和数据来说，当内核引导时，do_basic_setup()函数调用do_initcalls()函数，后者负责所有.init节函数的执行。
   在初始化完成后，用这些关键字标识的函数或数据所占的内存会被释放掉。
1） 所有标识为__init的函数在链接的时候都放在.init.text这个区段内，在这个区段中，函数的摆放顺序是和链接的顺序有关的，是不确定的。 
2） 所有的__init函数在区段.initcall.init中还保存了一份函数指针，在初始化时内核会通过这些函数指针调用这些__init函数指针，并在整个初始化完成后，释放整个init区段
（包括.init.text，.initcall.init等），注意，这些函数在内核初始化过程中的调用顺序只和这里的函数指针的顺序有关，和1）中所述的这些函数本身在.init.text区段中的顺
序无关。 

下面我们来看一个驱动程序的hello world程序是如何实现的：
{
    #include <linux/init.h>
    #include <linux/module.h>
    MODULE_LICENSE("Dual BSD/GPL");

    static int hello_init(void)
    {
            printk(KERN_ALERT "Hello, world\n");
            return 0;
    }
    static void hello_exit(void)
    {

            printk(KERN_ALERT "Goodbye, cruel world\n");
    }

    module_init(hello_init);
    module_exit(hello_exit);

}

内核模块的编译与应用程序的编译有些区别，此hello world模块的编译命令为：

make -C /xxx/xxx/kernel_src/ M=$(PWD) modules

其中/xxx/xxx/kernel_src/ 为已经配置编译过的内核源码路径，ubuntu下一般在/lib/modules/$(shell uname -r)/build目录下。

此函数只有两个函数，一个是hello_init，在insmod的时候执行，这个是模块的初始化函数，另一个是hello_exit，在rmmod的时候执行，是模块卸载时要执行的函数。此模块的
唯一功能就是在insmod的时候输出Hello，world，在rmmod的时候输出Goodbye，cruel world。
在编写应用程序时，我们一般都是由多个源文件组成的，这个时候编译肯定就不能继续使用命令行编译了，就要使用到Makefile。同样，驱动模块的编译也需要使用的makefile，
下面就是一个在编译含有多个源码文件的驱动模块时可以参考的Makefile文件。

Makefile模板
{
    ifndef CROSS_COMPILE
    export CROSS_COMPILE ?=arm-none-linux-gnueabi-
    endif

    ARCH ?= arm

    SRC_DIR := /home/XXX/XXX
    OBJ_DIR  := $(SRC_DIR)/obj
    PWD := $(shell pwd)

    LINUX_SRC ?= /home/XXX/kernel

    CFG_INC = -I$(SRC_DIR) \
        -I$(DIR_A) \
        -I$(DIR_B)

    CFG_FLAGS += -O2
    EXTRA_CFLAGS  += $(C_FLAGS) $(CFG_INC) $(CFG_INC)

    obj-m := mymodule.o

    mymodule-objs := a.o
    mymodule-objs += b.o
    mymodule-objs += c.o

    modules:
        @make ARCH=$(ARCH) -C $(LINUX_SRC) M=$(PWD) modules

    clean:
        @echo "cleaning..."
        rm -f mymodule.ko mymodule.o mymodule.mod.* modules.order Module.symvers
        rm -f $(mymodule-objs)

}

以上就是这一篇的内容，下一篇会从简单的字符驱动开始，介绍驱动编写的主要内容。




三.写一个简单的字符设备驱动 

在linux设备驱动第一篇：设备驱动程序简介中简单介绍了字符驱动，本篇简单介绍如何写一个简单的字符设备驱动。本篇借鉴LDD中的源码，实现一个与硬件设备无关的字符设备驱动，
仅仅操作从内核中分配的一些内存。

下面就开始学习如何写一个简单的字符设备驱动。首先我们来分解一下字符设备驱动都有那些结构或者方法组成，也就是说实现一个可以使用的字符设备驱动我们必须做些什么工作。
1、主设备号和次设备号
对于字符设备的访问是通过文件系统中的设备名称进行的。他们通常位于/dev目录下。如下：

    xxx@ubuntu:~$ ls -l /dev/  
    total 0  
    brw-rw----  1 root disk        7,   0  3月 25 10:34 loop0  
    brw-rw----  1 root disk        7,   1  3月 25 10:34 loop1  
    brw-rw----  1 root disk        7,   2  3月 25 10:34 loop2  
    crw-rw-rw-  1 root tty         5,   0  3月 25 12:48 tty  
    crw--w----  1 root tty         4,   0  3月 25 10:34 tty0  
    crw-rw----  1 root tty         4,   1  3月 25 10:34 tty1  
    crw--w----  1 root tty         4,  10  3月 25 10:34 tty10  

其中b代表块设备，c代表字符设备。对于普通文件来说，ls -l会列出文件的长度，而对于设备文件来说，上面的7,5,4等代表的是对应设备的主设备号，而后面的0,1,2,10等则
是对应设备的次设备号。那么主设备号和次设备号分别代表什么意义呢？一般情况下，可以这样理解，主设备号标识设备对应的驱动程序，也就是说1个主设备号对应一个驱动程序。
当然，现在也有多个驱动程序共享主设备号的情况。而次设备号有内核使用，用于确定/dev下的设备文件对应的具体设备。举一个例子，虚拟控制台和串口终端有驱动程序4管理，
而不同的终端分别有不同的次设备号。（主设备号对应一类设备的驱动程序，而从设备号应该是具体设备的驱动）
1.1、设备编号的表达
在内核中，dev_t用来保存设备编号，包括主设备号和次设备号。在2.6的内核版本种，dev_t是一个32位的数，其中12位用来表示主设备号，其余20位用来标识次设备号。
通过dev_t获取主设备号和次设备号使用下面的宏：
MAJOR(dev_t dev);
MINOR(dev_t dev);
相反，通过主设备号和次设备号转换为dev_t类型使用：
MKDEV(int major, int minor);
1.2、分配和释放设备编号
在构建一个字符设备之前，驱动程序首先要获得一个或者多个设备编号，这类似一个营业执照，有了营业执照才在内核中正常工作营业。完成此工作的函数是：

    int register_chrdev_region(dev_t first, unsigned int count, const char *name);  

first是要分配的设备编号范围的起始值。count是连续设备的编号的个数。name是和该设备编号范围关联的设备名称，他将出现在/proc/devices和sysfs中。此函数成功返回0，
失败返回负的错误码。此函数是在已知主设备号的情况下使用，在未知主设备号的情况下，我们使用下面的函数：

    int alloc_chrdev_region(dev_t *dev, unsigned int firstminor, unsigned int count, const char *name);  

dev用于输出申请到的设备编号，firstminor要使用的第一个此设备编号。
在不使用时需要释放这些设备编号，已提供其他设备程序使用：


    void unregister_chrdev_region(dev_t dev, unsigned int count);  

此函数多在模块的清除函数中调用。
分配到设备编号之后，我们只是拿到了营业执照，虽说现在已经准备的差不多了,但是我们只是从内核中申请到了设备号,应用程序还是不能对此设备作任何事情,我们需要一个简单的
函数来把设备编号和此设备能实现的功能连接起来,这样我们的模块才能提供具体的功能.这个操作很简单，稍后就会提到，在此之前先介绍几个重要的数据结构。

2、重要的数据结构
注册设备编号仅仅是完成一个字符设备驱动的第一步。下面介绍大部分驱动都会包含的三个重要的内核的数据结构。
2.1、文件操作file_operations
file_operations是第一个重要的结构，定义在 <linux/fs.h>, 是一个函数指针的集合，设备所能提供的功能大部分都由此结构提供。这些操作也是设备相关的系统调用的具体实现。
此结构的具体实现如下所示：

    struct file_operations {  
            //它是一个指向拥有这个结构的模块的指针. 这个成员用来在它的操作还在被使用时阻止模块被卸载. 几乎所有时间中, 它被简单初始化为 THIS_MODULE 
            //这个定义了一个驱动所能进行的操作 
            struct module *owner;  
            loff_t (*llseek) (struct file *, loff_t, int);  
            ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);  
            ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);  
            ssize_t (*aio_read) (struct kiocb *, const struct iovec *, unsigned long, loff_t);  
            ssize_t (*aio_write) (struct kiocb *, const struct iovec *, unsigned long, loff_t);  
            ssize_t (*read_iter) (struct kiocb *, struct iov_iter *);  
            ssize_t (*write_iter) (struct kiocb *, struct iov_iter *);  
            int (*iterate) (struct file *, struct dir_context *);  
            unsigned int (*poll) (struct file *, struct poll_table_struct *);  
            long (*unlocked_ioctl) (struct file *, unsigned int, unsigned long);  
            long (*compat_ioctl) (struct file *, unsigned int, unsigned long);  
            int (*mmap) (struct file *, struct vm_area_struct *);  
            int (*open) (struct inode *, struct file *);  
            int (*flush) (struct file *, fl_owner_t id);  
            int (*release) (struct inode *, struct file *);  
            int (*fsync) (struct file *, loff_t, loff_t, int datasync);  
            int (*aio_fsync) (struct kiocb *, int datasync);  
            int (*fasync) (int, struct file *, int);  
            int (*lock) (struct file *, int, struct file_lock *);  
            ssize_t (*sendpage) (struct file *, struct page *, int, size_t, loff_t *, int);  
            unsigned long (*get_unmapped_area)(struct file *, unsigned long, unsigned long, unsigned long, unsigned long);  
            int (*check_flags)(int);  
            int (*flock) (struct file *, int, struct file_lock *);  
            ssize_t (*splice_write)(struct pipe_inode_info *, struct file *, loff_t *, size_t, unsigned int);  
            ssize_t (*splice_read)(struct file *, loff_t *, struct pipe_inode_info *, size_t, unsigned int);  
            int (*setlease)(struct file *, long, struct file_lock **);  
            long (*fallocate)(struct file *file, int mode, loff_t offset,  
                              loff_t len);  
            int (*show_fdinfo)(struct seq_file *m, struct file *f);  
    };  

需要说明的是这里面的函数在驱动中不用全部实现，不支持的操作留置为NULL。
2.2、文件结构struct file
struct file, 定义于 <linux/fs.h>, 是设备驱动中第二个最重要的数据结构。文件结构代表一个打开的文件. (它不特定给设备驱动; 系统中每个打开的文件有一个关联的
struct file 在内核空间). 它由内核在 open 时创建, 并传递给在文件上操作的任何函数, 直到最后的关闭. 在文件的所有实例都关闭后, 内核释放这个数据结构。
 file结构的详细可参考fs.h，这里列出来几个重要的成员。

    struct file_operations *f_op：就是上面刚刚介绍的文件操作的集合结构。

    mode_t f_mode：文件模式确定文件是可读的或者是可写的(或者都是), 通过位 FMODE_READ 和 FMODE_WRITE. 你可能想在你的 open 或者 ioctl 
    函数中检查这个成员的读写许可, 但是你不需要检查读写许可, 因为内核在调用你的方法之前检查. 当文件还没有为那种存取而打开时读或写的企图被拒绝, 驱动甚至不知道这个情况
    
    loff_t f_pos：当前读写位置. loff_t 在所有平台都是 64 位。驱动可以读这个值, 如果它需要知道文件中的当前位置, 但是正常地不应该改变它。

    unsigned int f_flags：这些是文件标志, 例如 O_RDONLY, O_NONBLOCK, 和 O_SYNC. 驱动应当检查 O_NONBLOCK 标志来看是否是请求非阻塞操作。

    void *private_data：open 系统调用设置这个指针为 NULL, 在为驱动调用 open 方法之前. 你可自由使用这个成员或者忽略它; 你可以使用这个成员来指向分配的数据, 
    但是接着你必须记住在内核销毁文件结构之前, 在 release 方法中释放那个内存. private_data 是一个有用的资源, 在系统调用间保留状态信息, 我们大部分例子模块都使用它

2.3、inode 结构

inode 结构由内核在内部用来表示文件. 因此, 它和代表打开文件描述符的文件结构是不同的. 可能有代表单个文件的多个打开描述符的许多文件结构, 但是它们都指向一个单个
inode 结构。

inode 结构包含大量关于文件的信息。但对于驱动程序编写来说一般不用关心，暂且不说。
3、字符设备的注册
内核在内部使用类型 struct cdev 的结构来代表字符设备. 在内核调用你的设备操作前, 你编写分配并注册一个或几个这些结构。

有 2 种方法来分配和初始化一个这些结构. 如果你想在运行时获得一个独立的 cdev 结构, 你可以为此使用这样的代码:

    struct cdev *my_cdev = cdev_alloc();  
    my_cdev->ops = &my_fops;  

更多的情况是把cdv结构嵌入到你自己封装的设备结构中，这时需要使用下面的方法来分配和初始化：

    void cdev_init(struct cdev *cdev, struct file_operations *fops);  

后面的例子程序就是这么做的。一旦 cdev 结构建立, 最后的步骤是把它告诉内核：


    int cdev_add(struct cdev *dev, dev_t num, unsigned int count)  

这里, dev 是 cdev 结构, num 是这个设备响应的第一个设备号, count 是应当关联到设备的设备号的数目. 常常 count 是 1。

从系统去除一个字符设备, 调用:

    void cdev_del(struct cdev *dev);  

4、一个简单的字符设备
上面大致介绍了实现一个字符设备所要做的工作，下面就来一个真实的例子来总结上面介绍的内容。源码中的关键地方已经作了注释。

{
    #include <linux/module.h>  
    #include <linux/types.h>  
    #include <linux/fs.h>  
    #include <linux/errno.h>  
    #include <linux/mm.h>  
    #include <linux/sched.h>  
    #include <linux/init.h>  
    #include <linux/cdev.h>  
    #include <asm/io.h>  
    #include <asm/uaccess.h>  
    #include <linux/timer.h>  
    #include <asm/atomic.h>  
    #include <linux/slab.h>  
    #include <linux/device.h>  
      
    #define CDEVDEMO_MAJOR 255  /*预设cdevdemo的主设备号*/  
      
    static int cdevdemo_major = CDEVDEMO_MAJOR;  
      
    /*设备结构体,此结构体可以封装设备相关的一些信息等 
      信号量等也可以封装在此结构中，后续的设备模块一般都 
      应该封装一个这样的结构体，但此结构体中必须包含某些 
      成员，对于字符设备来说，我们必须包含struct cdev cdev*/  
    struct cdevdemo_dev   
    {  
        struct cdev cdev;  
    };  
      
    struct cdevdemo_dev *cdevdemo_devp; /*设备结构体指针*/  
      
    /*文件打开函数，上层对此设备调用open时会执行*/  
    int cdevdemo_open(struct inode *inode, struct file *filp)     
    {  
        printk(KERN_NOTICE "======== cdevdemo_open ");  
        return 0;  
    }  
      
    /*文件释放，上层对此设备调用close时会执行*/  
    int cdevdemo_release(struct inode *inode, struct file *filp)      
    {  
        printk(KERN_NOTICE "======== cdevdemo_release ");     
        return 0;  
    }  
      
    /*文件的读操作，上层对此设备调用read时会执行*/  
    static ssize_t cdevdemo_read(struct file *filp, char __user *buf, size_t count, loff_t *ppos)  
    {  
        printk(KERN_NOTICE "======== cdevdemo_read ");    
    }  
      
    /* 文件操作结构体，文中已经讲过这个结构*/  
    static const struct file_operations cdevdemo_fops =  
    {  
        .owner = THIS_MODULE,  
        .open = cdevdemo_open,  
        .release = cdevdemo_release,  
        .read = cdevdemo_read,  
    };  
      
    /*初始化并注册cdev*/  
    static void cdevdemo_setup_cdev(struct cdevdemo_dev *dev, int index)  
    {  
        printk(KERN_NOTICE "======== cdevdemo_setup_cdev 1");     
        int err, devno = MKDEV(cdevdemo_major, index);  
        printk(KERN_NOTICE "======== cdevdemo_setup_cdev 2");  
      
        /*初始化一个字符设备，设备所支持的操作在cdevdemo_fops中*/     
        cdev_init(&dev->cdev, &cdevdemo_fops);  
        printk(KERN_NOTICE "======== cdevdemo_setup_cdev 3");     
        dev->cdev.owner = THIS_MODULE;  
        dev->cdev.ops = &cdevdemo_fops;  
        printk(KERN_NOTICE "======== cdevdemo_setup_cdev 4");     
        err = cdev_add(&dev->cdev, devno, 1);  
        printk(KERN_NOTICE "======== cdevdemo_setup_cdev 5");  
        if(err)  
        {  
            printk(KERN_NOTICE "Error %d add cdevdemo %d", err, index);   
        }  
    }  
      
    int cdevdemo_init(void)  
    {  
        printk(KERN_NOTICE "======== cdevdemo_init ");    
        int ret;  
        dev_t devno = MKDEV(cdevdemo_major, 0);  
      
        struct class *cdevdemo_class;  
        /*申请设备号，如果申请失败采用动态申请方式*/  
        if(cdevdemo_major)  
        {  
            printk(KERN_NOTICE "======== cdevdemo_init 1");  
            ret = register_chrdev_region(devno, 1, "cdevdemo");  
        }else  
        {  
            printk(KERN_NOTICE "======== cdevdemo_init 2");  
            ret = alloc_chrdev_region(&devno,0,1,"cdevdemo");  
            cdevdemo_major = MAJOR(devno);  
        }  
        if(ret < 0)  
        {  
            printk(KERN_NOTICE "======== cdevdemo_init 3");  
            return ret;  
        }  
        /*动态申请设备结构体内存*/  
        cdevdemo_devp = kmalloc(sizeof(struct cdevdemo_dev), GFP_KERNEL);  
        if(!cdevdemo_devp)  /*申请失败*/  
        {  
            ret = -ENOMEM;  
            printk(KERN_NOTICE "Error add cdevdemo");     
            goto fail_malloc;  
        }  
      
        memset(cdevdemo_devp,0,sizeof(struct cdevdemo_dev));  
        printk(KERN_NOTICE "======== cdevdemo_init 3");  
        cdevdemo_setup_cdev(cdevdemo_devp, 0);  
      
        /*下面两行是创建了一个总线类型，会在/sys/class下生成cdevdemo目录 
          这里的还有一个主要作用是执行device_create后会在/dev/下自动生成 
          cdevdemo设备节点。而如果不调用此函数，如果想通过设备节点访问设备 
          需要手动mknod来创建设备节点后再访问。*/  
        cdevdemo_class = class_create(THIS_MODULE, "cdevdemo");  
        device_create(cdevdemo_class, NULL, MKDEV(cdevdemo_major, 0), NULL, "cdevdemo");  
      
        printk(KERN_NOTICE "======== cdevdemo_init 4");  
        return 0;  
      
        fail_malloc:  
            unregister_chrdev_region(devno,1);  
    }  
      
    void cdevdemo_exit(void)    /*模块卸载*/  
    {  
        printk(KERN_NOTICE "End cdevdemo");   
        cdev_del(&cdevdemo_devp->cdev);  /*注销cdev*/  
        kfree(cdevdemo_devp);       /*释放设备结构体内存*/  
        unregister_chrdev_region(MKDEV(cdevdemo_major,0),1);    //释放设备号  
    }  
      
    MODULE_LICENSE("Dual BSD/GPL");  
    module_param(cdevdemo_major, int, S_IRUGO);  
    module_init(cdevdemo_init);  
    module_exit(cdevdemo_exit);  


}


Makefile文件如下：

    ifneq ($(KERNELRELEASE),)  
    obj-m := cdevdemo.o  
    else  
    KERNELDIR ?= /lib/modules/$(shell uname -r)/build  
    PWD := $(shell pwd)  
    default:  
        $(MAKE) -C $(KERNELDIR) M=$(PWD) modules  
    endif  
      
    clean:  
        rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions modules.order  Module.symvers  

温馨提示：测试环境为Linux ubuntu 3.16.0-33-generic。
5、总结
本篇主要介绍了简单字符设备的编写与实现以及其中的关键点。下一篇会主要讲解下驱动的一些常用的调试技巧。




四.从如何定位oops的代码行谈驱动调试方法 

这里的makefile有问题

objdump helloworld.o -D > err.txt  
addr2line -C -f -e helloworld.o d  

上一篇我们大概聊了如何写一个简单的字符设备驱动，我们不是神，写代码肯定会出现问题，我们需要在编写代码的过程中不断调试。在普通的c应用程序中，
我们经常使用printf来输出信息，或者使用gdb来调试程序，那么驱动程序如何调试呢？我们知道在调试程序时经常遇到的问题就是野指针或者数组越界带来的问题，
在应用程序中运行这种程序就会报segmentation fault的错误，而由于驱动程序的特殊性，出现此类情况后往往会直接造成系统宕机，并会抛出oops信息。
那么我们如何来分析oops信息呢，甚至根据oops信息来定位具体的出错的代码行呢？下面就根据一个简单的实例来说明如何调试驱动程序。
如何根据oops定位代码行

我们借用linux设备驱动第二篇：构造和运行模块里面的hello world程序来演示出错的情况，含有错误代码的hello world如下：

{
    #include <linux/init.h>  
    #include <linux/module.h>  
    MODULE_LICENSE("Dual BSD/GPL");  
        
    static int hello_init(void)  
    {  
        char *p = NULL;  
        memcpy(p, "test", 4);  
        printk(KERN_ALERT "Hello, world\n");  
        return 0;  
    }  
    static void hello_exit(void)  
    {  
        printk(KERN_ALERT "Goodbye, cruel world\n");  
    }  
        
    module_init(hello_init);  
    module_exit(hello_exit);  
}


Makefile文件如下：
{
    ifneq ($(KERNELRELEASE),)  
    obj-m := helloworld.o  
    else  
    KERNELDIR ?= /lib/modules/$(shell uname -r)/build  
    PWD := $(shell pwd)  
    default:  
            $(MAKE) -C $(KERNELDIR) M=$(PWD) modules  
    endif  
      
    clean:  
        rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions modules.order  Module.symvers  

}


很明显，以上代码的第8行是一个空指针错误。insmod后会出现下面的oops信息：

    [  459.516441] BUG: unable to handle kernel NULL pointer dereference at           (null)  
    [  459.516445] <span style="color:#ff0000;">IP: [<ffffffffc061400d>] hello_init+0xd/0x30 [helloworld]</span>"  
    [  459.516448] PGD 0   
    [  459.516450] Oops: 0002 [#1] SMP   
    [  459.516452] Modules linked in: helloworld(OE+) vmw_vsock_vmci_transport vsock coretemp crct10dif_pclmul crc32_pclmul ghash_clmulni_intel aesni_intel vmw_balloon snd_ens1371 aes_x86_64 lrw snd_ac97_codec gf128mul glue_helper ablk_helper cryptd ac97_bus gameport snd_pcm serio_raw snd_seq_midi snd_seq_midi_event snd_rawmidi snd_seq snd_seq_device snd_timer vmwgfx btusb ttm snd drm_kms_helper drm soundcore shpchp vmw_vmci i2c_piix4 rfcomm bnep bluetooth 6lowpan_iphc parport_pc ppdev mac_hid lp parport hid_generic usbhid hid psmouse ahci libahci floppy e1000 vmw_pvscsi vmxnet3 mptspi mptscsih mptbase scsi_transport_spi pata_acpi [last unloaded: helloworld]  
    [  459.516476] CPU: 0 PID: 4531 Comm: insmod Tainted: G           OE 3.16.0-33-generic #44~14.04.1-Ubuntu  
    [  459.516478] Hardware name: VMware, Inc. VMware Virtual Platform/440BX Desktop Reference Platform, BIOS 6.00 05/20/2014  
    [  459.516479] task: ffff88003821f010 ti: ffff880038fa0000 task.ti: ffff880038fa0000  
    [  459.516480] RIP: 0010:[<ffffffffc061400d>]  [<ffffffffc061400d>] hello_init+0xd/0x30 [helloworld]  
    [  459.516483] RSP: 0018:ffff880038fa3d40  EFLAGS: 00010246  
    [  459.516484] RAX: ffff88000c31d901 RBX: ffffffff81c1a020 RCX: 000000000004b29f  
    [  459.516485] RDX: 000000000004b29e RSI: 0000000000000017 RDI: ffffffffc0615024  
    [  459.516485] RBP: ffff880038fa3db8 R08: 0000000000015e80 R09: ffff88003d615e80  
    [  459.516486] R10: ffffea000030c740 R11: ffffffff81002138 R12: ffff88000c31d0c0  
    [  459.516487] R13: 0000000000000000 R14: ffffffffc0614000 R15: ffffffffc0616000  
    [  459.516488] FS:  00007f8a6fa86740(0000) GS:ffff88003d600000(0000) knlGS:0000000000000000  
    [  459.516489] CS:  0010 DS: 0000 ES: 0000 CR0: 0000000080050033  
    [  459.516490] CR2: 0000000000000000 CR3: 0000000038760000 CR4: 00000000003407f0  
    [  459.516522] DR0: 0000000000000000 DR1: 0000000000000000 DR2: 0000000000000000  
    [  459.516524] DR3: 0000000000000000 DR6: 00000000fffe0ff0 DR7: 0000000000000400  
    [  459.516524] Stack:  
    [  459.516537]  ffff880038fa3db8 ffffffff81002144 0000000000000001 0000000000000001  
    [  459.516540]  0000000000000001 ffff880028ab5040 0000000000000001 ffff880038fa3da0  
    [  459.516541]  ffffffff8119d0b2 ffffffffc0616018 00000000bd1141ac ffffffffc0616018  
    [  459.516543] Call Trace:  
    [  459.516548]  [<ffffffff81002144>] ? do_one_initcall+0xd4/0x210  
    [  459.516550]  [<ffffffff8119d0b2>] ? __vunmap+0xb2/0x100  
    [  459.516554]  [<ffffffff810ed9b1>] load_module+0x13c1/0x1b80  
    [  459.516557]  [<ffffffff810e9560>] ? store_uevent+0x40/0x40  
    [  459.516560]  [<ffffffff810ee2e6>] SyS_finit_module+0x86/0xb0  
    [  459.516563]  [<ffffffff8176be6d>] system_call_fastpath+0x1a/0x1f  
    [  459.516564] Code: <c7> 04 25 00 00 00 00 74 65 73 74 31 c0 48 89 e5 e8 a2 86 14 c1 31   
    [  459.516573] RIP  [<ffffffffc061400d>] hello_init+0xd/0x30 [helloworld]  
    [  459.516575]  RSP <ffff880038fa3d40>  
    [  459.516576] CR2: 0000000000000000  
    [  459.516578] ---[ end trace 7c52cc8624b7ea60 ]---  

下面简单分析下oops信息的内容。

由BUG: unable to handle kernel NULL pointer dereference at (null)知道出错的原因是使用了空指针。标红的部分确定了具体出错的函数。
Modules linked in: helloworld表明了引起oops问题的具体模块。call trace列出了函数的调用信息。这些信息中其中标红的部分是最有用的，我们可以根据其信息
找到具体出错的代码行。下面就来说下，如何定位到具体出错的代码行。

第一步我们需要使用objdump把编译生成的bin文件反汇编，我们这里就是helloworld.o，如下命令把反汇编信息保存到err.txt文件中：

    objdump helloworld.o -D > err.txt  


err.txt内容如下：

    helloworld.o:     file format elf64-x86-64  
      
      
    Disassembly of section .text:  
      
    <span style="color:#ff0000;>0000000000000000 <init_module>:</span>  
       0:   e8 00 00 00 00          callq  5 <init_module+0x5>  
       5:   55                      push   %rbp  
       6:   48 c7 c7 00 00 00 00    mov    $0x0,%rdi  
       d:   c7 04 25 00 00 00 00    movl   $0x74736574,0x0  
      14:   74 65 73 74   
      18:   31 c0                   xor    %eax,%eax  
      1a:   48 89 e5                mov    %rsp,%rbp  
      1d:   e8 00 00 00 00          callq  22 <init_module+0x22>  
      22:   31 c0                   xor    %eax,%eax  
      24:   5d                      pop    %rbp  
      25:   c3                      retq     
      26:   66 2e 0f 1f 84 00 00    nopw   %cs:0x0(%rax,%rax,1)  
      2d:   00 00 00   
      
    0000000000000030 <cleanup_module>:  
      30:   e8 00 00 00 00          callq  35 <cleanup_module+0x5>  
      35:   55                      push   %rbp  
      36:   48 c7 c7 00 00 00 00    mov    $0x0,%rdi  
      3d:   31 c0                   xor    %eax,%eax  
      3f:   48 89 e5                mov    %rsp,%rbp  
      42:   e8 00 00 00 00          callq  47 <cleanup_module+0x17>  
      47:   5d                      pop    %rbp  
      48:   c3                      retq     
      
    Disassembly of section .rodata.str1.1:  
      
    0000000000000000 <.rodata.str1.1>:  
       0:   01 31                   add    %esi,(%rcx)  
       2:   48                      rex.W  
       3:   65                      gs  
       4:   6c                      insb   (%dx),%es:(%rdi)  
       5:   6c                      insb   (%dx),%es:(%rdi)  
       6:   6f                      outsl  %ds:(%rsi),(%dx)  
       7:   2c 20                   sub    $0x20,%al  
       9:   77 6f                   ja     7a <cleanup_module+0x4a>  
       b:   72 6c                   jb     79 <cleanup_module+0x49>  
       d:   64 0a 00                or     %fs:(%rax),%al  
      10:   01 31                   add    %esi,(%rcx)  
      12:   47 6f                   rex.RXB outsl %ds:(%rsi),(%dx)  
      14:   6f                      outsl  %ds:(%rsi),(%dx)  
      15:   64                      fs  
      16:   62                      (bad)    
      17:   79 65                   jns    7e <cleanup_module+0x4e>  
      19:   2c 20                   sub    $0x20,%al  
      1b:   63 72 75                movslq 0x75(%rdx),%esi  
      1e:   65                      gs  
      1f:   6c                      insb   (%dx),%es:(%rdi)  
      20:   20 77 6f                and    %dh,0x6f(%rdi)  
      23:   72 6c                   jb     91 <cleanup_module+0x61>  
      25:   64 0a 00                or     %fs:(%rax),%al  
      
    Disassembly of section .modinfo:  
      
    0000000000000000 <__UNIQUE_ID_license0>:  
       0:   6c                      insb   (%dx),%es:(%rdi)  
       1:   69 63 65 6e 73 65 3d    imul   $0x3d65736e,0x65(%rbx),%esp  
       8:   44 75 61                rex.R jne 6c <cleanup_module+0x3c>  
       b:   6c                      insb   (%dx),%es:(%rdi)  
       c:   20 42 53                and    %al,0x53(%rdx)  
       f:   44 2f                   rex.R (bad)   
      11:   47 50                   rex.RXB push %r8  
      13:   4c                      rex.WR  
        ...  
      
    Disassembly of section .comment:  
      
    0000000000000000 <.comment>:  
       0:   00 47 43                add    %al,0x43(%rdi)  
       3:   43 3a 20                rex.XB cmp (%r8),%spl  
       6:   28 55 62                sub    %dl,0x62(%rbp)  
       9:   75 6e                   jne    79 <cleanup_module+0x49>  
       b:   74 75                   je     82 <cleanup_module+0x52>  
       d:   20 34 2e                and    %dh,(%rsi,%rbp,1)  
      10:   38 2e                   cmp    %ch,(%rsi)  
      12:   32 2d 31 39 75 62       xor    0x62753931(%rip),%ch        # 62753949 <cleanup_module+0x62753919>  
      18:   75 6e                   jne    88 <cleanup_module+0x58>  
      1a:   74 75                   je     91 <cleanup_module+0x61>  
      1c:   31 29                   xor    %ebp,(%rcx)  
      1e:   20 34 2e                and    %dh,(%rsi,%rbp,1)  
      21:   38 2e                   cmp    %ch,(%rsi)  
      23:   32 00                   xor    (%rax),%al  
      
    Disassembly of section __mcount_loc:  
      
    0000000000000000 <__mcount_loc>:  

由oops信息我们知道出错的地方是hello_init的地址偏移0xd。而有dump信息知道，hello_init的地址即init_module的地址，因为hello_init即本模块的初始化入口，
如果在其他函数中出错，dump信息中就会有相应符号的地址。由此我们得到出错的地址是0xd，下一步我们就可以使用addr2line来定位具体的代码行：

    addr2line -C -f -e helloworld.o d  

此命令就可以得到行号了。以上就是通过oops信息来定位驱动崩溃的行号。

其他调试手段

以上就是通过oops信息来获取具体的导致崩溃的代码行，这种情况都是用在遇到比较严重的错误导致内核挂掉的情况下使用的，另外比较常用的调试手段就是使用printk来输出打印信息。
printk的使用方法类似printf，只是要注意一下打印级别，详细介绍在Linux设备驱动第二篇：构造和运行模块中已有描述，另外需要注意的是大量使用printk会严重拖慢系统，
所以使用过程中也要注意。

以上两种调试手段是我工作中最常用的，还有一些其他的调试手段，例如使用/proc文件系统，使用trace等用户空间程序，使用gdb，kgdb等，这些调试手段一般不太容易使用或者不太方便使用，所以这里就不在介绍了。

介绍完驱动的调试方法后，下一篇会介绍下linux驱动的并发与竞态，欢迎关注。




五.驱动中的并发与竟态 
综述

在上一篇介绍了Linux驱动的调试方法，这一篇介绍一下在驱动编程中会遇到的并发和竟态以及如何处理并发和竞争。

首先什么是并发与竟态呢？并发(concurrency)指的是多个执行单元同时、并行被执行。而并发的执行单元对共享资源(硬件资源和软件上的全局、静态变量)的访问则容易导致竞态
(race conditions)。可能导致并发和竟态的情况有：

    SMP（Symmetric Multi-Processing），对称多处理结构。SMP是一种紧耦合、共享存储的系统模型，它的特点是多个CPU使用共同的系统总线，因此可访问共同的外设和存储器。 
    中断。中断可 打断正在执行的进程，若中断处理程序访问进程正在访问的资源，则竞态也会发生。中断也可能被新的更高优先级的中断打断，因此，多个中断之间也可能引起并发而
    导致竞态。
    内核进程的抢占。linux是可抢占的，所以一个内核进程可能被另一个高优先级的内核进程抢占。如果两个进程共同访问共享资源，就会出现竟态。

以上三种情况只有SMP是真正意义上的并行，而其他都是宏观上的并行，微观上的串行。但其都会引发对临界共享区的竞争问题。而解决竞态问题的途径是保证对共享资源的互斥访问，
即一个执行单元在访问共享资源的时候，其他的执行单元被禁止访问。那么linux内核中如何做到对对共享资源的互斥访问呢？在linux驱动编程中，常用的解决并发与竟态的手段有
信号量与互斥锁，Completions 机制，自旋锁（spin lock），以及一些其他的不使用锁的实现方式。下面一一介绍。

信号量与互斥锁
信号量其实就是一个整型值，其核心是一个想进入临界区的进程将在相关信号量上调用 P; 如果信号量的值大于零, 这个值递减 1 并且进程继续. 相反,，如果信号量的值是0
( 或更小 ), 进程必须等待直到别人释放信号量. 解锁一个信号量通过调用 V 完成； 这个函数递增信号量的值,并且， 如果需要, 唤醒等待的进程。而当信号量的初始值为1的时候，
就变成了互斥锁。
信号量的典型使用形式：

    //声明信号量  
    struct semaphore sem;  
      
    //初始化信号量  
    void sema_init(struct semaphore *sem, int val)  
        //常用下面两种形式  
    #define init_MUTEX(sem) sema_init(sem, 1)  
    #define init_MUTEX_LOCKED(sem) sema_init(sem, 0)  
        //以下是初始化信号量的快捷方式，最常用的  
    DECLARE_MUTEX(name)    //初始化name的信号量为1  
    DECLARE_MUTEX_LOCKED(name) //初始化信号量为0  
      
    //常用操作  
    DECLARE_MUTEX(mount_sem);  
    down(&mount_sem); //获取信号量  
    ...  
    critical section    //临界区  
    ...  
    up(&mount_sem);    //释放信号量  

常用的down操作还有

    // 类似down(),因为down()而进入休眠的进程不能被信号打断，而因为down_interruptible()而进入休眠的进程能被信号打断，   
    // 信号也会导致该函数返回，此时返回值非0  
    int down_interruptible(struct semaphore *sem);  
    // 尝试获得信号量sem，若立即获得，它就获得该信号量并返回0，否则，返回非0.它不会导致调用者睡眠，可在中断上下文使用  
    int down_trylock(struct semaphore *sem);  

Completions 机制
完成量(completion)提供了一种比信号量更好的同步机制，它用于一个执行单元等待另一个执行单元执行完某事。

    </pre></div><div><pre name="code" class="cpp">// 定义完成量  
    struct completion my_completion;  
       
    // 初始化completion  
    init_completion(&my_completion);  
       
    // 定义和初始化快捷方式：  
    DECLEAR_COMPLETION(my_completion);  
       
    // 等待一个completion被唤醒  
    void wait_for_completion(struct completion *c);  
       
    // 唤醒完成量  
    void cmplete(struct completion *c);  
    void cmplete_all(struct completion *c);  

自旋锁
若一个进程要访问临界资源，测试锁空闲，则进程获得这个锁并继续执行；若测试结果表明锁扔被占用，进程将在一个小的循环内重复“测试并设置”操作，
进行所谓的“自旋”，等待自旋锁持有者释放这个锁。自旋锁与互斥锁类似，但是互斥锁不能用在可能睡眠的代码中，而自旋锁可以用在可睡眠的代码中，
典型的应用是可以用在中断处理函数中。自旋锁的相关操作：
自旋锁不会引起睡眠

    // 定义自旋锁   
    spinlock_t spin;   
       
    // 初始化自旋锁  
    spin_lock_init(lock);  
       
    // 获得自旋锁：若能立即获得锁，它获得锁并返回，否则，自旋，直到该锁持有者释放  
    spin_lock(lock);   
       
    // 尝试获得自旋锁：若能立即获得锁，它获得并返回真，否则立即返回假，不再自旋  
    spin_trylock(lock);   
       
    // 释放自旋锁: 与spin_lock(lock)和spin_trylock(lock)配对使用  
    spin_unlock(lock);   
       
      自旋锁的使用：  
    // 定义一个自旋锁  
    spinlock_t lock;  
    spin_lock_init(&lock);  
       
    spin_lock(&lock);  // 获取自旋锁，保护临界区  
    ...  // 临界区  
    spin_unlock();  // 解锁  


自旋锁持有期间内核的抢占将被禁止。自旋锁可以保证临界区不受别的CPU和本CPU内的抢占进程打扰，但是得到锁的代码路径在执行临界区的时候还可能受到中断和底半部(BH)的影响。
为防止这种影响，需要用到自旋锁的衍生：

    spin_lock_irq() = spin_lock() + local_irq_disable()  
    spin_unlock_irq() = spin_unlock() + local_irq_enable()  
    spin_lock_irqsave() = spin_lock() + local_irq_save()  
    spin_unlock_irqrestore() = spin_unlock() + local_irq_restore()  
    spin_lock_bh() = spin_lock() + local_bh_disable()  
    spin_unlock_bh() = spin_unlock() + local_bh_enable()  

其他的一些选择
以上是linux驱动编程中经常用到的锁机制，下面讲一些内核中其他的一些实现。
不加锁算法
有时, 你可以重新打造你的算法来完全避免加锁的需要.。许多读者/写者情况 -- 如果只有一个写者 -- 常常能够在这个方式下工作.。如果写者小心使数据结构，
由读者所见的，是一直一致的,，有可能创建一个不加锁的数据结构。在linux内核中就有一个通用的无锁的环形缓冲实现，具体内容参考<linux/kfifo.h>。

原子变量与位操作
原子操作指的是在执行过程中不会被别的代码路径所中断的操作。原子变量与位操作都是原子操作。以下是其相关操作介绍。

    // 设置原子变量的值  
    void atomic_set(atomic_t *v, int i);  // 设置原子变量的值为i  
    atomic_t v = ATOMIC_INIT(0);  // 定义原子变量v，并初始化为0  
       
    // 获取原子变量的值  
    atomic_read(atomic_t *v);  // 返回原子变量的值  
       
    // 原子变量加/减  
    void atomic_add(int i, atomic_t *v);  // 原子变量加i  
    void atomic_sub(int i, atomic_t *v);  // 原子变量减i  
       
    // 原子变量自增/自减  
    void atomic_inc(atomic_t *v);  // 原子变量增加1  
    void atomic_dec(atomic_t *v);  // 原子变量减少1  
       
    // 操作并测试：对原子变量进行自增、自减和减操作后(没有加)测试其是否为0，为0则返回true，否则返回false  
    int atomic_inc_and_test(atomic_t *v);  
    int atomic_dec_and_test(atomic_t *v);  
    int atomic_sub_and_test(int i, atomic_t *v);  
       
    // 操作并返回： 对原子变量进行加/减和自增/自减操作，并返回新的值  
    int atomic_add_return(int i, atomic_t *v);  
    int atomic_sub_return(int i, atomic_t *v);  
    int atomic_inc_return(atomic_t *v);  
    int atomic_dec_return(atomic_t *v);  
      位原子操作：  
    // 设置位  
    void set_bit(nr, void *addr);  // 设置addr地址的第nr位，即将位写1  
       
    // 清除位  
    void clear_bit(nr, void *addr);  // 清除addr地址的第nr位，即将位写0  
       
    // 改变位  
    void change_bit(nr, void *addr);  // 对addr地址的第nr位取反  
       
    // 测试位  
    test_bit(nr, void *addr); // 返回addr地址的第nr位  
       
    // 测试并操作：等同于执行test_bit(nr, void *addr)后再执行xxx_bit(nr, void *addr)  
    int test_and_set_bit(nr, void *addr);  
    int test_and_clear_bit(nr, void *addr);  
    int test_and_change_bit(nr, void *addr);  

seqlock（顺序锁）
使用seqlock锁，读执行单元不会被写执行单元阻塞，即读执行单元可以在写执行单元对被seqlock锁保护的共享资源进行写操作时仍然可以继续读，
而不必等待写执行单元完成写操作，写执行单元也不需要等待所有读执行单元完成读操作才去进行写操作。写执行单元之间仍是互斥的。若读操作期间，
发生了写操作，必须重新读取数据。seqlock锁必须要求被保护的共享资源不含有指针。

    // 获得顺序锁  
    void write_seqlock(seqlock_t *sl);  
    int write_tryseqlock(seqlock_t *sl);  
    write_seqlock_irqsave(lock, flags)  
    write_seqlock_irq(lock)  
    write_seqlock_bh()  
       
    // 释放顺序锁  
    void write_sequnlock(seqlock_t *sl);  
    write_sequnlock_irqrestore(lock, flags)  
    write_sequnlock_irq(lock)  
    write_sequnlock_bh()  
       
    // 写执行单元使用顺序锁的模式如下：  
    write_seqlock(&seqlock_a);  
    ...  // 写操作代码块  
    write_sequnlock(&seqlock_a);  
      读执行单元操作：  
    // 读开始：返回顺序锁sl当前顺序号  
    unsigned read_seqbegin(const seqlock_t *sl);  
    read_seqbegin_irqsave(lock, flags)  
       
    // 重读：读执行单元在访问完被顺序锁sl保护的共享资源后需要调用该函数来检查，在读访问期间是否有写操作。若有写操作，重读  
    int read_seqretry(const seqlock_t *sl, unsigned iv);  
    read_seqretry_irqrestore(lock, iv, flags)  
       
    // 读执行单元使用顺序锁的模式如下：  
    do{  
        seqnum = read_seqbegin(&seqlock_a);  
        // 读操作代码块   
        ...  
    }while(read_seqretry(&seqlock_a, seqnum));  

读取-拷贝-更新（RCU）
读取-拷贝-更新(RCU) 是一个高级的互斥方法，在合适的时候可以取得非常高的效率。RCU可以看作读写锁的高性能版本，相比读写锁，
RCU的优点在于既允许多个读执行单元同时访问被保护的数据，又允许多个读执行单元和多个写执行单元同时访问被保护的数据。但是RCU不能替代读写锁，
因为如果写比较多时，对读执行单元的性能提高不能弥补写执行单元导致的损失。由于平时应用较少，所以不做多说。
小结
以上就是linux驱动编程中涉及的并发与竞态的内容，下面做一个简单的小结。

现在的处理器基本上都是SMP类型的，而且在新的内核版本中，基本上都支持抢占式的操作，在linux中很多程序都是可重入的，要保护这些数据，就得使用不同的锁机制。
而锁机制的基本操作过程其实大同小异的，声明变量，上锁，执行临界区代码，然后再解锁。不同点在于，可以重入的限制不同，有的可以无限制重入，有的只允许异种操作重入，
而有的是不允许重入操作的，有的可以在可睡眠代码中使用，有的不可以在可睡眠代码中使用。而在考虑不同的锁机制的使用时，也要考虑CPU处理的效率问题，对于不同的代码长度，
不同的代码执行时间，选择一个好的锁对CPU的良好使用有很大的影响，否则将造成浪费。 

之前在linux设备驱动第三篇：写一个简单的字符设备驱动中介绍了简单的字符设备驱动，下一篇将介绍一些字符设备驱动中得高级操作。






六.高级字符驱动操作之iotcl 

 在之前我们介绍了如何实现一个简单的字符设备驱动，并介绍了简单的open，close，read，write等驱动提供的基本功能。但是一个真正的设备驱动往往提供了
 比简单读写更高级的功能。这一篇我们就来介绍一些驱动动中使用的一些高级的操作的实现。

大部分驱动除了提供对设备的读写操作外，还需要提供对硬件控制的接口，比如查询一个framebuffer设备能提供多大的分辨率，读取一个RTC设备的时间，
设置一个gpio的高低电平等等。而这些对硬件操作能力的实现一般都是通过ioctl方法来实现的

1. 原型介绍

ioctl在用户空间的原型为：int ioctl(int fd, unsigned long cmd, ...);

原型中的点不表示一个变数目的参数, 而是一个单个可选的参数, 传统上标识为 char *argp. 这些点在那里只是为了阻止在编译时的类型检查. 第 3

个参数的实际特点依赖所发出的特定的控制命令( 第 2 个参数 ). 一些命令不用参数, 一些用一个整数值, 以及一些使用指向其他数据的指针. 使用一个指针是传递任意数据到
ioctl 调用的方法; 设备接着可与用户空间交换任何数量的数据.

ioctl在内核空间的原型为：int (*ioctl) (struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg);

inode 和 filp 指针是对应应用程序传递的文件描述符 fd 的值, 和传递给 open 方法的相同参数. cmd参数从用户那里不改变地传下来, 并且可选的参数 

arg 参数以一个 unsigned long 的形式传递, 不管它是否由用户给定为一个整数或一个指针. 如果调用程序不传递第 3 个参数, 被驱动操作收到的arg 值是无定义的. 
因为类型检查在这个额外参数上被关闭, 编译器不能对此作出警告。

2. iotcl cmd的选择

在实现ioctl之前，我们应该来定义一组ioctl命令，一种简单的方法是使用一组简单的数字来标示，比如从0到9。这种情况一般也没有问题，但是最好不要这样做，
ioctl的cmd应该是在系统内是唯一的，这样可以防止向错误的设备发出正确的命令。而如果ioctl命令在系统内是唯一的，那么就不会发生这种情况。

Linux中把ioctl cmd划分成几个位段来帮助创建唯一的cmd。这几个位段一般是：type（模数），序号，传输方向和参数大小。在定义的时候可以参考include/asm/ioctl.h 
和 Documentation/ioctl-number.txt两个文件，头文件定义了构建cmd命令的宏，而ioctl-number.txt列举了内核中已经使用的tpye，为了唯一性，尽量不要和这里的type重叠。

以下是这几个位段的简单介绍：

type

魔数. 只是选择一个数(在参考了 ioctl-number.txt之后)并且使用它在整个驱动中. 这个成员是 8 位宽(_IOC_TYPEBITS).

number

序(顺序)号. 它是 8 位(_IOC_NRBITS)宽.

direction

数据传送的方向,如果这个特殊的命令涉及数据传送. 可能的值是 _IOC_NONE(没有数据传输), _IOC_READ, _IOC_WRITE, 和 _IOC_READ|_IOC_WRITE (数据在2个方向被传送). 数据传送是从应用程序的观点来看待的; _IOC_READ 意思是从设备读, 因此设备必须写到用户空间. 注意这个成员是一个位掩码, 因此 _IOC_READ 和 _IOC_WRITE 可使用一个逻辑 AND 操作来抽取.

size

涉及到的用户数据的大小. 这个成员的宽度是依赖体系的, 但是常常是 13 或者 14 位. 你可为你的特定体系在宏 _IOC_SIZEBITS 中找到它的值. 你使用这个 size 成员不是强制的 - 内核不检查它 -- 但是它是一个好主意. 正确使用这个成员可帮助检测用户空间程序的错误并使你实现向后兼容, 如果你曾需要改变相关数据项的大小. 如果你需要更大的数据结构, 但是, 你可忽略这个 size 成员. 我们很快见到如何使用这个成员.

下面是一个定义ioctl命令的展示：

/* Use 'k' as magic number */
#define SCULL_IOC_MAGIC 'k'
/* Please use a different 8-bit number in your code */

#define SCULL_IOCRESET _IO(SCULL_IOC_MAGIC, 0)
/*
 * S means "Set" through a ptr,
 * T means "Tell" directly with the argument value
 * G means "Get": reply by setting through a pointer
 * Q means "Query": response is on the return value
 * X means "eXchange": switch G and S atomically
 * H means "sHift": switch T and Q atomically
 */
#define SCULL_IOCSQUANTUM _IOW(SCULL_IOC_MAGIC, 1, int)
#define SCULL_IOCSQSET _IOW(SCULL_IOC_MAGIC, 2, int)
#define SCULL_IOCTQUANTUM _IO(SCULL_IOC_MAGIC, 3)
#define SCULL_IOCTQSET _IO(SCULL_IOC_MAGIC, 4)
#define SCULL_IOCGQUANTUM _IOR(SCULL_IOC_MAGIC, 5, int)
#define SCULL_IOCGQSET _IOR(SCULL_IOC_MAGIC, 6, int)
#define SCULL_IOCQQUANTUM _IO(SCULL_IOC_MAGIC, 7)
#define SCULL_IOCQQSET _IO(SCULL_IOC_MAGIC, 8)
#define SCULL_IOCXQUANTUM _IOWR(SCULL_IOC_MAGIC, 9, int)
#define SCULL_IOCXQSET _IOWR(SCULL_IOC_MAGIC,10, int)
#define SCULL_IOCHQUANTUM _IO(SCULL_IOC_MAGIC, 11)
#define SCULL_IOCHQSET _IO(SCULL_IOC_MAGIC, 12)

#define SCULL_IOC_MAXNR 14

关于_IOWR等宏定义更多的内容可参考头文件中的定义。
3. IOCTL的返回值

IOCTL的实现往往都是一个switch case语句，返回值依赖每个case分支的实现。当遇到没有定义的cmd时改返回什么值呢，我建议使用-EINVAL，表示无用的参数。另外一点，
在case分支比较多的时候，有些人大意常常会忘记写break，导致后面的case分支同样执行，导致错误发生。

4. IOCTL的arg参数

有些ioctl命令并不需要arg这个参数，而大部分ioctl需要在应用层和内核层传递数据，就需要用到这个参数。当arg参数是一个整形的时候，非常简单，我们直接拿来使用就可以了。
如果是一个指针，就需要小心一些。

应用层和内核层的数据交换我们常用的是copy_from_user 和 copy_to_user 函数，它们可用来安全地用来移动数据。 这些函数也可用在 ioctl 方法中。
但是ioctl中的数据项常常是很小的数据，用这两个函数有点笨重了，我们可以尝试使用其他的方式来实现数据的传递。

int access_ok(int type, const void *addr, unsigned long size);

这个函数用来检查给定的地址是否满足特定的访问需求，这个函数只检查而没有数据copy。使用access_ok之后就可以安全地传输数据。可使用下面的接口来做数据的传输：

put_user(datum, ptr)
       __put_user(datum, ptr)
这些宏定义写 datum 到用户空间；它们相对快，且应当被调用来代替 copy_to_user 无论何时要传送单个值时。这些宏已被编写来允许传递任何类型的指针到 put_user,
只要它是一个用户空间地址。传送的数据大小依赖 prt 参数的类型，并且在编译时使用 sizeof 和 typeof 等编译器内建宏确定。结果是，如果 prt 是一个 char 指针，
传送一个字节，以及对于 2, 4, 和 可能的 8 字节。

put_user 检查来确保这个进程能够写入给定的内存地址，它在成功时返回 0，并且在错误时返回 -EFAULT。 __put_user 进行更少的检查(它不调用 access_ok)，
但是仍然能够失败如果被指向的内存对用户是不可写的。因此， __put_user 应当只用在内存区已经用 access_ok 检查过的时候。

作为一个通用的规则，当你实现一个 read 方法时，调用 __put_user 来节省几个周期，或者当你拷贝几个项时，因此， 在第一次数据传送之前调用 access_ok 一次，
 如同上面 ioctl 所示。

get_user(local, ptr)
       __get_user(local, ptr)
这些宏定义用来从用户空间接收单个数据。它们象 put_user 和 __put_user，但是在相反方向传递数据。获取的值存储于本地变量 local； 返回值指出这个操作是否成功。
再次， __get_user 应当只用在已经使用 access_ok 校验过的地址。

以上是ioctl操作相关的内容，由于篇幅原因，就写到这里，下一节接着写一些其他的高级操作方法，例如阻塞IO、非阻塞IO等。敬请关注。



七.高级字符驱动操作之阻塞IO 

 我们之前介绍过简单的read，write操作，那么会有一个问题：当驱动无法立即响应请求该怎么办？比如一个进程调用read读取数据，当没有数据可读时该怎么办，
是立即返回还是等到有数据的时候；另一种情况是进程调用write向设备写数据，如果缓冲区满了或者设备正忙的时候怎么办，是立即返回还是继续等待直到设备可写？这种情况下，
一般的缺省做法是使进程睡眠直到请求可以满足为止。本篇就介绍遇到这类问题驱动的处理方法。
睡眠

什么是睡眠？一个进程睡眠意味着它暂时放弃了CPU的运行权，直到某个条件发生后才可再次被系统调度。

在驱动里面很容易使一个进程进入睡眠状态，但是这里有几个规则需要特别注意。

    原子上下文不能睡眠。这意味着驱动在持有一个自旋锁, seqlock, 或者 RCU 锁时不能睡眠。
    关闭中断的情况下不能睡眠。在中断处理函数中不能睡眠。
    在持有信号量时可以睡眠，但是会造成其他等待的进程也会进入睡眠，所以应该特别注意，睡眠时间应很短。
    在被唤醒后应做一些必要的检查，确定你等待的条件已经满足。因为你不知道睡眠的这段时间发生了什么。
    睡眠前确定能被唤醒，否则不要睡眠。

如何睡眠和唤醒

睡眠的进程会进入等待队列，一个等待队列可以如下声明：

DECLARE_WAIT_QUEUE_HEAD(name);
或者动态地, 如下:

wait_queue_head_t my_queue;
init_waitqueue_head(&my_queue);

当一个进程需要睡眠，可以调用下面的接口：

//进程被置为不可中断的睡眠，一般不要这样
wait_event(queue, condition)
//它可能被信号中断，此版本应该检查返回值，若返回非零则可能是被某些信号打断，驱动应///该返回-ERESTARTSYS.
wait_event_interruptible(queue, condition)
//下面两个等待一段时间，超时后返回0.
wait_event_timeout(queue, condition, timeout)
wait_event_interruptible_timeout(queue, condition, timeout)

要唤醒休眠的进程，那么其他的进程要调用唤醒函数：

//以下函数唤醒所有的在给定队列上等待的进程，一般情况下带interruptible的配对，不带//的配对
void wake_up(wait_queue_head_t *queue);
void wake_up_interruptible(wait_queue_head_t *queue);

阻塞和非阻塞的选择

上面说了睡眠的方法，这种实现就是阻塞IO的实现，还有一种情况是要求不管IO是否可用，调用都要立即返回，就是非阻塞的实现。比如read时，虽然没有数据可读，
但是我不想等待，我要立马返回。

非阻塞的IO由 filp->f_flags 中的 O_NONBLOCK 标志来指示，这个标志位于<Linux/fcntl.h>, 被 <linux/fs.h>自动包含。这个标志可以在open的时候指定。

缺省状态下IO是阻塞的（没有指定O_NONBLOCK的情况下），在实现read/write的时候需要符合下面的标准：

    如果一个进程调用 read 但是没有数据可用(尚未), 这个进程必须阻塞. 这个进程在有数据达到时被立刻唤醒, 并且那个数据被返回给调用者, 即便小于在给方法的 
    count 参数中请求的数量。
    如果一个进程调用 write 并且在缓冲中没有空间, 这个进程必须阻塞, 并且它必须在一个与用作 read 的不同的等待队列中. 当一些数据被写入硬件设备, 
    并且在输出缓冲中的空间变空闲, 这个进程被唤醒并且写调用成功, 尽管数据可能只被部分写入如果在缓冲只没有空间给被请求的 count 字节。

这两句话都假设有输入和输出缓冲，实际上也是这样，几乎每个设备驱动都有输入输出缓冲。缓冲提高了访问效率，防止了数据的丢失。

如果指定O_NONBLOCK，即非阻塞的访问。read和write的做法是不同的。在这种情况下，这些调用简单的返回-EAGAIN。只有read，write和open文件操作收到非阻塞标志的影响。

下面是一个简单的read的实现，其中兼容了阻塞和非阻塞的实现（关键地方以添加注释）：

static ssize_t scull_p_read (struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
        struct scull_pipe *dev = filp->private_data;
        if (down_interruptible(&dev->sem))
                return -ERESTARTSYS;

        while (dev->rp == dev->wp)
        { /* nothing to read */
                up(&dev->sem); /* release the lock */
                //判断是否是阻塞访问，如果是非阻塞访问，那么立即返回-EAGAIN.
                if (filp->f_flags & O_NONBLOCK)

                        return -EAGAIN;
                PDEBUG("\"%s\" reading: going to sleep\n", current->comm); 
                //如果是阻塞访问，那么睡眠等待，等到读条件满足时继续执行。
                if (wait_event_interruptible(dev->inq, (dev->rp != dev->wp)))
                        return -ERESTARTSYS; /* signal: tell the fs layer to handle it */ /* otherwise loop, but first reacquire the lock */
                if (down_interruptible(&dev->sem))
                        return -ERESTARTSYS;
        }
        /* ok, data is there, return something */
        
        //以下即正常读取数据。
        if (dev->wp > dev->rp)
                count = min(count, (size_t)(dev->wp - dev->rp));
        else /* the write pointer has wrapped, return data up to dev->end */
                count = min(count, (size_t)(dev->end - dev->rp));
        if (copy_to_user(buf, dev->rp, count))
        {
                up (&dev->sem);
                return -EFAULT;
        }
        dev->rp += count;
        if (dev->rp == dev->end)

                dev->rp = dev->buffer; /* wrapped */
        up (&dev->sem);

        /* finally, awake any writers and return */
        wake_up_interruptible(&dev->outq);
        PDEBUG("\"%s\" did read %li bytes\n",current->comm, (long)count);
        return count;
}

互斥等待

之前我们说过当一个进程调用wake_up后，所有这个队列上等待的进程被置为可运行的。一般情况下这样是没有问题的，但是在个别的情况下，可能提前知道只有一个被唤醒的进程
将成功获得需要的资源，并且其他的进程将再次睡眠。如果等待的进程太多，全部唤醒在进入睡眠这样的操作也是耗费资源的，会降低系统的性能。为了应对这种情况，内核中添加了
一个互斥等待的选项。这样的结果是，进行互斥等待的进程被一次唤醒一个。

互斥等待一般情况下用不到，所以不再关注。

这篇就暂时说到这里，下一篇继续看其他的一些高级字符驱动操作poll/select等。