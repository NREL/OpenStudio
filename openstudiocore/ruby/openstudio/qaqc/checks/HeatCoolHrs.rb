#at a whole building level or a zone level?
#should we have different criteria for core/shell zones? just look for exterior walls as the defining characteristic
#for non-air systems, this is not a valid for radiant/convective/baseboard systems - need to look at baseboard or radiant slabs
#should we use a different output variable?  estimated load to setpoint
#things with no air inlet node to zone aren't reflected in this check 

#the goal of this check is to figure out if sizing is correct


#heating hours and cooling hours(max for all zones)
def heat_cool_hrs_check(model,sql)
  heat_cool_hrs_check = Check.new
  heat_cool_hrs_check.name = "Heating and Cooling Hours Check"
  heat_cool_hrs_check.category = "Xcel EDA"
  heat_cool_hrs_check.description = "Check that the heating and cooling hours are within expected range for each zone, and not when zones have signinficant simul
  taneous heating and cooling"    
  zones = model.getThermalZones
  all_zones_heating_hours = Array.new
  all_zones_cooling_hours = Array.new
  all_zones_simultaneous_heating_and_cooling = Array.new
  all_zones_floating_hours = Array.new
  zones.each do |zone|
    debug_puts(zone.name)
    hourly_zone_heating_energy = get_timeseries_array(sql, $env_period, $hourly_time_step, "Zone Air System Sensible Heating Energy", zone.name.get)
    zone_heating_hours = hourly_zone_heating_energy.select { |i| i != 0 }.length
    debug_puts("zone_heating_hours = #{zone_heating_hours}")
    all_zones_heating_hours << "#{zone_heating_hours} - #{zone.name}"
    hourly_zone_cooling_energy = get_timeseries_array(sql, $env_period, $hourly_time_step, "Zone Air System Sensible Cooling Energy", zone.name.get)
    zone_cooling_hours = hourly_zone_cooling_energy.select { |i| i != 0 }.length
    debug_puts("zone_cooling_hours = #{zone_cooling_hours}")
    all_zones_cooling_hours << "#{zone_cooling_hours} - #{zone.name}"
    #simultaneous heating and cooling, and floating hours (no heating or cooling)
    i = 0
    simultaneous_heating_and_cooling_hours = 0
    floating_hours = 0
    hourly_zone_heating_energy.each do |hourly_heating_value|
      hourly_cooling_value = hourly_zone_cooling_energy[i]
      i += 1
      if hourly_heating_value > 0 and hourly_cooling_value > 0
        simultaneous_heating_and_cooling_hours += 1
      end
      if hourly_heating_value == 0 and hourly_cooling_value == 0
        floating_hours += 1
      end
    end
    debug_puts("simultaneous_heating_and_cooling_hours = #{simultaneous_heating_and_cooling_hours}")
    all_zones_simultaneous_heating_and_cooling << "#{simultaneous_heating_and_cooling_hours} - #{zone.name}"
    debug_puts("floating_hours = #{floating_hours}")
    all_zones_floating_hours << "#{floating_hours} - #{zone.name}"
    #flag if heating hours > 5000 for this zone
    if zone_heating_hours > 5000
      heat_cool_hrs_check.add_flag(Flag.new($eda,"#{zone.name} has #{zone_heating_hours} heating hours; > than the 5,000 hrs expected by Xcel EDA"))
    end
    #flag if cooling hours > 3000 for this zone
    if zone_cooling_hours > 3000
      heat_cool_hrs_check.add_flag(Flag.new($eda,"#{zone.name} has #{zone_cooling_hours} cooling hours; > than the 3,000 hrs expected by Xcel EDA"))
    end      
    #flag if simultaneous heating & cooling hours > 500 for this zone
    if simultaneous_heating_and_cooling_hours > 500
      heat_cool_hrs_check.add_flag(Flag.new($eda,"#{zone.name} has #{simultaneous_heating_and_cooling_hours} simultaneous heating and cooling hours; > than the 500 hrs expected by Xcel EDA"))
    end      
       
  end
  debug_puts heat_cool_hrs_check.inspect
  
  # debug_puts "heating hours for all zones"
  # debug_puts all_zones_heating_hours.sort
  # debug_puts "cooling hours for all zones"
  # debug_puts all_zones_cooling_hours.sort
  # debug_puts "simultaneous heating and cooling"
  # debug_puts all_zones_simultaneous_heating_and_cooling.sort
  # debug_puts "floating hours (no heating or cooling)"
  # debug_puts all_zones_floating_hours.sort
  
  return heat_cool_hrs_check
end    
  
  
  
