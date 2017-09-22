import unittest
from testbase import MQWebTest

'''
Test ListenerStatusController
'''
class TestListenerStatusActions(MQWebTest):

	'''
	Test Inquire HTTP GET
	'''
	def testInquire(self):

		json = self.getJSON('/api/lsstatus/inquire/' + self.qmgr)

	'''
	 Test Inquire with HTTP POST
	'''
	def testInquireJSON(self):

		filter = { "ListenerName" : "*" }
		json = self.appJSON('/api/lsstatus/inquire/' + self.qmgr, filter)
		
suite = unittest.TestLoader().loadTestsFromTestCase(TestListenerStatusActions)
unittest.TextTestRunner(verbosity=2).run(suite)
