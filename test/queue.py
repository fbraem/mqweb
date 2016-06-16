import unittest
from testbase import MQWebTest

'''
Test for QueueController
'''
class TestQueueActions(MQWebTest):

	'''
	 Test Inquire with HTTP GET
	'''
	def testInquire(self):

		json = self.getJSON('/api/queue/inquire/' + self.qmgr)

	'''
	 Test Inquire with HTTP POST
	'''
	def testInquireJSON(self):

		filter = { "QName" : "*" }
		json = self.appJSON('/api/queue/inquire/' + self.qmgr, filter)

suite = unittest.TestLoader().loadTestsFromTestCase(TestQueueActions)
unittest.TextTestRunner(verbosity=2).run(suite)
