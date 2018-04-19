#!/usr/bin/env python
# coding=utf-8

height,weight = eval(input("请输入身高(米)和体重(公斤)："))

def func(height,weight):
    bmi = weight / pow(height,2)
    who,nat = "",""
    if bmi < 18.5:
        who,nat = "偏瘦","偏瘦"
    elif 18.5 <= bmi < 24:
        who,nat = "正常","正常"
    elif 24 <= bmi < 25:
        who,nat = "正常","偏胖"
    elif 25 <= bmi < 28:
        who,nat = "偏胖","偏胖"
    elif 28 <= bmi < 30:
        who,nat = "偏胖","肥胖"
    
    return bmi
    

print("BMI 数值为：{:.2f}".format(func(height,weight)))
