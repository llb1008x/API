#!/bin/bash

#Gionee qudw 20131124 add for merge amigo build env begin
if [ "${BUILD_OPTIONS}" == "platform" -o "${GN_PLATFORM_DRIVER_ONLY_SUPPORT}" == "yes" ];then
    echo "*** platform or driveronly version, do not perform auto config below!"
else

#Gionee <Amigo_KeepApk> <lingfen> <20150131> add for CR01469614 begin	
if [ -f "system/core/rootdir/init.rc" ];then
    if ! `grep -wq "#Gionee lingfen add for CR00740221 start" system/core/rootdir/init.rc` ;then
        sed -i "/mkdir \/data\/app 0771 system system/a \ \t#Gionee lingfen add for CR00740221 start\n\tmkdir /data/amt 0777 shell system\n\t#Gionee lingfen add for CR00740221  end\n" system/core/rootdir/init.rc
    fi
    if ! `grep -wq "# Gionee <Amigo_Theme> <huming> <2012/03/27> modify for CR00556892 begin" system/core/rootdir/init.rc` ;then
        sed -i "/mkdir \/data\/misc 01771 system misc/a \ \t# Gionee <Amigo_Theme> <huming> <2012/03/27> modify for CR00556892 begin\n\tmkdir /data/misc/gionee 0777 root root\n\tmkdir /data/misc/gionee/theme 0777 root root\n\tmkdir /data/misc/gionee/theme/core 0777 root root\n\tmkdir /data/misc/gionee/lockpaper 0777 root root\n\tmkdir /data/misc/gionee/theme/dex/ 0777 root root\n\tmkdir /data/misc/gionee/theme/dex/core 0777 root root\n\tmkdir /data/misc/gionee/theme/Temp 0777 root root\n\tmkdir /data/misc/gionee/theme/zip 0777 root root\n\tmkdir /data/misc/gionee/recovery 0777 root root\n\t# Gionee <Amigo_Theme> <huming> <2012/03/27> modify for CR00556892 end\n" system/core/rootdir/init.rc
    fi
fi
#Gionee <Amigo_KeepApk> <lingfen> <20150131> add for CR01469614 end

if [ -f "system/core/rootdir/init.rc" ];then
    if ! `grep -wq "system/framework/qcnvitems.jar" system/core/rootdir/init.rc` ;then 
        sed -i 's@export BOOTCLASSPATH.*@&:/system/framework/qcnvitems.jar@' system/core/rootdir/init.rc
    fi
fi

if [ -f "system/core/rootdir/init.rc" ];then
    if ! `grep -wq "system/framework/qcrilhook.jar" system/core/rootdir/init.rc` ;then 
        sed -i 's@export BOOTCLASSPATH.*@&:/system/framework/qcrilhook.jar@' system/core/rootdir/init.rc
    fi
fi

#Gionee lingfen  configuration gnframework to init.rc file  start
if [ -f "system/core/rootdir/init.rc" ];then
    if ! `grep -wq "system/framework/gnframework.jar" system/core/rootdir/init.rc` ;then 
        sed -i 's@export BOOTCLASSPATH.*@&:/system/framework/gnframework.jar@' system/core/rootdir/init.rc
    fi
fi
#Gionee lingfen  configuration gnframework to init.rc file  end

#Gionee <lingfen> <2014-03-31> add for CR01160783 begin
if [ -f "system/core/rootdir/init.rc" -a "$GN_APK_AMIGO_SYSTEMMANAGER_SUPPORT" == "yes" ];then
    if ! `grep -wq "#Gionee <lingfen> <2014-03-31> add for CR01160783 begin" system/core/rootdir/init.rc` ;then
        sed -i "/^on post-fs$/a \ \t#Gionee <lingfen> <2014-03-31> add for CR01160783 begin\n\tmount ext4 /emmc@android /system noatime rw remount\n\tchown root root /system/xbin/amigosu\n\tchmod 4755 /system/xbin/amigosu\n\tchown root root /system/xbin/tunman\n\tchmod 0755 /system/xbin/tunman\n\tmount ext4 /emmc@android /system noatime ro remount\n\t#Gionee <lingfen> <2014-03-31> add for CR01160783 end\n" system/core/rootdir/init.rc
    fi
    #Gionee <lingfen> <2014-06-05> add for CR01275097 begin
    if ! `grep -wq "#Gionee <lingfen> <2014-06-05> add for CR01275097 begin" system/core/rootdir/init.rc` ;then
        sed -i '$a\\n#Gionee <lingfen> <2014-06-05> add for CR01275097 begin \nservice amigosu /system/xbin/amigosu --daemon\n\tclass main\n\tsocket amigosu stream 0666 root root\n\tseclabel u:r:dumpstate:s0\nservice tunman /system/xbin/tunman\n\tclass main\n\tsocket tunman stream 0666 system system\n\tseclabel u:r:tunman:s0\n#Gionee <lingfen> <2014-06-05> add for CR01275097 end' system/core/rootdir/init.rc
    fi
    #Gionee <lingfen> <2014-06-05> add for CR01275097 end
