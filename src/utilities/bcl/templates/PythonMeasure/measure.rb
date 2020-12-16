# insert your copyright here

# see the URL below for information on how to write OpenStudio measures
# http://nrel.github.io/OpenStudio-user-documentation/reference/measure_writing_guide/
# start the measure

  require 'pycall/import'
  
class PythonMeasureName < OpenStudio::Measure::PythonMeasure

  include PyCall::Import
    
  # human readable name
  def name
    # Measure name should be the title case of the class name.
    return 'NAME_TEXT'
  end

  # human readable description
  def description
    return 'DESCRIPTION_TEXT'
  end

  # human readable description of modeling approach
  def modeler_description
    return 'MODELER_DESCRIPTION_TEXT'
  end

  # define the arguments that the user will input
  def arguments()
    args = OpenStudio::Measure::OSArgumentVector.new

    # the name of the space to add to the model
    example_arg = OpenStudio::Measure::OSArgument.makeStringArgument('example_arg', true)
    example_arg.setDisplayName('example argument')
    example_arg.setDescription('This is a placeholder for an argument')
    args << example_arg

    return args
  end

  # define what happens when the measure is run
  def run(model, runner, user_arguments)
    super(model, runner, user_arguments)

    # use the built-in error checking
    if !runner.validateUserArguments(arguments(), user_arguments)
      return false
    end

    # assign the user inputs to variables
    example_arg = runner.getStringArgumentValue('example_arg', user_arguments)

    # check the example_arg for reasonableness
    if example_arg.empty?
      runner.registerError('Empty example arg was entered.')
      return false
    end

    pyimport :math
    result = math.sin(math.pi / 4) - Math.sin(Math::PI / 4)   # => 0.0
    runner.registerInfo("PYCALL test result = #{result}")
    
    # report initial condition of model
    #runner.registerInitialCondition("")

    # Info statements
    #runner.registerInfo("")

    # report final condition of model
    #runner.registerFinalCondition("")

    return true
  end
end

# register the measure to be used by the application
PythonMeasureName.new.registerWithApplication
