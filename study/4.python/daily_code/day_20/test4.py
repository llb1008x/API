#!/usr/bin/env python
# coding=utf-8

#政府工作报告词云，对文件核心内容的展现
import jieba
import wordcloud
from scipy.misc import imread

#读入文件
f=open("新时代中国特色社会主义.txt",'rt')
t=f.read()
f.close()

#导入图片生成不同的形状
mask=imread("2.jpg")

#分词，生成词云
ls=jieba.lcut(t)
txt=" ".join(ls)
w=wordcloud.WordCloud(font_path="./HYYuanLongHei-35J.ttf",
                        mask=mask,
                        width=3000,
                        height=2400,
                        background_color="white"
                        )
#输出词云                        
w.generate(txt)
w.to_file("grwordcloud.png")                        
                        
