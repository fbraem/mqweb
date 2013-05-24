<link rel="stylesheet" href="/static/css/formalize.css" type="text/css" media="all" />
<script src="/static/js/jquery.form.js" type="text/javascript" charset="utf-8"></script>
<script src="/static/js/jquery.formalize.legacy.min.js" type="text/javascript" charset="utf-8"></script>
<script	type="text/javascript">
var QueuesModel = function()
{
	var self = this;
	
	self.loading = ko.observable(false);
	self.queues = ko.observable(null);
	self.error = ko.observable(null);
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
				queuesModel.queues(data.queues);
			}
		}
	};
	$('#queueFilterForm').ajaxForm(options);
	
	$("#filterSubmit").prop("disabled", false);
});
</script>