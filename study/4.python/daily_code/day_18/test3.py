#!/usr/bin/env python
# coding=utf-8

#中国大学排名
import requests
from bs4 import BeautifulSoup
import bs4 
import sys


reload(sys)
sys.setdefaultencoding('utf-8')

url="http://www.zuihaodaxue.cn/zuihaodaxuepaiming2016.html"


#获取网页内容
def getHTMLText(url):
    try:
        r=requests.get(url)
        #r.raise_for_status()
        r.encoding=r.apparent_encoding
        return r.text
    except:
        print("warning!")
        return ""
     
    
    
#提取网页内容
def fillUnivList(ulist,html):
    soup=BeautifulSoup(html,'html.parser')
    for tr in soup.find("tbody").children:
        if isinstance(tr,bs4.element.Tag):
            tds=tr('td')
            ulist.append([tds[0].string,tds[1].string,tds[3].string])
        
    pass
    
#打印网页内容
def printUnivList(ulist,num):
#中文输出对齐 ?
    tlpt="{0:^10}\t{1:{3}^10}\t{2:^10}"
    #print(tlpt.format("排名","学校名称","总分",chr(12288)))
    print("{:^10}\t{:^6}\t{:^10}".format("排名","学校名称","总分"))
    for i in range(num):
        u=ulist[i]
        print("{:^10}\t{:^6}\t{:^10}".format(u[0],u[1],u[2]))
        
    print("Suc"+str(num))
   
    
    
def main():
    uinfo=[]
    html=getHTMLText(url)
    fillUnivList(uinfo,html)
    printUnivList(uinfo,20)




main()    

