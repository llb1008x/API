#!/usr/bin/python
#-*- coding: utf-8 -*-
import re
import os
import sys
import shutil
import chardet

reload(sys)
sys.setdefaultencoding("utf8")
sys.path.append('lib')

import common

Options = common.Options
Options.ignore=True
Options.input=os.path.abspath('./log')
Options.plotdir=os.path.abspath('./plot')
Options.plotfile=''
Options.keyword='[\s|\S]*'
Options.CRInfo={}
Options.PlotData=[]
Options.outTraces='outtraces'

usage_str='''
    -p (--plot)
        指定一个策略文件或目录,默认为plot目录下的所有策略。如: plot/test.tx 、plot/filedir 

    -I (--igoner)
        根据策略文件查找log关键字时，不忽略大小写。默认为忽略。    

    -k (--keyword)
        指定keyword, 替换策略文件中， method key 中的 $gnaikeyword 值，可以动态修改。
        如果没有指定，$gnaikeyword 变量默认为任意值。    
'''

def Getopt(argv):
	def options_handle(o, a):
		if o in ('-p', '--plot'):
			Options.plotfile=a
		elif o in ('-I','--ignore'):
			Options.ignore=False
		elif o in ('-k', '--keyword'):
			Options.keyword=a
		else:
			return False
		return True
	common.GetOptions(argv,
		extra_opts = 'p:I:k:',
		extra_long_opts =['plotfile=', 'ignore', 'keyword='],
		extra_opts_handler = options_handle,
		extra_usage = usage_str
		)
	
	if os.path.exists(Options.outTraces):
	    shutil.rmtree(Options.outTraces)
	os.makedirs(Options.outTraces)

def findLocalLogTime(logdir, info):
	for root, dirs, files in os.walk(logdir):
		if 'LOGTIME' in files :
			time= open(os.path.join(root, 'LOGTIME'), 'r').read()
			info['time']=time

def GetLogFromLocal():
    info={}
    info['time']=''
    if Options.input and os.path.isdir(Options.input):
        info['pathdir']=set([])
        info['pathdir'].add(Options.input)
        findLocalLogTime(Options.input, info)
        Options.CRInfo['local']=info
    if Options.input and os.path.isfile(Options.input):
        info['pathfile']=[os.path.abspath(Options.input)]
        extractCRLog(info)
        extractDBG(info)
        Options.CRInfo['local']=info

def ParsePlot(plotfile):
	with open(plotfile,'r') as fd:
		plot={}
		methods=[]
		plot['plotfile']=plotfile
		for line in fd.readlines():
			line=line.strip()
			if not line: continue
			if line.startswith('#'): continue
			key = line.split(':',1)[0]
			value = line.split(':',1)[1]
			if key.lower() == 'method':
				method={}
				for l in value.split(','):
					if not l : continue
					method[l.split('=',1)[0].strip()]=l.split('=',1)[1].strip()
				methods.append(method)
			else:
				coding = chardet.detect(value.strip())['encoding']

				if coding:
					plot[key.strip()]=value.strip().decode(coding)
				else:
					plot[key.strip()]=value.strip()
		plot['methods']=methods
		Options.PlotData.append(plot)



def GetLocalPlots():
	if Options.plotfile:
		if os.path.isfile(Options.plotfile):
			ParsePlot(Options.plotfile)
		elif os.path.isdir(Options.plotfile):
			for root, dirs, files in os.walk(Options.plotfile):
				for f in files:
					if f.startswith('.'):  continue
					ParsePlot(os.path.join(root, f))

		else:
			print "策略文件类型错误。"

	else:
		for root, dirs, files in os.walk(Options.plotdir):
			for f in files:
				if f.startswith('.'):  continue
				ParsePlot(os.path.join(root, f))

	#print Options.PlotData

def ParseFile(logpathfile,plotkey, plotline, traces, info):
	buf=[]
	data=[]
	with open(logpathfile, 'r') as fd: buf=fd.readlines()
	plotkey = plotkey.replace('$gnaikeyword',Options.keyword)
	i = 0 
	while 0 <= i < len(buf):
		line = buf[i]
		if Options.ignore:
			if not re.search(str(plotkey), line, re.IGNORECASE):
				i = i + 1
				continue
		else:
			if not re.search(str(plotkey), line):
				i = i + 1
				continue

		if plotline:
			if plotline > 0:
				if (i + plotline) < len(buf):
					data.append(''.join(buf[i:i+plotline]))
					i = i + plotline
				else:
					data.append(''.join(buf[i:len(buf)]))
					break
			else:
				if (i + plotline) > 0 :
					data.append(''.join(buf[i+plotline:i + 1]))
				else:
					data.append(''.join(buf[0:i + 1]))
		else:
			pattern='(\d{2}-\d{2} \d{2}:\d{2}:\d{2}.\d+)[\d\s]+[EDIVW]\s(\w+)?\s*:'
			m=re.match(pattern, line)
			if m:
				trace=''
				errorTag=m.group(2)
				for j in range(i,len(buf)):
					line = buf[j]
					if line.find(errorTag) == -1: break
					trace += line

				if trace:
					data.append(trace)
					i = i + j

			else:
				data.append(line)

		i = i + 1

	if data:
		traces.append((logpathfile, data))





def ParseCR(info):
	for pathdir in info['pathdir']:
		info['logfiles']=[]
		for root, dirs, files in os.walk(pathdir):
			for f in files:
				for p in Options.PlotData:
					if not p['methods']: continue
					target=[]
					methods=p['methods']
					for m in methods:
						plotfile=m['file']
						plotkey=m['key'].strip()
						plotline=int(m.get('line',0))
						traces=[]
						if re.match(str(plotfile), f, re.IGNORECASE):
							logpathfile=os.path.join(root, f)
							ParseFile(logpathfile,plotkey, plotline, traces, info)

						if traces:
							target.append((m, traces))

					if target:
						info['targets'].append((p,target))


def PasreLogFromPlot():
	'''
		targets is [(p, target)]
		target is (m, traces)
		traces is (logfile, data)
		data is []
	'''
	for cr, info in Options.CRInfo.items():
		info['targets']=[]
		ParseCR(info)

def printTargrt():
    #fd = open('outfile_traces.txt', 'w')
    for cr, info in Options.CRInfo.items():
        if not info['targets']:  continue
        fd = open('%s/%s_traces.txt'%(Options.outTraces,cr), 'w')
        
        if info.get('targets'): 
            print '*****', 'Bug:', cr, r'时间:', info['time'], '*****'
            print >> fd, '*****','Bug:', cr, r'时间:', info['time'], '*****'

            for p, target in info['targets']:
            	print r"问题：", p.get('name', None)
            	print >> fd, r"问题：", p.get('name', None)
            	print r"类是bug ID:", p.get('id', None)
            	print >> fd, r"类是bug ID:", p.get('id', None)
            	for m, traces in target:
            		for f, trace in traces:
            			print f
            			print >> fd, f
            			for t in trace:
            				print t
            				print >>fd, t
            				print '--------------'
            				print >> fd , '--------------'
    fd.close()


def main(argv):
	Getopt(argv)
	GetLogFromLocal()
	GetLocalPlots()
	PasreLogFromPlot()
	printTargrt()


if __name__ == '__main__':
    main(sys.argv)
