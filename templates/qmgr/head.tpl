<script type="text/javascript">

var mqWebApp = angular.module('mqWebApp',[]);

mqWebApp.directive('mqEventQtip', function($http, $compile, $templateCache) {
	return {
		restrict: 'A',
		scope : {
			mqWebURL : '@mqEventQtip'
		},
		link : function(scope, element, attrs) {
			$(element).qtip({
				position: {
					my: 'bottom left',
					at: 'top right'
				},
				content: {
					text: function(event, api) {
						$http.get(scope.mqWebURL, { cache: false })
						.success(function(content, status) {
							// Set the tooltip content upon successful retrieval
							var contentDOM = $(content);
							// When a title id is found, use it as qtip title and make
							// the title invisible.
							var titleElement = contentDOM.find("#eventTitle");
							if ( titleElement )
							{
								titleElement.hide();
								api.set('content.title', titleElement.text());
							}
							api.set('content.text', contentDOM.html());
						}).error(function(xhr, status, error) {
							// Upon failure... set the tooltip content to the status and error value
							api.set('content.text', status + ': ' + error);
						});
		
						return 'Loading...'; // Set some initial text
					}
				}
			});
		}
	}
});
  
mqWebApp.controller('QmgrController', function($scope, $http) {
	$scope.loading = false;
	$scope.mqweb = null;
	$scope.qmgr = null;
	$scope.error = null;

	$scope.load = function() {
		$scope.loading = true;
		$http.get('/qmgr/view.json/<?= mqweb.qmgr ?>', { cache: false }) 
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
});

mqWebApp.controller('LocalQueueController', function($scope, $http) {
	$scope.loading = false;
	$scope.mqweb = null;
	$scope.queues = null;
	$scope.error = null;

	$scope.load = function() {
		$scope.loading = true;
		$http.get('/queue/list.json/<?= mqweb.qmgr ?>', {
			cache: false,
			params : {
				queueDepth : 1,
				queueExcludeSystem: 1,
				queueUsage : "normal",
				queueExcludeTemp : 1
			}
		}).success(function(data, status) {
			$scope.loading = false;
			$scope.mqweb = data.mqweb;
			$scope.queues = data.queues;
			$scope.error = data.error;
		}).error(function(data, status) {
			$scope.loading = false;
		});
	};
	
	$scope.load();
});

mqWebApp.controller('XmitQueueController', function($scope, $http) {
	$scope.loading = false;
	$scope.mqweb = null;
	$scope.queues = null;
	$scope.error = null;

	$scope.load = function() {
		$scope.loading = true;
		$http.get('/queue/list.json/<?= mqweb.qmgr ?>', {
			cache : false,
			params : {
				queueDepth : 1,
				queueExcludeSystem: 0,
				queueUsage : "xmitq"
			}
		}).success(function(data, status) {
			$scope.loading = false;
			$scope.mqweb = data.mqweb;
			$scope.queues = data.queues;
			$scope.error = data.error;
		}).error(function(data, status) {
			$scope.loading = false;
		});
	};
	
	$scope.load();
});

mqWebApp.controller('ChannelStatusController', function($scope, $http) {
	$scope.loading = false;
	$scope.mqweb = null;
	$scope.channels = null;
	$scope.error = null;

	$scope.load = function() {
		$scope.loading = true;
		$http.get('/chs/list.json/<?= mqweb.qmgr ?>', { cache: false })
			.success(function(data, status) {
				$scope.loading = false;
				$scope.mqweb = data.mqweb;
				$scope.channels = data.channels;
				$scope.error = data.error;
				})
			.error(function(data, status) {
				$scope.loading = false;
			});
		};
	$scope.load();
});

mqWebApp.controller('EventMessageController', function($scope, $http) {

	$scope.loading = false;
	$scope.events = null;
	$scope.error = null;
	$scope.curdepth = 0;

	$scope.load = function() {
		$scope.loading = true;
		$http.get("/message/event.json/<?= mqweb.qmgr  ?>/SYSTEM.ADMIN.QMGR.EVENT?limit=3", { cache: false })
			.success(function(data, status) {
				$scope.loading = false;
				$scope.error = data.error;
				$scope.events = data.events;
				$scope.curdepth = data.queue.curdepth;
				//eventTips();
			})
			.error(function(data, error) {
				$scope.loading = false;
				//TODO:$("#eventMessages").html("An error occurred  while retrieving event messages from SYSTEM.ADMIN.QMGR.EVENT: " + status  + ", " + error);
			});
		};
	$scope.load();
});

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
</script>