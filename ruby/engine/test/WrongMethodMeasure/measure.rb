########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

class WrongMethodMeasure < OpenStudio::Measure::ModelMeasure

  def name
    return "Wrong Method Measure"
  end

  def modeler_description
     return "The arguments method calls a non existing method on the model passed as argument"
  end

  def arguments(model)
    model.nonExistingMethod()
    args = OpenStudio::Measure::OSArgumentVector.new

    return args
  end

  def run(model, runner, user_arguments)
    super(model, runner, user_arguments)

    return true
  end

end

WrongMethodMeasure.new.registerWithApplication
