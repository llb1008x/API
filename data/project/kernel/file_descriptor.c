

1.概念


File descriptor

在linux OS里，所有都可以抽象成文件，比如普通的文件、目录、块设备、字符设备、socket、管道等等。

文件描述符（File descriptor）是linux里的一个概念，是文件的抽象化概念，详情请看：wiki链接

当通过一些系统调用（如open/socket等），会返回一个fd（其实就是数字）给你，然后根据这个fd对对应的文件进行操作，比如读、写。

分配fd

有些系统调用会分配fd给你用，比如open/socket等，而fd是数字，那么这个数字是怎么计算出来的？

其实内核为每个进程维护了一个数组，数组下标就是fd，里面存储的是对这个文件的描述了。

我们看下进程描述符，里面就有files指针，维护着所有打开的文件信息：

struct task_struct {
    volatile long state; /* -1 unrunnable, 0 runnable, >0 stopped */
    void *stack;
    atomic_t usage;
    unsigned int flags; /* per process flags, defined below */
    unsigned int ptrace;

    ......
    /* open file information */
    struct files_struct *files;
    ......
}

接着看files_struct结构体：

struct fdtable
{
    unsigned int max_fds;
    struct file __rcu **fd; /* current fd array */
    unsigned long *close_on_exec;
    unsigned long *open_fds;
    struct rcu_head rcu;
};

struct files_struct {
 /* read mostly part */
    atomic_t count;
    struct fdtable __rcu *fdt;
    struct fdtable fdtab;
    /* written part on a separate cache line in SMP */
    spinlock_t file_lock ____cacheline_aligned_in_smp;
    int next_fd;
    unsigned long close_on_exec_init[1];
    unsigned long open_fds_init[1];
    struct file __rcu * fd_array[NR_OPEN_DEFAULT];
};

fdtable里的fd就是一个数组，file结构体就是为打开文件的信息了。

分配fd的原则

从最小的空闲的数字分配。如果已打开了0、1、2、3、5，再打开fd，分配出去的是4，而不是6！

详情请查看函数：alloc_fd()，在kernel-3.18/fs/file.c

操作fd

可以通过read、write等函数操作fd

回收fd

用完fd后需要close关闭这个fd，那么这个fd对应的数字就被系统回收了，下一次的open将重新利用起来。

查看已打开的fd

没有进程都可以查看当前已打开的fd的详细信息。ls -l /proc/$pid/fd，看到如下信息：

lrwx------ root root 2015-03-20 15:47 0 -> /dev/null
lrwx------ root root 2015-03-20 15:47 1 -> /dev/null
l-wx------ root root 2015-03-20 15:47 10 -> /dev/cpuctl/tasks
lrwx------ root root 2015-03-20 15:47 100 -> /dev/ashmem
lrwx------ root root 2015-03-20 15:47 101 -> /dev/ashmem
lr-x------ root root 2015-03-20 15:47 102 -> /system/media/audio/ui/Dock.ogg
lrwx------ root root 2015-03-20 15:47 103 -> /dev/ashmem
lrwx------ root root 2015-03-20 15:47 104 -> /dev/ashmem
lr-x------ root root 2015-03-20 15:47 105 -> /system/media/

第1~3列是权限，第4~5列是时间，第6列是fd，第7列是对应打开的文件路径。我们通过这个可以清晰看到进程打开了哪些文件。

如果有db，db里的PROCESS_FILE_STATE就是通过ls -l /proc/$pid/fd获取的，也可以看这只文件。

阈值

linux默认对每个进程最大能打开的fd的个数是1024（软限制，硬限制是4096），你可以通过/proc/$pid/limits查看Max open files：

当打开的文件数超过这个值将无法再打开文件了。

如果进程确实需要同时打开更多文件，可以在程序调用setrlimit()控制：

struct rlimit r;

getrlimit(RLIMIT_NOFILE, &r);
r.rlim_cur = 2000; /* 阈值设定到2000 */
setrlimit(RLIMIT_NOFILE, &r);

注意：r.rlim_cur只能在0~r.rlim_max范围。


2.可能的问题



fd泄漏

