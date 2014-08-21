require 'openstudio'
require 'openstudio/ruleset/ShowRunnerOutput'
require 'minitest/autorun'

require_relative '../measure.rb'

require 'fileutils'

class ReportingMeasure_Test < MiniTest::Unit::TestCase
  
  # class level variable
  @@co = OpenStudio::Runmanager::ConfigOptions.new(true)
  
  # paths to expected test files, includes osm and eplusout.sql
  def modelPath
    return "#{File.dirname(__FILE__)}/ExampleModel.osm"
  end
  
  def runDir
    # always generate test output in specially named 'output' directory so result files are not made part of the measure
    return "#{File.dirname(__FILE__)}/output/"
  end
  
  def sqlPath
    return "#{File.dirname(__FILE__)}/output/ModelToIdf/EnergyPlusPreProcess-0/EnergyPlus-0/eplusout.sql"
  end
  
  def reportPath
    return "#{File.dirname(__FILE__)}/report.html"
  end
  
  # create test files if they do not exist when the test first runs 
  def setup

    if File.exist?(reportPath())
      FileUtils.rm(reportPath())
    end
    
    assert(File.exist?(modelPath()))
    
    if not File.exist?(runDir())
      Dir.mkdir "#{runDir()}"
    end
    assert(File.exist?(runDir()))
    
    @@co.findTools(false, true, false, true)
    
    if not File.exist?(sqlPath())
      puts "Running EnergyPlus"
      
      wf = OpenStudio::Runmanager::Workflow.new("modeltoidf->energypluspreprocess->energyplus")
      wf.add(@@co.getTools())
      job = wf.create(OpenStudio::Path.new(runDir()), OpenStudio::Path.new(modelPath()))
      
      rm = OpenStudio::Runmanager::RunManager.new
      rm.enqueue(job, true)
      rm.waitForFinished
    end
  end

  # delete output files when tests end
  def teardown
  
    # comment this out if you don't want to rerun EnergyPlus each time
    if File.exist?(sqlPath())
      #FileUtils.rm(sqlPath())
    end
    
    # comment this out if you want to see the resulting report
    if File.exist?(reportPath())
      #FileUtils.rm(reportPath())
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
    
    assert(File.exist?(modelPath()))
    assert(File.exist?(sqlPath()))
    
    # create an instance of the measure
    measure = ReportingMeasure.new
    
    # create an instance of a runner
    runner = OpenStudio::Ruleset::OSRunner.new
    
    # set up runner, this will happen automatically when measure is run in PAT or OpenStudio
    runner.setLastOpenStudioModelPath(OpenStudio::Path.new(modelPath))
    runner.setLastEpwFilePath(epw)
    runner.setLastEnergyPlusSqlFilePath(OpenStudio::Path.new(sqlPath))
    
    # make sure we have a weather data location
    assert(!@@co.getDefaultEPWLocation.to_s.empty?)
    epw = @@co.getDefaultEPWLocation / OpenStudio::Path.new("USA_CO_Golden-NREL.724666_TMY3.epw")
    assert(File.exist?(epw.to_s))
    
    # get arguments
    arguments = measure.arguments()
    argument_map = OpenStudio::Ruleset::convertOSArgumentVectorToMap(arguments)
    
    # delete the output if it exists
    if File.exist?(reportPath())
      FileUtils.rm(reportPath())
    end
    assert(not File.exist?(reportPath()))
   
    # run the measure
    measure.run(model, runner, argument_map)
    result = runner.result
    assert_equal("Success", result.value.valueName)
    
    # make sure the report file exists
    assert(File.exist?(reportPath()))
    
  end  

end
