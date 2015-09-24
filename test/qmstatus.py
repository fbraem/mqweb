import unittest
from testbase import MQWebTest

'''
Test QueueManagerStatusController
'''
class TestQManagerStatusActions(MQWebTest):

	'''
	Test Inquire HTTP GET
	'''
	def testInquire(self):

		print
		
		data = self.getJSON('/api/qmstatus/inquire/' + self.qmgr)

		self.assertFalse('mqweb' not in data, 'No mqweb data returned')

		if 'error' in data:
			self.assertFalse(True, 'Received a WebSphere MQ error:' + 
				str(data['error']['reason']['code']))

		self.assertFalse('status' not in data, 'No status data returned')

		self.assertTrue(self.checkIds(data['status']), 
			'There are unmapped Websphere MQ attributes')

	'''
	 Test Inquire with HTTP POST
	'''
	def testInquireJSON(self):

		print
		
		filter = { }

		data = self.appJSON('/api/qmstatus/inquire/' + self.qmgr, filter)
		self.assertFalse('mqweb' not in data, 'No mqweb data returned')

		if 'error' in data:
			self.assertFalse(True, 'Received a WebSphere MQ error:' 
				+ str(data['error']['reason']['code']))

		self.assertFalse('status' not in data, 'No information returned')
		
suite = unittest.TestLoader().loadTestsFromTestCase(TestQManagerStatusActions)
unittest.TextTestRunner(verbosity=2).run(suite)
