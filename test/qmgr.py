import unittest
from testbase import MQWebTest

'''
Test QueueManagerController
'''
class TestQueueManagerActions(MQWebTest):

	'''
	Test Inquire HTTP GET
	'''
	def testInquire(self):

		print
		
		data = self.getJSON('/api/qmgr/inquire/' + self.qmgr)
		
		self.assertFalse('mqweb' not in data, 'No mqweb data returned')

		if 'error' in data:
			self.assertFalse(True, 'Received a WebSphere MQ error:' +
				str(data['error']['reason']['code']))

		self.assertFalse('qmgr' not in data, 'No queuemanager data returned')

		self.assertTrue(self.checkIds(data['qmgr']), 
			'There are unmapped Websphere MQ attributes')

	'''
	 Test Inquire with HTTP POST
	'''
	def testInquireJSON(self):

		print
		
		filter = { }

		data = self.appJSON('/api/qmgr/inquire/' + self.qmgr, filter)
		self.assertFalse('mqweb' not in data, 'No mqweb data returned')

		if 'error' in data:
			self.assertFalse(True, 'Received a WebSphere MQ error:' 
				+ str(data['error']['reason']['code']))

		self.assertFalse('qmgr' not in data, 'No information returned')

suite = unittest.TestLoader().loadTestsFromTestCase(TestQueueManagerActions)
unittest.TextTestRunner(verbosity=2).run(suite)
