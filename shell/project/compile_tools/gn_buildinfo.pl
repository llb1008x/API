#!/usr/bin/perl

################################################################################
#Gionee property area of robot, don't touch it. begin
################################################################################

#mmi start
#mtk mmi start
#mtk mmi end 

#qcom mmi start
if(defined($ENV{GN_RW_GN_MMI_AUTOTEST3})) {
	print "gn.mmi.autotest3=$ENV{GN_RW_GN_MMI_AUTOTEST3}\n";
}

#GIonee kongjj 20160418 add for AP_SN start
if(defined($ENV{GN_RW_GN_MMI_BACKUP_TO_PRODUCTINFO})) {
	print "gn.mmi.backup.to.pruductinfo=$ENV{GN_RW_GN_MMI_BACKUP_TO_PRODUCTINFO}\n";
}
#GIonee kongjj 20160418 add for AP_SN end

if(defined($ENV{GN_RW_GN_MMI_PRESSURETEST})) {
	print "gn.mmi.pressuretest=$ENV{GN_RW_GN_MMI_PRESSURETEST}\n";
}
if(defined($ENV{GN_RW_GN_MMI_PHONELOOPBACK2})) {
	print "gn.mmi.phoneloopback2=$ENV{GN_RW_GN_MMI_PHONELOOPBACK2}\n";
}
if(defined($ENV{GN_RW_GN_MMI_PHONELOOPBACK3})) {
	print "gn.mmi.phoneloopback3=$ENV{GN_RW_GN_MMI_PHONELOOPBACK3}\n";
}
#qcom mmi end
#mmi end

if(defined($ENV{GN_RO_GN_GNVERNUMBERREL})) {
	print "ro.gn.gnvernumberrel=$ENV{GN_RO_GN_GNVERNUMBERREL}\n";
}

if(defined($ENV{GN_RO_CONFIG_NOTIFICATION_SOUND})) {
	print "ro.config.notification_sound=$ENV{GN_RO_CONFIG_NOTIFICATION_SOUND}\n";
}

if(defined($ENV{GN_RO_CONFIG_ALARM_ALERT})) {
	print "ro.config.alarm_alert=$ENV{GN_RO_CONFIG_ALARM_ALERT}\n";
}

if(defined($ENV{GN_RO_CONFIG_RINGTONE})) {
	print "ro.config.ringtone=$ENV{GN_RO_CONFIG_RINGTONE}\n";
}

#Gionee fuziqing 20160529 delete for GNDCR #14232 begin
#if(defined($ENV{GN_RO_CONFIG_RINGTONE})) {
#	print "ro.config.wallpaper_component=$ENV{GN_RO_CONFIG_WALLPAPER_COMPONENT}\n";
#}
#Gionee fuziqing 20160529 delete for GNDCR #14232 end

if(defined($ENV{GN_RO_GN_DEFAULT_RINGTONE2})) {
	print "ro.gn.default.ringtone2=$ENV{GN_RO_GN_DEFAULT_RINGTONE2}\n";
}

if(defined($ENV{GN_RO_GN_DEFAULT_MMSTONE})) {
	print "ro.gn.default.mmstone=$ENV{GN_RO_GN_DEFAULT_MMSTONE}\n";
}

if(defined($ENV{GN_RO_GN_DEFAULT_MMSTONE2})) {
	print "ro.gn.default.mmstone2=$ENV{GN_RO_GN_DEFAULT_MMSTONE2}\n";
}

if(defined($ENV{GN_RO_GN_DEFAULT_VIDEOCALL})) {
	print "ro.gn.default.videocall=$ENV{GN_RO_GN_DEFAULT_VIDEOCALL}\n";
}

if(defined($ENV{GN_RO_GN_DEFAULT_INPUTMETHOD})) {
	print "ro.gn.default.inputmethod=$ENV{GN_RO_GN_DEFAULT_INPUTMETHOD}\n";
}

if(defined($ENV{GN_RO_GN_OP_SPECIAL_VN})) {
	print "ro.gn.op_special_vn=$ENV{GN_RO_GN_OP_SPECIAL_VN}\n";
}

if(defined($ENV{GN_RO_GN_SDGCALL_ACCESS_SUPPORT})) {
	print "ro.gn.sdgcall.access.support=$ENV{GN_RO_GN_SDGCALL_ACCESS_SUPPORT}\n";
}

if(defined($ENV{GN_RO_GN_HAND_ANSWER_SUPPORT})) {
	print "ro.gn.hand.answer.support=$ENV{GN_RO_GN_HAND_ANSWER_SUPPORT}\n";
}

if(defined($ENV{GN_RO_GN_PHONE_CLOSEDUALMIC})) {
	print "ro.gn.phone.closedualmic=$ENV{GN_RO_GN_PHONE_CLOSEDUALMIC}\n";
}

if(defined($ENV{GN_RO_BUILD_DISPLAY_ID})) {
	print "ro.build.display.id=$ENV{GN_RO_BUILD_DISPLAY_ID}\n";
}

if(defined($ENV{GN_PRODUCT_ORIGIN_MODEL})) {
	print "gn.product.origin.model=$ENV{GN_PRODUCT_ORIGIN_MODEL}\n";
}

if(defined($ENV{GN_RO_PRODUCT_MODEL})) {
	print "ro.product.model=$ENV{GN_RO_PRODUCT_MODEL}\n";
}

if(defined($ENV{GN_RO_PRODUCT_BRAND})) {
	print "ro.product.brand=$ENV{GN_RO_PRODUCT_BRAND}\n";
}

if(defined($ENV{GN_RO_PRODUCT_NAME})) {
	print "ro.product.name=$ENV{GN_RO_PRODUCT_NAME}\n";
}

if(defined($ENV{GN_RO_PRODUCT_DEVICE})) {
	print "ro.product.device=$ENV{GN_RO_PRODUCT_DEVICE}\n";
}

if(defined($ENV{GN_RO_GN_GNROMVERNUMBER})) {
	print "ro.gn.gnromvernumber=$ENV{GN_RO_GN_GNROMVERNUMBER}\n";
}

if(defined($ENV{GN_RO_GN_EXTVERNUMBER})) {
	print "ro.gn.extvernumber=$ENV{GN_RO_GN_EXTVERNUMBER}\n";
}

if(defined($ENV{GN_RO_GN_EXTMODEL})) {
	print "ro.gn.extmodel=$ENV{GN_RO_GN_EXTMODEL}\n";
}

if(defined($ENV{GN_RO_PRODUCT_MANUFACTURER})) {
	print "ro.product.manufacturer=$ENV{GN_RO_PRODUCT_MANUFACTURER}\n";
}

if(defined($ENV{GN_RO_GN_HAPTIC_VIBRATOR_PROP})) {
	print "ro.gn.haptic.vibrator.prop=$ENV{GN_RO_GN_HAPTIC_VIBRATOR_PROP}\n";
}

if(defined($ENV{GN_RO_GN_MUSIC_STANDARD_HEADSET})) {
	print "ro.gn.music.st.headset=$ENV{GN_RO_GN_MUSIC_STANDARD_HEADSET}\n";
}

if(defined($ENV{GN_RO_GN_MUSIC_DTS})) {
	print "ro.gn.music.dts=$ENV{GN_RO_GN_MUSIC_DTS}\n";
}

if(defined($ENV{GN_RO_GN_MUSIC_SRS})) {
	print "ro.gn.music.srs=$ENV{GN_RO_GN_MUSIC_SRS}\n";
}

if(defined($ENV{GN_RO_GN_SDCARD_TYPE})) {
	print "ro.gn.sdcard.type=$ENV{GN_RO_GN_SDCARD_TYPE}\n";
}

if(defined($ENV{GN_RO_GN_AMOLED_LCMACL_SUPPORT})) {
	print "ro.gn.amoled.lcmacl.support=$ENV{GN_RO_GN_AMOLED_LCMACL_SUPPORT}\n";
}

if(defined($ENV{GN_RO_GN_SOUNDCTRL_SUPPORT})) {
	print "ro.gn.soundctrl.support=$ENV{GN_RO_GN_SOUNDCTRL_SUPPORT}\n";
}

if(defined($ENV{GN_RO_GN_MTK_POWEROFF_ALARM_PROP})) {
	print "ro.gn.mtk.poweroff.alarm.prop=$ENV{GN_RO_GN_MTK_POWEROFF_ALARM_PROP}\n";
}

if(defined($ENV{GN_RO_GN_VOICEWAKE_SUPPORT})) {
	print "ro.gn.voicewake.support=$ENV{GN_RO_GN_VOICEWAKE_SUPPORT}\n";
}

if(defined($ENV{GN_RO_GN_CLICKWAKE_SUPPORT})) {
	print "ro.gn.clickwake.support=$ENV{GN_RO_GN_CLICKWAKE_SUPPORT}\n";
}

if(defined($ENV{GN_RO_GN_SINGLE_HAND_SUPPORT})) {
	print "ro.gn.single.hand.support=$ENV{GN_RO_GN_SINGLE_HAND_SUPPORT}\n";
}

if(defined($ENV{GN_RO_GN_QUICKOPERATE_SUPPORT})) {
	print "ro.gn.quickoperate.support=$ENV{GN_RO_GN_QUICKOPERATE_SUPPORT}\n";
}

if(defined($ENV{GN_RO_GN_GLOVE_PATTERNS_SUPPORT})) {
	print "ro.gn.glove_patterns.support=$ENV{GN_RO_GN_GLOVE_PATTERNS_SUPPORT}\n";
}

if(defined($ENV{GN_RO_GN_RESPIRATIONLAMP_SUPPORT})) {
	print "ro.gn.respirationlamp.support=$ENV{GN_RO_GN_RESPIRATIONLAMP_SUPPORT}\n";
}

if(defined($ENV{GN_RO_GN_OPTRSPECSEGDEF})) {
	print "ro.gn.optrspecsegdef=$ENV{GN_RO_GN_OPTRSPECSEGDEF}\n";
}

if(defined($ENV{GN_RO_GN_PLATFORM_SUPPORT})) {
	print "ro.gn.platform.support=$ENV{GN_RO_GN_PLATFORM_SUPPORT}\n";
}

if(defined($ENV{GN_RW_GN_SENSOR_ADJUST_LCD_SUPPORT})) {
	print "gn.sensor.adjust.lcd.support=$ENV{GN_RW_GN_SENSOR_ADJUST_LCD_SUPPORT}\n";
}

if(defined($ENV{GN_RO_GN_LCD_EFFECT_SUPPORT})) {
	print "ro.gn.lcd.effect.support=$ENV{GN_RO_GN_LCD_EFFECT_SUPPORT}\n";
}

if(defined($ENV{GN_RO_GN_GN2SDCARDSWAP})) {
	print "ro.gn.gn2sdcardswap=$ENV{GN_RO_GN_GN2SDCARDSWAP}\n";
}

if(defined($ENV{GN_RO_PRODUCT_SCREENSIZE})) {
	print "ro.product.screensize=$ENV{GN_RO_PRODUCT_SCREENSIZE}\n";
}
#gionee gongshicheng 2016-06-09 add for gndcr18247 start
if(defined($ENV{GIONEE_VERSION_BY})) {
	print "ro.build.version.GNBYversion=$ENV{GIONEE_VERSION_BY}\n";
}
#gionee gongshicheng 2016-06-09 add for gndcr18247 end

#if(defined($ENV{GN_RO_IREADER_CHANNEL})) {
#	print "ro.ireader.channel=$ENV{GN_RO_IREADER_CHANNEL}\n";
#}

#Gionee_BJY3: wangch@gionee.com 2016.5.17 for project BBL7516A start
#if(defined($GN_IREADER_CHANNEL)) {
#    print "ro.ireader.channel=$GN_IREADER_CHANNEL\n";
#}
if(defined($ENV{GN_IREADER_CHANNEL})) {
    print "ro.ireader.channel=$ENV{GN_IREADER_CHANNEL}\n";
}
#Gionee_BJY3: wangch@gionee.com 2016.5.17 for project BBL7516A  end

if(defined($ENV{GN_RO_IREADER_CHANNEL})) {
	print "ro.ireader.channel=$ENV{GN_RO_IREADER_CHANNEL}\n";
}

if(defined($ENV{GN_RO_GN_CTASEC_SUPPORT})) {
	print "ro.gn.ctasec.support=$ENV{GN_RO_GN_CTASEC_SUPPORT}\n";
}

if(defined($ENV{GN_RO_GN_TDSCDMA_SUPPORT})) {
	print "ro.gn.tdscdma.support=$ENV{GN_RO_GN_TDSCDMA_SUPPORT}\n";
}

if(defined($ENV{GN_RW_RUNTIME_CTS_TEST})) {
	print "runtime.cts.test=$ENV{GN_RW_RUNTIME_CTS_TEST}\n";
}

if(defined($ENV{GN_RO_GN_GNPRODUCTID})) {
	print "ro.gn.gnproductid=$ENV{GN_RO_GN_GNPRODUCTID}\n";
}

if(defined($ENV{GN_RO_GN_GNPROJECTID})) {
	print "ro.gn.gnprojectid=$ENV{GN_RO_GN_GNPROJECTID}\n";
}

if(defined($ENV{GN_RO_GN_GNZNVERNUMBER})) {
	print "ro.gn.gnznvernumber=$ENV{GN_RO_GN_GNZNVERNUMBER}\n";
}

if(defined($ENV{GN_RO_GN_GNVERNUMBER})) {
	print "ro.gn.gnvernumber=$ENV{GN_RO_GN_GNVERNUMBER}\n";
}

if(defined($ENV{GN_RO_GN_NFC_SUPPORT})) {
	print "ro.gn.nfc.support=$ENV{GN_RO_GN_NFC_SUPPORT}\n";
}

if(defined($ENV{GN_RO_GN_GSD_PROP})) {
	print "ro.gn.gsd.prop=$ENV{GN_RO_GN_GSD_PROP}\n";
}

if(defined($ENV{GN_RO_GN_EFFECT_OPEN})) {
	print "ro.gn.effect.open=$ENV{GN_RO_GN_EFFECT_OPEN}\n";
}

if(defined($ENV{GN_RO_GN_DISTANCEGESTURE_SUPPORT})) {
	print "ro.gn.distancegesture.support=$ENV{GN_RO_GN_DISTANCEGESTURE_SUPPORT}\n";
}

if(defined($ENV{GN_RO_GN_NEW_SCREEN_TIMEOUT})) {
	print "ro.gn.new.screen.timeout=$ENV{GN_RO_GN_NEW_SCREEN_TIMEOUT}\n";
}

if(defined($ENV{GN_RO_GN_HARDWAREVER})) {
	print "ro.gn.hardwarever=$ENV{GN_RO_GN_HARDWAREVER}\n";
}

if(defined($ENV{GN_RO_GN_USB_UI_SUPPORT})) {
	print "ro.gn.usb.ui.support=$ENV{GN_RO_GN_USB_UI_SUPPORT}\n";
}

if(defined($ENV{GN_RO_GN_SV_VERSION}) || defined($ENV{GN_SV_VERSION})) {
	print "ro.gn.sv.version=$ENV{GN_SV_VERSION}\n";
}

if(defined($ENV{GN_RO_GN_GESTURE_SUPPORT})) {
	print "ro.gn.gesture.support=$ENV{GN_RO_GN_GESTURE_SUPPORT}\n";
}

if(defined($ENV{GN_RO_GN_SSGACCESSCALL_SUPPORT})) {
	print "ro.gn.ssgaccesscall.support=$ENV{GN_RO_GN_SSGACCESSCALL_SUPPORT}\n";
}

if(defined($ENV{GN_RO_GN_SSGDIAL_SUPPORT})) {
	print "ro.gn.ssgdial.support=$ENV{GN_RO_GN_SSGDIAL_SUPPORT}\n";
}

if(defined($ENV{GN_RO_GN_NET_AUTOSWITCH_SUPPORT})) {
	print "ro.gn.net.autoswitch.support=$ENV{GN_RO_GN_NET_AUTOSWITCH_SUPPORT}\n";
}

