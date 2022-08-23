#require '/usr/local/openstudio-3.2.1/Ruby/openstudio'

include OpenStudio::Model

m = Model.new

a = AirLoopHVAC.new(m)
sz = a.sizingSystem
sz.setHeatingDesignAirFlowRate(1.2)
sz.setSystemOutdoorAirMethod('VentilationRateProcedure')
sz.setZoneMaximumOutdoorAirFraction(1.0)

m.save('test_vt_SizingSystem.osm', true)
