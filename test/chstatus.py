import unittest
from testbase import MQWebTest

'''
Test ChannelStatusController
'''
class TestChannelStatusActions(MQWebTest):

	'''
	Test Inquire with HTTP GET
	'''
	def testInquire(self):
	
		print

		data = self.getJSON('/api/chstatus/inquire/' + self.qmgr)

		self.assertFalse('mqweb' not in data, 'No mqweb data returned')

		if 'error' in data:
			self.assertFalse(True, 'Received a WebSphere MQ error:' 
				+ str(data['error']['reason']['code']))

		self.assertFalse('statuses' not in data, 'No status data returned')
		self.assertFalse(len(data['statuses']) == 0, 
			'Empty JSON array returned. ' + 
			'Does MQWeb have permission to view this information?')

		self.assertTrue(self.checkIds(data['statuses'][0]), 
			'There are unmapped Websphere MQ attributes')

	'''
	Test Inquire with HTTP POST
	'''
	def testInquireJSON(self):

		print
		
		filter = { "ChannelName" : "*" }
		data = self.appJSON('/api/chstatus/inquire/' + self.qmgr, filter)
		self.assertFalse('mqweb' not in data, 'No mqweb data returned')

		if 'error' in data:
			self.assertFalse(True, 'Received a WebSphere MQ error:' 
				+ str(data['error']['reason']['code']))
        
		self.assertFalse('statuses' not in data, 'No statuses returned')
		self.assertFalse(len(data['statuses']) == 0, 
			'Empty JSON array returned. ' + 
			'Does MQWeb have permission to view this information?')

suite = unittest.TestLoader().loadTestsFromTestCase(TestChannelStatusActions)
unittest.TextTestRunner(verbosity=2).run(suite)
