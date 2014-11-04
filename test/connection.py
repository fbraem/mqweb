import unittest
from testbase import MQWebTest

class TestConnectionActions(MQWebTest):

	def testInquire(self):

		data = self.getJSON('/api/conn/inquire/' + self.qmgr)
		
		self.assertFalse('mqweb' not in data, 'No mqweb data returned')

		if 'error' in data:
			self.assertFalse(True, 'Received a WebSphere MQ error:' + str(data['error']['reason']['code']))

		self.assertFalse('connections' not in data, 'No connections returned')

		self.assertTrue(self.checkIds(data['connections'][0]), 'There are unmapped Websphere MQ attributes')

suite = unittest.TestLoader().loadTestsFromTestCase(TestConnectionActions)
unittest.TextTestRunner(verbosity=2).run(suite)
