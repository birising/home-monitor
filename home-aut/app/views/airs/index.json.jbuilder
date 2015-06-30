json.array!(@airs) do |air|
  json.extract! air, :id, :temperature, :humidity, :pressure, :time_measurement
  json.url air_url(air, format: :json)
end
