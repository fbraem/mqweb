<div style="padding:5px;">
 <h2 id="eventTitle" style="margin-bottom:10px;">MQRC_UNKNOWN_OBJECT_NAME</h2>
 <p style="margin-bottom:10px;">On an MQOPEN or MQPUT1 call, the ObjectQMgrName field in the object descriptor MQOD is set to one of the following options. It is either:</p>
  <ul style="margin-left:30px;margin-bottom:10px">
   <li>Blank</li>
   <li>The name of the local queue manager</li>
   <li>The name of a local definition of a remote queue (a queue-manager alias) in which the RemoteQMgrName attribute is the name of the local queue manager.</li>
   </ul>
   <p style="margin-bottom:10px;">However, the ObjectName in the object descriptor is not recognized for the specified object type.
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
  <?ifexist event.QName.value ?>
   <h2>Queue Name:</h2>
   <div class="detail">
    <?= event.QName.value ?>
   </div>
  <?endif ?>
  <?ifexist event.ProcessName.value ?>
   <h2>Process Name:</h2>
   <div class="detail">
    <?= event.ProcessName.value ?>
   </div>
  <?endif ?>
  <?ifexist event.ObjectQMgrName.value ?>
   <h2>Object Queuemanager Name:</h2>
   <div class="detail">
    <?= event.ObjectQMgrName.value ?>
   </div>
  <?endif ?>
  <?ifexist event.TopicName.value ?>
   <h2>Topic Name:</h2>
   <div class="detail">
    <?= event.TopicName.value ?>
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