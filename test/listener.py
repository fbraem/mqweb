import unittest
from testbase import MQWebTest

'''
Test ListenerController
'''
class TestListenerActions(MQWebTest):

	'''
	Test Inquire HTTP GET
	'''
	def testInquire(self):

		json = self.getJSON('/api/listener/inquire/' + self.qmgr)

	'''
	 Test Inquire with HTTP POST
	'''
	def testInquireJSON(self):

		filter = { "ListenerName" : "*" }
		json = self.appJSON('/api/listener/inquire/' + self.qmgr, filter)

suite = unittest.TestLoader().loadTestsFromTestCase(TestListenerActions)
unittest.TextTestRunner(verbosity=2).run(suite)
