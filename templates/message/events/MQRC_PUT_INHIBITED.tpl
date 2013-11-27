<div style="padding:5px;">
 <h2 id="eventTitle" style="margin-bottom:10px;">MQRC_PUT_INHIBITED</h2>
 <p style="margin-bottom:10px;">
  MQPUT and MQPUT1 calls are currently inhibited for the queue or topic (see the InhibitPut queue attribute in InhibitPut (MQLONG) or the InhibitPublications topic attribute in Topic attributes for the queue to which this queue resolves.
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
  <?ifexist event.QName ?>
   <h2>Queue Name:</h2>
   <div class="detail">
    <?= event.QName.value ?>
   </div>
  <?endif ?>
  <?ifexist event.ObjectQMgrName ?>
   <h2>Object Queuemanager Name:</h2>
   <div class="detail">
    <?= event.ObjectQMgrName.value ?>
   </div>
  <?endif ?>
  <?ifexist event.TopicString ?>
   <h2>Topic:</h2>
   <div class="detail">
    <?= event.TopicString.value ?>
   </div>
  <?endif ?>
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