<script src="/static/js/number_format.js" type="text/javascript" charset="utf-8"></script>
<script type="text/html" id="channelDetail">
	<div style="float:right;">
		<a href="#" data-bind="click:$root.reload"><img class="tip" src="/static/images/view-refresh-icon.png" style="float:right;padding-top:8px;padding-right:5px;" alt="Reload" /></a>
	</div>
	<div class="details" style="width:70%;margin:auto">
		<h2>Name:</h2>
		<div class="detail" data-bind="text:ChannelName.value" />
		<h2>Type:</h2>
		<div class="detail" data-bind="text:ChannelType.display" />
		<div data-bind="if: $data.Desc && $data.Desc.value">
			<h2>Description:</h2>
	    <div class="detail">
				<em style="padding:5px;font-size:0.8em" data-bind="text: $data.Desc.value"></em>
			</div> <!-- detail -->
		</div>
    <div data-bind="if: $data.AlterationDate">
     <h2>Alteration Date:</h2>
     <div class="detail">
      <span data-bind="text: AlterationDate.value" /> <span data-bind="text: AlterationTime.value" />
     </div> <!-- detail -->
    </div>
    <div data-bind="if: $data.ChannelDesc && $data.ChannelDesc.value">
     <h2>Description:</h2>
     <div class="detail">
      <span data-bind="text: ChannelDesc.value" />
     </div> <!-- detail -->
    </div>
    <div data-bind="if: $data.ConnectionName && $data.ConnectionName.value">
     <h2>Connection Name:</h2>
     <div class="detail">
      <span data-bind="text: ConnectionName.value" />
     </div> <!-- detail -->
    </div>
    <div data-bind="if: $data.MCAUserIdentifier && $data.MCAUserIdentifier.value">
     <h2>MCA User:</h2>
     <div class="detail">
      <span data-bind="text: MCAUserIdentifier.value" />
     </div> <!-- detail -->
    </div>
    <div data-bind="if: $data.MaxInstances">
     <h2>Maximum Instances:</h2>
     <div class="detail">
      <span data-bind="text: number_format(MaxInstances.value, 0, ',', '.')" />
     </div> <!-- detail -->
    </div>
    <div data-bind="if: $data.MaxMsgLength">
     <h2>Maximum Message Length:</h2>
     <div class="detail">
      <span data-bind="text: number_format(MaxMsgLength.value, 0, ',', '.')" /> bytes
     </div> <!-- detail -->
    </div>
    <div data-bind="if: $data.ChannelMonitoring && $data.ChannelMonitoring.value">
     <h2>Channel Monitoring:</h2>
     <div class="detail">
      <span data-bind="text: ChannelMonitoring.display" />
     </div> <!-- detail -->
    </div>
    <div data-bind="if: $data.ReceiveExit && $data.ReceiveExit.value">
     <h2>Receive Exit:</h2>
     <div class="detail">
      <span data-bind="text: ReceiveExit.value" />
     </div> <!-- detail -->
    </div>
    <div data-bind="if: $data.ReceiveUserData && $data.ReceiveUserData.value">
     <h2>Receive User Data:</h2>
     <div class="detail">
      <span data-bind="text: ReceiveUserData.value" />
     </div> <!-- detail -->
    </div>
    <div data-bind="if: $data.SecurityExit && $data.SecurityExit.value">
     <h2>Security Exit:</h2>
     <div class="detail">
      <span data-bind="text: SecurityExit.value" />
     </div> <!-- detail -->
    </div>
    <div data-bind="if: $data.SecurityUserData && $data.SecurityUserData.value">
     <h2>Security User Data:</h2>
     <div class="detail">
      <span data-bind="text: SecurityUserData.value" />
     </div> <!-- detail -->
    </div>
    <div data-bind="if: $data.TransportType && $data.TransportType.display">
     <h2>Transport Type:</h2>
     <div class="detail">
      <span data-bind="text: TransportType.display" />
     </div> <!-- detail -->
    </div>
    <div data-bind="if: $data.XmitQName && $data.XmitQName.value">
     <h2>Transmission Queue:</h2>
     <div class="detail">
			<a data-bind="attr: {href: '/queue/view/<?=mqweb.qmgr?>/' + XmitQName.value}, text:XmitQName.value" />
     </div> <!-- detail -->
	 	</div>
   </div> <!-- details -->
</script>