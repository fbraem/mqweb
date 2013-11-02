<div style="padding:5px;">
 <h2 id="eventTitle" style="margin-bottom:10px;">MQRC_NOT_AUTHORIZED</h2>
 <h3>Not Authorized (type 3)</h3>
 <p style="margin-bottom:10px;">
  When closing a queue using the MQCLOSE call, the user is not authorized to delete the object, which is a permanent dynamic queue, and the Hobj parameter specified on the MQCLOSE call is not the handle returned by the MQOPEN call that created the queue.
  <br /><br />
  When closing a subscription using an MQCLOSE call, the user has requested that the subscription is removed using the MQCO_REMOVE_SUB option, but the user is not the creator of the subscription or does not have sub authority on the topic associated with the subscription. 
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
  <?ifexist event.TopicString ?>
   <h2>Topic:</h2>
   <div class="detail">
    <?= event.TopicString.value ?>
   </div>
  <?endif ?>
  <?ifexist event.AdminTopicNames ?>
   <h2>Topic Administration Objects:</h2>
   <div class="detail">
    <?= event.AdminTopicNames.value ?>
   </div>
  <?endif ?>
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