
#require 'rubygems'
require 'json'
require 'fileutils'
require 'openstudio'

class SpaceTypeGenerator

def initialize(path_to_standards_json, path_to_master_schedules_library)
  
  #load the data from the JSON file into a ruby hash
  @standards = {}
  temp = File.read(path_to_standards_json.to_s)
  @standards = JSON.parse(temp)
  @spc_types = @standards["space_types"]
  @climate_zone_sets = @standards["climate_zone_sets"]
  @climate_zones = @standards["climate_zones"]
  if @spc_types.nil? or @climate_zone_sets.nil? or @climate_zones.nil?
    puts "The space types json file did not load correctly."
    exit
  end
  
  #check that the data was loaded correctly
  check_data = @spc_types["189.1-2009"]["ClimateZone 1-3"]["Hospital"]["Radiology"]["lighting_w_per_area"]
  unless (check_data-0.36).abs < 0.0000001
    puts "The space types json file does not have expected content."
    exit
  end

  #load up the osm with all the reference building schedules
  path_to_master_schedules_library = OpenStudio::Path.new(path_to_master_schedules_library)
  
  vt = OpenStudio::OSVersion::VersionTranslator.new
  @schedule_library = vt.loadModel(path_to_master_schedules_library).get  
  
  @created_names = []
end

def make_name(template, clim, building_type, spc_type)

  clim = clim.gsub("ClimateZone ", "CZ")
  if clim == "CZ1-8"
    clim = ""
  end
  
  if building_type == "FullServiceRestaurant"
    building_type = "FullSrvRest"
  elsif building_type == "Hospital"
    building_type = "Hospital"
  elsif building_type == "LargeHotel"
    building_type = "LrgHotel"
  elsif building_type == "LargeOffice"
    building_type = "LrgOffice"
  elsif building_type == "MediumOffice"
    building_type = "MedOffice"
  elsif building_type == "Mid-riseApartment"
    building_type = "MidApt"
  elsif building_type == "Office"
    building_type = "Office"
  elsif building_type == "Outpatient"
    building_type = "Outpatient"
  elsif building_type == "PrimarySchool"
    building_type = "PriSchl"
  elsif building_type == "QuickServiceRestaurant"
    building_type = "QckSrvRest"
  elsif building_type == "Retail"
    building_type = "Retail"
  elsif building_type == "SecondarySchool"
    building_type = "SecSchl"
  elsif building_type == "SmallHotel"
    building_type = "SmHotel"
  elsif building_type == "SmallOffice"
    building_type = "SmOffice"
  elsif building_type == "StripMall"
    building_type = "StMall"
  elsif building_type == "SuperMarket"
    building_type = "SpMarket"
  elsif building_type == "Warehouse"
    building_type = "Warehouse"
  end
  
  
  parts = [template]
  
  if not building_type.empty?
    parts << building_type
  end
    
  if not spc_type.empty?
    parts << spc_type
  end
  
  if not clim.empty?
    parts << clim
  end
  
  result = parts.join(' - ')
  
  @created_names << result
  
  return result
end

def longest_name
  sorted_names = @created_names.sort{|x,y| x.size <=> y.size}
  return sorted_names[-1]
end

# check if a specific climate zone is included in a given climate zone set
def is_climate_zone_in_climate_zone_set(climate_zone, climate_zone_set)
  if data = @climate_zone_sets[climate_zone_set]
    if climate_zones = data["climate_zones"]
      if climate_zones.include?(climate_zone)
        return true
      end
    end
  end
  return false
end

# pass in a specific climate zone here and get the climate zone set to use for generate_construction_set
def find_climate_zone_set(template, clim, building_type, spc_type)
  possible_climate_zone_sets = []
  
  if tmp1 = @spc_types[template]
    tmp1.each_pair do |climate_zone_set, tmp2|
      if tmp3 = tmp2[building_type]
        if data = tmp3[spc_type]
          possible_climate_zone_sets << climate_zone_set
        end
      end
    end
  end
  
  result = nil
  possible_climate_zone_sets.each do |possible_climate_zone_set|
    if climate_zone_set = @climate_zone_sets[possible_climate_zone_set]
      if climate_zones = climate_zone_set['climate_zones']
        if climate_zones.include?(clim)
          if not result
            result = possible_climate_zone_set
          else
            puts "Error, climate zone contained in multiple climate zone sets"
          end
        end
      end
    end
  end
  return result
