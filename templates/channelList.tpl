<div style="margin:10px;">
 <table id="channelTable" style="width:100%">
  <tr>
   <th></th><th>Type</th><th>Alteration Date</th><th>Status</th>
  </tr>
  <? for channel channels ?>
    <tr class="detail">
     <td>
      <a href="/<?= qmgr.name ?>/Channels/<?= channel.ChannelName.value ?>?type=<?= channel.ChannelType.display ?>"><?= channel.ChannelName.value ?></a>
      <?if channel.autodefined?><sup>auto-defined</sup><?endif ?>
      <?if channel.channelDesc ?>
       <br /><em style="padding:5px;font-size:0.8em"><?= channel.channelDesc ?></em> 
      <?endif ?>
     </td>
     <td><?= channel.ChannelType.display ?></td>
     <td><?= channel.AlterationDate.value ?> <?= channel.AlterationTime.value ?></td>
     <td>
      <?ifexist channel.status.ChannelStatus ?>
       <?= channel.status.ChannelStatus.display ?>
      <?else ?>
       inactive
      <?endif ?>
     </td>
    </tr>
  <? endfor ?>
 </table>
</div>
