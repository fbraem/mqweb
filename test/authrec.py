import unittest
from testbase import MQWebTest

class TestAuthorityRecordActions(MQWebTest):

	def testInquire(self):

		data = self.getJSON('/api/authrec/inquire/' + self.qmgr + '?options=Name%20All%20Matching&options=Entity%20Explicit')
		
		self.assertFalse('mqweb' not in data, 'No mqweb data returned')

		if 'error' in data:
			self.assertFalse(True, 'Received a WebSphere MQ error:' + str(data['error']['reason']['code']))

		self.assertFalse('authrecs' not in data, 'No authority records returned')
		self.assertFalse(len(data['authrecs']) == 0, 'Empty JSON array returned. Does MQWeb have permission to view this information?')
		self.assertTrue(self.checkIds(data['authrecs'][0]), 'There are unmapped Websphere MQ attributes')

suite = unittest.TestLoader().loadTestsFromTestCase(TestAuthorityRecordActions)
unittest.TextTestRunner(verbosity=2).run(suite)
