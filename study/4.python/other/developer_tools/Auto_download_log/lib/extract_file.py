#!/usr/bin/env python
# encoding: utf-8

import os
import re
import sys
import getopt
import urllib
import shutil
import threading
import zipfile
import subprocess
import chardet
from time import sleep

reload(sys)
sys.setdefaultencoding('utf-8')
sys.path.append('lib')

import common

class OPtions(object):
	pass

Options = OPtions()
Options.tool = 'tool/QAAT/aee_extract'

class ExtractFile:
	winTool = False

	def __init__(self, filepath):
		self.filepath = os.path.abspath(filepath)
		self.outdir = ''

	def __del__(self):
		pass

	def Run(self, args, **kwargs):
		return subprocess.Popen(args, **kwargs)

	def getFileType(self):
		if self.filepath.endswith('dbg') or self.filepath.endswith('db'):
			return 'db'
		if self.filepath.endswith('zip') or self.filepath.endswith('JE') or self.filepath.endswith('SWT'):
			return 'zip'
		if self.filepath.endswith('KE'):
			return 'ke'
		if self.filepath.endswith('rar'):
			return 'rar'
		return None

	def get_file(self):
		print "laihhhh"
		return self.filepath

	def extract_file(self):
		if self.filepath.endswith('dbg') or self.filepath.endswith('db'):
			self.extract_db()
		if self.filepath.endswith('zip') or self.filepath.endswith('JE') or self.filepath.endswith('SWT'):
			self.extract_zip()
		if self.filepath.endswith('KE'):
			self.extract_ke()
		if self.filepath.endswith('rar'):
			self.extract_rar()
		return None
	

	def extract_rar(self):
		self.outdir = self.filepath + '.DEC'
		print self.outdir
		if not os.path.exists(self.outdir):
			os.makedirs(self.outdir)
		cmd = [
			'unrar',
			'x',
			'-o-',
			self.filepath,
			self.outdir]
		p = self.Run(cmd, stdout = subprocess.PIPE, stderr = subprocess.PIPE)
		(o, e) = p.communicate()
		if p.returncode != 0:
			print e
		else:
			with open('%s.DEC/URL' % self.filepath, 'w') as fd:
				fd.write(self.filepath)

	def extract_db(self):
		if ExtractFile.winTool:
			cmd = [
				'wine',
				Options.tool,
				self.filepath]
		else:
			cmd = [
				Options.tool,
				self.filepath]
		p = self.Run(cmd, shell = False, stdout = subprocess.PIPE, stderr = subprocess.PIPE)
		(o, e) = p.communicate()
		if p.returncode != 0:
			print e
		else:
			with open('%s.DEC/URL' % self.filepath, 'w') as fd:
				fd.write(self.filepath)
			self.outdir = self.filepath + '.DEC'

	def extract_ke(self):
		cmd = [
			'./gn_exp_qcom_ke.py',
			'-i',
			self.filepath,
			'-o',
			'%s.DEC' % self.filepath]
		p = self.Run(cmd, shell = False, stdout = subprocess.PIPE, stderr = subprocess.PIPE)
		(o, e) = p.communicate()
		if p.returncode != 0:
			print e
		else:
			with open('%s.DEC/URL' % self.filepath, 'w') as fd:
				fd.write(self.filepath)
			self.outdir = self.filepath + '.DEC'

	def extract_zip2(self):
		outdir = self.filepath + '.DEC'
		cmd = 'unzip -aq %s -d %s' % (self.filepath, outdir)
		print cmd
		ret = os.system(cmd)
		if ret == 0:
			self.outdir = outdir

	def extract_zip(self):
		try:
			zp = zipfile.ZipFile(self.filepath)
		except Exception:
			err = None
			print 'zipError: %s %s' % (err, self.filepath)
			return ''

		self.outdir = self.filepath + '.DEC'
		print self.outdir
		for zinfo in zp.infolist():
			#print zinfo
			try:
				name = zinfo.filename
				if type(name) != type(u'unicode'):
					coding = chardet.detect(name)['encoding']
					if coding:
						name = unicode(name, coding)
				name = common.strQ2B(name)
				name = re.sub('\\s', '_', name)
				if not name.endswith('/'):
					f = os.path.join(self.outdir, name)
					dir = os.path.dirname(f)
					if not os.path.exists(dir):
						os.makedirs(dir)
					file(f, 'wb').write(zp.read(zinfo.filename))

			except Exception,e:
				err = None
				print 'zipError: %s %s' % (e, self.filepath)
				return ''

				