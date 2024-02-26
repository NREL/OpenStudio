#require '/usr/local/openstudio-3.7.0/Ruby/openstudio'

include OpenStudio::Model

m = Model.new

hx = HeatExchangerAirToAirSensibleAndLatent.new(m)

m.save('test_vt_HeatExchangerAirToAirSensibleAndLatent.osm', true)