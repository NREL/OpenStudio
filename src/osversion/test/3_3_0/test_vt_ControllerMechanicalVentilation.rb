#require '/usr/local/openstudio-3.2.1/Ruby/openstudio'

include OpenStudio::Model

m = Model.new

c = ControllerMechanicalVentilation.new(m)
c.setSystemOutdoorAirMethod('VentilationRateProcedure')

m.save('test_vt_ControllerMechanicalVentilation.osm', true)
