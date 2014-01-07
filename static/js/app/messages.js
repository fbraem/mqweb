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
		var url = '/message/browse/' + config.qmgrName + '/' + config.queueName;
		if ( config.messageId )
		{
			url += '/' + config.messageId;
		}
		$http.get(url, {
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
				$scope.error = data.error;
				$scope.mqweb = data.mqweb;
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

mqWebApp.controller('MessageDumpController', ['$scope', '$http', 'MQWEB_CONFIG', function($scope, $http, config) {
	$scope.loading = false;
	$scope.mqweb = null;
	$scope.message = null;
	$scope.error = null;
	$scope.http_rc = 0;
	$scope.ascii = true;
	$scope.ebcdic = true;
	
	$scope.load = function() {
		$scope.loading = true;
		var url = '/message/dump.json/' + config.qmgrName + '/' + config.queueName + '/' + config.messageId;
		$http.get(url, {
					cache: false,
					})
			.success(function(data, status) {
				$scope.loading = false;
				$scope.mqweb = data.mqweb;
				$scope.message = data.message;
				$scope.http_rc = 0;
				$scope.error = data.error;
			}).error(function(data, status) {
				$scope.loading = false;
				$scope.http_rc = status;
			});
	};
	$scope.load();
}]);
