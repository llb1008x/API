#!/bin/bash
#This flag's name was designed a little wired, because it won't like to be used by others
GN_COMPILE_TOOLS_PATH="gionee/config"
Usage()
{
	echo	"Usage: envsetup project"
	echo	"Projects:"
	echo	"  one of available projects."
}

if [ $# -lt 1 ]; then
  Usage
fi

GN_PROJECT=$1; export GN_PROJECT

read_product_config()
{
    if [ -e "${GN_COMPILE_TOOLS_PATH}/${GIONEEPRODUCTID}.mk" ]; then
        while read line
        do
            real_line=`echo $line | sed -s 's/ //g' | sed -s 's/#.*//g' | sed -s 's/:=/ /g'` # change ":=" to others rather than containing "="
            #echo real_line=${real_line}
            line_head=${real_line%=*}
            #echo line_head=$line_head
            tail_line=${real_line#*=}
            #echo tail_line=$tail_line
            if [ "$line_head" != "" ] && [ "$line_head" != "$real_line" ] && [ "$tail_line" != ""  ]; then
                export $line_head=$tail_line
                #echo $line_head=$tail_line
            fi
        done < ${GN_COMPILE_TOOLS_PATH}/${GIONEEPRODUCTID}.mk
    else
        echo "*** ${GN_COMPILE_TOOLS_PATH}/${GIONEEPRODUCTID}.mk NOT Exists ***";
    fi
}

read_product_config;

. build/envsetup.sh
setpaths

#Gionee:huangsf 20120209 add for build variant setting start
if [ -z "$TARGET_BUILD_VARIANT" ]; then
#choosecombo 1 msm8974 3
choosecombo 1 msm8226 3
else
    local index=1
    local v
    for v in ${VARIANT_CHOICES[@]}
    do
        if [ "$v" = "$TARGET_BUILD_VARIANT" ]; then
            break
        else
            index=$(($index+1))
        fi
    done
    #choosecombo 1 msm8974 $index
    choosecombo 1 msm8226 $index
fi
#Gionee:huangsf 20120209 add for build variant setting end

if [ -e ${GN_COMPILE_TOOLS_PATH}/${GIONEEPRODUCTID}.mk ]; then
    #exit 1;
    gn_cdefs=""
    gn_kernel_cdefs=""
    GN_KERNEL_CFG_FILE=`grep 'GN_KERNEL_CFG_FILE' ${GN_COMPILE_TOOLS_PATH}/${GIONEEPRODUCTID}.mk | sed 's/ //g' | awk -F= '{print $2}'`
    GN_KERNEL_TMP_FILE=`grep 'GN_KERNEL_TMP_FILE' ${GN_COMPILE_TOOLS_PATH}/${GIONEEPRODUCTID}.mk | sed 's/ //g' | awk -F= '{print $2}'`
    if [ -e "$GN_KERNEL_TMP_FILE" -a `expr length "$GN_KERNEL_CFG_FILE"` -gt 0 -a -e "$GN_KERNEL_CFG_FILE" ];then
        while read LINE; do
            LINE=`echo $LINE | sed -s 's/ //g' | sed -s 's/#.*//g'`
            FUNV=(`echo $LINE | awk -F= '{for(i=1;i<=NF;i++)print $i }'`)
            if [ ${#FUNV[*]} -lt 2 ];then
                continue
            fi
            if [ "${FUNV[1]}" = "n" ];then
                testv=`grep "^CONFIG_${FUNV[0]}=" $GN_KERNEL_CFG_FILE `
                if [ `expr length "$testv"` -gt 0 ];then
                    rstr=`sed -i "s/CONFIG_${FUNV[0]}=.*/#CONFIG_${FUNV[0]} is not set/g" $GN_KERNEL_CFG_FILE`
                fi
            else
                testv=`grep "\bCONFIG_${FUNV[0]}\b" $GN_KERNEL_CFG_FILE `
                if [ `expr length "$testv"` -gt 0 ];then
                    if [ ! "`grep \"CONFIG_${FUNV[0]}\b\" $GN_KERNEL_CFG_FILE `" = "$LINE" ];then
                        rstr=`sed -i "s/$testv/CONFIG_$LINE/g" $GN_KERNEL_CFG_FILE`
                    fi
                else
#           echo CONFIG_$LINE >> $GN_KERNEL_CFG_FILE
                    if [ "${FUNV[1]}" = "y" ];then
                        gn_kernel_cdefs="$gn_kernel_cdefs -DCONFIG_${FUNV[0]}"
                    else
                        gn_kernel_cdefs="$gn_kernel_cdefs -DCONFIG_$LINE"
                    fi
                fi
                if [ "${FUNV[1]}" = "y" ];then
                    gn_cdefs="$gn_cdefs -DCONFIG_${FUNV[0]}"
                else
                    gn_cdefs="$gn_cdefs -DCONFIG_$LINE"
                fi
            fi
        done < $GN_KERNEL_TMP_FILE
    fi
    
    #Gionee lingfen add for CR00526442 start 
    if [ ${GN_3RD_RECOVER_FROM_SDCARD} == "y" ];then
        gn_cdefs="${gn_cdefs} -DCONFIG_GN_3RD_RECOVER_FROM_SDCARD"
    fi
    if [ ${GN_3RD_RECOVER_FROM_RAM} == "y" ];then
        gn_cdefs="${gn_cdefs} -DCONFIG_GN_3RD_RECOVER_FROM_RAM"
    fi
    #Gionee lingfen add for CR00526442 start 

    #Gionee <cheny> <20131026> add for CR00934000 start 
    if [ ${GN_FACTORY_RESET_KEEP_APK} == "y" ];then
        gn_cdefs="${gn_cdefs} -DCONFIG_GN_FACTORY_RESET_KEEP_APK"
    fi
    #Gionee <cheny> <20131026> add for CR00934000 end

    GN_BOOTABLE_TMP_FILE=`grep 'GN_BOOTABLE_TMP_FILE' ${GN_COMPILE_TOOLS_PATH}/${GIONEEPRODUCTID}.mk | sed 's/ //g' | awk -F= '{print $2}'`
    export GN_BOOTABLE_TMP_FILE
    if [ `expr length "$GN_BOOTABLE_TMP_FILE"` -gt 0 ];then
        echo GN_CFLAGS := $gn_cdefs > $GN_BOOTABLE_TMP_FILE
    fi

    GN_KCONFIG_TMP_FILE=`grep 'GN_KCONFIG_TMP_FILE' ${GN_COMPILE_TOOLS_PATH}/${GIONEEPRODUCTID}.mk | sed 's/ //g' | awk -F= '{print $2}'`
    export GN_KCONFIG_TMP_FILE
    if [ `expr length "$GN_KCONFIG_TMP_FILE"` -gt 0 ];then
        echo GN_CFLAGS := $gn_kernel_cdefs > $GN_KCONFIG_TMP_FILE
    fi
fi
