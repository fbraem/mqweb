<script type="text/javascript">
 $("a.hex").fancybox();
</script>
<div style="display:none;" id="dialog">
</div>
<div style="margin:10px;">
 <table id="messageTable" style="width:100%">
  <thead>
   <tr>
    <th>Put Date</th><th>Id</th><th>Format</th><th>Length</th><th>User</th><th>Application</th><th>Encoding</th><th>CCSID</th>
   </tr>
  </thead>
  <tbody>
   <?for message messages ?>
    <tr class="detail">
     <td><?= message.putDate ?></td>
     <td><?= message.id ?></td>
     <td><?= message.format ?></a></td>
     <td style="text-align:right"><a class="hex" href="/message/view/<?= mqweb.qmgr ?>/<?= queue.name ?>/<?= message.id ?>?type=hex" title="Hex View"><?= message.length ?></a></td>
     <td><?= message.user ?></td>
     <td><?= message.putApplication ?></td>
     <td><?= message.encoding ?></td>
     <td><?= message.ccsid ?></td>
    </tr>
    <?ifexist message.data ?>
     <tr class="detail">
      <td></td><td colspan="5">{message.data}</td>
     </tr>
    <?endif ?>
   <?endfor ?>
  </tbody>
 </table>
</div>

