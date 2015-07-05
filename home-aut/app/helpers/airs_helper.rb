module AirsHelper






	def get_data_for_temperature_graph
		raturn_value = {}
		Air.all.where("created_at > ?", 30.minutes.ago).each do |aquarium|
		  raturn_value[aquarium.created_at] = aquarium.temperature
		end
		raturn_value
	end


def get_data_for_humidity_graph
		raturn_value = {}
		Air.all.where("created_at > ?", 1.days.ago).each do |aquarium|
		  raturn_value[aquarium.created_at] = aquarium.humidity
		end
		raturn_value
	end



	def get_data_for_wind_speed
		raturn_value = {}
		Wind.all.where("created_at > ?", 30.minutes.ago).each do |aquarium|
		  raturn_value[aquarium.created_at] = aquarium.speed
		end
		raturn_value
	end




	def get_data_for_wind_dir
		raturn_value = {}
		Wind.all.where("created_at > ?", 30.minutes.ago).each do |aquarium|
		  raturn_value[aquarium.created_at] = aquarium.dir
		end
		raturn_value
	end









end
