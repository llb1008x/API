#!/usr/bin/env python
# coding=utf-8

#循环
'''
def factorial(n):
    result = n;
    for i in range(1,n):
        result *= i
    return result

num = input("please input:")
print(factorial(int(num)))
'''

#递归版
'''
def factorial(n):
    if n == 1:
        return 1
    else :
        return factorial(n-1)*n    
        
num = input("please input :")
print(factorial(int(num)))
'''

#二分法查找
def search(sequence,number,lower,upper):
    if lower == upper:
        assert number ==sequence[upper]
        return upper
    else :
        middle = (lower+upper) // 2
        if number > sequence[middle]:
            return search(sequence,number,middle+1,upper)
        else :
            return search(sequence,number,lower,middle)

seq = [34,67,8,123,4,100,96]
seq.sort()
print(seq)
num = input("please input:")
print(search(seq,int(num),0,6))
