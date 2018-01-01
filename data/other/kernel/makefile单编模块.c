

【背景】
    如果我们修改了 Android 系统中 kernel 目录下的代码，那么要将修改后的内容应用到设备上的话很简单，只编译 boot.img 并烧写 boot 分区就可以了，
    这不会花费很多时间。但如果我们修改的内容位于 frameworks/、devices/ 或者 hardware/ 等目录下的话，重新编译 system.img 和烧写 system 
    分区是非常花时间的。因此，我们需要寻求一种更便捷的方法，修改了哪里的代码就只编译相应模块，再将新的模块更新到设备上就可以了。Android 机制为我们提供了
     mm、mma 等工具来实现这个想法。


【操作实例】

    以我本次操作为例。我修改了 frameworks/av/services/audioflinger/ 目录下的 Threads.cpp 文件，查看该目录下的 Android.mk 文件可以知道
     Threads.cpp 影响的是 libaudioflinger.so 动态库，那么我们在编译模块后，就应该将 libaudioflinger.so 更新到设备上，再重启设备就 OK 了。

    需要注意的是，需要先在项目根目录下执行 source build/envsetup.sh 命令启用 mm 等工具后才能使用它们。另外，如果直接使用 mm 命令编译当前目录
    下的模块失败，可能是模块的依赖文件不存在，那么可以使用 mma 命令。mma 命令会编译当前目录下的模块及其依赖文件。

    编译生成的动态库在 out/target/product/xxxx/system/lib/ 目录下。使用以下命令更新设备上的库文件：

        adb  push  libaudioflinger.so  /system/lib/

    如果上方命令执行失败，提示被操作分区为 read-only 分区，那么我们需要在 root 权限下重新挂载 system 分区为可读写状态再重新 adb push 库文件到设备。命令如下：

        # mount  -o  remount,rw  /system