require 'openstudio'
require 'openstudio/ruleset/ShowRunnerOutput'
require 'minitest/autorun'

require_relative '../measure.rb'

require 'fileutils'

class ReportingMeasure_Test < MiniTest::Unit::TestCase

  # class level variable
  @@co = OpenStudio::Runmanager::ConfigOptions.new(true)

  def model_in_path
    return "#{File.dirname(__FILE__)}/example_model.osm"
  end
  
  def epw_path
    # make sure we have a weather data location
    assert(!@@co.getDefaultEPWLocation.to_s.empty?)
    epw = @@co.getDefaultEPWLocation / OpenStudio::Path.new("USA_CO_Golden-NREL.724666_TMY3.epw")
    assert(File.exist?(epw.to_s))
    
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
    return "#{run_dir(test_name)}/ModelToIdf/EnergyPlusPreProcess-0/EnergyPlus-0/eplusout.sql"
  end
  
  def report_path(test_name)
    return "#{run_dir(test_name)}/report.html"
  end

  # create test files if they do not exist when the test first runs 
  def setup_test(test_name, idf_output_requests)
  
    @@co.findTools(false, true, false, true)
    
    if !File.exist?(run_dir(test_name))
      FileUtils.mkdir_p(run_dir(test_name))
    end
    assert(File.exist?(run_dir(test_name)))
    
    if File.exist?(report_path(test_name))
      FileUtils.rm(report_path(test_name))
    end

    assert(File.exist?(model_in_path))
    
    if File.exist?(model_out_path(test_name))
      FileUtils.rm(model_out_path(test_name))
    end

    # convert output requests to OSM for testing, OS App and PAT will add these to the E+ Idf 
    workspace = OpenStudio::Workspace.new("Draft".to_StrictnessLevel, "EnergyPlus".to_IddFileType)
    workspace.addObjects(idf_output_requests)
    rt = OpenStudio::EnergyPlus::ReverseTranslator.new
    request_model = rt.translateWorkspace(workspace)

    translator = OpenStudio::OSVersion::VersionTranslator.new
    model = translator.loadModel(model_in_path)
    assert((not model.empty?))
    model = model.get
    model.addObjects(request_model.objects)
    model.save(model_out_path(test_name), true)

    if !File.exist?(sql_path(test_name))
      puts "Running EnergyPlus"

      wf = OpenStudio::Runmanager::Workflow.new("modeltoidf->energypluspreprocess->energyplus")
      wf.add(@@co.getTools())
      job = wf.create(OpenStudio::Path.new(run_dir(test_name)), OpenStudio::Path.new(model_out_path(test_name)), OpenStudio::Path.new(epw_path))

      rm = OpenStudio::Runmanager::RunManager.new
      rm.enqueue(job, true)
      rm.waitForFinished
    end
  end

  def test_number_of_arguments_and_argument_names
    # create an instance of the measure
    measure = ReportingMeasure.new

    # get arguments and test that they are what we are expecting
    arguments = measure.arguments()
    assert_equal(0, arguments.size)
  end

  def test_good_argument_values
  
    test_name = "test_good_argument_values"

    # create an instance of the measure
    measure = ReportingMeasure.new

    # create an instance of a runner
    runner = OpenStudio::Ruleset::OSRunner.new
    
    # get arguments
    arguments = measure.arguments()
    argument_map = OpenStudio::Ruleset.convertOSArgumentVectorToMap(arguments)
    
    # get the energyplus output requests, this will be done automatically by OS App and PAT
    idf_output_requests = measure.energyPlusOutputRequests(runner, argument_map)
    assert_equal(1, idf_output_requests.size)

    # mimic the process of running this measure in OS App or PAT
    setup_test(test_name, idf_output_requests)
    
    assert(File.exist?(model_out_path(test_name)))
    assert(File.exist?(sql_path(test_name)))
    assert(File.exist?(epw_path))

    # set up runner, this will happen automatically when measure is run in PAT or OpenStudio
    runner.setLastOpenStudioModelPath(OpenStudio::Path.new(model_out_path(test_name)))
    runner.setLastEpwFilePath(epw_path)
    runner.setLastEnergyPlusSqlFilePath(OpenStudio::Path.new(sql_path(test_name)))

    # delete the output if it exists
    if File.exist?(report_path(test_name))
      FileUtils.rm(report_path(test_name))
    end
    assert(!File.exist?(report_path(test_name)))
    
    # temporarily change directory to the run directory and run the measure
    start_dir = Dir.pwd
    begin
      Dir.chdir(run_dir(test_name))

      # run the measure
      measure.run(runner, argument_map)
      result = runner.result
      assert_equal("Success", result.value.valueName)
    ensure
      Dir.chdir(start_dir)
    end
    
    # make sure the report file exists
    assert(File.exist?(report_path(test_name)))
  end

end
