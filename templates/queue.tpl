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
    <li><a href="#queue"><?= queue.name ?></a></li>
    <li><a href="#messages">Messages</a></li>
   </ul>
   <div id="queue">
    <?if queue.description ?>
     <em style="padding:5px;"><?= queue.description ?></em>
    <?endif ?>
    <br />
    <div class="details" id="qmgrDetail">
     <?ifexist queue.definitionType ?>
      <h2>Type:</h2><div class="detail"><?= queue.type ?> - <?= queue.definitionType ?> - <?= queue.usage ?></div> <!-- detail -->
     <?else ?>
      <h2>Type:</h2><div class="detail"><?= queue.type ?> <?= queue.usage ?></div> <!-- detail -->
     <?endif ?>
     <?ifexist queue.curdepth ?>
      <h2>Depth:</h2><div class="detail"><?= queue.curdepth ?> / <?ifexist queue.maxQueueDepth ?><?= queue.maxQueueDepth ?><?else ?>-<?endif ?></div> <!-- detail -->
     <?else ?>
      <h2>Depth:</h2><div class="detail">- / <?ifexist queue.maxQueueDepth ?><?= queue.maxQueueDepth ?><?else ?>-<?endif ?></div> <!-- detail -->
     <?endif ?>
     <?ifexist queue.inputprocs ?>
      <h2>MQOPEN:</h2><div class="detail"><?= queue.inputprocs ?> for Input, <?= queue.outputprocs ?> for Output</div> <!-- detail -->
     <?endif ?>
     <?ifexist queue.getAllowed ?>
      <h2>Operations:</h2><div class="detail">Get <?= queue.getAllowed ?> - Put <?= queue.putAllowed ?></div> <!-- detail -->
     <?endif ?>
     <?ifexist queue.createDate ?>
      <h2>Creation Date:</h2><div class="detail"><?= queue.createDate ?></div> <!-- detail -->
     <?endif ?>
     <h2>Alteration Date:</h2><div class="detail"><?= queue.alterDate ?></div> <!-- detail -->
     <?if queue.backoutQueue ?>
      <h2>Backout:</h2>
      <div class="detail">
       <a href="/<?= qmgr.name ?>/Queues/<?= queue.backoutQueue ?>"><?= queue.backoutQueue ?></a> - Threshold: <?= queue.backoutThreshold?> - Harden: <?= queue.hardenBackout ?>
      </div> <!-- detail -->
     <?endif ?>
     <?if queue.baseQueue ?>
      <h2>Target Queue:</h2><div class="detail"><a href="/<?= qmgr.name ?>/Queues/<?= queue.baseQueue ?>"><?= queue.baseQueue ?></a></div> <!-- detail -->
     <?endif ?>
     <h2>Default Binding:</h2><div class="detail"><?= queue.bind ?></div> <!-- detail -->
     <?ifexist queue.inputopen ?>
      <h2>Default Input Open:</h2><div class="detail"><?= queue.inputopen ?></div> <!-- detail -->
     <? endif ?>
     <?ifexist queue.distributionList ?>
      <h2>Distribution List:</h2><div class="detail"><?= queue.distributionList ?></div> <!-- detail -->
     <?endif ?>
     <?ifexist queue.persist ?>
      <h2>Default Persistence:</h2><div class="detail"><?= queue.persist ?></div> <!-- detail -->
     <?endif ?>
     <?ifexist queue.priority ?>
      <h2>Default Priority:</h2><div class="detail"><?= queue.priority ?></div> <!-- detail -->
     <?endif ?>
     <?ifexist queue.defReadAhead ?>
      <h2>Default Read Ahead:</h2><div class="detail"><?= queue.defReadAhead ?></div> <!-- detail -->
     <?endif ?>
     <?ifexist queue.defPutRespType ?>
      <h2>Default Put Response Type:</h2><div class="detail"><?= queue.defPutRespType ?></div> <!-- detail -->
     <?endif ?>
     <?ifexist queue.depthHighEvent ?>
      <h2>Depth Hight Event:</h2><div class="detail"><?= queue.depthHighEvent ?> - Limit: <?= queue.depthHighLimit ?></div> <!-- detail -->
     <?endif ?>
     <?ifexist queue.depthLowEvent ?>
      <h2>Depth Low Event:</h2><div class="detail"><?= queue.depthLowEvent ?> - Limit: <?= queue.depthLowLimit ?></div> <!-- detail -->
     <?endif ?>
     <?ifexist queue.depthMaxEvent ?>
      <h2>Depth Max Event:</h2><div class="detail"><?= queue.depthMaxEvent ?></div> <!-- detail -->
     <?endif ?>
     <?if queue.initiationQueue ?>
      <h2>Initiation Queue:</h2><div class="detail"><a href="/<?= qmgr.name ?>/Queues/<?= queue.initiationQueue ?>"><?= queue.initationQueue ?></a></div> <!-- detail -->
     <?endif ?>
     <?ifexist queue.maxMessageLength ?>
      <h2>Maximum Message Length:</h2><div class="detail"><?= queue.maxMessageLength ?></div> <!-- detail -->
     <?endif ?>
     <?ifexist queue.messageDelivery ?>
      <h2>Message Delivery:</h2><div class="detail"><?= queue.messageDelivery ?></div> <!-- detail -->
     <?endif ?>
     <?if queue.process ?>
      <h2>Process:</h2><div class="detail"><?= queue.process ?></div> <!-- detail -->
     <?endif ?>
     <?ifexist queue.triggerControl ?>
       <h2>Trigger:</h2>
       <div class="detail">
        <?= queue.triggerControl ?> - Type: <?= queue.triggerType ?> - Depth: <?= queue.triggerDepth ?> - Message Priority: <?= queue.triggerMessagePriority ?> - Data: <?= queue.triggerData ?>
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
	                   url: '<?= queue.name ?>/Messages'
	                 }; 
	   jQuery('#messageFilterForm').ajaxForm(options);
	  </script>
   </div> <!-- messages -->
  </div> <!-- tabs -->
 </body>
</html>

