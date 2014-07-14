#what does xcel want to call water-to-water heat pump - Drew -> electric
#don't forget absorbtion powered chillers eventually
#what about service water heating? you can have non-connected boilers -> try to pick up changes in boilers
#preheat and precool coils on OA system
#not considered fuel switching if going from no fuel -> some fuel

  
  #heating & cooling primary fuel swap
def heat_cool_fuel_swap_check(base_model,base_sql,prop_model,prop_sql)
  heat_cool_fuel_swap_check = Check.new
  heat_cool_fuel_swap_check.name = "Fuel Swap Check"
  heat_cool_fuel_swap_check.category = "Xcel EDA"
  heat_cool_fuel_swap_check.description = "Check that the baseline and the design alternative use the same fuels." 
  #make a hash to hold the results
  zone_fuels = Hash.new{|h,k| h[k]=Hash.new() }
  #get the zones from both models
  baseline_zones = base_model.getThermalZones
  proposed_zones = prop_model.getThermalZones
  #get zone names from both models.  will be used as basis of fuel comparison
  baseline_zone_names = Array.new
  proposed_zone_names = Array.new
  #make arrays of the zone names from each model
  baseline_zones.each do |baseline_zone|
    baseline_zone_names << baseline_zone.name.get
  end
  proposed_zones.each do |proposed_zone|
    proposed_zone_names << proposed_zone.name.get
  end
  #output the sorted zone name lists
  debug_puts("**baseline**")
  debug_puts(baseline_zone_names.sort)
  debug_puts("**proposed**")
  debug_puts(proposed_zone_names.sort)

  #check the heating fuel type of a plant loop
  def plant_loop_heating_fuel(plant_loop)
    debug_puts("  looking for plant loop heating fuel for #{plant_loop.name}")
    #create an array to hold all the heating fuels on the plant loop
    plant_loop_heating_fuels = Array.new
    #loop throug all the supply components and check heating fuel for each one
    plant_loop.supplyComponents.each do |supply_component|
      if supply_component.to_DistrictHeating.empty? == false
        plant_loop_heating_fuels <<  "District Heating"  
      elsif supply_component.to_BoilerHotWater.empty? == false
        boiler_hot_water = supply_component.to_BoilerHotWater.get
        plant_loop_heating_fuels << boiler_hot_water.fuelType  
      end
    end
    return plant_loop_heating_fuels.uniq
  end

  #check the cooling fuel type of a plant loop
  def plant_loop_cooling_fuel(plant_loop)
    debug_puts("  looking for plant loop heating fuel for #{plant_loop.name}")
    #create an array to hold all the heating fuels on the plant loop
    plant_loop_cooling_fuels = Array.new
    #loop throug all the supply components and check heating fuel for each one
    plant_loop.supplyComponents.each do |supply_component|
      if supply_component.to_DistrictCooling.empty? == false
        plant_loop_cooling_fuels <<  "District Cooling"  
      elsif supply_component.to_ChillerElectricEIR.empty? == false
        plant_loop_cooling_fuels << "Electricity"
      end
    end
    return plant_loop_cooling_fuels.uniq
  end

  #check the heating fuel type of a reheat terminal
  def reheat_terminal_heating_fuel(reheat_terminal)
    debug_puts("  looking for reheat terminal heating fuel for #{reheat_terminal.name}") 
    #create an array to hold all the heating fuels in the terminal
    reheat_terminal_heating_fuels = Array.new
    #get the reheat coil
    heating_coil = reheat_terminal.reheatCoil 
    if heating_coil.to_CoilHeatingDXSingleSpeed.empty? == false
      reheat_terminal_heating_fuels += get_heating_coil_heating_fuel(heating_coil)
    elsif heating_coil.to_CoilHeatingElectric.empty? == false
      reheat_terminal_heating_fuels += get_heating_coil_heating_fuel(heating_coil)
    elsif heating_coil.to_CoilHeatingGas.empty? == false
      reheat_terminal_heating_fuels += get_heating_coil_heating_fuel(heating_coil)
    elsif heating_coil.to_CoilHeatingWater.empty? == false
      reheat_terminal_heating_fuels += get_heating_coil_heating_fuel(heating_coil)
    end
    return reheat_terminal_heating_fuels.uniq
  end

  #check the heating fuel type of a heating coil
  def get_heating_coil_heating_fuel(heating_coil)
    debug_puts("  looking for heating coil heating fuel for #{heating_coil.name}") 
    #create an array to hold all the heating fuels in the terminal
    heating_coil_heating_fuels = Array.new
   #get the reheat coil
    if heating_coil.to_CoilHeatingDXSingleSpeed.empty? == false
      heating_coil_heating_fuels << "Electricity"
    elsif heating_coil.to_CoilHeatingElectric.empty? == false
      heating_coil_heating_fuels << "Electricity"
    elsif heating_coil.to_CoilHeatingGas.empty? == false
      heating_coil_heating_fuels << "Natural Gas"
    elsif heating_coil.to_CoilHeatingWater.empty? == false
      #need to check what the heating fuel for the plant loop is
      heating_coil = heating_coil.to_CoilHeatingWater.get
      plant_loop = heating_coil.plantLoop.get
      reheat_coil_fuels = plant_loop_heating_fuel(plant_loop)
      heating_coil_heating_fuels += reheat_coil_fuels
    end
    return heating_coil_heating_fuels
  end

  #check the cooling fuel type of a cooling coil
  def get_cooling_coil_cooling_fuel(cooling_coil)
    debug_puts("  looking for cooling coil cooling fuel for #{cooling_coil.name}") 
    #create an array to hold all the heating fuels in the terminal
    cooling_coil_cooling_fuels = Array.new
    if cooling_coil.to_CoilCoolingDXSingleSpeed.empty? == false
      cooling_coil_cooling_fuels << "Electricity"
    elsif cooling_coil.to_CoilCoolingDXTwoSpeed.empty? == false
      cooling_coil_cooling_fuels << "Electricity"
    elsif cooling_coil.to_EvaporativeCoolerDirectResearchSpecial.empty? == false
      cooling_coil_cooling_fuels << "Electricity"
    elsif cooling_coil.to_CoilCoolingWater.empty? == false
      #need to check what the cooling fuel for the plant loop is
      cooling_coil = cooling_coil.to_CoilCoolingWater.get
      plant_loop = cooling_coil.plantLoop.get
      cooling_coil_cooling_fuels += plant_loop_cooling_fuel(plant_loop)
    end
    return cooling_coil_cooling_fuels
  end

  #check the heating fuels for a zone
  def get_zone_heating_fuels(zone)
    #create an array to hold the results
    equipment_heating_fuels = Array.new
    zone.equipment.each do |equipment|
      #TODO Enumerable and code blocks and collect or select methods
      debug_puts("  #{equipment.name}")
      if equipment.to_AirTerminalSingleDuctParallelPIUReheat.empty? == false
        terminal = equipment.to_AirTerminalSingleDuctParallelPIUReheat.get
        equipment_heating_fuels += reheat_terminal_heating_fuel(terminal)
      elsif equipment.to_AirTerminalSingleDuctVAVReheat.empty? == false
        terminal = equipment.to_AirTerminalSingleDuctVAVReheat.get
        equipment_heating_fuels += reheat_terminal_heating_fuel(terminal)
      elsif equipment.to_ZoneHVACPackagedTerminalHeatPump.empty? == false
        equipment_heating_fuels << "Electricity"
      end
    end
    return equipment_heating_fuels.uniq.sort
  end

  #check the cooling fuels for a zone
  def get_zone_cooling_fuels(zone)
    #create an array to hold the results
    equipment_cooling_fuels = Array.new
    zone.equipment.each do |equipment|
      #TODO Enumerable and code blocks and collect or select methods
      debug_puts("  #{equipment.name}")
      if equipment.to_ZoneHVACPackagedTerminalAirConditioner.empty? == false
        equipment_cooling_fuels << "Electricity"
      end
    end
    return equipment_cooling_fuels.uniq.sort
  end

  #check the heating fuels for a zones airloop
  def get_zone_airloop_heating_fuels(zone)  
    #create an array to hold the results
    airloop_heating_fuels = Array.new
    if zone.airLoopHVAC.empty? #== false
      debug_puts("  This zone is not connected to an airloop.")
      return airloop_heating_fuels
    else
      zone.airLoopHVAC.get.supplyComponents.each do |supply_component|
        if supply_component.to_CoilHeatingDXSingleSpeed.empty? == false
          airloop_heating_fuels += get_heating_coil_heating_fuel(supply_component)
        elsif supply_component.to_CoilHeatingElectric.empty? == false
          airloop_heating_fuels += get_heating_coil_heating_fuel(supply_component)
        elsif supply_component.to_CoilHeatingGas.empty? == false
          airloop_heating_fuels += get_heating_coil_heating_fuel(supply_component)
        elsif supply_component.to_CoilHeatingWater.empty? == false
          airloop_heating_fuels += get_heating_coil_heating_fuel(supply_component)
        end
      end
    end
    return airloop_heating_fuels.uniq.sort
  end

  #check the cooling fuels for a zones airloop
  def get_zone_airloop_cooling_fuels(zone)  
    #create an array to hold the results
    airloop_cooling_fuels = Array.new
    if zone.airLoopHVAC.empty? #== false
      debug_puts("  This zone is not connected to an airloop.")
      return airloop_cooling_fuels
    else
      zone.airLoopHVAC.get.supplyComponents.each do |supply_component|
        if supply_component.to_CoilCoolingDXSingleSpeed.empty? == false
          airloop_cooling_fuels += get_cooling_coil_cooling_fuel(supply_component)
        elsif supply_component.to_CoilCoolingDXTwoSpeed.empty? == false
          airloop_cooling_fuels += get_cooling_coil_cooling_fuel(supply_component)
        elsif supply_component.to_EvaporativeCoolerDirectResearchSpecial.empty? == false
          airloop_cooling_fuels += get_cooling_coil_cooling_fuel(supply_component)
        elsif supply_component.to_CoilCoolingWater.empty? == false
          airloop_cooling_fuels += get_cooling_coil_cooling_fuel(supply_component)
        end
      end
    end
    return airloop_cooling_fuels.uniq.sort
  end

  #loop through all thermal zones, comparing fuels for heating and cooling
  baseline_zone_names.sort.each do |baseline_zone_name|
    #first, make sure the zone exists in both models.  use names to compare.
    if not proposed_zone_names.include?(baseline_zone_name)
      heat_cool_fuel_swap_check.add_flag(Flag.new($eda, "The zone called #{baseline_zone_name} exists in the baseline model, but not the proposed model, therfore the check for switching of primary heating and cooling fuels could not be run for this zone"))
      debug_puts "The zone called #{baseline_zone_name} exists in the baseline model, but not the proposed model, therfore the check for switching of primary heating and cooling fuels could not be run for this zone"
      next
    end
    debug_puts("****")
    debug_puts("Zone name = #{baseline_zone_name}")
    #default heating and cooling sources to "Unknown" for all zones
    baseline_zone_hvac_heating = "Unknown"
    baseline_zone_hvac_cooling = "Unknown"
    baseline_airloop_heating = "Unknown"
    baseline_airloop_cooling = "Unknown"  
    proposed_zone_hvac_heating = "Unknown"
    proposed_zone_hvac_cooling = "Unknown"
    proposed_airloop_heating = "Unknown"
    proposed_airloop_cooling = "Unknown"  
    #since zone exists in both models, get each zone by name
    baseline_zone = base_model.getObjectsByName(baseline_zone_name)[0].to_ThermalZone.get
    proposed_zone = prop_model.getObjectsByName(baseline_zone_name)[0].to_ThermalZone.get
    
    #check the zone hvac heating fuel(s)
      debug_puts("checking the zone-level equipment's heating fuel(s)")
      baseline_zone_hvac_heating = get_zone_heating_fuels(baseline_zone)
      proposed_zone_hvac_heating = get_zone_heating_fuels(proposed_zone)
      #throw a flag if different
      if baseline_zone_hvac_heating != proposed_zone_hvac_heating
        heat_cool_fuel_swap_check.add_flag(Flag.new($eda, "Zone-level heating fuel switching: zone name = #{baseline_zone_name}; baseline_zone_hvac_heating = #{baseline_zone_hvac_heating}; proposed_zone_hvac_heating = #{proposed_zone_hvac_heating}"))
      end
      
    #check the zone hvac cooling fuel(s)
      debug_puts("checking the zone-level equipment's cooling fuel(s)")
      baseline_zone_hvac_cooling = get_zone_cooling_fuels(baseline_zone)
      proposed_zone_hvac_cooling = get_zone_cooling_fuels(proposed_zone)  
      #throw a flag if different
      if baseline_zone_hvac_cooling != proposed_zone_hvac_cooling
        heat_cool_fuel_swap_check.add_flag(Flag.new($eda, "Zone-level cooling fuel switching: zone name = #{baseline_zone_name}; baseline_zone_hvac_cooling = #{baseline_zone_hvac_cooling}; proposed_zone_hvac_cooling = #{proposed_zone_hvac_cooling}"))
      end
    
    #check the zone airloop heating fuel(s)
      debug_puts("checking the zone's airloop's heating fuel(s)")
      baseline_airloop_heating = get_zone_airloop_heating_fuels(baseline_zone)
      proposed_airloop_heating = get_zone_airloop_heating_fuels(proposed_zone)
      #throw a flag if different
      if baseline_airloop_heating != proposed_airloop_heating
        heat_cool_fuel_swap_check.add_flag(Flag.new($eda, "Airloop heating fuel switching: zone name = #{baseline_zone_name}; baseline_airloop_heating = #{baseline_airloop_heating}; proposed_airloop_heating = #{proposed_airloop_heating}"))
      end  
    
    #check the zone airloop cooling fuel(s)
      debug_puts("checking the zone's airloop's cooling fuel(s)")
      baseline_airloop_cooling = get_zone_airloop_cooling_fuels(baseline_zone)
      proposed_airloop_cooling = get_zone_airloop_cooling_fuels(proposed_zone)
      #throw a flag if different
      if baseline_airloop_cooling != proposed_airloop_cooling
        heat_cool_fuel_swap_check.add_flag(Flag.new($eda, "Airloop cooling fuel switching: zone name = #{baseline_zone_name}; baseline_airloop_cooling = #{baseline_airloop_cooling}; proposed_airloop_cooling = #{proposed_airloop_cooling}"))
      end  
  end
  debug_puts heat_cool_fuel_swap_check.inspect
  
  return heat_cool_fuel_swap_check
end
  
