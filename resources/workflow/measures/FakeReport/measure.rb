require 'openstudio'

class FakeReport < OpenStudio::Measure::ReportingMeasure

  def name
    return "Fake Report"
  end

  #define the arguments that the user will input
  def arguments(model = nil)
    args = OpenStudio::Measure::OSArgumentVector.new

    return args
  end #end the arguments method

  def outputs
    result = OpenStudio::Measure::OSOutputVector.new
    result << OpenStudio::Measure::OSOutput.makeDoubleOutput('net_site_energy', false)
    return result
  end

  #define what happens when the measure is run
  def run(runner, user_arguments)
    super(runner, user_arguments)

    #use the built-in error checking
    if not runner.validateUserArguments(arguments(), user_arguments)
      return false
    end

    runner.registerInfo("runner.registerInfo called")
    runner.registerWarning("runner.registerWarning called")

    # Register some constant values, so we can also do it with postprocessing only
    runner.registerValue("FakeReport", true)
    runner.registerValue("net_site_energy", "Net Site Energy", 167.1, "GJ")
    runner.registerValue("something!with.invalid_chars_", "Test Sanitizing", 1, "")
    runner.registerFinalCondition("Goodbye.")
    return true

  end

end

FakeReport.new.registerWithApplication
