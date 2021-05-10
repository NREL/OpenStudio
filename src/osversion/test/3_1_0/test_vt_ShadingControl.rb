require '/usr/local/openstudio-3.0.1/Ruby/openstudio'

include OpenStudio::Model

m = Model.new

pts = OpenStudio::Point3dVector.new;

pts << OpenStudio::Point3d.new(0, 0, 0)
pts << OpenStudio::Point3d.new(0, 1, 0)
pts << OpenStudio::Point3d.new(1, 1, 0)
pts << OpenStudio::Point3d.new(1, 0, 0)

ss = OpenStudio::Model::SubSurface.new(pts, m)
c = Construction.new(m)
ss.setConstruction(c)
ss.setName("SubSurface")

sc = ShadingControl.new(c)
sc.setName("ShadingControl")
ss.setShadingControl(sc)
ss.setViewFactortoGround(0.5)

m.save('test_vt_ShadingControl.osm', true)

