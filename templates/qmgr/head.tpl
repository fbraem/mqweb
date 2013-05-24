<script type="text/javascript">
	var QmgrModel = function()
	{
		var self = this;
		
		self.qmgr = ko.observable(null);
		self.loading = ko.observable(false);
		self.error = ko.observable(null);
		
		this.load = function()
			{
				$.ajax(
				{
					beforeSend: function() {
						self.qmgr(null);
						self.error(null);
						self.loading(true);
					},
					url: "/qmgr/view.json/<?= mqweb.qmgr ?>",
					cache: false,
					dataType: "json",
					success: function(data) {
						self.loading(false);
						if ( data.error )
						{
							self.error(data.error);
						}
						else
						{
							self.qmgr(data.qmgr);
						}
					},
					error: function (request, status, error)
					{
						self.loading(false);
						self.error(null);
						self.qmgr(null);
						//TODO: Handle Error on page
					}
				});
			};
	}
	
	var LocalQueueModel = function()
	{
		var self = this;
		
		self.queues = ko.observable(null);
		self.loading = ko.observable(false);
		self.error = ko.observable(null);
		
		this.load = function()
			{
				$.ajax(
				{
					beforeSend: function() {
						self.queues(null);
						self.error(null);
						self.loading(true);
					},
					url: "/queue/list.json/<?= mqweb.qmgr ?>",
					data : {
						queueDepth : 1,
						queueExcludeSystem: 1,
						queueUsage : "normal"
					},
					cache: false,
					dataType: "json",
					success: function(data) {
						self.loading(false);
						if ( data.error )
						{
							self.error(data.error);
						}
						else
						{
							for(q in data.queues)
							{
								data.queues[q].url = '/queue/view/<?=mqweb.qmgr?>/' + data.queues[q].QName.value;
							}
							self.queues(data.queues);
						}
					},
					error: function (request, status, error)
					{
						self.loading(false);
						self.error(null);
						self.queues(null);
						//TODO: $("#localQueues").html("An error occurred while retrieving local queues: " + status + ", " + error);
					}
				});
			};

	}
	
	var XmitQueueModel = function()
	{
		var self = this;
		
		self.loading = ko.observable(false);
		self.queues = ko.observable(null);
		self.error = ko.observable(null);
		
		this.load = function()
			{
				$.ajax(
				{
					beforeSend: function() {
						self.queues(null);
						self.error(null);
						self.loading(true);
					},
					url: "/queue/list.json/<?= mqweb.qmgr ?>",
					data : {
						queueDepth : 1,
						queueExcludeSystem: 0,
						queueUsage : "xmitq"
					},
					cache: false,
					dataType: "json",
					success: function(data) {
						self.loading(false);
						if ( data.error )
						{
							self.error(data.error);
						}
						else
						{
							for(q in data.queues)
							{
								data.queues[q].url = '/queue/view/<?=mqweb.qmgr?>/' + data.queues[q].QName.value;
							}
							self.queues(data.queues);
						}
					},
					error: function (request, status, error)
					{
						self.loading(false);
						//TODO: $("#localQueues").html("An error occurred while retrieving local queues: " + status + ", " + error);
					}
				});
			};
	}

	function statusImage(status)
	{
		if ( status.ChannelStatus.display == 'Retrying' )
		{
			return "mqChannelStatusRetryingFlag";
		}
		else if ( status.ChannelStatus.display == 'Stopped' )
		{
			return "mqChannelStatusStoppedFlag";
		}
		else if ( status.ChannelStatus.display == 'Running' )
		{
			return "mqChannelStatusRunningFlag";
		}
		return "mqChannelStatusOtherFlag";
	}

	var ChannelModel = function()
	{
		var self = this;
		
		self.loading = ko.observable(false);
		self.channels = ko.observable(null);
		self.error = ko.observable(null);
		
		self.load = function()
		{
			$.ajax(
			{
				beforeSend: function() {
					self.channels(null);
					self.error(null);
					self.loading(true);
				},
				url: "/chs/list.json/<?= mqweb.qmgr ?>",
				cache: false,
				dataType: "json",
				success: function(data)
				{
					self.loading(false);
					if ( data.error )
					{
						self.error(data.error);
					}
					else
					{ 
						self.channels(data.statuses);
						
						$(".tip").qtip({ 
							content : { 
								attr : 'alt' 
							}
						});
					}
				},
				error: function (request, status, error)
				{
					self.loading(false);
					//TODO: $("#channelStatus").html("An error occurred while retrieving channel status: " + status + ", " + error);
				}
			});
			$(".tip").qtip({ 
				content : { 
					attr : 'alt'
				}
			});
		}
	}

	var EventMessageModel = function()
	{
		var self = this;

		self.loading = ko.observable(false);
		self.events = ko.observable(null);
		self.error = ko.observable(null);
		self.count = ko.observable(0);
		self.curdepth = ko.observable(0);
		self.partialView = ko.computed(function() { return (!self.loading()) && self.count() < self.curdepth(); }, self);
		
		self.load = function()
		{
			$.ajax(
			{
				beforeSend: function() 
				{
					self.events(null);
					self.error(null);
					self.loading(true);
				},
				url: "/message/event.json/<?= mqweb.qmgr  ?>/SYSTEM.ADMIN.QMGR.EVENT?limit=3",
				cache: false,
				dataType: "json",
				success: function(data)
				{
					self.loading(false);
	
					if ( data.error )
					{
						self.error(data.error);
						self.count(0);
						self.curdepth(0);
					}
					else
					{
						self.events(data.events);
						self.count(data.events.length);
						self.curdepth(data.queues[0].CurrentQDepth.value);
					}
				},
				error: function (request, status, error)
				{
					vm.loading(false);
					//TODO:$("#eventMessages").html("An error occurred  while retrieving event messages from SYSTEM.ADMIN.QMGR.EVENT: " + status  + ", " + error);
				}
			});
		}
	}

	var viewModel = {
		qmgrModel : new QmgrModel(),
		localQueueModel : new LocalQueueModel(),
		xmitQueueModel : new XmitQueueModel(),
		channelModel : new ChannelModel(),
		eventMessageModel : new EventMessageModel()
	};

	$(document).ready(function()
	{
		$(".tip").qtip({ 
			content : { 
				attr : 'alt' 
			}
		});
		
		ko.applyBindings(viewModel);

		viewModel.qmgrModel.load();
		viewModel.localQueueModel.load();
		viewModel.xmitQueueModel.load();
		viewModel.channelModel.load();
		viewModel.eventMessageModel.load();
	});
</script>