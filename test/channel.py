import unittest
from testbase import MQWebTest

'''
Test ChannelController
'''
class TestChannelActions(MQWebTest):

	'''
	Test Inquire with HTTP GET
	'''
	def testInquire(self):

		json = self.getJSON('/api/channel/inquire/' + self.qmgr)

	'''
	Test Inquire with HTTP POST
	'''
	def testInquireJSON(self):

		filter = { "ChannelName" : "*" }
		json = self.appJSON('/api/channel/inquire/' + self.qmgr, filter)

suite = unittest.TestLoader().loadTestsFromTestCase(TestChannelActions)
unittest.TextTestRunner(verbosity=2).run(suite)
