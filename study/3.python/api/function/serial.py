#!/usr/bin/env python
# coding=utf-8

def checkIndex(key):
    '''所给的键是能接收的索引吗'''
    if not isinstance(key,(int,long)):
        raise TypeError
    if key < 0 :
        raise IndexError

class ArithmeticSequence:
    def __init__(self,start=0,step=1):
        '''初始化算数序列'''
        self.start=start
        self.step=step
        self.changed={}

    def __getitem__(self,key):
        '''get an item the Arithmetic Sequence'''
        checkIndex(key)

        try : 
            return self.changed[key]
            
        except KeyError:
            return self.start + key*self.step
    
    def __setitem__(self,key,value):
        '''修改算数序列中的一个项'''
        checkIndex(key)
        self.changed[key] = value


s = ArithmeticSequence(1,2)
print(s[1000000])

