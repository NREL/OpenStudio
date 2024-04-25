# insert your copyright here

require 'openstudio'
require 'openstudio/measure/ShowRunnerOutput'
require 'minitest/autorun'
require 'fileutils'

require_relative '../measure'

class ReportingMeasureNameTest < Minitest::Test
  def model_in_path_default
    return "#{File.dirname(__FILE__)}/example_model.osm"
  end

  def epw_path_default
    # make sure we have a weather data location
    epw = File.expand_path("#{File.dirname(__FILE__)}/USA_CO_Golden-NREL.724666_TMY3.epw")
    assert_path_exists(epw.to_s)
    return epw.to_s
  end

  def run_dir(test_name)
    # always generate test output in specially named 'output' directory so result files are not made part of the measure
    return "#{File.dirname(__FILE__)}/output/#{test_name}"
  end

  def model_out_path(test_name)
    return "#{run_dir(test_name)}/example_model.osm"
  end

  def sql_path(test_name)
    return "#{run_dir(test_name)}/run/eplusout.sql"
  end

  def report_path(test_name)
    return "#{run_dir(test_name)}/report.html"
  end

  # create test files if they do not exist when the test first runs
  def setup_test(test_name, idf_output_requests, model_in_path = model_in_path_default, epw_path = epw_path_default)
    if !File.exist?(run_dir(test_name))
      FileUtils.mkdir_p(run_dir(test_name))
    end
    assert_path_exists(run_dir(test_name))

    if File.exist?(report_path(test_name))
      FileUtils.rm(report_path(test_name))
    end

    assert_path_exists(model_in_path)

    if File.exist?(model_out_path(test_name))
      FileUtils.rm(model_out_path(test_name))
    end

    # convert output requests to OSM for testing, OS App and PAT will add these to the E+ Idf
    workspace = OpenStudio::Workspace.new('Draft'.to_StrictnessLevel, 'EnergyPlus'.to_IddFileType)
    workspace.addObjects(idf_output_requests)
    rt = OpenStudio::EnergyPlus::ReverseTranslator.new
    request_model = rt.translateWorkspace(workspace)

    translator = OpenStudio::OSVersion::VersionTranslator.new
    model = translator.loadModel(model_in_path)
    refute_empty(model)
    model = model.get
    model.addObjects(request_model.objects)
    model.save(model_out_path(test_name), true)

    if ENV['OPENSTUDIO_TEST_NO_CACHE_SQLFILE']
      if File.exist?(sql_path(test_name))
        FileUtils.rm_f(sql_path(test_name))
      end
    end

    osw_path = File.join(run_dir(test_name), 'in.osw')
    osw_path = File.absolute_path(osw_path)

    workflow = OpenStudio::WorkflowJSON.new
    workflow.setSeedFile(File.absolute_path(model_out_path(test_name)))
    workflow.setWeatherFile(File.absolute_path(epw_path))
    workflow.saveAs(osw_path)

    cli_path = OpenStudio.getOpenStudioCLI
    cmd = "\"#{cli_path}\" run -w \"#{osw_path}\""
    puts cmd
    system(cmd)

  end

  def test_number_of_arguments_and_argument_names
    # create an instance of the measure
    measure = ReportingMeasureName.new

    # Make an empty model
    model = OpenStudio::Model::Model.new

    # get arguments and test that they are what we are expecting
    arguments = measure.arguments(model)
    assert_equal(1, arguments.size)
  end

  def test_with_drybulb_temp
    test_name = 'test_with_drybulb_temp'

    # create an instance of the measure
    measure = ReportingMeasureName.new

    # create runner with empty OSW
    osw = OpenStudio::WorkflowJSON.new
    runner = OpenStudio::Measure::OSRunner.new(osw)

    # Make an empty model
    model = OpenStudio::Model::Model.new

    # get arguments
    arguments = measure.arguments(model)
    argument_map = OpenStudio::Measure.convertOSArgumentVectorToMap(arguments)

    # temp set path so idf_output_requests work
    runner.setLastOpenStudioModelPath(model_in_path_default)

    # get the energyplus output requests, this will be done automatically by OS App and PAT
    idf_output_requests = measure.energyPlusOutputRequests(runner, argument_map)
    assert_equal(1, idf_output_requests.size)

    # mimic the process of running this measure in OS App or PAT. Optionally set custom model_in_path and custom epw_path.
    epw_path = epw_path_default
    setup_test(test_name, idf_output_requests)

    assert_path_exists(model_out_path(test_name))
    assert_path_exists(sql_path(test_name))
    assert_path_exists(epw_path)

    # set up runner, this will happen automatically when measure is run in PAT or OpenStudio
    runner.setLastOpenStudioModelPath(model_out_path(test_name))
    runner.setLastEpwFilePath(epw_path)
    runner.setLastEnergyPlusSqlFilePath(sql_path(test_name))

    # delete the output if it exists
    if File.exist?(report_path(test_name))
      FileUtils.rm(report_path(test_name))
    end
    refute_path_exists(report_path(test_name))

    # temporarily change directory to the run directory and run the measure
    Dir.chdir(run_dir(test_name)) do
      measure.run(runner, argument_map)
    end

    result = runner.result
    show_output(result)
    assert_equal('Success', result.value.valueName)
    assert_empty(result.stepWarnings)

    sqlFile = runner.lastEnergyPlusSqlFile.get
    if !sqlFile.connectionOpen
      sqlFile.reopen
    end
    hours = sqlFile.hoursSimulated
    refute_empty(hours)
    assert_equal(8760.0, hours.get)

    # make sure the report file exists
    assert_path_exists(report_path(test_name))
  end

  def test_without_drybulb_temp
    test_name = 'test_without_drybulb_temp'

    # create an instance of the measure
    measure = ReportingMeasureName.new

    # create runner with empty OSW
    osw = OpenStudio::WorkflowJSON.new
    runner = OpenStudio::Measure::OSRunner.new(osw)

    # Make an empty model
    model = OpenStudio::Model::Model.new

    # get arguments
    arguments = measure.arguments(model)
    argument_map = OpenStudio::Measure.convertOSArgumentVectorToMap(arguments)

    # set non-default measure argument
    report_drybulb_temp = arguments[0].clone
    assert(report_drybulb_temp.setValue(false))
    argument_map['report_drybulb_temp'] = report_drybulb_temp

    # temp set path so idf_output_requests work
    runner.setLastOpenStudioModelPath(model_in_path_default)

    # get the energyplus output requests, this will be done automatically by OS App and PAT
    idf_output_requests = measure.energyPlusOutputRequests(runner, argument_map)
    assert_equal(0, idf_output_requests.size)

    # mimic the process of running this measure in OS App or PAT.
    # Optionally set custom model_in_path and custom epw_path.
    epw_path = epw_path_default
    setup_test(test_name, idf_output_requests)

    assert_path_exists(model_out_path(test_name))
    assert_path_exists(sql_path(test_name))
    assert_path_exists(epw_path)

    # set up runner, this will happen automatically when measure is run in PAT or OpenStudio
    runner.setLastOpenStudioModelPath(model_out_path(test_name))
    runner.setLastEpwFilePath(epw_path)
    runner.setLastEnergyPlusSqlFilePath(sql_path(test_name))

    # delete the output if it exists
    if File.exist?(report_path(test_name))
      FileUtils.rm(report_path(test_name))
    end
    refute_path_exists(report_path(test_name))

    # temporarily change directory to the run directory and run the measure
    Dir.chdir(run_dir(test_name)) do
      measure.run(runner, argument_map)
    end

    result = runner.result
    show_output(result)
    assert_equal('Success', result.value.valueName)
    assert_empty(result.stepWarnings)

    # sqlFile = OpenStudio::SqlFile.new(OpenStudio::Path.new(sql_path(test_name)))
    sqlFile = runner.lastEnergyPlusSqlFile.get
    if !sqlFile.connectionOpen
      sqlFile.reopen
    end
    hours = sqlFile.hoursSimulated
    refute_empty(hours)
    assert_equal(8760.0, hours.get)

    # make sure the report file exists
    assert_path_exists(report_path(test_name))
  end
end

