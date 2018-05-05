#!/usr/bin/env python
# coding=utf-8

#爬虫网络图片

import requests
import os

url="http://image.nationalgeographic.com.cn/2017/0211/20170211061910157.jpg"
root='/home/llb/project/API/study/python/daily_code/day_16/'
path=root+url.split('/')[-1]

def getHTMLText(url):
    try:
        if not os.path.exists(root):
           os.mkdir(root)
        if not os.path.exists(path):
            r=requests.get(url)
            with open(path,'wb') as f:
                f.write(r.content) 
                f.close()
                print("文件保存成功")  
        else:
            print("文件已存在")
    except:
        print("爬取异常")   

def main():
    print(getHTMLText(url))
    
    
main()   

