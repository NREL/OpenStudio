require '/usr/local/openstudio-2.9.1/Ruby/openstudio'

include OpenStudio::Model

iddObject_fuelTypesRenames = [
  "OS:OtherEquipment",
  "OS:Exterior:FuelEquipment",
  "OS:AirConditioner:VariableRefrigerantFlow",
  "OS:Boiler:HotWater",
  "OS:Boiler:Steam",
  "OS:Coil:Cooling:DX:MultiSpeed",
  "OS:Coil:Heating:Gas",
  "OS:Coil:Heating:DX:MultiSpeed",
  "OS:WaterHeater:Mixed",
  "OS:WaterHeater:Stratified",
  "OS:Generator:MicroTurbine",
  "OS:LifeCycleCost:UsePriceEscalation",
  "OS:Meter:Custom",
  "OS:Meter:CustomDecrement",
  "OS:EnergyManagementSystem:MeteredOutputVariable",
]

bad_fuels = ["FuelOil#1", "FuelOil#2", "PropaneGas"]

m = Model.new

factory = OpenStudio::IddFileAndFactoryWrapper.new("OpenStudio".to_IddFileType)

meter_custom = MeterCustomDecrement.new(m, "MeterCustomDecrement")
meter_custom.setFuelType("FuelOil#1")
ems = EnergyManagementSystemMeteredOutputVariable.new(m, "Coucou")
ems.setResourceType("FuelOil#1")

iddObject_fuelTypesRenames.each do |iddname|
  obj = factory.getObject(iddname).get
  className = iddname.gsub('OS:', '').gsub(':','')
  begin
    #m_obj = m.addObject(OpenStudio::IdfObject.new(obj)).get
    m_obj = eval("#{className}.new(m)")
  rescue
    begin
      m_objdef = eval("#{className}Definition.new(m)")
      m_obj = eval("#{className}.new(m_objdef)")
    rescue
      begin
        m_obj = eval("m.get#{className}s()[0]")
      rescue
        puts "Failed to instantiate an object of #{iddname}"
        next
      end
    end
  end

  obj.numFields.times do |i|
    field = obj.getField(i).get
    if field.name[/(fuel|resource)/i]
      bad_fuels = field.keys.select{|k|  ["FuelOil#1", "FuelOil#2", "PropaneGas"].include?(k.name)}
      if (bad_fuels.size > 0)
        ok = m_obj.setString(i, bad_fuels.sample.name)
        if !ok
          puts "Failed to set #{i} to #{bad_fuel} for #{obj.name}"
        end
      end
    end
  end
end

m.save('test_vt_fuel.osm', true)
