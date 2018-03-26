#!/usr/bin/env python
# coding=utf-8

#一个简单的数据库

people = {

    'Alice':{
        'phone':'2341'.
        'addr':'Foo drive 23'
    },

    'Beth':{
        'phone':'9102'.
        'addr':'Bar street 42'
    },

    'Ceil':{
        'phone':'3158',
        'addr':'Baz avenue 90'
    }
}

#针对电话号码和地址使用的描述线性标签
labels = {
    'phone':'phone number',
    'addr':'address'
}

name = raw_input("Name:")

#查找号码还是地址？
request = raw_input("phone number (p) or address (a)?")

#