if(defined($ENV{GN_RO_GN_WLAN_WIFIDIRECT_SUPPORT})) {
	print "ro.gn.wlan.wifidirect.support=$ENV{GN_RO_GN_WLAN_WIFIDIRECT_SUPPORT}\n";
}

if(defined($ENV{GN_RO_GN_APPWDIALOG_SUPPORT})) {
	print "ro.gn.appwdialog.support=$ENV{GN_RO_GN_APPWDIALOG_SUPPORT}\n";
}

if(defined($ENV{GN_RO_GN_SMARTSTAY_SUPPORT})) {
	print "ro.gn.smartstay.support=$ENV{GN_RO_GN_SMARTSTAY_SUPPORT}\n";
}

if(defined($ENV{GN_RO_GN_CHAMELEON_DEFCOLOR_ENABLE})) {
	print "ro.gn.chameleon.defcolor.enable=$ENV{GN_RO_GN_CHAMELEON_DEFCOLOR_ENABLE}\n";
}

if(defined($ENV{GN_RO_GN_GESTURE_ONE_KEYEVENT})) {
	print "ro.gn.gesture.one.keyevent=$ENV{GN_RO_GN_GESTURE_ONE_KEYEVENT}\n";
}

if(defined($ENV{GN_RO_GN_SMALLSCREEN_SUPPORT})) {
	print "ro.gn.smallscreen.support=$ENV{GN_RO_GN_SMALLSCREEN_SUPPORT}\n";
}

if(defined($ENV{GN_RO_GN_OOM_OPT})) {
	print "ro.gn.oom.opt=$ENV{GN_RO_GN_OOM_OPT}\n";
}
#Gionee <Amigo_Perf_Opt> <hukaixiang> <20160819> add for CR01749177 begin
if(defined($ENV{GN_RO_MAX_SERVICE_STARTING_BG})) {
	print "ro.config.max_starting_bg=$ENV{GN_RO_MAX_SERVICE_STARTING_BG}\n";
}

if(defined($ENV{GN_RO_NOT_KEEP_RECENT_BITMAPS})) {
	print "ro.gn.not.keep.bitmaps=$ENV{GN_RO_NOT_KEEP_RECENT_BITMAPS}\n";
}

if(defined($ENV{GN_NOT_RESTART_ZOMBIE_SERVICE})) {
	print "ro.gn.not.restart.service=$ENV{GN_NOT_RESTART_ZOMBIE_SERVICE}\n";
}
#Gionee <Amigo_Perf_Opt> <hukaixiang> <20160819> add for CR01749177 end

if(defined($ENV{GN_RO_GN_SKY_LIGHT_SUPPORT})) {
	print "ro.gn.sky.light.support=$ENV{GN_RO_GN_SKY_LIGHT_SUPPORT}\n";
}

# Gionee <Amigo_SkyLight> <guozj> <2015-10-09> add for CR01565152 begin
if(defined($ENV{GN_RO_GN_SKY_LIGHT_OFFSET})) {
	print "ro.gn.sky.light.offset=$ENV{GN_RO_GN_SKY_LIGHT_OFFSET}\n";
}
# Gionee <Amigo_SkyLight> <guozj> <2015-10-09> add for CR01565152 end

if(defined($ENV{GN_RO_GN_NAVI_KEYGUARD_APP_SUPPORT})) {
	print "ro.gn.navi.keyguard.app.support=$ENV{GN_RO_GN_NAVI_KEYGUARD_APP_SUPPORT}\n";
}

if(defined($ENV{GN_RO_GN_MATCH_NUMBERLENGTH})) {
	print "ro.gn.match.numberlength=$ENV{GN_RO_GN_MATCH_NUMBERLENGTH}\n";
}

if(defined($ENV{GN_RW_PERSIST_SYS_PERMISSION_LEVEL})) {
	print "persist.sys.permission.level=$ENV{GN_RW_PERSIST_SYS_PERMISSION_LEVEL}\n";
}

if(defined($ENV{GN_RO_GN_CUTEST})) {
	print "ro.gn.cutest=$ENV{GN_RO_GN_CUTEST}\n";
}

if(defined($ENV{GN_RO_GN_CMCCTEST})) {
	print "ro.gn.cmcctest=$ENV{GN_RO_GN_CMCCTEST}\n";
}

if(defined($ENV{GN_RO_GN_CTTEST})) {
	print "ro.gn.cttest=$ENV{GN_RO_GN_CTTEST}\n";
}

if(defined($ENV{GN_RO_GN_CAMERA_ERASER_SUPPORT})) {
	print "ro.gn.camera.eraser.support=$ENV{GN_RO_GN_CAMERA_ERASER_SUPPORT}\n";
}

if(defined($ENV{GN_RO_GN_CAMERA_BESTFACE_SUPPORT})) {
	print "ro.gn.camera.bestface.support=$ENV{GN_RO_GN_CAMERA_BESTFACE_SUPPORT}\n";
}

if(defined($ENV{GN_RO_GN_CAMERA_GNSPORTS_SUPPORT})) {
	print "ro.gn.camera.gnsports.support=$ENV{GN_RO_GN_CAMERA_GNSPORTS_SUPPORT}\n";
}

if(defined($ENV{GN_RO_GN_CAMERA_PQ_SUPPOR})) {
	print "ro.gn.camera.pq.suppor=$ENV{GN_RO_GN_CAMERA_PQ_SUPPOR}\n";
}

if(defined($ENV{GN_RO_GN_CAMERA_FD_MENU_SUPPORT})) {
	print "ro.gn.camera.fd.menu.support=$ENV{GN_RO_GN_CAMERA_FD_MENU_SUPPORT}\n";
}

if(defined($ENV{GN_RO_GN_CAMERA_PREVIEW_STANDARD})) {
	print "ro.gn.camera.preview.standard=$ENV{GN_RO_GN_CAMERA_PREVIEW_STANDARD}\n";
}

if(defined($ENV{GN_RO_GN_CAMERA_ZSL_DEFAULT_ON})) {
	print "ro.gn.camera.zsl.default.on=$ENV{GN_RO_GN_CAMERA_ZSL_DEFAULT_ON}\n";
}

if(defined($ENV{GN_RO_GN_CAMERA_GOTOCHARM_SUPPORT})) {
	print "ro.gn.camera.gotocharm.support=$ENV{GN_RO_GN_CAMERA_GOTOCHARM_SUPPORT}\n";
}

if(defined($ENV{GN_RO_GN_CAMERA_PICBEST_SUPPORT})) {
	print "ro.gn.camera.picbest.support=$ENV{GN_RO_GN_CAMERA_PICBEST_SUPPORT}\n";
}

if(defined($ENV{GN_RO_GN_CAMERA_PICLEAR_SUPPORT})) {
	print "ro.gn.camera.piclear.support=$ENV{GN_RO_GN_CAMERA_PICLEAR_SUPPORT}\n";
}

if(defined($ENV{GN_RO_GN_CAMERA_ACTIONPATH_SUPPORT})) {
	print "ro.gn.camera.actionpath.support=$ENV{GN_RO_GN_CAMERA_ACTIONPATH_SUPPORT}\n";
}

if(defined($ENV{GN_RO_GN_CAMERA_FD_DEFAULT_ON})) {
	print "ro.gn.camera.fd.default.on=$ENV{GN_RO_GN_CAMERA_FD_DEFAULT_ON}\n";
}

if(defined($ENV{GN_RO_GN_GEMINI_SUPPORT})) {
	print "ro.gn.gemini.support=$ENV{GN_RO_GN_GEMINI_SUPPORT}\n";
}

if(defined($ENV{GN_RO_FLOATINGTOUCH_FUNC})) {
	print "ro.gn.floatingtouch.func=$ENV{GN_RO_FLOATINGTOUCH_FUNC}\n";
}

if(defined($ENV{GN_RW_PERSIST_SYS_TIMEZONE})) {
	print "persist.sys.timezone=$ENV{GN_RW_PERSIST_SYS_TIMEZONE}\n";
}

if(defined($ENV{GN_RO_SYSTEM_BLUR_SUPPORT})) {
	print "ro.gn.system.blur.support=$ENV{GN_RO_SYSTEM_BLUR_SUPPORT}\n";
}

################################################################################
#Gionee property area of robot, don't touch it. end
################################################################################

#Gionee qinkai 2015-4-2 modified for CR01460955 begin
if (exists $ENV{GN_OVERSEA_CUSTOM} && $ENV{GN_OVERSEA_CUSTOM} eq "VISUALFAN") {
    print "persist.sys.timezone=Europe/Athens\n";
#Gionee qinkai 2015-4-2 modified for CR01460955 end
#Gionee wubin 2016-3-2 modified start
}elsif(exists $ENV{GN_OVERSEA_CUSTOM} && $ENV{GN_OVERSEA_CUSTOM} eq "GERMANY_MOBISTEL"){
    print "persist.sys.timezone=Europe/Amsterdam\n";
#Gionee wubin 2016-3-2 modified end

#gionee chenwu 2016-03-16 modify for CR01654080 begin
}elsif (exists $ENV{GN_OVERSEA_CUSTOM} && $ENV{GN_OVERSEA_CUSTOM} eq "INDIA_GIONEE") {
    print "persist.sys.timezone=Asia/Calcutta\n";
#gionee chenwu 2016-03-16 modify for CR01654080 end
}elsif (exists $ENV{GN_OVERSEA_CUSTOM} && $ENV{GN_OVERSEA_CUSTOM} eq "ALGERIA_CONDOR") {
    print "persist.sys.timezone=Africa/Brazzaville\n";
#gionee zhanglz 2016-03-28 modify for CR01662526 begin
}elsif (exists $ENV{GN_OVERSEA_CUSTOM} && $ENV{GN_OVERSEA_CUSTOM} eq "SOUTH_AMERICA_BLU") {
    print "persist.sys.timezone=America/New_York\n";
#gionee zhanglz 2016-03-28 modify for CR01662526 end
#GIONEE: heqianjin 2016-06-21 modify for CR01721202 begin
}elsif (exists $ENV{GN_OVERSEA_CUSTOM} && $ENV{GN_OVERSEA_CUSTOM} eq "KENYA_GIONEE") {
    print "persist.sys.timezone=Africa/Nairobi\n";
}elsif (exists $ENV{GN_OVERSEA_CUSTOM} && $ENV{GN_OVERSEA_CUSTOM} eq "EGYPT_GIONEE") {
    print "persist.sys.timezone=Africa/Cairo\n";
#GIONEE: heqianjin 2015-06-21 modify for CR01721202 end
#Gionee luoguangming 2016.05.10 modify CR01693327 begin
}elsif (exists $ENV{GN_OVERSEA_CUSTOM} && $ENV{GN_OVERSEA_CUSTOM} eq "BANGLADESH_WALTON") {
    print "persist.sys.timezone=Asia/Almaty\n";
#Gionee luoguangming 2016.05.10 modify CR01693327  end 
#Gionee lucy 2016.07.21 modify qm begin
}elsif (exists $ENV{GN_OVERSEA_CUSTOM} && $ENV{GN_OVERSEA_CUSTOM} eq "PAKISTAN_QMOBILE") {
    print "persist.sys.timezone=Asia/Karachi\n";
#Gionee lucy 2016.07.21 modify qm end
#Gionee qianxiaoshan 2016.07.30 modify CR01739862 begin
}elsif (exists $ENV{GN_OVERSEA_CUSTOM} && $ENV{GN_OVERSEA_CUSTOM} eq "MEXICO_COPPEL") {
    print "persist.sys.timezone=America/Mexico_City\n";
#Gionee qianxiaoshan 2016.07.30 modify CR01739862 end
}else{
	print "persist.sys.timezone=Asia/Shanghai\n";
  }
#GIONEE: lujian 2015-05-18 modify for "CR01483225" begin
if(defined($ENV{GN_RW_GN_MMI_GPS_COCLOCK})) {
	print "gn.mmi.gps.coclock=$ENV{GN_RW_GN_MMI_GPS_COCLOCK}\n";
}
#GIONEE: lujian 2015-05-18 modify for "CR01483225"   end
if(defined($ENV{GN_RW_GN_MMI_BACKUP_TO_PRODUCTINFO})) {
	print "gn.mmi.backup.to.pruductinfo=$ENV{GN_RW_GN_MMI_BACKUP_TO_PRODUCTINFO}\n";
}
#GIONEE: lujian 2015-01-27 modify for "CR01435990" begin
if (exists $ENV{GN_MMI_NEED_CALI} && $ENV{GN_MMI_NEED_CALI} ne "")
{
    print "ro.gn.need.cal=$ENV{GN_MMI_NEED_CALI}\n";
}
#GIONEE: lujian 2015-01-27 modify for "CR01435990"   end

#GIONEE: lujian 2015-01-27 modify for "CR01435990" begin
if (exists $ENV{GN_MMI_ORIGINAL_FM} && $ENV{GN_MMI_ORIGINAL_FM} ne "")
{
    print "gn.mmi.original.fm=$ENV{GN_MMI_ORIGINAL_FM}\n";
}
#GIONEE: lujian 2015-01-27 modify for "CR01435990"   end

#Gionee fengxb 2011-10-5 add for CR00366523 start
if(defined($ENV{GN_MATCH_NUMBERLENGTH})) {
	print "ro.gn.match.numberlength=$ENV{GN_MATCH_NUMBERLENGTH}\n";
}
#Gionee fengxb 2011-10-5 add for CR00366523 end

#Gionee taofapan 2016-3-15 add for CR01652086 begin
if (exists $ENV{GN_AMGIO_VIBRATOR})
{
    print "ro.amigo.vibrator=$ENV{GN_AMGIO_VIBRATOR}\n";
}
#Gionee taofapan 2016-3-15 add for CR01652086 end
#Gionee wubin 2016-04-13 add for CR01674972 begin
if (exists $ENV{GN_DEF_WRITE_DISK_OPTION_SUPPORT} && $ENV{GN_DEF_WRITE_DISK_OPTION_SUPPORT} eq "yes")
{
    print "ro.gn.writediskoption.support=$ENV{GN_DEF_WRITE_DISK_OPTION_SUPPORT}\n";
}
#Gionee wubin 2016-04-13 add for CR01674972 end

#Gionee wubin 2016-04-01 add for CR01646947 begin
if (exists $ENV{GN_GPS_CLOSE_SUPPORT} && $ENV{GN_GPS_CLOSE_SUPPORT} eq "yes")
{
    print "ro.gn.gps.close.support=$ENV{GN_GPS_CLOSE_SUPPORT}\n";
}
#Gionee wubin 2016-04-01 add for CR01646947 end
if(defined($ENV{GN_RO_GN_BUTTONLIGHT_SUPPORT})) {
	print "ro.gn.buttonlight.support=$ENV{GN_RO_GN_BUTTONLIGHT_SUPPORT}\n";
}
#Gionee fengyao 20160307 add for CR01643740 being
if(defined($ENV{GN_RW_GN_MMI_LTETDDANT})) {
	print "gn.mmi.ltetddant=$ENV{GN_RW_GN_MMI_LTETDDANT}\n";
}
#Gionee fengyao 20160307 add for CR01643740 end
#GIONEE: luohui 2015-1-20 modify for Oversea start->
if (exists $ENV{GN_OVERSEA_PRODUCT} && $ENV{GN_OVERSEA_PRODUCT} eq "yes") {
    if(exists $ENV{GIONEEPROJECTID}) {
	    print "ro.gn.gnproductid=$ENV{GIONEEPROJECTID}\n";
    }

    if(exists $ENV{GIONEEPRODUCTID}) {
	    print "ro.gn.gnprojectid=$ENV{GIONEEPRODUCTID}\n";
    }
}else{
    if(defined($ENV{GN_RO_GN_GNPRODUCTID})) {
	    print "ro.gn.gnproductid=$ENV{GN_RO_GN_GNPRODUCTID}\n";
    }

    if(defined($ENV{GN_RO_GN_GNPROJECTID})) {
	    print "ro.gn.gnprojectid=$ENV{GN_RO_GN_GNPROJECTID}\n";
    }
}
#GIONEE: luohui 2015-1-20 modify for Oversea end<-

