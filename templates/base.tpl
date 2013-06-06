<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html lang="en-US" xmlns="http://www.w3.org/1999/xhtml" dir="ltr">
<head>
	<title>MQWeb - <?= mqweb.qmgr ?></title>
	<meta http-equiv="Content-type" content="text/html; charset=utf-8" />
	<link rel="shortcut icon" href="/static/css/images/favicon.ico" />
	<link rel="stylesheet" href="/static/css/jquery.qtip.css" type="text/css" media="all" />
	<link rel="stylesheet" href="/static/css/style.css" type="text/css" media="all" />
	<script src="/static/js/jquery-1.9.1.min.js" type="text/javascript" charset="utf-8"></script>
	<script src="/static/js/jquery.qtip.js" type="text/javascript" charset="utf-8"></script>
	<script src="/static/js/knockout-2.2.1.js" type="text/javascript" charset="utf-8"></script>
	<script src="/static/js/center.js" type="text/javascript" charset="utf-8"></script>
	<?= mqweb.views.head ?>
</head>
<body>
	<script type="text/html" id="mqErrorTemplate">    
		<div style="width:380px;margin: 0 auto;padding:10px;border:1px solid red;background-color:#FF9999;">
			<img  style="float:left;display:block;" src="/static/images/error.png" alt="error" />
			<div  style="float:left;margin-left:20px;">
				<strong>Object: </strong><span data-bind="text: object" /><br />
				<strong>Function: </strong><span data-bind="text: fn" /><br />
				<strong>Code: </strong><span data-bind="text: code" /><br />
				<strong>Reason: </strong><span data-bind="text: reason" />
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
						<h1>MQWeb - <a href="/qmgr/view/<?= mqweb.qmgr  ?>" title="home"><?= mqweb.qmgr ?></a></h1>
						<p class="slogan">Administrating Websphere MQ with a browser</p>
					</div>
					<div class="socials">
					<a href="https://github.com/fbraem/mqweb"><img style="position: absolute; top: 0; right: 0; border: 0;" src="https://s3.amazonaws.com/github/ribbons/forkme_right_red_aa0000.png" alt="Fork me on GitHub"></a>
<!--
						<a title="Facebook" class="facebook" href="#">facebook</a>
						<a title="Twitter" class="twitter" href="#">twitter</a>
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
				<div id="main">
					<?= mqweb.views.main ?>
				</div>
				<!-- END MAIN -->
			</div>
			<!-- END Wrapper Middle -->
			<div id="wrapper-bottom"></div>
			<!-- Footer -->
			<div id="footer">
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
