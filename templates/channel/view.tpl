<!-- Content -->
<pre><code data-bind="html:ko.toJSON($data.channel, null, 2)"> </code></pre>
<div id="content">
	<div class="post">
		<h3><strong><?= mqweb.qmgr	?></strong>	- Channel - <?= channelName ?></h3>
	</div>
	<div data-bind="if: channelModel.channel">
		<div data-bind="template: { name: 'channelDetail', data: channelModel.channel }">
		</div>
	</div>
	<div class="loader" data-bind="visible: channelModel.loading"></div>
	<div data-bind="template: { if: channelModel.error, name: 'mqErrorTemplate', data: channelModel.error }">
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
	<div class="cl"></div>
	<div class="col	contacts">
	</div>
	<div class="cl"></div>
</div>