require '/usr/local/openstudio-3.0.1/Ruby/openstudio'

include OpenStudio::Model

def test_vt_fuelTypeRenames()

  m = Model.new

  o = OutputMeter.new(m)
  o.setFuelType("Gas".to_FuelType)
  o.setInstallLocationType("Facility".to_InstallLocationType)
  raise if o.name != "Gas:Facility"
  ems_sensor = EnergyManagementSystemSensor.new(m, o)
  ems_sensor.setName("Gas_Facility")

  o = OutputMeter.new(m)
  o.setFuelType("FuelOil_1".to_FuelType)
  o.setEndUseType("Heating".to_EndUseType)
  raise if o.name != "Heating:FuelOil_1"
  ems_sensor = EnergyManagementSystemSensor.new(m, o)
  ems_sensor.setName("Heating_FuelOil_1")

  o = OutputMeter.new(m)
  o.setFuelType("Propane".to_FuelType)
  o.setEndUseType("WaterSystems".to_EndUseType)
  raise if o.name != "WaterSystems:Propane"
  ems_sensor = EnergyManagementSystemSensor.new(m, o)
  ems_sensor.setName("WaterSystems_Propane")

  # Test a few out vars
  vars = [
    "Boiler FuelOil#1 Energy",
    "Boiler Gas Rate",
    "Boiler Electric Power",
    "Cooling Coil Water Heating Electric Power",
    "Generator Requested Electric Power",
    "Air System DX Cooling Coil Electric Energy",
    # Introduce extra spaces on purpose...
    "Water  Heater  Gas Rate",
  ]
  vars.each do |v|
    out_var = OutputVariable.new(v, m)
    out_var.setName(v)
    ems_sensor = EnergyManagementSystemSensor.new(m, out_var)
    ems_sensor.setName(v.gsub(" ", "_").gsub("#", "_"))
  end

  # Add an extra one not linked to an actual out_var or out_meter
  # This is the only one that should be changed, the rest has handles
  var_name = "Facility Total HVAC Electric Demand Power"
  ems_sensor = EnergyManagementSystemSensor.new(m, var_name)
  ems_sensor.setName(var_name.gsub(" ", "_"))

  m.save('test_vt_fuelTypeRenames.osm', true)
end

def test_vt_fuelTypeRenames_MeterCustoms()

  m = Model.new

  mm = MeterCustom.new(m)
  mm.setFuelType("Electricity")
  mm.addKeyVarGroup("*", "Facility Total HVAC Electric Demand Power")
  mm.addKeyVarGroup("*", "Generator Blower      Electric Power   ");

  md = MeterCustomDecrement.new(m, "NaturalGas:Facility")
  md.setFuelType("NaturalGas")
  md.addKeyVarGroup("*", "Facility     Total HVAC Electric    Demand Power")
  md.addKeyVarGroup("*", "Generator Blower Electric Power");

  m.save('test_vt_fuelTypeRenames_MeterCustoms.osm', true)

end

test_vt_fuelTypeRenames()
test_vt_fuelTypeRenames_MeterCustoms()
