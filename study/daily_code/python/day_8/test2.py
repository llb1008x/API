#!/usr/bin/env python
# coding=utf-8

def average(*args):
    if len(args) == 0 :
        return 0.0
    else :
        return float(sum(args))/len(args)

print average()
print average(1, 2)
print average(1, 2, 2, 3, 4)
