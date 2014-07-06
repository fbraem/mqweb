var mqWebApp = angular.module('mqWebApp');

mqWebApp.run(function ($rootScope) {
	$rootScope.channels = null; // All queues

	// Call this when a channel has been reloaded on the detail page. 
	// So we see new values also on the overview page.
	$rootScope.updateChannel = function(channel) {
		if ( $rootScope.channels != null ) {
			for(c in $rootScope.channels) {
				if ( $rootScope.channels[c].data.ChannelName.value == channel.data.ChannelName.value
					&& $rootScope.channels[c].data.ChannelType.value == channel.data.ChannelType.value ) {
					$rootScope.channels[c].data = channel.data;
					break;
				}
			}
		}
	}
});

mqWebApp.controller('ChannelsController', ['$scope', '$rootScope', 'mqWebChannel', function($scope, $rootScope, mqWebChannel) {
	$scope.qmgr = mqWebChannel.getQueueManager();
	$scope.loading = false;
	$scope.mqweb = null;
	$scope.error = null;

	$scope.formData = {
		channelName : "*",
		channelType : "All",
		excludeSystem : false
	};

	$scope.load = function() {
		$scope.loading = true;
		mqWebChannel.inquire($scope.formData)
			.success(function(data, status) {
				$scope.loading = false;
				$scope.mqweb = data.mqweb;
				$rootScope.channels = [];

				data.channels.forEach(function(chl) {
					$rootScope.channels.push({
						loading: false,
						toggle: false,
						data: chl
					})
				});

				$scope.error = data.error;
			}).error(function(data, status) {
				$scope.loading = false;
			});
	};

	$scope.reload = function(channel) {
		channel.loading = true;
		mqWebChannel.inquire(channel.data.ChannelName.value, channel.data.ChannelType.display)
			.success(function(data, status) {
				channel.loading = false;
				if ( data.channels.length > 0 )
				{
					channel.data = data.channels[0];
					$rootScope.updateChannel(channel);
				}
			}).error(function(data, status) {
				channel.loading = false;
			});
	}

	$scope.toggle = function(chl) {
		chl.toggle = ! chl.toggle;
	}

}]);

mqWebApp.controller('ChannelController', ['$scope', '$rootScope', '$routeParams', 'mqWebChannel', 'mqWebChannelStatus', function($scope, $rootScope, $routeParams, mqWebChannel, mqWebChannelStatus) {
	$scope.loading = true;
	$scope.mqweb = null;
	$scope.error = null;
	$scope.channel = null;
	$scope.statuses = null;

	mqWebChannel.inquire($routeParams.channelName, $routeParams.channelType)
		.success(function(data, status) {
			$scope.loading = false;
			$scope.mqweb = data.mqweb;
			if ( data.channels.length > 0 )
			{
				$scope.channel = { 'data' : data.channels[0] };
				$rootScope.updateChannel($scope.channel);
			}
			$scope.error = data.error;
		}).error(function(data, status) {
			$scope.loading = false;
	});

	mqWebChannelStatus.inquire($routeParams.channelName)
		.success(function(data, status) {
			$scope.loading = false;
			$scope.mqweb = data.mqweb;
			$scope.statuses = data.statuses;
			$scope.error = data.error;
		}).error(function(data, status) {
			$scope.loading = false;
	});

	$scope.reload = function() {
		$scope.loading = true;
		mqWebChannel.inquire($scope.channel.data.ChannelName.value, $scope.channel.data.ChannelType.display)
			.success(function(data, status) {
				$scope.loading = false;
				if ( data.channels.length > 0 ) {
					$scope.channel.data = data.channels[0];
				}
				$rootScope.updateChannel($scope.channel);
			}).error(function(data, status) {
				$scope.loading = false;
			});
	}
}]);
