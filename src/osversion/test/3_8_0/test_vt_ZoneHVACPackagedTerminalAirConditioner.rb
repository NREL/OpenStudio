#require '/usr/local/openstudio-3.7.0/Ruby/openstudio'

include OpenStudio::Model

m = Model.new

ptac_hc = CoilHeatingElectric.new(m)
ptac_cc = CoilCoolingDXSingleSpeed.new(m)
ptac_fan = FanConstantVolume.new(m)
ptac = ZoneHVACPackagedTerminalAirConditioner.new(m, m.alwaysOnDiscreteSchedule, ptac_fan, ptac_hc, ptac_cc)

m.save('test_vt_ZoneHVACPackagedTerminalAirConditioner.osm', true)
