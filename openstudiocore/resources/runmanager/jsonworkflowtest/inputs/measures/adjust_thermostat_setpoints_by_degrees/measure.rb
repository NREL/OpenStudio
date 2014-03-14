#start the measure
class AdjustTheromstatSetpointsByDegrees < OpenStudio::Ruleset::ModelUserScript

  #define the name that a user will see
  def name
    return "Adjust Thermostat Setpoints by Degrees"
  end

  #define the arguments that the user will input
  def arguments(model)
    args = OpenStudio::Ruleset::OSArgumentVector.new

    #make an argument for adjustment to cooling setpoint
    cooling_adjustment = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("cooling_adjustment",true)
    cooling_adjustment.setDisplayName("Degrees Fahrenheit to Adjust Cooling Setpoint By.")
    cooling_adjustment.setDefaultValue(1.0)
    args << cooling_adjustment

    #make an argument for adjustment to heating setpoint
    heating_adjustment = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("heating_adjustment",true)
    heating_adjustment.setDisplayName("Degrees Fahrenheit to Adjust heating Setpoint By.")
    heating_adjustment.setDefaultValue(-1.0)
    args << heating_adjustment

    #make an argument for adjustment to heating setpoint
    alter_design_days = OpenStudio::Ruleset::OSArgument::makeBoolArgument("alter_design_days",true)
    alter_design_days.setDisplayName("Alter Design Day Thermostats?")
    alter_design_days.setDefaultValue(false)
    args << alter_design_days

    return args
  end #end the arguments method

  #define what happens when the measure is run
  def run(model, runner, user_arguments)
    super(model, runner, user_arguments)

    #use the built-in error checking
    if not runner.validateUserArguments(arguments(model), user_arguments)
      return false
    end

    #assign the user inputs to variables
    cooling_adjustment = runner.getDoubleArgumentValue("cooling_adjustment",user_arguments)
    heating_adjustment = runner.getDoubleArgumentValue("heating_adjustment",user_arguments)
    alter_design_days = runner.getBoolArgumentValue("alter_design_days",user_arguments)

    #ruby test to see if first charter of string is uppercase letter
    if cooling_adjustment < 0
      runner.registerWarning("Lowering the cooling setpoint will increase energy use.")
    elsif cooling_adjustment.abs > 50
      runner.registerWarning("#{cooling_adjustment} is a larger than typical setpoint adjustment")
    elsif cooling_adjustment.abs > 500
      runner.registerError("#{cooling_adjustment} is a larger than typical setpoint adjustment")
      return false
    end
    if heating_adjustment > 0
      runner.registerWarning("Raising the heating setpoint will increase energy use.")
    elsif heating_adjustment.abs > 50
      runner.registerWarning("#{heating_adjustment} is a larger than typical setpoint adjustment")
    elsif heating_adjustment.abs > 500
      runner.registerError("#{heating_adjustment} is a larger than typical setpoint adjustment")
      return false
    end

    #setup OpenStudio units that we will need
    temperature_ip_unit = OpenStudio::createUnit("F").get
    temperature_si_unit = OpenStudio::createUnit("C").get

    #define starting units
    cooling_adjustment_ip = OpenStudio::Quantity.new(cooling_adjustment, temperature_ip_unit)
    heating_adjustment_ip = OpenStudio::Quantity.new(heating_adjustment, temperature_ip_unit)

    #push schedules to hash to avoid making unnecessary duplicates
    clg_set_schs = {}
    htg_set_schs = {}

    #get thermostats and setpoint schedules
    thermostats = model.getThermostatSetpointDualSetpoints
    thermostats.each do |thermostat|
      #setup new cooling setpoint schedule
      clg_set_sch = thermostat.coolingSetpointTemperatureSchedule
      if not clg_set_sch.empty?
        # clone of not alredy in hash
        if clg_set_schs.has_key?(clg_set_sch.get.name.to_s)
          new_clg_set_sch = clg_set_schs[clg_set_sch.get.name.to_s]
        else
          new_clg_set_sch = clg_set_sch.get.clone(model)
          new_clg_set_sch = new_clg_set_sch.to_Schedule.get
          new_clg_set_sch_name = new_clg_set_sch.setName("#{new_clg_set_sch.name} adjusted by #{cooling_adjustment_ip}")

          #add to the hash
          clg_set_schs[clg_set_sch.get.name.to_s] = new_clg_set_sch
        end
        #hook up clone to thermostat
        thermostat.setCoolingSetpointTemperatureSchedule(new_clg_set_sch)
      else
        runner.registerWarning("Thermostat '#{thermostat.name.to_s}' doesn't have a cooling setpoint schedule")
      end #end if not clg_set_sch.empty?

      #setup new heating setpoint schedule
      htg_set_sch = thermostat.heatingSetpointTemperatureSchedule
      if not htg_set_sch.empty?
        # clone of not already in hash
        if htg_set_schs.has_key?(htg_set_sch.get.name.to_s)
          new_htg_set_sch = htg_set_schs[htg_set_sch.get.name.to_s]
        else
          new_htg_set_sch = htg_set_sch.get.clone(model)
          new_htg_set_sch = new_htg_set_sch.to_Schedule.get
          new_htg_set_sch_name = new_htg_set_sch.setName("#{new_htg_set_sch.name} adjusted by #{heating_adjustment_ip}")

          #add to the hash
          htg_set_schs[htg_set_sch.get.name.to_s] = new_htg_set_sch
        end
        # hook up clone to thermostat
        thermostat.setHeatingSetpointTemperatureSchedule(new_htg_set_sch)
      else
        runner.registerWarning("Thermostat '#{thermostat.name.to_s}' doesn't have a heating setpoint schedule.")
      end #end if not htg_set_sch.empty?

    end #end thermostats.each do

    #setting up variables to use for initial and final condition
    clg_sch_set_values = [] #may need to flatten this
    htg_sch_set_values = [] #may need to flatten this
    final_clg_sch_set_values = []
    final_htg_sch_set_values = []

    #consider issuing a warning if the model has un-conditioned thermal zones (no ideal air loads or hvac)
    zones = model.getThermalZones
    zones.each do |zone|
      # if you have a thermostat but don't have ideal air loads or zone equipment then issue a warning
      if not zone.thermostatSetpointDualSetpoint.empty? and not zone.useIdealAirLoads and not zone.equipment.size > 0
        runner.registerWarning("Thermal zone '#{zone.name.to_s}' has a thermostat but does not appear to be conditioned.")
      end
    end

    # make cooling schedule adjustments and rename. Put in check to skip and warn if schedule not ruleset
    clg_set_schs.each do |k,v| #old name and new object for schedule
      if not v.to_ScheduleRuleset.empty?

        #array to store profiles in
        profiles = []
        schedule = v.to_ScheduleRuleset.get

        #push default profiles to array
        default_rule = schedule.defaultDaySchedule
        profiles << default_rule

        #push profiles to array
        rules = schedule.scheduleRules
        rules.each do |rule|
          day_sch = rule.daySchedule
          profiles << day_sch
        end

        #add design days to array
        if alter_design_days == true
          summer_design = schedule.summerDesignDaySchedule
          winter_design = schedule.winterDesignDaySchedule
          profiles << summer_design
          #profiles << winter_design
        end

        profiles.each do |sch_day|
          day_time_vector = sch_day.times
          day_value_vector = sch_day.values
          clg_sch_set_values << day_value_vector
          sch_day.clearValues
            for i in 0..(day_time_vector.size - 1)
              v_si = OpenStudio::Quantity.new(day_value_vector[i], temperature_si_unit)
              v_ip = OpenStudio::convert(v_si, temperature_ip_unit).get
              target_v_ip = v_ip + cooling_adjustment_ip
              target_temp_si = OpenStudio::convert(target_v_ip, temperature_si_unit).get
              sch_day.addValue(day_time_vector[i],target_temp_si.value)
            end
          final_clg_sch_set_values << sch_day.values
        end #end of profiles.each do

      else
        runner.registerWarning("Schedule '#{k}' isn't a ScheduleRuleset object and won't be altered by this measure.")
        v.remove #remove un-used clone
      end
    end #end clg_set_schs.each do

    # make heating schedule adjustments and rename. Put in check to skip and warn if schedule not ruleset
    htg_set_schs.each do |k,v| #old name and new object for schedule
      if not v.to_ScheduleRuleset.empty?

        #array to store profiles in
        profiles = []
        schedule = v.to_ScheduleRuleset.get

        #push default profiles to array
        default_rule = schedule.defaultDaySchedule
        profiles << default_rule

        #push profiles to array
        rules = schedule.scheduleRules
        rules.each do |rule|
          day_sch = rule.daySchedule
          profiles << day_sch
        end

        #add design days to array
        if alter_design_days == true
          summer_design = schedule.summerDesignDaySchedule
          winter_design = schedule.winterDesignDaySchedule
          #profiles << summer_design
          profiles << winter_design
        end

        profiles.each do |sch_day|
          day_time_vector = sch_day.times
          day_value_vector = sch_day.values
          htg_sch_set_values << day_value_vector
          sch_day.clearValues
            for i in 0..(day_time_vector.size - 1)
              v_si = OpenStudio::Quantity.new(day_value_vector[i], temperature_si_unit)
              v_ip = OpenStudio::convert(v_si, temperature_ip_unit).get
              target_v_ip = v_ip + heating_adjustment_ip
              target_temp_si = OpenStudio::convert(target_v_ip, temperature_si_unit).get
              sch_day.addValue(day_time_vector[i],target_temp_si.value)
            end
          final_htg_sch_set_values << sch_day.values
        end #end of profiles.each do

      else
        runner.registerWarning("Schedule '#{k}' isn't a ScheduleRuleset object and won't be altered by this measure.")
        v.remove #remove un-used clone
      end
    end #end htg_set_schs.each do

    #get min and max heating and cooling and convert to IP
    clg_sch_set_values = clg_sch_set_values.flatten
    htg_sch_set_values = htg_sch_set_values.flatten
 
    #set NA flag if can't get values for schedules (e.g. if all compact)
    applicable_flag = false
    
    #get min and max if values exist
    if clg_sch_set_values.size > 0
      min_clg_si = OpenStudio::Quantity.new(clg_sch_set_values.min, temperature_si_unit)
      max_clg_si = OpenStudio::Quantity.new(clg_sch_set_values.max, temperature_si_unit)
      min_clg_ip = OpenStudio::convert(min_clg_si, temperature_ip_unit).get
      max_clg_ip = OpenStudio::convert(max_clg_si, temperature_ip_unit).get
      applicable_flag = true
    else
      min_clg_ip = "NA"
      max_clg_ip = "NA"
    end

    #get min and max if values exist
    if htg_sch_set_values.size > 0
      min_htg_si = OpenStudio::Quantity.new(htg_sch_set_values.min, temperature_si_unit)
      max_htg_si = OpenStudio::Quantity.new(htg_sch_set_values.max, temperature_si_unit)
      min_htg_ip = OpenStudio::convert(min_htg_si, temperature_ip_unit).get
      max_htg_ip = OpenStudio::convert(max_htg_si, temperature_ip_unit).get
      applicable_flag = true
    else
      min_htg_ip = "NA"
      max_htg_ip = "NA"
    end
    
    #not applicable if no schedules can be altered
    if applicable_flag == false
      runner.registerAsNotApplicable("No thermostat schedules in the models could be altered.")
    end
 
    #reporting initial condition of model
    starting_spaces = model.getSpaces
    runner.registerInitialCondition("Initial cooling setpoints used in the model range from #{min_clg_ip} to #{max_clg_ip}. Initial heating setpoints used in the model range from #{min_htg_ip} to #{max_htg_ip}.")

    #get min and max heating and cooling and convert to IP for final
    final_clg_sch_set_values = final_clg_sch_set_values.flatten
    final_htg_sch_set_values = final_htg_sch_set_values.flatten

    if clg_sch_set_values.size > 0
      final_min_clg_si = OpenStudio::Quantity.new(final_clg_sch_set_values.min, temperature_si_unit)
      final_max_clg_si = OpenStudio::Quantity.new(final_clg_sch_set_values.max, temperature_si_unit)
      final_min_clg_ip = OpenStudio::convert(final_min_clg_si, temperature_ip_unit).get
      final_max_clg_ip = OpenStudio::convert(final_max_clg_si, temperature_ip_unit).get
    else
      final_min_clg_ip = "NA"
      final_max_clg_ip = "NA"
    end

    #get min and max if values exist
    if htg_sch_set_values.size > 0
      final_min_htg_si = OpenStudio::Quantity.new(final_htg_sch_set_values.min, temperature_si_unit)
      final_max_htg_si = OpenStudio::Quantity.new(final_htg_sch_set_values.max, temperature_si_unit)
      final_min_htg_ip = OpenStudio::convert(final_min_htg_si, temperature_ip_unit).get
      final_max_htg_ip = OpenStudio::convert(final_max_htg_si, temperature_ip_unit).get
    else
      final_min_htg_ip = "NA"
      final_max_htg_ip = "NA"
    end
    
    #reporting final condition of model
    finishing_spaces = model.getSpaces
    runner.registerFinalCondition("Final cooling setpoints used in the model range from #{final_min_clg_ip} to #{final_max_clg_ip}. Final heating setpoints used in the model range from #{final_min_htg_ip} to #{final_max_htg_ip}.")

    return true

  end #end the run method

end #end the measure

#this allows the measure to be used by the application
AdjustTheromstatSetpointsByDegrees.new.registerWithApplication