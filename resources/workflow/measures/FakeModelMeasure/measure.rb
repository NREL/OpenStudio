class FakeModelMeasure < OpenStudio::Measure::ModelMeasure
  # human readable name
  def name
    # Measure name should be the title case of the class name.
    return 'A dumb ModelMeasure'
  end

  # human readable description
  def description
    return 'Does nothing'
  end

  # human readable description of modeling approach
  def modeler_description
    return 'Just for testing'
  end

  # define the arguments that the user will input
  def arguments(model)
    args = OpenStudio::Measure::OSArgumentVector.new

    return args
  end

  # define what happens when the measure is run
  def run(model, runner, user_arguments)
    super(model, runner, user_arguments)  # Do **NOT** remove this line

    # use the built-in error checking
    if !runner.validateUserArguments(arguments(model), user_arguments)
      return false
    end

    # report final condition of model
    runner.registerFinalCondition("The FakeModelMeasure run.")

    return true
  end
end

# register the measure to be used by the application
FakeModelMeasure.new.registerWithApplication
