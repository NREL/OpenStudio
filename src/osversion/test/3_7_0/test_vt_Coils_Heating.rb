#require '/usr/local/openstudio-3.6.1/Ruby/openstudio'

include OpenStudio::Model

m = Model.new

coilHeatingGas = CoilHeatingGas.new(m)
coilHeatingGas.setParasiticElectricLoad(1)
coilHeatingGas.setParasiticGasLoad(2)

coilHeatingGasMultiStage = CoilHeatingGasMultiStage.new(m)
coilHeatingGasMultiStage.setParasiticGasLoad(3)
coilHeatingGasMultiStageStageData = CoilHeatingGasMultiStageStageData.new(m)
coilHeatingGasMultiStageStageData.setParasiticElectricLoad(4)
coilHeatingGasMultiStage.addStage(coilHeatingGasMultiStageStageData)

coilHeatingDesuperheater = CoilHeatingDesuperheater.new(m)
coilHeatingDesuperheater.setParasiticElectricLoad(5)

m.save('test_vt_Coils_Heating.osm', true)
