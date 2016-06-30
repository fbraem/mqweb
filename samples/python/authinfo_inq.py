'''
 Inquire all SYSTEM authentication information objects from a queuemanager 
 MQWeb runs on localhost and is listening on port 8081. 
'''
import sys
import json
import httplib
import socket

if len(sys.argv) < 2 :
	print 'Please pass me the name of a queuemanager as argument'
	sys.exit(1)

url = "/api/authinfo/inquire/" + sys.argv[1] + '/SYSTEM*';

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
		if len(result['data']) == 0 :
			print "No authentication information found"
		else:
			for data in result['data']:
				print data['AuthInfoName']['value']

except httplib.HTTPException as e:
	print ('An HTTP error occurred while inquiring authentication information: ' +
		e.errno + e.strerror
	)
except socket.error as e:
	print e.strerror
	print 'Is the MQWeb daemon running?'
