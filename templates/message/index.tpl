<!-- Content -->
<div id="content">
	<div class="post">
		<h3><strong><?= mqweb.qmgr ?></strong> - Messages - <?= mqweb.queue ?></h3>
		<form id="messageFilterForm" action="/message/list.json/<?=mqweb.qmgr?>/<?=mqweb.queue?>" method="POST">
			<fieldset style="padding:5px;">
				<legend>&nbsp;Limits&nbsp;</legend>
				<div style="margin:5px; width:100%">
					<label style="position: absolute; text-align:right; width:130px;" for="limit">Limit Messages</label>
					<input style="margin-left: 140px;" type="text" id="limit" name="limit" value="" />
				</div>
				<div style="margin:5px; width:100%">
					<label style="position: absolute; text-align:right; width:130px;" for="teaser">Teaser Length</label>
					<input style="margin-left: 140px;" type="text" id="teaser" name="teaser" value="80" /> <span style="font-size:0.8em">(Only for messages with format MQSTR)</span>
				</div>
				<div>
					<input type="submit" value="Filter &gt;&gt;" id="filterSubmit" disabled="disabled" />
				</div>
			</fieldset>
		</form>
	</div>
</div>
<!-- END Content -->
<!-- Sidebar -->
<div id="sidebar">
	<div class="box">
	<!--
		<h3>Our Services</h3>
		<ul>
			<li><a title="List Queues Containing Messages" href="#">List Non-empty Queues</a></li>
			<li><a title="Ipsum is not simply" href="#">Ipsum is not simply</a></li>
			<li><a title="Dom text roots" href="#">Dom text roots</a></li>
			<li><a title="Popular belief lorem" href="#">Popular belief lorem</a></li>
			<li><a title="Ipsum is not simply" href="#">Ipsum is not simply</a></li>
		</ul>
	-->
	</div>
</div>
<!-- END Sidebar -->
<div class="cl"></div>
<div class="widgets">
	<div class="result" id="result">
		<table id="messageTable" data-bind="if: messagesModel.messages" style="width:100%;border-collapse:collapse;font-size:0.85em;">
			<thead>
				<tr>
					<th>Put Date</th><th>Id</th><th>Format</th><th>Length</th><th>User</th><th>Application</th><th>Encoding</th><th>CCSID</th>
				</tr>
			</thead>
			<tbody data-bind="foreach: { data: messagesModel.messages, as: 'message'}">
				<tr>
					<td data-bind="text: message.PutDate" />
					<td data-bind="text: message.MsgId" />
					<td data-bind="text: message.Format" />
					<td style="text-align:right">
						<a data-bind="attr: { href: '/message/dump/<?= mqweb.qmgr ?>/<?= mqweb.queue ?>/' + message.MsgId }"><span data-bind="text: message.Length" /></a>
					</td>
					<td data-bind="text: message.UserIdentifier" />
					<td data-bind="text: message.PutApplName" />
					<td data-bind="text: message.Encoding" />
					<td data-bind="text: message.CodedCharSetId" />
				</tr>
				<tr data-bind="if: message.data">
					<td></td>
					<td colspan="7"><pre data-bind="text: message.data" /></td>
				</tr>
			</tbody>
		</table>
	</div>
	<div class="cl"></div>
	<div class="col contacts">
	</div>
	<div class="cl"></div>
</div>
