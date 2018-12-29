#!/usr/bin/env python
# coding=utf-8

#标量数据绘制
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


#读取数据 plot3D 
def read_data():
    plot3d=tvtk.MultiBlockPLOT3DReader(xyz_file_name="combxyz.bin",#网格文件
                                       q_file_name="combq.bin",    #空气动力学结果文件
                                       scalar_function_number=100, #设置标量数据量
                                       vector_function_number=200  #设置适量数据量
                                       )
    #用plot3D计算其输出数据   
    plot3d.update()
    return plot3d


#配置等值面对象
def produce_contourfilter(grid):
    #创建等值面对象                                   
    con=tvtk.ContourFilter()
    #con.set_input_data(grid)
    con.set_input(grid)
    
    #指定轮廓数和数据范围
    con.generate_values(50,grid.point_data.scalars.range)
    
    #设定映射器的变量范围属性
    m=tvtk.PolyDataMapper(scalar_range=grid.point_data.scalars.range,
                          input_connection=con.output_port)
                          
    a=tvtk.Actor(mapper=m)
    #设定透明度
    a.property.opacity=0.5
                          
    return a

def main():
    plot3d=read_data()
    #获取读入的数据集对象
    grid=plot3d.output.get_block(0) 
    print(grid.points.to_array())   
    
    a=produce_contourfilter(grid)
    win=ivtk_scene(a)
    win.scene.isometric_view()
    event_loop()
    
    
main()    



