#!/usr/bin/env python
# coding=utf-8

#求定积分
import numpy as np
import matplotlib.pyplot as plt
import time 
from random import random

def fun(x):
    return x**2
    
#投点数
n = 10000
res = 0
#矩形区域边界
x_min,x_max = 0.0,1.0
y_min,y_max = 0.0,1.0

start = time.clock()


#矩形区域内随机投点
x = np.random.uniform(x_min,x_max,n)
y = np.random.uniform(y_min,y_max,n)

#统计落在函数下方的点
res = sum(np.where(y<fun(x),1,0))

#近似计算定积分的值
integral = 1.0*res / n
print('integral{:.4f}'.format(integral))

'''
L = [0,0]
P = [0,0]
#蒙特卡罗法 计算散落在圆内的面积
for i in range(1,n+1):
    x,y = random(),random()
    L.append(x)
    P.append(y)
   
    if y< fun(x):
        print("({:.2f},{:.2f})".format(x,y))
        res += 1

integral = 1.0*res / n 
end = time.clock()
dur = end- start
print("res:{:} integral:{:.4f} duration:{:.4f}".format(res,integral,dur))
'''

#画图
fig = plt.figure()
axes = fig.add_subplot(111)
axes.plot(x,y,'ro',markersize = 1)
plt.axis("equal")

axes.plot(np.linspace(x_min,x_max,10),fun(np.linspace(x_min,x_max,10)),'b-')

plt.show()
