<script type="text/javascript">
	var QmgrModel = function()
	{
		var self = this;

		self.mqweb = ko.observable(null);
		self.qmgr = ko.observable(null);
		self.error = ko.observable(null);
		self.loading = ko.observable(false);

		this.load = function()
			{
				$.ajax(
				{
					beforeSend: function() {
						self.mqweb(null);
						self.qmgr(null);
						self.error(null);
						self.loading(true);
					},
					url: "/qmgr/view.json/<?= mqweb.qmgr ?>",
					cache: false,
					dataType: "json",
					success: function(data) {
						self.loading(false);
						self.mqweb(data.mqweb);
						self.qmgr(data.qmgr);
						self.error(data.error);
					},
					error: function (request, status, error)
					{
						self.loading(false);
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
						queueUsage : "normal",
						queueExcludeTemp : 1
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
							if ( data.queues.length > 0 )
							{
								for(q in data.queues)
								{
									data.queues[q].url = '/queue/view/<?=mqweb.qmgr?>/' + data.queues[q].QName.value;
								}
								self.queues(data.queues);
							}
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
							if ( data.queues.length > 0 )
							{
								for(q in data.queues)
								{
									data.queues[q].url = '/queue/view/<?=mqweb.qmgr?>/' + data.queues[q].QName.value;
								}
								self.queues(data.queues);
							}
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
						if ( data.statuses.length > 0 )
						{
							// Only keep unique channel statuses
							var statuses = new Array();
							var channels = new Array();

							for(status in data.statuses)
							{
							if ( $.inArray(data.statuses[status].ChannelName.value, channels) == -1 )
							{
								channels.push(data.statuses[status].ChannelName.value);
								statuses.push(data.statuses[status]);
							}
							}

							self.channels(statuses);
						}
					}
				},
				error: function (request, status, error)
				{
					self.loading(false);
					//TODO: $("#channelStatus").html("An error occurred while retrieving channel status: " + status + ", " + error);
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
					self.count(0);
					self.curdepth(0);
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
					}
					else
					{
						self.events(data.events);
						self.count(data.events.length);
						self.curdepth(data.queue.curdepth);

						eventTips();
					}
				},
				error: function (request, status, error)
				{
					self.loading(false);
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
		$("[data-qtip]").qtip({
			content : {
				text: function(event, api)
					{
			return $(this).attr('data-qtip');
			},
		title: function(event, api)
					{
			return $(this).text();
			}
			},
			position: {
				my: 'bottom center',
				at: 'top center'
			}
		});
		$(".imgtip").qtip({
			content : {
				attr: 'alt'
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
<script type="text/javascript">
function eventTips()
{
	$('[data-url]').qtip({
		position: {
			my: 'bottom left',
			at: 'top right'
		},
		content: {
			text: function(event, api) {
				$.ajax({
					url: $(this).data('url') // Use data-url attribute for the URL
				})
				.then(function(content) {
					// Set the tooltip content upon successful retrieval
					var contentDOM = $(content);
					// When a title id is found, use it as qtip title and make
					// the title invisible.
					var titleElement = contentDOM.find("#eventTitle");
					if ( titleElement )
					{
						titleElement.hide();
						api.set('content.title', titleElement.text());
					}
					api.set('content.text', contentDOM.html());
				}, function(xhr, status, error) {
					// Upon failure... set the tooltip content to the status and error value
					api.set('content.text', status + ': ' + error);
				});

				return 'Loading...'; // Set some initial text
			}
		}
	});
}
</script>
