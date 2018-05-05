#!/bin/bash

# The script is to generate full/incremental packages for OTA.
# In order to make sure the script can run normally, please make
# sure the android build enviroment is successfully set up.
# XXX Note: QRD JB release after D1219 is prefered.

# Author:mengmeng
# Email:mengmeng@qti.qualcomm.com


function print_usage()
{
    echo "Please input the proper parameter"
    echo "-t: type. full/incremental"
    echo "-d: work directory"
    echo "-i: input zip"
    echo "-o: output zip"
    echo "-m: modem path"
}

# Options setup
# -t: type. full/incremental
# -d: work directory
# -i: input zip
# -o: output zip
# -m: mp image path

# example: generate_package.sh -d JB_REL_QRD -t delta -i ota_tmp_package1.zip
# -i ota_tmp_package2.zip -o output.zip

while getopts "t:d:i:o:m:" arg
do
    case $arg in
    i)
        if [ -z $AP_IMAGE_NAME_V1 ]; then
            AP_IMAGE_NAME_V1=$OPTARG
        else
            AP_IMAGE_NAME_V2=$OPTARG
        fi
        ;;
    o)
        OUTPUT_IMAGE_NAME=$OPTARG
        ;;
    d)
        WORK_DIR=$OPTARG
        ;;
    m)
        if [ -z $MP_IMAGE_PATH_V1 ]; then
            MP_IMAGE_PATH_V1=$OPTARG
        else
            MP_IMAGE_PATH_V2=$OPTARG
        fi
        ;;
    t)
        if [ "$OPTARG" = "full" ];then
            OUTPUT_FULL=true
        elif [ "$OPTARG" = "incremental" ];then
            OUTPUT_FULL=false
        else
            echo "image type not recognized"
            exit 1
        fi
        ;;

    *)
        print_usage
        exit 1
        ;;
    esac
done    

function dump_variable()
{
    echo ============================
    echo WORK_DIR=$WORK_DIR
    echo AP_IMAGE_NAME_V1=$AP_IMAGE_NAME_V1
    echo AP_IMAGE_NAME_V2=$AP_IMAGE_NAME_V2
    echo OUTPUT_FULL=$OUTPUT_FULL
    echo OUTPUT_IMAGE_NAME=$OUTPUT_IMAGE_NAME
    echo MP_IMAGE_PATH_V1=$MP_IMAGE_PATH_V1
    echo MP_IMAGE_PATH_V2=$MP_IMAGE_PATH_V2
    echo ============================
}


# Fix the arguments
if [ -z $WORK_DIR ];then
    if [ ! -z $ANDROID_BUILD_TOP ];then
        WORK_DIR=$ANDROID_BUILD_TOP
    else
        WORK_DIR=`basename $0`
    fi
fi

CUR_DIR=`pwd`
WORK_DIR=`pwd`/$WORK_DIR
#AP_IMAGE_NAME_V1=$CUR_DIR/$AP_IMAGE_NAME_V1
#AP_IMAGE_NAME_V2=$CUR_DIR/$AP_IMAGE_NAME_V2
#OUTPUT_IMAGE_NAME=$CUR_DIR/$OUTPUT_IMAGE_NAME
OUTPUT_IMAGE_NAME=$OUTPUT_IMAGE_NAME

if [ -z $MP_IMAGE_PATH_V1 ];then
    echo "MP path is not provided. Assuming no radio image provided"
#else
    #MP_IMAGE_PATH_V1=$CUR_DIR/$MP_IMAGE_PATH_V1
    #MP_IMAGE_PATH_V2=$CUR_DIR/$MP_IMAGE_PATH_V2
fi

export ANDROID_ROOT=$WORK_DIR

function check_environment()
{
   if ! [ -d $WORK_DIR/device -a -d $WORK_DIR/build -a $WORK_DIR/out ];then
       echo "device, build and out directory should be included in WORK_DIR"
       exit 1
   fi

   if [ -z $AP_IMAGE_NAME_V1 ]; then
       print_usage
       exit 1
   elif [ "$OUTPUT_FULL" = "false" -a "$AP_IMAGE_NAME_V2" = "" ];then
       print_usage
       exit 1
   fi

   if [ -z $OUTPUT_IMAGE_NAME ];then
        print_usage
        exit 1
   fi
}

