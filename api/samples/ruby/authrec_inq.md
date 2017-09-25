{% highlight ruby %}
require 'net/http'
require 'json'

uri = URI('http://localhost:8081/api/authrec/inquire/PIGEON')
http = Net::HTTP.new(uri.host, uri.port)

begin
	req = Net::HTTP::Post.new(uri.path, initheader = {'Content-Type' =>'application/json'})
	req.body = {
		Options: [ 
			'Name All Matching', 
			'Entity Explicit'
		]
	}.to_json
	response = http.request(req)
rescue Errno::ECONNREFUSED
	puts "Can't connect to MQWeb is it running?"
	abort
end

if response.code == "200"
	json = JSON.parse(response.body)
	if json["error"]
		puts "An MQException occurred: RC= #{json["error"]["reason"]["code"]} - #{json["error"]["reason"]["desc"]}"
	else
		json["data"].each do |authrec|
			authorizations = []
			authrec['AuthorizationList']['value'].each do |authority|
				authorizations << authority['text']
			end
			output = authorizations.join(', ')
			puts "#{authrec['ProfileName']['value']}(#{authrec['ObjectType']['text']}) - #{authrec['EntityName']['value']} : #{output}"
		end
	end
else
	puts "A problem occurred with HTTP request: #{response.code}"
end
{% endhighlight %}
