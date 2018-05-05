#!/usr/bin/env python
# coding=utf-8

#字符串反转
s = 'hello world'

def func(s):
    if len(s) <1:
        return s
    return func(s[1:])+s[0]
    
result = func(s)
print(result)

