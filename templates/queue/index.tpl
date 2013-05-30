<!-- Content -->
<script type="text/html" id="queueDetail">
	<div style="float:right;">
		<a href="#" data-bind="click:$root.reload"><img class="tip" src="/static/images/view-refresh-icon.png" style="float:right;padding-top:8px;padding-right:5px;" alt="Reload" /></a>
	</div>
	<div class="details" style="width:70%;margin:auto">
		<h2>Name:</h2>
		<div class="detail" data-bind="text:QName.value" />
		<h2>Type:</h2>
		<div class="detail" data-bind="text:QType.display" />
		<div data-bind="if: $data.CurrentQDepth">
			<h2>Depth:</h2>
			<div class="detail">
				<span data-bind="text:CurrentQDepth.value" />  / <span data-bind="text:MaxQDepth.value" />
			</div> <!-- detail -->
		</div>
		<div data-bind="if: !$data.CurrentQDepth && $data.MaxQDepth">
			<h2>Depth:</h2>
			<div class="detail">
				 - / <span data-bind="text:MaxQDepth.value" />
			</div> <!-- detail -->
		</div>
		<div data-bind="if: $data.OpenInputCount">
			<h2>MQOPEN:</h2>
			<div class="detail">
				<span data-bind="text:OpenInputCount.value" /> for Input, <span data-bind="text:OpenOutputCount.value" /> for Output
			</div> <!-- detail -->
		</div>
		<div data-bind="if: $data.InhibitGet">
			<h2>Operations:</h2>
			<div class="detail">
				Get <span data-bind="text:InhibitGet.display" /> - Put <span data-bind="text:InhibitPut.display" />
			</div> <!-- detail -->
		</div>
		<div data-bind="if: $data.CreationDate">
			<h2>Creation Date:</h2>
			<div class="detail">
				<span data-bind="text:CreationDate.value" /> <span data-bind="text:CreationTime.value" />
			</div> <!-- detail -->
		</div>
		<div data-bind="if: $data.AlterationDate">
			<h2>Alteration Date:</h2>
			<div class="detail">
				<span data-bind="text:AlterationDate.value" /> <span data-bind="text: AlterationTime.value" />
			</div> <!-- detail -->
		</div>
		<div data-bind="if: $data.BackoutRequeueName && $data.BackoutRequeueName.value">
			<h2>Backout:</h2>
			<div class="detail">
				<a href="/queue/view/<?= mqweb.qmgr ?>/<?= queue.BackoutRequeueName.value ?>" data-bind="text: BackoutRequeueName.value" /> - Threshold: <span data-bind="text: BackoutThreshold.value" /> - Harden: <span data-bind="text:HardenGetBackout.display" />
			</div> <!-- detail -->
		</div>
		<div data-bind="if: $data.BaseQName">
			<h2>Target Queue:</h2>
			<div class="detail">
				<a href="/queue/view/<?= mqweb.qmgr ?>/<?= queue.BaseQName.value ?>" data-bind="text: BaseQName.value" />
			</div> <!-- detail -->
		</div>
		<div data-bind="if: $data.DefBind">
			<h2>Default Binding:</h2>
			<div class="detail">
				<span data-bind="text:DefBind.display" />
			</div> <!-- detail -->
		</div>
		<div data-bind="if: $data.DefInputOpenOption">
			<h2>Default Input Open:</h2>
			<div class="detail">
				<span data-bind="text: DefInputOpenOption.display" />
			</div> <!-- detail -->
		</div>
		<div data-bind="if: $data.DistLists">
			<h2>Distribution List:</h2>
			<div class="detail">
				<span data-bind="text:DistLists.display" />
			</div> <!-- detail -->
		</div>
		<div data-bind="if: $data.DefPersistence">
			<h2>Default Persistence:</h2>
			<div class="detail">
				<span data-bind="text:DefPersistence.display" />
			</div> <!-- detail -->
		</div>
		<div data-bind="if: $data.DefPriority">
			<h2>Default Priority:</h2>
			<div class="detail">
				<span data-bind="text:DefPriority.value" />
			</div> <!-- detail -->
		</div>
		<div data-bind="if: $data.DefReadAhead">
			<h2>Default Read Ahead:</h2><div class="detail"><span data-bind="text:DefReadAhead.display" /></div> <!-- detail -->
		</div>
		<div data-bind="if: $data.DefaultPutResponse">
			<h2>Default Put Response Type:</h2><div class="detail"><span data-bind="text:DefaultPutResponse.display" /></div> <!-- detail -->
		</div>
		<div data-bind="if: $data.QDepthHighEven">
			<h2>Depth Hight Event:</h2><div class="detail"><span data-bind="text:QDepthHighEvent.display" /> - Limit: <span data-bind="text:QDepthHighLimit.value" /></div> <!-- detail -->
		</div>
		<div data-bind="if: $data.QDepthLowEvent">
			<h2>Depth Low Event:</h2><div class="detail"><span data-bind="text:QDepthLowEvent.display" /> - Limit: <span data-bind="text:QDepthLowLimit.value" /></div> <!-- detail -->
		</div>
		<div data-bind="if: $data.QDepthMaxEvent">
			<h2>Depth Max Event:</h2><div class="detail"><span data-bind="text:QDepthMaxEvent.display" /></div> <!-- detail -->
		</div>
		<div data-bind="if: $data.InitiationQName && $data.InitiationQName.value">
			<h2>Initiation Queue:</h2>
			<div class="detail">
				<a href="/queue/view/<?= mqweb.qmgr ?>/<?= queue.InitiationQName.value ?>" data-bind="text:InitiationQName.value" />
			</div> <!-- detail -->
		</div>
		<div data-bind="if: $data.MaxMsgLength">
			<h2>Maximum Message Length:</h2>
			<div class="detail">
				<span data-bind="text:MaxMsgLength.value" />
			</div> <!-- detail -->
		</div>
		<div data-bind="if: $data.MsgDeliverySequence">
			<h2>Message Delivery:</h2>
			<div class="detail">
				<span data-bind="text:MsgDeliverySequence.display" />
			</div> <!-- detail -->
		</div>
		<div data-bind="if: $data.ProcessName && $data.ProcessName.value">
			<h2>Process:</h2>
			<div class="detail">
				<span data-bind="text:ProcessName.value" />
			</div> <!-- detail -->
		</div>
		<div data-bind="if: $data.TriggerControl">
			<h2>Trigger:</h2>
			<div class="detail">
				<span data-bind="text:TriggerControl.display" /> - Type: <span data-bind="text:TriggerType.display" /><br />
				Depth: <span data-bind="text:TriggerDepth.value" /> - Message Priority: <span data-bind="text: TriggerMsgPriority.value" /><br />
				Data: <span data-bind="text:TriggerData.value" />
			</div> <!-- detail -->
		</div>
	</div> <!-- details -->
