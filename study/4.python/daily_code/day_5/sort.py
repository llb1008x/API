#!/usr/bin/env python
# coding=utf-8

#sort 排序 ，关键字参数 cmp,key,reverse

def cmp(a,b):
    if a > b :
        return 1
    elif a < b:
        return -1
    else :
        return 0

def main():
    number = [5,2,9,7]
    number.sort(cmp)
    print numbert
    
    
main()   


'''
x = ['aardvark','abalone','acme','add','aerate']
x.sort(key=len)
'''



