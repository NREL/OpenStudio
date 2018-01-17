#see the URL below for information on how to write OpenStudio measures
# http://nrel.github.io/OpenStudio-user-documentation/reference/measure_writing_guide/

#see the URL below for information on using life cycle cost objects in OpenStudio
# http://nrel.github.io/OpenStudio-user-documentation/tutorials/life_cycle_costing_examples/

#see the URL below for access to C++ documentation on model objects (click on "model" in the main window to view model objects)
# https://s3.amazonaws.com/openstudio-sdk-documentation/cpp/OpenStudio-1.5.0-doc/model/html/classes.html

require "#{File.dirname(__FILE__)}/resources/SpaceTypeGenerator"
require "#{File.dirname(__FILE__)}/resources/ConstructionSetGenerator"
require "#{File.dirname(__FILE__)}/resources/OsLib_Constructions"

#start the measure
class SpaceTypeAndConstructionSetWizard < OpenStudio::Ruleset::ModelUserScript

  #define the name that a user will see, this method may be deprecated as
  #the display name in PAT comes from the name field in measure.xml
  def name
    return "Space Type and Construction Set Wizard"
  end

  #define the arguments that the user will input
  def arguments(model)
    args = OpenStudio::Ruleset::OSArgumentVector.new

    # load the JSON standards file
    temp = File.read("#{File.dirname(__FILE__)}/resources/OpenStudio_Standards.json")
    standards = JSON.parse(temp)

    building_types = []
    templates = []
    climate_zones = []

    standards["space_types"].each_key do |template|
      templates << template
      standards["space_types"][template].each_key do |climate_zone_set|
        standards["space_types"][template][climate_zone_set].each_key do |building_type|
          building_types << building_type
        end
      end
    end

    standards["construction_sets"].each_key do |template|
      templates << template
      standards["space_types"][template].each_key do |climate_zone|
        standards["space_types"][template][climate_zone].each_key do |building_type|
          building_types << building_type
        end
      end
    end

    standards["climate_zones"].each_key do |climate_zone|
      next if climate_zone.include? "CEC"
      climate_zones << climate_zone
    end

    building_types.uniq!
    templates.uniq!
    climate_zones.uniq!

    #make an argument for building type
    buildingType = OpenStudio::Ruleset::OSArgument::makeChoiceArgument("buildingType", building_types, true)
    buildingType.setDisplayName("Building Type")
    buildingType.setDefaultValue("Office")
    args << buildingType

    #make an argument for vintage
    template = OpenStudio::Ruleset::OSArgument::makeChoiceArgument("template", templates, true) # vintage or standard for building
    template.setDisplayName("Template")
    template.setDefaultValue("189.1-2009")
    args << template

    #make an argument for climate zone
    climateZone = OpenStudio::Ruleset::OSArgument::makeChoiceArgument("climateZone", climate_zones, true)
    climateZone.setDisplayName("ASHRAE Climate Zone")
    climateZone.setDefaultValue("ASHRAE 169-2006-5B")
    args << climateZone

    #make an argument to add new space types
    createSpaceTypes = OpenStudio::Ruleset::OSArgument::makeBoolArgument("createSpaceTypes",true)
    createSpaceTypes.setDisplayName("Create Space Types?")
    createSpaceTypes.setDefaultValue(true)
    args << createSpaceTypes

    #make an argument to add new construction set
    createConstructionSet = OpenStudio::Ruleset::OSArgument::makeBoolArgument("createConstructionSet",true)
    createConstructionSet.setDisplayName("Create Construction Set?")
    createConstructionSet.setDefaultValue(true)
    args << createConstructionSet

    #make an argument to determine if building defaults should be set
    setBuildingDefaults = OpenStudio::Ruleset::OSArgument::makeBoolArgument("setBuildingDefaults",true)
    setBuildingDefaults.setDisplayName("Set Building Defaults Using New Objects?")
    setBuildingDefaults.setDefaultValue(true)
    args << setBuildingDefaults

    #todo - let user choose specific construction type for exterior wall, roof, slab, and interior floors

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
    building_type = runner.getStringArgumentValue("buildingType",user_arguments)
    template = runner.getStringArgumentValue("template",user_arguments)
    climate = runner.getStringArgumentValue("climateZone",user_arguments)
    createSpaceTypes = runner.getBoolArgumentValue("createSpaceTypes",user_arguments)
    createConstructionSet = runner.getBoolArgumentValue("createConstructionSet",user_arguments)
    setBuildingDefaults = runner.getBoolArgumentValue("setBuildingDefaults",user_arguments)

    #reporting initial condition of model
    starting_spaceTypes = model.getSpaceTypes
    starting_constructionSets = model.getDefaultConstructionSets
    runner.registerInitialCondition("The building started with #{starting_spaceTypes.size} space types and #{starting_constructionSets.size} construction sets.")

    # added for the benefit of SketchUp UserScript workflow
    puts "The building started with #{starting_spaceTypes.size} space types and #{starting_constructionSets.size} construction sets."

    #setup resources
    path_to_standards_json = "#{File.dirname(__FILE__)}/resources/OpenStudio_Standards.json"
    path_to_master_schedules_library = "#{File.dirname(__FILE__)}/resources/Master_Schedules.osm"

    #create generators
    space_type_generator = SpaceTypeGenerator.new(path_to_standards_json, path_to_master_schedules_library)
    construction_set_generator = ConstructionSetGenerator.new(path_to_standards_json)

    #load the data from the JSON file into a ruby hash
    standards = {}
    temp = File.read(path_to_standards_json)
    standards = JSON.parse(temp)
    space_types = standards["space_types"]
    construction_sets = standards["construction_sets"]

    #define default space types for each building type (I was going to require CreateTemplateModels, but that created issues, so I just copied the hash I need)
    default_space_type = Hash.new
    default_space_type["FullServiceRestaurant"] = "Dining"
    default_space_type["Hospital"] = "PatRoom"
    default_space_type["LargeHotel"] = "GuestRoom"
    default_space_type["MidriseApartment"] = "Apartment"
    default_space_type["Office"] = "OpenOffice"
    default_space_type["Outpatient"] = "Exam"
    default_space_type["PrimarySchool"] = "Classroom"
    default_space_type["QuickServiceRestaurant"] = "Dining"
    default_space_type["Retail"] = "Retail"
    default_space_type["SecondarySchool"] = "Classroom"
    default_space_type["SmallHotel"] = "GuestRoom"
    default_space_type["StripMall"] = "WholeBuilding"
    default_space_type["SuperMarket"] = "Sales/Produce"
    default_space_type["Warehouse"] = "Bulk"

    # add space types
    if createSpaceTypes
      for t in space_types.keys.sort
        next if not t == template

        for c in space_types[template].keys.sort
          next if not space_type_generator.is_climate_zone_in_climate_zone_set(climate, c)

          for b in space_types[template][c].keys.sort
            next if not b == building_type

            runner.registerInfo("Creating Space Types for: #{t}, #{c}, #{building_type}.")

            for space_type in space_types[template][c][building_type].keys.sort
              #generate space type
              result = space_type_generator.generate_space_type(template, c, building_type, space_type, model)
              runner.registerInfo("Creating '#{result[0].name}'.")

              #if space type matches hash for this building type set as building default
              default = default_space_type["#{building_type}"]
              if default == space_type and setBuildingDefaults
                runner.registerInfo("Setting Default Space Type for the Building.")

                # added for the benefit of SketchUp UserScript workflow
                puts "Setting Default Space Type for the Building."

                model.getBuilding.setSpaceType(result[0])
              end
            end #next space type
          end #next building type
        end #next climate
      end #next template
    end

    # get climate zone set from specific climate zone for construction set
    climateConst = construction_set_generator.find_climate_zone_set(template, climate, building_type, "")

    # add construction set
    if createConstructionSet

      for t in construction_sets.keys.sort
        next if not t == template
        for c in construction_sets[template].keys.sort
          next if not c == climateConst
          for b in construction_sets[template][climateConst].keys.sort
            next if not b == building_type

            for space_type in construction_sets[template][climateConst][building_type].keys.sort
              #generate construction set
              result = construction_set_generator.generate_construction_set(template, climateConst, building_type, space_type, model)
              runner.registerInfo("Creating Construction Set: #{result[0].name}")

              # list constructions used in construction set
              constructions = OsLib_Constructions.reportConstructionSetConstructions(result[0])
              constructions.uniq.each do |construction|
                runner.registerInfo("Creating '#{construction.name}'")
              end

              # set default construction set
              if setBuildingDefaults
                runner.registerInfo("Setting Default Construction Set for the Building.")
                model.getBuilding.setDefaultConstructionSet(result[0])
              end

            end #next space type
          end #next building type
        end #next climate
      end #next template
    end

    # thermostat schedules are brought in but cant be assigned until later on when there are thermal zones

    #reporting final condition of model
    finishing_spaceTypes = model.getSpaceTypes
    finishing_constructionSets = model.getDefaultConstructionSets

    # updated for the benefit of SketchUp UserScript workflow
    runner.registerFinalCondition("The model has #{finishing_spaceTypes.size} space types and #{finishing_constructionSets.size} construction sets.")

    return true

  end #end the run method

end #end the measure

#this allows the measure to be use by the application
SpaceTypeAndConstructionSetWizard.new.registerWithApplication