#!/usr/bin/env python
# coding=utf-8

#自动轨迹绘制
import turtle as t

#图形的基本信息
t.title("自动轨迹绘制")
t.setup(800,600,0,0)
t.pencolor("red")
t.pensize(5)

#读取数据,数据的定义
datals=[]
f=open("data.txt")
for line in f:
    line=line.replace("\n","")
    datals.append(list(map(eval,line.split(","))))
f.close()

#自动绘制
for i in range(len(datals)):
    t.pencolor(datals[i][3],datals[i][4],datals[i][5])
    t.fd(datals[i][0])
    if datals[i][1]:
        t.right(datals[i][2])
    else :
        t.left(datals[i][2])

t.done()
