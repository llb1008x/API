#!/usr/bin/python
#-*- coding: utf-8 -*-
import re
import os
import sys

reload(sys)
sys.setdefaultencoding("utf8")
sys.path.append('lib')

import common
import byredmine
import extract_file

Options = common.Options
Options.apikey=""
Options.queryid=""
Options.bugids=""
Options.output=os.path.abspath('./log')

usage_str='''
    -a (--apikey) 
        指定用户的APIkey。通过redmine账户获取。

    -q (--queryid)
        指定查询ID。已经创建查询的ID。
        
    -b (--bugids)
        指定bugid，可以制定一组id用英文的逗号分隔。   
'''


def Getopt(argv):
	def options_handle(o, a):
		if o in ('-a', '--apikey'):
			Options.apikey=a
		elif o in ('-q','--queryid'):
			Options.queryid=a
		elif o in ('-b','--bugids'):
		    Options.bugids=a
		else:
			return False
		return True

	common.GetOptions(argv,
		extra_opts = 'a:q:b:',
		extra_long_opts =['apikey=', 'queryid=','bugids='],
		extra_opts_handler = options_handle,
		extra_usage = usage_str
		)
	if not os.path.exists(Options.output):
	    os.makedirs(Options.output)

def getLogFromDec(data,info):
	info['url']=[]
	info['time']=''

	pattern="ftp://\S+?(\.zip|\.rar)"
	for line in data['Description'].split():
		if re.search('ftp://', line):
			m=re.search(pattern, line)
			if m: info['url'].append(m.group())
		else:
			m = re.search("(\d{1,2})[:|;|,|\.](\d{1,2})", line)
			if m:
				h = m.group(1)
				M = m.group(2)
				if 0 <= int(h) < 24 and  0 <= int(M) < 60:
					info['time']="%s:%s"%(h,M)

def getLogFromNote(data,info):
	if not data['Journals']: return
	pattern="ftp://\S+?(\.zip|\.rar)"
	for low in data['Journals']:
		notes = low.get('notes')
		if not notes:  continue
		m=re.search(pattern, notes, re.M)
		if not m:  continue
		info['url'].append(m.group())
		for line in re.split('\n*| *', notes):
			if re.search("(\d{1,2})[:|;|,|\.](\d{1,2})", line) and not re.search('ftp', line):
				m=re.search("(\d{1,2})[:|;|,|\.](\d{1,2})", line)
				info['time']="%s:%s"%(m.group(1),m.group(2))
				return

def downloadCRLog(data, info):
	info['pathfile']=[]
	for url in info['url']:
		logname=os.path.basename(url)
		logpathname=os.path.join(Options.output,str(data['Id']),logname)
		print url
		print logpathname
		if common.downloadFromUrl(url, logpathname):
			print logpathname, info['time']
			info['pathfile'].append(logpathname)

def extractCRLog(info):
	info['pathdir']=set([])
	for logPathName in info['pathfile']:
		E=extract_file.ExtractFile(logPathName)
		E.extract_file()

		if E.outdir : 
			info['pathdir'].add(E.outdir)
			if info['time']:
				fd = open(os.path.join(E.outdir, 'LOGTIME'), 'w')
				fd.write(info['time'])
				fd.close()

def extractDBG(info):
	for pathdir in info['pathdir']:
		for root, dir, files in os.walk(pathdir,topdown=False):
			for f in files:
				if f.endswith('dbg') or f.endswith('db'):
					db_file=os.path.join(root,f)
					E=extract_file.ExtractFile(db_file)
					E.extract_db()

def GetLogFromRedmine():
	datas=[]
	redmine=byredmine.ByRedmine(Options.apikey)
	if len(Options.queryid)>0:
	    redmine.getIssuesByConditionId(Options.queryid)
	if len(Options.bugids)>0:
	    redmine.getIssuesById(Options.bugids)
	datas += redmine.issuesToData()
	redmine.closeIssues()

	if datas: print "[AI研发] 找到 %d 个Bug"%len(datas)
	while datas:
		info={}
		data = datas.pop()
		getLogFromDec(data, info)
		getLogFromNote(data, info)
		downloadCRLog(data, info)
		extractCRLog(info)
		extractDBG(info)


	if Options.Debug:
		for k,v in Options.CRInfo.items():
			print k,v
        
def main(argv):
	Getopt(argv)
	if len(Options.apikey)>0 and  (len(Options.queryid)>0 or  len(Options.bugids)>0):
		GetLogFromRedmine()


if __name__ == '__main__':
    main(sys.argv)
