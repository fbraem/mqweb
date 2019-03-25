'''
 This sample resets the statistics for a queuemanager.
 MQWeb runs on localhost and is listening on port 8081.
'''
import json
import httplib
import socket
import argparse

parser = argparse.ArgumentParser(
	description='MQWeb - Python sample - Reset Queuemanager',
	epilog='For more information: http://www.mqweb.org'
)
parser.add_argument('-m', '--queuemanager', help='Name of the queuemanager', required=True)
parser.add_argument('-s', '--server', help='MQWeb host', default='localhost')
parser.add_argument('-p', '--port', help='MQWeb port', default=8081)
args = parser.parse_args()

url = '/api/qmgr/reset/' + args.queuemanager + '/Statistics'

try:
	conn = httplib.HTTPConnection(args.server, args.port)
	conn.request('GET', url)
	res = conn.getresponse()
	result = json.loads(res.read())

	if 'error' in result:
		print('MQ Error: {0} - {1}'.format(str(result['error']['reason']['code']), str(result['error']['reason']['desc'])))
	else:
		print('Reset successful')
except httplib.HTTPException as e:
	print('An HTTP error occurred while pinging queuemanager: {0} - {1}'.format(e.errno, e.strerror))
except socket.error as e:
	print(e.strerror)
 	print('Is the MQWeb daemon running?')
