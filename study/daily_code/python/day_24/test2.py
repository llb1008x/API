#!/usr/bin/env python
# coding=utf-8

#3D绘图
import numpy as np
from mayavi import mlab


''''
#points3D
#建立数据
t=np.linspace(0,4*np.pi,20)
x=np.sin(2*t)
y=np.cos(t)
z=np.cos(2*t)

s=2+np.sin(t)


#对数据进行可视化
points=mlab.points3d(x,y,z,colormap="Blues",scale_factor=.15)
mlab.show()


#plot3D

#建立数据
n_mer,n_long=6,11
dphi=np.pi/1000.0
phi=np.arange(0.0,2*np.pi+0.5*dphi,dphi)
mu=phi*n_mer
x=np.cos(mu)*(1+np.cos(n_long*mu/n_mer)*0.5)
y=np.sin(mu)*(1+np.cos(n_long*mu/n_mer)*0.5)
z=np.sin(n_long*mu/n_mer)*0.5


#对数据进行可视化
l=mlab.plot3d(x,y,z,np.sin(mu),tube_radius=0.25,colormap="Spectral")
mlab.show()


#image show
#建立数据
s=np.random.random((10,10))

#对数据进行可视化
img=mlab.imshow(s,colormap='gist_earth')
mlab.show()



#surf,contour_surf
def f(x,y):
    return np.sin(x-y)+np.cos(x+y)
    

x,y=np.mgrid[-7.:7.05:0.1,-5.:5.05:0.05]
s=mlab.surf(x,y,f)
#s=mlab.contour_surf(x,y,f)
mlab.show() 



#contour3d

x,y,z=np.ogrid[-5:5:64j,-5:5:64j,-5:5:64j]
scalars=x*x+y*y+z*z

obj=mlab.contour3d(scalars,contours=8,transparent=True)
mlab.show()

'''




  
