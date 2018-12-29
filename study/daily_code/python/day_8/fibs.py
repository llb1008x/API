#!/usr/bin/env python
# coding=utf-8

def fibs(num):
    result = [0,1]
    
    for i in range(num-2):
        result.append(result[-2]+result[-1])

    print result
    
score = input("how many numbers do you want?")    
print fibs(score)
