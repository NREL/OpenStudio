#require '/usr/local/openstudio-3.4.0/Ruby/openstudio'

include OpenStudio::Model

m = Model.new

coil = CoilHeatingGasMultiStage.new(m)

m.save('test_vt_CoilHeatingGasMultiStage.osm', true)
