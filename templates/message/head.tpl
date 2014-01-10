<script type="text/javascript">
	var mqWebApp = angular.module('mqWebApp');
	mqWebApp.constant('MQWEB_CONFIG', {
		qmgrName : '<?= mqweb.qmgr ?>',
		queueName : '<?= mqweb.queue ?>',
		html : 'message/messages.html' });
</script>
<script type="text/javascript" src="/static/js/app/messages.js"></script>