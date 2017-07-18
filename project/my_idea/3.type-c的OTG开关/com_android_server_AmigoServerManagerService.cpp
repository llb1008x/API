/** 
 *File Description: This is implementation of AmigoServerManagerService JNI Interface
 *Author: huqh
 *Create Date:20141228
 *Change List:
 */
 
 
#define LOG_TAG "AmigoServerManagerService"

#include "JNIHelp.h"
#include "jni.h"
#include <utils/Log.h>
#include <utils/misc.h>

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/timerfd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <linux/ioctl.h>
#include <linux/android_alarm.h>
#include <linux/rtc.h>

#include <android/log.h>  

namespace android {

//define sysfile nodes
static const char g_NodeFiles[][96] = 
{
"/sys/class/leds/amoled_lcm_acl/brightness",
"/sys/class/leds/amoled_lcm_partical/brightness",
"/sys/class/leds/amoled_lcm_hbm/brightness",
"/sys/class/leds/red/brightness",
"/sys/class/leds/blue/brightness",
"/sys/class/leds/green/brightness",
"/sys/class/leds/button-backlight/brightness",
"/sys/class/flashlightdrv/kd_camera_flashlight/torch",
"/sys/bus/platform/devices/tp_wake_switch/double_wake",
"/sys/bus/platform/devices/tp_wake_switch/glove_enable",
"/sys/bus/platform/devices/tp_wake_switch/gesture_wake",
"/sys/bus/platform/devices/tp_wake_switch/gesture_config",
"/sys/bus/platform/devices/tp_wake_switch/factory_check",
"/sys/bus/platform/drivers/gsensor/cali",
"/sys/bus/platform/drivers/als_ps/high_threshold",
"/sys/bus/platform/drivers/als_ps/low_threshold",
"/sys/bus/platform/drivers/als_ps/pdata",
"/sys/class/misc/m_alsps_misc/handanswer_status",
"/sys/class/input/inputx/batch",
"/sys/class/input/inputx/delay_ms",
"/sys/class/input/inputx/enable",
"/sys/class/input/inputx/flush",
"/sys/devices/platform/battery/power_supply/battery/BatteryAverageCurrent",
"/sys/devices/platform/battery/power_supply/battery/ChargerVoltage",
"/sys/devices/platform/battery/power_supply/battery/batt_temp",
"/sys/devices/platform/battery/power_supply/battery/batt_vol",
"/sys/devices/platform/battery/power_supply/battery/capacity",
"/sys/devices/platform/battery/power_supply/battery/health",
"/sys/devices/platform/battery/power_supply/battery/technology",
"/sys/devices/platform/mt-battery/BatteryNotify",
"/sys/class/timed_output/vibrator/enable",
"/sys/class/timed_output/vibrator/vibr_on",
"/sys/class/timed_output/vibrator/pattern",
"/sys/devices/virtual/switch/gn_hall_key/state",
"/sys/bus/platform/drivers/Accdet_Driver/set_headset_mode",
"/sys/devices/platform/gn_device_check",
"/sys/bus/platform/devices/tp_wake_switch/gesture_coordition",
"/sys/devices/platform/gn_device_check/name",
"/sys/devices/virtual/misc/x_misc/x_misc/status",
"/sys/class/flashlightdrv/kd_camera_flashlight/torch0",
"/sys/class/flashlightdrv/kd_camera_flashlight/torch1",
//Gionee <Amigo_Skylight> wangym <20150409> add for  CR01462639 begin
"/sys/devices/platform/gn_hall_key/state",
//Gionee <Amigo_Skylight> wangym <20150409> add for  CR01462639 end
//Gionee <BUG> wangym <20150530> add for  CR01492068 begin
"/sys/devices/platform/battery/Charging_CallState",
//Gionee <BUG> wangym <20150530> add for  CR01492068 end
//Gionee <fingerprint> <qudw> <20150528> modify for CR01490911
"/sys/bus/spi/devices/spi0.0/diag/nav_en",
//Gionee <qudw> <fingerprint> <20150608> add for CR01498174 begin
"/sys/bus/platform/devices/gn_fpc1020/setup/dst_pid",
"/sys/bus/platform/devices/gn_fpc1020/setup/dst_signo",
"/sys/bus/platform/devices/gn_fpc1020/setup/enabled",
//Gionee <qudw> <fingerprint> <20150608> add for CR01498174 end
//Gionee <qudw> <fingerprint> <20150709> add for CR01516884 begin
"/sys/bus/platform/devices/gn_fpc1020/pm/hw_reset",
"/sys/bus/platform/devices/gn_fpc1020/pm/notify_ack",
"/sys/bus/platform/devices/gn_fpc1020/pm/notify_enabled",
"/sys/bus/platform/devices/gn_fpc1020/pm/state",
"/sys/bus/platform/devices/gn_fpc1020/pm/supply_on",
"/sys/bus/platform/devices/gn_fpc1020/pm/wakeup_req",
//Gionee <qudw> <fingerprint> <20150709> add for CR01516884 end
//Gionee <wangh> <fingerprint> <20150725> add for CR01467156 begin
"/sys/bus/platform/devices/gn_fpc1020/spiclk/enable",
"/sys/bus/platform/devices/gn_fpc1020/fingerid/hwid",

//Gionee <wangh> <fingerprint> <20150725> add for CR01467156 end
//Gionee <GN_BSP_USB> <lilubao> <20170316> add for OTG SWITCH begin
"/otg/otg_mode",
//Gionee <GN_BSP_USB> <lilubao> <20170316> add for OTG SWITCH end
""
};

//define node type of sysfile
enum {
NODE_TYPE_LCM_ACL_BRIGHTNESS,
NODE_TYPE_LCM_PARTICAL_BRIGHTNESS,
NODE_TYPE_LCM_HBM_BRIGHTNESS,
NODE_TYPE_LED_RED_BRIGHTNESS,
NODE_TYPE_LED_BLUE_BRIGHTNESS,
NODE_TYPE_LED_GREEN_BRIGHTNESS,
NODE_TYPE_LED_BUTTONBACKLIGHT_BRIGHTNESS,
NODE_TYPE_FLASHLIGHT_CAMERA_TORCH,
NODE_TYPE_TPWAKESWITCH_DOUBLE_WAKE,
NODE_TYPE_TPWAKESWITCH_GLOVE_ENABLE,
NODE_TYPE_TPWAKESWITCH_GESTURE_WAKE,
NODE_TYPE_TPWAKESWITCH_GESTURE_CONFIG,
NODE_TYPE_TPWAKESWITCH_FACTORY_CHECK,
NODE_TYPE_GSENSRO_CALI,
NODE_TYPE_ALSPS_HIGH_THRESHOLD,
NODE_TYPE_ALSPS_LOW_THRESHOLD,
NODE_TYPE_ALSPS_PDATA,
NODE_TYPE_ALSPS_INCALL_CALI,
NODE_TYPE_INPUTX_BATCH,
NODE_TYPE_INPUTX_DELAYMS,
NODE_TYPE_INPUTX_ENABLE,
NODE_TYPE_INPUTX_FLUSH,
NODE_TYPE_BATTERY_AVERAGE_CURRENT,
NODE_TYPE_BATTERY_CHARGE_VOLTAGE,
NODE_TYPE_BATTERY_BATT_TEMP,
NODE_TYPE_BATTERY_BATT_VOL,
NODE_TYPE_BATTERY_CAPACITY,
NODE_TYPE_BATTERY_HEALTH,
NODE_TYPE_BATTERY_TECHNOLOGY,
NODE_TYPE_BATTERY_NOTIFY,
NODE_TYPE_VIBRATOR_ENABLE,
NODE_TYPE_VIBRATOR_VIBR_ON,
NODE_TYPE_VIBRATOR_PATTERN,
NODE_TYPE_HALL_SWITCH_STATE,
NODE_TYPE_ACCDET_HEADSET_MODE,
NODE_TYPE_GN_DEVICE_CHECK,
NODE_TYPE_TPWAKESWITCH_GESTURE_CONTENT,
NODE_TYPE_GN_DEVICE_GET,
NODE_TYPE_MISC_STATUS_GET,
NODE_TYPE_FLASHLIGHT_CAMERA_TORCH_0,
NODE_TYPE_FLASHLIGHT_CAMERA_TORCH_1,
//Gionee <Amigo_Skylight> wangym <20150409> add for  CR01462639 begin
NODE_TYPE_HALL_SWITCH_ON,
//Gionee <Amigo_Skylight> wangym <20150409> add for  CR01462639 end
//Gionee <BUG> wangym <20150530> add for  CR01492068 begin
NODE_TYPE_CALL_CHARGING_SWITCH_ON,
//Gionee <BUG> wangym <20150530> add for  CR01492068 end
//Gionee <fingerprint> <qudw> <20150528> modify for CR01490911
NODE_TYPE_FINGERPRINT_INPUTMODE_SWITCH,
//Gionee <qudw> <fingerprint> <20150608> add for CR01498174 begin
NODE_TYPE_FINGERPRINT_INTERRUPT_SETUP_DST_PID,
NODE_TYPE_FINGERPRINT_INTERRUPT_SETUP_DST_SIGNO,
NODE_TYPE_FINGERPRINT_INTERRUPT_SETUP_ENABLED,
//Gionee <qudw> <fingerprint> <20150608> add for CR01498174 end
//Gionee <qudw> <fingerprint> <20150709> add for CR01516884 begin
NODE_TYPE_FINGERPRINT_INTERRUPT_PM_HW_RESET,
NODE_TYPE_FINGERPRINT_INTERRUPT_PM_NOTIFY_ACK,
NODE_TYPE_FINGERPRINT_INTERRUPT_PM_NOTIFY_ENABLED,
NODE_TYPE_FINGERPRINT_INTERRUPT_PM_STATE,
NODE_TYPE_FINGERPRINT_INTERRUPT_PM_SUPPLY_ON,
NODE_TYPE_FINGERPRINT_INTERRUPT_PM_WAKEUP_REQ,
//Gionee <qudw> <fingerprint> <20150709> add for CR01516884 end
//Gionee <wangh> <fingerprint> <20150725> add for CR01467156 begin
NODE_TYPE_FINGERPRINT_INTERRUPT_SPICLK_ENABLE,
NODE_TYPE_FINGERPRINT_INTERRUPT_FINGER_HWID,
//Gionee <wangh> <fingerprint> <20150725> add for CR01467156 end
//Gionee <Amigo_UI> <wangym> <20161122> add for 31213 begin
NODE_TYPE_OTG_CHARGE_SWITCH,
//Gionee <Amigo_UI> <wangym> <20161122> add for 31213 end
NODE_TYPE_NUM
};


#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "keymatch JNI", __VA_ARGS__)

//to write the sysnode file
static void android_server_AmigoServerManagerService_setNodeState(JNIEnv* env, jobject obj, jint type, jint value)
{
    int fd = 0;
    char buf[16] = {0};
	
	  if (type >= NODE_TYPE_NUM)
	  {
	      ALOGD("huqh invalid sysnode type\n");
	      return;
	  }
	  
    fd = open(g_NodeFiles[type], O_RDWR);
	  if (fd < 0)
	  {
	      ALOGD("huqh open file for O_RDWR failed \n");
	      return;
    }

    sprintf(buf, "%d", value);
	  write(fd, buf, strlen(buf));
	  close(fd);
	
    return;
}

//to read the sysnode file
static jint android_server_AmigoServerManagerService_getNodeState(JNIEnv* env, jobject obj, jint type)
{
    int fd = 0;
    char buf[16] = {0};
	
	  if (type >= NODE_TYPE_NUM)
	  {
	      ALOGD("huqh invalid sysnode type\n");
	      return -1;
	  }
	  
    fd = open(g_NodeFiles[type], O_RDONLY);
    if (fd < 0)
	  {
	      ALOGD("huqh open file for O_RDONLY failed \n");
	      return -1;
	  }
	
    read(fd, &buf, sizeof(buf) - 1);
	  close(fd);
	
    return atoi(buf);
}

static jstring android_server_AmigoServerManagerService_getNodeContent(JNIEnv* env, jobject obj, jint type)
{
   FILE *file = NULL;
   char buffer[512 +1];

   LOGD("getNodeContent  type=%d,path=%s",type,g_NodeFiles[type]);
   
   file = fopen( g_NodeFiles[type], "r");

   if (file == NULL)
   {
        LOGD("getNodeContent open file failed\n");;
        return NULL;
   }
   
   fseek(file,0,SEEK_END);
   int nLen = ftell(file);       
   char message[nLen];

   memset(buffer,0,sizeof(buffer));
   memset(message,0,sizeof(message));

   fseek(file,0,SEEK_SET);   
	
   while (fgets(buffer, sizeof(buffer), file)) {
      sprintf(message,"%s%s",message,buffer);
      memset(buffer,0,sizeof(buffer));
    }
  
    fclose(file);
	
    return env->NewStringUTF(message);

}

//JNI Methods
static JNINativeMethod sMethods[] = {
     /* name, signature, funcPtr */
    {"nativeSetNodeState", "(II)V", (void*)android_server_AmigoServerManagerService_setNodeState},
    {"nativeGetNodeState", "(I)I", (void*)android_server_AmigoServerManagerService_getNodeState},
    {"nativeGetNodeContent", "(I)Ljava/lang/String;", (void*)android_server_AmigoServerManagerService_getNodeContent},
};

//register JNI Methods
int register_android_server_AmigoServerManagerService(JNIEnv* env)
{
    return jniRegisterNativeMethods(env, "com/android/server/AmigoServerManagerService",
                                    sMethods, NELEM(sMethods));
}

};


