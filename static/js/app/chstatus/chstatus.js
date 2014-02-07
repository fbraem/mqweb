var mqWebApp = angular.module('mqWebApp');

mqWebApp.run(function ($rootScope) {
	$rootScope.statuses = null; // All queues

	// Call this when a channel has been reloaded on the detail page. 
	// So we see new values also on the overview page.
	$rootScope.updateChannelStatus = function(status) {
		if ( $rootScope.statuses != null ) {
			for(c in $rootScope.statuses) {
				if ( $rootScope.statuses[c].data.ChannelName.value == status.data.ChannelName.value
					&& $rootScope.statuses[c].data.ChannelType.value == status.data.ChannelType.value ) {
					$rootScope.statuses[c].data = status.data;
					break;
				}
			}
		}
	}
});

mqWebApp.controller('ChannelStatusesController', ['$scope', '$rootScope', 'mqWebChannelStatus', function($scope, $rootScope, mqWebChannelStatus) {
	$scope.qmgr = mqWebChannel.getQueueManager();
	$scope.loading = false;
	$scope.mqweb = null;
	$scope.error = null;

	$scope.formData = {
		channelName : "*",
		channelType : "All",
		channelExcludeSystem : false
	};

	$scope.load = function() {
		$scope.loading = true;
		mqWebChannelStatus.inquire($scope.formData)
			.success(function(data, status) {
				$scope.loading = false;
				$scope.mqweb = data.mqweb;
				$rootScope.statuses = [];

				data.statuses.forEach(function(status) {
					$rootScope.statuses.push({
						loading: false,
						toggle: false,
						data: status
					})
				});

				$scope.error = data.error;
			}).error(function(data, status) {
				$scope.loading = false;
			});
	};

	$scope.reload = function(channelStatus) {
		channel.loading = true;
		mqWebChannelStatus.inquire(channelStatus.data.ChannelName.value, channelStatus.data.ChannelType.display)
			.success(function(data, status) {
				channelStatus.loading = false;
				if ( data.statuses.length > 0 )
				{
					channelStatus.data = data.statuses[0];
					$rootScope.updateChannelStatus(channelStatus);
				}
			}).error(function(data, status) {
				channelStatus.loading = false;
			});
	}

	$scope.toggle = function(status) {
		status.toggle = ! status.toggle;
	}

}]);

mqWebApp.controller('ChannelStatusController', ['$scope', '$rootScope', '$routeParams', 'mqWebChannelStatus', function($scope, $rootScope, $routeParams, mqWebChannelStatus) {
	$scope.loading = true;
	$scope.mqweb = null;
	$scope.error = null;
	$scope.status = null;

	mqWebChannelStatus.inquire($routeParams.channelName)
		.success(function(data, status) {
			$scope.loading = false;
			$scope.mqweb = data.mqweb;
			if ( data.statuses.length > 0 )
			{
				$scope.status = { 'data' : data.statuses[0] };
				$rootScope.updateChannelStatus($scope.status);
			}
			$scope.error = data.error;
		}).error(function(data, status) {
			$scope.loading = false;
	});

	$scope.reload = function() {
		$scope.loading = true;
		mqWebChannelStatus.inquire($scope.status.data.ChannelName.value)
			.success(function(data, status) {
				$scope.loading = false;
				if ( data.statuses.length > 0 ) {
					$scope.status.data = data.statuses[0];
				}
				$rootScope.updateChannelStatus($scope.status);
			}).error(function(data, status) {
				$scope.loading = false;
			});
	}
}]);
