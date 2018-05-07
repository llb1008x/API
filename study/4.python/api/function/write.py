#!/usr/bin/env python
# coding=utf-8


#文件的写入操作
fo=open("output.txt","a+")
ls=["中国","法国","美国"]
fo.writelines(ls)
fo.seek(0)

for line in fo:
    print(line)
fo.close()    
