<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html ng-app="mqWebApp" lang="en-US" xmlns="http://www.w3.org/1999/xhtml" dir="ltr">
<head>
	<title>MQWeb - <?= mqweb.qmgr ?></title>
	<meta http-equiv="Content-type" content="text/html; charset=utf-8" />
	<meta http-equiv="X-UA-Compatible" content="IE=9; IE=8; IE=7; IE=EDGE" />
	<link rel="shortcut icon" href="/static/css/images/favicon.ico" />
	<link rel="stylesheet" href="/static/css/style.css" type="text/css" media="all" />
	<link rel="stylesheet" href="/static/css/jquery.qtip.css" type="text/css" media="all" />
	<link rel="stylesheet" href="/static/css/ui-lightness/jquery-ui-1.10.3.custom.min.css" type="text/css" media="all" />
	<script src="//ajax.googleapis.com/ajax/libs/angularjs/1.2.4/angular.min.js"></script>
	<script src="//ajax.googleapis.com/ajax/libs/jquery/1.10.2/jquery.min.js"></script>	
	<script src="//ajax.googleapis.com/ajax/libs/jqueryui/1.10.3/jquery-ui.min.js"></script>
	<script src="/static/js/center.js" type="text/javascript" charset="utf-8"></script>
	<script src="/static/js/jquery.qtip.min.js" type="text/javascript" charset="utf-8"></script>
	<?= views.head ?>
</head>
<body>
	<script type="text/ng-template" id="mqErrorTemplate">    
		<div style="width:380px;margin: 0 auto;padding:10px;border:1px solid red;background-color:#FF9999;">
			<img  style="float:left;display:block;" src="/static/images/error.png" alt="error" />
			<div  style="float:left;margin-left:20px;">
				<strong>Object: </strong>{{ error.object }}<br />
				<strong>Function: </strong>{{ error.fn }}<br />
				<strong>Code: </strong>{{ error.code }}<br />
				<strong>Reason: </strong>{{ error.reason }}
			</div>
			<div  class="cl"> 
		</div>
	</script>
	<div id="wrapper">
		<div class="shell">
			<div id="wrapper-top"></div>
			<div id="wrapper-middle">
				<!-- HEADER -->
				<div id="header">
					<div id="logo">
						<h1>MQWeb - <a href="/qmgr/index/<?= mqweb.qmgr  ?>" title="home"><?= mqweb.qmgr ?></a></h1>
						<p class="slogan">Administrating Websphere MQ with a browser</p>
					</div>
					<a href="https://github.com/fbraem/mqweb"><img style="position: absolute; top: 0; right: 0; border: 0;" src="https://s3.amazonaws.com/github/ribbons/forkme_right_red_aa0000.png" alt="Fork me on GitHub"></a>
					<div class="socials">
						<a style="margin-left:200px;" title="WebSphere MQ Support Twitter" href="http://twitter.com/IBM_WMQ"><img src="/static/css/images/twitter.png" /></a>
						<a title="Facebook Websphere MQ" href="https://www.facebook.com/#!/pages/IBM-WebSphere-MQ/8304628654"><img src="/static/css/images/facebook.png" /></a>
<!--
						<a title="Digg" class="digg" href="#">digg</a>
						<a title="Delicious" class="delicious" href="#">Delicious</a>
						<a title="Stumble Upon" class="su" href="#">su</a>
						<a title="RSS" class="rss" href="#">rss</a> 
-->
					</div>
					<div class="cl"></div>
				</div> 
				<!-- END HEADER -->
				<!-- NAVIGATION -->
				<div id="navigation">
					<ul>
						<li><a title="Home" href="/qmgr/index/<?= mqweb.qmgr ?>">Home</a></li>
						<li><a title="Queues" href="/queue/index/<?= mqweb.qmgr ?>">Queues</a></li>
						<li><a title="Channels" href="/channel/index/<?= mqweb.qmgr ?>">Channels</a></li>
					</ul>
					<div class="cl"></div>
				</div>
				<!-- END NAVIGATION -->
				<!-- MAIN -->
				<?if mqweb.client ?>
					<img id="connectionTip" alt="Client Mode" title="Client Mode" src="/static/images/connection.png" style="float:right;padding:10px;" />
					<script type="text/javascript">
						$(document).ready(function()
						{
							$('#connectionTip').qtip({
								content : {
									title : 'Connection Mode',
									text : 'MQWeb connects to queuemanager <strong><?= mqweb.qmgr ?></strong> in client mode.'
									},
								position: {
									my: 'top center',
									at: 'bottom center'
									}
								});
						});
					</script>
				<?else ?>
					<img alt="Binding Mode" src="/static/images/connection.png" style="float:right;padding:10px;" />
					<script type="text/javascript">
						$(document).ready(function()
						{
							$('#connectionTip').qtip({
								content : {
									title : 'Connection Mode',
									text : 'MQWeb connects to queuemanager <strong><?= mqweb.qmgr ?></strong> in binding mode.'
									},
								position: {
									my: 'top center',
									at: 'bottom center'
									}
								});
						});
					</script>
				<?endif ?>
				<div id="main">

					<?= views.main ?>
				</div>
				<!-- END MAIN -->
			</div>
			<!-- END Wrapper Middle -->
			<div id="wrapper-bottom"></div>
			<!-- Footer -->
			<div id="footer">
				<a title="POCO" href="http://www.pocoproject.org"><img src="/static/images/poco.gif" alt="POCO" /></a>
				<p class="copy">Copyright 2013<span>|</span><a href="http://www.zumuta.be">www.zumuta.be</a>. Design by <a title="CSS Free Templates with jQuery Slider" href="http://css-free-templates.com/">CSS-Free-Templates.com</a></p>
			<!--  <p id="bottom-nav"><a title="Home" href="#">Home</a><span>|</span><a title="About" href="#">About</a><span>|</span><a title="Services" href="#">Services</a><span>|</span><a title="Projects" href="#">Projects</a><span>|</span><a title="Solutions" href="#">Solutions</a><span>|</span><a title="Jobs" href="#">Jobs</a><span>|</span><a title="Blog" href="#"> Blog</a><span>|</span><a title="Contacts" href="#">Contacts</a></p> -->
				<div class="cl"></div>
			</div>
			<!-- END Footer -->
		</div>
	</div>
	<!-- END Wrapper -->
</body>
</html>
