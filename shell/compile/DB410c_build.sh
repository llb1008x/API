#!/bin/bash 

#Copyright © 2015, Qualcomm Innovation Center, Inc. All rights reserved.  Confidential and proprietary. 
ITCVER="6.0.1_Marsh_Mallo_P2"
WORKDIR=`pwd`
BUILDROOT="$WORKDIR/APQ8016_410C_LA.BR.1.2.7-01010-8x16.0_$ITCVER"
PATCH_DIR="$WORKDIR/PATCH_8x16_410c_LA.BR.1.2.7-01010-8x16.0"
CAFTAG="LA.BR.1.2.7-01010-8x16.0.xml"

function SBC410C_SOURCE_CODE() {
# Do repo sanity test
if [ $? -eq 0 ]
then
	echo "Downloading code please wait.."
	repo init -u git://codeaurora.org/platform/manifest.git -b release -m ${CAFTAG} --repo-url=git://codeaurora.org/tools/repo.git 
	repo sync -j20
	repo forall -c "git checkout -b master"
	repo forall -c "git checkout -b LA.BR.1.2.7-01010-8x16"
else
	echo "repo tool problem, make sure you have set up your build environment"
	echo "1) http://source.android.com/source/initializing.html"
	echo "2) http://source.android.com/source/downloading.html (Installing Repo Section Only)"
	exit -1
fi
}

# Function to autoapply patches to CAF code
Apply_android_patches()
{	cd $WORKDIR
	wget https://www.codeaurora.org/patches/quic/la/PATCH_8x16_410c_LA.BR.1.2.7-01010-8x16.0.tar.gz
	tar -xvzf PATCH_8x16_410c_LA.BR.1.2.7-01010-8x16.0.tar.gz
	cd $BUILDROOT
	echo "Applying patches ..."
	if [ ! -e $PATCH_DIR ]
	then
		echo -e "$PATCH_DIR : Not Found "
	fi
	cd $PATCH_DIR
	patch_root_dir="$PATCH_DIR"
	android_patch_list=$(find . -type f -name "*.patch" | sort) &&
	for android_patch in $android_patch_list; do
		android_project=$(dirname $android_patch)
		echo -e "applying patches on $android_project ... "
		cd $BUILDROOT/$android_project 
		if [ $? -ne 0 ]; then
			echo -e "$android_project does not exist in BUILDROOT:$BUILDROOT "
			exit 1
		fi
		git am $patch_root_dir/$android_patch	
	done
}

#  Function to check whether host utilities exists
check_program() {
for cmd in "$@"
do
	which ${cmd} > /dev/null 2>&1
	if [ $? -ne 0 ]
	then
		echo
		echo -e "Cannot find command \"${cmd}\" "
		echo
		exit 1
	fi
done
}


#Main Script starts here
#Note: Check necessary program for installation
echo
echo -e "$QUALCOMM_410C_SBC Release Version : $ITCVER "
echo -e "$QUALCOMM_410C_SBC Workdir                   : $WORKDIR "
echo -e "$QUALCOMM_410C_SBC Build Root                : $BUILDROOT "
echo -e "$QUALCOMM_410C_SBC Patch Dir                 : $PATCH_DIR "
echo -e "$QUALCOMM_410C_SBC Codeaurora TAG            : $CAFTAG "
echo -n "Checking necessary program for installation......"
echo
check_program tar repo git patch
if [ -e $BUILDROOT ]
then
	cd $BUILDROOT
else 
	mkdir $BUILDROOT
	cd $BUILDROOT
fi

#1 Download code
SBC410C_SOURCE_CODE
cd $BUILDROOT
#2 Apply APQ8016 410C Snapdragon Dragonboard patches
Apply_android_patches
cd $BUILDROOT
echo -e "   Extracting proprietary binary package to $BUILDROOT ... "
tar -xzvf ../proprietary_LA.BR.1.2.7-01010-8x16.0_410C_May.tgz -C vendor/qcom/
mv vendor/qcom/proprietary/bootanimation.zip device/qcom/msm8916_64/
mv vendor/qcom/proprietary/WCNSS_qcom_wlan_nv.bin device/qcom/msm8916_64/

cd $BUILDROOT
#3 Build
source build/envsetup.sh 
lunch msm8916_64-userdebug 

make -j10 | tee log.txt

