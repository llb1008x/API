

代码结构要调整

一.android的休眠唤醒机制
1、背景介绍：
    睡眠/唤醒是嵌入式Linux非常重要的组成部分,因为优秀的睡眠唤醒机制可以是嵌入式设备尽可能的进入休眠状态，来延长电池的续航时间（这在移动终端消费类电子设备中是
    非常重要和有意义的！！）。但标准的Linux睡眠唤醒机制有其自身的一些缺陷（所有模块必须同时睡下或者唤醒），在某些情况下，这会导致能耗的白白浪费。因此Android
    在标准Linux睡眠唤醒的机制上作了新的改动(wake_lock唤醒、early_suspend和late_resume机制），从而很好的解决上面的问题。本文将以Android2.3.1版本为例，
    详细介绍标准Linux睡眠/唤醒是如何工作的, 并且Android中是如何把其自身特有的机制和Linux中标准的联系起来的。

2、标准Linux睡眠唤醒机制简介：
    在标准Linux中，休眠主要分三个主要的步骤：（1）冻结用户态进程和内核态任务；（2）调用注册的设备的suspend的回调函数，其调用顺序是按照驱动加载时的注册顺序。
    （3）休眠核心设备和使CPU进入休眠态冻结进程是内核把进程列表中所有的进程的状态都设置为停止,并且保存下所有进程的上下文。 当这些进程被解冻的时候,它们是不知道自己被冻结过的,
    只是简单的继续执行。
那么是如何让Linux进入休眠的呢？其实很简单，因为Android和kernel已经做了很多复杂的工作，所以用户只需可以通过读写sys文件/sys /power/state 就可以实现控制系统进入休眠。 
比如：  #echo  mem  > /sys/power/state  /////// 使系统进行睡眠

       #echo  on   > /sys/power/state////////////使系统从睡眠中唤醒过来

当然还有其它的状态操作，在下面的内容中将有介绍。

3、Android睡眠唤醒机制简介：

    Android在Linux内核原有的睡眠唤醒模块上基础上，主要增加了下面三个机制：

Wake _Lock 唤醒锁机制；

Early _Suspend 预挂起机制；

Late _Resume 迟唤醒机制；

其基本原理如下：当启动一个应用程序的时候，它都可以申请一个wake_lock唤醒锁，每当申请成功之后都会在内核中注册一下（通知系统内核，现在已经有锁被申请），当应用程序在某种情况
下释放wake_lock的时候，会注销之前所申请的wake_lock。特别要注意的是：只要是系统中有一个wake_lock的时候，系统此时都不能进行睡眠。但此时各个模块可以进行early_suspend。
当系统中所有的wake_lock都被释放之后，系统就会进入真正的kernel的睡眠状态。在系统启动的时候会创建一个主唤醒锁main_wake_lock，该锁是内核初始化并持有的一个WAKE_LOCK_SUSPEND
属性的非限时唤醒锁。因此，系统正常工作时，将始终因为该锁被内核持有而无法进入睡眠状态。也就是说在不添加新锁的情况下，只需将main_wake_lock 解锁，系统即可进入睡眠状态。    

    下面是Android睡眠唤醒模块框架
Android休眠唤醒机制简介（一）

    接下来我们将以上图的框架结构为主线，将进行非常非常详细地从最上层到最底层的跟踪！！！本文的主旨主要就是读者从Android最上层（Java写的应用程序）一步一步的往下跟进，
    经过Java、C++和C语言写的Framework层、JNI层、HAL层最后到达android的最底层（Kernel层）。通过本文的阅读，您将对android的整体有更加深入、宏观的理解和把握！

   主要涉及到的目录文件：
   {
        android/frameworks/base/core/java/android/os/PowerManager.java 

        android/frameworks/base/services/java/com/android/server/PowerManagerService.java

        android/frameworks/base/core/java/android/os/Power.java

        android/frameworks/base/core/jni/android_os_Power.cpp

        android/hardware/libhardware_legacy/power/power.c



        android/kernel/kernel/power/main.c 

        android/kernel/kernel/power/earlysuspend.c

        android/kernel/kernel/power/suspend.c

        android/kernel/kernel/power/wakelock.c

        android/kernel/kernel/power/userwakelock.c
   }


在应用程序框架层中，PowerManager类是面向上层应用程序的接口类，提供了Wake Lock机制（同时也是睡眠唤醒子系统）的基本接口（唤醒锁的获取和释放）。上层应用程序通过
调用这些接口，实现对系统电源状态的监控。PowerManager类通过IBinder这种Android中特有的通信模式，与PowerManagerService 类进行通信。PowerManagerService 
是PowerManager 类中定义的接口的具体实现，并进一步调用Power 类来与下一层进行通信。PowerManagerService 类是WakeLock 机制在应用程序框架层的核心，他们对应用
程调用PowerManager类接口时所传递的参数进行初步的分析和对应的设置，并管理一个唤醒锁队列，然后配合其他模块（例如WatchDog、BatteryService、ShutdownThread 等）
的状态信息，做出决策，调用Power类的对应接口，最终通过JNI 接口，调用到硬件抽象层中的函数，对sysfs 的用户接口进行操作，从而触发内核态实现的用。


 PowerManager.java：提供上层应用程序的接口；

 PowerManagerService.java：具体实现PowerManager类中的接口；

 Power.java：被PowerManagerService类调用；

android_os_Power.cpp：实现Power类中的JNI接口；

power.c：进行sysfs用户接口的操作。

其余涉及到的都是内核kernel中的文件，它们的作用将在后续给予介绍。


/**************************************************************************************************************************/
二.具体流程

    下面我将分别以两条路线（第一：获得wakelock唤醒锁。第二：系统进入睡眠。）来分别说明各自的流程，让读者对android睡眠唤醒机制有更深入的理解！

    第一部分：获得wakelock唤醒锁

    比如在应用程序中，当获得wakelock唤醒锁的时候，它首先是调用/android/frameworks/base/core/java/android/os/PowerManager类中的
    public void acquire()方法，而该方法通过android特有的通讯机制，会接着调用到PowerManagerService类中的public void acquireWakeLock。

public void acquireWakeLock(int flags, IBinder lock, String tag, WorkSource ws)
 {
        int uid = Binder.getCallingUid();
        int pid = Binder.getCallingPid();
        if (uid != Process.myUid()) {
            mContext.enforceCallingOrSelfPermission(android.Manifest.permission.WAKE_LOCK, null);
        }    
        if (ws != null) {
            enforceWakeSourcePermission(uid, pid);
        }    
        long ident = Binder.clearCallingIdentity();
        try {
            synchronized (mLocks) {
                acquireWakeLockLocked(flags, lock, uid, pid, tag, ws); 
            }    

        } finally {
            Binder.restoreCallingIdentity(ident);
        }    
}   
而 public void acquireWakeLock方法又调用了acquireWakeLockLocked。

public void acquireWakeLockLocked(int flags, IBinder lock, int uid, int pid, String tag,
            WorkSource ws)
 {
    if (mSpew) {
     Slog.d(TAG, "acquireWakeLock flags=0x" + Integer.toHexString(flags) + " tag=" + tag); }
        if (ws != null && ws.size() == 0) {ws = null;}
        int index = mLocks.getIndex(lock);
        WakeLock wl;
        boolean newlock;
        boolean diffsource;
        WorkSource oldsource;
      .
                 中间代码省略
      .   
          Power.acquireWakeLock(Power.PARTIAL_WAKE_LOCK,PARTIAL_NAME);
        }
        if (diffsource) {
            // If the lock sources have changed, need to first release the
            // old ones.
            noteStopWakeLocked(wl, oldsource);
        }
        if (newlock || diffsource) {
            noteStartWakeLocked(wl, ws);
        }
}

    我们可以看到在acquireWakeLockLocked 方法调用Power类中的public static native void acquireWakeLock(int lock, String id)方法。而该方法是调用
    android_os_Power.cpp中的static void acquireWakeLock（）函数。

