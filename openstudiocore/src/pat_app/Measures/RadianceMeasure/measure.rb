# see the URL below for information on how to write OpenStudio measures
# http://nrel.github.io/OpenStudio-user-documentation/measures/measure_writing_guide/

# design of this measure is at https://docs.google.com/document/d/16_TLRuhc4VFs2o0gRAp81hRObet7-s6fUEWo3HO7LpE/edit#

require_relative 'resources/RadiancePreprocess'

# start the measure
class RadianceMeasure < OpenStudio::Ruleset::ModelUserScript

  # human readable name
  def name
    return "Radiance Measure"
  end

  # human readable description
  def description
    return "Does stuff"
  end

  # human readable description of modeling approach
  def modeler_description
    return "Things"
  end

  # define the arguments that the user will input
  def arguments(model)
    args = OpenStudio::Ruleset::OSArgumentVector.new

    # the name of the space to add to the model
    space_name = OpenStudio::Ruleset::OSArgument.makeStringArgument("space_name", true)
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

    
    sql_file = OpenStudio::Radiance.getSqlFile(model)

    # report final condition of model
    runner.registerFinalCondition("The building finished with #{model.getSpaces.size} spaces.")

    return true

  end
  
end

# register the measure to be used by the application
RadianceMeasure.new.registerWithApplication
