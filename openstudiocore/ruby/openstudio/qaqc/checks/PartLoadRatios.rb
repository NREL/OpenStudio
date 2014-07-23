#what is the point of this check - mostly pointing out bad system designs
#distribution and % in each bin


=begin    
 peak_heat_cool_mo_check = Check.new
  peak_heat_cool_mo_check.name = "Peak Heating and Cooling Month Check"
  peak_heat_cool_mo_check.category = "Xcel EDA"
  peak_heat_cool_mo_check.description = "Check that the heating and cooling energy peak during the expected time of year."   
    #part load ratios
    #boilers
    boilers = model.getObjectsByType("OS:Boiler:HotWater".to_IddObjectType)
    boilers.each do |boiler|
      hourly_boiler_plr = get_timeseries_array(sql, env_period, hourly_time_step, "Boiler Fan Coil Part Load Ratio", boiler.name.get)
      #puts hourly_boiler_plr
      avg_hourly_boiler_plr = non_zero_array_average(hourly_boiler_plr)
      puts "#{boiler.name} part-load ratio = #{avg_hourly_boiler_plr}"
      qaqc[model_name][boiler.name]["avg_hourly_boiler_plr"] = avg_hourly_boiler_plr
    end  
    
    #chillers
    chillers = model.getObjectsByType("OS:Chiller:Electric:EIR".to_IddObjectType)
    chillers.each do |chiller|
      #puts chiller.name
      hourly_chiller_plr = get_timeseries_array(sql, env_period, hourly_time_step, "Chiller Fan Coil Part Load Ratio", chiller.name)
      avg_hourly_chiller_plr = non_zero_array_average(hourly_chiller_plr)
      qaqc[model_name][chiller.name]["avg_hourly_chiller_plr"] = avg_hourly_chiller_plr
    end
=end    
