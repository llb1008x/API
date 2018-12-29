#!/usr/bin/env python
# coding=utf-8

#信息提取的一般方法

import requests
import re
from bs4 import BeautifulSoup

url='http://www.python123.io/ws/demo.html'
r=requests.get(url)
demo=r.text

soup=BeautifulSoup(demo,'html.parser')
for link in soup.find_all('a'):
    print(link.get('href'))
