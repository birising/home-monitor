module AirsHelper






	def get_data_for_temperature_graph
		raturn_value = {}
		Air.all.each do |aquarium|
		  raturn_value[aquarium.created_at] = aquarium.temperature
		end
		raturn_value
	end
end
