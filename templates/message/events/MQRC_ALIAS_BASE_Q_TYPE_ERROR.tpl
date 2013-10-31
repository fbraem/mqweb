<div style="padding:5px;">
 <h2 id="eventTitle" style="margin-bottom:10px;">MQRC_ALIAS_BASE_Q_TYPE_ERROR</h2>
 <p style="margin-bottom:10px;">
  An MQOPEN or MQPUT1 call was issued specifying an alias queue as the destination, but 
  the BaseObjectName in the alias queue definition resolves to a queue that is not a local 
  queue, or local definition of a remote queue. 
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
  <h2>Base Object Name:</h2>
  <div class="detail">
   <?= event.BaseObjectName.value ?>
  </div>
  <h2>Queue Type:</h2>
  <div class="detail">
   <?= event.QType.display ?>
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