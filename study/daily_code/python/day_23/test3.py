#!/usr/bin/env python
# coding=utf-8

from tvtk.api import tvtk

#imagedata 二维或三维图像的数据结构 ，可以理解为二维或三维数组
'''
#spacing 三维网格数据在X、 Y、 Z轴上的间距
#origin 三维网格数据的起点坐标
#dimensions 在X、 Y、 Z轴上的网格数
img=tvtk.ImageData(spacing=(1,1,1),origin=(1,2,3),dimensions=(3,4,5))

for n in range(6):
    print("%.1f ,%.1f ,%.1f" % img.get_point(n))
'''

#rectilineardata 间距不均匀网格，所有点都在正交的网格上
''''
import numpy as np

x=np.array([0,3,9,15]) 
y=np.array([0,1,5])
z=np.array([0,2,3])

r=tvtk.RectilinearGrid()
r.x_coordinates=x
r.y_coordinates=y
r.z_coordinates=z

r.dimensions=len(x),len(y),len(z)   

for n in range(6):
    print(r.get_point(n))
'''    
    
    
