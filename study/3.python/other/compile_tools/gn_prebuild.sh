#!/bin/bash
ALPS_PATH=`pwd`
PACKAGES_NAME=""
GN_PACKAGES_NAME=""
GN_COMPILE_TOOLS_PATH="gionee/config"
GN_ALPS_PATH="gionee/code/alps"
GN_BUILD_PATH="gionee/code/build"
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
	echo "PROJECT_TYPE:[$PROJECT_TYPE]"
	cleanPackages "$@"
	linkPackagesDir "$@"
    #Gionee bjy3 laihh add 20161009 start
	GIONEEPRJNAME=${PROJECT:0:6}
    #Gionee bjy3 laihh add 20161009 start end
    #Gionee bjy3 laihh add 20160809 start
    copy_to_alps "./${GN_BUILD_PATH}" "${ALPS_PATH}"
    copy_to_alps "./${GN_PACKAGES_NAME}/build" "${ALPS_PATH}/packages"
    #Gionee bjy3 laihh add 20160809 start end
	copy_to_alps "./${GN_DRIVER_PATH}/drv_common" ${ALPS_PATH}
    #gionee.bj.wangguojun modify begin
	copy_to_alps "./${GN_DRIVER_PATH}/project_common/${GIONEEPRJNAME}_DRV_COMMON" ${ALPS_PATH}
	echo "A============GIONEEPRJNAME= ${GIONEEPRJNAME} "
	echo "A============PROJECT     == ${PROJECT} "
    #gionee.bj.wangguojun modify end
	if [ "${BUILD_OPTIONS}" == "platform" -a "${PROJECT_TYPE}" == "platform" ];then
	    #gionee.bj.wangguojun modify begin
		copy_to_alps "./${GN_DRIVER_PATH}/project_common/drv_platform" "${ALPS_PATH}"
	    #gionee.bj.wangguojun modify end
	elif [ "${BUILD_OPTIONS}" == "platform" -a "${PROJECT_TYPE}" == "CTA" ];then
		copy_to_alps "./${GN_ALPS_PATH}/public/CTA" "${ALPS_PATH}"
	elif [ "${BUILD_OPTIONS}" == "platform" -a "${PROJECT_TYPE}" == "CMCC" ];then
		copy_to_alps "./${GN_ALPS_PATH}/public/CMCC" "${ALPS_PATH}"
		copy_to_alps "./${GN_ALPS_PATH}/public/ROM/gnframework-res" "${ALPS_PATH}/gnframework-res" 
		copy_to_alps "./${GN_ALPS_PATH}/public/ROM/amigoframework" "${ALPS_PATH}/amigoframework"
		copy_to_alps "./${GN_ALPS_PATH}/public/ROM/amigoframework/out" "${ALPS_PATH}/out"
	elif [ "${BUILD_OPTIONS}" == "platform" -a "${PROJECT_TYPE}" == "CT" ];then
		copy_to_alps "./${GN_ALPS_PATH}/public/CT" "${ALPS_PATH}"
		copy_to_alps "./${GN_ALPS_PATH}/public/ROM/gnframework-res" "${ALPS_PATH}/gnframework-res" 
		copy_to_alps "./${GN_ALPS_PATH}/public/ROM/amigoframework" "${ALPS_PATH}/amigoframework"
		copy_to_alps "./${GN_ALPS_PATH}/public/ROM/amigoframework/out" "${ALPS_PATH}/out"
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

	if [[ "${BUILD_OPTIONS}" != "platform" && "${PROJECT_TYPE}" != "CTA" && "${PROJECT_TYPE}" != "CMCC" && "${PROJECT_TYPE}" != "CT" ]];then
		#copy packages to alps
		if [ -d "./${GN_PACKAGES_NAME}/public/apk" ];then
			copy_to_alps "./${GN_PACKAGES_NAME}/public/apk" "${ALPS_PATH}/packages"
		fi
		
		#copy apk config to alps
		if [ -d "./${GN_PACKAGES_NAME}/public/apk_config" ];then
			copy_to_alps "./${GN_PACKAGES_NAME}/public/apk_config" "${ALPS_PATH}/packages"
		fi
		
		if [ -d "./${GN_PACKAGES_NAME}/public/code" ];then
			copy_to_alps "./${GN_PACKAGES_NAME}/public/code" "${ALPS_PATH}/packages"
		fi
	elif [ "${BUILD_OPTIONS}" == "platform" -a "${PROJECT_TYPE}" == "CT" ];then
		if [ -d "./${GN_PACKAGES_NAME}/public/code_ct" ];then
			copy_to_alps "./${GN_PACKAGES_NAME}/public/code_ct" "${ALPS_PATH}/packages"
		fi
	elif [ "${BUILD_OPTIONS}" == "platform" -a "${PROJECT_TYPE}" == "CMCC" ];then
		if [ -d "./${GN_PACKAGES_NAME}/public/code_cmcc" ];then
			copy_to_alps "./${GN_PACKAGES_NAME}/public/code_cmcc" "${ALPS_PATH}/packages"
		fi
    fi
	#copy gn_config to alps
	copy_to_alps "./${GN_PACKAGES_NAME}/private/${PROJECT}" "${ALPS_PATH}/packages"
	
	#处理图片资源包
	if [[ "${BUILD_OPTIONS}" != "platform" && "${PROJECT_TYPE}" != "CTA" &&  "${PROJECT_TYPE}" != "CT" ]];then
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
	tmpStrings=$(echo ${hdwareType} | awk -F "-" '{printf("%s",$7)}')
	if [[ -z "${tmpStrings}" ]];then
		tmpStrings=$(echo ${hdwareType} | awk -F "-" '{printf("%s",$3)}')
	fi
	hdwareType=$(echo ${tmpStrings%_*})
	hdwareType=$(echo ${hdwareType:0-2:2})
	echo "The HardWare Type is:[${PROJECT}.mk][$hdwareType]!"
	copy_to_alps "./${GN_DRIVER_PATH}/private/${PROJECT}_${hdwareType}" "${ALPS_PATH}"
	#Gionee bjy3 ranyi add 20130514 for hardware change end
	
	#Gionee bjy3 ranyi add 20160819 for GN8002S change start
	#为了处理 GN8002 与 GN8002S 的差异，这两个项目的差异仅仅只是型号不同
	#当存在差异的时候需要拷贝  项目名_DIFF  文件夹
	copy_to_alps "./${GN_DRIVER_PATH}/private/${PROJECT}_DIFF" "${ALPS_PATH}"
	copy_to_alps "./${GN_ALPS_PATH}/private/${PROJECT}_DIFF" "${ALPS_PATH}"
	copy_to_alps "./${GN_PACKAGES_NAME}/private/${PROJECT}_DIFF" "${ALPS_PATH}/packages"
	#Gionee bjy3 ranyi add 20160819 for GN8002S change end
}

main "$@"
