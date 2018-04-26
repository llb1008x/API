#!/bin/bash

#判断分区的使用情况
test=$(df -lh|grep sda1 |awk '{print $5}' |cut -d '%' -f 1)
echo $test

if [ "$test" -ge 90 ];then
	echo "warning!over use storage"
fi	
