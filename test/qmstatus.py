import unittest
from testbase import MQWebTest

'''
Test QueueManagerStatusController
'''
class TestQManagerStatusActions(MQWebTest):

	'''
	Test Inquire HTTP GET
	'''
	def testInquire(self):

		json = self.getJSON('/api/qmstatus/inquire/' + self.qmgr)

	'''
	 Test Inquire with HTTP POST
	'''
	def testInquireJSON(self):

		filter = { }
		json = self.appJSON('/api/qmstatus/inquire/' + self.qmgr, filter)
		
suite = unittest.TestLoader().loadTestsFromTestCase(TestQManagerStatusActions)
unittest.TextTestRunner(verbosity=2).run(suite)
