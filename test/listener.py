import unittest
from testbase import MQWebTest

'''
Test ListenerController
'''
class TestListenerActions(MQWebTest):

	'''
	Test Inquire HTTP GET
	'''
	def testInquire(self):

		print

		data = self.getJSON('/api/listener/inquire/' + self.qmgr)
		
		self.assertFalse('mqweb' not in data, 'No mqweb data returned')

		if 'error' in data:
			self.assertFalse(True, 'Received a WebSphere MQ error:' + 
				str(data['error']['reason']['code']))

		self.assertFalse('listeners' not in data, 'No listeners returned')

		self.assertFalse(len(data['listeners']) == 0, 
			'Empty JSON array returned. ' + 
			'Does MQWeb have permission to view this information?')

		self.assertTrue(self.checkIds(data['listeners'][0]), 
			'There are unmapped Websphere MQ attributes')

	'''
	 Test Inquire with HTTP POST
	'''
	def testInquireJSON(self):

		print
		
		filter = { "ListenerName" : "*" }

		data = self.appJSON('/api/listener/inquire/' + self.qmgr, filter)
		self.assertFalse('mqweb' not in data, 'No mqweb data returned')

		if 'error' in data:
			self.assertFalse(True, 'Received a WebSphere MQ error:' 
				+ str(data['error']['reason']['code']))
        
		self.assertFalse('listeners' not in data, 'No information returned')
		self.assertFalse(len(data['listeners']) == 0, 
			'Empty JSON array returned. ' +
			'Does MQWeb have permission to view this information?')

suite = unittest.TestLoader().loadTestsFromTestCase(TestListenerActions)
unittest.TextTestRunner(verbosity=2).run(suite)
