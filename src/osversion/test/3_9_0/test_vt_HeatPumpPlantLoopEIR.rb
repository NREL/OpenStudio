#require '/usr/local/openstudio-3.8.0/Ruby/openstudio'

include OpenStudio::Model

m = Model.new

heating = HeatPumpPlantLoopEIRHeating.new(m)
cooling = HeatPumpPlantLoopEIRCooling.new(m)

m.save('test_vt_HeatPumpPlantLoopEIR.osm', true)