static void acquireWakeLock(JNIEnv *env, jobject clazz, jint lock, jstring idObj)
{
    if (idObj == NULL) {
        throw_NullPointerException(env, "id is null");
        return ;
    }
    const char *id = env->GetStringUTFChars(idObj, NULL);
    acquire_wake_lock(lock, id);
    env->ReleaseStringUTFChars(idObj, id);
}

函数 acquire_wake_lock（）的实现在 power.c中，其定义如下：
int  acquire_wake_lock(int lock, const char* id)
{
    initialize_fds();
//    LOGI("acquire_wake_lock lock=%d id='%s'\n", lock, id);
    if (g_error) return g_error;
    int fd;
    if (lock == PARTIAL_WAKE_LOCK) {
        fd = g_fds[ACQUIRE_PARTIAL_WAKE_LOCK];
    }
    else {
        return EINVAL;
    }
    return write(fd, id, strlen(id));
}
    到现在为止，我们的代码流程已经走了一大半了，我们一开始介绍的android的上面几层Framework层、JNI层、HAL层都已经介绍了就剩下Kernel层了。下面就应该是和kernel层进行交互了。

    但是在android/hardware/libhardware_legacy/power/power.c中的acquire_wake_lock（）函数似乎没法和kernel层进行通信啊？？不急 要淡定！！在这个函数的最后不是还有
    一个返回语句return write(fd, id, strlen(id))嘛！！有人会说这句话看不出什么啊，我一开始用Source Insight代码阅读器跟踪的时候也没有找到它的原型，那个叫急啊!!呵呵 
    最后经过我的继续不断的努力查找（其实方法很简单，既然我从上往下的路断了，那我就换个方向，我最后又从下往上顺着代码走了一遍），终于被我发现了。

我们先看一下android/kernel/kernel/power/main.c中的一段代码，我将会做简单的分析，之后你就会明白刚才上面所产生的疑问了。

#ifdef CONFIG_USER_WAKELOCK
power_attr(wake_lock);
power_attr(wake_unlock);
#endif

static struct attribute * g[] = {
&state_attr.attr,
#ifdef CONFIG_PM_TRACE
&pm_trace_attr.attr,
#endif

#ifdef CONFIG_PM_SLEEP
&pm_async_attr.attr,
#ifdef CONFIG_PM_DEBUG
&pm_test_attr.attr,
#endif

#ifdef CONFIG_USER_WAKELOCK
&wake_lock_attr.attr,
&wake_unlock_attr.attr,
#endif

#endif
NULL,
};

static struct  attribute_group  attr_group = {
    .attrs = g,
};

#ifdef CONFIG_PM_RUNTIME
struct workqueue_struct *pm_wq;
EXPORT_SYMBOL_GPL(pm_wq);

static int __init pm_start_workqueue(void)
{
    pm_wq = create_freezeable_workqueue("pm");
    return pm_wq ? 0 : -ENOMEM;
}
#else
static inline int pm_start_workqueue(void) { return 0; }
#endif
static int __init pm_init(void)
{
    int error = pm_start_workqueue();
    if (error)
    return error;
    power_kobj = kobject_create_and_add("power", NULL);
    if (!power_kobj)
        return -ENOMEM;
    return sysfs_create_group(power_kobj, &attr_group);
}
core_initcall(pm_init);

    这段代码虽然简短，但看起来是不是还是比较费劲，没关系，我们倒过来看就比较清楚了。上面代码中的sysfs_create_group(power_kobj, &attr_group);的意思就是当我们
    在对sysfs/下相对的节点进行操作的时候会调用与attr_group里的相关函数，再往上面看其实就是指&wake_lock_attr.attr（对不同情况的操作会调用不同的attr_group，
    在第二条路的里面我们还会再次接触到这里）。power_attr(wake_lock)就是使具体的操作函数与其挂钩。我们现在来看一看这个挂钩过程是怎么实现的。

#define power_attr(_name) \
static struct kobj_attribute _name##_attr = {\
    .attr = { \
    .name = __stringify(_name),\
    .mode = 0644, \
    }, \

    .show = _name##_show,\
    .store = _name##_store,\
}

在该函数中##的作用通俗点讲就是“连接”的意思，比如power_attr(wake_lock)，
    static struct kobj_attribute  wake_lock_attr = {\
.attr = { \
.name = __stringify(wake_lock),\
.mode = 0644, \
}, \
.show = wake_lock_show, \
.store = wake_lock_store, \
}

    函数wake_lock_store和wake_lock_show就定义在android/kernel/kernel/power/userwakelock.c 
中。因此当我们对/sys/power/wake_lock进行操作的时候就会调用到userwakelock.c中定义的
wake_lock_store（）函数。
     好了，我们该回到原来我们产生疑问的地方了，在 power.c中我们将重新研究一下这这段代码，这时我们还得关注其中的另一个函数initialize_fds()。

int  acquire_wake_lock(int lock, const char* id)
{
    initialize_fds();
//    LOGI("acquire_wake_lock lock=%d id='%s'\n", lock, id);
    if (g_error) return g_error;
    int fd;
    if (lock == PARTIAL_WAKE_LOCK) {
        fd = g_fds[ACQUIRE_PARTIAL_WAKE_LOCK];
    }
    else {
        return EINVAL;
    }
 return write(fd, id, strlen(id));
}

initialize_fds(void)
{
    // XXX: should be this:
    //pthread_once(&g_initialized, open_file_descriptors);
    // XXX: not this:
    if (g_initialized == 0) {
        if(open_file_descriptors(NEW_PATHS) < 0) {
            open_file_descriptors(OLD_PATHS);
            on_state = "wake";
            off_state = "standby";
        }
        g_initialized = 1;
    }
}
    其实这个函数中最和新的步骤就是open_file_descriptors(NEW_PATHS) ；而
const char * const NEW_PATHS[] = {
    "/sys/power/wake_lock",
    "/sys/power/wake_unlock",
    "/sys/power/state"
};
    总之经过着一些列的步骤后，最终我们将在 return write(fd, id, strlen(id));时调用android/kernel/kernel/power/userwakelock.c 中的 wake_lock_store（）函数。

