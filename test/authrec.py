import unittest
from testbase import MQWebTest

'''
Test AuthorityRecordController
'''
class TestAuthorityRecordActions(MQWebTest):

	'''
	Test Inquire with HTTP GET
	'''
	def testInquire(self):

		json = self.getJSON('/api/authrec/inquire/' + 
			self.qmgr + 
			'?options=Name%20All%20Matching&options=Entity%20Explicit')

	'''
	 Test Inquire with HTTP POST
	'''
	def testInquireJSON(self):

		filter = { "Options" : ["Name All Matching", "Entity Explicit"] }
		json = self.appJSON('/api/authrec/inquire/' + self.qmgr, filter)

suite = unittest.TestLoader().loadTestsFromTestCase(TestAuthorityRecordActions)
unittest.TextTestRunner(verbosity=2).run(suite)
