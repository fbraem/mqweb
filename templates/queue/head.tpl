<script type="text/javascript">
	var mqWebApp = angular.module('mqWebApp');
	mqWebApp.constant('MQWEB_CONFIG', {
		qmgrName : '<?= mqweb.qmgr ?>'});
		
	mqWebApp.config(['$routeProvider',
		function($routeProvider) {
			$routeProvider.when('/', {
				templateUrl : '/static/html/queue/queues.html',
				controller : 'QueuesController'
			}).when('/queue/:queueName', {
				templateUrl : '/static/html/queue/queue.html',
				controller : 'QueueController'
			}).otherwise({
				redirectTo: '/'
			});
		}]);
</script>
<script type="text/javascript" src="/static/js/app/queues.js"></script>
