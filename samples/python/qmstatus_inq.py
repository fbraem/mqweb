'''
 This sample will show the startdate/time of a queuemanager.
 MQWeb runs on localhost and is listening on port 8081. 
'''

import sys
import json
import httplib
import socket
import argparse

parser = argparse.ArgumentParser(
	description='MQWeb - Python sample - Inquire startdate/time of a queuemanager',
	epilog="For more information: http://www.mqweb.org"
)
parser.add_argument('-m', '--queuemanager', help='Name of the queuemanager', required=True)
args = parser.parse_args()

url = "/api/qmstatus/inquire/" + args.queuemanager
input = {
	'QMStatusAttrs' : [
		'StartDate',
		'StartTime'
	]
}

try:
	headers = {
		'Content-Type': 'application/json'
	}
	conn = httplib.HTTPConnection('localhost', 8081)
	conn.request('POST', url, json.dumps(input), headers)
	res = conn.getresponse()
	result = json.loads(res.read())

	if 'error' in result:
		print ('Received a WebSphere MQ error: ' +	
			str(result['error']['reason']['code'])
		)
	else:
		print (result['data'][0]['QMgrName']['value'] 
			+ ' started on ' + result['data'][0]['StartDate']['value']
			+ ' ' + result['data'][0]['StartTime']['value']
		)
except httplib.HTTPException as e:
	print ('An HTTP error occurred while inquiring queuemanager status: ' +
		e.errno + e.strerror
	)
except socket.error as e:
	print e.strerror
	print 'Is the MQWeb daemon running?'
