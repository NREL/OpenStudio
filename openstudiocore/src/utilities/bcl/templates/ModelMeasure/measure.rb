# see the URL below for information on how to write OpenStudio measures
# http://openstudio.nrel.gov/openstudio-measure-writing-guide

# start the measure
class ModelMeasure < OpenStudio::Ruleset::ModelUserScript
  
  # machine readable name
  def name
    return "ModelMeasure"
  end
  # human readable name
  def display_name
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

    # make an argument for your name
    user_name = OpenStudio::Ruleset::OSArgument::makeStringArgument("user_name",true)
    user_name.setDisplayName("What is your name")
    args << user_name

    # make an argument to add new space true/false
    add_space = OpenStudio::Ruleset::OSArgument::makeBoolArgument("add_space",true)
    add_space.setDisplayName("Add a space to your model")
    add_space.setDefaultValue(true)
    args << add_space

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
    user_name = runner.getStringArgumentValue("user_name", user_arguments)
    add_space = runner.getBoolArgumentValue("add_space", user_arguments)

    # check the user_name for reasonableness
    if user_name == ""
      runner.registerError("No Name was Entered.")
      return false
    end

    # ruby test to see if first charter of string is uppercase letter
    if not user_name[0..0] =~ /[A-Z]/
      runner.registerWarning("Your entered name was not capitalized.")
    end

    # returning the name in an a sentence
    runner.registerInfo("Hello #{user_name}, thanks for entering your name.")

    # reporting initial condition of model
    runner.registerInitialCondition("The building started with #{model.getSpaces.size} spaces.")

    # adding a new space to the model
    if add_space == true
      new_space = OpenStudio::Model::Space.new(model)
      new_space.setName("Hello_World test space")
      runner.registerInfo("Space #{new_space.name} was added.")
    else
      runner.registerAsNotApplicable("You have chosen not to add a space. Your model will not be altered.")
      return true
    end

    # reporting final condition of model
    runner.registerFinalCondition("The building finished with #{model.getSpaces.size} spaces.")
    return true

  end
end

#this allows the measure to be use by the application
ModelMeasure.new.registerWithApplication