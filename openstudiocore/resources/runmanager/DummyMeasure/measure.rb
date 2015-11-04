class DummyMeasure < OpenStudio::Ruleset::ModelUserScript

  # override name to return the name of your script
  def name
    return "Dummy Measure"
  end

  # return a vector of arguments
  def arguments(model)
    result = OpenStudio::Ruleset::OSArgumentVector.new

    return result
  end

  def run(model, runner, user_arguments)
    super(model, runner, user_arguments)

    if not runner.validateUserArguments(arguments(model),user_arguments)
      return false
    end

    File.open("out.osm", "w") do |f|     
      f.write("dummy.osm")   
    end

    return true
  end

end

DummyMeasure.new.registerWithApplication
