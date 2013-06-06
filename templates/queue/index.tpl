<!-- Content -->
<? include "queue.tpl" ?>
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
					<th>Queue</th><th>Type</th><th>Depth</th><th>Input</th><th>Output</th><th>Get Enabled</th><th>Put Enabled</th><th>Usage</th><th> </th>
				</tr>
			</thead>
			<tbody data-bind="foreach: { data: queuesModel.queues, as: 'queue'}">
				<tr>
					<td data-bind="style: { borderBottomWidth: queue.toggle() ? '0px' : '1px' }">
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
					<td data-bind="text: queue.QType.display, style: { borderBottomWidth: queue.toggle() ? '0px' : '1px' }" />
					<td style="text-align:right" data-bind="style: {backgroundColor: queue.CurrentQDepth && queue.CurrentQDepth.value > 0 ? '#FF9966' : ''}">
						<div data-bind="if: queue.CurrentQDepth">
							<span data-bind="text:queue.CurrentQDepth.value" />
						</div>
						<div data-bind="ifnot: queue.CurrentQDepth">
							-
						</div>
					</td>
					<td style="text-align:right" data-bind="style: { borderBottomWidth: queue.toggle() ? '0px' : '1px' }">
						<div data-bind="if: queue.OpenInputCount">
							<span data-bind="text:queue.OpenInputCount.value" />
						</div>
						<div data-bind="ifnot: queue.OpenInputCount">
							-
						</div>
					</td>
					<td style="text-align:right" data-bind="style: { borderBottomWidth: queue.toggle() ? '0px' : '1px' }">
						<div data-bind="if: queue.OpenOutputCount">
							<span data-bind="text:queue.OpenOutputCount.value" />
						</div>
						<div data-bind="ifnot: queue.OpenOutputCount">
							-
						</div>
					</td>
					<td style="text-align:center" data-bind="style: { borderBottomWidth: queue.toggle() ? '0px' : '1px' }">
						<div data-bind="if: queue.InhibitGet">
							<img data-bind="visible: queue.InhibitGet.value == 0" src="/static/images/ok-icon.png" />
							<img data-bind="visible: queue.InhibitGet.value == 1" src="/static/images/not-ok-icon.png" />
						</div>
						<div data-bind="ifnot: queue.InhibitGet">
							-
						</div>
					</td>
					<td style="text-align:center" data-bind="style: { borderBottomWidth: queue.toggle() ? '0px' : '1px' }">
						<div data-bind="if: queue.InhibitPut">
							<img data-bind="visible: queue.InhibitPut.value == 0" src="/static/images/ok-icon.png" />
							<img data-bind="visible: queue.InhibitPut.value == 1" src="/static/images/not-ok-icon.png" />
						</div>
						<div data-bind="ifnot: queue.InhibitPut">
							-
						</div>
					</td>
					<td data-bind="style: { borderBottomWidth: queue.toggle() ? '0px' : '1px' }">
						<span data-bind="if: queue.Usage"><span data-bind="text:queue.Usage.display"></span></span>
					</td>
					<td>
						<a data-bind="attr: { href: '/queue/view/<?= mqweb.qmgr ?>/' + queue.QName.value }"><img src="/static/images/go-next-icon.png" /></a>
					</td>
				</tr>
				<tr data-bind="visible:queue.toggle" style="display:none">
					<td colspan="9">
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