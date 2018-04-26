#!/bin/bash

#ALA
#version1.3
#refresh time:2017-05-26
#------------------------
# APPS packet info start
CODE_PICTURE_PACKET_FULL_PATH=""
#GUI_FTP_USER="gui"
#GUI_FTP_PASSWD="gui123"
Tem_PRODUCT_COPY_FILES=""
ANDROID_PATH=""
PACKAGE_PATH=""
UPDATEAPPS_LIST=""
GET_APK_NAME=""
STORE_APK_ADDRESS=""
LOCAL_MODULE=""
APK_NAME=""
SELECT_PROJECT_NAME=""
ALA_FLAG="0"

# color output style start
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
COLOR_SUFFIX='\033[0m'
# color output style end

function errorAction()
{	

	echo -e "${COLOR_ERROR_PREFIX}当前的操作有错误，请在弹出的新窗口中处理完成错误之后再继续.....${COLOR_SUFFIX}"
	read -p <<ALA \
"当前的操作有错误，请在弹出的新窗口中处理完成错误之后再继续,
回车键继续...."
ALA
	#打开一个新的终端，在新的窗口中处理错误
	gnome-terminal
	read -p "请在新窗口中处理完错误之后，回车键将继续执行...."
}


function gitAndUnzipApkFileFromFTP()
{
	echo -e "${COLOR_GREEN_PREFIX} function:gitAndUnzipApkFileFromFTP ${COLOR_SUFFIX}"	
	local ftp_name="reldn"
	local ftp_password="ksjfsf@off"
	STORE_APK_ADDRESS="/tmp/DEV_BRANCH_"$(date +%y%m%d%M%S)
	if [[ -d ${STORE_APK_ADDRESS} ]];then
		rm -rf ${STORE_APK_ADDRESS}
	fi
	mkdir ${STORE_APK_ADDRESS}
	pushd ${STORE_APK_ADDRESS}
	while read perapkinfo
	 do
	 	echo "perapkinfo :[$perapkinfo]"
	 	APK_NAME=$(echo ${perapkinfo} |awk -F ',' '{print $1}')
	 	local apk_zip_name=$(echo ${perapkinfo} |awk -F ',' '{print $2}')
	 	local ftp_address="ftp://18.8.8.2/software_release/Applications/${APK_NAME}/${apk_zip_name}"
	 	echo "APK_NAME :[${APK_NAME}]"
	 	echo "apk_zip_name :[${apk_zip_name}]"
	 	echo "wget ${ftp_address} --ftp-user=${ftp_name} --ftp-password=${ftp_password} -P ${STORE_APK_ADDRESS}"
		wget ${ftp_address} --ftp-user=${ftp_name} --ftp-password=${ftp_password} -P ${STORE_APK_ADDRESS}
		if [ $? -ne 0 ];then
		    echo -e "${COLOR_ERROR_PREFIX} 从FTP下载代码失败，请检查！${COLOR_SUFFIX}"
		    exit 1
	    fi
	    #-----------解压-----------
		7z x ${apk_zip_name} -o./${APK_NAME}/
	    if [ $? -ne 0 ];then
		    echo -e "${COLOR_ERROR_PREFIX} ${FTP_PATCH_TAG}* 解压失败，请检查！${COLOR_SUFFIX}"
		    exit 1
	    fi
		echo "rm ${apk_zip_name}"
		rm ${apk_zip_name}
	done < <(echo ${GET_APK_INFO} |awk -F';' '{split($0,arr,";")}END{for(i=1;i<=NF;i++)printf("%s\n",arr[i])}')	
	popd >>/dev/null
}

