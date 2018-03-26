#!/usr/bin/env python
# coding=utf-8


TempStr = input()
if TempStr[0:1] in ['F','f']:
    C = (eval(TempStr[1:])-32)/1.8
    print("C{:.2f}".format(C))
elif TempStr[0:1] in ['C','c']:
    F = 1.8*eval(TempStr[1:])+32
    print("F{:.2f}".format(F))
else:
    print("输入格式错误") 





