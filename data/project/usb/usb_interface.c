
 USB 配置，接口，设置，endpoint描述符的关系


一个USB设置可以有多个配置，在初始化的过程中，会选择一个默认的配置，选择的方法根据驱动实现而不一样，linux usb会选择第一个标准的配置。
	else if (udev->descriptor.bDeviceClass !=
		USB_CLASS_VENDOR_SPEC &&
		(desc && desc->bInterfaceClass !=
		USB_CLASS_VENDOR_SPEC)) {
		best = c;
		break;
	}
	
比较的是配置里的class,还有配置第一个接口的第一个设置的class。如果不是VENDOR自己定义的class，也就是标准的class，linux就会选择其为默认的configuration.

一个配置可以有多个接口，一个接口会有多个设置。看了USB2.0标准spec后，发现只有interface的描述符，而没有设置的描述符，这是为啥呢？
在linux kernel里，接口用struct usb_interface来表示，接口里的设置是用struct usb_host_interface来表示。既然一个接口里包含多个设置，那我们看看接口的定义。

struct usb_interface {
	/* array of alternate settings for this interface,
	 * stored in no particular order */
	struct usb_host_interface *altsetting;

	struct usb_host_interface *cur_altsetting; /* the currently
	 * active alternate setting */
	unsigned num_altsetting; /* number of alternate settings */

	/* If there is an interface association descriptor then it will list
	 * the associated interfaces */
	struct usb_interface_assoc_descriptor *intf_assoc;

	int minor; /* minor number this interface is
	 * bound to */
	enum usb_interface_condition condition; /* state of binding */
	unsigned sysfs_files_created:1; /* the sysfs attributes exist */
	unsigned ep_devs_created:1; /* endpoint "devices" exist */
	unsigned unregistering:1; /* unregistration is in progress */
	unsigned needs_remote_wakeup:1; /* driver requires remote wakeup */
	unsigned needs_altsetting0:1; /* switch to altsetting 0 is pending */
	unsigned needs_binding:1; /* needs delayed unbind/rebind */
	unsigned reset_running:1;
	unsigned resetting_device:1; /* true: bandwidth alloc after reset */

	struct device dev; /* interface specific device info */
	struct device *usb_dev;
	atomic_t pm_usage_cnt; /* usage counter for autosuspend */
	struct work_struct reset_ws; /* for resets in atomic context */
};
 
红色部分就是接口里的设置，usb2.0标准里的interface描述符，其实是包含在接口里，也就是struct usb_host_interface里。主机在读取设备、配置、
接口、端点描述符之后 根据配置描述符中的bNumInterface接口数量生成对应数量的struct usb_interface，然后根据usb2.0标准里的
interface描述符生成对应的struct usb_host_interface，其中usb2.0标准里的interface描述符中的bInterfaceNumber，指的就是这个设置是属于哪 个接口，
也就是说生成的struct usb_host_interface属于哪个struct usb_interface，usb2.0标准里的interface描述符中的bAlternateSetting字段用于存放自己的
识别信息，即用于区分同一接口（struct usb_interfac）中的不同设置（struct usb_host_interface）
usb驱动中的prob disconnetc函数的参数都是针对与于usb接口的即struct usb_interface *interface 然后在函数中去遍历接口中的所有设置
在struct usb_host_interface结构体中再遍历此设置下的所有端口

那这到底是什么意思呢？下面是usb2.0协议里的一段话。

If a device configuration supported a single interface with two alternate settings, the configuration
descriptor would be followed by an interface descriptor with the bInterfaceNumber and bAlternateSetting
fields set to zero and then the endpoint descriptors for that setting, followed by another interface descriptor
and its associated endpoint descriptors. The second interface descriptor’s bInterfaceNumber field would
also be set to zero, but the bAlternateSetting field of the second interface descriptor would be set to one.

事实上这个interface描述符，就是设置描述符，这个描述符的其中一项叫做bInterfaceNumber，指的就是这个设置是属于哪 个接口。比如一个接口包含2个设置，那么就会有2个interface描述符，两个描述符里的bInterfaceNumber设置都为0，但是第一个设 置的bAlternateSetting为0， 第二个设置的bAlternateSetting为1。这样就区分开来了。

假如一共有5个设置，2个接口。这5个设置，2个属于接口1， 3个属于接口2.
则就会有2个struct usb_interface,第一个usb_interface里的altsetting有2个，第二个有3个。
协议里规定，接口的默认设置总是0号设置。


至于endpoint描述符，它是属于设置的，每个设置都会有endpoint描述符，也就是每个接口的设置都表示一种功能，既然是实现了功 能，那就必须通过endpoint来传输数据，那到底是用到了几个endpoint呢？每个endpoint的属性是？所以每个接口的设置里就会有 endpoint的描述符。

endpoint有个特性，要么只能input,要么只能output,但是也有一个特殊的endpoint，就是endpoint 0,每个USB设备都有这么一个endpoint0,除此endpoint，其他都只能单工的。而且每个endpoint每次传输的大小也不一样，所以就 有了这么个endpoint描述符来对其进行描述。


这里比较费解的就是接口和设置的关系，现在这么一分析就清楚多了。
