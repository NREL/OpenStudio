require '/usr/local/openstudio-3.2.0/Ruby/openstudio'

include OpenStudio::Model

m = Model.new

wh1 = WaterHeaterMixed.new(m)
wh2 = WaterHeaterStratified.new(m)

m.save('test_vt_WaterHeaterSizing.osm', true)
