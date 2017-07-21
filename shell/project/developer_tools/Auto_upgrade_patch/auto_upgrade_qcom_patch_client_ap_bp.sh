#!/bin/bash
PLATFORM=""
GN_ANDROID_PATH=""
GN_ANDROID_GIONEE_PATH=""
GN_PACKAGES_NAME=""
GN_PATCH_PATH=""
GN_PACKAGES_GIONEE_PATH=""
MODIFYLIST=""
DELETELIST=""
ADDLIST=""
GN_MODIFYLIST=""
PATCHTYPE=""
MODIFYLIST_PACKAGE=""
MODIFYLIST_ANROID=""
MODIFYLIST_ANROIDGIONEE=""
GN_DEV_BRANCH=""
QCOM_MAIN_BRANCH="qcom_original_code"
IS_REPO=""
#用于保存 repo 库中所有子库的路径
ARR_REPO_PROJECT_LIST=("")
FTP_PATCH_TAG=""
PATCH_TYPE=""
PATCH_BUILD_VERNO=""
PATCH_WEEK_NO=""
ALA_PARAMETER=""
BASE_PROJECT=""

#ALA ADD
#version:SV1.3.2
#DATE:2017.7.12
#适用与高通项目并与auto_update_qc_patch_server.sh配合使用
#ALA END

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



