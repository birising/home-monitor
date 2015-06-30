require 'test_helper'

class AirsControllerTest < ActionController::TestCase
  setup do
    @air = airs(:one)
  end

  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:airs)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create air" do
    assert_difference('Air.count') do
      post :create, air: { humidity: @air.humidity, pressure: @air.pressure, temperature: @air.temperature, time_measurement: @air.time_measurement }
    end

    assert_redirected_to air_path(assigns(:air))
  end

  test "should show air" do
    get :show, id: @air
    assert_response :success
  end

  test "should get edit" do
    get :edit, id: @air
    assert_response :success
  end

  test "should update air" do
    patch :update, id: @air, air: { humidity: @air.humidity, pressure: @air.pressure, temperature: @air.temperature, time_measurement: @air.time_measurement }
    assert_redirected_to air_path(assigns(:air))
  end

  test "should destroy air" do
    assert_difference('Air.count', -1) do
      delete :destroy, id: @air
    end

    assert_redirected_to airs_path
  end
end
