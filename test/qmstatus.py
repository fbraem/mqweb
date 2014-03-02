import unittest
from testbase import MQWebTest

class TestQueueManagerStatusActions(MQWebTest):

	def testInquire(self):

		data = self.getJSON('/api/qmstatus/inquire/' + self.qmgr)

		self.assertFalse('mqweb' not in data, 'No mqweb data returned')

		if 'error' in data:
			self.assertFalse(True, 'Received a WebSphere MQ error:' + str(data['error']['reason']['code']))

		self.assertFalse('status' not in data, 'No status data returned')

		self.assertTrue(self.checkIds(data['status']), 'There are unmapped Websphere MQ attributes')
		
suite = unittest.TestLoader().loadTestsFromTestCase(TestQueueManagerStatusActions)
unittest.TextTestRunner(verbosity=2).run(suite)
