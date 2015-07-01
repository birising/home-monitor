# require 'sidekiq'

# redis = { :namespace => 'leak' }
# Sidekiq.configure_client { |config| config.redis = redis }
# Sidekiq.configure_server { |config| config.redis = redis }

# $c = 0
# $max = 10_000

# # Start up sidekiq via
# # ./bin/sidekiq -r ./examples/leak.rb > /dev/null
# class MyWorker
#   include Sidekiq::Worker




#   def perform
  



#     alert = Air.new do |a| 
#     a.temperature= 80 
#     end
#     alert.save




#   end
# end

# # schedule some jobs to work on
# $max.times { MyWorker.perform_async }

require 'curb'
require 'rubygems'
require 'json'
ENV['RAILS_ENV'] = "development" # Set to your desired Rails environment name
require '/Users/Honza/Documents/PROGRAMING/PRIVATE/home-monitor/home-aut/config/environment.rb'
class Test 

  def run 


    http = Curl.get("192.168.88.250")


  puts http.body_str

  parsed = JSON.parse(http.body_str)
puts "<<<AIR>>>>"
puts parsed["air"]["temperature"]
puts parsed["air"]["humidity"]
puts parsed["air"]["pressure"]
puts "<<<WIND>>>>"
puts parsed["wind"]["speed"]
puts parsed["wind"]["dir"]
puts "<<<RAIN>>>>"
puts parsed["rain"]["total"]


    alert = Air.new do |a| 
    a.temperature = parsed["air"]["temperature"]
    a.humidity = parsed["air"]["humidity"]
    a.pressure = parsed["air"]["pressure"]
    end
    alert.save




  end


end



test = Test.new


test.run()