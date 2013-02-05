<h2>Queue Manager Active</h2>
<table>
	<tr>
		<th>Put DateTime</th><td><?= event.putDate ?></td>
	</tr>
	<tr>
		<th>Reason Code</th><td><?= reason.code ?> - <?= reason.desc ?></td>
	</tr>
	<tr>
		<th>Queue Manager</th><td><?= event.QMgrName.value ?></td>
	</tr>
</table>
<p>
This condition is detected when a queue manager becomes active.
</p>