function getGioneeAppsPathAndUpdaToNew()
{	
	echo -e "${COLOR_GREEN_PREFIX} function:getGioneeAppsPathAndUpdaToNew ${COLOR_SUFFIX}"
		while read permodulename
		do
			echo "---------------------------APKname:[${permodulename}]-----start----------------"
			echo "permodulename:[${permodulename}]" 
			CODE_APPS_FULL_PATH=""
			#LOCAL_MODULE=$(grep -r "LOCAL_MODULE :=" ${STORE_APK_ADDRESS}/${permodulename}/|awk -F ":= "  '{print $2}')
			if [[ $(find ${STORE_APK_ADDRESS}/${permodulename}/ -name "*.apk"|wc -l) != "1" ]];then
				echo "${permodulename}压缩包下存在多个.apk文件如下，请手动处理"
				echo "----------------------.apk文件列表如下-----------------------------"
				find ${STORE_APK_ADDRESS}/${permodulename}/ -name "*.apk"
				echo "-----------------------------------------------------------------"
				read -p "确认后请回车继续其他模块的自动提交" answer < /dev/tty
			continue < /dev/tty
			fi
			
			LOCAL_MODULE=$(find ${STORE_APK_ADDRESS}/${permodulename}/ -name "*.apk"|awk -F '.apk' '{print $1}')
			LOCAL_MODULE=${LOCAL_MODULE##*/}
			echo "LOCAL_MODULE:[${LOCAL_MODULE}]"
			#包含apk的gionee下所有路径,归类
			touch ${STORE_APK_ADDRESS}/${LOCAL_MODULE}_path_list
			touch ${STORE_APK_ADDRESS}/${LOCAL_MODULE}_private_path_list
			touch ${STORE_APK_ADDRESS}/${LOCAL_MODULE}_public_path_list
			find ${PACKAGE_PATH}/gionee -name ${LOCAL_MODULE}.apk > ${STORE_APK_ADDRESS}/${LOCAL_MODULE}_path_list
			echo "{STORE_APK_ADDRESS}/{LOCAL_MODULE}_path_list---list begin"
			cat ${STORE_APK_ADDRESS}/${LOCAL_MODULE}_path_list
			echo "{STORE_APK_ADDRESS}/{LOCAL_MODULE}_path_list---list end"
			#如果存在apk包，则获取所有私有或者公有路径
			#分别获私有和公有路径-----start-----
			
			while read per_project
			do
				echo "per_project:[${per_project}]---"
				local perline=""
				if [[ -n $(cat ${STORE_APK_ADDRESS}/${LOCAL_MODULE}_path_list|grep ${per_project}/apps) ]];then
					perline=$(cat ${STORE_APK_ADDRESS}/${LOCAL_MODULE}_path_list|grep ${per_project}/apps)
					echo "perline:[$perline]"
					echo "${per_project} ${perline}" >> ${STORE_APK_ADDRESS}/${LOCAL_MODULE}_private_path_list
				fi
			done < <(echo ${SELECT_PROJECT_NAME} |awk -F';' '{split($0,arr,";")}END{for(i=1;i<=NF;i++)printf("%s\n",arr[i])}')
			#匹配北京和深圳的公有路径
			if [[ -n $(cat ${STORE_APK_ADDRESS}/${LOCAL_MODULE}_path_list |grep '/public/') ]];then
				perline=$(cat ${STORE_APK_ADDRESS}/${LOCAL_MODULE}_path_list |grep '/public/')
				echo "${perline}" >> ${STORE_APK_ADDRESS}/${LOCAL_MODULE}_public_path_list
			elif [[ -n $(cat ${STORE_APK_ADDRESS}/${LOCAL_MODULE}_path_list |grep '/packages_/') ]];then
				perline=$(cat ${STORE_APK_ADDRESS}/${LOCAL_MODULE}_path_list |grep '/packages_/')
				echo "${perline}" >> ${STORE_APK_ADDRESS}/${LOCAL_MODULE}_public_path_list
			else
				echo "公有文件夹不存在此APK"
			fi

			#分别获私有和共有路径-----end-----
			##有其他项目共用publick文件并判断开关为开，则ALA_FLAG="1"，要把apk拷贝到对应每个私有目录下---start
			while read per_mk
			do
				echo $per_mk
				local gionee_prj=$(echo ${per_mk}|awk -F '.mk:' '{print $1}')
				gionee_prj=${gionee_prj#*/}
				echo "gionee_prj:[${gionee_prj}]"
				if [[  -z $(echo ${SELECT_PROJECT_NAME}|grep ${gionee_prj}) ]];then
					if [[ -z $(find ${PACKAGE_PATH}/gionee/ -name ${LOCAL_MODULE}.apk |grep ${gionee_prj}|grep -v 'gn_common') ]];then
					 ALA_FLAG="1"
					fi
				fi			
			done < <(grep -ir "${LOCAL_MODULE}_SUPPORT" ${ANDROID_PATH}/gionee/config |grep "yes")
						
 			#其他项目不用公有的APK（即开关为关或者是apk存在私有目录中）则提交apk到公有文件夹--已包含当前要提交的多个项目的相互判断
			if [[ ${ALA_FLAG} = "0" ]];then
				if [[ -n $(cat ${STORE_APK_ADDRESS}/${LOCAL_MODULE}_public_path_list) ]];then
					CODE_APPS_FULL_PATH=$(sed -n '1p' ${STORE_APK_ADDRESS}/${LOCAL_MODULE}_public_path_list)
					CODE_APPS_FULL_PATH=${CODE_APPS_FULL_PATH%/*}
					echo "2-------------CODE_APPS_FULL_PATH:[${CODE_APPS_FULL_PATH}]"
					PRJ_PLATFORM="替换到共有文件夹"
				#深圳高通代码结构  
				elif [[ -n $(echo ${PACKAGE_PATH}|awk -F '_qc_' '{print $2}') ]];then
					PRJ_PLATFORM="QCOM"
					CODE_APPS_FULL_PATH=${PACKAGE_PATH}/gionee/qcom/packages_*/apps/${LOCAL_MODULE}
				#北京高通代码结构
				elif [[ -n $(echo ${PACKAGE_PATH}|awk -F '_qcom_' '{print $2}') ]];then
					PRJ_PLATFORM="QCOM"
					CODE_APPS_FULL_PATH=${PACKAGE_PATH}/gionee/public/apk/apps/${LOCAL_MODULE}
				#北京MTK代码结构
				elif [[ -n $(echo ${PACKAGE_PATH}|awk -F '_mtk_' '{print $2}') ]];then
					PRJ_PLATFORM="MTK"
					CODE_APPS_FULL_PATH=${PACKAGE_PATH}/gionee/public/apk/apps/${LOCAL_MODULE}
				else
					echo "输入有误，请确认"
					exit
				fi
				echo "---{ALA_FLAG} = "0"--CODE_APPS_FULL_PATH:[$CODE_APPS_FULL_PATH] PRJ_PLATFORM:[$PRJ_PLATFORM] "
				cppermodultonew
				updatepermodultonew2
				turnToGitPatchAdd
				echo "---------------------------APKname:[all_APK}]public/apk/apps-----end----------------"

			else [[ ${ALA_FLAG} = "1" ]]
				while read project
				do
					echo $project
					if [[ -n $(cat ${STORE_APK_ADDRESS}/${LOCAL_MODULE}_private_path_list|grep $project/apps) ]];then								
						CODE_APPS_FULL_PATH=$(cat ${STORE_APK_ADDRESS}/${LOCAL_MODULE}_private_path_list|grep $project/apps|awk '{print $2}')
						echo "77777---CODE_APPS_FULL_PATH:[$CODE_APPS_FULL_PATH]"
						CODE_APPS_FULL_PATH=${CODE_APPS_FULL_PATH%/*}
						echo "888888---CODE_APPS_FULL_PATH:[$CODE_APPS_FULL_PATH]"
					#深圳高通代码结构  
					elif [[ -n $(echo ${PACKAGE_PATH}| awk -F '_qc_' '{print $2}') ]];then
						PRJ_PLATFORM="QCOM"
						CODE_APPS_FULL_PATH=${PACKAGE_PATH}/gionee/project/${project}/packages/apps/${LOCAL_MODULE}
					#北京高通代码结构
					elif [[ -n $(echo ${PACKAGE_PATH}| awk -F '_qcom_' '{print $2}') ]];then
						PRJ_PLATFORM="QCOM"
						CODE_APPS_FULL_PATH=${PACKAGE_PATH}/gionee/private/${project}/apps/${LOCAL_MODULE}
					#北京MTK代码结构
					elif [[ -n $(echo ${PACKAGE_PATH}| awk -F '_mtk_' '{print $2}') ]];then
						PRJ_PLATFORM="MTK"
						CODE_APPS_FULL_PATH=${PACKAGE_PATH}/gionee/private/${project}/apps/${LOCAL_MODULE}
					else
						echo "输入有误，请确认"
						exit
					fi
					echo "---{ALA_FLAG} = "1"--CODE_APPS_FULL_PATH:[$CODE_APPS_FULL_PATH] PRJ_PLATFORM:[$PRJ_PLATFORM] "
					cppermodultonew
					updatepermodultonew2
					turnToGitPatchAdd

					echo " ---------------------------APKname:[${permodulename}]-----end----------------"
				done < <(echo ${SELECT_PROJECT_NAME} |awk -F';' '{split($0,arr,";")}END{for(i=1;i<=NF;i++)printf("%s\n",arr[i])}')
			fi

		UPDATEAPPS_LIST="${LOCAL_MODULE},${UPDATEAPPS_LIST}"
		echo "UPDATEAPPS_LIST:${UPDATEAPPS_LIST}"
		done < <(ls -F ${STORE_APK_ADDRESS}/ | grep '/$' | awk -F '/' '{print $1}')

	echo "UPDATEAPPS_LIST:[${UPDATEAPPS_LIST}]"
	echo "CODE_APPS_FULL_PATH}/{permodulename}:[${CODE_APPS_FULL_PATH}/${permodulename}]"
	pwd
	#mtk commit private dir
	runRepoCommond "git status"
	read -p "请确认更新的文件列表是否正确，如无误，请按回车执行commit" git_status_flag
	runRepoCommond "git commit -m \"[Fixed,${RECORD_ID}]:${UPDATEAPPS_LIST}\""
	if [ $? -ne 0 ]; then
        errorAction
    else
    	echo "git commit successfully,"    
    fi
}

 function turnToGitPatchAdd()
{
	echo -e "${COLOR_GREEN_PREFIX} function:turnToGitPatchAdd ${COLOR_SUFFIX}"
	pushd ${CODE_APPS_FULL_PATH} >/dev/null
	git add -A ${CODE_APPS_FULL_PATH}/
	popd >/dev/null
}
function cppermodultonew()
{
	echo -e "${COLOR_GREEN_PREFIX} function:cppermodultonew ${COLOR_SUFFIX}"
	echo "LOCAL_MODULE:${LOCAL_MODULE}"
	echo "rm -rf ${CODE_APPS_FULL_PATH}/${LOCAL_MODULE}.apk"
	local tmp_store_apk_address=$(find ${STORE_APK_ADDRESS} -name ${LOCAL_MODULE}.apk)
	store_apk_address=${tmp_store_apk_address%/*}

	if [[ -d ${CODE_APPS_FULL_PATH} ]];then
		echo "999999999999--CODE_APPS_FULL_PATH:[$CODE_APPS_FULL_PATH]"
		rm -rf ${CODE_APPS_FULL_PATH}/${LOCAL_MODULE}.apk
		find ${CODE_APPS_FULL_PATH}/ -name *_Release_Note.txt|xargs rm -rf
		find ${CODE_APPS_FULL_PATH}/ -name *List_Note.txt|xargs rm -rf
		find ${CODE_APPS_FULL_PATH}/ -name  README|xargs rm -rf
		find ${CODE_APPS_FULL_PATH}/ -name *.md5|xargs rm -rf
		echo "cp -a `find ${STORE_APK_ADDRESS} -name ${LOCAL_MODULE}.apk` ${CODE_APPS_FULL_PATH}/"
		cp -a `find ${STORE_APK_ADDRESS} -name ${LOCAL_MODULE}.apk` ${CODE_APPS_FULL_PATH}/
	else
		echo "ss-----mkdir {CODE_APPS_FULL_PATH}:[mkdir ${CODE_APPS_FULL_PATH}]"
		mkdir ${CODE_APPS_FULL_PATH}
		cp -a `find ${STORE_APK_ADDRESS} -name ${LOCAL_MODULE}.apk` ${CODE_APPS_FULL_PATH}/
		#新加文件路径: +如果有新加模块的时候 ,从公有文件夹如果存在android.mk,则拷贝私有文件夹，如果不存在则新生成
		if [[ -n $(cat ${STORE_APK_ADDRESS}/${LOCAL_MODULE}_public_path_list) ]];then
				local tem_CODE_APPS_FULL_PATH=$(sed -n '1p' ${STORE_APK_ADDRESS}/${LOCAL_MODULE}_public_path_list)
				tem_CODE_APPS_FULL_PATH=${tem_CODE_APPS_FULL_PATH%/*}
			if [[ -n $(find ${tem_CODE_APPS_FULL_PATH} -name Android.mk) ]];then	
				cp -a ${tem_CODE_APPS_FULL_PATH}/Android.mk ${CODE_APPS_FULL_PATH}/
			fi
		fi
			gedit ${CODE_APPS_FULL_PATH}/Android.mk
			gedit ${ANDROID_PATH}/gionee/config/gn_common.mk
			#
			read -p "有新加模块${LOCAL_MODULE}，请输入Android.mk和修改gn_common.mk文件内容并保存，如无误，请回车继续" answer < /dev/tty
	fi
	
}

function updatepermodultonew2()
{
	echo -e "${COLOR_GREEN_PREFIX} function:updatepermodultonew2 ${COLOR_SUFFIX}"
	echo "CODE_APPS_FULL_PATH:[${CODE_APPS_FULL_PATH}]"
	pushd ${CODE_APPS_FULL_PATH} >/dev/null
	if [ -f Android.mk ];then
		sed -i '/PRODUCT_COPY_FILES/d' Android.mk
		sed -i '/$(LOCAL_PATH)/d' Android.mk
		sed -i '/$(BUILD_PREBUILT)/d' Android.mk 
		sed -i '$d' Android.mk
		if [[ -n $(find . -name "*.so") ||  $(find . -name "*.ini") || $(find . -name "*.dat") || $(find . -name "*.txt") ]];then
		echo 'PRODUCT_COPY_FILES +=\' >> Android.mk
		fi
		#过滤出.so文件并截取
		findConfigFile "so"
		findConfigFile "dat"
		findConfigFile "txt"
		findConfigFile "ini"
		echo "" >> Android.mk
		echo "include \$(BUILD_PREBUILT)" >> Android.mk
		echo "endif" >> Android.mk
		echo "${LOCAL_MODULE}_Android.mk,内容如下："
		echo '******************'
		cat Android.mk
	else
		read -p "Android.mk不存在，请确认，处理好后请回车继续" answer < /dev/tty
	fi
	popd >/dev/null
}

function findConfigFile()
{
	local DirType=""
	if [ "$1" = "so" ];then
		DirType="lib"
	else
		DirType="etc"
	fi
	while read perlibname
	do
		Tem_PRODUCT_COPY_FILES=${perlibname#*/}
		echo "\$(LOCAL_PATH)/${Tem_PRODUCT_COPY_FILES}:system/${DirType}/${Tem_PRODUCT_COPY_FILES} \\"  >> Android.mk
	done < <(find . -name "*.${1}" -print) 
}

function creatDevBranch ()
{ 	
	
	echo -e "${COLOR_GREEN_PREFIX} function:creatDevBranch ${COLOR_SUFFIX}"
	runRepoCommond "git fetch"
	if [ $? -ne 0 ]; then
        errorAction
    fi
	runRepoCommond "git reset HEAD"
	if [ $? -ne 0 ]; then
        errorAction
    fi
	runRepoCommond "git checkout ."
	if [ $? -ne 0 ]; then
        errorAction
    fi
	runRepoCommond "git clean -dfx"
	if [ $? -ne 0 ]; then
        errorAction
    fi
	runRepoCommond "git checkout -b ${DEV_BRANCH} origin/${PROJECT_BRANCH}"
	if [ $? -ne 0 ]; then
        errorAction
    fi
	runRepoCommond "git status"
}
function getRepoProjectList()
{
	local i=0
	local repoProjectListPatch="repo_project_list.txt"
	repo info | grep "Mount path: " > $repoProjectListPatch
	if [ $? -eq 0 ];then
		echo " 当前是 Repo 代码库！"
		IS_REPO="YES"
	else
		echo " 当前是独立的一个 git 代码库！"
		IS_REPO="NO"
	fi
	while read project_list
	do
		ARR_REPO_PROJECT_PATCHLIST[i]=$(echo $project_list | awk -F "Mount path: " '{printf("%s", $2)}')
		i=$(($i + 1))
	done < $repoProjectListPatch
	echo "ARR_REPO_PROJECT_PATCH	#HLIST len:[${#ARR_REPO_PROJECT_PATCHLIST[@]}]"
	rm $repoProjectListPatch
}


function runGitCommondInPerProject()
{
	for projects in ${ARR_REPO_PROJECT_PATCHLIST[*]}
	do
		pushd ${projects} #>/dev/null
		if [[ -n $(echo $1 | grep 'reset HEAD') || -n $(echo $1 | grep 'clean -dfx') ]];then
			echo -e "${COLOR_GREEN_PREFIX}[$1]>/dev/null]${COLOR_SUFFIX}"
			$1 >/dev/null
		elif [[ -n $(echo $1 | grep 'commit -m') ]];then
 			if [[ -z $(git status . | grep 'nothing to commit') && -z $(git status . | grep '无文件要提交') ]];then
				echo -e "${COLOR_GREEN_PREFIX}[$1]${COLOR_SUFFIX}"
				$1 #>/dev/null
			fi
		else
			echo "in ${projects} run git commond:"
			echo -e "${COLOR_GREEN_PREFIX}[$1]${COLOR_SUFFIX}"
			$1
		fi
		local ret=$?
		popd >/dev/null
		if [ $ret -ne 0 ];then
			return $ret
		fi
	done
}

function runRepoCommond()
{
	if [[ $IS_REPO = "YES" ]];then
		runGitCommondInPerProject "$1"
	else
		if [[ -n $(echo $1 | grep 'reset HEAD') || -n $(echo $1 | grep 'clean -dfx') ]];then
			echo "$1 >/dev/null"
			$1 >/dev/null
		else
			echo "$1"
			$1
		fi
	fi
	return $?
}

function pushApps()
{
echo -e "${COLOR_GREEN_PREFIX} function pushApps ${COLOR_SUFFIX}"
runRepoCommond "git fetch"
if [ $? -ne 0 ]; then
    errorAction
fi
runRepoCommond "git branch"
if [ $? -ne 0 ]; then
    errorAction
fi
runRepoCommond "git rebase origin/${PROJECT_BRANCH}"
if [ $? -ne 0 ]; then
    errorAction
fi
read -p "请确认Android.mk文件是否正确，如无误，请按回车继续执行push操作!" aa
runRepoCommond "git push origin ${DEV_BRANCH}:${PROJECT_BRANCH}"
if [ $? -ne 0 ]; then
    errorAction
fi
}

function get_input_List
{
local i=0
local j=""
for j in $1
do
	i=$(($i+1))
	j="$i :"$j
	echo $j

	arra[$i]=$j
done
}

function main()
{
	SCRIPT_MODEL="dev"
	if [[ ${SCRIPT_MODEL} = "test" ]];then
		SELECT_PROJECT_NAME="BBL7515A;BBL7515C;BBL7516A"
		PROJECT_BRANCH="code_main_dev"
		DEV_BRANCH="DEV_BRANCH_1706060443"
		STORE_APK_ADDRESS="/tmp/DEV_BRANCH_1706060443"
		RECORD_ID="65598"
	elif [[ ${SCRIPT_MODEL} = "dev" ]];then
	 	if [[  ! -f ../APK_UPDATE_CONFIG.txt ]];then
			local tmpStrings=`cat<<ALAA
#*************************************************\n		
		#需更新APK的项目名称为:\n
		SELECT_PROJECT_NAME=""\n
        #----\n
		#主干分支名称:code_main_dev\n
		PROJECT_BRANCH=""\n
        #---\n
		#Amigo_Account,Amigo_Account_01_4.2.1_V2.2.0.c.zip;Ami_Weather,Ami_Weather_01_V0.0.0.t28.zip\n
      	
		#此处只列举不包含三方包的ftp上的apk名和压缩包名.zip；三方包的提交。请按后边脚本中的提示执行.\n
		#此参数的输入格式为：APK名;APK版本号;APK名;APK版本号(注意:此处不包含三方包名,结尾无分号),\n
		GET_APK_INFO=""\n
        #---\n
		#格式#65010,#65598请不要有空格\n
		RECORD_ID=""\n
#**************************************************\n
ALAA` 
			echo -e $tmpStrings > ../APK_UPDATE_CONFIG.txt
			
		fi
		gedit ../APK_UPDATE_CONFIG.txt
		read -p "输入APK_UPDATE_CONFIG配置文件并保存后，点击回车继续" ddd
		source ../APK_UPDATE_CONFIG.txt
	else 
		while true 
		do	
			echo -e "${COLOR_GREEN_PREFIX} "请将此脚本放到android根目录执行"${COLOR_SUFFIX}"
			echo "eg : 放到~/G1605A/L28_6737M_65_G1605A_M0.MP1_V2.140.3_160908_ALPS/android_mtk_6737m_65_mp目录下 "
			echo ""
			echo "-----------------------------------START----------------------------------------!"
			echo -e "${COLOR_WARNING_PREFIX} 候选项目如下: ${COLOR_SUFFIX}"
			get_input_List "`./TmakeGionee -l`"
			echo ""
			read -p "请输入项目名序号,如有多个用分号隔开 eg: 1;2 请输入 :"  project_ids

			while read i
			do
				PROJECT_NAME=${arra[$i]#*:}
				#SELECT_PROJECT_NAME[i]=${PROJECT_NAME}
				SELECT_PROJECT_NAME=${PROJECT_NAME}\;${SELECT_PROJECT_NAME}
			done < <(echo ${project_ids} |awk -F';' '{split($0,arr,";")}END{for(i=1;i<=NF;i++)printf("%s\n",arr[i])}')
			echo "SELECT_PROJECT_NAME:[${SELECT_PROJECT_NAME}]"
			echo -e "${COLOR_WARNING_PREFIX} 候选主干分支名如下: ${COLOR_SUFFIX}"
			get_input_List "$(git branch -r| tr '[:upper:]' '[:lower:]'))"
			echo ""
			read -p "请输入主干分支名序号eg:1,请输入 :" i
			PROJECT_BRANCH=${arra[$i]#*/}
			echo ""
			#read -p "请输入开发分支名 eg:G1605A_57612_liuxh_20170309 :" DEV_BRANCH
			echo -e "${COLOR_WARNING_PREFIX} \"您要下载的APK名,ftp_APK的压缩包名\" ${COLOR_SUFFIX}"
			echo "Amigo_Account,Amigo_Account_01_4.2.1_V2.2.0.c.zip;Ami_Weather,Ami_Weather_01_V0.0.0.t28.zip"
			echo ""
			echo "此处只列举不包含三方包的ftp上的apk名和压缩包名.zip；三方包的提交。请按后边脚本中的提示执行."
			read -p "此参数的输入格式为：APK名;APK版本号;APK名;APK版本号(注意:此处不包含三方包名,结尾无分号),如上例子: " GET_APK_INFO
			echo -e "${COLOR_WARNING_PREFIX} 请输入GNNCR id or GNSPR id ${COLOR_SUFFIX}"
			echo ""
			read -p "格式'#65010,#65598请不要有空格 :" RECORD_ID
			echo  <<ALAA \
"*************************************************
请确认输入内容无误：
需更新APK的项目名称为: ${SELECT_PROJECT_NAME}
主干分支名称: $PROJECT_BRANCH
APK名,ftp_APK的压缩包名: $GET_APK_INFO
GNNCR id or GNSPR id: $RECORD_ID
**************************************************"
ALAA
			read -p "以上信息正确请输入"0"继续，否则输入1：" alaflag_yes
			if [ ${alaflag_yes} = "1" ];then
				continue
			fi
			break
		done
	fi

	#脚本模式选择结束
	DEV_BRANCH=$(echo ${SELECT_PROJECT_NAME}|sed 's/;/_/g')_${RECORD_ID##*#}_dev_$(date +%y%m%d%M%S)


				echo  <<ALAA \
"*************************************************
请确认输入内容无误：
需更新APK的项目名称为: ${SELECT_PROJECT_NAME}
主干分支名称: $PROJECT_BRANCH
APK名,ftp_APK的压缩包名: $GET_APK_INFO
GNNCR id or GNSPR id: $RECORD_ID
**************************************************"
ALAA
	ANDROID_PATH=`pwd`
	echo "ANDROID_PATH:[${ANDROID_PATH}]"
	PACKAGE_PATH=${ANDROID_PATH}/../$(ls ${ANDROID_PATH}/../ | grep "packages")
	echo "PACKAGE_PATH:[${PACKAGE_PATH}]"	
	getRepoProjectList
#	<<alaa
	creatDevBranch
	gitAndUnzipApkFileFromFTP
	echo "请确认是否有3方包提交："
	echo "如果有，请规范包名等相关信息并手动拷入${STORE_APK_ADDRESS}路径下，然后回车继续"
	echo "如果没有，请直接回车继续"
	read -p "确认后请按回车继续" 
#alaa
	getGioneeAppsPathAndUpdaToNew
	#rm -rf ${STORE_APK_ADDRESS}
	      echo "-----------------------------------END---------------------------------------!"
   #pushApps
	
	
	
}

main