#GIONEE:guoxt modify for CR01442146 2015-01-26 begin
if (exists $ENV{GN_OVERSEA_PRODUCT} && $ENV{GN_OVERSEA_PRODUCT} eq "yes")
{
	if (exists $ENV{GN_THEME_DEFAULT_PACKAGENAME} && $ENV{GN_THEME_DEFAULT_PACKAGENAME} ne "")
	{
	    print "ro.gn.theme.default=$ENV{GN_THEME_DEFAULT_PACKAGENAME}\n";
	}
}
#GIONEE:guoxt modify for CR01442146 2015-01-26 end

#Gionee guoxt 20150120 modify for gionee brand OTA begin
if ($GN_GNOTAUPDATE_SUPPORT eq "yes") 
{ 
    print "#ro.gn.gnotaupdate.support=$ENV{GN_GNOTAUPDATE_SUPPORT}\n";
}else{
    print "#ro.gn.gnotaupdate.support=no\n";
}

if($ENV{GN_APK_AMIGO_SETTINGUPDATE_SUPPORT} eq "yes" && defined(GN_OVERSEA_OTA_SERVER)){
    print "ro.gn.oversea.ota.server=$ENV{GN_OVERSEA_OTA_SERVER}\n";
}
#Gionee guoxt 20150120 modify for gionee brand OTA end

#GIONEE:guoxt modify for CR01437293 2015-01-19 begin
if (exists $ENV{GN_OVERSEA_PRODUCT} && $ENV{GN_OVERSEA_PRODUCT} eq "yes") {
    if(defined($ENV{GN_INTERNAL_VER_NUMBER})) {
        print "ro.gn.gnvernumber=$ENV{GN_INTERNAL_VER_NUMBER}\n";
    }  
}else{
    if(defined($ENV{GN_RO_GN_GNVERNUMBER})) {
        print "ro.gn.gnvernumber=$ENV{GN_RO_GN_GNVERNUMBER}\n";
    }
}

#Gionee 20150109 chenrui add for oversea start
if (exists $ENV{GN_OVERSEA_PRODUCT} && $ENV{GN_OVERSEA_PRODUCT} eq "yes") {
    print "ro.gn.oversea.product=$ENV{GN_OVERSEA_PRODUCT}\n";
}

if (exists $ENV{GN_OVERSEA_CUSTOM} && $ENV{GN_OVERSEA_CUSTOM} ne "") {
    print "ro.gn.oversea.custom=$ENV{GN_OVERSEA_CUSTOM}\n";
}
#Gionee 20150109 chenrui add for oversea end

#Gionee 20160105 chenrui add for CR01617980 start
if (defined($ENV{GN_HAOKAN_ONLINE_SUPPORT})) {
	print "ro.gn.haokan.online.support=$ENV{GN_HAOKAN_ONLINE_SUPPORT}\n";
}
#Gionee 20160105 chenrui add for CR01617980 end

#Gionee 20160106 chenrui add for CR01619167 start
if (exists $ENV{GN_NAVI_OWNER_INFO_SUPPORT} && $ENV{GN_NAVI_OWNER_INFO_SUPPORT} eq "yes")
{
    print "ro.gn.navi.owner.info.support=$ENV{GN_NAVI_OWNER_INFO_SUPPORT}\n";
}
#Gionee 20160106 chenrui add for CR01619167 end

#Gionee baiyh 2015-06-05 add for CR01497263 begin
if(defined($ENV{GN_RW_GN_ATUOMMI_KEYTEST_MENU})) {
	print "gn.autommi.keytest.menu=$ENV{GN_RW_GN_ATUOMMI_KEYTEST_MENU}\n";
}
if(defined($ENV{GN_RW_GN_ATUOMMI_KEYTEST_CAMERA})) {
	print "gn.autommi.keytest.camerea=$ENV{GN_RW_GN_ATUOMMI_KEYTEST_CAMERA}\n";
}
if(defined($ENV{GN_RW_GN_ATUOMMI_KEYTEST_FOCUS})) {
	print "gn.autommi.keytest.focus=$ENV{GN_RW_GN_ATUOMMI_KEYTEST_FOCUS}\n";
}
#Gionee baiyh 2015-06-05 add for CR01497263 end
#gionee  zhangpj 20150109 add for CR01429061 begin
if (exists $ENV{GN_CAMERA_ZSL_DEFAULT_ON} && $ENV{GN_CAMERA_ZSL_DEFAULT_ON} eq "yes")
{
    print "ro.gn.camera.zsl.default.on=$ENV{GN_CAMERA_ZSL_DEFAULT_ON}\n";
} else {
    print "ro.gn.camera.zsl.default.on=no\n";
}
#Gionee pangfei 20160331 add for close back camera zsl start
if (exists $ENV{GN_CAMERA_BACK_CAMERA_ZSL_DEFAULT_ON} && $ENV{GN_CAMERA_BACK_CAMERA_ZSL_DEFAULT_ON} eq "no")
{
    print "ro.gn.back.camera.zsl.on=$ENV{GN_CAMERA_BACK_CAMERA_ZSL_DEFAULT_ON}\n";
} else {
    print "ro.gn.back.camera.zsl.on=yes\n";
}
#Gionee pangfei 20160331 add for close back camera zsl end 

if (exists $ENV{GN_CAMERA_GOTOCHARM_SUPPORT} && $ENV{GN_CAMERA_GOTOCHARM_SUPPORT} eq "yes")
{
    print "ro.gn.camera.gotocharm.support=$ENV{GN_CAMERA_GOTOCHARM_SUPPORT}\n";
} else {
    print "ro.gn.camera.gotocharm.support=no\n";
}

if (exists $ENV{GN_CAMERA_PICBEST_SUPPORT} && $ENV{GN_CAMERA_PICBEST_SUPPORT} eq "yes")
{
    print "ro.gn.camera.picbest.support=$ENV{GN_CAMERA_PICBEST_SUPPORT}\n";
} else {
    print "ro.gn.camera.picbest.support=no\n";
}

if (exists $ENV{GN_CAMERA_PICLEAR_SUPPORT} && $ENV{GN_CAMERA_PICLEAR_SUPPORT} eq "yes")
{
    print "ro.gn.camera.piclear.support=$ENV{GN_CAMERA_PICLEAR_SUPPORT}\n";
} else {
    print "ro.gn.camera.piclear.support=no\n";
}

if (exists $ENV{GN_CAMERA_ACTIONPATH_SUPPORT} && $ENV{GN_CAMERA_ACTIONPATH_SUPPORT} eq "yes")
{
    print "ro.gn.camera.actionpath.support=$ENV{GN_CAMERA_ACTIONPATH_SUPPORT}\n";
} else {
    print "ro.gn.camera.actionpath.support=no\n";
}

if (exists $ENV{GN_CAMERA_FD_DEFAULT_ON} && $ENV{GN_CAMERA_FD_DEFAULT_ON} eq "yes")
{
    print "ro.gn.camera.fd.default.on=$ENV{GN_CAMERA_FD_DEFAULT_ON}\n";
} else {
    print "ro.gn.camera.fd.default.on=no\n";
}

if (exists $ENV{GN_CAMERA_FULLSCREEN_DEFAULT_ON} && $ENV{GN_CAMERA_FULLSCREEN_DEFAULT_ON} eq "yes")
{
    print "ro.gn.camera.default.fullscreen=$ENV{GN_CAMERA_FULLSCREEN_DEFAULT_ON}\n";
} else {
    print "ro.gn.camera.default.fullscreen=no\n";
}

if (exists $ENV{GN_PLUGIN_DOWNLOAD_SUPPORT} && $ENV{GN_PLUGIN_DOWNLOAD_SUPPORT} eq "yes")
{
    print "ro.gn.camera.plugin.download=$ENV{GN_PLUGIN_DOWNLOAD_SUPPORT}\n";
} else {
    print "ro.gn.camera.plugin.download=no\n";
}

if (exists $ENV{GN_PLUGIN_MOVE_SUPPORT} && $ENV{GN_PLUGIN_MOVE_SUPPORT} eq "yes")
{
    print "ro.gn.camera.plugin.move=$ENV{GN_PLUGIN_MOVE_SUPPORT}\n";
} else {
    print "ro.gn.camera.plugin.move=no\n";
}
#gionee  zhangpj 20150109 add for CR01429061 end
#Gionee baiyh 2015-02-26 add for CR01445228 begin
if (exists $ENV{GN_SUPPORT_PERMANENTMEMUKEY} && $ENV{GN_SUPPORT_PERMANENTMEMUKEY} eq "yes")
{
    print "ro.gn.permanentmenukey=$ENV{GN_SUPPORT_PERMANENTMEMUKEY}\n";
}
#Gionee baiyh 2015-02-26 add for CR01445228 end

#Gionee baiyh 2015-02-26 add for CR01445228 begin
if(defined($ENV{GN3RD_GMS_SUPPORT})) {
    print "ro.gn.gms.support=$ENV{GN3RD_GMS_SUPPORT}\n";
}else{
    print "ro.gn.gms.support=no\n";
}
#Gionee baiyh 2015-02-26 add for CR01445228 end

if (exists $ENV{GN_DEFAULT_INPUT_METHOD})
{
    print "ro.gn.default.inputmethod=$ENV{GN_DEFAULT_INPUT_METHOD}\n";
}

if(defined($ENV{GN_BRUSH_SWITCH})) {
	print "persist.sys.brush.switch=$ENV{GN_BRUSH_SWITCH}\n";
}

#Gionee <bug> <lichao> <2014-12-30> add for CR01432033 begin
#Gionee <bug> <lichao> <2015-02-13> modify
#Gionee xuwen 2013-07-09 add for CR00834689
if(defined($ENV{GN_AP_PW_DIALOG_SUPPORT})) {
	print "ro.gn.appwdialog.support=$ENV{GN_AP_PW_DIALOG_SUPPORT}\n";
}
if(defined($ENV{GN_NET_AUTOSWITCH_SUPPORT})) {
	print "ro.gn.net.autoswitch.support=$ENV{GN_NET_AUTOSWITCH_SUPPORT}\n";
}
#Gionee <bug> <lichao> <2014-12-30> add for CR01432033 end

#Gionee <Amigo_WlanShowDialog> <huangbx> <20150803> add for CR01531622 begin
if(defined($ENV{GN_WLAN_SHOWDIALOG_SUPPORT})) {
    print "ro.gn.wlanshowdialog.support=$ENV{GN_WLAN_SHOWDIALOG_SUPPORT}\n";
}
#Gionee <Amigo_WlanShowDialog> <huangbx> <20150803> add for CR01531622 end

#Gionee BSP1 yaoyc 20140927 add for incall p-sensor calibrate CR01391528 begin
if (defined($ENV{CONFIG_GN_BSP_PSENSOR_INCALL_CALIBRATION}))
{
	print "ro.gn.pscali.incall.support=$ENV{CONFIG_GN_BSP_PSENSOR_INCALL_CALIBRATION}\n";
}
#Gionee BSP1 yaoyc 20140927 add for incall p-sensor calibrate CR01391528 end

#Gionee 20160105 add by yangshi for CR01500526 flip not screen on begin
if(defined($ENV{CONFIG_GN_BSP_FLIP_SUPPORT})) {
    print "ro.gn.flipnotscreenon.support=$ENV{CONFIG_GN_BSP_FLIP_SUPPORT}\n";
}
#Gionee 20160105 add by yangshi for CR01500526 flip not screen on end

#Gionee 20160229 add by yangshi for CR01642331 begin
if (exists $ENV{CONFIG_GN_BSP_GPULOWPOWER_SUPPORT} && $ENV{CONFIG_GN_BSP_GPULOWPOWER_SUPPORT} eq "yes")
{
    print "ro.gn.fps.high=60\n";
    print "ro.gn.fps.low=30\n";
}
#Gionee 20160229 add by yangshi for CR01642331  end

#Gionee <Amigo_Skylight> wangym <20141208> add for  CR01422131 begin
if (defined($ENV{GN_APPSKYLIGHT_SUPPORT}))
{
	print "ro.gn.sky.light.support=$ENV{GN_APPSKYLIGHT_SUPPORT}\n";
}
#Gionee <Amigo_Skylight> wangym <20141208> add for  CR01422131 end
#Gionee <dual_weixin> liuran <20150128> add for  CR01631240 begin
if (defined($ENV{GN_RO_AMIGO_CLONE_SUPPORT}))
{
	print "ro.gn.amigo.clone.support=$ENV{GN_RO_AMIGO_CLONE_SUPPORT}\n";
}
#Gionee <dual_weixin> liuran <20150128> add for  CR01631240 end

#Gionee <Amigo_Oom_Opt> liuran <20150128> add for  CR01443018 begin
if (defined($ENV{GN_OOM_OPT_SUPPORT}))
{
	print "ro.gn.oom.opt=$ENV{GN_OOM_OPT_SUPPORT}\n";
}
#Gionee <Amigo_Oom_Opt> liuran <20150128> add for  CR01443018 end

#Gionee <Amigo_Fake_Kill> hukaixiang <20160412> add for  CR01674614 begin
if (defined($ENV{GN_FAKE_KILL_SUPPORT}))
{
	print "ro.gn.fake.kill=$ENV{GN_FAKE_KILL_SUPPORT}\n";
}
#Gionee <Amigo_Fake_Kill> hukaixiang <20160412> add for  CR01674614 end

#GIONEE:guoxt modify for CR01477578 2015-05-11 begin
if (exists $ENV{GN_OVERSEA_PRODUCT} && $ENV{GN_OVERSEA_PRODUCT} eq "yes")
{
  print "persist.sys.permission.level=1\n";
}
#GIONEE:guoxt modify for CR01477578 2015-05-11 end

if (exists $ENV{GN_EMULATED_STORAGE} && $ENV{GN_EMULATED_STORAGE} eq "yes")
{
    print "ro.gn.emulated.storage=$ENV{GN_EMULATED_STORAGE}\n";
}

#Gionee futao 2013-03-27 add for CR00790348 begin
if (exists $ENV{GN_OVERSEA_MUSIC_OFFLINE_SUPPORT} && $ENV{GN_OVERSEA_MUSIC_OFFLINE_SUPPORT} eq "yes")
{

    print "ro.gn.oversea.music.offline.ver=$ENV{GN_OVERSEA_MUSIC_OFFLINE_SUPPORT}\n";
}
#Gionee futao 2013-03-27 add for CR00790348 end

#Gionee:wangfei 2015-01-07 add for begin
if(defined($ENV{GN_ONLY_DISP_ALARM_GESTURE})) {
	print "ro.gn.only.disp.alarm.gesture=$ENV{GN_ONLY_DISP_ALARM_GESTURE}\n";
}else{
	print "ro.gn.only.disp.alarm.gesture=no\n";
}

if(defined($ENV{GN_HOTKNOT_SUPPORT})) {
	print "ro.gn.hotknot.support=$ENV{GN_HOTKNOT_SUPPORT}\n";
#}else{
#	print "ro.gn.hotknot.support=no\n";
}

#Gionee:wangfei 2015-01-07 add for end

#Gionee:guoxt 20150109 added for CR01435173 begin
if(exists $ENV{GN_RO_PRODUCT_NAME}){
    print "ro.product.board=$ENV{GN_RO_PRODUCT_NAME}\n";
}
#Gionee:guoxt 20150109 added for CR01435173 end

#Gionee guoxt 2015-01-12 add for CR01435173 begin
if (exists $ENV{GN_BUILD_PRODUCT} && $ENV{GN_BUILD_PRODUCT} ne "")
{
    print "ro.build.product=$ENV{GN_BUILD_PRODUCT}\n";
}

