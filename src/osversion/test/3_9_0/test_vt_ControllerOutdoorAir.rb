#require '/usr/local/openstudio-3.8.0/Ruby/openstudio'

include OpenStudio::Model

m = Model.new

controller = ControllerOutdoorAir.new(m)

m.save('test_vt_ControllerOutdoorAir.osm', true)
