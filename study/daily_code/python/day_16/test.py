#!/usr/bin/env python
# coding=utf-8

#IP地址查询

import requests


def getHTMLText(url):
    try:
        r=requests.get(url+"114.249.31.41")
        #r.raise_for_status()
        r.encoding=r.apparent_encoding
        print(r.text[:-500])
    except:
        print("爬取异常")   

def main():
    url="http://m.ip138.com/ip/asp?ip="
    print(getHTMLText(url))
    
    
main()   

