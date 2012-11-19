<div style="margin:10px;">
 <table id="queueTable" style="width:100%">
  <tr>
   <th></th><th>Type</th><th>Depth</th><th>Input</th><th>Output</th><th>Get Enabled</th><th>Put Enabled</th><th>Usage</th>
  </tr>
  <? for queue queues ?>
    <tr class="detail">
     <td><a href="/<?= qmgr.name ?>/Queues/<?= queue.name ?>"><?= queue.name ?></a>
      <? if queue.baseQueue ?>
       <br />
       <span style="padding:5px;font-size:0.8em">Target: <a href="/<?= qmgr.name ?>/Queues/<?= queue.baseQueue ?>"><?= queue.baseQueue ?></a></span>
      <? endif ?>
      <? if queue.remoteQueue ?>
       <br />
       <span style="padding:5px;font-size:0.8em">Qmgr: <?= queue.remoteQueuemanager ?> Queue: <?= queue.remoteQueue ?></span><br />
       <span style="padding:5px;font-size:0.8em">Transmission Queue: <a href="/<?= qmgr.name ?>/Queues/<?= queue.xmitQueue ?>"><?= queue.xmitQueue ?></a></span>
      <? endif ?>
      <? if queue.description ?>
        <br /><em style="padding:5px;font-size:0.8em"><?= queue.description ?></em>      
      <? endif ?>
     </td>
     <td><?= queue.type ?></td>
     <? ifexist queue.curdepth ?>
      <? if queue.curdepth ?>
       <td style="text-align:right;background-color:#FF9966">
        <?= queue.curdepth ?>
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
      <? ifexist queue.inputprocs ?>
       <?= queue.inputprocs ?>
      <? else ?>
       -
     <? endif ?>
     </td>
     <td style="text-align:right">
      <? ifexist queue.outputprocs ?>
       <?= queue.outputprocs ?>
      <? else ?>
       -
     <? endif ?>
     </td>
     <? if queue.getAllowed ?>
      <td style="text-align:center"><img src="/static/img/enabled.gif" /></td>
     <? else ?>
      <td style="text-align:center"><img src="/static/img/disabled.gif" /></td>
     <? endif ?>
     <? if queue.putAllowed ?>
      <td style="text-align:center"><img src="/static/img/enabled.gif" /></td>
     <? else ?>
      <td style="text-align:center"><img src="/static/img/disabled.gif" /></td>
     <? endif ?>
     <td><?= queue.usage ?></td>
    </tr>
  <? endfor ?>
 </table>
</div>