</script>
<div id="content">
	<div class="post">
		<h3><strong><?= mqweb.qmgr	?></strong>	-	Queues</h3>
		<form	id="queueFilterForm" action="/queue/list.json/<?=mqweb.qmgr?>" method="POST">
			<fieldset>
				<legend>&nbsp;Filter&nbsp;</legend>
				<div style="float:left;margin:5px;">
					<div style="margin:5px;">
						<div	style="margin:5px;">
							<label for="queueName">Queue Name: </label><input	type="text"	id="queueName" name="queueName"	value="*"	/>
						</div>
						<div	style="margin:5px;">
							<label	for="queueDepth">Queue Depth:	</label><input type="text" id="queueDepth" name="queueDepth" />
						</div>
						<div style="float:left;margin:5px;">
							<input	type="checkbox"	name="queueExcludeSystem"	value="1"	/> Exclude System	Queues<br	/>
							<input	type="checkbox"	name="queueExcludeTemp"	value="1"	/> Exclude Temporary Queues<br />
						</div>
					</div>
				</div>
				<fieldset	style="float:left;">
					<legend>&nbsp;Queue	Type&nbsp;</legend>
					<div style="float:left;margin:5px;">
						<input type="radio"	name="queueType" value="All" />All<br	/>
						<input type="radio"	name="queueType" value="Local" />Local<br	/>
						<input type="radio"	name="queueType" value="Alias" />Alias
					</div>
					<div style="float:left;margin:5px;">
						<input type="radio"	name="queueType" value="Cluster" />Cluster<br	/>
						<input type="radio"	name="queueType" value="Model" />Model<br	/>
						<input type="radio"	name="queueType" value="Remote"	/>Remote
					</div>
				</fieldset>
				<div style="clear:both;" />
				<div style="float:right;margin-right:10px;margin-top:-30px;">
					<input	type="submit"	value="Filter	&gt;&gt;"	id="filterSubmit" disabled="disabled"	/>
				</div>
			</fieldset>
		</form>
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
	<div class="loader" data-bind="visible: queuesModel.loading"></div>
	<div style="margin:10px;" data-bind="if: queuesModel.queues">
		<table style="width:100%;border-collapse:collapse">
			<thead>
				<tr>
					<th>Queue</th><th>Type</th><th>Depth</th><th>Input</th><th>Output</th><th>Get Enabled</th><th>Put Enabled</th><th>Usage</th>
				</tr>
			</thead>
			<tbody data-bind="foreach: { data: queuesModel.queues, as: 'queue'}">
				<tr>
					<td>
						<a data-bind="attr: { id: 'queueAnchor_' + queue.QName.value}, click: $parent.toggle"><span data-bind="text: queue.QName.value" /></a>
						<span style="padding:5px;font-size:0.8em" data-bind="if: queue.BaseQName">
							<br />Target: <a data-bind="attr: { href: '/queue/view/<?= mqweb.qmgr ?>/' + queue.BaseQName.value}, text: queue.BaseQName.value"></a>
						</span>
						<div style="font-size:0.8em" data-bind="if: queue.RemoteQName">
							<br />
							Qmgr: <span data-bind="text: queue.RemoteQmgrName.value" /> Queue: <span data-bind="text: queue.RemoteQName.value" /><br />
							Transmission Queue: <a data-bind="attr: { href: '/queue/view/<?= mqweb.qmgr ?>/' + queue.XmitQName.value}, text: queue.XmitQName.value"></a>
						</div>
						<div data-bind="if: queue.QDesc && queue.QDesc.value.length > 0">
							<em style="padding:5px;font-size:0.8em" data-bind="text: queue.QDesc.value"></em>
						</div>
					</td>
					<td data-bind="text: queue.QType.display" />
					<td style="text-align:right" data-bind="style: {backgroundColor: queue.CurrentQDepth && queue.CurrentQDepth.value > 0 ? '#FF9966' : ''}">
						<div data-bind="if: queue.CurrentQDepth">
							<span data-bind="text:queue.CurrentQDepth.value" />
						</div>
						<div data-bind="ifnot: queue.CurrentQDepth">
							-
						</div>
					</td>
					<td style="text-align:right">
						<div data-bind="if: queue.OpenInputCount">
							<span data-bind="text:queue.OpenInputCount.value" />
						</div>
						<div data-bind="ifnot: queue.OpenInputCount">
							-
						</div>
					</td>
					<td style="text-align:right">
						<div data-bind="if: queue.OpenOutputCount">
							<span data-bind="text:queue.OpenOutputCount.value" />
						</div>
						<div data-bind="ifnot: queue.OpenOutputCount">
							-
						</div>
					</td>
					<td style="text-align:center">
						<div data-bind="if: queue.InhibitGet">
							<img data-bind="visible: queue.InhibitGet.value == 0" src="/static/images/enabled.gif" />
							<img data-bind="visible: queue.InhibitGet.value == 1" src="/static/images/disabled.gif" />
						</div>
						<div data-bind="ifnot: queue.InhibitGet">
							-
						</div>
					</td>
					<td style="text-align:center">
						<div data-bind="if: queue.InhibitPut">
							<img data-bind="visible: queue.InhibitPut.value == 0" src="/static/images/enabled.gif" />
							<img data-bind="visible: queue.InhibitPut.value == 1" src="/static/images/disabled.gif" />
						</div>
						<div data-bind="ifnot: queue.InhibitPut">
							-
						</div>
					</td>
					<td>
						<span data-bind="if: queue.Usage"><span data-bind="text:queue.Usage.display"></span></span>
					</td>
				</tr>
				<tr data-bind="visible:queue.toggle" style="display:none">
					<td colspan="8">
						<div data-bind="template: { name: 'queueDetail', data: queue }">
						</div>
					</td>
				</tr>
			</tbody>
		</table>
	</div>
	<div data-bind="template: { if: queuesModel.error, name: 'mqErrorTemplate', data: queuesModel.error }">
	</div>
	<div class="cl"></div>
	<div class="col	contacts">
	</div>
	<div class="cl"></div>
</div>