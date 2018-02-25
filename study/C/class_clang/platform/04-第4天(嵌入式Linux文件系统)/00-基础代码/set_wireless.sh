#!/bin/sh
if [ -z $1 ]; then
	echo "wifiname = sunpluseduwifi"
	wifiname="sunpluseduwifi"
else
	wifiname=$1
fi
ifconfig wlan0 up
iwconfig wlan0 essid $wifiname
udhcpc -i wlan0
#ifconfig wlan0 192.168.1.123 netmask 255.255.255.0

