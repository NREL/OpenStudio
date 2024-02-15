class ModelMeasureRegistersError < OpenStudio::Measure::ModelMeasure

  def name
    return 'Model Measure Registers Error'
  end

  def description
    return 'Model Measure Registers Error'
  end

  def modeler_description
    return 'The Model Measure, in the run method does runner.registerError'
  end

  def arguments(model)
    args = OpenStudio::Measure::OSArgumentVector.new

    return args
  end

  def run(model, runner, user_arguments)
    super(model, runner, user_arguments)  # Do **NOT** remove this line

    # use the built-in error checking
    if !runner.validateUserArguments(arguments(model), user_arguments)
      return false
    end

    runner.registerValue("ModelMeasureRegistersError", true)
    runner.registerError("runner.registerError called")
    return false
  end
end

# register the measure to be used by the application
ModelMeasureRegistersError.new.registerWithApplication
