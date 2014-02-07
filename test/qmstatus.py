import unittest
import httplib
import json
from testbase import MQWebTest

class TestQueueManagerStatusActions(MQWebTest):

	def testInquire(self):
	
		try:
			conn = httplib.HTTPConnection(self.mqWebHost, self.mqWebPort)
			conn.request('GET', '/qmstatus/inquire/' + self.qmgr, "", MQWebTest.headers)
			res = conn.getresponse()
			data = json.loads(res.read())
		except:
			self.assertFalse(True, "Can't connect to MQWeb: " + self.mqWebHost + ":" + self.mqWebPort + ' (qmgr: ' + self.qmgr + ')')
		
		self.assertFalse('mqweb' not in data, 'No mqweb data returned')

		if 'error' in data:
			self.assertFalse(True, 'Received a WebSphere MQ error:' + str(data['error']['reason']['code']))

		self.assertFalse('status' not in data, 'No status data returned')

		self.assertTrue(self.checkIds(data['status']), 'There are unmapped Websphere MQ attributes')
		
suite = unittest.TestLoader().loadTestsFromTestCase(TestQueueManagerStatusActions)
unittest.TextTestRunner(verbosity=2).run(suite)
