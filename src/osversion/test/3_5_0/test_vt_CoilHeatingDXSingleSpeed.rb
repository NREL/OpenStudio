#require '/usr/local/openstudio-3.4.0/Ruby/openstudio'

include OpenStudio::Model

m = Model.new

coil = CoilHeatingDXSingleSpeed.new(m)

m.save('test_vt_CoilHeatingDXSingleSpeed.osm', true)
