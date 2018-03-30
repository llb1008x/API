#!/usr/bin/env python
# coding=utf-8

#一个简单的数据库

people = {

    'Alice':{
        'phone':'2341',
        'addr':'Foo drive 23'
    },

    'Beth':{
        'phone':'9102',
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
name = name.title()

#查找号码还是地址？
request = raw_input("phone number (p) or address (a)?")

#使用正确的键
if request == 'p':key = 'phone'
if request == 'a':key = 'addr'

'''
if name in people :
	print("%s's %s is %s ." % \
	(name ,labels[key],people[name][key]))
'''

#使用get提供默认值
person = people.get(name,{})
label = labels.get(key,key)
result = person.get(key,'not available')

print("%s s %s is %s." % (name,label,result))	
