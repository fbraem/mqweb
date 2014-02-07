var mqWebApp = angular.module('mqWebApp');

mqWebApp.directive('mqEventQtip', function($http, $compile, $templateCache) {
	return {
		restrict: 'A',
		link : function(scope, element, attrs) {
			$(element).qtip({
				position: {
					my: 'bottom left',
					at: 'top right'
				},
				content: {
					text: function(event, api) {
						var contentDOM = $(element).next('div'); // Use the "div" element next to this for the content
						var titleElement = contentDOM.find('#eventTitle');
						if ( titleElement )	{
							titleElement.hide();
							api.set('content.title', titleElement.text());
						}
						return contentDOM.html();
					}
				}
			});
		}
	}
});
	
mqWebApp.controller('QmgrController', ['$scope', 'mqWebQueueManager', function($scope, mqWebQueueManager) {
	$scope.loading = false;
	$scope.mqweb = null;
	$scope.qmgr = null;
	$scope.error = null;

	$scope.load = function() {
		$scope.loading = true;
		mqWebQueueManager.inquire()
			.success(function(data, status) {
				$scope.loading = false;
				$scope.mqweb = data.mqweb;
				$scope.qmgr = data.qmgr;
				$scope.error = data.error;
			}).error(function(data, status) {
				$scope.loading = false;
		});
	};
	
	$scope.load();
}]);

mqWebApp.controller('LocalQueueController', ['$scope', 'mqWebQueue', function($scope, mqWebQueue) {
	$scope.loading = false;
	$scope.mqweb = null;
	$scope.queues = null;
	$scope.error = null;
	$scope.http_rc = 0;

	$scope.load = function() {
		$scope.loading = true;
		mqWebQueue.inquire({
				queueDepth : 1,
				queueExcludeSystem: 'true',
				queueUsage : "normal",
				queueExcludeTemp : 'true'
		}).success(function(data, status) {
			$scope.loading = false;
			$scope.mqweb = data.mqweb;
			$scope.queues = data.queues;
			$scope.error = data.error;
		}).error(function(data, status) {
			$scope.loading = false;
			$scope.http_rc = status;
		});
	};
	
	$scope.load();
}]);

mqWebApp.controller('XmitQueueController', ['$scope', 'mqWebQueue', function($scope, mqWebQueue) {
	$scope.loading = false;
	$scope.mqweb = null;
	$scope.queues = null;
	$scope.error = null;
	$scope.http_rc = 0;

	$scope.load = function() {
		$scope.loading = true;
		mqWebQueue.inquire({
				queueDepth : 1,
				queueExcludeSystem: 'false',
				queueUsage : "xmitq"
		}).success(function(data, status) {
			$scope.loading = false;
			$scope.mqweb = data.mqweb;
			$scope.queues = data.queues;
			$scope.error = data.error;
		}).error(function(data, status) {
			$scope.loading = false;
			$scope.http_rc = status;
		});
	};
	
	$scope.load();
}]);

mqWebApp.controller('ChannelStatusController', ['$scope', 'mqWebChannelStatus', function($scope, mqWebChannelStatus) {
	$scope.loading = false;
	$scope.mqweb = null;
	$scope.statuses = null;
	$scope.error = null;
	$scope.http_rc = 0;

	$scope.load = function() {
		$scope.loading = true;
		mqWebChannelStatus.inquire()
			.success(function(data, status) {
				$scope.loading = false;
				$scope.mqweb = data.mqweb;
				$scope.statuses = data.statuses;
				$scope.error = data.error;
				})
			.error(function(data, status) {
				$scope.loading = false;
				$scope.http_rc = status;
			});
		};
	$scope.load();
}]);

mqWebApp.controller('EventMessageController', ['$scope', 'mqWebMessage', function($scope, mqWebMessage) {

	$scope.loading = false;
	$scope.events = null;
	$scope.error = null;
	$scope.curdepth = 0;
	$scope.http_rc = 0;

	$scope.load = function() {
		$scope.loading = true;
		mqWebMessage.events({
			limit : 3
		}).success(function(data, status) {
			$scope.loading = false;
			$scope.error = data.error;
				
			data.events.forEach(function(eventMsg) {
				if ( eventMsg.event.reason.code == 2035 /* MQRC_NOT_AUTHORIZED */ ) {
					switch(eventMsg.event.ReasonQualifier.value) {
						case 1: // MQRQ_CONN_NOT_AUTHORIZED
						case 20: // MQRQ_SYS_CONN_NOT_AUTHORIZED
							eventMsg.templateUrl = '/static/html/message/events/' + eventMsg.event.reason.desc + '-1.html';
							break;
						case 2: // MQRQ_OPEN_NOT_AUTHORIZED:
							eventMsg.templateUrl = '/static/html/message/events/' + eventMsg.event.reason.desc + '-2.html';
							break;
						case 3: // MQRQ_CLOSE_NOT_AUTHORIZED:
							eventMsg.templateUrl = '/static/html/message/events/' + eventMsg.event.reason.desc + '-3.html';
							break;
						case 4: // MQRQ_CMD_NOT_AUTHORIZED:
							eventMsg.templateUrl = '/static/html/message/events/' + eventMsg.event.reason.desc + '-4.html';
							break;
						case 17: // MQRQ_SUB_NOT_AUTHORIZED:
							eventMsg.templateUrl = '/static/html/message/events/' + eventMsg.event.reason.desc + '-5.html';
							break;
						case 18: // MQRQ_SUB_DEST_NOT_AUTHORIZED:
							eventMsg.templateUrl = '/static/html/message/events/' + eventMsg.event.reason.desc + '-6.html';
							break;
					}
				}
				else {
					eventMsg.templateUrl = '/static/html/message/events/' + eventMsg.event.reason.desc + '.html';
				}
			});
				
			$scope.events = data.events;
			$scope.curdepth = data.queue.curdepth;
		}).error(function(data, status) {
			$scope.loading = false;
			$scope.http_rc = status;
		});
	};
	$scope.load();
}]);

$(document).ready(function()
{
	$("[data-qtip]").qtip({
		content : {
			text: function(event, api) {
				return $(this).attr('data-qtip');
			},
			title: function(event, api) {
				return $(this).text();
			}
		},
		position: {
			my: 'bottom center',
			at: 'top center'
		}
	});
	$(".imgtip").qtip({
		content : {
			attr: 'alt'
		}
	});
});
