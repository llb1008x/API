#!/bin/bash

#added by yanggangqiang@wingtech.com to set the build ENV.

target_MAC="00:1b:11:09:9e:69"
check_mac_status="FAIL"

function check_address(){
    for i in `ifconfig |grep HWaddr|sed "s/^.*HWaddr //"`; 
    do
        echo "currently MAC address is: "$i
        if [ $i = $target_MAC ];then
          check_mac_status="SUCCESS"
        fi
    done
}

check_address
if [ $check_mac_status != "SUCCESS" ];then
which tunctl
    if [ $? != 0 ] ;then
        echo "please install tool to create a Macaddress..."
        sudo apt-get install uml-utilities
        sudo tunctl -t eth2
        sudo ifconfig eth2 hw ether $target_MAC
        sudo ifconfig eth2 up
    else
        sudo tunctl -t eth2
        sudo ifconfig eth2 hw ether $target_MAC
        sudo ifconfig eth2 up
    fi

fi

check_address

if [ $check_mac_status != "SUCCESS" ];then
    echo "install Mac address tool failed, please contact administrator..."
else
    echo "Install Mac address tool successfully!"
fi





