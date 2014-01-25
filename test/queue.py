import unittest
import httplib
import json
import ConfigParser

class TestQueueActions(unittest.TestCase):

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
		conn.request('GET', '/queue/inquire/' + self.qmgr)
		res = conn.getresponse()
		data = json.loads(res.read())
		
		self.assertTrue(len(data['queues']) != 0, 'No queues returned')
		
suite = unittest.TestLoader().loadTestsFromTestCase(TestQueueActions)
unittest.TextTestRunner(verbosity=2).run(suite)
