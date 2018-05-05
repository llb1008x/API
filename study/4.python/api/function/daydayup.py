#!/usr/bin/env python
# coding=utf-8

def dayUP(df):
    dayup = 1
    for i in range(365):
        if i%7 in [6,0]:
            dayup = dayup*(1-dayfactor)
        else:
            dayup = dayup*(1+dayfactor)

    return dayup
     
dayfactor = 0.01
while dayUP(dayfactor) < 37.78  : 
    dayfactor += 0.0001
      
print("工作日的力量：{:.3f}".format(dayfactor))            
