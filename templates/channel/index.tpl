<!-- content -->
<? include "channel.tpl" ?>
<!-- Content -->
<div id="content">
	<div class="post">
		<h3><strong><?= mqweb.qmgr  ?></strong> - Channels</h3>
		<form id="channelFilterForm"  action="/channel/list.json/<?=mqweb.qmgr?>" method="POST">
			<fieldset>
				<legend>&nbsp;Filter&nbsp;</legend>
					<div style="float:left;margin:5px;">
						<div	style="margin:5px;">
							<label for="channelName">Channel Name: </label><input	type="text"	id="channelName" name="channelName"	value="*"	/>
						</div>
						<div style="margin:5px;">
						 <input type="checkbox" name="channelExcludeSystem" value="1" /> Exclude System Channels
						</div>
					</div>
					<fieldset style="float:right;margin-right:10px;">
						<legend>&nbsp;Channel Type&nbsp;</legend>
					 	<div style="float:left;padding:10px;">
							<input type="radio" name="channelType" value="All" /> All<br />
							<input type="radio" name="channelType" value="Sender" /> Sender<br />
							<input type="radio" name="channelType" value="Server" /> Server<br />
							<input type="radio" name="channelType" value="Receiver" /> Receiver<br />
							<input type="radio" name="channelType" value="Requester" /> Requester<br />
						</div>
					 	<div style="float:left;padding:10px;">
							<input type="radio" name="channelType" value="Server-connection" /> Server-connection<br />
							<input type="radio" name="channelType" value="Client-connection" /> Client-connection<br />
							<input type="radio" name="channelType" value="Cluster-receiver" /> Cluster-receiver<br />
							<input type="radio" name="channelType" value="Cluster-sender" /> Cluster-sender<br />
							<input type="radio" name="channelType" value="Telemetry" /> Telemetry
				 		</div>
					</fieldset>
					<div style="clear:both;" />
					<div style="float:right;margin-right:10px;margin:10px;">
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
	<div class="loader" data-bind="visible: channelsModel.loading"></div>
	<div style="margin:10px;" data-bind="if: channelsModel.channels">
		<table style="width:100%;border-collapse:collapse">
			<thead>
				<tr>
					<th>Channel</th><th>Type</th><th> </th>
				</tr>
			</thead>
			<tbody data-bind="foreach: { data: channelsModel.channels, as: 'channel'}">
				<tr>
					<td data-bind="style: { borderBottomWidth: channel.toggle() ? '0px' : '1px' }">
						<a data-bind="attr: { id: 'channelAnchor_' + channel.ChannelName.value}, click: $parent.toggle"><span data-bind="text: channel.ChannelName.value" /></a>
						<div data-bind="if: channel.Desc && channel.Desc.value.length > 0">
							<em style="padding:5px;font-size:0.8em" data-bind="text: channel.Desc.value"></em>
						</div>
					</td>
					<td data-bind="text: channel.ChannelType.display, style: { borderBottomWidth: channel.toggle() ? '0px' : '1px' }" />
					<td>
						<a data-bind="attr: { href: '/channel/view/<?= mqweb.qmgr ?>/' + channel.ChannelName.value + '/' + channel.ChannelType.display }"><img src="/static/images/go-next-icon.png" /></a>
					</td>
				</tr>
				<tr data-bind="visible:channel.toggle" style="display:none">
					<td colspan="9">
						<div data-bind="template: { name: 'channelDetail', data: channel }">
						</div>
					</td>
				</tr>
			</tbody>
		</table>
	</div>
	<div data-bind="template: { if: channelsModel.error, name: 'mqErrorTemplate', data: channelsModel.error }">
	</div>
	<div class="cl"></div>
	<div class="col	contacts">
	</div>
	<div class="cl"></div>
</div>
