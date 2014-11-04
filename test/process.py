import unittest
from testbase import MQWebTest

class TestProcessActions(MQWebTest):

	def testInquire(self):

		data = self.getJSON('/api/process/inquire/' + self.qmgr)
		
		self.assertFalse('mqweb' not in data, 'No mqweb data returned')

		if 'error' in data:
			self.assertFalse(True, 'Received a WebSphere MQ error:' + str(data['error']['reason']['code']))

		self.assertFalse('processes' not in data, 'No processes returned')

		self.assertTrue(self.checkIds(data['processes'][0]), 'There are unmapped Websphere MQ attributes')

suite = unittest.TestLoader().loadTestsFromTestCase(TestProcessActions)
unittest.TextTestRunner(verbosity=2).run(suite)
