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

var channelModel = new ChannelModel();

$(document).ready(function() {
	ko.applyBindings(channelModel);
	channelModel.reload();
});
</script>
<? include "channel.tpl" ?>
