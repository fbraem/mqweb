{% highlight python %}
'''
 This sample will clear a queue.
 MQWeb runs on localhost and is listening on port 8081.
'''
import json
import httplib
import socket
import argparse

parser = argparse.ArgumentParser(
	description='MQWeb - Python sample - Clear Queue',
	epilog="For more information: http://www.mqweb.org"
)
parser.add_argument('-m', '--queuemanager', help='Name of the queuemanager', required=True)
parser.add_argument('-q', '--queue', help='Name of the queue', required=True)
args = parser.parse_args()

url = '/api/queue/clear/' + args.queuemanager + '/' + args.queue

try:
	conn = httplib.HTTPConnection('localhost', 8081)
	conn.request('GET', url)
	res = conn.getresponse()
	result = json.loads(res.read())

	if 'error' in result:
		print('MQ Error: {0} - {1}'.format(str(result['error']['reason']['code']), str(result['error']['reason']['desc'])))
	else:
		print('Queue {0} cleared'.format(args.queue))

except httplib.HTTPException as e:
	print ('An HTTP error occurred while inquiring queues: {0} - {1}'.format(e.errno, e.strerror))
except socket.error as e:
	print e.strerror
	print 'Is the MQWeb daemon running?'
{% endhighlight %}
