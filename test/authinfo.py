import unittest
from testbase import MQWebTest

'''
Test for AuthenticationInformationController
'''
class TestAuthInfoActions(MQWebTest):

	'''
	Test Inquire with HTTP GET
	'''
	def testInquire(self):

		print

		data = self.getJSON('/api/authinfo/inquire/' + self.qmgr)
		
		self.assertFalse('mqweb' not in data, 'No mqweb data returned')

		if 'error' in data:
			self.assertFalse(True, 'Received a WebSphere MQ error:' + 
				str(data['error']['reason']['code']))

		self.assertFalse('authinfos' not in data, 
			'No authentication information returned')
		self.assertFalse(len(data['authinfos']) == 0, 
			'Empty JSON array returned. ' +
			'Does MQWeb have permission to view this information?')

		self.assertTrue(self.checkIds(data['authinfos'][0]), 
			'There are unmapped Websphere MQ attributes')

	'''
	 Test Inquire with HTTP POST
	'''
	def testInquireJSON(self):

		print
		
		filter = { "AuthInfoName" : "*" }
		data = self.appJSON('/api/authinfo/inquire/' + self.qmgr, filter)
		self.assertFalse('mqweb' not in data, 'No mqweb data returned')

		if 'error' in data:
			self.assertFalse(True, 'Received a WebSphere MQ error:' 
				+ str(data['error']['reason']['code']))
        
		self.assertFalse('authinfos' not in data, 'No information returned')
		self.assertFalse(len(data['authinfos']) == 0, 
			'Empty JSON array returned. ' +
			'Does MQWeb have permission to view this information?')

suite = unittest.TestLoader().loadTestsFromTestCase(TestAuthInfoActions)
unittest.TextTestRunner(verbosity=2).run(suite)
