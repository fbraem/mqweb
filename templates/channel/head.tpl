<link rel="stylesheet" href="/static/css/formalize.css" type="text/css" media="all" />
<script src="/static/js/jquery.form.js" type="text/javascript" charset="utf-8"></script>
<script src="/static/js/jquery.formalize.legacy.min.js" type="text/javascript" charset="utf-8"></script>
<script	type="text/javascript">
var ChannelsModel = function()
{
	var self = this;
	
	self.loading = ko.observable(false);
	self.channels = ko.observableArray(null);
	self.error = ko.observable(null);
	
	self.toggle = function(item, event)
	{
		var context = ko.contextFor(event.target);
		context.$data.toggle(!context.$data.toggle());
		return false;
	};
	
	self.reload = function(data, event)
	{
		var context = ko.contextFor(event.target);

		$.ajax(
		{
			url: "/channel/list.json/<?= mqweb.qmgr ?>?channelName=" + context.$data.ChannelName.value,
			cache: false,
			dataType: "json",
			success: function(data) {
				//self.loading(false);
				if ( data.error )
				{
					//self.error(data.error);
				}
				else
				{
					data.channels[0].toggle = ko.observable(true);
					channelsModel.channels.replace(channelsModel.channels()[context.$index()], data.channels[0]);
				}
			},
			error: function (request, status, error)
			{
				alert(error);
				//self.loading(false);
				//TODO: Handle Error on page
			}
		});
		
		return false;
	}	
}

var channelsModel = new ChannelsModel();

$(document).ready(function() {
	ko.applyBindings(channelsModel);

	var options = {
		beforeSubmit : function() {
				channelsModel.loading(true);
				channelsModel.channels(null);
				channelsModel.error(null);
				return true;
			},
		success: function(data) {
			channelsModel.loading(false);
			if ( data.error )
			{
				channelsModel.error(data.error);
			}
			else
			{
				if ( data.channels.length > 0 )
				{
					for(q in data.channels)
					{
						data.channels[q].toggle = ko.observable(false);
					}
					channelsModel.channels(data.channels);
				}
			}
		}
	};
	$('#channelFilterForm').ajaxForm(options);
	
	$("#filterSubmit").prop("disabled", false);
});
</script>
<style>
	table tr:nth-child(even)
	{
		background-color:white;
	}
	table tr:nth-child(odd) td
	{
	}
	table tr:nth-child(4n+3)
	{
		background-color:#f9f9f9;
	}
	table tr:nth-child(4n+4)
	{
		background-color:#f9f9f9;
	}
</style>