if (exists $ENV{GN_GUESTMODE_SUPPORT} && $ENV{GN_GUESTMODE_SUPPORT} ne "")
{
    print "ro.gn.guestmode.support=$ENV{GN_GUESTMODE_SUPPORT}\n";
}
#Gionee guoxt 2015-01-12 add for CR01435173 end
#Gionee yanxh 2015-1-14,add for CR01436504 begin.
if (exists $ENV{GN_VIDEO_REPEATPLAY_SUPPORT} && $ENV{GN_VIDEO_REPEATPLAY_SUPPORT} eq "yes")
{
    print "#ro.gn.video.repeatplay.support=$ENV{GN_VIDEO_REPEATPLAY_SUPPORT}\n";
} else {
    print "#ro.gn.video.repeatplay.support=no\n";
}
#Gionee yanxh 2015-1-14,add for CR01436504 end
#Gionee wenjun 2015-1-14 add for CR01437456 begin.
if(defined($ENV{GN_OVERSEA_IFONT_SUPPORT})) {
	print "ro.gn.oversea.ifont_support=$ENV{GN_OVERSEA_IFONT_SUPPORT}\n";
}
#Gionee wenjun 2015-1-14 add for CR01437456 begin.
#Gionee guoxt 20150120 modify for gionee brand OTA begin
if ($GN_GNOTAUPDATE_SUPPORT eq "yes") 
{ 
    print "ro.gn.gnotaupdate.support=$ENV{GN_GNOTAUPDATE_SUPPORT}\n";
#}else{
#    print "ro.gn.gnotaupdate.support=no\n";
}

if($ENV{GN_APK_AMIGO_SETTINGUPDATE_SUPPORT} eq "yes" && defined($ENV{GN_OVERSEA_OTA_SERVER})){
    print "ro.gn.oversea.ota.server=$ENV{GN_OVERSEA_OTA_SERVER}\n";
}
#Gionee guoxt 20150120 modify for gionee brand OTA end

#GIONEE:guoxt modify for CR01442146 2015-01-26 begin
if (exists $ENV{GN_OVERSEA_PRODUCT} && $ENV{GN_OVERSEA_PRODUCT} eq "yes")
{
	if (exists $ENV{GN_THEME_DEFAULT_PACKAGENAME} && $ENV{GN_THEME_DEFAULT_PACKAGENAME} ne "")
	{
	    print "ro.gn.theme.default=$ENV{GN_THEME_DEFAULT_PACKAGENAME}\n";
	}
}
#GIONEE:guoxt modify for CR01442146 2015-01-26 end

#GIONEE: wangfei 2015-01-27 add for CR01442372 begin
if(defined($ENV{GN_DISTANCEGESTURE_SUPPORT})) {
	print "ro.gn.distancegesture.support=$ENV{GN_DISTANCEGESTURE_SUPPORT}\n";
}

if(defined($ENV{GN_SMART_VIBRATE_ALERT})) {
	print "ro.gn.smart.vibrate.alert=$ENV{GN_SMART_VIBRATE_ALERT}\n";
}
#GIONEE: wangfei 2015-01-27 add for CR01442372 end

#Gionee guoxt 20150520 modify for CR01485609 start
if (exists $ENV{GN_RO_REMOVE_AMIGO_SECURITY_PWD} && $ENV{GN_RO_REMOVE_AMIGO_SECURITY_PWD} eq "yes") 
{
    print "ro.gn.remove.security.pwd=$ENV{GN_RO_REMOVE_AMIGO_SECURITY_PWD}\n";
}
# Gionee guoxt 20150520 modify for CR01485609 end
#Gionee guoxt 20150525 modify for CR01488940 start
if(exists $ENV{GN_RO_IPOSETTING_SUPPORT} && $ENV{GN_RO_IPOSETTING_SUPPORT} eq "yes")
{
    print "ro.gn.iposetting.support=$GN_IPOSETTING_SUPPORT\n";
#}else{
#    print "ro.gn.iposetting.support=no\n";
}
#Gionee guoxt 20150525 modify for CR01488940 end
#GIONEE:guoxt modify for CR01443891 2015-01-30 begin
    if (exists $ENV{GN_AMIGO_SWITCH_FONTS_SUPPORT} && $ENV{GN_AMIGO_SWITCH_FONTS_SUPPORT} eq "yes")
    {
        print "ro.gn.switch.font=$ENV{GN_AMIGO_SWITCH_FONTS_SUPPORT}\n";
    }
#GIONEE:guoxt modify for CR01443891 2015-01-30 end

#Gionee <fingerprint> <qudw> <20150121> modify for CR01440177 begin
if(defined($ENV{GN_FINGERPRINT_SUPPORT})) {
    print "ro.gn.fingerprint.support=$ENV{GN_FINGERPRINT_SUPPORT}\n";
    print "sys.fingerprint.sensor.type=0\n";
} else {
    print "ro.gn.fingerprint.support=no\n";
}

if(defined($ENV{GN_FINGERTEST_EXT})){
    print "ro.gn.fingerprint.ext=$ENV{GN_FINGERTEST_EXT}\n";
}else{
	print "ro.gn.fingerprint.ext=no\n"	
}


if (exists $ENV{GN_FP_TEE_SUPPORT} && $ENV{GN_FP_TEE_SUPPORT} eq "yes")
{
    print "ro.gn.fp.tee.support=$ENV{GN_FP_TEE_SUPPORT}\n";
} else {
    print "ro.gn.fp.tee.support=no\n";
}

if (exists $ENV{CONFIG_GN_BSP_FINGER_COMPATIBLE_SW} && $ENV{CONFIG_GN_BSP_FINGER_COMPATIBLE_SW} eq "yes")
{
    print "ro.gn.fp.cmpt.support=sw\n";
}

if (exists $ENV{CONFIG_GN_BSP_FINGER_COMPATIBLE_HW} && $ENV{CONFIG_GN_BSP_FINGER_COMPATIBLE_HW} eq "yes")
{
    print "ro.gn.fp.cmpt.support=hw\n";
}

#Gionee <fingerprint> <qudw> <20150121> modify for CR01440177 end

#Gionee <NFC> <qudw> <20150202> modify for nfc begin
if (exists $ENV{GN_NFC_NXP_SUPPORT} && $ENV{GN_NFC_NXP_SUPPORT} eq "yes")
{
    print "ro.gn.nfc.nxp.support=$ENV{GN_NFC_NXP_SUPPORT}\n";
#} else {
#    print "ro.gn.nfc.nxp.support=no\n";
}
#Gionee <NFC> <qudw> <20150202> modify for nfc end

#Gionee <Finger_Unlock> <longzp> <20150331> add for CR01457465 begin
if(defined($ENV{GN_FINGERPRINT_UNLOCK_SUPPORT})) {
	print "ro.gn.finger.unlock.support=$ENV{GN_FINGERPRINT_UNLOCK_SUPPORT}\n";
}
#Gionee <Finger_Unlock> <longzp> <20150331> add for CR01457465 end

if(defined($ENV{CONFIG_GN_BSP_MTK_AK4375_SUPPORT})) {
	print "ro.gn.bsp.mtk.ak4375.support=$ENV{CONFIG_GN_BSP_MTK_AK4375_SUPPORT}\n";
}

#Gionee <gongsc> <20160525> add for gnspr13178 start
if(defined($ENV{GN_RO_COM_GOOGLE_CLIENTIDBASE})) {
	print "ro.com.google.clientidbase=$ENV{GN_RO_COM_GOOGLE_CLIENTIDBASE}\n";
}
#Gionee <gongsc> <20160525> add for gnspr13178 end

#Gionee <others> <wangmeng> <20150311> add for android navigationbar controller.begin
if (exists $ENV{GN_CONFIG_SHOW_NAVIGATIONBAR_SUPPORT}) 
{
	if ($ENV{GN_CONFIG_SHOW_NAVIGATIONBAR_SUPPORT} eq "no"){
        print "qemu.hw.mainkeys=1\n";
	}else{
        print "qemu.hw.mainkeys=0\n";
	}
}else{
    print "qemu.hw.mainkeys=1\n";
}
#Gionee <others> <wangmeng> <20150311> add for android navigationbar controller.end

#Gionee <bug> <lichao> <2015-03-13> add for CR01453981 begin
if(defined($ENV{GN_WLAN_WIFIDIRECT_SUPPORT})) {
	print "ro.gn.wlan.wifidirect.support=$ENV{GN_WLAN_WIFIDIRECT_SUPPORT}\n";
}
#Gionee <bug> <lichao> <2015-03-13> add for CR01453981 end

#Gionee <gn_wakeup> <longzp> <20150323> add for difference gesture chip begin
if(defined($ENV{GN_GESTURE_VENDOR})) {
	print "ro.gn.gesture.vendor=$ENV{GN_GESTURE_VENDOR}\n";
}
#Gionee <gn_wakeup> <longzp> <20150323> add for difference gesture chip end

#Gionee rench 2015-04-30 add for CR01468748 begin
if (exists $ENV{GN_CAMERA_STORAGE_LOCATION_SUPPORT} && $ENV{GN_CAMERA_STORAGE_LOCATION_SUPPORT} eq "yes")
{
    print "ro.gn.camera.storagelocation=$ENV{GN_CAMERA_STORAGE_LOCATION_SUPPORT}\n";
}
#Gionee rench 2015-04-30 add for CR01468748 end

#Gionee hongsq 2015-05-06 add for CR01475347 begin
if (exists $ENV{GN_OVERSEA_MUSIC_OFFLINE_SUPPORT} && $ENV{GN_OVERSEA_MUSIC_OFFLINE_SUPPORT} eq "yes")
{
    print "ro.gn.oversea.music.offline.ver=$ENV{GN_OVERSEA_MUSIC_OFFLINE_SUPPORT}\n";
}
if (exists $ENV{GN_OVERSEA_MUSIC_HEADSET_THREECLICK_SUPPORT} && $ENV{GN_OVERSEA_MUSIC_HEADSET_THREECLICK_SUPPORT} eq "yes")
{
    print "ro.gn.headset.threeclick=$ENV{GN_OVERSEA_MUSIC_HEADSET_THREECLICK_SUPPORT}\n";
}
#Gionee hongsq 2015-05-06 add for CR01475347 end

if(defined($ENV{GN_NFC_WIFI_DIRECT_SUPPORT})) {
	print "ro.gn.nfc.wifi_direct.support=$ENV{GN_NFC_WIFI_DIRECT_SUPPORT}\n";
}
#Gionee zhoujt 2015-04-25 add for GNNCR00010342 begin
if(defined($ENV{GIONEE_VERSION})) {
    print "ro.build.version.gioneeversion=$ENV{GIONEE_VERSION}\n";
}
#Gionee zhoujt 2015-04-25 add for GNNCR00010342 end
if(defined($ENV{GN_NEW_SCREEN_TIMEOUT})) {
	print "ro.gn.new.screen.timeout=$ENV{GN_NEW_SCREEN_TIMEOUT}\n";
}

#Gionee renfei 20150902 add for CR01546056 begin
if (exists $ENV{GN_ANTITHEFTTRACK_GI_SUPPORT} && $ENV{GN_ANTITHEFTTRACK_GI_SUPPORT} eq "yes")
{
    print "ro.gn.antithefttrack.support=$ENV{GN_ANTITHEFTTRACK_GI_SUPPORT}\n";
    print "persist.sys.att.spw.prop=0\n";
    print "persist.sys.att.fn.prop=0\n";
}
#Gionee renfei 20150902 add for CR01546056 end
#Gionee renfei 20150923 add for CR01558646 begin
if (exists $ENV{GN_ANTITHEFTTRACK_SUPPORT} && $ENV{GN_ANTITHEFTTRACK_SUPPORT} eq "yes")
{
    print "ro.gn.antistolen.tl.support=$ENV{GN_ANTITHEFTTRACK_SUPPORT}\n";
    print "ro.gn.antistolen.controlphone=$ENV{GN_ANTITHEFTTRACK_CONTROLPHONE_NUMBER}\n";
}
#Gionee renfei 20150923 add for CR01558646 end
#Gionee cuijiuyu 20121122 add for salesstatistic begin
if(defined($ENV{GN_OVERSEA_SALES_STATISTIC_SUPPORT})) {
	print "ro.gn.salesstatistic.support=$ENV{GN_OVERSEA_SALES_STATISTIC_SUPPORT}\n";
}
if(exists $ENV{GN_OVERSEA_SALES_STATISTIC_URL} && $ENV{GN_OVERSEA_SALES_STATISTIC_URL} ne "")
{
    print "ro.gn.sales.salesstatistic.url=$ENV{GN_OVERSEA_SALES_STATISTIC_URL}\n";
}
#Gionee cuijiuyu 20121122 add for salesstatistic end
#Gionee cuijiuyu 20160422 add for salesstatistic begin
if(defined($ENV{GN_OVERSEA_DATA_MANAGER_SUPPORT})) {
	print "ro.gn.datamanager.support=$ENV{GN_OVERSEA_DATA_MANAGER_SUPPORT}\n";
}
#Gionee cuijiuyu 20160422 add for salesstatistic end

#Gionee guoxt 2013-12-09 add for CR00971946 start
#only for PowerSaver Control
if (exists $ENV{GN_OVERSEA_COMMON_MAINBOARD}){
    print "ro.gn.common.mainboard.prop=$ENV{GN_OVERSEA_COMMON_MAINBOARD}\n";
}
#Gionee guoxt 2013-12-09 add for CR00971946 end

#Gionee rench 2015-02-09 add for CR01447376 begin
if (defined($ENV{GN_OVERSEA_MWC_SUPPORT}) && $ENV{GN_OVERSEA_MWC_SUPPORT} eq "yes")
{
    print "ro.gn.oversea.mwc.support=$ENV{GN_OVERSEA_MWC_SUPPORT}\n";
#}else{
#    print "ro.gn.oversea.mwc.support=no\n";
}
#Gionee rench 2015-02-09 add for CR01447376 end

#Gionee liangjb 20150209 add for CR01447508 start
if (exists $ENV{GN_VOICEMAIL_SUPPORT} && $ENV{GN_VOICEMAIL_SUPPORT} eq "yes") 
{
    print "ro.gn.voicemail.support=$ENV{GN_VOICEMAIL_SUPPORT}\n";
}
#Gionee liangjb 20150209 add for CR01447508 end

#GIONEE: wangfei 2015-02-10 add for CR01447387 begin
if(defined($ENV{GN_WIFIDIRECT_MENU_SUPPORT})) {
	print "ro.gn.wifidirect.menu.support=$ENV{GN_WIFIDIRECT_MENU_SUPPORT}\n";
}
#GIONEE: wangfei 2015-02-10 add for CR01441873 end

#GIONEE: chenbo 2015-02-11 add for CR01447301 begin
if(defined($ENV{GN_APK_AMIGO_SYSTEMMANAGER_SUPER_SUPPORT})) {
	print "ro.gn.sysmgr.super.support=$ENV{GN_APK_AMIGO_SYSTEMMANAGER_SUPER_SUPPORT}\n";
}
#GIONEE: chenbo 2015-02-11 add for CR01447301 end

#GIONEE: yuchao 2015-02-15 add for CR01449041 begin
if (exists $ENV{GN_LONG_VOL_KEY_EXPAND} && $ENV{GN_LONG_VOL_KEY_EXPAND} eq "yes") 
{
    print "#ro.gn.long.vol_key_expand=$ENV{GN_LONG_VOL_KEY_EXPAND}\n";
}
#GIONEE: yuchao 2015-02-15 add for CR01449041 end

#gionee yewq 2012-12-17 modify for CR00747189 begin
if (exists $ENV{GN_EMAIL_GENERALSIGNATURE} && $ENV{GN_EMAIL_GENERALSIGNATURE} eq "yes") 
{
    print "ro.gn.email.generalsignature=$ENV{GN_EMAIL_GENERALSIGNATURE}\n";
}
#gionee yewq 2012-12-17 modify for CR00747189 end

#gionee yangcuntao 2015-03-04 add for CR01449745 begin
if (exists $ENV{MOBILE_DATA_OFF_DEFAULT}) 
{
    print "ro.default_data_off=$ENV{MOBILE_DATA_OFF_DEFAULT}\n";
}
#gionee yangcuntao 2015-03-04 add for CR01449745 end

#Gionee yubo 2015-03-05 add for CR01450685 begin
if(defined($ENV{GN_SHUTDOWN_SUPPORT})) {
	print "ro.gn.shutdown.support=$ENV{GN_SHUTDOWN_SUPPORT}\n";
}
#Gionee yubo 2015-03-05 add for CR01450685 end

#Gionee rench 2015-04-23 add for CR01470087 begin
if (exists $ENV{GN_ALARM_INCOMING_CALL} && $ENV{GN_ALARM_INCOMING_CALL} eq "yes")
{
    print "ro.gn.alarm.incoming.call=$ENV{GN_ALARM_INCOMING_CALL}\n";
}
#Gionee rench 2015-04-23 add for CR01470087 end

