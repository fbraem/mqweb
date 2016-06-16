import unittest
from testbase import MQWebTest

'''
Test ConnectionController
'''
class TestConnectionActions(MQWebTest):

	'''
	Test Inquire with HTTP GET
	'''
	def testInquire(self):

		json = self.getJSON('/api/conn/inquire/' + self.qmgr)

	'''
	 Test Inquire with HTTP POST
	'''
	def testInquireJSON(self):

		filter = { }
		json = self.appJSON('/api/conn/inquire/' + self.qmgr, filter)

suite = unittest.TestLoader().loadTestsFromTestCase(TestConnectionActions)
unittest.TextTestRunner(verbosity=2).run(suite)
