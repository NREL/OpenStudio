#require '/usr/local/openstudio-3.5.0/Ruby/openstudio'

include OpenStudio::Model

m = Model.new

coil = GroundHeatExchangerHorizontalTrench.new(m)

m.save('test_vt_GroundHeatExchangerHorizontalTrench.osm', true)