#Gionee rench 2015-04-30 add for CR01468748 begin
if (exists $ENV{GN_CAMERA_STORAGE_LOCATION_SUPPORT} && $ENV{GN_CAMERA_STORAGE_LOCATION_SUPPORT} eq "yes")
{
    print "ro.gn.camera.storagelocation=$ENV{GN_CAMERA_STORAGE_LOCATION_SUPPORT}\n";
}
#Gionee rench 2015-04-30 add for CR01468748 end

#gionee yewq 2016-2-18 add for CR01635478 begin
if (exists $ENV{GN_SWITCH_STORAGE_SUPPORT} && $ENV{GN_SWITCH_STORAGE_SUPPORT} eq "yes")
{
    print "ro.gn.switch.storage.support=$ENV{GN_SWITCH_STORAGE_SUPPORT}\n";
}
#gionee yewq 2016-2-18 add for CR01635478 end

#gionee yewq 2016-02-18 add for CR01635533 begin
if (exists $ENV{GN_VIDEO_SNAPSHOT_SUPPORT} && $ENV{GN_VIDEO_SNAPSHOT_SUPPORT} eq "yes")
{
    print "ro.gn.video.snapshot.support=$ENV{GN_VIDEO_SNAPSHOT_SUPPORT}\n";
}
#gionee yewq 2016-02-18 add for CR01635533 end

#Gionee wangfei add for CR01454681 begin
if (exists $ENV{GN_SSGDIAL_SUPPORT})
{
    print "ro.gn.ssgdial.support=$ENV{GN_SSGDIAL_SUPPORT}\n";
}

if (exists $ENV{GN_SSGACCESSCALL_SUPPORT})
{
    print "ro.gn.ssgaccesscall.support=$ENV{GN_SSGACCESSCALL_SUPPORT}\n";
}

if (exists $ENV{GN_ONLY_FOUR_GESTURE} && $ENV{GN_ONLY_FOUR_GESTURE} eq "yes")
{
    print "ro.gn.only.four.gesture=$ENV{GN_ONLY_FOUR_GESTURE}\n";
}
#Gionee wangfei add for CR01454681 end
#Gionee hongsq 2015-05-13 add for CR01465162 begin
if (exists $ENV{GN_OVERSEA_MUSIC_OFFLINE_SUPPORT} && $ENV{GN_OVERSEA_MUSIC_OFFLINE_SUPPORT} eq "yes")
{
    print "ro.gn.oversea.music.offline.ver=$ENV{GN_OVERSEA_MUSIC_OFFLINE_SUPPORT}\n";
}
if (exists $ENV{GN_APK_AMI_CALENDAR_SUPPORT} && $ENV{GN_APK_AMI_CALENDAR_SUPPORT} eq "yes")
{
    print "ro.gn.oversea.ami.calendar.ver=$ENV{GN_APK_AMI_CALENDAR_SUPPORT}\n";
}
#Gionee hongsq 2015-05-13 add for CR01465162 end
#Gionee yangcuntao 2015-05-11 add for amigoframework locales setting begin
if (exists $ENV{GN_PRODUCT_LOCALES_EX1})
{
    print "ro.gn.product.locales.ex1=$ENV{GN_PRODUCT_LOCALES_EX1}\n";
}
if (exists $ENV{GN_PRODUCT_LOCALES_EX2})
{
    print "ro.gn.product.locales.ex2=$ENV{GN_PRODUCT_LOCALES_EX2}\n";
}
if (exists $ENV{GN_PRODUCT_LOCALES_EX3})
{
    print "ro.gn.product.locales.ex3=$ENV{GN_PRODUCT_LOCALES_EX3}\n";
}
if (exists $ENV{GN_PRODUCT_LOCALES_EX4})
{
    print "ro.gn.product.locales.ex4=$ENV{GN_PRODUCT_LOCALES_EX4}\n";
}
if (exists $ENV{GN_PRODUCT_LOCALES_EX5})
{
    print "ro.gn.product.locales.ex5=$ENV{GN_PRODUCT_LOCALES_EX5}\n";
}
#Gionee yangcuntao 2015-05-11 add for amigoframework locales setting end

print "dalvik.vm.checkjni=false\n";
#Gionee wangfei add for CR01484049 begin
if (exists $ENV{GN_SCREEN_PIN_SUPPORT})
{
    print "ro.gn.screen.pin.support=$ENV{GN_SCREEN_PIN_SUPPORT}\n";
}
#Gionee wangfei add for CR01484049 end

#Gionee wangfei 2015-05-19 add for CR01481055 begin
if (exists $ENV{GN_REMOVE_VIRTUAL_KEY} && $ENV{GN_REMOVE_VIRTUAL_KEY} eq "yes")
{
    print "ro.gn.remove.virtual.key=$ENV{GN_REMOVE_VIRTUAL_KEY}\n";
}
#Gionee wangfei 2015-05-19 add for CR01481055 end

#Gionee guoxt 20150520 modify for CR01484740 start
if (exists $ENV{GN_OVERSEA_ODM} && $ENV{GN_OVERSEA_ODM} eq "yes") 
{
    print "ro.gn.oversea.odm=$ENV{GN_OVERSEA_ODM}\n";
}
# Gionee guoxt 20150520 modify for CR01484740 end

#Gionee chenbo 20150710 add for CR01503847 begin
if (exists $ENV{GN_APK_GN_LONGSCREENSHOT_SUPPORT} && $ENV{GN_APK_GN_LONGSCREENSHOT_SUPPORT} eq "yes") 
{
    print "ro.gn.longscreenshot=$ENV{GN_APK_GN_LONGSCREENSHOT_SUPPORT}\n";
}
#Gionee chenbo 20150710 add for CR01503847 end

#GIONEE fuwenzhi 20160421 add ScreenRecorder apk begin
if (exists $ENV{GN_APK_GN_SCREENRECORDER_SUPPORT} && $ENV{GN_APK_GN_SCREENRECORDER_SUPPORT} eq "yes") 
{
    print "ro.gn.screenrecorder.support=$ENV{GN_APK_GN_SCREENRECORDER_SUPPORT}\n";
}
#GIONEE fuwenzhi 20160421 add ScreenRecorder apk end

#GIONEE fuwenzhi 20160524 add screenshots apk begin
if (exists $ENV{GN_APK_GN_SCREENSHOTS_SUPPORT} && $ENV{GN_APK_GN_SCREENSHOTS_SUPPORT} eq "yes")
{
    print "ro.gn.screenshots.support=$ENV{GN_APK_GN_SCREENSHOTS_SUPPORT}\n";
}
#GIONEE fuwenzhi 20160524 add screenshots apk end

#GIONEE li_j add for 7315GI 2015-06-26 begin
#GIONEE:guoxt modify for CR01440649 2015-01-19 begin
if (exists $ENV{GN_OVERSEA_PRODUCT} && $ENV{GN_OVERSEA_PRODUCT} eq "yes")
{
    #GIONEE li_j add for CR01451019 2015-03-10 begin
	if (exists $ENV{GN_DEFAULT_INPUT_METHOD} && $ENV{GN_DEFAULT_INPUT_METHOD} eq "yes")
	{
	    print "ro.gn.default.inputmethod=com.android.inputmethod.latin.LatinIME\n";
	}
	elsif (exists $ENV{GN_DEFAULT_INPUT_METHOD} && $ENV{GN_DEFAULT_INPUT_METHOD} eq "")
	{
	    print "ro.gn.default.inputmethod=com.android.inputmethod.latin.LatinIME\n";
	}
    #GIONEE li_j add for CR01451019 2015-03-10 end
	elsif (exists $ENV{GN_DEFAULT_INPUT_METHOD} && $ENV{GN_DEFAULT_INPUT_METHOD} ne "")
	{
	    print "ro.gn.default.inputmethod=$ENV{GN_DEFAULT_INPUT_METHOD}\n";
	}
}else{
	if (exists $ENV{GN_DEFAULT_INPUT_METHOD} && $ENV{GN_DEFAULT_INPUT_METHOD} eq "yes")
	{
	    print "ro.gn.default.inputmethod=com.iflytek.inputmethod.FlyIME\n";
	}
}
#GIONEE:guoxt modify for CR01440649 2015-01-19 end

#GIONEE:li_j 2015-05-20 add begin for salestrack
if(exists $ENV{GN_OVERSEA_SALES_TRACK_SUPPORT_GI} && $ENV{GN_OVERSEA_SALES_TRACK_SUPPORT_GI} ne "")
{
    print "ro.gn.oversea.salestrack.gi=$ENV{GN_OVERSEA_SALES_TRACK_SUPPORT_GI}\n";
}

if(exists $ENV{GN_OVERSEA_BRAND_SALES_TRACK_NUMBERA} && $ENV{GN_OVERSEA_BRAND_SALES_TRACK_NUMBERA} ne "")
{
    print "ro.gn.oversea.brand.salestracka=$ENV{GN_OVERSEA_BRAND_SALES_TRACK_NUMBERA}\n";
}

if(exists $ENV{GN_OVERSEA_BRAND_SALES_TRACK_NUMBERB} && $ENV{GN_OVERSEA_BRAND_SALES_TRACK_NUMBERB} ne "")
{
    print "ro.gn.oversea.brand.salestrackb=$ENV{GN_OVERSEA_BRAND_SALES_TRACK_NUMBERB}\n";
}
#GIONEE:li_j 2015-05-20 add end for salestrack

#Gionee lijinfang 2016-8-11 add for CR01744875 begin
if(exists $ENV{GN_OVERSEA_SALESTRACK_CI} && $ENV{GN_OVERSEA_SALESTRACK_CI} ne "")
{
    print "ro.gn.oversea.salestrack.ci=$ENV{GN_OVERSEA_SALESTRACK_CI}\n";
}
#Gionee lijinfang 2016-8-11 add for CR01744875 end

#GIONEE:chenxiong 2012-05-30 add begin for CR00608860 //tangzepeng 2012-8-30 merge begin 
if(exists $ENV{GN_OVERSEA_SALES_TRACK_SUPPORT} && $ENV{GN_OVERSEA_SALES_TRACK_SUPPORT} ne "")
{
    print "ro.gn.oversea.salestrack=$ENV{GN_OVERSEA_SALES_TRACK_SUPPORT}\n";
}
#GIONEE:chenxiong 2012-05-30 add end for CR00608860 //tangzepeng 2012-8-30 merge end 
# Gionee wangfei 2015-04-14 modify for CR01456610 begin
if (exists $ENV{GN_HANDSETHOOK_SUPPORT} && $ENV{GN_HANDSETHOOK_SUPPORT} eq "yes") 
{
    print "ro.gn.handsethook=$ENV{GN_HANDSETHOOK_SUPPORT}\n";
}
# Gionee wangfei 2015-04-14 modify for CR01456610 end
#Gionee: lixiaohu 2014-11-22 add for CR01408268 begin
if(exists $ENV{GN_AUTO_RECORDING_CALL_SUPPORT} && $ENV{GN_AUTO_RECORDING_CALL_SUPPORT} eq "yes") {
    print "ro.gn.auto.record.call=$ENV{GN_AUTO_RECORDING_CALL_SUPPORT}\n";
}
if(exists $ENV{GN_PHONE_AUTO_REDIAL_SUPPORT} && $ENV{GN_PHONE_AUTO_REDIAL_SUPPORT} eq "yes") {
    print "ro.gn.phone.auto.redial.support=$ENV{GN_PHONE_AUTO_REDIAL_SUPPORT}\n";
}
#Gionee: lixiaohu 2014-11-22 add for CR01408268 end

#GIONEE li_j 2015-06-04 add CR01496622 begin
if (exists $ENV{GN_MEMORY_DETAIL_SUPPORT} && $ENV{GN_MEMORY_DETAIL_SUPPORT} eq "yes")
{
    print "ro.gn.memory.detail.support=$ENV{GN_MEMORY_DETAIL_SUPPORT}\n";
}
#GIONEE li_j 2015-06-04 add CR01496622 end

#Gionee: li_j 2015-6-9 add for CR01498711 begin
if(exists $ENV{GN_SAR_STR} && $ENV{GN_SAR_STR} ne "")
{
    print "ro.gn.sar.str=$ENV{GN_SAR_STR}\n";
}
#Gionee: li_j 2015-6-9 add for CR01498711 end

#GIONEE li_j 2015-6-13 add for CR01501444 start
if (exists $ENV{GN_IN_CALL_VIBRATION} && $ENV{GN_IN_CALL_VIBRATION} eq "yes")
{
    print "ro.gn.in.call.vibration=$ENV{GN_IN_CALL_VIBRATION}\n";
}
#GIONEE li_j 2015-6-13 add for CR01501444 end

#GIONEE li_j 2015-6-13 add for CR01500369 start
if (exists $ENV{GN_OVERSEA_YOUJU_SUPPORT} && $ENV{GN_OVERSEA_YOUJU_SUPPORT} eq "yes") 
{
    print "ro.gn.oversea.youju.support=$ENV{GN_OVERSEA_YOUJU_SUPPORT}\n";
}
#GIONEE li_j 2015-6-13 add for CR01500369 start

#Gionee li_j 2015-6-13 add for OTA questionnaire begin
if(exists $ENV{GN_OTA_QN_SUPPORT} && $ENV{GN_OTA_QN_SUPPORT} ne "")
{
    print "ro.gn.ota.qn.support=$ENV{GN_OTA_QN_SUPPORT}\n";
}
#Gionee li_j 2015-6-13 add for OTA questionnaire end

#GIONEE li_j add for 7315GI 2015-06-26 begin
#Gionee liming 2015-06-23 add for CR01502000 start
if (exists $ENV{GN_SCREENON_RECEIVE_SMS} && $ENV{GN_SCREENON_RECEIVE_SMS} eq "yes") 
{
	print "ro.gn.screenon.receive.sms=$ENV{GN_SCREENON_RECEIVE_SMS}\n";
	print "ro.gn.screenon.time=$ENV{GN_SCREENON_TIME}\n";
}
#Gionee liming 2015-06-23 add for CR01502000 end

#Gionee chenglijun 2015.07.01 modify CR01510399 begin
if (exists $ENV{GN_APK_AMIGO_SYSTEMUI_SUPPORT})
{
    print "ro.gn.amigo_systemui_support=$ENV{GN_APK_AMIGO_SYSTEMUI_SUPPORT}\n";
}
#Gionee chenglijun 2015.07.01 modify CR01510399 end

#Gionee chenglijun 2015.06.27 modify for CR01529363 merge from CR01508737 begin
if (exists $ENV{GN_APPSKYLIGHT_ORIGINAL_KEYGUARD_SUPPORT} && $ENV{GN_APPSKYLIGHT_ORIGINAL_KEYGUARD_SUPPORT} eq "yes")
{
    print "ro.gn.skylight_Ori_Lock_support=$ENV{GN_APPSKYLIGHT_ORIGINAL_KEYGUARD_SUPPORT}\n";
}
if (exists $ENV{GN_APK_AMIGO_DYMANICWEATHER_SUPPORT} && $ENV{GN_APK_AMIGO_DYMANICWEATHER_SUPPORT} eq "yes")
{
    print "ro.gn.amigo_weather_support=$ENV{GN_APK_AMIGO_DYMANICWEATHER_SUPPORT}\n";
}
#Gionee chenglijun 2015.06.27 modify for CR01529363 merge from CR01508737 end

if(exists $ENV{GN_SEND_ERROR_REPORT_DOMAIN} && $ENV{GN_SEND_ERROR_REPORT_DOMAIN} ne "")
{
    print "ro.gn.send.error.report.domain=$ENV{GN_SEND_ERROR_REPORT_DOMAIN}\n";
}

if(exists $ENV{GN_SEND_ERROR_REPORT_TEST_DOMAIN} && $ENV{GN_SEND_ERROR_REPORT_TEST_DOMAIN} ne "")
{
	print "persist.sys.report.test.domain=$ENV{GN_SEND_ERROR_REPORT_TEST_DOMAIN}\n";
}
#GIONEE li_j add for 7315GI 2015-06-26 end

