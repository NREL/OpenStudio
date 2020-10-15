require 'openstudio'

include OpenStudio::Model
m = Model.new

ExteriorFuelEquipment::validFuelTypeValues.each_with_index do |fuel_type, i|
  designLevel = 1000*(i+1)
  ed = ExteriorFuelEquipmentDefinition.new(m)
  ed.setDesignLevel(designLevel)
  ed.setName("Definition #{fuel_type} - #{designLevel} W")

  e = ExteriorFuelEquipment.new(ed)
  e.setName("#{fuel_type} - #{designLevel} W")
  e.setFuelType(fuel_type)
end

fuel_type = "Water"
ed = ExteriorWaterEquipmentDefinition.new(m)
ed.setName("Definition #{fuel_type} - 1 m3/s")
e = ExteriorWaterEquipment.new(ed)
e.setName("#{fuel_type} - 1 m3/s")


ft = OpenStudio::EnergyPlus::ForwardTranslator.new
w = ft.translateModel(m)

# Fake having the OpenStudio results measure
OpenStudio::EndUseFuelType.getValues.each do |i|
  end_use = OpenStudio::EndUseFuelType.new(i)
  end_use_description = end_use.valueDescription()

  idf_table = OpenStudio::IdfObject.new("Output:Table:Monthly".to_IddObjectType)

  idf_table.setName("Building Energy Performance - #{end_use_description}")
  idf_table.setInt(1, 2) # Digits after Decimal
  idf_table.pushExtensibleGroup()
  idf_table.setString(2, "ExteriorEquipment:#{end_use_description.gsub(" ", "")}")
  idf_table.setString(3, "SumOrAverage")

  #OpenStudio::EndUseCategoryType::getValues.each do |i_cat|
    #category = OpenStudio::EndUseCategoryType.new(i_cat).valueName()
    #eg = idf_table.pushExtensibleGroup()
    #eg.setString(0, "#{category}:#{end_use_description.gsub(" ", "")}")
    #eg.setString(1, "SumOrAverage")
  #end
  w.addObject(idf_table)

  idf_table = OpenStudio::IdfObject.new("Output:Table:Monthly".to_IddObjectType)

  idf_table.setName("Building Energy Performance - #{end_use_description} Peak Demand")
  idf_table.setInt(1, 2) # Digits after Decimal
  eg = idf_table.pushExtensibleGroup()
  eg.setString(0, "#{end_use_description.gsub(" ", "")}:Facility")
  eg.setString(1, "Maximum")

  eg = idf_table.pushExtensibleGroup()
  eg.setString(0, "ExteriorEquipment:#{end_use_description.gsub(" ", "")}")
  eg.setString(1, "ValueWhenMaximumOrMinimum")

  w.addObject(idf_table)
end

w.save('in.idf', true)
