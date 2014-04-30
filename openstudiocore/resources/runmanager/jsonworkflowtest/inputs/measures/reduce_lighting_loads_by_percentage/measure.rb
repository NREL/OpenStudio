#start the measure
class ReduceLightingLoadsByPercentage < OpenStudio::Ruleset::ModelUserScript

  #define the name that a user will see
  def name
    return "Reduce Lighting Loads by Percentage"
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
    lighting_power_reduction_percent = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("lighting_power_reduction_percent",true)
    lighting_power_reduction_percent.setDisplayName("Lighting Power Reduction (%).")
    lighting_power_reduction_percent.setDefaultValue(30.0)
    args << lighting_power_reduction_percent

    #make an argument for material and installation cost
    material_and_installation_cost = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("material_and_installation_cost",true)
    material_and_installation_cost.setDisplayName("Increase in Material and Installation Cost for Lighting per Floor Area (%).")
    material_and_installation_cost.setDefaultValue(0.0)
    args << material_and_installation_cost

    #make an argument for demolition cost
    demolition_cost = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("demolition_cost",true)
    demolition_cost.setDisplayName("Increase in Demolition Costs for Lighting per Floor Area (%).")
    demolition_cost.setDefaultValue(0.0)
    args << demolition_cost

    #make an argument for years until costs start
    years_until_costs_start = OpenStudio::Ruleset::OSArgument::makeIntegerArgument("years_until_costs_start",true)
    years_until_costs_start.setDisplayName("Years Until Costs Start (whole years).")
    years_until_costs_start.setDefaultValue(0)
    args << years_until_costs_start

    #make a choice argument for when demo costs occur
    demo_cost_initial_const = OpenStudio::Ruleset::OSArgument::makeBoolArgument("demo_cost_initial_const",true)
    demo_cost_initial_const.setDisplayName("Demolition Costs Occur During Initial Construction?")
    demo_cost_initial_const.setDefaultValue(false)
    args << demo_cost_initial_const

    #make an argument for expected life
    expected_life = OpenStudio::Ruleset::OSArgument::makeIntegerArgument("expected_life",true)
    expected_life.setDisplayName("Expected Life (whole years).")
    expected_life.setDefaultValue(15)
    args << expected_life

    #make an argument for O & M cost
    om_cost = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("om_cost",true)
    om_cost.setDisplayName("Increase O & M Costs for Lighting per Floor Area (%).")
    om_cost.setDefaultValue(0.0)
    args << om_cost

    #make an argument for O & M frequency
    om_frequency = OpenStudio::Ruleset::OSArgument::makeIntegerArgument("om_frequency",true)
    om_frequency.setDisplayName("O & M Frequency (whole years).")
    om_frequency.setDefaultValue(1)
    args << om_frequency

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
    lighting_power_reduction_percent = runner.getDoubleArgumentValue("lighting_power_reduction_percent",user_arguments)
    material_and_installation_cost = runner.getDoubleArgumentValue("material_and_installation_cost",user_arguments)
    demolition_cost = runner.getDoubleArgumentValue("demolition_cost",user_arguments)
    years_until_costs_start = runner.getIntegerArgumentValue("years_until_costs_start",user_arguments)
    demo_cost_initial_const = runner.getBoolArgumentValue("demo_cost_initial_const",user_arguments)
    expected_life = runner.getIntegerArgumentValue("expected_life",user_arguments)
    om_cost = runner.getDoubleArgumentValue("om_cost",user_arguments)
    om_frequency = runner.getIntegerArgumentValue("om_frequency",user_arguments)

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

    #check the lighting_power_reduction_percent and for reasonableness
    if lighting_power_reduction_percent > 100
      runner.registerError("Please Enter a Value less than or equal to 100 for the Lighting Power Reduction Percentage.")
      return false
    elsif lighting_power_reduction_percent == 0
      runner.registerInfo("No lighting power adjustment requested, but some life cycle costs may still be affected.")
    elsif lighting_power_reduction_percent < 1 and lighting_power_reduction_percent > -1
      runner.registerWarning("A Lighting Power Reduction Percentage of #{lighting_power_reduction_percent} percent is abnormally low.")
    elsif lighting_power_reduction_percent > 90
      runner.registerWarning("A Lighting Power Reduction Percentage of #{lighting_power_reduction_percent} percent is abnormally high.")
    elsif lighting_power_reduction_percent < 0
      runner.registerInfo("The requested value for lighting power reduction percentage was negative. This will result in an increase in lighting power.")
    end

   #check lifecycle cost arguments for reasonableness
    if material_and_installation_cost < -100
      runner.registerError("Material and Installation Cost percentage increase can't be less than -100.")
      return false
    end

    if demolition_cost < -100
      runner.registerError("Demolition Cost percentage increase can't be less than -100.")
      return false
    end

    if years_until_costs_start < 0
      runner.registerError("Enter an integer greater than or equal to 0 for Years Until Costs Start.")
      return false
    end

    if expected_life < 1
      runner.registerError("Enter an integer greater than or equal to 1 for Expected Life.")
      return false
    end

    if om_cost < -100
      runner.registerError("O & M Cost percentage increase can't be less than -100.")
      return false
    end

    if om_frequency < 1
      runner.registerError("Choose an integer greater than 0 for O & M Frequency.")
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

    #helper to make it easier to do unit conversions on the fly.  The definition be called through this measure.
    def unit_helper(number,from_unit_string,to_unit_string)
      converted_number = OpenStudio::convert(OpenStudio::Quantity.new(number, OpenStudio::createUnit(from_unit_string).get), OpenStudio::createUnit(to_unit_string).get).get.value
    end

    #helper that loops through lifecycle costs getting total costs under "Construction" or "Salvage" category and add to counter if occurs during year 0
    def get_total_costs_for_objects(objects)
      counter = 0
      objects.each do |object|
        object_LCCs = object.lifeCycleCosts
        object_LCCs.each do |object_LCC|
          if object_LCC.category == "Construction" or object_LCC.category == "Salvage"
            if object_LCC.yearsFromStart == 0
              counter += object_LCC.totalCost
            end
          end
        end
      end
      return counter
    end #end of def get_total_costs_for_objects(objects)

    #counter for demo cost of baseline objects
    demo_costs_of_baseline_objects = 0

    #counter for year 0 capital costs
    yr0_capital_totalCosts = 0

    #get initial light and luminaire costs and multiply by -1
    yr0_capital_totalCosts +=  get_total_costs_for_objects(model.getLightsDefinitions)*-1
    yr0_capital_totalCosts += get_total_costs_for_objects(model.getLuminaireDefinitions)*-1

    #report initial condition
    building = model.getBuilding
    building_lighting_power = building.lightingPower
    building_LPD =  unit_helper(building.lightingPowerPerFloorArea,"W/m^2","W/ft^2")
    runner.registerInitialCondition("The model's initial building lighting power was  #{neat_numbers(building_lighting_power,0)} watts, a lighting power density of #{neat_numbers(building_LPD)} w/ft^2.")

    #get space types in model
    if apply_to_building
      space_types = model.getSpaceTypes
    else
      space_types = []
      space_types << space_type #only run on a single space type
    end

    #helper def to add to demo cost related to baseline objects
    def add_to_baseline_demo_cost_counter(baseline_object, demo_cost_initial_const)
      counter = 0
      if demo_cost_initial_const == true
        baseline_object_LCCs = baseline_object.lifeCycleCosts
        baseline_object_LCCs.each do |baseline_object_LCC|
          if baseline_object_LCC.category == "Salvage"
            counter += baseline_object_LCC.totalCost
          end
        end
      end
      return counter
    end #end of def add_to_baseline_demo_cost_counter

    #def to alter performance and life cycle costs of objects
    def alter_performance_and_lcc(object, lighting_power_reduction_percent, material_and_installation_cost, demolition_cost, om_cost, years_until_costs_start, expected_life, om_frequency, runner)

      #edit clone based on percentage reduction
      new_def = object
      if not new_def.lightingLevel.empty?
        new_lighting_level = new_def.setLightingLevel(new_def.lightingLevel.get - new_def.lightingLevel.get*lighting_power_reduction_percent*0.01)
      elsif not new_def.wattsperSpaceFloorArea.empty?
        new_lighting_per_area = new_def.setWattsperSpaceFloorArea(new_def.wattsperSpaceFloorArea.get - new_def.wattsperSpaceFloorArea.get*lighting_power_reduction_percent*0.01)
      elsif not new_def.wattsperPerson.empty?
        new_lighting_per_person = new_def.setWattsperPerson(new_def.wattsperPerson.get - new_def.wattsperPerson.get*lighting_power_reduction_percent*0.01)
      else
        runner.registerWarning("'#{new_def.name}' is used by one or more instances and has no load values. Its performance was not altered.")
      end

      new_def_LCCs = new_def.lifeCycleCosts
      if new_def_LCCs.size == 0
        if not material_and_installation_cost.abs + demolition_cost.abs + om_cost.abs == 0
          runner.registerWarning("'#{new_def.name}' had no life cycle cost objects. No cost was added for it.")
        end
      else
        new_def_LCCs.each do |new_def_LCC|
          if new_def_LCC.category == "Construction"
            new_def_LCC.setCost(new_def_LCC.cost * (1 + material_and_installation_cost/100))
            new_def_LCC.setYearsFromStart(years_until_costs_start) #just uses argument value, does not need existing value
            new_def_LCC.setRepeatPeriodYears(expected_life) #just uses argument value, does not need existing value
          elsif new_def_LCC.category == "Salvage"
            new_def_LCC.setCost(new_def_LCC.cost * (1 + demolition_cost/100))
            new_def_LCC.setYearsFromStart(years_until_costs_start + expected_life) #just uses argument value, does not need existing value
            new_def_LCC.setRepeatPeriodYears(expected_life) #just uses argument value, does not need existing value
          elsif new_def_LCC.category == "Maintenance"
            new_def_LCC.setCost(new_def_LCC.cost * (1 + om_cost/100))
            new_def_LCC.setRepeatPeriodYears(om_frequency) #just uses argument value, does not need existing value
          end

          #reset any month durations
          new_def_LCC.resetRepeatPeriodMonths
          new_def_LCC.resetMonthsFromStart
        end

      end #end of new_def_LCCs.each do |new_def_LCC|

    end #end of def alter_performance_and_lcc()

    #make a hash of old defs and new lights and luminaire defs
    cloned_lights_defs = {}
    cloned_luminaire_defs = {}

    #loop through space types
    space_types.each do |space_type|
      next if not space_type.spaces.size > 0
      space_type_lights = space_type.lights
      space_type_lights.each do |space_type_light|

        new_def = nil

        #clone def if it has not already been cloned
        exist_def = space_type_light.lightsDefinition
        if not cloned_lights_defs[exist_def.name.to_s].nil?
          new_def = cloned_lights_defs[exist_def.name.to_s]
        else
          #clone rename and add to hash
          new_def = exist_def.clone(model)
          new_def_name = new_def.setName("#{exist_def.name} - #{lighting_power_reduction_percent} percent reduction")
          new_def = new_def.to_LightsDefinition.get
          cloned_lights_defs[exist_def.name.to_s] = new_def

          #add demo cost of object being removed to one counter for one time demo cost for baseline objects
          demo_costs_of_baseline_objects += add_to_baseline_demo_cost_counter(exist_def, demo_cost_initial_const)

          #call def to alter performance and life cycle costs
          alter_performance_and_lcc(new_def, lighting_power_reduction_percent, material_and_installation_cost, demolition_cost, om_cost, years_until_costs_start, expected_life, om_frequency, runner)

        end #end if cloned_lights_defs.any?

        #link instance with clone and rename
        updated_instance = space_type_light.setLightsDefinition(new_def.to_LightsDefinition.get)
        updated_instance_name = space_type_light.setName("#{space_type_light.name} #{lighting_power_reduction_percent} percent reduction")

      end #end space_type_lights.each do

      space_type_luminaires = space_type.luminaires
      space_type_luminaires.each do |space_type_luminaire|

        #clone def if it has not already been cloned
        exist_def = space_type_luminaire.luminaireDefinition
        if cloned_luminaire_defs.any? {|k,v| k.include? exist_def.name}
          new_def = cloned_luminaire_defs[exist_def.name]
        else
          #clone rename and add to hash
          new_def = exist_def.clone(model)
          new_def_name = new_def.setName("#{new_def.name} - #{lighting_power_reduction_percent} percent reduction")
          cloned_luminaire_defs[exist_def.name] = new_def
          new_def = new_def.to_LightsDefinition.get

          #add demo cost of object being removed to one counter for one time demo cost for baseline objects
          demo_costs_of_baseline_objects += add_to_baseline_demo_cost_counter(exist_def, demo_cost_initial_const)

          #call def to alter performance and life cycle costs
          alter_performance_and_lcc(new_def, lighting_power_reduction_percent, material_and_installation_cost, demolition_cost, om_cost, years_until_costs_start, expected_life, om_frequency, runner)

        end #end cloned_luminaire_defs.any?

        #link instance with clone and rename
        updated_instance = space_type_light.setLightsDefinition(new_def.to_LightsDefinition.get)
        updated_instance_name = space_type_light.setName("#{space_type_light.name} #{lighting_power_reduction_percent} percent reduction")

      end #end space_type_luminaires.each do

    end #end space types each do

    #getting spaces in the model
    spaces = model.getSpaces

    #get space types in model
    if apply_to_building
      spaces = model.getSpaces
    else
      if not space_type.spaces.empty?
        spaces = space_type.spaces #only run on a single space type
      end
    end

    spaces.each do |space|
      space_lights = space.lights
      space_lights.each do |space_light|

        #clone def if it has not already been cloned
        exist_def = space_light.lightsDefinition
        if cloned_lights_defs.any? {|k,v| k.include? exist_def.name.to_s}
          new_def = cloned_lights_defs[exist_def.name.to_s]
        else
          #clone rename and add to hash
          new_def = exist_def.clone(model)
          new_def_name = new_def.setName("#{new_def.name} - #{lighting_power_reduction_percent} percent reduction")
          cloned_lights_defs[exist_def.name] = new_def
          new_def = new_def.to_LightsDefinition.get

          #add demo cost of object being removed to one counter for one time demo cost for baseline objects
          demo_costs_of_baseline_objects += add_to_baseline_demo_cost_counter(exist_def, demo_cost_initial_const)

          #call def to alter performance and life cycle costs
          alter_performance_and_lcc(new_def, lighting_power_reduction_percent, material_and_installation_cost, demolition_cost, om_cost, years_until_costs_start, expected_life, om_frequency, runner)

        end #end if cloned_lights_defs.any?

        #link instance with clone and rename
        updated_instance = space_light.setLightsDefinition(new_def.to_LightsDefinition.get)
        updated_instance_name = space_light.setName("#{space_light.name} #{lighting_power_reduction_percent} percent reduction")

      end #end space_lights.each do

      space_luminaires = space.luminaires
      space_luminaires.each do |space_luminaire|

        #clone def if it has not already been cloned
        exist_def = space_luminaire.luminaireDefinition
        if cloned_luminaire_defs.any? {|k,v| k.include? exist_def.name}
          new_def = cloned_luminaire_defs[exist_def.name]
        else
          #clone rename and add to hash
          new_def = exist_def.clone(model)
          new_def_name = new_def.setName("#{new_def.name} - #{lighting_power_reduction_percent} percent reduction")
          cloned_luminaire_defs[exist_def.name] = new_def
          new_def = new_def.to_LightsDefinition.get

          #add demo cost of object being removed to one counter for one time demo cost for baseline objects
          demo_costs_of_baseline_objects += add_to_baseline_demo_cost_counter(exist_def, demo_cost_initial_const)

          #call def to alter performance and life cycle costs
          alter_performance_and_lcc(new_def, lighting_power_reduction_percent, material_and_installation_cost, demolition_cost, om_cost, years_until_costs_start, expected_life, om_frequency, runner)

        end #end cloned_luminaire_defs.any?

        #link instance with clone and rename
        updated_instance = space_light.setLightsDefinition(new_def)
        updated_instance_name = space_light.setName("#{space_light.name} - #{lighting_power_reduction_percent} percent reduction")

      end #end space_type_luminaires.each do
    end #end of loop through spaces

    if cloned_lights_defs.size == 0 and cloned_luminaire_defs.size == 0
      runner.registerAsNotApplicable("No lighting or luminaire objects were found in the specified space type(s).")
    end

    #get final light and luminaire costs to use in final condition
    yr0_capital_totalCosts +=  get_total_costs_for_objects(model.getLightsDefinitions)
    yr0_capital_totalCosts += get_total_costs_for_objects(model.getLuminaireDefinitions)

    #add one time demo cost of removed lights and luminaires if appropriate
    if demo_cost_initial_const == true
      building = model.getBuilding
      lcc_baseline_demo = OpenStudio::Model::LifeCycleCost.createLifeCycleCost("LCC_baseline_demo", building, demo_costs_of_baseline_objects, "CostPerEach", "Salvage", 0, years_until_costs_start).get #using 0 for repeat period since one time cost.
      runner.registerInfo("Adding one time cost of $#{neat_numbers(lcc_baseline_demo.totalCost,0)} related to demolition of baseline objects.")

      #if demo occurs on year 0 then add to initial capital cost counter
      if lcc_baseline_demo.yearsFromStart == 0
        yr0_capital_totalCosts += lcc_baseline_demo.totalCost
      end
    end

    #report final condition
    final_building = model.getBuilding
    final_building_lighting_power = final_building.lightingPower
    final_building_LPD =  unit_helper(final_building.lightingPowerPerFloorArea,"W/m^2","W/ft^2")
    runner.registerFinalCondition("The model's final final lighting power was  #{neat_numbers(final_building_lighting_power,0)} watts, a lighting power density of #{neat_numbers(final_building_LPD)} w/ft^2. Initial capital costs associated with the improvements are $#{neat_numbers(yr0_capital_totalCosts,0)}.")

    return true

  end #end the run method

end #end the measure

#this allows the measure to be used by the application
ReduceLightingLoadsByPercentage.new.registerWithApplication