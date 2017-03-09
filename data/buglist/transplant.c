/******************************************************************************************************

								关于移植方面的
*******************************************************************************************************/




#1		移植关机充电logo
{
GNSPR#53551  	电源管理：关机充电动画改善（移植充电显示相关的，BBL7332的hd720图标，S8的代码）
代码不多，也没什么变化，主要是细节（图片的差异用beyond compare，rules.mk里图片加进去的数组下标要对应，
编译烧版本要知道烧哪了，是否编译进去了）

多用beyond compare找差异

1.移植需要注意的地方（首先应该明确关机充电流程，充电logo显示，切换，要不要把开机，关机动画也顺便修改一下）

	
  hd720充电的图标--->rules.mk要加进去编译成logo.bin
	rules.mk
	把这行注释了，并加到开关宏的后面
	#$(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_kpoc_charge.raw
	然后就是图片目录要不要替换或者换个名字，把编译的名字也换一个


  lk阶段代码移植--->mt6735...编译脚本加进编译的开关宏
	
	两个条件宏GN_MTK_BSP_LK_CHARGE_GIONEELOGO，CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT,
	两个宏用了if-else条件编译
	platform.c,mt_logo.c

	gnbj6737t_66_m0.mk 但是BBL7332那两行是注释的
	#GioneeDrv LiLuBao 20161110 modify for logo change begin
	#DEFINES += GN_MTK_BSP_LK_CHARGE_GIONEELOGO
	CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT = yes
	DEFINES += CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT
	#GioneeDrv LiLuBao 20161110 modify for logo change end

	增加了G1605A.mk编译CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT


  libshowlogo目录下的代码--->G1605.mk总的编译脚本加进编译的宏
	在外部的vendor里面，改完之后还要在金立目录下提交
	lk目录下show_animation_common.c，
	external目录下show_animation_common.c，
	cust_display.h ，bootlogo.cpp  ，common.cpp，charging_animation.cpp

	gionee目录下有几个是要分层加编译选项的

	
	vendor/mediatek/proprietary/external/charger/Android.mk 脚本里面加了--->这个不需要
	#Gionee LiLuBao 20161110 modify for gioneelogo begin
	ifeq ($(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT), yes)
	LOCAL_CFLAGS += -DCONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT
	endif
	#Gionee LiLuBao 20161110 modify for gioneelogo begin

	编译的脚本改的有问题？改的一直打印不出来,这么简单的工作搞那么长时间....
	宏开关以外的也没打印？
	rules.mk	Android.mk	gnbj6737t_66_m0.mk	G1605A.mk

	单编有时候有问题，改的目录也不对，log的等级不够？
	其实是你烧错了地方

	
	a.代码相关
	CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT这个宏控制S8关机充电显示的图标

	G1605A上是这个宏GN_MTK_BSP_LK_CHARGE_GIONEELOGO			（kpoc？）

	BBL7332上相关的代码
	gionee/project/BBL7332A02_A_CTA/bootable/bootloader/lk/project/gionee6735_65c_l1.mk:#CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT = yes
	gionee/project/BBL7332A02_A_CTA/bootable/bootloader/lk/project/gionee6735_65c_l1.mk:#DEFINES += CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT
	gionee/project/BBL7332/bootable/bootloader/lk/project/gionee6735_66t_l1.mk:#CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT = yes
	gionee/project/BBL7332/bootable/bootloader/lk/project/gionee6735_66t_l1.mk:#DEFINES += CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT
	gionee/project/BBL7332/bootable/bootloader/lk/dev/logo/rules.mk~:ifeq ($(strip $(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT)), yes)
	gionee/project/BBL7332/bootable/bootloader/lk/dev/logo/rules.mk:ifeq ($(strip $(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT)), yes)
	gionee/project/BBL7332/bootable/bootloader/lk/platform/mt6735/platform.c:	#if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT)
	gionee/project/BBL7332/bootable/bootloader/lk/platform/mt6735/mt_logo.c:        #if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT)
	gionee/project/BBL7332/bootable/bootloader/lk/platform/mt6735/mt_logo.c:#if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT)
	gionee/project/BBL7332/bootable/bootloader/lk/platform/mt6735/mt_logo.c:        #if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT)
	gionee/project/BBL7332/bootable/bootloader/lk/lib/libshowlogo/show_animation_common.c~:#if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT)
	gionee/project/BBL7332/bootable/bootloader/lk/lib/libshowlogo/show_animation_common.c:#if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT)
	gionee/project/BBL7332/vendor/mediatek/proprietary/external/libshowlogo/show_animation_common.c:#if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT)
	gionee/project/BBL7332/vendor/mediatek/proprietary/external/libshowlogo/show_animation_common.c:    #ifndef CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT 
	gionee/project/BBL7332/vendor/mediatek/proprietary/external/libshowlogo/cust_display.h:#if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT)
	gionee/project/BBL7332/vendor/mediatek/proprietary/external/charger/bootlogo.cpp:	#if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT) 
	gionee/project/BBL7332/vendor/mediatek/proprietary/external/charger/common.cpp:	#if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT) 
	gionee/project/BBL7332A02_A/bootable/bootloader/lk/project/gionee6735_65c_l1.mk:#CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT = yes
	gionee/project/BBL7332A02_A/bootable/bootloader/lk/project/gionee6735_65c_l1.mk:#DEFINES += CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT

	gionee/alps_drv/bootable/bootloader/lk/dev/logo/rules.mk:ifeq ($(strip $(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT)), yes)
	gionee/alps_drv/bootable/bootloader/lk/platform/mt6735/platform.c:            #if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT)
	gionee/alps_drv/bootable/bootloader/lk/platform/mt6735/mt_logo.c:        #if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT)
	gionee/alps_drv/bootable/bootloader/lk/platform/mt6735/mt_logo.c:#if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT)
	gionee/alps_drv/bootable/bootloader/lk/platform/mt6735/mt_logo.c:        #if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT)
	gionee/alps_drv/bootable/bootloader/lk/lib/libshowlogo/show_animation_common.c:#if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT)
	gionee/alps_drv/vendor/mediatek/proprietary/external/libshowlogo/show_animation_common.h:#if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT)                        
	gionee/alps_drv/vendor/mediatek/proprietary/external/libshowlogo/show_animation_common.c:#if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT)
	gionee/alps_drv/vendor/mediatek/proprietary/external/libshowlogo/show_animation_common.c:    #ifndef CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT 
	gionee/alps_drv/vendor/mediatek/proprietary/external/libshowlogo/charging_animation.cpp:#if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT)
	gionee/alps_drv/vendor/mediatek/proprietary/external/libshowlogo/charging_animation.cpp:    	#if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT)
	gionee/alps_drv/vendor/mediatek/proprietary/external/libshowlogo/charging_animation.cpp:		#if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT)
	gionee/alps_drv/vendor/mediatek/proprietary/external/libshowlogo/charging_animation.h:#if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT)
	gionee/alps_drv/vendor/mediatek/proprietary/external/charger/bootlogo.cpp:	#if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT) 
	gionee/alps_drv/vendor/mediatek/proprietary/external/charger/common.cpp:	#if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT) 
	gionee/alps_drv/vendor/mediatek/proprietary/external/ipod/ipodcommon.cpp:	#if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT) 
	gionee/alps_drv/vendor/mediatek/proprietary/external/ipod/bootlogo.cpp:	#if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT) 


	/*
	 * Check logo.bin address if valid, and get logo related info
	 * pinfo[0] : the number of all pictures in logo.bin
	 * pinf0[1] : the whole length of logo.bin
	 * pinf0[2] : the index = 0 picture's length
	 * pinfo[3+index] - pinfo[2+index] : means index length
	 */


	logo index 反应不同状态logo，数组下标
	// Common LOGO index
	#define BOOT_LOGO_INDEX   0 
	#define KERNEL_LOGO_INDEX   38 

	#define ANIM_V0_BACKGROUND_INDEX   1 
	#define ANIM_V1_BACKGROUND_INDEX   35
	 
	 
	#define LOW_BATTERY_INDEX   2 
	#define CHARGER_OV_INDEX   3 
	#define FULL_BATTERY_INDEX   37 


	这是后又来了一个问题就是patch，补丁？


3.总结，这次移植用了这么长时间，好心酸.....，最后发现图片压缩的一个脚本有问题
	
	a.有时间把整个流程走一遍？
	logo.bin怎么生成的，boot_logo,kernel_logo怎么调用的，从哪开始调用的，动画（应该是以一定频率播放图片），fstab分区，电量的百分比变化


	b.关机充电的流程，充电动画
	
		1、正常开机流程，按开机键。

		可大致分成三部分

		（1）OS_level：UBOOT、kenrel、init这三步完成系统启动；

		（2）Android_level：这部分完成android部的初始化；

		（3）Home Screen：这部分就是我们看到的launcher部分。
	
		2.关机状态下，只走到init阶段
	


	c.关机状态下主要涉及两个过程
	lk阶段：
		显示uboot的logo，这个过程有一个检测的过程，在platform_init()函数中进行一定的初始化包括LCM背光，对电池状态的检测，充电类型的检测
	，调用mt_disp_show_kpoc_charge_logo()进行关机充电在lk阶段的初始化调用，显示logo就是用framebuffer的方式对不同的像素点用RGB不同的比例显示出来
	然后按一定频率刷新屏幕，显示不同的图片。最终显示的画面是不同图片的组合
		
	system阶段
		对电池状态进行检测,电池处于什么样的状态，显示什么百分比，动画图标的变换










相关的几个代码
*******************************************************************************************************************************************

	这其中有这几个函数，涉及到最后显示出来的效果framebuffer的处理
/**********************************************************************************************************************************
 * Fill a screen size buffer with logo content
 *********************************************************************************************************************************/
void fill_animation_logo(unsigned int index, void *fill_addr, void * dec_logo_addr, void * logo_addr, LCM_SCREEN_T phical_screen)
{
    LOGO_PARA_T logo_info;
    int logo_width;
    int logo_height;
    int raw_data_size;
    if(check_logo_index_valid(index, logo_addr, &logo_info) != CHECK_LOGO_BIN_OK)
        return;

    raw_data_size = decompress_logo((void*)logo_info.inaddr, dec_logo_addr, logo_info.logolen, phical_screen.fb_size);
    //RECT_REGION_T rect = {0, 0, phical_screen.width, phical_screen.height};
    logo_width =phical_screen.width;
    logo_height = phical_screen.height;
    if (phical_screen.rotation == 270 || phical_screen.rotation == 90) {
        logo_width =phical_screen.height;
        logo_height = phical_screen.width;
    }
    if (0 == bits) {
        if (raw_data_size == logo_width*logo_height*4) {
            bits = 32;
        } else if (raw_data_size == logo_width*logo_height*2) {
            bits = 16;
        } else {
            LOG_ANIM("[show_animation_common: %s %d]Logo data error\n",__FUNCTION__,__LINE__);
            return;
        }
        LOG_ANIM("[show_animation_common: %s %d]bits = %d\n",__FUNCTION__,__LINE__, bits);
    }

    RECT_REGION_T rect = {0, 0, logo_width, logo_height};

    fill_rect_with_content(fill_addr, rect, dec_logo_addr, phical_screen, bits);


}
	

/***********************************************************************************************************************************
 * Draw a rectangle region  with logo content
 **********************************************************************************************************************************/
void fill_rect_with_content(void *fill_addr, RECT_REGION_T rect, void *src_addr, LCM_SCREEN_T phical_screen, unsigned int bits)
{
	SLOGD("[show_logo_common: %s %d]\n",__FUNCTION__,__LINE__);
    if (check_rect_valid(rect) != CHECK_RECT_OK)
        return;
    if (bits == 32) {
        if (phical_screen.fill_dst_bits == 16) {
            fill_rect_with_content_by_16bit_argb8888((unsigned short *)fill_addr, rect, (unsigned int *)src_addr, phical_screen);
        } else if (phical_screen.fill_dst_bits == 32){
            fill_rect_with_content_by_32bit_argb8888((unsigned int *)fill_addr, rect, (unsigned int *)src_addr, phical_screen, bits);
        } else {
            SLOGD("[show_logo_common %s %d]unsupported phical_screen.fill_dst_bits =%d\n",__FUNCTION__,__LINE__, phical_screen.fill_dst_bits );
        }
    } else {
        if (phical_screen.fill_dst_bits == 16) {
            fill_rect_with_content_by_16bit_rgb565((unsigned short *)fill_addr, rect, (unsigned short *)src_addr, phical_screen);
        } else if (phical_screen.fill_dst_bits == 32){
            fill_rect_with_content_by_32bit_rgb565((unsigned int *)fill_addr, rect, (unsigned short *)src_addr, phical_screen, bits);
        } else {
            SLOGD("[show_logo_common %s %d]unsupported phical_screen.fill_dst_bits =%d\n",__FUNCTION__,__LINE__, phical_screen.fill_dst_bits );
        }
}








}

	
	
	
	
	
	
	

#2    移植充电相关的代码到海外版上，从android M到android N上
{
	具体17W05A移植.txt，这个移植的过程中应该说dws和gpio的配置是需要掌握的？？
	
	
	
	8.BUG#66789
	手机为开机状态，长按电源键11s，手机不会自动重启	
	hal_kpd.c   keypad.h	gnbj6737t_66_m0_debug_defconfig   gnbj6737t_66_m0_defconfig
	
	CONFIG_ONEKEY_REBOOT_NORMAL_MODE=y
	CONFIG_ONEKEY_REBOOT_OTHER_MODE=y
	
	home键one，组合键two
	
	长按power键,reboot or shudown
	cust_rtc.h
	
}












#3	移植OTG开关到BBL7337上
{
		
	相关的宏开关：
	
		GN_BATTERY_OTGCHARGE_SWITCH_SUPPORT
		
		这个宏是不需要的，7515OTG反向充电提示
		GN_BATTERY_OTGCHARGE_NOTIFY_ENABLE
		
	关键词：	
		"OTG SWITCH"
		
	设备节点：	
		/sys/devices/platform/battery/Otg_Charge_Switch
		
		
		
	项目脚本：
		gnbj6750_66_m_debug_defconfig	脚本里面不用加入编译的宏开关
		
		充电芯片不一样，充电算法也不一样
		GM2.0	,bq24196
		./drivers/misc/mediatek/power/mt6755/bq24196.c
		./drivers/misc/mediatek/power/mt6755/bq24196.h
		
		./code/driver/drv_common/vendor/mediatek/proprietary/bootable/bootloader/lk/platform/mt6755/bq25890.c
		./code/driver/drv_common/vendor/mediatek/proprietary/bootable/bootloader/lk/platform/mt6755/include/platform/bq25890.h

		
		gpio引脚配置，iddig_gpio是23?
		{
			init.mt6755.rc,gnbj6755_66t_m.dts,gnbj6755_66_m.dts
			,xhci-mtk-driver.c,musb_init.c,
			battery_common.h,battery_common.c,xhci-mtk-driver.c,charging.h,
			com_android_server_AmigoServerManagerService.cpp,AmigoServerManager.java
			
			battery_common.c  7337是battery_common_fg_20.c
		
			#if defined(CONFIG_MTK_BQ24196_SUPPORT)
			//GioneeDrv GuoJianqiu 20160425 modify for OTG function begin
			bq24196_set_otg_config(0x01);	/* OTG */
			bq24196_set_boost_lim(0x01);	/* 1.3A on VBUS */
			bq24196_set_en_hiz(0x0);
			//GioneeDrv GuoJianqiu 20160425 modify for OTG function end
			#endif
			
			
			#if defined(CONFIG_MTK_BQ25896_SUPPORT)
			//GioneeDrv GuoJianqiu 20160425 modify for OTG function begin
			#if defined(CONFIG_GN_DUBLE_CHARGE_IC_SUPPORT)
			bq25891_set_en_hiz(0x01);
			#endif
			bq25890_otg_en(0x01);
			bq25890_set_boost_ilim(0x05);	/* 1.875A */
			bq25890_set_boost_vlim(0x0C);	/* 5.318V */
			bq25890_set_en_hiz(0x00);
			//GioneeDrv GuoJianqiu 20160425 modify for OTG function end
			#elif defined(CONFIG_MTK_OTG_PMIC_BOOST_5V)
			mtk_enable_pmic_otg_mode();
			#endif
			
			gn_otg_charge_switch_State
		}
	
	


	BBL7515:
		{
			
			./code/driver/project_common/BBL7515_DRV_COMMON/device/mediatek/mt6755/init.mt6755.rc:565:    # Gionee GuoJianqiu 20160704 modify for OTG SWITCH begin
			./code/driver/project_common/BBL7515_DRV_COMMON/device/mediatek/mt6755/init.mt6755.rc:567:    # Gionee GuoJianqiu 20160704 modify for OTG SWITCH end
			
			./code/driver/project_common/BBL7515_DRV_COMMON/kernel-3.18/arch/arm64/boot/dts/gnbj6755_66t_m.dts:1196://Gionee <GN_BSP_OTG> <guojq> <20161126> modify for OTG SWITCH begin	
			./code/driver/project_common/BBL7515_DRV_COMMON/kernel-3.18/arch/arm64/boot/dts/gnbj6755_66t_m.dts:1204://Gionee <GN_BSP_OTG> <guojq> <20161126> modify for OTG SWITCH end
			./code/driver/project_common/BBL7515_DRV_COMMON/kernel-3.18/arch/arm64/boot/dts/gnbj6755_66t_m.dts:1229://Gionee <GN_BSP_OTG> <guojq> <20161126> modify for OTG SWITCH begin
			./code/driver/project_common/BBL7515_DRV_COMMON/kernel-3.18/arch/arm64/boot/dts/gnbj6755_66t_m.dts:1237://Gionee <GN_BSP_OTG> <guojq> <20161126> modify for OTG SWITCH end
			./code/driver/project_common/BBL7515_DRV_COMMON/kernel-3.18/arch/arm64/boot/dts/gnbj6755_66_m.dts:1196://Gionee <GN_BSP_OTG> <guojq> <20161126> modify for OTG SWITCH begin	
			./code/driver/project_common/BBL7515_DRV_COMMON/kernel-3.18/arch/arm64/boot/dts/gnbj6755_66_m.dts:1204://Gionee <GN_BSP_OTG> <guojq> <20161126> modify for OTG SWITCH end
			./code/driver/project_common/BBL7515_DRV_COMMON/kernel-3.18/arch/arm64/boot/dts/gnbj6755_66_m.dts:1229://Gionee <GN_BSP_OTG> <guojq> <20161126> modify for OTG SWITCH begin
			./code/driver/project_common/BBL7515_DRV_COMMON/kernel-3.18/arch/arm64/boot/dts/gnbj6755_66_m.dts:1237://Gionee <GN_BSP_OTG> <guojq> <20161126> modify for OTG SWITCH end
			
			
			
			./code/driver/project_common/BBL7515_DRV_COMMON/kernel-3.18/drivers/misc/mediatek/xhci/xhci-mtk-driver.c:105://Gionee GuoJianqiu 201601026 modify for OTG SWITCH begin
			./code/driver/project_common/BBL7515_DRV_COMMON/kernel-3.18/drivers/misc/mediatek/xhci/xhci-mtk-driver.c:117://Gionee GuoJianqiu 201601026 modify for OTG SWITCH end
			./code/driver/project_common/BBL7515_DRV_COMMON/kernel-3.18/drivers/misc/mediatek/xhci/xhci-mtk-driver.c:133:	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH begin
			./code/driver/project_common/BBL7515_DRV_COMMON/kernel-3.18/drivers/misc/mediatek/xhci/xhci-mtk-driver.c:137:	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH end
			./code/driver/project_common/BBL7515_DRV_COMMON/kernel-3.18/drivers/misc/mediatek/xhci/xhci-mtk-driver.c:144:	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH begin
			./code/driver/project_common/BBL7515_DRV_COMMON/kernel-3.18/drivers/misc/mediatek/xhci/xhci-mtk-driver.c:149:	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH end
			./code/driver/project_common/BBL7515_DRV_COMMON/kernel-3.18/drivers/misc/mediatek/xhci/xhci-mtk-driver.c:157:	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH begin
			./code/driver/project_common/BBL7515_DRV_COMMON/kernel-3.18/drivers/misc/mediatek/xhci/xhci-mtk-driver.c:161:	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH end
			./code/driver/project_common/BBL7515_DRV_COMMON/kernel-3.18/drivers/misc/mediatek/xhci/xhci-mtk-driver.c:168:	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH begin
			./code/driver/project_common/BBL7515_DRV_COMMON/kernel-3.18/drivers/misc/mediatek/xhci/xhci-mtk-driver.c:173:	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH end
			./code/driver/project_common/BBL7515_DRV_COMMON/kernel-3.18/drivers/misc/mediatek/xhci/xhci-mtk-driver.c:186:	//Gionee GuoJianqiu 20160704 modify for OTG SWITCH begin
			./code/driver/project_common/BBL7515_DRV_COMMON/kernel-3.18/drivers/misc/mediatek/xhci/xhci-mtk-driver.c:192:	//Gionee GuoJianqiu 20160704 modify for OTG SWITCH end
			./code/driver/project_common/BBL7515_DRV_COMMON/kernel-3.18/drivers/misc/mediatek/xhci/xhci-mtk-driver.c:453:	//Gionee GuoJianqiu 20160704 modify for OTG SWITCH begin
			./code/driver/project_common/BBL7515_DRV_COMMON/kernel-3.18/drivers/misc/mediatek/xhci/xhci-mtk-driver.c:457:	//Gionee GuoJianqiu 20160704 modify for OTG SWITCH end
			./code/driver/project_common/BBL7515_DRV_COMMON/kernel-3.18/drivers/misc/mediatek/xhci/xhci-mtk-driver.c:470:	//Gionee GuoJianqiu 20160704 modify for OTG SWITCH begin
			./code/driver/project_common/BBL7515_DRV_COMMON/kernel-3.18/drivers/misc/mediatek/xhci/xhci-mtk-driver.c:474:	//Gionee GuoJianqiu 20160704 modify for OTG SWITCH end
			./code/driver/project_common/BBL7515_DRV_COMMON/kernel-3.18/drivers/misc/mediatek/xhci/xhci-mtk-driver.c:685://Gionee GuoJianqiu 201601026 modify for OTG SWITCH begin
			./code/driver/project_common/BBL7515_DRV_COMMON/kernel-3.18/drivers/misc/mediatek/xhci/xhci-mtk-driver.c:745://Gionee GuoJianqiu 201601026 modify for OTG SWITCH end
			./code/driver/project_common/BBL7515_DRV_COMMON/kernel-3.18/drivers/misc/mediatek/xhci/xhci-mtk-driver.c:794:	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH begin
			./code/driver/project_common/BBL7515_DRV_COMMON/kernel-3.18/drivers/misc/mediatek/xhci/xhci-mtk-driver.c:814:	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH end
			./code/driver/project_common/BBL7515_DRV_COMMON/kernel-3.18/drivers/misc/mediatek/mu3d/drv/musb_init.c:84://Gionee GuoJianqiu 201601026 modify for OTG SWITCH begin
			./code/driver/project_common/BBL7515_DRV_COMMON/kernel-3.18/drivers/misc/mediatek/mu3d/drv/musb_init.c:94://Gionee GuoJianqiu 201601026 modify for OTG SWITCH end
			./code/driver/project_common/BBL7515_DRV_COMMON/kernel-3.18/drivers/misc/mediatek/mu3d/drv/musb_init.c:833:		//Gionee GuoJianqiu 201601026 modify for OTG SWITCH begin
			./code/driver/project_common/BBL7515_DRV_COMMON/kernel-3.18/drivers/misc/mediatek/mu3d/drv/musb_init.c:845:		//Gionee GuoJianqiu 201601026 modify for OTG SWITCH end		
			
			./code/driver/project_common/BBL7515_DRV_COMMON/kernel-3.18/drivers/power/mediatek/battery_common.c:103://Gionee GuoJianqiu 20160704 modify for OTG SWITCH begin
			./code/driver/project_common/BBL7515_DRV_COMMON/kernel-3.18/drivers/power/mediatek/battery_common.c:110://Gionee GuoJianqiu 20160704 modify for OTG SWITCH end
			./code/driver/project_common/BBL7515_DRV_COMMON/kernel-3.18/drivers/power/mediatek/battery_common.c:458:		//Gionee GuoJianqiu 20160704 modify for OTG SWITCH begin
			./code/driver/project_common/BBL7515_DRV_COMMON/kernel-3.18/drivers/power/mediatek/battery_common.c:464:		//Gionee GuoJianqiu 20160704 modify for OTG SWITCH end
			./code/driver/project_common/BBL7515_DRV_COMMON/kernel-3.18/drivers/power/mediatek/battery_common.c:1814://Gionee GuoJianqiu 20160704 modify for OTG SWITCH begin
			./code/driver/project_common/BBL7515_DRV_COMMON/kernel-3.18/drivers/power/mediatek/battery_common.c:1945://Gionee GuoJianqiu 20160704 modify for OTG SWITCH end
			./code/driver/project_common/BBL7515_DRV_COMMON/kernel-3.18/drivers/power/mediatek/battery_common.c:1947://Gionee GuoJianqiu 201601026 modify for OTG SWITCH begin
			./code/driver/project_common/BBL7515_DRV_COMMON/kernel-3.18/drivers/power/mediatek/battery_common.c:1987://Gionee GuoJianqiu 201601026 modify for OTG SWITCH end
			./code/driver/project_common/BBL7515_DRV_COMMON/kernel-3.18/drivers/power/mediatek/battery_common.c:2934:	//Gionee GuoJianqiu 20160704 modify for OTG SWITCH begin
			./code/driver/project_common/BBL7515_DRV_COMMON/kernel-3.18/drivers/power/mediatek/battery_common.c:2940:	//Gionee GuoJianqiu 20160704 modify for OTG SWITCH end
			./code/driver/project_common/BBL7515_DRV_COMMON/kernel-3.18/drivers/power/mediatek/battery_common.c:3516:		//Gionee GuoJianqiu 20160704 modify for OTG SWITCH begin
			./code/driver/project_common/BBL7515_DRV_COMMON/kernel-3.18/drivers/power/mediatek/battery_common.c:3525:		//Gionee GuoJianqiu 20160704 modify for OTG SWITCH end
			./code/driver/project_common/BBL7515_DRV_COMMON/kernel-3.18/drivers/power/mediatek/battery_common.c:5051:		//Gionee GuoJianqiu 20160704 modify for OTG SWITCH begin
			./code/driver/project_common/BBL7515_DRV_COMMON/kernel-3.18/drivers/power/mediatek/battery_common.c:5055:		//Gionee GuoJianqiu 20160704 modify for OTG SWITCH end
			./code/driver/project_common/BBL7515_DRV_COMMON/kernel-3.18/drivers/power/mediatek/battery_common.c:5057:		//Gionee GuoJianqiu 201601026 modify for OTG SWITCH begin
			./code/driver/project_common/BBL7515_DRV_COMMON/kernel-3.18/drivers/power/mediatek/battery_common.c:5061:		//Gionee GuoJianqiu 201601026 modify for OTG SWITCH end

			
			"/sys/devices/platform/battery/Otg_Charge_Switch",?
			"/sys/devices/platform/battery/Otg_Charge_State",
			NODE_TYPE_OTG_CHARGE_SWITCH,?
			public static final int NODE_TYPE_OTG_CHARGE_SWITCH = 61;  ?
			
			
			./code/alps/public/ROM/frameworks/base/services/core/jni/com_android_server_AmigoServerManagerService.cpp:116:/*< Gionee yinlf Otg_Charge 20160702 begin*/
			./code/alps/public/ROM/frameworks/base/services/core/jni/com_android_server_AmigoServerManagerService.cpp:117:"/sys/devices/platform/battery/Otg_Charge_State",
			./code/alps/public/ROM/frameworks/base/services/core/jni/com_android_server_AmigoServerManagerService.cpp:118:/* Gionee yinlf Otg_Charge 20160702 end >*/
			./code/alps/public/ROM/frameworks/base/services/core/jni/com_android_server_AmigoServerManagerService.cpp:201:/*< Gionee yinlf OTG_CHARGE 20160702 begin*/
			./code/alps/public/ROM/frameworks/base/services/core/jni/com_android_server_AmigoServerManagerService.cpp:202:NODE_TYPE_OTG_CHARGE,
			./code/alps/public/ROM/frameworks/base/services/core/jni/com_android_server_AmigoServerManagerService.cpp:203:/* Gionee yinlf OTG_CHARGE 20160702 end >*/
			./code/alps/public/ROM/frameworks/base/core/java/android/os/amigoserver/AmigoServerManager.java:115:    /*<Gionee yinlf OTG_CHARGE 20160702 begin*/
			./code/alps/public/ROM/frameworks/base/core/java/android/os/amigoserver/AmigoServerManager.java:116:    public static final int NODE_TYPE_OTG_CHARGE = 61;                          ///sys/devices/platform/battery/Otg_Charge_State
			./code/alps/public/ROM/frameworks/base/core/java/android/os/amigoserver/AmigoServerManager.java:117:    /*Gionee yinlf OTG_CHARGE 20160702 end >*/		
	
							
			./code/driver/project_common/BBL7516_DRV_COMMON/kernel-3.18/drivers/misc/mediatek/include/mt-plat/mt6755/include/mach/mt_charging.h:115://Gionee GuoJianqiu 20160704 modify for OTG SWITCH end
			./code/driver/project_common/BBL7516_DRV_COMMON/kernel-3.18/drivers/power/mediatek/battery_common.c:103://Gionee GuoJianqiu 20160704 modify for OTG SWITCH begin
			./code/driver/project_common/BBL7516_DRV_COMMON/kernel-3.18/drivers/power/mediatek/battery_common.c:110://Gionee GuoJianqiu 20160704 modify for OTG SWITCH end
			./code/driver/project_common/BBL7516_DRV_COMMON/kernel-3.18/drivers/power/mediatek/battery_common.c:458:		//Gionee GuoJianqiu 20160704 modify for OTG SWITCH begin
			./code/driver/project_common/BBL7516_DRV_COMMON/kernel-3.18/drivers/power/mediatek/battery_common.c:464:		//Gionee GuoJianqiu 20160704 modify for OTG SWITCH end
			./code/driver/project_common/BBL7516_DRV_COMMON/kernel-3.18/drivers/power/mediatek/battery_common.c:1814://Gionee GuoJianqiu 20160704 modify for OTG SWITCH begin
			./code/driver/project_common/BBL7516_DRV_COMMON/kernel-3.18/drivers/power/mediatek/battery_common.c:1945://Gionee GuoJianqiu 20160704 modify for OTG SWITCH end
			./code/driver/project_common/BBL7516_DRV_COMMON/kernel-3.18/drivers/power/mediatek/battery_common.c:1947://Gionee GuoJianqiu 201601026 modify for OTG SWITCH begin
			./code/driver/project_common/BBL7516_DRV_COMMON/kernel-3.18/drivers/power/mediatek/battery_common.c:1987://Gionee GuoJianqiu 201601026 modify for OTG SWITCH end
			./code/driver/project_common/BBL7516_DRV_COMMON/kernel-3.18/drivers/power/mediatek/battery_common.c:2934:	//Gionee GuoJianqiu 20160704 modify for OTG SWITCH begin
			./code/driver/project_common/BBL7516_DRV_COMMON/kernel-3.18/drivers/power/mediatek/battery_common.c:2940:	//Gionee GuoJianqiu 20160704 modify for OTG SWITCH end
			./code/driver/project_common/BBL7516_DRV_COMMON/kernel-3.18/drivers/power/mediatek/battery_common.c:3516:		//Gionee GuoJianqiu 20160704 modify for OTG SWITCH begin
			./code/driver/project_common/BBL7516_DRV_COMMON/kernel-3.18/drivers/power/mediatek/battery_common.c:3525:		//Gionee GuoJianqiu 20160704 modify for OTG SWITCH end
			./code/driver/project_common/BBL7516_DRV_COMMON/kernel-3.18/drivers/power/mediatek/battery_common.c:5051:		//Gionee GuoJianqiu 20160704 modify for OTG SWITCH begin
			./code/driver/project_common/BBL7516_DRV_COMMON/kernel-3.18/drivers/power/mediatek/battery_common.c:5055:		//Gionee GuoJianqiu 20160704 modify for OTG SWITCH end
			./code/driver/project_common/BBL7516_DRV_COMMON/kernel-3.18/drivers/power/mediatek/battery_common.c:5057:		//Gionee GuoJianqiu 201601026 modify for OTG SWITCH begin
			./code/driver/project_common/BBL7516_DRV_COMMON/kernel-3.18/drivers/power/mediatek/battery_common.c:5061:		//Gionee GuoJianqiu 201601026 modify for OTG SWITCH end
		
		
		}

			



}




