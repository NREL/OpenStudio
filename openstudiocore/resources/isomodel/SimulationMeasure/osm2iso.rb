
######################################################################
# == Synopsis 
#
#   Load an osm file, convert to an ISO file
# do this externally from 
#
# == Usage
#
#   ruby osm2iso.rb ARGV[0] 
# 
#   ARGV[0] - path to OSM file for conversion
#
# == Examples (replace input.osm with your input file/path
#
#   ruby osm2iso.rb input.osm
#
#V 0.84 15-Oct-2013 RTM 
# changed variable name for IPLV to IPLVToCopRatio to reflect code changes

#V 0.83 11-Oct-2013 RTM
# added code to add comments to the generated .ism file

#V 0.82 06-Oct-2013 RTM
# made some minor output changes to reflect changes made for creating an OpenStudio Measure

#V 0.81 04-Oct-2013 RTM
# changed output and log file names


#V 0.80 20-Sep-2013 RTM
# modifying the HVAC parsing to sure we get packaged equipment analyzed properly
# correct LPD assignment to subtract unoccupied LPD from occupied because unoccupied is added within isomodel to account for
# parasitic lighting

# V 0.73 06-Aug-2013 Ralph T Muehleisen
# added in COP calculation
# added heating efficiency calculation
# added in DHW summation

# V 0.72 02-Aug-2013 Ralph T Muehleisen
# added wall/roof U value, absorption, emissivity and heat capacity extraction
# set interior heat capacity to 100000 J/K/m2 as that's typical for a concrete slab

# V 0.71 02-Aug-2013 Ralph T Muehleisen
# corrected/completed infiltration extraction
# added log file creation
# changed default U/SHGC values

# V 0.7 31-Jul-2013 Ralph T Muehleisen
# added outdoor ventilation air extraction
#
# V 0.6 28-Jul-2013 Ralph T Muehleisen
#	Added infiltration extraction
#	Added terrain class extraction
# 	Updated DHW and Outside air default calculations to be fixed rates per person

# V 0.5 26-Jul-2013 Ralph T Muehleisen
#	Added schedule search and computation of occupancy/unoccupancy fractions
# 	Added thermostat extraction

require 'openstudio'

weather_file_path="c:\\epw\\ord.epw"

if ARGV.length < 1
	puts "Usage: ruby osm2iso.rb 'C:\\path\\to\\model.osm'"
	exit(-1)
 end

 # use this as a very small number to add to denominators to avoid divide by zero
epsilon = 0.00000000000001


input_file = ARGV[0]
# strip off the OSM and add an ISO to the filename to get the output file
osm2iso_log_file = input_file.chomp(".osm") + ".osm2iso.log"
# strip off the OSM and add an ISO to the filename to get the output file
osm2iso_output_file = input_file.chomp(".osm") + ".ISO"



basename = File.basename(input_file,".*")	# get the base file without the path to it or extensions at the end
dirname = File.dirname(input_file)	# get the path to the file without the file itself
osm2iso_log_file = File.join(dirname, basename + ".osm2iso.log")

# output file is base filename + .ISOout + .csv
osm2iso_output_file = File.join(dirname, basename + ".ism")


puts "Writing processing log to #{osm2iso_log_file}"
# create output file and start writing out 
logfile = File.new(osm2iso_log_file, 'w')


# print out the name of this script with $0
#puts "Running  #{$0}  on input file  #{input_file}"
#logfile.puts "Running ruby #{$0} #{input_file}"
logfile.puts "Starting Log File"

#logfile.puts "Input File: #{input_file}"
logfile.puts "Output File: #{osm2iso_output_file}"

# puts "...Loading OSM"
# logfile.puts "*****************************"
# logfile.puts "  Loading Model"
# logfile.puts "*****************************"

# # load the model
model = OpenStudio::Model::Model::load(OpenStudio::Path.new(input_file))
if model.empty?
  puts "Model cannot be read"
  return false
