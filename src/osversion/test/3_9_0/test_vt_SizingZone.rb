#require '/usr/local/openstudio-3.8.0/Ruby/openstudio'

include OpenStudio::Model

m = Model.new

z = ThermalZone.new(m)
sz = z.sizingZone

m.save('test_vt_SizingZone.osm', true)