ssize_t  wake_lock_store(
        struct kobject *kobj, struct kobj_attribute *attr,
        const char *buf, size_t n)
{
        long timeout;
        struct user_wake_lock *l; 
        mutex_lock(&tree_lock);
        l = lookup_wake_lock_name(buf, 1, &timeout);
        if (IS_ERR(l)) {
                n = PTR_ERR(l);
                goto bad_name;
        }
if (debug_mask & DEBUG_ACCESS)
                pr_info("wake_lock_store: %s, timeout %ld\n", l->name, timeout);
        if (timeout)
                wake_lock_timeout(&l->wake_lock, timeout);
        else
                wake_lock(&l->wake_lock);
bad_name:
        mutex_unlock(&tree_lock);
        return n;
}
    该函数执行的基本流程为：首先调用lookup_wake_lock_name()来获得指定的唤醒锁，若延迟参数timeout为零的话，就调用 wake_lock（）否则就调用wake_lock_timeout（），
    但不管调用哪个最后都会调用到android/kernel/kernel/power/wakelock.c中的函数static void wake_lock_internal（）。

static void wake_lock_internal(struct wake_lock *lock, long timeout, int has_timeout)
{
    int type;
    unsigned long irqflags;
    long expire_in;

    spin_lock_irqsave(&list_lock, irqflags);
    type = lock->flags & WAKE_LOCK_TYPE_MASK;
    BUG_ON(type >= WAKE_LOCK_TYPE_COUNT);
    BUG_ON(!(lock->flags & WAKE_LOCK_INITIALIZED));
    #ifdef CONFIG_WAKELOCK_STAT
    if (type == WAKE_LOCK_SUSPEND && wait_for_wakeup) {
        if (debug_mask & DEBUG_WAKEUP)
        pr_info("wakeup wake lock: %s\n", lock->name);
        wait_for_wakeup = 0;
        lock->stat.wakeup_count++;
    }

    if ((lock->flags & WAKE_LOCK_AUTO_EXPIRE) &&
        (long)(lock->expires - jiffies) <= 0) {
    wake_unlock_stat_locked(lock, 0);
    lock->stat.last_time = ktime_get();
    }
    #endif

    if (!(lock->flags & WAKE_LOCK_ACTIVE)) {
        lock->flags |= WAKE_LOCK_ACTIVE;
        #ifdef CONFIG_WAKELOCK_STAT
        lock->stat.last_time = ktime_get();
        #endif
    }

    list_del(&lock->link);

    if (has_timeout) {
        if (debug_mask & DEBUG_WAKE_LOCK)
            pr_info("wake_lock: %s, type %d, timeout %ld.lu\n",
            lock->name, type, timeout / HZ,(timeout % HZ) * MSEC_PER_SEC / HZ);
            lock->expires = jiffies + timeout;
            lock->flags |= WAKE_LOCK_AUTO_EXPIRE;
            list_add_tail(&lock->link, &active_wake_locks[type]);
    } else {
    if (debug_mask & DEBUG_WAKE_LOCK)
        pr_info("wake_lock: %s, type %d\n", lock->name, type);
        lock->expires = LONG_MAX;
        lock->flags &= ~WAKE_LOCK_AUTO_EXPIRE;
        list_add(&lock->link, &active_wake_locks[type]);
    }

    if (type == WAKE_LOCK_SUSPEND) {
        current_event_num++;
    #ifdef CONFIG_WAKELOCK_STAT
    if (lock == &main_wake_lock)
        update_sleep_wait_stats_locked(1);
    else if (!wake_lock_active(&main_wake_lock))
        update_sleep_wait_stats_locked(0);
    #endif
    if (has_timeout)
        expire_in = has_wake_lock_locked(type);
    else
        expire_in = -1;

    if (expire_in > 0) {
        if (debug_mask & DEBUG_EXPIRE)
        pr_info("wake_lock: %s, start expire timer, "
        "%ld\n", lock->name, expire_in);
        mod_timer(&expire_timer, jiffies + expire_in);
    } else {
        if (del_timer(&expire_timer))
                if (debug_mask & DEBUG_EXPIRE)
                    pr_info("wake_lock: %s, stop expire timer\n",
            lock->name);
            if (expire_in == 0)
            queue_work(suspend_work_queue, &suspend_work);
        }
    }
    spin_unlock_irqrestore(&list_lock, irqflags);
 }

   到这里为止，我们走的第一条路就到目的地了，这个函数具体做了什么，在这里就不仔细分析了，大家可以自己再跟下或者上网查相关资料，理解这个函数不难。
 

   第二部分：系统进入睡眠

有了上面第一部分的学习，再看第二部分的话，会容易很多。假如现在我们按了PAD上的power睡眠键，经过一些列的事件处理后，它会调用到PowerManager类中的

  public void goToSleep(long time) 
   {   
        try {
            mService.goToSleep(time);
        } catch (RemoteException e) {
        }   
    }
而该函数会调用到PowerManagerService类中的public void goToSleep（）方法；
      public void goToSleep(long time)
      {
        goToSleepWithReason(time, WindowManagerPolicy.OFF_BECAUSE_OF_USER);
   }
     goToSleepWithReason（）会调用goToSleepLocked()方法，接着会调用setPowerState();而setPowerState()方法里会调用setScreenStateLocked（），setScreenStateLocked（）又会调用到Power类中的JNI接口setScreenState（）,其具体实现是在android_os_Power.cpp文件中； 
      static int setScreenState(JNIEnv *env, jobject clazz, jboolean on) 
     {
          return set_screen_state(on);
     }

 函数中return set_screen_state（）的实现是android/hardware/libhardware_legacy/power/power.c
    set_screen_state(int on)
{
    QEMU_FALLBACK(set_screen_state(on));
    LOGI("*** set_screen_state %d", on);
    initialize_fds();
    //LOGI("go_to_sleep eventTime=%lld now=%lld g_error=%s\n", eventTime,
    // systemTime(), strerror(g_error));
    if (g_error) return g_error;
    char buf[32];
    int len;
    if(on)
        len = snprintf(buf, sizeof(buf), "%s", on_state);

    else
        len = snprintf(buf, sizeof(buf), "%s", off_state);
    buf[sizeof(buf) - 1] = '\0';
    len = write(g_fds[REQUEST_STATE], buf, len);

    if(len < 0) {
        LOGE("Failed setting last user activity: g_error=%d\n", g_error);
    }
    return 0;

    看！！代码到这里是不是跟第一部分很相似？不错，如果接着往下分析的话，可以套用上面第一部分的分析思路，最终len = write(g_fds[REQUEST_STATE], buf, len);语句调用的是android//kernel/kernel/power/main.c中的set_screen_state( )；

当我们在sys/power/state（android/hardware/libhardware_legacy/power/power.c）进行读写操作的时候，（linux/kernel/power/main.c）中的state_store（）函数会被调用，在该函数中会分成两个分支：

static ssize_t state_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t n)
{
#ifdef CONFIG_SUSPEND
#ifdef CONFIG_EARLYSUSPEND
        suspend_state_t state = PM_SUSPEND_ON;
#else
        suspend_state_t state = PM_SUSPEND_STANDBY;
#endif
        const char * const *s;
#endif
        char *p;
        int len;
        int error = -EINVAL;
        p = memchr(buf, '\n', n);
        len = p ? p - buf : n;
        if (len == 4 && !strncmp(buf, "disk", len)) {
                error = hibernate();
  goto Exit;
        }
#ifdef CONFIG_SUSPEND
        for (s = &pm_states[state]; state < PM_SUSPEND_MAX; s++, state++) {
                if (*s && len == strlen(*s) && !strncmp(buf, *s, len))
                        break;
        }
        if (state < PM_SUSPEND_MAX && *s)
#ifdef CONFIG_EARLYSUSPEND
                if (state == PM_SUSPEND_ON || valid_state(state)) {
                        error = 0;
                        request_suspend_state(state);
                }
#else
                error = enter_state(state);
#endif
#endif
Exit:
        return error ? error : n;
}                                    
Android特有的earlysuspend： request_suspend_state(state)
Linux标准的suspend：       enter_state(state)

