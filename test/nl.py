import unittest
from testbase import MQWebTest

'''
Test NamelistController
'''
class TestNamelistActions(MQWebTest):

	'''
	Test Inquire HTTP GET
	'''
	def testInquire(self):

		json = self.getJSON('/api/nl/inquire/' + self.qmgr)

	'''
	 Test Inquire with HTTP POST
	'''
	def testInquireJSON(self):

		filter = { "NamelistName" : "*" }
		json = self.appJSON('/api/nl/inquire/' + self.qmgr, filter)

suite = unittest.TestLoader().loadTestsFromTestCase(TestNamelistActions)
unittest.TextTestRunner(verbosity=2).run(suite)
