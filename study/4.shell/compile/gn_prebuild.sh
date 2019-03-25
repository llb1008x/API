#!/bin/bash
ALPS_PATH=`pwd`
PACKAGES_NAME=""
GN_PACKAGES_NAME=""
GN_COMPILE_TOOLS_PATH="gionee/config"
GN_ALPS_PATH="gionee/code/alps"
GN_DRIVER_PATH="gionee/code/driver"
PROJECT=${GIONEEPRODUCTID}
IS_SIGN_FLAG="NO"

function copy_to_alps(){
    sourcedir="$1"
    destdir="$2"
    if [ -d "${sourcedir}" ];then
        echo "copy ${sourcedir} to  ${destdir} "
        #if [ "`ls -A ${sourcedir}`" != "" ]; then
        #	mkdir -p ${destdir}
        #	cp -a ${sourcedir}/* ${destdir}/
        #fi
        pushd $sourcedir >/dev/null
        find . -path "*\.git" -prune -o -type f -follow -print | cpio -pdmu --quiet $destdir
        popd >/dev/null
    fi
}

function cleanPackages()
{
	if [ "${PROJECT:(-4):4}" == "sign" ];then
	    PROJECT=${PROJECT%_*}
	    IS_SIGN_FLAG="YES"
	fi
	echo "PROJECT:${PROJECT},GIONEEPRODUCTID:${GIONEEPRODUCTID}"
	if [ -z "${PROJECT}" ]; then
		echo "*** procuction invalid. ***"
		exit 1
	else
		rm -rf "./packages"
		VENDOR="$1"
		[ -n "$2" ] && PLATFORM_VERSION="_$2"
		echo "PLATFORM_VERSION:${PLATFORM_VERSION}"
	fi
}

function linkPackagesDir()
{
	local localPackagePath=$(ls ../ | grep "packages")
	PACKAGES_NAME="../${localPackagePath}"
	GN_PACKAGES_NAME="${PACKAGES_NAME}/gionee"
	echo "GN_PACKAGES_NAME:${GN_PACKAGES_NAME}"
	if [ -d "${PACKAGES_NAME}" ];then
		echo "ln -f -s ${PACKAGES_NAME} to packages."
		ln -f -s "${PACKAGES_NAME}" "./packages"
	else
		echo "*** Link package directory error ***" >&2
		exit 1
	fi
}

