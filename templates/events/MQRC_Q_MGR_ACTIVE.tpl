<h2>Queue Manager Active</h2>
<table>
	<tr>
		<td>Reason Code</td><td><?= reason.code ?> - <?= reason.desc ?></td>
		<td>Put DateTime</td><td><?= event.putDate ?></td>
		<td>Queue Manager</td><td><?= event.QMgrName.value ?></td>
	</tr>
</table>
<p>
This condition is detected when a queue manager becomes active.
</p>
