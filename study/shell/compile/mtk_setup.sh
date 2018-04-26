#!/bin/bash

source_list="/etc/apt/source.list"
string=$(echo $(lsb_release -r | grep "Release:" | awk -F " " '{printf("%s",$2)}'))
if [ $string = "12.04" ];then
	echo "cur ubuntu is 12.04"
else
	echo "cur linux version is wrong!"
	exit 1
fi
string=$(echo $(lsb_release -d | grep "Description:" | awk -F " " '{printf("%s",$3)}'))
echo $string
if [ $string = "12.04.4" ];then
	echo "-----开始安装 Ubuntu 12.04.4 android 编译环境-----"
else
	echo "-----开始安装 Ubuntu 12.04.5 android 编译环境-----"
	source_list="/etc/apt/sources.list"
fi

sudo chmod o+w ${source_list}
sudo cp ${source_list} ${source_list}_backup
sudo echo 'deb http://archive.ubuntu.com/ubuntu/ precise main restricted universe multiverse' >> ${source_list}
sudo echo 'deb http://archive.ubuntu.com/ubuntu/ precise-updates main restricted universe multiverse' >> ${source_list}
sudo echo 'deb http://archive.ubuntu.com/ubuntu/ precise-sercurity main restricted universe multiverse' >> ${source_list}
sudo chmod o-w ${source_list}

sudo apt-get update

sudo apt-get install git gnupg flex bison gperf build-essential \
  zip curl libc6-dev libncurses5-dev:i386 x11proto-core-dev \
  libx11-dev:i386 libreadline6-dev:i386 libgl1-mesa-glx:i386 \
  libgl1-mesa-dev g++-multilib mingw32 tofrodos \
  python-markdown libxml2-utils xsltproc zlib1g-dev:i386

sudo ln -s /usr/lib/i386-linux-gnu/mesa/libGL.so.1 /usr/lib/i386-linux-gnu/libGL.so

sudo mkdir -p /opt/java/
sudo cp jdk-6u45-linux-x64.bin /opt/java/
cd /opt/java
sudo chmod a+x jdk-6u45-linux-x64.bin 
sudo ./jdk-6u45-linux-x64.bin
cd -

#sudo echo 'export JAVA_HOME=/opt/java/jdk1.6.0_45' >> /etc/bash.bashrc
#sudo echo 'export JRE_HOME=$JAVA_HOME/jre' >> /etc/bash.bashrc
#sudo echo 'export CLASSPATH=$CLASSPATH:$JAVA_HOME/lib:$JRE_HOME/lib' >> /etc/bash.bashrc
#sudo echo 'export PATH=$PATH:$JAVA_HOME/bin:$JRE_HOME/bin:$JAVA_HOME/lib:$JAVA_HOME' >> /etc/bash.bashrc
#source /etc/bash.bashrc

sudo apt-get -y install openjdk-7-jdk
sudo update-java-alternatives -s java-1.7.0-openjdk-amd64
#sudo apt-get install openjdk-7-jdk
#sudo update-alternatives --config java
#sudo update-alternatives --config javac

sudo apt-get install python-xlrd python-xlwt p7zip-full
echo "-----环境安装结束-----"
