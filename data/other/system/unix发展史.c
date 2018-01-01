
Unix操作系统的历史演变

作者：solarium
目录

    1 简介
    2 历史
        2.1 初创期
        2.2 发展期
        2.3 现况
        2.4 1127部门的解散
    3 标准
    4 自由的类Unix系统

简介

Ken 和Dennis最早是在贝尔实验室开发Unix的，此后的10年，Unix在学术机构和大型企业中得到了广泛的应用，当时的UNIX拥有者 AT&T公司以低廉甚至免费的许可将Unix源码授权给学术机构
做研究或教学之用，许多机构在此源码基础上加以扩充和改进，形成了所谓的Unix “变种(Variations)”，这些变种反过来也促进了Unix的发展，其中最著名的变种之一是由加州大学Berkeley
分校开发的BSD产品。

    后来AT&T意识到了Unix的商业价值，不再将Unix源码授权给学术机构，并对之前的Unix及其变种声明了版权权利。变种BSD Unix在Unix的历史发展中具有相当大的影响力，被很多商业厂家采用，
成为很多商用Unix的基础。BSD使用主版本加次版本的方法标识，如 4.2BSD，4.3BSD，在原始版本的基础上还有派生版本，这些版本通常有自己的名字，如4.3BSD-Net/1，4.3BSD-Net/2等。
其 不断增大的影响力终于引起了AT&T的关注，于是开始了一场旷日持久的版权官司，这场官司一直打到 AT&T将自己的Unix系统实验室卖掉，新接手的Novell公司采取了一种比较开明的做法，
允许伯克利自由发布自己的BSD，但是前提是必须 将来自于AT&T的代码完全删除，于是诞生了4.4 BSD Lite版，由于这个版本不存在法律问题，4.4BSD Lite成为了现代BSD系统的基础版本。
尽管后来，非商业版的UNIX系统又经过了很多演变，但其最终，都是建立在BSD版本上（Linux除外）。所 以从这个角度上，4.4 BSD又是所有自由版(Free版）Unix的基础，它们和Unix V及
Linux等共同构成Unix操作系统这片璀璨的星空。

BSD在发展中也逐渐衍生出3个主要的分支：FreeBSD，OpenBSD和NetBSD。

此 后的几十年中，Unix仍在不断变化，其版权所有者不断变更，授权者的数量也在增加。Unix的版权曾经为AT&T所有，之后 Novell拥有了Unix，再之后Novell又将版权出售给了SCO（这一事实双方尚存在争议）。有很多大公司在取得了Unix的授权之后，开发了自 己的Unix产品，比如IBM的AIX，HP的HPUX，SUN的Solaris和SGI的IRIX。

Unix因为其安全可靠，高效强大的特点在服务器领域得到了广泛的应用。直到GNU/Linux流行开始前，Unix也是科学计算、大型机、超级计算机等所用操作系统的主流。

 
历史

 
初创期

Unix 的诞生和Multics（Multiplexed Information and Computing System）是有一定渊源的。Multics是由麻省理工学院，AT&T贝尔实验室和通用电气合作进行的操作系统项目，被设计运行在GE- 645大型主机上，但是由于整个目标过于庞大，糅合了太多的特性，Multics虽然发布了一些产品，但是性能都很低，最终以失败而告终。

AT&T最终撤出了投入Multics项目的资源，其中一个开发者，Ken Thompson则继续为GE-645开发软件，并最终编写了一个太空旅行游戏。经过实际运行后，他发现游戏速度很慢而且耗费昂贵——每次运行会花费75美元。

在Dennis Ritchie的帮助下，Thompson用PDP-7的汇编语言重写了这个游戏，并使其在DEC PDP-7上运行起来。这次经历加上Multics项目的经验，促使Thompson开始了一个DEC PDP-7上的新操作系统项目。Thompson和Ritchie领导一组开发者，开发了一个新的多任务操作系统。这个系统包括命令解释器和一些实用程 序，这个项目被称为UNICS（Uniplexed Information and Computing System），因为它可以支持同时的多用户操作。后来这个名字被改为UNIX。

 
发展期

最初的Unix 是用汇编语言编写的，一些应用是由叫做B语言的解释型语言和汇编语言混合编写的。B语言在进行系统编程时不够强大，所以 Thompson和Ritchie对其进行了改造，并与1971年共同发明了C语言。1973年Thompson和Ritchie用C语言重写了 Unix。在当时，为了实现最高效率，系统程序都是由汇编语言编写，所以Thompson和Ritchie此举是极具大胆创新和革命意义的。用C语言编写 的Unix代码简洁紧凑、易移植、易读、易修改，为此后Unix的发展奠定了坚实基础。

1974年，Thompson和Ritchie合作在ACM通信上发表了一片关于UNIX的文章，这是UNIX第一次出现在贝尔实验室以外。此后UNIX被政府机关，研究机构，企业和大学注意到，并逐渐流行开来。

1975 年，UNIX发布了4、5、6三个版本。1978年，已经有大约600台计算机在运行UNIX。1979年，版本7发布，这是最后一个广泛 发布的研究型UNIX版本。20世纪80年代相继发布的8、9、10版本只授权给了少数大学。此后这个方向上的研究导致了Plan 9的出现，这是一个新的分布式操作系统。

1982年，AT&T基于版本7开发了UNIX System Ⅲ的第一个版本，这是一个商业版本仅供出售。为了解决混乱的UNIX版本情况，AT&T综合了其他大学和公司开发的各种UNIX，开发了UNIX System V Release 1。

这 个新的UNIX商业发布版本不再包含源代码，所以加州大学Berkeley分校继续开发BSD UNIX，作为UNIX System III和V的替代选择。BSD对UNIX最重要的贡献之一是TCP/IP。BSD有8个主要的发行版中包含了TCP/IP：4.1c、4.2、4.3、 4.3-Tahoe、4.3-Reno、Net2、4.4以及 4.4-lite。这些发布版中的TCP/IP代码几乎是现在所有系统中TCP/IP实现的前辈，包括AT&T System V UNIX 和Microsoft Windows。

其他一些公司也开始为其自己的小型机或工作站提供商业版本的UNIX系统，有些选择System V作为基础版本，有些则选择了BSD。BSD的一名主要开发者，Bill Joy，在BSD基础上开发了SunOS，并最终创办了Sun Microsystems。

1991 年，一群BSD开发者（Donn Seeley、Mike Karels、Bill Jolitz 和 Trent Hein）离开了加州大学，创办了Berkeley Software Design, Inc (BSDI)。BSDI是第一家在便宜常见的Intel平台上提供全功能商业BSD UNIX的厂商。后来Bill Jolitz 离开了BSDI，开始了386BSD的工作。386BSD被认为是FreeBSD、OpenBSD 和 NetBSD的先辈。

AT& T继续为UNIX System V增加了文件锁定，系统管理，作业控制，流和远程文件系统。1987到1989年，AT&T决定将Xenix（微软开发的一个x86-pc上的 UNIX版本），BSD，SunOS和System V融合为System V Release 4（SVR4）。这个新发布版将多种特性融为一体，结束了混乱的竞争局面。

1993年以后，大多数商业UNIX发行商都基于SVR4开发自己的UNIX变体了。


UNIX进化史
现况

UNIX System V Release 4发布后不久，AT&T就将其所有UNIX权利出售给了Novell。Novell期望以此来对抗微软的Windows NT，但其核心市场受到了严重伤害，最终Novell将SVR4的权利出售给了X/OPEN Consortium，后者是定义UNIX标准的产业团体。最后X/OPEN和OSF/1合并，创建了Open Group。Open Group定义的多个标准定义着什么是以及什么不是UNIX。

实际的UNIX代码则辗转到了Santa Cruz Operation，这家公司后来出售给了Caldera Systems。Caldera原来也出售Linux系统，交易完成后，新公司又被重命名为SCO Group。

 
1127部门的解散

根据一项报导指出，当年负责研发UNIX与后续维护工作的贝尔实验室1127部门已于2005年8月正式宣告解散。Ken Thompson已退休，现居加州；Dennis Ritchie调到别的部门；Douglas McIlroy在达特茅斯学院担任教授等等。
标准

从1980年代开始，POSIX，一个开放的操作系统标准就在制定中，IEEE制定的POSIX标准现在是UNIX系统的基础部分。
自由的类Unix系统

Richard Stallman建立了GNU项目，要创建一个能够自由发布的类UNIX系统。20年来，这个项目不断发展壮大，包含了越来越多的内容。现在，GNU项目开发的产品，比如Emacs，GCC等已经成为各种其他自由发布的类UNIX产品中的核心角色。

1990 年，Linus Torvalds决定编写一个自己的Minix内核，初名为Linus' Minix，意为Linus的Minix内核，后来改名为Linux，此内核于1991年正式发布，并逐渐引起人们的注意。当GNU软件与Linux内核 结合后，GNU软件构成了这个POSIX兼容操作系统GNU/Linux的基础。今天GNU/Linux已经成为发展最为活跃的自由/开放源码的类 Unix操作系统。

1994年，BSD Unix走上了复兴的道路。BSD的开发也走向了几个不同的方向，并最终导致了FreeBSD、OpenBSD和NetBSD的出现。

类Unix操作系统

A/UX | AIX | BSD | DragonFly BSD | FreeBSD | GNU | HP-UX | IRIX | Linux | LynxOS | Mac OS X | Minix | NetBSD | NEXTSTEP | OpenBSD | QNX | SCO OpenServer | Solaris | System V | Tru64 | Xenix |