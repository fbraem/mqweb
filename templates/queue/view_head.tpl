<script	type="text/javascript">
var QueueModel = function()
{
	var self = this;
	
	self.loading = ko.observable(false);
	self.queue = ko.observable(null);
	self.error = ko.observable(null);
	
	self.reload = function()
	{
		$.ajax(
		{
			url: "/queue/view.json/<?= mqweb.qmgr ?>/<?= queueName ?>",
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
					self.queue(data.queue);
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

var queueModel = new QueueModel();

$(document).ready(function() {
	ko.applyBindings(queueModel);
	queueModel.reload();
});
</script>
<? include "queue.tpl" ?>
