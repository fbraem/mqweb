import unittest
from testbase import MQWebTest

class TestListenerStatusActions(MQWebTest):

	def testInquire(self):

		data = self.getJSON('/lsstatus/inquire/' + self.qmgr)

		self.assertFalse('mqweb' not in data, 'No mqweb data returned')

		if 'error' in data:
			self.assertFalse(True, 'Received a WebSphere MQ error:' + str(data['error']['reason']['code']))

		self.assertFalse('statuses' not in data, 'No status data returned')

		self.assertTrue(self.checkIds(data['statuses'][0]), 'There are unmapped Websphere MQ attributes')
		
suite = unittest.TestLoader().loadTestsFromTestCase(TestListenerStatusActions)
unittest.TextTestRunner(verbosity=2).run(suite)
