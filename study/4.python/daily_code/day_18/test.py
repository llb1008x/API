#!/usr/bin/env python
# coding=utf-8

import requests
from bs4 import BeautifulSoup

url='http://python123.io/ws/demo.html'

#获取网页
r=requests.get(url)
r.status_code
print(r.text)
demo=r.text

#从网页上爬取数据，对html进行解析
soup=BeautifulSoup(demo,"html.parser")


'''
下行遍历
.contents
.children
.descendants

上行遍历
.parent
.parents

平行遍历
.next_sibling
.previous_sibling
.next_siblings
.previous_siblings

'''
#标签树的上行遍历
soup=BeautifulSoup(demo,'html.parser')
for parent in soup.body.parents:
    if parent is None:
        print(parent)
    else:
        print(parent.name)  
        
#标签树的平行遍历
for sibling in soup.body.next_siblings:
    print(sibling)        

