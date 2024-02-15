require 'openstudio'

class ReportingMeasureRegistersError < OpenStudio::Measure::ReportingMeasure

  def name
    return "Reporting Measure Registers Error"
  end

  def modeler_description
    return "The Reporting Measure, in the run method does runner.registerError"
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

    runner.registerValue("ReportingMeasureRegistersError", true)
    runner.registerError("runner.registerError called")
    return false
  end

end

ReportingMeasureRegistersError.new.registerWithApplication
