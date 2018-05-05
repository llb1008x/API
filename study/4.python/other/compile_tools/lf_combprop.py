#!/usr/bin/python
#-*- coding: utf-8 -*-

import os
import sys
import getopt

class OPtions(object): pass
Options = OPtions()
Options.propfile=''
Options.Debug=False

def Usage():
    print '-h,--help: print help message.'
    print '-v, --version: print script version'

def Version():
    print 'V1.0.0.a'

def OutPut(args):
    print 'Hello, %s'%args

def Getopt(argv):
    try:
        opts, args = getopt.getopt(argv[1:], 'hvf:', ['help','--file'])
    except getopt.GetoptError, err:
        print str(err)
        Usage()
        sys.exit(2)

    for o, a in opts:
        #print o,a
        if o in ('-h', '--help'):
            Usage()
            sys.exit(0)
        elif o in ('-v', '--version'):
            Version()
            sys.exit(0)
        elif o in ('-D', '--debug'):
            Options.Debug=True
        elif o in ('-f', '--file'):
            Options.propfile=a
        else:
            print 'unhandled option'
            Usage()
            sys.exit(1)

    if not  os.path.exists(Options.propfile): 
        sys.write('prop file not exist\n')
        sys.exists(2)

def CombPropFile():
    d={}
    with open(Options.propfile,'r') as f: 
        for line in f.readlines():
            line=line.strip()
            if line.startswith('#') or not line: continue
            key = line.split('=')[0]
            value = line.split('=')[-1]
            d[key] = value
        f.close()

    l = d.keys()
    l.sort()

    with open(Options.propfile,'w') as f:
        for k in l: 
            str="%s=%s\n"%(k,d[k])
            f.write(str)
        f.close()

def main(argv):
    Getopt(argv)
    CombPropFile()

if __name__ == '__main__':
    main(sys.argv)