fd的分配不可能是无限的，毕竟数组不可能无限大。如果程序没有写好，不停的open，总有一次会open失败的。

open失败会返回-1，同时errno被设置为24，具体含义在errno-base.h里：是EMFILE，表示too many open files。

检测泄漏

1. 有些代码会将open失败返回值打印出来，比如CursorWindow fd leak（从APLog_xxx/main_log可以看到）：

E CursorWindow: Could not create CursorWindow from Parcel due to error -24.

kernel系统调用失败返回值都是负值，-24解读为24，也就是too many open files。

另外MTK在kernel分配fd释放失败的位置添加了代码：

kernel-3.18/fs/file.c

int __alloc_fd(struct files_struct *files, unsigned start, unsigned end, unsigned flags)
{
    ......
out:
    spin_unlock(&files->file_lock);
#ifdef FD_OVER_CHECK
    if (error == -EMFILE && !dump_current_open_files) {
        dump_current_open_files = 0x1;
        pr_err("[FDLEAK][%d:%s]fd over RLIMIT_NOFILE:%ld\n",
        current->pid, current->comm, rlimit(RLIMIT_NOFILE));
        fd_show_open_files(current->pid, files, fdt);
    }
#endif
    return error;
}

因此可以从APLog_xxx/kernel_log看到（搜索OverAllocFDError关键字）：

也可以直接ls -l /proc/$pid/fd查看。

2. binder IMemory dup失败：

3. 在Looper里无法打开管道：

07-19 11:20:43.102 F/Looper  (200): Could not create wake pipe. errno=24

fd被意外关闭

如果程序没写好，很可能用已关闭的fd再去读写等操作，这个操作将会失败，同时errno被设置为9，具体含义在errno-base.h里：是EBADF，表示bad file number。

这种问题通常比较难调试。


3.调试


naive fd

fd泄漏

原理

对所有打开的fd记录调用栈，然后根据统计相同的调用栈次数，按从大到小排行，最大重复次数的调用栈就是泄漏点。

MTK有这样一套调试机制，请查考：[FAQ11422]如何开启fdleak调试功能

启动fd调试机制

该机制在L以及之后的版本是通过libudf.so实现的(KK及之前的版本是通过libc_fdleak_debug_mtk.so实现的)，因此要用这个机制之前，必须将libudf.so加载到要调试的进程。如何加载呢？我们是通过LD_PRELOAD将libudf.so加载到所有进程的。修改方法如下：

    首先检查下手机里LD_PRELOAD环境变量内容（毕竟我们是往里加，不是替换），用adb shell echo $LD_PRELOAD就可以查看了。
    如果存在libsigchain.so，那么libudf.so必须放在libsigchain.so之后。
    如果不存在libsigchain.so，那么libudf.so必须放在最前面。
    修改的位置在vendor\mediatek\proprietary\external\aee\config_external\init.aee.customer.rc

根据上面的方法，例子如下：

存在libsigchain.so，那么添加的内容是：

on init
    export LD_PRELOAD libsigchain.so:libudf.so:...

不存在libsigchain.so，那么添加的内容是：

on init
    export LD_PRELOAD libudf.so:...

"..."是原先LD_PRELOAD的内容，如果原先LD_PRELOAD没有内容，则":..."就不需要了。

检查fd调试机制

编译下载后，需要检查是否成功启动fd调试机制，可以通过查看maps查找是否存在libudf.so来判断：

adb shell cat /proc/$pid/maps|grep "libudf.so"

$pid为你要调试的进程的pid，如果maps存在libudf.so，则表示正确启动了，否则要检查上面的流程了。

打开fd调试机制

打开该机制的方法：

setprop persist.debug.fdleak 1

通常我们只需要调试某个进程，因此可以设定只调试某个进程（名称为binary文件名）：

setprop persist.debug.fdleak.program mediaserver

如果是app，则对应的binary文件名是app_process

setprop persist.debug.fdleak.program app_process

并不是所有打开的fd都记录调用栈，默认只记录256~1000的fd的调用栈，上限1000无法改动，下限256可以调整，调整方法如下（下面例子调整为30）：