#GIONEE: wangfei 2015-07-02 add for CR01505780 begin
if (exists $ENV{GN_REMOVE_FINGER_GESTURE} && $ENV{GN_REMOVE_FINGER_GESTURE} eq "yes")
{
    print "ro.gn.remove.finger.gesture=$ENV{GN_REMOVE_FINGER_GESTURE}\n";
}
#GIONEE: wangfei 2015-07-02 add for CR01505780 end

#GIONEE:guoxt modify for CR01437293 2015-01-19 begin
if (exists $ENV{GN_OVERSEA_PRODUCT} && $ENV{GN_OVERSEA_PRODUCT} eq "yes")
{
    if(defined($ENV{GN_OVERSEA_EXTERNAL_VERNUMBER})) {
         print "ro.gn.extvernumber=$ENV{GN_OVERSEA_EXTERNAL_VERNUMBER}\n";
    }
    if(defined($ENV{GN_OVERSEA_HW_VERSION})) {
         print "ro.gn.extHWvernumber=$ENV{GN_OVERSEA_HW_VERSION}\n";
    }
}
#GIONEE:guoxt modify for CR01437293 2015-01-19 end

#Gionee zhouyl 20150721 add for CR01520447 begin
if (exists $ENV{GN_4G_3G_SWITCH} && $ENV{GN_4G_3G_SWITCH} eq "yes")
{
    print "ro.gn.4g.3g.switch=$ENV{GN_4G_3G_SWITCH}\n";
}
#Gionee zhouyl 20150721 add for CR01520447 end
#Gionee li_j 2015-08-03 add for  CR01531986 begin
if (exists $ENV{GN_PASSWORD_PROTECTION} && $ENV{GN_PASSWORD_PROTECTION} eq "yes")
{
    print "ro.gn.password.protection=$ENV{GN_PASSWORD_PROTECTION}\n";
}
#Gionee li_j 2015-08-03 add for CR01531986 end

#Gionee li_j 2015-08-06 add for  CR01534272 begin
if (exists $ENV{GN_LAUNCHER_GUIDE_SUPPORT} && $ENV{GN_LAUNCHER_GUIDE_SUPPORT} eq "yes")
{
    print "ro.gn.launcher.guide.support=$ENV{GN_LAUNCHER_GUIDE_SUPPORT}\n";
}
#Gionee li_j 2015-08-06 add for CR01534272  end
#GIONEE li_j 2015-06-04 add CR01496622 begin
if (exists $ENV{GN_MEMORY_DETAIL_SUPPORT} && $ENV{GN_MEMORY_DETAIL_SUPPORT} eq "yes")
{
    print "ro.gn.memory.detail.support=$ENV{GN_MEMORY_DETAIL_SUPPORT}\n";
}
#GIONEE li_j 2015-06-04 add CR01496622 end
#GIONEE:li_j 2015-05-19 add begin 
if(exists $ENV{GN_MMS_SERVICE_SUPPORT} && $ENV{GN_MMS_SERVICE_SUPPORT} eq "yes")
{
    print "ro.gn.feature.tab.service=$ENV{GN_MMS_SERVICE_SUPPORT}\n";
}
#GIONEE:li_j 2015-05-19 add end
#Gionee lijinfang 2015-4-9 add for CR01463051 begin
if(exists $ENV{GN_SEND_ERROR_REPORT_TEST_DOMAIN} && $ENV{GN_SEND_ERROR_REPORT_TEST_DOMAIN} ne "")
{
	print "persist.sys.report.test.domain=$ENV{GN_SEND_ERROR_REPORT_TEST_DOMAIN}\n";
}
#Gionee lijinfang 2015-4-9 add for CR01463051 end

#Gionee <Gn_Assist> <eyb> <2015-09-28> for VOICE_SMS_TRANSLATE begin
if(defined($ENV{GN_RO_VOICE_SMS_TRANSLATE})) {
    print "ro.local.sms.translate=$ENV{GN_RO_VOICE_SMS_TRANSLATE}\n";
}
if(defined($ENV{GN_RO_VOICE_SETTING_CONTROL})) {
    print "ro.voice.setting.switch=$ENV{GN_RO_VOICE_SETTING_CONTROL}\n";
}
#Gionee <Gn_Assist> <eyb> <2015-09-28> for VOICE_SMS_TRANSLATE end

#Gionee <GN_CLONE_APP> <qudw> <20160111> modify for CR01620985 begin
if (exists $ENV{GN_APP_CLONE_SUPPORT} && $ENV{GN_APP_CLONE_SUPPORT} eq "yes")
{
    print "ro.gn.app.clone.support=$ENV{GN_APP_CLONE_SUPPORT}\n";
}
#Gionee <GN_CLONE_APP> <qudw> <20160111> modify for CR01620985 end

#Gionee 20150110 chenrui add for CR01432011 start
if (defined($ENV{GN_USB_UI_SUPPORT})) {
	print "ro.gn.usb.ui.support=$ENV{GN_USB_UI_SUPPORT}\n";
}
#Gionee 20150110 chenrui add for CR01432011 end

#Gionee 20160715 chenrui add for CR01731146 start
if (exists $ENV{GN_APK_AMIGO_STORYLOCKER_SUPPORT} && $ENV{GN_APK_AMIGO_STORYLOCKER_SUPPORT} ne "") {
	print "ro.gn.apk.amigo.storylocker=$ENV{GN_APK_AMIGO_STORYLOCKER_SUPPORT}\n";
}
#Gionee 20160715 chenrui add for CR01731146 end

#Gionee 20160315 chenrui add for CR01647849 start
if (defined($ENV{GN_LOCKER_ONLINE_SUPPORT})) {
	print "ro.gn.locker.online.support=$ENV{GN_LOCKER_ONLINE_SUPPORT}\n";
}
#Gionee 20160315 chenrui add for CR01647849 end

#Gionee 20160506 chenrui add for CR01682347 start
if (exists $ENV{GN_LOCKER_OFFLINE} && $ENV{GN_LOCKER_OFFLINE} eq "yes") {
	print "ro.gn.locker.offline=$ENV{GN_LOCKER_OFFLINE}\n";
}
#Gionee 20160506 chenrui add for CR01682347 end

#Gionee 20160325 chenrui add for CR01643607 start
if (exists $ENV{GN_WIPE_NOT_SUPPORT} && $ENV{GN_WIPE_NOT_SUPPORT} eq "yes") {
	print "ro.gn.wipe.not.support=$ENV{GN_WIPE_NOT_SUPPORT}\n";
}
#Gionee 20160325 chenrui add for CR01643607 end

#Gionee 20160419 chenrui add for CR01675607 start
if (exists $ENV{GN_FP_BLACK_SCREEN_UNLOCK} && $ENV{GN_FP_BLACK_SCREEN_UNLOCK} eq "yes") {
	print "ro.gn.fp.black.screen.unlock=$ENV{GN_FP_BLACK_SCREEN_UNLOCK}\n";
}
#Gionee 20160419 chenrui add for CR01675607 end

#Gionee 20160615 chenrui add for CR01619167 start
if (exists $ENV{GN_LOCKER_OWNER_INFO_SUPPORT} && $ENV{GN_LOCKER_OWNER_INFO_SUPPORT} eq "yes") {
    print "ro.gn.locker.owner.info.support=$ENV{GN_LOCKER_OWNER_INFO_SUPPORT}\n";
}
#Gionee 20160615 chenrui add for CR01619167 end

#Gionee 20150725 chenrui add for CR01523192 start
if (exists $ENV{GN_NAVI_OWNER_INFO_SUPPORT} && $ENV{GN_NAVI_OWNER_INFO_SUPPORT} eq "yes")
{
    print "ro.gn.navi.owner.info.support=$ENV{GN_NAVI_OWNER_INFO_SUPPORT}\n";
}
#Gionee 20150725 chenrui add for CR01523192 end

#Gionee 20150902 chenrui add for CR01547283 start
if (exists $ENV{GN_APK_AMIGO_SYSTEMUI_SUPPORT})
{
    print "ro.gn.amigo.systemui.support=$ENV{GN_APK_AMIGO_SYSTEMUI_SUPPORT}\n";
}
#Gionee 20150902 chenrui add for CR01547283 end

#Gionee 20150916 chenrui add for CR01552346 start
if (exists $ENV{GN_DOUBLE_TAP_SUPPORT})
{
    print "ro.gn.double.tap.support=$ENV{GN_DOUBLE_TAP_SUPPORT}\n";
}
#Gionee 20150916 chenrui add for CR01552346 end

#Gionee 20160520 chenrui add for CR01703973 start
#default yes
if (exists $ENV{GN_KG_3D_TOUCH_SUPPORT} && $ENV{GN_KG_3D_TOUCH_SUPPORT} ne "") {
    print "ro.gn.kg.3d.touch.support=$ENV{GN_KG_3D_TOUCH_SUPPORT}\n";
}
#Gionee 20160520 chenrui add for CR01703973 end

#Gionee 20160504 chenrui add for CR01620318 start
if (exists $ENV{GN_KEYGUARD_CARRIER_DISPLAY} && $ENV{GN_KEYGUARD_CARRIER_DISPLAY} eq "yes") {
    print "ro.gn.keyguard.carrier.display=$ENV{GN_KEYGUARD_CARRIER_DISPLAY}\n";
}
#Gionee 20160504 chenrui add for CR01620318 end

if (exists $ENV{GN_THREE_FINGER_GUESTMODE} && $ENV{GN_THREE_FINGER_GUESTMODE} eq "yes") {
    print "ro.gn.three.finger.guestmode=$ENV{GN_THREE_FINGER_GUESTMODE}\n";
}

#Gionee 20160409 lixiaohong add for Marshmallow face unlock start
if (exists $ENV{GN_FACE_UNLOCK_SUPPORT} && $ENV{GN_FACE_UNLOCK_SUPPORT} eq "yes") {
    print "ro.gn.amigo.face.unlock=$ENV{GN_FACE_UNLOCK_SUPPORT}\n";
}
#Gionee 20160409 lixiaohong add for Marshmallow face unlock end
#GIONEE 20160823 lixiaohong add for PrivateSpace support start
if (exists $ENV{GN_APK_GN_ENCRYPTIONSPACE_SUPPORT} && $ENV{GN_APK_GN_ENCRYPTIONSPACE_SUPPORT} eq "yes") {
    print "ro.encryptionspace.enabled=true\n";
    print "persist.private.enroll=close\n";
}
#GIONEE 20160823 lixiaohong add for PrivateSpace support end
if (exists $ENV{GN_MUSIC_KEYGUARD_WIDGET} && $ENV{GN_MUSIC_KEYGUARD_WIDGET} eq "yes") {
    print "ro.gn.music.keyguard.widget=$ENV{GN_MUSIC_KEYGUARD_WIDGET}\n";
}

if (exists $ENV{GN_SKYLIGHT_MENU} && $ENV{GN_SKYLIGHT_MENU} eq "yes")
{
    print "ro.gn.skylight.menu=$ENV{GN_SKYLIGHT_MENU}\n";
}

if (exists $ENV{GN_PRIVATESPACE_EXIST} && $ENV{GN_PRIVATESPACE_EXIST} eq "yes")
{
    print "ro.gn.privatespace.exist=$ENV{GN_PRIVATESPACE_EXIST}\n";
}
#Gionee hushengsong 2015-11-07 modify for CR01587010 begin
if (exists $ENV{GN_SECRUE_SCREEN_PIN_SUPPORT} && $ENV{GN_SECRUE_SCREEN_PIN_SUPPORT} eq "yes")
{
    print "ro.gn.secure.screen.pin.support=$ENV{GN_SECRUE_SCREEN_PIN_SUPPORT}\n";
}
#Gionee hushengsong 2015-11-07 modify for CR01587010 end

#Gionee hushengsong 2015-11-13 modify for CR01586054 begin
if (exists $ENV{GN_CUSTOM_GESTURE_SWITCH} && $ENV{GN_CUSTOM_GESTURE_SWITCH} eq "yes")
{
    print "ro.gn.custom.gesture.switch=$ENV{GN_CUSTOM_GESTURE_SWITCH}\n";
}
#Gionee hushengsong 2015-11-13 modify for CR01586054 end

#gionee futao 20151103 modify for CR01580966 begin
if(defined($ENV{GN_RW_GN_AUTOMMI_KEYTEST_APP})) {
	print "gn.autommi.keytest.app=$ENV{GN_RW_GN_AUTOMMI_KEYTEST_APP}\n";
}
if(defined($ENV{GN_RW_GN_ATUOMMI_KEYTEST_HALL})) {
	print "gn.autommi.keytest.hall=$ENV{GN_RW_GN_ATUOMMI_KEYTEST_HALL}\n";
}
if(defined($ENV{GN_RW_GN_ATUOMMI_KEYTEST_BACK})) {
	print "gn.autommi.keytest.back=$ENV{GN_RW_GN_ATUOMMI_KEYTEST_BACK}\n";
}
if(defined($ENV{GN_RW_GN_ATUOMMI_KEYTEST_HOME})) {
	print "gn.autommi.keytest.home=$ENV{GN_RW_GN_ATUOMMI_KEYTEST_HOME}\n";
}
#gionee futao 20151103 modify for CR01580966 end

#gionee chen_long02 2015-05-27 add for CR01490203 begin
if (exists $ENV{GN_VIDEO_PLAY_TIP_SUPPORT} && $ENV{GN_VIDEO_PLAY_TIP_SUPPORT} eq "yes")
{
    print "ro.gn.video.play.tip.support=$ENV{GN_VIDEO_PLAY_TIP_SUPPORT}\n";
}
#gionee chen_long02 2015-05-27 add for CR01490203 end

if (exists $ENV{GN_VIDEO_DTS_SUPPORT} && $ENV{GN_VIDEO_DTS_SUPPORT} eq "yes")
{
    print "ro.gn.video.dts.support=$ENV{GN_VIDEO_DTS_SUPPORT}\n";
}

#Gionee <Amigo_BatteryRecord> <xush> <2015-5-30> add for CR01493888 begin
if(defined($ENV{GN_BATTERY_RECORD})) {
    print "ro.gn.battery.record=$ENV{GN_BATTERY_RECORD}\n";
}
#Gionee <Amigo_BatteryRecord> <xush> <2015-5-30> add for CR01493888 end

