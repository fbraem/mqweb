import unittest
import json
from testbase import MQWebTest

class TestQueueActions(MQWebTest):

	"""
	def testInquire(self):

		data = self.getJSON('/api/queue/inquire/' + self.qmgr)
		
		self.assertFalse('mqweb' not in data, 'No mqweb data returned')

		if 'error' in data:
			self.assertFalse(True, 'Received a WebSphere MQ error:' + str(data['error']['reason']['code']))

		self.assertFalse('queues' not in data, 'No queues returned')

		self.assertTrue(self.checkIds(data['queues'][0]), 'There are unmapped Websphere MQ attributes')
		print json.dumps(data, indent=4)
	"""
	def testInquireJSON(self):
		filter = json.loads('{ "QName" : "*" }')
		data = self.appJSON('/api/queue/inquire/' + self.qmgr, filter)
                self.assertFalse('mqweb' not in data, 'No mqweb data returned')

                if 'error' in data:
                        self.assertFalse(True, 'Received a WebSphere MQ error:' + str(data['error']['reason']['code']))

                self.assertFalse('queues' not in data, 'No queues returned')

                self.assertTrue(self.checkIds(data['queues'][0]), 'There are unmapped Websphere MQ attributes')
                print json.dumps(data, indent=4)


suite = unittest.TestLoader().loadTestsFromTestCase(TestQueueActions)
unittest.TextTestRunner(verbosity=2).run(suite)
