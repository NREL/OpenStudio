
#require 'rubygems'
require 'json'
require 'fileutils'
require 'openstudio'

class ConstructionSetGenerator

def initialize(path_to_standards_json)
  
  #load the data from the JSON file into a ruby hash
  @standards = {}
  temp = File.read(path_to_standards_json.to_s)
  @standards = JSON.parse(temp)
  @construction_sets = @standards["construction_sets"]
  @constructions = @standards["constructions"]
  @materials = @standards["materials"]
  @climate_zone_sets = @standards["climate_zone_sets"]
  @climate_zones = @standards["climate_zones"]
  if @construction_sets.nil? or @constructions.nil? or @materials.nil? or @climate_zone_sets.nil? or @climate_zones.nil?
    puts "The standards json file did not load correctly."
    exit
  end

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
  
  if not clim.empty?
    parts << clim
  end
  
  if not building_type.empty?
    parts << building_type
  end
    
  if not spc_type.empty?
    parts << spc_type
  end
  
  result = parts.join(' - ')
  
  @created_names << result

  return result
end

def longest_name
  sorted_names = @created_names.sort{|x,y| x.size <=> y.size}
  return sorted_names[-1]
end

def make_material(material_name, data, model)  

  material = nil
  material_type = data["material_type"]
  
  if material_type == "StandardOpaqueMaterial"
    material = OpenStudio::Model::StandardOpaqueMaterial.new(model)
    material.setName(material_name)
    
    material.setRoughness(data["roughness"].to_s)
    material.setThickness(OpenStudio::convert(data["thickness"].to_f, "in", "m").get)
    material.setConductivity(OpenStudio::convert(data["conductivity"].to_f, "Btu*in/hr*ft^2*R", "W/m*K").get)
    material.setDensity(OpenStudio::convert(data["density"].to_f, "lb/ft^3", "kg/m^3").get)
    material.setSpecificHeat(OpenStudio::convert(data["specific_heat"].to_f, "Btu/lb*R", "J/kg*K").get)
    material.setThermalAbsorptance(data["thermal_absorptance"].to_f)
    material.setSolarAbsorptance(data["solar_absorptance"].to_f)
    material.setVisibleAbsorptance(data["visible_absorptance"].to_f)
    
  elsif material_type == "MasslessOpaqueMaterial" 
    material = OpenStudio::Model::MasslessOpaqueMaterial.new(model)
    material.setName(material_name)
    
    material.setConductivity(OpenStudio::convert(data["conductivity"].to_f, "Btu*in/hr*ft^2*R", "W/m*K").get)
    material.setDensity(OpenStudio::convert(data["density"].to_f, "lb/ft^3", "kg/m^3").get)
    material.setSpecificHeat(OpenStudio::convert(data["specific_heat"].to_f, "Btu/lb*R", "J/kg*K").get)
    material.setThermalAbsorptance(data["thermal_absorptance"].to_f)
    material.setSolarAbsorptance(data["solar_absorptance"].to_f)
    material.setVisibleAbsorptance(data["visible_absorptance"].to_f)
    
  elsif material_type == "AirGap"
    material = OpenStudio::Model::AirGap.new(model)
    material.setName(material_name)
    
    material.setThermalResistance(OpenStudio::convert(data["resistance"].to_f, "hr*ft^2*R/Btu*in", "m*K/W").get)

  elsif material_type == "Gas"
    material = OpenStudio::Model::Gas.new(model)
    material.setName(material_name)

    material.setThickness(OpenStudio::convert(data["thickness"].to_f, "in", "m").get)
    material.setGasType(data["gas_type"].to_s)
    
  elsif material_type == "SimpleGlazing" 
    material = OpenStudio::Model::SimpleGlazing.new(model)
    material.setName(material_name)
    
    material.setUFactor(OpenStudio::convert(data["u_factor"].to_f, "Btu/hr*ft^2*R", "W/m^2*K").get)
    material.setSolarHeatGainCoefficient(data["solar_heat_gain_coefficient"].to_f)
    material.setVisibleTransmittance(data["visible_transmittance"].to_f)

  elsif material_type == "StandardGlazing" 
    material = OpenStudio::Model::StandardGlazing.new(model)
    material.setName(material_name)
    
    material.setOpticalDataType(data["optical_data_type"].to_s)
    material.setThickness(OpenStudio::convert(data["thickness"].to_f, "in", "m").get)
    material.setSolarTransmittanceatNormalIncidence(data["solar_transmittance_at_normal_incidence"].to_f)
    material.setFrontSideSolarReflectanceatNormalIncidence(data["front_side_solar_reflectance_at_normal_incidence"].to_f)
    material.setBackSideSolarReflectanceatNormalIncidence(data["back_side_solar_reflectance_at_normal_incidence"].to_f)
    material.setVisibleTransmittanceatNormalIncidence(data["visible_transmittance_at_normal_incidence"].to_f)
    material.setFrontSideVisibleReflectanceatNormalIncidence(data["front_side_visible_reflectance_at_normal_incidence"].to_f)
    material.setBackSideVisibleReflectanceatNormalIncidence(data["back_side_visible_reflectance_at_normal_incidence"].to_f)
    material.setInfraredTransmittanceatNormalIncidence(data["infrared_transmittance_at_normal_incidence"].to_f)
    material.setFrontSideInfraredHemisphericalEmissivity(data["front_side_infrared_hemispherical_emissivity"].to_f)
    material.setBackSideInfraredHemisphericalEmissivity(data["back_side_infrared_hemispherical_emissivity"].to_f) 
    material.setConductivity(OpenStudio::convert(data["conductivity"].to_f, "Btu*in/hr*ft^2*R", "W/m*K").get)
    material.setDirtCorrectionFactorforSolarandVisibleTransmittance(data["dirt_correction_factor_for_solar_and_visible_transmittance"].to_f) 
    if /true/i.match(data["solar_diffusing"].to_s)
      material.setSolarDiffusing(true) 
    else
      material.setSolarDiffusing(false)
    end
    
  else
    puts "Unknown material type #{material_type}"
    exit
  end
  
  return material
