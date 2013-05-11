<!DOCTYPE	html PUBLIC	"-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html	lang="en-US" xmlns="http://www.w3.org/1999/xhtml"	dir="ltr">
<head>
	<title>MQWeb - <?= mqweb.qmgr ?></title>
	<meta	http-equiv="Content-type"	content="text/html;	charset=utf-8" />
	<link	rel="shortcut	icon"	href="/static/css/images/favicon.ico"	/>
	<link	rel="stylesheet" href="/static/css/jquery.qtip.css"	type="text/css"	media="all"	/>
	<link	rel="stylesheet" href="/static/css/style.css"	type="text/css"	media="all"	/>
	<script	src="/static/js/jquery-1.9.1.min.js" type="text/javascript"	charset="utf-8"></script>
	<script	src="/static/js/jquery.formalize.legacy.min.js" type="text/javascript" charset="utf-8"></script>
	<script	src="/static/js/jquery.qtip.min.js" type="text/javascript" charset="utf-8"></script>
	<script src="/static/js/knockout-2.2.1.js" type="text/javascript" charset="utf-8"></script>
	<script	type="text/javascript">
	function Queue(data)
	{
		this.name = data.QName.value;
		this.curdepth = data.CurrentQDepth.value;
		this.url = '/queue/view/<?=mqweb.qmgr?>/' + this.name;
	}
	
	var LocalQueueModel = function()
	{
		var self = this;
		
		self.queues = ko.observableArray([]);
		self.loading = ko.observable(false);
		self.error = ko.observable();
		
		this.load = function()
			{
				$.ajax(
				{
					beforeSend:	function() {
						self.queues.removeAll();
						self.loading(true);
					},
					url: "/queue/list.json/<?= mqweb.qmgr ?>",
					data : {
						queueDepth : 1,
						queueExcludeSystem: 1,
						queueUsage : "normal"
					},
					cache: false,
					dataType:	"json",
					success: function(data) {
						if ( data.error	)
						{
							self.loading(false);
							alert("RC=" + data.error.reason);
							return;
						}
		
						var mappedQueues = $.map(data.queues, function(item) { return new Queue(item);});
						self.queues(mappedQueues);
						self.loading(false);
					},
					error: function	(request,	status,	error)
					{
						self.loading(false);
						//TODO: $("#localQueues").html("An error occurred	while	retrieving local queues: " + status	+	", " + error);
					}
				});
			};
	}
	
	var XmitQueueModel = function()
	{
		var self = this;
		
		self.loading = ko.observable(false);
		self.queues = ko.observableArray([]);
		
		this.load = function()
			{
				$.ajax(
				{
					beforeSend:	function() {
						self.queues.removeAll();
						self.loading(true);
					},
					url: "/queue/list.json/<?= mqweb.qmgr ?>",
					data : {
						queueDepth : 1,
						queueExcludeSystem: 0,
						queueUsage : "xmitq"
					},
					cache: false,
					dataType:	"json",
					success: function(data) {
						if ( data.error	)
						{
							alert("RC=" + data.error.reason);
							return;
						}
		
						var mappedQueues = $.map(data.queues, function(item) { return new Queue(item);});
						self.queues(mappedQueues);
						self.loading(false);
					},
					error: function	(request,	status,	error)
					{
						self.loading(false);
						//TODO: $("#localQueues").html("An error occurred	while	retrieving local queues: " + status	+	", " + error);
					}
				});
			};
	}

	function Channel(data)
	{
		this.name = data.ChannelName.value;
		this.status = data.ChannelStatus.display;
		
		if ( this.status == 'Retrying' )
		{
			this.statusImage = '<img class="tip" src="/static/images/flag-red-icon.png" alt="The channel ' + this.name + ' has status Retrying." />';
		}
		else if ( this.status == 'Stopped' )
		{
			this.statusImage = '<img class="tip" src="/static/images/flag-black-icon.png" alt="The channel ' + this.name + ' has status Stopped." />';
		}
		else if ( this.status == 'Running' )
		{
			this.statusImage = '<img class="tip" src="/static/images/flag-green-icon.png" alt="The channel ' + this.name + ' has status Running." />';
		}
		else
		{
			this.statusImage = '<img class="tip" src="/static/images/flag-yellow-icon.png" alt="The channel ' + this.name + ' has status ' + this.status + '." />';
		}
	}

	var ChannelModel = function()
	{
		var self = this;
		
		self.loading = ko.observable(false);
		self.channels = ko.observableArray([]);

		self.load = function()
		{
			$.ajax(
			{
				beforeSend:	function() {
					self.channels.removeAll();
					self.loading(true);
				},
				url: "/chs/list.json/<?= mqweb.qmgr ?>",
				cache: false,
				dataType:	"json",
				success: function(data)
				{
					if ( data.error	)
					{
						alert("RC=" + data.error.reason);
						return;
					}
	
					var mappedChannels = $.map(data.statuses, function(item) { return new Channel(item);});
					self.channels(mappedChannels);
					self.loading(false);
					
					$(".tip").qtip({ 
						content : { 
							attr : 'alt' 
						}
					});
					
				},
				error: function	(request,	status,	error)
				{
					self.loading(false);
					//TODO: $("#channelStatus").html("An error occurred	while	retrieving channel status: " + status	+	", " + error);
				}
			});
			$(".tip").qtip({ 
				content : { 
					attr : 'alt'
				}
			});
		}
	}

	function Event(data)
	{
		this.putDate = data.putDate;
		this.reason = data.reason;
		this.desc = data.desc;
	}
	
	function loadEvents(vm)
	{
		$.ajax(
		{
			beforeSend:	function() 
			{
				vm.events(null);
				vm.error(null);
				vm.loading(true);
			},
			url: "/message/event.json/<?=	mqweb.qmgr	?>/SYSTEM.ADMIN.QMGR.EVENT?limit=3",
			cache: false,
			dataType:	"json",
			success: function(data)
			{
				vm.loading(false);

				if ( data.error )
				{
					vm.error(data.error);
					vm.count(0);
					vm.curdepth(0);
				}
				else
				{
					vm.events(data.events);
					vm.count(data.events.length);
					vm.curdepth(data.queues[0].CurrentQDepth.value);
				}
			},
			error: function	(request,	status,	error)
			{
				vm.loading(false);
				//TODO:$("#eventMessages").html("An error occurred	while	retrieving event messages from SYSTEM.ADMIN.QMGR.EVENT: " + status	+	", " + error);
			}
		});
	}
	
	var EventMessageModel = function()
	{
		var self = this;

		self.loading = ko.observable(false);
		self.events = ko.observable(null);
		self.error = ko.observable(null);
		self.count = ko.observable(0);
		self.curdepth = ko.observable(0);
		self.partialView = ko.computed(function() { return (!self.loading()) && self.count() < self.curdepth(); }, self);
	}

	var viewModel = {
		localQueueModel : new LocalQueueModel(),
		xmitQueueModel : new XmitQueueModel(),
		channelModel : new ChannelModel(),
		eventMessageModel : new EventMessageModel()
	};

	$(document).ready(function()
	{
	  $(".tip").qtip({ 
	  	content : { 
	  		attr : 'alt' 
	  	}
	  });
	  
		ko.applyBindings(viewModel);

		viewModel.localQueueModel.load();
		viewModel.xmitQueueModel.load();
		viewModel.channelModel.load();
		loadEvents(viewModel.eventMessageModel);
	});
	</script>
