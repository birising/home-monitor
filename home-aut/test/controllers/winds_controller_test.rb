require 'test_helper'

class WindsControllerTest < ActionController::TestCase
  setup do
    @wind = winds(:one)
  end

  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:winds)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create wind" do
    assert_difference('Wind.count') do
      post :create, wind: { dir: @wind.dir, speed: @wind.speed }
    end

    assert_redirected_to wind_path(assigns(:wind))
  end

  test "should show wind" do
    get :show, id: @wind
    assert_response :success
  end

  test "should get edit" do
    get :edit, id: @wind
    assert_response :success
  end

  test "should update wind" do
    patch :update, id: @wind, wind: { dir: @wind.dir, speed: @wind.speed }
    assert_redirected_to wind_path(assigns(:wind))
  end

  test "should destroy wind" do
    assert_difference('Wind.count', -1) do
      delete :destroy, id: @wind
    end

    assert_redirected_to winds_path
  end
end
