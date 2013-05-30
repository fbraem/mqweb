<link rel="stylesheet" href="/static/css/formalize.css" type="text/css" media="all" />
<script src="/static/js/jquery.form.js" type="text/javascript" charset="utf-8"></script>
<script src="/static/js/jquery.formalize.legacy.min.js" type="text/javascript" charset="utf-8"></script>
<script	type="text/javascript">
var QueuesModel = function()
{
	var self = this;
	
	self.loading = ko.observable(false);
	self.queues = ko.observableArray(null);
	self.error = ko.observable(null);
	
	self.toggle = function(item, event)
	{
		var context = ko.contextFor(event.target);
		context.$data.toggle(!context.$data.toggle());

		var rows = $(event.target).parents("tr");
		if ( context.$data.toggle() )
		{
			rows.first().children("td").css("border-bottom-style", "none");
		}
		else
		{
			rows.first().children("td").css("border-bottom-style", "solid");
		}
		return false;
	};
	
	self.reload = function(data, event)
	{
		var context = ko.contextFor(event.target);
		
		$.ajax(
		{
			url: "/queue/list.json/<?= mqweb.qmgr ?>?queueName=" + context.$data.QName.value,
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
					data.queues[0].toggle = ko.observable(true);
					queuesModel.queues.replace(queuesModel.queues()[context.$index()], data.queues[0]);

					var rows = $(event.target).parents("tr");
					rows.children("td").css("border-bottom-style", "none");
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

var queuesModel = new QueuesModel();

$(document).ready(function() {
	ko.applyBindings(queuesModel);

	var options = {
		beforeSubmit : function() {
				queuesModel.loading(true);
				queuesModel.queues(null);
				queuesModel.error(null);
				return true;
			},
		success: function(data) {
			queuesModel.loading(false);
			if ( data.error )
			{
				queuesModel.error(data.error);
			}
			else
			{
				if ( data.queues.length > 0 )
				{
					for(q in data.queues)
					{
						data.queues[q].toggle = ko.observable(false);
					}
					queuesModel.queues(data.queues);
				}
			}
		}
	};
	$('#queueFilterForm').ajaxForm(options);
	
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