setprop persist.debug.fdleak.thd 30

不过，通常没有特别需求的话，不用设定这个thd的。

复现问题

设定完上面的property后，需要重启才能生效！

注意：需要打开coredump才能分析问题。

重启后复现问题。如果打开的fd超过1000，程序将abort，这时将会产生db，里面包含PROCESS_COREDUMP。我们需要分析这个coredump才能提取出调用栈信息。

db里面的__exp_main.txt如果看到如下调用栈（由fdleak_record_backtrace调用了abort()），则表示fd泄漏引起的abort了：

分析问题

抓到的mtklog用E-Consulter（MOL工具栏里下载）分析，分析完coredump会提供分析报告，将会提示可能的泄漏点，例子如下：

分析报告会列出前3个打开fd最多的调用栈（第1个明显存在泄漏，打开了299次没有关闭）：

== 文件句柄检查 ==
文件句柄已分配超过512个(可能存在fd泄露)，以下列出分配最多次数的调用栈:
已分配: 229次
分配调用栈:
 libc.so pipe() + 66 <bionic/libc/bionic/fdleak_debug_func.c:37>
 libutils.so android::Looper::Looper() + 104 <system/core/libutils/Looper.cpp:73>
 libandroid_runtime.so android::NativeMessageQueue::NativeMessageQueue() + 74 <frameworks/base/core/jni/android_os_MessageQueue.cpp:70>
 libandroid_runtime.so android::android_os_MessageQueue_nativeInit() + 10 <frameworks/base/core/jni/android_os_MessageQueue.cpp:118>
 libdvm.so dvmPlatformInvoke() + 110 <mediatek/protect/dalvik/vm/arch/arm/CallEABI.S:256>
 libdvm.so dvmCallJNIMethod() + 482 <mediatek/protect/dalvik/vm/Jni.cpp:1238>
 libdvm.so dalvik_mterp() + 18 <mediatek/protect/dalvik/vm/mterp/out/InterpAsm-armv7-a-neon.S:17294>
 == 栈结束 ==
已分配: 228次
分配调用栈:
 libc.so pipe() + 54 <bionic/libc/bionic/fdleak_debug_func.c:36>
 libutils.so android::Looper::Looper() + 104 <system/core/libutils/Looper.cpp:73>
 libandroid_runtime.so android::NativeMessageQueue::NativeMessageQueue() + 74 <frameworks/base/core/jni/android_os_MessageQueue.cpp:70>
 libandroid_runtime.so android::android_os_MessageQueue_nativeInit() + 10 <frameworks/base/core/jni/android_os_MessageQueue.cpp:118>
 libdvm.so dvmPlatformInvoke() + 110 <mediatek/protect/dalvik/vm/arch/arm/CallEABI.S:256>
 libdvm.so dvmCallJNIMethod() + 482 <mediatek/protect/dalvik/vm/Jni.cpp:1238>
 libdvm.so dalvik_mterp() + 18 <mediatek/protect/dalvik/vm/mterp/out/InterpAsm-armv7-a-neon.S:17294>
 == 栈结束 ==
已分配: 57次
分配调用栈:
 libc.so socket() + 38 <bionic/libc/bionic/fdleak_debug_func.c:59>
 bluetooth.blueangel.so bt_hid_init_socket() + 103
 bluetooth.blueangel.so 0x61DF0AEC() + 44
 libbluetooth_jni.so android::initializeNative() + 130 <packages/apps/Bluetooth/jni/com_android_bluetooth_hid.cpp:206>
 libdvm.so dvmPlatformInvoke() + 110 <mediatek/protect/dalvik/vm/arch/arm/CallEABI.S:256>
 libdvm.so dvmCallJNIMethod() + 482 <mediatek/protect/dalvik/vm/Jni.cpp:1238>
 libdvm.so dalvik_mterp() + 18 <mediatek/protect/dalvik/vm/mterp/out/InterpAsm-armv7-a-neon.S:17294>
 == 栈结束 ==

剩下就是检查代码逻辑，修复问题了。

fd被意外关闭

