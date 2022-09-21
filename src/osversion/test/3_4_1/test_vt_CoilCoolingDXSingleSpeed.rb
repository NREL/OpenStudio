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
cc.setMinimumOutdoorDryBulbTemperatureforCompressorOperation(15)
cc.setNominalTimeForCondensateRemovalToBegin(16)
cc.setRatioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity(0.17)
cc.setMaximumCyclingRate(0.18)
cc.setLatentCapacityTimeConstant(19)

cc.setCondenserType("EvaporativelyCooled")
cc.setEvaporativeCondenserEffectiveness(0.22)

cc.setCrankcaseHeaterCapacity(25)
cc.setMaximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation(26)

m.save('test_vt_CoilCoolingDXSingleSpeed.osm', true)
