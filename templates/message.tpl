<script type="text/javascript"> 
  	$(function() 
  	{
	  	$('#messageDumpTable > tbody > tr:odd').css('background-color', '#E2FEFE');
	  	$('#messageDumpTable > tbody > tr:even').css('background-color', '#FDFDEB');
	  	
	  	$("a#ascii").button();
	  	$("a#ascii").click(function(event)
											   {
											     	$('#messageDumpTable td:nth-child(3), #messageDumpTable th:nth-child(3)').toggle();
											     	if ( $('#messageDumpTable th:nth-child(3)').is(':visible') )
											     	{
											     	  $("span", this).text("Hide ASCII");
											     	}
											     	else
											     	{
											     	  $("span", this).text("Show ASCII");
											     	}
											   });
	  	
	  	$("a#ebcdic").button();
	  	$("a#ebcdic").click(function(event)
											    {
											     	$('#messageDumpTable td:nth-child(4), #messageDumpTable th:nth-child(4)').toggle();
											     	if ( $('#messageDumpTable th:nth-child(4)').is(':visible') )
											     	{
											     	  $("span", this).text("Hide EBCDIC");
											     	}
											     	else
											     	{
											     	  $("span", this).text("Show EBCDIC");
											     	}
											    });
  	});
</script>
<h4>id: </strong><?= message.id ?></h4>
<a id="ascii" href="#">Hide ASCII</a>&nbsp;<a id="ebcdic" href="#">Hide EBCDIC</a>
<table id="messageDumpTable" style="font-size:0.8em;border-collapse:collapse;">
 <thead>
  <tr>
   <th>Position</th><th>Hex</th><th>ASCII</th><th>EBCDIC</th>
  </tr>
 </thead>
 <tbody>
  <? for dump message.dump ?>
   <tr>
    <th style="padding-left:5px;padding-right:5px;border-left:1px dotted black;border-right:1px dotted black;"><code><?= dump.position ?></code></th>
    <td nowrap="nowrap" style="padding-left:5px;padding-right:5px;border-right:1px dotted black;"><code><?= dump.hex ?></code></td>
    <td style="padding-left:5px;padding-right:5px;border-right:1px dotted black;"><code><?= dump.ascii ?></code></td>
    <td style="padding-left:5px;padding-right:5px;border-right:1px dotted black;"><code><?= dump.ebcdic ?></code></td>
   </tr>
  <? endfor ?>
 </tbody>
</table>