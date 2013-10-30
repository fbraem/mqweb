<div style="padding:5px;">
 <h2 style="margin-bottom:10px;">MQRC_Q_MGR_ACTIVE</h2>
 <p style="margin-bottom:10px;">
  This condition is detected when a queue manager becomes active.
 </p>
 <div class="details">
  <h2>Put DateTime:</h2>
  <div class="detail"><?= event.putDate ?></div>
  <h2>Reason Code:</h2>
  <div class="detail"><?= reason.code ?> - <?= reason.desc ?></div>
  <h2>Queue Manager:</h2>
  <div class="detail"><?= event.QMgrName.value ?></div>
 </div>
</div>