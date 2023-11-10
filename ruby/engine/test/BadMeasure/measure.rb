class BadMeasure < OpenStudio::Measure::ModelMeasure

  def name
    return "Bad Measure"
  end

  def arguments(model)
    raise "oops"

    # args = OpenStudio::Measure::OSArgumentVector.new

    # return args
  end

  def run(model, runner, user_arguments)
    super(model, runner, user_arguments)

    return true
  end

end

BadMeasure.new.registerWithApplication
