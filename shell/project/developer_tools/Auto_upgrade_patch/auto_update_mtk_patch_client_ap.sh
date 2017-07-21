#!/bin/bash
#ALA ADD
#version:SV1.0
#DATE:2017.6.23
#适用与MTK_ap项目
#ALA END

MTK_ANDROID_PATH=`pwd`
MTK_PACKAGES_NAME=""
MTK_PATCH_PATH=""
MTK_PACKAGES_DIR_FLAG="FALSE"

PATCH_BUILD_VERNO=""
PATCH_WEEK_NO=""

DIFF_PATH=~/mydiff
ANDROID_DIFF_PATH=${DIFF_PATH}/android
PACKAGES_DIFF_PATH=${DIFF_PATH}/packages

#用于保存 repo 库中所有子库的路径
ARR_REPO_PROJECT_LIST=("")

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
#v

#===================================================================
#获取 repo 库中所有子库的路径
#===================================================================
function countRepoProjectList()
{
    local i=0
    local repoProjectListPatch="repo_project_list.txt"
    repo info | grep "Mount path: " > $repoProjectListPatch
    while read line
    do
        ARR_REPO_PROJECT_LIST[i]=$(echo $line | awk -F "Mount path: " '{printf("%s", $2)}')
        i=$(($i + 1))
    done < $repoProjectListPatch
    rm $repoProjectListPatch
}

