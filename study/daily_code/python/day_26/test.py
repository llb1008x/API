#!/usr/bin/env python
# coding=utf-8

import numpy as np


#生成csv文件
a=np.arange(100).reshape(5,20)
np.savetxt('a.csv',a,fmt='%d',delimiter=',')


#读取csv文件
b=np.loadtxt('a.csv',delimiter=',')
print(b)


np.savez('b.csv',a)

