'''
 This sample will show all SYSTEM queues from a queuemanager and
 prints the current queue depth if this property exists for the queue.
 MQWeb runs on localhost and is listening on port 8081.
'''
import json
import httplib
import socket
import argparse

parser = argparse.ArgumentParser(
	description='MQWeb - Python sample - Inquire System Queues',
	epilog="For more information: http://www.mqweb.org"
)
parser.add_argument('-m', '--queuemanager', help='Name of the queuemanager', required=True)
args = parser.parse_args()

url = "/api/queue/inquire/" + args.queuemanager + "/SYSTEM*"

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
		for data in result['data']:
			line = data['QName']['value']
			if 'CurrentQDepth' in data:
				line += ': ' + str(data['CurrentQDepth']['value'])
			print line

except httplib.HTTPException as e:
	print ('An HTTP error occurred while inquiring queues: ' +
		e.errno + e.strerror
	)
except socket.error as e:
	print e.strerror
	print 'Is the MQWeb daemon running?'
