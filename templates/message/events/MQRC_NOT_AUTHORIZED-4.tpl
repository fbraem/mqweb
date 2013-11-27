<div style="padding:5px;">
 <h2 id="eventTitle" style="margin-bottom:10px;">MQRC_NOT_AUTHORIZED</h2>
 <h3>Not Authorized (type 4)</h3>
 <p style="margin-bottom:10px;">
  Indicates that a command has been issued from a user ID that is not authorized to access the object specified in the command.
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
  <h2>Command</h2>
  <div class="detail">
   <?= event.Command.display ?>
  </div>
 </div>
</div>