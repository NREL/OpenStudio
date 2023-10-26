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


o = OutputMeter.new(m)
o.setFuelType("Steam".to_FuelType)
o.setInstallLocationType("Facility".to_InstallLocationType)
raise if o.name != "Steam:Facility"
ems_sensor = EnergyManagementSystemSensor.new(m, o)
ems_sensor.setName("Steam_Facility")

o = OutputMeter.new(m)
o.setFuelType("DistrictHeating".to_FuelType)
o.setEndUseType("Heating".to_EndUseType)
raise if o.name != "Heating:DistrictHeating"
ems_sensor = EnergyManagementSystemSensor.new(m, o)
ems_sensor.setName("Heating_DistrictHeating")

# Test
vars = [
  'District Cooling Chilled Water Energy',
  'District Cooling Chilled Water Rate',
  'District Cooling Rate',
  'District Cooling Inlet Temperature',
  'District Cooling Outlet Temperature',
  'District Cooling Mass Flow Rate',
  'District Heating Hot Water Energy',
  'District Heating Hot Water Rate',
  'District Heating Rate',
  'District Heating Inlet Temperature',
  'District Heating Outlet Temperature',
  'District Heating Mass Flow Rate'
]
vars.each do |v|
  out_var = OutputVariable.new(v, m)
  out_var.setName(v)
  ems_sensor = EnergyManagementSystemSensor.new(m, out_var)
  ems_sensor.setName(v.gsub(" ", "_").gsub("#", "_"))
end


# Add an extra one not linked to an actual out_var or out_meter
# This is the only one that should be changed, the rest has handles
var_name = "District Cooling Inlet Temperature"
ems_sensor = EnergyManagementSystemSensor.new(m, var_name)
ems_sensor.setName("#{var_name.gsub(' ', '_')}_Standandalone")

m.save('test_vt_FuelTypeRenames.osm', true)
