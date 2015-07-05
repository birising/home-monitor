

class AirsController < ApplicationController
  before_action :set_air, only: [:show, :edit, :update, :destroy]

  # GET /airs
  # GET /airs.json
  def index
    @airs = Air.all


raturn_value = []
data  = []
    Air.all.where("created_at > ?", 1.days.ago).each do |aquarium|
      raturn_value.push (aquarium.temperature)
      data.push([aquarium.created_at.to_date.to_datetime.to_i,aquarium.temperature])
    end
    @startdate = Air.first.created_at




@count = Air.all.count

  @h = LazyHighCharts::HighChart.new('graph') do |f|
    f.options[:title][:text] = "Temperature"
    f.options[:chart][:defaultSeriesType] = "line"
    f.options[:chart][:inverted] = false
    f.options[:chart][:zoomType] = 'x'
    f.options[:legend][:layout] = "horizontal"
    f.options[:legend][:borderWidth] = "0"
    f.options[:yAxis][:title][:text] = @startdate.to_date.to_datetime.to_i
    f.options[:yAxis][:labels][:format] = '{value} °C'
    f.series(  :name => "Temperature", :color => "#2cc9c5", :data => data)
    f.options[:xAxis] = {:minTickInterval => 1, :type => "datetime", :dateTimeLabelFormats => { day: "%b %e"}, :data =>data}

end


raturn_value = []
data  = []
    Air.all.where("created_at > ?", 1.days.ago).each do |aquarium|
      raturn_value.push (aquarium.temperature)
      data.push([aquarium.created_at.to_date.to_datetime.to_i,aquarium.humidity])
    end
    @startdate = Air.first.created_at




@count = Air.all.count

  @a = LazyHighCharts::HighChart.new('graph') do |f|
    f.options[:title][:text] = "Humidity"
    f.options[:chart][:defaultSeriesType] = "line"
    f.options[:chart][:inverted] = false
    f.options[:chart][:zoomType] = 'x'
    f.options[:legend][:layout] = "horizontal"
    f.options[:legend][:borderWidth] = "0"
    f.options[:yAxis][:title][:text] = @startdate.to_date.to_datetime.to_i
    f.options[:yAxis][:labels][:format] = '{value} %'
    f.series(  :name => "Humidity", :color => "#2cc9c5", :data => data)
    f.options[:xAxis] = {:minTickInterval => 1, :type => "datetime", :dateTimeLabelFormats => { day: "%b %e"}, :data =>data}

end




raturn_value = []
data  = []
    Wind.all.where("created_at > ?", 1.days.ago).each do |aquarium|

      data.push([aquarium.created_at.to_date.to_datetime.to_i,aquarium.speed])
    end
    @startdate = Air.first.created_at




@count = Air.all.count

  @speed = LazyHighCharts::HighChart.new('graph') do |f|
    f.options[:title][:text] = "Speed"
    f.options[:chart][:defaultSeriesType] = "line"
    f.options[:chart][:inverted] = false
    f.options[:chart][:zoomType] = 'x'
    f.options[:legend][:layout] = "horizontal"
    f.options[:legend][:borderWidth] = "0"
    f.options[:yAxis][:title][:text] = @startdate.to_date.to_datetime.to_i
    f.options[:yAxis][:labels][:format] = '{value} m/s'
    f.series(  :name => "Speed", :color => "#2cc9c5", :data => data)
    f.options[:xAxis] = {:minTickInterval => 1, :type => "datetime", :dateTimeLabelFormats => { day: "%b %e"}, :data =>data}

end

  end






  # GET /airs/1
  # GET /airs/1.json
  def show
  end

  # GET /airs/new
  def new
    @air = Air.new
  end

  # GET /airs/1/edit
  def edit
  end

  # POST /airs
  # POST /airs.json
  def create
    @air = Air.new(air_params)

    respond_to do |format|
      if @air.save
        format.html { redirect_to @air, notice: 'Air was successfully created.' }
        format.json { render :show, status: :created, location: @air }
      else
        format.html { render :new }
        format.json { render json: @air.errors, status: :unprocessable_entity }
      end
    end
  end

  # PATCH/PUT /airs/1
  # PATCH/PUT /airs/1.json
  def update
    respond_to do |format|
      if @air.update(air_params)
        format.html { redirect_to @air, notice: 'Air was successfully updated.' }
        format.json { render :show, status: :ok, location: @air }
      else
        format.html { render :edit }
        format.json { render json: @air.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /airs/1
  # DELETE /airs/1.json
  def destroy
    @air.destroy
    respond_to do |format|
      format.html { redirect_to airs_url, notice: 'Air was successfully destroyed.' }
      format.json { head :no_content }
    end
  end

  private
    # Use callbacks to share common setup or constraints between actions.
    def set_air
      @air = Air.find(params[:id])
    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def air_params
      params.require(:air).permit(:temperature, :humidity, :pressure)
    end
end
