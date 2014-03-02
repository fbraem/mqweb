<script type="text/javascript" src="/static/js/app/qmgr/provider.js"></script>
<script type="text/javascript" src="/static/js/app/queue/provider.js"></script>
<script type="text/javascript" src="/static/js/app/chstatus/provider.js"></script>
<script type="text/javascript" src="/static/js/app/message/provider.js"></script>
<script type="text/javascript">
	angular.module('mqWebApp').config(function(mqWebQueueManagerProvider) {
		mqWebQueueManagerProvider.setQueueManager('<?= mqweb.qmgr ?>');
	});
	angular.module('mqWebApp').config(function(mqWebQueueProvider) {
		mqWebQueueProvider.setQueueManager('<?= mqweb.qmgr ?>');
	});
	angular.module('mqWebApp').config(function(mqWebChannelStatusProvider) {
		mqWebChannelStatusProvider.setQueueManager('<?= mqweb.qmgr ?>');
	});
	angular.module('mqWebApp').config(function(mqWebMessageProvider) {
		mqWebMessageProvider.setQueueManager('<?= mqweb.qmgr ?>');
		mqWebMessageProvider.setQueue('SYSTEM.ADMIN.QMGR.EVENT');
	});
</script>
<script type="text/javascript" src="/static/js/app/dashboard.js"></script>
