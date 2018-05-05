#!/usr/bin/env python
# coding=utf-8

__metaclass__ = type

class Person:
    def setName(self,name):
        self.name = name
    
    def getName(self):
        return self.name

    def greet(self):
        print("hello,world! I'm %s." % self.name)
