import unittest
from testbase import MQWebTest

class TestListenerActions(MQWebTest):

	def testInquire(self):

		data = self.getJSON('/listener/inquire/' + self.qmgr)
		
		self.assertFalse('mqweb' not in data, 'No mqweb data returned')

		if 'error' in data:
			self.assertFalse(True, 'Received a WebSphere MQ error:' + str(data['error']['reason']['code']))

		self.assertFalse('listeners' not in data, 'No listeners returned')

		self.assertTrue(self.checkIds(data['listeners'][0]), 'There are unmapped Websphere MQ attributes')

suite = unittest.TestLoader().loadTestsFromTestCase(TestListenerActions)
unittest.TextTestRunner(verbosity=2).run(suite)
