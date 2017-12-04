


/***************************************************************************************************/
1.GNSPR #139249,待机耗电】手机满电100%，插一张电信卡（卡未欠费，手机一直是搜网状态），开启数据业务，连接WIFI-AP，
待机15小时，耗电16%
{
	src_detect_handler usb插入拔出检测
	
	
	充电器的类型
	enum power_supply_type {
		POWER_SUPPLY_TYPE_UNKNOWN = 0,
		POWER_SUPPLY_TYPE_BATTERY,
		POWER_SUPPLY_TYPE_UPS,
		POWER_SUPPLY_TYPE_MAINS,
		POWER_SUPPLY_TYPE_USB,		/* Standard Downstream Port */
		POWER_SUPPLY_TYPE_USB_DCP,	/* Dedicated Charging Port */
		POWER_SUPPLY_TYPE_USB_CDP,	/* Charging Downstream Port */
		POWER_SUPPLY_TYPE_USB_ACA,	/* Accessory Charger Adapters */
		POWER_SUPPLY_TYPE_USB_HVDCP,	/* High Voltage DCP */
		POWER_SUPPLY_TYPE_USB_HVDCP_3,	/* Efficient High Voltage DCP */
		POWER_SUPPLY_TYPE_USB_PD,	/* Power Delivery */
		POWER_SUPPLY_TYPE_WIRELESS,	/* Accessory Charger Adapters */
		POWER_SUPPLY_TYPE_USB_FLOAT,	/* Floating charger */
		POWER_SUPPLY_TYPE_BMS,		/* Battery Monitor System */
		POWER_SUPPLY_TYPE_PARALLEL,	/* Parallel Path */
		POWER_SUPPLY_TYPE_MAIN,		/* Main Path */
		POWER_SUPPLY_TYPE_WIPOWER,	/* Wipower */
		POWER_SUPPLY_TYPE_TYPEC,	/* Type-C */
		POWER_SUPPLY_TYPE_UFP,		/* Type-C UFP */
		POWER_SUPPLY_TYPE_DFP,		/* TYpe-C DFP */
	};


	qpnp-power-on.c 按键id，长按powerkey重启时间
	#define QPNP_PON_S1_TIMER_MAX			10256
	#define QPNP_PON_S2_TIMER_MAX			2000
	#define QPNP_PON_S3_TIMER_SECS_MAX		128
	#define QPNP_PON_S3_DBC_DELAY_MASK		0x07
	#define QPNP_PON_RESET_TYPE_MAX			0xF


}
