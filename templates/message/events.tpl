<h4>Events</h4>
<table id="messageDumpTable" style="font-size:0.8em;border-collapse:collapse;">
 <thead>
  <tr>
   <th>Date</th>
   <th>Reason</th>
   <th> </th>
  </tr>
 </thead>
 <tbody>
  <? for event events ?>
   <tr>
    <td><?=event.putDate?></td>
    <td><?=event.reason?> - <?= event.desc ?></td>
    <td><?=event.id?></td>
    <td>
     <?= event.fields ?>
    </td>
   </tr>
  <? endfor ?>
 </tbody>
</table>