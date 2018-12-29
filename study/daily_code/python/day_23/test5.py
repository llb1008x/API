#!/usr/bin/env python
# coding=utf-8

#矢量数据绘制
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
                                       scalar_function_number=10, #设置标量数据量
                                       vector_function_number=20  #设置适量数据量
                                       )
    #用plot3D计算其输出数据   
    plot3d.update()
    return plot3d


def main():
    plot3d=read_data()
    #获取读入的数据集对象
    grid=plot3d.output.get_block(0) 
    print(grid.points.to_array())   
    
    #对数据集中的数据进行随机选取
    mask=tvtk.MaskPoints(random_mode=True,on_ratio=50)
    mask.set_input(grid)
    
    #创建标示箭头的polydata 数据集
    glyph_source=tvtk.ArrowSource()
    
    #在mask采样后的polydata数据集每个点上放置一个箭头
    #箭头的方向，长度和颜色由于点对应的矢量和标量决定
    glyph=tvtk.Glyph3D(input_connection=mask.output_port,
                        scale_factor=4)
    glyph.set_source_connection(glyph_source.output_port)
    m=tvtk.PolyDataMapper(scalar_range=grid.point_data.scalars.range,
                          input_connection=glyph.output_port)
                          
    a=tvtk.Actor(mapper=m)    

    win=ivtk_scene(a)
    win.scene.isometric_view()
    event_loop()
    
    
main()    



