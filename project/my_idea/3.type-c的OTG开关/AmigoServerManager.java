//Gionee <Amigo_Server> wangym 20141225 add for AmigoServer CR01431164 begin
/*
 * Copyright (C) 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package android.os.amigoserver;

import android.content.Context;
import android.os.RemoteException;
import android.os.ServiceManager;
import android.util.Log;

/**
 * Class that operates consumer infrared on the device.
 *
 * <p>
 * To obtain an instance of the system infrared transmitter, call
 * {link android.content.Context#getSystemService(java.lang.String)
 * Context.getSystemService()} with
 * {link android.content.Context#AMIGO_SYSTEM_SERVICE} as the argument.
 * </p>
 */
public final class AmigoServerManager  {
    private static final String TAG = "AmigoServerManager";
    private final IAmigoServerManager mService;
    public static final int  NODE_TYPE_LCM_ACL_BRIGHTNESS = 0;   //"/sys/class/leds/amoled_lcm_acl/brightness"
    public static final int  NODE_TYPE_LCM_PARTICAL_BRIGHTNESS =1 ;  //"/sys/class/leds/amoled_lcm_partical/brightness"
    public static final int  NODE_TYPE_LCM_HBM_BRIGHTNESS =2;    //"/sys/class/leds/amoled_lcm_hbm/brightness",
    public static final int  NODE_TYPE_LED_RED_BRIGHTNESS=3; //"/sys/class/leds/red/brightness"
    public static final int  NODE_TYPE_LED_BLUE_BRIGHTNESS=4;    //"/sys/class/leds/blue/brightness"
    public static final int  NODE_TYPE_LED_GREEN_BRIGHTNESS=5;   ///sys/class/leds/green/brightness
    public static final int  NODE_TYPE_LED_BUTTONBACKLIGHT_BRIGHTNESS=6; ///sys/class/leds/button-backlight/brightness
    public static final int  NODE_TYPE_FLASHLIGHT_CAMERA_TORCH=7; ////sys/class/flashlightdrv/kd_camera_flashlight/torch
    public static final int  NODE_TYPE_TPWAKESWITCH_DOUBLE_WAKE=8;   //"/sys/bus/platform/devices/tp_wake_switch/double_wake
    public static final int  NODE_TYPE_TPWAKESWITCH_GLOVE_ENABLE=9;  ///sys/bus/platform/devices/tp_wake_switch/glove_enable
    public static final int  NODE_TYPE_TPWAKESWITCH_GESTURE_WAKE=10; ///sys/bus/platform/devices/tp_wake_switch/gesture_wake
    public static final int  NODE_TYPE_TPWAKESWITCH_GESTURE_CONFIG=11; ///sys/bus/platform/devices/tp_wake_switch/gesture_config
    public static final int  NODE_TYPE_TPWAKESWITCH_FACTORY_CHECK=12;    ///sys/bus/platform/devices/tp_wake_switch/factory_check
    public static final int  NODE_TYPE_GSENSRO_CALI=13;  ///sys/bus/platform/drivers/gsensor/cali
    public static final int  NODE_TYPE_ALSPS_HIGH_THRESHOLD=14; ///sys/bus/platform/drivers/als_ps/high_threshold
    public static final int  NODE_TYPE_ALSPS_LOW_THRESHOLD=15;   ///sys/bus/platform/drivers/als_ps/high_threshold
    public static final int  NODE_TYPE_ALSPS_PDATA=16;   ///sys/bus/platform/drivers/als_ps/low_threshold
    public static final int  NODE_TYPE_ALSPS_INCALL_CALI=17; ///sys/class/misc/m_alsps_misc/handanswer_status
    public static final int  NODE_TYPE_INPUTX_BATCH=18;  ///sys/class/input/inputx/batch
    public static final int  NODE_TYPE_INPUTX_DELAYMS=19;    ///sys/class/input/inputx/delay_ms
    public static final int  NODE_TYPE_INPUTX_ENABLE=20; ///sys/class/input/inputx/enable
    public static final int  NODE_TYPE_INPUTX_FLUSH=21;  ///sys/class/input/inputx/flush
    public static final int  NODE_TYPE_BATTERY_AVERAGE_CURRENT=22;   ///sys/devices/platform/battery/power_supply/battery/BatteryAverageCurrent
    public static final int  NODE_TYPE_BATTERY_CHARGE_VOLTAGE=23;    ///sys/devices/platform/battery/power_supply/battery/ChargerVoltage
    public static final int  NODE_TYPE_BATTERY_BATT_TEMP=24; ///sys/devices/platform/battery/power_supply/battery/batt_temp
    public static final int  NODE_TYPE_BATTERY_BATT_VOL=25;  ///sys/devices/platform/battery/power_supply/battery/batt_vol
    public static final int  NODE_TYPE_BATTERY_CAPACITY=26;  ///sys/devices/platform/battery/power_supply/battery/capacity
    public static final int  NODE_TYPE_BATTERY_HEALTH=27;    ///sys/devices/platform/battery/power_supply/battery/health
    public static final int  NODE_TYPE_BATTERY_TECHNOLOGY=28;    ///sys/devices/platform/battery/power_supply/battery/technology
    public static final int  NODE_TYPE_BATTERY_NOTIFY=29;    ///sys/devices/platform/mt-battery/BatteryNotify
    public static final int  NODE_TYPE_VIBRATOR_ENABLE=30;   ///sys/class/timed_output/vibrator/enable
    public static final int  NODE_TYPE_VIBRATOR_VIBR_ON=31;  ///sys/class/timed_output/vibrator/vibr_on
    public static final int  NODE_TYPE_VIBRATOR_PATTERN=32;  ///sys/class/timed_output/vibrator/pattern
    public static final int  NODE_TYPE_HALL_SWITCH_STATE=33; ///sys/devices/virtual/switch/gn_hall_key/state
    public static final int  NODE_TYPE_ACCDET_HEADSET_MODE=34;   ///sys/bus/platform/drivers/Accdet_Driver/set_headset_mode
    public static final int  NODE_TYPE_GN_DEVICE_CHECK=35;   ///sys/devices/platform/gn_device_check

