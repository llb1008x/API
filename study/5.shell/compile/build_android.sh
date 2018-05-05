#!/bin/bash

CPU_JOB_NUM=$(grep processor /proc/cpuinfo | awk '{field=$NF};END{print field+1}')
CLIENT=$(whoami)

ROOT_DIR=$(pwd)
KERNEL_DIR="$ROOT_DIR/../iTop4412_Kernel_3.0"
export KERNEL_DIR

#SEC_PRODUCT='generic' #Enable for generic build
SEC_PRODUCT=smdk4x12
WIFI_CROSS_COMPILER=/usr/local/arm/arm-2009q3/bin/arm-none-linux-gnueabi-
export WIFI_CROSS_COMPILER 

OUT_DIR="$ROOT_DIR/out/target/product/$SEC_PRODUCT"
OUT_HOSTBIN_DIR="$ROOT_DIR/out/host/linux-x86/bin"

option="recovery"

function check_exit()
{
	if [ $? != 0 ]
	then
		exit $?
	fi
}
# SEMCO wifi module build
function build_wifi()
{
	echo
	echo '[[[[[[[ Build wifi ]]]]]]]'
	echo

	cd $ROOT_DIR/compat-wireless
	make
  cd ..
}

function build_android()
{
	echo
	echo '[[[[[[[ Build android platform ]]]]]]]'
	echo

	START_TIME=`date +%s`
	if [ $SEC_PRODUCT = "generic" ]
	then
		echo make -j$CPU_JOB_NUM
		echo
		make -j$CPU_JOB_NUM
	else
		echo make -j$CPU_JOB_NUM PRODUCT-full_$SEC_PRODUCT-eng
		echo
		make -j$CPU_JOB_NUM PRODUCT-full_$SEC_PRODUCT-eng
	fi
	check_exit

	END_TIME=`date +%s`
	let "ELAPSED_TIME=$END_TIME-$START_TIME"
	echo "Total compile time is $ELAPSED_TIME seconds"
}

function make_uboot_img()
{
	cd $OUT_DIR

	echo
	echo '[[[[[[[ Make ramdisk image for u-boot ]]]]]]]'
	echo

	mkimage -A arm -O linux -T ramdisk -C none -a 0x40800000 -n "ramdisk" -d ramdisk.img ramdisk-uboot.img
	check_exit
	rm -f ramdisk.img

	echo
	cd ../../../..
}
#----------NOTICE--------------------------------------------------------
#before compile the recovery image, please setting the option likes below
#	TARGET_NO_KERNEL := false
#in file BoardConfig.mk
#------------------------------------------------------------------------
function make_recovery_img()
{
	echo "MAKE RECOVERY IMAGE----"
	chmod 777 build/tools/releasetools/ota_from_target_files
	#mkimage -A arm -O linux -T ramdisk -C none -a 0x40800000 -n "ramdisk" -d ramdisk-recovery.img ramdisk-recovery-uboot.img
	make otapackage TARGET_PRODUCT=full_smdk4x12 #mj
}
function make_fastboot_img()
{
	echo
	echo '[[[[[[[ Make additional images for fastboot ]]]]]]]'
	echo

	if [ ! -f $KERNEL_DIR/arch/arm/boot/zImage ]
	then
		echo "No zImage is found at $KERNEL_DIR/arch/arm/boot"
		echo '  Please set KERNEL_DIR if you want to make additional images'
		echo "  Ex.) export KERNEL_DIR=~ID/android_kernel_$SEC_PRODUCT"
		echo
		return
	fi

	echo 'boot.img ->' $OUT_DIR
	cp $KERNEL_DIR/arch/arm/boot/zImage $OUT_DIR/zImage
	$OUT_HOSTBIN_DIR/mkbootimg --kernel $OUT_DIR/zImage --ramdisk $OUT_DIR/ramdisk-uboot.img -o $OUT_DIR/boot.img
	check_exit

	echo 'update.zip ->' $OUT_DIR
	zip -j $OUT_DIR/update.zip $OUT_DIR/android-info.txt $OUT_DIR/boot.img $OUT_DIR/system.img
	check_exit

	echo
}

echo
echo '                Build android for '$SEC_PRODUCT''
echo

case "$SEC_PRODUCT" in
	smdkv310)
		build_android
		make_uboot_img
		make_fastboot_img
		;;
	smdk4x12)
		#build_wifi
		build_android
		make_uboot_img
		make_fastboot_img
		if [[ $1 = $option ]]
		then
			make_recovery_img
		fi
		;;
	smdk5250)
		build_android
		make_uboot_img
		make_fastboot_img
		;;
	generic)
		build_android
		make_uboot_img
		;;
	*)
		echo "Please, set SEC_PRODUCT"
		echo "  export SEC_PRODUCT=smdkv310 or SEC_PRODUCT=smdk4x12 or SEC_PRODUCT=smdk5250"
		echo "     or "
		echo "  export SEC_PRODUCT=generic"
		exit 1
		;;
esac

echo ok success !!!

exit 0
