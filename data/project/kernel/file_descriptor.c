

文件描述符

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


3.