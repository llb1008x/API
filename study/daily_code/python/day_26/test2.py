#!/usr/bin/env python
# coding=utf-8

from PIL import Image
import numpy as np

#打开图片,convert是将图片转换成灰度值，只有二维
#im=np.array(Image.open("webwxgetmsgimg.jpeg").convert("L"))
im=np.array(Image.open("webwxgetmsgimg.jpeg"))
print(im.shape,im.dtype)
print(im)

#操作图片的rgb值
#b=[205,235,255]-im
#b=255-im
#线性变换
#b=(105/255)*im+150  
#像素平方
b=255*(im/255)**2


#生成新的图片数组
a=Image.fromarray(b.astype('uint8'))
a.save("tmp.jpg")

