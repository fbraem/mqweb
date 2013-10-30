<!-- Content -->
<!-- <pre><code data-bind="html:ko.toJSON($data, null, 2)"> </code></pre> -->
<div id="content">
	<div class="post">
		<h3><strong><?= mqweb.qmgr	?></strong>	- Channel - <?= channelName ?></h3>
	</div>
	<div data-bind="if: viewModel.channelModel.channel">
		<div data-bind="template: { name: 'channelDetail', data: viewModel.channelModel.channel }">
		</div>
	</div>
	<div class="loader" data-bind="visible: channelModel.loading"></div>
	<div data-bind="template: { if: viewModel.channelModel.error, name: 'mqErrorTemplate', data: channelModel.error }">
	</div>
</div>
<!-- END Content -->
<!-- Sidebar -->
<div id="sidebar">
	<div class="box">
		<h3>Status</h3>
		<div data-bind="if:viewModel.statusModel.statuses">
		 <table style="width:100%;border-collapse:collapse">
		 	<thead>
		 	 <tr>
		 	  <th>Start Time</th><th>Status</th><th>Substate</th><th>Type</th>
		 	 </tr>
		 	</thead>
		  <tbody data-bind="foreach: { data: viewModel.statusModel.statuses, as: 'status'}">
		   <tr>
		    <td><span data-bind="text:status.ChannelStartDate.value" />&nbsp;<span data-bind="text:status.ChannelStartTime.value" /></td>
		    <td data-bind="text: status.ChannelStatus.display" />
		    <td data-bind="text: status.SubState.display" />
		    <td data-bind="text: status.ChannelInstanceType.display" />
		   </tr>
		  </tbody>
		 </table>
		</div>
	</div>
</div>
<!-- END Sidebar -->
<div class="cl"></div>
<div class="widgets">
	<div class="cl"></div>
	<div class="col	contacts">
	</div>
	<div class="cl"></div>
</div>