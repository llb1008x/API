linux下类似Bus Hound的工具
0
推荐在linux大家有时候需要调试usb接口的串口消息，但是没有类似于windows下的bus hound工具，感觉比较痛苦，其实linux内核提供了usbmon这个工具，可以收集串口信息。

1.准备：

挂接debugfs (这个可以在内核配置中使能)，加载usbmon模块(如果usbmon编译成模块). 如果usbmon编译到内核中的话，第二步可以省略

# mount -t debugfs none_debugs /sys/kernel/debug
# modprobe usbmon
#

确定bus数据已经准备就绪

# ls /sys/kernel/debug/usb/usbmon
0s  0u  1s  1t  1u  2s  2t  2u  3s  3t  3u  4s  4t  4u
#

现在你就可以选择要么使用'0u'(抓bus上的所有包),并且直接跳到第三步,或者进行第二步找到你设备所在的bus,这样就过滤掉很多烦人的信息

2. 查找设备所在的bus
运行"cat /proc/bus/usb/devices", 就会发现设备T开头的行.T行有一个bus口,如下所示
 T:  Bus=03 Lev=01 Prnt=01 Port=00 Cnt=01 Dev#=  2 Spd=12  MxCh= 0
 D:  Ver= 1.10 Cls=00(>ifc ) Sub=00 Prot=00 MxPS= 8 #Cfgs=  1
 P:  Vendor=0557 ProdID=2004 Rev= 1.00
 S:  Manufacturer=ATEN
 S:  Product=UC100KM V2.00
 Bus=03 说明它在bus3口上

3. 开始查看信息
# cat /sys/kernel/debug/usb/usbmon/3u > /tmp/1.mon.out

命令用来监听某个bus口，要么你可以监听所有的bus口

# cat /sys/kernel/debug/usb/usbmon/0u > /tmp/1.mon.out
* 数据格式
现在支持两种数据格式: '1t'格式和'1u'格式. '1t' 在内核 2.6.21上不支持. '1u'数据格式增加了一些数据项，如：ISO框架信息,间隔时间等.它产生了稍微多余的行，但是确实是'1t'格式的有效补充.
数据内容的详细说明（从左到右）
- URB Tag. URB地址

- Timestamp, 间隔时间

- Event Type. 时间类型.
支持的类型是: S - submission, C - callback, E - submission error.

- "Address" (通常是一个"管道").它包括4项内容,用冒号分割: URB type ,direction, Bus number, Device address, Endpoint number.
Type和direction被封装成如下数据格式:
Ci Co   Control input and output
Zi Zo   Isochronous input and output
Ii Io   Interrupt input and output
Bi Bo   Bulk input and output
Bus number, Device address, 和Endpoint是十进制数字, 但是他们可能因为阅读便利而设置成为0.

- URB Status word. 这个要么是字母或者是一些由分号隔开的数字

- Setup packet, 如果出现的话，包括5个字串: 他们是bmRequestType,bRequest, wValue, wIndex, wLength, (USB Specification 2.0.协议中有详细介绍)

- 同步号

- Data Length. 数据长度

- Data tag. 即使数据长度不为0，usbmon也不一定总能捕捉到数据，数据包内容只有在此tag为'='才有效

- Data words 大头十六进制数.
举例:
输入控制得到端口状态的示例
d5ea89a0 3575914555 S Ci:1:001:0 s a3 00 0000 0003 0004 4 <
d5ea89a0 3575914560 C Ci:1:001:0 0 4 = 01050000
 
通过bus5口发送一个31字节长度的块包（其中保存一个SCSI命令 0x5E ）到存储设备
dd65f0e8 4128379752 S Bo:1:005:2 -115 31 = 55534243 5e000000 00000000 00000600 00000000 00000000 00000000 000000
dd65f0e8 4128379808 C Bo:1:005:2 0 31 >


详细信息请参阅：http://www.mjmwired.net/kernel/Documentation/usb/usbmon.txt
转自：http://hi.baidu.com/kahn_qiao/item/cb1c57e151f4eeaace2d4f9d
 
======================================================================================================================================================
LinuxUSB驱动程序调试
：这是LinuxUSB驱动程序调试--003：Ubuntu下的准备工作的详细页面，介绍了和linux,有关的知识、技巧、经验，和一些linux源码等。
Ubuntu下的准备工作：
（1）如何查看内核中的printk输出： dmesg
    再配合管道命令，   dmesg |head   ，    dmesg |tail
    通过这个命令，我们可以看到内中的printk输出的内容，这是很有用的一个
    工具。
 
（2）的USB抓包工具：usb_mon
 其实，Ubuntu下已经内置了USB抓包工具---USB_MON 
【其实 make menuconfig 时候会让我们选择的】
使用方法如下：
    # mount -t debugfs none /sys/kernel/debug
    # modprobe usbmon
查看当前的USB device：
         # cat  /sys/kernel/debug/usb/devices
        
   下面的输出：
  T:  Bus=01 Lev=01 Prnt=01 Port=00 Cnt=01 Dev#= 12 Spd=480 MxCh= 0
  D:  Ver= 2.00 Cls=00(>ifc ) Sub=00 Prot=00 MxPS=64 #Cfgs=  1
  P:  Vendor=601a ProdID=4740 Rev= 1.00
  S:  Manufacturer=Ingenic
  S:  Product=JZ4740 USB Boot Device
  C:* #Ifs= 1 Cfg#= 1 Atr=c0 MxPwr=  2mA
  I:* If#= 0 Alt= 0 #EPs= 2 Cls=ff(vend.) Sub=00 Prot=50 Driver=skeleton
  E:  Ad=01(O) Atr=02(Bulk) MxPS= 512 Ivl=0ms
  E:  Ad=81(I) Atr=02(Bulk) MxPS= 512 Ivl=0ms
        
  如何看懂上面的输出？  Documentation/usb/usbmon.txt
  Bus=01 means it's bus 1.
通过上面的 Vendor=601a ProdID 可以找到我们设备的Bus号。
上面Bus=01。
使用下面的名利，就开始监听 Bus01上的全部USB数据包。
     # cat  /sys/kernel/debug/usb/usbmon/1u
要想开始监听 全部 Bus 上的全部USB数据包：
     # cat  /sys/kernel/debug/usb/usbmon/0u
    
  一旦有数据包，就会打印出来：
    # cat /sys/kernel/debug/usb/usbmon/1u
      f07dbb80 3233986312 S Co:1:012:0 s 40 01 80c0 0000 0000 0
      f07dbb80 3234327703 C Co:1:012:0 0 0
      f07dbb80 3234327880 S Co:1:012:0 s 40 02 0000 0200 0000 0
      f07dbb80 3234329561 C Co:1:012:0 0 0
      f07dbb80 3234329625 S Bi:1:012:1 -115 32 <
      f07dbb80 3234331550 C Bi:1:012:1 -75 32 = 00000000 00000000  。。。
 
读懂上面的数据，还是要看usbmon.txt
   URB tag  ：标志
   TimeStamp：毫秒数
   Event Type：  S - submission,
                 C - callback,
                 E - submission error.

   URBType：BusNo：DeviceAddr：EndPointNum
          Ci Co   Control input and output
          Zi Zo   Isochronous input and output
          Ii Io   Interrupt input and output
          Bi Bo   Bulk input and output
   URB Status： 0为没有错误。-XXX是错误码。