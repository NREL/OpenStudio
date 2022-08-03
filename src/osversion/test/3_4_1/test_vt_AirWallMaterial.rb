#require '/usr/local/openstudio-3.4.0/Ruby/openstudio'

include OpenStudio::Model

m = Model.new

airWall = AirWallMaterial.new(m)

constr = Construction.new(m)
constr.setLayers([airWall])

points = OpenStudio::Point3dVector.new
points << OpenStudio::Point3d.new(0, 1, 0)
points << OpenStudio::Point3d.new(0, 0, 0)
points << OpenStudio::Point3d.new(1, 0, 0)
points << OpenStudio::Point3d.new(1, 1, 0)

surface = Surface.new(points, m)

surface.setConstruction(constr)

m.save('test_vt_AirWallMaterial.osm', true)
