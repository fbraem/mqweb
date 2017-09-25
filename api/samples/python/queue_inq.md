{% highlight python %}
'''
 This sample will show all SYSTEM queues from a queuemanager and
 prints the current queue depth if this property exists for the queue.
 MQWeb runs on localhost and is listening on port 8081. 
'''
import sys
import json
import httplib
import socket

if len(sys.argv) < 2 :
	print 'Please pass me the name of a queuemanager as argument'
	sys.exit(1)

url = "/api/queue/inquire/" + sys.argv[1] + "/SYSTEM*";

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
		for data in result['data']:
			line = data['QName']['value']
			if 'CurrentQDepth' in data:
				line += ': ' + str(data['CurrentQDepth']['value'])
			print line

except httplib.HTTPException as e:
	print ('An HTTP error occurred while inquiring queues: ' +
		e.errno + e.strerror
	)
except socket.error as e:
	print e.strerror
	print 'Is the MQWeb daemon running?'
{% endhighlight %}
