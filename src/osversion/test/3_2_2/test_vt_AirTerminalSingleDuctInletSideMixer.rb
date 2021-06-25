#require '/usr/local/openstudio-3.2.1/Ruby/openstudio'

include OpenStudio::Model

m = Model.new


a = AirLoopHVAC.new(m)

z = ThermalZone.new(m)

atu = AirTerminalSingleDuctInletSideMixer.new(m)
a.addBranchForZone(z, atu)

fan = FanConstantVolume.new(m)
heatingCoil = CoilHeatingWater.new(m)
coolingCoil = CoilCoolingWater.new(m)
fc = ZoneHVACFourPipeFanCoil.new(m, m.alwaysOnDiscreteSchedule, fan, coolingCoil, heatingCoil)
fc.addToNode(atu.outletModelObject.get.to_Node.get)

z.zoneAirNode().setName("Zone Air Node");
z.returnAirModelObjects[0].setName("Zone Return Air Node")
atu.inletModelObject.get.setName("ATU InletSideMixer Inlet Node")
atu.outletModelObject().get.setName("ATU InletSideMixer Outlet to FC Inlet Node")
fc.outletNode().get.setName("FC Outlet Node")
z.exhaustPortList.modelObjects[0].setName("Zone Exhaust Air Node")

m.save('test_vt_AirTerminalSingleDuctInletSideMixer.osm', true)
