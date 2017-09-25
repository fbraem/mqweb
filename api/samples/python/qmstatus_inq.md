{% highlight python %}
'''
 This sample will show the startdate/time of a queuemanager.
 MQWeb runs on localhost and is listening on port 8081. 
'''

import sys
import json
import httplib
import socket

if len(sys.argv) < 2 :
	print 'Please pass me the name of a queuemanager as argument'
	sys.exit(1)

url = "/api/qmstatus/inquire/" + sys.argv[1]
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
{% endhighlight %}
