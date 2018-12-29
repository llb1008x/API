#!/usr/bin/env python
# coding=utf-8

#显示一个三维对象
from tvtk.api import tvtk

#创建一个长方体数据源
s=tvtk.CubeSource(x_length=1.0,y_length=2.0,z_length=3.0)
#使用polydatamapper将数据转换为图形数据
m=tvtk.PolyDataMapper(input_connection=s.output_port)


#创建一个Actor
a=tvtk.Actor(mapper=m)
#创建一个renderer,将actor添加进去
r=tvtk.Renderer(background=(0,0,0) )
r.add_actor(a)
#创建一个renderWindow(窗口),将renderer添加进去
w=tvtk.RenderWindow(size=(3000,3000))
w.add_renderer(r)
#创建一个renderWindowInteractor,窗口交互式工具
i=tvtk.RenderWindowInteractor(render_window=w)
#开启交互
i.initialize()
i.start()