end

# pass in the climate zone set here
def generate_space_type(template, clim, building_type, spc_type, model = nil)

  if model.nil?
    model = OpenStudio::Model::Model.new
  end

  puts "generating space type: #{template}-#{clim}-#{building_type}-#{spc_type}"

  #grabs a schedule with a specific name from the library, clones it into the space type model, and returns itself to the user
  def get_sch_from_lib(sch_name, model)
    #first check model
    sch = model.getObjectByTypeAndName("OS_Schedule_Ruleset".to_IddObjectType, sch_name)
    if not sch.empty?
      # could clone here if you really wanted to
      return sch.get.to_ScheduleRuleset.get
    end
    
    #get the correct space type from the library file
    sch = @schedule_library.getObjectByTypeAndName("OS_Schedule_Ruleset".to_IddObjectType, sch_name)
    
    if sch.empty?
      puts "schedule called '#{sch_name}' not found in master schedule library"
      exit
    end
    #clone the space type from the library model into the space type model
    clone_of_sch = sch.get.to_Schedule.get.clone(model)
    new_sch = clone_of_sch.to_ScheduleRuleset.get
    return new_sch
  end
  
  name = make_name(template, clim, building_type, spc_type)

  #create a new space type and name it
  space_type = OpenStudio::Model::SpaceType.new(model)
  space_type.setName(name)

  #set the standards building type and space type for this new space type
  space_type.setStandardsBuildingType(building_type)
  space_type.setStandardsSpaceType(spc_type)
  
  #set the rendering color of the space type  
  rgb = @spc_types[template][clim][building_type][spc_type]["rgb"]
  rgb = rgb.split('_')
  r = rgb[0].to_i
  g = rgb[1].to_i
  b = rgb[2].to_i
  rendering_color = OpenStudio::Model::RenderingColor.new(model)
  rendering_color.setRenderingRedValue(r)
  rendering_color.setRenderingGreenValue(g)
  rendering_color.setRenderingBlueValue(b)
  space_type.setRenderingColor(rendering_color)
  
  #create the schedule set for the space type
  default_sch_set = OpenStudio::Model::DefaultScheduleSet.new(model)
  default_sch_set.setName("#{name} Schedule Set")
  space_type.setDefaultScheduleSet(default_sch_set)

  #lighting  
    
    make_lighting = false
    lighting_per_area = @spc_types[template][clim][building_type][spc_type]["lighting_w_per_area"]
    lighting_per_person = @spc_types[template][clim][building_type][spc_type]["lighting_w_per_person"]
    unless (lighting_per_area == 0 or lighting_per_area.nil?) then make_lighting = true end
    unless (lighting_per_person == 0 or lighting_per_person.nil?) then make_lighting = true end
    
    if make_lighting == true
    
      #create the lighting definition 
      lights_def = OpenStudio::Model::LightsDefinition.new(model)
      lights_def.setName("#{name} Lights Definition")
      unless  lighting_per_area == 0 or lighting_per_area.nil?
        lights_def.setWattsperSpaceFloorArea(OpenStudio::convert(lighting_per_area,"W/ft^2","W/m^2").get)
      end
      unless lighting_per_person == 0 or lighting_per_person.nil?
        lights_def.setWattsperPerson(OpenStudio::convert(lighting_per_person,"W/person","W/person").get)
      end

      #create the lighting instance and hook it up to the space type
      lights = OpenStudio::Model::Lights.new(lights_def)
      lights.setName("#{name} Lights")
      lights.setSpaceType(space_type)  
      
      #get the lighting schedule and set it as the default
      lighting_sch = @spc_types[template][clim][building_type][spc_type]["lighting_sch"]
      unless lighting_sch.nil?
        default_sch_set.setLightingSchedule(get_sch_from_lib(lighting_sch, model))
      end    
    
    end

  #ventilation

    make_ventilation = false
    ventilation_per_area = @spc_types[template][clim][building_type][spc_type]["ventilation_per_area"]  
    ventilation_per_person = @spc_types[template][clim][building_type][spc_type]["ventilation_per_person"]
    ventilation_ach = @spc_types[template][clim][building_type][spc_type]["ventilation_ach"]
    unless (ventilation_per_area  == 0 or ventilation_per_area.nil?) then make_ventilation = true  end
    unless(ventilation_per_person == 0 or ventilation_per_person.nil?) then make_ventilation = true end
    unless(ventilation_ach == 0 or ventilation_ach.nil?) then make_ventilation = true end
    
    if make_ventilation == true
      
      #create the ventilation object and hook it up to the space type
      ventilation = OpenStudio::Model::DesignSpecificationOutdoorAir.new(model)
      ventilation.setName("#{name} Ventilation")
      space_type.setDesignSpecificationOutdoorAir(ventilation)
      ventilation.setOutdoorAirMethod("Sum")
      unless ventilation_per_area  == 0 or ventilation_per_area.nil? 
        ventilation.setOutdoorAirFlowperFloorArea(OpenStudio::convert(ventilation_per_area,"ft^3/min*ft^2","m^3/s*m^2").get)
      end
      unless ventilation_per_person == 0 or ventilation_per_person.nil?
        ventilation.setOutdoorAirFlowperPerson(OpenStudio::convert(ventilation_per_person,"ft^3/min*person","m^3/s*person").get)
      end
      unless ventilation_ach == 0 or ventilation_ach.nil?
        ventilation.setOutdoorAirFlowAirChangesperHour(ventilation_ach)
      end
      
    end
    
  #occupancy

    make_people = false
    occupancy_per_area = @spc_types[template][clim][building_type][spc_type]["occupancy_per_area"]
    unless(occupancy_per_area == 0 or occupancy_per_area.nil?) then make_people = true end
    
    if make_people == true

      #create the people definition
      people_def = OpenStudio::Model::PeopleDefinition.new(model)
      people_def.setName("#{name} People Definition")
      unless  occupancy_per_area == 0 or occupancy_per_area.nil?
        people_def.setPeopleperSpaceFloorArea(OpenStudio::convert(occupancy_per_area/1000,"people/ft^2","people/m^2").get)
      end    
      
      #create the people instance and hook it up to the space type
      people = OpenStudio::Model::People.new(people_def)
      people.setName("#{name} People")
      people.setSpaceType(space_type)
      
      #get the occupancy and occupant activity schedules from the library and set as the default
      occupancy_sch = @spc_types[template][clim][building_type][spc_type]["occupancy_sch"]
      unless occupancy_sch.nil?
        default_sch_set.setNumberofPeopleSchedule(get_sch_from_lib(occupancy_sch, model))
      end
      occupancy_activity_sch = @spc_types[template][clim][building_type][spc_type]["occupancy_activity_sch"]  
      unless occupancy_activity_sch.nil?
        default_sch_set.setPeopleActivityLevelSchedule(get_sch_from_lib(occupancy_activity_sch, model))
      end
      
    end
    
  #infiltration

    make_infiltration = false
    infiltration_per_area_ext = @spc_types[template][clim][building_type][spc_type]["infiltration_per_area_ext"]      
    unless(infiltration_per_area_ext == 0 or infiltration_per_area_ext.nil?) then make_infiltration = true end

    if make_infiltration == true
      
      #create the infiltration object and hook it up to the space type
      infiltration = OpenStudio::Model::SpaceInfiltrationDesignFlowRate.new(model)
      infiltration.setName("#{name} Infiltration")
      infiltration.setSpaceType(space_type)
      unless infiltration_per_area_ext == 0 or infiltration_per_area_ext.nil?
        infiltration.setFlowperExteriorSurfaceArea(OpenStudio::convert(infiltration_per_area_ext,"ft^3/min*ft^2","m^3/s*m^2").get)
      end
      
      #get the infiltration schedule from the library and set as the default
      infiltration_sch = @spc_types[template][clim][building_type][spc_type]["infiltration_sch"]
      unless infiltration_sch.nil?
        default_sch_set.setInfiltrationSchedule(get_sch_from_lib(infiltration_sch, model))
      end

    end    
      
  #electric equipment

    make_electric_equipment = false
    elec_equip_per_area = @spc_types[template][clim][building_type][spc_type]["elec_equip_per_area"]
    unless(elec_equip_per_area == 0 or elec_equip_per_area.nil?) then make_electric_equipment = true end
    
    if make_electric_equipment == true
    
      #create the electric equipment definition
      elec_equip_def = OpenStudio::Model::ElectricEquipmentDefinition.new(model)
      elec_equip_def.setName("#{name} Electric Equipment Definition")  
      unless  elec_equip_per_area == 0 or elec_equip_per_area.nil?
        elec_equip_def.setWattsperSpaceFloorArea(OpenStudio::convert(elec_equip_per_area,"W/ft^2","W/m^2").get)
      end
        
      #create the electric equipment instance and hook it up to the space type
      elec_equip = OpenStudio::Model::ElectricEquipment.new(elec_equip_def)
      elec_equip.setName("#{name} Electric Equipment")
      elec_equip.setSpaceType(space_type)
      
      #get the electric equipment schedule from the library and set as the default
      elec_equip_sch = @spc_types[template][clim][building_type][spc_type]["elec_equip_sch"]
      unless elec_equip_sch.nil?
        default_sch_set.setElectricEquipmentSchedule(get_sch_from_lib(elec_equip_sch, model))
      end
      
    end
      
  #gas equipment
    
    make_gas_equipment = false
    gas_equip_per_area = @spc_types[template][clim][building_type][spc_type]["gas_equip_per_area"]
    unless  (gas_equip_per_area == 0 or gas_equip_per_area.nil?) then make_gas_equipment = true end
    
    if make_gas_equipment == true
    
      #create the gas equipment definition
      gas_equip_def = OpenStudio::Model::GasEquipmentDefinition.new(model)
      gas_equip_def.setName("#{name} Gas Equipment Definition")
      unless  gas_equip_per_area == 0 or gas_equip_per_area.nil?
        gas_equip_def.setWattsperSpaceFloorArea(OpenStudio::convert(gas_equip_per_area,"Btu/hr*ft^2","W/m^2").get)
      end
      
      #create the gas equipment instance and hook it up to the space type
      gas_equip = OpenStudio::Model::GasEquipment.new(gas_equip_def)
      gas_equip.setName("#{name} Gas Equipment")
      gas_equip.setSpaceType(space_type)
      
      #get the gas equipment schedule from the library and set as the default
      gas_equip_sch = @spc_types[template][clim][building_type][spc_type]["gas_equip_sch"]
      unless gas_equip_sch.nil?
        default_sch_set.setGasEquipmentSchedule(get_sch_from_lib(gas_equip_sch, model))
      end

    end
    
    thermostat = OpenStudio::Model::ThermostatSetpointDualSetpoint.new(model)
    thermostat.setName("#{name} Thermostat")
    
    heating_setpoint_sch = @spc_types[template][clim][building_type][spc_type]["heating_setpoint_sch"]
    unless heating_setpoint_sch.nil?
      thermostat.setHeatingSetpointTemperatureSchedule(get_sch_from_lib(heating_setpoint_sch, model))
    end
   
    cooling_setpoint_sch = @spc_types[template][clim][building_type][spc_type]["cooling_setpoint_sch"]
    unless cooling_setpoint_sch.nil?
      thermostat.setCoolingSetpointTemperatureSchedule(get_sch_from_lib(cooling_setpoint_sch, model))
    end
  
  #componentize the space type
  space_type_component = space_type.createComponent

