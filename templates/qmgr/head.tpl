<script type="text/javascript">

var mqWebApp = angular.module('mqWebApp',[]);
  
mqWebApp.controller('QmgrController', ['$scope', '$http', function($scope, $http) {
	$scope.loading = false;
	$scope.mqweb = null;
	$scope.qmgr = null;
	$scope.error = null;

	$scope.load = function() {
		$scope.loading = true;
		$http.get('/qmgr/view.json/<?= mqweb.qmgr ?>') 
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

mqWebApp.controller('LocalQueueController', ['$scope', '$http', function($scope, $http) {
	$scope.loading = false;
	$scope.mqweb = null;
	$scope.queues = null;
	$scope.error = null;

	$scope.load = function() {
		$scope.loading = true;
		$http.get('/queue/list.json/<?= mqweb.qmgr ?>', {
			data : {
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
}]);

mqWebApp.controller('XmitQueueController', ['$scope', '$http', function($scope, $http) {
	$scope.loading = false;
	$scope.mqweb = null;
	$scope.queues = null;
	$scope.error = null;

	$scope.load = function() {
		$scope.loading = true;
		$http.get('/queue/list.json/<?= mqweb.qmgr ?>', {
			data : {
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
}]);

mqWebApp.controller('ChannelStatusController', ['$scope', '$http', function($scope, $http) {
	$scope.loading = false;
	$scope.mqweb = null;
	$scope.channels = null;
	$scope.error = null;

	$scope.load = function() {
		$scope.loading = true;
		$http.get('/chs/list.json/<?= mqweb.qmgr ?>')
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
}]);

/*
	function statusImage(status)
	{
		if ( status.ChannelStatus.display == 'Retrying' )
		{
			return "mqChannelStatusRetryingFlag";
		}
		else if ( status.ChannelStatus.display == 'Stopped' )
		{
			return "mqChannelStatusStoppedFlag";
		}
		else if ( status.ChannelStatus.display == 'Running' )
		{
			return "mqChannelStatusRunningFlag";
		}
		return "mqChannelStatusOtherFlag";
	}

	var EventMessageModel = function()
	{
		var self = this;

		self.loading = ko.observable(false);
		self.events = ko.observable(null);
		self.error = ko.observable(null);
		self.count = ko.observable(0);
		self.curdepth = ko.observable(0);
		self.partialView = ko.computed(function() { return (!self.loading()) && self.count() < self.curdepth(); }, self);

		self.load = function()
		{
			$.ajax(
			{
				beforeSend: function()
				{
					self.events(null);
					self.error(null);
					self.loading(true);
					self.count(0);
					self.curdepth(0);
				},
				url: "/message/event.json/<?= mqweb.qmgr  ?>/SYSTEM.ADMIN.QMGR.EVENT?limit=3",
				cache: false,
				dataType: "json",
				success: function(data)
				{
					self.loading(false);

					if ( data.error )
					{
						self.error(data.error);
					}
					else
					{
						self.events(data.events);
						self.count(data.events.length);
						self.curdepth(data.queue.curdepth);

						eventTips();
					}
				},
				error: function (request, status, error)
				{
					self.loading(false);
					//TODO:$("#eventMessages").html("An error occurred  while retrieving event messages from SYSTEM.ADMIN.QMGR.EVENT: " + status  + ", " + error);
				}
			});
		}
	}
*/
	$(document).ready(function()
	{
		$("[data-qtip]").qtip({
			content : {
				text: function(event, api)
					{
			return $(this).attr('data-qtip');
			},
		title: function(event, api)
					{
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
<script type="text/javascript">
function eventTips()
{
	$('[data-url]').qtip({
		position: {
			my: 'bottom left',
			at: 'top right'
		},
		content: {
			text: function(event, api) {
				$.ajax({
					url: $(this).data('url') // Use data-url attribute for the URL
				})
				.then(function(content) {
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
				}, function(xhr, status, error) {
					// Upon failure... set the tooltip content to the status and error value
					api.set('content.text', status + ': ' + error);
				});

				return 'Loading...'; // Set some initial text
			}
		}
	});
}
</script>
