<script type="text/javascript">
	var mqWebApp = angular.module('mqWebApp');
	mqWebApp.constant('MQWEB_CONFIG', {
		qmgrName : '<?= mqweb.qmgr ?>',
		queueName : '<?= queueName ?>',
		html: 'queue/queue.html',
		autoload : true
	});
</script>
<script type="text/javascript" src="/static/js/app/queues.js"></script>
