/***************************************************************************************************************
								一直充电代码到17W05A
***************************************************************************************************************/

1.charging

快充接口
	#include "mtk_pep_intf.h"
	#include "mtk_pep20_intf.h"

	#if defined(CONFIG_MTK_PUMP_EXPRESS_SUPPORT) || defined(CONFIG_MTK_PUMP_EXPRESS_PLUS_SUPPORT)
	#ifndef PUMP_EXPRESS_SERIES
	#define PUMP_EXPRESS_SERIES
	#endif
	#endif

	#if defined(CONFIG_MTK_PUMP_EXPRESS_PLUS_20_SUPPORT)
	#ifndef PUMP_EXPRESS_SERIES
	#define PUMP_EXPRESS_SERIES
	#endif
	#endif


	
						
lk阶段：



G1605A
	./project/gnbj6737t_66_m0.mk:#GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./project/gnbj6737t_66_m0.mk:#GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./platform/mt6735/mt_pmic.c://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./platform/mt6735/mt_pmic.c://GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./platform/mt6735/mt_battery.c://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./platform/mt6735/mt_battery.c://GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./platform/mt6735/mt_battery.c://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./platform/mt6735/mt_battery.c://GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./platform/mt6735/mt_battery.c:		//GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./platform/mt6735/mt_battery.c:		//GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./platform/mt6735/mt_battery.c:		//GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./platform/mt6735/mt_battery.c:		//GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./platform/mt6735/mt_battery.c:	//GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./platform/mt6735/mt_battery.c:	//GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./platform/mt6735/mt_battery.c://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./platform/mt6735/mt_battery.c://GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./platform/mt6735/bq24158.c:#define HIGH_BATTERY_VOLTAGE_SUPPORT 	//Gionee GuoJianqiu 20150408 modify for fix bugs begin
	./platform/mt6735/bq24158.c:#define PRECC_BATVOL 2800 //preCC 2.8V 	//Gionee GuoJianqiu 20150430 modify for GNNCR00010432 begin
	./platform/mt6735/bq24158.c:#define BQ24158_I2C_ID	I2C2 	 //Gionee GuoJianqiu 20150407 modify for fix bugs begin
	./platform/mt6735/bq24158.c:static CHARGER_TYPE g_chr_type_num = CHARGER_UNKNOWN; 	//Gionee GuoJianqiu 20150430 modify for GNNCR00010432 begin
	./platform/mt6735/bq24158.c:int hw_charging_get_charger_type(void); 			//Gionee GuoJianqiu 20150430 modify for GNNCR00010432 end
	./platform/mt6735/bq24158.c:    int temp_CC_value = 0; 	//Gionee GuoJianqiu 20150430 modify for GNNCR00010432 begin
	./platform/mt6735/bq24158.c:    kal_int32 bat_val = 0; 		//Gionee GuoJianqiu 20150430 modify for GNNCR00010432 end
	./platform/mt6735/bq24158.c://Gionee GuoJianqiu 20150430 modify for GNNCR00010432 begin
	./platform/mt6735/bq24158.c://Gionee GuoJianqiu 20150430 modify for GNNCR00010432 end
	./platform/mt6735/bq24158.c:    printf("[BATTERY:bq24158] charger enable/disable %d , CC value(%dmA)\r\n", bEnable, temp_CC_value); 	//Gionee GuoJianqiu 20150430 modify for GNNCR00010432 begin
	./platform/mt6735/bq24158.c://Gionee GuoJianqiu 20150430 modify for GNNCR00010432 begin
	./platform/mt6735/bq24158.c://Gionee GuoJianqiu 20150324 modify for fix errs begin        
	./platform/mt6735/bq24158.c://Gionee GuoJianqiu 20150324 modify for fix errs end
	./platform/mt6735/bq24158.c://Gionee GuoJianqiu 20150430 modify for GNNCR00010432 end
	./platform/mt6735/platform.c:    //GioneeDrv GuoJianqiu 20160618 modify for  GNSPR #15455 begin
	./platform/mt6735/platform.c:    //GioneeDrv GuoJianqiu 20160618 modify for  GNSPR #15455 end

	./platform/mt6735/rules.mk:        #GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./platform/mt6735/rules.mk:        #GioneeDrv GuoJianqiu 20151223 modify for platform change end






mt_pmic.c,mt_battery.c,bq24158.c,platform.c,mt_logo.c,gnbj6737t_66_m0.mk,rules.mk




17W05A	

	mt_pmic.c,mt_battery.c,bq24158.c,platform.c





preloader阶段：
	
G1605A
	./platform/mt6735/src/drivers/pmic.c://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./platform/mt6735/src/drivers/pmic.c://GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./platform/mt6735/src/drivers/pmic.c:                //GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./platform/mt6735/src/drivers/pmic.c:                //GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./platform/mt6735/src/drivers/inc/keypad.h://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./platform/mt6735/src/drivers/inc/keypad.h://GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./custom/gnbj6737t_66_m0/inc/cust_rtc.h://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./custom/gnbj6737t_66_m0/inc/cust_rtc.h://GioneeDrv GuoJianqiu 20151223 modify for platform change end

	./platform/mt6735/src/drivers/inc/platform.h://GioneeDrv LiLuBao 20161121 modify for fixed GNSPR53131 begin         
	./platform/mt6735/src/drivers/inc/platform.h://GioneeDrv LiLuBao 20161121 modify for fixed GNSPR53131 end
	./platform/mt6735/src/drivers/inc/platform.h://GioneeDrv LiLuBao 20161121 modify for fixed GNSPR53131 begin
	./platform/mt6735/src/drivers/inc/platform.h://GioneeDrv LiLuBao 20161121 modify for fixed GNSPR53131 end



	pmic.c,keypad.h,cust_rtc.h,platform.h










