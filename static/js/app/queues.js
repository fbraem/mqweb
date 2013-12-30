var mqWebApp = angular.module('mqWebApp');

mqWebApp.controller('QueuesController', ['$scope', '$http', 'MQWEB_CONFIG', function($scope, $http, config) {
	$scope.loading = false;
	$scope.mqweb = null;
	$scope.queues = null;
	$scope.error = null;
	
	$scope.formData = {
		queueName : config.queueName ? config.queueName : "*",
		queueType : config.queueType ? config.queueType : "All",
		queueExcludeSystem : false,
		queueExcludeTemp : false
	};

	$scope.load = function() {
		$scope.loading = true;
		$http.get('/queue/inquire/' + config.qmgrName, {
					cache: false,
					params : $scope.formData
					})
			.success(function(data, status) {
				$scope.loading = false;
				$scope.mqweb = data.mqweb;
				$scope.queues = [];
				for(q in data.queues)
				{
					$scope.queues.push({
						loading: false,
						toggle: false,
						data: data.queues[q]
					});
				}
				$scope.error = data.error;
			}).error(function(data, status) {
				$scope.loading = false;
		});
	};
	
	$scope.reload = function(queue)
	{
		queue.loading = true;
		$http.get('/queue/inquire/' + config.qmgrName,
			{	cache : false,
				params: {
					queueName: queue.data.QName.value,
					queueType: queue.data.QType.display
				}
			})
			.success(function(data, status) {
				queue.loading = false;
				if ( data.queues.length > 0 )
				{
					queue.data = data.queues[0];
				}
			}).error(function(data, status) {
				queue.loading = false;
			});
	}

	if ( config.autoload )
	{
		$scope.load();
	}
}]);
