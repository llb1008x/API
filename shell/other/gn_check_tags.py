#-*- coding: utf-8 -*-
import string
import re
import os
import sys
import getopt
import subprocess
reload(sys)
sys.setdefaultencoding( "utf-8" )
text_characters = "".join(map(chr, range(32, 127))) + "\n\r\t\b"
_null_trans = string.maketrans("", "")

class OPtion():
    pass

Option=OPtion()
Option.patch=False
Option.Debug=False
Option.notstandTag={}
Option.nothasTag={}
Option.notmerryFile={}

def Usage():
    print '-h,--help: print help message.'
    print '-v, --version: print script version'
    print '--debug: debug'

def Version():
    print 'V2.0.0.a'

def OutPut(args):
    print 'Hello, %s'%args

def matchTagLine():
    #p_title="^[//|#|@|\*|<\!--|\s*~\s*]+\s*gionee\s*"
    p_title="^(//|#|@|/\*+|<\!--)\s*gionee\s+"
    #p_title="(//|#|<!--)+\s*gionee\s+"
    re_title = re.compile(p_title, re.IGNORECASE)

    p_type="<*(amigo_\w+|bug|gn_bsp_\w+|gn_oversea_\w+|gn_by_\w+)>*\s+"
    re_type = re.compile(p_type,re.IGNORECASE)

    p_auther="<*\w+>*\s+"
    re_auther  = re.compile(p_auther,re.IGNORECASE)

    #p_time="<*(\d{4}-*\d{2}-*\d{2})>*\s+"
    p_time="<*\d{2,4}-*\d{1,2}-*\d{1,2}>*\s+"
    re_time = re.compile(p_time,re.IGNORECASE)

    p_line= p_title + p_type +  p_auther + p_time
    #print p_line
    re_line = re.compile(p_line,re.IGNORECASE)

    return re_title, re_line


def checkAddFileTags(status, mdir, mfile):
    if status != "A" :  return	
    #if mfile.find('project/') != -1:
    #    tmpmfile=mfile.split('/',2)[-1]
    #else:
    #    tmpmfile=mfile

    #if not os.path.exists(os.path.join("%s/../"%mdir, tmpmfile)): return	
    re_title, re_line = matchTagLine()
    pathFile=os.path.join(mdir, mfile)
    hasTag=False
    notstandTag=[]
    nothasTag=[]
    data=[]

    with open(pathFile,'r') as handle: data=handle.readlines()
    if notcheckfile(data): return 
    for i in range(0, len(data)):
        line=data[i]
        line=line.strip()
        if not re_title.search(line): continue
        hasTag=True
        m_line = re_line.match(line)
        if not m_line: 
            #print line
            #print "不标准的tag的行数"
            notstandTag.append(i + 1)
    if not hasTag:
        nothasTag.append("%d-%d"%(1, len(data) + 1))
    if notstandTag:
        Option.notstandTag[mfile]=notstandTag
    if nothasTag:
        Option.nothasTag[mfile]=nothasTag


def Run(args, **kwargs):
    #print "running: ", " ".join(args)
    return subprocess.Popen(" ".join(args), **kwargs)

def parseTag(data, fileData, nothasTag, notstandTag):
    if re.match("^diff", data[0]): return
    re_title,re_line=matchTagLine()

    #print data

    tmpdata=[]
    start=-1
    hasmodify=False
    for i in range(0, len(data)):
        line=data[i]
        if re.match("^-", line): continue
        if re.match("^@@", line):  
            start=int(line.split()[2].strip('+').split(',')[0])
            offset=int(line.split()[2].strip('+').split(',')[1])
        if re.search("^\+[\w|\W]+",line): hasmodify=True
        tmpdata.append(line)

    if not hasmodify or start == -1 :  
        #print "Error: 不是一个正规的diff"
        return

    hasTag=False
    modifyStart=-1
    for line in tmpdata:
        tmpline=line.strip()
        #print tmpline
        if not re.search("^\+[\w|\W]+", tmpline): continue
        if modifyStart == -1 :  modifyStart =  start + tmpdata.index(line) - 1
        tmpline=tmpline.lstrip('+')
        tmpline=tmpline.strip()
        if not re_title.search(tmpline): continue
        hasTag=True
        m_line = re_line.match(tmpline)
        if not m_line: 
            #不标准的tag的行数
            notstandTag.append(start + tmpdata.index(line) - 1)

    if not hasTag:
        bound=20
        while (modifyStart - bound <= 0) and (bound > 0):
            bound -= 1
        #print modifyStart , bound
        tmpdata=fileData[modifyStart - bound: modifyStart-1]  
        count=0
        while tmpdata: 
            line = tmpdata.pop()
            tmpline=line.strip()
            count+=1
            if not re_title.search(tmpline): continue
            if re.search("[\w|\W]+(begin|start)",tmpline, re.IGNORECASE):
                hasTag=True
                m_line = re_line.match(tmpline)
                if not m_line: 
                    #不标准的tag的行数
                    notstandTag.append(modifyStart - count)
                break
            else:
                break

    if not hasTag:
        nothasTag.append("%d-%d"%(start, start+offset-1))

def notcheckfile(data):
    if not data: return 
    line=data[0]
    if not line : return False
    flags=['amigo_skip']
    re_title,re_line=matchTagLine()
    m_line = re_line.match(line)
    if not m_line: return False
    if m_line.group(2).lower() in flags: return True
    return False
    
