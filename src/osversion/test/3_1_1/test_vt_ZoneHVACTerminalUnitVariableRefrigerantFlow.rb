# require '/usr/local/openstudio-3.1.0/Ruby/openstudio'

include OpenStudio::Model

m = Model.new

vrf = ZoneHVACTerminalUnitVariableRefrigerantFlow.new(m)
vrf.supplyAirFanOperatingModeSchedule.setName("Fan Op Schedule")
vrf.supplyAirFan.setName("Supply Air Fan")
m.save('test_vt_ZoneHVACTerminalUnitVariableRefrigerantFlow.osm', true)
