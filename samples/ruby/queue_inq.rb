require 'net/http'
require 'uri'
require 'json'
 
uri = URI.parse('http://localhost:8081/api/queue/inquire/PIGEON')
 
http = Net::HTTP.new(uri.host, uri.port)

begin
	response = http.request(Net::HTTP::Get.new(uri.request_uri))
rescue Errno::ECONNREFUSED
	puts "Can't connect to MQWeb. Is it running?"
	abort
end

if response.code == "200"
	json = JSON.parse(response.body)
	if json["error"]
		puts "An MQException occurred: RC= #{json["error"]["reason"]["code"]} - #{json["error"]["reason"]["desc"]}"
	else
		json["data"].each do |queue|
			puts queue["QName"]["value"]
		end
	end
else
	puts "A problem occurred with HTTP request: #{response.code}"
end
