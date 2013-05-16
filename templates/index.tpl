<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html lang="en-US" xmlns="http://www.w3.org/1999/xhtml" dir="ltr">
<head>
	<title>MQWeb - <?= mqweb.qmgr ?></title>
	<meta http-equiv="Content-type" content="text/html; charset=utf-8" />
	<link rel="shortcut icon" href="/static/css/images/favicon.ico" />
	<link rel="stylesheet" href="/static/css/jquery.qtip.css" type="text/css" media="all" />
	<link rel="stylesheet" href="/static/css/style.css" type="text/css" media="all" />
	<script src="/static/js/jquery-1.9.1.min.js" type="text/javascript" charset="utf-8"></script>
	<script src="/static/js/jquery.formalize.legacy.min.js" type="text/javascript" charset="utf-8"></script>
	<script src="/static/js/jquery.qtip.js" type="text/javascript" charset="utf-8"></script>
	<script src="/static/js/knockout-2.2.1.js" type="text/javascript" charset="utf-8"></script>
	<script type="text/javascript">
	var QmgrModel = function()
	{
		var self = this;
		
		self.qmgr = ko.observable(null);
		self.loading = ko.observable(false);
		self.error = ko.observable(null);
		
		this.load = function()
			{
				$.ajax(
				{
					beforeSend: function() {
						self.qmgr(null);
						self.error(null);
						self.loading(true);
					},
					url: "/qmgr/view.json/<?= mqweb.qmgr ?>",
					cache: false,
					dataType: "json",
					success: function(data) {
						self.loading(false);
						if ( data.error )
						{
							self.error(data.error);
						}
						else
						{
							self.qmgr(data.qmgr);
						}
					},
					error: function (request, status, error)
					{
						self.loading(false);
						self.error(null);
						self.qmgr(null);
						//TODO: Handle Error on page
					}
				});
			};
	}
	
	var LocalQueueModel = function()
	{
		var self = this;
		
		self.queues = ko.observable(null);
		self.loading = ko.observable(false);
		self.error = ko.observable(null);
		
		this.load = function()
			{
				$.ajax(
				{
					beforeSend: function() {
						self.queues(null);
						self.error(null);
						self.loading(true);
					},
					url: "/queue/list.json/<?= mqweb.qmgr ?>",
					data : {
						queueDepth : 1,
						queueExcludeSystem: 1,
						queueUsage : "normal"
					},
					cache: false,
					dataType: "json",
					success: function(data) {
						self.loading(false);
						if ( data.error )
						{
							self.error(data.error);
						}
						else
						{
							for(q in data.queues)
							{
								data.queues[q].url = '/queue/view/<?=mqweb.qmgr?>/' + data.queues[q].QName.value;
							}
							self.queues(data.queues);
						}
					},
					error: function (request, status, error)
					{
						self.loading(false);
						self.error(null);
						self.queues(null);
						//TODO: $("#localQueues").html("An error occurred while retrieving local queues: " + status + ", " + error);
					}
				});
			};

	}
	
	var XmitQueueModel = function()
	{
		var self = this;
		
		self.loading = ko.observable(false);
		self.queues = ko.observable(null);
		self.error = ko.observable(null);
		
		this.load = function()
			{
				$.ajax(
				{
					beforeSend: function() {
						self.queues(null);
						self.error(null);
						self.loading(true);
					},
					url: "/queue/list.json/<?= mqweb.qmgr ?>",
					data : {
						queueDepth : 1,
						queueExcludeSystem: 0,
						queueUsage : "xmitq"
					},
					cache: false,
					dataType: "json",
					success: function(data) {
						self.loading(false);
						if ( data.error )
						{
							self.error(data.error);
						}
						else
						{
							for(q in data.queues)
							{
								data.queues[q].url = '/queue/view/<?=mqweb.qmgr?>/' + data.queues[q].QName.value;
							}
							self.queues(data.queues);
						}
					},
					error: function (request, status, error)
					{
						self.loading(false);
						//TODO: $("#localQueues").html("An error occurred while retrieving local queues: " + status + ", " + error);
					}
				});
			};
	}

	function statusImage(status)
	{
		if ( status.ChannelStatus.display == 'Retrying' )
		{
			return "mqChannelStatusRetryingFlag";
		}
		else if ( status.ChannelStatus.display == 'Stopped' )
		{
			return "mqChannelStatusStoppedFlag";
		}
		else if ( status.ChannelStatus.display == 'Running' )
		{
			return "mqChannelStatusRunningFlag";
		}
		return "mqChannelStatusOtherFlag";
	}

	var ChannelModel = function()
	{
		var self = this;
		
		self.loading = ko.observable(false);
		self.channels = ko.observable(null);
		self.error = ko.observable(null);
		
		self.load = function()
		{
			$.ajax(
			{
				beforeSend: function() {
					self.channels(null);
					self.error(null);
					self.loading(true);
				},
				url: "/chs/list.json/<?= mqweb.qmgr ?>",
				cache: false,
				dataType: "json",
				success: function(data)
				{
					self.loading(false);
					if ( data.error )
					{
						self.error(data.error);
					}
					else
					{ 
						self.channels(data.statuses);
						
						$(".tip").qtip({ 
							content : { 
								attr : 'alt' 
							}
						});
					}
				},
				error: function (request, status, error)
				{
					self.loading(false);
					//TODO: $("#channelStatus").html("An error occurred while retrieving channel status: " + status + ", " + error);
				}
			});
			$(".tip").qtip({ 
				content : { 
					attr : 'alt'
				}
			});
		}
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
		
		self.load = function()
		{
			$.ajax(
			{
				beforeSend: function() 
				{
					self.events(null);
					self.error(null);
					self.loading(true);
				},
				url: "/message/event.json/<?= mqweb.qmgr  ?>/SYSTEM.ADMIN.QMGR.EVENT?limit=3",
				cache: false,
				dataType: "json",
				success: function(data)
				{
					self.loading(false);
	
					if ( data.error )
					{
						self.error(data.error);
						self.count(0);
						self.curdepth(0);
					}
					else
					{
						self.events(data.events);
						self.count(data.events.length);
						self.curdepth(data.queues[0].CurrentQDepth.value);
					}
				},
				error: function (request, status, error)
				{
					vm.loading(false);
					//TODO:$("#eventMessages").html("An error occurred  while retrieving event messages from SYSTEM.ADMIN.QMGR.EVENT: " + status  + ", " + error);
				}
			});
		}
	}

	var viewModel = {
		qmgrModel : new QmgrModel(),
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

		viewModel.qmgrModel.load();
		viewModel.localQueueModel.load();
		viewModel.xmitQueueModel.load();
		viewModel.channelModel.load();
		viewModel.eventMessageModel.load();
	});
	</script>
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
	<script type="text/html" id="mqChannelStatusRetryingFlag">
		<img data-bind="attr: { alt : 'The channel ' + ChannelName.value + ' has status Retrying' }" class="tip" src="/static/images/flag-red-icon.png" />
	</script>
	<script type="text/html" id="mqChannelStatusStoppedFlag">
		<img data-bind="attr: { alt : 'The channel ' + ChannelName.value + ' has status Stopped' }" class="tip" src="/static/images/flag-black-icon.png" />
	</script>
	<script type="text/html" id="mqChannelStatusRunningFlag">
		<img data-bind="attr: { alt : 'The channel ' + ChannelName.value + ' has status Running' }" class="tip" src="/static/images/flag-green-icon.png" />
	</script>
	<script type="text/html" id="mqChannelStatusOtherFlag">
		<img data-bind="attr: { alt : 'The channel ' + ChannelName.value + ' has status ' + ChannelStatus.display }" class="tip" src="/static/images/flag-red-yellow.png" />
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
						<li><a title="Home" href="/qmgr/view/<?= mqweb.qmgr ?>">Home</a></li>
						<li><a title="Queues" href="/queue/index/<?= mqweb.qmgr ?>">Queues</a></li>
						<li><a title="Channels" href="/channel/index/<?= mqweb.qmgr ?>">Channels</a></li>
					</ul>
					<div class="cl"></div>
				</div>
				<!-- END NAVIGATION -->
				<!-- MAIN -->
				<div id="main">
					<div id="content">
						<div class="post">
							<h3>QueueManager <strong><?= mqweb.qmgr ?></strong></h3>
							<img src="/static/css/images/briefcase.jpg" alt="briefcase" />
							<div data-bind="with: qmgrModel.qmgr">
								<!-- <span data-bind="text: ko.toJSON($data)" /> -->
								<p>
									<em><span data-bind="text: QMgrDesc.value" /></em>
								</p>
								<!-- DETAILS -->
								<div class="details" style="float:left;">
									<div>
										<h2>QueueManager ID</h2>
										<div class="detail" data-bind="text: QMgrIdentifier.value"> </div>
									</div>
									<div>
										<h2>Creation Date</h2>
										<div class="detail">
											<span data-bind="text: CreationDate.value"> </span>&nbsp;<span data-bind="text: CreationTime.value"> </span>
										</div>
									</div>
									<div>
										<h2>Alteration Date</h2>
										<div class="detail">
											<span data-bind="text: AlterationDate.value" /> </span>&nbsp;<span data-bind="text: AlterationTime.value"> </span>
										</div>
									</div>
									<div>
										<h2>Dead Letter Queue</h2>
										<div class="detail">
											<a href="/queue/view/<?= mqweb.qmgr ?>/TODO" data-bind="text: DeadLetterQName.value"> </a>
										</div>
									</div>
								</div>
								<!-- END DETAILS -->
							</div>
							<div class="loader" data-bind="visible: qmgrModel.loading"> </div>
						</div>
					</div> <!-- content -->
					<!-- Sidebar -->
					<div id="sidebar">
						<div class="box">
							<h3>Our Services</h3>
							<ul>
								<li><a title="List Queues Containing Messages" href="#">List Non-empty Queues</a></li>
								<li><a title="Ipsum is not simply" href="#">Ipsum is not simply</a></li>
								<li><a title="Dom text roots" href="#">Dom text roots</a></li>
								<li><a title="Popular belief lorem" href="#">Popular belief lorem</a></li>
								<li><a title="Ipsum is not simply" href="#">Ipsum is not simply</a></li>
							</ul>
						</div>
					</div>
					<!-- END Sidebar -->
					<div class="cl"></div>
					<div class="widgets">
						<div class="col activities">
							<div id="localQueues">
								<img class="tip" src="/static/images/tip-icon.png" style="float:right;padding-top:8px;" alt="This is a list with local queues that contains at least one message." />
								<a href="#" onclick="viewModel.localQueueModel.load();return false;"><img class="tip" src="/static/images/view-refresh-icon.png" style="float:right;padding-top:8px;padding-right:5px;" alt="Reload" /></a>
								<h3>Local Queues</h3>
								<div data-bind="visible:localQueueModel.queues" style="display:none">
									<table data-bind="if:localQueueModel.queues && localQueueModel.queues.length > 0" style="font-size:0.8em;border-collapse:collapse;">
										<thead>
											<tr><th>Queue</th><th>Depth</th></tr>
										</thead>
										<tbody data-bind="foreach: localQueueModel.queues">
											<tr>
												<td><a data-bind="attr: {href: url}, text: QName.value" />
												<td data-bind="text: CurrentQDepth.value" />
											</tr>
										</tbody>
									</table>
									<p data-bind="if:localQueueModel.queues && localQueueModel.queues.length == 0">
										No queues found.
									</p>
								</div>
								<div class="loader" data-bind="visible: localQueueModel.loading"></div>
							</div> <!-- localQueues -->
						</div> <!-- activities -->
						<div class="col activities">
							<div id="xmitQueues">
								<img class="tip" src="/static/images/tip-icon.png" style="float:right;padding-top:8px;" alt="This is a list with transmission queues that contains at least one message. On a healthy system, this list would be empty ..." />
								<a href="#" onclick="viewModel.xmitQueueModel.load();return false;"><img class="tip" src="/static/images/view-refresh-icon.png" style="float:right;padding-top:8px;padding-right:5px;" alt="Reload" /></a>
								<h3>Transmission Queues</h3>
								<div data-bind="visible:xmitQueueModel.queues" style="display:none">
									<table data-bind="if: xmitQueueModel.queues" style="font-size:0.8em;border-collapse:collapse;">
										<thead>
											<tr><th>Queue</th><th>Depth</th></tr>
										</thead>
										<tbody data-bind="foreach: xmitQueueModel.queues">
											<tr>
												<td><a data-bind="attr: {href: url}, text: QName.value" />
												<td data-bind="text: CurrentQDepth.value" />
											</tr>
										</tbody>
									</table>
								</div>
								<div class="loader" data-bind="visible: xmitQueueModel.loading"></div>
							</div> <!-- xmitQueues -->
						</div> <!-- activities -->
						<div class="col activities">
							<img class="tip" src="/static/images/tip-icon.png" style="float:right;padding-top:8px;" alt="This is a list with channel statuses." />
							<a title="Reload" href="#" onclick="viewModel.channelModel.load();return false;"><img class="tip" src="/static/images/view-refresh-icon.png" style="float:right;padding-top:8px;padding-right:5px;" alt="Reload" /></a>
							<h3>Channel Status</h3>
							<div id="channels">
								<div data-bind="visible: channelModel.channels" style="display:none">
									<table data-bind="if: channelModel.channels" style="font-size:0.8em;border-collapse:collapse;">
										<thead>
											<tr><th colspan="2">Channel</th><th>Status</th></tr>
										</thead>
										<tbody data-bind="foreach:channelModel.channels">
											<tr>
												<td data-bind="template: { name: statusImage }" />
												<td data-bind="text: ChannelName.value" />
												<td data-bind="text: ChannelStatus.display" />
											</tr>
										</tbody>
									</table>
								</div>
								<div class="loader" data-bind="visible: channelModel.loading"></div>
							</div> <!-- channels -->
						</div> <!-- activities -->
						<div class="cl"></div>
					</div>
					<div class="widgets">
						<div class="projects">
							<a title="Reload" href="#" onclick="viewModel.eventMessageModel.load();return false;"><img class="tip" src="/static/images/view-refresh-icon.png" style="float:right;padding-top:8px;padding-right:5px;" alt="Reload" /></a>
							<h3>SYSTEM.ADMIN.QMGR.EVENT</h3>
							<div id="eventMessages">
								<div data-bind="visible: eventMessageModel.events" style="display:none">
									<table data-bind="if: eventMessageModel.events" style="border-spacing:0;border-collapse:collapse;">
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
									Only the first <strong><span data-bind="text: eventMessageModel.count"></strong> messages are shown. Use <a href="/queue/view/<?=mqweb.qmgr?>/SYSTEM.ADMIN.QMGR.EVENT">queue detail</a> page to browse all messages.
								</p>
								<div class="loader" data-bind="visible: eventMessageModel.loading"></div>
								<div>
									<div data-bind="template: { if: eventMessageModel.error, name: 'mqErrorTemplate', data: eventMessageModel.error }">
									</div>
								</div>
							</div> <!-- eventMessages -->
							<div class="cl"></div>
						</div>
						<div class="col contacts">
							<h3>Contact Us</h3>
							<ul class="box">
								<li><a title="Zumuta!" href="http://www.zumuta.be">My Website</a></li>
								<li><a title="Github Sourcecode Repository" href="https://github.com/fbraem/mqweb">Github Sourcecode Repository</a></li>
								<li><a title="Issues/Bugs" href="https://github.com/fbraem/mqweb/issues">Isssue / Bug Reports</a></li>
							</ul>
						</div>
						<div class="cl"></div>
					</div>
				</div>
				<!-- END Main -->
			</div>
			<!-- END Wrapper Middle -->
			<div id="wrapper-bottom"></div>
			<!-- Footer -->
			<div id="footer">
				<p class="copy">Copyright 2012<span>|</span><a href="http://www.zumuta.be">www.zumuta.be</a>. Design by <a title="CSS Free Templates with jQuery Slider" href="http://css-free-templates.com/">CSS-Free-Templates.com</a></p>
			<!--  <p id="bottom-nav"><a title="Home" href="#">Home</a><span>|</span><a title="About" href="#">About</a><span>|</span><a title="Services" href="#">Services</a><span>|</span><a title="Projects" href="#">Projects</a><span>|</span><a title="Solutions" href="#">Solutions</a><span>|</span><a title="Jobs" href="#">Jobs</a><span>|</span><a title="Blog" href="#"> Blog</a><span>|</span><a title="Contacts" href="#">Contacts</a></p> -->
				<div class="cl"></div>
			</div>
			<!-- END Footer -->
		</div>
	</div>
	<!-- END Wrapper -->
</body>
</html>
