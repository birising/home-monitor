class CreateWinds < ActiveRecord::Migration
  def change
    create_table :winds do |t|
      t.float :speed
      t.integer :dir

      t.timestamps null: false
    end
  end
end
