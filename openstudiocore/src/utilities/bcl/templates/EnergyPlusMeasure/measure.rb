# see the URL below for information on how to write OpenStudio measures
# http://nrel.github.io/OpenStudio-user-documentation/reference/measure_writing_guide/

# start the measure
class EnergyPlusMeasureName < OpenStudio::Measure::EnergyPlusMeasure

  # human readable name
  def name
    return "NAME_TEXT"
  end

  # human readable description
  def description
    return "DESCRIPTION_TEXT"
  end

  # human readable description of modeling approach
  def modeler_description
    return "MODELER_DESCRIPTION_TEXT"
  end

  # define the arguments that the user will input
  def arguments(workspace)
    args = OpenStudio::Measure::OSArgumentVector.new

    # the name of the zone to add to the model
    zone_name = OpenStudio::Measure::OSArgument.makeStringArgument("zone_name", true)
    zone_name.setDisplayName("New zone name")
    zone_name.setDescription("This name will be used as the name of the new zone.")
    args << zone_name

    return args
  end 

  # define what happens when the measure is run
  def run(workspace, runner, user_arguments)
    super(workspace, runner, user_arguments)

    # use the built-in error checking 
    if !runner.validateUserArguments(arguments(workspace), user_arguments)
      return false
    end

    # assign the user inputs to variables
    zone_name = runner.getStringArgumentValue("zone_name", user_arguments)

    # check the user_name for reasonableness
    if zone_name.empty?
      runner.registerError("Empty zone name was entered.")
      return false
    end
    
    # get all thermal zones in the starting model
    zones = workspace.getObjectsByType("Zone".to_IddObjectType)

    # reporting initial condition of model
    runner.registerInitialCondition("The building started with #{zones.size} zones.")

    # add a new zone to the model with the new name
    # http://apps1.eere.energy.gov/buildings/energyplus/pdfs/inputoutputreference.pdf#nameddest=Zone
    new_zone_string = "    
    Zone,
      #{zone_name},            !- Name
      0,                       !- Direction of Relative North {deg}
      0,                       !- X Origin {m}
      0,                       !- Y Origin {m}
      0,                       !- Z Origin {m}
      1,                       !- Type
      1,                       !- Multiplier
      autocalculate,           !- Ceiling Height {m}
      autocalculate;           !- Volume {m3}
      "
    idfObject = OpenStudio::IdfObject::load(new_zone_string)
    object = idfObject.get
    wsObject = workspace.addObject(object)
    new_zone = wsObject.get

    # echo the new zone's name back to the user, using the index based getString method
    runner.registerInfo("A zone named '#{new_zone.getString(0)}' was added.")

    # report final condition of model
    finishing_zones = workspace.getObjectsByType("Zone".to_IddObjectType)
    runner.registerFinalCondition("The building finished with #{finishing_zones.size} zones.")
    
    return true
 
  end

end 

# register the measure to be used by the application
EnergyPlusMeasureName.new.registerWithApplication