function initLocalVar()
{
	echo -e "${COLOR_GREEN_PREFIX} function initLocalVar ${COLOR_SUFFIX}"
	GN_PATCH_PATH="/tmp/${PROJECT_LABEL}_patch"
	if [[ -z ${GN_PATCH_PATH} || ! -e ${GN_PATCH_PATH} ]];then
		mkdir ${GN_PATCH_PATH}
	else
		rm -rf ${GN_PATCH_PATH}/*
	fi
	echo "${GN_ANDROID_PATH}"
	GN_ANDROID_PATH=$(pwd)
	local GN_PACKAGES_NAME=$(ls ${GN_ANDROID_PATH}/../ | grep "packages")
	GN_PACKAGES_PATH="${GN_ANDROID_PATH}/../${GN_PACKAGES_NAME}"
	GN_PACKAGES_GIONEE_PATH="${GN_PACKAGES_PATH}/gionee"
	GN_ANDROID_GIONEE_PATH="GN_ANDROID_PATH/gionee"
	FTP_PATCH_IP="10.8.10.22"
	PATCH_FTP_PATH="ftp://10.8.10.22/GNBJ_PATCH_CONTROL/Gionee/"
	
	PATCH_FILE_FTP_PATH="ftp://10.8.10.22/GNBJ_PATCH_CONTROL/Gionee/QCOM/${PLATFORM}"
	echo "GN_PACKAGES_NAME:[${GN_PACKAGES_NAME}]"

}


function getRepoProjectList()
{
#	repo sync
	echo -e "${COLOR_GREEN_PREFIX} function:getRepoProjectList ${COLOR_SUFFIX}"
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
		ARR_REPO_PROJECT_LIST[i]=$(echo $project_list | awk -F "Mount path: " '{printf("%s", $2)}')
		i=$(($i + 1))
	done < $repoProjectListPatch
	echo "ARR_REPO_PROJECT_LIST	#HLIST len:[${#ARR_REPO_PROJECT_LIST[@]}]"
	rm $repoProjectListPatch
}

#在每个git库中执行git命令，为了解决repo命令会暂停（暂停会导致自动化编译不能进行）的问题。
function runGitCommondInPerProject()
{
	#echo -e "${COLOR_GREEN_PREFIX} function:runGitCommondInPerProject ${COLOR_SUFFIX}"
	for projects in ${ARR_REPO_PROJECT_LIST[*]}
	do
		pushd ${projects} >/dev/null
		if [[ -n $(echo $1 | grep 'reset HEAD') || -n $(echo $1 | grep 'clean -dfx') ]];then
			echo "in $projects run git commond:"
			echo -e "${COLOR_GREEN_PREFIX}[$1 >/dev/null]${COLOR_SUFFIX}"
			$1 >/dev/null
		else
			echo "in ${projects} run git commond:"
			echo -e "${COLOR_GREEN_PREFIX}[$1]${COLOR_SUFFIX}"
			$1
		fi
		local ret=$?
		popd >/dev/null
		if [ $ret -ne 0 ]; then
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

function errorAction()
{	

	#判断如果是由jenkins自动化系统调用时，则直接退出，不能暂停
	if [[ ${JENKINS_AUTO_COMPILER_FLAG} = "TRUE" ]];then
		echo -e "${COLOR_ERROR_PREFIX} !!! 当前的操作有错误 !!! ${COLOR_SUFFIX}"
		exit 2
	fi
	echo -e "${COLOR_ERROR_PREFIX}当前的操作有错误，请在弹出的新窗口中处理完成错误之后再继续.....${COLOR_SUFFIX}"
	read -p <<ALA \
"当前的操作有错误，请在弹出的新窗口中处理完成错误之后再继续,
回车键继续...."
ALA
	#打开一个新的终端，在新的窗口中处理错误
	gnome-terminal
	read -p "请在新窗口中处理完错误之后，回车键将继续执行...."
}

function creatDevBranch ()
{ 	
	echo -e "${COLOR_GREEN_PREFIX} function:creatDevBranch ${COLOR_SUFFIX}"
	pushd ${GN_ANDROID_PATH}
	echo "GN_ANDROID_PATH:${GN_ANDROID_PATH}"
	GN_DEV_BRANCH=${PLATFORM}_${PATCH_ID}_patch_dev_$(date +%y%m%d%M%S)
	echo "GN_DEV_BRANCH:[$GN_DEV_BRANCH]"
	getRepoProjectList
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
	runRepoCommond "git checkout -b ${GN_DEV_BRANCH} ${PROJECT_LABEL}"
	if [ $? -ne 0 ]; then
        errorAction
    fi
	runRepoCommond "git status"
	echo "T{PATCH_ID} T${PATCH_ID} "
	pwd
	FTP_PATCH_TAG=$(git tag|grep "${PATCH_ID}"|awk -F "_P" '{printf("%s", $1)}')

	echo  "FTP_PATCH_TAG: [${FTP_PATCH_TAG}]"

}


function gitPatchFileFromFTP()
{
	echo -e "${COLOR_GREEN_PREFIX} function gitPatchFileFromFTP ${COLOR_SUFFIX}"
	pushd ${GN_PATCH_PATH} >/dev/null
	rm -rf ${GN_PATCH_PATH}/*
	
	if [[ ${PATCH_TYPE} = "bp" ]];then
		wget  ${PATCH_FILE_FTP_PATH}/${FTP_PATCH_TAG}/*_MODEM.7z -P ${GN_PATCH_PATH}
		if [ $? -ne 0 ];then
			echo -e "${COLOR_ERROR_PREFIX} ${PATCH_FILE_FTP_PATH}/${FTP_PATCH_TAG}* has not found! ${COLOR_SUFFIX}"
			exit 1
		fi
	elif [[ ${PATCH_TYPE} = "ap" ]];then
		wget  ${PATCH_FILE_FTP_PATH}/${FTP_PATCH_TAG}/*_ALPS.7z -P ${GN_PATCH_PATH}
		if [ $? -ne 0 ];then
			echo -e "${COLOR_ERROR_PREFIX} ${PATCH_FILE_FTP_PATH}/${FTP_PATCH_TAG}* has not found! ${COLOR_SUFFIX}"
			exit 1
		fi
	else
		echo "请确认要打的patch是上层还是modem，输入有误！！！"
	fi

	pwd
	ls ./ >../7z_list.txt
	while  read lines
	do 
		echo "lines:${lines}333"
		pwd
		7z x ${lines} -o./
	    if [ $? -ne 0 ];then
		    echo -e "${COLOR_ERROR_PREFIX} ${FTP_PATCH_TAG}* 解压失败，请检查！${COLOR_SUFFIX}"
		    exit 1
	    fi
		echo "rm ${lines}"
	done < <(cat ../7z_list.txt)
	rm ../7z_list.txt
	rm ${GN_PATCH_PATH}/*.7z

	popd >/dev/null

}
function gitrmDelFile()
{
	if [[ -d ${GN_PATCH_PATH}/$1_diff ]];then
        rm -rf ${GN_PATCH_PATH}/$1_diff/*
    else
        mkdir ${GN_PATCH_PATH}/$1_diff
    fi
    pushd ${GN_PATCH_PATH}/$1_diff >/dev/null
    echo "${1} gionee目录自动patch,冲突文件列表:" > $1_GN_PATCH_REJ_LIST.txt
    echo "${1} gionee目录自动patch,失败文件列表:" >  $1_GN_PATCH_FAIL_LIST.txt  
    echo "${1} gionee目录自动patch,成功文件列表:"  > $1_GN_PATCH_SUCCESS_LIST.txt
    echo "${1} gionee目录自动patch,二进制文件文件列表:" >  $1_GN_PATCH_BINARY_LIST.txt 
    echo "${1} gionee目录自动patch,不做处理文件列表，例如相机效果文件:" >  $1_GN_PATCH_IGNORE_LIST.txt 
    echo "${1} gionee目录自动patch,删除库文件列表:"  > $1_GN_PATCH_LIB_DEL_LIST.txt 
    echo "${1} gionee目录自动patch,内容没变文件列表:"  > $1_GN_PATCH_SAME_LIST.txt
    echo "${1} gionee目录文件夹存在的同名文件列表:"  > $1_gionee_LIST.txt
    echo "${1} gionee目录文件夹存在的同名删除文件列表:" > $1_gionee_delfile_LIST.txt
    popd >/dev/null
   
    while read delline
    do
            delline=$(echo ${delline}|awk '{print $2}')
            #local tem_delline=${3}/${delline}
            pushd $3 >/dev/null
            
           
            git rm ${delline}
            if [ $? -ne 0 ]; then
	            gnome-terminal </dev/null &
	            read -p "删除文件出错请确认，然后按回车继续！" answer</dev/tty  
            fi            
            local temp=$(find ./gionee/ -name "*"|grep --exclude-dir=".git" "${delline}")
            if [[ -n $temp ]];then
           		echo $temp >> ${GN_PATCH_PATH}/$1_diff/$1_gionee_delfile_LIST.txt
            fi
            popd >/dev/null
      done < <(sed -n "/D\t/P" $(find . -maxdepth 1 -name "GNP_*.txt" |grep _$2.txt))         
      if [[ -n $(sed -n "2p" ${GN_PATCH_PATH}/$1_diff/$1_gionee_delfile_LIST.txt) ]];then     	  
            echo -e "${COLOR_GREEN_PREFIX} 请确认需删除的gionee下同名文件，并cd $3/gionee目录执行以下删除命令！ ${COLOR_SUFFIX}"           
            pushd $3/gionee >/dev/null
            sed -n '2,$P' ${GN_PATCH_PATH}/$1_diff/$1_gionee_delfile_LIST.txt|sed  "s#./gionee/# git rm  #g"
            echo ""
            gnome-terminal </dev/null &
            read -p "确认已处理,请按回车继续！" answer</dev/tty  
            git add -u
            popd >/dev/null
      fi

}

function checkDeleteFileFromPatch()
{
    echo -e "${COLOR_GREEN_PREFIX} function checkDeleteFileFromPatch ${COLOR_SUFFIX}"
    pushd ${GN_PATCH_PATH} >/dev/null
    while read per_xls
    do   		
        if [[ -n $(find . -maxdepth 1 -name ${per_xls##*/} |grep _ALPS.txt) ]];then
        	echo "alps"
        	gitrmDelFile "alps" "ALPS" "${GN_ANDROID_PATH}"
        elif [[ -n $(find . -maxdepth 1 -name ${per_xls##*/} |grep _PACKAGES.txt) ]];then
        	echo "packages"
        	gitrmDelFile "packages" "PACKAGES" "${GN_PACKAGES_PATH}"
        elif [[ -n $(find . -maxdepth 1 -name ${per_xls##*/} |grep _MODEM.txt) ]];then
        	echo "modem"
			gitrmDelFile "modem" "MODEM" "${GN_ANDROID_PATH}"
        else
        	echo "删除清单文件有误，请确认！"
        fi   
    done < <(find . -maxdepth 1 -name "GNP_*.txt")		
    popd >/dev/null
    echo -e "${COLOR_GREEN_PREFIX} 如无误，请按回车继续 ${COLOR_SUFFIX}"
}


function copygnrootDir()
{
	echo -e "${COLOR_GREEN_PREFIX} function copygnrootDir--$2--${COLOR_SUFFIX}"
    echo "function copygnrootDir_$1"
    if [[ -z $(find  ${GN_PATCH_PATH} -name "GNP_*$3") ]];then
        echo -e "${COLOR_WARNING_PREFIX} file ${GN_PATCH_PATH}/*$3 does not exsit!${COLOR_SUFFIX}"
   		return
    fi
    pushd ${GN_PATCH_PATH} >/dev/null
    cat $(ls|grep "$3")|awk -F "A\t" '{print $2}'| sed "/^$/d" > $2_modifyfile.txt
    cat $(ls|grep "$3")|awk -F "M\t" '{print $2}'| sed "/^$/d" >> $2_modifyfile.txt
    cat $2_modifyfile.txt
    echo "cp -a $2/* $1/"
    cp -a $2/* $1/
    if [ $? -ne 0 ]; then
        echo "cp -a $2/* $1/ error!!!"
        exit 1
    fi

    popd >/dev/null
}

function patchGioneeDiffs() 
{
	local compare_app=bcompare
	echo -e "${COLOR_GREEN_PREFIX} function generateAndroidDiffs--$1-- ${COLOR_SUFFIX}"
	 if [[ -z $(find  ${GN_PATCH_PATH} -name "GNP_*$3") ]];then
        echo -e "${COLOR_WARNING_PREFIX} file ${GN_PATCH_PATH}/*$3 does not exsit!${COLOR_SUFFIX}"
   		return
    fi
    pushd $2> /dev/null
    git add -A
    while read line 
    do
        echo "line:[$line]"
        local difffile="$2/${line}"
        local diffdir=$(dirname ${difffile})
        local diffname=$(basename ${difffile})      
        echo "gionee同名文件---查找中---"
        local temp=$(find ./gionee/ -name "${diffname}"|grep --exclude-dir=".git" "${line}")
        if [[ -n $temp ]];then
        	echo "包含关系路径---查找中---"
	       	#判断 gionee diff文件可能是否存在包含关系的的路径导致误打--begin"
			touch ${GN_PATCH_PATH}/$1_diff/${diffname%%.*}_gionee_same.txt
			echo "--line:${line}----------"
			find . -name "${diffname}"|grep --exclude-dir=".git" "${line}"|grep -v gionee >${GN_PATCH_PATH}/$1_diff/$1_same.txt
			cat ${GN_PATCH_PATH}/$1_diff/$1_same.txt|awk -F "^./" '{print $2}'|grep -v "^${line}" >${GN_PATCH_PATH}/$1_diff/$1_same.txttmp
			cat ${GN_PATCH_PATH}/$1_diff/$1_same.txttmp > ${GN_PATCH_PATH}/$1_diff/$1_same.txt
			if [[ -n $(cat ${GN_PATCH_PATH}/$1_diff/$1_same.txt) ]];then
				#存在包含关系路径的情况，如果gionee路径下文件匹配到有问题的路径，则不添加此路径到最终列表，否则添加
				while read pertemp
				do
					local include_flag="n"
						for i in $(cat ${GN_PATCH_PATH}/$1_diff/$1_same.txt)
						do
							if [[ -n $(echo ${pertemp} |grep "$i") ]];then	
								include_flag="y"
								break
							fi
						done
						if [[ ${include_flag} = "n" ]];then
						echo ${pertemp} >>${GN_PATCH_PATH}/$1_diff/${diffname%%.*}_gionee_same.txt
						fi
				done < <(find ./gionee/ -name "${diffname}"|grep --exclude-dir=".git" "${line}")
			rm ${GN_PATCH_PATH}/$1_diff/$1_same.txt
			fi
			#判断 gionee diff文件可能是否存在包含关系的的路径导致误打--end"
			echo "gionee存在同名文件列表如下:"
        	cat ${GN_PATCH_PATH}/$1_diff/${diffname%%.*}_gionee_same.txt >> ${GN_PATCH_PATH}/$1_diff/$1_gionee_LIST.txt
	        pushd ${diffdir} >/dev/null
	        if [[ $? -ne 0 ]];then
	        	echo "${diffdir}不存在请确认"
	        	exit 1
	        fi
	        git diff --cached ${diffname} > ${diffname}.diff
	        popd >/dev/null
	        while read gnline
	        do
	        	echo "gnline:[${gnline}]"
	            local gndifffile="$2/${gnline}"
	            local gndiffdir=$(dirname ${gndifffile})
	            local gndiffname=$(basename ${gndifffile})
	            pwd
	            file -b ${gnline}
	            if [[ -z $(file -b ${gnline} | grep ' text') ]]; then
		            # 二进制文件
		            echo -e "${COLOR_WARNING_PREFIX}二进制文件: [${gnline}]${COLOR_SUFFIX}"
		            if [[ ${gnline} == *.so ]]; then
		                pushd ${gndiffdir} > /dev/null
		                if [[ -z $(diff $2/${difffile} $2/${gndifffile}) ]];then
		                	echo ${gnline} >> ${GN_PATCH_PATH}/$1_diff/$1_GN_PATCH_LIB_DEL_LIST.txt
		                	git rm -rf $2/${gndifffile}
		                	echo "$2/${gndifffile}gionee目录与根目录相同，故已删除" >> ${GN_PATCH_PATH}/$1_diff/$1_GN_PATCH_LIB_DEL_LIST.txt
		            	else
		            		echo ${gnline}---有更新请手动处理 >> ${GN_PATCH_PATH}/$1_diff/$1_GN_PATCH_BINARY_LIST.txt
		                	git log -n1 ${gndifffile} >> ${GN_PATCH_PATH}/$1_diff/$1_GN_PATCH_BINARY_LIST.txt
		                	echo "------" >> ${GN_PATCH_PATH}/$1_diff/$1_GN_PATCH_BINARY_LIST.txt
		                fi
		            else
		                echo ${gnline}---有更新请手动处理 >> ${GN_PATCH_PATH}/$1_diff/$1_GN_PATCH_BINARY_LIST.txt
		                pushd ${gndiffdir} > /dev/null
		                git log -n1 ${gndifffile}>> ${GN_PATCH_PATH}/$1_diff/$1_GN_PATCH_BINARY_LIST.txt
		                echo "------" >> ${GN_PATCH_PATH}/$1_diff/$1_GN_PATCH_BINARY_LIST.txtS
		            fi
	                popd > /dev/null
	                continue
		        fi
		        #相机效果文件不做处理，列举即可
	            if [[ ${gnline} == */chromatix/* ]]; then
	            	echo "gnline == chromatix"
	            	echo ${gnline} >> ${GN_PATCH_PATH}/$1_diff/$1_GN_PATCH_IGNORE_LIST.txt
	             	pushd ${gndiffdir} > /dev/null
	                git log -n1 ${gndifffile}>> ${GN_PATCH_PATH}/$1_diff/$1_GN_PATCH_IGNORE_LIST.txt
	                echo >> ${BINARY_LIST}
	                popd > /dev/null
	            	continue
	        	fi
	            pushd ${gndiffdir} >/dev/null
	            ls
	            cat ${difffile}.diff
	            echo  "patch ${gndiffname} ${difffile}.diff"

	            patch ${gndiffname} ${difffile}.diff
	           
	            local ret=$?
                if [ ${ret} -ne 0 ];then
                    echo -e "${COLOR_ERROR_PREFIX}执行patch命令失败，执行结果:${ret} ${COLOR_SUFFIX}"
                    echo ${gndifffile} >> ${GN_PATCH_PATH}/$1_diff/$1_GN_PATCH_FAIL_LIST.txt
                    if [ ${ret} -eq 2 ]; then
                        echo ${gndifffile} >> ${GN_PATCH_PATH}/$1_diff/$1_GN_PATCH_SAME_LIST.txt
                    fi
                else
                    echo -e "${COLOR_GREEN_PREFIX}已成功打入: [${gndifffile}]${COLOR_SUFFIX}"
                    echo ${gndifffile} >> ${GN_PATCH_PATH}/$1_diff/$1_GN_PATCH_SUCCESS_LIST.txt
                    git add ${gndifffile}
                    if [ -f ${gndifffile}.orig ];then
                    rm ${gndifffile}.orig
                	fi
                    popd > /dev/null
                    continue
                fi
                local answer=""
                if [ -f ${gndifffile}.rej ];then
                    read -p "手动 merge 以上文件请输入 Y 或 y :"  answer < /dev/tty
                    if [[ ${answer} = "y" || ${answer} = "Y" ]]; then
                       gedit ${gndifffile}.rej </dev/null &
	                   gedit ${difffile}.diff </dev/null &
	                    ${compare_app} ${difffile} ${gndifffile}

	                    echo "diff文件: ${difffile}.diff-------rej文件: ${gndifffile}.rej已手动处理" >> ${GN_PATCH_PATH}/$1_diff/$1_GN_PATCH_REJ_LIST.txt
	                    git add ${gndifffile}
	                    rm ${gndifffile}.rej
	                 else
	                 	echo "diff文件: ${difffile}.diff-----rej文件: ${gndifffile}.rej未做处理" >> ${GN_PATCH_PATH}/$1_diff/$1_GN_PATCH_REJ_LIST.txt
                    fi
                    
                fi
                if [ -f ${gndifffile}.orig ];then
                    rm ${gndifffile}.orig
                fi
            	popd >/dev/null
	   		done < <(cat ${GN_PATCH_PATH}/$1_diff/${diffname%%.*}_gionee_same.txt)
            rm ${difffile}.diff
		    if [[ $? -ne 0 ]];then
	        	echo "rm ${difffile}.diff fail !!!"
	        	exit 1
			fi
        else
        	echo "gionee不存在同名文件${difffile}"
        fi
   	done <${GN_PATCH_PATH}/$1_modifyfile.txt

    while read linetxt
    do
    	if [[ -n $(sed -n "2p" ${linetxt}) ]];then
    		echo "linetxt  :${linetxt}"
    		gedit $linetxt </dev/null &
    		echo "如已确认并处理好以上文件信息，请按回车继续... 可在以下路径找到此文件:$linetxt"
    		read ppp </dev/tty 
		else
			rm ${linetxt}
    	fi
    done < <(find ${GN_PATCH_PATH}/$1_diff/ -type f)
    git status ${2}/gionee
    popd >/dev/null
}



function modifyPatchListAPPXls()
{
	echo "function modifyPatchListAPPXls"
	pushd ${GN_PATCH_PATH} >/dev/null
	while read line
	do
		echo -e "${COLOR_WARNING_PREFIX} 请手动修改 PatchList_APP.xls！ ${COLOR_SUFFIX}"
		gedit ${line} &
		libreoffice ${GN_ANDROID_PATH}/PatchList_APP.xls
	done < <(find . -maxdepth 1 -type f)
	popd >/dev/null
}

function addAndCommitCode()
{ 	
	echo -e "${COLOR_GREEN_PREFIX} function:addAndCommitCode ${COLOR_SUFFIX}"
	pushd ${GN_ANDROID_PATH}
	echo "GN_ANDROID_PATH:${GN_ANDROID_PATH}"
	echo "GN_DEV_BRANCH:[$GN_DEV_BRANCH]"
	getRepoProjectList
	rm repo_project_list.txt
	runRepoCommond "git status"
	if [ $? -ne 0 ]; then
        errorAction
    fi
    read -p "如已确认无误,则请按回车进行代码git commit"
    repo forall -cvp git commit -m "patch Fixed ${RECORD_ID} for ${PLATFORM}_PATCH${PATCH_ID}"
	if [ $? -ne 0 ]; then
        errorAction
    fi
    runRepoCommond "git status"
	if [ $? -ne 0 ]; then
        errorAction
    fi
	echo -e "${COLOR_GREEN_PREFIX} 本地代码编译及验证,确认无误后手动push到对应开发分支，命令为：repo forall -cvp git push origin ${GN_DEV_BRANCH}:${PROJECT_BRANCH}，辛苦了谢谢！${COLOR_SUFFIX} "

}

function anonymousFtp()
{
ftp -i -n<<!
open 10.8.10.22
user anonymous liuxh@gionee.com
cd GNBJ_PATCH_CONTROL/Gionee/$1
ls . /tmp/ala_platform
close
bye
!
}
function gitPlatformList()
{
			if [ -f /tmp/ala_platform ];then
				rm /tmp/ala_platform
			fi
			touch /tmp/ala_platform
			if [[ -n $(echo $(pwd)|grep -i MTK) ]];then
				anonymousFtp "MTK"
			else
				anonymousFtp "QCOM"
			fi
}

function get_input_List ()
{
	local i=0
	local j=""
	for j in $1
	do
		i=$(($i+1))
		j="$i :"$j
		echo $j

		arra[$i]=$j
		ARRA_SUM=${#arra[@]}
	done
}

function inputListParameterInit()
{
	echo -e "${COLOR_GREEN_PREFIX}----------------$3-------------------------${COLOR_SUFFIX}"
	local ala_list_id=""
	get_input_List "$1"
	echo -e "${COLOR_GREEN_PREFIX}$2${COLOR_SUFFIX}";read -p ": " ala_list_id
	if [[ $ala_list_id -lt "1" || $ala_list_id -gt "$ARRA_SUM " ]]; then
			echo "輸入有误,请重输入!"	
			continue
	fi
	ALA_PARAMETER=${arra[$ala_list_id]#*:}
	echo "             "
}

function getBaselabelOrBranchList()
{
	if [ -f getBaselabelOrBranchList.txt ];then
		rm getBaselabelOrBranchList.txt
	fi
	touch getBaselabelOrBranchList.txt
	echo $(git tag|grep -F "${BASE_PROJECT}"|sort -n -r|sed -n '1,5'p) |awk -F' ' '{split($0,arr," ")}END{for(i=1;i<=NF;i++)printf("%s\n",arr[i])}' >> getBaselabelOrBranchList.txt
	while read i
	do	
		if [[ -n $(echo "$i"|grep '*' ) ]];then
			echo "$i"|awk '{print $2}' >> getBaselabelOrBranchList.txt
		else
			echo "$i" >> getBaselabelOrBranchList.txt
		fi
	done < <(git branch -a|grep -v 'master')
}

function main()
{	
	SCRIPT_MODEL="qc"
	if [[ ${SCRIPT_MODEL} = "test" ]];then
		PROJECT_LABEL="BJ17G16A-T0016-170706AA"
		GN_ANDROID_PATH="/home/liuxh/ALA_DATA/17G16A/L33_QCOM_8920_17G16A_170605_ALPS/android_qcom_mp"
		PATCH_ID="T1"
		PROJECT_BRANCH="patch_main_dev"
		GN_PATCH_PATH="/tmp/BJ17G16A-T0016-170706AA_patch"
		RECORD_ID="sss"
		PLATFORM="QC8940"
		PATCH_TYPE="ap"

	elif [[ ${SCRIPT_MODEL} = "qc" ]];then
		while true 
		
			do
			echo “此脚本目前使用于---高通项目---AP BP---代码升级“
			echo “AP在android请放在根目录执行此脚本,eg:/data/G1602A_new/L29_QCT8976_G1602A_161020_ALPS/android_qc_m_qct8976”
			echo “MODEM请放在根目录运行脚本,eg:/data/MAIN_GIT_REPO_CODE/G1602A_MODEM_MAIN_REPO/L29_QCT8976_G1602A_161020_MODEM”
			if [[ -n $(echo $(pwd)|grep -i modem) ]];then
				PATCH_TYPE="bp"
			else
				PATCH_TYPE="ap"
			fi
			#read -p "如果为modem打patch请输入“bp”；如果是上层打patch请输入“ap”，输入数字即可: " PATCH_TYPE
			#显示选择label列表 begin
			echo "git fetch"
			git fetch
			#inputListParameterInit "输入列表" "注释信息"  "获取参数名"
			gitPlatformList
			inputListParameterInit "$(cat /tmp/ala_platform|awk '{print $9}')" "请选择平台名称流水号 " "平台列表"
			PLATFORM=${ALA_PARAMETER}
			inputListParameterInit "$(git tag|grep "GNP_"|awk -F "_T" '{print $2}'|awk -F "_" '{print $1}'|sort -u -r -n|sed 's/^/T/g')" "请选择高通版本更新,金立内部patch流水号 " "金立内部patch列表"
			PATCH_ID=${ALA_PARAMETER}
			inputListParameterInit "$(./TmakeGionee -l|awk -F "_" '{print $1}'| sed 's:[0-9]*$::'|grep -v "可以编译的项目名:,./makeGionee,jenkins"|sort -u|sed '/^ *$/d')" "请输入base项目名" "base项目名列表"
			BASE_PROJECT=${ALA_PARAMETER}
			getBaselabelOrBranchList
			inputListParameterInit "$(cat getBaselabelOrBranchList.txt)"  "基于哪个tag/branch建立开发分支,请选择base label/branch流水号 " "金立base_tag/branch列表"
			rm getBaselabelOrBranchList.txt
			PROJECT_LABEL=${ALA_PARAMETER}	
			inputListParameterInit "$(git branch -r)" "请选择即将推送的主干分支名流水号 " "主干分支列表"
			PROJECT_BRANCH=${ALA_PARAMETER}	
			read -p "请输入GNNCR id or GNSPR id（格式'#65010,#65598请不要有空格 ）: " RECORD_ID
echo  <<ALAA \
"*************************************************
请确认输入内容无误：

平台名称:  $PLATFORM
patch类型:  $PATCH_TYPE
金立内部patch流水号:  $PATCH_ID
GNNCR id or GNSPR id:  $RECORD_ID
base label:  $PROJECT_LABEL
主干分支名称:  $PROJECT_BRANCH
**************************************************"
ALAA
			read -p "以上信息正确请输入"0"继续，否则输入1：" alaflag_yes

			if [ ${alaflag_yes} = "1" ];then
				continue
			fi
			break
		done
	fi

	initLocalVar "$@"
	#如果当前是 repo 管理项目，则计算出各个 git 子库路径
	creatDevBranch
	gitPatchFileFromFTP
	checkDeleteFileFromPatch
	if [[ ${PATCH_TYPE} = "ap" ]];then
		copygnrootDir "${GN_ANDROID_PATH}" "alps" "_ALPS.txt"
		copygnrootDir "${GN_PACKAGES_PATH}" "packages" "_PACKAGES.txt"
		#判断如果 android/gionee 目录下存在修改的文件，则自动合并patch
		patchGioneeDiffs "alps" "${GN_ANDROID_PATH}"
		#判断如果 packages/gionee 目录下存在修改的文件，则自动合并patch
		patchGioneeDiffs  "packages" "${GN_PACKAGES_PATH}"
		addAndCommitCode
	elif [[ ${PATCH_TYPE} = "bp" ]];then
		copygnrootDir "${GN_ANDROID_PATH}" "modem" "_MODEM.txt"
		#判断如果 modem/gionee 目录下存在修改的文件，则自动合并patch
		patchGioneeDiffs "modem" "${GN_ANDROID_PATH}"
		addAndCommitCode
	else
		echo "输入有误，请再确认你要打patch类型: modem 或 上层"
		exit 1
	fi
	rm -rf ${GN_PATCH_PATH}
}

main "$@"