    public static final int  NODE_TYPE_TPWAKESWITCH_GESTURE_CONTENT = 36;///sys/bus/platform/devices/tp_wake_switch/gesture_coordition
    public static final int  NODE_TYPE_GN_DEVICE_GET = 37;  ///sys/devices/platform/gn_device_check/name
    public static final int  NODE_TYPE_MISC_STATUS_GET = 38; ///sys/devices/virtual/misc/x_misc/x_misc/status
    public static final int  NODE_TYPE_FLASHLIGHT_CAMERA_TORCH_0 = 39; ///sys/class/flashlightdrv/kd_camera_flashlight/torch0
    public static final int  NODE_TYPE_FLASHLIGHT_CAMERA_TORCH_1 = 40; ///sys/class/flashlightdrv/kd_camera_flashlight/torch1
    //Gionee <Amigo_Skylight> wangym <20150409> add for  CR01462639 begin
    public static final int  NODE_TYPE_HALL_SWITCH_ON = 41;///sys/devices/platform/gn_hall_key/state
     //Gionee <Amigo_Skylight> wangym <20150409> add for  CR01462639 end
     //Gionee <BUG> wangym <20150530> add for  CR01492068 begin
    public static final int  NODE_TYPE_CALL_CHARGING_SWITCH_ON = 42;///sys/devices/platform/battery/Charging_CallState
     //Gionee <BUG> wangym <20150530> add for  CR01492068 end
    //Gionee <fingerprint> <qudw> <20150528> modify for CR01490911
    public static final int NODE_TYPE_FINGERPRINT_INPUTMODE_SWITCH = 43; ///sys/bus/spi/devices/spi0.0/diag/nav_en
    //Gionee <qudw> <fingerprint> <20150608> add for CR01498174 begin
    public static final int NODE_TYPE_FINGERPRINT_INTERRUPT_SETUP_DST_PID = 44; ///sys/bus/platform/devices/gn_fpc1020/setup/dst_pid
    public static final int NODE_TYPE_FINGERPRINT_INTERRUPT_SETUP_DST_SIGNO = 45; ///sys/bus/platform/devices/gn_fpc1020/setup/dst_signo
    public static final int NODE_TYPE_FINGERPRINT_INTERRUPT_SETUP_ENABLED= 46; ///sys/bus/platform/devices/gn_fpc1020/setup/enabled
    //Gionee <qudw> <fingerprint> <20150608> add for CR01498174 end

