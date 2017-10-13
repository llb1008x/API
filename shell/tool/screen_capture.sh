#########################################################################
# File Name: screen_capture.sh
# Author: llb
# mail: llb1008x@126.com
# Created Time: 2017年03月08日 星期三 09时09分34秒

android 手机截屏
#########################################################################
#!/bin/bash
      
if [ $# -eq 0 ];  
    then  
    NAME=$(date +%Y%m%d_%H%M%S).png  
elif [ $1 = "-c" ]                 # colon  
    then  
    NAME=$(date +%Y%m%d_%H:%M:%S).png  
elif [ $1 = "-h" ]                 # hyphen  
    then  
    NAME=$(date +%Y%m%d_%H-%M-%S).png  
else  
    echo -e "\033[31m *Invalid argument.\n *Usage '$0 -[ch]' \033[0m"  
    exit 1  
fi  
      
adb shell /system/bin/screencap -p /sdcard/$NAME  
adb pull /sdcard/$NAME ~/project/DEBUG/  
  
# the '[' and ']' should have space with left and right  
if [ $? -eq 0 ]  
    then  
    echo -e "\033[32m *Screenshot save successfully.\n *The Path is ~/Pictures/${NAME} \033[0m"  
    else  
    echo -e "\033[31m *Screenshot save failed \033[0m"  
fi  

