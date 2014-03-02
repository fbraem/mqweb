var mqWebApp = angular.module('mqWebApp');

mqWebApp.run(function ($rootScope) {
	$rootScope.listeners = null; // All listeners

	// Call this when a listener has been reloaded on the detail page. 
	// So we see new values also on the overview page.
	$rootScope.updateListener = function(listener) {
		if ( $rootScope.listeners != null ) {
			for(c in $rootScope.listeners) {
				if ( $rootScope.listeners[c].data.ListenerName.value == listener.data.ListenerName.value ) {
					$rootScope.listeners[c].data = listener.data;
					break;
				}
			}
		}
	}
});

mqWebApp.controller('ListenersController', ['$scope', '$rootScope', 'mqWebListener', function($scope, $rootScope, mqWebListener) {
	$scope.qmgr = mqWebListener.getQueueManager();
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
		mqWebListener.inquire($scope.formData)
			.success(function(data, status) {
				$scope.loading = false;
				$scope.mqweb = data.mqweb;
				$rootScope.listeners = [];

				data.listeners.forEach(function(listener) {
					$rootScope.listeners.push({
						loading: false,
						toggle: false,
						data: listener
					})
				});

				$scope.error = data.error;
			}).error(function(data, status) {
				$scope.loading = false;
			});
	};

	$scope.reload = function(listener) {
		listener.loading = true;
		mqWebListener.inquire(listener.data.ListenerName.value)
			.success(function(data, status) {
				listener.loading = false;
				if ( data.listeners.length > 0 )
				{
					listener.data = data.listeners[0];
					$rootScope.updateListener(listener);
				}
			}).error(function(data, status) {
				listener.loading = false;
			});
	}

	$scope.toggle = function(listener) {
		listener.toggle = ! listener.toggle;
	}

}]);

mqWebApp.controller('ListenerController', ['$scope', '$rootScope', '$routeParams', 'mqWebListener', 'mqWebListenerStatus', function($scope, $rootScope, $routeParams, mqWebListener, mqWebListenerStatus) {
	$scope.loading = true;
	$scope.mqweb = null;
	$scope.error = null;
	$scope.listener = null;
	$scope.statuses = null;

	mqWebListener.inquire($routeParams.listenerName)
		.success(function(data, status) {
			$scope.loading = false;
			$scope.mqweb = data.mqweb;
			if ( data.listeners.length > 0 )
			{
				$scope.listener = { 'data' : data.listeners[0] };
				$rootScope.updateListener($scope.listener);
			}
			$scope.error = data.error;
		}).error(function(data, status) {
			$scope.loading = false;
	});

	mqWebListenerStatus.inquire($routeParams.listenerName)
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
		mqWebListener.inquire($scope.listener.data.ListenerName.value)
			.success(function(data, status) {
				$scope.loading = false;
				if ( data.listeners.length > 0 ) {
					$scope.listener.data = data.listeners[0];
				}
				$rootScope.updateListener($scope.listener);
			}).error(function(data, status) {
				$scope.loading = false;
			});
	}
}]);