end
model = model.get

puts "...Setting Defaults"
logfile.puts "*****************************"
logfile.puts "       Setting Defaults"
logfile.puts "*****************************"
# these are items that aren't in the OSM file can't be extracted and must be set by user	
occupancy_day_start = 1 	# set default starting day of occupancy as monday (Su=0, Mon=1 Sa=6)
occupancy_day_end = 5 		# set the default ending day of occupancy
occupancy_hour_start = 8 	# set the default starting hour for occupancy for an occupied day
occupancy_hour_end = 18 	# set the default ending hour for occupancy for an unoccupied day

logfile.puts "Occupancy Start Day = #{occupancy_day_start} with Sun = 0, Sat = 6"
logfile.puts "Occupancy End Day = #{occupancy_day_end} with Sun = 0, Sat = 6"
logfile.puts "Occupancy Start Hour = #{occupancy_hour_start} "
logfile.puts "Occupancy End Hour = #{occupancy_hour_end}"

#these are items that aren't modeled in OS so set default values
occupancy_sensors = 1 	# occupancy sensors off in ISO since they can't be modeled directly in OS 
logfile.puts "Occupancy Sensors set 1 because they are not yet supported "
const_illum_ctrl = 1 	# constant illumination control is off in ISO since it can't be modeled directly in OS		
logfile.puts "Constant Illumination Control set to 1 because it is not supported"	
ventilation_type = 1	#set ventilation type to mechanical only since OS doesn't handle natural ventilation yet
logfile.puts "Ventilation type set to 1 (mechanical only) because natural ventilation is not yet supported"
heat_recovery_fraction = 0.0  			# set heat recovery fraction to 0 since OS doesn't support heat recovery yet
logfile.puts "Heat recovery set to 0.0 because it is not syet upported"
bem_type = 1			# set bem_type=1 because OS doesn't handle BEM yet
logfile.puts "BAS/BEM set to none because it is not yet supported"

#these are inputs that are really not the same as anything in OS so set defaults or get user inputs
specific_fan_power = 1/0.7733  # set default specific fan power (L/W)
logfile.puts "Specific fan power set to 1.3 L/W to match EnergyPlus default of 773.3 W/(m3/s)"

fan_flow_control_factor = 1.0 # set default fan flow control factor to 1.0 (no energy savings)
logfile.puts "Fan Flow Control Factor set to 1.0 (no energy saving features)"

heating_pump_control_factor = 1.0 	# set default heating pump control factor to 1.0 (no control or vfd)
cooling_pump_control_factor = 1.0 	# set default cooling pump control factor to 1.0 (no control or vfd)
logfile.puts "Heating and Cooling Pump Control Factor set to 1.0 (no energy savings features)"

#these are inputs often ignored in OS models so use these as defaults
dhw_dist_type = 2  		#default DHW distribution_type is other/unknown (0, close to taps, 1 far from taps, 2 = circulation/other/ unknown 

logfile.puts "DHW distribution system set to circulation or unknown"

# until we can extract HVAC inputs and guess at HVAC type, set these defaults
hvac_type = 24			# set default HVAC type to VAV 
logfile.puts "HVAAC system type set VAV with water heating and water cooling"

cooling_IPLVToCop_ratio = 1.0 	# set default system partial load multiplier

logfile.puts "HVAC set cooling mean partial load fraction to 1.0"

exhaust_recirculation_fraction = 0.0 	# set fraction of air recirculated  to 0.0 (i.e. total outside air)
# set default infiltration rate until we write code to extract it
logfile.puts "Exhaust recirculation fraction set to 0.0"

logfile.puts "Shading Not Analyzed: Setting window  SCF=1, SDF=1"

# set SCF (external solar control) and SDF (shading) these for all directions N, NE, E, SE, S, SW, W, NW
window_SCF = Array.new(8,1.0) # set default window solar control factor = 1.0 (no overhang solar control)
window_SDF = Array.new(8,1.0) # set default window shading device factor = 1.0 (no shades)
	

