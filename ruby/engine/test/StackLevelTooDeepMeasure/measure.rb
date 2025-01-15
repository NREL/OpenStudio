########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

class StackLevelTooDeepMeasure < OpenStudio::Measure::ModelMeasure

  def name
    return "Stack Level Too Deep Measure"
  end

  def modeler_description
     return "The arguments method calls an infinitely recursive function"
  end

  def s(x)
    return x * s(x-1)
  end

  def arguments(model)
    s(10)
    args = OpenStudio::Measure::OSArgumentVector.new

    return args
  end

  def run(model, runner, user_arguments)
    super(model, runner, user_arguments)

    return true
  end

end

StackLevelTooDeepMeasure.new.registerWithApplication
