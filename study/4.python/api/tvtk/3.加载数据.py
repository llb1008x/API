#!/usr/bin/env python
# coding=utf-8

#数据集的加载
from tvtk.api import tvtk
from tvtk.tools import ivtk
from pyface.api import GUI


#创建绘制管线的窗口
def ivtk_scene(actors):
    #创建一个带crust 的窗口
    win=ivtk.IVTKWithCrustAndBrowser()
    win.open()
    win.scene.add_actor(actors)
    #修正窗口错误
    dialog=win.control.centralWidget().widget(0).widget(0)
    from pyface.qt import QtCore
    dialog.setWindowFlags(QtCore.Qt.WindowFlags(0x00000000))
    dialog.show()
    return win
    
#交互式，事件循环    
def event_loop():
    gui=GUI()
    gui.start_event_loop()

'''
#读取数据 plot3D 
def read_data():
    plot3d=tvtk.MultiBlockPLOT3DReader(xyz_file_name="combxyz.bin",#网格文件
                                       q_file_name="combq.bin",    #空气动力学结果文件
                                       scalar_function_number=100, #设置标量数据量
                                       vector_function_number=200  #设置适量数据量
                                       )
    
    plot3d.update()
    return plot3d

plot3d=read_data()
grid=plot3d.output.get_block(0) 
print(grid.points.to_array())   
                                   
    
'''   
#加载stl 文件 
#stl采用三角形面片离散的近似标示三维模型 
def produce_actor():
    s=tvtk.STLReader(file_name='python.stl')
    m=tvtk.PolyDataMapper(input_connection=s.output_port)
    a=tvtk.Actor(mapper=m)
    
    return a

a=produce_actor()
win=ivtk_scene(a)
win.scene.isometric_view()
event_loop()



