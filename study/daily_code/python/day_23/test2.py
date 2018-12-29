#!/usr/bin/env python
# coding=utf-8

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

#图形数据的基本参数    
s=tvtk.CubeSource(x_length=1.0,y_length=2.0,z_length=3.0)
m=tvtk.PolyDataMapper(input_connection=s.output_port)
a=tvtk.Actor(mapper=m)

win=ivtk_scene(a)
win.scene.isometric_view()
event_loop()

