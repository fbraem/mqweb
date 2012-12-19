<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
 <head>
  <title>MQWeb :: <?= qmgr.name ?></title>
  <link href="/static/css/redmond/jquery-ui-1.8.5.custom.css" rel="stylesheet" type="text/css" />
  <link href="/static/css/mqweb/mqweb.css" rel="stylesheet" type="text/css" />
  <script src="/static/js/jquery-1.4.2.min.js" type="text/javascript"> </script>
  <script src="/static/js/jquery-ui-1.8.5.custom.min.js" type="text/javascript"> </script>
  <script src="/static/js/jquery.form.js" type="text/javascript"> </script>
  <script type="text/javascript">
   jQuery(document).ready(function()
   {
     jQuery("#tabs").tabs();
   });
  </script>
 </head>
 <body>
  <h1 style="width:80%;float:left;"><?= qmgr.name ?></h1>
  <div style="float:right;font-size:0.8em;">
   Name: <a href="/<?= qmgr.name ?>"><?= qmgr.name ?></a><br />
   Id: <?= qmgr.id ?><br />
   <? ifexist qmgr.createDate ?>
     Created: <?= qmgr.createDate ?><br />
   <? endif ?>
   Modified: <?= qmgr.alterDate ?>
  </div>
  <? if qmgr.description ?>
   <div style="clear:both;background-color:#fbec88;border:1px solid #fad42e;padding:5px;margin-bottom:5px">
    <?= qmgr.description ?>
   </div>
  <? endif ?>
  <div style="clear:both;"> </div>
  <div id="tabs" style="margin-top:10px;"> <!-- Begin Tabs -->
   <ul>
    <li><a href="#qmgrDetail"><?=qmgr.name?></a></li>
    <li><a href="#queues">Queues</a></li>
    <li><a href="#channels">Channels</a></li>
   </ul>
   <div class="details" id="qmgrDetail"> <!-- Begin QMGR tab -->
    <h2>Id:</h2>
    <div class="detail">
     <?=qmgr.id?>
    </div>
    <h2>Creation Date:</h2>
    <div class="detail">
     <?=qmgr.createDate?>
    </div>
    <h2>Alteration Date:</h2>
    <div class="detail">
     <?=qmgr.alterDate?>
    </div>
    <h2>CharacterSet Id:</h2>
    <div class="detail">
     <?=qmgr.ccsid?>
    </div>
    <h2>Platform:</h2>
    <div class="detail">
     <?=qmgr.platform?>
    </div>
    <? if qmgr.deadLetterQueue ?>
     <h2>Dead letter Queue:</h2>
      <div class="detail">
       <a href="/<?= qmgr.name ?>/Queues/<?= qmgr.deadLetterQueue ?>"><?= qmgr.deadLetterQueue ?></a>
      </div>
    <? endif ?>
    <h2>Command Server:</h2>
    <div class="detail">
      <a href="/<?= qmgr.name ?>/Queues/<?= qmgr.commandQueue ?>"><?= qmgr.commandQueue ?></a> - Level: <?= qmgr.commandLevel ?>
    </div>
   </div> <!-- End QMGR tab -->
   <div id="queues"> <!-- Begin QUEUES tab -->
    <form id="queueFilterForm" action="">
     <fieldset>
      <legend>&nbsp;Filter&nbsp;</legend>
      <div style="float:left;">
       <label for="queueName">Queue Name: </label><input type="text" id="queueName" name="queueName" value="*" /><br />
       <label for="queueDepth">Queue Depth: </label><input type="text" id="queueDepth" name="queueDepth" /><br />
       <input type="submit" value="Filter &gt;&gt;" name="Filter" />
      </div>
      <fieldset style="float:left;margin-top:-10px;margin-left:30px;">
       <legend>&nbsp;Queue Type&nbsp;</legend>
       <div style="float:left;">
        <input type="radio" name="queueType" value="All" />All<br />
        <input type="radio" name="queueType" value="Local" />Local<br />
        <input type="radio" name="queueType" value="Alias" />Alias
       </div>
       <div style="float:left;">
        <input type="radio" name="queueType" value="Cluster" />Cluster<br />
        <input type="radio" name="queueType" value="Model" />Model<br />
        <input type="radio" name="queueType" value="Remote" />Remote
       </div>
      </fieldset>
      <div style="float:left;">
       <input type="checkbox" name="queueExcludeSystem" value="1" />Exclude System Queues<br />
       <input type="checkbox" name="queueExcludeTemp" value="1" />Exclude Temporary Queues<br />
      </div>
     </fieldset>
    </form>
    <div id="queueTableDiv"> </div>
    <script type="text/javascript">
     var options = { 
                     target: '#queueTableDiv',
                     type: 'POST',
                     url: '<?= qmgr.name ?>/Queues'
                   }; 
     jQuery('#queueFilterForm').ajaxForm(options);
    </script>
   </div> <!-- End Queues tab -->
   <div id="channels"> <!-- Begin Channels tabe -->
    <form id="channelFilterForm" action="">
     <fieldset>
      <legend>&nbsp;Filter&nbsp;</legend>
      <div style="float:left;">
       <label for="channelName">Channel Name: </label><input type="text" id="channelName" name="channelName" value="*" /><br />
       <input type="submit" value="Filter &gt;&gt;" name="Filter" />
      </div>
      <fieldset style="float:left;margin-top:-10px;margin-left:30px;">
       <legend>&nbsp;Channel Type&nbsp;</legend>
       <div style="float:left;">
        <input type="radio" name="channelType" value="All" />All<br />
        <input type="radio" name="channelType" value="Sender" />Sender
       </div>
       <div style="float:left;">
        <input type="radio" name="channelType" value="Server" />Server<br />
        <input type="radio" name="channelType" value="Receiver" />Receiver
       </div>
       <div style="float:left;">
        <input type="radio" name="channelType" value="Requester" />Requester<br />
        <input type="radio" name="channelType" value="Server-connection" />Server-connection
       </div>
       <div style="float:left;">
        <input type="radio" name="channelType" value="Client-connection" />Client-connection<br />
        <input type="radio" name="channelType" value="Cluster-receiver" />Cluster-receiver
       </div>
       <div style="float:left;">
        <input type="radio" name="channelType" value="Cluster-sender" />Cluster-sender<br />
        <input type="radio" name="channelType" value="Telemetry" />Telemetry
       </div>
      </fieldset>
      <div style="float:left;">
       <input type="checkbox" name="channelExcludeSystem" value="1" />Exclude System Channels<br />
      </div>
     </fieldset>
    </form>
    <div id="channelTableDiv"> </div>
    <script type="text/javascript">
      var options = { 
                      target: '#channelTableDiv',
                      type: 'POST',
                      url: '<?= qmgr.name ?>/Channels'
                    }; 
      jQuery('#channelFilterForm').ajaxForm(options);
    </script>
   </div> <!-- End Channels tabe -->
  </div> <!-- End tabs -->
 </body>
</html>
