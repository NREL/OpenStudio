# start the measure
class OutdatedReportingMeasure < OpenStudio::Ruleset::ReportingUserScript
  # human readable name
  def name
    # Measure name should be the title case of the class name.
    return 'Outdated Reporting Measure'
  end

  # human readable description
  def description
    return 'ReportingMeasure is an old ReportingUserScript'
  end

  # human readable description of modeling approach
  def modeler_description
    return "The ReportingMeasure Measure derives the deprecated ReportingUserScript class, and arguments doesn't take model"
  end

  # define the arguments that the user will input
  def arguments
    args = OpenStudio::Measure::OSArgumentVector.new

    # bool argument to report report_drybulb_temp
    report_drybulb_temp = OpenStudio::Measure::OSArgument.makeBoolArgument('report_drybulb_temp', true)
    report_drybulb_temp.setDisplayName('Add output variables for Drybulb Temperature')
    report_drybulb_temp.setDescription('Will add drybulb temp and report min/mix value in html.')
    report_drybulb_temp.setValue(true)
    args << report_drybulb_temp

    return args
  end

  # define the outputs that the measure will create
  def outputs
    outs = OpenStudio::Measure::OSOutputVector.new

    # this measure does not produce machine readable outputs with registerValue, return an empty list

    return outs
  end

  # return a vector of IdfObject's to request EnergyPlus objects needed by the run method
  # Warning: Do not change the name of this method to be snake_case. The method must be lowerCamelCase.
  def energyPlusOutputRequests(runner, user_arguments)
    super(runner, user_arguments)  # Do **NOT** remove this line

    result = OpenStudio::IdfObjectVector.new

    # use the built-in error checking
    unless runner.validateUserArguments(arguments, user_arguments)
      return result
    end

    if runner.getBoolArgumentValue('report_drybulb_temp', user_arguments)
      request = OpenStudio::IdfObject.load('Output:Variable,,Site Outdoor Air Drybulb Temperature,Hourly;').get
      result << request
    end

    return result
  end

  # define what happens when the measure is run
  def run(runner, user_arguments)
    super(runner, user_arguments)

    # get the last model and sql file
    model = runner.lastOpenStudioModel
    if model.empty?
      runner.registerError('Cannot find last model.')
      return false
    end
    model = model.get

    # use the built-in error checking (need model)
    if !runner.validateUserArguments(arguments, user_arguments)
      return false
    end

    # get measure arguments
    report_drybulb_temp = runner.getBoolArgumentValue('report_drybulb_temp', user_arguments)

    runner.registerValue("report_drybulb_temp", report_drybulb_temp)

    return true
  end
end

# register the measure to be used by the application
OutdatedReportingMeasure.new.registerWithApplication