puts "...Parsing Model"
logfile.puts "*****************************"
logfile.puts "  Parsing Model"
logfile.puts "*****************************"

# get the weatherfile if one is defined in the OSM and convert to a string
# wfu = model.getWeatherFile.url.to_s
# if wfu.empty?
	# logfile.puts "Warning: A Weather file not defined in the OSM"
	# weather_file_path="none"
# else
	# # strip off the file: part of the URL, only save the stuff after file:
	# weather_file_path=wfu.split(':').last
	# logfile.puts "Weather file #{weather_file_path} found"
# end

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

const_names = []
const_solarAbsorptance={}
const_thermalAbsorptance={}
const_heatCapacity = {}
const_U = {}
const_SHGC= {}

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
		
		const_R = 0.04 + 0.12  #set the starting R at 0.04 + 0.13 = avg outside film coeff + inside film coeff for walls
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
				# heat capacity is not defined for this material so calculate it ias thickness*density*specific heat
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
	
		const_R = 0.04 + 0.12  #set the starting R at 0.04 + 0.13 = avg outside film coeff + inside film coeff for walls

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
				const_SHGC[name] = 0.8
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
	logfile.printf("Solar Absorption = %0.2f, Thermal Absorption =%0.2f, SHGC = %0.2f \n",const_solarAbsorptance[name],const_thermalAbsorptance[name],const_SHGC[name])
end

# parse the schedules to find the average schedule value for occupied and unoccupied times
puts "...Parsing Schedules"

startDate = OpenStudio::Date.new("Jan".to_MonthOfYear,1)
endDate = OpenStudio::Date.new("Dec".to_MonthOfYear,31)

sched_names = []
occ_aves={}
unocc_aves={}
occupied_hours=0.0
unoccupied_hours = 0.0

logfile.puts "*****************************"
logfile.puts "  Calculating Schedule Averages"
logfile.puts "*****************************"

schedule_rulesets = model.getScheduleRulesets
logfile.puts "Found #{schedule_rulesets.size} schedules in the OSM"

schedule_rulesets.each do |schedule|
	occupied_sum=0;
	unoccupied_sum=0;
	occupied_hours=0;
	unoccupied_hours=0;
	
	# gets one day schedule for each day of the year
	daySchedules = schedule.getDaySchedules(startDate, endDate)
		
	#get the day of the week of the starting day of the schedule and subtract 1 from it because we increment before we compare
	day_of_week =  startDate.dayOfWeek.value - 1

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
light_total_area = epsilon   # set this to a tiny number to avoid divide by zero but to also avoid skewing the area
light_occ_total = 0
light_unocc_total = 0
elec_total_area = epsilon
elec_occ_total = 0
elec_unocc_total=0
gas_total_area = epsilon
gas_occ_total = 0
gas_unocc_total=0
people_total_area = epsilon
people_occ_total = 0
people_unocc_total = 0
activity_total_area = epsilon
activity_occ_total = 0
activity_unocc_total = 0

light_areas=[]
light_scheds=[]

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
lpd_occ_mult = light_occ_total/light_total_area
lpd_unocc_mult = light_unocc_total/light_total_area
epd_occ_mult = elec_occ_total/elec_total_area
epd_unocc_mult = elec_unocc_total/elec_total_area
gpd_occ_mult = gas_occ_total/gas_total_area
gpd_unocc_mult = gas_unocc_total/gas_total_area
people_occ_mult = people_occ_total/people_total_area
people_unocc_mult = people_unocc_total/people_total_area

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
cooling_setpoint_occ_=0
cooling_setpoint_unocc=0

