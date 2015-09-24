import unittest
from testbase import MQWebTest

'''
Test NamelistController
'''
class TestNamelistActions(MQWebTest):

	'''
	Test Inquire HTTP GET
	'''
	def testInquire(self):

		print

		data = self.getJSON('/api/nl/inquire/' + self.qmgr)
		
		self.assertFalse('mqweb' not in data, 'No mqweb data returned')

		if 'error' in data:
			self.assertFalse(True, 'Received a WebSphere MQ error:' + 
				str(data['error']['reason']['code']))

		self.assertFalse('namelists' not in data, 'No information returned')
		self.assertFalse(len(data['namelists']) == 0, 
			'Empty JSON array returned. ' +
			'Does MQWeb have permission to view this information?')

		self.assertTrue(self.checkIds(data['namelists'][0]), 
			'There are unmapped Websphere MQ attributes')

	'''
	 Test Inquire with HTTP POST
	'''
	def testInquireJSON(self):

		print
		
		filter = { "NamelistName" : "*" }

		data = self.appJSON('/api/nl/inquire/' + self.qmgr, filter)
		self.assertFalse('mqweb' not in data, 'No mqweb data returned')

		if 'error' in data:
			self.assertFalse(True, 'Received a WebSphere MQ error:' 
				+ str(data['error']['reason']['code']))
        
		self.assertFalse('namelists' not in data, 'No information returned')
		self.assertFalse(len(data['namelists']) == 0, 
			'Empty JSON array returned. ' +
			'Does MQWeb have permission to view this information?')

suite = unittest.TestLoader().loadTestsFromTestCase(TestNamelistActions)
unittest.TextTestRunner(verbosity=2).run(suite)
