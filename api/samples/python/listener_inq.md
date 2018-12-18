{% highlight python %}
'''
 Inquire all SYSTEM listeners with transporttype TCP from a queuemanager.
 MQWeb runs on localhost and is listening on port 8081.
'''
import json
import httplib
import socket
import argparse

parser = argparse.ArgumentParser(
	description='MQWeb - Python sample - Inquire all SYSTEM lisener with transporttype TCP',
	epilog="For more information: http://www.mqweb.org"
)
parser.add_argument('-m', '--queuemanager', help='Name of the queuemanager', required=True)
args = parser.parse_args()

url = '/api/listener/inquire/' + args.queuemanager + '/SYSTEM*/TCP'

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
			print "No listeners found"
		else:
			for data in result['data']:
				print data['ListenerName']['value']
except httplib.HTTPException as e:
	print ('An HTTP error occurred while inquiring listeners: ' +
		e.errno + e.strerror
	)
except socket.error as e:
	print e.strerror
	print 'Is the MQWeb daemon running?'
{% endhighlight %}
