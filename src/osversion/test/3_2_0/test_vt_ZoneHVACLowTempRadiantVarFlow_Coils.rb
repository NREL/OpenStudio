require '/usr/local/openstudio-3.1.0/Ruby/openstudio'

include OpenStudio::Model

m = Model.new

p = PlantLoop.new(m)

s_heating  = ScheduleConstant.new(m)
s_heating.setValue(10.0)
s_heating.setName("Heating Control Temperature Schedule")
heating_coil = CoilHeatingLowTempRadiantVarFlow.new(m, s_heating)
heating_coil.setMaximumHotWaterFlow(0.85)
heating_coil.setHeatingControlThrottlingRange(0.65) # In 3.1.0, min was 0.5
p.addDemandBranchForComponent(heating_coil)
heating_coil.inletModelObject.get.setName("Heating Coil Inlet Node")
heating_coil.outletModelObject.get.setName("Heating Coil Outlet Node")

s_cooling  = ScheduleConstant.new(m)
s_cooling.setValue(15.0)
s_cooling.setName("Cooling Control Temperature Schedule")
cooling_coil = CoilCoolingLowTempRadiantVarFlow.new(m, s_cooling)
cooling_coil.setMaximumColdWaterFlow(1.05)
cooling_coil.setCoolingControlThrottlingRange(0.75)
cooling_coil.setCondensationControlType("VariableOff")
cooling_coil.setCondensationControlDewpointOffset(1.3)
p.addDemandBranchForComponent(cooling_coil)
cooling_coil.inletModelObject.get.setName("Cooling Coil Inlet Node")
cooling_coil.outletModelObject.get.setName("Cooling Coil Outlet Node")

m.save('test_vt_ZoneHVACLowTempRadiantVarFlow_Coils.osm', true)
