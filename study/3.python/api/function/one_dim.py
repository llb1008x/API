#!/usr/bin/env python
# coding=utf-8

#一维数据的处理
'''
数据的维度：一维，二维，多维，高维
一维数据的标示：列表类型和集合类型
一维数据的存储:空格分隔，逗号分隔，特殊符号分隔
一维数据的处理：字符创方法,split()和.join()
'''

#数据的读入
fname=input("请输入文件名称:")

f=open(fname,'rt')
txt=f.read()
txt=txt.replace("\n","")
ls=txt.split(",")
print(ls)

f.close()


#数据的写入
ls=["中国","美国","法国"]
f=open("datas.txt","w")
f.write(",".join(ls))
f.close()
