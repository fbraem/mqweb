import unittest
import httplib
import ConfigParser
import re
import json

'''
Baseclass for all our tests
'''
class MQWebTest(unittest.TestCase):

	'''
	HTTP headers for POST
	'''
	headers = {
		'Content-Type': 'application/json'
	}

	qmgr = ''
	mqWebHost = 'localhost'
	mqWebPort = 8081

	'''
	Setup the test. Reads the config file test.cfg.
	The config file can contain the following properties:
		[MQWeb]
		host=host where MQWeb daemon runs (default is localhost)
		port=port used by MQWeb daemon to listen for incoming HTTP 
			 requests (default is 8081)
		[MQ]
		qmgr=queuemanager to use for testing
	'''
	@classmethod
	def setUpClass(cls):
		config = ConfigParser.ConfigParser()
		config.add_section('MQWeb')
		config.set('MQWeb', 'host', MQWebTest.mqWebHost)
		config.set('MQWeb', 'port', str(MQWebTest.mqWebPort)) 
		config.read('test.cfg')
		
		MQWebTest.qmgr = config.get('MQ', 'qmgr')
		MQWebTest.mqWebHost = config.get('MQWeb', 'host')
		MQWebTest.mqWebPort = config.get('MQWeb', 'port')

		print
		print 'Running test with the following configuration:'
		print 'MQWeb Host: ' + MQWebTest.mqWebHost
		print 'MQWeb Port: ' + MQWebTest.mqWebPort
		print 'MQ QueueManager: ' + MQWebTest.qmgr
		print

	'''
	MQ Attributes that can't be resolved by the dictionary stored in the SQLite
	database are returned as id_<nnnn>, where <nnnn> is the value of the C 
	constant. This method checks if there are any of these properties.
	'''
	def checkIds(self, data):
		ids = []
		for key in data:
			m = re.search(r"id_([0-9]+)", key)
			if m :
				ids.append(int(m.group(1)))

		if len(ids) > 0:
			print json.dumps(data, indent=4)
			print str(ids)
			return False
			
		return True

	'''
	HTTP GET implementation
	'''
	def getJSON(self, url):
		print 'Trying to connect to ' + url
		try:
			conn = httplib.HTTPConnection(self.mqWebHost, self.mqWebPort)
			conn.request('GET', url);
			res = conn.getresponse()
			data = json.loads(res.read())
		except httplib.HTTPException as e:
			print 'Exception Caught: ' + e.errno + e.strerror
			self.assertFalse(True, "Can't connect to MQWeb: " + 
				self.mqWebHost + ":" + 
				self.mqWebPort + 
				' (qmgr: ' + self.qmgr + ')')

		return data

	'''
	HTTP POST implemenation
	'''
	def appJSON(self, url, filter):
		print 'Trying to connect to ' + url
		try:
			conn = httplib.HTTPConnection(self.mqWebHost, self.mqWebPort)
			conn.request('POST', url, json.dumps(filter), self.headers)
			res = conn.getresponse()
			data = json.loads(res.read())
		except httplib.HTTPException as e:
			print 'Exception Caught: ' + e.errno + e.strerror
			self.assertFalse(True, "Can't connect to MQWeb: " + 
				self.mqWebHost + ":" + 
				self.mqWebPort + ' (qmgr: ' + self.qmgr + ')')

		return data
