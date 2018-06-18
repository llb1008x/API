#!/usr/bin/env python
# coding=utf-8

#正常的功能
def hello():
    print("hello,world!")
    
    
#测试部分    
def test():
    hello()    
    
#判断是作为模块还是主程序
if __name__ == '__main__':test()    

#定义，导入一些公共接口
__all__:
