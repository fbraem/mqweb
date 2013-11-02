<div style="padding:5px;">
 <h2 id="eventTitle" style="margin-bottom:10px;">MQRC_NOT_AUTHORIZED</h2>
 <h3>Not Authorized (type 5)</h3>
 <p style="margin-bottom:10px;">
  On an MQSUB call, the user is not authorized to use the destination queue with the required level of access. This event is only returned for subscriptions using non-managed destination queues.
  When creating, altering, or resuming a subscription, and a handle to the destination queue is supplied on the request, the user does not have PUT authority on the destination queue provided.
  <br /><br />
  When resuming or alerting a subscription and the handle to the destination queue is to be returned on the MQSUB call, and the user does not have PUT, GET and BROWSE authority on the destination queue.
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
  <h2>Options:</h2>
  <div class="detail">
   <?= event.Options.value" ?>
  </div>
  <h2>Topic:</h2>
  <div class="detail">
   <?= event.TopicString.value ?>
  </div>
  <h2>Topic Administration Objects:</h2>
  <div class="detail">
   <?= event.AdminTopicNames.value ?>
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
  <?ifexist event.DestQMgrName ?>
   <h2>Destination Queuemanager:</h2>
   <div class="detail">
    <?= event.DestQMgrName.value ?>
   </div>
  <?endif?>
  <h2>Destination Queue Name:</h2>
  <div class="detail">
   <?= event.DestQName.value ?>
  </div>
  <h2>Destination Open Options:</h2>
  <div class="detail">
   <?= event.DestOpenOptions.value ?>
  </div>
 </div>
</div>