end

def get_material(material_name, model)  
  # first check model
  model.getMaterials.each do |material|
    if material.name.to_s == material_name
      return material
    end
  end
  
  data = @materials[material_name]
  if data
    material = make_material(material_name, data, model) 
    return material 
  end
  
  puts "Cannot find material named '#{material_name}'"
end

def make_construction(construction_name, data, model)  
  construction = OpenStudio::Model::Construction.new(model)
  construction.setName(construction_name)
  
  standards_info = construction.standardsInformation
  
  intended_surface_type = data["intended_surface_type"]
  if not intended_surface_type
    intended_surface_type = ""
  end
  standards_info.setIntendedSurfaceType(intended_surface_type)
  
  standards_construction_type = data["standards_construction_type"]
  if not standards_construction_type
    standards_construction_type = ""
  end
  standards_info.setStandardsConstructionType(standards_construction_type)
  
  #TODO: could put color in the spreadsheet
  
  layers = OpenStudio::Model::MaterialVector.new
  data["materials"].each do |material_name|
    layers << get_material(material_name, model)
  end
  construction.setLayers(layers)
  
  return construction
end

def get_construction(construction_name, model)  
  # first check model
  model.getConstructions.each do |construction|
    if construction.name.to_s == construction_name
      return construction
    end
  end
  
  data = @constructions[construction_name]
  if data
    construction = make_construction(construction_name, data, model) 
    return construction 
  end
  
  puts "Cannot find construction named '#{construction_name}'"
end

def generate_all_constructions(model = nil)
  if model.nil?
    model = OpenStudio::Model::Model.new
  end
  
  for construction_name in @constructions.keys.sort
    get_construction(construction_name, model)  
  end
end

