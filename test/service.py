import unittest
from testbase import MQWebTest

class TestServiceActions(MQWebTest):

	def testInquire(self):

		data = self.getJSON('/api/service/inquire/' + self.qmgr)
		
		self.assertFalse('mqweb' not in data, 'No mqweb data returned')

		if 'error' in data:
			self.assertFalse(True, 'Received a WebSphere MQ error:' + str(data['error']['reason']['code']))

		self.assertFalse('services' not in data, 'No services returned')

		self.assertTrue(self.checkIds(data['services'][0]), 'There are unmapped Websphere MQ attributes')

suite = unittest.TestLoader().loadTestsFromTestCase(TestServiceActions)
unittest.TextTestRunner(verbosity=2).run(suite)
