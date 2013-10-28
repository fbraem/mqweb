<script	type="text/javascript">
var ChannelModel = function()
{
	var self = this;
	
	self.loading = ko.observable(false);
	self.channel = ko.observable(null);
	self.error = ko.observable(null);
	
	self.reload = function()
	{
		$.ajax(
		{
			url: "/channel/view.json/<?= mqweb.qmgr ?>/<?= channelName ?>/<?= channelType ?>",
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
					self.channel(data.channel);
				}
			},
			error: function (request, status, error)
			{
				alert(status);
				//self.loading(false);
				//TODO: Handle Error on page
			}
		});
		
		return false;
	}	
}

var StatusModel = function()
{
  var self = this;
  
  self.loading = ko.observable(false);
  self.statuses = ko.observable(null);
  self.error = ko.observable(null);
  
	self.reload = function()
	{
		$.ajax(
		{
			url: "/chs/view.json/<?= mqweb.qmgr ?>/<?= channelName ?>/<?= channelType ?>",
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
					self.statuses(data.statuses);
				}
			},
			error: function (request, status, error)
			{
				alert(status);
				self.loading(false);
				self.statuses(null);
				//TODO: Handle Error on page
			}
		});
		
		return false;
	}	
}

var viewModel = {
  channelModel : new ChannelModel(),
  statusModel : new StatusModel()
};

$(document).ready(function() {
	ko.applyBindings(viewModel);
	viewModel.channelModel.reload();
	viewModel.statusModel.reload();
});
</script>
<? include "channel.tpl" ?>
