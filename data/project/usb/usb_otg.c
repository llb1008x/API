





OTG 检测的原理是:
　　USB OTG标准在完全兼容USB2.0标准的基础上，增添了电源管理（节省功耗）功能，它允许设备既可作为主机，也可作为外设操作（两用OTG）。USB OTG技术可实现没有主机时设备与设备之间的数据传输。例如：数码相机可以直接与打印机连接并打印照片，手机与手机之间可以直接传送数据等，从而拓展了USB技术的应用范围。在OTG中，初始主机设备称为A设备，外设称为B设备。也就是说，手机既可以做外设，又可以做主机来传送数据，可用电缆的连接方式来决定初始角色(由ID线的状态来决定)。
USB OTG接口中有5条线：   
2条用来传送数据（D+ 、D-）;     
1条是电源线(VBUS);      
1条则是接地线(GND)、
1条是ID线。ID线---以用于识别不同的电缆端点，mini-A插头(即A外设)中的ID引脚接地，mini-B插头（即B外设）中的ID引脚浮空。当OTG设备检测到接地的ID引脚时，表示默认的是A设备（主机），而检测到ID引脚浮空的设备则认为是B设备（外设）。
只有而言支持USB OTG的设备（即可以做USB Host有可以做Slave的设备），USB_ID信号才有意义。
当设备检测到USB_ID信号为低时，表该设备应作为Host（主机，也称A设备）用。
当设备检测到USB_ID信号为高时，表示该设备作为Slave(外设，也称B设备）用。
实际的USB连接线中，是没有USB_ID这根线的。 都是在接口部分直接拉死的的。
对于Host端，只需将连接线的USB_ID pin和地短接即可，
对于Slave端，USB连接线的USB_ID pin是悬空的。（设备内部上拉）。

USB OTG的工作原理
　　OTG补充规范对USB 2.0的最重要的扩展是，其更具节能性的电源管理和允许设备以主机和外设两种形式工作。OTG有两种设备类型：两用OTG设备(Dualrole device)和外设式OTG设备(Peripheralonly OTG device) 。两用OTG设备完全符合USB 2.0规范，同时，他还要提供有限的主机能力和一个 MiniAB插座、支持主机流通协议(Host Negotiatio n Protocol，HNP)，并和外设式OTG设备一样支持事务请求协议 (Session Request Protocol， SRP)。当作为主机工作时，两用OTG设备可在总线上提供8 mA的电流，而以往标准主机则需要提供100～500 mA的电流。
　　2个两用OTG设备连接在一起时可交替以主机和从机的方式工作，这个特点兼容了现有USB 规范主机/外设的结构模型。OTG主机负责初始化数据通信的任务，比如：总线复位、获取USB 各种描述符和配置设备。这些配置完成后，2个OTG设备便可以分别以主机和从机方式传输信息，2个设备主从角色交换的过程由主机传输协议(HNP)定义。

下面从5个方面说明OTG的工作原理。
1.1主机（Adevice）和从机（Bdevice）的初始功能
　　设备的初始功能是通过定义连接器来实现的。OTG定义了一个 叫做MiniAB的袖珍插孔，他能直接接入MiniA或者MiniB插口，MiniAB有一个ID引脚上拉至电源端，MiniA插头有一个与地连接好的 ID（R<10 Ω），Mini B插头有一个与地连接的开路ID引脚(R>100 kΩ)。当2个OTG设备连接到一起的时 候 ，MiniA插头边的ID引脚会注入一个"0"状态，MiniB插头边的ID引脚为 "1"，ID为0的OTG设备默认为主机 （Adevice），ID为1的OTG设备默认为从机（B device）。

1.2 对话请求协议SRP(Session Request Protocol)
　　这个协议允许Adevice(可以是电池供电)在总线未使用 时通过切断Vbus来节省电源消耗，也为Bdevice启动总线活动提供了一种方法。任何一个Adevice， 包括PC或便携式电脑，都可以响应 SRP；任何一个Bdevice，包括一个标准USB外设， 都可以启动SRP；要求一个双重功能设备既能启动SRP，又能响应SRP。
在以往的USB系统运行过程中，主机提供5V的电源和不低于100mA的总线电流。当OTG 主机(指以主机方式工作的两用OTG设备，又称A-device)连接到有线电源时这种方法是适用的，但像手机这样的自供电移动设备则不能承受如此大的电 能浪费。为了节约电源延长电池的使用寿命，当总线上没有活动时，OTG主机将挂起总线电源VBUS。SRP协议可使OTG从机(指外设式设备或者以外设方 式工作的两用设备，又称Bdevice，此处指后者)请求A-device重新使能VBUS，而后A-device使用HNP协议交换两个设备的工作方式，这两步完成后由新的OTG主机开始事务传输。B-device可在前一事务结束2ms后的任意时间开始SRP，SRP的时序波形见图形1。
B-device将先后执行数据线脉冲调制(data-line pul-sing)和VBUS脉冲调制(VBUS pulsing) 。它通过使能数据线上拉电阻(全速和高速设备为D+，低速设备为D-)5～10ms实现前者，后者通过驱动VBUS实现。
VBUS必须要有足够长的时间对 其电容充电，这个时间应能保证不大于13μF的电容充电至2.1V(OTG设备的电容是6.5μF或更小)，从而不会对标准主机的96μF或更高的电容充 电至2.0V。该限制保证了从Bdevice引来的VBUS电流不会破坏标准主机的端口。
A-device检测到数据线脉冲调制或者VBUS脉冲调制后，首先复位总线，然后发送 Set_feature命令而先不进行设备的枚举，此时B-device尚处在默认的从机状态。如果Set_feature命令成功执行，说明B- device为两用OTG设备，A-device(使用HNP协议)挂起VBUS准备让B-device交换为主机方式接管总线。如果 Set_feature命令执行失败，说明B-device为外设式OTG设备，于是A-device使能VBUS准备开始一个传输事务（此时，A- device只是被唤醒，并未改变工作方式）。当Adevice认为总线上没有传输需要时，挂起VBUS以结束该事务。这种A-device自动检测 B-device是否支持HNP协议的特征称为"No Silent Failure"。

1.3 主机流通协议HNP(Host Negotiation Protocol)
当两用设备连接了一个Mini-A插头或者Mini-B插头时，它相应的便以OTG主机或者OTG外设的默认方式工作。A-device为默认主机，所以由它提供VBUS电源，且在检测到有设备接入时复位总线、枚举并配置B-device。
A-device在完成对B-device的使用后，可以通过查询B-device的 OTG性能描述符来判断是否支持HNP协议（即是否为两用OTG设备）。如支持HNP，B-device将返回有效的OTG性能描述符，Adevice则产生一个Set_feature命令（即HNP_Enable）来通知B-device可以在总线挂起的时候以主机方式工作，随后A-device挂起总线。
B-device通过上拉电阻（全速时）或者下拉电阻（高速时）拉低D+以示连接断开。随 后，作为对Bdevice断开的响应，A-device使能它的数据线并开始以从机方式工作。完成这些转换后，B-device和Adevice便各自以主机角色和外设角色使用总线。如果该B-device属于两用OTG设备且A-device不再使用它了，A-device便重发 Set_feature命令并挂起总线。若B-device申请角色转换时出错，A-device则拉低VBUS以结束该事务。当B-device正常结 束传输事务时便挂起VBUS使能其上拉电阻，重新以从机方式运行。A-device检测到总线挂起后，发出一个连接断开信号并重新以主机方式工作。
当两用设备连接了一个Mini-A插头或者Mini-B插头时，它相应的便以OTG主机或者OTG外设的默认方式工作。A-device为默认主机，所以由它提供VBUS电源，且在检测到有设备接入时复位总线、八举并配置B-device。
A-device在完成对B-device的使用后，可以通过查询B-device的OTG 性能描述符来判断是否支持HNP协议（即是否为两用OTG设备）。如支持HNP，B-device将返回有效的OTG性能描述符，A-device则产生一个Set_feature命令（即HNP_Enable）来通知B-device可以在总线挂起的时候以主机方式工作，随后Adevice挂起总线。 HNP的时序波形见图2。
B-device通过上拉电阻（全速时）或者下拉电阻（高速时）拉低D+以示连接断开。随 后，作为对B-device断开的响应，A-device使能它的数据线并开始以从机方式工作。完成这些转换后，B-device和A-device便各自以主机角色和外设角色使用总线。如果该B-device属于两用OTG设备且A-device不再使用它了，Adevice便重发 Set_feature命令并挂起总线。若B-device申请角色转换时出错，A-device则拉低VBUS以结束该事务。当B-device正常结 束传输事务时便挂起VBUS使能其上拉电阻，重新以从机方式运行。Adevice检测到总线挂起后，发出一个连接断开信号并重新以主机方式工作。
HNP是一种用来实现Adevice和Bdevice主机/从机转换的协议（实际上是电缆的反转）。

主/从机功能交换的结果表现在下列过程中：
(1)利用上拉电阻来发送信号给从机。
(2)Adevice可在Bdevice上设置"HNP Enable"特性。
(3)Bdevice断开上拉。
(4)ADevice与上拉电阻相连，表明Adevice从属于从机。
(5)Adevice给Vbus供电。
(6)Bdevice检测Adevice的上拉。
(7)复位/列举/使用Adevice。

OTG描述符
在枚举阶段，OTG A-device将会从B-device请求OTG配置描述符。此三字节描述符由三部分组成：bLength， bDescriptorType， 和bmAttributes，位定义如下图所示。
srp_support
如果设备支持SRP，则这一位设置为TRUE。
hnp_support
如果设备支持HNP，则这位设置为TRUE。
Set Feature Commands
A-device会使用SetFeature 命令使能B-device的特点行为，或表明A-device到B-device的特定能力。
b_hnp_enable
设置此特性表明B-device已经能够执行HNP。
此特性仅在总线复位或会话结束时清除，不能通过ClearFeature(b_hnp_enable) 命令清除。
a_hnp_support
设置此特性表明B-device连接的A-device端口支持HNP。A-device会在会话开始，任意B-device配置被选择之前设置此特性。
a_alt_hnp_support
设置此特性表明B-device连接的A-device端口没有HNP功能，但是此A-device 有备用的端口具备HNP功能。此特性仅在总线复位或会话结束时清除，不能通过ClearFeature(b_hnp_enable) 命令清除。


2 连接器和电缆
2.1 连接器
USB IF在OTG中定义了更小的连接器，可以同时用于主机式和外设式的设备、具有更低的电源需求、扩展的电源保护模式和利于上层软件开发的简洁设计。
OTG和现有的USB2.0规范完全兼容，一个主要的机械上改进是它的新式连接器。USB 2.0定义了三种连接器对（插头和插座）：Standard-A、Standard-B和Mini-B。Mini-B连接器是专为较小的外设（如移动电话 等）开发的。OTG规范增加了第四种插头：Mini-A，两种插座：Mini-A和Mini-AB。这些连接器比最初的USB连接器要小许多 （如：Mini-A插头截面积只有Standard-A插头的38%），更适合于便携式设备。
Mini-AB插座用于两用设备。Mini-A新增的ID脚（以前的USB插头不含此脚）在连接Mini-AB时接地短路，而Mini-B则使该脚断开，这样两用设备便可识别连接设备的类型以决定设备的默认角色。
Mini-A和Mini-B插头是严格配对适用的，如：不能将Mini-A插头和Mini- B插座配合使用。但是，Mini-AB插座适合于以上两种插头。在外形上，Mini-A更新近于椭圆形，而Mini-B更接近于正方形。另外，他们的插头 和插座内部的塑料都有颜色：Mini-A为白色，Mini-B为黑色，Mini-AB为灰色。

2.2 电缆
USB 2.0规范定义了两种电缆：Standard-A至Standard-B和Standard-A至Mini-B。OTG新增了两种电缆：Mini-A至 Standard-B和Mini-A至Mini-B。Mini-A-to-Mini-B电缆的延迟被减少到可以在“A端”使用适配器，如：用 Standard-A插头连接OTG两用设备需要一个Standard-A插座至Mini-A插头的适配器，而且Mini-A插头连接Standard- A插座时需要一个Mini-A插座至Standard-A插头的适配器。

3 USB On-The-Go设备类型
OTG有两种设备：两用OTG设备和外设式OTG设备。两用OTG设备可以作为USB外设或 者USBOTG主机，并且可为总线提供8mA电流。而外设式OTG设备不具备主机性能，它必须在向两用设备（主机）的请求获准后才能通信。两用OTG设备必须能以 全速方式运行，而高速运行的方式是可选的。外设型OTG设备可以设计在高速、全速和低速中的任意一种方式下工作。

3.1 两用OTG设备（dual-role device）
两用OTG设备都有一个Mini-AB插槽，所以一个Mini-A至Mini-B电缆可以直接将两个两用OTG设备连接在一起，而此时用户不会觉察到两个设备的不同，也不知道它们的默认主从配置。
A-device（主机）必须在事务传输过程中提供总线电源，这是A-device和B- device的主要不同，同时也说明了两个设备的连接的非对等关系。A-device负责为总线供电因此它控制通信发生的时机，B-device只有通过SRP协议向“主机”请求传输。
设计一个两用设备是比较困难的，因为它要具备：有限的主机能力、可作为一个全速的外设（可选的高速方式）、OTG目标设备的列表、目标设备的驱动程序、支持SRP、支持HNP、一个Mini-AB插座、VBUS上不小于8mA的电流输出、与用户通信的方式。

3.2 外设式OTG设备（Peripheral-only OTG device）
外设式OTG设备是普通的USB外设。它有一个OTG功能描述符说明其支持事务请求协议SRP而且它不是两用OTG设备。此外，外设式OTG设备只能配置Mini-B型插座或者必须有一个带Mini-A插头的附属电缆，而不能使用Mini-AB型插座。
SRP是B-device（此处指外设式设备）向A-device请求传输事务时必须使用的 协议。它由前面提到的数据线脉冲调制和VBUS脉冲调制（B-device产生）两种方法构成。A-device必须能检测这两种方法之一的信号，并开始 一个相应的传输事务。除了固件（firmware）略有增加外，B-device同时可产生两种信号的成本并没有增加成本，而且还使实现A-device 更加简单（只需根据实际情况提供其中一种方式的检测）。
一个OTG设备不必实现OTG的所有性能。例如，一个外设式OTG设备可能只需支持SRP和一个小于等于8mA的默认电流消耗配置。其实，添加对SRP支持也较简单：在VBUS上接一个驱动电阻并增加一些简单的逻辑，从而产生VBUS脉冲调制信号。
另外，无论是两用OTG设备还是外设式OTG设备，都可以和通用的许机相连，只不过两用OTG设备具有一定的主机能力（可以驱动特定的一组OTG设备工作）。

1.4 驱动程序
　　与PC主机不同，便携式设备没有便捷的方式和足够的空间装载新的驱动程序。因此，OTG 规范要求每个两用OTG设备有一个支持的外设式OTG目标设备的列表，列表中包括设备的类型和制造商等信息。
与PC机不同，OTG两用设备的驱动程序栈由USB主机栈和USB设备栈构成以满足两种工作方式的需要。OTG驱动程序通过连接器的不同或者是否有NHP交换设备的工作方式来决定使用USB主机栈还是USB设备栈。
当OTG两用设备以主机方式工作时，USB主机栈工作。其中 的主机控制器驱动程序负责USB 主机栈与硬件端点的数据交换，USB驱动程序枚举并保存设备的信息，目标外设主机类驱动程序支持目标设备列表里的设备。 主机类驱动程序由芯片制造商提供，同时，OTG提供通用的主机类驱动程序（可以修改以用于非通用设备）。
当OTG两用设备以从机方式工作时，USB设备栈工作。其中的设备控制器驱动程序负责USB设备栈与硬件端点的数据交换，USB协议层负责处理USB协议规范，设备类驱动程序的功能取决于该两用设备的功能（如数码照相机、存储设备、打印机等）。
OTG驱动程序负责处理两用OTG设备的工作方式转换，同时，他还可以返回其结果（如设备是否支持HNP）并处理总线错误。应用层程序通过OTG驱动程序开始或者结束一个传输事务， 通过USB主机栈或设备栈与硬件层交换数据。

1.5 数据流模型

OTG主机和设备被划分为功能层、USB设备层和USB接口层3个不同层次，USB接口层为OTG主机和OTG设备提供物理连接；
USB 系统软件使用主机控制器来管理主机与USB设备的数据传输。USB系统软件相对于主机控制器而言，处理的是以客户角度观察的数据传输及客户与设备的交互。
USB设备层为USB主机系统软件提供一个可用的逻辑设备。主机通过与之功能匹配的客户软件实现其各种功能。
OTG设备与以往的USB设备一样有两种通道：数据流通道和消息通道。数据流通道没有定义好的结果，而消息通道则有固定的结构。但是，每个通道都有一定的带宽、传输类型、传输方向和缓冲区大小。自供电设备配置一个默认的控制通道，由他提供该设备的配置和状态等信息。

USB OTG规范是USB2.0规范的补充而不是替代品。PC主机和标准外设并没有被取代，因为新的OTG仅适用于需要具有主机功能和更小体积的便携式设备。 OTG在这些外设间引入了点对点的(point-to-point)通信方式，这使得便携式仪器的发展有了更加广阔的空间。USB OTG已受到Cypress等芯片供应商、软件开发商和设备制造商的广泛支持，OTG不久将会成为新一代的“移动计算”解决方案。