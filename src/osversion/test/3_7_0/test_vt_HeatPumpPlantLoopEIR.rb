#require '/usr/local/openstudio-3.6.1/Ruby/openstudio'

include OpenStudio::Model

m = Model.new

hp_heating = HeatPumpPlantLoopEIRHeating.new(m)
hp_heating.electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve.setName("HeatEIRfPLR")

hp_cooling = HeatPumpPlantLoopEIRCooling.new(m)
hp_cooling.electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve.setName("CoolEIRfPLR")

m.save('test_vt_HeatPumpPlantLoopEIR.osm', true)
