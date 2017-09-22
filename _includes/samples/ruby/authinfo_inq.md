{% highlight ruby %}
require 'net/http'
require 'json'

uri = URI('http://localhost:8081/api/authinfo/inquire/PIGEON')
http = Net::HTTP.new(uri.host, uri.port)

begin
	req = Net::HTTP::Post.new(uri.path, initheader = {'Content-Type' =>'application/json'})
	req.body = { AuthInfoName: '*' }.to_json
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
		json["data"].each do |authinfo|
			puts authinfo["AuthInfoName"]["value"]
		end
	end
else
	puts "A problem occurred with HTTP request: #{response.code}"
end
{% endhighlight %}