这种情况通常是代码逻辑问题，在close之后又有可能拿这个fd去读写，linux就会报错了。

这个要如何调试呢？简单的方法是将获取和关闭fd的调用栈全部打印出来，等操作出问题后，再从log找最近一次操作该fd的调用栈，看看前面是否提前意外关闭了fd。

前面检查泄漏的机制是不适合调试这个问题了，但我们可以借用这个机制，只不过需要将每次获取和关闭fd的调用栈打印到log就好了。因此需要额外的property设定。

setprop persist.debug.fdleak.bt2log 1

设定完同样要重启一次才生效。

重启后调试进程在分配、关闭fd时就有对应的log吐出，样子如下：

分配fd 44时的调用栈：

关闭fd 40时的调用栈：

问题触发

通常这种问题不会导致进程崩溃，因此为了调试问题，可以在操作fd（比如read()/write()）返回异常后主动调用abort()使进程崩溃，抓取mtklog+db分析。

解析调用栈

方法1

由于只有地址，因此结合/proc/$pid/maps算出lib内的偏移量，然后再通过addr2line获取所在的文件名、行号等信息。

例子：

比如maps如下：

b6ec8000-b6ed9000 r-xp 00000000 b3:16 1329       /system/lib/libc.so

那么第1个free fd 40的调用栈地址0xb6ec86dc - 0xb6ec800 = 0x6dc

然后用arm-linux-androideabi-addr2line -Cfe libc.so 0x6dc就可以获取相应的信息了。具体可参考：

    [FAQ14333]异常发生后如何将log里对应的地址转换为所在的文件和行号？

方法2

用gdb加载PROCESS_COREDUMP和symbols，然后用命令：

list *$address

转换，$address为调用栈地址。



kernel fd
native那套机制无法调试kernel fd问题。

只能看OverAllocFDError输出的fd list来调试了，不过通常这种问题基本没有。


4.快速分析用户层 fd leaks
(1). 确定是哪类文件打开太多，没有关闭.
   *  fd leaks, 通常伴随着此进程会出现Java Exception, Native Exception 等. 在MTK 的AEE DB 中, 有一支文件 PROCESS_FILE_STATE 描述, 此进程的打开的所有文件.
查看此文件, 确定哪个或者哪种文件打开数量最多，即追查此类文件打开如此多, 而没有被关闭的原因.
    
   *  如果没有DB, 当发生文件句柄泄露到1024 时, 在L 版本后, 在Kernel Log 中search "FDLEAK",   在L 版本之前, 在Kernel Log 中search "FS_TAG", 即可枚举出所有的此进程所打开的文件.
    
   *  如果问题容易复现，可以直接 adb shell ls -a -l /proc/pid/fd , 直接打印出当前此process 所有打开的文件.
    
(2). 确定此类文件是在哪里打开.
    对于一些确定的文件, 比如/data/data/xxxx_app/yyyy 之类的文件, 通常开发者自己可以快速的确定打开文件的位置，基本上不需要debug.
    对于一些另外一些常见的场景说明如下：
    * 大批量的打开“anon_inode:[eventpoll]” 和 "pipe" 或者 "anon_inode:[eventfd]",  超过100个eventpoll, 通常情况下是开启了太多的HandlerThread/Looper/MessageQueue, 线程忘记关闭, 或者looper 没有释放. 可以抓取hprof 进行快速分析. 抓取hprof 可以参考FAQ:
         http://online.mediatek.com/Pages/FAQ.aspx?List=SW&FAQID=FAQ08893
    * 对于system server, 如果有大批量的socket 打开, 可能是因为Input Channel 没有关闭, 此类同样抓取hprof, 查看system server 中WindowState 的情况.
    * 大批量的打开“/dev/ashmem”, 如果是Context provider, 或者其他app, 很可能是打开数据库没有关闭, 或者数据库链接频繁打开忘记关闭. 这个时候查看这个进程的maps, cat proc/pid/maps, 即可看到这个ashmem 的name, 然后进一步可知道在哪里泄露.
    
