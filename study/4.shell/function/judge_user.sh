#!/bin/bash

# 判断当前用户
test=$(env |grep USER |cut -d "=" -f 2)
echo $test

if [ "$test" == "root" ];then
	echo "ok,you are super user!"
else 
	echo "sorry,you are not a super user!"	
fi	