注意：如果CONFIG_EARLYSUSPEND宏开的话，kernel会先走earlysuspend，反之则直接走suspend；从这里开始就要分两个分支了，如果支持earlysuspend的话就进入 request_suspend_state(state)函数，如果不支持的话就进入标准Linux的enter_state(state)函数。、

这两个函数分别在两个文件中kernel/kernel/power/earlysuspend.c和suspend.c。现在再回过头来看的话，感觉整个android中睡眠唤醒机制还是很清晰的。这两个函数体里又做了什么，在这里就不再做具体分析，大家可以自己对照代码或者上网查资料，因为本文的主旨是带读者从最上层应用层一直到最底层kernel层，把整个android的睡眠唤醒机制给走通。
{
    PowerManager.java                         goToSleep( )
    PowerManagerService.java                 goToSleep（）
    PowerManagerService.java             goToSleepWithReason（）
    PowerManagerService.java                 setPowerState()
    PowerManagerService.java             SetScreenStateLocked ()
    Power.java                             setScreenState（）
    android_os_Power.cpp                   setScreenState（）
    power.c                                  set_screen_state( )
    main.c                                 state_store( )
}



/**************************************************************************************************************************/

三.标准linux休眠过程：

l power management notifiers are executed with PM_SUSPEND_PREPARE 

l tasks are frozen

l target system sleep state is announced to the platform-handling code

l devices are suspended 

l platform-specific global suspend preparation methods are executed 

l non-boot CPUs are taken off-line

l interrupts are disabled on the remaining (main) CPU 

l late suspend of devices is carried out (一般有一些BUS driver的动作进行)‏

l platform-specific global methods are invoked to put the system to sleep 

 

标准linux唤醒过程：

l  the main CPU is switched to the appropriate mode, if necessary

l early resume of devices is carried out (一般有

l platform-specific global resume preparation methods are invoked

l devices are woken up

l tasks are thawed 

l power management notifiers are executed with PM_POST_SUSPEND 

 

用户可以通过sys文件系统控制系统进入休眠一些BUS driver的动作进行)‏

l interrupts are enabled on the main CPU 

  l non-boot CPUs are enabled ：

 

查看系统支持的休眠方式：

#cat /sys/power/state 

常见有standby(suspend to RAM)、mem(suspend to RAM)和disk(suspend to disk)，只是standby耗电更多，返回到正常工作状态的时间更短。

通过 #echo mem > /sys/power/state  让系统进入休眠。

#echo on > /sys/power/state  使系统唤醒。

 

Android休眠与唤醒

android是在传统的linux内核电源管理设计的基础上，结合手机设计的实际需求而进化出的一套电源管理系统，其核心内容有：wakelock 、early_suspend与late_resume。

wakelock在Android的电源管理系统中扮演一个核心的角色。wakelock是一种锁的机制， 只要有人拿着这个锁，系统就无法进入休眠，可以被用户态程序和内核获得。这个锁可以是有超时的或者是没有超时的，超时的锁会在时间过去以后自动解锁。如果没有锁了或者超时了，内核就会启动休眠的那套机制来进入休眠。

当系统在启动完毕后，会自己去加一把名为“main“的锁，而当系统有意愿去睡眠时则会先去释放这把“main”锁，在android中，在early_suspend的最后一步会去释放“main”锁（wake_unlock: main）。释放完后则会去检查是否还有其他存在的锁，如果没有则直接进入睡眠过程。

它的缺点是，如果有某一应用获锁而不释放或者因一直在执行某种操作而没时间来释放的话，则会导致系统一直进入不了睡眠状态，功耗过大。

 

early_suspend：先与linux内核的睡眠过程被调用。一般在手机系统的设计中对背光的操作等采用此类方法，因为背光需要的能耗过大。当然此操作与late_resume是配套使用的。一些在内核中要预先进行处理的事件可以先注册上early_suspend函数，当系统要进入睡眠之前会首先调用这些注册的函数。

 

本文中，linux kernel版本为 linux-2.6.29，android版本为 android 2.1

与android休眠唤醒主要相关的文件主要有：
{
    l linux_source/kernel/power/main.c

    l linux_source/kernel/power/earlysuspend.c

    l linux_source/kernel/power/wakelock.c

    l linux_source/kernel/power/process.c

    l linux_source/driver/base/power/main.c

    l linux_source/arch/xxx/mach-xxx/pm.c或linux_source/arch/xxx/plat-xxx/pm.c
}


Android 休眠过程如下：

 

当用户读写/sys/power/state时，linux_source/kernel/power/main.c中的state_store()函数会被调用。其中，android的early_suspend会执行request_suspend_state(state); 而标准的linux休眠则执行error = enter_state(state);

static ssize_t state_store(struct kobject *kobj, struct kobj_attribute *attr,

   const char *buf, size_t n)

{

#ifdef CONFIG_SUSPEND

#ifdef CONFIG_EARLYSUSPEND

    suspend_state_t state = PM_SUSPEND_ON;

#else

    suspend_state_t state = PM_SUSPEND_STANDBY;

#endif

    const char * const *s;

#endif

    char *p;
    int len;
    int error = -EINVAL;

    p = memchr(buf, '\n', n);

    len = p ? p - buf : n;

    if (len == 4 && !strncmp(buf, "disk", len)) {

        error = hibernate();
        goto Exit;
    }

 

#ifdef CONFIG_SUSPEND

for (s = &pm_states[state]; state < PM_SUSPEND_MAX; s++, state++) {

    if (*s && len == strlen(*s) && !strncmp(buf, *s, len))

        break;
}

if (state < PM_SUSPEND_MAX && *s)

#ifdef CONFIG_EARLYSUSPEND

if (state == PM_SUSPEND_ON || valid_state(state)) {

error = 0;

request_suspend_state(state);

}

#else

error = enter_state(state);

#endif

#endif

 Exit:

return error ? error : n;

}

 

在request_suspend_state(state)函数中，会调用early_suspend_work的工作队列，从而进入early_suspend()函数中。

static DECLARE_WORK(early_suspend_work, early_suspend);

