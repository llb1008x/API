#!/bin/bash

# \033[显示方式;前景色;背景色m
# 显示方式:0（默认值）、1（高亮）、22（非粗体）、4（下划线）、24（非下划线）、5（闪烁）、25（非闪烁）、7（反显）、27（非反显）
# 前景色:30（黑色）、31（红色）、32（绿色）、 33（黄色）、34（蓝色）、35（洋红）、36（青色）、37（白色）
# 背景色:40（黑色）、41（红色）、42（绿色）、 43（黄色）、44（蓝色）、45（洋红）、46（青色）、47（白色）
# \033[0m 默认
# \033[1;32;40m 绿色
# 033[1;31;40m 红色

COLOR_ERROR_PREFIX='\033[1;31;48m [ERROR]'
COLOR_WARNING_PREFIX='\033[1;33;48m [WARNING]'
COLOR_SUCCESS_PREFIX='\033[1;32;48m [SUCCESS]'
COLOR_GREEN_PREFIX='\033[1;32;48m'
COLOR_BLUE_PREFIX='\033[1;34;48m'
COLOR_SUFFIX='\033[0m'

function sign_TaTee()
{
	echo -e "${COLOR_GREEN_PREFIX} Star TATEE sign !${COLOR_SUFFIX}"
	local keyFile="vendor/mediatek/proprietary/trustzone/trustonic/source/bsp/platform/mt6735/kernel/pairVendorTltSig.pem"
	local logPath="BUILDING_LOG/TaTeeLog"
	if [ -d tmpTateeBin ];then
		rm -rf tmpTateeBin
	fi
	mkdir tmpTateeBin
	mkdir -p ${logPath}
	java -jar vendor/mediatek/proprietary/trustzone/trustonic/source/trustlets/ttcm/platform/mt6735/tlCm/MobiConfig-1.0.0.jar --config --in vendor/mediatek/proprietary/trustzone/trustonic/source/trustlets/ttcm/platform/mt6735/tlCm/release/tlCm.axf --out tmpTateeBin/tlCm.axf.conv --kphkey1 vendor/mediatek/proprietary/trustzone/trustonic/source/trustlets/ttcm/platform/mt6735/tlCm/pubKphRequestKid.pem  --rekey1 vendor/mediatek/proprietary/trustzone/trustonic/source/trustlets/ttcm/platform/mt6735/tlCm/pubRootTransportKid.pem --kid1 61451
	java -jar vendor/mediatek/proprietary/trustzone/trustonic/source/bsp/platform/mt6735/t-sdk/TlSdk/Out/Bin/MobiConvert/MobiConvert.jar -b tmpTateeBin/tlCm.axf.conv -servicetype 3 -output tmpTateeBin/07010000000000000000000000000000.tlbin -k ${keyFile} > ${logPath}/07010000000000000000000000000000.tlbin.log
	cp tmpTateeBin/07010000000000000000000000000000.tlbin device/gionee_bj/gnbj6737t_66_m0/07010000000000000000000000000000.tlbin
	cp tmpTateeBin/07010000000000000000000000000000.tlbin vendor/mediatek/proprietary/trustzone/trustonic/source/trustlets/ttcm/platform/mt6735/tlCm/07010000000000000000000000000000.tlbin
	
	#java -jar vendor/mediatek/proprietary/trustzone/trustonic/source/bsp/platform/mt6755/t-sdk/DrSdk/Out/Bin/MobiConvert/MobiConvert.jar  -servicetype 1  -numberofthreads 3  -bin vendor/mediatek/proprietary/trustzone/trustonic/source/trustlets/ttcm/platform/mt6735/tlCm/release/drfpc.axf -output tmpTateeBin/04020000000000000000000000000000.drbin  -d $((0 <<16|0x0400 )) -memtype 2  -flags $((0 |8))  -interfaceversion 1.0   -keyfile ${keyFile} > ${logPath}/04020000000000000000000000000000.drbin.log
	#cp tmpTateeBin/04020000000000000000000000000000.drbin vendor/mediatek/proprietary/trustzone/trustonic/source/trustlets/fingerprint/fpc_lib/04020000000000000000000000000000.drbin

	#java -jar vendor/mediatek/proprietary/trustzone/trustonic/source/bsp/platform/mt6735/t-sdk/TlSdk/Out/Bin/MobiConvert/MobiConvert.jar  -servicetype 3 -numberofthreads 1 -numberofinstances 1 -memtype 2 -flags $((4|8))  -bin vendor/fingerprints/fingerprint_ta/platform/tbase/Out/Bin/Debug/fpc_ta.axf -output  tmpTateeBin/04010000000000000000000000000000.tlbin -keyfile  ${keyFile}   -interfaceversion 0.0 -initheapsize 12582912   -maxheapsize 16777216 > ${logPath}/04010000000000000000000000000000.tlbin.log
	#cp tmpTateeBin/04010000000000000000000000000000.tlbin vendor/mediatek/proprietary/trustzone/trustonic/source/trustlets/fingerprint/fpc_lib/04010000000000000000000000000000.tlbin

	java -jar vendor/mediatek/proprietary/trustzone/trustonic/source/bsp/platform/mt6735/t-sdk/TlSdk/Out/Bin/MobiConvert/MobiConvert.jar -servicetype 3 -numberofthreads 1 -numberofinstances 1 -memtype 2 -flags $((0|8))  -bin vendor/mediatek/proprietary/trustzone/trustonic/source/trustlets/GioneePay/tlGioneePay.axf -output tmpTateeBin/07040000000000000000000000000000.tlbin -keyfile ${keyFile}  -interfaceversion 0.0 -initheapsize 4194304 -maxheapsize 8388608 > ${logPath}/07040000000000000000000000000000.tlbin.log
	cp tmpTateeBin/07040000000000000000000000000000.tlbin vendor/mediatek/proprietary/trustzone/trustonic/source/trustlets/GioneePay/07040000000000000000000000000000.tlbin

	#java -jar vendor/mediatek/proprietary/trustzone/trustonic/source/bsp/platform/mt6735/t-sdk/TlSdk/Out/Bin/MobiConvert/MobiConvert.jar -servicetype 3 -numberofthreads 1 -numberofinstances 1 -memtype 2 -flags $((0|8))  -bin vendor/mediatek/proprietary/trustzone/trustonic/source/trustlets/ifaa/tlifaa.axf -output tmpTateeBin/08010203000000000000000000000000.tabin -keyfile ${keyFile} -gp_level GP  -interfaceversion 0.0 -initheapsize 4194304 -maxheapsize 4194304 > ${logPath}/08010203000000000000000000000000.tabin.log
 	#cp tmpTateeBin/08010203000000000000000000000000.tabin vendor/mediatek/proprietary/trustzone/trustonic/source/trustlets/ifaa/08010203000000000000000000000000.tabin
	
	rm -rf tmpTateeBin
	echo -e "${COLOR_GREEN_PREFIX} End TATEE sign !${COLOR_SUFFIX}"
}

function main()
{
	sign_TaTee
}

main
