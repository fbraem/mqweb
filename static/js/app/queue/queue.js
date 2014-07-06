var mqWebApp = angular.module('mqWebApp');

mqWebApp.run(function ($rootScope) {
	$rootScope.queues = null; // All queues
	
	// Call this when a queue has been reloaded on the detail page. 
	// So we see new values also on the overview page.
	$rootScope.updateQueue = function(queue)
	{
		if ( $rootScope.queues != null )
		{
			for(q in $rootScope.queues)
			{
				if ( $rootScope.queues[q].data.QName.value == queue.data.QName.value )
				{
					$rootScope.queues[q].data = queue.data;
					break;
				}
			}
		}
	}
});

mqWebApp.controller('QueuesController', ['$scope', '$rootScope', 'mqWebQueue', function($scope, $rootScope, mqWebQueue) {
	$scope.qmgr = mqWebQueue.getQueueManager();

	$scope.loading = false;
	$scope.mqweb = null;
	$scope.error = null;
	$scope.queue = null; // Queue used for repeat
	
	$scope.formData = {
		queueName : "*",
		queueType : "All",
		excludeSystem : true,
		excludeTemp : true
	};

	$scope.load = function() {
		$scope.loading = true;
		mqWebQueue.inquire($scope.formData)
			.success(function(data, status) {
				$scope.loading = false;
				$scope.mqweb = data.mqweb;

				$rootScope.queues = [];
				data.queues.forEach(function(q) {
					$rootScope.queues.push({
						loading: false,
						toggle: false,
						data: q
					});
				});
				
				$scope.error = data.error;
			}).error(function(data, status) {
				$scope.loading = false;
		});
	};

	$scope.toggle = function(q)
	{
		q.toggle = ! q.toggle;
	}
	
	$scope.reload = function(queue)
	{
		mqWebQueue.inquire(queue.data.QName.value)
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
}]);

mqWebApp.controller('QueueController', ['$scope', '$rootScope', '$routeParams', 'mqWebQueue', function($scope, $rootScope, $routeParams, mqWebQueue) {

	$scope.loading = true;
	$scope.mqweb = null;
	$scope.error = null;
	$scope.queue = null;
	
	mqWebQueue.inquire($routeParams.queueName)
		.success(function(data, status) {
			$scope.loading = false;
			$scope.mqweb = data.mqweb;
			if ( data.queues.length > 0 )
			{
				$scope.queue = { 'data' : data.queues[0] };
				$rootScope.updateQueue($scope.queue);
			}
			$scope.error = data.error;
		}).error(function(data, status) {
			$scope.loading = false;
	});
		
	$scope.reload = function()
	{
		$scope.loading = true;
		mqWebQueue.inquire($scope.queue.data.QName.value)
			.success(function(data, status) {
				$scope.loading = false;
				if ( data.queues.length > 0 )
				{
					$scope.queue.data = data.queues[0];
				}
				$rootScope.updateQueue($scope.queue);
			}).error(function(data, status) {
				$scope.loading = false;
			});
	}
}]);
