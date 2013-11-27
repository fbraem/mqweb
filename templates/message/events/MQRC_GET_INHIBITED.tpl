<div style="padding:5px;">
 <h2 id="eventTitle" style="margin-bottom:10px;">MQRC_GET_INHIBITED</h2>
 <p style="margin-bottom:10px;">
  MQGET calls are currently inhibited for the queue (see InhibitGet (MQLONG) for the InhibitGet queue attribute) or for the queue to which this queue resolves.
 </p>
 <div class="details">
  <h2>Put DateTime:</h2>
  <div class="detail"><?= message.PutDate ?></div>
  <h2>Reason Code:</h2>
  <div class="detail">
   <?= reason.code ?> - <?= reason.desc ?>
  </div>
  <h2>Queue Manager:</h2>
  <div class="detail">
   <?= event.QMgrName.value ?>
  </div>
  <h2>Queue Name:</h2>
  <div class="detail">
   <?= event.QName.value ?>
  </div>
  <h2>Application Type</h2>
  <div class="detail">
   <?= event.ApplType.display ?>
  </div>
  <h2>Application Name</h2>
  <div class="detail">
   <?= event.ApplName.value ?>
  </div>
 </div>
</div>