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

		json = self.getJSON('/api/service/inquire/' + self.qmgr)

	'''
	 Test Inquire with HTTP POST
	'''
	def testInquireJSON(self):

		filter = { "ServiceName" : "*" }
		json = self.appJSON('/api/service/inquire/' + self.qmgr, filter)

suite = unittest.TestLoader().loadTestsFromTestCase(TestServiceActions)
unittest.TextTestRunner(verbosity=2).run(suite)
