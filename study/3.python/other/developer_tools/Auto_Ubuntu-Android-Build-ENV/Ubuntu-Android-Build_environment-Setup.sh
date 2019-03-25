#!/bin/bash

OS_VERSION=""

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

function getCurOSVersionNumber()
{
    OS_VERSION=$(echo $(lsb_release -r | grep "Release:" | awk -F " " '{printf("%s",$2)}'))
    if [[ "${OS_VERSION}" = "12.04" || "${OS_VERSION}" = "14.04" || "${OS_VERSION}" = "16.04" ]];then
        echo -e "${COLOR_GREEN_PREFIX}当前Ubuntu系统版本为:[${OS_VERSION}]${COLOR_SUFFIX}"
    else
        echo -e "${COLOR_ERROR_PREFIX}暂不支持对当前系统版本进行编译环境安装，目前仅支持 Ubuntu 12.04 14.04 16.04 !${COLOR_SUFFIX}"
        exit 1
    fi
}

function Update_Source()
{
	echo "function Update_Source"
	sudo apt-get update
}

function Install_sofrware()
{
	echo "sudo apt-get install $@"
	sudo apt-get -y install $@
}

function Set_Source_List()
{
	echo "sudo add-apt-repository $@"
	sudo add-apt-repository $@
}

function Switch_Java_Version()
{
	echo "sudo update-java-alternatives -s java-${1}-openjdk-amd64"
	sudo update-java-alternatives -s java-${1}-openjdk-amd64
}

function Ubuntu_12.04_env()
{
	echo "function Ubuntu_12.04_env"
	source_list="/etc/apt/source.list"
	string=$(echo $(lsb_release -d | grep "Description:" | awk -F " " '{printf("%s",$3)}'))
	echo $string
	if [ $string = "12.04.4" ];then
		echo -e "${COLOR_GREEN_PREFIX}-----开始安装 Ubuntu 12.04.4 android 编译环境-----${COLOR_SUFFIX}"
	else
		echo -e "${COLOR_GREEN_PREFIX}-----开始安装 Ubuntu 12.04.5 android 编译环境-----${COLOR_SUFFIX}"
		source_list="/etc/apt/sources.list"
	fi

	sudo chmod o+w ${source_list}
	sudo cp ${source_list} ${source_list}_backup
	sudo echo 'deb http://archive.ubuntu.com/ubuntu/ precise main restricted universe multiverse' >> ${source_list}
	sudo echo 'deb http://archive.ubuntu.com/ubuntu/ precise-updates main restricted universe multiverse' >> ${source_list}
	sudo echo 'deb http://archive.ubuntu.com/ubuntu/ precise-sercurity main restricted universe multiverse' >> ${source_list}
	sudo chmod o-w ${source_list}

    sudo add-apt-repository ppa:openjdk-r/ppa
	Update_Source
	Install_sofrware "git gnupg flex bison gperf build-essential zip curl libc6-dev libncurses5-dev:i386 x11proto-core-dev libx11-dev:i386 libreadline6-dev:i386 libgl1-mesa-glx:i386 libgl1-mesa-dev g++-multilib mingw32 tofrodos python-markdown libxml2-utils xsltproc zlib1g-dev:i386"

	sudo ln -s /usr/lib/i386-linux-gnu/mesa/libGL.so.1 /usr/lib/i386-linux-gnu/libGL.so

	if [ -e "jdk-6u45-linux-x64.bin" ];then
		if [ ! -d "/opt/java/" ];then
			sudo mkdir -p /opt/java/
		fi
		sudo cp jdk-6u45-linux-x64.bin /opt/java/
		pushd /opt/java >/dev/null
		sudo chmod a+x jdk-6u45-linux-x64.bin 
		sudo ./jdk-6u45-linux-x64.bin
		popd >/dev/null
	fi

    Install_sofrware "openjdk-7-jdk"
    Install_sofrware "openjdk-8-jdk"
    Switch_Java_Version "1.7.0"
    Install_sofrware "python-xlrd python-xlwt p7zip-full"
    Install_sofrware "gitg gitk"
    Install_sofrware "libqt3-mt"
    Install_sofrware "libqt3-mt:i386"
    Install_sofrware "sendemail"
}

function Ubuntu_14.04_env()
{
    echo "function Ubuntu_14.04_env"
    Update_Source
    Install_sofrware "openjdk-7-jdk"
    Switch_Java_Version "1.7.0"
    Install_sofrware "git-core gnupg flex bison gperf build-essential zip curl zlib1g-dev gcc-multilib g++-multilib libc6-dev-i386 lib32ncurses5-dev x11proto-core-dev libx11-dev lib32z-dev ccache libgl1-mesa-dev libxml2-utils xsltproc unzip"
}

function Ubuntu_16.04_env()
{
	echo "function Ubuntu_16.04_env"
	#install openjdk 7
	Set_Source_List "ppa:openjdk-r/ppa"
	Update_Source
	Install_sofrware "openjdk-7-jdk"
	Install_sofrware "openjdk-8-jdk"
	Switch_Java_Version "1.7.0"
	Install_sofrware "git-core gnupg flex bison gperf build-essential zip curl zlib1g-dev gcc-multilib g++-multilib libc6-dev-i386 lib32ncurses5-dev x11proto-core-dev libx11-dev lib32z-dev ccache libgl1-mesa-dev libxml2-utils xsltproc unzip m4"
}

function main()
{
	getCurOSVersionNumber
	Ubuntu_${OS_VERSION}_env
	echo -e "${COLOR_GREEN_PREFIX}-----环境安装结束-----${COLOR_PREFIX}"
}

main