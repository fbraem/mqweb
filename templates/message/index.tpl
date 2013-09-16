<!-- Content -->
<? include "message.tpl" ?>
<div id="content">
	<div class="post">
		<h3><strong><?= mqweb.qmgr ?></strong> - Messages - <?= mqweb.queue ?></h3>
		<form id="messageFilterForm" action="/message/list.json/<?=mqweb.qmgr?>/<?=mqweb.queue?>" method="POST">
			<fieldset>
				<legend>&nbsp;Limits&nbsp;</legend>
				<div style="float:left;margin:5px;">
				<div style="float:left;margin:5px;">
					<label for="limit">Limit Messages: </label>
					<input type="text" id="limit" name="limit" value="" /> Max: <?= queue.curdepth ?> <br />
				</div>
				<div style="float:left;margin:5px;">
					<label for="teaser">Length Teaser: </label>
					<input type="text" id="teaser" name="teaser" value="" /> (Only for MQSTR messages)<br />
				</div>
				<div style="clear:both" />
					<div style="float:right;margin-right:10px;">
						<input	type="submit" value="Filter &gt;&gt;" id="filterSubmit" disabled="disabled" />
					</div>
				</div>
			</fieldset>
		</form>
	</div>
</div>
<!-- END Content -->
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
	<div class="result" id="result">
		<table id="messageTable" data-bind="if: messagesModel.messages" style="width:100%;border-collapse:collapse">
			<thead>
				<tr>
					<th>Put Date</th><th>Id</th><th>Format</th><th>Length</th><th>User</th><th>Application</th><th>Encoding</th><th>CCSID</th>
				</tr>
			</thead>
			<tbody data-bind="foreach: { data: messagesModel.messages, as: 'message'}">
				<tr>
					<td data-bind="text: message.PutDate" />
					<td>
						<a data-bind="click: $parent.toggle"><span data-bind="text: message.MsgId" /></a>
					</td>
					<td data-bind="text: message.Format" />
					<td style="text-align:right" data-bind="text:message.Length">
					<!--<a class="hex" href="/message/view/<?= mqweb.qmgr ?>/<?= mqweb.queue ?>/<?= message.MsgId ?>?type=hex" title="Hex View"><?= message.length ?></a>-->
					</td>
					<td data-bind="text: message.UserIdentifier" />
					<td data-bind="text: message.PutApplName" />
					<td data-bind="text: message.Encoding" />
					<td data-bind="text: message.CodedCharSetId" />
				</tr>
				<tr data-bind="visible:message.toggle" style="display:none">
					<td colspan="8">
						<div data-bind="template: { name: 'messageDetail', data: message }">
						</div>
					</td>
				</tr>
<!--
				<tr data-bind="if: message.data">
					<td></td>
					<td colspan="5" data-bind="text: message.data" />
				</tr>
-->
			</tbody>
		</table>
	</div>
	<div class="cl"></div>
	<div class="col contacts">
	</div>
	<div class="cl"></div>
</div>
