require 'openstudio'
require 'tmpdir'
require 'erb'

# start the measure
class PythonPluginRuby < OpenStudio::Measure::ModelMeasure
  # human readable name
  def name
    return 'Python Plugin Ruby.'
  end

  # human readable description
  def description
    return 'TODO'
  end

  # human readable description of modeling approach
  def modeler_description
    return 'TODO'
  end

  def arguments(model)
    args = OpenStudio::Measure::OSArgumentVector.new

    return args
  end

  # define what happens when the measure is run
  def run(model, runner, user_arguments)
    super(model, runner, user_arguments)

    # use the built-in error checking
    if !runner.validateUserArguments(arguments(model), user_arguments)
      return false
    end

    # Add a PythonPlugin:Variable (all OS SDK PythonPluginVariable objects are
    # translated to a single E+ PythonPlugin:Variables (extensible object))
    py_var = OpenStudio::Model::PythonPluginVariable.new(model)
    py_var.setName('AverageBuildingTemp')

    # Add a PythonPlugin:OutputVariable for that variable
    py_out_var = OpenStudio::Model::PythonPluginOutputVariable.new(py_var)
    py_out_var.setName('Averaged Building Temperature')
    py_out_var.setTypeofDatainVariable('Averaged')
    py_out_var.setUpdateFrequency('ZoneTimestep')
    py_out_var.setUnits('C')

    # Add a regular Output:Variable that references it
    out_var = OpenStudio::Model::OutputVariable.new('PythonPlugin:OutputVariable', model)
    out_var.setKeyValue(py_out_var.nameString)
    out_var.setReportingFrequency('Timestep')

    # Add output variables for Zone Mean Air Temperature, so we can compare
    outputVariable = OpenStudio::Model::OutputVariable.new('Zone Mean Air Temperature', model)
    outputVariable.setReportingFrequency('Timestep')

    # Trend Variable: while this is a fully functioning object, you're probably
    # best just using a storage variable on the Python side (eg: a list)
    py_trend_var = OpenStudio::Model::PythonPluginTrendVariable.new(py_var)
    py_trend_var.setName('Running Averaged Building Temperature')
    n_timesteps = 24 * model.getTimestep.numberOfTimestepsPerHour
    py_trend_var.setNumberofTimestepstobeLogged(n_timesteps)

    py_var2 = OpenStudio::Model::PythonPluginVariable.new(model)
    py_var2.setName('RunningAverageBuildingTemp')

    py_out_trend_var = OpenStudio::Model::PythonPluginOutputVariable.new(py_var2)
    py_out_trend_var.setName('Running Averaged Building Temperature')
    py_out_trend_var.setTypeofDatainVariable('Averaged')
    py_out_trend_var.setUpdateFrequency('ZoneTimestep')
    py_out_trend_var.setUnits('C')

    out_trend_var = OpenStudio::Model::OutputVariable.new('PythonPlugin:OutputVariable', model)
    out_trend_var.setReportingFrequency('Timestep')

    pluginClassName = 'AverageZoneTemps'

    # get the python plugin program (erb template)
    pluginTemplatePath = File.join(File.dirname(__FILE__), '../../files/python_plugin_program_ruby.py')
    in_py = ''
    File.open(pluginTemplatePath, 'r') do |file|
      in_py = file.read
    end

    dataPath = File.join(File.dirname(__FILE__), '../../files/python_plugin_program.csv')
    OpenStudio::Model::ExternalFile.getExternalFile(model, dataPath)

    # configure plugin template with variable values
    renderer = ERB.new(in_py)
    out_py = renderer.result(binding)

    # Write it to a temporary directory so we don't pollute the current directory
    # ExternalFile will copy it
    pluginPath = File.join(Dir.tmpdir, 'python_plugin_program2.py')
    File.open(pluginPath, 'w') do |file|
      file << out_py
    end

    # create the external file object
    external_file = OpenStudio::Model::ExternalFile.getExternalFile(model, pluginPath)
    external_file = external_file.get

    # create the python plugin instance object
    python_plugin_instance = OpenStudio::Model::PythonPluginInstance.new(external_file, pluginClassName)
    python_plugin_instance.setRunDuringWarmupDays(false)

    return true
  end
end

# register the measure to be used by the application
PythonPluginRuby.new.registerWithApplication
