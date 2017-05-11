
 Linux内核编译详解 


学习了网上的一些资料，自己试着摸索了一下，整理出此文。

由于在下水平相当有限，不当之处，还望大家批评指正^_^


重要的参考资料有：

http://raspberrypi.stackexchange.com/questions/192/how-do-i-cross-compile-the-kernel-on-a-ubuntu-host

http://blog.csdn.net/xdw1985829/article/details/6833319


好了，下面进入正题。


一、准备工作



准备工作如何做，这里就不详说了。

a) 首先，你要有一台PC（这不废话么^_^），装好了Linux。

b) 安装好GCC(这个指的是host gcc，用于编译生成运行于pc机程序的)、make、ncurses等工具。

c) 下载一份纯净的Linux内核源码包，并解压好。

注意，如果你是为当前PC机编译内核，最好使用相应的Linux发行版的源码包。

不过这应该也不是必须的，因为我在我的Fedora 13上(其自带的内核版本是2.6.33.3)，就下载了一个标准的内核linux-2.6.32.65.tar.xz，并且顺利的编译安装成功了，
上电重启都OK的。不过，我使用的.config配置文件，是Fedora 13自带内核的配置文件，即/lib/modules/`uname -r`/build/.config

d) 如果你是移植Linux到嵌入式系统，则还要再下载安装交叉编译工具链。

例如，你的目标单板CPU可能是arm或mips等cpu，则安装相应的交叉编译工具链。安装后，需要将工具链路径添加到PATH环境变量中。例如，你安装的是arm工具链，
那么你在shell中执行类似如下的命令，假如有类似的输出，就说明安装好了。

[root@localhost linux-2.6.33.i686]# arm-linux-gcc  --version
arm-linux-gcc (Buildroot 2010.11) 4.3.5
Copyright (C) 2008 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.


注：arm的工具链，可以从这里下载:http://www.veryarm.com/cross-tools


二、设置编译目标


在配置或编译内核之前，首先要确定目标CPU架构，以及编译时采用什么工具链。这是最最基础的信息，首先要确定的。

如果你是为当前使用的PC机编译内核，则无须设置。否则的话，就要明确设置。这里以arm为例，来说明。

有两种设置方法()：

a) 修改Makefile

打开内核源码根目录下的Makefile，修改如下两个Makefile变量并保存。

ARCH           := arm
CROSS_COMPILE  := arm-linux-

注意，这里cross_compile的设置，是假定所用的交叉工具链的gcc程序名称为arm-linux-gcc。如果实际使用的gcc名称是some-thing-else-gcc，则这里照葫芦画瓢填
some-thing-else-即可。总之，要省去名称中最后的gcc那3个字母。

b) 每次执行make命令时，都通过命令行参数传入这些信息。这其实是通过make工具的命令行参数指定变量的值。

例如

配置内核时时，使用

make  ARCH=arm  CROSS_COMPILE=arm-linux-  menuconfig

编译内核时使用

make  ARCH=arm  CROSS_COMPILE=arm-linux- 


注意，实际上，对于编译PC机内核的情况，虽然用户没有明确设置，但并不是这两项没有配置。因为如果用户没有设置这两项，内核源码顶层Makefile(位于源码根目录下)
会通过如下方式生成这两个变量的值。

SUBARCH := $(shell uname -m | sed -e s/i.86/i386/ -e s/sun4u/sparc64/ \
 -e s/arm.*/arm/ -e s/sa110/arm/ \
 -e s/s390x/s390/ -e s/parisc64/parisc/ \
 -e s/ppc.*/powerpc/ -e s/mips.*/mips/ \
 -e s/sh[234].*/sh/ )

ARCH?= $(SUBARCH)
CROSS_COMPILE ?=

经过上面的代码，ARCH变成了PC编译机的arch，即SUBARCH。因此，如果PC机上uname -m输出的是ix86，则ARCH的值就成了i386。

而CROSS_COMPILE的值，如果没配置，则为空字符串。这样一来所使用的工具链程序的名称，就不再有类似arm-linux-这样的前缀，就相当于使用了PC机上的gcc。

最后再多说两句，ARCH的值还需要再进一步做泛化。因为内核源码的arch目录下，不存在i386这个目录，也没有sparc64这样的目录。

因此顶层makefile中又构造了一个SRCARCH变量，通过如下代码，生成他的值。这样一来，SRCARCH变量，才最终匹配到内核源码arch目录中的某一个架构名。


