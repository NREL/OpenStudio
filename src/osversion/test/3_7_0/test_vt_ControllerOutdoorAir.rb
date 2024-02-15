#require '/usr/local/openstudio-3.6.1/Ruby/openstudio'

include OpenStudio::Model

m = Model.new

controller = ControllerOutdoorAir.new(m)
controller.autosizeMaximumOutdoorAirFlowRate
controller.setMinimumOutdoorAirFlowRate(0.0)
controller.setEconomizerControlType("NoEconomizer")
controller.setEconomizerControlActionType("ModulateFlow")
controller.setEconomizerMaximumLimitDryBulbTemperature(28.0)
controller.setEconomizerMaximumLimitEnthalpy(64000.0)
controller.setEconomizerMinimumLimitDryBulbTemperature(-100.0)
controller.setLockoutType("NoLockout")
controller.setMinimumLimitType("FixedMinimum")
controller.setHighHumidityControl(false)
controller.setHeatRecoveryBypassControlType("BypassWhenWithinEconomizerLimits")

m.save('test_vt_ControllerOutdoorAir.osm', true)
