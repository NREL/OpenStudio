#This file defines a bunch of ruby helper methods that make certain openstudio calls simpler in ruby

#load a model into OS & version translates, exiting and erroring if a problem is found
def safe_load_model(model_path_string)  
  model_path = OpenStudio::Path.new(model_path_string)
  if OpenStudio::exists(model_path)
    versionTranslator = OpenStudio::OSVersion::VersionTranslator.new 
    model = versionTranslator.loadModel(model_path)
    if model.empty?
      puts "Version translation failed for #{model_path_string}"
      exit
    else
      model = model.get
    end
  else
    puts "#{model_path_string} couldn't be found"
    exit
  end
  return model
end

#load a sql file, exiting and erroring if a problem is found
def safe_load_sql(sql_path_string)
  sql_path = OpenStudio::Path.new(sql_path_string)
  if OpenStudio::exists(sql_path)
    sql = OpenStudio::SqlFile.new(sql_path)
  else 
    puts "#{sql_path} couldn't be found"
    exit
  end
  return sql
end

#function to wrap debug == true puts
def debug_puts(puts_text)
  if Debug_Mode == true
    puts "#{puts_text}"
  end
end

#gets a time series data vector from the sql file and puts the values into a standard array of numbers
def get_timeseries_array(openstudio_sql_file, env_period, timestep, variable_name, key_value)
    #puts openstudio_sql_file.class
    #puts env_period.class
    #puts timestep.class
    #puts variable_name.class
    #puts key_value.class
    key_value = key_value.upcase  #upper cases the key_value b/c it is always uppercased in the sql file.
    #timestep = timestep.capitalize  #capitalize the timestep b/c it is always capitalized in the sql file
    #timestep = timestep.split(" ").each{|word| word.capitalize!}.join(" ")    
    #returns an array of all keyValues matching the variable name, envPeriod, and reportingFrequency
    #we'll use this to check if the query will work before we send it.
    puts "*#{env_period}*#{timestep}*#{variable_name}"
    time_series_array = []
    puts env_period.class
    if env_period.nil?
      puts "here"
      time_series_array = [nil]
      return time_series_array    
    end
    possible_env_periods = openstudio_sql_file.availableEnvPeriods()
    if possible_env_periods.nil?
      time_series_array = [nil]
      return time_series_array
    end
    possible_timesteps = openstudio_sql_file.availableReportingFrequencies(env_period)
    if possible_timesteps.nil?
      time_series_array = [nil]
      return time_series_array
    end
    possible_variable_names = openstudio_sql_file.availableVariableNames(env_period,timestep)
    if possible_variable_names.nil?
      time_series_array = [nil]
      return time_series_array    
    end
    possible_key_values = openstudio_sql_file.availableKeyValues(env_period,timestep,variable_name)
    if possible_key_values.nil?
      time_series_array = [nil]
      return time_series_array    
    end
    
    if possible_key_values.include? key_value and 
      possible_variable_names.include? variable_name and
      possible_env_periods.include? env_period and 
      possible_timesteps.include? timestep
      #the query is valid
      time_series = openstudio_sql_file.timeSeries(env_period, timestep, variable_name, key_value)      
      if time_series #checks to see if time_series exists 
        time_series = time_series.get.values
        debug_puts "  #{key_value} time series length = #{time_series.size}"
        for i in 0..(time_series.size - 1)
          #puts "#{i.to_s} -- #{time_series[i]}"
          time_series_array << time_series[i]
        end
      end         
    else
      #do this if the query is not valid.  The comments might help troubleshoot.
      time_series_array = [nil]
      debug_puts "***The pieces below do NOT make a valid query***" 
      debug_puts "  *#{key_value}* - this key value might not exist for the variable you are looking for"
      debug_puts "  *#{timestep}* - this value should be Hourly, Monthly, Zone Timestep, HVAC System Timestep, etc"
      debug_puts "  *#{variable_name}* - every word should be capitalized EG:  Refrigeration System Total Compressor Electric Energy "
      debug_puts "  *#{env_period}* - you can get an array of all the valid env periods by using the sql_file.availableEnvPeriods() method "
      debug_puts "  Possible key values: #{possible_key_values}"
      debug_puts "  Possible Variable Names: #{possible_variable_names}"
      debug_puts "  Possible run periods:  #{possible_env_periods}"
      debug_puts "  Possible timesteps:  #{possible_timesteps}"
    end  
  return time_series_array
end

#gets the average of the numbers in an array
def non_zero_array_average(arr)
  debug_puts "average of the entire array = #{arr.inject{ |sum, el| sum + el }.to_f / arr.size}"
  arr.delete(0)
  debug_puts "average of the non-zero numbers in the array = #{arr.inject{ |sum, el| sum + el }.to_f / arr.size}"
  return arr.inject{ |sum, el| sum + el }.to_f / arr.size
end

#method for converting from IP to SI if you know the strings of the input and the output
def ip_to_si(number, ip_unit_string, si_unit_string)     
  ip_unit = OpenStudio::createUnit(ip_unit_string, "IP".to_UnitSystem).get
  si_unit = OpenStudio::createUnit(si_unit_string, "SI".to_UnitSystem).get
  #puts "#{ip_unit} --> #{si_unit}"
  ip_quantity = OpenStudio::Quantity.new(number, ip_unit)
  si_quantity = OpenStudio::convert(ip_quantity, si_unit).get
  #puts "#{ip_quantity} = #{si_quantity}" 
  return si_quantity.value
end
