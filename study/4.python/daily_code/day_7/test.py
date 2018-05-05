#!/usr/bin/env python
# coding=utf-8

from itertools import islice

class Fib:
    def __init__(self):
        self.prev = 0
        self.curr = 1
    
    def __iter__(self):
        return self
        
    def __next__(self):    
        value = self.curr
        self.curr += self.prev
        self.prev = value
        return value
        
f = Fib()
print(list(islice(f,0,10)))

     
