# see the URL below for information on how to write OpenStudio measures
# http://nrel.github.io/OpenStudio-user-documentation/reference/measure_writing_guide/

# start the measure
class ModelMeasureName < OpenStudio::Measure::ModelMeasure

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
  def arguments(model)
    args = OpenStudio::Measure::OSArgumentVector.new

    # the name of the space to add to the model
    space_name = OpenStudio::Measure::OSArgument.makeStringArgument("space_name", true)
    space_name.setDisplayName("New space name")
    space_name.setDescription("This name will be used as the name of the new space.")
    args << space_name

    return args
  end

  # define what happens when the measure is run
  def run(model, runner, user_arguments)
    super(model, runner, user_arguments)

    # use the built-in error checking
    if !runner.validateUserArguments(arguments(model), user_arguments)
      return false
    end

    # assign the user inputs to variables
    space_name = runner.getStringArgumentValue("space_name", user_arguments)

    # check the space_name for reasonableness
    if space_name.empty?
      runner.registerError("Empty space name was entered.")
      return false
    end

    # report initial condition of model
    runner.registerInitialCondition("The building started with #{model.getSpaces.size} spaces.")

    # add a new space to the model
    new_space = OpenStudio::Model::Space.new(model)
    new_space.setName(space_name)


    # echo the new space's name back to the user
    runner.registerInfo("Space #{new_space.name} was added.")

    # report final condition of model
    runner.registerFinalCondition("The building finished with #{model.getSpaces.size} spaces.")

    return true

  end
  
end

# register the measure to be used by the application
ModelMeasureName.new.registerWithApplication
