import unittest
from testbase import MQWebTest

'''
Test QueueManagerController
'''
class TestQueueManagerActions(MQWebTest):

	'''
	Test Inquire HTTP GET
	'''
	def testInquire(self):

		json = self.getJSON('/api/qmgr/inquire/' + self.qmgr)

	'''
	 Test Inquire with HTTP POST
	'''
	def testInquireJSON(self):
		
		filter = { }
		json = self.appJSON('/api/qmgr/inquire/' + self.qmgr, filter)

suite = unittest.TestLoader().loadTestsFromTestCase(TestQueueManagerActions)
unittest.TextTestRunner(verbosity=2).run(suite)
