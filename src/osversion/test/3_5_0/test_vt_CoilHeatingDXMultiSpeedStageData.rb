#require '/usr/local/openstudio-3.4.0/Ruby/openstudio'

include OpenStudio::Model

m = Model.new

stage1 = CoilHeatingDXMultiSpeedStageData.new(m)

m.save('test_vt_CoilHeatingDXMultiSpeedStageData.osm', true)
