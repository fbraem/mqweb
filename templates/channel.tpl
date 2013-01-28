<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" >
 <head>
  <title>MQWeb :: <?= qmgr.name ?> :: <?= channel.channelName ?></title>
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
   Name: <a href="/qmgr/view/<?= qmgr.name ?>"><?= qmgr.name ?></a><br />
   Id: <?= qmgr.id ?><br />
   Created: <?= qmgr.createDate ?><br />
   Modified: <?= qmgr.alterDate ?>
  </div>
  <?if qmgr.description ?>
   <div style="clear:both;background-color:#fbec88;border:1px solid #fad42e;padding:5px;margin-bottom:5px">
    <?= qmgr.description ?>
   </div>
  <?endif ?>
  <div style="clear:both"> </div>
  <div id="tabs">
   <ul>
    <li><a href="#channel"><?= channel.ChannelName.value ?></a></li>
    <li><a href="#status">Status</a></li>
   </ul>
   <div id="channel">
    <?if channel.ChannelDesc ?>
     <em style="padding:5px;"><?= channel.ChannelDesc.value ?></em>
    <?endif ?>
    <br />
    <div class="details">
     <h2>Type:</h2>
     <div class="detail">
      <?= channel.ChannelType.display ?>
     </div> <!-- detail -->
     <h2>Alteration Date:</h2>
     <div class="detail">
      <?= channel.AlterationDate.value ?> <?= channel.AlterationTime.value ?>
     </div> <!-- detail -->
     <?if channel.ConnectionName ?>
      <h2>Connection Name:</h2>
      <div class="detail">
       <?= channel.ConnectionName.value ?></a>
      </div> <!-- detail -->
		 <?endif ?>
     <?if channel.MCAUserIdentifier ?>
      <h2>MCA User:</h2>
      <div class="detail">
       <?= channel.MCAUserIdentifier.value ?></a>
      </div> <!-- detail -->
		 <?endif ?>
     <?if channel.SecurityExit ?>
      <h2>Security Exit:</h2>
      <div class="detail">
       <?= channel.SecurityExit.value ?></a>
      </div> <!-- detail -->
      <h2>Security User Data:</h2>
      <div class="detail">
       <?= channel.SecurityUserData.value ?></a>
      </div> <!-- detail -->
		 <?endif ?>
     <?if channel.XmitQName ?>
      <h2>Transmission Queue:</h2>
      <div class="detail">
       <a href="/queue/view/<?= qmgr.name ?>/<?= channel.XmitQName.value ?>"><?= channel.XmitQName.value ?></a>
      </div> <!-- detail -->
		 <?endif ?>
    </div> <!-- details -->
   </div> <!-- channel -->
   <div id="status">
    Channel status information will be available in a next version of MQWeb.
   </div> <!-- status -->
  </div> <!-- tabs -->
 </body>
</html>
