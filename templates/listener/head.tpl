<script type="text/javascript" src="/static/js/app/listener/provider.js"></script>
<script type="text/javascript" src="/static/js/app/lsstatus/provider.js"></script>
<script type="text/javascript">
	angular.module('mqWebApp').config(function(mqWebListenerProvider) {
    	mqWebListenerProvider.setQueueManager('<?= mqweb.qmgr ?>');
	});

	angular.module('mqWebApp').config(function(mqWebListenerStatusProvider) {
    	mqWebListenerStatusProvider.setQueueManager('<?= mqweb.qmgr ?>');
	});
	
	angular.module('mqWebApp').config(['$routeProvider',
		function($routeProvider) {
			$routeProvider.when('/', {
				templateUrl : '/static/html/listener/listeners.html',
				controller : 'ListenersController'
			}).when('/:listenerName', {
				templateUrl : '/static/html/listener/listener.html',
				controller : 'ListenerController'
			}).otherwise({
				redirectTo: '/'
			});
		}]);
</script>
<script type="text/javascript" src="/static/js/app/listener/listener.js"></script>
<script type="text/javascript" src="/static/js/app/lsstatus/lsstatus.js"></script>
