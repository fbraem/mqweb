{% highlight python %}
'''
 Supends a queuemanager in a cluster
 MQWeb runs on localhost and is listening on port 8081.
'''
import json
import httplib
import socket
import argparse

parser = argparse.ArgumentParser(
	description='MQWeb - Python sample - Suspend Cluster Queuemanager',
	epilog="For more information: http://www.mqweb.org"
)
parser.add_argument('-m', '--queuemanager', help='Name of the queuemanager', required=True)
parser.add_argument('-c', '--cluster', help='Name of the cluster', required=True)
args = parser.parse_args()

url = "/api/clusqmgr/suspend/" + args.queuemanager + '/' + args.cluster;

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
		print "Suspend command succesfully send."

except httplib.HTTPException as e:
	print ('An HTTP error occurred while suspending the queuemanager in the cluster: ' +
		e.errno + e.strerror
	)
except socket.error as e:
	print e.strerror
	print 'Is the MQWeb daemon running?'
{% endhighlight %}
