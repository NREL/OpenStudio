#start the measure
class RotateBuilding < OpenStudio::Ruleset::ModelUserScript

  #define the name that a user will see
  def name
    return "Rotate Building"
  end

  #define the arguments that the user will input
  def arguments(model)
    args = OpenStudio::Ruleset::OSArgumentVector.new

    #make an argument for your name
    relative_building_rotation = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("relative_building_rotation",true)
    relative_building_rotation.setDisplayName("Number of Degrees to Rotate Building (positive value is clockwise).")
    relative_building_rotation.setDefaultValue(90.0)
    args << relative_building_rotation

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
    relative_building_rotation = runner.getDoubleArgumentValue("relative_building_rotation",user_arguments)

    #check the relative_building_rotation for reasonableness
    if relative_building_rotation > 360 or relative_building_rotation < -360
      relative_building_rotation = relative_building_rotation -360.0*(relative_building_rotation/360.0).truncate
      runner.registerWarning("Requested rotation was not between -360 and 360. Effective relative rotation is #{relative_building_rotation} degrees.")
    end

    #reporting initial condition of model
    building = model.getBuilding
    runner.registerValue("orientation_initial",building.northAxis)
    runner.registerInitialCondition("The building's initial rotation was #{building.northAxis} degrees.")

    #report as not applicable if effective relative rotation is 0
    if relative_building_rotation == 0
      runner.registerAsNotApplicable("The requested rotation was 0 degrees. The model was not altered.")
    else
      #rotate the building
      final_building_angle = building.setNorthAxis(building.northAxis + relative_building_rotation)
      runner.registerInfo("The building has been rotated by #{relative_building_rotation} degrees.")
    end

    #check for site shading
    model_contains_site_shading = false
    shading_surface_groups = model.getShadingSurfaceGroups
    shading_surface_groups.each do |shading_surface_group|
      if shading_surface_group.shadingSurfaceType == "Site"
        model_contains_site_shading = true
      end
    end

    #issue warning if site shading surfaces exist
    if model_contains_site_shading
      runner.registerWarning("The model contains one or more site shading groups. They were not rotated with the building.")
    end

    #reporting final condition of model
    runner.registerValue("orientation_final",building.northAxis)
    runner.registerFinalCondition("The building's final rotation is #{building.northAxis} degrees.")

    return true

  end #end the run method


end #end the measure

#this allows the measure to be used by the application
RotateBuilding.new.registerWithApplication
