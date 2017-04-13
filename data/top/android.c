/*************************************************************
*android操作系统学习
*************************************************************/


1-3：设备上电起来后，跳转到Boot ROM(不是flash)中的boot code中执行把pre-loader加载起到ISRAM， 因为当前DRAM（RAM分SRAM跟DRAM，简单来说SRAM就是cache，
DRAM就是普通内存）还没有准备好，所以要先把pre-loader load到芯片内部的ISRAM（Internal SRAM)中。

4-6：pre-loader初始化好DRAM后就将lk从flash（nand/emmc）中加载到DRAM中运行；

7-8：解压bootimage成ramdisk跟kernel并载入DRAM中,初始化dtb；

9-11：lk跳转到kernl初始化, kernel初始化完成后fork出init进程， 然后拉起ramdisk中的init程序，进入用户空间初始化，init进程fork出zygote进程..
直到整个Android启动完成.


JNI层：
  上面我们只是说android让java开发人员能够方便的进行手机应用软件开发了，更具体的说应该是更方便的在linux操作系统上以java语言进行应用软件开发了。
所以说android不是操作系统，是linux操作系统下的一个框架。一个可以使用java语言进行开发的框架，“一个实现了通过java调用C或者C++(.so)进而linux操作系统的框架
”。这个框架是运行在linux系统上的一个程序，没有这个框架，下面的linux系统已经完全能够实现我们所需要的应用(和使用android达到一样的效果)，但是他不够人性化。
Linux强大的可移植性和java的平台无关性，两者的结合简直就是perfect。

上层应用和UI有java来完成，底层的硬件有C/C++来完成。如果底层的硬件有改动，只需要改动lib层(.so)和kernel层就可以了(我觉得叫做linux系统层更好)。
层是一个很好的概念，层代表着独立、自由。每一层的首要任务是负责自己层的事情，但是层与层之间需要沟通。Java层在实现某种功能时，如果需要调用底层的C组件，
那么就会使用JNI接口。JNI是Java Native Interface的缩写，译为Java本地接口。它允许Java代码和其他语言编写的代码进行交互。在android中提供JNI的方式，
让Java程序可以调用C语言程序。android中很多Java类都具有native接口，这些接口由本地实现，然后注册到系统中。

JNI代码放在以下的路径中：frameworks/base/core/jni/，这个路径中的内容被编译成库 libandroid_runtime.so

Java转化为字节码之后就需要虚拟机来执行了。Dalvik virtual machine。在java执行的过程中，如果java类需要使用C++函数，那么虚拟机就会载入这个C函数。
虚拟机可以让java和C之间通过标准的JNI相互调用。System.loadLibrary(*.so)这个动作就是java程序要求虚拟机载入C函数，载入之后java类和.so库就一起运行了。

