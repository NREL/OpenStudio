#require '/usr/local/openstudio-3.4.0/Ruby/openstudio'

include OpenStudio::Model

m = Model.new

pthp_hc = CoilHeatingDXSingleSpeed.new(m)
pthp_cc = CoilCoolingDXSingleSpeed.new(m)
pthp_suppHC = CoilHeatingElectric.new(m)
pthp_fan = FanConstantVolume.new(m)
pthp_fan.setName("PTHP Fan")
pthp = ZoneHVACPackagedTerminalHeatPump.new(m, m.alwaysOnDiscreteSchedule, pthp_fan, pthp_hc, pthp_cc, pthp_suppHC)
pthp.resetSupplyAirFanOperatingModeSchedule

ptac_hc = CoilHeatingElectric.new(m)
ptac_cc = CoilCoolingDXSingleSpeed.new(m)
ptac_fan = FanConstantVolume.new(m)
ptac_fan.setName("PTAC Fan")
ptac = ZoneHVACPackagedTerminalAirConditioner.new(m, m.alwaysOnDiscreteSchedule, ptac_fan, ptac_hc, ptac_cc)
ptac.resetSupplyAirFanOperatingModeSchedule

# Leaving the pthp fan will all defaults, harcoding this one so we test both
# cases and ensure the resulting Fan:SystemModel is correct
ptac_fan.setFanTotalEfficiency(0.75)
ptac_fan.setPressureRise(265)
ptac_fan.setMaximumFlowRate(0.5)
ptac_fan.setMotorEfficiency(0.89)
ptac_fan.setMotorInAirstreamFraction(0.92)
ptac_fan.setEndUseSubcategory("PTAC Fans")

m.save('test_vt_ZoneHVACPackaged.osm', true)