(3). 暴力确定文件打开的位置
    MTK 有开发了fd leaks debug 功能，可以记录每次打开fd 的backtrace, 可以参考FAQ：   http://online.mediatek.com/Pages/FAQ.aspx?List=SW&FAQID=FAQ11422
    
(4). 修正
    确认为何要频繁打开, 或者为何打开后忘记关闭, ^_^,  然后更新.



5.system_server fd泄漏引起NE

基本信息

    问题：终端使用Volume+/-调节音量后system_server发生NE

    版本：L版本

    平台：MT6735

分析过程

    这个问题已经按native fd泄漏调试方法打开了fd调试机制。因此拿到db（db.fatal.00.NE），用E-Consulter分析，加入symbols目录后，得出分析报告如下：

== 异常报告v1.7(仅供参考) ==
详细描述: 程序主动调用abort()
版本 : L0/eng build
发生时间: Fri Jan 1 09:39:54 CST 2010
命令行 : system_server
进程标识符(pid): 778, 父进程标识符(ppid): 366 (zygote64)
进程状态: 不可中断睡眠
优先级 : 118 (0~99: 实时进程, 100~139: 普通进程)


== 线程信息(共99个线程) ==
当前线程信息:
 线程名: AudioProfileService, 可中断睡眠, 线程标识符(tid): 993
 errno: 24
 线程状态: 在调用JNI方法
 Java调用栈:
 ...... android.database.sqlite.SQLiteConnection.nativeExecuteForLastInsertedRowId() <本地方法>
 system@framework@boot.oat android.database.sqlite.SQLiteConnection.executeForLastInsertedRowId(this=0x00000000131FFA00, sql=0x0000000013260F00, bindArgs=0x0000000013263220, cancellationSignal=0) 
 system@framework@boot.oat android.database.sqlite.SQLiteSession.executeForLastInsertedRowId(this=0x00000000137017C0, sql=0x0000000013260F00, bindArgs=0x0000000013263220, connectionFlags=2, cancellationSignal=0) 
 system@framework@boot.oat android.database.sqlite.SQLiteStatement.executeInsert(this=0x0000000013379580) 
 system@framework@boot.oat android.database.sqlite.SQLiteDatabase.insertWithOnConflict(this=0x0000000012EA7AF0, table=0x0000000013045240, nullColumnHack=0, initialValues=0x00000000133D0700, conflictAlgorithm=0) 
 system@framework@boot.oat android.database.sqlite.SQLiteDatabase.insert(this=0x0000000012EA7AF0, table=0x0000000013045240, nullColumnHack=0, values=0x00000000133D0700) 
 system@priv-app@SettingsProvider@SettingsProvider.apk@classes.dex com.android.providers.settings.SettingsProvider.insertForUser(this=0x0000000012F97290, url=0x00000000716CD418, initialValues=0x00000000133D0700, desiredUserHandle=0) 
 system@priv-app@SettingsProvider@SettingsProvider.apk@classes.dex com.android.providers.settings.SettingsProvider.call(this=0x0000000012F97290, method=0x0000000071698060, request=0x0000000013231BC0, args=0x0000000013257CE0) 
 system@framework@boot.oat android.content.ContentProvider$Transport.call(this=0x0000000012EA7550, callingPkg=0x000000007168E178, method=0x0000000071698060, arg=0x0000000013231BC0, extras=0x0000000013257CE0) 
 system@framework@boot.oat android.provider.Settings$NameValueCache.putStringForUser(this=0x00000000716CD688, cr=0x0000000012C89A20, name=0x0000000013231BC0, value=0x00000000717E4A90, userHandle=0) 
 system@framework@boot.oat android.provider.Settings$System.putStringForUser(this=0x0000000012C89A20, resolver=0x0000000013231BC0, name=0x00000000717E4A90, value=0, userHandle=1902958216) 
 system@framework@boot.oat android.provider.Settings$System.putIntForUser(this=0x0000000012C89A20, cr=0x0000000013231BC0, name=11, value=0, userHandle=1904102032) 
 system@framework@boot.oat android.provider.Settings$System.putInt(this=0x0000000012C89A20, cr=0x0000000013231BC0, name=11, value=0) 
 system@framework@services.jar@classes.dex com.mediatek.audioprofile.AudioProfileService$AudioProfileHandler.handleMessage(this=0x0000000012E2CD40, msg=0x00000000137DF340) 
 system@framework@boot.oat android.os.Handler.dispatchMessage(this=0x0000000012E2CD40, msg=0x00000000137DF340) 
 system@framework@boot.oat android.os.Looper.loop(this=0) 
 system@framework@services.jar@classes.dex com.mediatek.audioprofile.AudioProfileService$OverrideSystemThread.run(this=0x0000000013138920) 
 == 栈结束 ==
 本地调用栈:
 libc.so tgkill(参数2=993, 参数3=6) + 8
 libc.so pthread_kill(参数2=6) + 160
 libc.so raise(参数1=6) + 28
 libc.so abort() + 60
 libudf.so fdleak_record_backtrace(参数1=1023) + 300
 libudf.so open(参数1=0x0000007F7EAD7CF0, 参数2=0x0000000000080000, 参数3=384, 参数4=0x0000007F94DB7150()) + 168
 libsqlite.so 0x0000007F94DD4AC4(参数1=0x0000007F7EAD7CF0, 参数2=0, 参数7=1) + 96
 libsqlite.so 0x0000007F94DD4C88(参数1=0x000000559FFDD4C9, 参数2=0x0000007F7EAD7F2C, 参数3=0x0000007F94E6A000) + 156
 libsqlite.so 0x0000007F94DD56F4(参数2=2) + 64
 libsqlite.so 0x0000007F94DC92B8(参数1=0x000000559FFDD1C0, 参数2=0) + 440
 libsqlite.so 0x0000007F94DED428(参数2=0) + 576
 libsqlite.so 0x0000007F94DED7E4() + 180
 libsqlite.so 0x0000007F94DF073C() + 2152
 libsqlite.so 0x0000007F94E07758(参数1=0x00000055A05DE470) + 30576
 libsqlite.so sqlite3_step() + 648
 libandroid_runtime.so 0x0000007F97CBC724(参数2=0x00000055A001C6C0) + 24
 libandroid_runtime.so 0x0000007F97CBC7D8(参数1=0x00000055A00E9CB0, 参数2=0x0000007F7EAD8540, 参数3=0x00000055A001C6C0, 参数4=0x00000055A05DE470, 参数5=0x6FA387F000000001, 参数6=0x00000055A00F3600, 参数7=0x0000007F7EAD8500, 参数8=0x0000007F97CBC7F4) + 24
 libart.so art_quick_generic_jni_trampoline(参数2=0x00000055A001C6C0, 参数3=0x00000055A05DE470, 参数4=0x00000055A00F3600, 参数5=0x0000007F7EAD8B18, 参数6=0x00000000709C675D, 参数7=__stack_chk_guard, 参数8=0x0000007F7E9DF000) + 140
 ......
 == 栈结束 ==
 对应汇编指令:
 行号 地址 指令 提示
 0000007F97EC57C4: CMN X0, #0x1000 ; tgkill()参数 1可能有问题
 当时的寄存器值:
 X0: 0000000000000000, X1: 00000000000003E1, X2: 0000000000000006, X3: 0000007F7EADF000
 X4: 0000007F7EADF000, X5: 0000000000000005, X6: 0000000000000001, X7: 0000000000000020
 X8: 0000000000000083, X9: FEFEFF7E7D9CEEFF, X10: 7F7F7F7F7FFFFF7F, X11: 0000000000000001
 X12: 0000000000000001, X13: 0000007F7EAD748C, X14: 0000000000000001, X15: 000000000000000F
 X16: 0000007F97F017C0, X17: 0000007F97EC57BC, X18: 0000000000000000, X19: 0000007F7EADF000
 X20: 0000007F7EADEBB0, X21: 0000007F97F07000, X22: 0000000000000018, X23: 0000000000000006
 X24: 0000007F94E39CC8, X25: 0000007F94E39C98, X26: 0000007F94E6A000, X27: 0000000000080000
 X28: 0000000000000000, X29: 0000007F7EAD7970, X30: 0000007F97E832AC, SP: 0000007F7EAD7970
 PC: 0000007F97EC57C4


