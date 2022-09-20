#require '/usr/local/openstudio-3.4.0/Ruby/openstudio'

include OpenStudio::Model

m = Model.new

coil = CoilCoolingDXSingleSpeed.new(m)

m.save('test_vt_CoilCoolingDXSingleSpeed.osm', true)