# get the average heating and cooling thermostat values during occupied and unoccupied times 
# set some defaults in case  some thermostats aren't defined
heat_setpoint_occ_default = 21 # set the default temperature setpoint for heating during occupied times
heat_setpoint_unocc_default = 15 # set the default temperature setpoint for heating during unccupied times
cool_setpoint_occ_default = 25 # set the default temp setpoint for cooling during occupied times
cool_setpoint_unocc_default = 28 # set the default temp setpoint for cooling during unoccupied times

total_zone_area = epsilon
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
total_area=epsilon  # start with a small total area to avoid divide by zero
daylight_area = 0.0
spaces.each do |s|
	total_area += s.floorArea * s.multiplier  # add up the total floor area
	daylight_ctrls = s.daylightingControls
	if daylight_ctrls.size != 0   # check for a daylighting control of any kind, assume it's turned on
		daylight_area += s.floorArea * s.multiplier  # add up the total daylighting control area
	end
end
frac_daylight=daylight_area / total_area

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
	z_points=[]
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
# set default wall U = 0.3, alpha = 0.5, epsilon=0.5 window U=3.0, window SHGC = 0.5
direction_names= ["N","NE","E","SE","S","SW","W","NW","N"]
dirLimit = [0,45,90,135,180,225,270,315,360]
wall_areas=Array.new(8,0.0)  # generate an 8x1 array filled with zeros
window_areas=Array.new(8,0.0)
window_areas=Array.new(8,0.0)
wall_U = Array.new(8,0.3)
window_U = Array.new(8,3.0)
wall_solar_absorption = Array.new(8,0.5)
wall_thermal_emissivity = Array.new(8,0.5)
window_SHGC = Array.new(8,0.4)

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
			# add in lines to  compute SCF, and SDF in each direction
		end		
	end		
end 

#  Get the area weighted average of wall absorption, emissivity, U and window U and SHGC
for i in 0 .. 7
	if not (wall_areas[i] == 0.0)
		wall_solar_absorption[i] = wall_abs_sum[i] / wall_areas[i]
		wall_thermal_emissivity[i]= wall_emiss_sum[i] / wall_areas[i]
	end
	if not (wall_U_area[i] == 0.0)  # if any wall surfaces have a U value defined, use the weighted average of U values
		wall_U[i] = wall_U_sum[i]/wall_U_area[i]
	end
	if not (window_areas[i] == 0.0)
		window_U[i] = window_U_sum[i]/window_areas[i]
		window_SHGC[i] = window_SHGC_sum[i] / window_areas[i]
	end
end

total_wall_area =0.0
wall_areas.each do |a| 
	total_wall_area += a
end

total_window_area=0.0
window_areas.each do |a| 
	total_window_area += a
end

logfile.printf("Total Wall Area = %1.2f m2\n",total_wall_area)
logfile.printf("Total Window Area = %1.2f  m2\n",total_window_area)

# set default roof U, absorption, emissivity and skylight U and SHGC
roof_solar_absorption = 0.9  	# set default roof solar absorption coefficient to 0.9
roof_thermal_emissivity = 0.9	# set default roof thermal emissivity to 0.9
roof_U = 0.2					# set default roof U to 0.2 W/m2/K
skylight_U = 3
skylight_SHGC = 0.5

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
	skylight_area += s.grossArea - s.netArea	# get the skylight area
	
	subsurface=s.subSurfaces
	subsurface.each do |ss|
		skylight_U_sum += const_U[ss.construction.get.name.to_s] * ss.surface.get.netArea
		skylight_SHGC_sum+= const_SHGC[ss.construction.get.name.to_s] * ss.surface.get.netArea
	end	
end
logfile.puts "Found #{roof_count} roof surfaces"

if not (roof_area == 0.0)
	roof_solar_absorption = roof_abs_sum / roof_area 		# compute roof absorption as area weighted average
	roof_thermal_emissivity=  roof_emiss_sum/ roof_area 	# compute roof emissivity as an area weighted average
	roof_U = roof_U_sum / roof_area 						# compute roof U as area weighted average	
end

