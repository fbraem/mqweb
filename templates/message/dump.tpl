<div data-bind="with: message">
	<h3 style="margin-bottom:10px"><?= queueName ?></h3>
	<!-- <pre><code data-bind="html:ko.toJSON($data, null, 2)"> </code></pre> -->
	<div class="details">
		<h2>Message ID:</h2>
		<div class="detail" data-bind="text:MsgId"> </div>
		<h2>Correlation ID:</h2>
		<div class="detail" data-bind="text:CorrelId"> </div>
		<h2>Put Date:</h2>
		<div class="detail"><span data-bind="text:PutDate"> </div>
		<h2>Message Size:</h2>
		<div class="detail"><span data-bind="text:Length"> </span> bytes</div>
		<h2>Coded Character Set ID:</h2>
		<div class="detail" data-bind="text:CodedCharSetId"> </div>
		<h2>Encoding:</h2>
		<div class="detail" data-bind="text:Encoding"> </div>
	</div> <!-- details -->
	<div style="margin-top:10px;margin-bottom:10px">
	 <a id="ascii" href="#" data-bind="button: {}, click: $root.showAscii"><span data-bind="text: $root.ascii() ? 'Hide ASCII' : 'Show ASCII'" /></a>&nbsp;&nbsp;<a id="ebcdic" href="#" data-bind="button: {}, click: $root.showEbcdic"><span data-bind="text: $root.ebcdic() ? 'Hide EBCDIC' : 'Show EBCDIC'" /></a>
	</div>
	<table id="messageDumpTable" style="font-size:0.8em;border-collapse:collapse;">
	 <thead>
	  <tr>
	   <th>Position</th><th>Hex</th><th data-bind="visible: $parent.ascii">ASCII</th><th data-bind="visible: $parent.ebcdic">EBCDIC</th>
	  </tr>
	 </thead>
   <tbody data-bind="foreach: { data: dump, as: 'dump'}">
	  <tr>
  	 <td style="padding-left:5px;padding-right:5px;border-left:1px dotted black;border-right:1px dotted black;"><code data-bind="text:dump.position" /></td>
	   <td nowrap="nowrap" style="padding-left:5px;padding-right:5px;border-right:1px dotted black;"><code data-bind="text:dump.hex" /></td>
  	 <td data-bind="visible: $root.ascii" style="padding-left:5px;padding-right:5px;border-right:1px dotted black;"><code data-bind="html:dump.ascii" /></td>
   	 <td data-bind="visible: $root.ebcdic" style="padding-left:5px;padding-right:5px;border-right:1px dotted black;"><code data-bind="html:dump.ebcdic" /></td>
   </tr>
 	</tbody>
 </table>
</div>