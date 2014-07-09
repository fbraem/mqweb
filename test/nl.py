import unittest
from testbase import MQWebTest

class TestNamelistActions(MQWebTest):

	def testInquire(self):

		data = self.getJSON('/api/nl/inquire/' + self.qmgr)
		
		self.assertFalse('mqweb' not in data, 'No mqweb data returned')

		if 'error' in data:
			self.assertFalse(True, 'Received a WebSphere MQ error:' + str(data['error']['reason']['code']))

		self.assertFalse('namelists' not in data, 'No namelists returned')

		self.assertTrue(self.checkIds(data['namelists'][0]), 'There are unmapped Websphere MQ attributes')

suite = unittest.TestLoader().loadTestsFromTestCase(TestNamelistActions)
unittest.TextTestRunner(verbosity=2).run(suite)
