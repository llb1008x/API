#!/usr/bin/env python
# encoding: utf-8

import re
import os
import sys
import getopt
import ftplib
import subprocess


class OPTIONS(object):
    
    def __init__(self):
        self.input = ''
        self.output = ''
        self.Debug = False
        self.config = ''

Options = OPTIONS()
COMMON_DOCSTRING = '\n    -c (--config)\n        Specify a bugid config file.\n        \n    -i (--input)\n        Specify a input argv. eg: BugId\n\n    -o (--ouput)\n        Specify a output argv. eg: pathfile/pathdir.\n\n    -D (--debug) \n        debug\n\n    -h (--help)\n        Display this usage message and exit.\n'





def Usage(docstring):
    print docstring
    print COMMON_DOCSTRING

def GetOptions(argv, extra_opts = '', extra_long_opts = [], extra_opts_handler = None, extra_usage = ''):
    
    try:
        (opts, args) = getopt.gnu_getopt(argv, 'hi:o:Dc:' + extra_opts, [
            'help',
            'input=',
            'output=',
            'Debug',
            'config='] + extra_long_opts)
    except getopt.GetoptError:
        err = None
        Usage(extra_usage)
        print '**', str(err), '**'
        sys.exit(2)

    for (o, a) in opts:
        if o in ('-h', '--help'):
            Usage(extra_usage)
            sys.exit()
            continue
        if o in ('-i', '--input'):
            Options.input = a
            continue
        if o in ('-o', '--output'):
            Options.output = a
            continue
        if o in ('-c', '--config'):
            Options.config = a
            continue
        if o in ('-D', '--Debug'):
            Options.Debug = True
            continue
        if not extra_opts_handler is None:
        	if not extra_opts_handler(o, a) or False:
        		raise AssertionError('unknown option "%s"' % (o,))  
    return args


def Run(args, **kwargs):
    return subprocess.Popen(args, stdout = subprocess.PIPE, stderr = subprocess.PIPE, **kwargs)


def strQ2B(ustring):
    rstring = ''
    for uchar in ustring:
        inside_code = ord(uchar)
        if inside_code == 12288:
            inside_code = 32
        elif inside_code >= 65281 and inside_code <= 65374:
            inside_code -= 65248
        rstring += unichr(inside_code)
    
    return rstring

def downloadFromUrl(url, pathfile):
	if not os.path.exists(os.path.dirname(pathfile)):
		os.makedirs(os.path.dirname(pathfile))
	cmd = 'wget -c ' + url + ' -O ' + pathfile + ' -o ' + '/dev/null'
	ret = os.system(cmd)
	if ret == 0:
		return pathfile
	print 'download: %s failed' % url
	return ''  

