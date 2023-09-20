#require '/usr/local/openstudio-3.6.1/Ruby/openstudio'

include OpenStudio::Model

m = Model.new

dh = DistrictHeating.new(m)
dh.setName("My DistrictHeating")
p = PlantLoop.new(m)
p.addSupplyBranchForComponent(dh)
dh.inletModelObject.get.setName("DH Inlet")
dh.outletModelObject.get.setName("DH Outlet")
dh.setNominalCapacity(1000.0)

m.save('test_vt_RenameDistrictHeating.osm', true)
