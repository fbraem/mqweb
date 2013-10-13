<script	type="text/javascript">
var MessageModel = function()
{
	var self = this;
	
	self.loading = ko.observable(false);
	self.message = ko.observable(null);
	self.error = ko.observable(null);
	
	self.reload = function()
	{
		$.ajax(
		{
			url: "/message/view.json/<?= mqweb.qmgr ?>/<?= queueName ?>/<?= messageId ?>",
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
					self.message(data.message);
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

var model = new MessageModel();

$(document).ready(function() {
	ko.applyBindings(model);
	model.reload();
});
</script>
<? include "message.tpl" ?>
