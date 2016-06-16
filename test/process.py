import unittest
from testbase import MQWebTest

'''
Test ProcessController
'''
class TestProcessActions(MQWebTest):

	'''
	Test Inquire HTTP GET
	'''
	def testInquire(self):

		json = self.getJSON('/api/process/inquire/' + self.qmgr)

	'''
	 Test Inquire with HTTP POST
	'''
	def testInquireJSON(self):

		filter = { "ProcessName" : "*" }
		json = self.appJSON('/api/process/inquire/' + self.qmgr, filter)

suite = unittest.TestLoader().loadTestsFromTestCase(TestProcessActions)
unittest.TextTestRunner(verbosity=2).run(suite)
