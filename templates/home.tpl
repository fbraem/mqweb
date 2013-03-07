<!DOCTYPE	html PUBLIC	"-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html	lang="en-US" xmlns="http://www.w3.org/1999/xhtml"	dir="ltr">
<head>
	<title>MQWeb - <?= mqweb.qmgr ?></title>
	<meta	http-equiv="Content-type"	content="text/html;	charset=utf-8" />
	<link	rel="shortcut	icon"	href="/static/css/images/favicon.ico"	/>
	<link	rel="stylesheet" href="/static/css/style.css"	type="text/css"	media="all"	/>
	<style>
		td {
			border:	1px	solid	#C1DAD7;
			background:	#fff;
			padding: 3px 3px 3px 6px;
		}
		
		td.alt {
			background:	#F5FAFA;
		}		
	</style>
	<script	src="/static/js/jquery-1.9.1.min.js" type="text/javascript"	charset="utf-8"></script>
	<script	src="/static/js/jquery.formalize.legacy.min.js" type="text/javascript"	charset="utf-8"></script>
	<script	type="text/javascript">
	$(document).ready(function()
	{
		loadLocalQueues();
		loadXmitQueues();
		loadChannelStatus();
		loadEventMessages();
	});

	function loadLocalQueues()
	{	
		$.ajax(
		{
			beforeSend:	function() {
				$("#localQueues").html($("#ajaxOverlay").html());
			},
			url: "/queue/list.json/<?= mqweb.qmgr ?>?queueDepth=1&queueType=Local&queueUsage=normal",
			cache: false,
			dataType:	"json",
			success: function(data)
			{
				if ( data.error	)
				{
					showError($("#localQueues"), data.error);
					return;
				}

				var	i	=	1;
				var	div	=	'<table	style="width:100%;border-spacing:0;border-collapse:collapse">';
				$.each(data.queues,	function(key,	val)
				{
					var	cl = '';
					if ( i++ % 2 ==	0	)
					{
						cl = 'class="alt"';
					}
					div	+= '<tr><td	'	+	cl +'>'	+	val.QName.value	+	'</th><td	'	+	cl +'	style="text-align:right;padding-right:6px;">'	+	val.CurrentQDepth.value	+	'</td></tr>';
				});
				div	+= '</table>';
				$('#localQueues').html(div);
			},
			error: function	(request,	status,	error)
			{
				$("#localQueues").html("An error occurred	while	retrieving local queues: " + status	+	", " + error);
			}
		});
	}	
	
	function loadXmitQueues()
	{
		$.ajax(
		{
			beforeSend:	function() {
				$("#xmitQueues").html($("#ajaxOverlay").html());
			},
			url: "/queue/list.json/<?= mqweb.qmgr ?>?queueDepth=1&queueExcludeSystem=0&queueUsage=xmitq",
			cache: false,
			dataType:	"json",
			success: function(data)
			{
				if ( data.error	)
				{
					showError($("#xmitQueues"),	data.error);
					return;
				}

				var	i	=	1;
				var	div	=	'<table	style="width:100%;border-spacing:0;border-collapse:collapse">';
				$.each(data.queues,	function(key,	val)
				{
					var	cl = '';
					if ( i++ % 2 ==	0	)
					{
						cl = 'class="alt"';
					}
					div	+= '<tr><td	'	+	cl +'>'	+	val.QName.value	+	'</th><td	'	+	cl +'	style="text-align:right;padding-right:6px;">'	+	val.CurrentQDepth.value	+	'</td></tr>';
				});
				div	+= '</table>';
				$('#xmitQueues').html(div);
			},
			error: function	(request,	status,	error)
			{
				$("#xmitq").html("An error occurred	while	retrieving local queues: " + status	+	", " + error);
			}
		});
	}
	
	function loadChannelStatus()
	{
		$.ajax(
		{
			beforeSend:	function() {
				$("#channelStatus").html($("#ajaxOverlay").html());
			},
			url: "/chs/list.json/<?= mqweb.qmgr ?>",
			cache: false,
			dataType:	"json",
			success: function(data)
			{
				if ( data.error	)
				{
					showError($("#channelStatus"), data.error);
					return;
				}

				var	i	=	1;
				var	div	=	'<table	style="width:100%;border-spacing:0;border-collapse:collapse">';
				$.each(data.statuses,	function(key,	val)
				{
					var	cl = '';
					if ( i++ % 2 ==	0	)
					{
						cl = 'class="alt"';
					}
					div	+= '<tr><td	'	+	cl +'>'	+	val.ChannelName.value	+	'</th><td	'	+	cl +'	style="padding-right:6px;">' + val.ChannelStatus.display + '</td></tr>';
				});
				div	+= '</table>';
				$('#channelStatus').html(div);
			},
			error: function	(request,	status,	error)
			{
				$("#channelStatus").html("An error occurred	while	retrieving local queues: " + status	+	", " + error);
			}
		});
	}	

	function loadEventMessages()
	{
		$.ajax(
		{
			beforeSend:	function() {
				$("#eventMessages").html($("#ajaxOverlay").html());
			},
			url: "/message/event.json/<?=	mqweb.qmgr	?>/SYSTEM.ADMIN.QMGR.EVENT?limit=3",
			cache: false,
			dataType:	"json",
			success: function(data)
			{
				if ( data.error	)
				{
					showError($("#eventMessages"), data.error);
					return;
				}

				var	i	=	1;

				var	div	=	'<table	style="border-spacing:0;border-collapse:collapse">';
				div	+= '<thead>';
				div	+= '<tr>';
				div	+= '<th style="text-align:left;">Date</th>';
				div	+= '<th style="text-align:left;">Reason</th>';
				div	+= '</tr>';
				div	+= '</thead>';
				div	+= '<tbody>';

				$.each(data.events,	function(key,	val)
				{
					var	cl = '';
					if ( i++ % 2 ==	0	)
					{
						cl = 'class="alt"';
					}
					div	+= '<tr><td	'	+	cl +'>'	+	val.putDate	+	'</th><td	'	+	cl +'	style="padding-right:6px;">' + val.reason + ' - ' + val.desc + '</td></tr>';
				});
				div	+= '</tbody></table>';
				div += '<p style="margin-top:5px;">';
				div += 'Number of messages on this queue: <strong>' + data.queues[0].CurrentQDepth.value + '</strong><br />';
				if ( data.limited )
				{
					div += 'Only the first <strong>' + data.events.length + '</strong> messages are shown. Use queue detail page to browse all messages.';
				}
				div += '</p>';
				$('#eventMessages').html(div);
			},
			error: function	(request,	status,	error)
			{
				$("#eventMessages").html("An error occurred	while	retrieving event messages from SYSTEM.ADMIN.QMGR.EVENT: " + status	+	", " + error);
			}
		});
	}
	
	function showError(el, error)
	{
		var	html = '<div style="margin-top:5px;padding:10px;border:1px solid red;background-color:#FF9999;">';
		html +=	'<img	style="float:left;display:block;"	src="/static/images/error.png" alt="error" />'
		html +=	'<div	style="float:left;margin-left:20px;">';
		html +=	'<strong>Object: </strong>'	+	error.object + '<br	/>';
		html +=	'<strong>Function: </strong>'	+	error.fn + '<br	/>';
		html +=	'<strong>Code: </strong>'	+	error.code	+	'<br />';
		html +=	'<strong>Reason: </strong>'	+	error.reason;
		html +=	'</div><div	class="cl">	</div>';
		
		el.html(html);
	}
	</script>
