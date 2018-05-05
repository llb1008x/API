#!/usr/bin/env python
# coding=utf-8

#检查用户名和pin码
database = [
    ['albert','1234'],
    ['dilbert','4242'],
    ['smith','7524'],
    ['jones','9843'],
    ['llb','1008']
]

username = raw_input('user name:')
pin = raw_input('PIN code:')

if [username,pin] in database:
    print 'Acess granted'
else :
    print 'erro acess deny'

