var mqWebApp = angular.module('mqWebApp');

mqWebApp.controller('MessagesController', ['$scope', '$http', 'MQWEB_CONFIG', function($scope, $http, config) {
	$scope.loading = false;
	$scope.mqweb = null;
	$scope.messages = null;
	$scope.error = null;
	$scope.http_rc = 0;
	
	$scope.formData = {
	};

	$scope.load = function() {
		$scope.loading = true;
		$http.get('/message/browse/' + config.qmgrName + '/' + config.queueName, {
					cache: false,
					params : $scope.formData
					})
			.success(function(data, status) {
				$scope.loading = false;
				$scope.mqweb = data.mqweb;
				$scope.messages = [];
				$scope.http_rc = 0;
				
				data.messages.forEach(function(entry) { 
					$scope.messages.push({
						loading: false,
						toggle: false,
						data: entry
					});
				});
				$scope.error = data.error;
			}).error(function(data, status) {
				$scope.loading = false;
				$scope.http_rc = status;
				
		});
	};
	
	$scope.reload = function(message)
	{
		message.loading = true;
		$http.get('/message/browse/' + config.qmgrName + '/' + config.queueName + '/' + message.data.MsgId,
			{	cache : false	}
			)
			.success(function(data, status) {
				message.loading = false;
				$scope.http_rc = 0;
				if ( data.messages.length > 0 )
				{
					message.data = data.messages[0];
				}
			}).error(function(data, status) {
				message.loading = false;
				$scope.http_rc = status;
			});
	}

	if ( config.autoload )
	{
		$scope.load();
	}
}]);
