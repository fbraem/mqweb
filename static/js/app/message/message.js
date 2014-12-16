var mqWebApp = angular.module('mqWebApp');

mqWebApp.run(function ($rootScope) {
	$rootScope.messages = null; // All messages
});

mqWebApp.controller('MessagesController', ['$scope', '$rootScope', 'mqWebMessage', function($scope, $rootScope, mqWebMessage) {
	$scope.qmgr = mqWebMessage.getQueueManager();
	$scope.queue = mqWebMessage.getQueue();
	$scope.loading = false;
	$scope.mqweb = null;
	$scope.error = null;
	$scope.http_rc = 0;
	
	$scope.formData = {
		teaser : 80
	};

	$scope.load = function() {
		$scope.loading = true;
		mqWebMessage.browse($scope.formData)
			.success(function(data, status) {
				$scope.loading = false;
				$scope.mqweb = data.mqweb;
				$rootScope.messages = [];
				$scope.http_rc = 0;
				
				data.messages.forEach(function(entry) { 
					$rootScope.messages.push({
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
	
	$scope.toggle = function(msg)
	{
		msg.toggle = ! msg.toggle;
	}
	
	$scope.reload = function(message)
	{
		message.loading = true;
		mqWebMessage.browse(message.data.MsgId)
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
}]);

mqWebApp.controller('MessageController', ['$scope', '$routeParams', 'mqWebMessage', function($scope, $routeParams, mqWebMessage) {
	$scope.loading = false;
	$scope.mqweb = null;
	$scope.message = null;
	$scope.error = null;
	$scope.http_rc = 0;
	
	$scope.load = function() {
		$scope.loading = true;
		mqWebMessage.browse($routeParams.msgid)
			.success(function(data, status) {
				$scope.loading = false;
				$scope.mqweb = data.mqweb;
				if ( data.messages && data.messages.length > 0 ) {
					$scope.message = { data : data.messages[0] };
				}
				$scope.http_rc = 0;
				$scope.error = data.error;
			}).error(function(data, status) {
				$scope.loading = false;
				$scope.http_rc = status;
			});
	};
	$scope.load();
}]);


mqWebApp.controller('MessageDumpController', ['$scope', '$routeParams', 'mqWebMessage', function($scope, $routeParams, mqWebMessage) {
	$scope.loading = false;
	$scope.mqweb = null;
	$scope.message = null;
	$scope.error = null;
	$scope.http_rc = 0;

	$scope.load = function() {
		$scope.loading = true;
		mqWebMessage.dump($routeParams.msgid)
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
