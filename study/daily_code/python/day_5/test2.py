#!/usr/bin/env python
# coding=utf-8

import string as str

table = str.maketrans('cs','kz')
print len(table)

print table[97:123]

print str.maketrans('','')[97:123]
