#require '/usr/local/openstudio-3.6.1/Ruby/openstudio'

include OpenStudio::Model

m = Model.new

od = OtherEquipmentDefinition.new(m)
o = OtherEquipment.new(od)
o.setFuelType("SteAm")

ed = ExteriorFuelEquipmentDefinition.new(m)
e = ExteriorFuelEquipment.new(ed)
e.setFuelType("DISTRICTHEATING")

wh = WaterHeaterMixed.new(m)
wh.setHeaterFuelType("StEAm")
wh.setOffCycleParasiticFuelType("DisTrictHeating")
wh.setOnCycleParasiticFuelType("STEAM")


wh = WaterHeaterStratified.new(m)
wh.setHeaterFuelType("StEAm")
wh.setOffCycleParasiticFuelType("DisTrictHeating")
wh.setOnCycleParasiticFuelType("STEAM")

u1 = UtilityBill.new(OpenStudio::FuelType.new("FuelOil_1"), m)
u1.setName("Legacy")
u2 = UtilityBill.new(OpenStudio::FuelType.new("Steam"), m)
u2.setName("Steam")

mc = MeterCustom.new(m)
mc.setFuelType("DistrictHEATING")

mcd = MeterCustomDecrement.new(m, mc.nameString)
mcd.setFuelType("STEAM")

ems_mov = EnergyManagementSystemMeteredOutputVariable.new(m, "DUMMY")
ems_mov.setResourceType("STEAM")

py_var = PythonPluginVariable.new(m)
py_out = PythonPluginOutputVariable.new(py_var)
py_out.setResourceType("DISTRictHeating")

m.save('test_vt_FuelTypeRenames.osm', true)
