import unittest
import json
from testbase import MQWebTest

'''
Test for QueueController
'''
class TestQueueActions(MQWebTest):

	'''
	 Test Inquire with HTTP GET
	'''
	def testInquire(self):

		print
		
		data = self.getJSON('/api/queue/inquire/' + self.qmgr)
		
		self.assertFalse('mqweb' not in data, 'No mqweb data returned')

		if 'error' in data:
			self.assertFalse(True, 'Received a WebSphere MQ error:' + 
				str(data['error']['reason']['code']))

		self.assertFalse('queues' not in data, 'No queues returned')
		self.assertFalse(len(data['queues']) == 0, 
			'Empty JSON array returned. ' + 
			'Does MQWeb have permission to view this information?')

		self.assertTrue(self.checkIds(data['queues'][0]), 
			'There are unmapped Websphere MQ attributes')

	'''
	 Test Inquire with HTTP POST
	'''
	def testInquireJSON(self):

		print
		
		filter = { "QName" : "*" }
		data = self.appJSON('/api/queue/inquire/' + self.qmgr, filter)
		self.assertFalse('mqweb' not in data, 'No mqweb data returned')

		if 'error' in data:
			self.assertFalse(True, 'Received a WebSphere MQ error:' 
				+ str(data['error']['reason']['code']))
        
		self.assertFalse('queues' not in data, 'No queues returned')
		self.assertFalse(len(data['queues']) == 0, 
			'Empty JSON array returned. ' + 
			'Does MQWeb have permission to view this information?')

suite = unittest.TestLoader().loadTestsFromTestCase(TestQueueActions)
unittest.TextTestRunner(verbosity=2).run(suite)