#Gionee chen_long02 20150813 add for CR01537141 begin
if (exists $ENV{GN_FLOATVIDEO_CONTROL_SUPPORT} && $ENV{GN_FLOATVIDEO_CONTROL_SUPPORT} eq "yes") 
{
    print "ro.gn.floatvideo.control=$ENV{GN_FLOATVIDEO_CONTROL_SUPPORT}\n";
}
#Gionee chen_long02 20150813 add for CR01537141 end
#GIONEE:guoxt modify for salesstatistic  2015-07-21 begin
if (exists $ENV{GN_OVERSEA_PRODUCT} && $ENV{GN_OVERSEA_PRODUCT} eq "yes")
{
  print "persist.sys.net.sales.flag=0\n";
  print "persist.sys.mms.sales.flag=0\n";
}
#GIONEE:guoxt modify for salesstatistic  2015-07-21 end
if(defined($ENV{GN_RO_GN_MUSIC_SDCARD_RINGTONE})) {
	print "ro.gn.music.sdcard.ringtone=$ENV{GN_RO_GN_MUSIC_SDCARD_RINGTONE}\n";
}
#Gionee rench 2015-08-26 add for CR01543785 begin
if (exists $ENV{TORCH_GESTURE_SUPPORT} && $ENV{TORCH_GESTURE_SUPPORT} eq "yes")
{
    print "ro.gn.torch.gesture.support=$ENV{TORCH_GESTURE_SUPPORT}\n";
}
#Gionee rench 2015-08-26 add for CR01543785 end
#Gionee xiaopeng 2015-03-13 add for CR01453846 begin
if(defined($ENV{GN_LAUNCHER_DBLINETEXT})) {
	print "ro.gn.launcher.dblinetext=$ENV{GN_LAUNCHER_DBLINETEXT}\n";
}
#Gionee xiaopeng 2015-03-13 add for CR01453846 end
#Gionee renfei 20150902 add for CR01546056 begin
if (exists $ENV{GN_ANTITHEFTTRACK_GI_SUPPORT} && $ENV{GN_ANTITHEFTTRACK_GI_SUPPORT} eq "yes")
{
    print "ro.gn.antithefttrack.support=$ENV{GN_ANTITHEFTTRACK_GI_SUPPORT}\n";
    print "persist.sys.att.spw.prop=0\n";
    print "persist.sys.att.fn.prop=0\n";
}
#Gionee renfei 20150902 add for CR01546056 end
#Gionee renfei 20150923 add for CR01558646 begin
if (exists $ENV{GN_ANTITHEFTTRACK_SUPPORT} && $ENV{GN_ANTITHEFTTRACK_SUPPORT} eq "yes")
{
    print "ro.gn.antistolen.tl.support=$ENV{GN_ANTITHEFTTRACK_SUPPORT}\n";
    print "ro.gn.antistolen.controlphone=$ENV{GN_ANTITHEFTTRACK_CONTROLPHONE_NUMBER}\n";
}
#Gionee renfei 20150923 add for CR01558646 end
#gionee yewq 2015-9-15 add for CR01553983 begin
if(defined($ENV{GN_AMIGOVERSION})) {
    print "ro.gn.amigoversion=$ENV{GN_AMIGOVERSION}\n";
}
#gionee yewq 2015-9-15 add for CR01553983 end
#Gionee yubo 2015.11.20 add for CR01594729 begin
if (exists $ENV{GN_APK_AMIGO_SERVICE_SUPPORT} && $ENV{GN_APK_AMIGO_SERVICE_SUPPORT} eq "yes")
{
    print "ro.gn.amigo.service.support=$ENV{GN_APK_AMIGO_SERVICE_SUPPORT}\n";
}
#Gionee yubo 2015.11.20 add for CR01594729 begin
#Gionee xuyongji 2015.11.24 add for CR01597684 start
if(exists $ENV{GN_APP_Setting_APN_FILTER_SUPPORT} && $ENV{GN_APP_Setting_APN_FILTER_SUPPORT} ne "")
{
    print "ro.gn.apn.filter.support=$ENV{GN_APP_Setting_APN_FILTER_SUPPORT}\n";
}
#Gionee xuyongji 2015.11.24 add for CR01597684 end
#Gionee xuyongji 2015.8.7 add for CR01534807 begin
if (exists $ENV{GN_GROUP_MMS_SUPPORT} && $ENV{GN_GROUP_MMS_SUPPORT} eq "yes")
{
    print "ro.gn.group.mms.support=$ENV{GN_GROUP_MMS_SUPPORT}\n";
}
#Gionee xuyongji 2015.8.7 add for CR01534807 end
#Gionee xuyongji 2015.8.3 add for CR01532360 begin
if (exists $ENV{GN_MMS_SIZE_LIMIT} && $ENV{GN_MMS_SIZE_LIMIT} eq "yes")
{
    print "ro.gn.mms.size.limit=$ENV{GN_MMS_SIZE_LIMIT}\n";
}
#Gionee xuyongji 2015.8.3 add for CR01532360 end
#Gionee <Log_opt> <xush> <2015-11-14> add for CR01589976 begin
if(defined($ENV{GN_TOUCH_FILTER_LOG_DISABLE})) {
    print "sys.input.TouchFilterLogEnable=false\n";
}
#Gionee <Log_opt> <xush> <2015-11-14> add for CR01589976 end
#Gionee lucy 2015-11-28 add for QM begin
if (exists $ENV{GN_MEMORY_OTG_SUPPORT} && $ENV{GN_MEMORY_OTG_SUPPORT} eq "yes")
{
    print "ro.gn.memory.otg.support=$ENV{GN_MEMORY_OTG_SUPPORT}\n";
}
#Gionee lucy 2015-11-28 add for QM end
#Gionee <TP colours> <zhonggj> <2012-02-28> add for CR01087148 begin
if (exists $ENV{GN_DYNAMIC_THEME_SUPPORT} && $ENV{GN_DYNAMIC_THEME_SUPPORT} eq "yes")
{
    print "ro.gn.dynamic.theme.supprot=$ENV{GN_DYNAMIC_THEME_SUPPORT}\n";
    print "persist.radio.dynamic.theme=$ENV{GN_DYNAMIC_THEME_DEFAULT_VAULE}\n";
}
#Gionee <TP colours> <zhonggj> <2012-02-28> add for CR01087148 end
#Gionee rench 2015-12-05 add for CR01602065 begin
if (exists $ENV{GN_FRONT_BEAUTY_SUPPORT} && $ENV{GN_FRONT_BEAUTY_SUPPORT} eq "yes")
{
    print "ro.gn.front.beauty.support=$ENV{GN_FRONT_BEAUTY_SUPPORT}\n";
}
#Gionee rench 2015-12-05 add for CR01602065 end

#Gionee hushengsong 2015-12-10 modify for CR01606702 begin
if (exists $ENV{GN_FAST_CAMERA_SUPPORT} && $ENV{GN_FAST_CAMERA_SUPPORT} eq "yes")
{
    print "ro.gn.fast.camera.support=$ENV{GN_FAST_CAMERA_SUPPORT}\n";
}
#Gionee hushengsong 2015-12-10 modify for CR01606702 end
#Gionee yuchao 2015-12-11 add for CR01607949 begin
if (exists $ENV{GN_AMIGO_RECENT_SUPPORT} && $ENV{GN_AMIGO_RECENT_SUPPORT} eq "yes")
{
    print "ro.gn.recents.support=$ENV{GN_AMIGO_RECENT_SUPPORT}\n";
}
#Gionee yuchao 2015-12-11 add for CR01607949 end
#Gionee yuchao 2015-12-16 add for CR01610179 begin
if (exists $ENV{GN_AMIGO_VOLTE_SUPPORT} && $ENV{GN_AMIGO_VOLTE_SUPPORT} eq "yes")
{
    print "ro.gn.volte.support=$ENV{GN_AMIGO_VOLTE_SUPPORT}\n";
}
#Gionee yuchao 2015-12-16 add for CR01610179 end
#Gionee yuchao 2015-12-28 add for CR01521432 begin
if (exists $ENV{GN_AMIGO_NEWCLOSEALL_SUPPORT} && $ENV{GN_AMIGO_NEWCLOSEALL_SUPPORT} eq "yes")
{
    print "ro.gn.newcloseall.support=$ENV{GN_AMIGO_NEWCLOSEALL_SUPPORT}\n";
}
#Gionee yuchao 2015-12-28 add for CR01521432 end
#gionee taofp add for check oversea rom CR01586700 beign
if (exists $ENV{ADUPS_FOTA_SUPPORT} && $ENV{ADUPS_FOTA_SUPPORT} eq "yes")
{
    print "ro.gn.adups.fota.support=$ENV{ADUPS_FOTA_SUPPORT}\n";
}
#gionee taofp add for check oversea rom CR01586700 end
#Gionee: guanxiaowen 2015.03.24 add for condor start
if (exists $ENV{GN_CUSTOMER_MARKET_MODEL} && $ENV{GN_CUSTOMER_MARKET_MODEL} ne "")
{
    print "ro.customer.market.model=$ENV{GN_CUSTOMER_MARKET_MODEL}\n";
}
#Gionee: guanxiaowen 2015.03.24 add for condor end
#Gionee luo_jie 2016-03-31 add for CR01663705 begin
if (exists $ENV{GN_SMS_BEEP_SOUND_IN_CALL} && $ENV{GN_SMS_BEEP_SOUND_IN_CALL} eq "yes") 
{
    print "ro.gn.sms.beep.sound.in.call=$ENV{GN_SMS_BEEP_SOUND_IN_CALL}\n";
}
#Gionee luo_jie 2016-03-31 add for CR01663705 end
#Gionee xuyongji 2015-01-15 add for CR01624310 begin
if (exists $ENV{GN_SHAKE_CLEAN_SUPPORT} && $ENV{GN_SHAKE_CLEAN_SUPPORT} eq "no")
{
    print "ro.gn.shake.clean=$ENV{GN_SHAKE_CLEAN_SUPPORT}\n";
}
#Gionee xuyongji 2015-01-15 add for CR01624310 end
#Gionee:lijinfang 20160115 modify for CR01624725 begin
if (exists $ENV{GN_OVERSEA_YOUJU_SUPPORT} && $ENV{GN_OVERSEA_YOUJU_SUPPORT} eq "yes")
{
    print "ro.gn.userexperience.support=$ENV{GN_OVERSEA_YOUJU_SUPPORT}\n";
}
#Gionee:lijinfang 20160115 modify for CR01624725 end



#Gionee <GN_DUAL_SYSTEM_SUPPORT> <puym> <20160119> modify for CR01626755 begin
if (exists $ENV{GN_DUAL_SYSTEM_SUPPORT} && $ENV{GN_DUAL_SYSTEM_SUPPORT} eq "yes")
{
    print "ro.gn.app.dualsystem.support=$ENV{GN_DUAL_SYSTEM_SUPPORT}\n";
}
#Gionee <GN_DUAL_SYSTEM_SUPPORT> <puym> <20160119> modify for CR01626755 end

#Gionee <lixiaohu> <20150827> modify for CR01545207 begin
if (exists $ENV{GN_SHOW_MEID} && $ENV{GN_SHOW_MEID} eq "yes")
{
    print "ro.gn.show.meid=$ENV{GN_SHOW_MEID}\n";
}
#Gionee <lixiaohu> <20150827> modify for CR01545207 end
#Gionee lixiaohu 20150926 add for CR01560212 begin
if(exists $ENV{GN_SHOW_DUL_IMEI_MEID} && $ENV{GN_SHOW_DUL_IMEI_MEID} eq "yes")
{
    print "ro.gn.show.dul.imei=$ENV{GN_SHOW_DUL_IMEI_MEID}\n";
}
#Gionee lixiaohu 20150926 add for CR01560212 end

#Gionee <GN_SOTER_SUPPORT> <qudw> <20160130> modify for CR01633912 begin
if (exists $ENV{GN_SOTER_SUPPORT} && $ENV{GN_SOTER_SUPPORT} eq "yes")
{
    print "ro.gn.soter.support=$ENV{GN_SOTER_SUPPORT}\n";
}
#Gionee <GN_WECHAT_PAY_SUPPORT> <qudw> <20160130> modify for CR01633912 end

#Gionee hushengsong 2016-02-18 modify for remove push begin
if (exists $ENV{RO_GN_REMOVE_NOTIFICATION_PUSH} && $ENV{RO_GN_REMOVE_NOTIFICATION_PUSH} eq "yes")
{
    print "ro.gn.remove.notification.push=$ENV{RO_GN_REMOVE_NOTIFICATION_PUSH}\n";
}
#Gionee hushengsong 2016-02-18 modify for remove push end

#Gionee xiaopeng 2016-02-23 modify for CR01612878 begin
if (exists $ENV{GN_LAUNCHER_GUIDE_SUPPORT} && $ENV{GN_LAUNCHER_GUIDE_SUPPORT} eq "yes")
{
    print "ro.gn.launcher.guide.support=$ENV{GN_LAUNCHER_GUIDE_SUPPORT}\n";
}
#Gionee xiaopeng 2016-02-23 modify for CR01612878 end

#Gionee <GN_DUAL_SYSTEM_SUPPORT> <puym> <20160227> modify for CR01639284 begin
if (exists $ENV{GN_DUAL_SYSTEM_SUPPORT} && $ENV{GN_DUAL_SYSTEM_SUPPORT} eq "yes")
{
    print "fw.max_users=5\n";
}
#Gionee <GN_DUAL_SYSTEM_SUPPORT> <puym> <20160227> modify for CR01639284 end
#Gionee lixiaohong 2016-02-29 modify for music notification display on lockscreen begin
if (exists $ENV{GN_MUSIC_KEYGUARD_WIDGET} && $ENV{GN_MUSIC_KEYGUARD_WIDGET} eq "yes") {
    print "ro.gn.music.keyguard.widget=$ENV{GN_MUSIC_KEYGUARD_WIDGET}\n";
}
#Gionee lixiaohong 2016-02-29 modify for music notification display on lockscreen end
#Gionee taofp <20160315> modify for CR01650742 begin
if (exists $ENV{GN_MOBILE_DATA_ON} && $ENV{GN_MOBILE_DATA_ON} eq "yes") {
    print "ro.gn.mobile.data.on=$ENV{GN_MOBILE_DATA_ON}\n";
}
#Gionee taofp <20160315> modify for CR01650742 end

#Gionee rench 2016-03-15 add for CR01650369 begin
if (exists $ENV{GN_SWITCH_STORAGE_SUPPORT} && $ENV{GN_SWITCH_STORAGE_SUPPORT} eq "yes")
{
    print "ro.gn.switch.storage.support=$ENV{GN_SWITCH_STORAGE_SUPPORT}\n";
}
#Gionee rench 2016-03-15 add for CR01650369 end
#gionee yewq 2016-4-7 modify for CR01670005 begin
if (exists $ENV{GN_FRONT_FLASH_SUPPORT} && $ENV{GN_FRONT_FLASH_SUPPORT} eq "yes")
{
    print "ro.gn.front.flash.support=$ENV{GN_FRONT_FLASH_SUPPORT}\n";
}
#gionee yewq 2016-4-7 modify for CR01670005 end
#Gionee hushengsong 2016-02-20 modify for CR01636440 begin
if (exists $ENV{GN_REMOVE_WECHAT_CLONE} && $ENV{GN_REMOVE_WECHAT_CLONE} eq "yes")
{
    print "ro.gn.remove.wechat.clone=$ENV{GN_REMOVE_WECHAT_CLONE}\n";
}
#Gionee hushengsong 2016-02-20 modify for CR01636440 end 

