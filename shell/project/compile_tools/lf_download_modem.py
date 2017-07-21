#!/usr/bin/python
#-*- coding: utf-8 -*-

import os
import sys
import getopt
from ftplib import *
import re
import time

class OPtions(object): pass
Options = OPtions()
Options.count=0
Options.product=''
Options.project=''
Options.fileList=[]
Options.fileDist={}
Options.ftpObj=''
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
        opts, args = getopt.getopt(argv[1:], 'hvD', ['help','--debug'])
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
        else:
            print 'unhandled option'
            Usage()
            sys.exit(1)

    try:
        Options.product=args[0]
        Options.project=args[0][0:7]
    except:
        pass
    finally:
        if not Options.product or not Options.project:
            print "请指定一个产品mk"
            sys.exit(2)

def ConnectFtp(ip='18.8.8.2',port=21,timeout=30):
    try:
        ftp = FTP()  
        ftp.connect(ip,port,timeout) 
        ftp.login('modem_dn','975969TY') 
    except e:  
        print 'ERROR:cannot reach " %s"' %e
        return  

    if Options.Debug: ftp.getwelcome()
    #ftp.set_debuglevel(2)
    Options.ftpObj=ftp

def CloseFtp():
    Options.ftpObj.quit()

def Print(line):
    if not line or not line.startswith('-'): return
    name = line.split()[-1]
    if not name.startswith(Options.product): return
    Options.fileList.append(os.path.join(Options.project,name))

def GetLastModem():
    ftp=Options.ftpObj
    
    ftp.dir(Options.project,Print)
    
    ntime=time.time()
    for name in Options.fileList:
        ctime = ftp.sendcmd("MDTM %s"%name)
        ctime = ctime[4:-4]
        second = time.mktime(time.strptime(ctime, "%Y%m%d%H%M%S"))
        dtime  = ntime - second
        if dtime < 0 : continue
        Options.fileDist[dtime] = name

    lastFile=''
    try:
        lastFile = Options.fileDist[min(Options.fileDist.keys())]
    except:
        pass

    if Options.Debug: 
        print "modem files: ", Options.fileDist
        print "last is: ", lastFile
    return lastFile

def GetFile(filePath):
    if not filePath : return ''
    modemName = os.path.basename(filePath)
    if os.path.exists(modemName): 
        if Options.Debug: print modemName, "is exist, not download again."
        return  modemName
    f = open(modemName,'wb')         
    filename = 'RETR ' + filePath
    Options.ftpObj.retrbinary(filename,f.write)
    f.close()
    return modemName

def main(argv):
    Getopt(argv)
    ConnectFtp()
    modemPath = GetLastModem()
    modemName = GetFile(modemPath)
    CloseFtp()
    print modemName

if __name__ == '__main__':
    main(sys.argv)

