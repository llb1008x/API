#!/usr/bin/env python
# coding=utf-8

#百度搜索关键字 

import requests


keyword='python'

def getHTMLText(url):
    try:
        kv={"wd":keyword}
        r=requests.get(url,params=kv)
        print(r.request.url)
        r.raise_for_status()
        r.encoding=r.apparent_encoding
        print(len(r.text))
        
    except:
        return "产生异常"

        

def main():
    url="http://www.baidu.com/s"
    print(getHTMLText(url))
    
    
main()   

