import unittest
from testbase import MQWebTest

'''
Test ClusterQmgrController
'''
class TestClusQmgrActions(MQWebTest):

	def testInquire(self):

		print
		
		data = self.getJSON('/api/clusqmgr/inquire/' + self.qmgr)
		
		self.assertFalse('mqweb' not in data, 'No mqweb data returned')

		if 'error' in data:
			self.assertFalse(True, 'Received a WebSphere MQ error:' + 
				str(data['error']['reason']['code']))

		self.assertFalse('clusqmgrs' not in data, 'No clusqmgrs array returned')
			
		self.assertFalse(len(data['clusqmgrs']) == 0, 
			'Empty JSON array returned. ' + 
			'Does MQWeb have permission to view this information?')

		self.assertTrue(self.checkIds(data['clusqmgrs'][0]), 
			'There are unmapped Websphere MQ attributes')

	'''
	Test Inquire with HTTP POST
	'''
	def testInquireJSON(self):

		print
		
		filter = { "ClusterName" : "*", "ClusterQMgrName" : "*" }
		data = self.appJSON('/api/clusqmgr/inquire/' + self.qmgr, filter)
		self.assertFalse('mqweb' not in data, 'No mqweb data returned')

		if 'error' in data:
			self.assertFalse(True, 'Received a WebSphere MQ error:' 
				+ str(data['error']['reason']['code']))
        
		self.assertFalse('clusqmgrs' not in data, 'No clusqmgrs returned')
		self.assertFalse(len(data['clusqmgrs']) == 0, 
			'Empty JSON array returned. ' + 
			'Does MQWeb have permission to view this information?')

suite = unittest.TestLoader().loadTestsFromTestCase(TestClusQmgrActions)
unittest.TextTestRunner(verbosity=2).run(suite)
