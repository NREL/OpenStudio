require '/usr/local/openstudio-3.0.1/Ruby/openstudio'

include OpenStudio::Model

def test_vt_fuelTypeRenames()
  # Generate test for #4039
  # Inserted a required numeric at position 6

  m = Model.new

  o = OutputMeter.new(m)
  o.setFuelType("Gas".to_FuelType)
  o.setInstallLocationType("Facility".to_InstallLocationType)
  raise if o.name != "Gas:Facility"

  o = OutputMeter.new(m)
  o.setFuelType("FuelOil_1".to_FuelType)
  o.setEndUseType("Heating".to_EndUseType)
  raise if o.name != "Heating:FuelOil_1"

  o = OutputMeter.new(m)
  o.setFuelType("Propane".to_FuelType)
  o.setEndUseType("WaterSystems".to_EndUseType)
  raise if o.name != "WaterSystems:Propane"

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
  end

  m.save('test_vt_fuelTypeRenames.osm', true)
end


test_vt_fuelTypeRenames()