void request_suspend_state(suspend_state_t new_state)

{

unsigned long irqflags;

int old_sleep;

 

spin_lock_irqsave(&state_lock, irqflags);

old_sleep = state & SUSPEND_REQUESTED;

if (debug_mask & DEBUG_USER_STATE) {

struct timespec ts;

struct rtc_time tm;

getnstimeofday(&ts);

rtc_time_to_tm(ts.tv_sec, &tm);

pr_info("request_suspend_state: %s (%d->%d) at %lld "

"(%d-d-d d:d:d. lu UTC)\n",

new_state != PM_SUSPEND_ON ? "sleep" : "wakeup",

requested_suspend_state, new_state,

ktime_to_ns(ktime_get()),

tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,

tm.tm_hour, tm.tm_min, tm.tm_sec, ts.tv_nsec);

}

if (!old_sleep && new_state != PM_SUSPEND_ON) {

state |= SUSPEND_REQUESTED;

queue_work(suspend_work_queue, &early_suspend_work);

} else if (old_sleep && new_state == PM_SUSPEND_ON) {

state &= ~SUSPEND_REQUESTED;

wake_lock(&main_wake_lock);

queue_work(suspend_work_queue, &late_resume_work);

}

requested_suspend_state = new_state;

spin_unlock_irqrestore(&state_lock, irqflags);

}

 

在early_suspend()函数中，首先要判断当前请求的状态是否还是suspend，若不是，则直接退出了；若是，函数会调用已经注册的early_suspend的函数。然后同步文件系统，最后释放main_wake_lock。

static void early_suspend(struct work_struct *work)

{

struct early_suspend *pos;

unsigned long irqflags;

int abort = 0;

 

mutex_lock(&early_suspend_lock);

spin_lock_irqsave(&state_lock, irqflags);

if (state == SUSPEND_REQUESTED)

state |= SUSPENDED;

else

abort = 1;

spin_unlock_irqrestore(&state_lock, irqflags);

 

if (abort) {

if (debug_mask & DEBUG_SUSPEND)

pr_info("early_suspend: abort, state %d\n", state);

mutex_unlock(&early_suspend_lock);

goto abort;

}

 

if (debug_mask & DEBUG_SUSPEND)

pr_info("early_suspend: call handlers\n");

list_for_each_entry(pos, &early_suspend_handlers, link) {

if (pos->suspend != NULL)

pos->suspend(pos);

}

mutex_unlock(&early_suspend_lock);

 

if (debug_mask & DEBUG_SUSPEND)

pr_info("early_suspend: sync\n");

 

sys_sync();

abort:

spin_lock_irqsave(&state_lock, irqflags);

if (state == SUSPEND_REQUESTED_AND_SUSPENDED)

wake_unlock(&main_wake_lock);

spin_unlock_irqrestore(&state_lock, irqflags);

}

 


 

在wake_unlock()中，删除链表中wake_lock节点，判断当前是否存在wake_lock，若wake_lock的数目为0，则调用工作队列suspend_work，进入suspend状态。

static DECLARE_WORK(suspend_work, suspend);

void wake_unlock(struct wake_lock *lock)

{

int type;

unsigned long irqflags;

spin_lock_irqsave(&list_lock, irqflags);

type = lock->flags & WAKE_LOCK_TYPE_MASK;

#ifdef CONFIG_WAKELOCK_STAT

wake_unlock_stat_locked(lock, 0);

#endif

if (debug_mask & DEBUG_WAKE_LOCK)

pr_info("wake_unlock: %s\n", lock->name);

lock->flags &= ~(WAKE_LOCK_ACTIVE | WAKE_LOCK_AUTO_EXPIRE);

list_del(&lock->link);

list_add(&lock->link, &inactive_locks);

if (type == WAKE_LOCK_SUSPEND) {

long has_lock = has_wake_lock_locked(type);

if (has_lock > 0) {

if (debug_mask & DEBUG_EXPIRE)

pr_info("wake_unlock: %s, start expire timer, "

"%ld\n", lock->name, has_lock);

mod_timer(&expire_timer, jiffies + has_lock);

} else {

if (del_timer(&expire_timer))

if (debug_mask & DEBUG_EXPIRE)

pr_info("wake_unlock: %s, stop expire "

"timer\n", lock->name);

if (has_lock == 0)

queue_work(suspend_work_queue, &suspend_work);

}

if (lock == &main_wake_lock) {

if (debug_mask & DEBUG_SUSPEND)

print_active_locks(WAKE_LOCK_SUSPEND);

#ifdef CONFIG_WAKELOCK_STAT

update_sleep_wait_stats_locked(0);

#endif

}

}

spin_unlock_irqrestore(&list_lock, irqflags);

}

 

在suspend()函数中，先判断当前是否有wake_lock，若有，则退出；然后同步文件系统，最后调用pm_suspend()函数。

static void suspend(struct work_struct *work)

{

int ret;

int entry_event_num;

 

if (has_wake_lock(WAKE_LOCK_SUSPEND)) {

if (debug_mask & DEBUG_SUSPEND)

pr_info("suspend: abort suspend\n");

return;

}

 

entry_event_num = current_event_num;

sys_sync();

if (debug_mask & DEBUG_SUSPEND)

pr_info("suspend: enter suspend\n");

ret = pm_suspend(requested_suspend_state);

if (debug_mask & DEBUG_EXIT_SUSPEND) {

struct timespec ts;

struct rtc_time tm;

getnstimeofday(&ts);

rtc_time_to_tm(ts.tv_sec, &tm);

pr_info("suspend: exit suspend, ret = %d "

"(%d-d-d d:d:d. lu UTC)\n", ret,

tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,

tm.tm_hour, tm.tm_min, tm.tm_sec, ts.tv_nsec);

}

if (current_event_num == entry_event_num) {

if (debug_mask & DEBUG_SUSPEND)

pr_info("suspend: pm_suspend returned with no event\n");

wake_lock_timeout(&unknown_wakeup, HZ / 2);

}

}

 


在pm_suspend()函数中，enter_state()函数被调用，从而进入标准linux休眠过程。

int pm_suspend(suspend_state_t state)

{

if (state > PM_SUSPEND_ON && state <= PM_SUSPEND_MAX)

return enter_state(state);

return -EINVAL;

}

 

在enter_state()函数中，首先检查一些状态参数，再同步文件系统，然后调用suspend_prepare()来冻结进程，最后调用suspend_devices_and_enter()让外设进入休眠。

static int enter_state(suspend_state_t state)

{

int error;

 

if (!valid_state(state))

return -ENODEV;

 

if (!mutex_trylock(&pm_mutex))

return -EBUSY;

 

printk(KERN_INFO "PM: Syncing filesystems ... ");

sys_sync();

printk("done.\n");

 

pr_debug("PM: Preparing system for %s sleep\n", pm_states[state]);

error = suspend_prepare();

if (error)

goto Unlock;

 

if (suspend_test(TEST_FREEZER))

goto Finish;

 

pr_debug("PM: Entering %s sleep\n", pm_states[state]);

error = suspend_devices_and_enter(state);

 

 Finish:

pr_debug("PM: Finishing wakeup.\n");

suspend_finish();

 Unlock:

mutex_unlock(&pm_mutex);

return error;

}

 

