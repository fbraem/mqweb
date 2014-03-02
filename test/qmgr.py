import unittest
from testbase import MQWebTest

class TestQueueManagerActions(MQWebTest):

	def testInquire(self):

		data = self.getJSON('/api/qmgr/inquire/' + self.qmgr)
		
		self.assertFalse('mqweb' not in data, 'No mqweb data returned')

		if 'error' in data:
			self.assertFalse(True, 'Received a WebSphere MQ error:' + str(data['error']['reason']['code']))

		self.assertFalse('qmgr' not in data, 'No queuemanager data returned')

		self.assertTrue(self.checkIds(data['qmgr']), 'There are unmapped Websphere MQ attributes')

suite = unittest.TestLoader().loadTestsFromTestCase(TestQueueManagerActions)
unittest.TextTestRunner(verbosity=2).run(suite)