== C堆检查 ==
分配器: dlmalloc, 最多允许使用: 16EB, 最多使用: 14MB, 当前使用: 13MB, 泄露阈值: 128MB, 调试等级: 15

正常


== 文件句柄检查 ==
文件句柄已分配超过512个(可能存在fd泄露)，以下列出分配最多次数的调用栈:
已分配: 714次
分配调用栈:
 libudf.so open() + 166
 libnetd_client.so (anonymous namespace)::netdClientConnect() + 90   <system/netd/client/NetdClient.cpp:83>
 libc.so connect() + 14
 libc.so __system_property_set() + 262
 libandroid_runtime.so android::SystemProperties_set() + 98
 libart.so art_quick_generic_jni_trampoline() + 138
 libart.so 0x0000007F9276CDB0() + 729674
 == 栈结束 ==

已分配: 12次
分配调用栈:
 libudf.so socketpair() + 46
 libinput.so android::InputChannel::openInputChannelPair() + 50
 libandroid_runtime.so 0x0000007F97CC27F4() + 122
 libart.so art_quick_generic_jni_trampoline() + 138
 system@framework@boot.oat 0x000000006FFEA075()
 == 栈结束 ==

已分配: 3次
分配调用栈:
 libudf.so open() + 166
 libandroidfw.so OpenArchive() + 30
 libandroidfw.so android::ZipFileRO::open() + 18
 libandroidfw.so android::AssetManager::SharedZip::SharedZip() + 106
 libandroidfw.so android::AssetManager::SharedZip::get() + 238
 libandroidfw.so android::AssetManager::ZipSet::getZip() + 134
 libandroidfw.so android::AssetManager::openNonAssetInPathLocked() + 70
 libandroidfw.so android::AssetManager::addAssetPath() + 338
 libandroid_runtime.so 0x0000007F97CDD760() + 110
 libart.so art_quick_generic_jni_trampoline() + 138
 == 栈结束 ==

    从当前线程信息的本地调用栈来看，libudf.so发出的abort()，肯定是发生fd leak了。查看文件句柄检查，发现排行第1的调用栈，分配了714次的fd，这里泄漏了。

    一一检查调用栈里的函数，open()函数肯定不会有问题，接着检查下netdClientConnect()函数：