在suspend_prepare()函数中，先通过pm_prepare_console();给suspend分配一个虚拟终端来输出信息，再广播一个系统进入suspend的通报，关闭用户态的helper进程，然后调用suspend_freeze_processes()来冻结进程，最后会尝试释放一些内存。

static int suspend_prepare(void)

{

int error;

unsigned int free_pages;

 

if (!suspend_ops || !suspend_ops->enter)

return -EPERM;

 

pm_prepare_console();

 

error = pm_notifier_call_chain(PM_SUSPEND_PREPARE);

if (error)

goto Finish;

 

error = usermodehelper_disable();

if (error)

goto Finish;

 

if (suspend_freeze_processes()) {

error = -EAGAIN;

goto Thaw;

}

 

free_pages = global_page_state(NR_FREE_PAGES);

if (free_pages < FREE_PAGE_NUMBER) {

pr_debug("PM: free some memory\n");

shrink_all_memory(FREE_PAGE_NUMBER - free_pages);

if (nr_free_pages() < FREE_PAGE_NUMBER) {

error = -ENOMEM;

printk(KERN_ERR "PM: No enough memory\n");

}

}

if (!error)

return 0;

 

 Thaw:

suspend_thaw_processes();

usermodehelper_enable();

 Finish:

pm_notifier_call_chain(PM_POST_SUSPEND);

pm_restore_console();

return error;

}




/***********************************************************************************************/
四.在suspend_freeze_processes()函数中调用了freeze_processes()函数，而freeze_processes()函数中又调用了try_to_freeze_tasks()来完成冻结任务。在冻结过程中，会判断当前进程是否有wake_lock，若有，则冻结失败，函数会放弃冻结。

static int try_to_freeze_tasks(bool sig_only)

{

struct task_struct *g, *p;

unsigned long end_time;

unsigned int todo;

struct timeval start, end;

u64 elapsed_csecs64;

unsigned int elapsed_csecs;

unsigned int wakeup = 0;

 

do_gettimeofday(&start);

 

end_time = jiffies + TIMEOUT;

do {

todo = 0;

read_lock(&tasklist_lock);

do_each_thread(g, p) {

if (frozen(p) || !freezeable(p))

continue;

 

if (!freeze_task(p, sig_only))

continue;

 

 

if (!task_is_stopped_or_traced(p) &&

    !freezer_should_skip(p))

todo++;

} while_each_thread(g, p);

read_unlock(&tasklist_lock);

yield();

if (todo && has_wake_lock(WAKE_LOCK_SUSPEND)) {

wakeup = 1;

break;

}

if (time_after(jiffies, end_time))

break;

} while (todo);

 

do_gettimeofday(&end);

elapsed_csecs64 = timeval_to_ns(&end) - timeval_to_ns(&start);

do_div(elapsed_csecs64, NSEC_PER_SEC / 100);

elapsed_csecs = elapsed_csecs64;

 

if (todo) {

 

if(wakeup) {

printk("\n");

printk(KERN_ERR "Freezing of %s aborted\n",

sig_only ? "user space " : "tasks ");

}

else {

printk("\n");

printk(KERN_ERR "Freezing of tasks failed after %d.d seconds "

"(%d tasks refusing to freeze):\n",

elapsed_csecs / 100, elapsed_csecs % 100, todo);

show_state();

}

read_lock(&tasklist_lock);

do_each_thread(g, p) {

task_lock(p);

if (freezing(p) && !freezer_should_skip(p))

printk(KERN_ERR " %s\n", p->comm);

cancel_freezing(p);

task_unlock(p);

} while_each_thread(g, p);

read_unlock(&tasklist_lock);

} else {

printk("(elapsed %d.d seconds) ", elapsed_csecs / 100,

elapsed_csecs % 100);

}

return todo ? -EBUSY : 0;

}

 

到现在，所有的进程(也包括workqueue/kthread) 都已经停止了，内核态进程有可能在停止的时候握有一些信号量，所以如果这时候在外设里面去解锁这个信号量有可能会发生死锁， 所以在外设suspend()函数里面作lock/unlock锁要非常小心，建议不要在外设的suspend()里面等待锁。而且suspend的过程中，有一些log是无法输出的，所以一旦出现问题，非常难调试。

 

回到enter_state()函数中，再冻结进程完成后，调用suspend_devices_and_enter()函数让外设进入休眠。该函数中，首先休眠串口（之后不能再显示log，解决方法为在kernel配置选项的cmd_line中，添加”no_console_suspend”选项），再通过device_suspend()函数调用各驱动的suspend函数。

当外设进入休眠后，suspend_ops->prepare()被调用，suspend_ops是板级的PM操作(本文中粉红色的函数，依赖于具体的平台)，以s3c6410为例，其注册在linux_source/arch/arm/plat-s3c64xx/pm.c中，只定义了suspend_ops->enter()函数。

static struct platform_suspend_ops s3c6410_pm_ops = {

.enter = s3c6410_pm_enter,

.valid = suspend_valid_only_mem,

};

接下来，多CPU中的非启动CPU被关闭。

int suspend_devices_and_enter(suspend_state_t state)

{

int error;

 

if (!suspend_ops)

return -ENOSYS;

 

if (suspend_ops->begin) {

error = suspend_ops->begin(state);

if (error)

goto Close;

}

suspend_console();

suspend_test_start();

error = device_suspend(PMSG_SUSPEND);

if (error) {

printk(KERN_ERR "PM: Some devices failed to suspend\n");

goto Recover_platform;

}

suspend_test_finish("suspend devices");

if (suspend_test(TEST_DEVICES))

goto Recover_platform;

 

if (suspend_ops->prepare) {

error = suspend_ops->prepare();

if (error)

goto Resume_devices;

}

 

if (suspend_test(TEST_PLATFORM))

goto Finish;

 

error = disable_nonboot_cpus();

if (!error && !suspend_test(TEST_CPUS))

suspend_enter(state);

 

enable_nonboot_cpus();

 Finish:

if (suspend_ops->finish)

suspend_ops->finish();

 Resume_devices:

suspend_test_start();

device_resume(PMSG_RESUME);

suspend_test_finish("resume devices");

resume_console();

 Close:

if (suspend_ops->end)

suspend_ops->end();

return error;

 

 Recover_platform:

if (suspend_ops->recover)

suspend_ops->recover();

goto Resume_devices;

}

 

接下来suspend_enter()被调用，该函数首先关闭IRQ，然后调用device_power_down(), 它会调用suspend_late()函数, 这个函数是系统真正进入休眠最后调用的函数, 通常会在这个函数中作最后的检查，接下来休眠所有的系统设备和总线。最后调用 suspend_pos->enter() 来使CPU进入省电状态。这时候，整个休眠过程完成，代码的执行也就停在这里了。

static int suspend_enter(suspend_state_t state)

