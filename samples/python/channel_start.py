'''
 Starts a channel on a queuemanager
 MQWeb runs on localhost and is listening on port 8081.
'''
import json
import httplib
import socket
import argparse

parser = argparse.ArgumentParser(
	description='MQWeb - Python sample - Start Channel',
	epilog="For more information: http://www.mqweb.org"
)
parser.add_argument('-m', '--queuemanager', help='Name of the queuemanager', required=True)
parser.add_argument('-c', '--channel', help='Name of the channel', required=True)
args = parser.parse_args()

url = "/api/channel/start/" + args.queuemanager + '/' + args.channel;

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
		print "Start command succesfully send."
		print "Note: this doesn't mean the channel is running! Use chstatus to get the status of the channel."

except httplib.HTTPException as e:
	print ('An HTTP error occurred while starting a channel: ' +
		e.errno + e.strerror
	)
except socket.error as e:
	print e.strerror
	print 'Is the MQWeb daemon running?'
