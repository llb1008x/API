#!/usr/bin/env python
# coding=utf-8

#文件的操作

#这里不同的操作方法，主要在读取是读取文件的api不同，一次性读取占内存，逐行处理更和实际
#一次读取,逐行遍历文件
fname=input("请输入要打开的文件 :")
fo=open(fname,'r')

''' 
for line in fo.readlines():
    print(line)
''' 
#逐行读取   
for line in fo:
    print(line)   
   
   
fo.close()

