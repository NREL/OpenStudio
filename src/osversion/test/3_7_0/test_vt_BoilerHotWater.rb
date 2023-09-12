#require '/usr/local/openstudio-3.6.1/Ruby/openstudio'

include OpenStudio::Model

m = Model.new

boiler = BoilerHotWater.new(m)

m.save('test_vt_BoilerHotWater.osm', true)
