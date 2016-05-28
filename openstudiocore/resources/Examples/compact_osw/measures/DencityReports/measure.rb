# DencityReports generates data that are required for the DEnCity API.

# Author: Henry Horsey (github: henryhorsey)
# Creation Date: 6/27/2014

require 'openstudio'

class DencityReports < OpenStudio::Ruleset::ReportingUserScript

  #define the name that a user will see, this method may be deprecated as
  #the display name in PAT comes from the name field in measure.xml
  def name
    "Dencity Reports"
  end

  #define the arguments that the user will input
  def arguments
    args = OpenStudio::Ruleset::OSArgumentVector.new

    #make choice argument for facade
    choices = OpenStudio::StringVector.new
    choices << "MessagePack"
    choices << "CSV"
    choices << "Both"
    output_format = OpenStudio::Ruleset::OSArgument::makeChoiceArgument("output_format", choices)
    output_format.setDisplayName("Output Format")
    output_format.setDefaultValue("Both")
    args << output_format

    args
  end

  #short_os_fuel method
  def short_os_fuel(fuel_string)
    val = nil
    fuel_vec = fuel_string.split(" ")
    if fuel_vec[0] == "Electricity"
      val = "Elec"
    elsif fuel_vec[0] == "District"
      fuel_vec[1] == "Heating" ? val = "Dist Heat" : val = "Dist Cool"
    elsif fuel_vec[0] == "Natural"
      val = "NG"
    elsif fuel_vec[0] == "Additional"
      val = "Other Fuel"
    elsif fuel_vec[0] == "Water"
      val = "Water"
    else
      val = "Unknown"
    end

    val

  end

  #short_os_cat method
  def short_os_cat(category_string)
    val = nil
    cat_vec = category_string.split(" ")
    if cat_vec[0] == "Heating"
      val = "Heat"
    elsif cat_vec[0] == "Cooling"
      val = "Cool"
    elsif cat_vec[0] == "Humidification"
      val = "Humid"
    elsif cat_vec[0] == "Interior"
      cat_vec[1] == "Lighting" ? val = "Int Light" : val = "Int Equip"
    elsif cat_vec[0] == "Exterior"
      cat_vec[1] == "Lighting" ? val = "Ext Light" : val = "Ext Equip"
    elsif cat_vec[0] == "Heat"
      cat_vec[1] == "Recovery" ? val = "Heat Rec" : val = "Heat Rej"
    elsif cat_vec[0] == "Pumps"
      val = "Pumps"
    elsif cat_vec[0] == "Fans"
      val = "Fans"
    elsif cat_vec[0] == "Refrigeration"
      val = "Rfg"
    elsif cat_vec[0] == "Generators"
      val = "Gen"
    elsif cat_vec[0] == "Water"
      val = "Water Systems"
    else
      val = "Unknown"
    end

    val

  end

  #sql_query method
  def sql_query(runner, sql, report_name, query)
    val = nil
    result = sql.execAndReturnFirstDouble("SELECT Value FROM TabularDataWithStrings WHERE ReportName='#{report_name}' AND #{query}")
    if result.empty?
      runner.registerWarning("Query failed for #{report_name} and #{query}")
    else
      begin
        val = result.get
      rescue
        val = nil
        runner.registerWarning("Query result.get failed")
      end
    end

    val
  end

  #define what happens when the measure is run
  def run(runner, user_arguments)
    super(runner, user_arguments)

    #use the built-in error checking
    unless runner.validateUserArguments(arguments, user_arguments)
      return false
    end

    # require 'ruby-prof'

    begin
      # RubyProf.start

      output_format = runner.getStringArgumentValue("output_format", user_arguments)
      os_version = OpenStudio::VersionString.new(OpenStudio::openStudioVersion())
      min_version_feature1 = OpenStudio::VersionString.new("1.2.3")
      require "time"

      unless os_version >= min_version_feature1
        runner.registerError("Dencity Reports requires a version of OpenStudio greater than 1.2.3.")
        return false
      end

      # determine how to format time series output
      msgpack_flag = FALSE
      csv_flag = FALSE
      if output_format == "MessagePack" || output_format == "Both"
        msgpack_flag = TRUE
      end
      if output_format == "CSV" || output_format == "Both"
        csv_flag = TRUE
      end

      # get the last model and sql file
      model = runner.lastOpenStudioModel
      if model.empty?
        runner.registerError("Cannot find last model.")
        return false
      end
      model = model.get
      building = model.getBuilding

      runner.registerInfo("Model loaded")

      sql_file = runner.lastEnergyPlusSqlFile
      if sql_file.empty?
        runner.registerError("Cannot find last sql file.")
        return false
      end
      sql_file = sql_file.get
      model.setSqlFile(sql_file)

      runner.registerInfo("Sql loaded")

      #Initalize array that will be used to construct the DEnCity metadata csv
      metadata = Array.new
      metadata[0] = ["name", "display_name", "short_name", "description", "unit", "datatype", "user_defined"]

      #get building footprint to use for calculating end use EUIs
      building_area = sql_query(runner, sql_file, "AnnualBuildingUtilityPerformanceSummary", "TableName='Building Area' AND RowName='Total Building Area' AND ColumnName='Area'")
      building_area ||= "NIL"
      runner.registerValue("building_area", building_area, "m2")
      metadata[metadata.length] = ["building_area", "Total Building Area", "Bldg Area", "Total building area as calculated by Energy Plus", "square_meter", "double", "FALSE"]

      #get end use totals for fuels
      site_energy_use = 0.0
      OpenStudio::EndUseFuelType.getValues.each do |fuel_type|
        fuel_str = OpenStudio::EndUseFuelType.new(fuel_type).valueDescription
        fuel_type_aggregation = 0.0
        mult_factor = 1
        if fuel_str != "Water"
          runner_units_eui = "MJ/m2"
          metadata_units_eui = "megajoules_per_square_meter"
          mult_factor = 1000
          runner_units_agg = "GJ"
          metadata_units_agg = "gigajoule"
        else
          runner_units_eui = "m"
          metadata_units_eui = "meter"
          runner_units_agg = "m3"
          metadata_units_agg = "cubic meter"
        end
        OpenStudio::EndUseCategoryType.getValues.each do |category_type|
          category_str = OpenStudio::EndUseCategoryType.new(category_type).valueDescription
          temp_val = sql_query(runner, sql_file, "AnnualBuildingUtilityPerformanceSummary", "TableName='End Uses' AND RowName='#{category_str}' AND ColumnName='#{fuel_str}'")
          if temp_val
            eui_val = temp_val / building_area * mult_factor
            prefix_str = OpenStudio::toUnderscoreCase("#{fuel_str}_#{category_str}_eui")
            runner.registerValue("#{prefix_str}", eui_val, "#{runner_units_eui}")
            short_name = "#{short_os_fuel(fuel_str)} #{short_os_cat(category_str)} EUI"
            metadata[metadata.length] = [prefix_str, "#{category_str} #{fuel_str} EUI", short_name, "Total #{fuel_str.downcase} used for #{category_str.downcase} per square foot", metadata_units_eui, "double", "FALSE"]
            fuel_type_aggregation += temp_val
          end
        end
        if fuel_type_aggregation
          prefix_str = OpenStudio::toUnderscoreCase("total_#{fuel_str}_end_use")
          runner.registerValue(prefix_str, fuel_type_aggregation, "#{runner_units_agg}")
          short_name = "#{short_os_fuel(fuel_str)} Total"
          metadata[metadata.length] = [prefix_str, "Total #{fuel_str} End Use", short_name, "Total #{fuel_str.downcase} End Use", metadata_units_agg, "double", "FALSE"]
          site_energy_use += fuel_type_aggregation if fuel_str != "Water"
        end
      end

      runner.registerValue("site_energy_use", site_energy_use, "GJ")
      metadata[metadata.length] = ["site_energy_use", "Total Site Energy Use", "Site Energy", "Total energy consumption for the site", "gigajoule", "double", "FALSE"]

      #get monthly fuel aggregates
      #todo: get all monthly fuel type outputs, including non-present fuel types, mapping to 0
      OpenStudio::EndUseFuelType.getValues.each do |fuel_type|
        fuel_str = OpenStudio::EndUseFuelType.new(fuel_type).valueDescription
        mult_factor = 10**-6 / building_area
        runner_units = "MJ/m2"
        metadata_units = "megajoules_per_square_meter"
        if fuel_str == "Water"
          next
        end
        OpenStudio::MonthOfYear.getValues.each do |month|
          if month >= 1 and month <= 12
            fuel_and_month_aggregation = 0.0
            OpenStudio::EndUseCategoryType::getValues.each do |category_type|
              if sql_file.energyConsumptionByMonth(OpenStudio::EndUseFuelType.new(fuel_str), OpenStudio::EndUseCategoryType.new(category_type), OpenStudio::MonthOfYear.new(month)).is_initialized
                val_in_j = sql_file.energyConsumptionByMonth(OpenStudio::EndUseFuelType.new(fuel_str), OpenStudio::EndUseCategoryType.new(category_type), OpenStudio::MonthOfYear.new(month)).get
                fuel_and_month_aggregation += val_in_j
              end
            end
            fuel_and_month_aggregation *= mult_factor
            month_str = OpenStudio::MonthOfYear.new(month).valueDescription
            prefix_str = OpenStudio::toUnderscoreCase("#{month_str}_end_use_#{fuel_str}_eui")
            runner.registerValue("#{prefix_str}", fuel_and_month_aggregation, "#{runner_units}")
            short_name = "#{month_str[0..2]} #{short_os_fuel(fuel_str)} EUI"
            metadata[metadata.length] = ["#{prefix_str}", "#{month_str} #{fuel_str} EUI", short_name, "Total #{fuel_str.downcase} end use energy use per square meter in #{month_str}", metadata_units, "double", "FALSE"]
          end
        end
      end

      # queries that don't have API methods yet

      life_cycle_cost = sql_query(runner, sql_file, "Life-Cycle Cost Report", "TableName='Present Value by Category' AND RowName='Grand Total' AND ColumnName='Present Value'")
      runner.registerValue("life_cycle_cost", life_cycle_cost, "dollars")
      metadata[metadata.length] = ["life_cycle_cost", "Total Life Cycle Cost", "Life Cycle Cost", "Total calculated life cycle cost", "us_dollar", "double", "FALSE"]

      conditioned_area = sql_query(runner, sql_file, "AnnualBuildingUtilityPerformanceSummary", "TableName='Building Area' AND RowName='Net Conditioned Building Area' AND ColumnName='Area'")
      runner.registerValue("conditioned_area", conditioned_area, "m2")
      metadata[metadata.length] = ["conditioned_area", "Total Conditioned Area", "Cond Bldg Area", "Total conditioned building area as calculated by Energy Plus", "square_meter", "double", "FALSE"]

      unconditioned_area = sql_query(runner, sql_file, "AnnualBuildingUtilityPerformanceSummary", "TableName='Building Area' AND RowName='Unconditioned Building Area' AND ColumnName='Area'")
      runner.registerValue("unconditioned_area", unconditioned_area, "m2")
      metadata[metadata.length] = ["unconditioned_area", "Total Unconditioned Area", "Uncond Bldg Area", "Total unconditioned building area as calculated by EnergyPlus", "square_meter", "double", "FALSE"]

      total_site_eui = sql_query(runner, sql_file, "AnnualBuildingUtilityPerformanceSummary", "TableName='Site and Source Energy' AND RowName='Total Site Energy' AND ColumnName='Energy Per Conditioned Building Area'")
      runner.registerValue("total_site_eui", total_site_eui, "MJ/m2")
      metadata[metadata.length] = ["total_site_eui", "Total Site Energy Use Intensity", "Site EUI", "Total site energy use intensity per conditioned building area as calculated by EnergyPlus", "megajoules_per_square_meter", "double", "FALSE"]

      total_source_eui = sql_query(runner, sql_file, "AnnualBuildingUtilityPerformanceSummary", "TableName='Site and Source Energy' AND RowName='Total Source Energy' AND ColumnName='Energy Per Conditioned Building Area'")
      runner.registerValue("total_source_eui", total_source_eui, "MJ/m2")
      metadata[metadata.length] = ["total_source_eui", "Total Source Energy Use Intensity", "Source EUI", "Total source energy use intensity per conditioned building area as calculated by EnergyPlus", "megajoules_per_square_meter", "double", "FALSE"]

      time_setpoint_not_met_during_occupied_heating = sql_query(runner, sql_file, "AnnualBuildingUtilityPerformanceSummary", "TableName='Comfort and Setpoint Not Met Summary' AND RowName='Time Setpoint Not Met During Occupied Heating' AND ColumnName='Facility'")
      runner.registerValue("time_setpoint_not_met_during_occupied_heating", time_setpoint_not_met_during_occupied_heating, "hr")
      metadata[metadata.length] = ["time_setpoint_not_met_during_occupied_heating", "Occupied Time During Which Heating Setpoint Not Met", "Setpoint Missed Heat", "Hours during which the building was occupied but the heating setpoint temperature was not met", "hour", "double", "FALSE"]

      time_setpoint_not_met_during_occupied_cooling = sql_query(runner, sql_file, "AnnualBuildingUtilityPerformanceSummary", "TableName='Comfort and Setpoint Not Met Summary' AND RowName='Time Setpoint Not Met During Occupied Cooling' AND ColumnName='Facility'")
      runner.registerValue("time_setpoint_not_met_during_occupied_cooling", time_setpoint_not_met_during_occupied_cooling, "hr")
      metadata[metadata.length] = ["time_setpoint_not_met_during_occupied_cooling", "Occupied Time During Which Cooling Setpoint Not Met", "Setpoint Missed Cool", "Hours during which the building was occupied but the cooling setpoint temperature was not met", "hour", "double", "FALSE"]

      time_setpoint_not_met_during_occupied_hours = time_setpoint_not_met_during_occupied_heating + time_setpoint_not_met_during_occupied_cooling
      runner.registerValue("time_setpoint_not_met_during_occupied_hours", time_setpoint_not_met_during_occupied_hours, "hr")
      metadata[metadata.length] = ["time_setpoint_not_met_during_occupied_hours", "Occupied Time During Which Temperature Setpoint Not Met", "Setpoint Missed Total", "Hours during which the building was occupied but the setpoint temperatures were not met", "hour", "double", "FALSE"]

      window_to_wall_ratio_north = sql_query(runner, sql_file, "InputVerificationandResultsSummary", "TableName='Window-Wall Ratio' AND RowName='Gross Window-Wall Ratio' AND ColumnName='North (315 to 45 deg)'")
      runner.registerValue("window_to_wall_ratio_north", window_to_wall_ratio_north, "%")
      metadata[metadata.length] = ["window_to_wall_ratio_north", "North Window to Wall Ratio", "WWR North", "Window to wall ratio of wall objects facing between 315 and 45 degrees", "percent", "double", "FALSE"]

      window_to_wall_ratio_south = sql_query(runner, sql_file, "InputVerificationandResultsSummary", "TableName='Window-Wall Ratio' AND RowName='Gross Window-Wall Ratio' AND ColumnName='South (135 to 225 deg)'")
      runner.registerValue("window_to_wall_ratio_south", window_to_wall_ratio_south, "%")
      metadata[metadata.length] = ["window_to_wall_ratio_south", "South Window to Wall Ratio", "WWR South", "Window to wall ratio of wall objects facing between 135 and 225 degrees", "percent", "double", "FALSE"]

      window_to_wall_ratio_east = sql_query(runner, sql_file, "InputVerificationandResultsSummary", "TableName='Window-Wall Ratio' AND RowName='Gross Window-Wall Ratio' AND ColumnName='East (45 to 135 deg)'")
      runner.registerValue("window_to_wall_ratio_east", window_to_wall_ratio_east, "%")
      metadata[metadata.length] = ["window_to_wall_ratio_east", "East Window to Wall Ratio", "WWR East", "Window to wall ratio of wall objects facing between 45 and 135 degrees", "percent", "double", "FALSE"]

      window_to_wall_ratio_west = sql_query(runner, sql_file, "InputVerificationandResultsSummary", "TableName='Window-Wall Ratio' AND RowName='Gross Window-Wall Ratio' AND ColumnName='West (225 to 315 deg)'")
      runner.registerValue("window_to_wall_ratio_west", window_to_wall_ratio_west, "%")
      metadata[metadata.length] = ["window_to_wall_ratio_west", "West Window to Wall Ratio", "WWR West", "Window to wall ratio of wall objects facing between 225 and 315 degrees", "percent", "double", "FALSE"]

      lat = sql_query(runner, sql_file, "InputVerificationandResultsSummary", "TableName='General' AND RowName='Latitude' AND ColumnName='Value'")
      runner.registerValue("latitude", lat, "deg")
      metadata[metadata.length] = ["latitude", "Latitude", "Lat", "Building latitude based on weather file", "degrees_angular", "double", "FALSE"]

      long = sql_query(runner, sql_file, "InputVerificationandResultsSummary", "TableName='General' AND RowName='Longitude' AND ColumnName='Value'")
      runner.registerValue("longitude", long, "deg")
      metadata[metadata.length] = ["longitude", "Longitude", "Long", "Building longitude based on weather file", "degrees_angular", "double", "FALSE"]

      weather_file = sql_query(runner, sql_file, "InputVerificationandResultsSummary", "TableName='General' AND RowName='Weather File' AND ColumnName='Value'")
      runner.registerValue("weather_file", weather_file, "deg")
      metadata[metadata.length] = ["weather_file", "Weather File", "Weather File", "Name of weather file", "none", "string", "FALSE"]

      # queries with one-line API methods
      building_rotation = building.northAxis
      runner.registerValue("orientation", building_rotation, "deg")
      metadata[metadata.length] = ["orientation", "Building Orientation", "Orientation", "Degrees of building north axis off of true north", "degrees_angular", "double", "FALSE"]

      #floor_to_floor_height = building.nominalFloortoFloorHeight.get
      #runner.registerValue("floor_to_floor_height", floor_to_floor_height, "m")
      #metadata[metadata.length] = ["floor_to_floor_height", "Floor to Floor Height", "Flr-to-Flr Height", "Nominal floor to floor height of building", "meter", "double", "FALSE"]

      #total_building_volume = building_area * floor_to_floor_height
      #runner.registerValue("total_building_volume", total_building_volume, "m3")
      #metadata[metadata.length] = ["total_building_volume", "Total Building Volume", "Volume", "Building volume calculated by multiplying floor to floor height and footprint", "cubic_meter", "double", "FALSE"]

      total_occupancy = building.numberOfPeople
      runner.registerValue("total_occupancy", total_occupancy, "people")
      metadata[metadata.length] = ["total_occupancy", "Total Building Occupancy", "Bldg Occ", "Number of people in the building as calculated by EnergyPlus", "none", "double", "FALSE"]

      occupancy_density = building.peoplePerFloorArea
      runner.registerValue("occupant_density", occupancy_density, "people/m2")
      metadata[metadata.length] = ["occupant_density", "Building Occupancy Density", "Occ Density", "Number of people per floor area as calculated by EnergyPlus", "none", "double", "FALSE"]

      lighting_power = building.lightingPower
      runner.registerValue("lighting_power", lighting_power, "W")
      metadata[metadata.length] = ["lighting_power", "Lighting Power", "Lighting Pwr", "Total lighting power", "watt", "double", "FALSE"]

      lighting_power_density = building.lightingPowerPerFloorArea
      runner.registerValue("lighting_power_density", lighting_power_density, "W/m2")
      metadata[metadata.length] = ["lighting_power_density", "Lighting Power Density", "LPD", "Total lighting power density", "watts_per_square_meter", "double", "FALSE"]

      infiltration_rate = building.infiltrationDesignAirChangesPerHour
      runner.registerValue("infiltration_rate", infiltration_rate, "ACH")
      metadata[metadata.length] = ["infiltration_rate", "Infiltration Rate", "Infil Rate", "Infiltration rate of air into the building", "none", "double", "FALSE"]

      number_of_floors = building.standardsNumberOfStories.get if building.standardsNumberOfStories.is_initialized
      number_of_floors ||= "NIL"
      runner.registerValue("number_of_floors", number_of_floors, "")
      metadata[metadata.length] = ["number_of_floors", "Number of Floors", "Number of Floors", "Total number of storeys in the building", "none", "double", "FALSE"]

      building_type = building.standardsBuildingType if building.standardsBuildingType.is_initialized
      building_type ||= "NIL"
      runner.registerValue("building_type", building_type, "")
      metadata[metadata.length] = ["building_type", "Building Type", "Bldg Type", "Building type as defined by the modeler", "none", "string", "FALSE"]

      #get exterior wall, exterior roof, and ground plate areas
      exterior_wall_area = 0.0
      exterior_roof_area = 0.0
      ground_contact_area = 0.0
      surfaces = model.getSurfaces
      surfaces.each do |surface|
        if surface.outsideBoundaryCondition == "Outdoors" and surface.surfaceType == "Wall"
          exterior_wall_area += surface.netArea
        end
        if surface.outsideBoundaryCondition == "Outdoors" and surface.surfaceType == "RoofCeiling"
          exterior_roof_area += surface.netArea
        end
        if surface.outsideBoundaryCondition == "Ground" and surface.surfaceType == "Floor"
          ground_contact_area += surface.netArea
        end
      end

      runner.registerValue("exterior_wall_area", exterior_wall_area, "m2")
      metadata[metadata.length] = ["exterior_wall_area", "Exterior Wall Area", "Ext Wall Area", "Total area of all surfaces with the conditions of 'Outdoors' and 'Wall'", "square_meter", "double", "FALSE"]

      runner.registerValue("exterior_roof_area", exterior_roof_area, "m2")
      metadata[metadata.length] = ["exterior_roof_area", "Exterior Roof Area", "Ext Roof Area", "Total area of all surfaces with the conditions of 'Outdoors' and 'Roof'", "square_meter", "double", "FALSE"]

      runner.registerValue("ground_contact_area", ground_contact_area, "m2")
      metadata[metadata.length] = ["ground_contact_area", "Ground Contact Area", "Gnd Area", "Total area of all surfaces with the conditions of 'Ground' and 'Floor'", "square_meter", "double", "FALSE"]

      #get exterior fenestration area
      exterior_fenestration_area = 0.0
      subsurfaces = model.getSubSurfaces
      subsurfaces.each do |subsurface|
        if subsurface.outsideBoundaryCondition == "Outdoors"
          if subsurface.subSurfaceType == "FixedWindow" or subsurface.subSurfaceType == "OperableWindow"
            exterior_fenestration_area += subsurface.netArea
          end
        end
      end

      runner.registerValue("exterior_fenestration_area", exterior_fenestration_area, "m2")
      metadata[metadata.length] = ["exterior_fenestration_area", "Exterior Fenestration Area", "Window Area Total", "Total area of all surfaces with the conditions of 'Outdoors' and 'FixedWindow' or 'OperableWindow'", "square_meter", "double", "FALSE"]

      #get density of economizers in airloops
      num_airloops = 0
      num_economizers = 0
      model.getAirLoopHVACs.each do |air_loop|
        num_airloops += 1
        if air_loop.airLoopHVACOutdoorAirSystem.is_initialized
          air_loop_oa = air_loop.airLoopHVACOutdoorAirSystem.get
          air_loop_oa_controller = air_loop_oa.getControllerOutdoorAir
          if air_loop_oa_controller.getEconomizerControlType != "NoEconomizer"
            num_economizers += 1
          end
        end
      end
      economizer_density = num_economizers / num_airloops if num_airloops != 0
      economizer_density ||= "NIL"

      runner.registerValue("economizer_density", economizer_density, "")
      metadata[metadata.length] = ["economizer_density", "Economizer Density", "Econ Density", "Proportion of air loops with economizers to air loops without", "percent", "double", "FALSE"]

      #get aspect ratios
      north_wall_area = sql_query(runner, sql_file, "InputVerificationandResultsSummary", "TableName='Window-Wall Ratio' AND RowName='Gross Wall Area' AND ColumnName='North (315 to 45 deg)'")
      east_wall_area = sql_query(runner, sql_file, "InputVerificationandResultsSummary", "TableName='Window-Wall Ratio' AND RowName='Gross Wall Area' AND ColumnName='East (45 to 135 deg)'")
      aspect_ratio = north_wall_area / east_wall_area if north_wall_area != 0 && east_wall_area != 0
      aspect_ratio ||= "NIL"

      runner.registerValue("aspect_ratio", aspect_ratio, "")
      metadata[metadata.length] = ["aspect_ratio", "Aspect Ratio", "Aspect Ratio", "Proportion of north wall area to east wall area", "percent", "double", "FALSE"]

      #write metadata CSV
      runner.registerInfo("Saving Dencity metadata csv file")
      CSV.open("report_metadata.csv", "wb") do |csv|
        metadata.each do |elem|
          csv << elem
        end
      end
      runner.registerInfo("Saved Dencity metadata as report_metadata.csv")

      #get meter timeseries data and output as a msgpack or csv or both
      #todo: find a way for the sql call to not rely on RUN PERIOD 1
      timeseries_start = Time.now.to_i
      available_meters = sql_file.execAndReturnVectorOfString("SELECT VariableName FROM ReportMeterDataDictionary WHERE VariableType='Sum' AND ReportingFrequency='Hourly'")
      get_timeseries_flag = TRUE
      if available_meters.empty?
        runner.registerWarning("No meters found with Hourly reporting frequency to extract timeseries data from")
      else
        begin
          meter_strings = available_meters.get
          runner.registerInfo("The following meters were found: #{meter_strings}")
        rescue
          runner.registerWarning("Unable to retrieve timeseries strings")
          get_timeseries_flag = FALSE
        end
        meter_units = sql_file.execAndReturnVectorOfString("SELECT VariableUnits FROM ReportMeterDataDictionary WHERE VariableType='Sum' AND ReportingFrequency='Hourly'")
        begin
          meter_units = meter_units.get
          runner.registerInfo("Units were found for all available meters")
        rescue
          runner.registerWarning("Unable to retrieve timeseries unit strings")
          get_timeseries_flag = FALSE
        end
        runner.registerInfo("The following meter units were found: #{meter_units}")
        runner.registerError("Timeseries variable names and units of differing lengths. Exiting Dencity Reports.") if meter_units.size != meter_strings.size
        get_timeseries_flag = FALSE if meter_units.size != meter_strings.size
      end

      runner.registerInfo("get_timeseries_flag is set to #{get_timeseries_flag}")
      if get_timeseries_flag
        runner.registerInfo("Retrieving timeseries data")
        if msgpack_flag and csv_flag
          require "parallel"
          require "msgpack"
          msgpack_array = []
          csv_array = []
          mark0 = Time.now.to_i
          Parallel.each_with_index(meter_strings, :in_threads => 4) do |meter_string, meter_index|
            if sql_file.timeSeries("RUN PERIOD 1", "Hourly", meter_string, "").is_initialized
              sql_ts = sql_file.timeSeries("RUN PERIOD 1", "Hourly", meter_string, "").get
              ts_values = sql_ts.values
              ts_times = sql_ts.dateTimes
              timeseries_out = {}
              initial_epoch_time = Time.parse(ts_times[0].toString).to_i*1000
              timestep_in_epoch = (Time.parse(ts_times[1].toString).to_i - Time.parse(ts_times[0].toString).to_i)*1000
              timeseries_out[initial_epoch_time] = ts_values[0]
              next_epoch_time = initial_epoch_time
              for i in 1..ts_times.size - 1
                next_epoch_time += timestep_in_epoch
                timeseries_out[next_epoch_time] = ts_values[i]
              end
              csv_array << (["epoch_time"] + timeseries_out.to_a.transpose[0]) if meter_index == 0
              csv_array << ([meter_string.gsub(":", "_") + " [" + meter_units[meter_index] + "]"] + timeseries_out.to_a.transpose[1])
              meter_hash = {timeseries: {}}
              meter_hash[:timeseries][:fuel] = meter_string.gsub(":", "_")
              meter_hash[:timeseries][:interval] = Time.parse(ts_times[1].toString).to_i - Time.parse(ts_times[0].toString).to_i
              meter_hash[:timeseries][:interval_units] = "seconds"
              meter_hash[:timeseries][:data] = timeseries_out
              meter_hash[:timeseries][:units] = meter_units[meter_index]
              msgpack_array << meter_hash
            else
              runner.registerWarning("Timeseries #{meter_string} was empty.")
            end
          end
          mark1 = Time.now.to_i
          runner.registerInfo("DeltaMake=#{mark1-mark0}s")
          File.open("report_timeseries.msgpack", "w") do |file|
            file << {data: msgpack_array}.to_msgpack
          end
          runner.registerInfo("Saved timeseries data as report_timeseries.msgpack")
          csv_array = csv_array.transpose
          CSV.open("report_timeseries.csv", "w") do |csv|
            csv_array.each do |elem|
              csv << elem
            end
          end
          runner.registerInfo("Saved timeseries data as report_timeseries.csv")
          mark2 = Time.now.to_i
          runner.registerInfo("DeltaWrite=#{mark2-mark1}s")

        elsif msgpack_flag
          require "parallel"
          require "msgpack"
          msgpack_array = []
          mark0 = Time.now.to_i
          meter_strings.each_with_index(meter_strings, :in_threads => 4) do |meter_string, meter_index|
            if sql_file.timeSeries("RUN PERIOD 1", "Hourly", meter_string, "").is_initialized
              sql_ts = sql_file.timeSeries("RUN PERIOD 1", "Hourly", meter_string, "").get
              ts_values = sql_ts.values
              ts_times = sql_ts.dateTimes
              timeseries_out = {}
              initial_epoch_time = Time.parse(ts_times[0].toString).to_i*1000
              timestep_in_epoch = (Time.parse(ts_times[1].toString).to_i - Time.parse(ts_times[0].toString).to_i)*1000
              timeseries_out[initial_epoch_time] = ts_values[0]
              next_epoch_time = initial_epoch_time
              for i in 1..ts_times.size - 1
                next_epoch_time += timestep_in_epoch
                timeseries_out[next_epoch_time] = ts_values[i]
              end
              meter_hash = {timeseries: {}}
              meter_hash[:timeseries][:fuel] = meter_string.gsub(":", "_")
              meter_hash[:timeseries][:interval] = Time.parse(ts_times[1].toString).to_i - Time.parse(ts_times[0].toString).to_i
              meter_hash[:timeseries][:interval_units] = "seconds"
              meter_hash[:timeseries][:data] = timeseries_out
              meter_hash[:timeseries][:units] = meter_units[meter_index]
              msgpack_array << meter_hash
            else
              runner.registerWarning("Timeseries #{meter_string} was empty.")
            end
          end
          mark1 = Time.now.to_i
          runner.registerInfo("DeltaMake=#{mark1-mark0}s")
          File.open("report_timeseries.msgpack", "w") do |file|
            file << {data: msgpack_array}.to_msgpack
            runner.registerInfo("Saved timeseries data as report_timeseries.msgpack")
          end
          mark2 = Time.now.to_i
          runner.registerInfo("DeltaWrite=#{mark2-mark1}s")

        elsif csv_flag
          require "parallel"
          csv_array = []
          mark0 = Time.now.to_i
          Parallel.each_with_index(meter_strings, :in_threads => 4) do |meter_string, meter_index|
            if sql_file.timeSeries("RUN PERIOD 1", "Hourly", meter_string, "").is_initialized
              sql_ts = sql_file.timeSeries("RUN PERIOD 1", "Hourly", meter_string, "").get
              ts_values = sql_ts.values
              ts_times = sql_ts.dateTimes
              timeseries_out = {}
              initial_epoch_time = Time.parse(ts_times[0].toString).to_i*1000
              timestep_in_epoch = (Time.parse(ts_times[1].toString).to_i - Time.parse(ts_times[0].toString).to_i)*1000
              timeseries_out[initial_epoch_time] = ts_values[0]
              next_epoch_time = initial_epoch_time
              for i in 1..ts_times.size - 1
                next_epoch_time += timestep_in_epoch
                timeseries_out[next_epoch_time] = ts_values[i]
              end
              csv_array << (["epoch_time"] + timeseries_out.to_a.transpose[0]) if meter_index == 0
              csv_array << ([meter_string.gsub(":", "_") + " [" + meter_units[meter_index] + "]"] + timeseries_out.to_a.transpose[1])
            else
              runner.registerWarning("Timeseries #{meter_string} was empty.")
            end
          end
          mark1 = Time.now.to_i
          runner.registerInfo("DeltaMake=#{mark1-mark0}s")
          csv_array = csv_array.transpose
          CSV.open("report_timeseries.csv", "w") do |csv|
            csv_array.each do |elem|
              csv << elem
            end
          end
          runner.registerInfo("Saved timeseries data as report_timeseries.csv")
          mark2 = Time.now.to_i
          runner.registerInfo("DeltaWrite=#{mark2-mark1}s")
        end
      end
      timeseries_end = Time.now.to_i
      runner.registerInfo("Total Timeseries Time: #{timeseries_end-timeseries_start}")

      #closing the sql file
      sql_file.close

      #reporting final condition
      runner.registerFinalCondition("DEnCity Report generated successfully.")

    rescue => e
      fail "Measure failed with #{e.message}:#{e.backtrace}"
    ensure

      # profile_results = RubyProf.stop
      # FileUtils.mkdir_p 'results'
      # File.open("results/profile-graph.html", 'w') { |f| RubyProf::GraphHtmlPrinter.new(profile_results).print(f) }
      # File.open("results/profile-flat.txt", 'w') { |f| RubyProf::FlatPrinter.new(profile_results).print(f) }
      # File.open("results/profile-tree.prof", 'w') { |f| RubyProf::CallTreePrinter.new(profile_results).print(f) }


    end

    true

  end #end the run method

end #end the measure

#this allows the measure to be use by the application
DencityReports.new.registerWithApplication