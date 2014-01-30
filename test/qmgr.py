import unittest
import httplib
import json
import re
import ConfigParser

class MQWebTest(unittest.TestCase):

	def setUp(self):
		config = ConfigParser.ConfigParser()
		config.add_section('MQWeb')
		config.set('MQWeb', 'host', 'localhost')
		config.set('MQWeb', 'port', '8081') 
		config.read('test.cfg')
		self.qmgr = config.get('MQ', 'qmgr')
		self.mqWebHost = config.get('MQWeb', 'host')
		self.mqWebPort = config.get('MQWeb', 'port')

		print
		print 'Running test with the following configuration:'
		print 'MQWeb Host: ' + self.mqWebHost
		print 'MQWeb Port: ' + self.mqWebPort
		print 'MQ QueueManager: ' + self.qmgr
		print

class TestQueueManagerActions(MQWebTest):

	def testInquire(self):
		try:
			conn = httplib.HTTPConnection(self.mqWebHost, self.mqWebPort)
			conn.request('GET', '/qmgr/inquire/' + self.qmgr)
			res = conn.getresponse()
			data = json.loads(res.read())
		except:
			self.assertFalse(True, "Can't connect to MQWeb: " + self.mqWebHost + ":" + self.mqWebPort)
		
		self.assertFalse(data['mqweb'] == None, 'No mqweb data returned')
		self.assertFalse(data['qmgr'] == None, 'No queuemanager data returned')

		ids = []
		for key in data['qmgr']:
			m = re.search(r"id_([0-9]+)", key)
			if m :
				ids.append(int(m.group(1)))

		if len(ids) > 0:
				print json.dumps(data['qmgr'], indent=4)
				print str(ids)

		self.assertTrue(len(ids) == 0, 'There are unmapped WebSphere MQ attributes!')

suite = unittest.TestLoader().loadTestsFromTestCase(TestQueueManagerActions)
unittest.TextTestRunner(verbosity=2).run(suite)
