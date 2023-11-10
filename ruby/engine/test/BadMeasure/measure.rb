class BadMeasure < OpenStudio::Measure::ModelMeasure

  def name
    return "Bad Measure"
  end

  def modeler_description
     return "The arguments method calls another_method which does a raise ValueError"
  end

  def another_method
    raise "oops"
  end

  def arguments(model)
    another_method
    args = OpenStudio::Measure::OSArgumentVector.new

    return args
  end

  def run(model, runner, user_arguments)
    super(model, runner, user_arguments)

    return true
  end

end

BadMeasure.new.registerWithApplication
