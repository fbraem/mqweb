<h2>Not Authorized (type 2)</h2>
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
	<tr>
		<th>Queue Name</th><td><?= event.QName.value ?></td>
	</tr>
	<tr>
		<th>User</th><td><?= event.UserIdentifier.value ?></td>
	</tr>
</table>
<p>
On an MQOPEN or MQPUT1 call, the user is not authorized to open the object for the options specified.
</p>