SRCARCH := $(ARCH)  


ifeq ($(ARCH),i386)
        SRCARCH := x86
endif


ifeq ($(ARCH),x86_64)
        SRCARCH := x86
endif

ifeq ($(ARCH),sparc64)
       SRCARCH := sparc
endif

ifeq ($(ARCH),sh64)
       SRCARCH := sh
endif


三、配置内核


内核的功能那么多，我们需要哪些部分，每个部分编译成什么形式（编进内核还是编成模块），每个部分的工作参数如何，这些都是可以配置的。因此，在开始编译之前，
我们需要构建出一份配置清单，放到内核源码根目录下，命名为.config文件，然后根据此.config文件，编译出我们需要的内核。

但是，内核的配置项太多了，一个一个配，太麻烦了。而且，不同的CPU架构，所能配置的配置项集合，是不一样的。例如，某种CPU的某个功能特性要不要支持的配置项，
就是与CPU架构有关的配置项。所以，内核提供了一种简单的配置方法。

以arm为例，具体做法如下。

a) 根据我们的目标CPU架构，从内核源码arch/arm/configs目录下，找一个与目标系统最接近的配置文件(例如s3c2410_defconfig)，拷贝到内核源码根目录下，命名为.config。

注意，如果你是为当前PC机编译内核，最好拷贝如下文件到内核源码根目录下，做为初始配置文件。这个文件，是PC机当前运行的内核编译时使用的配置文件。

/lib/modules/`uname -r`/build/.config

这里顺便多说两句，PC机内核的配置文件，选择的功能真是多。不编不知道，一编才知道。Linux发行方这样做的目的，可能是想让所发行的Linux能够满足用户的各种需求吧。

b) 执行make menuconfig对此配置做一些需要的修改，退出时选择保存，就将新的配置更新到.config文件中了。

注意-1，我们执行此操作时，内核打开了一组配置项集合，让我们进行配置。这一组配置项集合，是由我们前面设置的CPU架构决定的。说得细一点，配置系统打开arch/arm/Kconfig
文件(make menuconfig执行时能看到有一行“scripts/kconfig/mconf arch/arm/Kconfig”这样的打印)，这个文件又包含了其他内核子系统的Kconfig文件
（文件名也可能是其他名字），其他子系统的Kconfig文件，再层层包含下层的Kconfig文件，从而生成了全部的配置项集合。而每一项配置项，当前设定的值（例如，是编进内核，
还是编译成模块，或者也可能是一项参数），则是由内核源码根目录下的.config文件生成的。

注意-2，即使你不需要对配置进行任何修改，都务必请执行一下make menuconfig，然后进入配置界面后直接退出并保存。不然的话，后面的编译可能会遇到问题。笔者就遇到过这个问题。笔者猜测原因可能是，初始的配置文件是基于老版本的内核做的，新版本的内核可能新增了一些基础功能项，从而导致功能项之间的依赖关系发生了变化。例如，老的配置文件中选中的一个功能项，在新版内核中的实现，可能依赖了更多的其他功能项。因此需要对旧的初始配置文件进行一些调整，从而保证各个功能项的依赖条件得到满足。经过make menuconfig之后，笔者发现，.config文件的内容的确发生了变化。


四、编译内核


编译本身很简单，对于2.6版本以上的内核，执行如下一条命令就搞定了。

make

我们不妨花点时间，理解一下内核编译的机制。

a) 内核如何使用config文件

前面生成了.config文件，这是个文本文件，其中都是一些类似如下的内容：

CONFIG_YENTA_ENE_TUNE=y
CONFIG_YENTA_TOSHIBA=y
CONFIG_PD6729=m
CONFIG_I82092=m

CONFIG_MTDRAM_ERASE_SIZE=128

能看出，有些是设置了将某个功能编译进内核，有些是设置了将某个功能编译成模块，有些是设置了某个功能的某个参数。

这个文件的语法，其实就是定义makefile变量的语法。没错，这就是makefile。

当我们执行make开始编译内核的时候，编译系统还会生成另一个config文件，那就是include/config/auto.conf。里面的内容和.config类似，只是内容少了一些。

内核编译的时候，顶层Makefile（位于源码根目录下），会包含上述config文件。

这样就获得了相应的makefile变量，从而知道如何编译内核的各个部分。

从顶层makefile中，可以看到如下代码：

ifeq ($(dot-config),1)
# Read in config
-include include/config/auto.conf

