var mqWebApp = angular.module('mqWebApp');

mqWebApp.run(function ($rootScope) {
	$rootScope.statuses = null; // All queues

	// Call this when a listener has been reloaded on the detail page. 
	// So we see new values also on the overview page.
	$rootScope.updateListenerStatus = function(status) {
		if ( $rootScope.statuses != null ) {
			for(l in $rootScope.statuses) {
				if ( $rootScope.statuses[l].data.ListenerName.value == status.data.ListenerName.value ) {
					$rootScope.statuses[l].data = status.data;
					break;
				}
			}
		}
	}
});

mqWebApp.controller('ListenerStatusesController', ['$scope', '$rootScope', 'mqWebListenerStatus', function($scope, $rootScope, mqWebListenerStatus) {
	$scope.qmgr = mqWebListenerStatus.getQueueManager();
	$scope.loading = false;
	$scope.mqweb = null;
	$scope.error = null;

	$scope.formData = {
		listenerName : "*",
		transportType : "All",
		listenerExcludeSystem : false
	};

	$scope.load = function() {
		$scope.loading = true;
		mqWebListenerStatus.inquire($scope.formData)
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

	$scope.reload = function(listenerStatus) {
		listener.loading = true;
		mqWebListenerStatus.inquire(listenerStatus.data.ListenerName.value)
			.success(function(data, status) {
				listenerStatus.loading = false;
				if ( data.statuses.length > 0 )
				{
					listenerStatus.data = data.statuses[0];
					$rootScope.updateListenerStatus(listenerStatus);
				}
			}).error(function(data, status) {
				listenerStatus.loading = false;
			});
	}

	$scope.toggle = function(status) {
		status.toggle = ! status.toggle;
	}

}]);

mqWebApp.controller('ListenerStatusController', ['$scope', '$rootScope', '$routeParams', 'mqWebListenerStatus', function($scope, $rootScope, $routeParams, mqWebListenerStatus) {
	$scope.loading = true;
	$scope.mqweb = null;
	$scope.error = null;
	$scope.status = null;

	mqWebListenerStatus.inquire($routeParams.listenerName)
		.success(function(data, status) {
			$scope.loading = false;
			$scope.mqweb = data.mqweb;
			if ( data.statuses && data.statuses.length > 0 )
			{
				$scope.status = { 'data' : data.statuses[0] };
				$rootScope.updateListenerStatus($scope.status);
			}
			$scope.error = data.error;
		}).error(function(data, status) {
			$scope.loading = false;
	});

	$scope.reload = function() {
		$scope.loading = true;
		mqWebListenerStatus.inquire($scope.status.data.ListenerName.value)
			.success(function(data, status) {
				$scope.loading = false;
				if ( data.statuses.length > 0 ) {
					$scope.status.data = data.statuses[0];
				}
				$rootScope.updateListenerStatus($scope.status);
			}).error(function(data, status) {
				$scope.loading = false;
			});
	}
}]);
