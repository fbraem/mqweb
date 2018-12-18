{% highlight python %}
'''
 This sample will create a new local queue.
 MQWeb runs on localhost and is listening on port 8081.
'''
import sys
import json
import httplib
import socket
import argparse

parser = argparse.ArgumentParser(
	description='MQWeb - Python sample - Set Channel Authentication Record',
	epilog="For more information: http://www.mqweb.org"
)
parser.add_argument('-m', '--queuemanager', help='Name of the queuemanager', required=True)
parser.add_argument('-p', '--profile', help='Name of the channel', required=True)
parser.add_argument('-a', '--address', help='The address of the client', required=True)
parser.add_argument('-mca', '--mcauser', help='The MCAUser id', required=True)
parser.add_argument('-c', '--clientuser', help='The client user id', required=True)
args = parser.parse_args()

url = "/api/chlauth/set/" + args.queuemanager

input = {
	'ChannelName' : args.profile,
	'Type' : 'UserMap',
	'ConnectionName' : args.address,
	'MCAUserIdentifier' : args.mcauser,
	'ClientUserIdentifier' : args.clientuser,
	'Action' : 'Replace',
	'UserSrc' : 'Map'
}

try:
	headers = {
		'Content-Type': 'application/json'
	}
	conn = httplib.HTTPConnection('localhost', 8081)
	conn.request('POST', url, json.dumps(input), headers)
	res = conn.getresponse()
	result = json.loads(res.read())
	print(result)
	if 'error' in result:
		print ('Received a WebSphere MQ error: ' +
			str(result['error']['reason']['code']) + ' - '  +
			result['error']['reason']['desc']
		)
	else:
		print('Channel authentication records added for ' + args.profile + ' on ' + args.queuemanager)

except httplib.HTTPException as e:
	print ('An HTTP error occurred while setting channel authentication records: ' +
		e.errno + e.strerror
	)
except socket.error as e:
	print e.strerror
	print 'Is the MQWeb daemon running?'
{% endhighlight %}