#在每个git库中执行git命令，为了解决repo命令会暂停（暂停会导致自动化编译不能进行）的问题。
function runGitCommondInPerProject()
{
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

function initLocalVar()
{
    MTK_PATCH_PATH=$1
    if [[ -z ${MTK_PATCH_PATH} || ! -e ${MTK_PATCH_PATH} ]];then
        echo -e "${COLOR_ERROR_PREFIX} Dir ${MTK_PATCH_PATH} is not exsit!${COLOR_SUFFIX}"
        exit 1
    fi
    local localPackagePath=$(ls ../ | grep "packages")
    MTK_PACKAGES_NAME="${MTK_ANDROID_PATH}/../${localPackagePath}"
}

function checkDeleteFileFromPatch()
{
    echo "function checkDeleteFileFromPatch"
    pushd ${MTK_PATCH_PATH} >/dev/null
    local isDeleteFlag="FALSE"
    while read line
    do
        local result=$(grep -R "delete" ${line})
        if [[ -n ${result} ]];then
            echo -e "${COLOR_WARNING_PREFIX} 注意，当前 Patch 有删除的文件，请手动处理！ ${COLOR_SUFFIX}"
            echo -e "${COLOR_GREEN_PREFIX} ${result} ${COLOR_SUFFIX}"
            isDeleteFlag="TRUE"
            break
        fi
    done < <(find . -maxdepth 1 -type f)
    if [ "${isDeleteFlag}" = "TRUE" ];then
        #打开一个新的终端，在新的窗口中处理错误
        pushd ${MTK_ANDROID_PATH} >/dev/null
        gnome-terminal
        read -p "请在新窗口中处理完错误之后，回车键将继续执行...."
        popd >/dev/null
    fi
    popd >/dev/null
}

function getBuildVernoFromPatch()
{
    echo "function getBuildVernoFromPatch [$1]"
    #get build version number from patch
    pushd ${MTK_PATCH_PATH} >/dev/null
    local mtkProjectConfig="alps/device/gionee_bj/$1/ProjectConfig.mk"
    if [ ! -e ${mtkProjectConfig} ];then
        echo -e "${COLOR_ERROR_PREFIX} 当前不存在 ProjectConfig.mk! ${COLOR_SUFFIX}"
        exit 1
    fi
    local tmpWeekNo=$(grep "MTK_WEEK_NO" ${mtkProjectConfig} | awk -F '=' '{printf("%s",$2)}' | sed s/[[:space:]]//g)
    local tmpBuildNo=$(grep "MTK_BUILD_VERNO" ${mtkProjectConfig} | awk -F '=' '{printf("%s",$2)}' | sed s/[[:space:]]//g)
    if [[ -z ${tmpWeekNo} || -z ${tmpBuildNo} ]];then
        exit 1
    fi
    PATCH_BUILD_VERNO=${tmpBuildNo}
    PATCH_WEEK_NO=${tmpWeekNo}

    echo -e "${COLOR_GREEN_PREFIX} PATCH_BUILD_VERNO:[${PATCH_BUILD_VERNO}],PATCH_WEEK_NO:[${PATCH_WEEK_NO}]${COLOR_SUFFIX}"
    popd >/dev/null
}

function copyAndroidDir()
{
    echo "function copyAndroidDir"
    if [ ! -e ${MTK_PATCH_PATH}/alps ];then
        echo -e "${COLOR_ERROR_PREFIX} Dir ${MTK_PATCH_PATH}/alps is not exsit!${COLOR_SUFFIX}"
        exit 1
    fi
    pushd ${MTK_PATCH_PATH}/alps >/dev/null
    while read line
    do
        if [ "${line}" = "packages" ];then
            MTK_PACKAGES_DIR_FLAG="TRUE"
            echo -e "${COLOR_WARNING_PREFIX} 注意，当前 Patch 有 Packages！ ${COLOR_SUFFIX}"
            continue
        fi
        echo "cp -a ${line} ${MTK_ANDROID_PATH}"
        cp -a ${line} ${MTK_ANDROID_PATH}
    done < <(ls)
    popd >/dev/null
}

function copyPackageDir()
{
    if [ "${MTK_PACKAGES_DIR_FLAG}" != "TRUE" ];then
        return
    fi
    echo "function copyPackageDir"
    echo "cp -a ${MTK_PATCH_PATH}/alps/packages/ ${MTK_PACKAGES_NAME}"
    cp -a ${MTK_PATCH_PATH}/alps/packages/* ${MTK_PACKAGES_NAME}
}

function checkDiffDir() {
    if [ -d ${DIFF_PATH} ]; then
        rm -r ${DIFF_PATH}
    fi
    mkdir ${DIFF_PATH}
}

function generateAndroidDiffs() {
    local list_file=${DIFF_PATH}/android.list
    pushd ${MTK_ANDROID_PATH} > /dev/null
    git add .
    git diff --name-only --cached > ${list_file}
    while read line; do
        find ./gionee -name "*" | grep "$line"
        if [ $? -ne 0 ];then
             echo "line=${line} continue"
             continue
        fi
        echo 为${line}生成diff
        local targetfile=${ANDROID_DIFF_PATH}/${line}
        local targetdir=$(dirname ${targetfile})
        local filename=$(basename ${targetfile})
        if [ ! -d ${targetdir} ]; then
            mkdir -p ${targetdir}
        fi
        git diff --cached ${line} > ${targetdir}/${filename}.diff
    done < ${list_file}
    popd > /dev/null
}

function generatePackagesDiffs() {
    if [ "${MTK_PACKAGES_DIR_FLAG}" != "TRUE" ];then
        return
    fi
    local list_file=${DIFF_PATH}/package.list
    pushd ${MTK_PACKAGES_NAME} > /dev/null
    git add .
    git diff --name-only --cached > ${list_file}
    while read line; do
        echo 为${line}生成diff
        local targetfile=${PACKAGES_DIFF_PATH}/${line}
        local targetdir=$(dirname ${targetfile})
        local filename=$(basename ${targetfile})
        if [ ! -d ${targetdir} ]; then
            mkdir -p ${targetdir}
        fi
        git diff --cached ${line} > ${targetdir}/${filename}.diff
    done < ${list_file}
    popd > /dev/null
}

function generateDiffs() {
    checkDiffDir
    generateAndroidDiffs
    generatePackagesDiffs
}

function checkCompareApp() {
    COMPARE_APPS="bcompare meld"
    COMPARE_APP=
    local item
    for item in ${COMPARE_APPS}; do
        which ${item} >/dev/null
        if [ $? -eq 0 ];then
            COMPARE_APP=${item}
            echo "比较软件：${COMPARE_APP}"
            return
        fi
    done
    echo -e "${COLOR_ERROR_PREFIX}没有找到可用比较软件(如meld、bcompare等)，请安装后再操作！${COLOR_SUFFIX}"
    exit
}

function checkPwd() {
    local curDir=$(basename $(pwd))
    if [[ ${curDir} != *android* ]]; then
        echo -e "${COLOR_ERROR_PREFIX} 当前不是 android 主目录，请进入 android 主目录执行此脚本！${COLOR_SUFFIX}"
        exit
    elif [[ ! -d .git ]]; then
        echo -e "${COLOR_ERROR_PREFIX} 当前不是 git 库目录，请进入 android 主目录执行此脚本！${COLOR_SUFFIX}"
        exit
    fi
}

function modifyPatchListAPPXls()
{
    echo "function modifyPatchListAPPXls"
    pushd ${MTK_PATCH_PATH} >/dev/null
    while read line
    do
        echo -e "${COLOR_WARNING_PREFIX} 请手动修改 PatchList_APP.xls！ ${COLOR_SUFFIX}"
        gedit ${line} &
        libreoffice ${MTK_ANDROID_PATH}/PatchList_APP.xls
    done < <(find . -maxdepth 1 -type f)
    popd >/dev/null
}

function changeVersionNumber()
{
    #echo "function changeVersionNumber [$@]"
    local tmpVersionNew="$1"
    local tmpVersionPrefix="$2"
    local tmpVersionFile="$3"
    if [ -z "$tmpVersionNew" ];then
        echo -e "${COLOR_ERROR_PREFIX} The new versions can not be empty${COLOR_SUFFIX}"
        exit 1
    fi
    if [ ! -f "$tmpVersionFile" ];then
        echo -e "${COLOR_ERROR_PREFIX} $tmpVersionFile file does not exist${COLOR_SUFFIX}"
        exit 2
    fi
    local tmpVersionOld=$(cat $tmpVersionFile | sed -n "/$tmpVersionPrefix/p")
    #echo "tmpVersionOld:[${tmpVersionOld}]"
    if [ -z "$tmpVersionOld" ];then
        echo -e "${COLOR_ERROR_PREFIX} No \"$tmpVersionPrefix\" in the $tmpVersionFile ${COLOR_SUFFIX}"
        exit 3
    fi
    tmp=$(echo ${tmpVersionOld%=*})
    newversion="${tmp} = ${tmpVersionNew}"
    echo "sed -i \"s@$tmpVersionOld@$newversion@\" $tmpVersionFile"
    sed -i "s@$tmpVersionOld@$newversion@" $tmpVersionFile
    if [ $? -ne 0 ];then
        echo -e "${COLOR_ERROR_PREFIX} Modify version for sed commond fail! ${COLOR_SUFFIX}"
        exit 4
    fi
}

function modifyVerForProjectMK()
{
    echo "function modifyVerForProjectMK"
    if [ ! -e ${MTK_ANDROID_PATH}/gionee/config ];then
        echo -e "${COLOR_ERROR_PREFIX} Dir ${MTK_ANDROID_PATH}/gionee is not exsit!${COLOR_SUFFIX}"
        exit 1
    fi
    pushd ${MTK_ANDROID_PATH}/gionee/config/ >/dev/null
    while read line
    do
        local tmpWeekNo=$(grep "MTK_WEEK_NO" ${line} | awk -F '=' '{printf("%s",$2)}' | sed s/[[:space:]]//g)
        local tmpBuildNo=$(grep "MTK_BUILD_VERNO" ${line} | awk -F '=' '{printf("%s",$2)}' | sed s/[[:space:]]//g)
        if [[ -z ${tmpWeekNo} || -z ${tmpBuildNo} ]];then
            continue
        fi
        local tmpMtkProject=$(grep "TARGET=" ${line} | awk -F '=' '{printf("%s",$2)}' | sed s/[[:space:]]//g)
        if [ -z "${tmpMtkProject}" ];then
            continue
        fi
        getBuildVernoFromPatch "${tmpMtkProject}"
        changeVersionNumber "${PATCH_WEEK_NO}" "MTK_WEEK_NO" "${line}"
        changeVersionNumber "${PATCH_BUILD_VERNO}" "MTK_BUILD_VERNO" "${line}"
    done < <(find ./ -maxdepth 1 -type f -name '*.mk')
    popd >/dev/null
}

function manualPortingGioneeFiles()
{
    local i=${1}
    local tmpPath=${2}
    local fileName=$(basename ${i})
    while read j
    do
        echo -e "${COLOR_GREEN_PREFIX}patch  :[${i}]${COLOR_SUFFIX}"
        echo -e "${COLOR_GREEN_PREFIX}gionee :[${j}]${COLOR_SUFFIX}"
        #echo -e "${COLOR_GREEN_PREFIX}请确认是否手动 merge 以上文件（Y/N）:${COLOR_SUFFIX}"
        #read answer < /dev/tty
        #if [[ ${answer} = "y" || ${answer} = "Y" ]]; then
            which ${COMPARE_APP} >/dev/null
            if [ $? -ne 0 ];then
                echo -e "${COLOR_ERROR_PREFIX}没有找到软件 ${COMPARE_APP}，请手动使用其他软件进行 merge，完成后回车键继续....${COLOR_SUFFIX}"
                read < /dev/tty
            else
                if [[ ${2} == alps ]]; then
                    echo "-------alps dir"
                    patch ${j} ${ANDROID_DIFF_PATH}/${i}.diff
                elif [[ ${2} == alps/packages ]]; then
                    echo "-------alps/packages dir"
                    patch ${j} ${PACKAGES_DIFF_PATH}/${i}.diff
                fi
                local ret=$?
                if [ ${ret} -ne 0 ];then
                    echo -e "${COLOR_ERROR_PREFIX} 未能成功合入， patch命令执行结果 :${ret} ${COLOR_SUFFIX}"
                else
                    echo -e "${COLOR_GREEN_PREFIX}已自动成功打入, ret=${ret}: [${j}]${COLOR_SUFFIX}"
                fi
                if [ -f ${j}.rej ]; then
                    echo -e "${COLOR_ERROR_PREFIX}未能自动打入: [${j}]${COLOR_SUFFIX}"
                    echo -e "${COLOR_GREEN_PREFIX}请确认是否手动 merge 以上文件（Y/N）:${COLOR_SUFFIX}"
                    read answer < /dev/tty
                    if [[ ${answer} = "y" || ${answer} = "Y" ]]; then
                        gedit ${j}.rej &
                        echo "${COMPARE_APP} ${MTK_PATCH_PATH}/${tmpPath}/${i} ${j}"
                        ${COMPARE_APP} ${MTK_PATCH_PATH}/${tmpPath}/${i} ${j}
                        rm ${j}.rej
                    fi
                fi
                if [ -f ${j}.orig ]; then
                    rm ${j}.orig
                fi
            fi
        #fi
    done < <(find gionee/ \( -path "*.git" -o -path "*YouJuAgent*" -o -path "*amigoframework*" -o -path "*gnframework-res*" \) -prune -o -name ${fileName} -print | grep "$i")
}

function manualPortingInAndroidGioneePath()
{
    echo "function manualPortingInAndroidGioneePath"
    pushd ${MTK_PATCH_PATH}/alps >/dev/null
    while read i
    do
        pushd ${MTK_ANDROID_PATH} >/dev/null
        manualPortingGioneeFiles "${i}" "alps"
        popd >/dev/null
    done < <(find ./ -path "./packages" -prune -o -type f -print)
    popd >/dev/null
}

function manualPortingInPackageGioneePath()
{
    echo "function manualPortingInPackageGioneePath"
    if [ ! -e ${MTK_PATCH_PATH}/alps/packages ];then
        echo -e "${COLOR_WARNING_PREFIX} Have no packages !${COLOR_SUFFIX}"
        return
    fi
    pushd ${MTK_PATCH_PATH}/alps/packages >/dev/null
    while read i
    do
        pushd ${MTK_PACKAGES_NAME} >/dev/null
        manualPortingGioneeFiles "${i}" "alps/packages"
        popd >/dev/null
    done < <(find ./ -type f)
    popd >/dev/null
}

function checkStatus()
{
    echo "function checkStatus"
    repo status
}

function main()
{
	echo "适用与MTK_ap项目"
    #判断当前目录是否正确，在android主目录执行脚本
    checkPwd
    #判断比较软件是否安装
    checkCompareApp

    initLocalVar "$@"
    #如果当前是 repo 管理项目，则计算出各个 git 子库路径
    countRepoProjectList
    #检查当前 patch 中是否有 delete 的文件
    checkDeleteFileFromPatch
    #拷贝 android 下的代码（由于 android 目录没有 Gionee 的修改，因此直接覆盖即可）
    copyAndroidDir
    #拷贝 packages 下的代码（由于 packages 目录没有 Gionee 的修改，因此直接覆盖即可）
    copyPackageDir

    #为每个文件生成diff
    generateDiffs

    #修改 gionee/config/项目.mk 文件，这个主要修改 patch 号
    modifyVerForProjectMK
    #修改 PatchList_APP.xls ，记录当前集成的 patch 列表
    modifyPatchListAPPXls
    #判断如果 android/gionee 目录下存在修改的文件，则手动进行 merge
    manualPortingInAndroidGioneePath
    #判断如果 packages/gionee 目录下存在修改的文件，则手动进行 merge
    manualPortingInPackageGioneePath
    checkStatus
    #rm $0
}

main "$@"
