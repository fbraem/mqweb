'''
 Inquire all channel authentication records.
 MQWeb runs on localhost and is listening on port 8081. 
'''
import sys
import json
import httplib
import socket
import argparse

parser = argparse.ArgumentParser(
	description='MQWeb - Python sample - Inquire Channel Authentication Record',
	epilog="For more information: http://www.mqweb.org"
)
parser.add_argument('-m', '--queuemanager', help='Name of the queuemanager', required=True)
parser.add_argument('-p', '--profile', help='Name of the channel', required=False)
args = parser.parse_args()

url = '/api/chlauth/inquire/' + args.queuemanager

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
		if len(result['data']) == 0:
			print "No channels found"
		else:
			for data in result['data']:
				print data
except httplib.HTTPException as e:
	print ('An HTTP error occurred while inquiring channels: ' +
		e.errno + e.strerror
	)
except socket.error as e:
	print e.strerror
	print 'Is the MQWeb daemon running?'
