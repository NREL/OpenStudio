#see the URL below for information on how to write OpenStudio measures
# http://openstudio.nrel.gov/openstudio-measure-writing-guide

#see the URL below for information on using life cycle cost objects in OpenStudio
# http://openstudio.nrel.gov/openstudio-life-cycle-examples

#see the URL below for access to C++ documentation on model objects (click on "model" in the main window to view model objects)
# http://openstudio.nrel.gov/sites/openstudio.nrel.gov/files/nv_data/cpp_documentation_it/model/html/namespaces.html

#start the measure
class ReduceVentilationByPercentage < OpenStudio::Ruleset::ModelUserScript
  
  #define the name that a user will see, this method may be deprecated as
  #the display name in PAT comes from the name field in measure.xml
  def name
    return "ReduceVentilationByPercentage"
  end

  #define the arguments that the user will input
  def arguments(model)
    args = OpenStudio::Ruleset::OSArgumentVector.new

    #make a choice argument for model objects
    space_type_handles = OpenStudio::StringVector.new
    space_type_display_names = OpenStudio::StringVector.new

    #putting model object and names into hash
    space_type_args = model.getSpaceTypes
    space_type_args_hash = {}
    space_type_args.each do |space_type_arg|
      space_type_args_hash[space_type_arg.name.to_s] = space_type_arg
    end

    #looping through sorted hash of model objects
    space_type_args_hash.sort.map do |key,value|
      #only include if space type is used in the model
      if value.spaces.size > 0
        space_type_handles << value.handle.to_s
        space_type_display_names << key
      end
    end

    #add building to string vector with space type
    building = model.getBuilding
    space_type_handles << building.handle.to_s
    space_type_display_names << "*Entire Building*"

    #make a choice argument for space type
    space_type = OpenStudio::Ruleset::OSArgument::makeChoiceArgument("space_type", space_type_handles, space_type_display_names)
    space_type.setDisplayName("Apply the Measure to a Specific Space Type or to the Entire Model.")
    space_type.setDefaultValue("*Entire Building*") #if no space type is chosen this will run on the entire building
    args << space_type

    #make an argument for reduction percentage
    design_spec_outdoor_air_reduction_percent = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("design_spec_outdoor_air_reduction_percent",true)
    design_spec_outdoor_air_reduction_percent.setDisplayName("Design Specification Outdoor Air Reduction (%).")
    design_spec_outdoor_air_reduction_percent.setDefaultValue(30.0)
    args << design_spec_outdoor_air_reduction_percent

    #no cost required to reduce required amount of outdoor air. Cost increase or decrease will relate to system sizing and ongoing energy use due to change in outdoor air provided.

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
    object = runner.getOptionalWorkspaceObjectChoiceValue("space_type",user_arguments,model)
    design_spec_outdoor_air_reduction_percent = runner.getDoubleArgumentValue("design_spec_outdoor_air_reduction_percent",user_arguments)

    #check the space_type for reasonableness and see if measure should run on space type or on the entire building
    apply_to_building = false
    space_type = nil
    if object.empty?
      handle = runner.getStringArgumentValue("space_type",user_arguments)
      if handle.empty?
        runner.registerError("No space type was chosen.")
      else
        runner.registerError("The selected space type with handle '#{handle}' was not found in the model. It may have been removed by another measure.")
      end
      return false
    else
      if not object.get.to_SpaceType.empty?
        space_type = object.get.to_SpaceType.get
      elsif not object.get.to_Building.empty?
        apply_to_building = true
      else
        runner.registerError("Script Error - argument not showing up as space type or building.")
        return false
      end
    end

    #check the design_spec_outdoor_air_reduction_percent and for reasonableness
    if design_spec_outdoor_air_reduction_percent > 100
      runner.registerError("Please enter a value less than or equal to 100 for the Outdoor Air Requirement reduction percentage.")
      return false
    elsif design_spec_outdoor_air_reduction_percent == 0
      runner.registerInfo("No Outdoor Air Requirement adjustment requested, but some life cycle costs may still be affected.")
    elsif design_spec_outdoor_air_reduction_percent < 1 and design_spec_outdoor_air_reduction_percent > -1
      runner.registerWarning("A Outdoor Air Requirement reduction percentage of #{design_spec_outdoor_air_reduction_percent} percent is abnormally low.")
    elsif design_spec_outdoor_air_reduction_percent > 90
      runner.registerWarning("A Outdoor Air Requirement reduction percentage of #{design_spec_outdoor_air_reduction_percent} percent is abnormally high.")
    elsif design_spec_outdoor_air_reduction_percent < 0
      runner.registerInfo("The requested value for Outdoor Air Requirement reduction percentage was negative. This will result in an increase in the Outdoor Air Requirement.")
    end

    #helper to make numbers pretty (converts 4125001.25641 to 4,125,001.26 or 4,125,001). The definition be called through this measure.
    def neat_numbers(number, roundto = 2) #round to 0 or 2)
      if roundto == 2
        number = sprintf "%.2f", number
      else
        number = number.round
      end
      #regex to add commas
      number.to_s.reverse.gsub(%r{([0-9]{3}(?=([0-9])))}, "\\1,").reverse
    end #end def neat_numbers

    #get space design_spec_outdoor_air_objects objects used in the model
    design_spec_outdoor_air_objects = model.getDesignSpecificationOutdoorAirs
    #todo - it would be nice to give ranges for different calculation methods but would take some work.

    #counters needed for measure
    altered_instances = 0

    #reporting initial condition of model
    if design_spec_outdoor_air_objects.size > 0
      runner.registerInitialCondition("The initial model contained #{design_spec_outdoor_air_objects.size} design specification outdoor air objects.")
    else
      runner.registerInitialCondition("The initial model did not contain any design specification outdoor air.")
    end

    #get space types in model
    building = model.building.get
    if apply_to_building
      space_types = model.getSpaceTypes
      affected_area_si = building.floorArea
    else
      space_types = []
      space_types << space_type #only run on a single space type
      affected_area_si = space_type.floorArea
    end

    #split apart any shared uses of design specification outdoor air
    design_spec_outdoor_air_objects.each do |design_spec_outdoor_air_object|
      direct_use_count = design_spec_outdoor_air_object.directUseCount
      next if not direct_use_count > 1
      direct_uses = design_spec_outdoor_air_object.sources
      original_cloned = false

      #adjust count test for direct uses that are component data
      direct_uses.each do |direct_use|
        component_data_source = direct_use.to_ComponentData
        if not component_data_source.empty?
          direct_use_count = direct_use_count -1
        end
      end
      next if not direct_use_count > 1

      direct_uses.each do |direct_use|

        #clone and hookup design spec OA
        space_type_source = direct_use.to_SpaceType
        if not space_type_source.empty?
          space_type_source = space_type_source.get
          cloned_object = design_spec_outdoor_air_object.clone
          space_type_source.setDesignSpecificationOutdoorAir(cloned_object.to_DesignSpecificationOutdoorAir.get)
          original_cloned = true
        end

        space_source = direct_use.to_Space
        if not space_source.empty?
          space_source = space_source.get
          cloned_object = design_spec_outdoor_air_object.clone
          space_source.setDesignSpecificationOutdoorAir(cloned_object.to_DesignSpecificationOutdoorAir.get)
          original_cloned = true
        end

      end #end of direct_uses.each do

      #delete the now unused design spec OA
      if original_cloned
        runner.registerInfo("Making shared object #{design_spec_outdoor_air_object.name} unique.")
        design_spec_outdoor_air_object.remove
      end

    end #end of design_spec_outdoor_air_objects.each do

    #def to alter performance and life cycle costs of objects
    def alter_performance(object, design_spec_outdoor_air_reduction_percent, runner)

      #edit instance based on percentage reduction
      instance = object

      #not checking if fields are empty because these are optional like values for space infiltration are.
      new_outdoor_air_per_person = instance.setOutdoorAirFlowperPerson(instance.outdoorAirFlowperPerson - instance.outdoorAirFlowperPerson*design_spec_outdoor_air_reduction_percent*0.01)
      new_outdoor_air_per_floor_area = instance.setOutdoorAirFlowperFloorArea(instance.outdoorAirFlowperFloorArea - instance.outdoorAirFlowperFloorArea*design_spec_outdoor_air_reduction_percent*0.01)
      new_outdoor_air_ach = instance.setOutdoorAirFlowAirChangesperHour(instance.outdoorAirFlowAirChangesperHour - instance.outdoorAirFlowAirChangesperHour*design_spec_outdoor_air_reduction_percent*0.01)

    end #end of def alter_performance()

    #array of instances to change
    instances_array = []

    #loop through space types
    space_types.each do |space_type|
      next if not space_type.spaces.size > 0
      instances_array << space_type.designSpecificationOutdoorAir
    end #end space types each do

    #get spaces in model
    if apply_to_building
      spaces = model.getSpaces
    else
      if not space_type.spaces.empty?
        spaces = space_type.spaces #only run on a single space type
      end
    end

    spaces.each do |space|
      instances_array << space.designSpecificationOutdoorAir
    end #end of loop through spaces

    instance_processed = []

    instances_array.each do |instance|

      next if instance.empty?
      instance = instance.get

      #only continue if this instance has not been processed yet
      next if instance_processed.include? instance
      instance_processed << instance

      #call def to alter performance and life cycle costs
      alter_performance(instance, design_spec_outdoor_air_reduction_percent, runner)

      #rename
      updated_instance_name = instance.setName("#{instance.name} (#{design_spec_outdoor_air_reduction_percent} percent reduction)")
      altered_instances += 1

    end

    if altered_instances == 0
      runner.registerAsNotApplicable("No design specification outdoor air objects were found in the specified space type(s).")
    end

    #report final condition
    affected_area_ip = OpenStudio::convert(affected_area_si,"m^2","ft^2").get
    runner.registerFinalCondition("#{altered_instances} design specification outdoor air objects in the model were altered affecting #{neat_numbers(affected_area_ip,0)}(ft^2).")

    return true

  end #end the run method

end #end the measure

#this allows the measure to be use by the application
ReduceVentilationByPercentage.new.registerWithApplication