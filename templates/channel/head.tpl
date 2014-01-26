<script type="text/javascript" src="/static/js/app/channel/provider.js"></script>
<script type="text/javascript" src="/static/js/app/chstatus/provider.js"></script>
<script type="text/javascript">
	angular.module('mqWebApp').config(function(mqWebChannelProvider) {
    	mqWebChannelProvider.setQueueManager('<?= mqweb.qmgr ?>');
	});

	angular.module('mqWebApp').config(function(mqWebChannelStatusProvider) {
    	mqWebChannelStatusProvider.setQueueManager('<?= mqweb.qmgr ?>');
	});
	
	angular.module('mqWebApp').config(['$routeProvider',
		function($routeProvider) {
			$routeProvider.when('/', {
				templateUrl : '/static/html/channel/channels.html',
				controller : 'ChannelsController'
			}).when('/:channelName/:channelType', {
				templateUrl : '/static/html/channel/channel.html',
				controller : 'ChannelController'
			}).otherwise({
				redirectTo: '/'
			});
		}]);
</script>
<script type="text/javascript" src="/static/js/app/channel/channel.js"></script>
<script type="text/javascript" src="/static/js/app/chstatus/chstatus.js"></script>
