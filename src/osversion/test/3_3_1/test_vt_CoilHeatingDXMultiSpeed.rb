#require '/usr/local/openstudio-3.3.0/Ruby/openstudio'

include OpenStudio::Model

m = Model.new

coil = CoilHeatingDXMultiSpeed.new(m)

stage1 = CoilHeatingDXMultiSpeedStageData.new(m)
coil.addStage(stage1)

stage2 = CoilHeatingDXMultiSpeedStageData.new(m)
coil.addStage(stage2)

stage3 = CoilHeatingDXMultiSpeedStageData.new(m)
coil.addStage(stage3)

stage4 = CoilHeatingDXMultiSpeedStageData.new(m)
coil.addStage(stage4)

m.save('test_vt_CoilHeatingDXMultiSpeed.osm', true)
