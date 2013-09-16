<link rel="stylesheet" href="/static/css/formalize.css" type="text/css" media="all" />
<script src="/static/js/jquery.form.js" type="text/javascript" charset="utf-8"></script>
<script src="/static/js/jquery.formalize.legacy.min.js" type="text/javascript" charset="utf-8"></script>
<script	type="text/javascript">
var MessagesModel = function()
{
	var self = this;
	
	self.loading = ko.observable(false);
	self.messages = ko.observableArray(null);
	self.error = ko.observable(null);
	self.toggle = function(item, event)
	{
		var context = ko.contextFor(event.target);
		context.$data.toggle(!context.$data.toggle());
		return false;
	};
}

var messagesModel = new MessagesModel();

$(document).ready(function() {
	ko.applyBindings(messagesModel);

	var options = {
		beforeSubmit : function() {
				messagesModel.loading(true);
				messagesModel.messages(null);
				messagesModel.error(null);
				return true;
			},
		success: function(data) {
			messagesModel.loading(false);
			if ( data.error )
			{
				messagesModel.error(data.error);
			}
			else
			{
				messagesModel.messages(data.messages);
			}
		}
	};
	$('#messageFilterForm').ajaxForm(options);
	
	$("#filterSubmit").prop("disabled", false);
});
</script>