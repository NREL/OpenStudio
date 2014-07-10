#can we use E+'s metric directly?  E+ will only use conditioned area
#we need to incorporate building type into the range checking - ASHRAE Standard 100
#how many hours did the model run for? - make sure 8760 - get from html file

#checks the EUI for the whole building
def eui_check(model,sql)
  eui_check = Check.new
  eui_check.name = "EUI Check"
  eui_check.category = "General"
  eui_check.description = "Check that the EUI of the building is reasonable."    
  building = model.getBuilding
  
  #make sure all required data are available
  if sql.totalSiteEnergy.empty?
    eui_check.add_flag(Flag.new($eda,"Site energy data unavailable; check not run"))
    return eui_check
  end
  
  total_site_energy_GJ = OpenStudio::Quantity.new(sql.totalSiteEnergy.get, $GJ_unit)
  total_site_energy_kBtu = OpenStudio::convert(total_site_energy_GJ, $kBtu_unit).get
  if total_site_energy_kBtu.value == 0
    eui_check.add_flag(Flag.new($eda,"No site energy use was found"))
    return eui_check
  end
  floor_area_m2 = OpenStudio::Quantity.new(building.floorArea, $m2_unit)
  floor_area_ft2 = OpenStudio::convert(floor_area_m2, $ft2_unit).get
  if floor_area_ft2.value == 0
    eui_check.add_flag(Flag.new($eda,"The building has 0 floor area"))
    return eui_check
  end
  site_EUI = total_site_energy_kBtu / floor_area_ft2
  if site_EUI.value > 200
    eui_check.add_flag(Flag.new($eda,"Site EUI of #{site_EUI} looks high.  A hospital or lab (high energy buildings) are around 200 kBtu/ft^2"))
  end
  if site_EUI.value < 30
    eui_check.add_flag(Flag.new($eda,"Site EUI of #{site_EUI} looks low.  A high efficiency office building is around 50 kBtu/ft^2"))
  end
  debug_puts eui_check.flags.inspect
  
  return eui_check
end    