def checkModifyFileTags(status, mdir, mfile):
    if status != "M": return
    pathFile=os.path.join(mdir, mfile)
    fileData=[]
    with open(pathFile,'r') as handle: fileData=handle.readlines()
    if notcheckfile(fileData): return 
    cmd=["git diff --cached %s"%pathFile]
    p = Run(cmd, stdout=subprocess.PIPE,stderr=subprocess.PIPE, shell=True, executable='/bin/bash')
    diffData=[]
    while True:  
        buff = p.stdout.readline()  
        diffData.append(buff.rstrip())
        if not buff and p.poll() != None:  
            break 
    p=0
    n=0
    nothasTag=[]
    notstandTag=[]
    l=len(diffData)
    while  p < l:
        line=diffData[p]
        m=re.match("^@@", line)
        if m:
            parseTag(diffData[n:p], fileData, nothasTag, notstandTag)
            n=p
        p+=1
    parseTag(diffData[n:p], fileData, nothasTag, notstandTag)
    if notstandTag:
        Option.notstandTag[mfile]=notstandTag

    if nothasTag:
        Option.nothasTag[mfile]=nothasTag


def checkRepeatedFile(status, mdir, mfile):
    msplit = mfile.split('/', 1)
    if len(msplit) != 2: return
    cmd=["git diff --cached --name-only"]
    p = Run(cmd, stdout=subprocess.PIPE,stderr=subprocess.STDOUT, shell=True, executable='/bin/bash')
    data=set([])
    while True:  
        buff = p.stdout.readline()  
        data.add(buff.rstrip())
        if not buff and p.poll() != None:  
            break 
    #print data
    data.remove(mfile)

    mRoot=msplit[0]
    mPath=msplit[1]
    baseDirList=['alps_build', 'alps_drv', 'alps', 'project']
    notmerryFile=[]
    if mRoot in baseDirList: 
        i=baseDirList.index(mRoot)
        for fdir in baseDirList[i:]:
            if fdir == mRoot: continue
            sameFile="%s/*%s"%(fdir, mPath)
            cmd=["git ls-files" , "%s"%sameFile]
            p = Run(cmd, stdout=subprocess.PIPE,stderr=subprocess.STDOUT, shell=True, executable='/bin/bash')
            gitFiles=set([])
            while True:  
                buff = p.stdout.readline()  
                gitFiles.add(buff.rstrip())
                if not buff and p.poll() != None:  
                    break 
            for f in gitFiles:
                if f not in data:
                    notmerryFile.append(f)

    if notmerryFile:
        Option.notmerryFile[mfile]=notmerryFile


def isTextFile(s, text_characters=text_characters, threshold=0.30):
    # 若s包含了空值，它不是文本  
    if "\0" in s:return False  
    # 一个"空"字符串是"文本"（这是一个主观但又很合理的选择）  
    if not s:return True  
    # 获得s的由非文本字符构成的子串  
    t = s.translate(_null_trans, text_characters)  
    # 如果不超过30%的字符是非文本字符，s是字符串  
    return len(t)/len(s) <= threshold 

def main(argv):
    try:
        opts, args = getopt.getopt(argv[1:], 'hp:', ['help','patch='])
    except getopt.GetoptError, err:
        print str(err)
        Usage()
        sys.exit(2)
    for o, a in opts:
        print o,a
        if o in ('-h', '--help'):
            Usage()
            sys.exit(0)
        elif o in ('-v', '--version'):
            Version()
            sys.exit(0)
        elif o in ('-p','--patch'):
            IsDiffPatch=True
        elif o in ('--debug'):
            Option.Debug=True
        else:
            print 'unhandled option'
            Usage()
            sys.exit(3)
    if len(args) != 3 : return
    status=args[0]
    gionee_dir=args[1]
    commit_msg_file=args[2]

    #with open(os.path.join(gionee_dir,commit_msg_file), 'r') as fd: 
    #    for m in fd.readlines():
    #        m=m.strip()
    #        if re.match("^\s*\d+\s+(patch|param|vendor)\s+", m, re.IGNORECASE): return

    #cmd=["git diff --cached --name-status"]
    #p = Run(cmd, stdout=subprocess.PIPE,stderr=subprocess.STDOUT, shell=True, executable='/bin/bash')
    #commitFiles=set([])
    #while True:  
    #    buff = p.stdout.readline()  
    #    commitFiles.add(buff.strip())
    #    if not buff and p.poll() != None:  
    #        break 
    #print commitFiles
    #print "check tags start 6"
    #for line in commitFiles:
    #    if not line : continue
    #    status=line.split()[0]
    mfile=commit_msg_file
    if not isTextFile(open(os.path.join(gionee_dir,mfile)).read(512)): return
    if os.path.basename(mfile) == 'gn_check_tags.py' : return
    if status == "A" : checkAddFileTags(status, gionee_dir,mfile)
    if status == "M" : checkModifyFileTags(status, gionee_dir, mfile)
    #checkRepeatedFile(status, gionee_dir, mfile)

    if Option.notstandTag:
        print "下列文件对应行数，Tag不标准:"
        for f,l in  Option.notstandTag.items():
            print "%s: %s"%(f,l)
        print ""
    if Option.nothasTag:
        print "下列文件对应修改点, 没有添加Tag, 请添加:"
        for f,l in  Option.nothasTag.items():
            print "%s: %s"%(f,l)
        print ""	
    #if Option.notmerryFile:
    #    print "下列文件有修改，但是对应文件没有同步:"
    #    for f,l in  Option.notmerryFile.items():
    #        print "%s: %s"%(f,l)
    #    print ""
if __name__ == '__main__':
    main(sys.argv)