if not (skylight_area == 0.0)
	skylight_U = skylight_U_sum / skylight_area
	skylight_SHGC = skylight_SHGC_sum /skylight_area
end

for i in 0..7
	logfile.printf("Wall%s: Area=%1.0f m2, U=%1.2f W/m2/K, Absorption=%1.2f, Emissivity=%1.2f \n",
		direction_names[i],wall_areas[i],wall_U[i],wall_solar_absorption[i],wall_thermal_emissivity[i])
end
for i in 0..7
	logfile.printf("Window%s: Area=%1.0f m2, U=%1.2f W/m2/K, SHGC=%1.2f, SCF=%1.2f, SDF=%1.2f \n",
		direction_names[i],window_areas[i],window_U[i],window_SHGC[i],window_SCF[i],window_SDF[i])
end


# surfaces.each do |s|
	# next if not s.surfaceType == "RoofCeiling"				# skip surface if not labeled roof
	# next if not s.outsideBoundaryCondition == "Outdoors"	# skip surface if not outside
	# roof_count +=1
	# absorption = const_solarAbsorptance[s.construction.get.name.to_s]
	# emissivity = const_thermalAbsorptance[s.construction.get.name.to_s]
	# heatcapacity = const_heatCapacity[s.construction.get.name.to_s]
	# roofU = const_U[s.construction.get.name.to_s]
	
	# roof_abs_sum += absorption * s.netArea
	# roof_emiss_sum += emissivity * s.netArea
	# roof_HC_sum += heatcapacity * s.netArea
	# roof_U_sum += roofU * s.netArea

	# roof_area += s.netArea				
	# skylight_area += s.grossArea - s.netArea	
	
	# subsurface=s.subSurfaces
	# subsurface.each do |ss|
		# skylight_U_sum += const_U[ss.construction.get.name.to_s] * ss.surface.get.netArea
		# skylight_SHGC_sum+= const_SHGC[ss.construction.get.name.to_s] * ss.surface.get.netArea
	# end	
	
# end

logfile.printf("Roof: Area=%1.0f m2, U=%1.2f W/m2/K, Absorption=%1.2f, Emissivity=%1.2f \n",roof_area,roof_U,roof_solar_absorption,roof_thermal_emissivity)
logfile.printf("Skylight: Area=%1.0f m2, U=%1.2f W/m2/K, SHGC=%1.2f \n",skylight_area,skylight_U,skylight_SHGC)

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

puts "...Calculating Infiltration"
logfile.puts "*****************************"
logfile.puts "   Calculating Infiltration"
logfile.puts "*****************************"

# compute infiltration
#first check to see if there effective leakage areas defined and if not those, then design flow rates
if not model.getSpaceInfiltrationEffectiveLeakageAreas.empty?
	infiltration = model.getSpaceInfiltrationEffectiveLeakageAreas
	infiltration.each do |infil|
		# set default average envelope air leakage (infiltration) as 7 m3/h/m2 which is about the EnergyPlus defaults
		logfile.puts "EffectiveLeakageArea not Implemented Yet, Infiltration Rate Set to 7.0 m3/m2/h @ 75 Pa"
                
                # JMT this line doesn't make any sense, why is it here?
		infiltration_rate = 7.0		
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
			logfile.puts "Infiltration schedule found for Infiltration object ##{count}, scaling by #{infil_frac}"
		else
			logfile.puts "No Infiltration schedule found for Infiltration object ##{count}, assuming always on"
			infil_frac = 1.0
		end
		infil_rate_sum +=  infil_rate_temp * infil_frac
	end
	logfile.printf("Total Infiltration = %1.3f m3/s at 4 Pa (natural pressure difference)\n",infil_rate_sum);

	# get avg infiltration rate and convert to from m3/m2/min @ 4 Pa to m3/m2/h @ 75 Pa with a .67 pressure exponent
	# assume constant infilration rate is based on a nominal 4 Pa pressure difference 
	infiltration_rate = infil_rate_sum/(total_wall_area + roof_area)*3600*(75/4)**0.67
