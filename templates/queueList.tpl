<div style="margin:10px;">
 <table id="queueTable" style="width:100%">
  <tr>
   <th></th><th>Type</th><th>Depth</th><th>Input</th><th>Output</th><th>Get Enabled</th><th>Put Enabled</th><th>Usage</th>
  </tr>
  <? for queue queues ?>
    <tr class="detail">
     <td><a href="/<?= qmgr.name ?>/Queues/<?= queue.QName.value ?>"><?= queue.QName.value ?></a>
      <? if queue.BaseQName ?>
       <br />
       <span style="padding:5px;font-size:0.8em">Target: <a href="/<?= qmgr.name ?>/Queues/<?= queue.BaseQName.value ?>"><?= queue.BaseQName.value ?></a></span>
      <? endif ?>
      <? if queue.RemoteQName ?>
       <br />
       <span style="padding:5px;font-size:0.8em">Qmgr: <?= queue.RemoteQmgrName.value ?> Queue: <?= queue.RemoteQName.value ?></span><br />
       <span style="padding:5px;font-size:0.8em">Transmission Queue: <a href="/<?= qmgr.name ?>/Queues/<?= queue.XmitQName.value ?>"><?= queue.XmitQName.value ?></a></span>
      <? endif ?>
      <? if queue.QDesc.value ?>
        <br /><em style="padding:5px;font-size:0.8em"><?= queue.QDesc.value ?></em>      
      <? endif ?>
     </td>
     <td><?= queue.QType.display ?></td>
     <? ifexist queue.CurrentQDepth ?>
      <? if queue.CurrentQDepth.value ?>
       <td style="text-align:right;background-color:#FF9966">
        <?= queue.CurrentQDepth.value ?>
       </td>
      <? else ?>
       <td style="text-align:right;">
        0
       </td>
      <? endif ?>
     <? else ?>
      <td style="text-align:right">-</td>
     <? endif ?>
     </td>
     <td style="text-align:right">
      <? ifexist queue.OpenInputCount ?>
       <?= queue.OpenInputCount.value ?>
      <? else ?>
       -
     <? endif ?>
     </td>
     <td style="text-align:right">
      <? ifexist queue.OpenOutputCount ?>
       <?= queue.OpenOutputCount.value ?>
      <? else ?>
       -
     <? endif ?>
     </td>
     <td style="text-align:center">
      <? if queue.InhibitGet ?>
       <? if queue.InhibitGet.value ?>
        <img src="/static/img/disabled.gif" />
       <? else ?>
        <img src="/static/img/enabled.gif" />
       <? endif ?>
      <? else ?>
       -
      <? endif ?>
     </td>
     <td style="text-align:center">
      <? if queue.InhibitPut ?>
       <? if queue.InhibitPut.value ?>
        <img src="/static/img/disabled.gif" />
       <? else ?>
        <img src="/static/img/enabled.gif" />
       <? endif ?>
      <? else ?>
       -
      <? endif ?>
     </td>
     <td>
      <? if queue.Usage ?>
       <?= queue.Usage.display ?></td>
      <? else ?>
       -
      <? endif ?>
    </tr>
  <? endfor ?>
 </table>
</div>
