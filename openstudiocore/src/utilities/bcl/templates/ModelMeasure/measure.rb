# see the URL below for information on how to write OpenStudio measures
# http://openstudio.nrel.gov/openstudio-measure-writing-guide

# start the measure
class ModelMeasure < OpenStudio::Ruleset::ModelUserScript
  
  # human readable name
  def name
    return "ModelMeasure"
  end
  
  # human readable description
  def description
    return "A human readable description goes here"
  end
  
  # human readable description
  def modeler_description
    return "A human readable description of the technical implementation details goes here"
  end
  
  # define the arguments that the user will input
  def arguments(model)
    args = OpenStudio::Ruleset::OSArgumentVector.new

    # the name of the space to add to the model
    space_name = OpenStudio::Ruleset::OSArgument::makeStringArgument("space_name", true)
    space_name.setDisplayName("New space name")
    space_name.setDescription("This name will be used as the name of the new space.")
    args << zone_name

    return args
  end

  # define what happens when the measure is run
  def run(model, runner, user_arguments)
    super(model, runner, user_arguments)

    # use the built-in error checking
    if not runner.validateUserArguments(arguments(model), user_arguments)
      return false
    end

    # assign the user inputs to variables
    space_name = runner.getStringArgumentValue("space_name", user_arguments)
    
    # check the space_name for reasonableness
    if space_name.empty?
      runner.registerError("Empty space name was Entered.")
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