</head>
<body>
	<div id="ajaxOverlay"	style="display:none;">
	 <div	id="ajaxSpinner" style="background:url('/static/images/ajaxSpinner.gif') no-repeat center	center transparent;	width:100%;height:40px;">
	 </div>
	</div>
	<!-- Wrapper -->
	<div id="wrapper">
		<div class="shell">
			<div id="wrapper-top"></div>
			<!-- Wrapper Middle	-->
			<div id="wrapper-middle">
				<!-- Header	-->
				<div id="header">
					<!-- Logo	-->
					<div id="logo">
						<h1>MQWeb	-	<a href="/qmgr/view/<?=	mqweb.qmgr	?>"	title="home"><?= mqweb.qmgr ?></a></h1>
						<p class="slogan">Administrating Websphere MQ	with a browser</p>
					</div>
					<div class="socials">
					<a href="https://github.com/fbraem/mqweb"><img style="position:	absolute;	top: 0;	right: 0;	border:	0;"	src="https://s3.amazonaws.com/github/ribbons/forkme_right_red_aa0000.png"	alt="Fork	me on	GitHub"></a>
<!--
						<a title="Facebook"	class="facebook" href="#">facebook</a>
						<a title="Twitter" class="twitter" href="#">twitter</a>
						<a title="Digg"	class="digg" href="#">digg</a>
						<a title="Delicious" class="delicious" href="#">Delicious</a>
						<a title="Stumble	Upon"	class="su" href="#">su</a>
						<a title="RSS" class="rss" href="#">rss</a>	
