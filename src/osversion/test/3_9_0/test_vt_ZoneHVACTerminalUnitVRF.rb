#require '/usr/local/openstudio-3.8.0/Ruby/openstudio'

include OpenStudio::Model

m = Model.new

cc = CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl.new(m)
hc = CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl.new(m)
fanVV = FanVariableVolume.new(m);
vrf = ZoneHVACTerminalUnitVariableRefrigerantFlow.new(m, cc, hc, fanVV)

m.save('test_vt_ZoneHVACTerminalUnitVRF.osm', true)