{

int error = 0;

 

device_pm_lock();

#ifdef CONFIG_CPU_FREQ

cpufreq_get_cpufreq_name(0);

strcpy(governor_name, cpufreq_governor_name);

if(strnicmp(governor_name, userspace_governor, CPUFREQ_NAME_LEN)) {

cpufreq_set_policy(0, "performance");

}

#endif 

arch_suspend_disable_irqs();

BUG_ON(!irqs_disabled());

 

if ((error = device_power_down(PMSG_SUSPEND))) {

printk(KERN_ERR "PM: Some devices failed to power down\n");

goto Done;

}

 

error = sysdev_suspend(PMSG_SUSPEND);

if (!error) {

if (!suspend_test(TEST_CORE))

error = suspend_ops->enter(state);

sysdev_resume();

}

 

device_power_up(PMSG_RESUME);

 Done:

arch_suspend_enable_irqs();

#ifdef CONFIG_CPU_FREQ

if(strnicmp(governor_name, userspace_governor, CPUFREQ_NAME_LEN)) {

cpufreq_set_policy(0, governor_name);

}

#endif 

BUG_ON(irqs_disabled());

device_pm_unlock();

return error;

}

 

在suspend_pos->enter()所对应的函数中，代码最终停止在pm_cpu_sleep();处。

static int s3c6410_pm_enter(suspend_state_t state)

{

 ……

s3c6410_pm_do_save(gpio_save, ARRAY_SIZE(gpio_save));

s3c6410_pm_do_save(irq_save, ARRAY_SIZE(irq_save));

s3c6410_pm_do_save(core_save, ARRAY_SIZE(core_save));

s3c6410_pm_do_save(sromc_save, ARRAY_SIZE(sromc_save));

 

 

 

__raw_writel(__raw_readl(S3C_WAKEUP_STAT), S3C_WAKEUP_STAT);

 

 

#ifdef CONFIG_MACH_SMDK6410

 

__raw_writel(0xffffff00, S3C_NORMAL_CFG);

 

 

__raw_writel(0xffffffff, S3C_HCLK_GATE);

__raw_writel(0xffffffff, S3C_PCLK_GATE);

__raw_writel(0xffffffff, S3C_SCLK_GATE);

        ……

 

 

if (s3c6410_cpu_save(regs_save) == 0) {

flush_cache_all();

pm_cpu_sleep();

}

 

 

cpu_init();

 

__raw_writel(s3c_eint_mask_val, S3C_EINT_MASK);

 

 

s3c6410_pm_do_restore_core(core_save, ARRAY_SIZE(core_save));

s3c6410_pm_do_restore(sromc_save, ARRAY_SIZE(sromc_save));

……

}

/****************************************************************************************************/

五.static int suspend_enter(suspend_state_t state)

{

int error = 0;

 

device_pm_lock();

#ifdef CONFIG_CPU_FREQ

cpufreq_get_cpufreq_name(0);

strcpy(governor_name, cpufreq_governor_name);

if(strnicmp(governor_name, userspace_governor, CPUFREQ_NAME_LEN)) {

cpufreq_set_policy(0, "performance");

}

#endif 

arch_suspend_disable_irqs();

BUG_ON(!irqs_disabled());

 

if ((error = device_power_down(PMSG_SUSPEND))) {

printk(KERN_ERR "PM: Some devices failed to power down\n");

goto Done;

}

 

error = sysdev_suspend(PMSG_SUSPEND);

if (!error) {

if (!suspend_test(TEST_CORE))

error = suspend_ops->enter(state);  //suspend过程完成处

sysdev_resume();

}

 

device_power_up(PMSG_RESUME);

 Done:

arch_suspend_enable_irqs();

#ifdef CONFIG_CPU_FREQ

if(strnicmp(governor_name, userspace_governor, CPUFREQ_NAME_LEN)) {

cpufreq_set_policy(0, governor_name);

}

#endif 

BUG_ON(irqs_disabled());

device_pm_unlock();

return error;

}

 

然后回到suspend_devices_and_enter()函数中，使能休眠时候停止掉的非启动CPU，继续唤醒每个设备，使能终端。


int suspend_devices_and_enter(suspend_state_t state)

{

int error;

 

if (!suspend_ops)

return -ENOSYS;

 

if (suspend_ops->begin) {

error = suspend_ops->begin(state);

if (error)

goto Close;

}

suspend_console();

suspend_test_start();

error = device_suspend(PMSG_SUSPEND);

if (error) {

printk(KERN_ERR "PM: Some devices failed to suspend\n");

goto Recover_platform;

}

suspend_test_finish("suspend devices");

if (suspend_test(TEST_DEVICES))

goto Recover_platform;

 

if (suspend_ops->prepare) {

error = suspend_ops->prepare();

if (error)

goto Resume_devices;

}

 

if (suspend_test(TEST_PLATFORM))

goto Finish;

 

error = disable_nonboot_cpus();

if (!error && !suspend_test(TEST_CPUS))

suspend_enter(state);  //suspend过程完成处

 

enable_nonboot_cpus();

 Finish:

if (suspend_ops->finish)

suspend_ops->finish();

 Resume_devices:

suspend_test_start();

device_resume(PMSG_RESUME);

suspend_test_finish("resume devices");

resume_console();

 Close:

if (suspend_ops->end)

suspend_ops->end();

return error;

 

 Recover_platform:

if (suspend_ops->recover)

suspend_ops->recover();

goto Resume_devices;

}

 

当suspend_devices_and_enter()执行完成后，系统外设已经唤醒，但进程依然是冻结的状态，返回到enter_state函数中，调用suspend_finish()函数。

static int enter_state(suspend_state_t state)

{

int error;

 

if (!valid_state(state))

return -ENODEV;

 

if (!mutex_trylock(&pm_mutex))

return -EBUSY;

 

printk(KERN_INFO "PM: Syncing filesystems ... ");

sys_sync();

printk("done.\n");

 

pr_debug("PM: Preparing system for %s sleep\n", pm_states[state]);

error = suspend_prepare();

if (error)

goto Unlock;

 

if (suspend_test(TEST_FREEZER))

goto Finish;

 

pr_debug("PM: Entering %s sleep\n", pm_states[state]);

error = suspend_devices_and_enter(state);  //suspend过程完成处

 

 Finish:

pr_debug("PM: Finishing wakeup.\n");

suspend_finish();

 Unlock:

mutex_unlock(&pm_mutex);

return error;

}

 

在suspend_finish()函数中，解冻进程和任务，使能用户空间helper进程，广播一个系统从suspend状态退出的notify，唤醒终端。

static void suspend_finish(void)

{

suspend_thaw_processes();

usermodehelper_enable();

pm_notifier_call_chain(PM_POST_SUSPEND);

pm_restore_console();

}

 

当所有的唤醒已经结束以后，用户进程都已经开始运行了，但没点亮屏幕，唤醒通常会是以下的几种原因：

如果是来电，那么Modem会通过发送命令给rild来让rild通知WindowManager有来电响应，这样就会远程调用PowerManagerService来写”on”到 /sys/power/state 来调用late resume()，执行点亮屏幕等操作。

