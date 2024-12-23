#require '/usr/local/openstudio-3.9.0/Ruby/openstudio'

include OpenStudio::Model

m = Model.new

ghe_vert = GroundHeatExchangerVertical.new(m)

m.save('test_vt_GroundHeatExchangerVertical.osm', true)
