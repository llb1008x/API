#!/usr/bin/env python
# coding=utf-8

#热量转换
val = raw_input("")

if val[-1] in ['J','j']:
    f = float(val[0:-1])/4.186
    print("%.3fcal"%f)
elif val[-1] in ['L','l']:
    c = float(val[0:-3])*4.186
    print("%.3fJ"%c)
else:
    print("输入有误")
