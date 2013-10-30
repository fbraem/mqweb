<script	type="text/javascript">
var MessageModel = function()
{
	var self = this;
	
	self.loading = ko.observable(false);
	self.message = ko.observable(null);
	self.error = ko.observable(null);
	self.ascii = ko.observable(true);
	self.ebcdic = ko.observable(true);
	
	self.reload = function()
	{
		$.ajax(
		{
			url: "/message/dump.json/<?= mqweb.qmgr ?>/<?= queueName ?>/<?= messageId ?>",
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
	
	self.showAscii = function(data, event)
	{
		self.ascii(! self.ascii());
	}

	self.showEbcdic = function(data, event)
	{
		self.ebcdic(! self.ebcdic());
	}
	
}

var messageModel = new MessageModel();

ko.bindingHandlers.button = {
	init: function(element, valueAccessor, allBindingsAccessor, viewModel)
	{
		$(element).button();
	}	
};

$(document).ready(function() {
	ko.applyBindings(messageModel);
	messageModel.reload();
});
</script>
<script type="text/javascript"> 
  	$(function() 
  	{
	  	$('#messageDumpTable > tbody > tr:odd').css('background-color', '#E2FEFE');
	  	$('#messageDumpTable > tbody > tr:even').css('background-color', '#FDFDEB');
  	});
</script>