<script type="text/javascript" src="/static/js/app/message/provider.js"></script>
<script type="text/javascript">
	angular.module('mqWebApp').config(function(mqWebMessageProvider) {
		mqWebMessageProvider.setQueueManager('<?= mqweb.qmgr ?>');
		mqWebMessageProvider.setQueue('<?= mqweb.queue ?>');
	});
	
	angular.module('mqWebApp').config(['$routeProvider',
		function($routeProvider) {
			$routeProvider.when('/', {
				templateUrl : '/static/html/message/messages.html',
				controller : 'MessagesController'
			}).when('/dump/:msgid', {
				templateUrl : '/static/html/message/dump.html',
				controller : 'MessageDumpController'
			}).when('/view/:msgid', {
				templateUrl : '/static/html/message/message.html',
				controller : 'MessageController'
			}).otherwise({
				redirectTo: '/'
			});
		}]);
</script>
<script type="text/javascript" src="/static/js/app/message/message.js"></script>