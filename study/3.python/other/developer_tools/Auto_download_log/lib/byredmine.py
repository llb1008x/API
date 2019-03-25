#!/usr/bin/env python
# encoding: utf-8

import os
import re
import sys
import json
import urllib2
from redmine import Redmine
from dateutil.relativedelta import relativedelta

reload(sys)
sys.setdefaultencoding('utf-8')
sys.path.append('lib')

import common

class ByRedmine:
	def __init__(self,key):
		self.by = Redmine('http://by.gionee.com', key = key)
		self.apikey = key


	def getNotesByIssue(self, issue):
		url = 'http://by.gionee.com/issues/%s.json?include=journals&key=%s' % (issue.id , self.apikey)
		page = urllib2.urlopen(url).read()
		jsonData = json.loads(page)
		journals = jsonData['issue']['journals']
		return journals

	def issuesToData(self):
		data = []
		for issue in self.issues:
			cr = { }
			cr['issue'] = issue
			try:
				cr['Id'] = issue.id
				cr['Status'] = issue.status.name
				cr['Headline'] = issue.subject
				cr['Description'] = common.strQ2B(issue.description.decode('utf-8'))
				cr['updated_on'] = issue.updated_on
			except Exception:
				e = None
				print issue.id, 'not qualified.'
				continue

			try:
				cr['Journals'] = self.getNotesByIssue(issue)
			except Exception:
				e = None
				cr['Journals'] = { }
				print issue.id, 'not Journals'
			data.append(cr)

		return data
              

	def getIssuesByConditionId(self, cid):
		self.issues = self.by.issues(query_id = cid)
	
	def getIssuesById(self, bugids):
	    self.issues = self.by.issues(issue_id = bugids)

	def closeIssues(self):
		self.issues.close()

