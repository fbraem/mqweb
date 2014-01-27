import unittest
import httplib
import json
import ConfigParser

class TestQueueManagerActions(unittest.TestCase):

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

	def testInquire(self):
		conn = httplib.HTTPConnection(self.mqWebHost, self.mqWebPort)
		conn.request('GET', '/qmgr/inquire/' + self.qmgr)
		res = conn.getresponse()
		data = json.loads(res.read())
		
		print json.dumps(data, indent=4)
		
		self.assertTrue(data['qmgr'] != None, 'No queuemanager data returned')
		
suite = unittest.TestLoader().loadTestsFromTestCase(TestQueueManagerActions)
unittest.TextTestRunner(verbosity=2).run(suite)