但是，这两个config文件的关系如何，到底会包含哪个，在下也没有理清。有清楚的，还望赐教:)


b) 内核如何编译各个子系统或模块

从上一步知道，通过config文件，内核顶层makefile已经生成了大量的makefile变量。

另一方面，每个子系统或模块，他们的源码目录中，都有一个Makefile，其中定义了本子系统或模块所需要编译的内容。

接下来，make工具就可以带着顶层makefile中生成的大量的makefile变量，一层层进入到各个子系统或模块所在的目录中去，去实现各目录中Makefile中定义的内容的编译。

而这些目录中的Makefile可以说是非常简单。

如果某个目录下，只有一个模块hello，此模块只有一个.c文件，例如xxx.c。那么其Makefile的全部内容只有如下一行。

obj-$(CONFIG_HELLO) := hello.o

如果hello模块，由main.c a.c b.c三个文件构成，则Makefile也只需要两行内容。

obj-$(CONFIG_HELLO) := hello.o

hello-objs := main.o a.o b.o

如果一个目录下存放了多个模块的C文件，别是hello、hello2、hello3。
hello模块的构成：main.c  a.c  b.c
hello2模块的构成：main2.c  a2.c  b2.c
hello3模块的构成：hello3.c
此时，Makefile只需要5行内容。

obj-$(CONFIG_HELLO) += hello.o

obj-$(CONFIG_HELLO2) += hello2.o

obj-$(CONFIG_HELLO3) += hello3.o

hello-objs := main.o a.o b.o
hello2-objs := main2.o a2.o b2.o


由于顶层Makefile中带有大量的变量，因此，子目录内Makefile中的$(CONFIG_HELLO) 变量经过解析后，会变成y或m。这样的话，每个子目录中的Makefile经过解析后，等于只是定义了一个变量，变量名为obj-m或obj-y。

变量obj-m或obj-y的值，则是一串.o文件的列表。表中每一项，代表一个功能项。如果变量名为obj-m，则此功能被编译成模块。如果变量名为obj-y，则此功能被编进内核。


c) 内核代码中，如何知道某个功能有没有配置，配置成了什么形式


当我们执行make开始编译内核的时候，编译系统还会生成一个C语言头文件include/generated/autoconf.h

这个文件中都是类似如下的内容：

#define CONFIG_DM9000 1
#define CONFIG_DM9000_DEBUGLEVEL 4
#define CONFIG_SND_RAWMIDI_SEQ_MODULE 1
第一行，是说明用户选择了将DM9000这个驱动编进内核，第二行是此驱动的一个参数。如果用户选择的是将DM9000编译成模块，则第一行的内容就变成如下形式了。
#define CONFIG_DM9000_MODULE 1


有了这个头文件，某个内核源码的.c文件中如果包含了这个头文件，通过#ifdef CONFIG_XXX就可以知道用户有没有配置XXX功能了。

好了，内核编译机制，在下理解的也很有限，这里就不多说了^_^


五、安装内核

a) 为当前PC机安装内核

依次执行如下两条命令，分别完成模块和内核的安装。
make  modules_install

make  install


然后打开boot/grub/grub.conf，会看到里面多出了一个条目。

将其中的timeout修改为5，以便开机时有5秒的时间选择启动哪一个内核。

最后，重启电脑。在bootloader界面出现时，选择启动新内核即可。


b) 为嵌入式系统安装内核

这就不是一句两句能说清的了，具体问题大家自己具体参考相关资料吧^_^

对于一般的arm单板，常见的方法是，PC机通过SecureCrt连接单板串口，通过网线连接单板网口，PC机上启动tftp服务器，把内核映像zImage文件放到tftp下载目录中。重启单板，SecureCrt中看到u-boot启动倒计时的时候，按任意键进入u-boot交互界面。然后在这个界面下，通过相关命令下载内核映像zImage文件，然后通过命令将下载的zImage烧写到单板的FLASH中。最后重启单板即可。


至于模块的安装，则很简单，通过如下一条命令搞定

make -C  /path/to/kernel_src_dir  modules_install  INSTALL_MOD_PATH=/path/to/rootfs_dir

上面的命令执行后，模块就已经安装到目标系统的根文件系统中了 。

当然，上面的根文件系统只是按一定的结构组织起来的一组目录与文件，他还需要被打包成具体的文件系统格式(如CramFS,squashfs,jffs2等)，然后烧写到flash中才能最终使用^_^