else
	logfile.puts "No Infiltration Design Flow Rates Found, Infiltration Rate Set to 2.0 m3/m2/h @ 75 Pa"
	infiltration_rate = 2.0		# set default average envelope air leakage (infiltration) as 2 m3/h/m2 which is about the EnergyPlus default
end
logfile.printf( "Infiltration rate set to %1.3f m3/h/m2 @ 75 Pa\n", infiltration_rate)

puts "...Parsing HVAC Info"
logfile.puts "*****************************"
logfile.puts "   Parsing HVAC Info"
logfile.puts "*****************************"

logfile.puts "*****************************"
logfile.puts "  Analyzing HVAC"
logfile.puts "*****************************"
puts "...Analyzing HVAC"

hvac_component_array = []
hvac_component_area_array=[]

plant_loops_array=[]
plant_loops_area_array=[]

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


cooling_coil_array=[]
cooling_coil_area_array=[]
heating_coil_array=[]
heating_coil_area_array=[]
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
cooling_coil_name_array=[]
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
	cooling_COP=0.0
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
if ((gas_sum + elec_sum) == 0.0  ) && (heating_coil_array.size>0)
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

# print out efficiencies
logfile.puts "Cooling System COP = #{cooling_COP}"
logfile.puts "Cooling System IPLV/COP Ratio =#{cooling_IPLVToCop_ratio}"
logfile.puts "Heating Fuel Type = #{heating_fuel_type}"  # heating energy, 1=electric, 0=gas
logfile.puts "Heating System Efficiency = #{heating_system_efficiency}"

# calculate fresh air ventilation rates
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

supply_exhaust_rate = freshair_flow_rate	# set exhaust = supply rate as default (i.e. no pressurization)
logfile.printf("Total ventilation fresh air flow rate = %1.2f L/s\n", freshair_flow_rate)
logfile.puts "No exhaust rate extraction - set air exhaust rate to ventilation rate" 


# Try to find DHW water components and get the water use

# search plant loops for water heaters
dhw_gas_array=[]
dhw_elec_array=[]
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

# use DHW distribution type to set DHW distribution efficiency
dhw_dist_eff_table=[1.0,0.8,0.6]  # create lookup table
dhw_distribution_efficiency = dhw_dist_eff_table[dhw_dist_type]

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

if (dhw_gas_sum + dhw_elec_sum) ==0.0	# if the gas + electric sum = 0 then there were no DHW heaters, so set demand to 0 no matter what
	dhw_demand = 0.0
end
	
# print out the DHW fuel type, system efficiency, distribution efficiency
logfile.printf("DHW: Fuel Type = %i, System Efficiency = %0.2f Distribution Efficiency = %0.2f, Demand = %1.2f m3/yr \n", 
	dhw_fuel_type,  dhw_system_efficiency, dhw_distribution_efficiency,dhw_demand)

puts "Writing Output to #{osm2iso_output_file}"
# create output file and start writing out 

