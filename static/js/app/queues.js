var mqWebApp = angular.module('mqWebApp');

mqWebApp.run(function ($rootScope) {
    $rootScope.queues = null; // All queues
    $rootScope.queue = null; // Selected queue
});

mqWebApp.controller('QueuesController', ['$scope', '$http', '$rootScope', 'MQWEB_CONFIG', 'queue', function($scope, $http, $rootScope, config, queueProvider) {
	$scope.loading = false;
	$scope.mqweb = null;
	$scope.error = null;
	
	$scope.formData = {
		queueName : config.queueName ? config.queueName : "*",
		queueType : config.queueType ? config.queueType : "All",
		queueExcludeSystem : true,
		queueExcludeTemp : true
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
				$rootScope.queues = [];
				for(q in data.queues)
				{
					$rootScope.queues.push({
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

	$scope.toggle = function(queue)
	{
		queue.toggle = ! queue.toggle;
		$rootScope.queue = queue;
	}
	
	$scope.reload = function(queue)
	{
		queueProvider.inquire(queue.data.QName.value)
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

mqWebApp.controller('QueueController', ['$scope', '$rootScope', '$routeParams', 'queue', function($scope, $rootScope, $routeParams, queueProvider) {
	$scope.loading = true;
	$scope.mqweb = null;
	$scope.error = null;
	
	queueProvider.inquire($routeParams.queueName)
		.success(function(data, status) {
			$scope.loading = false;
			$scope.mqweb = data.mqweb;
			if ( data.queues.length > 0 )
			{
				if ( $rootScope.queue == null )
				{
					$rootScope.queue = { 'data' : data.queues[0] };
				}
				else
				{
					$rootScope.queue.data = data.queues[0];
				}
			}
			$scope.error = data.error;
		}).error(function(data, status) {
			$scope.loading = false;
	});
		
	$scope.reload = function(queue)
	{
		$scope.loading = true;
		queueProvider.inquire(queue.data.QName.value)
			.success(function(data, status) {
				$scope.loading = false;
				if ( data.queues.length > 0 )
				{
					queue.data = data.queues[0];
				}
			}).error(function(data, status) {
				$scope.loading = false;
			});
	}

}]);
