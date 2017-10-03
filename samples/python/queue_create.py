'''
 This sample will create a new local queue.
 MQWeb runs on localhost and is listening on port 8081.
'''
import sys
import json
import httplib
import socket

if len(sys.argv) < 2 :
	print 'Please pass me the name of a queuemanager and a queue as argument'
	sys.exit(1)
if len(sys.argv) < 3 :
	print 'Please pass me the name of a queue as argument'
	sys.exit(1)

url = "/api/queue/create/" + sys.argv[1] + "/" + sys.argv[2] + '?QType=Local'

try:
	conn = httplib.HTTPConnection('localhost', 8081)
	conn.request('GET', url)
	res = conn.getresponse()
	result = json.loads(res.read())

	if 'error' in result:
		print ('Received a WebSphere MQ error: ' +
			str(result['error']['reason']['code']) + ' - '  +
			result['error']['reason']['desc']
		)
	else:
		print(sys.argv[2] + ' created on ' + sys.argv[1])

except httplib.HTTPException as e:
	print ('An HTTP error occurred while inquiring queues: ' +
		e.errno + e.strerror
	)
except socket.error as e:
	print e.strerror
	print 'Is the MQWeb daemon running?'