#Gionee hushengsong 2016-03-04 modify for CR01646191 begin
if (exists $ENV{GN_DEFAULT_DEV_MODE} && $ENV{GN_DEFAULT_DEV_MODE} eq "yes")
{
    print "ro.gn.default.dev.mode=$ENV{GN_DEFAULT_DEV_MODE}\n";
}
#Gionee hushengsong 2016-03-04 modify for CR01646191 end
#Gionee hushengsong 2016-03-11 modify for CR01645060 begin
if (exists $ENV{RO_GN_MTK_AAL_SUPPORT} && $ENV{RO_GN_MTK_AAL_SUPPORT} eq "yes")
{
    print "ro.gn.mtk.aal.support=$ENV{RO_GN_MTK_AAL_SUPPORT}\n";
}
#Gionee hushengsong 2016-03-11 modify for CR01645060 end
#Gionee zhanglz 2016.3.31 add for CR01664915 start
if(exists $ENV{GN_APP_Setting_APN_FILTER_SUPPORT} && $ENV{GN_APP_Setting_APN_FILTER_SUPPORT} ne "")
{
    print "ro.gn.apn.filter.support=$ENV{GN_APP_Setting_APN_FILTER_SUPPORT}\n";
}
#Gionee zhanglz 2016.3.31 add for CR01664915 end
#Gionee taofp 2016-03-11 modify for CR01628549 begin
if (exists $ENV{GN_OVERSEA_ROM} && $ENV{GN_OVERSEA_ROM} ne "")
{
    print "ro.gn.oversea.rom=$ENV{GN_OVERSEA_ROM}\n";
}
if (exists $ENV{GN_RO_GN_NFC_SUPPORT} && $ENV{GN_RO_GN_NFC_SUPPORT} eq "yes"){
#do nothing
}else{
    print "ro.setupwizard.suppress_d2d=true\n";
    print "ro.setupwizard.suppress_d2d_nfc=true\n";
}
#Gionee taofp 2016-03-11 modify for CR01628549 end
#Gionee lugouangming 2016.6.1 modify CR01711773 begin
if (exists $ENV{GN_MTP_DEFAULT_ON} && $ENV{GN_MTP_DEFAULT_ON} eq "yes")
{
    print "ro.gn.mtp.on=$ENV{GN_MTP_DEFAULT_ON}\n";
}
#Gionee lugouangming 2016.6.1 modify CR01711773  end 
##Gionee futao 20160330 modfiy for CR01663704 begin
if (exists $ENV{GN_RW_GN_MMI_SETCOLOR} && $ENV{GN_RW_GN_MMI_SETCOLOR} eq "yes") {
	print "gn.mmi.setcolor=$ENV{GN_RW_GN_MMI_SETCOLOR}\n";
}
if (exists $ENV{GN_RW_GN_MMI_WCDMA} && $ENV{GN_RW_GN_MMI_WCDMA} eq "yes") {
	print "gn.mmi.wcdma=$ENV{GN_RW_GN_MMI_WCDMA}\n";
}
if (exists $ENV{GN_RW_GN_MMI_CDMA} && $ENV{GN_RW_GN_MMI_CDMA} eq "yes") {
	print "gn.mmi.cdma=$ENV{GN_RW_GN_MMI_CDMA}\n";
}
if (exists $ENV{GN_RW_GN_MMI_TDSCDMA} && $ENV{GN_RW_GN_MMI_TDSCDMA} eq "yes") {
	print "gn.mmi.tdscdma=$ENV{GN_RW_GN_MMI_TDSCDMA}\n";
}
if (exists $ENV{GN_RW_GN_MMI_LTETDD} && $ENV{GN_RW_GN_MMI_LTETDD} eq "yes") {
	print "gn.mmi.ltetdd=$ENV{GN_RW_GN_MMI_LTETDD}\n";
}
if (exists $ENV{GN_RW_GN_MMI_LTEFDD} && $ENV{GN_RW_GN_MMI_LTEFDD} eq "yes") {
	print "gn.mmi.ltefdd=$ENV{GN_RW_GN_MMI_LTEFDD}\n";
}
if (exists $ENV{GN_RW_GN_MMI_GSM} && $ENV{GN_RW_GN_MMI_GSM} eq "no") {
	print "gn.mmi.gsm=$ENV{GN_RW_GN_MMI_GSM}\n";
}
##Gionee futao 20160330 modfiy for CR01663704 end
#Gionee yuchao 20160411 add for CR01664014 begin
if (exists $ENV{GN_MTK_4G_SUPPORT} && $ENV{GN_MTK_4G_SUPPORT} eq "no") {
	print "ro.gn.4G.support=$ENV{GN_MTK_4G_SUPPORT}\n";
}
#Gionee yuchao 20160411 add for CR01664014 end
#Gionee yewq 2016-4-16 modify for CR01670784 begin
if (exists $ENV{GN_BACK_FINGER_SUPPORT} && $ENV{GN_BACK_FINGER_SUPPORT} eq "yes")
{
    print "ro.gn.back.finger.support=$ENV{GN_BACK_FINGER_SUPPORT}\n";
}
#Gionee yewq 2016-4-16 modify for CR01670784 end
#Gionee taofp 20160428 add for open permission begin  	1625
if (exists $ENV{GN_ENABLE_PERMISSION} && $ENV{GN_ENABLE_PERMISSION} eq "yes")
{
	print "ro.gn.enable.permission=$ENV{GN_ENABLE_PERMISSION}\n";
}
#Gionee taofp 20160428 add for open permission end  
#Gionee fengpeipei 20160504 add for CR01683757 begin
if (exists $ENV{GN_RO_ROMRAM_DISPLAY} && $ENV{GN_RO_ROMRAM_DISPLAY} eq "yes") {
	print "ro.gn.romram.display=$ENV{GN_RO_ROMRAM_DISPLAY}\n";
}
#Gionee fengpeipei 20160504 add for CR01683757 end

#gionee chenwu 20160621 modify for CR01721071 begin
if(defined($ENV{GN_APK_AMIGO_SETUPWIZARD_SUPPORT})) {
print "ro.gn.setupwizard.support=$ENV{GN_APK_AMIGO_SETUPWIZARD_SUPPORT}\n";
}
#gionee chenwu 20160621 modify for CR01721071 end

#gionee chenwu 20160630 modify for CR01719695 begin
if (exists $ENV{GN_DESKTOP_SORT_SUPPORT} && $ENV{GN_DESKTOP_SORT_SUPPORT} eq "yes")
{
    print "ro.gn.desktop.sort.support=$ENV{GN_DESKTOP_SORT_SUPPORT}\n";
}
#gionee chenwu 20160630 modify for CR01719695 end

#gionee yuchao 20160725 add for CR01737216 begin
if (exists $ENV{GN_APK_GN_SCREENRECORDER_SUPPORT} && $ENV{GN_APK_GN_SCREENRECORDER_SUPPORT} eq "yes") 
{
    print "ro.gn.screenrecorder.support=$ENV{GN_APK_GN_SCREENRECORDER_SUPPORT}\n";
}
#gionee yuchao 20160725 add for CR01737216 end

#Gionee fuziqing 20160524 add for multiwindow begin
if (exists $ENV{GN_RO_MULTIWINDOW_SUPPORT} && $ENV{GN_RO_MULTIWINDOW_SUPPORT} eq "yes")
{
	print "ro.gn.multiwindow.support=true\n";
	print "ro.gn.ntfcenter.icon.support=false\n";
	print "ro.gn.headsup.icon.support=true\n";
}
#Gionee fuziqing 20160524 add for multiwindow end

#Gionee fengyao add for CR01732359 begin
if (exists $ENV{GN_CT_SUPPORT} && $ENV{GN_CT_SUPPORT} eq "yes")
{
    print "ro.operator.optr=OM\n";
}
#Gionee fengyao add for CR01732359 end
#Gionee zhangke 20160704 add for CR01725848 start
if(defined($ENV{GN_RW_GN_TP_DATA_SERVICE_SUPPORT})) {
	print "gn.tp.getdataservice=$ENV{GN_RW_GN_TP_DATA_SERVICE_SUPPORT}\n";
}
#Gionee zhangke 20160704 add for CR01725848 end
#Gionee hushengsong 2016-08-13 modify for CR01746077 begin
if (exists $ENV{RO_GN_SUPPORT_SELFIE_GESTURE} && $ENV{RO_GN_SUPPORT_SELFIE_GESTURE} eq "yes")
{
    print "ro.gn.support.selfie.gesture=$ENV{RO_GN_SUPPORT_SELFIE_GESTURE}\n";
}
#Gionee hushengsong 2016-08-13 modify for CR01746077 end

#Gionee luo_jie 2016-09-19 add for CR01762205 begin
if (exists $ENV{GN_RO_GN_PERFECT_SUSPENDBUTTON} && $ENV{GN_RO_GN_PERFECT_SUSPENDBUTTON} eq "yes")
{
    print "ro.gn.perfect.suspendbutton=$ENV{GN_RO_GN_PERFECT_SUSPENDBUTTON}\n";
}
#Gionee luo_jie 2016-09-19 add for CR01762205 end

#Gionee <xujinwen> <20160921> modify for the private space2.0 start
#Gionee chen_bin 2016-8-29 add for the private space 2.0
if (exists $ENV{GN_RO_ENCRYPTIONSPACE_SUPPORT} && $ENV{GN_RO_ENCRYPTIONSPACE_SUPPORT} eq "yes")
{
    print "ro.encryptionspace.enabled=true\n";
}

if (exists $ENV{GN_ZYT_NOCRYPT_MODE_SUPPORT} && $ENV{GN_ZYT_NOCRYPT_MODE_SUPPORT} eq "yes")
{
    print "ro.gn.zyt.nocrypt.mode.support=$ENV{GN_ZYT_NOCRYPT_MODE_SUPPORT}\n";
}
#Gionee chen_bin 2016-8-29 add for trustzone end


#Gionee <GN_ZYT_SUPPORT> <qudw> <20160311> modify for CR01650669 begin
if (exists $ENV{GN_ZYT_SUPPORT} && $ENV{GN_ZYT_SUPPORT} eq "yes")
{
    print "ro.gn.zyt.support=$ENV{GN_ZYT_SUPPORT}\n";
}

if (exists $ENV{GN_ZYT_SUPPORT} && $ENV{GN_ZYT_SUPPORT} eq "yes")
{
    print "persist.sys.zytstorekey=0\n";
}

if (exists $ENV{GN_ZYT_SUPPORT} && $ENV{GN_ZYT_SUPPORT} eq "yes")
{
    print "ro.gn.zyt.a3.upgrade.support=$ENV{GN_ZYT_SUPPORT}\n";
}

if (exists $ENV{GN_ZYT_CAM_PROTECT_SUPPORT} && $ENV{GN_ZYT_CAM_PROTECT_SUPPORT} eq "yes")
{
    print "ro.gn.zyt.cam.protect.support=$ENV{GN_ZYT_CAM_PROTECT_SUPPORT}\n";
}

if (exists $ENV{GN_ZYT_LOC_PROTECT_SUPPORT} && $ENV{GN_ZYT_LOC_PROTECT_SUPPORT} eq "yes")
{
    print "ro.gn.zyt.loc.protect.support=$ENV{GN_ZYT_LOC_PROTECT_SUPPORT}\n";
}

if (exists $ENV{GN_ZYT_REC_PROTECT_SUPPORT} && $ENV{GN_ZYT_REC_PROTECT_SUPPORT} eq "yes")
{
    print "ro.gn.zyt.rec.protect.support=$ENV{GN_ZYT_REC_PROTECT_SUPPORT}\n";
}

#Gionee <GN_ZYT_VOIP_PROTECT_SUPPORT> <huangrh> <20160520> modify for CR01703828 begin
if (exists $ENV{GN_ZYT_VOIP_PROTECT_SUPPORT} && $ENV{GN_ZYT_VOIP_PROTECT_SUPPORT} eq "yes")
{
    print "ro.gn.zyt.voip.protect.support=$ENV{GN_ZYT_VOIP_PROTECT_SUPPORT}\n";
}
#Gionee <GN_ZYT_VOIP_PROTECT_SUPPORT> <huangrh> <20160520> modify for CR01703828 end

if (exists $ENV{GN_ZYT_HW_ENC_SUPPORT} && $ENV{GN_ZYT_HW_ENC_SUPPORT} eq "yes")
{
    print "ro.gn.zyt.hw.enc.support=$ENV{GN_ZYT_HW_ENC_SUPPORT}\n";
}

if (exists $ENV{GN_ZYT_NOCRYPT_MODE_SUPPORT} && $ENV{GN_ZYT_NOCRYPT_MODE_SUPPORT} eq "yes")
{
    print "ro.gn.zyt.nocrypt.mode.support=$ENV{GN_ZYT_NOCRYPT_MODE_SUPPORT}\n";
}
#Gionee <GN_ZYT_SUPPORT> <qudw> <20160311> modify for CR01650669 end
#Gionee <xujinwen> <20160921> modify for the private space2.0 end

#Gionee <GN_HWFW_Fingerprint> zhangke add for 3147 begin
if(defined($ENV{GN_RO_KEYGUARD_SCREEN_OFF_FP_SUPPORT})) {
    print "ro.gn.keyguard.screen_off_fp=$ENV{GN_RO_KEYGUARD_SCREEN_OFF_FP_SUPPORT}\n";
}
#Gionee <GN_HWFW_Fingerprint> zhangke add for 3147 end

#Gionee hushengsong 2016-02-20 modify for CR01636440 begin
if (exists $ENV{GN_REMOVE_WECHAT_CLONE} && $ENV{GN_REMOVE_WECHAT_CLONE} eq "yes")
{
    print "ro.gn.remove.wechat.clone=$ENV{GN_REMOVE_WECHAT_CLONE}\n";
}
#Gionee hushengsong 2016-02-20 modify for CR01636440 end 

#Gionee <GN_SECURE_APP_BOOT_SCAN_SUPPORT> <puym> <20160328> modify for CR01662289 begin
if (exists $ENV{GN_SECURE_APP_BOOT_SCAN_SUPPORT} && $ENV{GN_SECURE_APP_BOOT_SCAN_SUPPORT} eq "yes")
{
    print "ro.gn.app.securescan.support=$ENV{GN_SECURE_APP_BOOT_SCAN_SUPPORT}\n";
}
#Gionee <GN_SECURE_APP_BOOT_SCAN_SUPPORT> <puym> <20160328> modify for CR01662289 end
#Gionee <GN_VENDOR_PACKAGE_VERIFIER_SUPPORT> <puyongming> <20160406> modify for CR01669758 begin
if (exists $ENV{GN_VENDOR_PACKAGE_VERIFIER_SUPPORT} && $ENV{GN_VENDOR_PACKAGE_VERIFIER_SUPPORT} eq "yes")
{
    print "ro.gn.app.vendverify.support=$ENV{GN_VENDOR_PACKAGE_VERIFIER_SUPPORT}\n";
}
#Gionee <GN_VENDOR_PACKAGE_VERIFIER_SUPPORT> <puyongming> <20160406> modify for CR01669758 end
#Gionee <GN_PRIVATE_PROVIDER_SUPPORT> <puym> <20160412> modify for CR01675072 begin
if (exists $ENV{GN_PRIVATE_PROVIDER_SUPPORT} && $ENV{GN_PRIVATE_PROVIDER_SUPPORT} eq "yes")
{
    print "ro.gn.app.priprovider.support=$ENV{GN_PRIVATE_PROVIDER_SUPPORT}\n";
}
#Gionee <GN_PRIVATE_PROVIDER_SUPPORT> <puym> <20160412> modify for CR01675072 end


# Gionee <GN_SECURE_PAY_SUPPORT> <puyongming> <20160529> modify for #14264 begin
if (exists $ENV{GN_SECURE_PAY_SUPPORT} && $ENV{GN_SECURE_PAY_SUPPORT} eq "yes")
{
    print "ro.gn.app.securepay.support=$ENV{GN_SECURE_PAY_SUPPORT}\n";
}
# Gionee <GN_SECURE_PAY_SUPPORT> <puyongming> <20160529> modify for #14264 end


#Gionee fuziqing 20160524 add for multiwindow begin
if (exists $ENV{GN_RO_MULTIWINDOW_SUPPORT} && $ENV{GN_RO_MULTIWINDOW_SUPPORT} eq "yes")
{
	print "ro.gn.multiwindow.support=true\n";
	print "ro.gn.ntfcenter.icon.support=false\n";
	print "ro.gn.headsup.icon.support=true\n";
}
#Gionee fuziqing 20160524 add for multiwindow end

#Gionee qudw 20160816 modify for ifaa fixed model name GNDCR #40547 begin
if (exists $ENV{GN_IFAA_MODEL_NAME_SUPPORT} && $ENV{GN_IFAA_MODEL_NAME_SUPPORT} ne "no")
{
    print "ro.gn.ifaa.model.name.support=$ENV{GN_IFAA_MODEL_NAME_SUPPORT}\n";
}
#Gionee qudw 20160816 modify for ifaa fixed model name GNDCR #40547 end

#Gionee yanhao 20161024 add for appclone begin
if (exists $ENV{GN_RO_APPCLONE_SUPPORT} && $ENV{GN_RO_APPCLONE_SUPPORT} eq "yes")
{
    print "ro.gn.amigo.appclone.support=yes\n";
}
#Gionee yanhao 20161024 add for appclone begin

#gionee gongshicheng 2016-10-28 add for gndcr42202 start

if(defined($ENV{GN_ACL_CABC_SUPPORT})) {
	print "ro.mtk.aal.support=$ENV{GN_ACL_CABC_SUPPORT}\n";
}
#gionee gongshicheng 2016-10-28 add for gndcr42202 end


#Gionee <Amigo_PrivateFingerprint> <huahy> <20160919> modify for GNDCR #48313 begin
if(defined($ENV{GN_PRIVATE_FINGERPRINT_SUPPORT})) {
	print "ro.gn.keyguard.screen_off_fp=true\n";
}
#Gionee <Amigo_PrivateFingerprint> <huahy> <20160919> modify for GNDCR #48313 end

#Gionee <bug> <liteng> <20161117> modify for GNDCR #55617  begin
if (exists $ENV{GN_RO_THEME_SUPPORT_V4} && $ENV{GN_RO_THEME_SUPPORT_V4} eq "yes")
{
    print "ro.gn.theme.support.v4=$ENV{GN_RO_THEME_SUPPORT_V4}\n";
}
#Gionee <bug> <liteng> <20161117> modify for GNDCR #55617  end

