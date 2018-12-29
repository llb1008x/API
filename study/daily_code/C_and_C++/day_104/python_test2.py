#!/usr/bin/env python
# coding=utf-8

import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
 
fig = plt.figure(1)
ax = fig.add_subplot(1,1,1, projection='3d') # 指定三维空间做图
 
t = np.linspace(0, 4, 200)  # 在0到4之间，均匀产生200点的数组
theta = t * 2 * np.pi # 角度
 
# r(t)=(sint,cost,t)
z = t 
x = np.sin(theta)
y = np.cos(theta)
ax.plot(x, y, z, label='r(t)')
 
# r’(t)
z = 1 
x = np.cos(theta)
y = -np.sin(theta)
ax.plot(x, y, z, label='r\'(t)')
 
ax.legend()
plt.show()
