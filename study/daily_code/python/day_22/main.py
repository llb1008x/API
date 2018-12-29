#!/usr/bin/env python
# coding=utf-8

#热量转换
val = raw_input("请输入热量：")

if val[-1] in ['J','j']:
    f = float(val[0:-1])/4.186
    print("转换后的温度为：%.3fcal"%f)
elif val[-1] in ['L','l']:
    c = float(val[0:-3])*4.186
    print("转换后的温度为：%.3fJ"%c)
else:
    print("输入有误")
