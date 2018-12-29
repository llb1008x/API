#!/usr/bin/env python
# coding=utf-8


#生成器
L = [x*x for x in range(10)]
print(L)

g = (x*x for x in range(10))
print(g)
