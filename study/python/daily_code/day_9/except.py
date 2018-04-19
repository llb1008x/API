#!/usr/bin/env python
# coding=utf-8

class MuffledCalculator:
    muffled = True
    def calc(self,expr):
        try:
            return eval(expr)
        #捕捉对象
        except ZeroDivisionError,e:
            if self.muffled:
                print e
                print 'Division by zero is illegal'
            else :
                raise

calculator = MuffledCalculator()
print(calculator.calc('10/2'))

print(calculator.calc('10/0'))
