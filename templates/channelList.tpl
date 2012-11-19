<div style="margin:10px;">
 <table id="channelTable" style="width:100%">
  <tr>
   <th></th><th>Type</th><th>Alteration Date</th><th>Status</th>
  </tr>
  <? for channel channels ?>
    <tr class="detail">
     <td>
      <?if channel.channelType.Server-connection ?>
	      <a href="/<?= qmgr.name ?>/Channels/<?= channel.channelName ?>?type=svrconn"><?= channel.channelName ?></a>
	    <?elsif channel.channelType.Client-connection ?>
	      <a href="/<?= qmgr.name ?>/Channels/<?= channel.channelName ?>?type=clntconn"><?= channel.channelName ?></a>
	    <?elsif channel.channelType.Sender ?>
	      <a href="/<?= qmgr.name ?>/Channels/<?= channel.channelName ?>?type=sender"><?= channel.channelName ?></a>
	    <?elsif channel.channelType.Receiver ?>
	      <a href="/<?= qmgr.name ?>/Channels/<?= channel.channelName ?>?type=receiver"><?= channel.channelName ?></a>
	    <?elsif channel.channelType.Server ?>
	      <a href="/<?= qmgr.name ?>/Channels/<?= channel.channelName ?>?type=server"><?= channel.channelName ?></a>
	    <?elsif channel.channelType.Cluster-receiver ?>
	      <a href="/<?= qmgr.name ?>/Channels/<?= channel.channelName ?>?type=clusrcvr"><?= channel.channelName ?></a>
	    <?elsif channel.channelType.Cluster-sender ?>
	      <a href="/<?= qmgr.name ?>/Channels/<?= channel.channelName ?>?type=clussdr"><?= channel.channelName ?></a>
	    <?elsif channel.channelType.Telemetry ?>
	      <a href="/<?= qmgr.name ?>/Channels/<?= channel.channelName ?>?type=mqtt"><?= channel.channelName ?></a>
	    <?elsif channel.channelType.Requester ?>
	      <a href="/<?= qmgr.name ?>/Channels/<?= channel.channelName ?>?type=requester"><?= channel.channelName ?></a>
	    <?endif ?>
      <?if channel.autodefined?><sup>auto-defined</sup><?endif ?>
      <?if channel.channelDesc ?>
       <br /><em style="padding:5px;font-size:0.8em"><?= channel.channelDesc ?></em> 
      <?endif ?>
     </td>
     <td><?= channel.channelType.value ?></td>
     <td><?= channel.alterationDate ?></td>
     <td>
      <?ifexist channel.status ?>
       <?= channel.status.channelStatus.value ?>
      <?else ?>
       inactive
      <?endif ?>
     </td>
    </tr>
  <? endfor ?>
 </table>
</div>