fi
#Gionee <lingfen> <2014-03-31> add for CR01160783 end


# Gionee <zhangxx> <2013-05-21> modify for CR00817680 begin
if [ -f "system/core/rootdir/init.rc" ];then
    if ! `grep -wq "system/framework/amigoframework.jar" system/core/rootdir/init.rc` ;then 
        sed -i 's@export BOOTCLASSPATH.*@&:/system/framework/amigoframework.jar@' system/core/rootdir/init.rc
    fi
fi
# Gionee <zhangxx> <2013-05-21> modify for CR00817680 end

# Gionee <hanbj> <2013-05-21> modify for CR00817680 begin
if [ -f "system/core/rootdir/init.rc" ];then
    if ! `grep -wq "system/framework/gionee-common.jar" system/core/rootdir/init.rc` ;then
        sed -i 's@export BOOTCLASSPATH.*@&:/system/framework/gionee-common.jar@' system/core/rootdir/init.rc
    fi
fi
fi
# Gionee <hanbj> <2013-05-21> modify for CR00817680 end
#Gionee qudw 20131124 add for merge amigo build env end


#Gionee lingfen 20120930 add for CR00705251 start
#Gionee lichao 20130822 modify for CR00859899 begin
#if [ "${GN_APP_CUSTOMERSERVICE_SUPPORT:0:3}" == "yes" ];then
#    version=${GN_APP_CUSTOMERSERVICE_SUPPORT:4}
if [ "${GN_APK_AMIGO_CUSTOMERSERVICE_SUPPORT:0:3}" == "yes" ];then
    version=${GN_APK_AMIGO_CUSTOMERSERVICE_SUPPORT:4}
    if [ -f "packages/apps/Amigo_CustomerService$version/assets/product_info.xml" ];then
        file="packages/apps/Amigo_CustomerService$version/assets/product_info.xml"

        if [ -n "$GN_PRODUCT_INFO_YEAR" ];then
            sed -i "s/2011年/$GN_PRODUCT_INFO_YEAR/"  $file
        fi

        if [ -n "$GN_PRODUCT_INFO_TYPE" ];then
            sed -i "s/3G手机，智能手机，拍照手机/$GN_PRODUCT_INFO_TYPE/"  $file
        fi

        if [ -n "$GN_PRODUCT_INFO_APPEARANCE" ];then
            sed -i "s/直板/$GN_PRODUCT_INFO_APPEARANCE/"  $file
        fi

        if [ -n "$GN_PRODUCT_INFO_SIZE" ];then
            sed -i "s/4.3英寸/$GN_PRODUCT_INFO_SIZE/"  $file
        fi

        if [ -n "$GN_PRODUCT_INFO_SCREEN" ];then
            sed -i "s/电容屏，多点触控/$GN_PRODUCT_INFO_SCREEN/" $file
        fi

        if [ -n "$GN_PRODUCT_INFO_MATERIAL" ];then
            sed -i "s/SUPER AMOLED PLUS/$GN_PRODUCT_INFO_MATERIAL/" $file
        fi

        if [ -n "$GN_PRODUCT_INFO_RESOLUTION" ];then
            sed -i "s/800 x 480像素/$GN_PRODUCT_INFO_RESOLUTION/" $file
        fi
        
        if [ -n "$GN_PRODUCT_INFO_COLOR" ];then
            sed -i "s/26万色/$GN_PRODUCT_INFO_COLOR/" $file
        fi

        if [ -n "$GN_PRODUCT_INFO_NETWORK_TYPE" ];then
            sed -i "s/双卡双模/$GN_PRODUCT_INFO_NETWORK_TYPE/" $file
        fi

        if [ -n "$GN_PRODUCT_INFO_NETWORK_MODE" ];then
            sed -i "s/GSM，WCDMA/$GN_PRODUCT_INFO_NETWORK_MODE/" $file
        fi

        if [ -n "$GN_PRODUCT_INFO_DATA_SERVICE" ];then
           sed -i "s/GPRS，EDGE，HSPA/$GN_PRODUCT_INFO_DATA_SERVICE/"  $file
        fi

        if [ -n "$GN_PRODUCT_INFO_DATA_FREQUENCY" ];then
            sed -i "s@2G，GSM850/900/1800/1900@$GN_PRODUCT_INFO_DATA_FREQUENCY@" $file
        fi
    fi

    if [ -d "packages/apps/GN_CustomerService$version/assets/" ];then
        while read dir
        do
            `dirname $0`/gn_auto_cfg_customservice.sh $dir
        done < <(find packages/apps/GN_CustomerService$version/assets/ -type d )
    fi
