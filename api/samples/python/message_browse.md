{% highlight python %}
'''
 This sample will browse messages from a queue.
 MQWeb runs on localhost and is listening on port 8081.
'''

import json
import httplib
import socket
import argparse

parser = argparse.ArgumentParser(
	description='MQWeb - Python sample - Browse messages from a queue',
	epilog="For more information: http://www.mqweb.org"
)
parser.add_argument('-m', '--queuemanager', help='Name of the queuemanager', required=True)
parser.add_argument('-q', '--queue', help='Name of the queue', required=True)
parser.add_argument('-i', '--id', help='Message ID', required=False)
args = parser.parse_args()

url = "/api/message/browse/" + args.queuemanager + '/' + args.queue
if (args.id is not None):
    url += '/' + args.id
size = 1024 * 32
url += '?size=' + str(size)

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
		count = 0
		for message in result['data']:
			count += 1
			print(json.dumps(message))
		print('Number of messages: ' + str(count))
except httplib.HTTPException as e:
	print ('An HTTP error occurred while inquiring queuemanager: ' +
		e.errno + e.strerror
	)
except socket.error as e:
	print e.strerror
	print 'Is the MQWeb daemon running?'
{% endhighlight %}
