#!/bin/bash

#判断某个service是否启动
test=$(ps aux |grep httpd |grep -v grep)

if [ -n "$test" ];then
	echo "&(date) httpd is ok " > /tmp/autostart-acc.log
else
	/etc/rc.d/init.d/httpd start & > /dev/null
	echo "&(date) restart httpd!!" >> /tmp/autostart-err.log
fi
