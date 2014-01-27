<script type="text/javascript" src="/static/js/app/queue/provider.js"></script>
<script type="text/javascript">
	angular.module('mqWebApp').config(function(mqWebQueueProvider) {
		mqWebQueueProvider.setQueueManager('<?= mqweb.qmgr ?>');
	});
	
	angular.module('mqWebApp').config(['$routeProvider',
		function($routeProvider) {
			$routeProvider.when('/', {
				templateUrl : '/static/html/queue/queues.html',
				controller : 'QueuesController'
			}).when('/:queueName', {
				templateUrl : '/static/html/queue/queue.html',
				controller : 'QueueController'
			}).otherwise({
				redirectTo: '/'
			});
		}]);
</script>
<script type="text/javascript" src="/static/js/app/queue/queue.js"></script>