# pass in a specific climate zone here and get the climate zone set to use for generate_construction_set
def find_climate_zone_set(template, clim, building_type, spc_type)
  possible_climate_zone_sets = []
  
  if tmp1 = @construction_sets[template]
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
def generate_construction_set(template, clim, building_type, spc_type, model = nil)

  if model.nil?
    model = OpenStudio::Model::Model.new
  end

  puts "generating construction set: #{template}-#{clim}-#{building_type}-#{spc_type}"

  data = nil
  if tmp1 = @construction_sets[template]
    if tmp2 = tmp1[clim]
      if tmp3 = tmp2[building_type]
        data = tmp3[spc_type]
      end
    end
  end
  
  if not data
    puts "Cannot find construction set for #{template} #{clim} #{building_type} #{spc_type}"
    exit
  end
  
  name = make_name(template, clim, building_type, spc_type)

  #create a new space type and name it
  construction_set = OpenStudio::Model::DefaultConstructionSet.new(model)
  construction_set.setName(name)
  
  # exterior surfaces constructions
  exterior_surfaces = OpenStudio::Model::DefaultSurfaceConstructions.new(model)
  construction_set.setDefaultExteriorSurfaceConstructions(exterior_surfaces)
  if construction_name = data["exterior_floor"]
    exterior_surfaces.setFloorConstruction(get_construction(construction_name, model))
  end      
  if construction_name = data["exterior_wall"]
    exterior_surfaces.setWallConstruction(get_construction(construction_name, model))
  end
  if construction_name = data["exterior_roof"]
    exterior_surfaces.setRoofCeilingConstruction(get_construction(construction_name, model))
  end    
  
  # interior surfaces constructions
  interior_surfaces = OpenStudio::Model::DefaultSurfaceConstructions.new(model)
  construction_set.setDefaultInteriorSurfaceConstructions(interior_surfaces)
  if construction_name = data["interior_floor"]
    interior_surfaces.setFloorConstruction(get_construction(construction_name, model))
  end      
  if construction_name = data["interior_wall"]
    interior_surfaces.setWallConstruction(get_construction(construction_name, model))
  end
  if construction_name = data["interior_ceiling"]
    interior_surfaces.setRoofCeilingConstruction(get_construction(construction_name, model))
  end  
  
  # ground contact surfaces constructions
  ground_surfaces = OpenStudio::Model::DefaultSurfaceConstructions.new(model)
  construction_set.setDefaultGroundContactSurfaceConstructions(ground_surfaces)
  if construction_name = data["ground_contact_floor"]
    ground_surfaces.setFloorConstruction(get_construction(construction_name, model))
  end      
  if construction_name = data["ground_contact_wall"]
    ground_surfaces.setWallConstruction(get_construction(construction_name, model))
  end
  if construction_name = data["ground_contact_ceiling"]
    ground_surfaces.setRoofCeilingConstruction(get_construction(construction_name, model))
  end
  
  # exterior sub surfaces constructions
  exterior_subsurfaces = OpenStudio::Model::DefaultSubSurfaceConstructions.new(model)
  construction_set.setDefaultExteriorSubSurfaceConstructions(exterior_subsurfaces)
  if construction_name = data["exterior_fixed_window"]
    exterior_subsurfaces.setFixedWindowConstruction(get_construction(construction_name, model))
  end
  if construction_name = data["exterior_operable_window"]
    exterior_subsurfaces.setOperableWindowConstruction(get_construction(construction_name, model))
  end
  if construction_name = data["exterior_door"]
    exterior_subsurfaces.setDoorConstruction(get_construction(construction_name, model))
  end  
  if construction_name = data["exterior_glass_door"]
    exterior_subsurfaces.setGlassDoorConstruction(get_construction(construction_name, model))
  end  
  if construction_name = data["exterior_overhead_door"]
    exterior_subsurfaces.setOverheadDoorConstruction(get_construction(construction_name, model))
  end  
  if construction_name = data["exterior_skylight"]
    exterior_subsurfaces.setOverheadDoorConstruction(get_construction(construction_name, model))
  end  
  if construction_name = data["tubular_daylight_dome"]
    exterior_subsurfaces.setTubularDaylightDomeConstruction(get_construction(construction_name, model))
  end  
  if construction_name = data["tubular_daylight_diffuser"]
    exterior_subsurfaces.setTubularDaylightDiffuserConstruction(get_construction(construction_name, model))
  end  
  
  # interior sub surfaces constructions
  interior_subsurfaces = OpenStudio::Model::DefaultSubSurfaceConstructions.new(model)
  construction_set.setDefaultInteriorSubSurfaceConstructions(interior_subsurfaces)
  if construction_name = data["interior_fixed_window"]
    interior_subsurfaces.setFixedWindowConstruction(get_construction(construction_name, model))
  end
  if construction_name = data["interior_operable_window"]
    interior_subsurfaces.setOperableWindowConstruction(get_construction(construction_name, model))
  end
  if construction_name = data["interior_door"]
    interior_subsurfaces.setDoorConstruction(get_construction(construction_name, model))
  end  
  
  # other constructions
  if construction_name = data["interior_partition"]
    construction_set.setInteriorPartitionConstruction(get_construction(construction_name, model))
  end
  if construction_name = data["space_shading"]
    construction_set.setSpaceShadingConstruction(get_construction(construction_name, model))
  end
  if construction_name = data["building_shading"]
    construction_set.setBuildingShadingConstruction(get_construction(construction_name, model))
  end
  if construction_name = data["site_shading"]
    construction_set.setSiteShadingConstruction(get_construction(construction_name, model))
  end
 
  #componentize the construction set
  construction_set_component = construction_set.createComponent

  #return the construction set and the componentized construction set
  return [construction_set, construction_set_component]
  
end #end generate_construction_set

end #end class ConstructionSetGenerator

