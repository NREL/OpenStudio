#require '/usr/local/openstudio-3.6.1/Ruby/openstudio'

include OpenStudio::Model

m = Model.new

l = LoadProfilePlant.new(m)

m.save('test_vt_LoadProfilePlant.osm', true)
