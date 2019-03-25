#!/usr/bin/env python
# coding=utf-8


#根据给定的年月日以数字的形式打印出日期
months=[
    'January',
    'February',
    'March',
    'April',
    'MAy',
    'June',
    'July',
    'August',
    'September',
    'October',
    'November',
    'December'
]

#以1-31的数字作为结尾的列表
endings = ['st','nd','rd']+17*['th']\
        +['st','nd','rd']+7*['th']\
        +['st']

year = raw_input('year:')
month = raw_input('month(1-12):')
day = raw_input('day(1-31):')

month_number = int(month)
day_number = int(day)

#月份和天数减1,作为索引
month_name = months[month_number-1]
ordinal = day+endings[day_number-1]

print month_name+' '+ordinal+','+year
