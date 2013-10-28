<script src="/static/js/number_format.js" type="text/javascript" charset="utf-8"></script>
<script type="text/html" id="qmgrDetail">
	<p>
		<em><span data-bind="text: QMgrDesc.value" /></em>
	</p>
	<!-- DETAILS -->
	<div class="details" style="float:left;width:70%">
		<div>
			<h2>QueueManager ID</h2>
			<div class="detail" data-bind="text: QMgrIdentifier.value"> </div>
		</div>
		<div data-bind="if:$data.CreationDate">
			<h2>Creation Date</h2>
			<div class="detail">
				<span data-bind="text: CreationDate.value"> </span>&nbsp;<span data-bind="text: CreationTime.value"> </span>
			</div>
		</div>
		<div data-bind="if:$data.AlterationDate">
			<h2>Alteration Date</h2>
			<div class="detail">
				<span data-bind="text: AlterationDate.value" /> </span>&nbsp;<span data-bind="text: AlterationTime.value"> </span>
			</div>
		</div>
		<div data-bind="if:$data.Platform">
			<h2>Platform</h2>
			<div class="detail">
				<span data-bind="text: Platform.display" />
			</div>
		</div>
		<div data-bind="if:$data.CodedCharSetID">
			<h2>Coded Characterset ID</h2>
			<div class="detail">
				<span data-bind="text: CodedCharSetID.value" />
			</div>
		</div>
		<div data-bind="if:$data.DeadLetterQName && $data.DeadLetterQName.value">
			<h2>Dead Letter Queue</h2>
			<div class="detail">
			  <a data-bind="attr: {href: '/queue/view/<?=mqweb.qmgr?>/' + DeadLetterQName.value}, text:DeadLetterQName.value" />
			</div>
		</div>
		<div data-bind="ifnot:$data.DeadLetterQName.value">
			<h2>Dead Letter Queue</h2>
			<div class="detail">
				<img style="float:left;" src="/static/images/warning-icon.png" alt="error" />
				<em style="color:#FF6666">There is no dead letter queue defined. It's recommended to have one!</em>
			</div>
		</div>
	</div>
	<!-- END DETAILS -->
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
<div id="content">
	<div class="post">
		<h3>QueueManager <strong><?= mqweb.qmgr ?></strong></h3>
		<!-- ko with: qmgrModel.mqweb -->
	   <img data-bind="visible: !zos" src="/static/css/images/briefcase.jpg" alt="briefcase" />
	   <img data-bind="visible: zos" src="/static/images/zos-icon.png" alt="z/OS" title="z/OS" />
	  <!-- /ko -->
		<div data-bind="if: qmgrModel.qmgr">
			<div data-bind="template: { name: 'qmgrDetail', data: qmgrModel.qmgr }">
			</div>
		</div>
		<div class="loader" data-bind="visible: qmgrModel.loading"> </div>
		<div data-bind="with: qmgrModel.mqweb" style="font-size:0.7em;clear:both;">
		 It took <span data-bind="text: number_format(elapsed, 2, ',', '.')"> </span> seconds to create this output.
	  </div>
	</div>
</div> <!-- content -->
<!-- Sidebar -->
<div id="sidebar">
	<div class="box">
		<h3>Our Services</h3>
		<ul>
			<li><a title="List Queues" href="/queue/index/<?= mqweb.qmgr ?>">List Queues</a></li>
			<li><a title="List Channels" href="/channel/index/<?= mqweb.qmgr ?>">List Channels</a></li>
		</ul>
		<h3 style="margin-top:15px">WebSphere MQ</h3>
		<ul>
		 <li><a title="Information Center" target="_blank" href="http://pic.dhe.ibm.com/infocenter/wmqv7/v7r1/index.jsp">Information Center</a></li>
		 <li><a title="MQSeries.net" target="_blank" href="http://www.mqseries.net/phpBB2">MQSeries.net Forum</a></li>
		</ul>
	</div>
</div>
<!-- END Sidebar -->
<div class="cl"></div>
<div class="widgets">
	<div class="col activities">
		<div id="localQueues">
			<a href="#" onclick="viewModel.localQueueModel.load();return false;"><img class="imgtip" src="/static/images/view-refresh-icon.png" style="float:right;padding-top:8px;padding-right:5px;" alt="Reload" /></a>
			<h3 qtip-content="This is a list with local queues that contains at least one message.<br /><br />System queues and transmission queues are omitted.">Local Queues</h3>
			<table data-bind="if:localQueueModel.queues" style="font-size:0.8em;border-collapse:collapse;">
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
			<p data-bind="ifnot:localQueueModel.queues">
				There are no local queues which contains messages.
				System queues were discarded in this view.
			</p>
			<div class="loader" data-bind="visible: localQueueModel.loading"></div>
		</div> <!-- localQueues -->
	</div> <!-- activities -->
	<div class="col activities">
		<div id="xmitQueues">
			<a href="#" onclick="viewModel.xmitQueueModel.load();return false;"><img class="imgtip" src="/static/images/view-refresh-icon.png" style="float:right;padding-top:8px;padding-right:5px;" alt="Reload" /></a>
			<h3 qtip-content="This is a list with transmission queues (USAGE=XMITQ) that contains at least one message.<br /><br />Normally, this list is empty when no channels has problems.">Transmission Queues</h3>
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
			<p data-bind="ifnot:xmitQueueModel.queues">
				All transmission queues are empty.
			</p>
			<div class="loader" data-bind="visible: xmitQueueModel.loading"></div>
		</div> <!-- xmitQueues -->
	</div> <!-- activities -->
	<div class="col activities">
		<a href="#" onclick="viewModel.channelModel.load();return false;"><img class="imgtip" src="/static/images/view-refresh-icon.png" style="float:right;padding-top:8px;padding-right:5px;" alt="Reload" /></a>
		<h3 qtip-content="This is a list with active channel statuses.">Channel Status</h3>
		<div id="channels">
			<table data-bind="if: channelModel.channels" style="font-size:0.8em;border-collapse:collapse;">
				<thead>
					<tr><th colspan="2">Channel</th><th>Status</th></tr>
				</thead>
				<tbody data-bind="foreach:channelModel.channels">
					<tr>
						<td data-bind="template: { name: statusImage }" />
						<td><a data-bind="attr: {href: '/channel/view/<?= mqweb.qmgr ?>/' + ChannelName.value + '/' + ChannelType.display}, text: ChannelName.value" /></td>
						<td data-bind="text: ChannelStatus.display" />
					</tr>
				</tbody>
			</table>
			<div data-bind="visible: !channelModel.loading">
			 <p data-bind="ifnot:channelModel.channels">
				 No channel status found.
			 </p>
			</div>
			<div class="loader" data-bind="visible: channelModel.loading"></div>
		</div> <!-- channels -->
	</div> <!-- activities -->
	<div class="cl"></div>
</div>
<div class="widgets">
	<div class="projects">
		<a href="#" onclick="viewModel.eventMessageModel.load();return false;"><img class="imgtip" src="/static/images/view-refresh-icon.png" style="float:right;padding-top:8px;padding-right:5px;" alt="Reload" /></a>
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
				<p data-bind="if:eventMessageModel.count == 0">
					No event messages found.
				</p>
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
			<li><a title="Issues/Bugs" href="https://github.com/fbraem/mqweb/issues">Issue / Bug Reports</a></li>
		</ul>
	</div>
	<div class="cl"></div>
</div>
