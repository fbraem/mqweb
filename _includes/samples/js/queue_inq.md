{% highlight js %}
var http = require('http');

var options = {
	hostname : '127.0.0.1',
	port : 8081,
	path : '/api/queue/inquire/PIGEON',
	method : 'GET',
	headers : { 
	}
};

var req = http.request(options, function(res) {

	var output = '';

	res.setEncoding('utf8');

	res.on('data', function (data) {
		output += data;
	});

	res.on('end', function() {
		var json = JSON.parse(output);
		if ( json.error ) {
			console.log("An MQException occurred: RC= " + json.error.reason.code + " - " + json.error.reason.desc);
		}
		else {
			if ( json.data ) {
				for(q in json.data) {
					console.log(json.data[q].QName.value);
				}
			}
		}
	});
});

req.on('error', function(e) {
	console.log('problem with request: ' + e.message);
});

req.end();
{% endhighlight %}
