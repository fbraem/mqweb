'''
 This sample will read all event messages from a queue.
 MQWeb runs on localhost and is listening on port 8081.
'''

import json
import httplib
import socket
import argparse

parser = argparse.ArgumentParser(
	description='MQWeb - Python sample - Browse Event Queue',
	epilog="For more information: http://www.mqweb.org"
)
parser.add_argument('-m', '--queuemanager', help='Name of the queuemanager', required=True)
parser.add_argument('-q', '--queue', help='Name of the event queue', required=True)
args = parser.parse_args()

url = "/api/message/browse/" + args.queuemanager + '/' + args.queue

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
			#print(json.dumps(message, indent=2))
			if 'event' in message :
				if 'ObjectType' in message['event'] :
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