-->
					</div>
					<div class="cl"></div>
				</div>
				<!-- END Header	-->
				<!-- Navigation	-->
				<div id="navigation">
					<ul>
						<li><a title="Home"	href="/qmgr/view/<?= mqweb.qmgr ?>">Home</a></li>
						<li><a title="Queues"	href="/queue/index/<?=	mqweb.qmgr	?>">Queues</a></li>
					</ul>
					<div class="cl"></div>
				</div>
				<!-- END Navigation	-->
				<!-- Main	-->
				<div id="main">
					<!-- Content -->
					<div id="content">
						<div class="post">
							<h3>QueueManager <strong><?= mqweb.qmgr ?></strong></h3>
							<img src="/static/css/images/briefcase.jpg"	alt="briefcase"	/>
							<p><strong><?= qmgr.QMgrDesc.value ?></strong></p>
							<div class="details" style="float:left;">
								<h2>QueueManager ID</h2><div class="detail"><?=	qmgr.QMgrIdentifier.value	?></div>
								<? ifexist qmgr.CreationDate ?>
									<h2>Creation Date</h2><div class="detail"><?=	qmgr.CreationDate.value	?> <?= qmgr.CreationTime.value ?></div>
								<? endif ?>
								<h2>Alteration Date</h2><div class="detail"><?=	qmgr.AlterationDate.value	?> <?= qmgr.AlterationTime.value ?></div>
								<h2>Dead Letter	Queue</h2><div class="detail"><a href="/queue/view/<?=mqweb.qmgr?>/<?=	qmgr.DeadLetterQName.value ?>"><?= qmgr.DeadLetterQName.value	?></a></div>
							</div>
						</div>
					</div>
					<!-- END Content -->
					<!-- Sidebar -->
					<div id="sidebar">
						<div class="box">
							<h3>Our	Services</h3>
							<ul>
								<li><a title="List Queues	Containing Messages" href="#">List Non-empty Queues</a></li>
								<li><a title="Ipsum	is not simply" href="#">Ipsum	is not simply</a></li>
								<li><a title="Dom	text roots"	href="#">Dom text	roots</a></li>
								<li><a title="Popular	belief lorem"	href="#">Popular belief	lorem</a></li>
								<li><a title="Ipsum	is not simply" href="#">Ipsum	is not simply</a></li>
							</ul>
						</div>
					</div>
					<!-- END Sidebar -->
					<div class="cl"></div>
					<div class="widgets">
						<div class="col	activities">
							<h3>Local	Queues</h3>
							<p style="margin-bottom:5px">This	is a list	with local queues	that contains	at least one message.</p>
							<div id="localQueues">
							</div>
							<a title="Reload"	class="read-more"	href="#" onclick="loadLocalQueues();return false;">Reload</a>
						</div>
						<div class="col	activities">
							<h3>Transmission Queues</h3>
							<p style="margin-bottom:5px">This	is a list	with transmission	queues that	contains at	least	one	message. On	a	healthy	system,	this list	would	be empty ...</p>
							<div id="xmitQueues">
							</div>
							<a title="Reload"	class="read-more"	href="#" onclick="loadXmitQueues();return	false;">Reload</a>
						</div>
						<div class="col	activities">
							<h3>Channel	Status</h3>
							<p style="margin-bottom:5px;">This	is a list	with channel statuses.</p>
							<div id="channelStatus">
							</div>
							<a title="Reload"	class="read-more"	href="#" onclick="loadChannelStatus();return false;">Reload</a>
						</div>
						<div class="cl"></div>
					</div>
					<div class="widgets">
						<div class="projects">
							<h3>SYSTEM.ADMIN.QMGR.EVENT</h3>
							<div id="eventMessages">
							</div>
							<div class="cl"></div>
						</div>
						<div class="col	contacts">
							<h3>Contact	Us</h3>
							<ul class="box">
								<li><a title="Zumuta!" href="http://www.zumuta.be">My Website</a></li>
								<li><a title="Github Sourcecode Repository" href="https://github.com/fbraem/mqweb">Github Sourcecode Repository</a></li>
								<li><a title="Issues/Bugs" href="https://github.com/fbraem/mqweb/issues">Isssue / Bug Reports</a></li>
							</ul>
						</div>
						<div class="cl"></div>
					</div>
				</div>
				<!-- END Main	-->
			</div>
			<!-- END Wrapper Middle	-->
			<div id="wrapper-bottom"></div>
			<!-- Footer	-->
			<div id="footer">
				<p class="copy">Copyright	2012<span>|</span><a href="http://www.zumuta.be">www.zumuta.be</a>.	Design by	<a title="CSS	Free Templates with	jQuery Slider" href="http://css-free-templates.com/">CSS-Free-Templates.com</a></p>
			<!--	<p id="bottom-nav"><a	title="Home" href="#">Home</a><span>|</span><a title="About" href="#">About</a><span>|</span><a	title="Services" href="#">Services</a><span>|</span><a title="Projects"	href="#">Projects</a><span>|</span><a	title="Solutions"	href="#">Solutions</a><span>|</span><a title="Jobs"	href="#">Jobs</a><span>|</span><a	title="Blog" href="#"> Blog</a><span>|</span><a	title="Contacts" href="#">Contacts</a></p> -->
				<div class="cl"></div>
			</div>
			<!-- END Footer	-->
		</div>
	</div>
	<!-- END Wrapper -->
</body>
</html>
