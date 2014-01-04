<link rel="stylesheet" href="/static/css/formalize.css" type="text/css" media="all" />
<script src="/static/js/jquery.form.js" type="text/javascript" charset="utf-8"></script>
<script src="/static/js/jquery.formalize.legacy.min.js" type="text/javascript" charset="utf-8"></script>
<script type="text/javascript">
	var mqWebApp = angular.module('mqWebApp');
	mqWebApp.constant('MQWEB_CONFIG', {
		qmgrName : '<?= mqweb.qmgr ?>',
		queueName : '<?= mqweb.queue ?>',
		html : 'message/messages.html' });
</script>
<script type="text/javascript" src="/static/js/app/messages.js"></script>