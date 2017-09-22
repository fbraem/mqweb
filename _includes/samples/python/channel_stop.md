{% highlight python %}
'''
 Stops a channel on a queuemanager 
 MQWeb runs on localhost and is listening on port 8081. 
'''
import sys
import json
import httplib
import socket

if len(sys.argv) < 3 :
	print 'Please pass me the name of a queuemanager and a channelname as argument'
	sys.exit(1)

url = "/api/channel/stop/" + sys.argv[1] + '/' + sys.argv[2];

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
		print "Stop command succesfully send."

except httplib.HTTPException as e:
	print ('An HTTP error occurred while stopping the channel: ' +
		e.errno + e.strerror
	)
except socket.error as e:
	print e.strerror
	print 'Is the MQWeb daemon running?'
{% endhighlight %}
