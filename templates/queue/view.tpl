<!-- Content -->
<div id="content">
	<div class="post">
		<h3><strong><?= mqweb.qmgr	?></strong>	- Queue - <?= queueName ?></h3>
	</div>
	<div data-bind="if: queueModel.queue">
		<div data-bind="template: { name: 'queueDetail', data: queueModel.queue }">
		</div>
	</div>
	<div class="loader" data-bind="visible: queueModel.loading"></div>
	<div data-bind="template: { if: queueModel.error, name: 'mqErrorTemplate', data: queueModel.error }">
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