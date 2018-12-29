#!/usr/bin/env python
# coding=utf-8


#二维数据的操作

#1.从csv格式的文件中读入数据
'''
fname=input("请输入文件的名称:")
fo=open(fname)
ls=[]
for line in fo:
    line=line.replace("\n","")
    ls.append(line.split(","))
    
#print(ls)    
#fo.close()

'''
#2.将数据写入csv文件中
fname=input("请输入文件的名称:")
ls=[['1','中国'],['2','美国'],['3',"法国"]]   #二维列表
fo=open(fname,'w')
for item in ls:
    fo.write(".".join(item)+'\n')

fo.close()


#二维数据的处理
ps=[[1,2],[2,3],[3,4]]
for i in ls :
    for j in i:
        print(i)

#fo.close()
