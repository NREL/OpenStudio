module OsLib_Schedules
  # create a ruleset schedule with a basic profile
  def self.createSimpleSchedule(model, options = {})
    defaults = {
      'name' => nil,
      'winterTimeValuePairs' => { 24.0 => 0.0 },
      'summerTimeValuePairs' => { 24.0 => 1.0 },
      'defaultTimeValuePairs' => { 24.0 => 1.0 }
    }

    # merge user inputs with defaults
    options = defaults.merge(options)

    # ScheduleRuleset
    sch_ruleset = OpenStudio::Model::ScheduleRuleset.new(model)
    if name
      sch_ruleset.setName(options['name'])
    end

    # Winter Design Day
    winter_dsn_day = OpenStudio::Model::ScheduleDay.new(model)
    sch_ruleset.setWinterDesignDaySchedule(winter_dsn_day)
    winter_dsn_day = sch_ruleset.winterDesignDaySchedule
    winter_dsn_day.setName("#{sch_ruleset.name} Winter Design Day")
    options['winterTimeValuePairs'].each do |k, v|
      hour = k.truncate
      min = ((k - hour) * 60).to_i
      winter_dsn_day.addValue(OpenStudio::Time.new(0, hour, min, 0), v)
    end

    # Summer Design Day
    summer_dsn_day = OpenStudio::Model::ScheduleDay.new(model)
    sch_ruleset.setSummerDesignDaySchedule(summer_dsn_day)
    summer_dsn_day = sch_ruleset.summerDesignDaySchedule
    summer_dsn_day.setName("#{sch_ruleset.name} Summer Design Day")
    options['summerTimeValuePairs'].each do |k, v|
      hour = k.truncate
      min = ((k - hour) * 60).to_i
      summer_dsn_day.addValue(OpenStudio::Time.new(0, hour, min, 0), v)
    end

    # All Days
    default_day = sch_ruleset.defaultDaySchedule
    default_day.setName("#{sch_ruleset.name} Schedule Week Day")
    options['defaultTimeValuePairs'].each do |k, v|
      hour = k.truncate
      min = ((k - hour) * 60).to_i
      default_day.addValue(OpenStudio::Time.new(0, hour, min, 0), v)
    end

    result = sch_ruleset
    return result
  end # end of OsLib_Schedules.createSimpleSchedule

  # find the maximum profile value for a schedule
  def self.getMinMaxAnnualProfileValue(model, schedule)
    # validate schedule
    if schedule.to_ScheduleRuleset.is_initialized
      schedule = schedule.to_ScheduleRuleset.get

      # gather profiles
      profiles = []
      defaultProfile = schedule.to_ScheduleRuleset.get.defaultDaySchedule
      profiles << defaultProfile
      rules = schedule.scheduleRules
      rules.each do |rule|
        profiles << rule.daySchedule
      end

      # test profiles
      min = nil
      max = nil
      profiles.each do |profile|
        profile.values.each do |value|
          if min.nil?
            min = value
          else
            if min > value then min = value end
          end
          if max.nil?
            max = value
          else
            if max < value then max = value end
          end
        end
      end
      result = { 'min' => min, 'max' => max } # this doesn't include summer and winter design day
    else
      result =  nil
    end

    return result
  end # end of OsLib_Schedules.getMaxAnnualProfileValue

  # find the maximum profile value for a schedule
  def self.simpleScheduleValueAdjust(model, schedule, double, modificationType = 'Multiplier') # can increase/decrease by percentage or static value
    # TODO: - add in design days, maybe as optional argument

    # give option to clone or not

    # gather profiles
    profiles = []
    defaultProfile = schedule.to_ScheduleRuleset.get.defaultDaySchedule
    profiles << defaultProfile
    rules = schedule.scheduleRules
    rules.each do |rule|
      profiles << rule.daySchedule
    end

    # alter profiles
    profiles.each do |profile|
      times = profile.times
      i = 0
      profile.values.each do |value|
        if modificationType == 'Multiplier' || modificationType == 'Percentage' # percentage was used early on but Multiplier is preferable
          profile.addValue(times[i], value * double)
        end
        if modificationType == 'Sum' || modificationType == 'Value' # value was used early on but Sum is preferable
          profile.addValue(times[i], value + double)
        end
        i += 1
      end
    end

    result = schedule
    return result
  end # end of OsLib_Schedules.getMaxAnnualProfileValue

  # change value when value passes/fails test
  def self.conditionalScheduleValueAdjust(model, schedule, valueTestDouble, passDouble, failDouble, floorDouble, modificationType = 'Multiplier') # can increase/decrease by percentage or static value
    # TODO: - add in design days, maybe as optional argument

    # give option to clone or not

    # gather profiles
    profiles = []
    defaultProfile = schedule.to_ScheduleRuleset.get.defaultDaySchedule
    profiles << defaultProfile
    rules = schedule.scheduleRules
    rules.each do |rule|
      profiles << rule.daySchedule
    end

    # alter profiles
    profiles.each do |profile|
      times = profile.times
      i = 0

      profile.values.each do |value|
        # run test on this value
        if value < valueTestDouble
          double = passDouble
        else
          double = failDouble
        end

        # skip if value is floor or less
        next if value <= floorDouble

        if modificationType == 'Multiplier'
          profile.addValue(times[i], [value * double, floorDouble].max) # take the max of the floor or resulting value
        end
        if modificationType == 'Sum'
          profile.addValue(times[i], [value + double, floorDouble].max) # take the max of the floor or resulting value
        end
        i += 1
      end
    end

    result = schedule
    return result
  end

  # change value when time passes test
  def self.timeConditionalScheduleValueAdjust(model, schedule, hhmm_before, hhmm__after, inside_double, outside_double, modificationType = 'Multiplier') # can increase/decrease by percentage or static value
    # setup variables
    array = hhmm_before.to_s.split('')
    before_hour = "#{array[0]}#{array[1]}".to_i
    before_min = "#{array[2]}#{array[3]}".to_i
    array = hhmm__after.to_s.split('')
    after_hour = "#{array[0]}#{array[1]}".to_i
    after_min = "#{array[2]}#{array[3]}".to_i

    # gather profiles
    profiles = []
    schedule = schedule.to_ScheduleRuleset.get
    defaultProfile = schedule.defaultDaySchedule
    profiles << defaultProfile
    rules = schedule.scheduleRules
    rules.each do |rule|
      profiles << rule.daySchedule
    end

    # alter profiles
    profiles.each do |day_sch|
      times = day_sch.times
      i = 0

      # set times special times needed for methods below
      before_time = OpenStudio::Time.new(0, before_hour, before_min, 0)
      after_time = OpenStudio::Time.new(0, after_hour, after_min, 0)
      # day_end_time = OpenStudio::Time.new(0, 24, 0, 0)

      # add datapoint at before and after time
      original_value_at_before_time = day_sch.getValue(before_time)
      original_value_at_after_time = day_sch.getValue(after_time)
      day_sch.addValue(before_time, original_value_at_before_time)
      day_sch.addValue(after_time, original_value_at_after_time)

      # make arrays for original times and values
      times = day_sch.times
      values = day_sch.values
      day_sch.clearValues

      # make arrays for new values
      new_times = []
      new_values = []

      # loop through original time/value pairs to populate new array
      for i in 0..(values.length - 1)
        new_times << times[i]

        if times[i] > before_time && times[i] <= after_time # updated this so times[i] == before_time goes into the else
          if inside_double.nil?
            new_values << values[i]
          elsif modificationType == 'Sum'
            new_values << inside_double + values[i]
          elsif modificationType == 'Replace'
            new_values << inside_double
          else # should be Multiplier
            new_values << inside_double * values[i]
          end
        else
          if outside_double.nil?
            new_values << values[i]
          elsif modificationType == 'Sum'
            new_values << outside_double + values[i]
          elsif modificationType == 'Replace'
            new_values << outside_double
          else # should be Multiplier
            new_values << outside_double * values[i]
          end
        end

      end

      # generate new day_sch values
      for i in 0..(new_values.length - 1)
        day_sch.addValue(new_times[i], new_values[i])
      end
    end

    result = schedule
    return result
  end

  # merge multiple schedules into one using load or other value to weight each schedules influence on the merge
  def self.weightedMergeScheduleRulesets(model, scheduleWeighHash)
    # WARNING NOT READY FOR GENERAL USE YET - this doesn't do anything with rules yet, just winter, summer, and default profile

    # get denominator for weight
    denominator = 0
    scheduleWeighHash.each do |schedule, weight|
      denominator += weight
    end

    # create new schedule
    sch_ruleset = OpenStudio::Model::ScheduleRuleset.new(model)
    sch_ruleset.setName('Merged Schedule') # TODO: - make this optional user argument

    # create winter design day profile
    winter_dsn_day = OpenStudio::Model::ScheduleDay.new(model)
    sch_ruleset.setWinterDesignDaySchedule(winter_dsn_day)
    winter_dsn_day = sch_ruleset.winterDesignDaySchedule
    winter_dsn_day.setName("#{sch_ruleset.name} Winter Design Day")

    # create  summer design day profile
    summer_dsn_day = OpenStudio::Model::ScheduleDay.new(model)
    sch_ruleset.setSummerDesignDaySchedule(summer_dsn_day)
    summer_dsn_day = sch_ruleset.summerDesignDaySchedule
    summer_dsn_day.setName("#{sch_ruleset.name} Summer Design Day")

    # create default profile
    default_day = sch_ruleset.defaultDaySchedule
    default_day.setName("#{sch_ruleset.name} Schedule Week Day")

    # hash of schedule rules
    rulesHash = {} # mon, tue, wed, thur, fri, sat, sun, startDate, endDate
    # to avoid stacking order issues across schedules, I may need to make a rule for each day of the week for each date range

    scheduleWeighHash.each do |schedule, weight|
      # populate winter design day profile
      oldWinterProfile = schedule.to_ScheduleRuleset.get.winterDesignDaySchedule
      times_final = summer_dsn_day.times
      i = 0
      valueUpdatedArray = []
      # loop through times already in profile and update values
      until i > times_final.size - 1
        value = oldWinterProfile.getValue(times_final[i]) * weight / denominator
        starting_value = winter_dsn_day.getValue(times_final[i])
        winter_dsn_day.addValue(times_final[i], value + starting_value)
        valueUpdatedArray << times_final[i]
        i += 1
      end
      # loop through any new times unique to the current old profile to be merged
      j = 0
      times = oldWinterProfile.times
      values = oldWinterProfile.values
      until j > times.size - 1
        unless valueUpdatedArray.include? times[j]
          value = values[j] * weight / denominator
          starting_value = winter_dsn_day.getValue(times[j])
          winter_dsn_day.addValue(times[j], value + starting_value)
        end
        j += 1
      end

      # populate summer design day profile
      oldSummerProfile = schedule.to_ScheduleRuleset.get.summerDesignDaySchedule
      times_final = summer_dsn_day.times
      i = 0
      valueUpdatedArray = []
      # loop through times already in profile and update values
      until i > times_final.size - 1
        value = oldSummerProfile.getValue(times_final[i]) * weight / denominator
        starting_value = summer_dsn_day.getValue(times_final[i])
        summer_dsn_day.addValue(times_final[i], value + starting_value)
        valueUpdatedArray << times_final[i]
        i += 1
      end
      # loop through any new times unique to the current old profile to be merged
      j = 0
      times = oldSummerProfile.times
      values = oldSummerProfile.values
      until j > times.size - 1
        unless valueUpdatedArray.include? times[j]
          value = values[j] * weight / denominator
          starting_value = summer_dsn_day.getValue(times[j])
          summer_dsn_day.addValue(times[j], value + starting_value)
        end
        j += 1
      end

      # populate default profile
      oldDefaultProfile = schedule.to_ScheduleRuleset.get.defaultDaySchedule
      times_final = default_day.times
      i = 0
      valueUpdatedArray = []
      # loop through times already in profile and update values
      until i > times_final.size - 1
        value = oldDefaultProfile.getValue(times_final[i]) * weight / denominator
        starting_value = default_day.getValue(times_final[i])
        default_day.addValue(times_final[i], value + starting_value)
        valueUpdatedArray << times_final[i]
        i += 1
      end
      # loop through any new times unique to the current old profile to be merged
      j = 0
      times = oldDefaultProfile.times
      values = oldDefaultProfile.values
      until j > times.size - 1
        unless valueUpdatedArray.include? times[j]
          value = values[j] * weight / denominator
          starting_value = default_day.getValue(times[j])
          default_day.addValue(times[j], value + starting_value)
        end
        j += 1
      end

      # create rules

      # gather data for rule profiles

      # populate rule profiles
    end

    result = { 'mergedSchedule' => sch_ruleset, 'denominator' => denominator }
    return result
  end # end of OsLib_Schedules.weightedMergeScheduleRulesets

  # create a new schedule using absolute velocity of existing schedule
  def self.scheduleFromRateOfChange(model, schedule)
    # clone source schedule
    newSchedule = schedule.clone(model)
    newSchedule.setName("#{schedule.name} - Rate of Change")
    newSchedule = newSchedule.to_ScheduleRuleset.get

    # create array of all profiles to change. This includes summer, winter, default, and rules
    profiles = []
    profiles << newSchedule.winterDesignDaySchedule
    profiles << newSchedule.summerDesignDaySchedule
    profiles << newSchedule.defaultDaySchedule

    # time values may need
    endProfileTime = OpenStudio::Time.new(0, 24, 0, 0)
    hourBumpTime = OpenStudio::Time.new(0, 1, 0, 0)
    oneHourLeftTime = OpenStudio::Time.new(0, 23, 0, 0)

    rules = newSchedule.scheduleRules
    rules.each do |rule|
      profiles << rule.daySchedule
    end

    profiles.uniq.each do |profile|
      times = profile.times
      values = profile.values

      i = 0
      valuesIntermediate = []
      timesIntermediate = []
      until i == (values.size)
        if i == 0
          valuesIntermediate << 0.0
          if times[i] > hourBumpTime
            timesIntermediate << times[i] - hourBumpTime
            if times[i + 1].nil?
              timeStepValue = endProfileTime.hours + endProfileTime.minutes / 60 - times[i].hours - times[i].minutes / 60
            else
              timeStepValue = times[i + 1].hours + times[i + 1].minutes / 60 - times[i].hours - times[i].minutes / 60
            end
            valuesIntermediate << (values[i + 1].to_f - values[i].to_f).abs / (timeStepValue * 2)
          end
          timesIntermediate << times[i]
        elsif i == (values.size - 1)
          if times[times.size - 2] < oneHourLeftTime
            timesIntermediate << times[times.size - 2] + hourBumpTime # this should be the second to last time
            timeStepValue = times[i - 1].hours + times[i - 1].minutes / 60 - times[i - 2].hours - times[i - 2].minutes / 60
            valuesIntermediate << (values[i - 1].to_f - values[i - 2].to_f).abs / (timeStepValue * 2)
          end
          valuesIntermediate << 0.0
          timesIntermediate << times[i] # this should be the last time
        else
          # get value multiplier based on how many hours it is spread over
          timeStepValue = times[i].hours + times[i].minutes / 60 - times[i - 1].hours - times[i - 1].minutes / 60
          valuesIntermediate << (values[i].to_f - values[i - 1].to_f).abs / timeStepValue
          timesIntermediate << times[i]
        end
        i += 1
      end

      # delete all profile values
      profile.clearValues

      i = 0
      until i == (timesIntermediate.size)
        if i == (timesIntermediate.size - 1)
          profile.addValue(timesIntermediate[i], valuesIntermediate[i].to_f)
        else
          profile.addValue(timesIntermediate[i], valuesIntermediate[i].to_f)
        end
        i += 1
      end
    end

    # fix velocity so it isn't fraction change per step, but per hour (I need to count hours between times and divide value by this)

    result = newSchedule
    return result
  end # end of OsLib_Schedules.createSimpleSchedule

  # create a complex ruleset schedule
  def self.createComplexSchedule(model, options = {})
    defaults = {
      'name' => nil,
      'default_day' => ['always_on', [24.0, 1.0]]
    }

    # merge user inputs with defaults
    options = defaults.merge(options)

    # ScheduleRuleset
    sch_ruleset = OpenStudio::Model::ScheduleRuleset.new(model)
    if name
      sch_ruleset.setName(options['name'])
    end

    # Winter Design Day
    unless options['winter_design_day'].nil?
      winter_dsn_day = OpenStudio::Model::ScheduleDay.new(model)
      sch_ruleset.setWinterDesignDaySchedule(winter_dsn_day)
      winter_dsn_day = sch_ruleset.winterDesignDaySchedule
      winter_dsn_day.setName("#{sch_ruleset.name} Winter Design Day")
      options['winter_design_day'].each do |data_pair|
        hour = data_pair[0].truncate
        min = ((data_pair[0] - hour) * 60).to_i
        winter_dsn_day.addValue(OpenStudio::Time.new(0, hour, min, 0), data_pair[1])
      end
    end

    # Summer Design Day
    unless options['summer_design_day'].nil?
      summer_dsn_day = OpenStudio::Model::ScheduleDay.new(model)
      sch_ruleset.setSummerDesignDaySchedule(summer_dsn_day)
      summer_dsn_day = sch_ruleset.summerDesignDaySchedule
      summer_dsn_day.setName("#{sch_ruleset.name} Summer Design Day")
      options['summer_design_day'].each do |data_pair|
        hour = data_pair[0].truncate
        min = ((data_pair[0] - hour) * 60).to_i
        summer_dsn_day.addValue(OpenStudio::Time.new(0, hour, min, 0), data_pair[1])
      end
    end

    # Default Day
    default_day = sch_ruleset.defaultDaySchedule
    default_day.setName("#{sch_ruleset.name} #{options['default_day'][0]}")
    default_data_array = options['default_day']
    default_data_array.delete_at(0)
    default_data_array.each do |data_pair|
      hour = data_pair[0].truncate
      min = ((data_pair[0] - hour) * 60).to_i
      default_day.addValue(OpenStudio::Time.new(0, hour, min, 0), data_pair[1])
    end

    # Rules
    unless options['rules'].nil?
      options['rules'].each do |data_array|
        rule = OpenStudio::Model::ScheduleRule.new(sch_ruleset)
        rule.setName("#{sch_ruleset.name} #{data_array[0]} Rule")
        date_range = data_array[1].split('-')
        start_date = date_range[0].split('/')
        end_date = date_range[1].split('/')
        rule.setStartDate(model.getYearDescription.makeDate(start_date[0].to_i, start_date[1].to_i))
        rule.setEndDate(model.getYearDescription.makeDate(end_date[0].to_i, end_date[1].to_i))
        days = data_array[2].split('/')
        rule.setApplySunday(true) if days.include? 'Sun'
        rule.setApplyMonday(true) if days.include? 'Mon'
        rule.setApplyTuesday(true) if days.include? 'Tue'
        rule.setApplyWednesday(true) if days.include? 'Wed'
        rule.setApplyThursday(true) if days.include? 'Thu'
        rule.setApplyFriday(true) if days.include? 'Fri'
        rule.setApplySaturday(true) if days.include? 'Sat'
        day_schedule = rule.daySchedule
        day_schedule.setName("#{sch_ruleset.name} #{data_array[0]}")
        data_array.delete_at(0)
        data_array.delete_at(0)
        data_array.delete_at(0)
        data_array.each do |data_pair|
          hour = data_pair[0].truncate
          min = ((data_pair[0] - hour) * 60).to_i
          day_schedule.addValue(OpenStudio::Time.new(0, hour, min, 0), data_pair[1])
        end
      end
    end

    result = sch_ruleset
    return result
  end # end of OsLib_Schedules.createComplexSchedule

  def self.addScheduleTypeLimits(model) # TODO: - make sure to add this new method to cofee when done
    type_limits = {}

    lightsScheduleTypeLimits = OpenStudio::Model::ScheduleTypeLimits.new(model)
    lightsScheduleTypeLimits.setName('Lights Schedule Type Limits')
    lightsScheduleTypeLimits.setLowerLimitValue(0.0)
    lightsScheduleTypeLimits.setUpperLimitValue(1.0)
    lightsScheduleTypeLimits.setNumericType('Continuous')
    lightsScheduleTypeLimits.setUnitType('Dimensionless')
    type_limits['Lights'] = lightsScheduleTypeLimits

    occupancyScheduleTypeLimits = OpenStudio::Model::ScheduleTypeLimits.new(model)
    occupancyScheduleTypeLimits.setName('Occupancy Schedule Type Limits')
    occupancyScheduleTypeLimits.setLowerLimitValue(0.0)
    occupancyScheduleTypeLimits.setUpperLimitValue(1.0)
    occupancyScheduleTypeLimits.setNumericType('Continuous')
    occupancyScheduleTypeLimits.setUnitType('Dimensionless')
    type_limits['Occupancy'] = occupancyScheduleTypeLimits

    peopleActivityScheduleTypeLimits = OpenStudio::Model::ScheduleTypeLimits.new(model)
    peopleActivityScheduleTypeLimits.setName('People Activity Type Limits')
    peopleActivityScheduleTypeLimits.setLowerLimitValue(0.0)
    # peopleActivityScheduleTypeLimits.setUpperLimitValue(1500.0)
    peopleActivityScheduleTypeLimits.setNumericType('Continuous')
    peopleActivityScheduleTypeLimits.setUnitType('ActivityLevel')
    type_limits['People Activity'] = peopleActivityScheduleTypeLimits

    equipmentScheduleTypeLimits = OpenStudio::Model::ScheduleTypeLimits.new(model)
    equipmentScheduleTypeLimits.setName('Equipment Schedule Type Limits')
    equipmentScheduleTypeLimits.setLowerLimitValue(0.0)
    equipmentScheduleTypeLimits.setUpperLimitValue(1.0)
    equipmentScheduleTypeLimits.setNumericType('Continuous')
    equipmentScheduleTypeLimits.setUnitType('Dimensionless')
    type_limits['Equipment'] = equipmentScheduleTypeLimits

    waterUseScheduleTypeLimits = OpenStudio::Model::ScheduleTypeLimits.new(model)
    waterUseScheduleTypeLimits.setName('Water Use Schedule Type Limits')
    waterUseScheduleTypeLimits.setLowerLimitValue(0.0)
    waterUseScheduleTypeLimits.setUpperLimitValue(1.0)
    waterUseScheduleTypeLimits.setNumericType('Continuous')
    waterUseScheduleTypeLimits.setUnitType('Dimensionless')
    type_limits['Water Use'] = waterUseScheduleTypeLimits

    elevatorsScheduleTypeLimits = OpenStudio::Model::ScheduleTypeLimits.new(model)
    elevatorsScheduleTypeLimits.setName('Elevators Schedule Type Limits')
    elevatorsScheduleTypeLimits.setLowerLimitValue(0.0)
    elevatorsScheduleTypeLimits.setUpperLimitValue(1.0)
    elevatorsScheduleTypeLimits.setNumericType('Continuous')
    elevatorsScheduleTypeLimits.setUnitType('Dimensionless')
    type_limits['Elevators'] = elevatorsScheduleTypeLimits

    processLoadsScheduleTypeLimits = OpenStudio::Model::ScheduleTypeLimits.new(model)
    processLoadsScheduleTypeLimits.setName('Process Loads Schedule Type Limits')
    processLoadsScheduleTypeLimits.setLowerLimitValue(0.0)
    processLoadsScheduleTypeLimits.setUpperLimitValue(1.0)
    processLoadsScheduleTypeLimits.setNumericType('Continuous')
    processLoadsScheduleTypeLimits.setUnitType('Dimensionless')
    type_limits['Process Load'] = elevatorsScheduleTypeLimits

    thermostatHeatingScheduleTypeLimits = OpenStudio::Model::ScheduleTypeLimits.new(model)
    thermostatHeatingScheduleTypeLimits.setName('Thermostat Heating Setpoint Schedule Type Limits')
    thermostatHeatingScheduleTypeLimits.setLowerLimitValue(0.0)
    thermostatHeatingScheduleTypeLimits.setUpperLimitValue(100.0)
    thermostatHeatingScheduleTypeLimits.setNumericType('Continuous')
    thermostatHeatingScheduleTypeLimits.setUnitType('Temperature')
    type_limits['Thermostat Heating Setpoint'] = thermostatHeatingScheduleTypeLimits

    temperatureScheduleTypeLimits = OpenStudio::Model::ScheduleTypeLimits.new(model)
    temperatureScheduleTypeLimits.setName('Thermostat Cooling Setpoint Schedule Type Limits')
    temperatureScheduleTypeLimits.setLowerLimitValue(0.0)
    temperatureScheduleTypeLimits.setUpperLimitValue(100.0)
    temperatureScheduleTypeLimits.setNumericType('Continuous')
    temperatureScheduleTypeLimits.setUnitType('Temperature')
    type_limits['Thermostat Cooling Setpoint'] = temperatureScheduleTypeLimits

    hvacOperationScheduleTypeLimits = OpenStudio::Model::ScheduleTypeLimits.new(model)
    hvacOperationScheduleTypeLimits.setName('HVAC Operation Schedule Type Limits')
    hvacOperationScheduleTypeLimits.setLowerLimitValue(0)
    hvacOperationScheduleTypeLimits.setUpperLimitValue(1)
    hvacOperationScheduleTypeLimits.setNumericType('Discrete')
    hvacOperationScheduleTypeLimits.setUnitType('Availability')
    type_limits['HVAC Operation'] = hvacOperationScheduleTypeLimits

    temperatureScheduleTypeLimits = OpenStudio::Model::ScheduleTypeLimits.new(model)
    temperatureScheduleTypeLimits.setName('Temperature Schedule Type Limits')
    temperatureScheduleTypeLimits.setNumericType('Continuous')
    temperatureScheduleTypeLimits.setUnitType('Temperature')
    type_limits['Temperature'] = temperatureScheduleTypeLimits

    fractionScheduleTypeLimits = OpenStudio::Model::ScheduleTypeLimits.new(model)
    fractionScheduleTypeLimits.setName('Fraction Schedule Type Limits')
    fractionScheduleTypeLimits.setLowerLimitValue(0.0)
    fractionScheduleTypeLimits.setUpperLimitValue(1.0)
    fractionScheduleTypeLimits.setNumericType('Continuous')
    fractionScheduleTypeLimits.setUnitType('Dimensionless')
    type_limits['Fraction'] = fractionScheduleTypeLimits

    dimensionlessScheduleTypeLimits = OpenStudio::Model::ScheduleTypeLimits.new(model)
    dimensionlessScheduleTypeLimits.setName('Dimensionless Schedule Type Limits')
    dimensionlessScheduleTypeLimits.setNumericType('Continuous')
    dimensionlessScheduleTypeLimits.setUnitType('Dimensionless')
    type_limits['Dimensionless'] = dimensionlessScheduleTypeLimits

    return type_limits
  end

  # create TimeSeries from ScheduleRuleset
  def self.create_timeseries_from_schedule_ruleset(model, schedule_ruleset)
    yd = model.getYearDescription
    start_date = yd.makeDate(1, 1)
    end_date = yd.makeDate(12, 31)

    values = OpenStudio::DoubleVector.new
    day = OpenStudio::Time.new(1.0)
    interval = OpenStudio::Time.new(1.0 / 48.0)
    day_schedules = schedule_ruleset.to_ScheduleRuleset.get.getDaySchedules(start_date, end_date)
    day_schedules.each do |day_schedule|
      time = interval
      while time < day
        values << day_schedule.getValue(time)
        time += interval
      end
    end
    time_series = OpenStudio::TimeSeries.new(start_date, interval, OpenStudio.createVector(values), '')
  end

  # create ScheduleVariableInterval from TimeSeries
  def self.create_schedule_variable_interval_from_time_series(model, time_series)
    result = OpenStudio::Model::ScheduleInterval.fromTimeSeries(time_series, model).get
  end

  def self.adjust_hours_of_operation_for_schedule_ruleset(runner, model, schedule, options = {})
    defaults = {
      'base_start_hoo' => 8.0, # may not be good idea to have default
      'base_finish_hoo' => 18.0, # may not be good idea to have default
      'delta_length_hoo' => 0.0,
      'shift_hoo' => 0.0,
      'default' => true,
      'mon' => true,
      'tue' => true,
      'wed' => true,
      'thur' => true,
      'fri' => true,
      'sat' => true,
      'sun' => true,
      'summer' => false,
      'winter' => false
    }

    # merge user inputs with defaults
    options = defaults.merge(options)

    # grab schedule out of argument
    if schedule.to_ScheduleRuleset.is_initialized
      schedule = schedule.to_ScheduleRuleset.get
    else
      runner.registerWarning("you should only pass ruleset schedules into this method. skipping #{schedule.name}")
      return nil
    end

    # array of all profiles to change
    profiles = []

    # push default profiles to array
    if options['default']
      default_rule = schedule.defaultDaySchedule
      profiles << default_rule
    end

    # push profiles to array
    rules = schedule.scheduleRules
    rules.each do |rule|
      day_sch = rule.daySchedule

      # if any day requested also exists in the rule, then it will be altered
      alter_rule = false
      if rule.applyMonday && rule.applyMonday == options['mon'] then alter_rule = true end
      if rule.applyTuesday && rule.applyTuesday == options['tue'] then alter_rule = true end
      if rule.applyWednesday && rule.applyWednesday == options['wed'] then alter_rule = true end
      if rule.applyThursday && rule.applyThursday == options['thur'] then alter_rule = true end
      if rule.applyFriday && rule.applyFriday == options['fri'] then alter_rule = true end
      if rule.applySaturday && rule.applySaturday == options['sat'] then alter_rule = true end
      if rule.applySunday && rule.applySunday == options['sun'] then alter_rule = true end

      # TODO: - add in logic to warn user about conflicts where a single rule has conflicting tests

      if alter_rule
        profiles << day_sch
      end
    end

    # add design days to array
    if options['summer']
      summer_design = schedule.summerDesignDaySchedule
      profiles << summer_design
    end
    if options['winter']
      winter_design = schedule.winterDesignDaySchedule
      profiles << winter_design
    end

    # give info messages as I change specific profiles
    runner.registerInfo("Adjusting #{schedule.name}")

    # rename schedule
    schedule.setName("#{schedule.name} - extend #{options['delta_length_hoo']} shift #{options['shift_hoo']}") # if I put inputs here name will get long

    # break time args into hours and minutes
    start_hoo_hours = (options['base_start_hoo']).to_i
    start_hoo_minutes = (((options['base_start_hoo']) - (options['base_start_hoo']).to_i) * 60).to_i
    finish_hoo_hours = (options['base_finish_hoo']).to_i
    finish_hoo_minutes = (((options['base_finish_hoo']) - (options['base_finish_hoo']).to_i) * 60).to_i
    delta_hours = (options['delta_length_hoo']).to_i
    delta_minutes = (((options['delta_length_hoo']) - (options['delta_length_hoo']).to_i) * 60).to_i
    shift_hours = (options['shift_hoo']).to_i
    shift_minutes = (((options['shift_hoo']) - (options['shift_hoo']).to_i) * 60).to_i

    # time objects to use in measure
    time_0 =  OpenStudio::Time.new(0, 0, 0, 0)
    time_1_min =  OpenStudio::Time.new(0, 0, 1, 0) # add this to avoid times in day profile less than this
    time_12 =  OpenStudio::Time.new(0, 12, 0, 0)
    time_24 =  OpenStudio::Time.new(0, 24, 0, 0)
    start_hoo_time = OpenStudio::Time.new(0, start_hoo_hours, start_hoo_minutes, 0)
    finish_hoo_time = OpenStudio::Time.new(0, finish_hoo_hours, finish_hoo_minutes, 0)
    delta_time = OpenStudio::Time.new(0, delta_hours, delta_minutes, 0) # not used
    shift_time = OpenStudio::Time.new(0, shift_hours, shift_minutes, 0)

    # calculations
    if options['base_start_hoo'] <= options['base_finish_hoo']
      base_opp_day_length = options['base_finish_hoo'] - options['base_start_hoo']
      mid_hoo = start_hoo_time + (finish_hoo_time - start_hoo_time) / 2
      mid_non_hoo = mid_hoo + time_12
      if mid_non_hoo > time_24 then mid_non_hoo -= time_24 end
    else
      base_opp_day_length = options['base_finish_hoo'] - options['base_start_hoo'] + 24
      mid_non_hoo = finish_hoo_time + (start_hoo_time - finish_hoo_time) / 2
      mid_hoo = mid_non_hoo + time_12
      if mid_non_hoo > time_24 then mid_non_hoo -= time_24 end
    end
    adjusted_opp_day_length = base_opp_day_length + options['delta_length_hoo']
    hoo_time_multiplier = adjusted_opp_day_length / base_opp_day_length
    non_hoo_time_multiplier = (24 - adjusted_opp_day_length) / (24 - base_opp_day_length)

    # check for invalid input
    if adjusted_opp_day_length < 0
      runner.registerError('Requested hours of operation adjustment results in an invalid negative hours of operation')
      return false
    end
    # check for invalid input
    if adjusted_opp_day_length > 24
      runner.registerError('Requested hours of operation adjustment results in more than 24 hours of operation')
      return false
    end

    # making some temp objects to avoid having to deal with wrap around for change of hoo times
    if mid_hoo < start_hoo_time then adj_mid_hoo = mid_hoo + time_24 else adj_mid_hoo = mid_hoo end
    if finish_hoo_time < adj_mid_hoo then adj_finish_hoo_time = finish_hoo_time + time_24 else adj_finish_hoo_time = finish_hoo_time end
    if mid_non_hoo < adj_finish_hoo_time then adj_mid_non_hoo = mid_non_hoo + time_24 else adj_mid_non_hoo = mid_non_hoo end
    adj_start = start_hoo_time + time_24 # not used

    # edit profiles
    profiles.each do |day_sch|
      times = day_sch.times
      values = day_sch.values

      # arrays for values to avoid overlap conflict of times
      new_times = []
      new_values = []

      # this is to store what datapoint will be first after midnight, and what the value at that time should be
      min_time_new = time_24
      min_time_value = nil

      # flag if found time at 24
      found_24_or_0 = false

      # push times to array
      times.each do |time|
        # create logic for four possible quadrants. Assume any quadrant can pass over 24/0 threshold
        if time < start_hoo_time then temp_time = time + time_24 else temp_time = time end

        # calculate change in time do to hoo delta
        if temp_time <= adj_finish_hoo_time
          expand_time = (temp_time - adj_mid_hoo) * hoo_time_multiplier - (temp_time - adj_mid_hoo)
        else
          expand_time = (temp_time - adj_mid_non_hoo) * non_hoo_time_multiplier - (temp_time - adj_mid_non_hoo)
        end

        new_time = time + shift_time + expand_time

        # adjust wrap around times
        if new_time < time_0
          new_time += time_24
        elsif new_time > time_24
          new_time -= time_24
        end
        new_times << new_time

        # see which new_time has the lowest value. Then add a value at 24 equal to that
        if !found_24_or_0 && new_time <= min_time_new
          min_time_new = new_time
          min_time_value = day_sch.getValue(time)
        elsif new_time == time_24 # this was added to address time exactly at 24
          min_time_new = new_time
          min_time_value = day_sch.getValue(time)
          found_24_or_0 = true
        elsif new_time == time_0
          min_time_new = new_time
          min_time_value = day_sch.getValue(time_0)
          found_24_or_0 = true
        end
      end  # end of times.each do

      # push values to array
      values.each do |value|
        new_values << value
      end # end of values.each do

      # add value for what will be 24
      new_times << time_24
      new_values << min_time_value

      # clear values
      day_sch.clearValues

      # make new values
      for i in 0..(new_values.length - 1)
        # skip datapoint if time is less than 1 minute, this will crash forward translation
        if new_times[i] > time_1_min
          day_sch.addValue(new_times[i], new_values[i])
        end
      end
    end  # end of profiles.each do

    return schedule
  end
  end