int netdClientConnect(int sockfd, const sockaddr* addr, socklen_t addrlen)
{
{
    char path[32];
    char line[32];
    int fd;

    snprintf(path, sizeof(path), "/proc/self/status");
    fd = open(path, O_RDONLY);
    if (fd > 0){
        read(fd,line,sizeof(line));
        line[31] = '\0';
        //format: Name: process_name
        if (strstr(&(line[6]),"ABenchMark") != NULL){
            close(fd);
            return -1;
        }
    }
}
    if (sockfd >= 0 && addr && FwmarkClient::shouldSetFwmark(addr->sa_family)) {
        FwmarkCommand command = {FwmarkCommand::ON_CONNECT, 0, 0};
        if (int error = FwmarkClient().send(&command, sizeof(command), sockfd)) {
            errno = -error;
            return -1;
        }
    }
    return libcConnect(sockfd, addr, addrlen);
}

    检查代码，发现只有匹配的才会close(fd)，不匹配的没有close(fd)，存在fd泄漏了。

解决方法

保证所有路径都可以把打开的fd关闭，修改如下：

int netdClientConnect(int sockfd, const sockaddr* addr, socklen_t addrlen)
{
{
    char path[32];
    char line[32];
    int fd;

    snprintf(path, sizeof(path), "/proc/self/status");
    fd = open(path, O_RDONLY);
    if (fd >= 0){
        read(fd,line,sizeof(line));
        line[31] = '\0';
        //format: Name: process_name
        if (strstr(&(line[6]),"ABenchMark") != NULL){
            close(fd);
            return -1;
        }
       close(fd);
    }
}

结语

    用fd调试机制，非常容易看出问题点。熟悉使用E-Consulter，将快速给出分析结果，提高效率。
    



