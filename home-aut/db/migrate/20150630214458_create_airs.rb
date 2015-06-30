class CreateAirs < ActiveRecord::Migration
  def change
    create_table :airs do |t|
      t.float :temperature
      t.float :humidity
      t.float :pressure
      t.date :time_measurement

      t.timestamps null: false
    end
  end
end
