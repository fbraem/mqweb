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
	description='MQWeb - Python sample - Create local queue',
	epilog="For more information: http://www.mqweb.org"
)
parser.add_argument('-m', '--queuemanager', help='Name of the queuemanager', required=True)
parser.add_argument('-q', '--queue', help='Name of the queue', required=True)
args = parser.parse_args()

url = "/api/queue/create/" + args.queuemanager + "/" + args.queue + '?QType=Local'

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
		print(args.queue + ' created on ' + args.queuemanager)

except httplib.HTTPException as e:
	print ('An HTTP error occurred while creating queue: ' +
		e.errno + e.strerror
	)
except socket.error as e:
	print e.strerror
	print 'Is the MQWeb daemon running?'
{% endhighlight %}
