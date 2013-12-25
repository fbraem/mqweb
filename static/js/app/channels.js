var mqWebApp = angular.module('mqWebApp');

mqWebApp.controller('ChannelsController', ['$scope', '$http', 'MQWEB_CONFIG', function($scope, $http, config) {
	$scope.loading = false;
	$scope.mqweb = null;
	$scope.channels = null;
	$scope.error = null;
	
	$scope.formData = {
		channelName : config.channelName ? config.channelName : "*",
		channelType : config.channelType ? config.channelType : "All",
		channelExcludeSystem : false
	};

	$scope.load = function() {
		$scope.loading = true;
		$http.get('/channel/list.json/' + config.qmgrName, {
							  cache: false,
							  params : $scope.formData
						  }) 
			.success(function(data, status) {
				$scope.loading = false;
				$scope.mqweb = data.mqweb;
				$scope.channels = [];
				for(c in data.channels)
				{
					var ourChannelData = { 
						loading: false, 
						toggle: false,
						data: data.channels[c] 
					};
					$scope.channels.push(ourChannelData);
				}
				$scope.error = data.error;
			}).error(function(data, status) {
				$scope.loading = false;
		});
	};
	
	$scope.reload = function(channel)
	{
		channel.loading = true;
		$http.get('/channel/list.json/' + config.qmgrName,
			{	cache : false,
				params: { 
					channelName : channel.data.ChannelName.value, 
					channelType: channel.data.ChannelType.display
				}
			})
			.success(function(data, status) {
				channel.loading = false;
				if ( data.channels.length > 0 )
				{
					channel.data = data.channels[0];
				}
			}).error(function(data, status) {
				channel.loading = false;
			});
	}

	if ( config.autoload )
	{
		$scope.load();
	}
}]);
