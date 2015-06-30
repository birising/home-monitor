json.array!(@winds) do |wind|
  json.extract! wind, :id, :speed, :dir
  json.url wind_url(wind, format: :json)
end
