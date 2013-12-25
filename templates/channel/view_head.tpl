<script type="text/javascript">
	var mqWebApp = angular.module('mqWebApp',[]);
	mqWebApp.constant('MQWEB_CONFIG', {
		qmgrName : '<?=mqweb.qmgr ?>',
		channelName : '<?=channelName ?>',
		channelType : '<?=channelType ?>',
		autoload : true
	});
</script>
<script type="text/javascript" src="/static/js/app/channels.js"></script>