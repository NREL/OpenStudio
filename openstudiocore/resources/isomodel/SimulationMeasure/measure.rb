
#see the URL below for information on how to write OpenStudio measures
# http://nrel.github.io/OpenStudio-user-documentation/measures/measure_writing_guide/

#see the URL below for access to C++ documentation on model objects
# https://s3.amazonaws.com/openstudio-sdk-documentation/cpp/OpenStudio-1.5.0-doc/model/html/classes.html


######################################################################
# == Synopsis 
#
#   This measure will load the current OpenStudio Model, convert to an ISM model, save the model to disk
# then load the model into an ISOmodel structure, run the ISOmodel code and save output to a CSV file

Default_Weather_File_Path = "c:\\epw\\ord.epw"	# define the default weather file path

#start the measure
class OSM2ISMV01 < OpenStudio::Ruleset::ModelUserScript
  
  #define the name that a user will see, this method may be deprecated as
  #the display name in PAT comes from the name field in measure.xml
  def name
    return "OSM2ISMV01"
  end
  
  #define the arguments that the user will input
  def arguments(model)
    args = OpenStudio::Ruleset::OSArgumentVector.new
    
	weather_file_path = OpenStudio::Ruleset::OSArgument::makeStringArgument("weather_file_path",true)
	weather_file_path.setDisplayName("Path to weather file")
	weather_file_path.setDefaultValue(Default_Weather_File_Path)
	args << weather_file_path
	



	# have the user select HVAC type from a list
	hvac_options= OpenStudio::StringVector.new
	hvac_options << "1 - No Cooling, Water heat, Individual Ctrl"
	hvac_options << "2 - No Cooling, Water heat, No Individual Ctrl"	
	hvac_options << "3 - No Cooling, Air heat, Individual Ctrl"	
	hvac_options << "4 - No Cooling, Air heat, No Individual Ctrl"
	hvac_options << "5 - Single Duct, Water heat, Water cool, Individual Ctrl"
	hvac_options << "6 - Single Duct, Water heat, air cool, Individual Ctrl"
	hvac_options << "7 - Single Duct, Water heat, Water cool, No Individual Ctrl"
	hvac_options << "8 - Single Duct, Air heat, Air cool, Individual Ctrl"
	hvac_options << "9 - Single Duct, Air heat, air cool, No Individual Ctrl"
	hvac_options << "10 - Dual Duct, Water heat, Water cool,Individual Ctrl"	
	hvac_options << "11 - Dual Duct, Water heat, Air cool, Individual Ctrl"	
	hvac_options << "12 - Dual Duct, Water heat, Air cool, No Individual Ctrl"
	hvac_options << "13 - Dual Duct, Air heat, Water cool, Individual Ctrl"
	hvac_options << "14 - Single Duct, Terminal reheat, Water heat, Water cool, Individual Ctrl"
	hvac_options << "15 - Single Duct, Terminal reheat, Water heat, Air cool, Individual Ctrl"
	hvac_options << "16 - Single Duct, Terminal reheat, Water heat, Air cool, No Individual Ctrl"
	hvac_options << "17 - CAV, Water heat, Water cool, Individual Ctrl"
	hvac_options << "18 - CAV, Water heat, Air cool, Individual Ctrl"
	hvac_options << "19 - CAV, Water heat, Air cool, NoIndividual Ctrl"
	hvac_options << "20 - CAV, Air heat, Water cool, Individual Ctrl"
	hvac_options << "21 - CAV, Air heat, Water cool, No Individual Ctrl"
	hvac_options << "22 - VAV, Water or Water+Air heat, Water cool, Individual Ctrl"
	hvac_options << "23 - VAV, Water or Water+Air heat, Air cool, Individual Ctrl"
	hvac_options << "24 - VAV, Water or Water+Air heat, Air cool, No Individual Ctrl"
	hvac_options << "25 - Fan Coil, 2 pipe"
	hvac_options << "29 - Induction System, 2-pipe changeover"
	hvac_options << "30 - Induction System, 3-pipe"
	hvac_options << "31 - Induction System, 4-pipe"
	hvac_options << "32 - 2 pipe radiant cooling panels (active and passive)"
	hvac_options << "33 - 4 pipe radiant cooling panels (active and passive)"
	hvac_options << "34 - Embedded cooling system in floors, walls, or ceilings"
	hvac_options << "35 - In Room units (including single duct)"
	hvac_options << "36 - DX Single Split"
	hvac_type_string = OpenStudio::Ruleset::OSArgument::makeChoiceArgument('hvac_type_string', hvac_options, true)
	hvac_type_string.setDisplayName("Select HVAC Type")
	hvac_type_string.setDefaultValue("24 - VAV, Water or Water+Air heat, Air cool, No Individual Ctrl")
	args << hvac_type_string  
	
	cooling_IPLVToCop_ratio = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("cooling_IPLVToCop_ratio", true)
	cooling_IPLVToCop_ratio.setDefaultValue(1.0)
	cooling_IPLVToCop_ratio.setDisplayName("Set IPLV/COP Partial Load Multiplier") 	# set default system partial load multiplier
	args << cooling_IPLVToCop_ratio

	exhaust_recirculation_fraction = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("exhaust_recirculation_fraction", true)
	exhaust_recirculation_fraction.setDefaultValue(0.0)
	exhaust_recirculation_fraction.setDisplayName("Exhaust Recirculation Fraction (0.0 - 1.0)")  # set fraction of air recirculated  to 0.0 (i.e. total outside air)
	args << exhaust_recirculation_fraction	
	

	# use SFP = 1.0/0.7733=1.29 as default as that is what OpenStudio uses as it's default
	specific_fan_power = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("specific_fan_power", true)
	specific_fan_power.setDisplayName("Specific Fan Power (L/s/W)")
	specific_fan_power.setDefaultValue(1.29)
	args << specific_fan_power
		
	heat_recovery_options = OpenStudio::StringVector.new
	heat_recovery_options << "0.0 - No Heat Recovery"
	heat_recovery_options << "0.40 - Loading Cold with Air Conditioning"
	heat_recovery_options << "0.60 - Heat Pipes or Two-elements System "	
	heat_recovery_options << "0.65 - Heat Exchange Plates or Pipes"
	heat_recovery_options << "0.70 - Enthalpy Wheel or Intermittent Heat Exchangers"	
	heat_recovery_string = OpenStudio::Ruleset::OSArgument::makeChoiceArgument("heat_recovery_string",heat_recovery_options, true)
	heat_recovery_string.setDefaultValue("0.0 - No Heat Recovery")
	heat_recovery_string.setDisplayName("Heat Recovery Options")
	args << heat_recovery_string

	# have the user input the bas/bem type
	bem_options= OpenStudio::StringVector.new
	bem_options << "1 - No BEM/BAS"
	bem_options << "2 - Simple BEM/BAS"
	bem_options << "3 - Advanced BEM/BAS with FDD"
	bem_type_string = OpenStudio::Ruleset::OSArgument::makeChoiceArgument("bem_type_string", bem_options, true)
	bem_type_string.setDisplayName("Select BEM Type")
	bem_type_string.setDefaultValue("1 - No BEM/BAS")
	args << bem_type_string 
	
	# have the user input the dhw distribution system type
	dhw_dist_options= OpenStudio::StringVector.new
	dhw_dist_options << "1 - All Taps Within 3m from Generation"
	dhw_dist_options << "2 - Some Taps More than 3m from Generation"
	dhw_dist_options << "3 - Circulation System or Unknown"
	dhw_dist_type_string = OpenStudio::Ruleset::OSArgument::makeChoiceArgument('dhw_dist_type_string', dhw_dist_options, true)
	dhw_dist_type_string.setDisplayName("Select DHW Distribution Type")
	dhw_dist_type_string.setDefaultValue("3 - Circulation System or Unknown")
	args << dhw_dist_type_string 


	fan_control_options = OpenStudio::StringVector.new
	fan_control_options << "1.0 - No Controls"
	fan_control_options << "0.5 - Variable Speed Fan"
	fan_control_options << "0.65 - Adjustable Inlet Blades"
	fan_control_string = OpenStudio::Ruleset::OSArgument::makeChoiceArgument('fan_control_string', fan_control_options, true)
	fan_control_string.setDisplayName("Select Fan Control Type")
	fan_control_string.setDefaultValue("1.0 - No Controls")
	args << fan_control_string
	
	heating_pump_control_options = OpenStudio::StringVector.new
	heating_pump_control_options << "1.0 - No Heating Pump Control"
	heating_pump_control_options << "0.5 - Auto Control on > 50% of Heating Pumps"
	heating_pump_control_options << "0.0 - No Pumps on Heating System"
	heating_pump_control_string = OpenStudio::Ruleset::OSArgument::makeChoiceArgument('heating_pump_control_string', heating_pump_control_options, true)
	heating_pump_control_string.setDisplayName("Select Heating Pump Control Type")
	heating_pump_control_string.setDefaultValue("1.0 - No Heating Pump Control")
	args << heating_pump_control_string
	
	cooling_pump_control_options = OpenStudio::StringVector.new
	cooling_pump_control_options << "1.0 - No Cooling Pump Control"
	cooling_pump_control_options << "0.5 - Auto Control on > 50% of Cooling Pumps"
	cooling_pump_control_options << "0.0 - No Pumps on Cooling System"
	cooling_pump_control_string = OpenStudio::Ruleset::OSArgument::makeChoiceArgument('cooling_pump_control_string', cooling_pump_control_options, true)
	cooling_pump_control_string.setDisplayName("Select Cooling Pump Control Type")
	cooling_pump_control_string.setDefaultValue("1.0 - No Cooling Pump Control")
	args << cooling_pump_control_string

	
	# #these are items that aren't modeled in OS so set default values
	# occupancy_sensors = 1 	# occupancy sensors set to off in ISO since they can't be modeled directly in OS 
	occupancy_sensors_options = OpenStudio::StringVector.new
	occupancy_sensors_options << "1 - No or Few Occupancy Sensors"
	occupancy_sensors_options << "2 - Occ. Sensors > 60% of Spaces"
	occupancy_sensors_string= OpenStudio::Ruleset::OSArgument::makeChoiceArgument('occupancy_sensors_string', occupancy_sensors_options, true)
	occupancy_sensors_string.setDisplayName("Lighting Occupancy Sensors")
	occupancy_sensors_string.setDefaultValue("1 - No or Few Occupancy Sensors")
	args << occupancy_sensors_string
	
	# logfile.puts "Occupancy Sensors set 1 because they are not yet supported "
	const_illum_ctrl_options = OpenStudio::StringVector.new
	const_illum_ctrl_options << "1 - No Constant Illumination Control"
	const_illum_ctrl_options << "2 - Const.  Illum.  Control Installed"
	const_illum_ctrl_string= OpenStudio::Ruleset::OSArgument::makeChoiceArgument('const_illum_ctrl_string', const_illum_ctrl_options, true)
	const_illum_ctrl_string.setDisplayName("Constant Illumination Control")
	const_illum_ctrl_string.setDefaultValue("1 - No Constant Illumination Control")
	args << const_illum_ctrl_string
	
	ventilation_type_options = OpenStudio::StringVector.new
	ventilation_type_options << "1 - Mechanical Ventilation"
	ventilation_type_options << "2 - Natural Ventilation"
	ventilation_type_options << "3 - Mechanical + Natural Ventilation"
	ventilation_type_string = OpenStudio::Ruleset::OSArgument::makeChoiceArgument('ventilation_type_string', ventilation_type_options, true)
	ventilation_type_string.setDisplayName("Select Ventilation Type")
	ventilation_type_string.setDefaultValue("1 - Mechanical Ventilation")
	args << ventilation_type_string
	
	scf_options = OpenStudio::StringVector.new
	scf_options << "1.0 - No Solar Control"
	scf_options << "0.7 - 30% Reduction"
	scf_options << "0.5 - 50% Reduction"
	scf_options << "0.3 - 70% Reduction"
	
	scf_string = OpenStudio::Ruleset::OSArgument::makeChoiceArgument("scf_string", scf_options, true)
	scf_string.setDisplayName("External Solar Control Factor (SCF)")
	scf_string.setDefaultValue("1.0 - No Solar Control")
	args << scf_string

	sdf_string = OpenStudio::Ruleset::OSArgument::makeChoiceArgument("sdf_string", scf_options, true)
	sdf_string.setDisplayName("Shading Device Factor (SDF)")
	sdf_string.setDefaultValue("1.0 - No Solar Control")
	args << sdf_string
	
	# ask user if they want to extract occupancy start/end for days and hours
	occupancy_extract_choice = OpenStudio::Ruleset::OSArgument::makeBoolArgument("occupancy_extract_choice",true)
	occupancy_extract_choice.setDisplayName("Get Occupancy start/end info from .osm?")
	occupancy_extract_choice.setDefaultValue(true)
	args << occupancy_extract_choice
	
	# ask user if they want to extract COP and efficiency from OSM model (default yes)
	hvac_extract_choice = OpenStudio::Ruleset::OSArgument::makeBoolArgument("hvac_extract_choice",true)
	hvac_extract_choice.setDisplayName("Get HVAC info from .osm?")
	hvac_extract_choice.setDefaultValue(true)
	args << hvac_extract_choice
	
	# ask user if we want to extract ventilation info from the OSM (default yes)
	ventilation_extract_choice = OpenStudio::Ruleset::OSArgument::makeBoolArgument("ventilation_extract_choice",true)
	ventilation_extract_choice.setDisplayName("Get Ventilation info from .osm?")
	ventilation_extract_choice.setDefaultValue(true)
	args << ventilation_extract_choice

	# ask user if we want to extract infiltration info from the OSM (default yes)
	infiltration_extract_choice = OpenStudio::Ruleset::OSArgument::makeBoolArgument("infiltration_extract_choice",true)
	infiltration_extract_choice.setDisplayName("Get Infiltration info from .osm?")
	infiltration_extract_choice.setDefaultValue(true)
	args << infiltration_extract_choice	
	
	# ask user if they want to extract DHW info from model (default yes)
	dhw_extract_choice = OpenStudio::Ruleset::OSArgument::makeBoolArgument("dhw_extract_choice",true)
	dhw_extract_choice.setDisplayName("Get DHW info from .osm?")
	dhw_extract_choice.setDefaultValue(true)
	args << dhw_extract_choice	
	
	# ask user if we want to extract glazing U and SHGC from OSM (default yes)
	glazing_extract_choice = OpenStudio::Ruleset::OSArgument::makeBoolArgument("glazing_extract_choice",true)
	glazing_extract_choice.setDisplayName("Get Glazing U and SHGC from .osm?")
	glazing_extract_choice.setDefaultValue(true)
	args << glazing_extract_choice

	# ask user if we want to extract wall U , absorption and emissivity (default yes)
	wall_roof_extract_choice = OpenStudio::Ruleset::OSArgument::makeBoolArgument("wall_roof_extract_choice",true)
	wall_roof_extract_choice.setDisplayName("Get Wall/Roof  U, etc. from .osm?")
	wall_roof_extract_choice.setDefaultValue(true)
	args << wall_roof_extract_choice

	# now we put inputs related to the checkboxes above
	
	# have the user select Occupancy Information from lists to reduce error checking on inputs
	occupancy_day_options = OpenStudio::StringVector.new
	occupancy_day_options << "0 - Sunday"
	occupancy_day_options << "1 - Monday"	
	occupancy_day_options << "2 - Tuesday"	
	occupancy_day_options << "3 - Wednesday"
	occupancy_day_options << "4 - Thursday"	
	occupancy_day_options << "5 - Friday"
	occupancy_day_options << "6 - Saturday"
	
	occupancy_day_start_string = OpenStudio::Ruleset::OSArgument::makeChoiceArgument('occupancy_day_start_string', occupancy_day_options, true)
	occupancy_day_start_string.setDisplayName("Starting Day of Primary Occupancy")
	occupancy_day_start_string.setDefaultValue("1 - Monday")
	args << occupancy_day_start_string 
	
	occupancy_day_end_string = OpenStudio::Ruleset::OSArgument::makeChoiceArgument('occupancy_day_end_string', occupancy_day_options, true)
	occupancy_day_end_string.setDisplayName("Ending Day of Primary Occupancy")
	occupancy_day_end_string.setDefaultValue("5 - Friday")
	args << occupancy_day_end_string 	
	
	occupancy_hour_options = OpenStudio::StringVector.new
	(0..23).each do |hour|
		occupancy_hour_options << "#{hour} : 00"
	end
	
	occupancy_hour_start_string = OpenStudio::Ruleset::OSArgument::makeChoiceArgument('occupancy_hour_start_string', occupancy_hour_options, true)
	occupancy_hour_start_string.setDisplayName("Starting Hour of Primary Occupancy")
	occupancy_hour_start_string.setDefaultValue("7 : 00")
	args << occupancy_hour_start_string 
	
	occupancy_hour_end_string = OpenStudio::Ruleset::OSArgument::makeChoiceArgument('occupancy_hour_end_string', occupancy_hour_options, true)
	occupancy_hour_end_string.setDisplayName("Ending Hour of Primary Occupancy")
	occupancy_hour_end_string.setDefaultValue("18 : 00")
	args << occupancy_hour_end_string
	
	
	
	
	
	
	# have the user input the COP, Heating Efficiency, and Heating Fuel Type if they chose not to extract it
	cooling_cop_choice = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("cooling_cop_choice", true)
	cooling_cop_choice.setDisplayName("HVAC: Avg Cooling Sys COP (W/W)")
	cooling_cop_choice.setDefaultValue(3.0)
	args << cooling_cop_choice
	
	heating_eff_choice = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("heating_eff_choice", true)
	heating_eff_choice.setDisplayName("HVAC: Avg Heating Sys Eff (W/W)")
	heating_eff_choice.setDefaultValue(1.0)
	args << heating_eff_choice
	
	fuel_type_options = OpenStudio::StringVector.new
	fuel_type_options << "1 - Electric"
	fuel_type_options << "2 - Natural Gas"

	heating_fuel_choice_string = OpenStudio::Ruleset::OSArgument::makeChoiceArgument("heating_fuel_choice_string", fuel_type_options, true)
	heating_fuel_choice_string.setDisplayName("HVAC: Select Heating Fuel Type")
	heating_fuel_choice_string.setDefaultValue("1 - Electric")
	args << heating_fuel_choice_string
	
	dhw_system_eff_choice = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("dhw_system_eff_choice", true)
	dhw_system_eff_choice.setDisplayName("DHW: DHW System Efficiency (W/W)")
	dhw_system_eff_choice.setDefaultValue(1.0)
	args << dhw_system_eff_choice
	
	dhw_fuel_choice_string = OpenStudio::Ruleset::OSArgument::makeChoiceArgument("dhw_fuel_choice_string", fuel_type_options, true)
	dhw_fuel_choice_string.setDisplayName("DHW: Select DHW Fuel Type")
	dhw_fuel_choice_string.setDefaultValue("1 - Electric")
	args << dhw_fuel_choice_string
	
	# get the number of people from the model to set default DHW use and ventilation rates
	nPeople = model.getBuilding.numberOfPeople
	 
	dhw_demand_choice = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("dhw_demand_choice", true)
	dhw_demand_choice.setDisplayName("DHW: DHW Demand (L/person/day)")
	dhw_demand_choice.setDefaultValue(3.8) # default to 3.8L/person/day per ASHRAE HVAC apps chapter 50 for offices
	args << dhw_demand_choice
	
	
	freshair_flow_rate_choice = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("freshair_flow_rate_choice", true)
	freshair_flow_rate_choice.setDisplayName("Vent: Fresh Air Supply Rate (L/s/person)")
	freshair_flow_rate_choice.setDefaultValue(10)  # default to 10 L/s/person which is about 20 cfm/person for ventilation
	args << freshair_flow_rate_choice

	infiltration_rate_choice=OpenStudio::Ruleset::OSArgument::makeDoubleArgument("infiltration_rate_choice", true)
	infiltration_rate_choice.setDisplayName("Infil: Envelope Infilt. (m3/hr/m2) @ 75 Pa")
	infiltration_rate_choice.setDefaultValue(7)	# use 75 Pa equivalent of EnergyPlus default value
	args << infiltration_rate_choice
	
	# have the user input the window U and SHGC values if they choose not to extract it
	glazing_U_choice = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("glazing_U_choice", true)
	glazing_U_choice.setDisplayName("Glazing: Avg U-Value (W/(m2 K)")
	glazing_U_choice.setDefaultValue(3.0)
	args << glazing_U_choice

	glazing_SHGC_choice = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("glazing_SHGC_choice", true)
	glazing_SHGC_choice.setDisplayName("Glazing: Avg SHGC")
	glazing_SHGC_choice.setDefaultValue(0.7)
	args << glazing_SHGC_choice
	
	wall_U_choice = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("wall_U_choice", true)
	wall_U_choice.setDisplayName("Wall: Avg U-Value (W/(m2 K)")
	wall_U_choice.setDefaultValue(0.3)
	args << wall_U_choice
	
	roof_U_choice = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("roof_U_choice", true)
	roof_U_choice.setDisplayName("Roof: Avg U-Value (W/(m2 K)")
	roof_U_choice.setDefaultValue(0.1)
	args << roof_U_choice	
	
	envelope_solar_abs_choice = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("envelope_solar_abs_choice", true)
	envelope_solar_abs_choice.setDisplayName("Envelope Solar Absorption Coefficients")
	envelope_solar_abs_choice.setDefaultValue(0.7)	
	args << envelope_solar_abs_choice
	
	envelope_thermal_emiss_choice = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("envelope_thermal_emiss_choice", true)
	envelope_thermal_emiss_choice.setDisplayName("Envelope Thermal Emissivity Coefficients")
	envelope_thermal_emiss_choice.setDefaultValue(0.7)
	args << envelope_thermal_emiss_choice
	
    return args
  end #end the arguments method

  #define what happens when the measure is run
  def run(model, runner, user_arguments)
    super(model, runner, user_arguments)
    
    #use the built-in error checking 
    if not runner.validateUserArguments(arguments(model), user_arguments)
      return false
    end
	
	########### assign user inputs to variables using runners
	weather_file_path = runner.getStringArgumentValue("weather_file_path",user_arguments)	
	occupancy_day_start_string = runner.getStringArgumentValue("occupancy_day_start_string",user_arguments)
	occupancy_day_end_string = runner.getStringArgumentValue("occupancy_day_end_string",user_arguments)	
	occupancy_hour_start_string = runner.getStringArgumentValue("occupancy_hour_start_string",user_arguments)	
	occupancy_hour_end_string = runner.getStringArgumentValue("occupancy_hour_end_string",user_arguments)	

	hvac_type_string = runner.getStringArgumentValue("hvac_type_string",user_arguments)
	specific_fan_power = runner.getDoubleArgumentValue("specific_fan_power",user_arguments)
	cooling_IPLVToCop_ratio = runner.getDoubleArgumentValue("cooling_IPLVToCop_ratio",user_arguments)	
	exhaust_recirculation_fraction = runner.getDoubleArgumentValue("exhaust_recirculation_fraction",user_arguments)
	occupancy_sensors_string = runner.getStringArgumentValue("occupancy_sensors_string",user_arguments)	
	fan_flow_control_string = runner.getStringArgumentValue("fan_control_string",user_arguments)	
	heating_pump_control_string = runner.getStringArgumentValue("heating_pump_control_string",user_arguments)
	cooling_pump_control_string = runner.getStringArgumentValue("cooling_pump_control_string",user_arguments)
	dhw_dist_type_string = runner.getStringArgumentValue("dhw_dist_type_string",user_arguments)
	const_illum_ctrl_string = runner.getStringArgumentValue("const_illum_ctrl_string",user_arguments)	
	bem_type_string = runner.getStringArgumentValue("bem_type_string",user_arguments)	
	ventilation_type_string = runner.getStringArgumentValue("ventilation_type_string",user_arguments)
	heat_recovery_string = runner.getStringArgumentValue("heat_recovery_string",user_arguments)
	scf_string = runner.getStringArgumentValue("scf_string",user_arguments)
	sdf_string = runner.getStringArgumentValue("sdf_string",user_arguments)
	occupancy_extract_choice = runner.getBoolArgumentValue("occupancy_extract_choice",user_arguments)
	hvac_extract_choice = runner.getBoolArgumentValue("hvac_extract_choice",user_arguments)
	glazing_extract_choice = runner.getBoolArgumentValue("glazing_extract_choice",user_arguments)
	wall_roof_extract_choice = runner.getBoolArgumentValue("wall_roof_extract_choice",user_arguments)
	dhw_extract_choice = runner.getBoolArgumentValue("dhw_extract_choice",user_arguments)
	infiltration_extract_choice = runner.getBoolArgumentValue("infiltration_extract_choice",user_arguments)
	ventilation_extract_choice = runner.getBoolArgumentValue("ventilation_extract_choice",user_arguments) 
	cooling_cop_choice = runner.getDoubleArgumentValue("cooling_cop_choice",user_arguments)
	heating_eff_choice = runner.getDoubleArgumentValue("heating_eff_choice",user_arguments)
	heating_fuel_choice_string = runner.getStringArgumentValue("heating_fuel_choice_string",user_arguments)
	dhw_system_eff_choice = runner.getDoubleArgumentValue("dhw_system_eff_choice",user_arguments)
	dhw_fuel_choice_string = runner.getStringArgumentValue("dhw_fuel_choice_string",user_arguments)
	dhw_demand_choice = runner.getDoubleArgumentValue("dhw_demand_choice",user_arguments)
	
	glazing_U_choice = runner.getDoubleArgumentValue("glazing_U_choice",user_arguments)
	glazing_SHGC_choice = runner.getDoubleArgumentValue("glazing_SHGC_choice",user_arguments)	
	wall_U_choice = runner.getDoubleArgumentValue("wall_U_choice",user_arguments)	
	envelope_thermal_emiss_choice = runner.getDoubleArgumentValue("envelope_thermal_emiss_choice",user_arguments)
	envelope_solar_abs_choice = runner.getDoubleArgumentValue("envelope_solar_abs_choice",user_arguments)	
	roof_U_choice = runner.getDoubleArgumentValue("roof_U_choice",user_arguments)	

	freshair_flow_rate_choice = runner.getDoubleArgumentValue("freshair_flow_rate_choice",user_arguments)	
	infiltration_rate_choice = runner.getDoubleArgumentValue("infiltration_rate_choice",user_arguments)	
	
	# set default inside and outside air film R values
	outside_air_film_R= 0.04	# set the outside air film coefficient R value in m2*K/W
	inside_air_film_R = 0.13  	# set the inside air film coefficient R value in m2*K/W
	
	########### start the real programming

	
	# set filenames and open log file 
	#set the base name of the file
	input_file="osm2iso"
	
	basename = File.basename(input_file,".*")	# get the base file without the path to it or extensions at the end
	dirname = File.dirname(input_file)	# get the path to the file without the file itself
	osm2iso_log_file = File.join(dirname, basename + ".osm2iso.log")

	# output file is base filename + .ism
	osm2iso_output_file = File.join(dirname, basename + ".ism")
	
	puts "Writing processing log to #{osm2iso_log_file}"

	time=Time.new
	# create output file and start writing out 
	logfile = File.new(osm2iso_log_file, 'w+')
	
	logfile.puts "Starting Log File #{osm2iso_log_file} on #{time.ctime}"

	logfile.puts "Input File: #{input_file}"
	logfile.puts "Output File: #{osm2iso_output_file}"

	puts "...Setting Defaults and Parsing User Inputs"
	logfile.puts "*****************************"
	logfile.puts "       Parsing User Inputs"
	logfile.puts "*****************************"
		
	# check weather file for existance
	if not File.exists?(weather_file_path)   # check if file does NOT exist
		puts " !!!ERROR!!!  Weather file #{weather_file_path} not found - conversion terminated!!!"
		logfile.puts "!!!ERROR!!!  Weather file #{weather_file_path} not found - conversion terminated!!!"
		runner.registerFinalCondition("Weather file #{weather_file_path} does not exist - conversion terminated")
		return false   # use this to stop further processing for now
	end
	
	# process occupancy info from user inputs 
	occupancy_day_start = occupancy_day_start_string.to_i  # convert the string to an integer.  The way the strings are written this will pull off the leading number	
	occupancy_day_end = occupancy_day_end_string.to_i  # convert the string to an integer.  The way the strings are written this will pull off the leading number
	if (occupancy_day_end < occupancy_day_start ) 
		puts "!!!ERROR!!!  You need Occupancy Starting Day < Occupancy Ending Day.  Conversion terminated"
		logfile.puts "!!!ERROR!!!  You need Occupancy Starting Day < Occupancy Ending Day.  Conversion terminated"
		runner.registerError("You need Occupancy Starting Day < Occupancy Ending Day.  Conversion terminated")
		return false
	end

	occupancy_hour_start = occupancy_hour_start_string.to_i  # convert the string to an integer.  The way the strings are written this will pull off the leading number		
	occupancy_hour_end = occupancy_hour_end_string.to_i  # convert the string to an integer.  The way the strings are written this will pull off the leading number
	if (occupancy_hour_end < occupancy_hour_start ) 
		puts "!!!ERROR!!!  You need Occupancy Starting Hour < Occupancy Ending Hour.  Conversion terminated"
		logfile.puts "!!!ERROR!!!  You need Occupancy Starting Hour < Occupancy Ending Hour.  Conversion terminated"
		runner.registerError("You need Occupancy Starting Hour < Occupancy Ending Hour.  Conversion terminated")
		return false
	end	

	
	logfile.puts "Occupancy Day Start set to  #{occupancy_day_start_string}"	
	logfile.puts "Occupancy Day End set to  #{occupancy_day_end_string}"
	logfile.puts "Occupancy Hour End set to  #{occupancy_hour_end_string}"
	logfile.puts "Occupancy Hour Start set to  #{occupancy_hour_start_string}"

	# check specific fan power, mplv, and exhaust recirculation  for reasonable values
	if (specific_fan_power < 0.1 ) or (specific_fan_power > 10)
		puts "Warning!   Specific fan power is usually between 0.1 and 10 L/s/W.  You entered #{specific_fan_power}."
		runner.registerWarning("Specific fan power is usually between 0.1 and 10 L/s/W.  You entered #{specific_fan_power}.")
	end

	if (cooling_IPLVToCop_ratio < 0.2 ) or (cooling_IPLVToCop_ratio > 10)
		puts "Warning!  IPLV/COP is usually between 0.2 and 10 L/s/W.  You entered #{cooling_IPLVToCop_ratio}."
		runner.registerWarning("IPLV/COP is usually between 0.2 and 10 L/s/W.  You entered #{cooling_IPLVToCop_ratio}.")
	end

	if (exhaust_recirculation_fraction < 0.0 ) or (exhaust_recirculation_fraction > 1.0)
		puts "Error!   Exhaust Recirculation Fraction must be between 0.0 and 1.0 .  You entered #{exhaust_recirculation_fraction}."
		puts "Program Terminated!\n"
		runner.registerError("Exhaust Recirculation Fraction must be between 0.0 and 1.0.  You entered #{exhaust_recirculation_fraction}.")
		return false
	end
	logfile.puts "Specific Fan Power set to #{specific_fan_power }"
	logfile.puts "Exhaust recirculation fraction set to #{exhaust_recirculation_fraction}"
	logfile.puts "HVAC IPLV/COP set to #{cooling_IPLVToCop_ratio}"

	# process occupancy sensor info from user inputs
	occupancy_sensors = occupancy_sensors_string.to_i  # convert the string to an integer.  The way the strings are written this will pull off the leading number
	logfile.puts "Occupancy Sensors set to  #{occupancy_sensors_string}"
	
	# get HVAC type from user inputs - no need to do error checks on input
	hvac_type = hvac_type_string.to_i  # convert the string to an integer.  The way the strings are written this will pull off the leading number
	logfile.puts "HVAC Type set to  #{hvac_type_string}"
	
	# process fan, heating pump, and cooling pump control factors
	fan_flow_control_factor = fan_flow_control_string.to_f  # convert the string to a float.  The way the strings are written this will pull off the leading number
	logfile.puts "Fan Flow Control set to  #{fan_flow_control_string}"
	heating_pump_control_factor = heating_pump_control_string.to_f  # convert the string to a float.  The way the strings are written this will pull off the leading number
	logfile.puts "Heating Pump Control set to  #{heating_pump_control_string}"
	cooling_pump_control_factor = heating_pump_control_string.to_f  # convert the string to a float.  The way the strings are written this will pull off the leading number
	logfile.puts "Cooling Pump Control set to  #{cooling_pump_control_string}"
	
	# get DHW distribution type from user input
	dhw_dist_type = dhw_dist_type_string.to_i # convert string to an integer
	logfile.puts "DHW Distribution Type set to  #{dhw_dist_type_string}"
	
	# process constant illumination control, BEM type, ventilation type, and heat recovery from user inputs
	const_illum_ctrl = const_illum_ctrl_string.to_i # convert string to an integer
	bem_type = bem_type_string.to_i # convert string to an integer
	ventilation_type = ventilation_type_string.to_i # convert string to an integer
	heat_recovery_fraction = heat_recovery_string.to_f # convert string to a float
	
	logfile.puts "Constant Illumination Control set to  #{const_illum_ctrl_string}"	
	logfile.puts "BEM Type set to  #{bem_type_string}"
	logfile.puts "Ventilation Type set to  #{ventilation_type_string}"			
	logfile.puts "Heat Recovery Fraction set to  #{heat_recovery_string}"	

	scf = scf_string.to_f # convert string to a float
	window_SCF = Array.new(8,scf)	# set the SCF for all the windows
	sdf = sdf_string.to_f # convert string to a float
	window_SDF = Array.new(8,sdf)	# set the SDF for all the windows
	
	logfile.puts "Solar Control Factor set to  #{scf_string} on all windows"	
	logfile.puts "Solar Control Factor set to  #{sdf_string} on all windows"	
	
	
	# process choices for HVAC and Window Info Extract
	logfile.puts "HVAC Info Extract Choice Set to  #{hvac_extract_choice}"	
	logfile.puts "DHW Info Extract Choice Set to  #{dhw_extract_choice}"
	logfile.puts "Glazing Info Extract Choice Set to  #{glazing_extract_choice}"	
	logfile.puts "Wall/Roof Info Extract Choice Set to  #{glazing_extract_choice}"
	
	dhw_fuel_choice= dhw_fuel_choice_string.to_i # convert string to an integer
	
	
	$stdout.flush  # flush the standard output buffer now
	logfile.flush	# flush the logfile output buffer now
	
	puts "...Parsing Model"
	logfile.puts "*****************************"
	logfile.puts "  Parsing Model"
	logfile.puts "*****************************"

	# get some important objects from the building model
	building=model.getBuilding
	facility=model.getFacility
	surfaces=model.getSurfaces
	logfile.puts "Found #{surfaces.size} surfaces"
	sub_surfaces = model.getSubSurfaces
	logfile.puts "Found #{sub_surfaces.size} sub surfaces"
	spaces = model.getSpaces
	logfile.puts "Found #{spaces.size} spaces"
	thermal_zones = model.getThermalZones
	logfile.puts "Found #{thermal_zones.size} thermal zones"
	space_types = model.getSpaceTypes
	logfile.puts "Found #{space_types.size} space types"

	constructions = model.getConstructions
	logfile.puts "Found #{constructions.size} constructions"

	internal_masses = model.getInternalMassDefinitions
	logfile.puts "Found #{internal_masses.size} Internal Mass Definitions"

	air_loops = model.getAirLoopHVACs
	logfile.puts "Found #{air_loops.size} Air Loops"
	plant_loops = model.getPlantLoops
	logfile.puts "Found #{plant_loops.size} Plant Loops"

	
	# get the terrain class from the site object or assign default if none given
	site = model.getSite
	if not site.terrain.empty?
		terrain = site.terrain
		logfile.puts "Terrain Type #{terrain} in OSM"
	else
		logfile.puts "Site Terrain Not Defined in OpenStudio Model - Set to Urban Terrain By Default"
		terrain = "Urban"
	end

	case terrain
	when "Ocean","Country"   # when we have ocean or country, there are no blocks so terrain_class=1.0
		terrain_class = 1.0
	when "Suburbs"
		terrain_class = 0.9
	else						# when city or urban set to 0.8
		terrain_class = 0.8
	end
	logfile.puts "Terrain class value set to #{terrain_class}"	
	
	number_of_people=building.numberOfPeople	# get the total nominal population
	logfile.printf("Nominal Number of people = %1.2f \n",number_of_people)

	logfile.puts "*****************************"
	logfile.puts "  Analyzing Construction"
	logfile.puts "*****************************"

	const_names = Array.new
	const_solarAbsorptance=Hash.new
	const_thermalAbsorptance=Hash.new
	const_heatCapacity = Hash.new
	const_U = Hash.new
	const_SHGC= Hash.new

	constructions.each do |const|
		name=const.name.to_s
		layers=const.layers
		const_names << name

		# assign 0.0 as default values in case we cannot assign it by searching through the construction
		const_U[name]= 0.0
		const_heatCapacity[name]= 0.0
		const_thermalAbsorptance[name] = 0.0
		const_solarAbsorptance[name] = 0.0
		const_SHGC[name] = 0.0

		# first find the opaque constructions - these could be exterior walls and roofs
		if const.isOpaque
		
			# get the 1st layer of construction (outside layer), convert to opaque material and get the thermal & solar Absorptance
			# store them in an array indexed by the name of the construction

			if not layers[0].to_OpaqueMaterial.empty?
				const_thermalAbsorptance[name]= layers[0].to_OpaqueMaterial.get.thermalAbsorptance
			end
			if not layers[0].to_OpaqueMaterial.empty?
				const_solarAbsorptance[name]=layers[0].to_OpaqueMaterial.get.solarAbsorptance	
			end	
			
			const_R = outside_air_film_R + inside_air_film_R  #set the starting R as outside+inside film coefficients
			const_heat_capacity = 0.0
			layers.each do |layer|
				if not layer.to_StandardOpaqueMaterial.empty?  # do this stuff if the material can convert to StandardOpaqueMaterial
					material = layer.to_StandardOpaqueMaterial.get
					layer_R = material.thermalResistance		# get the R value of the layer
					if not material.heatCapacity.empty?
						layer_heat_capacity = material.heatCapacity	# get the heat capacity directly
					else
						# if heat capacity is not defined, calculated it as thickness*density*specific heat
						layer_heat_capacity = material.specificHeat * material.density * material.thickness
					end
				elsif not layer.to_MasslessOpaqueMaterial.empty? # do this stuff if the material can convert to a MasslessOpaqueMaterial
					material = layer.to_MasslessOpaqueMaterial.get
					layer_R = material.thermalResistance		# get the R value of the layer
					# heat capacity is not defined for this material so calculate it as thickness*density*specific heat
					layer_heat_capacity = material.specificHeat * material.density * material.thickness
				elsif not layer.to_RoofVegetation.empty?
					material = layer.to_RoofVegetation.get
					layer_R = material.thermalResistance
					layer_heat_capacity = material.specificHeat * material.density * material.thickness	
				else
					layer_R = 0.0
					layer_heat_capacity = 0.0
				end		
				const_R += layer_R
				const_heat_capacity += layer_heat_capacity
			end
			const_heatCapacity[name] = const_heat_capacity
			const_U[name] = 1.0 / const_R
			
		# next find the fenestration/window constructions find U value, heat capcity and SHGC of assembly
		elsif const.isFenestration
			const_heatCapacity[name]= 0.0		# assume windows have no heat capacity
			const_thermalAbsorptance[name]= 0.0  # assume windows have an effective thermal emissivity of 0
			const_solarAbsorptance[name] = 0.0 	# assume windows have an effective solar absorption of 0
		
			const_R = outside_air_film_R + inside_air_film_R  #set the starting R as outside+inside film coefficients

			# first check if the construction is a  single layer 
			#simple glazing, if so pull off U factor and SHGC directly, set heat capacity to 0
			if layers.size == 1 
				
				layer=layers[0]
				if not layer.to_SimpleGlazing.empty?	# check to see if the layer is simple glazing
					material = layer.to_SimpleGlazing.get #convert to a simple glazing material
					const_U[name]=material.uFactor.to_f				# U factor is directly defined
					const_SHGC[name] = material.solarHeatGainCoefficient.to_f
				elsif not layer.to_StandardGlazing.empty? # check to see if the layer is standard glazing
					material = layer.to_StandardGlazing.get   	# convert the layer to a standard glazing
					const_U[name] = 1.0/(1.0/ material.thermalConductance + const_R)		# U value = thermal conductance + film resistance
					const_SHGC[name] = material.solarTransmittance.to_f	# SHGC = solar transmittance - ignore frame
				elsif not layer.to_ThermochromicGlazing.empty?
					logfile.puts "thermochromic Glazing Not Converted - only estimating U value, SHGC set to 0.3"
					material = layer.to_RefractionExtinctionGlazing
					const_U[name] = 1.0/(1.0/ material.thermalConductance.to_f + const_R)
					const_SHGC[name] = 0.3		
				elsif not layer.to_RefractionExtinctionGlazing
					logfile.puts "Refraction Extinction Glazing Not Converted - only estimating U value, others set to clear glass values"
					material = layer.to_RefractionExtinctionGlazing
					const_U[name] = 1.0/(1.0/ material.thermalConductance.to_f + const_R)
					const_SHGC[name] = 0.7
				end
			elsif layers.size == 3	# double glaze window.   assume glass - gas - glass layers
				# assume that a layer is not simple glazing
				if not layers[0].to_StandardGlazing.empty? # check to see if layer 1 is standard glazing
					# get layer 1 properties
					material1 = layers[0].to_StandardGlazing.get   	# convert the 1st layer to a standard glazing
					norm_ST1 = material1.solarTransmittanceatNormalIncidence.to_f
					uValue1 = material1.thermalConductance.to_f
					norm_back_SR1 = material1.backSideSolarReflectanceatNormalIncidence.to_f
					# Now get layer 2 properties
					if not layers[1].to_StandardGlazing.empty?	# check to see if layer 2 is standard glazing
						material2 = layers[1].to_StandardGlazing.get			# convert the 2nd layer to a gas mixture
						norm_ST2 = material2.solarTransmittanceatNormalIncidence.to_f
						uValue2 = material2.thermalConductance.to_f				
					elsif not layers[1].to_AirGap.empty? # check to see if layer 1 is an air gap
						material2 = layers[1].to_AirGap.get			# convert the 2nd layer to an air gap
						norm_ST2 = 1.0
						uValue2 = 1.0 / material2.thermalResistance.to_f			
					elsif not layers[1].to_Gas.empty? # check to see if layer 1 is a gas
						material2 = layers[1].to_Gas.get			# convert the 2nd layer to a simple gas layer
						norm_ST2 = 1.0
						uValue2 = material2.getThermalConductance(290).to_f		# get U value at 290 K				
					elsif not layers[1].to_GasMixture.empty? # check to see if layer 1 is a gas
						material2 = layers[1].to_GasMixture.get			# convert the 2nd layer to a simple gas layer
						norm_ST2 = 1.0
						uValue2 = material2.getThermalConductance(290).to_f		# get U value at 290 K	
					else # we can't figure out what it is so assume properties of simple 12 mm air gap w/ U~6.0 from ASHRAE 2009
						norm_ST2 = 1.0
						uValue2 = 6.0					
					end
					# now get layer 3 properties
					if not layers[2].to_StandardGlazing.empty?	# check to see if layer 2 is standard glazing
						material3= layers[2].to_StandardGlazing.get			# convert the 2nd layer to a gas mixture
						norm_ST3 = material3.solarTransmittanceatNormalIncidence.to_f
						uValue3 = material3.thermalConductance.to_f
						norm_front_SR3 = material3.frontSideSolarReflectanceatNormalIncidence.to_f					
					else # If it isn't standard glazing we don't know what it might be so assume properties of simple 3 mm clear glazing
						norm_ST3 = 0.87
						uValue3 = 6.0
						norm_front_SR3 = material3.frontSideSolarReflectanceatNormalIncidence.to_f					
					end
					# compute SHGC of a 3 layer window as ST1 * ST3 / (1 - R1 *R3)
					const_SHGC[name] = norm_ST1 * norm_ST3 /(1.0 - norm_back_SR1*norm_front_SR3)
					# for U value = 1/ (sum R values for each layer + film coefficients)
					const_U[name] = 1.0/(1.0/uValue1 + 1.0/uValue2 + 1.0/uValue3 + const_R)  
				
				elsif not layers[0].to_ThermochromicGlazing.empty?
					logfile.puts "thermochromic Glazing Not Converted - only estimating U value, SHGC set to 0.3"
					material = layer.to_RefractionExtinctionGlazing
					const_U[name] = 1.0/(1.0/ material.thermalConductance + const_R)
					const_SHGC[name] = 0.3
				elsif not layers[0].to_RefractionExtinctionGlazing
					logfile.puts "Refraction Extinction Glazing Not Converted - only estimating U value, others set to clear glass values"
					material = layer.to_RefractionExtinctionGlazing
					const_U[name] = 1.0/(1.0/ material.thermalConductance + const_R)
					const_SHGC[name] = 0.7
				end
			else
				logfile.puts "Only 1 and 3 layer windows are supported at this time, values set to a standard clear, double glaze window"
				const_U[name] = 3.5
				const_SHGC[name] = 0.8
			end
		end
	end

	const_names.each do |name|
		logfile.puts name
		logfile.printf("Ufactor = %0.2f W/K/m2, Heat Capacity = %0.0f J/K/m2, ", const_U[name].to_f, const_heatCapacity[name].to_f)
		logfile.printf("Solar Absorption = %0.2f, Thermal Absorption = %0.1f , SHGC = %0.2f \n",const_solarAbsorptance[name],const_thermalAbsorptance[name],const_SHGC[name])
	end

	# parse the schedules to find the average schedule value for occupied and unoccupied times
	puts "...Parsing Schedules"
	
	# get the year description from the model and set the calendar year to 2007
	# we use 2006 because Jan 1 is a sinday and then day 1 of the week is sunday 
	yd = model.getYearDescription
	yd.setCalendarYear(2006)

	t = Array.new(24)
	(0..23).each do |hour|
		t[hour] = OpenStudio::Time.new(hour / 24.0)
	end
	
	sched_names = Array.new
	occ_aves = Hash.new
	unocc_aves = Hash.new
	occupied_hours=0.0
	unoccupied_hours = 0.0

	logfile.puts "*****************************"
	logfile.puts "  Calculating Schedule Averages"
	logfile.puts "*****************************"

	schedule_rulesets = model.getScheduleRulesets
	logfile.puts "Found #{schedule_rulesets.size} schedules in the OSM"

	
	if occupancy_extract_choice

		# create a hash with all the schedule_ruleset since we can't access dayschedules directly from
		# the schedule stored in the space_type, we need to find the associated schedule_ruleset
		sched_rulesets = Hash.new
		schedule_rulesets.each do |ruleset|
			name=ruleset.name.to_s
			sched_rulesets[name] = ruleset
		end

		occupancy_schedules_ave = Array.new(7){Array.new(24,0.0)}
		nscheds= 0
		space_area_total = 0.0
		# get an area weighted average over all the space types. 
		space_types.each do |space_type|
			space_area = space_type.floorArea
			space_area_total += space_area
			space_type.people.each do |people|
				# look for an occupancy schedule
				if not people.numberofPeopleSchedule.empty?
					# get the schedule.   Unfortuntately we need to find the associated sched ruleset to get the day schedule
					sched = people.numberofPeopleSchedule.get
					ruleset = sched_rulesets[sched.name.to_s]
					(0..51).each do |week|			
						startDate = yd.makeDate(week*7 +1)
						endDate = yd.makeDate(week*7 + 8)
						nscheds +=1
						space_area_total += space_area
						dayschedule = ruleset.getDaySchedules(startDate,endDate)	# get the day schedules for the week
						(0..6).each do |day|
							(0..23).each do |hour|
								value = dayschedule[day].getValue(t[hour])
								occupancy_schedules_ave[day][hour] += value*space_area	# add in area weighted value			
							end
						end
					end
				end
			end
		end

		# now get the area weighted average values for the schedules by dividing by the number of schedules averaged
		# also get the total daily fractional occupancy (i.e. equivalent hours of full time occupancy)

		threshold = 7.0
		num_occ_days = 0
		occ_frac_sum = Array.new(7,0)
		day_occ = Array.new(7,0)
		(0..6).each do |day|
			(0..23).each do |hour|
				occupancy_schedules_ave[day][hour] = occupancy_schedules_ave[day][hour]/space_area_total
				occ_frac_sum[day] +=occupancy_schedules_ave[day][hour] 
			end
			if occ_frac_sum[day] > threshold
				day_occ[day] = 1
				num_occ_days += 1
			end
		end

		# now find the first and last occupied day of the week assuming a continuous stream
		first_occ_day = day_occ.index(1)	# use ruby index to find the first instance of 1 in the day_occ array
		last_occ_day = 6 - (day_occ.reverse).index(1)	# use index to find the last instance of 1 by reversing array to find how far from the end the last occupied day is

		# now, for the days considered occupied, find an "average" occupancy schedule to estimate the best starting and stopping times
			# # find an "average" schedule during the occupied days (i.e. do not include unoccupied days in this average)
		avg_occupied_schedule = Array.new(24,0)
		avg_occ_sum = 0.0
		(0..23).each do |hour|
			avg_occupied_schedule[hour]=0.0
			(first_occ_day .. last_occ_day).each do |day|
				avg_occupied_schedule[hour] += occupancy_schedules_ave[day][hour]
			end
			if (num_occ_days> 0)
				avg_occupied_schedule[hour] = avg_occupied_schedule[hour] / num_occ_days
			end
			avg_occ_sum += avg_occupied_schedule[hour]
		end

		# now estimate the average number of fully occupied hours per day by rounding down the sum to the nearest number of hours
		num_occ_hours = avg_occ_sum.floor

		#now we find the best start time for the occupancy by matching a sliding window of full occupancy to the average occupancy
		#we maximize the sum of the product of actual occupancy and the sliding window to get the best hours of operation
		# find the start time by maximizing the sum of the product of the start-to-end sliding window and the average hourly occupancy
		max_sum = 0.0
		max_index = 0
		(0 .. (23-num_occ_hours)).each do |start_hour|
			temp_sum = 0.0
			(start_hour .. start_hour + num_occ_hours).each do |i|
				temp_sum += avg_occupied_schedule[i] 
			end
			if temp_sum > max_sum
				max_sum = temp_sum
				max_index=start_hour
			end
		end

		occupancy_day_start = first_occ_day 		# set starting day for occupancy
		occupancy_day_end = last_occ_day					# set the ending day for occupancy
		occupancy_hour_start = max_index 			# set starting hour for occupied day
		occupancy_hour_end = max_index+ num_occ_hours + 1	# set ending hour for an occupied day.   Add one because we want occupancy to drop at that time

	end

	startDate = yd.makeDate(1)
	endDate = yd.makeDate(365)

	schedule_rulesets.each do |schedule|
		occupied_sum=0;
		unoccupied_sum=0;
		occupied_hours=0;
		unoccupied_hours=0;
		
		# gets one day schedule for each day of the year
		daySchedules = schedule.getDaySchedules(startDate, endDate)
			
		#get the day of the week of the starting day of the schedule and subtract 1 from it because we increment before we compare
		day_of_week =  startDate.dayOfWeek.value.to_i - 1
		
		# loop over the schedule for each day of the year
		daySchedules.each do |daySchedule|

			day_of_week +=1 # increment the day of week counter wrapping around at 7
			if day_of_week > 6
				day_of_week =0
			end
			
			# loop over each hour of the day
			(0..23).each do |t|
				# get the value at this hour
				time = OpenStudio::Time.new(t / 24.0)
				value = daySchedule.getValue(time)
				
				# check if the day of the week is an occupied day or not
				if (day_of_week >= occupancy_day_start) && (day_of_week <= occupancy_day_end)
					# check if the hour is also an occupied hour
					# if so, add value to occupied sum and increment occupied counter
					if (t >= occupancy_hour_start) && (t < occupancy_hour_end)
						occupied_sum+= value
						occupied_hours += 1
					else # if hour not occupied, add value to unoccupied and increment unoccupied counter
						unoccupied_sum+= value
						unoccupied_hours += 1
					end
				else	# if day is not occupied, add the hour value to unoccupied 
					unoccupied_sum+= value
					unoccupied_hours += 1
				end
			end
		end
		sched_names << schedule.name
		
		occ_aves[schedule.name.to_s] = occupied_sum / occupied_hours
		unocc_aves[schedule.name.to_s] = unoccupied_sum / unoccupied_hours
		logfile.printf("Schedule %s has occupied ave = %3.2f and unoccupied ave = %3.2f\n",schedule.name.to_s, occ_aves[schedule.name.to_s], unocc_aves[schedule.name.to_s])
		
	end

	# compute the number of occupied days in a year for water use calculations
	number_days_occupied_per_year = 52 * (occupancy_day_end - occupancy_day_start +1)
	
	# at this point we have an array with the schedule names and two arrays with the averages
	# that are indexed by the schedule names.   

	# define what fraction of the year are occupied and unoccupied
	frac_year_occ = occupied_hours/8760.0
	frac_year_unocc = unoccupied_hours/8760.0

	logfile.printf("Fraction of year occupied = %1.3f and Fraction of year unoccupied = %1.3f\n", frac_year_occ, frac_year_unocc)

	puts "...Calculating Electric and Gas Loads"
	logfile.puts "*****************************"
	logfile.puts "  Calculating Electric and Gas Loads"
	logfile.puts "*****************************"

	# get the light, electrical, and gas power densities and people densities
	nominal_lpd= building.lightingPowerPerFloorArea # overall nominal lighting power density
	nominal_epd = building.electricEquipmentPowerPerFloorArea # overall nominal electric equipment power density W/m2
	nominal_gpd = building.gasEquipmentPowerPerFloorArea # overall nominal gas equipment power density W/m2

	# loop over the lighting, equipment, and occupancy schedules to get the occupied and unoccupied values 
	# and compute the area weighted averages
	light_total_area = 0   # set this to a tiny number to avoid divide by zero but to also avoid skewing the area
	light_occ_total = 0
	light_unocc_total = 0
	elec_total_area = 0
	elec_occ_total = 0
	elec_unocc_total=0
	gas_total_area = 0
	gas_occ_total = 0
	gas_unocc_total=0
	people_total_area = 0
	people_occ_total = 0
	people_unocc_total = 0
	activity_total_area = 0
	activity_occ_total = 0
	activity_unocc_total = 0

	light_areas=Array.new
	light_scheds=Array.new
	space_types.each do |space_type|
		# work with the lighting schedule - luminaires are not supported at this time
		space_area = space_type.floorArea
		space_type.lights.each do |light|
			if not light.schedule.empty?	# look for a lighting schedule
				sched = light.schedule.get	# get the schedule
				light_total_area += space_area
				light_occ_total = occ_aves[sched.name.to_s] * space_area
				light_unocc_total = unocc_aves[sched.name.to_s] * space_area
			end
		end
		
		space_type.electricEquipment.each do |electric|
			if not electric.schedule.empty?  # look for an electric equipment schedule
				sched = electric.schedule.get
				elec_total_area += space_area
				elec_occ_total = occ_aves[sched.name.to_s] * space_area
				elec_unocc_total = unocc_aves[sched.name.to_s] * space_area
			end
		end

		space_type.gasEquipment.each do |gas|
			if not gas.schedule.empty? 	# look for an gas equipment schedule
				sched = gas.schedule.get
				gas_total_area += space_area
				gas_occ_total = occ_aves[sched.name.to_s] * space_area
				gas_unocc_total = unocc_aves[sched.name.to_s] * space_area
			end	
		end
		
		space_type.people.each do |people|
			# look for an occupancy schedule
			if not people.numberofPeopleSchedule.empty?
				sched = people.numberofPeopleSchedule.get
				people_total_area += space_area
				people_occ_total = occ_aves[sched.name.to_s] * space_area
				people_unocc_total = unocc_aves[sched.name.to_s] * space_area
			end
			
			# look for an occupant activity level schedule
			if not people.activityLevelSchedule.empty?
				sched = people.activityLevelSchedule.get
				activity_total_area += space_area
				activity_occ_total = occ_aves[sched.name.to_s] * space_area
				activity_unocc_total = unocc_aves[sched.name.to_s] * space_area
			end	
		end
		
	end

	# compute the fractional multipliers from the schedule data
	# set to 1.0 if the lighting area is 0
	if light_total_area > 0
		lpd_occ_mult = light_occ_total/light_total_area
		lpd_unocc_mult = light_unocc_total/light_total_area
	else
		lpd_occ_mult = 1
		lpd_unocc_mult = 1
	end
	if elec_total_area > 0
		epd_occ_mult = elec_occ_total/elec_total_area
		epd_unocc_mult = elec_unocc_total/elec_total_area
	else
		epd_occ_mult = 1
		epd_unocc_mult = 1
	end
	if gas_total_area > 0
		gpd_occ_mult = gas_occ_total/gas_total_area
		gpd_unocc_mult = gas_unocc_total/gas_total_area
	else
		gpd_occ_mult = 1
		gpd_unocc_mult = 1
	end
	if people_total_area > 0
		people_occ_mult = people_occ_total/people_total_area
		people_unocc_mult = people_unocc_total/people_total_area	
	else
		people_occ_mult = 1
		people_unocc_mult = 1
	end
	# compute the activity level (heat gain per person) from schedule data
	activity_level_occ= activity_occ_total/activity_total_area
	activity_level_unocc= activity_unocc_total / activity_total_area

	# compute the occupied and unoccupied lighting and equipment power densities

	lpd_unoccupied=nominal_lpd*lpd_unocc_mult
	# when computing occupied, subtract the occupied because the isomodel calc adds unoccupied to occupied to account for parasitic lighting
	# that people often forget
	lpd_occupied=nominal_lpd*lpd_occ_mult - lpd_unoccupied
	epd_occupied=nominal_epd*epd_occ_mult
	epd_unoccupied=nominal_epd*epd_unocc_mult
	gpd_occupied=nominal_gpd*gpd_occ_mult
	gpd_unoccupied=nominal_gpd*gpd_unocc_mult

	logfile.printf("Lighting Power Density: Occupied= %1.2f W/m2, Unoccupied = %1.2f W/m2\n",lpd_occupied, lpd_unoccupied)
	logfile.printf("Electric Equipment Power Density: Occupied= %1.2f W/m2, Unoccupied = %1.2f W/m2\n",epd_occupied, epd_unoccupied)
	logfile.printf("Gas Equipment Power Density: Occupied= %1.2f W/m2, Unoccupied = %1.2f W/m2\n",gpd_occupied, gpd_unoccupied)

	# compute the nominal area per person info
	nominal_app = 1.0/building.peoplePerFloorArea  # nominal area per person m^2/person

	# set the heat gain per person to be the average occupied value  
	heat_gain_per_person_occ = activity_level_occ
	heat_gain_per_person_unocc = activity_level_unocc

	# get the area per person scaled by occupancy multiplier
	area_per_person_occupied = nominal_app /people_occ_mult
	area_per_person_unoccupied = nominal_app/people_unocc_mult

	logfile.printf("Occupant Heat Gain: Occupied= %1.2f W/person, Unoccupied = %1.2f W/person\n",heat_gain_per_person_occ, heat_gain_per_person_unocc)
	logfile.printf("Area per person: Occupied= %1.2f m2/person, Unoccupied = %1.2f m2/person\n",area_per_person_occupied, area_per_person_unoccupied)

	# compute exterior lighting power
	exterior_lighting_power = 0.0
	if not facility.exteriorLights.empty?
		ext_lights=facility.exteriorLights  # get the vector of exterior lights
		# loop over the lights
		ext_lights.each do |exlight|
			ext_light_mult = exlight.multiplier
			ext_light_base = exlight.exteriorLightsDefinition.designLevel
			exterior_lighting_power = ext_light_base * ext_light_mult
		end
		logfile.puts "Found #{ext_lights.size} Exterior Lights"
	end
	logfile.printf("Exterior Lighting Power = %1.2f W\n ",exterior_lighting_power)
		
	# check for luminaire definitions and print a warning if they are defined
	luminaires = model.getLuminaires
	if not luminaires.empty?
		logfile.puts "Warning: Luminaires defined in OSM but not currently translated into ISO"
	end


	puts "...Calculating Thermostats"
	logfile.puts "*****************************"
	logfile.puts "  Calculating Thermostats"
	logfile.puts "*****************************"

	# set the occupied and unoccupied temperature setpoint by finding average setpoint over the occupied and unoccupied times
	thermostats = model.getThermostatSetpointDualSetpoints

	total_zone_area = 0
	heat_setpoint_occ_total=0
	heat_setpoint_unocc_total=0
	cool_setpoint_occ_total = 0
	cool_setpoint_unocc_total = 0

	# loop through the zones and find the thermostats rather than use the thermostat vector we can get from the model
	# we want to do a zone area weighted average in case the different zones have different 
	thermal_zones.each do |zone|		#loop through the zones 
		zone_area=zone.floorArea * zone.multiplier
		total_zone_area += zone_area

		if not zone.thermostatSetpointDualSetpoint.empty?
			thermostat=zone.thermostatSetpointDualSetpoint.get
			if not thermostat.coolingSetpointTemperatureSchedule.empty?
				cool_sched = thermostat.coolingSetpointTemperatureSchedule.get
				cool_setpoint_occ_total += occ_aves[cool_sched.name.to_s] * zone_area 
				cool_setpoint_unocc_total += unocc_aves[cool_sched.name.to_s] * zone_area
			else  # if we have no schedule, use the default values for thiz zone
				cool_setpoint_occ_total += cool_setpoint_occ_default * zone_area
				cool_setpoint_unocc_total += cool_setpoint_unocc_default * zone_area
			end
			if not thermostat.heatingSetpointTemperatureSchedule.empty?
				heat_sched = thermostat.heatingSetpointTemperatureSchedule.get
				heat_setpoint_occ_total += occ_aves[heat_sched.name.to_s] * zone_area 
				heat_setpoint_unocc_total += unocc_aves[heat_sched.name.to_s] * zone_area
			else
				heat_setpoint_occ_total += heat_setpoint_occ_default * zone_area
				heat_setpoint_unocc_total += heat_setpoint_unocc_default * zone_area	
			end
		else
			cool_setpoint_occ_total += cool_setpoint_occ_default * zone_area
			heat_setpoint_occ_total += heat_setpoint_occ_default * zone_area
			cool_setpoint_unocc_total += cool_setpoint_unocc_default * zone_area
			heat_setpoint_unocc_total += heat_setpoint_unocc_default * zone_area		
		end
	end
	cooling_setpoint_occ = cool_setpoint_occ_total/total_zone_area
	cooling_setpoint_unocc = cool_setpoint_unocc_total/total_zone_area
	heating_setpoint_occ = heat_setpoint_occ_total/total_zone_area
	heating_setpoint_unocc = heat_setpoint_unocc_total/total_zone_area
	logfile.printf("Cooling Setpoint Occupied = %1.2f C, Cooling Setpoint Unoccupied = %1.2f C\n",cooling_setpoint_occ, cooling_setpoint_unocc)
	logfile.printf("Heating Setpoint Occupied = %1.2f C, Heating Setpoint Unoccupied = %1.2f C\n",heating_setpoint_occ, heating_setpoint_unocc)

	# try to find if >60% of floor area has daylight control
	# loop through spaces, get area and check for any daylighting controls
	daylight_area = 0.0
	total_area = 0.0
	spaces.each do |s|
		total_area += s.floorArea * s.multiplier  # add up the total floor area
		daylight_ctrls = s.daylightingControls
		if daylight_ctrls.size != 0   # check for a daylighting control of any kind, assume it's turned on
			daylight_area += s.floorArea * s.multiplier  # add up the total daylighting control area
		end
	end
	if total_area > 0
		frac_daylight=daylight_area / total_area
	else
		frac_daylight = 0
	end
	
	logfile.printf("Calculated %1.2f of total area has daylighting controls \n",frac_daylight)
	if frac_daylight > 0.6
		daylight_sensors=2  # turn on daylighting controls in ISO model 
	else
		daylight_sensors=1  # turn off daylighting controls in ISO model
	end

	puts "...Parsing Geometry"

	logfile.puts "*****************************"
	logfile.puts "    Parsing Geometry"
	logfile.puts "*****************************"

	# get the overall floor area from the building object
	# first see if conditioned floor area is set and use that if it is, otherwise use regular floor area
	if not building.conditionedFloorArea.empty?
		floor_area = building.conditionedFloorarea
	else
		floor_area = building.floorArea 
	end
	logfile.puts "Floor Area = #{floor_area} m2"

	# Compute the building height as the difference from max and min vertex height
	maxz = -1000.0
	minz = 1000.0
	spaces.each do |sp|
	  # loop through space surfaces to find max z value
		z_points=Array.new
		sp.surfaces.each do |s|  
			s.vertices.each do |vertex|
				z_points << vertex.z  # get out all the z points for the surface and put in the array
			end
			minsz = z_points.min + sp.zOrigin	# find the min z for the surface and add the space origin offset
			maxsz = z_points.max + sp.zOrigin	# find the max z for the surface and add the space origin offset
			if maxsz > maxz then maxz = maxsz end
			if minsz < minz then minz = minsz end
		end
	end
	building_height=maxz-minz

	logfile.puts "Building Height  = #{building_height} m"

	# find the areas of wall, roofs, and windows
	# create arrays for the walls and windows
	direction_names= ["N","NE","E","SE","S","SW","W","NW","N"]
	dirLimit = [0,45,90,135,180,225,270,315,360]
	
	wall_areas=Array.new(8,0.0)  # generate an 8x1 array filled with zeros
	window_areas=Array.new(8,0.0)
	window_areas=Array.new(8,0.0)
	wall_U = Array.new(8,0.0)
	window_U = Array.new(8,0.0)
	wall_solar_absorption = Array.new(8,0.0)
	wall_thermal_emissivity = Array.new(8,0.0)
	window_SHGC = Array.new(8,0.0)

	wall_HC_sum = 0.0
	wall_U_area = Array.new(8,0.0)
	wall_U_sum = Array.new(8,0.0)
	wall_abs_sum = Array.new(8,0.0)
	wall_emiss_sum =Array.new(8,0.0)
	wall_abs_area =Array.new(8,0.0)

	window_HC_sum=0.0
	window_U_sum =Array.new(8,0.0)
	window_U_area = Array.new(8,0.0)
	window_SHGC_sum = Array.new(8,0.0)

	# extract outside walls and windows, and add up areas and add up weighted surface properties
	logfile.puts "Found #{building.exteriorWalls.size} exterior wall surfaces"
	building.exteriorWalls.each do |s|
		
		absorption = const_solarAbsorptance[s.construction.get.name.to_s]
		emissivity = const_thermalAbsorptance[s.construction.get.name.to_s]
		heatcapacity = const_heatCapacity[s.construction.get.name.to_s]
	 
		azimuth = s.azimuth*180/3.141592654  # get the surface azimuth and convert from radians to degrees
		az = azimuth + 22.5  # rotate the coordinate system 22.5 degrees so N is 0 to 45 instead of -22.5 to 22.5
		if az > 360 then 
			az = az - 360	# remap anything between 360 and 382.5 back to 0 to 22.5
		end 
		for i in 0 .. 7
			if (az > dirLimit[i]) && (az<= dirLimit[i+1])

				wall_areas[i] +=s.netArea	# add in the wall area without window
				wall_abs_sum[i] += absorption * s.netArea
				wall_emiss_sum[i] += emissivity * s.netArea		
				wall_HC_sum += heatcapacity *s.netArea
		
				wallU = const_U[s.construction.get.name.to_s]
				# if the U value is not NaN add in the U value and area for weighting
				if not wallU.nan?
					wall_U_area[i]+= s.netArea
					wall_U_sum[i] += wallU * s.netArea
				end
				subsurface=s.subSurfaces
				subsurface.each do |ss|
					windowU = const_U[ss.construction.get.name.to_s]
					windowSHGC = const_SHGC[ss.construction.get.name.to_s]
					if not windowU.nan?
						window_U_area[i] += ss.surface.get.netArea
						window_U_sum[i] += windowU * ss.surface.get.netArea
						window_SHGC_sum[i] += windowSHGC * ss.surface.get.netArea
					end
				end
				window_areas[i] += s.grossArea-s.netArea		# get just the window area	
				# add in lines to  compute SCF, and SDF in each direction here
			end		
		end		
	end 

	total_wall_area =0.0
	wall_areas.each do |a| 
		total_wall_area += a
	end
	logfile.printf("Total Wall Area = %1.2f m2\n",total_wall_area)
	
	total_window_area=0.0
	window_areas.each do |a| 
		total_window_area += a
	end
	logfile.printf("Total Window Area = %1.2f  m2\n",total_window_area)

	#  Get the area weighted average of wall absorption, emissivity, U and window U and SHGC
	if wall_roof_extract_choice
		(0..7).each do |i|
			if not (wall_areas[i] == 0.0)
				wall_solar_absorption[i] = wall_abs_sum[i] / wall_areas[i]
				wall_thermal_emissivity[i]= wall_emiss_sum[i] / wall_areas[i]
			else	# if wall areas are 0, set wall properties to zero
				wall_solar_absorption[i] = 0.0
				wall_thermal_emissivity[i] = 0.0
			end
			if not (wall_U_area[i] == 0.0)  # if any wall surfaces have a U value defined, use the weighted average of U values
				wall_U[i] = wall_U_sum[i]/wall_U_area[i]
			else	# if wall_U areas are 0, set the wall U value to zero
				wall_U[i] = 0.0
			end
		end
	else
		(0..7).each do |i|
			wall_U[i] = wall_U_choice
			wall_solar_absorption[i] = envelope_solar_abs_choice
			wall_thermal_emissivity[i] = envelope_thermal_emiss_choice
		end
	end

	if glazing_extract_choice	# if we chose to extract glazing info from the OSM file
		# get the area weighte average of window U and SHGC
		(0..7).each do |i|
			if not (window_areas[i] == 0.0)
				window_U[i] = window_U_sum[i]/window_areas[i]
				window_SHGC[i] = window_SHGC_sum[i] / window_areas[i]
			else	# if window areas are zero, set window properties to zero
				window_U[i] = 0.0
				window_SHGC[i] = 0.0
				window_SCF[i] = 0.0
				window_SDF[i] = 0.0
			end
		end
	else	# if we elected to take glazing info from user input
		(0..7).each do |i|
			window_U[i] = glazing_U_choice
			window_SHGC[i] = glazing_SHGC_choice
		end
	end	
	
	for i in 0..7
		logfile.printf("Wall%s: Area=%1.0f m2, U=%1.2f W/m2/K, Absorption=%1.2f, Emissivity=%1.2f \n",
			direction_names[i],wall_areas[i],wall_U[i],wall_solar_absorption[i],wall_thermal_emissivity[i])
	end
	for i in 0..7
		logfile.printf("Window%s: Area=%1.0f m2, U=%1.2f W/m2/K, SHGC=%1.2f, SCF=%1.2f, SDF=%1.2f \n",
			direction_names[i],window_areas[i],window_U[i],window_SHGC[i],window_SCF[i],window_SDF[i])
	end

	roof_area=0.0
	skylight_area=0.0

	roof_abs_sum = 0.0
	roof_emiss_sum = 0.0
	roof_U_sum =0.0
	roof_HC_sum = 0.0
	skylight_U_sum =0.0
	skylight_SHGC_sum = 0.0
	roof_count=0

	# the building.roofs variable does not seem to return the roof surfaces in a vector as it supposed to 
	# so we search through all surfaces and find the outside roof surfaces ourselves
	# calculate area and average solar absorption and thermal emissivity
	surfaces.each do |s|
		next if not (s.surfaceType == "RoofCeiling"	)			# skip surface if not labeled roof
		next if not (s.outsideBoundaryCondition == "Outdoors")	# skip surface if not outside
		roof_count +=1
		absorption = const_solarAbsorptance[s.construction.get.name.to_s]
		emissivity = const_thermalAbsorptance[s.construction.get.name.to_s]
		heatcapacity = const_heatCapacity[s.construction.get.name.to_s]
		roofU = const_U[s.construction.get.name.to_s]
		
		roof_abs_sum += absorption * s.netArea
		roof_emiss_sum += emissivity * s.netArea
		roof_HC_sum += heatcapacity * s.netArea
		roof_U_sum += roofU * s.netArea

		roof_area += s.netArea					# get the roof area without skylights

		subsurface=s.subSurfaces	# find subsurfaces on roof and assume they are skylights
		subsurface.each do |ss|
			skylight_area += ss.surface.get.netArea
			skylight_U_sum += const_U[ss.construction.get.name.to_s] * ss.surface.get.netArea
			skylight_SHGC_sum+= const_SHGC[ss.construction.get.name.to_s] * ss.surface.get.netArea
		end	
	end
	logfile.puts "Found #{roof_count} roof surfaces" 

	#  Get the area weighted average of wall absorption, emissivity, U and window U and SHGC
	if wall_roof_extract_choice
		if not (roof_area == 0.0)
			roof_solar_absorption = roof_abs_sum / roof_area 		# compute roof absorption as area weighted average
			roof_thermal_emissivity=  roof_emiss_sum/ roof_area 	# compute roof emissivity as an area weighted average
			roof_U = roof_U_sum / roof_area 						# compute roof U as area weighted average	
		else	# if roof area is zero, wset roof properties to zero
			roof_U = 0.0
			roof_solar_absorption = 0.0
			roof_thermal_emissivity = 0.0	
		end	
	else
		roof_U = roof_U_choice
		roof_solar_absorption = envelope_solar_abs_choice
		roof_thermal_emissivity = envelope_thermal_emiss_choice
	end

	#  Get the area weighted average of wall absorption, emissivity, U and window U and SHGC
	if glazing_extract_choice
		if not (skylight_area == 0.0)
			skylight_U = skylight_U_sum / skylight_area
			skylight_SHGC = skylight_SHGC_sum /skylight_area
		else # if skylight area is zero, set skylight area to zero
			skylight_U = 0.0
			skylight_SHGC = 0.0
		end
	else
		skylight_U = glazing_U_choice
		skylight_SHGC = glazing_SHGC_choice
	end	
	
	logfile.printf("Roof: Area=%1.0f m2, U=%1.2f W/m2/K, Absorption=%1.2f, Emissivity=%1.2f \n",roof_area,roof_U,roof_solar_absorption,roof_thermal_emissivity)
	logfile.printf("Skylight: Area=%1.0f m2, U=%1.2f W/m2/K, SHGC=%1.2f \n",skylight_area,skylight_U,glazing_SHGC_choice)

	# compute exterior heat capacity
	exterior_heat_capacity = (roof_HC_sum+ wall_HC_sum + window_HC_sum)/(total_wall_area + roof_area+ total_window_area)
	logfile.printf("Exterior Heat Capacity = %1.0f J/K/m2 based on surface area\n", exterior_heat_capacity)

	# add up the heat capacity of the defined interior surfaces including any internale mass elements
	# compute as normalized to floor area and not surface area
	interior_HC_sum = 0.0
	surfaces.each do |s|
		next if s.outsideBoundaryCondition == "Outdoors"	# skip surface if outside
		interior_HC_sum += const_heatCapacity[s.construction.get.name.to_s] * s.netArea
	end

	# get and add in the heat capacity of any defined internal masses
	internal_mass_HC_sum =0.0
	internal_masses.each do |mass|
		internal_mass_HC_sum += mass.surfaceArea.to_f * const_heatCapacity[mass.construction.get.name.to_s]
	end
	interior_heat_capacity = (interior_HC_sum +internal_mass_HC_sum)/building.floorArea
	logfile.printf("Interior  Heat Capacity = %1.0f J/K/m2 based on floor area\n", interior_heat_capacity)

	if not infiltration_extract_choice
		infiltration_rate = infiltration_rate_choice
		logfile.puts "Infiltration extraction not selected.  Using user input values instead"
	else
	
		puts "...Calculating Infiltration"
		logfile.puts "*****************************"
		logfile.puts "   Calculating Infiltration"
		logfile.puts "*****************************"

		# compute infiltration
		#first check to see if there effective leakage areas defined and if not those, then design flow rates
		if not model.getSpaceInfiltrationEffectiveLeakageAreas.empty?
			infiltration = model.getSpaceInfiltrationEffectiveLeakageAreas
			infiltration.each do |infil|
				# set default average envelope air leakage from measure input which is about the EnergyPlus defaults
				infiltration_rate = infiltration_rate_choice		
				logfile.puts "EffectiveLeakageArea not Implemented Yet, Using user input infiltration rate"
			end
		elsif not model.getSpaceInfiltrationDesignFlowRates.empty?
			infiltration = model.getSpaceInfiltrationDesignFlowRates
			logfile.puts "Found #{infiltration.size} SpaceInfiltrationDesignFlowRate objects"
			
			infil_frac_sum = 0.0
			infil_rate_sum = 0.0
			count = 0
			infiltration.each do |infil|
				count += 1
				rate_type = infil.designFlowRateCalculationMethod
				if not infil.space.empty?
					logfile.puts "Space defined for Infiltration object ##{count}"
					case rate_type
					when "Flow/Space"
						# add in the design flow rate per space * number of spaces 
						infil_rate_temp = infil.designFlowRate.to_s.to_f
						logfile.puts "Infiltration Object # #{count} is Flow/Space, #{infil_rate_temp} m3/s added"
					when "Flow/ExteriorArea", "Flow/ExteriorWallArea"
						# add in the flow/exterior area  * the exterior area
						infil_rate_temp = infil.flowperExteriorSurfaceArea.to_s.to_f * infil.space.exteriorArea
						logfile.puts "Infiltration Object # #{count} is Flow/ExteriorArea, #{infil_rate_temp} m3/s added"
					when "Flow/Area"
						infil_rate_temp = infil.flowperSpaceFloorArea.to_s.to_f * infil.space.floorArea
						logfile.puts "Infiltration Object # #{count} is Flow/Area, #{infil_rate_temp} m3/s added"
					when "AirChanges/Hour"
						infil_rate_temp= 0
						logfile.puts "Infiltration Object # #{count} is Flow/Space, Not implemented yet so nothing added"
					else
						infil_rate_temp= 0
						logfile.puts "Infiltration Ojbect # #{count} has no calc method defined, so nothing added"
					end
				elsif not infil.spaceType.empty?	
					logfile.puts "No space defined in Infiltration object # #{count}, using spacetype object instead"
					st=infil.spaceType.get
					st_volume =0.0
					st_exterior_area = 0.0
					st_num = st.spaces.size
					st.spaces.each do |s|
						st_volume += s.volume
						st_exterior_area += s.exteriorArea
					end
					logfile.puts "Found #{st.spaces.size} spaces in spacetype for Infiltration object #{count}"
					logfile. puts "Infiltration design flow rate found of type #{rate_type}"
					
					case rate_type
					when "Flow/Space"
						# add in the design flow rate per space * number of spaces 
						infil_rate_temp= infil.designFlowRate.to_s.to_f * st.spaces.size
						logfile.puts "Infiltration Object # #{count} is Flow/Space, #{infil_rate_temp} m3/s added"
					when "Flow/ExteriorArea", "Flow/ExteriorWallArea"
						# add in the flow/exterior area  * the exterior area
						infil_rate_temp= infil.flowperExteriorSurfaceArea.to_s.to_f * st_exterior_area
						logfile.puts "Infiltration Object # #{count} is Flow/ExteriorArea, #{infil_rate_temp} m3/s added"				
					when "Flow/Area"
						# add in the flow/floor area * floor area
						infil_rate_temp = infil.flowperSpaceFloorArea.to_s.to_f * st.floorArea
						logfile.puts "Infiltration Object # #{count} is Flow/Area, #{infil_rate_temp} m3/s added"				
					when "AirChanges/Hour"
						infil_rate_temp= 0
						infil_rate_temp = infil.flowperSpaceFloorArea.to_s.to_f * st.floorArea
						logfile.puts "Infiltration Object # #{count} is AirChanges/Hour, #{infil_rate_temp} m3/s added"
					else
						infil_rate_temp = infil.airChangesperHour.to_s.to_f * st_volume
						logfile.puts "Infiltration Ojbect # #{count} has no calc method defined, so nothing added"				
					end
				else
					logfile.puts "No space or spacetype defined for Infiltration object # #{count}, skipping analysis"
				end
				
				# now look for the schedule for the infil object and add up the fraction
				if not infil.schedule.empty?
					sched = infil.schedule.get.name.to_s
					# if a schedule exists, get the average yearly fraction by a time weighted average of occupied and unoccupied averages
					infil_frac =  occ_aves[sched]*frac_year_occ + unocc_aves[sched]*frac_year_unocc
					logfile.puts "Infiltration schedule found for Infiltration object #{count}, scaling by #{infil_frac}"
				else
					logfile.puts "No Infiltration schedule found for Infiltration object #{count}, assuming always on"
					infil_frac = 1.0
				end
				infil_rate_sum +=  infil_rate_temp * infil_frac
			end
			logfile.printf("Total Infiltration = %1.3f m3/s at 4 Pa (natural pressure difference)\n",infil_rate_sum);

			# get avg infiltration rate and convert to from m3/m2/min @ 4 Pa to m3/m2/h @ 75 Pa with a .67 pressure exponent
			# assume constant infilration rate is based on a nominal 4 Pa pressure difference 
			infiltration_rate = infil_rate_sum/(total_wall_area + roof_area)*3600*(75/4)**0.67
		else
			infiltration_rate = infiltration_rate_choice
			logfile.puts "No Infiltration Design Flow Rates Found. Use user input as default"
		end
	end
	logfile.printf( "Infiltration rate set to %1.3f m3/h/m2 @ 75 Pa\n", infiltration_rate)

	puts "...Parsing HVAC Info"
	logfile.puts "*****************************"
	logfile.puts "   Parsing HVAC Info"
	logfile.puts "*****************************"

	# set the HVAC info using user intp or parse and gather info for COP, Heating Eff, and Heating fuel type
	if not hvac_extract_choice 
		puts "Using User Input HVAC Info"
		cooling_COP = cooling_cop_choice
		heating_system_efficiency = heating_eff_choice
		heating_fuel_type = heating_fuel_choice_string.to_i	
	else
		logfile.puts "*****************************"
		logfile.puts "  Analyzing HVAC"
		logfile.puts "*****************************"
		puts "...Analyzing HVAC"

		hvac_component_array = Array.new
		hvac_component_area_array=Array.new

		plant_loops_array=Array.new
		plant_loops_area_array=Array.new

		# look through the air loops and extract the components
		# for components that can connect to a plant water loop (namely things with water heating and cooling coils)
		air_loops.each do |air_loop|
			supply_components = air_loop.supplyComponents

			zone_area = 0.0
			# get the total area of all the zones connected through the loop
			air_loop.thermalZones.each do |z|
				zone_area += z.floorArea	#
			end

			# loop through the components and store them all in a single array
			# if 
			supply_components.each do |component|
				next if not component.to_Node.empty?   				# skip if the component is a node
				next if not component.to_ConnectorMixer.empty?  	# skip of component is a mixer connector
				next if not component.to_ConnectorSplitter.empty? 	# skip if component is a splitter
				next if not component.to_PipeAdiabatic.empty?		# skip if component is a pipe
					
				# if the component is a water cooling coil, do not save, but instead save the attached plant loop
				if (not component.to_CoilCoolingWater.empty?)  # if the component is a cooling coil, don't save the attached plant loop	
					plant_loops_array << component.to_CoilCoolingWater.get.plantLoop
					plant_loops_area_array << zone_area
				# if the component is a water cooling coil, do not save but instead save the attached plant loop
				elsif not component.to_CoilHeatingWater.empty? # if the component is a heating coil, don't save and search the attached plant loop
					plant_loop_components = component.to_CoilHeatingWater.get.plantLoop.get.supplyComponents  # get the cooling coil plant loop
					plant_loops_array << component.to_CoilHeatingWater.get.plantLoop
					plant_loops_area_array << zone_area	
				else # otherwise this component is not hooked to a water loop so save it for later parsing
					hvac_component_array << component
					hvac_component_area_array << zone_area
				end
			end
		end

		(0 .. (plant_loops_array.size - 1)).each do |i|
			supply_components = plant_loops_array[i].get.supplyComponents
			zone_area = plant_loops_area_array[i]

			supply_components.each do |component|
				next if not component.to_Node.empty?   				# skip if the component is a node
				next if not component.to_ConnectorMixer.empty?  	# skip of component is a mixer connector
				next if not component.to_ConnectorSplitter.empty? 	# skip if component is a splitter
				next if not component.to_PipeAdiabatic.empty?		# skip if component is a pipe
				
				hvac_component_array << component
				hvac_component_area_array << zone_area	
			end
		end

		# now add in any HVAC components not part of an air loop (PTAC, PTHP, unit heaters, etc)
		thermal_zones.each do |zone|
			zone.equipment.each do |component|
				hvac_component_array << component
				hvac_component_area_array << zone.floorArea
			end
		end

		cooling_coil_array=Array.new
		cooling_coil_area_array=Array.new
		heating_coil_array=Array.new
		heating_coil_area_array=Array.new
		(0 .. (hvac_component_array.size-1)).each do |i|
			component=hvac_component_array[i]
			area = hvac_component_area_array[i]

			# first check for PTAC
			if not component.to_ZoneHVACPackagedTerminalAirConditioner.empty?
				ptac=component.to_ZoneHVACPackagedTerminalAirConditioner.get		
				cooling_coil_array << component.to_ZoneHVACPackagedTerminalAirConditioner.get.coolingCoil
				cooling_coil_area_array << area
				heating_coil_array << component.to_ZoneHVACPackagedTerminalAirConditioner.get.heatingCoil
				heating_coil_area_array << area
			# next check for PTHP
			elsif not component.to_ZoneHVACPackagedTerminalHeatPump.empty?
				pthp=component.to_ZoneHVACPackagedTerminalHeatPump.get
				cooling_coil_array << component.to_ZoneHVACPackagedTerminalHeatPump.get.coolingCoil
				cooling_coil_area_array << area
				heating_coil_array << component.to_ZoneHVACPackagedTerminalHeatPump.get.heatingCoil
				heating_coil_area_array << area
			# next check for water to air heat pump
			elsif not component.to_ZoneHVACWaterToAirHeatPump.empty?
				wahp=component.to_ZoneHVACWaterToAirHeatPump.get
				cooling_coil_array << component.to_ZoneHVACWaterToAirHeatPump.get.coolingCoil
				cooling_coil_area_array << area			
				heating_coil_array << component.to_ZoneHVACWaterToAirHeatPump.get.heatingCoil			
				heating_coil_area_array << area
			# next check for a component Heater	
			elsif not component.to_ZoneHVACUnitHeater.empty?
				heating_coil_array << component.to_ZoneHVACUnitHeater.get.heatingCoil
				heating_coil_area_array << area	
			#check for unitary air-to-air heat pump
			elsif not component.to_AirLoopHVACUnitaryHeatPumpAirToAir.empty?
				cooling_coil_array << component.to_AirLoopHVACUnitaryHeatPumpAirToAir.get.coolingCoil
				cooling_coil_area_array << area	
				heating_coil_array << component.to_AirLoopHVACUnitaryHeatPumpAirToAir.get.heatingCoil
				heating_coil_area_array << area		
			# next check for the cooling and heating coils themselves and save them to the lists
			elsif not component.to_CoilCoolingDXSingleSpeed.empty?   # check for single speed DX coil
				cooling_coil_array << component
				cooling_coil_area_array << area
			elsif not component.to_CoilCoolingDXTwoSpeed.empty?  # check for a two speed DX coil
				cooling_coil_array << component
				cooling_coil_area_array << area		
			elsif not component.to_CoilCoolingWater.empty?  # check for plain cooling water coil
				cooling_coil_array << component
				cooling_coil_area_array << area	
			elsif not component.to_ChillerElectricEIR.empty? # check for electric chiller
				cooling_coil_array << component
				cooling_coil_area_array << area		
			elsif not component.to_CoilHeatingWater.empty? 	# check for plain heating water coil
				heating_coil_array << component
				heating_coil_area_array << area
			elsif not component.to_CoilHeatingElectric.empty?
				heating_coil_array << component
				heating_coil_area_array << area	
			elsif not component.to_CoilHeatingGas.empty?
				heating_coil_array << component
				heating_coil_area_array << area	
			elsif not component.to_BoilerHotWater.empty?
				heating_coil_array << component
				heating_coil_area_array << area	
			elsif not component.to_BoilerSteam.empty?
				heating_coil_array << component
				heating_coil_area_array << area	
			end
		end

		logfile.puts "Found #{cooling_coil_array.size} cooling elements"
		logfile.puts "Found #{heating_coil_array.size} heating elements"

		# go through the cooling coil array list and add up the COP*zone area and zone area to compute zonearea weighted COP
		cop_sum = 0.0
		cop_area_sum = 0.0
		cooling_coil_name_array=Array.new
		(0 .. (cooling_coil_array.size - 1)).each do |i|
			coil = cooling_coil_array[i]
			area = cooling_coil_area_array[i]
			if not coil.to_CoilCoolingDXSingleSpeed.empty?   # check for single speed DX coil		
				cop_sum += coil.to_CoilCoolingDXSingleSpeed.get.ratedCOP.to_f*area
				cop_area_sum += area
			elsif not coil.to_CoilCoolingDXTwoSpeed.empty?	# check for two speed DX coil
				cop_sum += coil.to_CoilCoolingDXTwoSpeed.get.ratedHighSpeedCOP.to_f*area
				cop_area_sum += area
			elsif not coil.to_AirLoopHVACUnitaryHeatPumpAirToAir.empty? # check for heat pump
				cop_sum += coil.to_AirLoopHVACUnitaryHeatPumpAirToAir.get.coolingCoil.to_CoilCoolingDXSingleSpeed.get.ratedCOP.to_f*area
				cop_area_sum += area
			elsif not coil.to_CoilCoolingWater.empty? # check for water cooling coils
				cop_sum += coil.to_CoilCoolingWater.get*area   	#
				cop_area_sum += area
			elsif not coil.to_ChillerElectricEIR.empty?
				cop_sum += coil.to_ChillerElectricEIR.get.referenceCOP*area   	#
				cop_area_sum += area
			
			end
		end

		# if the COP sum is 0 but the cooling coil array is non-zero, check the plant loop for a chiller
		if (cop_sum == 0.0  ) && (cooling_coil_array.size > 0)
			puts "checking plant loop for chiller"
			
			plant_loops.each do |plant_loop|
				supply_components = plant_loop.supplyComponents
				# loop through the components and get the efficiency
				area=building.floorArea
				supply_components.each do |component|
					# if we are here then we don't have a specific area assigned to the cooling unit so use the full building floor area
					if not component.to_CoilCoolingDXSingleSpeed.empty?   # check for single speed DX coil
						cop_array << component.to_CoilCoolingDXSingleSpeed.get.ratedCOP.to_f
						cop_areas << building.floorArea	
					elsif not component.to_CoilCoolingDXTwoSpeed.empty?	# check for two speed DX coil
						cop_array <<  component.to_CoilCoolingDXTwoSpeed.get.ratedHighSpeedCOP.to_f
						cop_areas << area	
					elsif not component.to_to_ChillerElectricEIR.empty?   # check for a chiller
						cop_sum += coil.to_ChillerElectricEIR.get.referenceCOP*area   	#
						cop_area_sum += area			
					end
				end
			end
		end

		if cop_area_sum > 0.0
			cooling_COP = cop_sum / cop_area_sum
			logfile.puts "Area Weighted Average Cooling COP = #{cooling_COP}"
		else
			cooling_COP=1E+12	# set COP to huge number if no cooling so cooling was found
			logfile.puts "No Cooling Equipment Found, set COP = #{cooling_COP}"
		end
		
		# go through the heating coil array list and add up the COP*zone area for gas and electic heating coils
		elec_sum = 0.0
		elec_area_sum = 0.0
		gas_sum = 0.0
		gas_area_sum = 0.0
		(0 .. (heating_coil_array.size - 1)).each do |i|
			coil = heating_coil_array[i]
			area = heating_coil_area_array[i]

			if not coil.to_CoilHeatingGas.empty?   # check for gas heating coil
				gas_sum += coil.to_CoilHeatingGas.get.gasBurnerEfficiency.to_f*area
				gas_area_sum += area
			elsif not coil.to_CoilHeatingElectric.empty?	# check for electric heating coil
				elec_sum += coil.to_CoilHeatingElectric.get.efficiency.to_f*area
				elec_area_sum += area
			elsif not coil.to_CoilHeatingDXSingleSpeed.empty?   # check for single speed DX heating coil
				elec_sum += coil.to_CoilHeatingDXSingleSpeed.get.ratedCOP.to_f*area
				elec_area_sum += area
			elsif not coil.to_AirLoopHVACUnitaryHeatPumpAirToAir.empty? # check for unitary heat pump, extract coil and get 
				elec_sum += coil.to_AirLoopHVACUnitaryHeatPumpAirToAir.get.heatingCoil.to_CoilHeatingDXSingleSpeed.get.ratedCOP.to_f*area	
				elec_area_sum += area	
			elsif not coil.to_BoilerHotWater.empty?
				boiler =coil.to_BoilerHotWater.get
				if boiler.fuelType.to_s =="NaturalGas"			
					gas_sum +=  boiler.nominalThermalEfficiency.to_f*area
					gas_area_sum +=  area					
				else
					elec_sum += boiler.nominalThermalEfficiency.to_f*area
					elec_area_sum += area
				end
			end # elsif not coil	
		end	

		# if there seems to be no heating energy used but there are heating elements detected, check the plant loop for a boiler
		if ((gas_sum + elec_sum) == 0.0  ) && (heating_coil_array.size > 0)
			plant_loops.each do |plant_loop|
				supply_components = plant_loop.supplyComponents
				# loop through the components and get the efficiency
				area=building.floorArea
				supply_components.each do |component|
					if not component.to_BoilerHotWater.empty?   # check for a hot water boiler
						boiler = component.to_BoilerHotWater.get
						if boiler.fuelType.to_s =="NaturalGas"			
							gas_sum +=  boiler.nominalThermalEfficiency.to_f*area
							gas_area_sum += area		
						else
							elec_sum += boiler.nominalThermalEfficiency.to_f*area
							elec_area_sum += area	
						end
					end
				end
			end
		end 

		if gas_sum > elec_sum
			heating_fuel_type = 2  # set heating fuel type to gas if heating_gas_area is larger than electric area
			logfile.puts "Fuel Type Determined to be Mostly or All Gas, HVAC Fuel Type = 2"
		else
			logfile.puts "Fuel Type Determined to be Mostly or All Electric, HVAC Fuel Type = 1"
			heating_fuel_type = 1
		end

		if (gas_sum + elec_sum) > 0.0
			heating_system_efficiency = (elec_sum + gas_sum)/(elec_area_sum + gas_area_sum)
			logfile.puts "Average Heating Efficiency = #{heating_system_efficiency}"
		else
			logfile.puts "No heating elements found - setting Average Heating Efficiency = 0.8 and Heating Fuel Type = 2"
			heating_system_efficiency = 0.8
			heating_fuel_type = 2
		end		
	end
	
	# set up the HVAC loss tables
	# hvac type 		0		1		2		3		4		5		6		7		8		9
	#					10		11		12		13		14		15		16		17		18		19
	#					20		21		22		23		24		25		26		27		28		29
	#					30		31		32		33		34		35		36		37		38		39
	hvac_waste_table =[	0.00,	0.00, 	0.00, 	0.00, 	0.00, 	0.03, 	0.00,	0.00, 	0.04, 	0.00,  	
						0.03, 	0.00,	0.00, 	0.08, 	0.03, 	0.00, 	0.00, 	0.03, 	0.00, 	0.00,
						0.04,  	0.00, 	0.03, 	0.00, 	0.00,	0.04, 	0.00, 	0.00, 	0.08, 	0.04, 	
						0.00,	0.00,	0.04,	0.00,	0.04,	0.00,	0.00]
						
	hvac_hot_table =[	0.00,	0.08, 	0.25, 	0.00, 	0.36, 	0.08,	0.08, 	0.25, 	0.00, 	0.36, 	
						0.08, 	0.08,	0.25, 	0.00, 	0.08, 	0.08, 	0.25, 	0.08, 	0.08, 	0.25,
						0.00,  	0.36, 	0.08, 	0.08, 	0.25,	0.08, 	0.00, 	0.00, 	0.00, 	0.08, 	
						0.00,	0.00,	0.08,	0.00,	0.08,	0.00,	0.00]

	hvac_cold_table =[	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.01,	0.00, 	0.00, 	0.00, 	0.00, 	
						0.01, 	0.00,	0.00, 	0.01, 	0.01, 	0.00, 	0.00, 	0.01, 	0.00, 	0.00,
						0.00,  	0.00, 	0.01, 	0.00, 	0.00,	0.01, 	0.00, 	0.00, 	0.01, 	0.01, 
						0.00,	0.00,	0.01,	0.00,	0.01,	0.00,	0.00]
						
	#extract losses from loss tables based on hvac type
	hvac_waste_factor=hvac_waste_table[hvac_type]
	hvac_heating_loss_factor = hvac_hot_table[hvac_type]
	hvac_cooling_loss_factor=hvac_cold_table[hvac_type]

	logfile.printf("HVAC Waste Factor = %1.2f \n",hvac_waste_factor)
	logfile.printf("HVAC Heating Loss Factor = %1.2f \n",hvac_heating_loss_factor)
	logfile.printf("HVAC Cooling Loss  Factor = %1.2f \n",hvac_cooling_loss_factor)
	
	# print out efficiencies to logfile
	logfile.puts "Cooling System COP = #{cooling_COP}"
	logfile.puts "Cooling System IPLV/COP = #{cooling_IPLVToCop_ratio}"
	logfile.puts "Heating Fuel Type = #{heating_fuel_type}"  # heating energy, 1=electric, 0=gas
	logfile.puts "Heating System Efficiency = #{heating_system_efficiency}"

	# calculate fresh air ventilation rates
	
	if not ventilation_extract_choice
		# if we have not checked the extract from OSM box, set from user inputs
		freshair_flow_rate = freshair_flow_rate_choice*number_of_people
	else
		freshair_flow_rate = 0.0
		spaces.each do |s|
			
			if not s.designSpecificationOutdoorAir.empty?
				outdoor_air = s.designSpecificationOutdoorAir.get
			
				# these methods will return 0.0 if nothing is defined
				air_ach = outdoor_air.outdoorAirFlowAirChangesperHour* s.volume / 3.6 	 		# get ACH and convert to L/s
				air_rate = outdoor_air.outdoorAirFlowRate * 1000								# get air flow rate in L/s
				air_per_person = outdoor_air.outdoorAirFlowperPerson * s.numberOfPeople * 1000 	# get in m3/s/person and convert to L/s
				air_per_area = outdoor_air.outdoorAirFlowperFloorArea * s.floorArea * 1000 		# get in m3/s/m2 and convert to L/s

				if not outdoor_air.outdoorAirFlowRateFractionSchedule.empty?
					sched = outdoor_air.outdoorAirFlowRateFractionSchedule.get.name.to_s
					sched_mult=occ_aves[sched]*frac_year_occ + unocc_aves[sched]*frac_year_unocc
				else
					sched_mult=1.0	# if no schedule is defined set multiplier to 1.0
				end

				if outdoor_air.outdoorAirMethod == "Maximum" 
					space_air_rate = [air_per_person, air_per_area, air_rate, air_ach].max * s.multiplier * sched_mult
				else 	# if maximum is not selected, assume sum is selected
					space_air_rate = (air_per_person + air_per_area + air_rate + air_ach) * s.multiplier * sched_mult
				end
				
			else  # if no outdoor air rate is defined for space, default to 20 cfm/nominal person and convert to L/S from cfm
				logfile.puts " No outdoor air rate define for space #{s.name} , default of 20cm/nominal person used"
				space_air_rate = 20 * s.numberOfPeople/2.12
			end
			freshair_flow_rate += space_air_rate
		end
	end
	supply_exhaust_rate = freshair_flow_rate	# set exhaust = supply rate as default (i.e. no pressurization)

	logfile.printf("Total ventilation fresh air flow rate = %1.2f L/s\n", freshair_flow_rate)
	logfile.puts "No exhaust rate extraction - set air exhaust rate to ventilation rate" 

	# use DHW distribution type to set DHW distribution efficiency
	dhw_dist_eff_table=[1.0,0.8,0.6]  # create lookup table
	dhw_distribution_efficiency = dhw_dist_eff_table[dhw_dist_type-1]
	
	if not dhw_extract_choice
		dhw_system_efficiency = dhw_system_eff_choice
		dhw_fuel_type = dhw_fuel_choice
		# demand in L/person/day * number of people * number of days / 1000 L/m3 to get m3/yr
		dhw_demand = dhw_demand_choice*number_of_people*number_days_occupied_per_year/1000
	else
		# Try to find DHW water components and get the water use
		# search plant loops for water heaters
		dhw_gas_array=Array.new
		dhw_elec_array=Array.new
		plant_loops.each do |plant_loop|
			supply_components = plant_loop.supplyComponents
			# loop through the components and find the components that are water heaters
			supply_components.each do |component|
				if not component.to_WaterHeaterMixed.empty?   # enter here if the item is a water heater
					heater = component.to_WaterHeaterMixed.get			# convert component to water heater type
					if heater.heaterFuelType.to_s =="NaturalGas"		# check to see if the heater is gas or electric	
						dhw_gas_array << heater.heaterThermalEfficiency.to_f	# store the efficiency
					else
						dhw_elec_array << heater.heaterThermalEfficiency.to_f	# store the efficiency
					end
				end
			end
		end

		# add up all the uses from the individual components
		dhw_gas_sum=0.0
		dhw_gas_array.each do |gas|
			dhw_gas_sum += gas	
		end
		dhw_elec_sum=0.0
		dhw_elec_array.each do |elec|
			dhw_elec_sum += elec
		end

		# find the average efficiency of water heaters
		n_dhw_units = dhw_gas_array.size + dhw_elec_array.size
		if n_dhw_units > 0  # if the number of water heaters is > 0
			dhw_system_efficiency = (dhw_gas_sum + dhw_elec_sum)/(dhw_gas_array.size + dhw_elec_array.size)
		else
			dhw_system_efficiency = 1.0
		end

		if dhw_gas_sum > dhw_elec_sum   # if there are more gas DHW water heaters, fuel type is gas
			dhw_fuel_type = 2
		else
			dhw_fuel_type = 1
		end

		# extract the water usage by starting with the water use connections and then finding equipment
		water_connects = model.getWaterUseConnectionss
		dhw_demand = 0.0
		water_connects.each do |connect|
			equipments = connect.waterUseEquipment
			equipments.each do |equip|
				if not equip.flowRateFractionSchedule.empty?
					frac_sched = equip.flowRateFractionSchedule.get.name.to_s
					# get multiplier by doing a weighted average of occupied and unoccupied times
					mult = occ_aves[frac_sched]*frac_year_occ + unocc_aves[frac_sched]*frac_year_unocc 
				else
					mult = 1.0
				end
				peak = equip.waterUseEquipmentDefinition.peakFlowRate*3600*8760  # get peak value and convert from m3/s to m3/yr
				logfile.printf("DHW: %s, peak use = %1.0f m3/yr, mult = %0.2f \n",frac_sched, peak, mult);
				dhw_demand += peak * mult
			end
		end

		if (dhw_gas_sum + dhw_elec_sum ) == 0.0	# if the gas + electric sum = 0 then there were no DHW heaters, so set demand to 0 no matter what
			dhw_demand = 0.0
		end

	end
		
	# print out the DHW fuel type, system efficiency, distribution efficiency
	logfile.printf("DHW: Fuel Type = %i, System Efficiency = %0.2f Distribution Efficiency = %0.2f, Demand = %1.2f m3/yr \n", 
		dhw_fuel_type,  dhw_system_efficiency, dhw_distribution_efficiency,dhw_demand)

	time=Time.new # get the current system clock
		
	puts "Writing Output to #{osm2iso_output_file}"
	# create output file and start writing out 
	
	
	
	outfile = File.new(osm2iso_output_file, 'w+')
	
	outfile.puts "##{osm2iso_output_file} generated by osm2iso measure on #{input_file} on #{time.ctime}"
	outfile.printf("weatherFilePath = %s\n",weather_file_path)
		outfile.puts "\# Terrain class urban/city = 0.9, suburban/some shielding = 0.9, country/open = 1.0"
	outfile.printf("terrainClass = %3.1f\n",terrain_class)
	
		outfile.puts "\# building height is in m,  floor area is in m^2, people density is m2/person"
	outfile.printf("buildingHeight = %3.1f\n",building_height)
	outfile.printf("floorArea = %3.1f\n",floor_area)
	outfile.printf("buildingOccupancyFrom = %d\n",occupancy_day_start)
	outfile.printf("buildingOccupancyTo = %d\n",occupancy_day_end)
	outfile.printf("equivFullLoadOccupancyFrom = %d\n",occupancy_hour_start)
	outfile.printf("equivFullLoadOccupancyTo = %d\n",occupancy_hour_end)
	outfile.printf("peopleDensityOccupied = %3.2f\n",area_per_person_occupied)
	outfile.printf("peopleDensityUnoccupied = %3.2f\n",area_per_person_unoccupied)
	
		outfile.puts "\# LPD, EPD, GPD all in W/m2, ext light power in W, and heatgainperperson is W/person"
	outfile.printf("lightingPowerIntensityOccupied = %3.2f\n",lpd_occupied)
	outfile.printf("lightingPowerIntensityUnoccupied = %3.2f\n",lpd_unoccupied)
	outfile.printf("elecPowerAppliancesOccupied = %3.2f\n",epd_occupied)
	outfile.printf("elecPowerAppliancesUnoccupied = %3.2f\n",epd_unoccupied)
	outfile.printf("gasPowerAppliancesOccupied = %3.2f\n",gpd_occupied)
	outfile.printf("gasPowerAppliancesUnoccupied = %3.2f\n",gpd_unoccupied)
	outfile.printf("exteriorLightingPower = %3.2f\n",exterior_lighting_power)
	outfile.printf("heatGainPerPerson = %3.2f\n",activity_level_occ)
	# when we implement occupied and unoccupied heat gain per person, we comment above and uncomment below
	#outfile.printf(""heatGainPerPersonOccupied = %3.2f\n",activity_level_occ)
	#outfile.printf(""heatGainPerPersonUnoccupied = %3.2f\n",activity_level_unocc)
		outfile.puts "\# Temp set points in degrees C"
	outfile.printf("heatingOccupiedSetpoint = %3.1f\n",heating_setpoint_occ)
	outfile.printf("heatingUnoccupiedSetpoint = %3.1f\n",heating_setpoint_unocc)
	outfile.printf("coolingOccupiedSetpoint = %3.1f\n",cooling_setpoint_occ)
	outfile.printf("coolingUnoccupiedSetpoint = %3.1f\n",cooling_setpoint_unocc)
		outfile.puts "\# HVAC waste, heating and cooling loss factors set by HVAC type from EN 15243"
	outfile.printf("hvacWasteFactor = %3.3f\n",hvac_waste_factor)
	outfile.printf("hvacHeatingLossFactor = %3.3f\n",hvac_heating_loss_factor)
	outfile.printf("hvacCoolingLossFactor = %3.3f\n",hvac_cooling_loss_factor)
		outfile.printf( "\# daylight sensors, occupancy sensors, illum control are set to 1 if there is no control.   See iso 15193 Annex F/G for values" )
	outfile.printf("daylightSensorSystem = %d\n",daylight_sensors)
	outfile.printf("lightingOccupancySensorSystem = %d\n",occupancy_sensors)
	outfile.printf("constantIlluminationControl = %d\n",const_illum_ctrl)
	
		outfile.puts "\# COP is W/W, coolingSystemIPLV is the ratio of IPLV/COP"
	outfile.printf("coolingSystemCOP = %3.1f\n",cooling_COP)
	outfile.printf("coolingSystemIPLVToCopRatio = %3.1f\n",cooling_IPLVToCop_ratio)
	
		outfile.puts "\# energycarrier is 1 if electric, 2 if gas"
	outfile.printf("heatingEnergyCarrier = %d\n",heating_fuel_type) # heating energy, 1=electric, 0=gas
	outfile.printf("heatingSystemEfficiency = %3.1f\n",heating_system_efficiency)
		outfile.puts "\# vent type is 1 if mech, 2 if natural, 3 if mixed"
	outfile.printf("ventilationType = %3.1f\n",ventilation_type)
		outfile.puts "\# ventilation flow rates in L/s"
	outfile.printf("freshAirFlowRate = %3.1f\n",freshair_flow_rate)
	outfile.printf("supplyExhaustRate = %3.1f\n",supply_exhaust_rate)
		outfile.puts "\# heatRecover = efficiency of heat recovery (0 for none), exhaustAirRecir = fraction of supply air recirculated"
	outfile.printf("heatRecovery = %3.1f\n",heat_recovery_fraction)  # heat recovery efficienc, 0 for no heat recovery
	outfile.printf("exhaustAirRecirculation = %3.1f\n",exhaust_recirculation_fraction)  # fraction of supply air that is recirculated
		outfile.puts "\# DHW demand in m3/yr.  Use 10 m3/yr per person as a default for offices"
	outfile.printf("dhwDemand = %3.1f\n",dhw_demand) #
	
	# when we implement occupied and unoccupied infiltration, we comment above and uncomment below
	#outfile.puts "dhwDemandOccupied=%3.1f\n",dhw_demand_occ)
	#outfile.puts "dhwDemandUnoccupied=%3.1f\n",dhw_demand_unocc)
		outfile.puts "\# dhwDistribution efficiency all taps w/i 3m = 1, taps more than 3m = 0.8, circulation or unknown = 0.6, see NEN 2916 12.6, "
	outfile.printf("dhwSystemEfficiency = %3.1f\n",dhw_system_efficiency)
	outfile.printf("dhwDistributionEfficiency = %3.1f\n",dhw_distribution_efficiency)
	outfile.printf("dhwEnergyCarrier = %d\n",dhw_fuel_type)
		outfile.puts "\# Surface Heat capacity in J/K/m2"
	outfile.printf("interiorHeatCapacity = %5.0f\n",interior_heat_capacity)
	outfile.printf("exteriorHeatCapacity = %5.0f\n",exterior_heat_capacity)
		outfile.puts "\# BEM type 1= none, 2 = simple, 3=advanced with FDD"
	outfile.printf("bemType = %d\n",bem_type)
		outfile.puts "\# specific fan power in L/s/W,  flow control factor is energy reduction from fan control measures"
	outfile.printf( "specificFanPower = %3.2f\n",specific_fan_power)
		outfile.puts "\# fan flow control  1= no control, 0.75 = inlet blade adjuct, 0.65= variable speed  see NEN 2916 7.3.3.4  "
	outfile.printf("fanFlowcontrolFactor = %3.2f\n",fan_flow_control_factor)
		outfile.puts "\# infiltration in m3/m2/hr based on surface area"
	outfile.printf("infiltration = %3.2f\n",infiltration_rate) # leakage per unit surface area in m^3/m^2/hr
	# when we implement occupied and unoccupied infiltration, we comment above and uncomment below
	#outfile.puts "infiltrationOccupied=#{infiltration_rate_occ)
	#outfile.puts "infiltrationUnoccupied=#{infiltration_rate_unocc)
		outfile.puts "\# pump control 0= no pump, 0.5 = auto pump controls for more 50% of pumps, 1.0 = all other cases.   See NEN 2914 9.4.3"
	outfile.printf( "heatingPumpControl=%3.2f\n",heating_pump_control_factor)
	outfile.printf( "coolingPumpControl=%3.2f\n",cooling_pump_control_factor)

		outfile.puts "\# Areas in m2, U values in W/m2/K"
		outfile.puts "\# SCF = solar control factor (external control), SDF = shading device factor = internal shading"
	# write out roof and skylight info
	outfile.printf( "roofArea = %2.1f\n",roof_area)
	outfile.printf( "roofUValue = %4.4f\n",roof_U)
	outfile.printf( "roofSolarAbsorption = %4.4f\n",roof_solar_absorption)
	outfile.printf( "roofThermalEmissivity= %4.4f\n",roof_thermal_emissivity)

	outfile.printf( "skylightArea = %2.1f\n",skylight_area)
	outfile.printf( "skylightUValue = %4.4f\n",skylight_U)
	outfile.printf( "skylightSHGC = %4.4f\n",glazing_SHGC_choice)

	# write out the wall and window info
	for i in 0 .. 7
		outfile.printf("WallArea%s = %3.1f\n",direction_names[i],wall_areas[i])
		outfile.printf("WallUvalue%s = %4.4f\n",direction_names[i], wall_U[i])
		outfile.printf("WallSolarAbsorption%s = %4.4f\n",direction_names[i],wall_solar_absorption[i])
		outfile.printf("WallThermalEmissivity%s = %4.4f\n",direction_names[i],wall_thermal_emissivity[i])
	end
	for i in 0 .. 7
		outfile.printf("WindowArea%s = %3.1f\n",direction_names[i],window_areas[i])
		outfile.printf("WindowUvalue%s = %4.4f\n",direction_names[i],window_U[i])
		outfile.printf("WindowSHGC%s = %4.4f\n",direction_names[i],window_SHGC[i])
		outfile.printf("WindowSCF%s = %4.4f\n",direction_names[i],window_SCF[i])
		outfile.printf("WindowSDF%s= %4.4f\n",direction_names[i],window_SDF[i])		
	end
	outfile.close
	
	logfile.puts "ISM file generation complete, #{osm2iso_output_file} has been saved to disk"
	logfile.close
	puts "ISM file generation complete, #{osm2iso_output_file} has been saved to disk"
	puts
	puts
	
	# flush the output buffer
	$stdout.flush

	
	sleep 1 # wait for file buffers to really close out
	
		
	# The following code loads in the generated .ism file and runs the C++ implementation within OpenStudio
	
	# use the isom output file as input file for ISO model run
	runiso_input_file = osm2iso_output_file
	
	# print out the name of this script with $0
	puts "Running isomodel calculation on input file  #{runiso_input_file}"

	basename = File.basename(runiso_input_file,".*")	# get the base file without the path to it or extensions at the end
	dirname = File.dirname(runiso_input_file)	# get the path to the file without the file itself
	#create a new log file for the runiso part of the measure
	runiso_log_file = File.join(dirname, basename + ".runiso.log")

	# total kWh and kBtu output file is base filename + .runiso + .csv
	runiso_output_file = File.join(dirname, basename + ".runiso.csv")
	
	# eui in kWh/m2 only output file is base filename + .eui + .csv
	eui_output_file = File.join(dirname, basename + ".eui.csv")
	
	puts "Writing processing log to #{runiso_log_file}"	
	puts "Writing output to #{runiso_output_file} and #{eui_output_file}"

	
	time=Time.new  # get the current clock time for adding time to logfile
	# create log file and start writing out 
	logfile = File.new(runiso_log_file, 'w+') 
	logfile.puts "Starting Log File #{runiso_log_file} at #{time.ctime}"


	logfile.puts "Input File: #{runiso_input_file}"
	logfile.puts "Output Files: #{runiso_output_file} and #{eui_output_file}"

	puts "...Loading ISO Model"

	logfile.puts "*****************************"
	logfile.puts "  Loading ISOmodel"
	logfile.puts "*****************************"


	puts "...Creating ISOmodel object"
	logfile.puts "Creating ISOmodel object"
	

	
	userModel = OpenStudio::ISOModel::UserModel.new


	#sleep 1  	# pause 1 second to let things settle before we try to load in the .ism file
	
	puts "...Loading ISOmodel from file #{runiso_input_file}"
	logfile.puts "Loading ISOmodel from file #{runiso_input_file}"


	userModel.load(runiso_input_file)
	
	#sleep 1 # pause for a second to make sure system has settled
	
	puts "Weather file path = #{userModel.weatherFilePath}"
	logfile.puts "Weather file path = #{userModel.weatherFilePath}"
	
	
	$stdout.flush   # flush the buffer in case we crash when running
	logfile.flush 	# flush the buffer in case we crash when running
	
	puts
	puts "...Running ISOmodel Simulation"

	logfile.puts "*****************************"
	logfile.puts "  Running ISOmodel Simulation"
	logfile.puts "*****************************"



	t1 = Time.now
	simisoModel= userModel.toSimModel
	simResults = simisoModel.simulate
	t2 = Time.now
	elapsed = ((t2 - t1)*1000).to_i

	puts "Elapsed time = #{elapsed} milliseconds"
	logfile.puts "Elapsed time = #{elapsed} milliseconds"

	
	$stdout.flush   # flush the buffer in case we crash when running
	logfile.flush 	# flush the buffer in case we crash when running
	
	
	puts
	puts "...Saving ISOmodel Simulation Results"

	logfile.puts "*****************************"
	logfile.puts "  Saving ISOmodel Simulation Results"
	logfile.puts "*****************************"

	puts 

	 elec_enduse=Array.new(14) {Array.new(12,0.0)}
	 gas_enduse=Array.new(14) {Array.new(12,0.0)}
	(0..13).each do |cat|
		(0..11).each do |month|
			elec_enduse[cat][month] = simResults.monthlyResults[month].getEndUse(OpenStudio::EndUses.fuelTypes[0], OpenStudio::EndUses.categories[cat])
			gas_enduse[cat][month] = simResults.monthlyResults[month].getEndUse(OpenStudio::EndUses.fuelTypes[1], OpenStudio::EndUses.categories[cat])
		end	
	end
	  
	  
	floorArea=userModel.floorArea
	months = ['Jan','Feb','Mar','Apr','May','Jun','Jul','Aug','Sep','Oct','Nov','Dec'] 

	# openstudio::OPENSTUDIO_ENUM(EndUseFuelType ,((Electricity)(Electricity))((Gas)(Natural Gas))((OtherFuel)(Other Fuel))((DistrictCooling)(District Cooling))((DistrictHeating)(District Heating))((Water)(Water)) 	 )	
	#openstudio::OPENSTUDIO_ENUM(EndUseCategoryType ,((Heating)(Heating))((Cooling)(Cooling))((InteriorLights)(Interior Lighting))((ExteriorLights)(Exterior Lighting))((InteriorEquipment)(Interior Equipment))((ExteriorEquipment)(Exterior Equipment))((Fans)(Fans))((Pumps)(Pumps))((HeatRejection)(Heat Rejection))((Humidifier)(Humidification))((HeatRecovery)(Heat Recovery))((WaterSystems)(Water Systems))((Refrigeration)(Refrigeration))((Generators)(Generators)) 	 )	
	 puts "Monthly Energy Use By Category in kWh/m2"
	 puts "Month \tEHeat \tECool \tEIntLt \tEExtLt \tEInEqp \tEFan \tEPump \tEDHW \tGHeat \tGInEqp \tGDHW" 
	 
	 logfile.puts "Monthly Energy Use By Category in kWh/m2"
	 logfile.puts "Month \tEHeat \tECool \tEIntLt \tEExtLt \tEInEqp \tEFan \tEPump \tEDHW \tGHeat  \tGInEqp \tGDHW" 
	 
	 totaluse=Array.new(13,0.0)
	
	 
	 (0..11).each do |month|
		print months[month]
		logfile.print months[month], "\t"

		# electric heating
		use=elec_enduse[0][month]
		totaluse[0] +=use
		printf("\t%3.2f",use)
		logfile.printf("\t%3.2f",use)
		
		# electric cooling
		use=elec_enduse[1][month]
		totaluse[1] +=use
		printf("\t%3.2f",use)
		logfile.printf("\t%3.2f",use)
		
		# electric interior lighting
		use=elec_enduse[2][month]
		totaluse[2] +=use
		printf("\t%3.2f",use)
		logfile.printf("\t%3.2f",use)
		
		# electric exterior lighting
		use=elec_enduse[3][month]
		totaluse[3] +=use
		printf("\t%3.2f",use)
		logfile.printf("\t%3.2f",use)
		
		#electric interior equipment
		use=elec_enduse[4][month]
		totaluse[4] +=use
		printf("\t%3.2f",use)
		logfile.printf("\t%3.2f",use)
		
		#electric fan
		use=elec_enduse[6][month]
		totaluse[5] +=use
		printf("\t%3.2f",use)
		logfile.printf("\t%3.2f",use)

		#electric pump
		use=elec_enduse[7][month]
		totaluse[6] +=use
		printf("\t%3.2f",use)
		logfile.printf("\t%3.2f",use)
		
		#electric DHW
		use=elec_enduse[11][month]
		totaluse[7] +=use
		printf("\t%3.2f",use)
		logfile.printf("\t%3.2f",use)
		
		#gas heat
		use=gas_enduse[0][month]	
		totaluse[8] +=use
		printf("\t%3.2f",use)
		logfile.printf("\t%3.2f",use)
		

		#gas Int Equip
		use=gas_enduse[4][month]	
		totaluse[9] +=use
		printf("\t%3.2f",use)
		logfile.printf("\t%3.2f",use)
		

		# gas DHW
		use=gas_enduse[11][month]	
		totaluse[10] +=use
		printf("\t%3.2f",use)
		logfile.printf("\t%3.2f",use)
		
		print "\n"
		logfile.print "\n"
	end

	(0..11).each do |i|
		printf("----\t")
		logfile.printf("----\t")
	end
	print "\n"
	logfile.print "\n"

	print "Total\t"
	logfile.print "Total\t"


	(0..10).each do |i|
		printf("%3.2f\t",totaluse[i])
		logfile.printf("%3.2f\t",totaluse[i])
	end

	print "\n\n"
	logfile.print "\n\n"

	electotal = 0.0
	(0..7).each do |i|
	electotal += totaluse[i]
	end

	gastotal = 0.0
	(8..10).each do |i|
	gastotal += totaluse[i]
	end

	total=electotal+gastotal

	printf("Elec Total: %4.2f kWh/m2, %4.2f kBtu/ft2, %4.2f kWh, %4.2f kBtu \n",electotal,electotal*0.3171,electotal*floorArea,electotal*floorArea*3.412)
	printf("Gas Total: %4.2f kWh/m2, %4.2f kBtu/ft2, %4.2f kWh, %4.2f kBtu\n",gastotal,gastotal*0.3171,gastotal*floorArea,gastotal*floorArea*3.412)
	printf("All Total: %4.2f kWh/m2, %4.2f kBtu/ft2, %4.2f kWh, %4.2f kBtu\n",total,total*0.3171,total*floorArea,total*floorArea*3.412)

	logfile.printf("Elec Total: %4.2f kWh/m2, %4.2f kBtu/ft2, %4.2f kWh, %4.2f kBtu \n",electotal,electotal*0.3171,electotal*floorArea,electotal*floorArea*3.412)
	logfile.printf("Gas Total: %4.2f kWh/m2, %4.2f kBtu/ft2, %4.2f kWh, %4.2f kBtu\n",gastotal,gastotal*0.3171,gastotal*floorArea,gastotal*floorArea*3.412)
	logfile.printf("All Total: %4.2f kWh/m2, %4.2f kBtu/ft2, %4.2f kWh, %4.2f kBtu\n",total,total*0.3171,total*floorArea,total*floorArea*3.412)
	
	# find totals by end use category and monthly totals
	categories=["Heating","Cooling","Interior Lighting","Exterior Lighting","Interior Equipment","Exterior Equipment", \
		"Fans","Pumps","Heat Rejection","Humidification","Heat Recovery","Water Systems","Refrigeration","Generators"]

	elec_cat_total=Array.new(12,0.0)
	gas_cat_total=Array.new(12,0.0)

	elec_month_total = Array.new(14,0.0)
	gas_month_total = Array.new(14,0.0)

	elec_total = 0.0
	gas_total = 0.0
	
	# find the category totals for each month
	(0..11).each do |month|
		
		(0..13).each do |cat|
			elec_cat_total[month] += elec_enduse[cat][month]
			gas_cat_total[month] += gas_enduse[cat][month]
		end
		elec_total += elec_cat_total[month]
		gas_total += gas_cat_total[month]
	end

	# find the monthl totals for each category
	(0..13).each do |cat|
		(0..11).each do |month|
			elec_month_total[cat]+= elec_enduse[cat][month]
			gas_month_total[cat]+= gas_enduse[cat][month]
		end
	end

	$stdout.flush   # flush the buffer in case we crash when running
	logfile.flush 	# flush the buffer in case we crash when running
	
	# save output file to CSV format
	# open the output file
	outfile = File.new(runiso_output_file, 'w+')
	puts "...Writing output to #{runiso_output_file}"
	logfile.puts "Writing output to #{runiso_output_file}"
	
	# generate the electricity consumption table in kWh
	outfile.puts "Electricity Consumption (kWh)"
	outfile.puts "Category , Jan, Feb, Mar, Apr , May, Jun, Jul, Aug, Sep, Oct, Nov, Dec, Total"

	(0..13).each do |cat|
		outfile.print categories[cat],", "
		(0..11).each do |month|
			outfile.printf("%1.5g, ",elec_enduse[cat][month]*floorArea)
		end
		outfile.printf("%1.5g \n",elec_month_total[cat]*floorArea)
	end

	outfile.print "Total,"
	(0..11).each do |month|
		outfile.printf("%1.5g, ", elec_cat_total[month]*floorArea)
	end
	outfile.printf("%1.5g \n", elec_total)

	outfile.print"\n"


	# generate the gas consumption table in MMBtu (millions of Btu)
	outfile.puts "Gas Consumption (kWh)"
	outfile.puts "Category , Jan, Feb, Mar, Apr , May, Jun, Jul, Aug, Sep, Oct, Nov, Dec, Total"
	kWh2MMBTU = 3.412/1000
	(0..13).each do |cat|
		outfile.print categories[cat],", "
		(0..11).each do |month|
			outfile.printf("%4.3g, ",gas_enduse[cat][month]*kWh2MMBTU*floorArea)
		end
		outfile.printf("%4.3g \n",gas_month_total[cat]*kWh2MMBTU*floorArea)
	end

	outfile.print "Total,"
	(0..11).each do |month|
		outfile.printf("%4.3g, ", gas_cat_total[month]*kWh2MMBTU*floorArea)
	end
	outfile.printf("%4.3g \n", gas_total*kWh2MMBTU*floorArea)
	outfile.close
	
	logfile.puts "Output CSV generation is complete,  #{runiso_output_file} has been saved to disk"
	puts "Output CSV generation is complete,  #{runiso_output_file} has been saved to disk"
	
	
	# open EUIfile
	euifile = File.new(eui_output_file, 'w')

	# generate the electricity consumption table in kWh
	euifile.puts "Electricity EUI (kWh/m2)"
	euifile.puts "Category , Jan, Feb, Mar, Apr , May, Jun, Jul, Aug, Sep, Oct, Nov, Dec, Total"

	(0..13).each do |cat|
		euifile.print categories[cat],", "
		(0..11).each do |month|
			euifile.printf("%1.5g, ",elec_enduse[cat][month])
		end
		euifile.printf("%1.5g \n",elec_month_total[cat])
	end

	euifile.print "Total,"
	(0..11).each do |month|
		euifile.printf("%1.5g, ", elec_cat_total[month])
	end
	euifile.printf("%1.5g \n", elec_total)
	euifile.print"\n"


	# generate the gas consumption table in MMBtu (millions of Btu)
	euifile.puts "Gas EUI (kWh/m2)"
	euifile.puts "Category , Jan, Feb, Mar, Apr , May, Jun, Jul, Aug, Sep, Oct, Nov, Dec, Total"
	(0..13).each do |cat|
		euifile.print categories[cat],", "
		(0..11).each do |month|
			euifile.printf("%4.3g, ",gas_enduse[cat][month])
		end
		euifile.printf("%4.3g \n",gas_month_total[cat])
	end

	euifile.print "Total,"
	(0..11).each do |month|
		euifile.printf("%4.3g, ", gas_cat_total[month])
	end
	euifile.printf("%4.3g \n", gas_total)
	
	
	euifile.puts 
	euifile.puts "Total EUIs (kWh/m2)"
	euifile.puts "Month , Jan, Feb, Mar, Apr , May, Jun, Jul, Aug, Sep, Oct, Nov, Dec, Total"
	euifile.print "Total,"
	(0..11).each do |month|
		euifile.printf("%4.3g, ", elec_cat_total[month]+gas_cat_total[month])
	end
	euifile.printf("%4.3g \n", elec_total+gas_total)
	
	
	
	
	euifile.close
	
	logfile.puts "EUI CSV generation is complete,  #{eui_output_file} has been saved to disk"
	puts "EUI CSV generation is complete,  #{eui_output_file} has been saved to disk"




	logfile.puts "Closing logfile"
	logfile.close

	runner.registerFinalCondition("The building was simulated using the ISO Model and written to #{runiso_output_file}")
	
	return false   # use this to stop further processing for now by issuing an error
	#return true
  end #end the run method
end #end the measure

#this allows the measure to be use by the application
OSM2ISMV01.new.registerWithApplication