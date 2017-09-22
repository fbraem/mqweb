{% highlight python %}
'''
 Inquire all authority service information for a queuemanager.
 MQWeb runs on localhost and is listening on port 8081. 
'''
import sys
import json
import httplib
import socket

if len(sys.argv) < 2 :
	print 'Please pass me the name of a queuemanager as argument'
	sys.exit(1)

url = '/api/authservice/inquire/' + sys.argv[1]

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
			print "No authority services found"
		else:
			for data in result['data']:
				for service in data['ServiceComponent']['value']:
					print service
except httplib.HTTPException as e:
	print ('An HTTP error occurred while inquiring authority services: ' +
		e.errno + e.strerror
	)
except socket.error as e:
	print e.strerror
	print 'Is the MQWeb daemon running?'
{% endhighlight %}
