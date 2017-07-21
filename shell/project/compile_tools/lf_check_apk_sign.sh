#!/bin/bash
#
#       DESCRIPTION:    检查apk 是否需要系统权限，判断是否需要系统签名
#
#       SCRIPT NAME:    gn_check_apk_sign.sh
#
#       Usage:   gn_check_apk_sign.sh 
#                
#
#       Input:  stdin
#
#       Output: 
#
#       AUTHOR:         Ling Fen
#
#       EMAIL:          lingfen@gionee.com
#
#       DATE:           2015-09-16
#
#       HISTORY:
#       REVISOR         DATE                    MODIFICATION

usage(){
printf "
检查apk 是否需要系统权限，判断是否需要系统签名

Usage : $0 

options:
    [--help] Show help message

example: 
"
}
get_opts(){
    opts=$(getopt -o hc:s: --long help,check:,sign:, -- "$@")     
    if [ $? -ne 0 ];then
        usage 
        exit 1
    fi

    eval set -- "$opts"
    while true 
    do
        case "$1" in 
            -h|--help)
                usage
                exit 0
                ;;
            -c|--check)
                GN_APK_PATH="$2"
                shift 2
                ;;
            -s|--sign)
                GN_SIGN_NAME="$2"
                shift 2
                ;;
            --)
                shift
                break
                ;;
            *)
                usage
                exit 1
                ;;
        esac
    done
    if [ -e "$GN_APK_PATH" ];then
        if [ ! -e "$APK_SIGN_LIST" ];then
            echo "check权限文件不存在"
        fi
    fi
}

check_apk_sign(){
    [ ! -e "$GN_APK_PATH" ] && return 
    local is_need_system_sig=false
    local apk_system_sig=()
    local n=0
    for line in `$AAPT d permissions $GN_APK_PATH | awk -F ":" '$0 ~ /uses-permission:/ {print $2}'`
    do
        #echo $line
        eval $line
        if `grep -qw  "$name" "$APK_SIGN_LIST"` ;then
            is_need_system_sig=true
            apk_system_sig[$n]=$name
            let n+=1
        fi
    done
    if $is_need_system_sig  ;then
        echo "yes"
    fi
    ##echo ${apk_system_sig[@]}
}

main(){
   get_opts "$@" 
   check_apk_sign
}

CURRENT_DIR=$(cd `dirname $0`; pwd)
AAPT=out/host/linux-x86/bin/aapt
APK_SIGN_LIST=$CURRENT_DIR/lf_system_sign.list
main "$@"
