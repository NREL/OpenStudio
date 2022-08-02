#require '/usr/local/openstudio-3.4.0/Ruby/openstudio'

include OpenStudio::Model

m = Model.new

airWall = AirWallMaterial.new(m)

constr = Construction.new(m)
constr.setLayers([airWall])

m.save('test_vt_AirWallMaterial.osm', true)
