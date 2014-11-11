import unittest
from testbase import MQWebTest

'''
Test ServiceController
'''
class TestServiceActions(MQWebTest):

	'''
	Test Inquire HTTP GET
	'''
	def testInquire(self):

		data = self.getJSON('/api/service/inquire/' + self.qmgr)
		
		self.assertFalse('mqweb' not in data, 'No mqweb data returned')

		if 'error' in data:
			self.assertFalse(True, 'Received a WebSphere MQ error:' + 
				str(data['error']['reason']['code']))

		self.assertFalse('services' not in data, 'No information returned')
		self.assertFalse(len(data['services']) == 0, 
			'Empty JSON array returned. ' + 
			'Does MQWeb have permission to view this information?')

		self.assertTrue(self.checkIds(data['services'][0]), 
			'There are unmapped Websphere MQ attributes')

	'''
	 Test Inquire with HTTP POST
	'''
	def testInquireJSON(self):

		print
		
		filter = { "ServiceName" : "*" }
		data = self.appJSON('/api/service/inquire/' + self.qmgr, filter)
		self.assertFalse('mqweb' not in data, 'No mqweb data returned')

		if 'error' in data:
			self.assertFalse(True, 'Received a WebSphere MQ error:' 
				+ str(data['error']['reason']['code']))
        
		self.assertFalse('services' not in data, 'No information returned')
		self.assertFalse(len(data['services']) == 0, 
			'Empty JSON array returned. ' + 
			'Does MQWeb have permission to view this information?')

suite = unittest.TestLoader().loadTestsFromTestCase(TestServiceActions)
unittest.TextTestRunner(verbosity=2).run(suite)