    //Gionee <qudw> <fingerprint> <20150709> add for CR01516884 begin
    public static final int NODE_TYPE_FINGERPRINT_INTERRUPT_PM_HW_RESET = 47; ///sys/bus/platform/devices/gn_fpc1020/pm/hw_reset
    public static final int NODE_TYPE_FINGERPRINT_INTERRUPT_PM_NOTIFY_ACK = 48; ///sys/bus/platform/devices/gn_fpc1020/pm/notify_ack
    public static final int NODE_TYPE_FINGERPRINT_INTERRUPT_PM_NOTIFY_ENABLED = 49; ///sys/bus/platform/devices/gn_fpc1020/pm/notify_enabled
    public static final int NODE_TYPE_FINGERPRINT_INTERRUPT_PM_STATE = 50; ///sys/bus/platform/devices/gn_fpc1020/pm/state
    public static final int NODE_TYPE_FINGERPRINT_INTERRUPT_PM_SUPPLY_ON = 51; ///sys/bus/platform/devices/gn_fpc1020/pm/supply_on
    public static final int NODE_TYPE_FINGERPRINT_INTERRUPT_PM_WAKEUP_REQ = 52; ///sys/bus/platform/devices/gn_fpc1020/pm/wakeup_req
    //Gionee <qudw> <fingerprint> <20150709> add for CR01516884 end

    //Gionee <wangh> <fingerprint> <20150725> add for CR01467156 begin
    public static final int NODE_TYPE_FINGERPRINT_INTERRUPT_SPICLK_ENABLE = 53; ///sys/bus/platform/devices/gn_fpc1020/spiclk/enable
    public static final int NODE_TYPE_FINGERPRINT_INTERRUPT_FINGERID_HWID = 54; ///sys/bus/platform/devices/gn_fpc1020/fingerid/hwid
    //Gionee <wangh> <fingerprint> <20150725> add for CR01467156 end

     //Gionee <GN_BSP_USB> <lilubao> <20170316> add for OTG SWITCH begin
    public static final int NODE_TYPE_OTG_CHARGE_SWITCH = 55;							///otg/otg_mod
    //Gionee <GN_BSP_USB> <lilubao> <20170316> add for OTG SWITCH end
    /**
     * hide to prevent subclassing from outside of the framework
     */
    public AmigoServerManager(Context context) {
        mService = IAmigoServerManager.Stub.asInterface(
                ServiceManager.getService("amigoserver"));
    }
    public void SetNodeState(int NodeType,int mValue) {
        // TODO Auto-generated method stub
        try{				
             mService.setNodeState(NodeType,mValue);
        }catch (RemoteException e){
        }
    }
    public int GetNodeState(int NodeType)  {
        try{				
            return mService.getNodeState(NodeType);
        }catch (RemoteException e){
            return (-1);
        }
    }
    
    public String getNodeContent(int NodeType) {
        try {
            return mService.getNodeContent(NodeType);
        } catch (RemoteException e) {
            return null;
        }
    }
}
//Gionee <Amigo_Server> wangym 20141225 add for AmigoServer CR01431164 end