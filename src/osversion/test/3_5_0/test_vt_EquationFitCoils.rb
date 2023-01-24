#require '/usr/local/openstudio-3.4.0/Ruby/openstudio'

include OpenStudio::Model

def test_vt_CoilHeatingWaterToAirHeatPumpEquationFit
  m = Model.new
  ch = CoilHeatingWaterToAirHeatPumpEquationFit.new(m)
  m.save('test_vt_CoilHeatingWaterToAirHeatPumpEquationFit.osm')
end

def test_vt_CoilCoolingWaterToAirHeatPumpEquationFit
  m = Model.new
  cc = CoilCoolingWaterToAirHeatPumpEquationFit.new(m)
  m.save('test_vt_CoilCoolingWaterToAirHeatPumpEquationFit.osm')
end

test_vt_CoilHeatingWaterToAirHeatPumpEquationFit()
test_vt_CoilCoolingWaterToAirHeatPumpEquationFit()
