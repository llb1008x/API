#!/usr/bin/env python
# coding=utf-8

#初始化工作
def init(data):
    data['first'] = {}
    data['middle'] = {}
    data['last'] = {}

#查找数据
def lookup(data,label,name):
    return data[label].get(name)

#存储数据
def store(data,*full_names):
    for full_name in full_names:
        names = full_name.split()
        if len(names) == 2:
            names.insert(1,'')
        
        labels = 'first','middle','last'
        
        for label,name in zip(labels,names):
            people = lookup(data,label,name)
            if people:
                people.append(full_name)
            else :
                data[label][name] = [full_name]

MyNames = {}
init(MyNames)
#store(MyNames,'magnus lie hetland')
store(MyNames,'han Solo')
store(MyNames,'Luke Skywalker','Anakein Skywalker')
print(lookup(MyNames,'last','Skywalker'))


