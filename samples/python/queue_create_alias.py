'''
 This sample will create a new alias queue.
 MQWeb runs on localhost and is listening on port 8081.
'''
import sys
import json
import httplib
import socket
import argparse

parser = argparse.ArgumentParser(
	description='MQWeb - Python sample - Create alias queue',
	epilog="For more information: http://www.mqweb.org"
)
parser.add_argument('-m', '--queuemanager', help='Name of the queuemanager', required=True)
parser.add_argument('-a', '--alias', help='Name of the alias queue', required=True)
parser.add_argument('-t', '--target', help='Name of the target queue', required=True)
args = parser.parse_args()

url = "/api/queue/create/" + args.queuemanager

input = {
	'QName' : args.alias,
	'QType' : 'Alias',
	'BaseObjectName' : args.target,
	'BaseType' : 'Queue'
}

try:
	conn = httplib.HTTPConnection('localhost', 8081)
	headers = {
		'Content-Type': 'application/json'
	}
	conn.request('POST', url, json.dumps(input), headers)
	res = conn.getresponse()
	result = json.loads(res.read())

	if 'error' in result:
		print ('Received a WebSphere MQ error: ' +
			str(result['error']['reason']['code']) + ' - '  +
			result['error']['reason']['desc']
		)
	else:
		print('Alias ' + args.alias + ' for ' + args.target + ' created on ' + args.queuemanager)

except httplib.HTTPException as e:
	print ('An HTTP error occurred while inquiring queues: ' +
		e.errno + e.strerror
	)
except socket.error as e:
	print e.strerror
	print 'Is the MQWeb daemon running?'