function main()
{
	cleanPackages "$@"
	linkPackagesDir "$@"
	GIONEEPRJNAME=${PROJECT:0:7}
	copy_to_alps "./${GN_DRIVER_PATH}/drv_common" ${ALPS_PATH}
    #gionee.bj.wangguojun modify begin
	copy_to_alps "./${GN_DRIVER_PATH}/project_common/${GIONEEPRJNAME}_DRV_COMMON" ${ALPS_PATH}
	echo "A============GIONEEPRJNAME= ${GIONEEPRJNAME} "
	echo "A============PROJECT======= ${PROJECT} "
    #gionee.bj.wangguojun modify end
	if [ "${BUILD_OPTIONS}" == "platform" -a "${PROJECT:(-8):8}" == "platform" ];then
	    #gionee.bj.wangguojun modify begin
		copy_to_alps "./${GN_DRIVER_PATH}/project_common/drv_platform" "${ALPS_PATH}"
	    #gionee.bj.wangguojun modify end
	elif [ "${BUILD_OPTIONS}" == "platform" -a "${PROJECT:(-3):3}" == "CTA" ];then
		copy_to_alps "./${GN_ALPS_PATH}/public/CTA" "${ALPS_PATH}"
	elif [ "${BUILD_OPTIONS}" == "platform" -a "${PROJECT:(-4):4}" == "CMCC" ];then
		copy_to_alps "./${GN_ALPS_PATH}/public/CMCC" "${ALPS_PATH}"
	elif [ "${BUILD_OPTIONS}" == "platform" -a "${PROJECT:(-2):2}" == "CT" ];then
		copy_to_alps "./${GN_ALPS_PATH}/public/CT" "${ALPS_PATH}"
	else
		#应用所有项目共用
		copy_to_alps "./${GN_ALPS_PATH}/public/ROM" "${ALPS_PATH}"

		copy_to_alps "./${GN_ALPS_PATH}/public/ROM/gnframework-res" "${ALPS_PATH}/gnframework-res" 
		copy_to_alps "./${GN_ALPS_PATH}/public/ROM/YouJuAgent" "${ALPS_PATH}/YouJuAgent"
		copy_to_alps "./${GN_ALPS_PATH}/public/ROM/YouJuAgent/out" "${ALPS_PATH}/out"
		copy_to_alps "./${GN_ALPS_PATH}/public/ROM/amigoframework" "${ALPS_PATH}/amigoframework"
		copy_to_alps "./${GN_ALPS_PATH}/public/ROM/amigoframework/out" "${ALPS_PATH}/out"
	fi
	#gionee.bj.wangguojun modify begin
	copy_to_alps "./${GN_DRIVER_PATH}/project/${PROJECT}" "${ALPS_PATH}"
	if [ "${IS_SIGN_FLAG}" == "YES" ];then
	    copy_to_alps "./${GN_DRIVER_PATH}/project/${PROJECT}_sign" "${ALPS_PATH}"
	fi
	#gionee.bj.wangguojun modify end
	copy_to_alps "./${GN_ALPS_PATH}/private/${PROJECT}" "${ALPS_PATH}"

	if [[ "${BUILD_OPTIONS}" != "platform" && "${PROJECT:(-3):3}" != "CTA" && "${PROJECT:(-4):4}" != "CMCC" && "${PROJECT:(-2):2}" != "CT" ]];then
		#copy packages to alps
		if [ -d "./${GN_PACKAGES_NAME}/public/apk" ];then
			copy_to_alps "./${GN_PACKAGES_NAME}/public/apk" "${ALPS_PATH}/packages"
		fi

		if [ -d "./${GN_PACKAGES_NAME}/public/code" ];then
			copy_to_alps "./${GN_PACKAGES_NAME}/public/code" "${ALPS_PATH}/packages"
		fi
    fi
	#copy gn_config to alps
	copy_to_alps "./${GN_PACKAGES_NAME}/private/${PROJECT}" "${ALPS_PATH}/packages"
	if [[ "${BUILD_OPTIONS}" != "platform" && "${PROJECT:(-3):3}" != "CTA" &&  "${PROJECT:(-2):2}" != "CT" ]];then
		#unzip gn_image.zip and copy to alps
		if [ -e "./${GN_PACKAGES_NAME}/private/gn_img_"${PROJECT}".zip" ]
		then
		    echo "unzip -o -q ./${GN_PACKAGES_NAME}/private/gn_img_${PROJECT}.zip  -d ./${GN_PACKAGES_NAME}/private"
			unzip -o -q ./${GN_PACKAGES_NAME}/private/gn_img_${PROJECT}.zip  -d ./${GN_PACKAGES_NAME}/private
			if [ -d "./${GN_PACKAGES_NAME}/private/gn_img" ];then
				find ./${GN_PACKAGES_NAME}/private/gn_img -type f -name "Thumbs.db*" | xargs rm -rf
				copy_to_alps "./${GN_PACKAGES_NAME}/private/gn_img" "${ALPS_PATH}"
				rm -rf ./${GN_PACKAGES_NAME}/private/gn_img
			else
				echo "*** gn_image dirs tree error ***"
			fi
		fi
		. `dirname $0`/gn_auto_config.sh
	fi

	#Gionee bjy3 ranyi add 20130514 for hardware change start
	hdwareType="GIONEE_VERSION_BY"
	hdwareType=$(grep ${hdwareType} ${GN_COMPILE_TOOLS_PATH}/${PROJECT}.mk | awk -F "=" '{printf("%s",$2)}')
	hdwareType=$(echo ${hdwareType:0-2:2})
	echo "The HardWare Type is:[${PROJECT}.mk][$hdwareType]!"
	copy_to_alps "./${GN_DRIVER_PATH}/private/${PROJECT}_${hdwareType}" "${ALPS_PATH}"
	#Gionee bjy3 ranyi add 20130514 for hardware change end
}

main "$@"