time=Time.new  # get the current system clock time
File.open(osm2iso_output_file, 'w') do |file|

	file.puts "\##{osm2iso_output_file} generated by osm2iso.rb on #{input_file} on #{time.ctime}"

	file.puts "weatherFilePath = #{weather_file_path}"
	file.puts "\# Terrain class urban/city = 0.8, suburban/some shielding = 0.9, country/open = 1.0"
	file.puts "terrainClass = #{terrain_class}"
	file.puts "\# building height is in m,  floor area is in m^2, people density is m2/person"
	file.puts "buildingHeight = #{building_height}"
	file.puts "floorArea = #{floor_area}"
	file.puts "peopleDensityOccupied = #{area_per_person_occupied}"
	file.puts "peopleDensityUnoccupied = #{area_per_person_unoccupied}"
	
	file.puts "buildingOccupancyFrom = #{occupancy_day_start}"
	file.puts "buildingOccupancyTo = #{occupancy_day_end}"
	file.puts "equivFullLoadOccupancyFrom = #{occupancy_hour_start}"
	file.puts "equivFullLoadOccupancyTo = #{occupancy_hour_end}"

	file.puts "\# LPD, EPD, GPD all in W/m2, ext light power in W, an heat gain is W/person"
	file.puts "lightingPowerIntensityOccupied = #{lpd_occupied}"
	file.puts "lightingPowerIntensityUnoccupied = #{lpd_unoccupied}"
	file.puts "elecPowerAppliancesOccupied = #{epd_occupied}"
	file.puts "elecPowerAppliancesUnoccupied = #{epd_unoccupied}"
	file.puts "gasPowerAppliancesOccupied = #{gpd_occupied}"
	file.puts "gasPowerAppliancesUnoccupied = #{gpd_unoccupied}"
	file.puts "exteriorLightingPower = #{exterior_lighting_power}"
    file.puts "heatGainPerPerson=#{activity_level_occ}"
	# when we implement occupied and unoccupied heat gain per person, we comment above and uncomment below
    #file.puts "heatGainPerPersonOccupied=#{activity_level_occ}"
	#file.puts "heatGainPerPersonUnoccupied=#{activity_level_unocc}"
	
	file.puts "\# Temp set points in degrees C"
	file.puts "heatingOccupiedSetpoint = #{heating_setpoint_occ}"
	file.puts "heatingUnoccupiedSetpoint= #{heating_setpoint_unocc}"
	file.puts "coolingOccupiedSetpoint= #{cooling_setpoint_occ}"
	file.puts "coolingUnoccupiedSetpoint= #{cooling_setpoint_unocc}"
	
	file.puts "\# HVAC waste, heating and cooling loss factors set by HVAC type from EN 15243"
	file.puts "hvacWasteFactor = #{hvac_waste_factor}"	
	file.puts "hvacHeatingLossFactor = #{hvac_heating_loss_factor}"
	file.puts "hvacCoolingLossFactor = #{hvac_cooling_loss_factor}"
	
	file.puts "\# daylight sensors, occupancy sensors, illum control are set to 1 if there is no control.   See iso 15193 Annex F/G for values"
	file.puts "daylightSensorSystem = #{daylight_sensors}"
	file.puts "lightingOccupancySensorSystem = #{occupancy_sensors}"
	file.puts "constantIlluminationControl = #{const_illum_ctrl}"
	
	file.puts "\# COP is W/W, coolingSystemIPLV is the ratio of IPLV/COP"
	file.puts "coolingSystemCOP = #{cooling_COP}"
	file.puts "coolingSystemIPLVToCopRatio =#{cooling_IPLVToCop_ratio}"
	
	file.puts "\#energycarrier is 1 if electric, 2 if gas"
	file.puts "heatingEnergyCarrier = #{heating_fuel_type}"  # heating energy, 1=electric, 0=gas
	file.puts "heatingSystemEfficiency = #{heating_system_efficiency}"
	
	file.puts "\# vent type is 1 if mech, 2 if natural, 3 if mixed"
	file.puts "ventilationType = #{ventilation_type}" 
	file.puts "\# ventilation flow rates in L/s"
	file.puts "freshAirFlowRate = #{freshair_flow_rate}"
	file.puts "supplyExhaustRate = #{supply_exhaust_rate}"

	file.puts "\# heatRecover = efficiency of heat recovery (0 for none), exhaustAirRecir = fraction of supply air recirculated"
	file.puts "heatRecovery = #{heat_recovery_fraction}"  # heat recovery efficiency, 0 for no heat recovery
	file.puts "exhaustAirRecirculation = #{exhaust_recirculation_fraction}"  # fraction of supply air that is recirculated

	file.puts "\# DHW demand in m3/yr use 10 m3/yr/person as a default for offices"
	file.puts "dhwDemand =  #{dhw_demand}"  #
	# when we implement occupied and unoccupied infiltration, we comment above and uncomment below
    #file.puts "dhwDemandOccupied=#{dhw_demand_occ}"
	#file.puts "dhwDemandUnoccupied=#{dhw_demand_unocc}"
	
	file.puts "\# dhwDistribution efficiency all taps w/i 3m = 1, taps more than 3m = 0.8, circulation or unknown = 0.6, see NEN 2916 12.6, "
	file.puts "dhwSystemEfficiency = #{dhw_system_efficiency}"
	file.puts "dhwDistributionEfficiency = #{dhw_distribution_efficiency}"
	file.puts "dhwEnergyCarrier = #{dhw_fuel_type}"	

	file.puts "\# Surface Heat capacity in J/K/m2"
	file.puts "interiorHeatCapacity = #{interior_heat_capacity}"
	file.puts "exteriorHeatCapacity = #{exterior_heat_capacity}"

	file.puts "\# BEM type 1= none, 2 = simple, 3=advanced with FDD"
	file.puts "bemType = #{bem_type}"
	
	file.puts "\# specific fan power in L/s/W,  flow control factor is energy reduction from fan control measures"
	file.puts "specificFanPower = #{specific_fan_power} "
	
	file.puts "\# fan flow control  1= no control, 0.75 = inlet blade adjuct, 0.65= variable speed  see NEN 2916 7.3.3.4  "
	file.puts "fanFlowcontrolFactor = #{fan_flow_control_factor}"
	
	file.puts "\# infiltration in m3/m2/hr based on surface area"
	file.puts "infiltration = #{infiltration_rate}" # leakage per unit surface area in m^3/m^2/hr
	# when we implement occupied and unoccupied infiltration, we comment above and uncomment below
    #file.puts "infiltrationOccupied=#{infiltration_rate_occ}"
	#file.puts "infiltrationUnoccupied=#{infiltration_rate_unocc}"

	file.puts "\# pump control 0= no pump, 0.5 = auto pump controls for more 50% of pumps, 1.0 = all other cases.   See NEN 2914 9.4.3"
    file.puts "heatingPumpControl=#{heating_pump_control_factor}"
    file.puts "coolingPumpControl=#{cooling_pump_control_factor}"

	# write out roof and skylight info
	file.puts "\# Areas in m2, U values in W/m2/K"
	file.puts "\# SCF = solar control factor (external control), SDF = shading device factor = internal shading"
	file.puts "roofArea = #{roof_area}"
    file.puts "roofUValue = #{roof_U}"
    file.puts "roofSolarAbsorption = #{roof_solar_absorption}"
	file.puts "roofThermalEmissivity= #{roof_thermal_emissivity}"

	file.puts "skylightArea = #{skylight_area}"
	file.puts "skylightUValue = #{skylight_U}"
	file.puts "skylightSHGC = #{skylight_SHGC}"

	# write out the wall and window info
	for i in 0 .. 7
		file.puts "WallArea#{direction_names[i]} = #{wall_areas[i]}"
		file.puts "WallUvalue#{direction_names[i]} = #{wall_U[i]}"
		file.puts "WallSolarAbsorption#{direction_names[i]} = #{wall_solar_absorption[i]}"
		file.puts "WallThermalEmissivity#{direction_names[i]} = #{wall_thermal_emissivity[i]}"
	end
	for i in 0 .. 7
		file.puts "WindowArea#{direction_names[i]} = #{window_areas[i]}"
		file.puts "WindowUvalue#{direction_names[i]} = #{window_U[i]}"
		file.puts "WindowSHGC#{direction_names[i]} = #{window_SHGC[i]}"
		file.puts "WindowSCF#{direction_names[i]} = #{window_SCF[i]}"	
		file.puts "WindowSDF#{direction_names[i]} = #{window_SDF[i]}"			
	end
end
logfile.close
puts "Done Converting!"