kernel阶段：

	./arch/arm64/configs/gnbj6737t_66_m0_debug_defconfig:#GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./arch/arm64/configs/gnbj6737t_66_m0_debug_defconfig:#GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./arch/arm64/configs/gnbj6737t_66_m0_debug_defconfig:#GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./arch/arm64/configs/gnbj6737t_66_m0_debug_defconfig:#GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./arch/arm64/configs/gnbj6737t_66_m0_defconfig:#GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./arch/arm64/configs/gnbj6737t_66_m0_defconfig:#GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./arch/arm64/configs/gnbj6737t_66_m0_defconfig:#GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./arch/arm64/configs/gnbj6737t_66_m0_defconfig:#GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./arch/arm64/boot/dts/mt6735.dtsi:	//GioneeDrv GuoJianqiu 20160425 for charging begin
	./arch/arm64/boot/dts/mt6735.dtsi:	//GioneeDrv GuoJianqiu 20160425 for charging end


	./drivers/misc/mediatek/include/mt-plat/battery_common.h://GioneeDrv GuoJianqiu 20160429 modify for platform change begin
	./drivers/misc/mediatek/include/mt-plat/battery_common.h://GioneeDrv GuoJianqiu 20160429 modify for platform change end
	./drivers/misc/mediatek/include/mt-plat/battery_common.h://Gionee GuoJianqiu 20150514 modify for GNNCR00010646 begin
	./drivers/misc/mediatek/include/mt-plat/battery_common.h://Gionee GuoJianqiu 20150514 modify for GNNCR00010646 end

	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h://GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h://GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h://GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h://GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h://GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h://GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h://GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h://GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h:#define CUST_POWERON_DELTA_CAPACITY_TOLRANCE	25	//Gionee GuoJianqiu 20160301 modify for GNSPR #4119
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h://GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h://GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter_table.h://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter_table.h://GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter_table.h://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter_table.h://GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_charging.h://Gionee GuoJianqiu 20150312 modify for fix bugs begin
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_charging.h://Gionee GuoJianqiu 20150312 modify for fix bugs end
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_charging.h://Gionee GuoJianqiu 20151207 modify for fix CR01604790 begin
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_charging.h://Gionee GuoJianqiu 20151207 modify for fix CR01604790 end
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_charging.h://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_charging.h://GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_charging.h://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_charging.h://GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_charging.h://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_charging.h://GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_charging.h://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_charging.h://GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_charging.h://GioneeDrv GuoJianqiu 20160429 modify for platform change begin
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_charging.h://GioneeDrv GuoJianqiu 20160429 modify for platform change end
	./drivers/misc/mediatek/include/mt-plat/charging.h:	//GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/include/mt-plat/charging.h:	//GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/misc/mediatek/include/mt-plat/battery_meter_hal.h://GioneeDrv GuoJianqiu 20160429 modify for platform change begin
	./drivers/misc/mediatek/include/mt-plat/battery_meter_hal.h://GioneeDrv GuoJianqiu 20160429 modify for platform change end
	./drivers/misc/mediatek/power/mt6735/Makefile:                    #GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/power/mt6735/Makefile:                    #GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/misc/mediatek/power/mt6735/bq24158.c:#define BQ24158_BUSNUM 2 		//Gionee GuoJianqiu 20150407 modify for fix bugs begin
	./drivers/misc/mediatek/power/mt6735/bq24158.c:        battery_log(BAT_LOG_CRTI,"[store_bq24158_access] buf is %s and size is %zu \n",buf,size); 	//Gionee GuoJianqiu 20150408 modify for fix errors begin
	./drivers/misc/mediatek/power/mt6735/bq24158.c:static int __init bq24158_subsys_init(void) 	//Gionee GuoJianqiu 20150408 modify for fix bugs begin
	./drivers/misc/mediatek/power/mt6735/bq24158.c://module_init(bq24158_init); 				//Gionee GuoJianqiu 20150408 modify for fix bugs begin
	./drivers/misc/mediatek/power/mt6735/bq24158.c:subsys_initcall(bq24158_subsys_init); 	//Gionee GuoJianqiu 20150408 modify for fix bugs end
	./drivers/misc/mediatek/power/mt6735/pmic_chr_type_det.c:	//GioneeDrv GuoJianqiu 20160429 modify for platform change begin
	./drivers/misc/mediatek/power/mt6735/pmic_chr_type_det.c:	//GioneeDrv GuoJianqiu 20160429 modify for platform change end
	./drivers/misc/mediatek/power/mt6735/pmic_chr_type_det.c:	//GioneeDrv GuoJianqiu 20160429 modify for platform change begin
	./drivers/misc/mediatek/power/mt6735/pmic_chr_type_det.c:	//GioneeDrv GuoJianqiu 20160429 modify for platform change end
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:extern kal_uint32 upmu_get_rgs_chrdet(void); 	//Gionee GuoJianqiu 20150214 modify for fix errs begin
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:    val = pmic_get_register_value_nolock(PMIC_RGS_CHRDET);	 //Gionee GuoJianqiu 20150214 modify for fix errs begin
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:	//Gionee GuoJianqiu 201601121 modify for GNSPR #56307 begin
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:	//Gionee GuoJianqiu 201601121 modify for GNSPR #56307 begin
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:	bc11_set_register_value(PMIC_RG_USBDL_SET,0x0);//force leave USBDL mode 	//Gionee GuoJianqiu 20150408 modify for fix bugs begin
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:       bc11_set_register_value(PMIC_RG_USBDL_RST,0x1);//force leave USBDL mode;	//Gionee GuoJianqiu 20150408 modify for fix bugs end
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:    //Gionee GuoJianqiu 201601121 modify for GNSPR #56307 begin
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:    //Gionee GuoJianqiu 201601121 modify for GNSPR #56307 end
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:    kal_uint32 bootmode = 0; 	//GioneeDrv GuoJianqiu 20151223 modify for platform change
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:		//GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:		//GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c://BATTERY_VOLTAGE_ENUM cv_voltage=BATTERY_VOLT_04_200000_V; 	//Gionee GuoJianqiu 20150723 modify for GNNCR00011362
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:	//cv_voltage=VBAT_CV_VTH[register_value]; 	//Gionee GuoJianqiu 20150723 modify for GNNCR00011362
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:    /*if(mt6325_upmu_get_swcid() == PMIC6325_E1_CID_CODE)	 //Gionee GuoJianqiu 20150214 modify for fix errs begin
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:    else*/		//Gionee GuoJianqiu 20150214 modify for fix errs end
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:        /*if(is_mt6311_exist())		 //Gionee GuoJianqiu 20150214 modify for fix errs begin
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:        else*/		//Gionee GuoJianqiu 20150214 modify for fix errs end
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:    pmic_set_register_value(PMIC_RG_VCDT_HV_VTH,register_value);//;	 	//Gionee GuoJianqiu 20150214 modify for fix errs begin
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:    *(kal_bool*)(data) = pmic_get_register_value(PMIC_RGS_VCDT_HV_DET); 		//Gionee GuoJianqiu 20150214 modify for fix errs begin
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:    pmic_set_register_value(PMIC_BATON_TDET_EN,1); 	//Gionee GuoJianqiu 20150724 modify for GNSPR00037414
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:    val=pmic_get_register_value(PMIC_BATON_TDET_EN); 	//Gionee GuoJianqiu 20150214 modify for fix errs begin
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:    	pmic_set_register_value(PMIC_BATON_TDET_EN,1);		 //Gionee GuoJianqiu 20150214 modify for fix errs begin
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:    	pmic_set_register_value(PMIC_RG_BATON_EN,1);		 //Gionee GuoJianqiu 20150214 modify for fix errs begin
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:    	*(kal_bool*)(data) = pmic_get_register_value(PMIC_RGS_BATON_UNDET);		 //Gionee GuoJianqiu 20150214 modify for fix errs begin
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:    val = pmic_get_register_value_nolock(PMIC_RGS_CHRDET); 	//Gionee GuoJianqiu 20150214 modify for fix errs begin
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:    if(((g_diso_state >> 1) & 0x3) != 0x0 || pmic_get_register_value_nolock(PMIC_RGS_CHRDET)) 	//Gionee GuoJianqiu 20150214 modify for fix errs begin
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:	kernel_restart("battery service reboot system"); 	//GioneeDrv GuoJianqiu 20151223 modify for platform change
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:	//GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:	//GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:        ,charging_set_error_state 	//Gionee GuoJianqiu 20150408 modify for fix bugs begin
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:};								//Gionee GuoJianqiu 20150408 modify for fix bugs end
	./drivers/misc/mediatek/power/Kconfig:#GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/power/Kconfig:#GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/power/power_supply_sysfs.c:	//GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/power/power_supply_sysfs.c:	//GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/power/mediatek/switch_charging.c://GioneeDrv GuoJianqiu 20160429 modify for platform change begin
	./drivers/power/mediatek/switch_charging.c://GioneeDrv GuoJianqiu 20160429 modify for platform change end
	./drivers/power/mediatek/switch_charging.c://Gionee GuoJianqiu 20150514 modify for GNNCR00010646 begin
	./drivers/power/mediatek/switch_charging.c://Gionee GuoJianqiu 20150514 modify for GNNCR00010646 end
	./drivers/power/mediatek/switch_charging.c: extern kal_bool upmu_is_chr_det(void);	//Gionee GuoJianqiu 20150603 modify for fix CR01492674
	./drivers/power/mediatek/switch_charging.c://Gionee GuoJianqiu 20150611 modify for CR01498036 begin
	./drivers/power/mediatek/switch_charging.c: //Gionee GuoJianqiu 20150611 modify for CR01498036 end
	./drivers/power/mediatek/switch_charging.c://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/power/mediatek/switch_charging.c://GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/power/mediatek/switch_charging.c:	//GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/power/mediatek/switch_charging.c:	//GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/power/mediatek/switch_charging.c:			g_temp_CC_value = CHARGE_CURRENT_500_00_MA;	//Gionee GuoJianqiu 20150610 modify for fix CR01498761
	./drivers/power/mediatek/switch_charging.c:			g_temp_CC_value = CHARGE_CURRENT_1500_00_MA;	//Gionee GuoJianqiu 20150417 modify for fix errs begin
	./drivers/power/mediatek/switch_charging.c:		//Gionee GuoJianqiu 20150514 modify for GNNCR00010646 begin
	./drivers/power/mediatek/switch_charging.c:			//GioneeDrv GuoJianqiu 20160429 modify for platform change begin
	./drivers/power/mediatek/switch_charging.c:			//GioneeDrv GuoJianqiu 20160429 modify for platform change end
	./drivers/power/mediatek/switch_charging.c:		//Gionee GuoJianqiu 20150514 modify for GNNCR00010646 end
	./drivers/power/mediatek/switch_charging.c:		g_temp_CC_value = CHARGE_CURRENT_500_00_MA;	//Gionee GuoJianqiu 20150610 modify for fix CR01498761
	./drivers/power/mediatek/switch_charging.c:	//Gionee GuoJianqiu 20150608 modify for fix CR01498099 begin
	./drivers/power/mediatek/switch_charging.c:	//Gionee GuoJianqiu 20150608 modify for fix CR01498099 end
	./drivers/power/mediatek/switch_charging.c:			g_temp_CC_value = CHARGE_CURRENT_500_00_MA;	//Gionee GuoJianqiu 20150610 modify for fix CR01498761
	./drivers/power/mediatek/switch_charging.c:	if (upmu_is_chr_det() == KAL_TRUE)	//Gionee GuoJianqiu 20150603 modify for fix CR01492674
	./drivers/power/mediatek/switch_charging.c:			//GioneeDrv GuoJianqiu 20160429 modify for platform change begin
	./drivers/power/mediatek/switch_charging.c:			//GioneeDrv GuoJianqiu 20160429 modify for platform change end
	./drivers/power/mediatek/switch_charging.c:	//GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/power/mediatek/switch_charging.c:		//Gionee GuoJianqiu 20141130 modify for GNSPR00027029 begin
	./drivers/power/mediatek/switch_charging.c:		//Gionee GuoJianqiu 20141130 modify for GNSPR00027029 end
	./drivers/power/mediatek/switch_charging.c:	//GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/power/mediatek/switch_charging.c:		//GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/power/mediatek/switch_charging.c:		//GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/power/mediatek/switch_charging.c:			//GioneeDrv GuoJianqiu 20160429 modify for platform change begin
	./drivers/power/mediatek/switch_charging.c:			//GioneeDrv GuoJianqiu 20160429 modify for platform change end
	./drivers/power/mediatek/switch_charging.c:	battery_log(BAT_LOG_CRTI, "[BATTERY] pchr_turn_on_charging(), enable =%d !\r\n",	//Gionee GuoJianqiu 20150318 modify for fix bugs
	./drivers/power/mediatek/switch_charging.c:	battery_log(BAT_LOG_CRTI, "[BATTERY] CC mode charge, timer=%d on %d !!\n\r",	//Gionee GuoJianqiu 20150318 modify for fix bugs
	./drivers/power/mediatek/switch_charging.c:    //Gionee GuoJianqiu 20141130 modify for GNSPR00027029
	./drivers/power/mediatek/switch_charging.c:	//Gionee GuoJianqiu 20141130 modify for GNSPR00027029
	./drivers/power/mediatek/battery_meter_fg_20.c://GioneeDrv GuoJianqiu 20160429 modify for platform change begin
	./drivers/power/mediatek/battery_meter_fg_20.c://GioneeDrv GuoJianqiu 20160429 modify for platform change end
	./drivers/power/mediatek/battery_meter_fg_20.c:	//GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/power/mediatek/battery_meter_fg_20.c:	//GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/power/mediatek/battery_meter_fg_20.c:    	//Gionee GuoJianqiu 20150318 modify for fix bugs begin
	./drivers/power/mediatek/battery_meter_fg_20.c:    	//Gionee GuoJianqiu 20150318 modify for fix bugs end
	./drivers/power/mediatek/battery_meter_fg_20.c:    	//Gionee GuoJianqiu 20150318 modify for fix bugs begin
	./drivers/power/mediatek/battery_meter_fg_20.c:    	//Gionee GuoJianqiu 20150318 modify for fix bugs end
	./drivers/power/mediatek/battery_meter_fg_20.c:	//Gionee GuoJianqiu 20150318 modify for fix bugs begin
	./drivers/power/mediatek/battery_meter_fg_20.c:	//Gionee GuoJianqiu 20150318 modify for fix bugs end	
	./drivers/power/mediatek/battery_meter_fg_20.c://Gionee GuoJianqiu 20141216 modify for GNSPR00029961 begin
	./drivers/power/mediatek/battery_meter_fg_20.c://Gionee GuoJianqiu 20141216 modify for GNSPR00029961 end
	./drivers/power/mediatek/battery_meter_fg_20.c://Gionee GuoJianqiu 20150612 modify for CR01481562 begin
	./drivers/power/mediatek/battery_meter_fg_20.c://Gionee GuoJianqiu 20150612 modify for CR01481562 end
	./drivers/power/mediatek/battery_meter_fg_20.c://Gionee GuoJianqiu 20150612 modify for CR01481562 begin
	./drivers/power/mediatek/battery_meter_fg_20.c://Gionee GuoJianqiu 20150612 modify for CR01481562 end
	./drivers/power/mediatek/battery_meter_fg_20.c:	//GioneeDrv GuoJianqiu 20160908 modify for GNSPR #42886 begin
	./drivers/power/mediatek/battery_meter_fg_20.c:	//GioneeDrv GuoJianqiu 20160908 modify for GNSPR #42886 end
	./drivers/power/mediatek/battery_meter_fg_20.c://GioneeDrv GuoJianqiu 20160429 modify for platform change begin
	./drivers/power/mediatek/battery_meter_fg_20.c://GioneeDrv GuoJianqiu 20160429 modify for platform change end
	./drivers/power/mediatek/battery_meter_fg_20.c://Gionee GuoJianqiu 20150424 modify for fix bugs begin
	./drivers/power/mediatek/battery_meter_fg_20.c://Gionee GuoJianqiu 20150313 modify for fix bugs end
	./drivers/power/mediatek/battery_meter.c://GioneeDrv GuoJianqiu 20160429 modify for platform change begin
	./drivers/power/mediatek/battery_meter.c://GioneeDrv GuoJianqiu 20160429 modify for platform change end
	./drivers/power/mediatek/battery_meter.c://Gionee GuoJianqiu 20150318 modify for fix CR01549150 begin
	./drivers/power/mediatek/battery_meter.c://Gionee GuoJianqiu 20150318 modify for fix CR01549150 end
	./drivers/power/mediatek/battery_meter.c://Gionee GuoJianqiu 20150318 modify for fix CR01549150 begin
	./drivers/power/mediatek/battery_meter.c://Gionee GuoJianqiu 20150318 modify for fix CR01549150 end
	./drivers/power/mediatek/battery_common_fg_20.c:	//Gionee GuoJianqiu 20150318 modify for fix bugs begin
	./drivers/power/mediatek/battery_common_fg_20.c:	//Gionee GuoJianqiu 20150318 modify for fix bugs end
	./drivers/power/mediatek/battery_common_fg_20.c:	//Gionee GuoJianqiu 20150318 modify for fix bugs begin
	./drivers/power/mediatek/battery_common_fg_20.c:	//Gionee GuoJianqiu 20150318 modify for fix bugs end
	./drivers/power/mediatek/battery_common_fg_20.c://Gionee GuoJianqiu 20150611 modify for CR01498036 begin
	./drivers/power/mediatek/battery_common_fg_20.c://Gionee GuoJianqiu 20150611 modify for CR01498036 end
	./drivers/power/mediatek/battery_common_fg_20.c:	//Gionee GuoJianqiu 20150424 modify for fix bugs begin
	./drivers/power/mediatek/battery_common_fg_20.c:	//Gionee GuoJianqiu 20150318 modify for fix bugs end
	./drivers/power/mediatek/battery_common_fg_20.c:	//GioneeDrv GuoJianqiu 20160429 modify for platform change begin
	./drivers/power/mediatek/battery_common_fg_20.c:	//GioneeDrv GuoJianqiu 20160429 modify for platform change end
	./drivers/power/mediatek/battery_common_fg_20.c://Gionee GuoJianqiu 201601026 modify for OTG SWITCH begin
	./drivers/power/mediatek/battery_common_fg_20.c://Gionee GuoJianqiu 201601026 modify for OTG SWITCH end
	./drivers/power/mediatek/battery_common_fg_20.c://Gionee GuoJianqiu 20150514 modify for GNNCR00010646 begin
	./drivers/power/mediatek/battery_common_fg_20.c:	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH begin
	./drivers/power/mediatek/battery_common_fg_20.c:	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH end
	./drivers/power/mediatek/battery_common_fg_20.c:	//GioneeDrv GuoJianqiu 20160429 modify for platform change begin
	./drivers/power/mediatek/battery_common_fg_20.c:	//GioneeDrv GuoJianqiu 20160429 modify for platform change end

	./drivers/power/mediatek/battery_common_fg_20.c:	//GioneeDrv GuoJianqiu 20160429 modify for platform change begin
	./drivers/power/mediatek/battery_common_fg_20.c:	//GioneeDrv GuoJianqiu 20160429 modify for platform change end
	./drivers/power/mediatek/battery_common_fg_20.c://Gionee GuoJianqiu 20150514 modify for GNNCR00010646 end
	./drivers/power/mediatek/battery_common_fg_20.c:	//Gionee GuoJianqiu 20150424 modify for fix bugs begin
	./drivers/power/mediatek/battery_common_fg_20.c:	//Gionee GuoJianqiu 20150318 modify for fix bugs end
	./drivers/power/mediatek/battery_common_fg_20.c:			//Gionee GuoJianqiu 20161113 modify for fix GNSPR #54689 begin
	./drivers/power/mediatek/battery_common_fg_20.c:			//Gionee GuoJianqiu 20161113 modify for fix GNSPR #54689 end
	./drivers/power/mediatek/battery_common_fg_20.c:	return (BMT_status.bat_in_recharging_state /*|| BMT_status.bat_full == KAL_TRUE*/);	//Gionee GuoJianqiu 20141226 modify for GNSPR00031054
	./drivers/power/mediatek/battery_common_fg_20.c:	if (previous_SOC == -1 && bat_vol <= SYSTEM_OFF_VOLTAGE) {	//Gionee GuoJianqiu 20160217 modify for GNSPR4723
	./drivers/power/mediatek/battery_common_fg_20.c:	//GioneeDrv GuoJianqiu 20160429 modify for platform change begin
	./drivers/power/mediatek/battery_common_fg_20.c:	//GioneeDrv GuoJianqiu 20160429 modify for platform change end
	./drivers/power/mediatek/battery_common_fg_20.c:	//Gionee GuoJianqiu 20150318 modify for fix bugs begin
	./drivers/power/mediatek/battery_common_fg_20.c:	//Gionee GuoJianqiu 20150424 modify for fix bugs end
	./drivers/power/mediatek/battery_common_fg_20.c:	//Gionee GuoJianqiu 20150611 modify for CR01498036 begin
	./drivers/power/mediatek/battery_common_fg_20.c:	//Gionee GuoJianqiu 20150611 modify for CR01498036 end
	./drivers/power/mediatek/battery_common_fg_20.c:        //Gionee GuoJianqiu 20150318 modify for fix bugs begin
	./drivers/power/mediatek/battery_common_fg_20.c:        //Gionee GuoJianqiu 20150318 modify for fix bugs end
	./drivers/power/mediatek/battery_common_fg_20.c:	//Gionee GuoJianqiu 20150318 modify for fix bugs begin
	./drivers/power/mediatek/battery_common_fg_20.c:	//Gionee GuoJianqiu 20150318 modify for fix bugs end
	./drivers/power/mediatek/battery_common_fg_20.c:		//Gionee GuoJianqiu 20150318 modify for fix bugs begin
	./drivers/power/mediatek/battery_common_fg_20.c:		//Gionee GuoJianqiu 20150318 modify for fix bugs end
	./drivers/power/mediatek/battery_common_fg_20.c://Gionee GuoJianqiu 20141210 modify for GNSPR00027866 begin
	./drivers/power/mediatek/battery_common_fg_20.c://Gionee GuoJianqiu 20141210 modify for GNSPR00027866 begin
	./drivers/power/mediatek/battery_common_fg_20.c:		//Gionee GuoJianqiu 20151201 modify for fix CR01595060 begin
	./drivers/power/mediatek/battery_common_fg_20.c:		//Gionee GuoJianqiu 20151201 modify for fix CR01595060 end
	./drivers/power/mediatek/battery_common_fg_20.c:		//Gionee GuoJianqiu 201601117 modify for GNSPR #54468 begin
	./drivers/power/mediatek/battery_common_fg_20.c:		//Gionee GuoJianqiu 201601117 modify for GNSPR #54468 end
	./drivers/power/mediatek/battery_common_fg_20.c:	battery_log(BAT_LOG_CRTI, "******** battery : bat_thread_wakeup  ********\n");	//Gionee GuoJianqiu 20150318 modify for fix bugs begin
	./drivers/power/mediatek/battery_common_fg_20.c:	battery_log(BAT_LOG_CRTI, "******** battery : bat_update_thread_wakeup  ********\n"); 	//Gionee GuoJianqiu 20150318 modify for fix bugs
	./drivers/power/mediatek/battery_common_fg_20.c:	battery_log(BAT_LOG_CRTI, "[check_battery_exist] baton_count = %d\n ", baton_count);	//Gionee GuoJianqiu 20150724 modify for GNSPR00037414	
	./drivers/power/mediatek/battery_common_fg_20.c:					    "[BATTERY] Battery is not exist, power off Charge IC and system (%d)\n",	//Gionee GuoJianqiu 20150318 modify for fix bugs
	./drivers/power/mediatek/battery_common_fg_20.c:			//Gionee GuoJianqiu 20150724 modify for GNSPR00037414 begin
	./drivers/power/mediatek/battery_common_fg_20.c:			//Gionee GuoJianqiu 20150724 modify for GNSPR00037414 end
	./drivers/power/mediatek/battery_common_fg_20.c:	//Gionee GuoJianqiu 20150424 modify for fix bugs begin
	./drivers/power/mediatek/battery_common_fg_20.c:	//Gionee GuoJianqiu 20150424 modify for fix bugs end
	./drivers/power/mediatek/battery_common_fg_20.c:	//Gionee GuoJianqiu 20141210 modify for GNSPR00027866 begin
	./drivers/power/mediatek/battery_common_fg_20.c:	//Gionee GuoJianqiu 20141210 modify for GNSPR00027866 end
	./drivers/power/mediatek/battery_common_fg_20.c:		//Gionee GuoJianqiu 20141210 modify for GNSPR00027866 begin
	./drivers/power/mediatek/battery_common_fg_20.c:		//Gionee GuoJianqiu 20141210 modify for GNSPR00027866 end
	./drivers/power/mediatek/battery_common_fg_20.c:		battery_log(BAT_LOG_CRTI,	//Gionee GuoJianqiu 20150318 modify for fix bugs
	./drivers/power/mediatek/battery_common_fg_20.c://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/power/mediatek/battery_common_fg_20.c://GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/power/mediatek/battery_common_fg_20.c:	//GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/power/mediatek/battery_common_fg_20.c:	//GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/power/mediatek/battery_common_fg_20.c:		//Gionee GuoJianqiu 20150514 modify for GNNCR00010646 begin
	./drivers/power/mediatek/battery_common_fg_20.c:		//Gionee GuoJianqiu 20150514 modify for GNNCR00010646 end

	./drivers/power/mediatek/battery_common_fg_20.c:	//Gionee GuoJianqiu 20150103 modify for GNSPR00028091 begin
	./drivers/power/mediatek/battery_common_fg_20.c:	//Gionee GuoJianqiu 20150103 modify for GNSPR00028091 end
	./drivers/power/mediatek/battery_common_fg_20.c:		//Gionee GuoJianqiu 20150103 modify for GNSPR00028091 begin
	./drivers/power/mediatek/battery_common_fg_20.c:		//Gionee GuoJianqiu 20150103 modify for GNSPR00028091 end
	./include/linux/power_supply.h:	//GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./include/linux/power_supply.h:	//GioneeDrv GuoJianqiu 20151223 modify for platform change end	







gnbj6737t_66_m0_debug_defconfig,mt6735.dtsi,gnbj6737t_66_m0.dts,gnbj6737t_66_m0.dts,
battery_meter_fg_20.c,battery_common_fg_20.c,power_supply.h,musb_core.c,battery_meter.c
usb20.h,usb20_phy.c,usb20.c,usb20_host.c,battery_common.h,mt_battery_meter.h,mt_battery_meter_table.h,mt_charging.h,battery_meter_hal.h
Makefile,bq24158.c,pmic_chr_type_det.c,charging_hw_bq24158,Kconfig,power_supply_sysfs.c,switch_charging.c,


pmic_chr_type_det.c检测延迟时间要改150ms改为200ms




2.OTG
	./drivers/misc/mediatek/usb20/mt6735/usb20.h://Gionee GuoJianqiu 201601026 modify for OTG SWITCH end
	./drivers/misc/mediatek/usb20/mt6735/usb20.h://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/usb20/mt6735/usb20.h://GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/misc/mediatek/usb20/mt6735/usb20_phy.c:	//Gionee GuoJianqiu 201601026 modify for GNSPR #54677 begin
	./drivers/misc/mediatek/usb20/mt6735/usb20_phy.c:	//Gionee GuoJianqiu 201601026 modify for GNSPR #54677 end
	./drivers/misc/mediatek/usb20/mt6735/usb20.c://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/usb20/mt6735/usb20.c://GioneeDrv GuoJianqiu 20151223 modify for platform change end	
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c://Gionee GuoJianqiu 201601026 modify for OTG SWITCH begin
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c://Gionee GuoJianqiu 201601026 modify for OTG SWITCH end
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c:	//GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c:	//GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c:	//GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c:	//GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c://GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c:	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH begin
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c:	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH end
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c:	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH begin
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c:	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH end
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c:	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH begin
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c:	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH end
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c:	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH begin
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c:	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH end
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c://Gionee GuoJianqiu 201601026 modify for OTG SWITCH begin
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c:	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH begin
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c:	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH end
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c:	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH begin
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c:	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH end
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c://Gionee GuoJianqiu 201601026 modify for OTG SWITCH end
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c:	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH begin
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c:	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH end
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c:	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH begin
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c:	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH end


	./arch/arm64/boot/dts/gnbj6737t_66_m0.dts:/* GioneeDrv GuoJianqiu 20160425 modify for OTG function begin */
	./arch/arm64/boot/dts/gnbj6737t_66_m0.dts:/* GioneeDrv GuoJianqiu 20160425 modify for OTG function end */
	./drivers/misc/mediatek/usb20/mt6735/																																																																																																																																																																																																																																																																																																																																																																																																																																																									://Gionee GuoJianqiu 201601026 modify for OTG SWITCH begin


	./drivers/misc/mediatek/usb20/musb_core.c:		//GioneeDrv LiLuBao 20161118 modify for platform change begin
	./drivers/misc/mediatek/usb20/musb_core.c:		//GioneeDrv LiLuBao 20161118 modify for platform change end
	./drivers/misc/mediatek/usb20/musb_core.c:	//GioneeDrv LiLuBao 20161118 modify for platform change begin
	./drivers/misc/mediatek/usb20/musb_core.c:	//GioneeDrv LiLuBao 20161118 modify for platform change end


	./drivers/power/mediatek/battery_common_fg_20.c:		//Gionee GuoJianqiu 201601026 modify for OTG SWITCH begin
	./drivers/power/mediatek/battery_common_fg_20.c:		//Gionee GuoJianqiu 201601026 modify for OTG SWITCH end


	./drivers/misc/mediatek/include/mt-plat/battery_common.h://Gionee GuoJianqiu 201601026 modify for OTG SWITCH begin
	./drivers/misc/mediatek/include/mt-plat/battery_common.h://Gionee GuoJianqiu 201601026 modify for OTG SWITCH end






3.power off  charging  logo

	./vendor/mediatek/proprietary/external/libshowlogo/show_animation_common.h://GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
	./vendor/mediatek/proprietary/external/libshowlogo/show_animation_common.h://GioneeDrv LiLuBao 20161113 modify for change gioneelogo end
	./vendor/mediatek/proprietary/external/libshowlogo/show_animation_common.c://GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
	./vendor/mediatek/proprietary/external/libshowlogo/show_animation_common.c://GioneeDrv LiLuBao 20161113 modify for change gioneelogo end
	./vendor/mediatek/proprietary/external/libshowlogo/show_animation_common.c:    //GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
	./vendor/mediatek/proprietary/external/libshowlogo/show_animation_common.c:    //GioneeDrv LiLuBao 20161113 modify for change gioneelogo end
	./vendor/mediatek/proprietary/external/libshowlogo/charging_animation.cpp://GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
	./vendor/mediatek/proprietary/external/libshowlogo/charging_animation.cpp://GioneeDrv LiLuBao 20161113 modify for change gioneelogo end
	./vendor/mediatek/proprietary/external/libshowlogo/charging_animation.cpp:	//GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
	./vendor/mediatek/proprietary/external/libshowlogo/charging_animation.cpp:	//GioneeDrv LiLuBao 20161113 modify for change gioneelogo end
	./vendor/mediatek/proprietary/external/libshowlogo/charging_animation.cpp:	//GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
	./vendor/mediatek/proprietary/external/libshowlogo/charging_animation.cpp:	//GioneeDrv LiLuBao 20161113 modify for change gioneelogo end
	./vendor/mediatek/proprietary/external/libshowlogo/charging_animation.h://GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
	./vendor/mediatek/proprietary/external/libshowlogo/charging_animation.h://GioneeDrv LiLuBao 20161113 modify for change gioneelogo end
	./vendor/mediatek/proprietary/external/charger/bootlogo.cpp:    //GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
	./vendor/mediatek/proprietary/external/charger/bootlogo.cpp:    //GioneeDrv LiLuBao 20161113 modify for change gioneelogo end
	./vendor/mediatek/proprietary/external/charger/common.cpp:	//GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
	./vendor/mediatek/proprietary/external/charger/common.cpp:	//GioneeDrv LiLuBao 20161113 modify for change gioneelogo end

	./vendor/mediatek/proprietary/bootable/bootloader/lk/project/gnbj6737t_66_m0.mk:#GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
	./vendor/mediatek/proprietary/bootable/bootloader/lk/project/gnbj6737t_66_m0.mk:#GioneeDrv LiLuBao 20161113 modify for change gioneelogo end
	./vendor/mediatek/proprietary/bootable/bootloader/lk/project/gnbj6737t_66_m0.mk:#DEFINES += GN_MTK_BSP_LK_CHARGE_GIONEELOGO
	./vendor/mediatek/proprietary/bootable/bootloader/lk/dev/logo/rules.mk:#GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
	./vendor/mediatek/proprietary/bootable/bootloader/lk/dev/logo/rules.mk:#GioneeDrv LiLuBao 20161113 modify for change gioneelogo end
	./vendor/mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/platform.c:			//GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
	./vendor/mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/platform.c:			//GioneeDrv LiLuBao 20161113 modify for change gioneelogo end	
	./vendor/mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/platform.c:			//Gionee GuoJianqiu 20150522 modify for gioneelogo begin
	./vendor/mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/platform.c:			#elif defined(GN_MTK_BSP_LK_CHARGE_GIONEELOGO)
	./vendor/mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/platform.c:			//Gionee GuoJianqiu 20150522 modify for gioneelogo end
	./vendor/mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/mt_logo.c:	//GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
	./vendor/mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/mt_logo.c:	//GioneeDrv LiLuBao 20161113 modify for change gioneelogo end
	./vendor/mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/mt_logo.c://GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
	./vendor/mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/mt_logo.c://GioneeDrv LiLuBao 20161113 modify for change gioneelogo end
	./vendor/mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/mt_logo.c://Gionee GuoJianqiu 20150522 modify for gioneelogo begin
	./vendor/mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/mt_logo.c:#elif defined(GN_MTK_BSP_LK_CHARGE_GIONEELOGO)
	./vendor/mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/mt_logo.c://Gionee GuoJianqiu 20150522 modify for gioneelogo end
	./vendor/mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/mt_logo.c:	//GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
	./vendor/mediatek/proprietary/bootable/bootloader/lk/lib/libshowlogo/show_animation_common.h://GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
	./vendor/mediatek/proprietary/bootable/bootloader/lk/lib/libshowlogo/show_animation_common.h://GioneeDrv LiLuBao 20161113 modify for change gioneelogo end
	./vendor/mediatek/proprietary/bootable/bootloader/lk/lib/libshowlogo/show_animation_common.c://GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
	./vendor/mediatek/proprietary/bootable/bootloader/lk/lib/libshowlogo/show_animation_common.c://GioneeDrv LiLuBao 20161113 modify for change gioneelogo end

分辨率

宏开关

图片数组的下标

MTK_CARRIEREXPRESS_PACK











4.fuelgauged  GM2.0电量算法 		但是这个验证的现象？

验证：首先会调用battery_common_fg_20.c,battery_meter_fg_20.c几个文件，log搜索fgadc
手机显示的电量72%，将电池取出后，静置半个小时后，测量电池的电压并与ZCV table(mt_battery_meter_table.h)比较误差在5%以内，则正常
实际结果72%，电池电压4.07V在误差范围内

	./kernel-3.18/arch/arm64/configs/gnbj6737t_66_m0_debug_defconfig:366:CONFIG_MTK_HAFG_20=y
	./kernel-3.18/arch/arm64/configs/gnbj6737t_66_m0_defconfig:360:CONFIG_MTK_HAFG_20=y


	./kernel-3.18/drivers/power/mediatek/switch_charging.c:78:#if defined(CONFIG_MTK_HAFG_20)
	./kernel-3.18/drivers/power/mediatek/switch_charging.c:699:#if defined(CONFIG_MTK_HAFG_20)
	./kernel-3.18/drivers/power/mediatek/switch_charging.c:1220:			#if defined(CONFIG_MTK_HAFG_20)
	./kernel-3.18/drivers/power/mediatek/switch_charging.c:1247:#if defined(CONFIG_MTK_HAFG_20)
	./kernel-3.18/drivers/power/mediatek/switch_charging.c:1307:#ifndef CONFIG_MTK_HAFG_20
	./kernel-3.18/drivers/power/mediatek/battery_meter_fg_20.c:1627:#if defined(CONFIG_MTK_HAFG_20)
	./kernel-3.18/drivers/power/mediatek/battery_common_fg_20.c:1580:#if defined(CONFIG_MTK_HAFG_20)



	./kernel-3.18/arch/arm64/boot/dts/gnbj6737t_66_m0.dts:8:#ifdef CONFIG_MTK_HAFG_20
	./kernel-3.18/drivers/misc/mediatek/include/mt-plat/battery_common.h:354:#if defined(CONFIG_MTK_HAFG_20)
	./kernel-3.18/drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h:46:#if defined(CONFIG_MTK_HAFG_20)
	./kernel-3.18/drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h:77:#ifdef CONFIG_MTK_HAFG_20
	./kernel-3.18/drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h:220:#ifdef CONFIG_MTK_HAFG_20
	./kernel-3.18/drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h:350:#ifdef CONFIG_MTK_HAFG_20
	./kernel-3.18/drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter_table.h:77:#if defined(CONFIG_MTK_HAFG_20)
	./kernel-3.18/drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter_table.h:1071:#if defined(CONFIG_MTK_HAFG_20)


	应该还包含相应的电池曲线，参数
	
	
	switch_charging.c  battery_common_fg_20.c  battery_meter_fg_20.c  battery_common.h  mt_battery_meter.h   mt_battery_meter_table.h
	battery_meter_hal.h  修改了log等级
	gnbj6737t_66_m0_debug_defconfig   gnbj6737t_66_m0_defconfig
	
	{
		CONFIG_MTK_HAFG_20		GM2.0定义的电量计算算法
		fgadc		打印log
	
		CONFIG_SWITCH_INPUT_OUTPUT_CURRENT_SUPPORT   不支持(应该是IC支持充电和向外给其他设备充电)
	}
	
	
	

5.other			其他零散的代码

	mt_logo.c, 

vendor目录下

	preloader阶段：
		
		./mediatek/proprietary/bootable/bootloader/preloader/platform/mt6735/src/drivers/pmic.c:10://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
		./mediatek/proprietary/bootable/bootloader/preloader/platform/mt6735/src/drivers/pmic.c:12://GioneeDrv GuoJianqiu 20151223 modify for platform change end
		./mediatek/proprietary/bootable/bootloader/preloader/platform/mt6735/src/drivers/pmic.c:555:                //GioneeDrv GuoJianqiu 20151223 modify for platform change begin
		./mediatek/proprietary/bootable/bootloader/preloader/platform/mt6735/src/drivers/pmic.c:557:                //GioneeDrv GuoJianqiu 20151223 modify for platform change end
		./mediatek/proprietary/bootable/bootloader/preloader/platform/mt6735/src/drivers/inc/keypad.h:64://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
		./mediatek/proprietary/bootable/bootloader/preloader/platform/mt6735/src/drivers/inc/keypad.h:70://GioneeDrv GuoJianqiu 20151223 modify for platform change end
		./mediatek/proprietary/bootable/bootloader/preloader/custom/gnbj6737t_66_m0/inc/cust_rtc.h:48://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
		./mediatek/proprietary/bootable/bootloader/preloader/custom/gnbj6737t_66_m0/inc/cust_rtc.h:51://GioneeDrv GuoJianqiu 20151223 modify for platform change end
		
		./mediatek/proprietary/bootable/bootloader/preloader/platform/mt6735/src/drivers/inc/platform.h:62://GioneeDrv LiLuBao 20161121 modify for fixed GNSPR53131 begin         
		./mediatek/proprietary/bootable/bootloader/preloader/platform/mt6735/src/drivers/inc/platform.h:64://GioneeDrv LiLuBao 20161121 modify for fixed GNSPR53131 end
		./mediatek/proprietary/bootable/bootloader/preloader/platform/mt6735/src/drivers/inc/platform.h:69://GioneeDrv LiLuBao 20161121 modify for fixed GNSPR53131 begin
		./mediatek/proprietary/bootable/bootloader/preloader/platform/mt6735/src/drivers/inc/platform.h:71://GioneeDrv LiLuBao 20161121 modify for fixed GNSPR53131 end
	
	
	lk阶段：
		./mediatek/proprietary/bootable/bootloader/lk/project/gnbj6737t_66_m0.mk:12:#GioneeDrv GuoJianqiu 20151223 modify for platform change begin
		./mediatek/proprietary/bootable/bootloader/lk/project/gnbj6737t_66_m0.mk:20:#GioneeDrv GuoJianqiu 20151223 modify for platform change end
		./mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/mt_pmic.c:617://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
		./mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/mt_pmic.c:619://GioneeDrv GuoJianqiu 20151223 modify for platform change end
		./mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/mt_battery.c:31://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
		./mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/mt_battery.c:35://GioneeDrv GuoJianqiu 20151223 modify for platform change end
		./mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/mt_battery.c:161://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
		./mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/mt_battery.c:167://GioneeDrv GuoJianqiu 20151223 modify for platform change end
		./mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/mt_battery.c:213:		//GioneeDrv GuoJianqiu 20151223 modify for platform change begin
		./mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/mt_battery.c:215:		//GioneeDrv GuoJianqiu 20151223 modify for platform change end
		./mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/mt_battery.c:256:		//GioneeDrv GuoJianqiu 20151223 modify for platform change begin
		./mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/mt_battery.c:258:		//GioneeDrv GuoJianqiu 20151223 modify for platform change end
		./mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/mt_battery.c:277:	//GioneeDrv GuoJianqiu 20151223 modify for platform change begin
		./mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/mt_battery.c:279:	//GioneeDrv GuoJianqiu 20151223 modify for platform change end
		./mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/mt_battery.c:335://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
		./mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/mt_battery.c:348://GioneeDrv GuoJianqiu 20151223 modify for platform change end
		./mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/bq24158.c:15:#define HIGH_BATTERY_VOLTAGE_SUPPORT 	//Gionee GuoJianqiu 20150408 modify for fix bugs begin
		./mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/bq24158.c:24:#define PRECC_BATVOL 2800 //preCC 2.8V 	//Gionee GuoJianqiu 20150430 modify for GNNCR00010432 begin
		./mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/bq24158.c:33:#define BQ24158_I2C_ID	I2C2 	 //Gionee GuoJianqiu 20150407 modify for fix bugs begin
		./mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/bq24158.c:597:static CHARGER_TYPE g_chr_type_num = CHARGER_UNKNOWN; 	//Gionee GuoJianqiu 20150430 modify for GNNCR00010432 begin
		./mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/bq24158.c:598:int hw_charging_get_charger_type(void); 			//Gionee GuoJianqiu 20150430 modify for GNNCR00010432 end
		./mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/bq24158.c:603:    int temp_CC_value = 0; 	//Gionee GuoJianqiu 20150430 modify for GNNCR00010432 begin
		./mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/bq24158.c:604:    kal_int32 bat_val = 0; 		//Gionee GuoJianqiu 20150430 modify for GNNCR00010432 end
		./mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/bq24158.c:615://Gionee GuoJianqiu 20150430 modify for GNNCR00010432 begin
		./mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/bq24158.c:648://Gionee GuoJianqiu 20150430 modify for GNNCR00010432 end
		./mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/bq24158.c:654:    printf("[BATTERY:bq24158] charger enable/disable %d , CC value(%dmA)\r\n", bEnable, temp_CC_value); 	//Gionee 			GuoJianqiu 20150430 modify for GNNCR00010432 begin
		./mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/bq24158.c:679://Gionee GuoJianqiu 20150430 modify for GNNCR00010432 begin
		./mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/bq24158.c:714://Gionee GuoJianqiu 20150324 modify for fix errs begin        
		./mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/bq24158.c:895://Gionee GuoJianqiu 20150324 modify for fix errs end
		./mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/bq24158.c:960://Gionee GuoJianqiu 20150430 modify for GNNCR00010432 end
		./mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/platform.c:753:    //GioneeDrv GuoJianqiu 20160618 modify for  GNSPR #15455 begin
		./mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/platform.c:757:    //GioneeDrv GuoJianqiu 20160618 modify for  GNSPR #15455 end
		./mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/platform.c:786:			//Gionee GuoJianqiu 20150522 modify for gioneelogo begin
		./mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/platform.c:792:			//Gionee GuoJianqiu 20150522 modify for gioneelogo end
		./mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/mt_logo.c:258://Gionee GuoJianqiu 20150522 modify for gioneelogo begin
		./mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/mt_logo.c:276://Gionee GuoJianqiu 20150522 modify for gioneelogo end
		./mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/rules.mk:180:        #GioneeDrv GuoJianqiu 20151223 modify for platform change begin
		./mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/rules.mk:185:        #GioneeDrv GuoJianqiu 20151223 modify for platform change end
	
	mt_logo.c
	
	external：
		./mediatek/proprietary/external/libshowlogo/show_animation_common.c:313://Gionee GuoJianqiu 20150928 modify for fix CR01556059 begin
		./mediatek/proprietary/external/libshowlogo/show_animation_common.c:337://Gionee GuoJianqiu 20150928 modify for fix CR01556059 end
		./mediatek/proprietary/external/charger/key_control.cpp:39://Gionee GuoJianqiu 20151123 modify for fix CR01586895 begin
		./mediatek/proprietary/external/charger/key_control.cpp:41://Gionee GuoJianqiu 20151123 modify for fix CR01586895 end
		./mediatek/proprietary/external/charger/charging_control.cpp:137://GioneeDrv GuoJianqiu 20160429 modify for platform change begin
		./mediatek/proprietary/external/charger/charging_control.cpp:145://GioneeDrv GuoJianqiu 20160429 modify for platform change end
		./mediatek/proprietary/external/charger/charging_control.cpp:153:	//Gionee GuoJianqiu 20151026 modify for CR01570555 begin
		./mediatek/proprietary/external/charger/charging_control.cpp:156:	//Gionee GuoJianqiu 20151026 modify for CR01570555 end
	
		
	key_control.cpp, charging_control.cpp
	
	
	
	
kernel目录下
	./arch/arm64/configs/gnbj6737t_66_m0_debug_defconfig:348:#GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./arch/arm64/configs/gnbj6737t_66_m0_debug_defconfig:351:#GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./arch/arm64/configs/gnbj6737t_66_m0_debug_defconfig:364:#GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./arch/arm64/configs/gnbj6737t_66_m0_debug_defconfig:368:#GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./arch/arm64/configs/gnbj6737t_66_m0_defconfig:340:#GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./arch/arm64/configs/gnbj6737t_66_m0_defconfig:343:#GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./arch/arm64/configs/gnbj6737t_66_m0_defconfig:358:#GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./arch/arm64/configs/gnbj6737t_66_m0_defconfig:362:#GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./arch/arm64/boot/dts/mt6735.dtsi:3018:	//GioneeDrv GuoJianqiu 20160425 for charging begin
	./arch/arm64/boot/dts/mt6735.dtsi:3023:	//GioneeDrv GuoJianqiu 20160425 for charging end


	./drivers/misc/mediatek/usb20/mt6735/usb20_phy.c:250:	//Gionee GuoJianqiu 201601026 modify for GNSPR #54677 begin
	./drivers/misc/mediatek/usb20/mt6735/usb20_phy.c:252:	//Gionee GuoJianqiu 201601026 modify for GNSPR #54677 end
	./drivers/misc/mediatek/usb20/mt6735/usb20.c:390://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/usb20/mt6735/usb20.c:392://GioneeDrv GuoJianqiu 20151223 modify for platform change end	
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c:99:	//GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c:117:	//GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c:151:	//GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c:168:	//GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c:213://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c:254://GioneeDrv GuoJianqiu 20151223 modify for platform change end

	./drivers/misc/mediatek/include/mt-plat/battery_common.h:11://GioneeDrv GuoJianqiu 20160429 modify for platform change begin
	./drivers/misc/mediatek/include/mt-plat/battery_common.h:15://GioneeDrv GuoJianqiu 20160429 modify for platform change end
	./drivers/misc/mediatek/include/mt-plat/battery_common.h:325://Gionee GuoJianqiu 20150514 modify for GNNCR00010646 begin
	./drivers/misc/mediatek/include/mt-plat/battery_common.h:330://Gionee GuoJianqiu 20150514 modify for GNNCR00010646 end

	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h:10://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h:12://GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h:20://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h:22://GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h:42://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h:44://GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h:45://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h:69://GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h:82://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h:84://GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h:88://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h:90://GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h:94://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h:96://GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h:108://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h:129://GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h:137:#define CUST_POWERON_DELTA_CAPACITY_TOLRANCE	25	//Gionee GuoJianqiu 20160301 modify for GNSPR #4119
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h:142://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h:144://GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h:153://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h:155://GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h:168://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter.h:170://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter_table.h:76://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter_table.h:966://GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter_table.h:1070://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_battery_meter_table.h:1627://GioneeDrv GuoJianqiu 20151223 modify for platform change end
	
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_charging.h:4://Gionee GuoJianqiu 20150312 modify for fix bugs begin
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_charging.h:7://Gionee GuoJianqiu 20150312 modify for fix bugs end
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_charging.h:11://Gionee GuoJianqiu 20151207 modify for fix CR01604790 begin
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_charging.h:19://Gionee GuoJianqiu 20151207 modify for fix CR01604790 end
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_charging.h:23://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_charging.h:29://GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_charging.h:34://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_charging.h:42://GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_charging.h:49://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_charging.h:54://GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_charging.h:70://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_charging.h:75://GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_charging.h:76://GioneeDrv GuoJianqiu 20160429 modify for platform change begin
	./drivers/misc/mediatek/include/mt-plat/mt6735/include/mach/mt_charging.h:79://GioneeDrv GuoJianqiu 20160429 modify for platform change end
	
	./drivers/misc/mediatek/include/mt-plat/charging.h:103:	//GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/include/mt-plat/charging.h:114:	//GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/misc/mediatek/include/mt-plat/battery_meter_hal.h:68://GioneeDrv GuoJianqiu 20160429 modify for platform change begin
	./drivers/misc/mediatek/include/mt-plat/battery_meter_hal.h:70://GioneeDrv GuoJianqiu 20160429 modify for platform change end
	
	./drivers/misc/mediatek/power/mt6735/Makefile:27:                    #GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/power/mt6735/Makefile:33:                    #GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/misc/mediatek/power/mt6735/bq24158.c:54:#define BQ24158_BUSNUM 2 		//Gionee GuoJianqiu 20150407 modify for fix bugs begin
	./drivers/misc/mediatek/power/mt6735/bq24158.c:658:        battery_log(BAT_LOG_CRTI,"[store_bq24158_access] buf is %s and size is %zu \n",buf,size); 	//Gionee GuoJianqiu 20150408 modify for fix errors begin
	./drivers/misc/mediatek/power/mt6735/bq24158.c:706:static int __init bq24158_subsys_init(void) 	//Gionee GuoJianqiu 20150408 modify for fix bugs begin
	./drivers/misc/mediatek/power/mt6735/bq24158.c:748://module_init(bq24158_init); 				//Gionee GuoJianqiu 20150408 modify for fix bugs begin
	./drivers/misc/mediatek/power/mt6735/bq24158.c:750:subsys_initcall(bq24158_subsys_init); 	//Gionee GuoJianqiu 20150408 modify for fix bugs end
	./drivers/misc/mediatek/power/mt6735/pmic_chr_type_det.c:67:	//GioneeDrv GuoJianqiu 20160429 modify for platform change begin
	./drivers/misc/mediatek/power/mt6735/pmic_chr_type_det.c:69:	//GioneeDrv GuoJianqiu 20160429 modify for platform change end
	./drivers/misc/mediatek/power/mt6735/pmic_chr_type_det.c:114:	//GioneeDrv GuoJianqiu 20160429 modify for platform change begin
	./drivers/misc/mediatek/power/mt6735/pmic_chr_type_det.c:116:	//GioneeDrv GuoJianqiu 20160429 modify for platform change end
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:195:extern kal_uint32 upmu_get_rgs_chrdet(void); 	//Gionee GuoJianqiu 20150214 modify for fix errs begin
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:278:    val = pmic_get_register_value_nolock(PMIC_RGS_CHRDET);	 //Gionee GuoJianqiu 20150214 modify for fix errs begin
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:289:	//Gionee GuoJianqiu 201601121 modify for GNSPR #56307 begin
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:291:	//Gionee GuoJianqiu 201601121 modify for GNSPR #56307 begin
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:300:	bc11_set_register_value(PMIC_RG_USBDL_SET,0x0);//force leave USBDL mode 	//Gionee GuoJianqiu 20150408 modify for fix bugs begin
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:301:       bc11_set_register_value(PMIC_RG_USBDL_RST,0x1);//force leave USBDL mode;	//Gionee GuoJianqiu 20150408 modify for fix bugs end
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:313:    //Gionee GuoJianqiu 201601121 modify for GNSPR #56307 begin
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:319:    //Gionee GuoJianqiu 201601121 modify for GNSPR #56307 end
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:352:    kal_uint32 bootmode = 0; 	//GioneeDrv GuoJianqiu 20151223 modify for platform change
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:379:		//GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:383:		//GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:393://BATTERY_VOLTAGE_ENUM cv_voltage=BATTERY_VOLT_04_200000_V; 	//Gionee GuoJianqiu 20150723 modify for GNNCR00011362
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:403:	//cv_voltage=VBAT_CV_VTH[register_value]; 	//Gionee GuoJianqiu 20150723 modify for GNNCR00011362
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:409:    /*if(mt6325_upmu_get_swcid() == PMIC6325_E1_CID_CODE)	 //Gionee GuoJianqiu 20150214 modify for fix errs begin
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:419:    else*/		//Gionee GuoJianqiu 20150214 modify for fix errs end
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:421:        /*if(is_mt6311_exist())		 //Gionee GuoJianqiu 20150214 modify for fix errs begin
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:437:        else*/		//Gionee GuoJianqiu 20150214 modify for fix errs end
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:549:    pmic_set_register_value(PMIC_RG_VCDT_HV_VTH,register_value);//;	 	//Gionee GuoJianqiu 20150214 modify for fix errs begin
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:563:    *(kal_bool*)(data) = pmic_get_register_value(PMIC_RGS_VCDT_HV_DET); 		//Gionee GuoJianqiu 20150214 modify for fix errs begin
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:579:    pmic_set_register_value(PMIC_BATON_TDET_EN,1); 	//Gionee GuoJianqiu 20150724 modify for GNSPR00037414
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:580:    val=pmic_get_register_value(PMIC_BATON_TDET_EN); 	//Gionee GuoJianqiu 20150214 modify for fix errs begin
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:583:    	pmic_set_register_value(PMIC_BATON_TDET_EN,1);		 //Gionee GuoJianqiu 20150214 modify for fix errs begin
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:584:    	pmic_set_register_value(PMIC_RG_BATON_EN,1);		 //Gionee GuoJianqiu 20150214 modify for fix errs begin
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:585:    	*(kal_bool*)(data) = pmic_get_register_value(PMIC_RGS_BATON_UNDET);		 //Gionee GuoJianqiu 20150214 modify for fix errs begin
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:605:    val = pmic_get_register_value_nolock(PMIC_RGS_CHRDET); 	//Gionee GuoJianqiu 20150214 modify for fix errs begin
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:607:    if(((g_diso_state >> 1) & 0x3) != 0x0 || pmic_get_register_value_nolock(PMIC_RGS_CHRDET)) 	//Gionee GuoJianqiu 20150214 modify for fix errs begin
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:711:	kernel_restart("battery service reboot system"); 	//GioneeDrv GuoJianqiu 20151223 modify for platform change
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:738:	//GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:743:	//GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:1408:        ,charging_set_error_state 	//Gionee GuoJianqiu 20150408 modify for fix bugs begin
	./drivers/misc/mediatek/power/mt6735/charging_hw_bq24158.c:1411:};								//Gionee GuoJianqiu 20150408 modify for fix bugs end
	
	./drivers/misc/mediatek/power/Kconfig:8:#GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/misc/mediatek/power/Kconfig:17:#GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/power/power_supply_sysfs.c:217:	//GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/power/power_supply_sysfs.c:219:	//GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/power/mediatek/switch_charging.c:80://GioneeDrv GuoJianqiu 20160429 modify for platform change begin
	./drivers/power/mediatek/switch_charging.c:82://GioneeDrv GuoJianqiu 20160429 modify for platform change end
	./drivers/power/mediatek/switch_charging.c:108://Gionee GuoJianqiu 20150514 modify for GNNCR00010646 begin
	./drivers/power/mediatek/switch_charging.c:112://Gionee GuoJianqiu 20150514 modify for GNNCR00010646 end
	./drivers/power/mediatek/switch_charging.c:136: extern kal_bool upmu_is_chr_det(void);	//Gionee GuoJianqiu 20150603 modify for fix CR01492674
	./drivers/power/mediatek/switch_charging.c:138://Gionee GuoJianqiu 20150611 modify for CR01498036 begin
	./drivers/power/mediatek/switch_charging.c:149: //Gionee GuoJianqiu 20150611 modify for CR01498036 end
	./drivers/power/mediatek/switch_charging.c:734://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/power/mediatek/switch_charging.c:745://GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/power/mediatek/switch_charging.c:755:	//GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/power/mediatek/switch_charging.c:769:	//GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/power/mediatek/switch_charging.c:773:			g_temp_CC_value = CHARGE_CURRENT_500_00_MA;	//Gionee GuoJianqiu 20150610 modify for fix CR01498761
	./drivers/power/mediatek/switch_charging.c:791:			g_temp_CC_value = CHARGE_CURRENT_1500_00_MA;	//Gionee GuoJianqiu 20150417 modify for fix errs begin
	./drivers/power/mediatek/switch_charging.c:794:		//Gionee GuoJianqiu 20150514 modify for GNNCR00010646 begin
	./drivers/power/mediatek/switch_charging.c:799:			//GioneeDrv GuoJianqiu 20160429 modify for platform change begin
	./drivers/power/mediatek/switch_charging.c:807:			//GioneeDrv GuoJianqiu 20160429 modify for platform change end
	./drivers/power/mediatek/switch_charging.c:810:		//Gionee GuoJianqiu 20150514 modify for GNNCR00010646 end
	./drivers/power/mediatek/switch_charging.c:814:		g_temp_CC_value = CHARGE_CURRENT_500_00_MA;	//Gionee GuoJianqiu 20150610 modify for fix CR01498761
	./drivers/power/mediatek/switch_charging.c:817:	//Gionee GuoJianqiu 20150608 modify for fix CR01498099 begin
	./drivers/power/mediatek/switch_charging.c:821:	//Gionee GuoJianqiu 20150608 modify for fix CR01498099 end
	./drivers/power/mediatek/switch_charging.c:991:			g_temp_CC_value = CHARGE_CURRENT_500_00_MA;	//Gionee GuoJianqiu 20150610 modify for fix CR01498761
	./drivers/power/mediatek/switch_charging.c:999:	if (upmu_is_chr_det() == KAL_TRUE)	//Gionee GuoJianqiu 20150603 modify for fix CR01492674
	./drivers/power/mediatek/switch_charging.c:1061:			//GioneeDrv GuoJianqiu 20160429 modify for platform change begin
	./drivers/power/mediatek/switch_charging.c:1069:			//GioneeDrv GuoJianqiu 20160429 modify for platform change end
	./drivers/power/mediatek/switch_charging.c:1107:	//GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/power/mediatek/switch_charging.c:1116:		//Gionee GuoJianqiu 20141130 modify for GNSPR00027029 begin
	./drivers/power/mediatek/switch_charging.c:1118:		//Gionee GuoJianqiu 20141130 modify for GNSPR00027029 end
	./drivers/power/mediatek/switch_charging.c:1120:	//GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/power/mediatek/switch_charging.c:1167:		//GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/power/mediatek/switch_charging.c:1181:		//GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/power/mediatek/switch_charging.c:1207:			//GioneeDrv GuoJianqiu 20160429 modify for platform change begin
	./drivers/power/mediatek/switch_charging.c:1214:			//GioneeDrv GuoJianqiu 20160429 modify for platform change end
	./drivers/power/mediatek/switch_charging.c:1230:	battery_log(BAT_LOG_CRTI, "[BATTERY] pchr_turn_on_charging(), enable =%d !\r\n",	//Gionee GuoJianqiu 20150318 modify for fix bugs
	./drivers/power/mediatek/switch_charging.c:1267:	battery_log(BAT_LOG_CRTI, "[BATTERY] CC mode charge, timer=%d on %d !!\n\r",	//Gionee GuoJianqiu 20150318 modify for fix bugs
	./drivers/power/mediatek/switch_charging.c:1278:    //Gionee GuoJianqiu 20141130 modify for GNSPR00027029
	./drivers/power/mediatek/switch_charging.c:1301:	//Gionee GuoJianqiu 20141130 modify for GNSPR00027029
	./drivers/power/mediatek/battery_meter_fg_20.c:60://GioneeDrv GuoJianqiu 20160429 modify for platform change begin
	./drivers/power/mediatek/battery_meter_fg_20.c:62://GioneeDrv GuoJianqiu 20160429 modify for platform change end
	./drivers/power/mediatek/battery_meter_fg_20.c:1072:	//GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/power/mediatek/battery_meter_fg_20.c:1074:	//GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/power/mediatek/battery_meter_fg_20.c:1094:    	//Gionee GuoJianqiu 20150318 modify for fix bugs begin
	./drivers/power/mediatek/battery_meter_fg_20.c:1100:    	//Gionee GuoJianqiu 20150318 modify for fix bugs end
	./drivers/power/mediatek/battery_meter_fg_20.c:1257:    	//Gionee GuoJianqiu 20150318 modify for fix bugs begin
	./drivers/power/mediatek/battery_meter_fg_20.c:1263:    	//Gionee GuoJianqiu 20150318 modify for fix bugs end
	./drivers/power/mediatek/battery_meter_fg_20.c:1417:	//Gionee GuoJianqiu 20150318 modify for fix bugs begin
	./drivers/power/mediatek/battery_meter_fg_20.c:1422:	//Gionee GuoJianqiu 20150318 modify for fix bugs end	
	./drivers/power/mediatek/battery_meter_fg_20.c:1521://Gionee GuoJianqiu 20141216 modify for GNSPR00029961 begin
	./drivers/power/mediatek/battery_meter_fg_20.c:1525://Gionee GuoJianqiu 20141216 modify for GNSPR00029961 end
	./drivers/power/mediatek/battery_meter_fg_20.c:1562://Gionee GuoJianqiu 20150612 modify for CR01481562 begin
	./drivers/power/mediatek/battery_meter_fg_20.c:1572://Gionee GuoJianqiu 20150612 modify for CR01481562 end
	./drivers/power/mediatek/battery_meter_fg_20.c:1577://Gionee GuoJianqiu 20150612 modify for CR01481562 begin
	./drivers/power/mediatek/battery_meter_fg_20.c:1582://Gionee GuoJianqiu 20150612 modify for CR01481562 end
	./drivers/power/mediatek/battery_meter_fg_20.c:1689:	//GioneeDrv GuoJianqiu 20160908 modify for GNSPR #42886 begin
	./drivers/power/mediatek/battery_meter_fg_20.c:1692:	//GioneeDrv GuoJianqiu 20160908 modify for GNSPR #42886 end
	./drivers/power/mediatek/battery_meter_fg_20.c:1849://GioneeDrv GuoJianqiu 20160429 modify for platform change begin
	./drivers/power/mediatek/battery_meter_fg_20.c:1852://GioneeDrv GuoJianqiu 20160429 modify for platform change end
	./drivers/power/mediatek/battery_meter_fg_20.c:1942://Gionee GuoJianqiu 20150424 modify for fix bugs begin
	./drivers/power/mediatek/battery_meter_fg_20.c:1966://Gionee GuoJianqiu 20150313 modify for fix bugs end
	./drivers/power/mediatek/battery_meter.c:52://GioneeDrv GuoJianqiu 20160429 modify for platform change begin
	./drivers/power/mediatek/battery_meter.c:54://GioneeDrv GuoJianqiu 20160429 modify for platform change end
	./drivers/power/mediatek/battery_meter.c:258://Gionee GuoJianqiu 20150318 modify for fix CR01549150 begin
	./drivers/power/mediatek/battery_meter.c:290://Gionee GuoJianqiu 20150318 modify for fix CR01549150 end
	./drivers/power/mediatek/battery_meter.c:4451://Gionee GuoJianqiu 20150318 modify for fix CR01549150 begin
	./drivers/power/mediatek/battery_meter.c:4455://Gionee GuoJianqiu 20150318 modify for fix CR01549150 end
	./drivers/power/mediatek/battery_common_fg_20.c:263:	//Gionee GuoJianqiu 20150318 modify for fix bugs begin
	./drivers/power/mediatek/battery_common_fg_20.c:265:	//Gionee GuoJianqiu 20150318 modify for fix bugs end
	./drivers/power/mediatek/battery_common_fg_20.c:302:	//Gionee GuoJianqiu 20150318 modify for fix bugs begin
	./drivers/power/mediatek/battery_common_fg_20.c:304:	//Gionee GuoJianqiu 20150318 modify for fix bugs end
	./drivers/power/mediatek/battery_common_fg_20.c:316://Gionee GuoJianqiu 20150611 modify for CR01498036 begin
	./drivers/power/mediatek/battery_common_fg_20.c:319://Gionee GuoJianqiu 20150611 modify for CR01498036 end
	./drivers/power/mediatek/battery_common_fg_20.c:617:	//Gionee GuoJianqiu 20150424 modify for fix bugs begin
	./drivers/power/mediatek/battery_common_fg_20.c:621:	//Gionee GuoJianqiu 20150318 modify for fix bugs end
	./drivers/power/mediatek/battery_common_fg_20.c:721:	//GioneeDrv GuoJianqiu 20160429 modify for platform change begin
	./drivers/power/mediatek/battery_common_fg_20.c:723:	//GioneeDrv GuoJianqiu 20160429 modify for platform change end

	./drivers/power/mediatek/battery_common_fg_20.c:1664://Gionee GuoJianqiu 20150514 modify for GNNCR00010646 begin
	
	./drivers/power/mediatek/battery_common_fg_20.c:1680:	//GioneeDrv GuoJianqiu 20160429 modify for platform change begin
	./drivers/power/mediatek/battery_common_fg_20.c:1695:	//GioneeDrv GuoJianqiu 20160429 modify for platform change end

	./drivers/power/mediatek/battery_common_fg_20.c:1713:	//GioneeDrv GuoJianqiu 20160429 modify for platform change begin
	./drivers/power/mediatek/battery_common_fg_20.c:1728:	//GioneeDrv GuoJianqiu 20160429 modify for platform change end
	./drivers/power/mediatek/battery_common_fg_20.c:1731://Gionee GuoJianqiu 20150514 modify for GNNCR00010646 end
	./drivers/power/mediatek/battery_common_fg_20.c:1740:	//Gionee GuoJianqiu 20150424 modify for fix bugs begin
	./drivers/power/mediatek/battery_common_fg_20.c:1742:	//Gionee GuoJianqiu 20150318 modify for fix bugs end
	./drivers/power/mediatek/battery_common_fg_20.c:1825:			//Gionee GuoJianqiu 20161113 modify for fix GNSPR #54689 begin
	./drivers/power/mediatek/battery_common_fg_20.c:1827:			//Gionee GuoJianqiu 20161113 modify for fix GNSPR #54689 end
	./drivers/power/mediatek/battery_common_fg_20.c:1976:	return (BMT_status.bat_in_recharging_state /*|| BMT_status.bat_full == KAL_TRUE*/);	//Gionee GuoJianqiu 20141226 modify for GNSPR00031054
	./drivers/power/mediatek/battery_common_fg_20.c:2174:	if (previous_SOC == -1 && bat_vol <= SYSTEM_OFF_VOLTAGE) {	//Gionee GuoJianqiu 20160217 modify for GNSPR4723
	./drivers/power/mediatek/battery_common_fg_20.c:2207:	//GioneeDrv GuoJianqiu 20160429 modify for platform change begin
	./drivers/power/mediatek/battery_common_fg_20.c:2213:	//GioneeDrv GuoJianqiu 20160429 modify for platform change end
	./drivers/power/mediatek/battery_common_fg_20.c:2214:	//Gionee GuoJianqiu 20150318 modify for fix bugs begin
	./drivers/power/mediatek/battery_common_fg_20.c:2216:	//Gionee GuoJianqiu 20150424 modify for fix bugs end
	./drivers/power/mediatek/battery_common_fg_20.c:2217:	//Gionee GuoJianqiu 20150611 modify for CR01498036 begin
	./drivers/power/mediatek/battery_common_fg_20.c:2223:	//Gionee GuoJianqiu 20150611 modify for CR01498036 end
	./drivers/power/mediatek/battery_common_fg_20.c:2292:        //Gionee GuoJianqiu 20150318 modify for fix bugs begin
	./drivers/power/mediatek/battery_common_fg_20.c:2303:        //Gionee GuoJianqiu 20150318 modify for fix bugs end
	./drivers/power/mediatek/battery_common_fg_20.c:2410:	//Gionee GuoJianqiu 20150318 modify for fix bugs begin
	./drivers/power/mediatek/battery_common_fg_20.c:2416:	//Gionee GuoJianqiu 20150318 modify for fix bugs end
	./drivers/power/mediatek/battery_common_fg_20.c:2468:		//Gionee GuoJianqiu 20150318 modify for fix bugs begin
	./drivers/power/mediatek/battery_common_fg_20.c:2470:		//Gionee GuoJianqiu 20150318 modify for fix bugs end
	./drivers/power/mediatek/battery_common_fg_20.c:2613://Gionee GuoJianqiu 20141210 modify for GNSPR00027866 begin
	./drivers/power/mediatek/battery_common_fg_20.c:2625://Gionee GuoJianqiu 20141210 modify for GNSPR00027866 begin
	./drivers/power/mediatek/battery_common_fg_20.c:2663:		//Gionee GuoJianqiu 20151201 modify for fix CR01595060 begin
	./drivers/power/mediatek/battery_common_fg_20.c:2672:		//Gionee GuoJianqiu 20151201 modify for fix CR01595060 end
	./drivers/power/mediatek/battery_common_fg_20.c:2816:		//Gionee GuoJianqiu 201601117 modify for GNSPR #54468 begin
	./drivers/power/mediatek/battery_common_fg_20.c:2818:		//Gionee GuoJianqiu 201601117 modify for GNSPR #54468 end
	./drivers/power/mediatek/battery_common_fg_20.c:3041:	battery_log(BAT_LOG_CRTI, "******** battery : bat_thread_wakeup  ********\n");	//Gionee GuoJianqiu 20150318 modify for fix bugs begin
	./drivers/power/mediatek/battery_common_fg_20.c:3069:	battery_log(BAT_LOG_CRTI, "******** battery : bat_update_thread_wakeup  ********\n"); 	//Gionee GuoJianqiu 20150318 modify for fix bugs
	./drivers/power/mediatek/battery_common_fg_20.c:3294:	battery_log(BAT_LOG_CRTI, "[check_battery_exist] baton_count = %d\n ", baton_count);	//Gionee GuoJianqiu 20150724 modify for GNSPR00037414	
	./drivers/power/mediatek/battery_common_fg_20.c:3303:					    "[BATTERY] Battery is not exist, power off Charge IC and system (%d)\n",	//Gionee GuoJianqiu 20150318 modify for fix bugs
	./drivers/power/mediatek/battery_common_fg_20.c:3307:			//Gionee GuoJianqiu 20150724 modify for GNSPR00037414 begin
	./drivers/power/mediatek/battery_common_fg_20.c:3313:			//Gionee GuoJianqiu 20150724 modify for GNSPR00037414 end
	./drivers/power/mediatek/battery_common_fg_20.c:3324:	//Gionee GuoJianqiu 20150424 modify for fix bugs begin
	./drivers/power/mediatek/battery_common_fg_20.c:3326:	//Gionee GuoJianqiu 20150424 modify for fix bugs end
	./drivers/power/mediatek/battery_common_fg_20.c:3328:	//Gionee GuoJianqiu 20141210 modify for GNSPR00027866 begin
	./drivers/power/mediatek/battery_common_fg_20.c:3330:	//Gionee GuoJianqiu 20141210 modify for GNSPR00027866 end
	./drivers/power/mediatek/battery_common_fg_20.c:3351:		//Gionee GuoJianqiu 20141210 modify for GNSPR00027866 begin
	./drivers/power/mediatek/battery_common_fg_20.c:3363:		//Gionee GuoJianqiu 20141210 modify for GNSPR00027866 end
	./drivers/power/mediatek/battery_common_fg_20.c:3415:		battery_log(BAT_LOG_CRTI,	//Gionee GuoJianqiu 20150318 modify for fix bugs
	./drivers/power/mediatek/battery_common_fg_20.c:3699://GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/power/mediatek/battery_common_fg_20.c:3701://GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/power/mediatek/battery_common_fg_20.c:3906:	//GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./drivers/power/mediatek/battery_common_fg_20.c:3909:	//GioneeDrv GuoJianqiu 20151223 modify for platform change end
	./drivers/power/mediatek/battery_common_fg_20.c:4053:		//Gionee GuoJianqiu 20150514 modify for GNNCR00010646 begin
	./drivers/power/mediatek/battery_common_fg_20.c:4058:		//Gionee GuoJianqiu 20150514 modify for GNNCR00010646 end

	./drivers/power/mediatek/battery_common_fg_20.c:4155:	//Gionee GuoJianqiu 20150103 modify for GNSPR00028091 begin
	./drivers/power/mediatek/battery_common_fg_20.c:4157:	//Gionee GuoJianqiu 20150103 modify for GNSPR00028091 end
	./drivers/power/mediatek/battery_common_fg_20.c:4186:		//Gionee GuoJianqiu 20150103 modify for GNSPR00028091 begin
	./drivers/power/mediatek/battery_common_fg_20.c:4192:		//Gionee GuoJianqiu 20150103 modify for GNSPR00028091 end
	./include/linux/power_supply.h:166:	//GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	./include/linux/power_supply.h:168:	//GioneeDrv GuoJianqiu 20151223 modify for platform change end






	mt6735.dtsi,mt_charging.h


	更改OTG开关
	./arch/arm64/boot/dts/gnbj6737t_66_m0.dts:1002:/* GioneeDrv GuoJianqiu 20160425 modify for OTG function begin */
	./arch/arm64/boot/dts/gnbj6737t_66_m0.dts:1061:/* GioneeDrv GuoJianqiu 20160425 modify for OTG function end */
	./drivers/misc/mediatek/usb20/mt6735/usb20.h:21://Gionee GuoJianqiu 201601026 modify for OTG SWITCH begin
	./drivers/misc/mediatek/usb20/mt6735/usb20.h:27://Gionee GuoJianqiu 201601026 modify for OTG SWITCH end
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c:51://Gionee GuoJianqiu 201601026 modify for OTG SWITCH begin
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c:56://Gionee GuoJianqiu 201601026 modify for OTG SWITCH end
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c:309:	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH begin
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c:314:	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH end
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c:352:	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH begin
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c:356:	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH end
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c:369:	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH begin
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c:374:	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH end
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c:387:	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH begin
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c:391:	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH end
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c:527://Gionee GuoJianqiu 201601026 modify for OTG SWITCH begin
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c:574:	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH begin
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c:576:	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH end
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c:613:	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH begin
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c:615:	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH end
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c:762://Gionee GuoJianqiu 201601026 modify for OTG SWITCH end
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c:784:	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH begin
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c:797:	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH end
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c:817:	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH begin
	./drivers/misc/mediatek/usb20/mt6735/usb20_host.c:827:	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH end
	./drivers/misc/mediatek/include/mt-plat/battery_common.h:332://Gionee GuoJianqiu 201601026 modify for OTG SWITCH begin
	./drivers/misc/mediatek/include/mt-plat/battery_common.h:338://Gionee GuoJianqiu 201601026 modify for OTG SWITCH end
	./drivers/power/mediatek/battery_common_fg_20.c:1622://Gionee GuoJianqiu 201601026 modify for OTG SWITCH begin
	./drivers/power/mediatek/battery_common_fg_20.c:1662://Gionee GuoJianqiu 201601026 modify for OTG SWITCH end
	./drivers/power/mediatek/battery_common_fg_20.c:1669:	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH begin
	./drivers/power/mediatek/battery_common_fg_20.c:1674:	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH end
	./drivers/power/mediatek/battery_common_fg_20.c:1702:	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH begin
	./drivers/power/mediatek/battery_common_fg_20.c:1707:	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH end
	./drivers/power/mediatek/battery_common_fg_20.c:4060:		//Gionee GuoJianqiu 201601026 modify for OTG SWITCH begin
	./drivers/power/mediatek/battery_common_fg_20.c:4064:		//Gionee GuoJianqiu 201601026 modify for OTG SWITCH end

项目脚本	



dws文件是否需要改，看看spe和dws文件配置是否正确



	
	
	
7.BUG#67460
	
	修改SD卡的格式为exfat时，插入到测试机后不识别SD卡。
	
	【测试版本】SW17W05A_0201_T5016
	【测试步骤】将SD卡插入到读卡器，格式化SD卡格式为exfat,格式化完后将SD卡插入到测试机上查看
	【实际结果】不 能识别
	【预期结果】能识别fat fat32 exfat格式的SD卡
	
	
	fat32支持但是exfat格式是不支持的
	
	
	
	
	
	
	
	
	
	
	
	
8.BUG#66789
	手机为开机状态，长按电源键11s，手机不会自动重启	
	hal_kpd.c   keypad.h	gnbj6737t_66_m0_debug_defconfig   gnbj6737t_66_m0_defconfig
	
	CONFIG_ONEKEY_REBOOT_NORMAL_MODE=y
	CONFIG_ONEKEY_REBOOT_OTHER_MODE=y
	
	home键one，组合键two
	
	长按power键,reboot or shudown
	cust_rtc.h



9.写号失败，因为电量计初始化的原因
电量计初始化的位置不对
-- service fuelgauged /system/bin/fuelgauged
++ service fuelgauged /vendor/bin/fuelgauged
class main




10.关机充电的中文logo改成英文logo



11.充电过早截止
	现象：
	1、从1%开始充电。19秒后电量下降到0%，继续充电8分钟后电量才恢复到1%
	2、充电到96%时电流就已经截止，继续充电4分钟后电量才到100%，应该先充电到100%再电流截止。

	分析：


	1%~2%用了2min 所以第一个问题应该是电池电压较低导致充电开始时间长

	第二个问题充电截止，电量达到95%就截止了，充电应该先达到100%，然后电池电压达到多少V就截止
	所以这是后的问题是充电截止代码里在哪里，是怎么控制的

	BAT_BatteryFullAction，charging_full_check
MTK_FG: 1.[FGADC_UI_FG]FG version:150329 (FG% 100,UI% 96,tracking Time 0,Qmax_T_0mAH 4093, I 745, Iavg 1229, T 27, Tavg 27, RTC=96)

--->所以问题最后的原因是(这个问题还要思考一下)：
	跟软件上电池的容量设定有关,电池充到3800mAh就认为是充满了，
	还有就是充电的截止电流
	然后这边还有另一个问题就是正常放电


	charging_hw_bq24158.c文件里面，charging_hw_init









	






	
	
	
	1.几个宏未定义对应的回调函数，以后对比导入几个空函数，先注掉
	{
		CHARGING_CMD_SET_VINDPM,
		CHARGING_CMD_SET_VBUS_OVP_EN,
		CHARGING_CMD_GET_BIF_VBAT,
		CHARGING_CMD_SET_CHRIND_CK_PDN,
		CHARGING_CMD_SW_INIT,
		CHARGING_CMD_ENABLE_SAFETY_TIMER,
		CHARGING_CMD_SET_HIZ_SWCHR,
		CHARGING_CMD_GET_BIF_TBAT,
		CHARGING_CMD_SET_TA20_RESET,
		CHARGING_CMD_SET_TA20_CURRENT_PATTERN,
		CHARGING_CMD_SET_DP,
		CHARGING_CMD_GET_CHARGER_TEMPERATURE,
		CHARGING_CMD_SET_BOOST_CURRENT_LIMIT,
		CHARGING_CMD_ENABLE_OTG,
		CHARGING_CMD_ENABLE_POWER_PATH,
		CHARGING_CMD_GET_BIF_IS_EXIST,
		CHARGING_CMD_GET_INPUT_CURRENT,
		CHARGING_CMD_ENABLE_DIRECT_CHARGE,
		CHARGING_CMD_GET_IS_POWER_PATH_ENABLE,
		CHARGING_CMD_GET_IS_SAFETY_TIMER_ENABLE,
		CHARGING_CMD_SET_PWRSTAT_LED_EN,
		CHARGING_CMD_GET_IBUS,
		CHARGING_CMD_GET_VBUS,
		CHARGING_CMD_RESET_DC_WATCH_DOG_TIMER,
		CHARGING_CMD_RUN_AICL,
		CHARGING_CMD_SET_IRCMP_RESISTOR,
		CHARGING_CMD_SET_IRCMP_VOLT_CLAMP,
	
	}
	
	
	
	
charging.h	
	
	typedef enum {
		CHARGING_CMD_INIT,
		CHARGING_CMD_DUMP_REGISTER,
		CHARGING_CMD_ENABLE,
		CHARGING_CMD_SET_CV_VOLTAGE,
		CHARGING_CMD_GET_CURRENT,
		CHARGING_CMD_SET_CURRENT,
		CHARGING_CMD_SET_INPUT_CURRENT,
		CHARGING_CMD_GET_CHARGING_STATUS,
		CHARGING_CMD_RESET_WATCH_DOG_TIMER,
		CHARGING_CMD_SET_HV_THRESHOLD,
		CHARGING_CMD_GET_HV_STATUS,
		CHARGING_CMD_GET_BATTERY_STATUS,
		CHARGING_CMD_GET_CHARGER_DET_STATUS,
		CHARGING_CMD_GET_CHARGER_TYPE,
		CHARGING_CMD_GET_IS_PCM_TIMER_TRIGGER,
		CHARGING_CMD_SET_PLATFORM_RESET,
		CHARGING_CMD_GET_PLATFORM_BOOT_MODE,
		CHARGING_CMD_SET_POWER_OFF,
		CHARGING_CMD_GET_POWER_SOURCE,
		CHARGING_CMD_GET_CSDAC_FALL_FLAG,
		CHARGING_CMD_SET_TA_CURRENT_PATTERN,
		CHARGING_CMD_SET_ERROR_STATE,
		CHARGING_CMD_DISO_INIT,
		CHARGING_CMD_GET_DISO_STATE,
	
		//GioneeDrv GuoJianqiu 20151223 modify for platform change begin
		#if defined(CONFIG_MTK_BQ25896_SUPPORT)
		CHARGING_CMD_SET_VINDPM,
		CHARGING_CMD_SET_VBUS_OVP_EN,
		CHARGING_CMD_GET_BIF_VBAT,
		CHARGING_CMD_SET_CHRIND_CK_PDN,
		CHARGING_CMD_SW_INIT,
		CHARGING_CMD_ENABLE_SAFETY_TIMER,
		CHARGING_CMD_SET_HIZ_SWCHR,
		CHARGING_CMD_GET_BIF_TBAT,
		CHARGING_CMD_SET_TA20_RESET,
		CHARGING_CMD_SET_TA20_CURRENT_PATTERN,
		CHARGING_CMD_SET_DP,
		CHARGING_CMD_GET_CHARGER_TEMPERATURE,
		CHARGING_CMD_SET_BOOST_CURRENT_LIMIT,
		CHARGING_CMD_ENABLE_OTG,
		CHARGING_CMD_ENABLE_POWER_PATH,
		CHARGING_CMD_GET_BIF_IS_EXIST,
		CHARGING_CMD_GET_INPUT_CURRENT,
		CHARGING_CMD_ENABLE_DIRECT_CHARGE,
		CHARGING_CMD_GET_IS_POWER_PATH_ENABLE,
		CHARGING_CMD_GET_IS_SAFETY_TIMER_ENABLE,
		CHARGING_CMD_SET_PWRSTAT_LED_EN,
		CHARGING_CMD_GET_IBUS,
		CHARGING_CMD_GET_VBUS,
		CHARGING_CMD_RESET_DC_WATCH_DOG_TIMER,
		CHARGING_CMD_RUN_AICL,
		CHARGING_CMD_SET_IRCMP_RESISTOR,
		CHARGING_CMD_SET_IRCMP_VOLT_CLAMP,
		#endif
		//GioneeDrv GuoJianqiu 20151223 modify for platform change end

		CHARGING_CMD_NUMBER
	} CHARGING_CTRL_CMD;	
	
	
	
	
	
charging_hw_bq24158: bq24158充电芯片支持的功能

	static kal_uint32 (* const charging_func[CHARGING_CMD_NUMBER])(void *data)=
	{
		    charging_hw_init
		    ,charging_dump_register      
		    ,charging_enable
		    ,charging_set_cv_voltage
		    ,charging_get_current
		    ,charging_set_current
		    ,charging_set_input_current
		    ,charging_get_charging_status
		    ,charging_reset_watch_dog_timer
		    ,charging_set_hv_threshold
		    ,charging_get_hv_status
		    ,charging_get_battery_status
		    ,charging_get_charger_det_status
		    ,charging_get_charger_type
		    ,charging_get_is_pcm_timer_trigger
		    ,charging_set_platform_reset
		    ,charging_get_platfrom_boot_mode
		    ,charging_set_power_off
		    ,charging_get_power_source
		    ,charging_get_csdac_full_flag
		    ,charging_set_ta_current_pattern
		    ,charging_set_error_state 	//Gionee GuoJianqiu 20150408 modify for fix bugs begin
		    ,charging_diso_init
		    ,charging_get_diso_state
	};		
	
	
	
	
	
	
	
	2.打印debug log的等级，log控制的开关
	
	battery_meter_fg_20.c



	battery_meter_hal.h

	Enable_FGADC_LOG  GM2.0使能打印log的等级，只有等级比这个高的才会打印
	#define BM_DAEMON_DEFAULT_LOG_LEVEL   
	Enable_FGADC_LOG
	
	#define bm_debug(fmt, args...)   \
	do {									\
		if (Enable_FGADC_LOG >= BMLOG_DEBUG_LEVEL) {		\
			pr_debug(fmt, ##args); \
		}								   \
	} while (0)
	
	
	






区别：

1.定义了快充接口

2.EMMI测试

3.GN_BATTERY_OTGCHARGE_SWITCH_SUPPORT   OTG开关，OTG功能相关的

3.常用的宏改了，改成变量

4.GM2.0电量计算算法没用到

5.寄存器，调用底层命令相关的宏函数