check_environment 
dump_variable

function setup_directory()
{
	mkdir -p $TMP_DIR_NAME/delta_package
	mkdir -p $TMP_DIR_NAME/image/v1/RADIO
	mkdir -p $TMP_DIR_NAME/image/v2/RADIO
	mkdir -p $TMP_DIR_NAME/image/tmp_package
}

function clean_output()
{
	rm -rf $TMP_DIR_NAME
}

TMP_DIR_NAME=/tmp/image$RANDOM
clean_output
setup_directory

# Generate MP raw image
function generate_raw_radio()
{
    local MPIMG_PATH=$1
	local OUTPUT_PATH=$2

	if [ ! -d $OUTPUT_PATH ];then
		mkdir -p $OUTPUT_PATH
	fi

	cat $MPIMG_PATH/amss.mbn > $OUTPUT_PATH/AMSS.mbn
	cat $MPIMG_PATH/amsshd.mbn > $OUTPUT_PATH/AMSSHD.mbn
	dd if=$MPIMG_PATH/oemsblhd.mbn of=$OUTPUT_PATH/OEMSBL.bin ibs=512 conv=sync
	cat $MPIMG_PATH/oemsbl.mbn >> $OUTPUT_PATH/OEMSBL.bin
	cat $MPIMG_PATH/qcsbl.mbn > $OUTPUT_PATH/QCSBL.bin
	cat $MPIMG_PATH/qcsblhd_cfgdata.mbn > $OUTPUT_PATH/QCSBLHD.bin
	dd if=$MPIMG_PATH/emmc_appsboothd.mbn of=$OUTPUT_PATH/APPSBOOT.bin ibs=512 conv=sync
	cat $MPIMG_PATH/emmc_appsboot.mbn >> $OUTPUT_PATH/APPSBOOT.bin
}

 cp $AP_IMAGE_NAME_V1 $TMP_DIR_NAME/image/tmp_package/`basename $AP_IMAGE_NAME_V1`

if [ "$OUTPUT_FULL" = "false" ];then 
 cp $AP_IMAGE_NAME_V2 $TMP_DIR_NAME/image/tmp_package/`basename $AP_IMAGE_NAME_V2`
fi

if [ ! -z $MP_IMAGE_PATH_V1 ];then
    generate_raw_radio $MP_IMAGE_PATH_V1 $TMP_DIR_NAME/image/v1/RADIO
    pushd $TMP_DIR_NAME/image/v1/
    zip -ur $TMP_DIR_NAME/image/tmp_package/`basename $AP_IMAGE_NAME_V1` RADIO
    popd
    if [ "$OUTPUT_FULL" = "false" ];then
        generate_raw_radio $MP_IMAGE_PATH_V2 $TMP_DIR_NAME/image/v2/RADIO
        pushd $TMP_DIR_NAME/image/v2/
        zip -ur $TMP_DIR_NAME/image/tmp_package/`basename $AP_IMAGE_NAME_V2` RADIO
        popd
    fi
else
	echo ddddd
   cp $AP_IMAGE_NAME_V1 $TMP_DIR_NAME/image/tmp_package/`basename $AP_IMAGE_NAME_V1`
   if [ "$OUTPUT_FULL" = "false" ];then
      cp $AP_IMAGE_NAME_V2 $TMP_DIR_NAME/image/tmp_package/`basename $AP_IMAGE_NAME_V2`
   fi
fi

cd $WORK_DIR

if [ "$OUTPUT_FULL" = "true" ];then
    ./build/tools/releasetools/ota_from_target_files -v  -x pagesize=4096 -p out/host/linux-x86  -d MMC -f 1 -k build/target/product/security/testkey $TMP_DIR_NAME/image/tmp_package/`basename $AP_IMAGE_NAME_V1` $OUTPUT_IMAGE_NAME
else
    ./build/tools/releasetools/ota_from_target_files -x pagesize=4096 -f 0 -d "MMC" -p out/host/linux-x86 -k build/target/product/security/testkey -v -i $TMP_DIR_NAME/image/tmp_package/`basename $AP_IMAGE_NAME_V1` $TMP_DIR_NAME/image/tmp_package/`basename $AP_IMAGE_NAME_V2` $OUTPUT_IMAGE_NAME
fi

cd $CUR_DIR
clean_output
