<div style="padding:5px;">
 <h2 id="eventTitle" style="margin-bottom:10px;">MQRC_NOT_AUTHORIZED</h2>
 <h3>Not Authorized (type 1)</h3>
 <p style="margin-bottom:10px;">
  On an MQCONN or system connection call, the user is not authorized to connect to the queue manager.
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
  <h2>User</h2>
  <div class="detail">
   <?= event.UserIdentifier.value ?>
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