#!/usr/bin/env python
# coding=utf-8


#使用给定的宽度打印格式化后的价格列表
width = input('please enter width')

price_width = 10
item_width = width-price_width

header_format = '%-*s%*s'
format = '%-*s%*.2f'

print '=' * width
print header_format % (item_width,'Item',price_width,'Price')

print '-' * width

print format % (item_width,'Apples',price_width,0.4)
print format % (item_width,'Pears',price_width,0.5)
print format % (item_width,'Cantaloupes',price_width,1.92)
print format % (item_width,'Dried Apricots(16 oz.)',price_width,8)
print format % (item_width,'Prunes(4 1bs.)',price_width,12)

print '=' * width


'''
dirs = '','usr','bin','env'
'/'.join(dirs)
'''
