#!/usr/bin/env python
# coding=utf-8

#wordcloud的使用
import jieba
import wordcloud  
from scipy.misc import imread


#需要处理的文本
txt="程序设计语言是计算机能够理解和识别用户操作意图的一直陪嗯交互体系，\
能够按照 规则组织计算机指令， 是计算机能够自动进行各种运算处理"
mk=imread("apple-red.png")
#生成词云对象，并设定参数
w=wordcloud.WordCloud(width=10000,
                    font_path="./HYYuanLongHei-35J.ttf",
                    height=7000,
                    )
#输出词云对象
w.generate(" ".join(jieba.lcut(txt)))
w.to_file("pywcloud.png")



