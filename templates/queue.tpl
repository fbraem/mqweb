<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" >
 <head>
  <title>MQWeb :: <?= qmgr.name ?> :: <?= queue.name ?></title>
  <link href="/static/css/redmond/jquery-ui-1.8.5.custom.css" rel="stylesheet" type="text/css"/>
  <link href="/static/css/mqweb/mqweb.css" rel="stylesheet" type="text/css"/>
  <script src="/static/js/jquery-1.4.2.min.js" type="text/javascript"> </script>
  <script src="/static/js/jquery-ui-1.8.5.custom.min.js" type="text/javascript"> </script>
  <script src="/static/js/jquery.form.js" type="text/javascript"> </script>
  <script src="/static/js/fancybox/jquery.fancybox-1.3.4.pack.js" type="text/javascript"> </script>
  <script type="text/javascript">
   jQuery(document).ready(function()
   {
     jQuery("#tabs").tabs();
   });
  </script>
  <link rel="stylesheet" type="text/css" href="/static/js/fancybox/jquery.fancybox-1.3.4.css" media="screen" />
 </head>
 <body>
  <h1 style="width:80%;float:left;"><?= qmgr.name ?></h1>
  <div style="float:right;font-size:0.8em;">
   Name: <a href="/<?= qmgr.name ?>"><?= qmgr.name ?></a><br />
   Id: <?= qmgr.id ?><br />
   Created: <?= qmgr.createDate ?><br />
   Modified: <?= qmgr.alterDate ?>
  </div>
  <?if qmgr.description ?>
   <div style="clear:both;background-color:#fbec88;border:1px solid #fad42e;padding:5px;margin-bottom:5px">
    <?= qmgr.description ?>
   </div>
  <?endif ?>
    <div style="clear:both" />
  <div id="tabs">
   <ul>
    <li><a href="#queue"><?= queue.QName.value ?></a></li>
    <li><a href="#messages">Messages</a></li>
   </ul>
   <div id="queue">
    <?if queue.description ?>
     <em style="padding:5px;"><?= queue.QDesc.value ?></em>
    <?endif ?>
    <br />
    <div class="details" id="qmgrDetail">
     <?ifexist queue.DefinitionType ?>
      <h2>Type:</h2><div class="detail"><?= queue.QType.display ?> - <?= queue.DefinitionType.display ?> - <?= queue.Usage.display ?></div> <!-- detail -->
     <?else ?>
      <h2>Type:</h2><div class="detail"><?= queue.QType.display ?> <?= queue.Usage.display ?></div> <!-- detail -->
     <?endif ?>
     <?ifexist queue.CurrentQDepth ?>
      <h2>Depth:</h2><div class="detail"><?= queue.CurrentQDepth.value ?> / <?ifexist queue.MaxQDepth ?><?= queue.MaxQDepth.value ?><?else ?>-<?endif ?></div> <!-- detail -->
     <?else ?>
      <h2>Depth:</h2><div class="detail">- / <?ifexist queue.MaxQDepth ?><?= queue.MaxQDepth.value ?><?else ?>-<?endif ?></div> <!-- detail -->
     <?endif ?>
     <?ifexist queue.OpenInputCount ?>
      <h2>MQOPEN:</h2><div class="detail"><?= queue.OpenInputCount.value ?> for Input, <?= queue.OpenOutputCount.value ?> for Output</div> <!-- detail -->
     <?endif ?>
     <?ifexist queue.InhibitGet ?>
      <h2>Operations:</h2><div class="detail">Get <?= queue.InhibitGet.display ?> - Put <?= queue.InhibitPut.display ?></div> <!-- detail -->
     <?endif ?>
     <?ifexist queue.CreationDate ?>
      <h2>Creation Date:</h2><div class="detail"><?= queue.CreationDate.value ?> <?= queue.CreationTime.value ?></div> <!-- detail -->
     <?endif ?>
     <h2>Alteration Date:</h2><div class="detail"><?= queue.AlterationDate.value ?> <?= queue.AlterationTime.value ?></div> <!-- detail -->
     <?if queue.BackoutRequeueName.value ?>
      <h2>Backout:</h2>
      <div class="detail">
       <a href="/<?= qmgr.name ?>/Queues/<?= queue.BackoutRequeueName.value ?>"><?= queue.BackoutRequeueName.value ?></a> - Threshold: <?= queue.BackoutThreshold.value?> - Harden: <?= queue.HardenGetBackout.display ?>
      </div> <!-- detail -->
     <?endif ?>
     <?if queue.BaseQName ?>
      <h2>Target Queue:</h2><div class="detail"><a href="/<?= qmgr.name ?>/Queues/<?= queue.BaseQName.value ?>"><?= queue.BaseQName.value ?></a></div> <!-- detail -->
     <?endif ?>
     <h2>Default Binding:</h2><div class="detail"><?= queue.DefBind.display ?></div> <!-- detail -->
     <?ifexist queue.DefInputOpenOption ?>
      <h2>Default Input Open:</h2><div class="detail"><?= queue.DefInputOpenOption.display ?></div> <!-- detail -->
     <? endif ?>
     <?ifexist queue.DistLists ?>
      <h2>Distribution List:</h2><div class="detail"><?= queue.DistLists.display ?></div> <!-- detail -->
     <?endif ?>
     <?ifexist queue.DefPersistence ?>
      <h2>Default Persistence:</h2><div class="detail"><?= queue.DefPersistence.display ?></div> <!-- detail -->
     <?endif ?>
     <?ifexist queue.DefPriority ?>
      <h2>Default Priority:</h2><div class="detail"><?= queue.DefPriority.value ?></div> <!-- detail -->
     <?endif ?>
     <?ifexist queue.DefReadAhead ?>
      <h2>Default Read Ahead:</h2><div class="detail"><?= queue.DefReadAhead.display ?></div> <!-- detail -->
     <?endif ?>
     <?ifexist queue.DefaultPutResponse ?>
      <h2>Default Put Response Type:</h2><div class="detail"><?= queue.DefaultPutResponse.display ?></div> <!-- detail -->
     <?endif ?>
     <?ifexist queue.QDepthHighEvent ?>
      <h2>Depth Hight Event:</h2><div class="detail"><?= queue.QDepthHighEvent.display ?> - Limit: <?= queue.QDepthHighLimit.value ?></div> <!-- detail -->
     <?endif ?>
     <?ifexist queue.QDepthLowEvent ?>
      <h2>Depth Low Event:</h2><div class="detail"><?= queue.QDepthLowEvent.display ?> - Limit: <?= queue.QDepthLowLimit.value ?></div> <!-- detail -->
     <?endif ?>
     <?ifexist queue.QDepthMaxEvent ?>
      <h2>Depth Max Event:</h2><div class="detail"><?= queue.QDepthMaxEvent.display ?></div> <!-- detail -->
     <?endif ?>
     <?if queue.InitiationQName.value ?>
      <h2>Initiation Queue:</h2><div class="detail"><a href="/<?= qmgr.name ?>/Queues/<?= queue.InitiationQName.value ?>"><?= queue.InitiationQName.value ?></a></div> <!-- detail -->
     <?endif ?>
     <?ifexist queue.MaxMsgLength ?>
      <h2>Maximum Message Length:</h2><div class="detail"><?= queue.MaxMsgLength.value ?></div> <!-- detail -->
     <?endif ?>
     <?ifexist queue.MsgDeliverySequence ?>
      <h2>Message Delivery:</h2><div class="detail"><?= queue.MsgDeliverySequence.display ?></div> <!-- detail -->
     <?endif ?>
     <?if queue.ProcessName.value ?>
      <h2>Process:</h2><div class="detail"><?= queue.ProcessName.value ?></div> <!-- detail -->
     <?endif ?>
     <?ifexist queue.TriggerControl ?>
       <h2>Trigger:</h2>
       <div class="detail">
        <?= queue.TriggerControl.display ?> - Type: <?= queue.TriggerType.display ?> - Depth: <?= queue.TriggerDepth.value ?> - Message Priority: <?= queue.TriggerMsgPriority.value ?> - Data: <?= Queue.TriggerData.value ?>
       </div> <!-- detail -->
     <? endif ?>
    </div> <!-- details -->
   </div> <!-- queue -->
   <div id="messages">
	  <form id="messageFilterForm">
	   <fieldset>
	    <legend>&nbsp;Limits&nbsp;</legend>
	    <div style="float:left;">
	     <label for="limit">Limit Messages: </label>
	     <input type="text" id="limit" name="limit" value="" /> Max: <?= queue.curdepth ?> <br />
	     <label for="teaser">Length Teaser: </label>
	     <input type="text" id="teaser" name="teaser" value="" /> (Only for MQSTR messages)<br />
	     <input type="submit" value="Browse &gt;&gt;" name="Filter" />
	    </div>
	   </fieldset>
	  </form>
	  <div id="messageTableDiv" />
	  <script type="text/javascript">
	   var options = { 
	                   target: '#messageTableDiv',
	                   type: 'POST',
	                   url: '<?= queue.QName.value ?>/Messages'
	                 }; 
	   jQuery('#messageFilterForm').ajaxForm(options);
	  </script>
   </div> <!-- messages -->
  </div> <!-- tabs -->
 </body>
</html>