fi
#Gionee lichao 20130822 modify for CR00859899 end

#Gionee: <other> <wangmeng> <20140313> add for CR01113180 start
if [ "${GN_APK_AMIGO_SERVICE_SUPPORT:0:3}" == "yes" ];then
    if [ -f "packages/apps/Amigo_Service/assets/product_info.xml" ];then
        file="packages/apps/Amigo_Service/assets/product_info.xml"

        if [ -n "$GN_PRODUCT_INFO_YEAR" ];then
            sed -i "s/2011年/$GN_PRODUCT_INFO_YEAR/"  $file
        fi

        if [ -n "$GN_PRODUCT_INFO_TYPE" ];then
            sed -i "s/3G手机，智能手机，拍照手机/$GN_PRODUCT_INFO_TYPE/"  $file
        fi

        if [ -n "$GN_PRODUCT_INFO_APPEARANCE" ];then
            sed -i "s/直板/$GN_PRODUCT_INFO_APPEARANCE/"  $file
        fi

        if [ -n "$GN_PRODUCT_INFO_SIZE" ];then
            sed -i "s/4.3英寸/$GN_PRODUCT_INFO_SIZE/"  $file
        fi

        if [ -n "$GN_PRODUCT_INFO_SCREEN" ];then
            sed -i "s/电容屏，多点触控/$GN_PRODUCT_INFO_SCREEN/" $file
        fi

        if [ -n "$GN_PRODUCT_INFO_MATERIAL" ];then
            sed -i "s/SUPER AMOLED PLUS/$GN_PRODUCT_INFO_MATERIAL/" $file
        fi

        if [ -n "$GN_PRODUCT_INFO_RESOLUTION" ];then
            sed -i "s/800 x 480像素/$GN_PRODUCT_INFO_RESOLUTION/" $file
        fi
        
        if [ -n "$GN_PRODUCT_INFO_COLOR" ];then
            sed -i "s/26万色/$GN_PRODUCT_INFO_COLOR/" $file
        fi

        if [ -n "$GN_PRODUCT_INFO_NETWORK_TYPE" ];then
            sed -i "s/双卡双模/$GN_PRODUCT_INFO_NETWORK_TYPE/" $file
        fi

        if [ -n "$GN_PRODUCT_INFO_NETWORK_MODE" ];then
            sed -i "s/GSM，WCDMA/$GN_PRODUCT_INFO_NETWORK_MODE/" $file
        fi

        if [ -n "$GN_PRODUCT_INFO_DATA_SERVICE" ];then
           sed -i "s/GPRS，EDGE，HSPA/$GN_PRODUCT_INFO_DATA_SERVICE/"  $file
        fi

        if [ -n "$GN_PRODUCT_INFO_DATA_FREQUENCY" ];then
            sed -i "s@2G，GSM850/900/1800/1900@$GN_PRODUCT_INFO_DATA_FREQUENCY@" $file
        fi
    fi
fi
#Gionee: <other> <wangmeng> <20140313> add for CR01113180 end

#gionee lairufei add to produce Android.mk in packages/apps/GN_system_data/ begin
    system_data_dir="packages/apps/GN_system_data/"
    if [ -d "$system_data_dir" ]
    then
        list_file(){
            dir=$1
            find $dir \( -name "Android.mk" -o -path "*\.svn" \) -prune -o -type f -print|while read file 
        do
            echo -e "\t\$(LOCAL_PATH)/${file##$dir}:system/app/${file##$dir} \\"
        done
    }

    list=$(list_file $system_data_dir)
    if [ -n "$list" ]
    then
        cat > $system_data_dir/Android.mk <<EOF
LOCAL_PATH := \$(call my-dir)
PRODUCT_COPY_FILES += \\
$list
EOF
    fi
    fi
#gionee lairufei add to produce Android.mk in packages/apps/GN_system_data/ end

#Gionee lingfen 20120930 add for CR00705251 end
