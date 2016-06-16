import unittest
from testbase import MQWebTest

'''
Test ClusterQmgrController
'''
class TestClusQmgrActions(MQWebTest):

	def testInquire(self):

		json = self.getJSON('/api/clusqmgr/inquire/' + self.qmgr)

	'''
	Test Inquire with HTTP POST
	'''
	def testInquireJSON(self):

		filter = { "ClusterName" : "*", "ClusterQMgrName" : "*" }
		json = self.appJSON('/api/clusqmgr/inquire/' + self.qmgr, filter)

suite = unittest.TestLoader().loadTestsFromTestCase(TestClusQmgrActions)
unittest.TextTestRunner(verbosity=2).run(suite)