</head>
<body>
	<script type="text/html" id="mqErrorTemplate">    
		<div style="margin-top:5px;padding:10px;border:1px solid red;background-color:#FF9999;">
			<img	style="float:left;display:block;"	src="/static/images/error.png" alt="error" />
			<div	style="float:left;margin-left:20px;">
				<strong>Object: </strong><span data-bind:"text: object" /><br	/>
				<strong>Function: </strong><span data-bind:"text: fn" /><br	/>
				<strong>Code: </strong><span data-bind:"text: code" /><br />
				<strong>Reason: </strong><span data-bind:"text: reason" />
			</div>
			<div	class="cl">	
		</div>
	</script>
	<script type="text/html" id="mqEvents">
		<table style="border-spacing:0;border-collapse:collapse">
			<thead>
				<tr>
					<th style="text-align:left;">Date</th>
					<th style="text-align:left;" colspan="2">Reason</th>
				</tr>
			</thead>
			<tbody data-bind="foreach: $data">
				<tr>
					<td data-bind="text: putDate" />
					<td data-bind="text: reason" />
					<td data-bind="text: desc" />
				</tr>
			</tbody>
		</table>
	</script>
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
						<li><a title="Channels" href="/channel/index/<?=  mqweb.qmgr  ?>">Channels</a></li>
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
							<div id="localQueues">
								<img class="tip" src="/static/images/tip-icon.png" style="float:right;padding-top:8px;" alt="This is a list	with local queues	that contains	at least one message." />
								<a href="#" onclick="viewModel.localQueueModel.load();return false;"><img class="tip" src="/static/images/view-refresh-icon.png" style="float:right;padding-top:8px;padding-right:5px;" alt="Reload" /></a>
								<h3>Local	Queues</h3>
								<table data-bind="visible: localQueueModel.queues().length > 0" style="font-size:0.8em;border-collapse:collapse;display:none">
									<thead>
										<tr><th>Queue</th><th>Depth</th></tr>
									</thead>
									<tbody data-bind="foreach: localQueueModel.queues">
										<tr>
											<td><a data-bind="attr: {href: url}, text: name" />
											<td data-bind="text: curdepth" />
										</tr>
									</tbody>
								</table>
								<div class="loader" data-bind="visible: localQueueModel.loading"></div>
							</div> <!-- localQueues -->
						</div> <!-- activities -->
						<div class="col	activities">
							<div id="xmitQueues">
								<img class="tip" src="/static/images/tip-icon.png" style="float:right;padding-top:8px;" alt="This	is a list	with transmission	queues that	contains at	least	one	message. On	a	healthy	system,	this list	would	be empty ..." />
								<a href="#" onclick="viewModel.xmitQueueModel.load();return false;"><img class="tip" src="/static/images/view-refresh-icon.png" style="float:right;padding-top:8px;padding-right:5px;" alt="Reload" /></a>
								<h3>Transmission Queues</h3>
								<table data-bind="visible: xmitQueueModel.queues().length > 0" style="font-size:0.8em;border-collapse:collapse;display:none">
									<thead>
										<tr><th>Queue</th><th>Depth</th></tr>
									</thead>
									<tbody data-bind="foreach: xmitQueueModel.queues">
										<tr>
											<td data-bind="text: name" />
											<td data-bind="text: curdepth" />
										</tr>
									</tbody>
								</table>
								<div class="loader" data-bind="visible: xmitQueueModel.loading"></div>
							</div> <!-- xmitQueues -->
						</div> <!-- activities -->
						<div class="col	activities">
							<img class="tip" src="/static/images/tip-icon.png" style="float:right;padding-top:8px;" alt="This	is a list	with channel statuses." />
							<a title="Reload" href="#" onclick="viewModel.channelModel.load();return false;"><img class="tip" src="/static/images/view-refresh-icon.png" style="float:right;padding-top:8px;padding-right:5px;" alt="Reload" /></a>
							<h3>Channel	Status</h3>
							<div id="channels">
								<table data-bind="visible: channelModel.channels().length > 0" style="font-size:0.8em;border-collapse:collapse;display:none">
									<thead>
										<tr><th colspan="2">Channel</th><th>Status</th></tr>
									</thead>
									<tbody data-bind="foreach:channelModel.channels">
										<tr>
											<td data-bind="html: statusImage" />
											<td data-bind="text: name" />
											<td data-bind="text: status" />
										</tr>
									</tbody>
								</table>
								<div class="loader" data-bind="visible: channelModel.loading"></div>
							</div> <!-- channels -->
						</div> <!-- activities -->
						<div class="cl"></div>
					</div>
					<div class="widgets">
						<div class="projects">
							<a title="Reload" href="#" onclick="loadEvents(viewModel.eventMessageModel);return false;"><img class="tip" src="/static/images/view-refresh-icon.png" style="float:right;padding-top:8px;padding-right:5px;" alt="Reload" /></a>
							<h3>SYSTEM.ADMIN.QMGR.EVENT</h3>
							<div id="eventMessages">
								<div data-bind="if: eventMessageModel.events">
									<table data-bind="visible: true" style="border-spacing:0;border-collapse:collapse;display:none">
										<thead>
											<tr>
												<th style="text-align:left;">Date</th>
												<th style="text-align:left;" colspan="2">Reason</th>
											</tr>
										</thead>
										<tbody data-bind="foreach: eventMessageModel.events">
											<tr>
												<td data-bind="text: putDate" />
												<td data-bind="text: reason" />
												<td data-bind="text: desc" />
											</tr>
										</tbody>
									</table>
								</div>
								<p data-bind="visible: eventMessageModel.partialView" style="margin-top:5px;display:none">
									Number of messages on this queue: <strong><span data-bind="text: eventMessageModel.curdepth" /></strong><br />
									Only the first <strong><span data-bind="text: eventMessageModel.count"></strong> messages are shown. Use queue detail page to browse all messages.
								</p>
								<div class="loader" data-bind="visible: eventMessageModel.loading"></div>
								<div data-bind="if: eventMessageModel.error">
									<div data-bind="template: { name: 'mqErrorTemplate', data: eventMessageModel.error }">
									</div>
								</div>
							</div> <!-- eventMessages -->
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
