#!/bin/sh

#auto get ip
#udhcpc

#config ip netmask gw
#ifconfig eth0 172.20.223.123 netmask 255.255.255.0  
#route add default gw 172.20.223.254

echo "usage:./mount_nfs serverip serverpath localpath"
if [ -z $1 ]; then
	echo "serverip is Null"
fi
if [ -z $2 ]; then
	echo "serverpath is Null"
	exit 1
fi
if [ -z $3 ]; then
	echo "localpath default /mnt/nfs"
fi
serverip=$1
serverpath=$2
localpath=/mnt

mount -o nolock,wsize=1024,rsize=1024 $serverip:$serverpath $localpath


if [ $? = 0 ]; then
	echo "nfs mount succeed!!"
	echo "if you want to umount the serverpath"
	echo "please input umount like this: umount /mnt"
else
	echo "nfs mount failed,please check and try agaig!!"
fi