用户按键事件会送到WindowManager中，WindowManager会处理这些按键事件，按键分为几种情况，如果按键不是唤醒键，那么WindowManager会主动放弃wakeLock来使系统进入再次休眠；如果按键是唤醒键，那么WindowManger就会调用PowerManagerService中的接口来执行late Resume。

 

当”on”被写入到/sys/power/state之后，同early_suspend过程，request_suspend_state()被调用，只是执行的工作队列变为late_resume_work。在late_resume函数中，唤醒调用了early_suspend的设备。

static DECLARE_WORK(late_resume_work, late_resume);

static void late_resume(struct work_struct *work)

{

struct early_suspend *pos;

unsigned long irqflags;

int abort = 0;

 

mutex_lock(&early_suspend_lock);

spin_lock_irqsave(&state_lock, irqflags);

if (state == SUSPENDED)

state &= ~SUSPENDED;

else

abort = 1;

spin_unlock_irqrestore(&state_lock, irqflags);

 

if (abort) {

if (debug_mask & DEBUG_SUSPEND)

pr_info("late_resume: abort, state %d\n", state);

goto abort;

}

if (debug_mask & DEBUG_SUSPEND)

pr_info("late_resume: call handlers\n");

list_for_each_entry_reverse(pos, &early_suspend_handlers, link)

if (pos->resume != NULL)

pos->resume(pos);

if (debug_mask & DEBUG_SUSPEND)

pr_info("late_resume: done\n");

abort:

mutex_unlock(&early_suspend_lock);

}


/**********************************************************************************************/

六.关于wake_lock

在上文中，已经介绍了wakelock机制，下面从代码的角度进行介绍。

wakelock有2种类型，常用为WAKE_LOCK_SUSPEND，作用是防止系统进入睡眠。WAKE_LOCK_IDLE

这种锁不会影响到系统进入休眠，但是如果这种锁被持有，那么系统将无法进入idle空闲模式。

enum {

WAKE_LOCK_SUSPEND, 

WAKE_LOCK_IDLE,    

WAKE_LOCK_TYPE_COUNT

};

 

Wakelock有加锁和解锁2种操作，加锁有2种方式，第一种是永久加锁（wake_lock），这种锁必须手动的解锁；另一种是超时锁（wake_lock_timeout），这种锁在过去指定时间后，会自动解锁。

void wake_lock(struct wake_lock *lock)

{

wake_lock_internal(lock, 0, 0);

}

 

void wake_lock_timeout(struct wake_lock *lock, long timeout)

{

wake_lock_internal(lock, timeout, 1);

}

 

对于wakelock，timeout = has_timeout = 0；直接加锁后，然后退出；

static void wake_lock_internal(

struct wake_lock *lock, long timeout, int has_timeout)

{

int type;

unsigned long irqflags;

long expire_in;

 

spin_lock_irqsave(&list_lock, irqflags);

type = lock->flags & WAKE_LOCK_TYPE_MASK;

BUG_ON(type >= WAKE_LOCK_TYPE_COUNT);

BUG_ON(!(lock->flags & WAKE_LOCK_INITIALIZED));

#ifdef CONFIG_WAKELOCK_STAT

if (type == WAKE_LOCK_SUSPEND && wait_for_wakeup) {

if (debug_mask & DEBUG_WAKEUP)

pr_info("wakeup wake lock: %s\n", lock->name);

wait_for_wakeup = 0;

lock->stat.wakeup_count++;

}

if ((lock->flags & WAKE_LOCK_AUTO_EXPIRE) &&

    (long)(lock->expires - jiffies) <= 0) {

wake_unlock_stat_locked(lock, 0);

lock->stat.last_time = ktime_get();

}

#endif

if (!(lock->flags & WAKE_LOCK_ACTIVE)) {

lock->flags |= WAKE_LOCK_ACTIVE;

#ifdef CONFIG_WAKELOCK_STAT

lock->stat.last_time = ktime_get();

#endif

}

list_del(&lock->link);

if (has_timeout) {

if (debug_mask & DEBUG_WAKE_LOCK)

pr_info("wake_lock: %s, type %d, timeout %ld.lu\n",

lock->name, type, timeout / HZ,

(timeout % HZ) * MSEC_PER_SEC / HZ);

lock->expires = jiffies + timeout;

lock->flags |= WAKE_LOCK_AUTO_EXPIRE;

list_add_tail(&lock->link, &active_wake_locks[type]);

} else {

if (debug_mask & DEBUG_WAKE_LOCK)

pr_info("wake_lock: %s, type %d\n", lock->name, type);

lock->expires = LONG_MAX;

lock->flags &= ~WAKE_LOCK_AUTO_EXPIRE;

list_add(&lock->link, &active_wake_locks[type]);

}

if (type == WAKE_LOCK_SUSPEND) {

current_event_num++;

#ifdef CONFIG_WAKELOCK_STAT

if (lock == &main_wake_lock)

update_sleep_wait_stats_locked(1);

else if (!wake_lock_active(&main_wake_lock))

update_sleep_wait_stats_locked(0);

#endif

if (has_timeout)

expire_in = has_wake_lock_locked(type);

else

expire_in = -1;

if (expire_in > 0) {

if (debug_mask & DEBUG_EXPIRE)

pr_info("wake_lock: %s, start expire timer, "

"%ld\n", lock->name, expire_in);

mod_timer(&expire_timer, jiffies + expire_in);

} else {

if (del_timer(&expire_timer))

if (debug_mask & DEBUG_EXPIRE)

pr_info("wake_lock: %s, stop expire timer\n",

lock->name);

if (expire_in == 0)

queue_work(suspend_work_queue, &suspend_work);

}

}

spin_unlock_irqrestore(&list_lock, irqflags);

}

而对于wake_lock_timeout，在经过timeout时间后，才加锁。再判断当前持有wakelock时，启动另一个定时器，在expire_timer的回调函数中再次判断是否持有wakelock。

static void expire_wake_locks(unsigned long data)

{

long has_lock;

unsigned long irqflags;

if (debug_mask & DEBUG_EXPIRE)

pr_info("expire_wake_locks: start\n");

spin_lock_irqsave(&list_lock, irqflags);

if (debug_mask & DEBUG_SUSPEND)

print_active_locks(WAKE_LOCK_SUSPEND);

has_lock = has_wake_lock_locked(WAKE_LOCK_SUSPEND);

if (debug_mask & DEBUG_EXPIRE)

pr_info("expire_wake_locks: done, has_lock %ld\n", has_lock);

if (has_lock == 0)

queue_work(suspend_work_queue, &suspend_work);

spin_unlock_irqrestore(&list_lock, irqflags);

}

 

static DEFINE_TIMER(expire_timer, expire_wake_locks, 0, 0);

 

在wakelock中，有2个地方可以让系统从early_suspend进入suspend状态。分别是：

l 在wake_unlock中，解锁之后，若没有其他的wakelock，则进入suspend。

l 在超时锁的定时器超时后，定时器的回调函数，会判断有没有其他的wakelock，若没有，则进入suspend。





