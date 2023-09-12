#require '/usr/local/openstudio-3.6.1/Ruby/openstudio'

include OpenStudio::Model

m = Model.new

boiler = BoilerHotWater.new(m)
boiler.setFuelType("Propane")
boiler.setNominalCapacity(1.0)
boiler.setNominalThermalEfficiency(0.9)
boiler.setEfficiencyCurveTemperatureEvaluationVariable("EnteringBoiler")
curve = CurveQuadratic.new(m)
boiler.setNormalizedBoilerEfficiencyCurve(curve)
boiler.setDesignWaterFlowRate(3.0)
boiler.setMinimumPartLoadRatio(4.0)
boiler.setMaximumPartLoadRatio(5.0)
boiler.setOptimumPartLoadRatio(6.0)
boiler.setWaterOutletUpperTemperatureLimit(7.0)
boiler.setBoilerFlowMode("LeavingSetpointModulated")
boiler.setParasiticElectricLoad(8.0)
boiler.setSizingFactor(9.0)
boiler.setEndUseSubcategory("Test")

m.save('test_vt_BoilerHotWater.osm', true)
