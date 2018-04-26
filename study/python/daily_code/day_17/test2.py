#!/usr/bin/env python
# coding=utf-8

#二维数据的处理
#数据读出
fo = open(filename)
ls = []
for line in fo:
    line = line.replace('\n','')
    ls.append(line.split(','))
for.close()

#数据写入
ls = [[],[],[]] 
f=open(filename,'w')

for item in ls:
    f.write(','.join(item)+'\n')
f.close() 


#遍历二维数据
ls = [[],[],[]]

for row in ls:
    for colum in row:
        print(ls[row][colum])
