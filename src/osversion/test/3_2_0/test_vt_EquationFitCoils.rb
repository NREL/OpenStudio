require '/usr/local/openstudio-3.1.0/Ruby/openstudio'

include OpenStudio::Model

def test_vt_CoilHeatingWaterToAirHeatPumpEquationFit
  m = Model.new
  ch = CoilHeatingWaterToAirHeatPumpEquationFit.new(m)
  ch.setRatedHeatingCoefficientofPerformance(4.5)
  m.save('test_vt_CoilHeatingWaterToAirHeatPumpEquationFit.osm')
end

def test_vt_CoilCoolingWaterToAirHeatPumpEquationFit
  m = Model.new
  cc = CoilCoolingWaterToAirHeatPumpEquationFit.new(m)
  cc.setRatedCoolingCoefficientofPerformance(4.2)
  cc.setNominalTimeforCondensateRemovaltoBegin(360)
  cc.setRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity(0.1)
  m.save('test_vt_CoilCoolingWaterToAirHeatPumpEquationFit.osm')
end


def test_vt_HeatPumpWaterToWaterEquationFitHeating
  m = Model.new
  hph = HeatPumpWaterToWaterEquationFitHeating.new(m)
  m.save('test_vt_HeatPumpWaterToWaterEquationFitHeating.osm')
end

def test_vt_HeatPumpWaterToWaterEquationFitCooling
  m = Model.new
  hpc = HeatPumpWaterToWaterEquationFitCooling.new(m)
  m.save('test_vt_HeatPumpWaterToWaterEquationFitCooling.osm')
end

test_vt_CoilHeatingWaterToAirHeatPumpEquationFit()
test_vt_CoilCoolingWaterToAirHeatPumpEquationFit()
test_vt_HeatPumpWaterToWaterEquationFitHeating()
test_vt_HeatPumpWaterToWaterEquationFitCooling()
