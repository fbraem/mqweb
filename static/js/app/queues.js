var mqWebApp = angular.module('mqWebApp');

function QueuesController($scope, $http, $rootScope, config) {
	if ( ! $rootScope.queues )
	{
		$rootScope.queues = null;
	}
	
	if ( ! $rootScope.queue )
	{
		$rootScope.queue = null; // Selected queue
	}

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
};

QueuesController.$inject= ['$scope', '$http', '$rootScope', 'MQWEB_CONFIG'];

mqWebApp.controller('QueueController', ['$scope', '$http', '$rootScope', '$routeParams', 'MQWEB_CONFIG', function($scope, $http, $rootScope, $routeParams, config) {
	$scope.loading = true;
	$scope.mqweb = null;
	$scope.error = null;
	
	$http.get('/queue/inquire/' + config.qmgrName, {
					cache: false,
					params : { queueName : $routeParams.queueName }
					})
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
}]);
