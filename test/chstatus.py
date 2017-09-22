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
	
		json = self.getJSON('/api/chstatus/inquire/' + self.qmgr)

	'''
	Test Inquire with HTTP POST
	'''
	def testInquireJSON(self):

		filter = { "ChannelName" : "*" }
		json = self.appJSON('/api/chstatus/inquire/' + self.qmgr, filter)

suite = unittest.TestLoader().loadTestsFromTestCase(TestChannelStatusActions)
unittest.TextTestRunner(verbosity=2).run(suite)