=begin
  #TODO make this return BCL component space types?

  #setup the file names and save paths that will be used
  file_name = "nrel_ref_bldg_space_type"
  component_dir = "#{Dir.pwd}/#{component_name}"
  osm_file_path = OpenStudio::Path.new("#{component_dir}/files/#{file_name}.osm")
  osc_file_path = OpenStudio::Path.new("#{component_dir}/files/#{file_name}.osc")

  #puts "component_dir = #{component_dir}"

  puts "creating directories"
  FileUtils.rm_rf(component_dir) if File.exists?(component_dir) and File.directory?(component_dir)
  FileUtils.mkdir_p(component_dir)
  FileUtils.mkdir_p("#{component_dir}/files/")

  #save the space type as a .osm
  #puts "saving osm to #{osm_file_path}"
  model.toIdfFile().save(osm_file_path,true)
  
  #save the componentized space type as a .osc
  #puts "saving osc to #{osc_file_path}"
  space_type_component.toIdfFile().save(osc_file_path,true)

  #make the BCL component
  puts "creating BCL component"
  component = BCL::Component.new(component_dir)
  puts "created uid = #{component.uuid}"

  #add component information
  component.name = component_name
  component.description = "This space type represent spaces in typical commercial buildings in the United States.  The information to create these space types was taken from the DOE Commercial Reference Building Models, which can be found at http://www1.eere.energy.gov/buildings/commercial_initiative/reference_buildings.html.  These space types include plug loads, gas equipment loads (cooking, etc), occupancy, infiltration, and ventilation rates, as well as schedules.  These space types should be viewed as starting points, and should be reviewed before being used to make decisions."
  component.source_manufacturer = "DOE"
  component.source_url = "http://www1.eere.energy.gov/buildings/commercial_initiative/reference_buildings.html"
  component.add_provenance("dgoldwas", Time.now.gmtime.strftime('%Y-%m-%dT%H:%M:%SZ'), "")
  component.add_tag("Space Types") # todo: what is the taxonomy string for space type? is there one?

  #add arguments as attributes
  component.add_attribute("NREL_reference_building_vintage", template, "")
  component.add_attribute("Climate_zone", clim, "")
  component.add_attribute("NREL_reference_building_primary_space_type", building_type, "")
  component.add_attribute("NREL_reference_building_secondary_space_type", spc_type, "")

  #openstudio type attribute
  component.add_attribute("OpenStudio Type", space_type.iddObjectType.valueDescription, "")
              
  #add other attributes
  component.add_attribute("Lighting Standard",  @spc_types[template][clim][building_type][spc_type]["lighting_standard"], "")
  component.add_attribute("Lighting Primary Space Type",  @spc_types[template][clim][building_type][spc_type]["lighting_pri_spc_type"], "")
  component.add_attribute("Lighting Secondary Space Type",  @spc_types[template][clim][building_type][spc_type]["lighting_sec_spc_type"], "")

  component.add_attribute("Ventilation Standard",  @spc_types[template][clim][building_type][spc_type]["ventilation_standard"], "")
  component.add_attribute("Ventilation Primary Space Type",  @spc_types[template][clim][building_type][spc_type]["ventilation_pri_spc_type"], "")
  component.add_attribute("Ventilation Secondary Space Type",  @spc_types[template][clim][building_type][spc_type]["ventilation_sec_spc_type"], "")

  component.add_attribute("Occupancy Standard",  "NREL reference buildings", "")
  component.add_attribute("Occupancy Primary Space Type",  building_type, "")
  component.add_attribute("Occupancy Secondary Space Type",  spc_type, "")

  component.add_attribute("Infiltration, Gas Equipment, Electric Equipment, and Schedules Standard",  "NREL reference buildings", "")
  component.add_attribute("Infiltration, Gas Equipment, Electric Equipment, and Schedules Primary Space Type",  building_type, "")
  component.add_attribute("Infiltration, Gas Equipment, Electric Equipment, and Schedules Secondary Space Type",  spc_type, "")

  #add the osm and osc files to the component
  component.add_file("OpenStudio", "0.9.3",  osm_file_path.to_s, "#{file_name}.osm", "osm")
  component.add_file("OpenStudio", "0.9.3",  osc_file_path.to_s, "#{file_name}.osc", "osc")

  #puts "saving component to #{component_dir}"
  component.save_component_xml(component_dir)

=end  

  #return the space type and the componentized space type
  return [space_type, space_type_component]
  
end #end generate_space_type

end #end class SpaceTypeGenerator

