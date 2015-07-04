class AirsController < ApplicationController
  before_action :set_air, only: [:show, :edit, :update, :destroy]

  # GET /airs
  # GET /airs.json
  def index
    @airs = Air.all
    def index
  @h = LazyHighCharts::HighChart.new('pie') do |f|
  f.chart({:defaultSeriesType=>"pie" , :margin=> [50, 200, 60, 170]} )
  series = {
    :type=> 'pie',
    :name=> 'Browser share',
    :data=> [
      ['Firefox', 45.0],
      ['IE', 26.8],
     {
        :name=> 'Chrome', 
        :y=> 12.8,
        :sliced=> true,
        :selected=> true
     },
      ['Safari', 8.5],
      ['Opera', 6.2],
      ['Others', 0.7]
    ]
  }
  f.series(series)
  f.options[:title][:text] = "THA PIE"
  f.legend(:layout=> 'vertical',:style=> {:left=> 'auto', :bottom=> 'auto',:right=> '50px',:top=> '100px'}) 
  f.plot_options(:pie=>{
    :allowPointSelect=>true, 
    :cursor=>"pointer" , 
    :dataLabels=>{
      :enabled=>true,
      :color=>"black",
      :style=>{
        :font=>"13px Trebuchet MS, Verdana, sans-serif"
      }
    }
  })
 end
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
