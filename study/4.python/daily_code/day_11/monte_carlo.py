#!/usr/bin/env python
# coding=utf-8

#圆周率的计算

#v1 公式法
'''
N = eval(raw_input("N:"))
pi = 0.0

for k in xrange(N):
    pi += 1.0/pow(16,k)*( \
        4/(8*k+1)-2/(8*k+4)- \
        1/(8*k+5)-1/(8*k+6))
        
print("圆周率值是:{:.5f}".format(pi))
'''

#v2 蒙特卡罗法
from random import random
#from time import perf_counter
import matplotlib.pyplot as plt

#这个数值大了就满屏都是点
NUM = 100
DARTS = NUM*NUM
hits = 0.0
#start = perf_counter()

#加个标题
plt.title('scatter diagram')
L = [0,0]
P = [0,0]
#蒙特卡罗法 计算散落在圆内的面积
for i in range(1,DARTS+1):
    x,y = random(),random()
    L.append(x)
    P.append(y)
    dist = pow(x**2+y**2,0.5)
    if dist <= 1.0:
        print("({:.2f},{:.2f})".format(x,y))
        hits = hits +1

pi = 4*(hits/DARTS)
print("圆周率是:{}".format(pi))
#print("运行时间是:{:.5f}s".format(perf_counter()-start))

#画点,散点图
#plt.xlim(xmax=NUM/100,xmin=0)
#plt.ylim(ymax=NUM/100,ymin=0)
plt.plot(L,P,'ro')
plt.show()


#v3 还是蒙特卡罗法 
'''
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import Circle

# 投点次数
n = 10000

# 圆的信息
r = 1.0         # 半径
a, b = (0., 0.) # 圆心

# 正方形区域边界
x_min, x_max = a-r, a+r
y_min, y_max = b-r, b+r

# 在正方形区域内随机投点
x = np.random.uniform(x_min, x_max, n) # 均匀分布
y = np.random.uniform(y_min, y_max, n)

# 计算 点到圆心的距离
d = np.sqrt((x-a)**2 + (y-b)**2)

# 统计 落在圆内的点的数目
res = sum(np.where(d < r, 1, 0))

# 计算 pi 的近似值（Monte Carlo方法的精髓：用统计值去近似真实值）
pi = 4.0 * res / n

print('pi: ', pi)

# 画个图看看
fig = plt.figure() 
axes = fig.add_subplot(111) 
axes.plot(x, y,'ro',markersize = 1)
plt.axis('equal') # 防止图像变形

circle = Circle(xy=(a,b), radius=r, alpha=0.5)
axes.add_patch(circle)

plt.show()
'''


