'''
 This sample will show the description of a queuemanager.
 MQWeb runs on localhost and is listening on port 8081. 
'''

import sys
import json
import httplib
import socket

if len(sys.argv) < 2 :
	print 'Please pass me the name of a queuemanager as argument'
	sys.exit(1)

url = "/api/qmgr/inquire/" + sys.argv[1]

try:
	conn = httplib.HTTPConnection('localhost', 8081)
	conn.request('GET', url)
	res = conn.getresponse()
	result = json.loads(res.read())

	if 'error' in result:
		print ('Received a WebSphere MQ error: ' +	
			str(result['error']['reason']['code'])
		)
	else:
		print (result['data'][0]['QMgrName']['value'] 
			+ ' : ' + result['data'][0]['QMgrDesc']['value']
		)
except httplib.HTTPException as e:
	print ('An HTTP error occurred while inquiring queuemanager: ' +
		e.errno + e.strerror
	)
except socket.error as e:
	print e.strerror
	print 'Is the MQWeb daemon running?'
