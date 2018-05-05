#!/usr/bin/env python
# coding=utf-8

#爬取网页的通用代码框架

import requests


def getHTMLText(url):
    try:
        r=requests.get(url)
        r.raise_for_status()
        r.encoding=r.apparent_encoding
        return r.text
    except:
        return "产生异常"

        


def main():
    url="https://www.baidu.com"
    print(getHTMLText(url))
    
main()   




