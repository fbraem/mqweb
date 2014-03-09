import unittest
from testbase import MQWebTest

class TestQueueActions(MQWebTest):

	def testInquire(self):

		data = self.getJSON('/api/clusqmgr/inquire/' + self.qmgr)
		
		self.assertFalse('mqweb' not in data, 'No mqweb data returned')

		if 'error' in data:
			self.assertFalse(True, 'Received a WebSphere MQ error:' + str(data['error']['reason']['code']))

		self.assertFalse('clusqmgrs' not in data, 'No clusqmgrs array returned')
		self.assertFalse(len(data['clusqmgrs']) == 0, 'No cluster information found')
		
		self.assertTrue(self.checkIds(data['clusqmgrs'][0]), 'There are unmapped Websphere MQ attributes')

suite = unittest.TestLoader().loadTestsFromTestCase(TestQueueActions)
unittest.TextTestRunner(verbosity=2).run(suite)
