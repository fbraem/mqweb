import unittest
from testbase import MQWebTest

'''
Test for AuthenticationInformationController
'''
class TestAuthInfoActions(MQWebTest):

	'''
	Test Inquire with HTTP GET
	'''
	def testInquire(self):

		json = self.getJSON('/api/authinfo/inquire/' + self.qmgr)

	'''
	Test Empty Result with HTTP GET
	'''
	def testEmptyInquire(self):
		json = self.getJSON('/api/authinfo/inquire/' + self.qmgr + '/ZZZ*', True)

	'''
	 Test Inquire with HTTP POST
	'''
	def testInquireJSON(self):

		print

		filter = { "AuthInfoName" : "*" }
		json = self.appJSON('/api/authinfo/inquire/' + self.qmgr, filter)

suite = unittest.TestLoader().loadTestsFromTestCase(TestAuthInfoActions)
unittest.TextTestRunner(verbosity=2).run(suite)
