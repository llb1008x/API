
android项目工程的文件结构


1:abi

applicationbinary interface，应用程序二进制接口，生成libgabi++.so相关库文件


2:art
google在4.4后加入用来代替Dalvik的运行时

3:bionic
Android的C library，即C库文件

4:bootable
启动引导相关代码
|-- bootloader 适合各种bootloader的通用代码
|  |-- lk             little kernel
|  |-- preloader 
|  |-- recovery  recovery模式

5:build
存放系统编译规则及generic等基础开发配置包
|-- core    核心编译规则
|-- libs      库
|-- target  目标机编译对象
|-- tools    编译中使用的工具及脚本

6:cts
Android兼容性测试套件

7:dalvik
dalvik虚拟机

8:developers
开发者使用，存放几个例子
|-- build        编译相关
|-- demos     两个demo
|-- samples   例子

9:development
开发者需要的一些例程及工具
|-- apps     一些核心应用程序
|-- build     编译脚本模板
|-- cmds    有个monkey工具
|-- docs     文档
|-- host     主机端USB驱动等
|-- ide       集成开发环境
|-- libraries sdk启动模拟器使用
|-- ndk      本地开发套件——c语言开发套件
|-- perftests 全景功能
|-- samples  例程
|-- scripts    脚本
|-- sdk        sdk配置
|-- sdk_overlay        
|-- sys-img  
|-- testrunner  测试用
|-- tools     一些工具
|-- tutorials  也是一些例子
10:device
设备相关代码，这里仅举例一个，也是mtk aosp后向国际大厂同步的做法
|-- mediatek
|  |-- build        编译相关
|  |-- common  通用配置
|  |-- mt6735    平台配置

11:docs
介绍开源的相关文档

12:external
android使用的一些开源的模组，很多子目录，不一一列出

13:frameworks
framework目录，android核心框架
|-- av       音频相关
|-- base   基本内容
| |-- api        api汇总
| |-- cmds    重要命令：am、app_proce等
| |-- core     核心库
| |-- data     字体和声音等数据文件
| |-- docs    文档
| |-- drm      drm监权相关
| |-- graphics  图形相关
| |-- include    头文件
| |-- keystore  和数据签名证书相关
| |-- libs          库
| |-- location   定位相关
| |-- media     媒体相关库
| |-- native     提供jni
| |-- nfc-extras    nfc提供
| |-- obex       蓝牙传输库
| |-- opengl    2D-3D加速库
| |-- packages   设置、TTS、VPN程序
| |-- policy    锁屏剥离出来的
| |-- rs           脚本语言
| |-- samples  例子
| |-- sax         解析器
| |-- services 各种服务
| |-- telecomm 电话
| |-- telephony 电话通讯管理
| |-- test-runner   测试工具相关
| |-- tests         各种测试
| |-- tools         一些工具
| |-- wifi           无线网络

14:hardware
部分厂家开源的硬解适配层HAL代码

15:kernel-3.10
内核

16:libcore
核心库相关

17:libnativehelper
jni用到的库

18:md32

19:ndk
ndk相关

20:packages
各种apk

21:pdk
google用来减少碎片划的东西

22:prebuilts
预编译的一些资源

23:sdk
顾名思义sdk及模拟器等

24:system
系统组件库等

25:tools
又一些工具

26:vendor
厂商定制代码
|-- mediatek