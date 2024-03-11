#require '/usr/local/openstudio-3.7.0/Ruby/openstudio'

include OpenStudio::Model

m = Model.new

ptac_hc = CoilHeatingElectric.new(m)
ptac_cc = CoilCoolingDXSingleSpeed.new(m)
ptac_fan = FanConstantVolume.new(m)
ptac = ZoneHVACPackagedTerminalAirConditioner.new(m, m.alwaysOnDiscreteSchedule, ptac_fan, ptac_hc, ptac_cc)

pthp_hc = CoilHeatingDXSingleSpeed.new(m)
pthp_cc = CoilCoolingDXSingleSpeed.new(m)
pthp_suppHC = CoilHeatingElectric.new(m)
pthp_fan = FanConstantVolume.new(m)
pthp = ZoneHVACPackagedTerminalHeatPump.new(m, m.alwaysOnDiscreteSchedule, pthp_fan, pthp_hc, pthp_cc, pthp_suppHC)

supplyFan = OpenStudio::Model::FanOnOff.new(m)
cc = CoilCoolingWaterToAirHeatPumpEquationFit.new(m)
hc = CoilHeatingWaterToAirHeatPumpEquationFit.new(m)
supplementalHC = OpenStudio::Model::CoilHeatingElectric.new(m)
wahp = OpenStudio::Model::ZoneHVACWaterToAirHeatPump.new(m, m.alwaysOnDiscreteSchedule, supplyFan, hc, cc, supplementalHC)

unitary = AirLoopHVACUnitarySystem.new(m)

m.save('test_vt_NoLoadSupplyAirFlowRateControlSetToLowSpeed.osm', true)
