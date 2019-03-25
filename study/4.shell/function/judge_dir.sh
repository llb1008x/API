#!/bin/bash

#判断是否为一个目录
read -t 30 -p "please input dir:" dir

if [ -d "$dir" ];then
	echo "this is a dir"
else 
	echo "this isn't a dir"
fi
