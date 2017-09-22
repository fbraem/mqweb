'''
 This sample will read all event messages from queue SYSTEM.ADMIN.CONFIG.EVENT
 and show all events that are related to queues.
 MQWeb runs on localhost and is listening on port 8081. 
'''

import sys
import json
import httplib
import socket

if len(sys.argv) < 2 :
	print 'Please pass me the name of a queuemanager as argument'
	sys.exit(1)

url = "/api/message/browse/" + sys.argv[1] + '/SYSTEM.ADMIN.CONFIG.EVENT'

try:
	conn = httplib.HTTPConnection('localhost', 8081)
	conn.request('GET', url)
	res = conn.getresponse()
	result = json.loads(res.read())

	if 'error' in result:
		print('Received a WebSphere MQ error: ' +	
			str(result['error']['reason']['code'])
		)
	else:
		for message in result['data']:
			if 'event' in message :
					#print(json.dumps(message, indent=2))
					if message['event']['ObjectType']['text'] == 'Queue' :
							print("MQ Command {0} generated a config event for {1}".format(
									message['event']['EventOrigin']['text'], 
									message['event']['QName']['value'])
							)
except httplib.HTTPException as e:
	print ('An HTTP error occurred while inquiring queuemanager: ' +
		e.errno + e.strerror
	)
except socket.error as e:
	print e.strerror
	print 'Is the MQWeb daemon running?'
