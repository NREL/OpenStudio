require 'openstudio'

require 'openstudio/ruleset/ShowRunnerOutput'

require "#{File.dirname(__FILE__)}/../measure.rb"

require 'fileutils'

require 'test/unit'

class CalibrationReports_Test < Test::Unit::TestCase
    
  # paths to expected test files, includes osm and eplusout.sql
  def modelPath
    return "#{File.dirname(__FILE__)}/House1203_a_ver112.osm"
  end
  
  def runDir
    return "#{File.dirname(__FILE__)}/House1203_a_ver112/"
  end
  
  def sqlPath
    return "#{File.dirname(__FILE__)}/House1203_a_ver112/ModelToIdf/EnergyPlus-0/eplusout.sql"
  end
  
  def reportPath
    return "./report.html"
  end
  
  # create test files if they do not exist
  def setup
    if File.exist?(modelPath())
      #FileUtils.rm(modelPath())
    end
    if File.exist?(runDir())
      #FileUtils.rm_rf(runDir())
    end
    if File.exist?(reportPath())
      #FileUtils.rm(reportPath())
    end
    
    # check that the measure is not out of date
    measure_path = OpenStudio::system_complete(OpenStudio::Path.new("#{File.dirname(__FILE__)}/../"))
    measure = OpenStudio::BCLMeasure.new(measure_path)
    assert_equal("Calibration Reports", measure.name())
    if (measure.checkForUpdates())
      puts "Measure is out of date, automatically saving"
      measure.save()
    end
    assert((not measure.checkForUpdates()))
    
    if not File.exist?(modelPath())
      puts "Creating example model"
      model = OpenStudio::Model::exampleModel()
      model.getRunPeriod.remove
      model.getSimulationControl.setRunSimulationforSizingPeriods(true)
      model.getSimulationControl.setRunSimulationforWeatherFileRunPeriods(false)
      model.save(OpenStudio::Path.new(modelPath()), true)
    end
    
    if not File.exist?(sqlPath())
      puts "Running EnergyPlus"
      
      co = OpenStudio::Runmanager::ConfigOptions.new(true)
      co.findTools(false, true, false, true)
      
      wf = OpenStudio::Runmanager::Workflow.new("modeltoidf->energyplus")
      wf.add(co.getTools())
      job = wf.create(OpenStudio::Path.new(runDir()), OpenStudio::Path.new(modelPath()))

      rm = OpenStudio::Runmanager::RunManager.new
      rm.enqueue(job, true)
      rm.waitForFinished
    end
  end

  # delete test files, comment this out if you do not want to run EnergyPlus each time you run tests
  def teardown
    if File.exist?(modelPath())
      #FileUtils.rm(modelPath())
    end
    if File.exist?(runDir())
      #FileUtils.rm_rf(runDir())
    end
    if File.exist?(reportPath())
      #FileUtils.rm(reportPath())
    end
  end
  
  # the actual test
  def test_CalibrationReports
     
    assert(File.exist?(modelPath()))
    assert(File.exist?(sqlPath()))
     
    # create an instance of the measure
    measure = CalibrationReports.new
    
    # create an instance of a runner
    runner = OpenStudio::Ruleset::OSRunner.new
    
    # get arguments and test that they are what we are expecting
    arguments = measure.arguments()
    assert_equal(0, arguments.size)
    
    # set up runner, this will happen automatically when measure is run in PAT
    runner.setLastOpenStudioModelPath(OpenStudio::Path.new(modelPath))    
    runner.setLastEnergyPlusSqlFilePath(OpenStudio::Path.new(sqlPath))    
       
    # set argument values to good values and run the measure
    argument_map = OpenStudio::Ruleset::OSArgumentMap.new
    measure.run(runner, argument_map)
    result = runner.result
    show_output(result)
    assert(result.value.valueName == "Success")
    assert(result.warnings.size == 0)
    #assert(result.info.size == 1)
    
    assert(File.exist?(reportPath()))
    
  end  

end
