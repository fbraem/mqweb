<script type="text/html" id="queueDetail">
	<div style="float:right;">
		<a href="#" data-bind="click:$root.reload"><img class="tip" src="/static/images/view-refresh-icon.png" style="float:right;padding-top:8px;padding-right:5px;" alt="Reload" /></a>
	</div>
	<div class="details" style="width:70%;margin:auto">
		<h2>Name:</h2>
		<div class="detail" data-bind="text:QName.value" />
		<h2>Type:</h2>
		<div class="detail" data-bind="text:QType.display" />
		<div data-bind="if: $data.CurrentQDepth">
			<h2>Depth:</h2>
			<div class="detail">
				<a data-bind="attr: {href: '/message/index/<?=mqweb.qmgr?>/' + QName.value}, text:CurrentQDepth.value" />
				 / <span data-bind="text:MaxQDepth.value" />
			</div> <!-- detail -->
		</div>
		<div data-bind="if: !$data.CurrentQDepth && $data.MaxQDepth">
			<h2>Depth:</h2>
			<div class="detail">
				 - / <span data-bind="text:MaxQDepth.value" />
			</div> <!-- detail -->
		</div>
		<div data-bind="if: $data.OpenInputCount">
			<h2>MQOPEN:</h2>
			<div class="detail">
				<span data-bind="text:OpenInputCount.value" /> for Input, <span data-bind="text:OpenOutputCount.value" /> for Output
			</div> <!-- detail -->
		</div>
		<div data-bind="if: $data.InhibitGet">
			<h2>Operations:</h2>
			<div class="detail">
				Get <span data-bind="text:InhibitGet.display" /> - Put <span data-bind="text:InhibitPut.display" />
			</div> <!-- detail -->
		</div>
		<div data-bind="if: $data.CreationDate">
			<h2>Creation Date:</h2>
			<div class="detail">
				<span data-bind="text:CreationDate.value" /> <span data-bind="text:CreationTime.value" />
			</div> <!-- detail -->
		</div>
		<div data-bind="if: $data.AlterationDate">
			<h2>Alteration Date:</h2>
			<div class="detail">
				<span data-bind="text:AlterationDate.value" /> <span data-bind="text: AlterationTime.value" />
			</div> <!-- detail -->
		</div>
		<div data-bind="if: $data.BackoutRequeueName && $data.BackoutRequeueName.value">
			<h2>Backout:</h2>
			<div class="detail">
				<a href="/queue/view/<?= mqweb.qmgr ?>/<?= queue.BackoutRequeueName.value ?>" data-bind="text: BackoutRequeueName.value" /> - Threshold: <span data-bind="text: BackoutThreshold.value" /> - Harden: <span data-bind="text:HardenGetBackout.display" />
			</div> <!-- detail -->
		</div>
		<div data-bind="if: $data.BaseQName">
			<h2>Target Queue:</h2>
			<div class="detail">
				<a href="/queue/view/<?= mqweb.qmgr ?>/<?= queue.BaseQName.value ?>" data-bind="text: BaseQName.value" />
			</div> <!-- detail -->
		</div>
		<div data-bind="if: $data.DefBind">
			<h2>Default Binding:</h2>
			<div class="detail">
				<span data-bind="text:DefBind.display" />
			</div> <!-- detail -->
		</div>
		<div data-bind="if: $data.DefInputOpenOption">
			<h2>Default Input Open:</h2>
			<div class="detail">
				<span data-bind="text: DefInputOpenOption.display" />
			</div> <!-- detail -->
		</div>
		<div data-bind="if: $data.DistLists">
			<h2>Distribution List:</h2>
			<div class="detail">
				<span data-bind="text:DistLists.display" />
			</div> <!-- detail -->
		</div>
		<div data-bind="if: $data.DefPersistence">
			<h2>Default Persistence:</h2>
			<div class="detail">
				<span data-bind="text:DefPersistence.display" />
			</div> <!-- detail -->
		</div>
		<div data-bind="if: $data.DefPriority">
			<h2>Default Priority:</h2>
			<div class="detail">
				<span data-bind="text:DefPriority.value" />
			</div> <!-- detail -->
		</div>
		<div data-bind="if: $data.DefReadAhead">
			<h2>Default Read Ahead:</h2><div class="detail"><span data-bind="text:DefReadAhead.display" /></div> <!-- detail -->
		</div>
		<div data-bind="if: $data.DefaultPutResponse">
			<h2>Default Put Response Type:</h2><div class="detail"><span data-bind="text:DefaultPutResponse.display" /></div> <!-- detail -->
		</div>
		<div data-bind="if: $data.QDepthHighEven">
			<h2>Depth Hight Event:</h2><div class="detail"><span data-bind="text:QDepthHighEvent.display" /> - Limit: <span data-bind="text:QDepthHighLimit.value" /></div> <!-- detail -->
		</div>
		<div data-bind="if: $data.QDepthLowEvent">
			<h2>Depth Low Event:</h2><div class="detail"><span data-bind="text:QDepthLowEvent.display" /> - Limit: <span data-bind="text:QDepthLowLimit.value" /></div> <!-- detail -->
		</div>
		<div data-bind="if: $data.QDepthMaxEvent">
			<h2>Depth Max Event:</h2><div class="detail"><span data-bind="text:QDepthMaxEvent.display" /></div> <!-- detail -->
		</div>
		<div data-bind="if: $data.InitiationQName && $data.InitiationQName.value">
			<h2>Initiation Queue:</h2>
			<div class="detail">
				<a href="/queue/view/<?= mqweb.qmgr ?>/<?= queue.InitiationQName.value ?>" data-bind="text:InitiationQName.value" />
			</div> <!-- detail -->
		</div>
		<div data-bind="if: $data.MaxMsgLength">
			<h2>Maximum Message Length:</h2>
			<div class="detail">
				<span data-bind="text:MaxMsgLength.value" />
			</div> <!-- detail -->
		</div>
		<div data-bind="if: $data.MsgDeliverySequence">
			<h2>Message Delivery:</h2>
			<div class="detail">
				<span data-bind="text:MsgDeliverySequence.display" />
			</div> <!-- detail -->
		</div>
		<div data-bind="if: $data.ProcessName && $data.ProcessName.value">
			<h2>Process:</h2>
			<div class="detail">
				<span data-bind="text:ProcessName.value" />
			</div> <!-- detail -->
		</div>
		<div data-bind="if: $data.TriggerControl">
			<h2>Trigger:</h2>
			<div class="detail">
				<span data-bind="text:TriggerControl.display" /> - Type: <span data-bind="text:TriggerType.display" /><br />
				Depth: <span data-bind="text:TriggerDepth.value" /> - Message Priority: <span data-bind="text: TriggerMsgPriority.value" /><br />
				Data: <span data-bind="text:TriggerData.value" />
			</div> <!-- detail -->
		</div>
	</div> <!-- details -->
</script>
