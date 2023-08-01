#require '/usr/local/openstudio-3.6.1/Ruby/openstudio'

include OpenStudio::Model

m = Model.new

coil = GroundHeatExchangerVertical.new(m)

m.save('test_vt_GroundHeatExchangerVertical.osm', true)
