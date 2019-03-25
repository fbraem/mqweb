'''
 This sample will read all queue statistic messages from SYSTEM.ADMIN.STATISTICS.QUEUE.
 MQWeb runs on localhost and is listening on port 8081.
'''

import json
import httplib
import socket
import argparse

parser = argparse.ArgumentParser(
	description='MQWeb - Python sample - Browse statistic messages from SYSTEM.ADMIN.STATISTICS.QUEUE',
	epilog="For more information: http://www.mqweb.org"
)
parser.add_argument('-m', '--queuemanager', help='Name of the queuemanager', required=True)
args = parser.parse_args()

size = 1024 * 32
url = "/api/message/browse/" + args.queuemanager + '/SYSTEM.ADMIN.STATISTICS.QUEUE?size=' + str(size)

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
			if 'admin' in message:
				parameters = message['admin']['parameters']
				print(str(parameters['IntervalStartDate']['value']) + ' ' +
					str(parameters['IntervalStartTime']['value']) + ' ' +
					str(parameters['IntervalEndDate']['value']) + ' ' +
					str(parameters['IntervalEndTime']['value']))
				if 'QStatisticsData' in parameters:
					queues = {}
					for statistics in parameters['QStatisticsData']['value']:
						queue = {
							'depth' : {
								'min' : statistics['QMinDepth']['value'],
								'max' : statistics['QMaxDepth']['value']
							},
							'get' : {
								'count' : statistics['GetCount']['value'][0] + statistics['GetCount']['value'][1],
								'bytes' : statistics['GetBytes']['value'][0] + statistics['GetBytes']['value'][1],
								'fail' : statistics['GetFailCount']['value']
							},
							'put' : {
								'count' : statistics['PutCount']['value'][0] + statistics['PutCount']['value'][1] + statistics['Put1Count']['value'][0] + statistics['Put1Count']['value'][1],
								'bytes' : statistics['PutBytes']['value'][0] + statistics['PutBytes']['value'][1],
								'fail' : statistics['PutFailCount']['value'] + statistics['Put1FailCount']['value']
							},
							'browse' : {
								'count' : statistics['BrowseCount']['value'][0] + statistics['BrowseCount']['value'][1],
								'bytes' : statistics['BrowseBytes']['value'][0] + statistics['BrowseBytes']['value'][1],
								'fail' : statistics['BrowseFailCount']['value']
							}
						}
						queues[statistics['QName']['value']] = queue
					print(json.dumps(queues))
				else:
					print(json.dumps(parameters))
		print('Number of messages: ' + str(count))
except httplib.HTTPException as e:
	print ('An HTTP error occurred while inquiring queuemanager: ' +
		e.errno + e.strerror
	)
except socket.error as e:
	print e.strerror
	print 'Is the MQWeb daemon running?'
