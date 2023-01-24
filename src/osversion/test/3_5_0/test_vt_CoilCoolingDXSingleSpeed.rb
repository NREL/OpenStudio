#require '/usr/local/openstudio-3.4.0/Ruby/openstudio'

include OpenStudio::Model

m = Model.new

cc1 = CoilCoolingDXSingleSpeed.new(m)
cc1.setName("CC with blanks")
[16, 17, 18, 19].each { |i|
  raise if !cc1.isEmpty(i)
}

cc = CoilCoolingDXSingleSpeed.new(m)
cc.setName("CC with numbers")
cc.setMinimumOutdoorDryBulbTemperatureforCompressorOperation(16)
cc.setNominalTimeForCondensateRemovalToBegin(17)
cc.setRatioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity(0.18)
cc.setMaximumCyclingRate(0.19)
cc.setLatentCapacityTimeConstant(20)

cc.setCondenserType("EvaporativelyCooled")
cc.setEvaporativeCondenserEffectiveness(0.23)

cc.setCrankcaseHeaterCapacity(26)
cc.setMaximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation(27)

m.save('test_vt_CoilCoolingDXSingleSpeed.osm', true)
