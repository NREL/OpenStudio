#combine sizing with PLRs - sizing/ft2 + min PLRs
#Add VAV fans
#Looking at the sizing ratios - run the design days, look at the results
#check the type of design days they used 0.4% or 2% and right location - set in the protocol
#what about cooling days where people aren't there; therefore ventilation OA down
#check the sizing:parameters object
#put in divide by zero traps

#sizing of equipment
def equip_sizing_check(model, sql)
  equip_sizing_check = Check.new
  equip_sizing_check.name = "Equipment Sizing Check"
  equip_sizing_check.category = "Xcel EDA"
  equip_sizing_check.description = "Check that equipment is sized large enough to meet loads without being oversized."  
  #heating equipment
    #boilers
    boilers = model.getObjectsByType("OS:Boiler:HotWater".to_IddObjectType)
    boilers.each do |boiler|    
      boiler_cap_W_query = "SELECT value FROM componentsizes WHERE (comptype = 'Boiler:HotWater') AND (description = 'Nominal Capacity') AND (units = 'W') AND (compname = '#{boiler.name.get.upcase}')"
      timestep_boiler_cap = get_timeseries_array(sql, $env_period, $zone_time_step, "Boiler Heating Rate", boiler.name.get)
      max_timestep_boiler_cap = timestep_boiler_cap.max
      sized_boiler_cap = get_double(sql,boiler_cap_W_query)
      boiler_sized_to_experienced_ratio = sized_boiler_cap/max_timestep_boiler_cap
      #flag if the sizing factor > 1.25 for heating equipment
      if boiler_sized_to_experienced_ratio > 1.25
        equip_sizing_check.add_flag(Flag.new($eda,"#{boiler.name} has a sizing ratio of #{boiler_sized_to_experienced_ratio}; > than the 1.25 sizing ratio expected by Xcel EDA"))
      end
      debug_puts "#{boiler.name} - boiler_sized_to_experienced_ratio = #{boiler_sized_to_experienced_ratio}"
      debug_puts "sized_boiler_cap = #{sized_boiler_cap}"
      debug_puts "max_timestep_boiler_cap = #{max_timestep_boiler_cap}"
    end  
     
    #gas & electric heating coils
    gas_heating_coils = model.getObjectsByType("OS:Coil:Heating:Gas".to_IddObjectType)
    electric_heating_coils = model.getObjectsByType("OS:Coil:Heating:Electric".to_IddObjectType)
    #water_heating_coils = model.getObjectsByType("OS:Coil:Heating:Water".to_IddObjectType)
    heating_coils = gas_heating_coils + electric_heating_coils
    heating_coils.each do |heating_coil|
      heating_coil_cap_W_query = "SELECT Value FROM TabularDataWithStrings WHERE (ReportName = 'EquipmentSummary') AND (ReportForString = 'Entire Facility') AND (TableName = 'Heating Coils') AND (RowName = '#{heating_coil.name.get.upcase}') AND (ColumnName='Nominal Total Capacity') and (Units = 'W')"
      timestep_heating_coil_cap = get_timeseries_array(sql, $env_period, $zone_time_step, "Heating Coil Air Heating Rate", heating_coil.name.get)
      max_timestep_heating_coil_cap = timestep_heating_coil_cap.max
      sized_heating_coil_cap = get_double(sql,heating_coil_cap_W_query)
      heating_coil_sized_to_experienced_ratio = sized_heating_coil_cap/max_timestep_heating_coil_cap
      #flag if the sizing factor > 1.25 for heating equipment
      if heating_coil_sized_to_experienced_ratio > 1.25
        equip_sizing_check.add_flag(Flag.new($eda,"#{heating_coil.name} has a sizing ratio of #{heating_coil_sized_to_experienced_ratio}; > than the 1.25 sizing ratio expected by Xcel EDA"))
      end
      debug_puts "#{heating_coil.name} - heating_coil_sized_to_experienced_ratio = #{heating_coil_sized_to_experienced_ratio}"
      debug_puts "sized_heating_coil_cap = #{sized_heating_coil_cap}"
      debug_puts "max_timestep_heating_coil_cap = #{max_timestep_heating_coil_cap}"
    end       

  #cooling equipment
    #chillers
    chillers = model.getObjectsByType("OS:Chiller:Electric:EIR".to_IddObjectType)
    chillers.each do |chiller|    
      chiller_cap_W_query = "SELECT value FROM componentsizes WHERE (comptype = 'Chiller:Electric:EIR') AND (description = 'Reference Capacity') AND (units = 'W') AND (compname = '#{chiller.name.get.upcase}')"
      timestep_chiller_cap = get_timeseries_array(sql, $env_period, $zone_time_step, "Chiller Evaporator Cooling Energy Rate", chiller.name.get)
      max_timestep_chiller_cap = timestep_chiller_cap.max
      sized_chiller_cap = get_double(sql,chiller_cap_W_query)
      chiller_sized_to_experienced_ratio = sized_chiller_cap/max_timestep_chiller_cap
      #flag if the sizing factor > 1.15 for cooling equipment
      if chiller_sized_to_experienced_ratio > 1.15
        equip_sizing_check.add_flag(Flag.new($eda,"#{chiller.name} has a sizing ratio of #{chiller_sized_to_experienced_ratio}; > than the 1.15 sizing ratio expected by Xcel EDA"))
      end      
      debug_puts "#{chiller.name} - chiller_sized_to_experienced_ratio = #{chiller_sized_to_experienced_ratio}"
      debug_puts "max_timestep_chiller_cap = #{max_timestep_chiller_cap}"
      debug_puts "sized_chiller_cap = #{sized_chiller_cap}"
    end  
    
    #dx cooling coils
    #one speed
    one_speed_dx_cooling_coils = model.getObjectsByType("OS:Coil:Cooling:DX:SingleSpeed".to_IddObjectType)
    one_speed_dx_cooling_coils.each do |one_speed_dx_cooling_coil|    
      one_speed_dx_cooling_coil_cap_W_query = "SELECT value FROM componentsizes WHERE (units = 'W') AND (description = 'Rated Total Cooling Capacity (gross)') AND (compname = '#{one_speed_dx_cooling_coil.name.get.upcase}')"
      timestep_one_speed_dx_cooling_coil_cap = get_timeseries_array(sql, $env_period, $zone_time_step, "Cooling Coil Total Cooling Rate", one_speed_dx_cooling_coil.name.get)
      max_timestep_one_speed_dx_cooling_coil_cap = timestep_one_speed_dx_cooling_coil_cap.max
      sized_one_speed_dx_cooling_coil_cap = get_double(sql,one_speed_dx_cooling_coil_cap_W_query)
      one_speed_dx_cooling_coil_sized_to_experienced_ratio = sized_one_speed_dx_cooling_coil_cap/max_timestep_one_speed_dx_cooling_coil_cap
      #flag if the sizing factor > 1.15 for cooling equipment
      if one_speed_dx_cooling_coil_sized_to_experienced_ratio > 1.15
        equip_sizing_check.add_flag(Flag.new($eda,"#{one_speed_dx_cooling_coil.name} has a sizing ratio of #{one_speed_dx_cooling_coil_sized_to_experienced_ratio}; > than the 1.15 sizing ratio expected by Xcel EDA"))
      end   
      debug_puts "#{one_speed_dx_cooling_coil.name} - one_speed_dx_cooling_coil_sized_to_experienced_ratio = #{one_speed_dx_cooling_coil_sized_to_experienced_ratio}"
      debug_puts "max_timestep_one_speed_dx_cooling_coil_cap = #{max_timestep_one_speed_dx_cooling_coil_cap}"
      debug_puts "sized_one_speed_dx_cooling_coil_cap = #{sized_one_speed_dx_cooling_coil_cap}"
    end      
    #two speed
    two_speed_dx_cooling_coils = model.getObjectsByType("OS:Coil:Cooling:DX:TwoSpeed".to_IddObjectType)
    two_speed_dx_cooling_coils.each do |two_speed_dx_cooling_coil|    
      two_speed_dx_cooling_coil_cap_W_query = "SELECT value FROM componentsizes WHERE (units = 'W') AND (description = 'Rated High Speed Total Cooling Capacity (gross)') AND (compname = '#{two_speed_dx_cooling_coil.name.get.upcase}')"
      timestep_two_speed_dx_cooling_coil_cap = get_timeseries_array(sql, $env_period, $zone_time_step, "Cooling Coil Total Cooling Rate", two_speed_dx_cooling_coil.name.get)
      max_timestep_two_speed_dx_cooling_coil_cap = timestep_two_speed_dx_cooling_coil_cap.max
      sized_two_speed_dx_cooling_coil_cap = get_double(sql,two_speed_dx_cooling_coil_cap_W_query)
      two_speed_dx_cooling_coil_sized_to_experienced_ratio = sized_two_speed_dx_cooling_coil_cap/max_timestep_two_speed_dx_cooling_coil_cap
      #flag if the sizing factor > 1.15 for cooling equipment
      if two_speed_dx_cooling_coil_sized_to_experienced_ratio > 1.15
        equip_sizing_check.add_flag(Flag.new($eda,"#{two_speed_dx_cooling_coil.name} has a sizing ratio of #{two_speed_dx_cooling_coil_sized_to_experienced_ratio}; > than the 1.15 sizing ratio expected by Xcel EDA"))
      end
      debug_puts "#{two_speed_dx_cooling_coil.name} - two_speed_dx_cooling_coil_sized_to_experienced_ratio = #{two_speed_dx_cooling_coil_sized_to_experienced_ratio}"
      debug_puts "max_timestep_two_speed_dx_cooling_coil_cap = #{max_timestep_two_speed_dx_cooling_coil_cap}"
      debug_puts "sized_two_speed_dx_cooling_coil_cap = #{sized_two_speed_dx_cooling_coil_cap}"
    end      
   
    #cooling towers
    cooling_towers = model.getObjectsByType("OS:CoolingTower:SingleSpeed".to_IddObjectType)
    cooling_towers.each do |cooling_towers|    
      cooling_towers_cap_W_query = "SELECT Value FROM TabularDataWithStrings WHERE (ReportName = 'EquipmentSummary') AND (ReportForString = 'Entire Facility') AND (TableName = 'Central Plant') AND (RowName = '#{cooling_towers.name.get.upcase}') AND (ColumnName='Nominal Capacity') and (Units = 'W')"
      timestep_cooling_towers_cap = get_timeseries_array(sql, $env_period, $zone_time_step, "Cooling Tower Heat Transfer Rate", cooling_towers.name.get)
      max_timestep_cooling_towers_cap = timestep_cooling_towers_cap.max
      sized_cooling_towers_cap = get_double(sql,cooling_towers_cap_W_query)
      cooling_towers_sized_to_experienced_ratio = sized_cooling_towers_cap/max_timestep_cooling_towers_cap
      #flag if the sizing factor > 1.15 for cooling equipment
      if cooling_towers_sized_to_experienced_ratio > 1.15
        equip_sizing_check.add_flag(Flag.new($eda,"#{cooling_towers.name} has a sizing ratio of #{cooling_towers_sized_to_experienced_ratio}; > than the 1.15 sizing ratio expected by Xcel EDA"))
      end
      debug_puts "#{cooling_towers.name} - cooling_towers_sized_to_experienced_ratio = #{cooling_towers_sized_to_experienced_ratio}"
      debug_puts "max_timestep_cooling_towers_cap = #{max_timestep_cooling_towers_cap}"
      debug_puts "sized_cooling_towers_cap = #{sized_cooling_towers_cap}"
    end         
  debug_puts equip_sizing_check.inspect
  
  return equip_sizing_check
end
  
